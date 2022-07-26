#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "battle_string_ids.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "util.h"
#include "item.h"
#include "random.h"
#include "pokemon.h"
#include "battle_ai_script_commands.h"
#include "menu.h"
#include "malloc.h"
#include "dynamax.h"
#include "string_util.h"
#include "window.h"
#include "sound.h"
#include "constants/abilities.h"
#include "constants/battle_ai.h"
#include "constants/battle_move_effects.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"
#include "constants/pokemon.h"

struct StatFractions
{
    u8 dividend;
    u8 divisor;
};

static const struct StatFractions sAccuracyStageRatios[] =
{
    {  33, 100 }, // -6
    {  36, 100 }, // -5
    {  43, 100 }, // -4
    {  50, 100 }, // -3
    {  60, 100 }, // -2
    {  75, 100 }, // -1
    {   1,   1 }, //  0
    { 133, 100 }, // +1
    { 166, 100 }, // +2
    {   2,   1 }, // +3
    { 233, 100 }, // +4
    { 133,  50 }, // +5
    {   3,   1 }, // +6
};

static const u8 gText_RaidShieldProtected[] = _("The mysterious barrier is\nprotecting {B_DEF_NAME_WITH_PREFIX}\lfrom the impacts of the move!");
static const u8 CraftyShieldProtectedString[] = _("The Crafty Shield protected\n{B_DEF_NAME_WITH_PREFIX}!");
static const u8 MatBlockProtectedString[] = _("{B_CURRENT_MOVE} was blocked by the\nkicked-up mat!");
static const u8 QuickGuardProtectedString[] = _("The Quick Guard protected\n{B_DEF_NAME_WITH_PREFIX}!");
static const u8 WideGuardProtectedString[] = _("The Wide Guard protected\n{B_DEF_NAME_WITH_PREFIX}!");
static const u8 RoastedBerryString[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nwas burnt up!");

static u32 AccuracyCalcPassDefAbilityItemEffect(u16 move, u8 bankAtk, u8 bankDef, u8 defAbility, u8 defEffect);

bool8 JumpIfMoveAffectedByProtect(u16 move, u8 bankAtk, u8 bankDef, bool8 actuallyJump)
{
    bool8 affected;

	if ((IsAnyMaxMove(move) && !IsDynamaxed(bankDef)) //Otherwise using a Max Move on Max Guard
	|| (gBattleStruct->zMoveData.active && SPLIT(move) != SPLIT_STATUS))
		return FALSE;

	affected = ProtectAffects(move, bankAtk, bankDef, TRUE);

	if (affected)
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleStruct->ResultFlags[bankDef] = MOVE_RESULT_MISSED;
		
		if (actuallyJump)
			JumpIfMoveFailed(7, move);
	}

	return affected;
}

