#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "battle_message.h"
#include "battle_controllers.h"
#include "util.h"
#include "item.h"
#include "random.h"
#include "pokemon.h"
#include "build_pokemon.h"
#include "battle_ai_script_commands.h"
#include "menu.h"
#include "malloc.h"
#include "link.h"
#include "dynamax.h"
#include "frontier.h"
#include "mega.h"
#include "constants/abilities.h"
#include "constants/hold_effects.h"
#include "constants/battle_move_effects.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/species.h"

struct SpecialZMove
{
	u16 species;
	u16 item;
	u16 move;
	u16 zmove;
};

static const struct SpecialZMove sSpecialZMoveTable[] =
{
	{SPECIES_RAICHU_A,				ITEM_ALORAICHIUM_Z, 		MOVE_THUNDERBOLT, 		MOVE_STOKED_SPARKSURFER},
	{SPECIES_DECIDUEYE,				ITEM_DECIDIUM_Z, 			MOVE_SPIRITSHACKLE,		MOVE_SINISTER_ARROW_RAID},
	{SPECIES_EEVEE,					ITEM_EEVIUM_Z, 				MOVE_LASTRESORT,		MOVE_EXTREME_EVOBOOST},
	{SPECIES_INCINEROAR,			ITEM_INCINIUM_Z, 			MOVE_DARKESTLARIAT,		MOVE_MALICIOUS_MOONSAULT},
	{SPECIES_KOMMO_O,				ITEM_KOMMONIUM_Z,			MOVE_CLANGINGSCALES,	MOVE_CLANGOROUS_SOULBLAZE},
	{SPECIES_LUNALA,				ITEM_LUNALIUM_Z,			MOVE_MOONGEISTBEAM,		MOVE_MENACING_MOONRAZE_MAELSTROM},
	{SPECIES_NECROZMA_DAWN_WINGS,	ITEM_LUNALIUM_Z,			MOVE_MOONGEISTBEAM,		MOVE_MENACING_MOONRAZE_MAELSTROM},
	{SPECIES_LYCANROC,				ITEM_LYCANIUM_Z,			MOVE_STONEEDGE,			MOVE_SPLINTERED_STORMSHARDS},
	{SPECIES_LYCANROC_N,			ITEM_LYCANIUM_Z,			MOVE_STONEEDGE,			MOVE_SPLINTERED_STORMSHARDS},
	{SPECIES_LYCANROC_DUSK,			ITEM_LYCANIUM_Z,			MOVE_STONEEDGE,			MOVE_SPLINTERED_STORMSHARDS},
	{SPECIES_MARSHADOW,				ITEM_MARSHADIUM_Z, 			MOVE_SPECTRALTHIEF,		MOVE_SOUL_STEALING_7_STAR_STRIKE},
	{SPECIES_MEW,					ITEM_MEWNIUM_Z, 			MOVE_PSYCHIC,			MOVE_GENESIS_SUPERNOVA},
	{SPECIES_MIMIKYU,				ITEM_MIMIKIUM_Z, 			MOVE_PLAYROUGH,			MOVE_LETS_SNUGGLE_FOREVER},
	{SPECIES_MIMIKYU_BUSTED,		ITEM_MIMIKIUM_Z, 			MOVE_PLAYROUGH,			MOVE_LETS_SNUGGLE_FOREVER},
	{SPECIES_PIKACHU,				ITEM_PIKANIUM_Z, 			MOVE_VOLTTACKLE,		MOVE_CATASTROPIKA},
	{SPECIES_PIKACHU_CAP_ORIGINAL,	ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PIKACHU_CAP_HOENN,		ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PIKACHU_CAP_SINNOH,	ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PIKACHU_CAP_UNOVA,		ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PIKACHU_CAP_KALOS,		ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PIKACHU_CAP_ALOLA,		ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PIKACHU_CAP_PARTNER,	ITEM_PIKASHUNIUM_Z, 		MOVE_THUNDERBOLT,		MOVE_10000000_VOLT_THUNDERBOLT},
	{SPECIES_PRIMARINA,				ITEM_PRIMARIUM_Z, 			MOVE_SPARKLINGARIA,		MOVE_OCEANIC_OPERETTA},
	{SPECIES_SNORLAX,				ITEM_SNORLIUM_Z, 			MOVE_GIGAIMPACT,		MOVE_PULVERIZING_PANCAKE},
	{SPECIES_SOLGALEO,				ITEM_SOLGANIUM_Z, 			MOVE_SUNSTEELSTRIKE,	MOVE_SEARING_SUNRAZE_SMASH},
	{SPECIES_NECROZMA_DUSK_MANE,	ITEM_SOLGANIUM_Z, 			MOVE_SUNSTEELSTRIKE,	MOVE_SEARING_SUNRAZE_SMASH},
	{SPECIES_TAPU_KOKO,				ITEM_TAPUNIUM_Z, 			MOVE_NATURESMADNESS,	MOVE_GUARDIAN_OF_ALOLA},
	{SPECIES_TAPU_BULU,				ITEM_TAPUNIUM_Z, 			MOVE_NATURESMADNESS,	MOVE_GUARDIAN_OF_ALOLA},
	{SPECIES_TAPU_LELE,				ITEM_TAPUNIUM_Z, 			MOVE_NATURESMADNESS,	MOVE_GUARDIAN_OF_ALOLA},
	{SPECIES_TAPU_FINI,				ITEM_TAPUNIUM_Z, 			MOVE_NATURESMADNESS,	MOVE_GUARDIAN_OF_ALOLA},
	{SPECIES_NECROZMA_ULTRA,		ITEM_ULTRANECROZIUM_Z, 		MOVE_PHOTONGEYSER, 		MOVE_LIGHT_THAT_BURNS_THE_SKY},
};

