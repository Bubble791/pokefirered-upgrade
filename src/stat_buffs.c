#include "global.h"
#include "item.h"
#include "text.h"
#include "util.h"
#include "link.h"
#include "berry.h"
#include "random.h"
#include "pokemon.h"
#include "string_util.h"
#include "field_weather.h"
#include "event_data.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "battle_message.h"
#include "battle_controllers.h"
#include "battle_string_ids.h"
#include "battle_ai_script_commands.h"
#include "dynamax.h"
#include "strings.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/weather.h"
#include "constants/abilities.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/battle_anim.h"
#include "constants/hold_effects.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_script_commands.h"

bool8 DefiantActivation(void)
{
	switch (ABILITY(gBattlerTarget)) 
	{
		case ABILITY_DEFIANT:
			gBattleScripting.statChanger = INCREASE_2 | STAT_STAGE_ATK;
			break;

		case ABILITY_COMPETITIVE:
			gBattleScripting.statChanger = INCREASE_2 | STAT_STAGE_SPATK;
			break;

		case ABILITY_RATTLED:
			if (gBattleStruct->intimidateActive)
				gBattleScripting.statChanger = INCREASE_1 | STAT_STAGE_SPEED;
			break;

		default:
			return FALSE;
	}

	BattleScriptPushCursor();
	gBattlescriptCurrInstr = BattleScript_DefiantCompetitive;
	gBattleScripting.battler = gBattlerTarget;
	return TRUE;
}