bool8 ProtectAffects(u16 move, u8 bankAtk, u8 bankDef, bool8 set)
{
	u8 effect = 0;
	u8 protectFlag = gBattleMoves[move].flags & FLAG_PROTECT_AFFECTED;
	u8 split = SPLIT(move);
	u8 contact = CheckContact(move, bankAtk);
	u8 target = gBattleMoves[move].target;
	u8 defSide = SIDE(bankDef);

	if (ProtectedByMaxGuard(bankDef, move))
	{
		effect = 1;
		gBattleStruct->missStringId[bankDef] = gBattleCommunication[6] = 1;
	}
	if (gProtectStructs[bankDef].protected && protectFlag)
	{
		effect = 1;
		gBattleStruct->missStringId[bankDef] = gBattleCommunication[6] = 1;
	}
	else if (gProtectStructs[bankDef].KingsShield && protectFlag && split != SPLIT_STATUS)
	{
		effect = 1;
		gBattleStruct->missStringId[bankDef] = 1;
		if (contact && set)
		{
			gProtectStructs[bankDef].kingsshield_damage = 1;
			gBattleCommunication[6] = 1;
		}
	}
	else if (gProtectStructs[bankDef].SpikyShield && protectFlag)
	{
		effect = 1;
		gBattleStruct->missStringId[bankDef] = 1;
		if (contact && set)
		{
			gProtectStructs[bankDef].spikyshield_damage = 1;
			gBattleCommunication[6] = 1;
		}
	}
	else if (gProtectStructs[bankDef].BanefulBunker && protectFlag)
	{
		effect = 1;
		gBattleStruct->missStringId[bankDef] = 1;
		if (contact && set)
		{
			gProtectStructs[bankDef].banefulbunker_damage = 1;
			gBattleCommunication[6] = 1;
		}
	}
	else if (gProtectStructs[bankDef].obstruct && protectFlag)
	{
		effect = 1;
		gBattleStruct->missStringId[bankDef] = 1;
		if (contact && set)
		{
			gProtectStructs[bankDef].obstructDamage = TRUE;
			gBattleCommunication[6] = 1;
		}
	}
	else if (gSideStatuses[defSide] & SIDE_STATUS_CRAFTY_SHIELD && target != MOVE_TARGET_USER && split == SPLIT_STATUS)
	{
		effect = 1;
		gBattleStringLoader = CraftyShieldProtectedString;
		gBattleStruct->missStringId[bankDef] = gBattleCommunication[6] = 5;
	}
	else if (gSideStatuses[defSide] & SIDE_STATUS_MAT_BLOCK && protectFlag && split != SPLIT_STATUS)
	{
		effect = 1;
		gBattleStringLoader = MatBlockProtectedString;
		gBattleStruct->missStringId[bankDef] = gBattleCommunication[6] = 6;
	}
	else if (gSideStatuses[defSide] & SIDE_STATUS_QUICK_GUARD && protectFlag && PriorityCalc(bankAtk, B_ACTION_USE_MOVE, move) > 0)
	{
		effect = 1;
		gBattleStringLoader = QuickGuardProtectedString;
		gBattleStruct->missStringId[bankDef] = gBattleCommunication[6] = 7;
	}
	else if (gSideStatuses[defSide] & SIDE_STATUS_WIDE_GUARD && protectFlag && (target == MOVE_TARGET_BOTH || target == MOVE_TARGET_FOES_AND_ALLY))
	{
		effect = 1;
		gBattleStringLoader = WideGuardProtectedString;
		gBattleStruct->missStringId[bankDef] = gBattleCommunication[6] = 8;
	}
	else if (IsRaidBattle()
	&& split == SPLIT_STATUS
	&& gBattlerAttacker != bankDef
	&& bankDef == BANK_RAID_BOSS
	&& gBattleStruct->dynamaxData.raidShieldsUp)
	{
		effect = 1;
		gBattleStringLoader = gText_RaidShieldProtected;
		gBattleStruct->missStringId[bankDef] = gBattleCommunication[6] = 9;
	}

	return effect;
}

bool8 DoesProtectionMoveBlockMove(u8 bankAtk, u8 bankDef, u16 atkMove, u16 protectMove)
{
	u8 protectFlag = gBattleMoves[atkMove].flags & FLAG_PROTECT_AFFECTED;
	u8 split = SPLIT(atkMove);
	u8 target = gBattleMoves[atkMove].target;

	if (!CheckTableForMove(atkMove, gMovesThatLiftProtectTable))
	{
		switch (protectMove) {
			case MOVE_PROTECT:
			case MOVE_SPIKYSHIELD:
			case MOVE_BANEFULBUNKER:
				return protectFlag != 0;

			case MOVE_KINGSSHIELD:
			case MOVE_OBSTRUCT:
				return protectFlag && split != SPLIT_STATUS;

			case MOVE_MATBLOCK:
				return gDisableStructs[bankDef].isFirstTurn && protectFlag && split != SPLIT_STATUS;

			case MOVE_CRAFTYSHIELD:
				return target != MOVE_TARGET_USER && split == SPLIT_STATUS;

			case MOVE_QUICKGUARD:
				return protectFlag && PriorityCalc(bankAtk, B_ACTION_USE_MOVE, atkMove) > 0;

			case MOVE_WIDEGUARD:
				return protectFlag && target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY);
		}
	}

	return FALSE;
}