void SetZEffect(void)
{
	int i;

	gBattleStruct->zMoveData.runningZEffect = TRUE;

	if (gBattleStruct->zMoveData.effect == Z_EFFECT_CURSE) {
		if (IsOfType(gBattlerAttacker, TYPE_GHOST))
			gBattleStruct->zMoveData.effect = Z_EFFECT_RECOVER_HP;
		else
			gBattleStruct->zMoveData.effect = Z_EFFECT_ATK_UP_1;
	}

	gBattleScripting.battler = gBattlerAttacker;

	switch (gBattleStruct->zMoveData.effect) {
		case Z_EFFECT_RESET_STATS:
			for (i = 0; i < BATTLE_STATS_NO-1; ++i) {
				if (gBattleMons[gBattlerAttacker].statStages[i] < 6) {
					gBattleMons[gBattlerAttacker].statStages[i] = 6;
				}
			}
			BattleScriptPush(gBattlescriptCurrInstr + 5);
			gBattlescriptCurrInstr = BattleScript_StatsResetZMove - 5;
			break;

		case Z_EFFECT_ALL_STATS_UP_1:
			if (!MainStatsMaxed(gBattlerAttacker)) {
				for (i = 0; i < STAT_ACC-1; ++i) { //Doesn't increase Acc or Evsn
					if (gBattleMons[gBattlerAttacker].statStages[i] < 12)
						++gBattleMons[gBattlerAttacker].statStages[i];
				}
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_AllStatsUpZMove - 5;
			}
			break;

		case Z_EFFECT_BOOST_CRITS:
			if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_FOCUS_ENERGY)) {
				gBattleMons[gBattlerAttacker].status2 |= STATUS2_FOCUS_ENERGY;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_BoostCritsZMove - 5;
			}
			break;

		case Z_EFFECT_FOLLOW_ME:
			gSideTimers[SIDE(gBattlerAttacker)].followmeTimer = 1;
			gSideTimers[SIDE(gBattlerAttacker)].followmeTarget = gBattlerAttacker;
			BattleScriptPush(gBattlescriptCurrInstr + 5);
			gBattlescriptCurrInstr = BattleScript_FollowMeZMove - 5;
			break;

		case Z_EFFECT_RECOVER_HP:
			if (gBattleMons[gBattlerAttacker].hp != gBattleMons[gBattlerAttacker].maxHP) {
				gBattleMoveDamage = (-1) * gBattleMons[gBattlerAttacker].maxHP;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_RecoverHPZMove - 5;
			}
			break;

		case Z_EFFECT_RESTORE_REPLACEMENT_HP:
			gBattleStruct->zMoveData.healReplacement = TRUE;
			BattleScriptPush(gBattlescriptCurrInstr + 5);
			gBattlescriptCurrInstr = BattleScript_SetUpHealReplacementZMove - 5;
			break;

		case Z_EFFECT_ATK_UP_1:
		case Z_EFFECT_DEF_UP_1:
		case Z_EFFECT_SPD_UP_1:
		case Z_EFFECT_SPATK_UP_1:
		case Z_EFFECT_SPDEF_UP_1:
		case Z_EFFECT_ACC_UP_1:
		case Z_EFFECT_EVSN_UP_1:
			if (!ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), gBattleStruct->zMoveData.effect - Z_EFFECT_ATK_UP_1 + 1, MOVE_EFFECT_AFFECTS_USER, 0)) {
				gBattleScripting.animArg1 = 0xE + (gBattleStruct->zMoveData.effect - Z_EFFECT_ATK_UP_1 + 1);
				gBattleScripting.animArg2 = 0;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_StatUpZMove - 5;
			}
			break;

		case Z_EFFECT_ATK_UP_2:
		case Z_EFFECT_DEF_UP_2:
		case Z_EFFECT_SPD_UP_2:
		case Z_EFFECT_SPATK_UP_2:
		case Z_EFFECT_SPDEF_UP_2:
		case Z_EFFECT_ACC_UP_2:
		case Z_EFFECT_EVSN_UP_2:
			if (!ChangeStatBuffs(SET_STAT_BUFF_VALUE(2), gBattleStruct->zMoveData.effect - Z_EFFECT_ATK_UP_2 + 1, MOVE_EFFECT_AFFECTS_USER, 0)) {
				gBattleScripting.animArg1 = 0xE + (gBattleStruct->zMoveData.effect - Z_EFFECT_ATK_UP_2 + 1);
				gBattleScripting.animArg2 = 0;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_StatUpZMove - 5;
			}
			break;

		case Z_EFFECT_ATK_UP_3:
		case Z_EFFECT_DEF_UP_3:
		case Z_EFFECT_SPD_UP_3:
		case Z_EFFECT_SPATK_UP_3:
		case Z_EFFECT_SPDEF_UP_3:
		case Z_EFFECT_ACC_UP_3:
		case Z_EFFECT_EVSN_UP_3:
			if (!ChangeStatBuffs(SET_STAT_BUFF_VALUE(3), gBattleStruct->zMoveData.effect - Z_EFFECT_ATK_UP_3 + 1, MOVE_EFFECT_AFFECTS_USER, 0)) {
				gBattleScripting.animArg1 = 0xE + (gBattleStruct->zMoveData.effect - Z_EFFECT_ATK_UP_3 + 1);
				gBattleScripting.animArg2 = 0;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_StatUpZMove - 5;
			}
			break;
	}
	gBattleStruct->zMoveData.runningZEffect = FALSE;
}