u8 ChangeStatBuffs(s8 statValue, u8 statId, u8 flags, const u8* BS_ptr)
{
	bool8 certain = FALSE;
	bool8 affectsUser = FALSE;
	bool8 notProtectAffected = FALSE;
	u32 index;
    ability_t ability;

	if (flags & MOVE_EFFECT_AFFECTS_USER)
	{
		gActiveBattler = gBattlerAttacker;
		affectsUser = TRUE;
	}
	else
		gActiveBattler = gBattlerTarget;

	ability = ABILITY(gActiveBattler);

	flags &= ~(MOVE_EFFECT_AFFECTS_USER);

	if (flags & MOVE_EFFECT_CERTAIN)
		certain++;
	flags &= ~(MOVE_EFFECT_CERTAIN);

	if (flags & STAT_CHANGE_NOT_PROTECT_AFFECTED)
		notProtectAffected++;
	flags &= ~(STAT_CHANGE_NOT_PROTECT_AFFECTED);

	PREPARE_STAT_BUFFER(gBattleTextBuff1, statId)

	if ((statValue << 0x18) < 0) // stat decrease
	{
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY && !gBattleStruct->zMoveData.runningZEffect)
			goto RAISE_STAT_BUFF;

	LOWER_STAT_BUFF:
		if (BankSideHasMist(gActiveBattler)
		&& !certain
		&& gCurrentMove != MOVE_CURSE
		&& ABILITY(gBattlerAttacker) != ABILITY_INFILTRATOR)
		{
			if (flags == STAT_CHANGE_BS_PTR)
			{
				if (gSpecialStatuses[gActiveBattler].statLowered)
				{
					gBattlescriptCurrInstr = BS_ptr;
				}
				else
				{
					BattleScriptPush(BS_ptr);
					gBattleScripting.battler = gActiveBattler;
					gBattlescriptCurrInstr = BattleScript_MistProtected;
					gSpecialStatuses[gActiveBattler].statLowered = 1;
				}
			}
			return STAT_CHANGE_DIDNT_WORK;
		}

		else if (gCurrentMove != MOVE_CURSE
			  && notProtectAffected != TRUE
			  && JumpIfMoveAffectedByProtect(gCurrentMove, gBattlerAttacker, gBattlerTarget, TRUE))
		{
			gBattlescriptCurrInstr = BattleScript_ButItFailed;
			return STAT_CHANGE_DIDNT_WORK;
		}

		else if ((ability == ABILITY_CLEAR_BODY
			  ||  ability == ABILITY_WHITE_SMOKE
			  //||  ability == ABILITY_FULLMETALBODY
			  || (ability == ABILITY_FLOWERVEIL && IsOfType(gActiveBattler, TYPE_GRASS)))
		&& !certain && gCurrentMove != MOVE_CURSE)
		{
			if (flags == STAT_CHANGE_BS_PTR)
			{
				if (gSpecialStatuses[gActiveBattler].statLowered)
				{
					gBattlescriptCurrInstr = BS_ptr;
				}
				else
				{
					BattleScriptPush(BS_ptr);
					gBattleScripting.battler = gActiveBattler;
					gBattleCommunication[0] = gActiveBattler;
					gBattlescriptCurrInstr = BattleScript_AbilityNoStatLoss;
					gLastUsedAbility = gBattleMons[gActiveBattler].ability;
					RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
					gSpecialStatuses[gActiveBattler].statLowered = 1;
				}
			}
			return STAT_CHANGE_DIDNT_WORK;
		}

		else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
		&& IsOfType(gActiveBattler, TYPE_GRASS)
		&& ABILITY(PARTNER(gActiveBattler)) == ABILITY_FLOWERVEIL
		&& !certain && gCurrentMove != MOVE_CURSE)
		{
			if (flags == STAT_CHANGE_BS_PTR)
			{
				if (gSpecialStatuses[gActiveBattler].statLowered)
				{
					gBattlescriptCurrInstr = BS_ptr;
				}
				else
				{
					BattleScriptPush(BS_ptr);
					gBattleScripting.battler = PARTNER(gActiveBattler);
					gBattleCommunication[0] = gActiveBattler;
					gBattlescriptCurrInstr = BattleScript_PartnerAbilityNoStatLoss;
					gSpecialStatuses[gActiveBattler].statLowered = 1;
				}
			}
			return STAT_CHANGE_DIDNT_WORK;
		}

		else if (((ability == ABILITY_KEEN_EYE && statId == STAT_STAGE_ACC)
			  ||  (ability == ABILITY_HYPER_CUTTER && statId == STAT_STAGE_ATK)
			  ||  (ability == ABILITY_BIGPECKS && statId == STAT_STAGE_DEF)
			  ||  (ability == ABILITY_INNER_FOCUS && gBattleStruct->intimidateActive)
			  ||  (ability == ABILITY_OWN_TEMPO && gBattleStruct->intimidateActive)
			  ||  (ability == ABILITY_OBLIVIOUS && gBattleStruct->intimidateActive)
			  ||  (ability == ABILITY_SCRAPPY && gBattleStruct->intimidateActive))
		&& !certain)
		{
			if (flags == STAT_CHANGE_BS_PTR)
			{
				BattleScriptPush(BS_ptr);
				gBattleScripting.battler = gActiveBattler;
				gBattlescriptCurrInstr = BattleScript_AbilityNoSpecificStatLoss;
			}
			return STAT_CHANGE_DIDNT_WORK;
		}
		else if (ability == ABILITY_MIRRORARMOR && gBattleStruct->intimidateActive && !certain)
		{
			if (flags == STAT_CHANGE_BS_PTR)
			{
				BattleScriptPush(BS_ptr);
				gBattleScripting.battler = gActiveBattler;
				gBattlescriptCurrInstr = BattleScript_MirrorArmorReflectsIntimidate;
			}
			return STAT_CHANGE_DIDNT_WORK;
		}
		else if (ability == ABILITY_MIRRORARMOR && !affectsUser && gBattlerAttacker != gBattlerTarget && gActiveBattler == gBattlerTarget)
		{
			if (flags == STAT_CHANGE_BS_PTR)
			{
				BattleScriptPush(BS_ptr);
				gBattleScripting.battler = gActiveBattler;
				gBattlescriptCurrInstr = BattleScript_MirrorArmorReflectsStatLoss;
			}
			return STAT_CHANGE_DIDNT_WORK;
		}
		else if ((ability == ABILITY_SHIELD_DUST || SheerForceCheck()) && !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE) && flags == 0)
		{
			return STAT_CHANGE_DIDNT_WORK;
		}
		else // try to decrease
		{
			statValue = -GET_STAT_BUFF_VALUE(statValue);

			if (ability == ABILITY_SIMPLE && !gBattleStruct->zMoveData.runningZEffect)
				statValue *= 2;

			gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
			index = 1;
			if (statValue == -2)
			{
				gBattleTextBuff2[1] = B_BUFF_STRING;
				gBattleTextBuff2[2] = STRINGID_STATHARSHLY;
				gBattleTextBuff2[3] = STRINGID_STATHARSHLY >> 8;
				index = 4;
			}
			else if (statValue <= -3)
			{
				gBattleTextBuff2[1] = B_BUFF_STRING;
				gBattleTextBuff2[2] = 0x85;
				gBattleTextBuff2[3] = 0x1;
				index = 4;
			}
			if (!gBattleStruct->zMoveData.runningZEffect) {
				gBattleTextBuff2[index] = B_BUFF_STRING;
				index++;
				gBattleTextBuff2[index] = STRINGID_STATFELL;
				index++;
				gBattleTextBuff2[index] = STRINGID_STATFELL >> 8;
				index++;
			}
			gBattleTextBuff2[index] = B_BUFF_EOS;

			if (STAT_STAGE(gActiveBattler, statId) == 0)
				gBattleCommunication[MULTISTRING_CHOOSER] = 2;
			else
				gBattleCommunication[MULTISTRING_CHOOSER] = (gBattlerTarget == gActiveBattler);

			gBattleStruct->statFellThisTurn[gActiveBattler] = TRUE;
			gBattleStruct->statFellThisRound[gActiveBattler] = TRUE;
		}
	}

	//Stat Increase
	else
	{
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY && !gBattleStruct->zMoveData.runningZEffect)
			goto LOWER_STAT_BUFF;

	RAISE_STAT_BUFF:
		statValue = GET_STAT_BUFF_VALUE(statValue);

		if (ability == ABILITY_SIMPLE && !gBattleStruct->zMoveData.runningZEffect)
			statValue *= 2;

		gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
		index = 1;
		if (statValue == 2)
		{
			gBattleTextBuff2[1] = B_BUFF_STRING;
			gBattleTextBuff2[2] = STRINGID_STATSHARPLY;
			gBattleTextBuff2[3] = STRINGID_STATSHARPLY >> 8;
			index = 4;
		}
		if (statValue >= 3)
		{
			gBattleTextBuff2[1] = B_BUFF_STRING;
			gBattleTextBuff2[2] = 0x86;
			gBattleTextBuff2[3] = 0x1;
			index = 4;
		}
		if (!gBattleStruct->zMoveData.runningZEffect) {
			gBattleTextBuff2[index] = B_BUFF_STRING;
			index++;
			gBattleTextBuff2[index] = STRINGID_STATROSE;
			index++;
			gBattleTextBuff2[index] = STRINGID_STATROSE >> 8;
			index++;
		}
		gBattleTextBuff2[index] = B_BUFF_EOS;

		if (gBattleMons[gActiveBattler].statStages[statId - 1] == STAT_STAGE_MAX)
			gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		else
			gBattleCommunication[MULTISTRING_CHOOSER] = (gBattlerTarget == gActiveBattler);

		gBattleStruct->statRoseThisRound[gActiveBattler] = TRUE;
	}

	gBattleMons[gActiveBattler].statStages[statId - 1] += statValue;
	if (gBattleMons[gActiveBattler].statStages[statId - 1] < 0)
		gBattleMons[gActiveBattler].statStages[statId - 1] = 0;
	if (gBattleMons[gActiveBattler].statStages[statId - 1] > STAT_STAGE_MAX)
		gBattleMons[gActiveBattler].statStages[statId - 1] = STAT_STAGE_MAX;

	if (gBattleCommunication[MULTISTRING_CHOOSER] == 2 && flags & STAT_CHANGE_BS_PTR)
		gMoveResultFlags |= MOVE_RESULT_MISSED;

	if (gBattleCommunication[MULTISTRING_CHOOSER] == 2 && !(flags & STAT_CHANGE_BS_PTR))
		return STAT_CHANGE_DIDNT_WORK;

	return STAT_CHANGE_WORKED;
}