bool8 MissesDueToSemiInvulnerability(u8 bankAtk, u8 bankDef, u16 move)
{
	if (!CanHitSemiInvulnerableTarget(bankAtk, bankDef, move))
	{
		if (((gStatuses3[bankDef] & (STATUS3_ON_AIR | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET)) && !CheckTableForMove(move, gIgnoreInAirMoves))
		||  ((gStatuses3[bankDef] & STATUS3_UNDERGROUND) && !CheckTableForMove(move, gIgnoreUndergoundMoves))
		||  ((gStatuses3[bankDef] & STATUS3_UNDERWATER) && !CheckTableForMove(move, gIgnoreUnderwaterMoves))
		||   (gStatuses3[bankDef] & STATUS3_DISAPPEARED))
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

bool8 AccuracyCalcHelper(u16 move, u8 bankDef)
{
	u8 doneStatus = FALSE;
	gBattleStruct->ResultFlags[bankDef] &= ~MOVE_RESULT_NO_EFFECT;
	if (MissesDueToSemiInvulnerability(gBattlerAttacker, bankDef, move))
	{
		gBattleStruct->ResultFlags[bankDef] = MOVE_RESULT_MISSED;
		//JumpIfMoveFailed(7, move);
		gHitMarker &= ~(HITMARKER_IGNORE_ON_AIR | HITMARKER_IGNORE_UNDERGROUND | HITMARKER_IGNORE_UNDERWATER);
		return TRUE;
	}

	//lock-on/mind reader checked,
	//then no guard,
	//always hiting toxic on poison types,
	//then stomp on a minimized target,
	//then always hitting telekinesis except 0HKO moves,
	//then 0 acc moves
	if (((gStatuses3[bankDef] & STATUS3_ALWAYS_HITS) && gDisableStructs[bankDef].battlerWithSureHit == gBattlerAttacker)
	||   (ABILITY(gBattlerAttacker) == ABILITY_NOGUARD) || (ABILITY(bankDef) == ABILITY_NOGUARD)
	||   (move == MOVE_TOXIC && IsOfType(gBattlerAttacker, TYPE_POISON))
	||   (CheckTableForMove(move, gAlwaysHitWhenMinimizedMoves) && gStatuses3[bankDef] & STATUS3_MINIMIZED)
	||  ((gStatuses3[bankDef] & STATUS3_TELEKINESIS) && gBattleMoves[move].effect != EFFECT_OHKO)
	||	 gBattleMoves[move].accuracy == 0)
	{
		//JumpIfMoveFailed(7, move);
		doneStatus = TRUE;
	}
	else if (WEATHER_HAS_EFFECT)
	{
		if (((gBattleWeather & WEATHER_RAIN_ANY) && CheckTableForMove(move, gAlwaysHitInRainMoves) && ITEM_EFFECT(bankDef) != HOLD_EFFECT_UTILITY_UMBRELLA)
		||  ((gBattleWeather & WEATHER_HAIL_ANY) && move == MOVE_BLIZZARD))
		{
			//JumpIfMoveFailed(7, move);
			doneStatus = TRUE;
		}
	}

	gHitMarker &= ~(HITMARKER_IGNORE_ON_AIR | HITMARKER_IGNORE_UNDERGROUND | HITMARKER_IGNORE_UNDERWATER);
	return doneStatus;
}

u32 AccuracyCalc(u16 move, u8 bankAtk, u8 bankDef)
{
	return AccuracyCalcPassDefAbilityItemEffect(move, bankAtk, bankDef, ABILITY(bankDef), ITEM_EFFECT(bankDef));
}

static u32 AccuracyCalcPassDefAbilityItemEffect(u16 move, u8 bankAtk, u8 bankDef, u8 defAbility, u8 defEffect)
{
	u8 moveAcc;
	s8 buff;
	u32 calc;
	u8 atkEffect  = ITEM_EFFECT(bankAtk);
	//u8 defEffect  = ITEM_EFFECT(bankDef);
	u8 atkQuality = ITEM_QUALITY(bankAtk);
	u8 defQuality = ITEM_QUALITY(bankDef);
	u8 atkAbility = ABILITY(bankAtk);
	//u8 defAbility = ABILITY(bankDef);
	u8 moveSplit = CalcMoveSplit(gBattlerAttacker, move);

	u8 acc;
	if (defAbility == ABILITY_UNAWARE)
		acc = 6;
	else
		acc = STAT_STAGE(bankAtk, STAT_STAGE_ACC);

	if ((gBattleMons[bankDef].status2 & STATUS2_FORESIGHT)
	||  (gBattleMons[bankDef].status2 & STATUS3_MIRACLE_EYED)
	||   atkAbility == ABILITY_UNAWARE
	||   atkAbility == ABILITY_KEEN_EYE
	||   CheckTableForMove(move, gIgnoreStatChangesMoves))
	{
		buff = acc;
	}
	else
		buff = acc + 6 - STAT_STAGE(bankDef, STAT_STAGE_EVASION);

	if (buff < STAT_STAGE_MIN)
		buff = STAT_STAGE_MIN;
	if (buff > STAT_STAGE_MAX)
		buff = STAT_STAGE_MAX;

	moveAcc = gBattleMoves[move].accuracy;

	//Check Thunder + Hurricane in sunny weather
	if (WEATHER_HAS_EFFECT
	&& (gBattleWeather & WEATHER_SUN_ANY)
	&& defEffect != HOLD_EFFECT_UTILITY_UMBRELLA
	&& CheckTableForMove(move, gAlwaysHitInRainMoves))
		moveAcc = 50;

	//Check Wonder Skin for Status moves
	if (defAbility == ABILITY_WONDERSKIN && moveSplit == SPLIT_STATUS && moveAcc > 50)
		moveAcc = 50;

	calc = sAccuracyStageRatios[buff].dividend * moveAcc;
	calc = calc / sAccuracyStageRatios[buff].divisor;

	switch (atkAbility) {
		case ABILITY_COMPOUND_EYES:
			calc = (calc * 130) / 100; // 1.3 Compound Eyes boost
			break;

		case ABILITY_HUSTLE:
			if (moveSplit == SPLIT_PHYSICAL)
				calc = (calc * 80) / 100; // 0.8 Hustle loss;
			break;

		case ABILITY_VICTORYSTAR:
			calc = (calc * 110)/ 100; // 1.1 Victory Star boost
	}

	if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(bankAtk)) == ABILITY_VICTORYSTAR)
		calc = (calc * 110)/ 100; // 1.1 Victory Star partner boost

	if (WEATHER_HAS_EFFECT)
	{
		switch (defAbility) {
			case ABILITY_SAND_VEIL:
				if (gBattleWeather & WEATHER_SANDSTORM_ANY)
					calc = (calc * 80)/ 100; // 0.8 Sand Veil loss
				break;

			case ABILITY_SNOWCLOAK:
				if (gBattleWeather & WEATHER_HAIL_ANY)
					calc = (calc * 80)/ 100; // 0.8 Snow Cloak loss
		}

		if (gBattleWeather & WEATHER_FOG_ANY)
		{
			#ifdef KEENEYE_INFILTRATOR_DONT_LOSS_ACC_ON_FOG
			if (atkAbility != ABILITY_KEENEYE && atkAbility != ABILITY_INFILTRATOR)
			#endif
				calc = (calc * 60) / 100; // 0.6 Fog loss
		}
	}

	if (defAbility == ABILITY_TANGLEDFEET && IsConfused(bankDef))
		calc /= 2; // 0.5 Tangled Feet loss

	switch (atkEffect) {
		case HOLD_EFFECT_WIDE_LENS:
			calc = (calc * (100 + atkQuality)) / 100; // 1.1 Wide Lens boost
			break;

		case HOLD_EFFECT_ZOOM_LENS:
			if (BankMovedBefore(bankDef, bankAtk))
				calc = (calc * (100 + atkQuality)) / 100; // 1.2 Zoom Lens boost
	}

	if (IsGravityActive())
		calc = (calc * 5) / 3; // 5/3 Gravity boost

	if (defEffect == HOLD_EFFECT_EVASION_UP)
		calc = (calc * (100 - defQuality)) / 100; // 0.9 Bright Powder/Lax Incense loss

	if (gBattleStruct->MicleBerryBits & gBitTable[bankAtk])
	{
		if (ABILITY(bankAtk) == ABILITY_RIPEN)
			calc = (calc * 14) / 10; // 1.4 Micle Berry Boost
		else
			calc = (calc * 12) / 10; // 1.2 Micle Berry Boost
	}

	if (gBattleMoves[move].accuracy == 0) //Always hit
		calc = 0xFFFF;

	return calc;
}