u16 GetTypeBasedZMove(u16 move, u8 bank)
{
	u8 moveType = gBattleMoves[move].type;

	if (move == MOVE_WEATHERBALL || move == MOVE_AURAWHEEL || move == MOVE_TERRAINPULSE)
		moveType = GetMoveTypeSpecial(bank, move);

	if (moveType < TYPE_FIRE)
		return MOVE_BREAKNECK_BLITZ_P + (moveType * 2) + CalcMoveSplit(bank, move);
	else if (moveType >= TYPE_FAIRY)
		return MOVE_TWINKLE_TACKLE_P + ((moveType - TYPE_FAIRY) * 2) + CalcMoveSplit(bank, move);
	else
		return MOVE_BREAKNECK_BLITZ_P + ((moveType - 1) * 2) + CalcMoveSplit(bank, move);
}

u16 GetSpecialZMove(u16 move, u16 species, u16 item)
{
	bool8 isSpecialZCrystal = FALSE;
    u32 i;
	for (i = 0; i < ARRAY_COUNT(sSpecialZMoveTable); ++i)
	{
		if (sSpecialZMoveTable[i].item == item)
		{
			isSpecialZCrystal = TRUE;

			if (sSpecialZMoveTable[i].species == species
			&&  sSpecialZMoveTable[i].move == move)
			{
				return sSpecialZMoveTable[i].zmove;
			}
		}
	}

	if (isSpecialZCrystal)
		return 0xFFFF;
	else
		return MOVE_NONE;
}