bool8 NewXSpecialBoost(u16 item, u8 boostAmount)
{
	u8 stat;
	bool8 retVal = TRUE; //Didn't work by default

	if (item == ITEM_X_SP_DEF) //Not in Gen 3 need todo
		stat = STAT_STAGE_SPDEF;
	else
		stat = STAT_STAGE_SPATK;

	if (STAT_STAGE(gActiveBattler, stat) < STAT_STAGE_MAX)
	{
		STAT_STAGE(gActiveBattler, stat) += boostAmount;
		if (STAT_STAGE(gActiveBattler, stat) > STAT_STAGE_MAX)
			STAT_STAGE(gActiveBattler, stat) = STAT_STAGE_MAX;
		retVal = FALSE;
		
		if (stat == STAT_STAGE_SPDEF)
			gBattleStruct->usedXSpDef = TRUE;
		else
			gBattleStruct->usedXSpDef = FALSE;
	}

	return retVal;
}

void PrepareStringForUsingXItem(u32 stat)
{
	u8 index = 4;

	stat = sStatsToRaise[stat];
	if (stat == STAT_SPATK && gBattleStruct->usedXSpDef)
		stat = STAT_SPDEF;

    gBattlerTarget = gBattlerInMenuId;
    StringCopy(gBattleTextBuff1, gStatNamesTable[stat]);
   
	gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
	gBattleTextBuff2[1] = B_BUFF_STRING;
	gBattleTextBuff2[2] = STRINGID_STATSHARPLY;
	gBattleTextBuff2[3] = STRINGID_STATSHARPLY >> 8;
	gBattleTextBuff2[index++] = B_BUFF_STRING;
	gBattleTextBuff2[index++] = STRINGID_STATROSE;
	gBattleTextBuff2[index++] = STRINGID_STATROSE >> 8;
	gBattleTextBuff2[index] = B_BUFF_EOS;

    BattleStringExpandPlaceholdersToDisplayedString(gText_PkmnsStatChanged2); //gText_PkmnsStatChanged2
}

void PrepareStringForAIUsingXItem(void)
{
	u8 index = 4;

	PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_ATK);
	gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
	gBattleTextBuff2[1] = B_BUFF_STRING;
	gBattleTextBuff2[2] = STRINGID_STATSHARPLY;
	gBattleTextBuff2[3] = STRINGID_STATSHARPLY >> 8;
	gBattleTextBuff2[index++] = B_BUFF_STRING;
	gBattleTextBuff2[index++] = STRINGID_STATROSE;
	gBattleTextBuff2[index++] = STRINGID_STATROSE >> 8;
	gBattleTextBuff2[index] = B_BUFF_EOS;

	while (!(gBattleStruct->AI_itemFlags[gBattlerAttacker & BIT_FLANK] & 1))
	{
		gBattleStruct->AI_itemFlags[gBattlerAttacker & BIT_FLANK] >>= 1;
		++gBattleTextBuff1[2];
	}

	gBattleScripting.animArg1 = gBattleTextBuff1[2] + STAT_ANIM_PLUS2 - 1;
	gBattleScripting.animArg2 = 0;
}