u32 VisualAccuracyCalc(u16 move, u8 bankAtk, u8 bankDef)
{
	u8 defEffect  = GetRecordedItemEffect(bankDef);
	u8 defAbility = GetRecordedAbility(bankDef);

	return AccuracyCalcPassDefAbilityItemEffect(move, bankAtk, bankDef, defAbility, defEffect);
}

u32 VisualAccuracyCalc_NoTarget(u16 move, u8 bankAtk)
{
	u8 moveAcc;
	u8 acc;
	u32 calc;
	u8 atkEffect  = ITEM_EFFECT(bankAtk);
	u8 atkQuality = ITEM_QUALITY(bankAtk);
	u8 atkAbility = ABILITY(bankAtk);
	u8 moveSplit = SPLIT(move);

	acc = gBattleMons[bankAtk].statStages[STAT_STAGE_ACC-1];
	moveAcc = gBattleMoves[move].accuracy;

	//Check Thunder + Hurricane in sunny weather
	if (WEATHER_HAS_EFFECT
	&& (gBattleWeather & WEATHER_SUN_ANY)
	&& CheckTableForMove(move, gAlwaysHitInRainMoves))
		moveAcc = 50;

	calc = sAccuracyStageRatios[acc].dividend * moveAcc;
	calc = calc / sAccuracyStageRatios[acc].divisor;

	switch (atkAbility)
    {
		case ABILITY_COMPOUND_EYES:
			calc = (calc * 130) /  100; // 1.3 Compound Eyes boost
			break;

		case ABILITY_HUSTLE:
			if (moveSplit == SPLIT_PHYSICAL)
				calc = (calc * 80) /  100; // 0.8 Hustle loss;
			break;

		case ABILITY_VICTORYSTAR:
			calc = (calc * 110) /  100; // 1.1 Victory Star boost
	}

	if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(bankAtk)) == ABILITY_VICTORYSTAR)
		calc = (calc * 110) /  100; // 1.1 Victory Star partner boost

	if (WEATHER_HAS_EFFECT &&  gBattleWeather & WEATHER_FOG_ANY)
	{
		#ifdef UNBOUND
		if (atkAbility != ABILITY_KEENEYE && atkAbility != ABILITY_INFILTRATOR)
		#endif
			calc = (calc * 60) /  100; // 0.6 Fog loss
	}

	if (atkEffect == HOLD_EFFECT_WIDE_LENS)
		calc = (calc * (100 + atkQuality)) /  100; // 1.1 Wide Lens boost

	if (IsGravityActive())
		calc = (calc * 5) / 3; // 5/3 Gravity boost

	if (gBattleStruct->MicleBerryBits & (1 << bankAtk))
	{
		if (ABILITY(bankAtk) == ABILITY_RIPEN)
			calc = (calc * 14) / 10; // 1.4 Micle Berry Boost
		else
			calc = (calc * 12) / 10; // 1.2 Micle Berry Boost
	}

	if (WEATHER_HAS_EFFECT)
	{
		if (((gBattleWeather & WEATHER_RAIN_ANY) && CheckTableForMove(move, gAlwaysHitInRainMoves))
		||  ((gBattleWeather & WEATHER_HAIL_ANY) && move == MOVE_BLIZZARD))
			calc = 0; //No Miss
	}

	if (gBattleMoves[move].accuracy == 0) //Always hit
		calc = 0xFFFF;

	return calc;
}

void JumpIfMoveFailed(u8 adder, u16 move)
{
	const u8* BS_ptr = gBattlescriptCurrInstr + adder;
	if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
	{
		gLastLandedMoves[gBattlerTarget] = 0;
		gLastHitByType[gBattlerTarget] = 0;
		BS_ptr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		TrySetDestinyBondToHappen();
		if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, gBattlerTarget, 0, 0, move))
			return;
	}
	gBattlescriptCurrInstr = BS_ptr;
}