bool8 IsSpecialZCrystal(u16 item)
{
    u32 i;
	for (i = 0; i < ARRAY_COUNT(sSpecialZMoveTable); ++i)
	{
		if (sSpecialZMoveTable[i].item == item)
			return TRUE;
	}

	return FALSE;
}

const u8* GetZMoveName(u16 move)
{
	if (!IsZMove(move))
		return gMoveNames[MOVE_BREAKNECK_BLITZ_P];
	else
		return gMoveNames[move];
}

bool8 DoesZMoveUsageStopMegaEvolution(u8 bank)
{
	return gBattleStruct->zMoveData.used[bank]
		&& gBattleStruct->zMoveData.partyIndex[SIDE(bank)] & gBitTable[gBattlerPartyIndexes[bank]];
}

bool8 DoesZMoveUsageStopDynamaxing(u8 bank)
{
	return DoesZMoveUsageStopMegaEvolution(bank);
}

u16 CanUseZMove(u8 bank, u8 moveIndex, u16 move)
{
	struct Pokemon* mon = GetBankPartyData(bank);
	u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);

	if (move == MOVE_NONE)
		move = gBattleMons[bank].moves[moveIndex];

	#if (defined VAR_KEYSTONE && !defined DEBUG_MEGA)
	//Z-Moves can't be used until receieving a Keystone
	if (!(gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))) //Z-Moves can always be used in these battles
	{
		u8 position = GetBattlerPosition(bank);
		if (position == B_POSITION_PLAYER_LEFT || (!IsTagBattle() && position == B_POSITION_PLAYER_RIGHT))
		{
			u16 keystone = VarGet(VAR_KEYSTONE);
			if (keystone == ITEM_NONE)
				return MOVE_NONE;
		}
	}
	#endif

	if (IsMegaZMoveBannedBattle()
	|| IsMega(bank)
	|| IsRedPrimal(bank)
	|| IsBluePrimal(bank))
		return MOVE_NONE;

	if (IsZCrystal(item) || item == ITEM_ULTRANECROZIUM_Z) //The only "Mega Stone" that let's you use a Z-Move
	{
		u16 zMove = GetSpecialZMove(move, SPECIES(bank), item);
		if (zMove != MOVE_NONE && zMove != 0xFFFF) //There's a special Z-Move
			return zMove;

		if (move != MOVE_NONE && zMove != 0xFFFF //Special Z-Crystal
		&& gBattleMoves[move].type == ItemId_GetHoldEffectParam(item))
		{
			if (SPLIT(move) == SPLIT_STATUS)
				return 0xFFFF;
			else
				return GetTypeBasedZMove(move, bank);
		}
	}

	return MOVE_NONE;
}

u16 ReplaceWithZMoveRuntime(u8 bankAtk, u16 move)
{
	if (gBattleStruct->zMoveData.toBeUsed[bankAtk]
	&& !gBattleStruct->zMoveData.used[bankAtk]
	&& SPLIT(move) != SPLIT_STATUS)
	{
		u16 zMove = CanUseZMove(bankAtk, 0xFF, move);
		if (zMove != MOVE_NONE)
			move = zMove;
	}
	else if (IsDynamaxed(bankAtk) || (gBattleStruct->dynamaxData.toBeUsed[bankAtk] && !gBattleStruct->dynamaxData.used[bankAtk]))
	{
        u16 maxMove;

		if (IsRaidBattle() && bankAtk == BANK_RAID_BOSS && IsRaidBossUsingRegularMove(bankAtk, move))
			return move; //This turn the raid boss isn't using a Max Move

		maxMove = GetMaxMoveByMove(bankAtk, move);
		if (maxMove != MOVE_NONE)
			move = maxMove;
	}

	return move;
}
