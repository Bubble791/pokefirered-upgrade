#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "util.h"
#include "item.h"
#include "random.h"
#include "dynamax.h"
#include "frontier.h"
#include "event_data.h"
#include "mega.h"
#include "set_z_effect.h"
#include "battle_controllers.h"
#include "battle_scripts.h"
#include "battle_ai_script_commands.h"
#include "constants/abilities.h"
#include "constants/battle_ai.h"
#include "constants/battle_move_effects.h"
#include "constants/moves.h"
#include "constants/trainers.h"
#include "constants/species.h"
#include "constants/hold_effects.h"
#include "constants/item_effects.h"
#include "constants/items.h"

enum
{
	AIState_SettingUp,
	AIState_Processing,
	AIState_FinishedProcessing,
	AIState_DoNotProcess,
};

struct SmartWildMons
{
	u16 species;
	u32 aiFlags;
};

#define CAN_SWITCH_OUT(bank) (!IsTrapped(bank, TRUE) && ((IS_SINGLE_BATTLE && ViableMonCountFromBankLoadPartyRange(bank) >= 2) \
													  || (IS_DOUBLE_BATTLE && ViableMonCountFromBankLoadPartyRange(bank) >= 3)))

//Exported Constants
#define AI_THINKING_STRUCT ((struct AI_ThinkingStruct*) (gBattleResources->ai))
#define INCREASE_VIABILITY(x) (IncreaseViability(&viability, x))
#define DECREASE_VIABILITY(x) (DecreaseViability(&viability, x))
#define MOVE_PREDICTION_SWITCH 0xFFFF

#define CHECK_NO_SPECIAL_PROTECTION 0x0
#define CHECK_QUICK_GUARD 0x1
#define CHECK_WIDE_GUARD 0x2
#define CHECK_CRAFTY_SHIELD 0x4
#define CHECK_MAT_BLOCK 0x8


#define DOUBLES_INCREASE_HIT_FOE 4
#define DOUBLES_INCREASE_KO_FOE 2
#define DOUBLES_INCREASE_STRONGEST_MOVE 1

#define DOUBLES_DECREASE_HIT_PARTNER 3
#define DOUBLES_DECREASE_DESTINY_BOND 2

#define BEST_DOUBLES_KO_SCORE (DOUBLES_INCREASE_HIT_FOE + DOUBLES_INCREASE_HIT_FOE + DOUBLES_INCREASE_KO_FOE + DOUBLES_INCREASE_KO_FOE)

#define SWITCHING_INCREASE_KO_FOE 4
#define SWITCHING_INCREASE_RESIST_ALL_MOVES 4
#define SWITCHING_INCREASE_REVENGE_KILL 2 //Can only happen if can KO in the first place
#define SWITCHING_INCREASE_WALLS_FOE 2 //Can only wall if no Super-Effective moves against foe
#define SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE 1
#define SWITCHING_INCREASE_CAN_REMOVE_HAZARDS 10

#define SWITCHING_DECREASE_WEAK_TO_MOVE 1

#define SWITCHING_SCORE_MAX (SWITCHING_INCREASE_KO_FOE + SWITCHING_INCREASE_RESIST_ALL_MOVES + SWITCHING_INCREASE_REVENGE_KILL)

static const struct SmartWildMons sSmartWildAITable[] =
{
	{SPECIES_ARTICUNO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZAPDOS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MOLTRES, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MEWTWO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MEW, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_RAIKOU, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ENTEI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_SUICUNE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LUGIA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_HO_OH, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_CELEBI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_REGIROCK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_REGICE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_REGISTEEL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LATIAS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LATIOS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KYOGRE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GROUDON, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_RAYQUAZA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_JIRACHI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DEOXYS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DEOXYS_ATTACK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DEOXYS_DEFENSE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DEOXYS_SPEED, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_AZELF, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MESPRIT, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_UXIE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DIALGA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_PALKIA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GIRATINA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GIRATINA_ORIGIN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_CRESSELIA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_HEATRAN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_REGIGIGAS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_PHIONE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MANAPHY, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DARKRAI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_SHAYMIN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_SHAYMIN_SKY, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_FIGHT, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_FLYING, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_POISON, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_GROUND, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_ROCK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_BUG, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_GHOST, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_STEEL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_FIRE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_WATER, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_GRASS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_ELECTRIC, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_PSYCHIC, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_ICE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_DRAGON, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_DARK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ARCEUS_FAIRY, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_VICTINI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_COBALION, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TERRAKION, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_VIRIZION, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TORNADUS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TORNADUS_THERIAN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_THUNDURUS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_THUNDURUS_THERIAN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LANDORUS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LANDORUS_THERIAN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_RESHIRAM, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZEKROM, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KYUREM, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KYUREM_BLACK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KYUREM_WHITE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KELDEO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KELDEO_RESOLUTE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MELOETTA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MELOETTA_PIROUETTE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GENESECT, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GENESECT_SHOCK, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GENESECT_BURN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GENESECT_CHILL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GENESECT_DOUSE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_XERNEAS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_YVELTAL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZYGARDE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZYGARDE_10, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_DIANCIE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_HOOPA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_HOOPA_UNBOUND, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_VOLCANION, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TYPE_NULL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_SILVALLY, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TAPU_KOKO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TAPU_LELE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TAPU_BULU, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_TAPU_FINI, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_COSMOG, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_COSMOEM, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_SOLGALEO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_LUNALA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_NIHILEGO, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_BUZZWOLE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_PHEROMOSA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_XURKITREE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_CELESTEELA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KARTANA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_GUZZLORD, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_POIPOLE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_NAGANADEL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_STAKATAKA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_BLACEPHALON, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MAGEARNA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MAGEARNA_P, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MARSHADOW, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_NECROZMA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_NECROZMA_DUSK_MANE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_NECROZMA_DAWN_WINGS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZERAORA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MELTAN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_MELMETAL, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZACIAN, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZACIAN_CROWNED, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZAMAZENTA, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZAMAZENTA_CROWNED, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ETERNATUS, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_ZARUDE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_KUBFU, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_URSHIFU_SINGLE, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_URSHIFU_RAPID, AI_SCRIPT_CHECK_BAD_MOVE},
	{SPECIES_CALYREX, AI_SCRIPT_CHECK_BAD_MOVE},
	{0xFFFF, 0}
};

/*
static u8 ChooseMoveOrAction_Singles(struct AIScript* aiScriptData);
static u8 ChooseMoveOrAction_Doubles(struct AIScript* aiScriptData);
static void BattleAI_DoAIProcessing(struct AIScript* aiScriptData);
static void CalculateAIPredictions(void);
static bool8 ShouldSwitch(void);
static bool8 ShouldSwitchIfOnlyBadMovesLeft(void);
static bool8 FindMonThatAbsorbsOpponentsMove(void);
static bool8 ShouldSwitchIfNaturalCureOrRegenerator(void);
static bool8 PassOnWish(void);
static bool8 SemiInvulnerableTroll(void);
static u8 GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves);
static bool8 RunAllSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves);
static bool8 TheCalcForSemiInvulnerableTroll(u8 bankAtk, u8 flags, bool8 checkLockedMoves);
static bool8 CanStopLockedMove(void);
static bool8 IsYawned(void);
static bool8 IsTakingAnnoyingSecondaryDamage(void);
static bool8 ShouldSwitchToAvoidDeath(void);
static bool8 ShouldSwitchIfWonderGuard(void);
static void CalcMostSuitableMonSwitchIfNecessary(void);
static void PredictMovesForBanks(void);
static void RunCalcShouldAIDynamax(void);
static void UpdateStrongestMoves(void);
static void UpdateBestDoublesKillingMoves(void);
static u32 GetMaxByteIndexInList(const u8 array[], const u32 size);
static u8 GetAI_ItemType(u16 itemId, const u8 *itemEffect); //Fixed from vanilla
static bool8 ShouldAIUseItem(void);*/
static u32 CalcPredictedDamageForCounterMoves(u16 move, u8 bankAtk, u8 bankDef);
static bool8 CalculateMoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits);
//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////ai_master.c//////////////////////////////////////////////

#define NUM_COPY_STATS STAT_SPDEF

void TryRevertTempMegaEvolveBank(u8 bank, struct BattlePokemon* backupMon, u16* backupSpecies, u8* backupAbility)
{
	if (*backupSpecies != SPECIES_NONE)
	{
		struct Pokemon* mon = GetBankPartyData(bank);
		mon->box.species = *backupSpecies;
		CalculateMonStats(mon); //Revert from temp mega
		*GetAbilityLocation(bank) = *backupAbility;
		memcpy(&gBattleMons[bank], backupMon, sizeof(gBattleMons[bank]));
	}
}

void TryTempMegaEvolveBank(u8 bank, struct BattlePokemon* backupMon, u16* backupSpecies, u8* backupAbility)
{
	if (gBattleStruct->ai.megaPotential[bank] != NULL) //Mon will probably mega evolve
	{
		struct Pokemon* mon = GetBankPartyData(bank);
		*backupSpecies = mon->box.species;
		*backupAbility = *GetAbilityLocation(bank);
		memcpy(backupMon, &gBattleMons[bank], sizeof(gBattleMons[bank]));

		mon->box.species = ((struct Evolution*) gBattleStruct->ai.megaPotential[bank])->targetSpecies;
		CalculateMonStats(mon); //Temporarily mega evolve mon
		memcpy(&gBattleMons[bank].attack, &mon->attack, sizeof(u16) * NUM_COPY_STATS);
		*GetAbilityLocation(bank) = GetMonAbility(mon);
		if (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS)
		{
			UpdateTypesForCamomons(bank);
		}
		else
		{
			gBattleMons[bank].type1 = gBaseStats[mon->box.species].type1;
			gBattleMons[bank].type2 = gBaseStats[mon->box.species].type2;
		}
	}
	else
	{
		*backupSpecies = SPECIES_NONE;
		*backupAbility = ABILITY_NONE;
	}
}

/*
static u8 ChooseMoveOrAction_Singles(struct AIScript* aiScriptData)
{
	u8 currentMoveArray[4];
	u8 consideredMoveArray[4];
	u8 numOfBestMoves;
	s32 i;
	u32 flags = AI_THINKING_STRUCT->aiFlags;

	RecordLastUsedMoveByTarget();
	PopulateAIScriptStructWithBaseDefenderData(aiScriptData, gBattlerTarget);

	while (flags != 0)
	{
		if (flags & 1)
		{
			AI_THINKING_STRUCT->aiState = AIState_SettingUp;
			BattleAI_DoAIProcessing(aiScriptData);
		}
		flags >>= 1;
		AI_THINKING_STRUCT->aiLogicId++;
		AI_THINKING_STRUCT->movesetIndex = 0;
	}

	// Check special AI actions.
	if (AI_THINKING_STRUCT->aiAction & AI_ACTION_FLEE)
		return AI_CHOICE_FLEE;
	if (AI_THINKING_STRUCT->aiAction & AI_ACTION_WATCH)
		return AI_CHOICE_WATCH;

	numOfBestMoves = 1;
	currentMoveArray[0] = AI_THINKING_STRUCT->score[0];
	consideredMoveArray[0] = 0;

	for (i = 1; i < MAX_MON_MOVES; i++)
	{
		if (gBattleMons[gBattlerAttacker].moves[i] != MOVE_NONE)
		{
			// In ruby, the order of these if statements is reversed.
			if (currentMoveArray[0] == AI_THINKING_STRUCT->score[i])
			{
				currentMoveArray[numOfBestMoves] = AI_THINKING_STRUCT->score[i];
				consideredMoveArray[numOfBestMoves++] = i;
			}
			if (currentMoveArray[0] < AI_THINKING_STRUCT->score[i])
			{
				numOfBestMoves = 1;
				currentMoveArray[0] = AI_THINKING_STRUCT->score[i];
				consideredMoveArray[0] = i;
			}
		}
	}

	return consideredMoveArray[Random() % numOfBestMoves];
}

static u8 ChooseMoveOrAction_Doubles(struct AIScript* aiScriptData)
{
	s32 i;
	s32 j;
	u32 flags;
	s16 bestMovePointsForTarget[4];
	s8 mostViableTargetsArray[4];
	u8 actionOrMoveIndex[4];
	u8 mostViableMovesScores[4];
	u8 mostViableMovesIndices[4];
	s32 mostViableTargetsNo;
	s32 mostViableMovesNo;
	s16 mostMovePoints;
    bool8 statusMoveOption = FALSE;
    u32 mostDamage;
    struct DamageCalc damageData = {0};

	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
	{
		if (i == gBattlerAttacker || !BATTLER_ALIVE(i))
		{
			actionOrMoveIndex[i] = 0xFF;
			bestMovePointsForTarget[i] = -1;
		}
		else
		{
			BattleAI_SetupAIData(0xF);
			PopulateAIScriptStructWithBaseDefenderData(aiScriptData, gBattlerTarget = i);

			if ((i & BIT_SIDE) != (gBattlerAttacker & BIT_SIDE))
				RecordLastUsedMoveByTarget();

			AI_THINKING_STRUCT->aiLogicId = 0;
			AI_THINKING_STRUCT->movesetIndex = 0;
			flags = AI_THINKING_STRUCT->aiFlags;
			while (flags != 0)
			{
				if (flags & 1)
				{
					AI_THINKING_STRUCT->aiState = AIState_SettingUp;
					BattleAI_DoAIProcessing(aiScriptData);
				}

				flags >>= 1;
				AI_THINKING_STRUCT->aiLogicId++;
				AI_THINKING_STRUCT->movesetIndex = 0;
			}

			if (AI_THINKING_STRUCT->aiAction & AI_ACTION_FLEE)
			{
				actionOrMoveIndex[i] = AI_CHOICE_FLEE;
			}
			else if (AI_THINKING_STRUCT->aiAction & AI_ACTION_WATCH)
			{
				actionOrMoveIndex[i] = AI_CHOICE_WATCH;
			}
			else
			{
				mostViableMovesScores[0] = AI_THINKING_STRUCT->score[0];
				mostViableMovesIndices[0] = 0;
				mostViableMovesNo = 1;
				for (j = 1; j < MAX_MON_MOVES; j++)
				{
					if (gBattleMons[gBattlerAttacker].moves[j] != MOVE_NONE)
					{
						if (mostViableMovesScores[0] == AI_THINKING_STRUCT->score[j])
						{
							mostViableMovesScores[mostViableMovesNo] = AI_THINKING_STRUCT->score[j];
							mostViableMovesIndices[mostViableMovesNo] = j;
							mostViableMovesNo++;
						}
						if (mostViableMovesScores[0] < AI_THINKING_STRUCT->score[j])
						{
							mostViableMovesScores[0] = AI_THINKING_STRUCT->score[j];
							mostViableMovesIndices[0] = j;
							mostViableMovesNo = 1;
						}
					}
				}

				actionOrMoveIndex[i] = mostViableMovesIndices[Random() % mostViableMovesNo];
				bestMovePointsForTarget[i] = mostViableMovesScores[0];

				// Don't use a move against ally if it has less than 101 points.
				if (i == (gBattlerAttacker ^ BIT_FLANK) && bestMovePointsForTarget[i] < 101)
				{
					bestMovePointsForTarget[i] = -1;
				}
			}
		}
	}

	mostMovePoints = bestMovePointsForTarget[0];
	mostViableTargetsArray[0] = 0;
	mostViableTargetsNo = 1;

	mostDamage = (actionOrMoveIndex[0] < MAX_MON_MOVES && SPLIT(actionOrMoveIndex[0]) != SPLIT_STATUS) ?
		GetFinalAIMoveDamage(actionOrMoveIndex[0], gBattlerAttacker, mostViableTargetsArray[0], 1, NULL) : 0;

	damageData.bankAtk = gBattlerAttacker;
	PopulateDamageCalcStructWithBaseAttackerData(&damageData);
	for (i = 1; i < MAX_BATTLERS_COUNT; i++)
	{
		if (bestMovePointsForTarget[i] == mostMovePoints)
		{
			u16 move = actionOrMoveIndex[i];
			u16 pastMove = actionOrMoveIndex[i - 1];
			u32 thisDamage = 0;
			statusMoveOption = move < MAX_MON_MOVES && SPLIT(move) == SPLIT_STATUS;

			if (move < MAX_MON_MOVES //Move was chosen
			&&  pastMove < MAX_MON_MOVES
			&&  SPLIT(move) != SPLIT_STATUS
			&&  SPLIT(pastMove) != SPLIT_STATUS)
			{
				//Choose the target which the most damage can be done to
				damageData.bankDef = i;
				PopulateDamageCalcStructWithBaseDefenderData(&damageData);
				thisDamage = GetFinalAIMoveDamage(move, gBattlerAttacker, i, 1, &damageData);
				if (thisDamage <= mostDamage)
					continue; //Don't store this target if less damage can be done to it

				if (!statusMoveOption)
				{
					mostViableTargetsArray[0] = i;
					mostViableTargetsNo = 1;
					mostDamage = thisDamage;
					continue;
				}
			}

			mostViableTargetsArray[mostViableTargetsNo] = i;
			mostViableTargetsNo++;
		}
		else if (bestMovePointsForTarget[i] > mostMovePoints)
		{
			mostMovePoints = bestMovePointsForTarget[i];
			mostViableTargetsArray[0] = i;
			mostViableTargetsNo = 1;
		}
	}

	gBattlerTarget = mostViableTargetsArray[Random() % mostViableTargetsNo];
	return actionOrMoveIndex[gBattlerTarget];
}

static void BattleAI_DoAIProcessing(struct AIScript* aiScriptData)
{
	while (AI_THINKING_STRUCT->aiState != AIState_FinishedProcessing)
	{
		switch (AI_THINKING_STRUCT->aiState)
		{
			case AIState_DoNotProcess: // Needed to match.
				break;
			case AIState_SettingUp:
				if (gBattleMons[gBattlerAttacker].pp[AI_THINKING_STRUCT->movesetIndex] == 0)
					AI_THINKING_STRUCT->moveConsidered = MOVE_NONE;
				else
					AI_THINKING_STRUCT->moveConsidered = gBattleMons[gBattlerAttacker].moves[AI_THINKING_STRUCT->movesetIndex];

				AI_THINKING_STRUCT->aiState++;
				break;

			case AIState_Processing:
				if (AI_THINKING_STRUCT->moveConsidered != MOVE_NONE
				&&  AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex] > 0)
				{
					if (AI_THINKING_STRUCT->aiLogicId < ARRAY_COUNT(sBattleAIScriptTable) //AI Script exists
					&&  sBattleAIScriptTable[AI_THINKING_STRUCT->aiLogicId] != NULL)
					{
						AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex] =
							sBattleAIScriptTable[AI_THINKING_STRUCT->aiLogicId](gBattlerAttacker,
																				gBattlerTarget,
																				AI_THINKING_STRUCT->moveConsidered,
																				AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex],
																				aiScriptData); //Run AI script
					}
				}
				else
				{
					AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex] = 0;
				}

				AI_THINKING_STRUCT->movesetIndex++;

				if (AI_THINKING_STRUCT->movesetIndex < MAX_MON_MOVES && !(AI_THINKING_STRUCT->aiAction & AI_ACTION_DO_NOT_ATTACK))
					AI_THINKING_STRUCT->aiState = AIState_SettingUp;
				else
					AI_THINKING_STRUCT->aiState++;

			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AI_TrySwitchOrUseItem(void)
{
	struct Pokemon* party;
	u8 battlerIn1, battlerIn2;
	u8 firstId, lastId;
	bool8 ret = FALSE;

	struct BattlePokemon backupMonAtk;
	u8 backupAbilityAtk = ABILITY_NONE;
	u16 backupSpeciesAtk = SPECIES_NONE;

	if (RAID_BATTLE_END)
		goto DONT_THINK;

	//Calulate everything important now to save as much processing time as possible later
	CalculateAIPredictions();

	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		TryTempMegaEvolveBank(gActiveBattler, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);

		if (ShouldSwitch()) //0x8039A80
		{
			if (gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] == PARTY_SIZE)
			{
				u8 monToSwitchId = GetMostSuitableMonToSwitchInto();
				if (monToSwitchId == PARTY_SIZE)
				{
					if (!(IS_DOUBLE_BATTLE))
					{
						battlerIn1 = gActiveBattler;
						battlerIn2 = battlerIn1;
					}
					else
					{
						battlerIn1 = gActiveBattler;
						if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
							battlerIn2 = battlerIn1;
						else
							battlerIn2 = PARTNER(battlerIn1);
					}

					for (monToSwitchId = firstId; monToSwitchId < lastId; ++monToSwitchId)
					{
						if (party[monToSwitchId].hp == 0
						||  GetMonData(&party[monToSwitchId], MON_DATA_IS_EGG, 0)
						||  monToSwitchId == gBattlerPartyIndexes[battlerIn1]
						||	monToSwitchId == gBattlerPartyIndexes[battlerIn2]
						||	monToSwitchId == gBattleStruct->monToSwitchIntoId[battlerIn1]
						||	monToSwitchId == gBattleStruct->monToSwitchIntoId[battlerIn2])
							continue;

						break;
					}
				}

				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = monToSwitchId;
			}

			gBattleStruct->monToSwitchIntoId[gActiveBattler] = gBattleStruct->switchoutIndex[SIDE(gActiveBattler)];
			ret = TRUE;
		}
		else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT)
		{
			//Partner isn't allowed to use items
		}
		else if (ShouldAIUseItem())
			ret = TRUE;

		TryRevertTempMegaEvolveBank(gActiveBattler, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
		if (ret) return;
	}

DONT_THINK:
	//mgba_printf(MGBA_LOG_INFO, "AI thinking complete.");
	BtlController_EmitTwoReturnValues(1, B_ACTION_USE_MOVE, (gActiveBattler ^ BIT_SIDE) << 8);
}

static void CalculateAIPredictions(void)
{
    u8 backupBattler;
    int i;

	if (!gBattleStruct->calculatedAIPredictions) //Only calculate these things once per turn
	{
		//mgba_printf(MGBA_LOG_INFO, "Calculating strongest moves...");
		UpdateStrongestMoves();
		//mgba_printf(MGBA_LOG_WARN, "Calculating doubles killing moves...");
		UpdateBestDoublesKillingMoves(); //Takes long time
		//mgba_printf(MGBA_LOG_INFO, "Predicting moves..");
		PredictMovesForBanks(); //Takes long time
		//mgba_printf(MGBA_LOG_WARN, "Calculating Dynamax mon...");
		RunCalcShouldAIDynamax(); //Allows move predictions to change outcome
		//mgba_printf(MGBA_LOG_INFO, "Calculating switching...");

		gBattleStruct->calculatedAIPredictions = TRUE;

		backupBattler = gActiveBattler;
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (GetBattlerPosition(i) == B_POSITION_PLAYER_LEFT && !(gBattleTypeFlags & BATTLE_TYPE_MOCK_BATTLE))
				continue; //Only calculate for player if player not in control

			if (GetBattlerPosition(i) == B_POSITION_PLAYER_RIGHT && !IsTagBattle())
				continue; //Only calculate for player if player not in control

			if (gBattleStruct->ai.calculatedAISwitchings[i] && BATTLER_ALIVE(i)) //So Multi Battles still work properly
			{
				ResetBestMonToSwitchInto(i);
				gBattleStruct->ai.calculatedAISwitchings[gActiveBattler] = FALSE;

				if (!BankSideHasTwoTrainers(gActiveBattler))
					gBattleStruct->ai.calculatedAISwitchings[PARTNER(gActiveBattler)] = FALSE;
			}
		}
		gActiveBattler = backupBattler;
	}
}

static bool8 ShouldSwitch(void)
{
	u8 battlerIn1, battlerIn2;
	u8 firstId, lastId;
	u8 availableToSwitch;
	struct Pokemon *party;
	int i;

	if (IsTrapped(gActiveBattler, TRUE))
		return FALSE;

	availableToSwitch = 0;
	if (IS_DOUBLE_BATTLE)
	{
		battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(GetBattlerPosition(gActiveBattler) ^ BIT_FLANK)])
			battlerIn2 = gActiveBattler;
		else
			battlerIn2 = GetBattlerAtPosition(GetBattlerPosition(gActiveBattler) ^ BIT_FLANK);
	}
	else
	{
		battlerIn1 = gActiveBattler;
		battlerIn2 = gActiveBattler;
	}

	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	for (i = firstId; i < lastId; ++i)
	{
		if (party[i].hp == 0
		||	GetMonData(&party[i], MON_DATA_SPECIES2, 0) == SPECIES_NONE
		|| 	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
		||	i == gBattlerPartyIndexes[battlerIn1]
		||	i == gBattlerPartyIndexes[battlerIn2]
		||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
		||  i == gBattleStruct->monToSwitchIntoId[battlerIn2])
			continue;

		++availableToSwitch;
	}

	if (availableToSwitch == 0)
		return FALSE;
	if (ShouldSwitchIfOnlyBadMovesLeft())
		return TRUE;
	if (ShouldSwitchIfPerishSong())
		return TRUE;
	if (ShouldSwitchIfWonderGuard())
		return TRUE;
	if (FindMonThatAbsorbsOpponentsMove())
		return TRUE;
	if (ShouldSwitchIfNaturalCureOrRegenerator())
		return TRUE;
	if (PassOnWish())
		return TRUE;
	if (SemiInvulnerableTroll())
		return TRUE;
	if (CanStopLockedMove())
		return TRUE;
	if (IsYawned())
		return TRUE;
	if (IsTakingAnnoyingSecondaryDamage())
		return TRUE;
	if (ShouldSwitchToAvoidDeath())
		return TRUE;
	//if (CanKillAFoe(gActiveBattler))
	//	return FALSE;
	//if (AreStatsRaised())
	//	return FALSE;

	return FALSE;
}*/

void LoadBattlersAndFoes(u8* battlerIn1, u8* battlerIn2, u8* foe1, u8* foe2)
{
	if (IS_DOUBLE_BATTLE)
	{
		*battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
			*battlerIn2 = gActiveBattler;
		else
			*battlerIn2 = PARTNER(gActiveBattler);

		if (gAbsentBattlerFlags & gBitTable[FOE(gActiveBattler)])
			*foe1 = *foe2 = PARTNER(FOE(gActiveBattler));
		else if (gAbsentBattlerFlags & gBitTable[PARTNER(FOE(gActiveBattler))])
			*foe1 = *foe2 = FOE(gActiveBattler);
		else
		{
			*foe1 = FOE(gActiveBattler);
			*foe2 = PARTNER(FOE(gActiveBattler));
		}
	}
	else
	{
		*battlerIn1 = gActiveBattler;
		*battlerIn2 = gActiveBattler;
		*foe1 = FOE(gActiveBattler);
		*foe2 = *foe1;
	}
}
/*
static bool8 PredictedMoveWontDoTooMuchToMon(u8 activeBattler, struct Pokemon* mon, u8 foe)
{
	u16 defMove = IsValidMovePrediction(foe, activeBattler);
	u32 predictedDmg = (defMove == MOVE_NONE) ? 0 : AI_CalcMonDefDmg(foe, activeBattler, defMove, mon, NULL);

	if (predictedDmg == 0)
		return TRUE;

	if (predictedDmg >= mon->hp)
		return FALSE; //Don't switch and sack your other mon

	if (predictedDmg < mon->hp / 2)
		return TRUE; //Can take multiple hits

	return predictedDmg < mon->maxHP / 2
		&& HealingMoveInMonMoveset(mon)
		&& SpeedCalcMon(SIDE(activeBattler), mon) > SpeedCalc(foe); //Has time to heal
}

static bool8 ShouldSwitchIfOnlyBadMovesLeft(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);
	
	if (gBattleStruct->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	if (IS_DOUBLE_BATTLE)
	{
		if ((!BATTLER_ALIVE(foe1) || OnlyBadMovesLeftInMoveset(gActiveBattler, foe1))
		&&  (!BATTLER_ALIVE(foe2) || OnlyBadMovesLeftInMoveset(gActiveBattler, foe2)))
		{
			gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
			BtlController_EmitTwoReturnValues(1, B_ACTION_SWITCH, 0);
			return TRUE;
		}
	}
	else
	{
		if (OnlyBadMovesLeftInMoveset(gActiveBattler, foe1))
		{
			u8 firstId, lastId, bestMon;
			struct Pokemon *party;
			party = LoadPartyRange(gActiveBattler, &firstId, &lastId);
			bestMon = GetMostSuitableMonToSwitchInto();

			if (PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMon], foe1))
			{
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
				BtlController_EmitTwoReturnValues(1, B_ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}

	return FALSE;
}

static bool8 FindMonThatAbsorbsOpponentsMove(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	u16 predictedMove1, predictedMove2;
	u8 absorbingTypeAbility1, absorbingTypeAbility2, absorbingTypeAbility3;
	u8 firstId, lastId;
	struct Pokemon *party;
	int i;
    u8 moveType;
    ability_t atkAbility;

	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	predictedMove1 = IsValidMovePrediction(foe1, gActiveBattler);
	predictedMove2 = IsValidMovePrediction(foe2, gActiveBattler);

	if (IS_SINGLE_BATTLE)
	{
		if (!MoveWouldHitFirst(predictedMove1, foe2, gActiveBattler)) //AI goes first
		{
			if (CanKnockOut(gActiveBattler, foe1))
				return FALSE; //Just KO the opponent and don't worry about switching out
		}
		else
		{
			if (!CanKnockOut(foe1, gActiveBattler) //The enemy can't KO you first
			&&   CanKnockOut(gActiveBattler, foe1)
			&&   AnyStatGreaterThan(gActiveBattler, 6 + 0)) //Has stat boosts +7 or more
				return FALSE; //Just KO the opponent and don't worry about switching out if you're boosted up
		}

		if (!CanKnockOut(foe1, gActiveBattler) //The enemy can't KO you
		&& AnyStatGreaterThan(gActiveBattler, 6 + 1)) //AI is invested in stat boosts +8 or more
			return FALSE;

		if (IS_BEHIND_SUBSTITUTE(gActiveBattler) //Make use of your substitute before switching
		&& !DamagingMoveThaCanBreakThroughSubstituteInMoveset(foe1, gActiveBattler))
			return FALSE;
	}
	else //Double Battle
	{
		u16 bestMove1 = gBattleStruct->ai.bestDoublesKillingMoves[gActiveBattler][foe1];
		u16 bestMove2 = gBattleStruct->ai.bestDoublesKillingMoves[gActiveBattler][foe2];
		if (GetDoubleKillingScore(bestMove1, gActiveBattler, foe1) >= BEST_DOUBLES_KO_SCORE - 2 //10: Hit 2 Foes, KO 1 Foe/Strongest Move 2 Foes
		||  GetDoubleKillingScore(bestMove2, gActiveBattler, foe2) >= BEST_DOUBLES_KO_SCORE - 2) //10: Hit 2 Foes, KO 1 Foe/Strongest Move 2 Foes
			return FALSE; //Don't switch if this mon can do some major damage to the enemy side

		if (AnyStatGreaterThan(gActiveBattler, 6 + 1) //AI is invested in stat boosts +8 or more
		&& (GetDoubleKillingScore(bestMove1, gActiveBattler, foe1) >= BEST_DOUBLES_KO_SCORE / 2 //6: Hit 1 Foe, KO 1 Foe
		 || GetDoubleKillingScore(bestMove2, gActiveBattler, foe2) >= BEST_DOUBLES_KO_SCORE / 2)) //6: Hit 1 Foe, KO 1 Foe
			return FALSE;
	}

	if (STAT_STAGE(gActiveBattler, STAT_STAGE_EVASION) >= 6 + 3)
		return FALSE; //Invested in Evasion so don't switch

	if (((predictedMove1 == MOVE_NONE || predictedMove1 == MOVE_PREDICTION_SWITCH) && (predictedMove2 == MOVE_NONE || predictedMove2 == MOVE_PREDICTION_SWITCH))
	|| (SPLIT(predictedMove1) == SPLIT_STATUS && SPLIT(predictedMove2) == SPLIT_STATUS))
		return FALSE;

	
	if (predictedMove1 != MOVE_NONE && predictedMove1 != MOVE_PREDICTION_SWITCH)
		moveType = GetMoveTypeSpecial(foe1, predictedMove1);
	else
		moveType = GetMoveTypeSpecial(foe2, predictedMove2);

	switch (moveType) {
		case TYPE_FIRE:
			absorbingTypeAbility1 = ABILITY_FLASH_FIRE;
			absorbingTypeAbility2 = ABILITY_FLASH_FIRE;
			absorbingTypeAbility3 = ABILITY_FLASH_FIRE;
			break;
		case TYPE_ELECTRIC:
			absorbingTypeAbility1 = ABILITY_VOLT_ABSORB;
			absorbingTypeAbility2 = ABILITY_LIGHTNING_ROD;
			absorbingTypeAbility3 = ABILITY_MOTORDRIVE;
			break;
		case TYPE_WATER:
			absorbingTypeAbility1 = ABILITY_WATER_ABSORB;
			absorbingTypeAbility2 = ABILITY_DRYSKIN;
			absorbingTypeAbility3 = ABILITY_STORMDRAIN;
			break;
		case TYPE_GRASS:
			absorbingTypeAbility1 = ABILITY_SAPSIPPER;
			absorbingTypeAbility2 = ABILITY_SAPSIPPER;
			absorbingTypeAbility3 = ABILITY_SAPSIPPER;
			break;
		default:
			return FALSE;
	}

	atkAbility = GetPredictedAIAbility(gActiveBattler, foe1);
	if (atkAbility == absorbingTypeAbility1
	||  atkAbility == absorbingTypeAbility2
	||  atkAbility == absorbingTypeAbility3)
		return FALSE;

	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	for (i = firstId; i < lastId; i++)
	{
		u16 species = party[i].box.species;
		u8 monAbility = GetMonAbility(&party[i]);

		if (party[i].hp == 0
		||  species == SPECIES_NONE
		||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
		||	i == gBattlerPartyIndexes[battlerIn1]
		||	i == gBattlerPartyIndexes[battlerIn2]
		||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
		||	i == gBattleStruct->monToSwitchIntoId[battlerIn2])
			continue;

		if (monAbility == absorbingTypeAbility1
		||  monAbility == absorbingTypeAbility2
		||  monAbility == absorbingTypeAbility3)
		{
			// we found a mon.
			gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = i;
			BtlController_EmitTwoReturnValues(1, B_ACTION_SWITCH, 0);
			return TRUE;
		}
	}

	return FALSE;
}

static bool8 ShouldSwitchIfNaturalCureOrRegenerator(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	u16 aiMovePrediction;
	u16 foe1MovePrediction, foe2MovePrediction;

	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	switch (ABILITY(gActiveBattler)) {
		case ABILITY_NATURAL_CURE:
			if (WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 4))
				return FALSE; //Don't switch out if you'll take a lot of damage of switch in

			if (gBattleMons[gActiveBattler].status1 & (STATUS1_SLEEP | STATUS1_FREEZE))
				break;
			if (gBattleMons[gActiveBattler].status1 //Has regular status and over half health
			&& gBattleMons[gActiveBattler].hp >= gBattleMons[gActiveBattler].maxHP / 2
			&& !IsDynamaxed(gActiveBattler))
				break;
			return FALSE;

		//Try switch if less than half health, enemy can kill, and mon can't kill enemy first
		case ABILITY_REGENERATOR:
			if (gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 2
			|| IsDynamaxed(gActiveBattler))
				return FALSE;

			if (WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 3))
				return FALSE; //Don't switch out if you'll lose more than you gain by switching out here

			foe1MovePrediction = IsValidMovePrediction(foe1, gActiveBattler);
			foe2MovePrediction = IsValidMovePrediction(foe2, gActiveBattler);

			if ((BATTLER_ALIVE(foe1) && foe1MovePrediction != MOVE_NONE && MoveKnocksOutXHits(foe1MovePrediction, foe1, gActiveBattler, 1)) //Foe can kill AI
			|| (IsDoubleBattle() && BATTLER_ALIVE(foe2) && foe2MovePrediction != MOVE_NONE && MoveKnocksOutXHits(foe2MovePrediction, foe2, gActiveBattler, 1)))
			{
				if (BATTLER_ALIVE(foe1))
				{
					aiMovePrediction = IsValidMovePrediction(gActiveBattler, foe1);
					if (aiMovePrediction != MOVE_NONE && MoveWouldHitFirst(aiMovePrediction, gActiveBattler, foe1) && MoveKnocksOutXHits(aiMovePrediction, gActiveBattler, foe1, 1))
						return FALSE; //Don't switch if can knock out enemy first or enemy can't kill
					else
						break;
				}

				if (IsDoubleBattle() && BATTLER_ALIVE(foe2))
				{
					aiMovePrediction = IsValidMovePrediction(gActiveBattler, foe2);
					if (aiMovePrediction != MOVE_NONE && MoveWouldHitFirst(aiMovePrediction, gActiveBattler, foe2) && MoveKnocksOutXHits(aiMovePrediction, gActiveBattler, foe2, 1))
						return FALSE; //Don't switch if can knock out enemy first or enemy can't kill
					else
						break;
				}
			}

			return FALSE;

		default:
			return FALSE;
	}

	gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
	BtlController_EmitTwoReturnValues(1, B_ACTION_SWITCH, 0);
	return TRUE;
}

bool8 HasSuperEffectiveMoveAgainstOpponents(bool8 noRng)
{
	u8 opposingBattler;
	int i;
	u8 moveFlags;
	u16 move;

	opposingBattler = FOE(gActiveBattler);
	if (!(gAbsentBattlerFlags & gBitTable[opposingBattler]))
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			move = gBattleMons[gActiveBattler].moves[i];
			if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS)
				continue;

			moveFlags = AI_SpecialTypeCalc(move, gActiveBattler, opposingBattler);

			if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
			{
				if (noRng)
					return TRUE;
				if ((Random() % 10) != 0)
					return TRUE;
			}
		}
	}

	if (!(IS_DOUBLE_BATTLE))
		return FALSE;

	opposingBattler = PARTNER(FOE(gActiveBattler));
	if (!(gAbsentBattlerFlags & gBitTable[opposingBattler]))
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			move = gBattleMons[gActiveBattler].moves[i];
			if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS)
				continue;

			moveFlags = AI_SpecialTypeCalc(move, gActiveBattler, opposingBattler);

			if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
			{
				if (noRng)
					return TRUE;
				if ((Random() % 10) != 0)
					return TRUE;
			}
		}
	}

	return FALSE;
}

static bool8 PassOnWish(void)
{
	pokemon_t* party;
	u8 firstId, lastId;
	u8 bank = gActiveBattler;
	u8 battlerIn1, battlerIn2;
	u8 opposingBattler1, opposingBattler2;
	int i;

	if (IS_DOUBLE_BATTLE)
	{
		opposingBattler1 = FOE(bank);
		opposingBattler2 = PARTNER(opposingBattler1);
		battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
			battlerIn2 = gActiveBattler;
		else
			battlerIn2 = PARTNER(gActiveBattler);
	}
	else
	{
		opposingBattler1 = FOE(bank);
		opposingBattler2 = opposingBattler1;
		battlerIn1 = gActiveBattler;
		battlerIn2 = gActiveBattler;
	}

	if (gWishFutureKnock.wishCounter[bank])
	{
		//Don't switch if your health is less than half and you can survive an opponent's hit
		if (gBattleMons[bank].hp < gBattleMons[bank].maxHP / 2
		&& ((!CanKnockOut(opposingBattler1, bank) && !(IS_DOUBLE_BATTLE && CanKnockOut(opposingBattler2, bank))) || HasProtectionMoveInMoveset(bank, CHECK_MAT_BLOCK)))
			return FALSE;

		party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

		for (i = firstId; i < lastId; ++i)
		{
			if (party[i].hp == 0
			||  party[i].species == SPECIES_NONE
			||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			||	i == gBattlerPartyIndexes[battlerIn1]
			||	i == gBattlerPartyIndexes[battlerIn2]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn2])
				continue;

			if (party[i].hp < party[i].maxHP / 2)
			{
				gBattleStruct->switchoutIndex[SIDE(bank)] = i;
				BtlController_EmitTwoReturnValues(1, B_ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}
	return FALSE;
}

static bool8 SemiInvulnerableTroll(void)
{
	u8 opposingBattler1, opposingBattler2;

	if (IS_DOUBLE_BATTLE)
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = PARTNER(opposingBattler1);
	}
	else
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = opposingBattler1;
	}

	if (IS_DOUBLE_BATTLE)
	{
		if (!(gStatuses3[opposingBattler1] & STATUS3_SEMI_INVULNERABLE) && !(gStatuses3[opposingBattler2] & STATUS3_SEMI_INVULNERABLE))
			return FALSE;
		#ifndef REALLY_SMART_AI
			if (ViableMonCountFromBank(gActiveBattler) > 1)
				return FALSE; //AI doesn't know which mon is being targeted if there's more than one on the field
		#endif
	}
	else if (!(gStatuses3[opposingBattler1] & STATUS3_SEMI_INVULNERABLE))
		return FALSE;

	if (IsDynamaxed(gActiveBattler) && gBattleStruct->dynamaxData.timer[gActiveBattler] > 1) //Going to be Dynamaxed after this turn
		return FALSE;

	if (RunAllSemiInvulnerableLockedMoveCalcs(opposingBattler1, opposingBattler2, FALSE))
		return TRUE;

	return FALSE;
}

static u8 GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves)
{
	u8 option1 = TheCalcForSemiInvulnerableTroll(opposingBattler1, MOVE_RESULT_DOESNT_AFFECT_FOE, checkLockedMoves);
	if (option1 != PARTY_SIZE)
	{
		return option1;
	}

	if (IS_DOUBLE_BATTLE)
	{
		u8 option2 = TheCalcForSemiInvulnerableTroll(opposingBattler2, MOVE_RESULT_DOESNT_AFFECT_FOE, checkLockedMoves);
		if (option2 != PARTY_SIZE)
		{
			return option2;
		}
	}

	u8 option3 = TheCalcForSemiInvulnerableTroll(opposingBattler1, MOVE_RESULT_NOT_VERY_EFFECTIVE, checkLockedMoves);
	if (option3 != PARTY_SIZE)
	{
		return option3;
	}

	if (IS_DOUBLE_BATTLE)
	{
		u8 option4 = TheCalcForSemiInvulnerableTroll(opposingBattler2, MOVE_RESULT_NOT_VERY_EFFECTIVE, checkLockedMoves);
		if (option4 != PARTY_SIZE)
		{
			return option4;
		}
	}

	return PARTY_SIZE;
}

static bool8 RunAllSemiInvulnerableLockedMoveCalcs(u8 opposingBattler1, u8 opposingBattler2, bool8 checkLockedMoves)
{
	u8 option = GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(opposingBattler1, opposingBattler2, checkLockedMoves);
	if (option != PARTY_SIZE)
	{
		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = option;
		BtlController_EmitTwoReturnValues(1, B_ACTION_SWITCH, 0);
	}

	return FALSE;
}

static u8 TheCalcForSemiInvulnerableTroll(u8 bankAtk, u8 flags, bool8 checkLockedMoves)
{
	u8 firstId, lastId;
	u8 battlerIn1, battlerIn2;
	int i;
    pokemon_t* party;

	if (IS_DOUBLE_BATTLE)
	{
		battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
			battlerIn2 = gActiveBattler;
		else
			battlerIn2 = PARTNER(gActiveBattler);
	}
	else
	{
		battlerIn1 = gActiveBattler;
		battlerIn2 = gActiveBattler;
	}

	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	if (((gStatuses3[bankAtk] & STATUS3_SEMI_INVULNERABLE) || (checkLockedMoves && gLockedMoves[bankAtk] != MOVE_NONE))
	&&	gBattleStruct->moveTarget[bankAtk] == gActiveBattler)
	{
		u8 newFlags = AI_SpecialTypeCalc(gLockedMoves[bankAtk], bankAtk, gActiveBattler);
		if (BATTLER_ALIVE(gActiveBattler)) //Not end turn switch
		{
			if (newFlags & flags) //Target already has these type flags so why switch?
				return PARTY_SIZE;
		}

		for (i = firstId; i < lastId; ++i)
		{
			if (party[i].hp == 0
			||  party[i].species == SPECIES_NONE
			||	GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			||	i == gBattlerPartyIndexes[battlerIn1]
			||	i == gBattlerPartyIndexes[battlerIn2]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn1]
			||	i == gBattleStruct->monToSwitchIntoId[battlerIn2])
				continue;

			if (AI_TypeCalc(gLockedMoves[bankAtk], bankAtk, &party[i]) & flags)
			{
				return i;
			}
		}
	}

	return PARTY_SIZE;
}

static bool8 CanStopLockedMove(void)
{
	u8 opposingBattler1, opposingBattler2;

	if (IS_DOUBLE_BATTLE)
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = PARTNER(opposingBattler1);
	}
	else
	{
		opposingBattler1 = FOE(gActiveBattler);
		opposingBattler2 = opposingBattler1;
	}

	if (IS_DOUBLE_BATTLE)
	{
		if (!(gLockedMoves[opposingBattler1] && SPLIT(gLockedMoves[opposingBattler1]) != SPLIT_STATUS) && !(gLockedMoves[opposingBattler2] && SPLIT(gLockedMoves[opposingBattler2]) != SPLIT_STATUS))
			return FALSE;
		#ifndef REALLY_SMART_AI
			if (ViableMonCountFromBank(gActiveBattler) > 1)
				return FALSE; //AI doesn't know which mon is being targeted if there's more than one on the field
		#endif
	}
	else if (!(gLockedMoves[opposingBattler1] && SPLIT(gLockedMoves[opposingBattler1]) != SPLIT_STATUS))
		return FALSE;

	if (IsDynamaxed(gActiveBattler) && gBattleStruct->dynamaxData.timer[gActiveBattler] > 1) //Going to be Dynamaxed after this turn
		return FALSE;

	if (RunAllSemiInvulnerableLockedMoveCalcs(opposingBattler1, opposingBattler2, TRUE))
		return TRUE;

	return FALSE;
}

static bool8 IsYawned(void)
{
    int i;

	if (ABILITY(gActiveBattler) != ABILITY_NATURAL_CURE
	&& gStatuses3[gActiveBattler] & STATUS3_YAWN
	&& CanBePutToSleep(gActiveBattler, FALSE) //Could have been yawned and then afflicted with another status condition
	&& gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 4)
	{
		u8 battlerIn1, battlerIn2;
		u8 foe1, foe2;
        u8 activeSide;

		LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

		//Don't switch if one foe left and you can KO it
		if (ViableMonCountFromBank(foe1) <= 1)
		{
			if (BATTLER_ALIVE(foe1))
			{
				if (CanKnockOut(gActiveBattler, foe1))
					return FALSE;
			}

			if (BATTLER_ALIVE(foe2))
			{
				if (CanKnockOut(gActiveBattler, foe2))
					return FALSE;
			}
		}

		//Don't switch if there's an enemy taking trap damage from this mon
		activeSide = SIDE(gActiveBattler);
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (SIDE(i) != activeSide)
			{
				if (gBattleMons[i].status2 & STATUS2_WRAPPED
				&& ABILITY(i) != ABILITY_MAGICGUARD //Taking trap damage
				&& gBattleStruct->wrappedBy[i] == gActiveBattler)
					return FALSE;
			}
		}

		//Don't switch if invested in Evasion and it'll hold up
		if (STAT_STAGE(gActiveBattler, STAT_STAGE_EVASION) >= 6 + 3
		&& !ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_UNAWARE)
		&& !ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_KEENEYE))
			return FALSE;

		//Don't switch if you can fight through the sleep
		u8 ability = ABILITY(gActiveBattler);
		u8 itemEffect = ITEM_EFFECT(gActiveBattler);
		if (itemEffect == HOLD_EFFECT_CURE_SLP
		|| itemEffect == HOLD_EFFECT_CURE_STATUS
		|| ability == ABILITY_EARLY_BIRD
		|| ability == ABILITY_SHED_SKIN
		|| MoveEffectInMoveset(EFFECT_SNORE, gActiveBattler)
		|| MoveEffectInMoveset(EFFECT_SLEEP_TALK, gActiveBattler)
		|| (CheckGrounding(gActiveBattler)
		 && (MoveInMoveset(MOVE_ELECTRICTERRAIN, gActiveBattler) || MoveInMoveset(MOVE_MISTYTERRAIN, gActiveBattler))))
			return FALSE;

		//Check if can do major damage instead of switching
		if (IS_SINGLE_BATTLE)
		{
			u16 foePrediction = IsValidMovePrediction(foe1, gActiveBattler);

			if (!IsPredictedToSwitch(foe1, gActiveBattler) //Enemy isn't going to switch
			&&  gBattleMoves[foePrediction].effect != EFFECT_PROTECT //Enemy isn't going to protect
			&& (gBattleMoves[foePrediction].effect != EFFECT_SEMI_INVULNERABLE || !MoveWouldHitFirst(foePrediction, foe1, gActiveBattler))) //Enemy isn't going to dodge attack first with Fly etc.
			{
				if (!MoveWouldHitFirst(foePrediction, foe2, gActiveBattler)) //AI goes first
				{
					if (CanKnockOut(gActiveBattler, foe1))
						return FALSE; //You're going to fall asleep, but at least take down the enemy with you
				}
				else
				{
					if (!CanKnockOut(foe1, gActiveBattler) //The enemy can't KO you first
					&&  CanKnockOut(gActiveBattler, foe1))
						return FALSE; //You're going to fall asleep, but at least take down the enemy with you
				}
			}
		}
		else //Double Battle
		{
			u16 bestMove1 = gBattleStruct->ai.bestDoublesKillingMoves[gActiveBattler][foe1];
			u16 bestMove2 = gBattleStruct->ai.bestDoublesKillingMoves[gActiveBattler][foe2];
			if (GetDoubleKillingScore(bestMove1, gActiveBattler, foe1) >= BEST_DOUBLES_KO_SCORE
			||  GetDoubleKillingScore(bestMove2, gActiveBattler, foe2) >= BEST_DOUBLES_KO_SCORE)
				return FALSE; //Don't switch if this mon can do some major damage to the enemy side
		}

		gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
		BtlController_EmitTwoReturnValues(1, B_ACTION_SWITCH, 0);
		return TRUE;
	}
	return FALSE;
}

static bool8 IsTakingAnnoyingSecondaryDamage(void)
{
	if (GetPredictedAIAbility(gActiveBattler, FOE(gActiveBattler)) != ABILITY_MAGICGUARD
	&& !CanKillAFoe(gActiveBattler)
	&& !IsDynamaxed(gActiveBattler)
	&& AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE) //Has smart AI
	{
		if ((gStatuses3[gActiveBattler] & STATUS3_LEECHSEED && (Random() & 3) == 0) //25% chance to switch out when seeded
		|| ((gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP) > 1 && gBattleMons[gActiveBattler].status2 & STATUS2_NIGHTMARE)
		||  gBattleMons[gActiveBattler].status2 & (STATUS2_CURSED)
		||  (gBattleMons[gActiveBattler].status1 & STATUS1_TOXIC_COUNTER) > 0x600) //Been sitting with toxic for 6 turns
		{
			if (!WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 4)) //Don't switch out if you'll take a lot of damage of switch in
			{
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
				BtlController_EmitTwoReturnValues(1, B_ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}

	return FALSE;
}

static bool8 ShouldSwitchToAvoidDeath(void)
{
	if (gBattleStruct->ai.switchingCooldown[gActiveBattler]) //Just switched in
		return FALSE;

	if (IS_SINGLE_BATTLE
	&& AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE) //Has smart AI
	{
		u16 atkMove = IsValidMovePrediction(gActiveBattler, FOE(gActiveBattler));
		u16 defMove = IsValidMovePrediction(FOE(gActiveBattler), gActiveBattler);

		if (defMove != MOVE_NONE //Foe going to attack
		&& (atkMove == MOVE_NONE || !MoveWouldHitFirst(atkMove, gActiveBattler, FOE(gActiveBattler))) //Attacker wouldn't go first
		&& (!IS_BEHIND_SUBSTITUTE(gActiveBattler) || !MoveBlockedBySubstitute(defMove, FOE(gActiveBattler), gActiveBattler))
		&&  MoveKnocksOutXHits(defMove, FOE(gActiveBattler), gActiveBattler, 1) //Foe will kill
		&& !WillTakeSignificantDamageFromEntryHazards(gActiveBattler, 2)) //50% health loss
		{
			u8 firstId, lastId;
			struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, &lastId);
			u8 bestMon = GetMostSuitableMonToSwitchInto();
			u32 resultFlags = AI_TypeCalc(defMove, FOE(gActiveBattler), &party[bestMon]);

			if ((resultFlags & MOVE_RESULT_NO_EFFECT && GetMostSuitableMonToSwitchIntoScore() >= SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE) //Has some sort of followup
			||  (PredictedMoveWontDoTooMuchToMon(gActiveBattler, &party[bestMon], FOE(gActiveBattler)) && GetMostSuitableMonToSwitchIntoScore() >= SWITCHING_INCREASE_WALLS_FOE))
			{
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = PARTY_SIZE;
				BtlController_EmitTwoReturnValues(1, B_ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}

	return FALSE;
}

//Most likely this function won't get used anymore. GetMostSuitableMonToSwitchInto
//now handles all of the switching checks.
bool8 FindMonWithFlagsAndSuperEffective(u8 flags, u8 moduloPercent)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	int i, j;
	u8 start, end;
	u16 move;
	u8 moveFlags;
    pokemon_t* party;

	if (IS_DOUBLE_BATTLE)
	{
		battlerIn1 = gActiveBattler;
		if (gAbsentBattlerFlags & gBitTable[PARTNER(gActiveBattler)])
			battlerIn2 = gActiveBattler;
		else
			battlerIn2 = PARTNER(gActiveBattler);

		if (gAbsentBattlerFlags & gBitTable[FOE(gActiveBattler)])
			foe1 = foe2 = PARTNER(FOE(gActiveBattler));
		else if (gAbsentBattlerFlags & gBitTable[PARTNER(FOE(gActiveBattler))])
			foe1 = foe2 = FOE(gActiveBattler);
		else
		{
			foe1 = FOE(gActiveBattler);
			foe2 = PARTNER(FOE(gActiveBattler));
		}
	}
	else
	{
		battlerIn1 = gActiveBattler;
		battlerIn2 = gActiveBattler;
		foe1 = FOE(gActiveBattler);
		foe2 = foe1;
	}

	u16 predictedMove1 = IsValidMovePrediction(foe1, gActiveBattler);
	u16 predictedMove2 = IsValidMovePrediction(foe2, gActiveBattler);

	if (((predictedMove1 == MOVE_NONE || predictedMove1 == MOVE_PREDICTION_SWITCH) && (predictedMove2 == MOVE_NONE || predictedMove2 == MOVE_PREDICTION_SWITCH))
	|| (SPLIT(predictedMove1) == SPLIT_STATUS && SPLIT(predictedMove2) == SPLIT_STATUS))
		return FALSE;

	party = LoadPartyRange(gActiveBattler, &start, &end);

//Party Search
	for (i = start; i < end; ++i)
	{
		if (party[i].hp == 0
		|| party[i].box.species == SPECIES_NONE
		|| GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
		|| i == gBattlerPartyIndexes[battlerIn1]
		|| i == gBattlerPartyIndexes[battlerIn2]
		|| i == gBattleStruct->monToSwitchIntoId[battlerIn1]
		|| i == gBattleStruct->monToSwitchIntoId[battlerIn2])
			continue;

		if (predictedMove1 != MOVE_NONE && predictedMove1 != MOVE_PREDICTION_SWITCH)
			moveFlags = AI_TypeCalc(predictedMove1, foe1, &party[i]);
		else
			moveFlags = AI_TypeCalc(predictedMove2, foe2, &party[i]);

		if (moveFlags & flags)
		{
			if (predictedMove1 != MOVE_NONE && predictedMove1 != MOVE_PREDICTION_SWITCH)
				battlerIn1 = foe1;
			else
				battlerIn1 = foe2;

			for (j = 0; j < 4; j++) {
				move = party[i].moves[j];
				if (move == 0 || SPLIT(move) == SPLIT_STATUS)
					continue;

				moveFlags = TypeCalc(move, gActiveBattler, battlerIn1, &party[i], TRUE);

				if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE && (Random() % moduloPercent) == 0) {
					gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = i; //There was a GetBattlerPosition here but its a pretty useless function
					BtlController_EmitTwoReturnValues(1, B_ACTION_SWITCH, 0);
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

static bool8 ShouldSwitchIfWonderGuard(void)
{
	u8 battlerIn1;
	u8 opposingBattler;
	u8 moveFlags;
	int i, j;
	u8 start, end;
	opposingBattler = FOE(gActiveBattler);

	if (IS_DOUBLE_BATTLE)
		return FALSE;

	if (ABILITY(opposingBattler) != ABILITY_WONDER_GUARD)
		return FALSE;

	if (WillFaintFromSecondaryDamage(opposingBattler))
		return FALSE;

	//Check if pokemon has a super effective move, Mold Breaker Move, or move that can hurt Shedinja
	u8 bankAtk = gActiveBattler;
	u8 bankDef = opposingBattler;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(gActiveBattler, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CheckTableForMove(move, gMoldBreakerMoves))
				return FALSE;

			if (SPLIT(move) != SPLIT_STATUS)
			{
				u8 atkAbility = GetAIAbility(bankAtk, bankDef, move);
				if (atkAbility == ABILITY_MOLDBREAKER
				||  atkAbility == ABILITY_TERAVOLT
				||  atkAbility == ABILITY_TURBOBLAZE)
					return FALSE;

				moveFlags = AI_SpecialTypeCalc(move, bankAtk, bankDef);
				if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
					return FALSE;
			}
			else if (!MoveBlockedBySubstitute(move, bankAtk, bankDef))
			{
				switch (gBattleMoves[move].effect) {
					case EFFECT_SLEEP:
					case EFFECT_YAWN:
						if (CanBePutToSleep(bankDef, TRUE))
							return FALSE;
						break;
					case EFFECT_ROAR:
						if (BankHasMonToSwitchTo(bankDef))
							return FALSE;
						break;
					case EFFECT_TOXIC:
					case EFFECT_POISON:
					PSN_CHECK:
						if (CanBePoisoned(bankDef, bankAtk, TRUE))
							return FALSE;
						break;
					case EFFECT_WILL_O_WISP:
					BRN_CHECK:
						if (CanBeBurned(bankDef, TRUE))
							return FALSE;
						break;
					case EFFECT_CONFUSE:
					case EFFECT_SWAGGER:
					case EFFECT_FLATTER:
						if (CanBeConfused(bankDef, TRUE))
							return FALSE;
						break;
					case EFFECT_PARALYZE:
						if (CanBeParalyzed(bankDef, TRUE))
							return FALSE;
						break;
					case EFFECT_LEECH_SEED:
						if (!IsOfType(bankDef, TYPE_GRASS))
							return FALSE;
						break;
					case EFFECT_NIGHTMARE:
						if (gBattleMons[bankDef].status1 & STATUS_SLEEP)
							return FALSE;
						break;
					case EFFECT_CURSE:
						if (IsOfType(bankAtk, TYPE_GHOST))
							return FALSE;
						break;
					case EFFECT_SPIKES:
						switch (move) {
							case MOVE_STEALTHROCK:
								if (gSideTimers[SIDE(bankDef)].srAmount == 0)
									return FALSE;
								break;

							case MOVE_TOXICSPIKES:
								if (gSideTimers[SIDE(bankDef)].tspikesAmount < 2)
									return FALSE;
								break;

							case MOVE_STICKYWEB:
								if (gSideTimers[SIDE(bankDef)].stickyWeb == 0)
									return FALSE;
								break;

							default: //Spikes
								if (gSideTimers[SIDE(bankDef)].spikesAmount < 3)
									return FALSE;
								break;
						}
						break;
					case EFFECT_PERISH_SONG:
						if (!(gStatuses3[bankDef] & STATUS3_PERISH_SONG))
							return FALSE;
						break;
					case EFFECT_SANDSTORM:
						if (SandstormHurts(bankDef))
							return FALSE;
						break;
					case EFFECT_HAIL:
						if (HailHurts(bankDef))
							return FALSE;
						break;
					case EFFECT_BATON_PASS:
					case EFFECT_TELEPORT:
						return FALSE;
					case EFFECT_MEMENTO:
						if (SPLIT(move) == SPLIT_STATUS)
							return FALSE;
						break;
					case EFFECT_TRICK:
						if (move == MOVE_TRICK)
						{
							u8 atkSpecies = SPECIES(bankAtk);
							u8 defSpecies = SPECIES(bankDef);

							if (CanTransferItem(atkSpecies, ITEM(bankAtk))
							&& CanTransferItem(atkSpecies, ITEM(bankDef))
							&& CanTransferItem(defSpecies, ITEM(bankAtk))
							&& CanTransferItem(defSpecies, ITEM(bankDef)))
							{
								switch (ITEM_EFFECT(bankAtk)) {
									case HOLD_EFFECT_TOXIC_ORB:
										goto PSN_CHECK;
									case HOLD_EFFECT_FLAME_ORB:
										goto BRN_CHECK;
									case HOLD_EFFECT_BLACK_SLUDGE:
										if (!IsOfType(bankDef, TYPE_POISON))
											return FALSE;
										break;
									case HOLD_EFFECT_STICKY_BARB:
										return FALSE;
								}
							}
						}
						break;
					case EFFECT_WISH:
						if (gWishFutureKnock.wishCounter[bankAtk] == 0)
							return FALSE;
						break;
					case EFFECT_SKILL_SWAP:
						if (move != MOVE_SKILLSWAP)
							return FALSE;
						break;

					case EFFECT_TYPE_CHANGES:
						switch (move) {
							case MOVE_TRICKORTREAT:
								if (!IsOfType(bankDef, TYPE_GHOST))
									return FALSE;
								break;
							case MOVE_FORESTSCURSE:
								if (!IsOfType(bankDef, TYPE_GRASS))
									return FALSE;
								break;
						}
						break;
					case EFFECT_TEAM_EFFECTS:
						switch (move) {
							case MOVE_TAILWIND:
								if (gBattleStruct->TailwindTimers[SIDE(bankAtk)] == 0)
									return FALSE;
								break;
							case MOVE_LUCKYCHANT:
								if (gBattleStruct->LuckyChantTimers[SIDE(bankAtk)] == 0)
									return FALSE;
								break;
						}
						break;
				}
			}
		}
	}


	battlerIn1 = gActiveBattler;
	pokemon_t* party = LoadPartyRange(gActiveBattler, &start, &end);

	//Find a pokemon in the party that has a super effective move
	for (i = start; i < end; ++i)
	{
		if (party[i].hp == 0
			|| party[i].box.species == SPECIES_NONE
			|| GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			|| i == gBattlerPartyIndexes[battlerIn1]
			|| i == gBattleStruct->monToSwitchIntoId[battlerIn1])
			continue;

		for (j = 0; j < MAX_MON_MOVES; j++) {
			u16 move = party[i].moves[j];
			if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS)
				continue;

			moveFlags = TypeCalc(move, gActiveBattler, opposingBattler, &party[i], TRUE);
			if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
			{
				// we found a mon
				gBattleStruct->switchoutIndex[SIDE(gActiveBattler)] = i;
				BtlController_EmitTwoReturnValues(1, B_ACTION_SWITCH, 0);
				return TRUE;
			}
		}
	}

	return FALSE; // at this point there is not a single pokemon in the party that has a super effective move against a pokemon with wonder guard
}


//Add logic about switching in a partner to resist spread move in doubles
u8 GetMostSuitableMonToSwitchInto(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	u8 firstId, lastId;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);
	struct Pokemon* party = LoadPartyRange(gActiveBattler, &firstId, & lastId);

	CalcMostSuitableMonSwitchIfNecessary();
	u8 option1 = gBattleStruct->ai.bestMonIdToSwitchInto[gActiveBattler][0];
	u8 option2 = gBattleStruct->ai.bestMonIdToSwitchInto[gActiveBattler][1];

	if (option1 == PARTY_SIZE
	||  GetMonData(&party[option1], MON_DATA_HP, NULL) == 0) //Best mon is dead
	{
		CalcMostSuitableMonToSwitchInto();
		gBattleStruct->ai.calculatedAISwitchings[gActiveBattler] = TRUE;

		if (!BankSideHasTwoTrainers(gActiveBattler))
			gBattleStruct->ai.calculatedAISwitchings[PARTNER(gActiveBattler)] = TRUE;
	}

	if (option1 == gBattleStruct->monToSwitchIntoId[battlerIn1]
	||  option1 == gBattleStruct->monToSwitchIntoId[battlerIn2])
		return option2;

	return option1;
}

s8 GetMostSuitableMonToSwitchIntoScore(void)
{
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);

	CalcMostSuitableMonSwitchIfNecessary();
	u8 option1 = gBattleStruct->ai.bestMonIdToSwitchInto[gActiveBattler][0];

	if (option1 == gBattleStruct->monToSwitchIntoId[battlerIn1]
	||  option1 == gBattleStruct->monToSwitchIntoId[battlerIn2])
		return gBattleStruct->ai.bestMonIdToSwitchIntoScores[gActiveBattler][1];

	return gBattleStruct->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0];
}

static void CalcMostSuitableMonSwitchIfNecessary(void)
{
	//Calc switching only if necessary
	if (!gBattleStruct->ai.calculatedAISwitchings[gActiveBattler] && BATTLER_ALIVE(gActiveBattler)) //So Multi Battles still work properly
	{
		CalcMostSuitableMonToSwitchInto();
		gBattleStruct->ai.calculatedAISwitchings[gActiveBattler] = TRUE;

		if (!BankSideHasTwoTrainers(gActiveBattler))
			gBattleStruct->ai.calculatedAISwitchings[PARTNER(gActiveBattler)] = TRUE;
	}
}

u8 CalcMostSuitableMonToSwitchInto(void)
{
	//u8 opposingBattler;
	u32 i, j, k;
	u8 bestMonId, secondBestMonId;
	u8 battlerIn1, battlerIn2;
	u8 foe, foe1, foe2;
	u8 firstId, lastId; // + 1
	u16 move;
	struct Pokemon* party;

	u8 lastValidMon = PARTY_SIZE;
	u8 secondLastValidMon = PARTY_SIZE;
	s8 scores[PARTY_SIZE] = {0};
	bool8 canNegateToxicSpikes[PARTY_SIZE] = {FALSE};
	bool8 canRemoveHazards[PARTY_SIZE] = {FALSE};

	if (gBattleStruct->monToSwitchIntoId[gActiveBattler] != PARTY_SIZE)
		return gBattleStruct->monToSwitchIntoId[gActiveBattler];

	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);
	party = LoadPartyRange(gActiveBattler, &firstId, & lastId);

	//Check if point even running calcs
	u8 count = 0;
	for (i = firstId; i < lastId; ++i)
	{
		if (party[i].box.species != SPECIES_NONE
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
		&& party[i].hp > 0
		&& gBattlerPartyIndexes[battlerIn1] != i
		&& gBattlerPartyIndexes[battlerIn2] != i
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn1]
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn2])
		{
			++count;
		}
	}

	if (count == 0)
	{
		ResetBestMonToSwitchInto(gActiveBattler);
		return PARTY_SIZE;
	}

	//Try to counter a locked move
	//u8 option = GetBestPartyNumberForSemiInvulnerableLockedMoveCalcs(opposingBattler, opposingBattler, TRUE);
	//if (option != PARTY_SIZE)
	//	return option;

	//Find the mon who is most suitable
	bestMonId = PARTY_SIZE;
	secondBestMonId = PARTY_SIZE;
	for (i = firstId; i < lastId; ++i)
	{
		if (party[i].species != SPECIES_NONE
		&& party[i].hp > 0
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
		&& i != gBattlerPartyIndexes[battlerIn1]
		&& i != gBattlerPartyIndexes[battlerIn2]
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn1]
		&& i != gBattleStruct->monToSwitchIntoId[battlerIn2])
		{
			u8 foes[] = {foe1, foe2};
			u8 moveLimitations = CheckMoveLimitationsFromParty(&party[i], 0, 0xFF);
			secondLastValidMon = lastValidMon;
			lastValidMon = i;

			u16 species = GetMonData(&party[i], MON_DATA_SPECIES, NULL);
			canNegateToxicSpikes[i] = CheckGroundingFromPartyData(&party[i])
									&& (gBaseStats[species].type1 == TYPE_POISON || gBaseStats[species].type2 == TYPE_POISON);

			if (WillFaintFromEntryHazards(&party[i], SIDE(gActiveBattler)))
				continue; //Don't switch in the mon if it'll faint on reentry

			struct DamageCalc damageData = {0};
			damageData.bankAtk = gActiveBattler;
			damageData.monAtk = &party[i];
			PopulateDamageCalcStructWithBaseAttackerData(&damageData);

			for (j = 0; j < gBattlersCount / 2; ++j) //Loop through all enemies on field
			{
				foe = foes[j];

				if (BATTLER_ALIVE(foe)
				&& (j == 0 || foes[0] != foes[j])) //Don't check same opponent twice
				{
					u8 typeEffectiveness = 0;
					u8 ability = GetMonAbility(&party[i]);
					bool8 isWeakToMove = FALSE;
					bool8 isNormalEffectiveness = FALSE;

					damageData.bankDef = foe;
					PopulateDamageCalcStructWithBaseDefenderData(&damageData);

					//Check Offensive Capabilities
					if (CanKnockOutFromParty(&party[i], foe, &damageData))
					{
						scores[i] += SWITCHING_INCREASE_KO_FOE;

						if (ability == ABILITY_MOXIE
						||  ability == ABILITY_SOULHEART
						||  ability == ABILITY_BEASTBOOST)
							scores[i] += SWITCHING_INCREASE_REVENGE_KILL;
						else
						{
							for (k = 0; k < MAX_MON_MOVES; ++k)
							{
								move = GetMonData(&party[i], MON_DATA_MOVE1 + k, 0);

								if (gBattleMoves[move].effect == EFFECT_RAPID_SPIN //Includes Defog
								&&  gSideStatuses[SIDE(gActiveBattler)] & SIDE_STATUS_SPIKES)
								{
									if (IS_SINGLE_BATTLE) //Single Battle
										canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 2; //There's a point in removing the hazards
									else //Double Battle
										canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 3; //There's a point in removing the hazards
								}

								if (move == MOVE_FELLSTINGER
								&&  !(gBitTable[k] & moveLimitations))
								{
									if (MoveKnocksOutXHitsFromParty(move, &party[i], foe, 1, &damageData))
									{
										scores[i] += SWITCHING_INCREASE_REVENGE_KILL;
										break;
									}
								}
								else if (SPLIT(move) != SPLIT_STATUS
								&& PriorityCalcMon(&party[i], move) > 0
								&& MoveKnocksOutXHitsFromParty(move, &party[i], foe, 1, &damageData))
								{
									//Priority move that KOs
									scores[i] += SWITCHING_INCREASE_REVENGE_KILL;
									break;
								}
							}
						}
					}
					else //This mon can't KO the foe
					{
						for (k = 0; k < MAX_MON_MOVES; ++k)
						{
							move = GetMonData(&party[i], MON_DATA_MOVE1 + k, 0);

							if (gBattleMoves[move].effect == EFFECT_RAPID_SPIN //Includes Defog
							&&  gSideStatuses[SIDE(gActiveBattler)] & SIDE_STATUS_SPIKES)
							{
								if (!IS_DOUBLE_BATTLE) //Single Battle
									canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 2; //There's a point in removing the hazards
								else //Double Battle
									canRemoveHazards[i] = ViableMonCountFromBank(gActiveBattler) >= 3; //There's a point in removing the hazards
							}

							if (move != MOVE_NONE
							&& SPLIT(move) != SPLIT_STATUS
							&& !(gBitTable[k] & moveLimitations)
							&& TypeCalc(move, gActiveBattler, foe, &party[i], TRUE) & MOVE_RESULT_SUPER_EFFECTIVE)
							{
								scores[i] += SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE; //Only checked if can't KO
								break;
							}
						}
					}

					//Check Defensive Capabilities
					bool8 physMoveInMoveset = FALSE;
					bool8 specMoveInMoveset = FALSE;
					u8 foeMoveLimitations = CheckMoveLimitations(foe, 0, 0xFF);
					struct DamageCalc foeDamageData = {0};
					foeDamageData.bankAtk = foe;
					foeDamageData.bankDef = gActiveBattler; //For the side
					foeDamageData.monDef = &party[i];
					PopulateDamageCalcStructWithBaseAttackerData(&foeDamageData);
					PopulateDamageCalcStructWithBaseDefenderData(&foeDamageData);

					for (k = 0; k < MAX_MON_MOVES; ++k)
					{
						move = GetBattleMonMove(foe, k);

						if (move == MOVE_NONE)
							break;

						if (gBattleMons[foe].status2 & STATUS2_MULTIPLETURNS
						&&  MoveInMoveset(gLockedMoves[foe], foe)
						&&  move != gLockedMoves[foe])
							continue; //Skip non-locked moves

						u8 split = CalcMoveSplit(foe, move);
						if (split == SPLIT_PHYSICAL)
							physMoveInMoveset = TRUE;
						else if (split == SPLIT_SPECIAL)
							specMoveInMoveset = TRUE;
						else
							continue; //Skip status moves

						if (!(gBitTable[k] & foeMoveLimitations))
						{
							typeEffectiveness = AI_TypeCalc(move, foe, &party[i]);

							if (typeEffectiveness & MOVE_RESULT_SUPER_EFFECTIVE)
							{
								isWeakToMove = TRUE;
								break; //Only need 1 check for this to pass
							}
							else if (typeEffectiveness & MOVE_RESULT_NOT_VERY_EFFECTIVE)
							{
								if (!isNormalEffectiveness && IS_SINGLE_BATTLE) //Only need 1 check to pass and don't waste extra time in doubles
								{
									//This function takes time for each move for each Pokemon so we try to call it as little as possible
									u32 dmg = AI_CalcMonDefDmg(foe, gActiveBattler, move, &party[i], &foeDamageData);
									if (dmg >= party[i].hp / 2) //Move does half of over half of the health remaining
										isNormalEffectiveness = TRUE;
								}
							}
							else if (typeEffectiveness == 0)
								isNormalEffectiveness = TRUE;
							//By default it either resists or is immune
						}
					}

					if (isWeakToMove)
						scores[i] -= SWITCHING_DECREASE_WEAK_TO_MOVE;
					else if (!isNormalEffectiveness) //Foe has no Super-Effective or Normal-Effectiveness moves
						scores[i] += SWITCHING_INCREASE_RESIST_ALL_MOVES;
					else
					{
						bool8 cantWall = FALSE;
						u32 attack, spAttack;
						APPLY_STAT_MOD(attack, &gBattleMons[foe], gBattleMons[foe].attack, STAT_STAGE_ATK);
						APPLY_STAT_MOD(spAttack, &gBattleMons[foe], gBattleMons[foe].spAttack, STAT_STAGE_SPATK);

						if (physMoveInMoveset && GetMonData(&party[i], MON_DATA_DEF, NULL) <= attack)
							cantWall = TRUE;
						else if (specMoveInMoveset && GetMonData(&party[i], MON_DATA_SPDEF, NULL) <= spAttack)
							cantWall = TRUE;

						if (!cantWall)
							scores[i] += SWITCHING_INCREASE_WALLS_FOE;
					}
				}
			}

			if (!IS_DOUBLE_BATTLE || ViableMonCountFromBank(foe1) == 1) //Single Battle or 1 target left
			{
				if (scores[i] >= SWITCHING_SCORE_MAX && canRemoveHazards[i]) //This mon is perfect
				{
					if (IS_SINGLE_BATTLE)
					{
						bestMonId = i;
						break;
					}
					else //Double Battle
					{
						if (bestMonId == PARTY_SIZE)
							bestMonId = i;
						else
						{
							secondBestMonId = i;
							break;
						}
					}
				}
			}
			else //Double Battle
			{
				if (scores[i] >= SWITCHING_SCORE_MAX * 2 && canRemoveHazards[i]) //This mon is perfect
				{
					if (IS_SINGLE_BATTLE)
					{
						bestMonId = i;
						break;
					}
					else //Double Battle
					{
						if (bestMonId == PARTY_SIZE)
							bestMonId = i;
						else
						{
							secondBestMonId = i;
							break;
						}
					}
				}
			}

			if (bestMonId == PARTY_SIZE)
				bestMonId = i;
			else if (scores[i] > scores[bestMonId])
			{
				if (IS_DOUBLE_BATTLE)
					secondBestMonId = bestMonId;

				bestMonId = i;
			}
			else if (IS_DOUBLE_BATTLE && secondBestMonId == PARTY_SIZE)
				secondBestMonId = i;
		}
	}

	if (bestMonId != PARTY_SIZE)
	{
		if (scores[bestMonId] < 8)
		{
			bool8 tSpikesActive = gSideTimers[SIDE(gActiveBattler)].tspikesAmount > 0;
			for (i = firstId; i < lastId; ++i)
			{
				if (canRemoveHazards[i]
				|| (tSpikesActive && canNegateToxicSpikes[i]))
				{
					if (IS_DOUBLE_BATTLE)
					{
						gBattleStruct->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = SWITCHING_INCREASE_CAN_REMOVE_HAZARDS * 2;

						if (!BankSideHasTwoTrainers(gActiveBattler))
							gBattleStruct->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = SWITCHING_INCREASE_CAN_REMOVE_HAZARDS * 2;
					}
					else
						gBattleStruct->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = SWITCHING_INCREASE_CAN_REMOVE_HAZARDS;

					gBattleStruct->ai.bestMonIdToSwitchInto[gActiveBattler][0] = i;
					if (!BankSideHasTwoTrainers(gActiveBattler))
						gBattleStruct->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = i;

					if (IS_SINGLE_BATTLE)
						return i; //Just send out the Pokemon that can remove the hazards
					else
					{
						secondBestMonId = bestMonId;
						bestMonId = i;
						break;
					}
				}
			}
		}

		if (IS_DOUBLE_BATTLE)
		{
			gBattleStruct->ai.bestMonIdToSwitchInto[gActiveBattler][0] = bestMonId;
			gBattleStruct->ai.bestMonIdToSwitchInto[gActiveBattler][1] = secondBestMonId;
			gBattleStruct->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = scores[bestMonId];
			gBattleStruct->ai.bestMonIdToSwitchIntoScores[gActiveBattler][1] = scores[secondBestMonId];

			if (!BankSideHasTwoTrainers(gActiveBattler)) //Store data for second mon too
			{
				gBattleStruct->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = bestMonId;
				gBattleStruct->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][1] = secondBestMonId;
				gBattleStruct->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = scores[bestMonId];
				gBattleStruct->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][1] = scores[secondBestMonId];
			}
		}
		else
		{
			gBattleStruct->ai.bestMonIdToSwitchInto[gActiveBattler][0] = bestMonId;
			gBattleStruct->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = scores[bestMonId];
		}

		return bestMonId;
	}

	//If for some reason we weren't able to find a mon above,
	//pick the last checked available mon now.
	gBattleStruct->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = 0;
	gBattleStruct->ai.bestMonIdToSwitchInto[gActiveBattler][0] = lastValidMon;
	gBattleStruct->ai.bestMonIdToSwitchIntoScores[gActiveBattler][0] = 0;
	gBattleStruct->ai.bestMonIdToSwitchInto[gActiveBattler][0] = secondLastValidMon;

	if (!BankSideHasTwoTrainers(gActiveBattler)) //Store data for second mon too
	{
		gBattleStruct->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = 0;
		gBattleStruct->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = lastValidMon;
		gBattleStruct->ai.bestMonIdToSwitchIntoScores[PARTNER(gActiveBattler)][0] = 0;
		gBattleStruct->ai.bestMonIdToSwitchInto[PARTNER(gActiveBattler)][0] = secondLastValidMon;
	}

	return lastValidMon; //If lastValidMon is still PARTY_SIZE when reaching here, there is a bigger problem
}

void ResetBestMonToSwitchInto(u8 bank)
{
	gBattleStruct->ai.bestMonIdToSwitchIntoScores[bank][0] = 0;
	gBattleStruct->ai.bestMonIdToSwitchInto[bank][0] = PARTY_SIZE;
	gBattleStruct->ai.bestMonIdToSwitchIntoScores[bank][1] = 0;
	gBattleStruct->ai.bestMonIdToSwitchInto[bank][1] = PARTY_SIZE;

	if (!BankSideHasTwoTrainers(bank)) //Store data for second mon too
	{
		gBattleStruct->ai.bestMonIdToSwitchIntoScores[PARTNER(bank)][0] = 0;
		gBattleStruct->ai.bestMonIdToSwitchInto[PARTNER(bank)][0] = PARTY_SIZE;
		gBattleStruct->ai.bestMonIdToSwitchIntoScores[PARTNER(bank)][1] = 0;
		gBattleStruct->ai.bestMonIdToSwitchInto[PARTNER(bank)][1] = PARTY_SIZE;
	}
}

void RemoveBestMonToSwitchInto(u8 bank)
{
	//secondBestMonId -> bestMonId
	gBattleStruct->ai.bestMonIdToSwitchIntoScores[bank][0] = gBattleStruct->ai.bestMonIdToSwitchIntoScores[bank][1];
	gBattleStruct->ai.bestMonIdToSwitchInto[bank][0] = gBattleStruct->ai.bestMonIdToSwitchInto[bank][1];

	gBattleStruct->ai.bestMonIdToSwitchIntoScores[bank][1] = 0;
	gBattleStruct->ai.bestMonIdToSwitchInto[bank][1] = PARTY_SIZE;

	if (!BankSideHasTwoTrainers(bank)) //Store data for second mon too
	{
		gBattleStruct->ai.bestMonIdToSwitchIntoScores[PARTNER(bank)][0] = gBattleStruct->ai.bestMonIdToSwitchIntoScores[PARTNER(bank)][1];
		gBattleStruct->ai.bestMonIdToSwitchInto[PARTNER(bank)][0] = gBattleStruct->ai.bestMonIdToSwitchInto[PARTNER(bank)][1];

		gBattleStruct->ai.bestMonIdToSwitchIntoScores[PARTNER(bank)][1] = 0;
		gBattleStruct->ai.bestMonIdToSwitchInto[PARTNER(bank)][1] = PARTY_SIZE;
	}
}
*/
u32 WildMonIsSmart(u8 bank)
{
	#ifdef WILD_ALWAYS_SMART
		return TRUE;
	#else
        u32 i;
		u16 species = SPECIES(bank);

		if (IsMegaSpecies(species)
		|| IsRedPrimalSpecies(species)
		|| IsBluePrimalSpecies(species)
		|| IsUltraNecrozmaSpecies(species))
			return AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_SEMI_SMART;

		for (i = 0; sSmartWildAITable[i].species != 0xFFFF; ++i)
		{
			if (sSmartWildAITable[i].species == species)
				return sSmartWildAITable[i].aiFlags;
		}

		return FALSE;
	#endif
}
/*
void PopulateAIScriptStructWithBaseAttackerData(struct AIScript* data, u8 bankAtk)
{
	data->atkSpecies = SPECIES(bankAtk);
	data->atkItem = ITEM(bankAtk);
	data->atkItemEffect = ITEM_EFFECT(bankAtk);
	data->atkItemQuality = ITEM_QUALITY(bankAtk);

	data->atkStatus1 = gBattleMons[bankAtk].status1;
	data->atkStatus2 = gBattleMons[bankAtk].status2;
	data->atkStatus3 = gStatuses3[bankAtk];
	data->atkGender = GetGenderFromSpeciesAndPersonality(data->atkSpecies, gBattleMons[bankAtk].personality);

	data->atkAttack = gBattleMons[bankAtk].attack;
	data->atkDefense = gBattleMons[bankAtk].defense;
	data->atkSpeed = gBattleMons[bankAtk].speed;
	data->atkSpAtk = gBattleMons[bankAtk].spAttack;
	data->atkSpDef = gBattleMons[bankAtk].spDefense;

	data->bankAtkPartner = (IS_DOUBLE_BATTLE) ? PARTNER(bankAtk) : bankAtk;
	data->atkPartnerAbility = (IS_DOUBLE_BATTLE) ? ABILITY(data->bankAtkPartner) : ABILITY_NONE;

	//Load Alternative targets
	data->foe1 = FOE(bankAtk);
	data->foe2 = (IS_DOUBLE_BATTLE) ? PARTNER(data->foe1) : data->foe1;
}

void PopulateAIScriptStructWithBaseDefenderData(struct AIScript* data, u8 bankDef)
{
	data->defSpecies = SPECIES(bankDef);
	data->defItem = ITEM(bankDef);
	data->defItemEffect = ITEM_EFFECT(bankDef);
	data->defItemQuality = ITEM_QUALITY(bankDef);

	data->defStatus1 = gBattleMons[bankDef].status1;
	data->defStatus2 = gBattleMons[bankDef].status2;
	data->defStatus3 = gStatuses3[bankDef];
	data->defGender = GetGenderFromSpeciesAndPersonality(data->defSpecies, gBattleMons[bankDef].personality);

	data->defAttack = gBattleMons[bankDef].attack;
	data->defDefense = gBattleMons[bankDef].defense;
	data->defSpeed = gBattleMons[bankDef].speed;
	data->defSpAtk = gBattleMons[bankDef].spAttack;
	data->defSpDef = gBattleMons[bankDef].spDefense;

	data->bankDefPartner = (IS_DOUBLE_BATTLE) ? PARTNER(bankDef) : bankDef;
	data->defPartnerAbility = (IS_DOUBLE_BATTLE) ? ABILITY(data->bankDefPartner) : ABILITY_NONE;

	data->partnerMove = MOVE_NONE;
	if (!IsBankIncapacitated(data->bankAtkPartner))
		data->partnerMove = GetAIChosenMove(data->bankAtkPartner, bankDef);

	data->partnerHandling = IS_DOUBLE_BATTLE
		&& BATTLER_ALIVE(data->bankAtkPartner)
		&& !IsBankIncapacitated(data->bankAtkPartner)
		&& gChosenMoveByBattler[data->bankAtkPartner] != MOVE_NONE //Partner actually selected a move
		&& gBattleStruct->moveTarget[data->bankAtkPartner] == bankDef
		&& gBattleMoves[data->partnerMove].target & MOVE_TARGET_SELECTED //Partner isn't using spread move
		&& CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, 0, bankDef) >= 2 //With one target left, both Pokemon should aim for the same target
		&& MoveKnocksOutXHits(data->partnerMove, data->bankAtkPartner, gBattleStruct->moveTarget[data->bankAtkPartner], 1);
}

static void PredictMovesForBanks(void)
{
	int i, j;
	u8 viabilities[MAX_MON_MOVES] = {0};
	u8 bestMoves[MAX_MON_MOVES] = {0};
	struct AIScript aiScriptData = {0};
    u8 bankAtk;

	Memset(gBattleStruct->ai.movePredictions, 0, sizeof(gBattleStruct->ai.movePredictions)); //Clear old predictions

	for (bankAtk = 0; bankAtk < gBattlersCount; ++bankAtk)
	{
		if (BATTLER_ALIVE(bankAtk))
		{
            u8 bankDef;
			u32 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF); //Don't predict Dynamax
			PopulateAIScriptStructWithBaseAttackerData(&aiScriptData, bankAtk);

			for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
			{
				if (bankAtk == bankDef || !BATTLER_ALIVE(bankDef)) continue;

				if (gBattleMons[bankAtk].status2 & STATUS2_RECHARGE
				||  gDisableStructs[bankAtk].truantCounter != 0)
				{
					StoreMovePrediction(bankAtk, bankDef, MOVE_NONE);
				}
				else if (IsBankAsleep(bankAtk)
				&& !MoveEffectInMoveset(EFFECT_SLEEP_TALK, bankAtk) && !MoveEffectInMoveset(EFFECT_SNORE, bankAtk)) //Can't get around sleep
				{
					StoreMovePrediction(bankAtk, bankDef, MOVE_NONE);
				}
				else if (gBattleMons[bankAtk].status2 & STATUS2_MULTIPLETURNS
				&& MoveInMoveset(gLockedMoves[bankAtk], bankAtk)) //Still knows locked move
				{
					StoreMovePrediction(bankAtk, bankDef, gLockedMoves[bankAtk]);
				}
				else
				{
					u32 backupFlags = AI_THINKING_STRUCT->aiFlags; //Backup flags so killing in negatives is ignored
					AI_THINKING_STRUCT->aiFlags = 7;
					PopulateAIScriptStructWithBaseDefenderData(&aiScriptData, bankDef);

					for (i = 0; i < MAX_MON_MOVES && gBattleMons[bankAtk].moves[i] != MOVE_NONE; ++i)
					{
						viabilities[i] = 0;
						bestMoves[i] = 0;

						if (gBitTable[i] & moveLimitations) continue;

						u16 move = gBattleMons[bankAtk].moves[i];
						move = TryReplaceMoveWithZMove(bankAtk, bankDef, move);
						viabilities[i] = AIScript_Negatives(bankAtk, bankDef, move, 100, &aiScriptData);
						viabilities[i] = AIScript_Positives(bankAtk, bankDef, move, viabilities[i], &aiScriptData);
					}

					AI_THINKING_STRUCT->aiFlags = backupFlags;

					bestMoves[j = 0] = GetMaxByteIndexInList(viabilities, MAX_MON_MOVES) + 1;
					for (i = 0; i < MAX_MON_MOVES; ++i)
					{
						if (i + 1 != bestMoves[0] //i is not the index returned from GetMaxByteIndexInList
						&& viabilities[i] == viabilities[bestMoves[j] - 1])
							bestMoves[++j] = i + 1;
					}

					if (viabilities[GetMaxByteIndexInList(viabilities, MAX_MON_MOVES)] < 100) //Best move has viability < 100
						StoreSwitchPrediction(bankAtk, bankDef);
					else
						StoreMovePrediction(bankAtk, bankDef, gBattleMons[bankAtk].moves[bestMoves[Random() % (j + 1)] - 1]);

					Memset(viabilities, 0, sizeof(viabilities));
				}
			}
		}
	}
}

static void UpdateStrongestMoves(void)
{
	u8 bankAtk, bankDef;

	for (bankAtk = 0; bankAtk < gBattlersCount; ++bankAtk)
	{
		struct BattlePokemon backupMonAtk;
		u8 backupAbilityAtk = ABILITY_NONE;
		u16 backupSpeciesAtk = SPECIES_NONE;

		if (!IS_TRANSFORMED(bankAtk)
		&& !BankMegaEvolved(bankAtk, FALSE)
		&&  MegaEvolutionEnabled(bankAtk)
		&& !DoesZMoveUsageStopMegaEvolution(bankAtk))
		{
			gBattleStruct->ai.megaPotential[bankAtk] = CanMegaEvolve(bankAtk, FALSE);

			if (gBattleStruct->ai.megaPotential[bankAtk] == NULL)
				gBattleStruct->ai.megaPotential[bankAtk] = CanMegaEvolve(bankAtk, TRUE); //Check Ultra Burst
		}

		if (IS_SINGLE_BATTLE) //There's a high chance these values will be used in singles so calc now.
		{
			TryTempMegaEvolveBank(bankAtk, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);

			for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
			{
				if (bankAtk == bankDef || bankDef == PARTNER(bankAtk))
					continue; //Don't bother calculating for these Pokemon. Never used

				struct BattlePokemon backupMonDef;
				u8 backupAbilityDef = ABILITY_NONE;
				u16 backupSpeciesDef = SPECIES_NONE;
				TryTempMegaEvolveBank(bankDef, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);

				gBattleStruct->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);
				gBattleStruct->ai.canKnockOut[bankAtk][bankDef] = MoveKnocksOutXHits(gBattleStruct->ai.strongestMove[bankAtk][bankDef], bankAtk, bankDef, 1);
				gBattleStruct->ai.can2HKO[bankAtk][bankDef] = (gBattleStruct->ai.canKnockOut[bankAtk][bankDef]) ? TRUE
													  : MoveKnocksOutXHits(gBattleStruct->ai.strongestMove[bankAtk][bankDef], bankAtk, bankDef, 2); //If you can KO in 1 hit you can KO in 2

				TryRevertTempMegaEvolveBank(bankDef, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);
			}

			TryRevertTempMegaEvolveBank(bankAtk, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
		}
	}
}

static void UpdateBestDoublesKillingMoves(void)
{
	if (IS_DOUBLE_BATTLE)
	{
		u8 bankAtk, bankDef;

		for (bankAtk = 0; bankAtk < gBattlersCount; ++bankAtk)
		{
			//mgba_printf(MGBA_LOG_INFO, "");
			for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
			{
				if (bankAtk == bankDef || bankDef == PARTNER(bankAtk) || !BATTLER_ALIVE(bankDef))
					continue; //Don't bother calculating for these Pokemon. Never used

				//mgba_printf(MGBA_LOG_WARN, "");
				UpdateBestDoubleKillingMoveScore(bankAtk, bankDef, PARTNER(bankAtk), PARTNER(bankDef), gBattleStruct->ai.bestDoublesKillingScores[bankAtk][bankDef], &gBattleStruct->ai.bestDoublesKillingMoves[bankAtk][bankDef]);
			}
		}
	}
}

static void RunCalcShouldAIDynamax(void)
{
    u8 i, j;
	for (i = 0; i < NUM_BATTLE_SIDES; ++i)
		CalcAIDynamaxMon(i);

	for (i = 0; i < gBattlersCount; ++i)
	{
		for (j = 0; j < gBattlersCount; ++j)
			CalcShouldAIDynamax(i, j);
	}
}

static u32 GetMaxByteIndexInList(const u8 array[], const u32 size)
{
	u8 maxIndex = 0;
    u32 i;

	for (i = 0; i < size; ++i)
	{
		if (array[i] > array[maxIndex])
			maxIndex = i;
	}

	return maxIndex;
}

static u8 GetAI_ItemType(u16 itemId, const u8 *itemEffect) //Fixed from vanilla
{
    if (itemId == ITEM_FULL_RESTORE)
        return AI_ITEM_FULL_RESTORE;
    else if (itemEffect[4] & ITEM4_HEAL_HP)
        return AI_ITEM_HEAL_HP;
    else if (itemEffect[3] & ITEM3_STATUS_ALL)
        return AI_ITEM_CURE_CONDITION;
    else if (itemEffect[0] & (ITEM0_HIGH_CRIT | ITEM0_X_ATTACK) || itemEffect[1] != 0 || itemEffect[2] != 0)
        return AI_ITEM_X_STAT;
    else if (itemEffect[3] & ITEM3_MIST)
        return AI_ITEM_GUARD_SPECS;
    else
        return AI_ITEM_NOT_RECOGNIZABLE;
}

static bool8 ShouldAIUseItem(void)
{
	u32 i;
	u8 validMons = 0;
	bool8 shouldUse = FALSE;
	
	if (SIDE(gActiveBattler) == B_SIDE_PLAYER)
		return FALSE;

	struct Pokemon* party;
	u8 firstId, lastId;
	party = LoadPartyRange(gActiveBattler, &firstId, &lastId);

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		if (MON_CAN_BATTLE(&party[i]))
			++validMons;
	}

	for (i = 0; i < 4; ++i) //Number of Trainer items
	{
		u16 item;
		const u8 *itemEffects;
		u8 paramOffset;

		//if (i > 0 && validMons > (BATTLE_HISTORY->itemsNo - i) + 1) //Spread out item usage
		//	continue;
		item = BATTLE_HISTORY->trainerItems[i];
		itemEffects = gItemEffectTable[item - ITEM_POTION];

		if (item == ITEM_NONE || itemEffects == NULL)
			continue;

		switch (gBattleStruct->AI_itemType[gActiveBattler & BIT_FLANK] = GetAI_ItemType(item, itemEffects))
		{
			case AI_ITEM_FULL_RESTORE:
				if (BATTLER_ALIVE(gActiveBattler) && !BATTLER_MAX_HP(gActiveBattler))
				{
					FULL_RESTORE_LOGIC:
					if (AI_THINKING_STRUCT->aiFlags <= AI_SCRIPT_CHECK_BAD_MOVE) //Dumb AI
					{
						if (gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP / 4)
							shouldUse = TRUE;
					}
					else if (gBattleMons[gActiveBattler].hp < gBattleMons[gActiveBattler].maxHP / 2) //Smart AI should only use at less than half health
					{
						u8 foe = FOE(gActiveBattler);
						if ((BATTLER_ALIVE(foe) && ShouldRecover(gActiveBattler, foe, 0xFFFF))
						|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(foe)) && ShouldRecover(gActiveBattler, PARTNER(foe), 0xFFFF)))
						{
							shouldUse = TRUE;
						}
					}
				}
				break;
			case AI_ITEM_HEAL_HP:
				paramOffset = GetItemEffectParamOffset(item, 4, 4);
				if (paramOffset > 0 && BATTLER_ALIVE(gActiveBattler) && !BATTLER_MAX_HP(gActiveBattler))
				{
					if (gBattleMons[gActiveBattler].maxHP - gBattleMons[gActiveBattler].hp > itemEffects[paramOffset]) //Item won't restore all HP
						shouldUse = TRUE;
					else
						goto FULL_RESTORE_LOGIC;
				}
				break;
			case AI_ITEM_CURE_CONDITION: ;
				u32 status1 = gBattleMons[gActiveBattler].status1;
				gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] = 0;
				if (itemEffects[3] & ITEM3_SLEEP && status1 & STATUS1_SLEEP)
				{
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x20;
					shouldUse = TRUE;
				}
				if (itemEffects[3] & ITEM3_POISON && (status1 & STATUS1_PSN_ANY))
				{
					gBattleMons[gActiveBattler].status1 = 0; //Temporarily remove status
					if (!GoodIdeaToPoisonSelf(gActiveBattler)) //Pokemon shouldn't be poisoned
					{
						gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x10; //So heal it
						shouldUse = TRUE;
					}
					gBattleMons[gActiveBattler].status1 = status1; //Restore from backup
				}
				if (itemEffects[3] & ITEM3_BURN && status1 & STATUS1_BURN)
				{
					gBattleMons[gActiveBattler].status1 = 0; //Temporarily remove status
					if (!GoodIdeaToBurnSelf(gActiveBattler)) //Pokemon shouldn't be burned
					{
						gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x8; //So heal it
						shouldUse = TRUE;
					}
					gBattleMons[gActiveBattler].status1 = status1; //Restore from backup
				}
				if (itemEffects[3] & ITEM3_FREEZE && status1 & STATUS1_FREEZE)
				{
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x4;
					shouldUse = TRUE;
				}
				if (itemEffects[3] & ITEM3_PARALYSIS && status1 & STATUS1_PARALYSIS)
				{
					gBattleMons[gActiveBattler].status1 = 0; //Temporarily remove status
					if (!GoodIdeaToParalyzeSelf(gActiveBattler)) //Pokemon shouldn't be paralyzed
					{
						gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x2; //So heal it
						shouldUse = TRUE;
					}
					gBattleMons[gActiveBattler].status1 = status1; //Restore from backup
				}
				if (itemEffects[3] & ITEM3_CONFUSION && gBattleMons[gActiveBattler].status2 & STATUS2_CONFUSION)
				{
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x1;
					shouldUse = TRUE;
				}
				break;
			case AI_ITEM_X_STAT:
				gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] = 0;
				if (!gDisableStructs[gActiveBattler].isFirstTurn)
					break;
				if (itemEffects[0] & ITEM0_X_ATTACK)
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x1;
				if (itemEffects[1] & ITEM1_X_DEFEND)
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x2;
				if (itemEffects[1] & ITEM1_X_SPEED)
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x4;
				if (itemEffects[2] & ITEM2_X_SPATK)
				{
					if (item != ITEM_X_SP_DEF) //Sp. Atk
						gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x8;
					else //Sp. Def
						gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x10;
				}
				if (itemEffects[2] & ITEM2_X_ACCURACY)
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x20;
				if (itemEffects[0] & ITEM0_HIGH_CRIT)
					gBattleStruct->AI_itemFlags[gActiveBattler & BIT_FLANK] |= 0x80;
				shouldUse = TRUE;
				break;
			case AI_ITEM_GUARD_SPECS:
				if (gDisableStructs[gActiveBattler].isFirstTurn && gSideTimers[SIDE(gActiveBattler)].mistTimer == 0)
					shouldUse = TRUE;
				break;
			case AI_ITEM_NOT_RECOGNIZABLE:
				return FALSE;
		}

		if (shouldUse)
		{
			BtlController_EmitTwoReturnValues(1, ACTION_USE_ITEM, 0);
			gBattleStruct->chosenItem[gActiveBattler & BIT_FLANK] = item;
			BATTLE_HISTORY->trainerItems[i] = 0;
			return shouldUse;
		}
	}
	return FALSE;
}

#ifdef VAR_GAME_DIFFICULTY
static bool8 IsGoodIdeaToDoShiftSwitch(u8 switchBank, u8 foe)
{
	if (!CanKnockOut(switchBank, foe) //Current mon out can't KO new mon being switched in
	&& !WillTakeSignificantDamageFromEntryHazards(switchBank, 2)) //50% health loss
	{
		u8 mostSuitableScore = GetMostSuitableMonToSwitchIntoScore();

		if (mostSuitableScore > SWITCHING_INCREASE_KO_FOE) //Potential switch in has at least two advantages
			return TRUE;
		
		if (OnlyBadMovesLeftInMoveset(switchBank, foe)) //AI mon has nothing good against this new foe
			return TRUE;

		if (CanKnockOut(foe, switchBank) && mostSuitableScore >= SWITCHING_INCREASE_KO_FOE) //New foe can KO current AI mon
			return TRUE;
	}

	return FALSE; //Don't switch
}
#endif
*/
void ShouldDoAIShiftSwitch(void)
{
	#ifdef VAR_GAME_DIFFICULTY
	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
	&& !IS_DOUBLE_BATTLE
	&& gBattleScripting.battleStyle == OPTIONS_BATTLE_STYLE_SHIFT
	&& BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)) //AI has a mon that can be switched out
	&& VarGet(VAR_GAME_DIFFICULTY) >= OPTIONS_EXPERT_DIFFICULTY)
	{
		gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		u8 foe = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);

		CalculateAIPredictions();
		if (IsGoodIdeaToDoShiftSwitch(gActiveBattler, foe))
			return; //Continue in script
	}
	#endif

	gBattlescriptCurrInstr = BattleScript_FaintedMonChooseAnotherRejoin - 5;
}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////ai_util.c///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


bool8 CanKillAFoe(u8 bank)
{
	u8 foe = FOE(bank);
	if (!(gAbsentBattlerFlags & gBitTable[foe]) && CanKnockOut(bank, foe))
		return TRUE;

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gAbsentBattlerFlags & gBitTable[PARTNER(foe)]))
		return CanKnockOut(bank, PARTNER(foe));

	return FALSE;
}

bool8 CanKnockOut(u8 bankAtk, u8 bankDef)
{
	if (!BATTLER_ALIVE(bankAtk) || !BATTLER_ALIVE(bankDef))
		return FALSE; //Can't KO if you're dead or target is dead

	if (gBattleStruct->ai.canKnockOut[bankAtk][bankDef] == 0xFF) //Hasn't been calculated yet
	{
		struct BattlePokemon backupMonAtk, backupMonDef;
		u8 backupAbilityAtk = ABILITY_NONE; u8 backupAbilityDef = ABILITY_NONE;
		u16 backupSpeciesAtk = SPECIES_NONE; u16 backupSpeciesDef = SPECIES_NONE;

		TryTempMegaEvolveBank(bankAtk, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
		TryTempMegaEvolveBank(bankDef, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);

		if (gBattleStruct->ai.strongestMove[bankAtk][bankDef] == 0xFFFF)
			gBattleStruct->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);

		gBattleStruct->ai.canKnockOut[bankAtk][bankDef] = MoveKnocksOutXHits(gBattleStruct->ai.strongestMove[bankAtk][bankDef], bankAtk, bankDef, 1);

		if (gBattleStruct->ai.canKnockOut[bankAtk][bankDef])
			gBattleStruct->ai.can2HKO[bankAtk][bankDef] = TRUE; //If you can KO in 1 hit you can KO in 2

		TryRevertTempMegaEvolveBank(bankDef, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);
		TryRevertTempMegaEvolveBank(bankAtk, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
	}

	return gBattleStruct->ai.canKnockOut[bankAtk][bankDef];
}
/*
bool8 GetCanKnockOut(u8 bankAtk, u8 bankDef)
{
	int i;
	u16 move;
	bool8 isAsleep = IsBankAsleep(bankAtk);

	if (IsBankIncapacitated(bankAtk)
	|| gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (isAsleep && move != MOVE_SNORE)
			continue;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (MoveKnocksOutXHits(move, bankAtk, bankDef, 1))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 Can2HKO(u8 bankAtk, u8 bankDef)
{
	if (gBattleStruct->ai.can2HKO[bankAtk][bankDef] == 0xFF) //Hasn't been calculated yet
	{
		struct BattlePokemon backupMonAtk, backupMonDef;
		u8 backupAbilityAtk = ABILITY_NONE; u8 backupAbilityDef = ABILITY_NONE;
		u16 backupSpeciesAtk = SPECIES_NONE; u16 backupSpeciesDef = SPECIES_NONE;

		TryTempMegaEvolveBank(bankAtk, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
		TryTempMegaEvolveBank(bankDef, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);

		if (gBattleStruct->ai.strongestMove[bankAtk][bankDef] == 0xFFFF)
			gBattleStruct->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);

		gBattleStruct->ai.can2HKO[bankAtk][bankDef] = MoveKnocksOutXHits(gBattleStruct->ai.strongestMove[bankAtk][bankDef], bankAtk, bankDef, 2);

		TryRevertTempMegaEvolveBank(bankDef, &backupMonDef, &backupSpeciesDef, &backupAbilityDef);
		TryRevertTempMegaEvolveBank(bankAtk, &backupMonAtk, &backupSpeciesAtk, &backupAbilityAtk);
	}

	return gBattleStruct->ai.can2HKO[bankAtk][bankDef];
}

bool8 GetCan2HKO(u8 bankAtk, u8 bankDef)
{
	int i;
	u16 move;
	bool8 isAsleep = IsBankAsleep(bankAtk);

	if (IsBankIncapacitated(bankAtk)
	|| gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (isAsleep && move != MOVE_SNORE)
			continue;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (MoveKnocksOutXHits(move, bankAtk, bankDef, 2))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 CanKnockOutAfterHealing(u8 bankAtk, u8 bankDef, u16 healAmount, u8 numHits)
{
	int i;
	u16 move;
	bool8 isAsleep = IsBankAsleep(bankAtk);

	if (IsBankIncapacitated(bankAtk)
	|| gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	//Temporarily increase target's hp for calcs
	u16 backupHp = gBattleMons[bankDef].hp;
	gBattleMons[bankDef].hp = MathMin(backupHp + healAmount, gBattleMons[bankDef].maxHP);

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (isAsleep && move != MOVE_SNORE)
			continue;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalculateMoveKnocksOutXHits(move, bankAtk, bankDef, numHits)) //Need fresh calculation since data is locked earlier
			{
				gBattleMons[bankDef].hp = backupHp;
				return TRUE;
			}
		}
	}

	gBattleMons[bankDef].hp = backupHp;
	return FALSE;
}

bool8 CanKnockOutWithoutMove(const u16 ignoredMove, const u8 bankAtk, const u8 bankDef, const bool8 ignoreFutureAttacks)
{
	u16 move;
	int i;
	bool8 isAsleep = IsBankAsleep(bankAtk);

	if (IsBankIncapacitated(bankAtk)
	|| gAbsentBattlerFlags & (gBitTable[bankAtk] | gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == ignoredMove || (ignoreFutureAttacks && gBattleMoves[move].effect == EFFECT_FUTURE_SIGHT)) //Can't actually knock out right now
			continue;

		if (move == MOVE_NONE)
			break;

		if (isAsleep && move != MOVE_SNORE)
			continue;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (MoveKnocksOutXHits(move, bankAtk, bankDef, 1))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 MoveKnocksOutPossiblyGoesFirstWithBestAccuracy(u16 move, u8 bankAtk, u8 bankDef, bool8 checkGoingFirst)
{
	u16 currMove, currAcc;

	u8 bestMoveIndex = 0xFF;
	u16 bestAcc = 0;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		currMove = GetBattleMonMove(bankAtk, i);
		if (currMove == MOVE_NONE)
			break;

		currMove = TryReplaceMoveWithZMove(bankAtk, bankDef, currMove);

		if (!(gBitTable[i] & moveLimitations))
		{
			currAcc = CalcAIAccuracy(currMove, bankAtk, bankDef);

			if ((!checkGoingFirst || MoveWouldHitFirst(currMove, bankAtk, bankDef))
			&& MoveKnocksOutXHits(currMove, bankAtk, bankDef, 1))
			{
				if (MoveWillHit(currMove, bankAtk, bankDef) || currAcc > bestAcc)
				{
					bestAcc = currAcc;
					bestMoveIndex = i;
				}
				else if (currAcc == bestAcc)
				{
					if (PriorityCalc(bankAtk, ACTION_USE_MOVE, currMove) > PriorityCalc(bankAtk, ACTION_USE_MOVE, gBattleMons[bankAtk].moves[bestMoveIndex])) //The better move is still the one with more priority
						bestMoveIndex = i;
				}
			}
		}
	}

	if (bestMoveIndex == 0xFF) //No moves knock out and go first
		return FALSE;

	if (gBattleMons[bankAtk].moves[bestMoveIndex] == move)
		return TRUE;

	return FALSE;
}

bool8 IsWeakestContactMoveWithBestAccuracy(u16 move, u8 bankAtk, u8 bankDef)
{
	u16 currMove, currAcc, moveEffect;

	u8 bestMoveIndex = 0xFF;
	u16 bestAcc = 0;
	u32 bestDmg = 0xFFFFFFFF;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	struct DamageCalc damageData = {0};
	damageData.bankAtk = bankAtk;
	damageData.bankDef = bankDef;
	PopulateDamageCalcStructWithBaseAttackerData(&damageData);
	PopulateDamageCalcStructWithBaseDefenderData(&damageData);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		currMove = GetBattleMonMove(bankAtk, i);

		if (currMove == MOVE_NONE)
			break;

		moveEffect = gBattleMoves[currMove].effect;

		if (!(gBitTable[i] & moveLimitations)
		&& CheckContact(currMove, bankAtk)
		&& moveEffect != EFFECT_RECHARGE
		&& moveEffect != EFFECT_COUNTER
		&& moveEffect != EFFECT_MIRROR_COAT
		&& moveEffect != EFFECT_BURN_UP
		&& moveEffect != EFFECT_FAKE_OUT
		&& moveEffect != EFFECT_OHKO) //Don't use these move effects on partner
		{
			currAcc = AccuracyCalc(currMove, bankAtk, bankDef);
			u32 dmg = GetFinalAIMoveDamage(currMove, bankAtk, bankDef, 1, &damageData);

			if (dmg < bestDmg && currAcc > bestAcc)
			{
				bestAcc = currAcc;
				bestDmg = dmg;
				bestMoveIndex = i;
			}
		}
	}

	if (bestMoveIndex == 0xFF) //No moves knock out and go first
		return FALSE;

	if (gBattleMons[bankAtk].moves[bestMoveIndex] == move)
		return TRUE;

	return FALSE;
}

bool8 StrongestMoveGoesFirst(u16 move, u8 bankAtk, u8 bankDef)
{
	u16 currMove;
	u32 currDmg;

	u8 bestMoveIndex = 0xFF;
	u32 bestDmg = 0;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));

	struct DamageCalc damageData = {0};
	damageData.bankAtk = bankAtk;
	damageData.bankDef = bankDef;
	PopulateDamageCalcStructWithBaseAttackerData(&damageData);
	PopulateDamageCalcStructWithBaseDefenderData(&damageData);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		currMove = GetBattleMonMove(bankAtk, i);

		if (currMove == MOVE_NONE)
			break;

		currMove = TryReplaceMoveWithZMove(bankAtk, bankDef, currMove);

		if (!(gBitTable[i] & moveLimitations))
		{
			currDmg = GetFinalAIMoveDamage(currMove, bankAtk, bankDef, 1, &damageData);

			if (MoveWouldHitFirst(currMove, bankAtk, bankDef)
			&& currDmg > bestDmg)
			{
				bestDmg = currDmg;
				bestMoveIndex = i;
			}
		}
	}

	if (bestMoveIndex == 0xFF) //No moves knock out and go first
		return FALSE;

	if (gBattleMons[bankAtk].moves[bestMoveIndex] == move)
		return TRUE;

	return FALSE;
}

bool8 CanKnockOutFromParty(struct Pokemon* monAtk, u8 bankDef, struct DamageCalc* damageData)
{
	int i;
	u16 move;
	bool8 isAsleep = GetMonData(monAtk, MON_DATA_STATUS, NULL) & STATUS_SLEEP;

	if (gAbsentBattlerFlags & (gBitTable[bankDef]))
		return FALSE;

	u8 moveLimitations = CheckMoveLimitationsFromParty(monAtk, 0, 0xFF);

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetMonData(monAtk, MON_DATA_MOVE1 + i, NULL);

		if (move == MOVE_NONE)
			break;

		if (isAsleep && move != MOVE_SNORE)
			continue;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (MoveKnocksOutXHitsFromParty(move, monAtk, bankDef, 1, damageData))
				return TRUE;
		}
	}

	return FALSE;
}
*/
void UpdateBestDoubleKillingMoveScore(u8 bankAtk, u8 bankDef, u8 bankAtkPartner, u8 bankDefPartner, s8 bestMoveScores[MAX_BATTLERS_COUNT], u16* bestMove)
{
	//mgba_printf(MGBA_LOG_ERROR, "");
	int i, j;
	u8 currTarget;
	u16 move;
	s8 moveScores[MAX_MON_MOVES][MAX_BATTLERS_COUNT] = {0};
	u16 turnedToZMove[MAX_MON_MOVES] = {0};

	u16 partnerMove = MOVE_NONE;
	u16 partnerTarget = gBattleStruct->moveTarget[bankAtkPartner];
	if (gChosenMoveByBattler[bankAtkPartner] != MOVE_NONE)
		partnerMove = GetAIChosenMove(bankAtkPartner, partnerTarget);
{
	bool8 partnerIncapacitated = IsBankIncapacitated(bankAtkPartner);
	bool8 partnerKnocksOut = partnerTarget != bankAtkPartner && MoveKnocksOutXHits(partnerMove, bankAtkPartner, partnerTarget, 1);

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);
	
	u8 foes[2] = {bankDef, bankDefPartner};
	bool8 foeAlive[2] = {BATTLER_ALIVE(bankDef), BATTLER_ALIVE(bankDefPartner)};
	bool8 partnerHandling[2] = {FALSE, FALSE};
	bool8 foesAlive = CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef);
	u8 bestIndex = 0;
	struct DamageCalc damageDatas[2];

	if (partnerMove != MOVE_NONE && !partnerIncapacitated && partnerKnocksOut && foesAlive >= 2) //More than 1 target left
	{
		for (j = 0; j < gBattlersCount / 2; ++j)
		{
			if (partnerTarget == foes[j])
				partnerHandling[j] = TRUE;
		}
	}

	
	for (j = 0; j < gBattlersCount / 2; ++j)
	{
		if (foeAlive[j] && !partnerHandling[j]) //Damage will actually get calculated
		{
			//Populate the damage calc structs
			struct DamageCalc* damageData = &damageDatas[j];
			memset(damageData, 0, sizeof(damageDatas[j]));
			damageData->bankAtk = bankAtk;
			damageData->bankDef = foes[j];
			PopulateDamageCalcStructWithBaseAttackerData(damageData);
			PopulateDamageCalcStructWithBaseDefenderData(damageData);
		}
	}

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		//mgba_printf(MGBA_LOG_INFO, "");
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (SPLIT(move) == SPLIT_STATUS)
			continue;

		move = TryReplaceMoveWithZMove(bankAtk, bankDef, move);
		if (IsZMove(move) || IsAnyMaxMove(move))
			turnedToZMove[i] = move;

		if (!(gBitTable[i] & moveLimitations))
		{
			for (j = 0; j < gBattlersCount / 2; ++j)
			{
				//mgba_printf(MGBA_LOG_WARN, "");
				currTarget = foes[j];

				if (foeAlive[j] && (j == 0 || gBattleMoves[move].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY)) //Only can hit second foe with spread move
				&& !partnerHandling[j]) //Don't count the target if the partner is already taking care of it
				{
					if (!(AI_SpecialTypeCalc(move, bankAtk, currTarget) & MOVE_RESULT_NO_EFFECT)) //Move has effect on current target
					{
						moveScores[i][currTarget] += DOUBLES_INCREASE_HIT_FOE; //Hit one enemy

						if (MoveKnocksOutXHits(move, bankAtk, currTarget, 1))
						{
							moveScores[i][currTarget] += DOUBLES_INCREASE_KO_FOE; //KO enemy

							if (gBattleMons[currTarget].status2 & STATUS2_DESTINY_BOND)
								moveScores[i][currTarget] -= DOUBLES_DECREASE_DESTINY_BOND;
						}
						else if (IsStrongestMove(move, bankAtk, currTarget))
							moveScores[i][currTarget] += DOUBLES_INCREASE_STRONGEST_MOVE;
						else
						{
							#define CALC (CalcSecondaryEffectChance(bankAtk, move) >= 50)
							//These move effects are good even if they do minimal damage
							switch (gBattleMoves[move].effect) {
								case EFFECT_FLINCH_HIT:
									if (MoveWouldHitFirst(move, bankAtk, currTarget))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_PARALYZE_HIT:
									if (CALC && !BadIdeaToParalyze(currTarget, bankAtk))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_BURN_HIT:
									if (CALC && !BadIdeaToBurn(currTarget, bankAtk))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_FREEZE_HIT:
									if (CALC && !BadIdeaToFreeze(currTarget, bankAtk))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_POISON_HIT:
								case EFFECT_BAD_POISON_HIT:
									if (CALC && !BadIdeaToPoison(currTarget, bankAtk))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_ATTACK_DOWN_HIT:
									if (CALC && GoodIdeaToLowerAttack(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_DEFENSE_DOWN_HIT:
									if (CALC && GoodIdeaToLowerDefense(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_SPECIAL_ATTACK_DOWN_HIT:
									if (CALC && GoodIdeaToLowerSpAtk(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_SPECIAL_DEFENSE_DOWN_HIT:
									if (CALC && GoodIdeaToLowerSpDef(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_SPEED_DOWN_HIT:
									if (CALC && GoodIdeaToLowerSpeed(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_ACCURACY_DOWN_HIT:
									if (CALC && GoodIdeaToLowerAccuracy(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_EVASION_DOWN_HIT:
									if (CALC && GoodIdeaToLowerEvasion(currTarget, bankAtk, move))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_CONFUSE_HIT:
									if (CALC && CanBeConfused(bankDef, TRUE))
										break;
									goto DEFAULT_CHECK;
								case EFFECT_SPEED_UP_1_HIT:
								case EFFECT_ATTACK_UP_HIT:
									if (CALC)
										break;
									goto DEFAULT_CHECK;

								default:
								DEFAULT_CHECK:
									if (foesAlive >= 2) //If there's only 1 target we rely on the strongest move check
									{
										u32 dmg = GetFinalAIMoveDamage(move, bankAtk, currTarget, 1, &damageDatas[j]);
										if (dmg < gBattleMons[currTarget].maxHP / 4)
											moveScores[i][currTarget] -= (DOUBLES_INCREASE_HIT_FOE - 1); //Count it as if you basically don't do damage to the enemy
										else if (dmg < gBattleMons[currTarget].maxHP / 3)
											moveScores[i][currTarget] -= (DOUBLES_INCREASE_HIT_FOE - 2); //Count less than basically not hitting the enemy
									}
							}
							#undef CALC
						}
					}
				}
			}

			if (gBattleMoves[move].target & MOVE_TARGET_FOES_AND_ALLY
			&&  RangeMoveCanHurtPartner(move, bankAtk, bankAtkPartner))
			{
				u8 status1 = gBattleMons[bankAtkPartner].status1;
				if ((status1 & STATUS1_SLEEP) <= 1 //Partner will be awake to use move
				&& !(status1 & STATUS1_FREEZE)
				&& !gDisableStructs[bankAtkPartner].truantCounter)
				{
					if (partnerMove != MOVE_NONE) //Partner has chosen a move
					{
						u8 moveEffect = gBattleMoves[partnerMove].effect;

						if (moveEffect == EFFECT_PROTECT
						&&  partnerMove != MOVE_QUICKGUARD
						&&  partnerMove != MOVE_CRAFTYSHIELD
						&& (partnerMove != MOVE_MATBLOCK || gDisableStructs[bankAtkPartner].isFirstTurn))
						{
							//Partner is protecting so use the spread move
						}
						else if (moveEffect == EFFECT_SEMI_INVULNERABLE && !MoveWouldHitFirst(move, bankAtk, bankAtkPartner))
						{
						}
						else
							moveScores[i][bankAtkPartner] -= DOUBLES_DECREASE_HIT_PARTNER; //Hitting partner is bad
					}
					else if (!HasProtectionMoveInMoveset(bankAtkPartner, CHECK_WIDE_GUARD | CHECK_MAT_BLOCK) //Ally can't protect against attack
					|| (gBattleMoves[gLastResultingMoves[bankAtkPartner]].effect == EFFECT_PROTECT && gDisableStructs[bankAtkPartner].protectUses >= 1)) //Don't rely on double protect
					{
						moveScores[i][bankAtkPartner] -= DOUBLES_DECREASE_HIT_PARTNER; //Hitting partner is bad
					}
				}
				else
					moveScores[i][bankAtkPartner] -= DOUBLES_DECREASE_HIT_PARTNER; //Hitting partner is bad
			}
		}
	}

	
	for (i = 1; i < MAX_MON_MOVES; ++i)
	{
		u8 currScore = moveScores[i][bankDef]
					 + moveScores[i][bankDefPartner]
					 + moveScores[i][bankAtkPartner];

		u8 bestScore = moveScores[bestIndex][bankDef]
					 + moveScores[bestIndex][bankDefPartner]
					 + moveScores[bestIndex][bankAtkPartner];

		if (currScore > bestScore)
			bestIndex = i;
		else if (currScore == bestScore)
		{
			u8 currentBestRange = gBattleMoves[gBattleMons[bankAtk].moves[bestIndex]].target;
			u8 checkBestRange = gBattleMoves[gBattleMons[bankAtk].moves[i]].target;

			if (currentBestRange & MOVE_TARGET_FOES_AND_ALLY
			&& !(checkBestRange & MOVE_TARGET_FOES_AND_ALLY)
			&& checkBestRange & MOVE_TARGET_BOTH
			&& RangeMoveCanHurtPartner(gBattleMons[bankAtk].moves[bestIndex], bankAtk, bankAtkPartner))
				bestIndex = i; //If an all-hitting move and a both foes move have the same score, use the one that only hits both foes
		}
	}

	bestMoveScores[bankDef] = moveScores[bestIndex][bankDef];
	bestMoveScores[bankDefPartner] = moveScores[bestIndex][bankDefPartner];
	bestMoveScores[bankAtkPartner] = moveScores[bestIndex][bankAtkPartner];

	*bestMove = gBattleMons[bankAtk].moves[bestIndex];
	if (turnedToZMove[bestIndex] != MOVE_NONE)
		*bestMove = turnedToZMove[bestIndex];
}
}

u8 GetDoubleKillingScore(u16 move, u8 bankAtk, u8 bankDef)
{
	if (gBattleMoves[move].target & MOVE_TARGET_FOES_AND_ALLY
	&&  gBattleMoves[gChosenMoveByBattler[PARTNER(bankAtk)]].effect == EFFECT_PROTECT
	&& !gBattleStruct->recalculatedBestDoublesKillingScores[bankAtk])
	{
		//Recalculate as partner has chosen to protect from spread move
		gBattleStruct->recalculatedBestDoublesKillingScores[bankAtk] = TRUE;
		UpdateBestDoubleKillingMoveScore(bankAtk, bankDef, PARTNER(bankAtk), PARTNER(bankDef), gBattleStruct->ai.bestDoublesKillingScores[bankAtk][bankDef], &gBattleStruct->ai.bestDoublesKillingMoves[bankAtk][bankDef]);
	}

	if (move == gBattleStruct->ai.bestDoublesKillingMoves[bankAtk][bankDef])
	{
		//When a Pokemon attacks a target, it can gain or lose points for the damage it does
		//to the surrounding field
		return gBattleStruct->ai.bestDoublesKillingScores[bankAtk][bankDef][bankDef]
		     + gBattleStruct->ai.bestDoublesKillingScores[bankAtk][bankDef][PARTNER(bankDef)]
			 + gBattleStruct->ai.bestDoublesKillingScores[bankAtk][bankDef][PARTNER(bankAtk)];
	}

	return 0;
}

static void RemoveDoublesKillingScore(u8 bankAtk, u8 bankDef)
{
	//Reset the points received by attacking bankDef for both foes
	UpdateBestDoubleKillingMoveScore(bankAtk, bankDef, PARTNER(bankAtk), PARTNER(bankDef), gBattleStruct->ai.bestDoublesKillingScores[bankAtk][bankDef], &gBattleStruct->ai.bestDoublesKillingMoves[bankAtk][bankDef]);
}

void TryRemoveDoublesKillingScore(u8 bankAtk, u8 bankDef, u16 chosenMove)
{
	u8 partner = PARTNER(bankAtk);

	if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(partner))
	{
		u16 partnerKillingMove = gBattleStruct->ai.bestDoublesKillingMoves[partner][bankDef];

		if (!(bankAtk & BIT_FLANK) //Don't bother with reseting scores if no Pokemon are going to choose moves after this
		&& !IsBankIncapacitated(bankAtk) //Not actually going to hit a target this turn
		&& SIDE(bankAtk) != SIDE(bankDef) //No scores are calculated for hitting partners
		&& (CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef) >= 2 //Two targets that can be hit on enemy side
		 || MoveWouldHitFirst(chosenMove, bankAtk, bankDef) //This mon is going to hit the enemy before it can attack
		 || !MoveWouldHitFirst(partnerKillingMove, partner, bankDef)) //The partner won't hit the enemy before it can attack
		&& MoveKnocksOutXHits(chosenMove, bankAtk, bankDef, 1))
			RemoveDoublesKillingScore(partner, bankDef); //This mon's got it covered
	}
}

bool8 RangeMoveCanHurtPartner(u16 move, u8 bankAtk, u8 bankAtkPartner)
{
	return BATTLER_ALIVE(bankAtkPartner)
		&& ABILITY(bankAtkPartner) != ABILITY_TELEPATHY
		&& !(BATTLER_SEMI_INVULNERABLE(bankAtkPartner) && MoveWouldHitFirst(move, bankAtk, bankAtkPartner)) //Target will still be semi-invulnerable when attack hits
		&& !(AI_SpecialTypeCalc(move, bankAtk, bankAtkPartner) & MOVE_RESULT_NO_EFFECT); //Move has effect
}

static bool8 CalculateMoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits)
{
	u8 ability = ABILITY(bankDef);
	u16 species = SPECIES(bankDef);
	bool8 noMoldBreakers = NO_MOLD_BREAKERS(ABILITY(bankAtk), move);

	if (MoveBlockedBySubstitute(move, bankAtk, bankDef)
	#ifdef SPECIES_MIMIKYU
	|| (ability == ABILITY_DISGUISE && species == SPECIES_MIMIKYU && noMoldBreakers)
	#endif
	#ifdef SPECIES_EISCUE
	|| (ability == ABILITY_ICEFACE && species == SPECIES_EISCUE && SPLIT(move) == SPLIT_PHYSICAL && noMoldBreakers)
	#endif
	)
	{
		if (numHits > 0)
			numHits -= 1; //Takes at least a hit to break Disguise/Ice Face or sub
	}

	if (GetFinalAIMoveDamage(move, bankAtk, bankDef, numHits, NULL) >= gBattleMons[bankDef].hp)
		return TRUE;

	return FALSE;
}

bool8 MoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits)
{
	u8 movePos;

	switch (numHits) {
		case 1:
			if (gBattleMoves[move].effect == EFFECT_FUTURE_SIGHT)
				return FALSE; //Really always 3 hits

			movePos = FindMovePositionInMoveset(move, bankAtk);
			if (movePos >= MAX_MON_MOVES)
				break; //Move not in moveset

			if (gBattleStruct->ai.moveKnocksOut1Hit[bankAtk][bankDef][movePos] != 0xFF)
				return gBattleStruct->ai.moveKnocksOut1Hit[bankAtk][bankDef][movePos];
			return gBattleStruct->ai.moveKnocksOut1Hit[bankAtk][bankDef][movePos] = CalculateMoveKnocksOutXHits(move, bankAtk, bankDef, 1);

		case 2:
			if (gBattleMoves[move].effect == EFFECT_FUTURE_SIGHT)
				return FALSE; //Really always 3 hits

			movePos = FindMovePositionInMoveset(move, bankAtk);
			if (movePos >= MAX_MON_MOVES)
				break; //Move not in moveset

			if (gBattleStruct->ai.moveKnocksOut2Hits[bankAtk][bankDef][movePos] != 0xFF)
				return gBattleStruct->ai.moveKnocksOut2Hits[bankAtk][bankDef][movePos];
			return gBattleStruct->ai.moveKnocksOut2Hits[bankAtk][bankDef][movePos] = CalculateMoveKnocksOutXHits(move, bankAtk, bankDef, 2);
	}

	return CalculateMoveKnocksOutXHits(move, bankAtk, bankDef, numHits);
}

bool8 MoveKnocksOutXHitsFromParty(u16 move, struct Pokemon* monAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData)
{
	u8 ability = ABILITY(bankDef);
	u16 species = SPECIES(bankDef);
	bool8 noMoldBreakers = NO_MOLD_BREAKERS(GetMonAbility(monAtk), move);

	if (MonMoveBlockedBySubstitute(move, monAtk, bankDef)
	#ifdef SPECIES_MIMIKYU
	|| (ability == ABILITY_DISGUISE && species == SPECIES_MIMIKYU && noMoldBreakers)
	#endif
	#ifdef SPECIES_EISCUE
	|| (ability == ABILITY_ICEFACE && species == SPECIES_EISCUE && SPLIT(move) == SPLIT_PHYSICAL && noMoldBreakers)
	#endif
	)
	{
		if (numHits > 0)
			numHits -= 1; //Takes at least a hit to break Disguise/Ice Face or sub
	}

	if (CalcFinalAIMoveDamageFromParty(move, monAtk, bankDef, numHits, damageData) >= gBattleMons[bankDef].hp)
		return TRUE;

	return FALSE;
}

static bool8 MoveKnocksOutAfterDynamax(u16 move, u8 bankAtk, u8 bankDef)
{
	bool8 ret;
	u8 movePos = FindMovePositionInMoveset(move, bankAtk);
	u16 backupHP, backupMaxHP;

	if (movePos < MAX_MON_MOVES && !gBattleStruct->ai.moveKnocksOut1Hit[bankAtk][bankDef][movePos])
		return FALSE; //Move doesn't normally KO, certainly won't KO after Dynamaxing

	//Temporarily Dynamax target
	backupHP = gBattleMons[bankDef].hp;
	backupMaxHP = gBattleMons[bankDef].maxHP;
	gBattleMons[bankDef].hp *= GetDynamaxHPBoost(bankDef);
	gBattleMons[bankDef].maxHP *= GetDynamaxHPBoost(bankDef);
	gBattleStruct->dynamaxData.timer[bankDef] += 1;

	ret = CalculateMoveKnocksOutXHits(move, bankAtk, bankDef, 1);

	//Revert Dynamax
	gBattleStruct->dynamaxData.timer[bankDef] -= 1;
	gBattleMons[bankDef].maxHP = backupMaxHP;
	gBattleMons[bankDef].hp = backupHP;

	return ret;
}

u16 CalcFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData)
{
	u32 dmg;
	u8 defAbility;
	if (move == MOVE_NONE || numHits == 0 || gBattleMoves[move].power == 0)
		return 0;

	switch (gBattleMoves[move].effect) {
		case EFFECT_FAKE_OUT:
			if (!gDisableStructs[bankAtk].isFirstTurn)
				return 0;
			break;

		case EFFECT_BURN_UP:
			if (!IsOfType(bankAtk, TYPE_FIRE))
				return 0;
			break;

		case EFFECT_OHKO:
			if (gBattleMons[bankAtk].level <= gBattleMons[bankDef].level)
				return 0;
			if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
				return 0;

			return gBattleMons[bankDef].hp;

		case EFFECT_COUNTER: //Includes Metal Burst
		case EFFECT_MIRROR_COAT:
			return CalcPredictedDamageForCounterMoves(move, bankAtk, bankDef);
	}

	dmg = NewAI_CalcDmg(bankAtk, bankDef, move, damageData);
	if (dmg >= gBattleMons[bankDef].hp)
		return gBattleMons[bankDef].hp;

	defAbility = ABILITY(bankDef);
	if (numHits >= 2 && BATTLER_MAX_HP(bankDef) && (defAbility == ABILITY_MULTISCALE || defAbility == ABILITY_SHADOWSHIELD))
		return MathMin(dmg + (dmg * 2) * (numHits - 1), gBattleMons[bankDef].maxHP); //Adjust damage on subsequent hits

	return MathMin(dmg * numHits, gBattleMons[bankDef].maxHP);
}

u32 GetFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData)
{
	u8 movePos = FindMovePositionInMoveset(move, bankAtk);
	if (movePos < MAX_MON_MOVES) //Move in moveset
	{
		if (gBattleStruct->ai.damageByMove[bankAtk][bankDef][movePos] != 0xFFFFFFFF)
			return gBattleStruct->ai.damageByMove[bankAtk][bankDef][movePos] * numHits;
		gBattleStruct->ai.damageByMove[bankAtk][bankDef][movePos] = CalcFinalAIMoveDamage(move, bankAtk, bankDef, 1, damageData);
		return gBattleStruct->ai.damageByMove[bankAtk][bankDef][movePos] * numHits;
	}

	return CalcFinalAIMoveDamage(move, bankAtk, bankDef, 1, damageData) * numHits;
}

u16 CalcFinalAIMoveDamageFromParty(u16 move, struct Pokemon* monAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData)
{
	if (move == MOVE_NONE || SPLIT(move) == SPLIT_STATUS || gBattleMoves[move].power == 0)
		return 0;

	switch (gBattleMoves[move].effect) {
		case EFFECT_OHKO:
			if (GetMonData(monAtk, MON_DATA_LEVEL, NULL) <= gBattleMons[bankDef].level)
				return 0;
			if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
				return 0;

			return gBattleMons[bankDef].hp;

		case EFFECT_COUNTER: //Includes Metal Burst
		case EFFECT_MIRROR_COAT:
			return 0;
	}

	return MathMin(AI_CalcPartyDmg(FOE(bankDef), bankDef, move, monAtk, damageData) * numHits, gBattleMons[bankDef].maxHP);
}

static u32 CalcPredictedDamageForCounterMoves(u16 move, u8 bankAtk, u8 bankDef)
{
	u16 predictedMove;
	u32 predictedDamage = 0;
	
	if (AI_SpecialTypeCalc(move, bankAtk, bankDef) & MOVE_RESULT_NO_EFFECT)
		return 0; //These moves are subject to immunities

	if (gBattleStruct->ai.strongestMove[bankDef][bankAtk] != 0xFFFF) //Don't force calculation here - can cause infinite loop if both Pokemon have a counter move
		predictedMove = GetStrongestMove(bankDef, bankAtk); //Get the strongest move as the predicted move
	else
		predictedMove = IsValidMovePrediction(bankDef, bankAtk);

	if (predictedMove != MOVE_NONE && SPLIT(predictedMove) != SPLIT_STATUS && !MoveBlockedBySubstitute(predictedMove, bankDef, bankAtk))
	{
		predictedDamage = GetFinalAIMoveDamage(predictedMove, bankDef, bankAtk, 1, NULL); //The damage the enemy will do to the AI

		switch (move) {
			case MOVE_COUNTER:
				if (CalcMoveSplit(bankDef, predictedMove) == SPLIT_PHYSICAL)
					predictedDamage *= 2;
				else
					predictedDamage = 0;
				break;
			case MOVE_MIRRORCOAT:
				if (CalcMoveSplit(bankDef, predictedMove) == SPLIT_SPECIAL)
					predictedDamage *= 2;
				else
					predictedDamage = 0;
				break;
			default: //Metal Burst
				predictedDamage = (predictedDamage * 3) / 2;
		}
	}

	return predictedDamage;
}

u16 CalcStrongestMove(const u8 bankAtk, const u8 bankDef, const bool8 onlySpreadMoves)
{
	u16 move;
	u32 predictedDamage;
	u16 strongestMove = gBattleMons[bankAtk].moves[0];
	u32 highestDamage = 0;
	u16 defHP = gBattleMons[bankDef].hp;
	struct DamageCalc damageData = {0};
	u8 moveLimitations;
	u32 i;

	if (defHP == 0) //Foe dead
		return MOVE_NONE;

	moveLimitations = CheckMoveLimitations(bankAtk, 0, AdjustMoveLimitationFlagsForAI(bankAtk, bankDef));
	predictedDamage = 0;

	//Save time and do this now instead of before each move
	damageData.bankAtk = bankAtk;
	damageData.bankDef = bankDef;
	PopulateDamageCalcStructWithBaseAttackerData(&damageData);
	PopulateDamageCalcStructWithBaseDefenderData(&damageData);

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = gBattleMons[bankAtk].moves[i];
		if (move == MOVE_NONE)
			break;

		move = TryReplaceMoveWithZMove(bankAtk, bankDef, move);

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].power == 0
			|| (onlySpreadMoves && !(gBattleMoves[move].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY))))
				continue;

			if (gBattleMoves[move].effect == EFFECT_COUNTER || gBattleMoves[move].effect == EFFECT_MIRROR_COAT) //Includes Metal Burst
			{
				predictedDamage = CalcPredictedDamageForCounterMoves(move, bankAtk, bankDef);
				if (predictedDamage > (u32) highestDamage)
				{
					strongestMove = move;
					highestDamage = predictedDamage;
				}
			}
			else if (gBattleMoves[move].effect == EFFECT_OHKO)
			{
				gBattleStruct->ai.moveKnocksOut1Hit[bankAtk][bankDef][i] = FALSE;
				gBattleStruct->ai.damageByMove[bankAtk][bankDef][i] = 0;
				if (gBattleMons[bankAtk].level <= gBattleMons[bankDef].level)
					continue;
				if (move == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
					continue;
				if (ABILITY(bankDef) == ABILITY_STURDY)
					continue;
				if (MoveWillHit(move, bankAtk, bankDef))
				{
					gBattleStruct->ai.moveKnocksOut1Hit[bankAtk][bankDef][i] = TRUE;
					gBattleStruct->ai.damageByMove[bankAtk][bankDef][i] = defHP;
					return move; //No stronger move that OHKO move that can kill
				}
			}
			else
			{
				predictedDamage = CalcFinalAIMoveDamage(move, bankAtk, bankDef, 1, &damageData);

				if (predictedDamage > (u32) highestDamage)
				{
					strongestMove = move;
					highestDamage = predictedDamage;
				}
				else if (predictedDamage == highestDamage
				&& PriorityCalc(bankAtk, B_ACTION_USE_MOVE, move) > PriorityCalc(bankAtk, B_ACTION_USE_MOVE, strongestMove)) //Use faster of two strongest moves
				{
					strongestMove = move;
				}
				else if (predictedDamage == highestDamage) //Find which move has better Acc
				{
					u16 currAcc = CalcAIAccuracy(move, bankAtk, bankDef);
					u16 bestMoveAcc = CalcAIAccuracy(strongestMove, bankAtk, bankDef);

					if (currAcc > bestMoveAcc)
					{
						strongestMove = move;
						highestDamage = predictedDamage;
					}
				}
			}

			gBattleStruct->ai.damageByMove[bankAtk][bankDef][i] = predictedDamage;
			if (predictedDamage >= defHP)
				gBattleStruct->ai.moveKnocksOut1Hit[bankAtk][bankDef][i] = TRUE;
			else
				gBattleStruct->ai.moveKnocksOut1Hit[bankAtk][bankDef][i] = FALSE;
		}
	}

	return strongestMove;
}

bool8 IsStrongestMove(const u16 currentMove, const u8 bankAtk, const u8 bankDef)
{
	if (gBattleStruct->ai.strongestMove[bankAtk][bankDef] == 0xFFFF)
		gBattleStruct->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);

	return gBattleStruct->ai.strongestMove[bankAtk][bankDef] == currentMove;
}

u16 GetStrongestMove(const u8 bankAtk, const u8 bankDef)
{
	if (gBattleStruct->ai.strongestMove[bankAtk][bankDef] == 0xFFFF)
		gBattleStruct->ai.strongestMove[bankAtk][bankDef] = CalcStrongestMove(bankAtk, bankDef, FALSE);

	return gBattleStruct->ai.strongestMove[bankAtk][bankDef];
}

static void ClearStrongestMoveAndCanKnockOut(const u8 bankAtk, const u8 bankDef)
{
	gBattleStruct->ai.strongestMove[bankAtk][bankDef] = 0xFFFF;
	gBattleStruct->ai.canKnockOut[bankAtk][bankDef] = 0xFF;
	gBattleStruct->ai.can2HKO[bankAtk][bankDef] = 0xFF;
}

bool8 MoveWillHit(u16 move, u8 bankAtk, u8 bankDef)
{
	#ifdef REALLY_SMART_AI
		u8 defAbility = BATTLE_HISTORY->abilities[bankDef];
	#else
		u8 defAbility = ABILITY(bankDef);
	#endif

	if (ABILITY(bankAtk) == ABILITY_NOGUARD
	||  defAbility == ABILITY_NOGUARD
	||  (gStatuses3[bankDef] & STATUS3_ALWAYS_HITS && gDisableStructs[bankDef].battlerWithSureHit == bankAtk))
		return TRUE;

	if (((gStatuses3[bankDef] & (STATUS3_ON_AIR | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET)) && !CheckTableForMove(move, gIgnoreInAirMoves))
	||  ((gStatuses3[bankDef] & STATUS3_UNDERGROUND) && !CheckTableForMove(move, gIgnoreUndergoundMoves))
	||  ((gStatuses3[bankDef] & STATUS3_UNDERWATER) && !CheckTableForMove(move, gIgnoreUnderwaterMoves))
	||   (gStatuses3[bankDef] & STATUS3_DISAPPEARED))
		return FALSE;

	if ((move == MOVE_TOXIC && IsOfType(bankAtk, TYPE_POISON))
	||  (CheckTableForMove(move, gAlwaysHitWhenMinimizedMoves) && gStatuses3[bankDef] & STATUS3_MINIMIZED)
	|| ((gStatuses3[bankDef] & STATUS3_TELEKINESIS) && gBattleMoves[move].effect != EFFECT_OHKO)
	||  gBattleMoves[move].accuracy == 0
	|| (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY) && CheckTableForMove(move, gAlwaysHitInRainMoves))
	||  IsZMove(move)
	||  IsAnyMaxMove(move))
		return TRUE;

	return FALSE;
}

bool8 MoveWouldHitFirst(u16 move, u16 bankAtk, u16 bankDef)
{
	//Use move prediction
	u16 defMovePrediction = IsValidMovePrediction(bankDef, bankAtk);
	return MoveWouldHitBeforeOtherMove(move, bankAtk, defMovePrediction, bankDef);
}

bool8 MoveWouldHitBeforeOtherMove(u16 moveAtk, u8 bankAtk, u16 moveDef, u8 bankDef)
{
	u32 temp;
	u32 bankAtkSpeed, bankDefSpeed;

//Priority Calc
	if (moveDef == MOVE_NONE)
	{
		if (PriorityCalc(bankAtk, B_ACTION_USE_MOVE, moveAtk) > 0)
			return TRUE;
	}
	else
	{
		if (PriorityCalc(bankAtk, B_ACTION_USE_MOVE, moveAtk) > PriorityCalc(bankDef, B_ACTION_USE_MOVE, moveDef))
			return TRUE;
	}

//BracketCalc
	if (BracketCalc(bankAtk) > BracketCalc(bankDef)) //Hehehe...AI knows when its Quick Claw activates
		return TRUE;

//SpeedCalc
	bankAtkSpeed = SpeedCalc(bankAtk);
	bankDefSpeed = SpeedCalc(bankDef);
	if (IsTrickRoomActive())
	{
		temp = bankDefSpeed;
		bankDefSpeed = bankAtkSpeed;
		bankAtkSpeed = temp;
	}

	if (bankAtkSpeed > bankDefSpeed)
		return TRUE;

	return FALSE;
}

bool8 IsUsefulToFlinchTarget(u8 bankDef)
{
	if (IsDynamaxed(bankDef))
		return FALSE;

	if (IsBankAsleep(bankDef)
	&&  !MoveInMoveset(MOVE_SLEEPTALK, bankDef)
	&&  !MoveInMoveset(MOVE_SNORE, bankDef))
		return FALSE;

	return TRUE;
}

bool8 IsBankAsleep(u8 bank)
{
	return gBattleMons[bank].status1 & STATUS1_SLEEP
	#ifdef REALLY_SMART_AI
	&& gBattleMons[bank].status1 > 1 //Won't wake up this turn
	#endif
	;
}

bool8 IsBankIncapacitated(u8 bank)
{
	if (IsBankAsleep(bank))
		return TRUE;

	if (gBattleMons[bank].status1 & STATUS1_FREEZE)
		return TRUE;

	if (gBattleMons[bank].status2 & STATUS2_RECHARGE
	||  (ABILITY(bank) == ABILITY_TRUANT && gDisableStructs[bank].truantCounter != 0))
		return TRUE;

	return FALSE;
}
/*
bool8 WillFaintFromWeatherSoon(u8 bank)
{
	if (TakesDamageFromSandstorm(bank) || TakesDamageFromHail(bank))
	{
		return gBattleMons[bank].hp <= GetBaseMaxHP(bank) / 16;
	}

	return FALSE;
}

bool8 WillTakeSignificantDamageFromEntryHazards(u8 bank, u8 healthFraction)
{
	u32 dmg = 0;

	if (gSideStatuses[SIDE(bank)] & SIDE_STATUS_SPIKES
	&& GetMonAbility(GetBankPartyData(bank)) != ABILITY_MAGICGUARD
	&& ITEM_EFFECT(bank) != ITEM_EFFECT_HEAVY_DUTY_BOOTS)
	{
		struct Pokemon* mon = GetBankPartyData(bank);

		if (gSideTimers[SIDE(bank)].srAmount > 0)
			dmg += CalcStealthRockDamagePartyMon(mon);

		if (gSideTimers[SIDE(bank)].steelsurge > 0)
			dmg += CalcSteelsurgeDamagePartyMon(mon);

		if (gSideTimers[SIDE(bank)].spikesAmount > 0)
			dmg += CalcSpikesDamagePartyMon(mon, SIDE(bank));

		if (dmg >= GetBaseCurrentHP(bank))
			return TRUE;

		if (dmg >= GetBaseMaxHP(bank) / healthFraction) //More or equal than a quarter of max health
			return TRUE;
	}

	return FALSE;
}

u8 CountBanksPositiveStatStages(u8 bank)
{
	u8 positiveStages = 0;
	for (int i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) > 6)
			++positiveStages;
	}

	return positiveStages;
}

u8 CountBanksNegativeStatStages(u8 bank)
{
	u8 negativeStages = 0;
	for (int i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) < 6)
			++negativeStages;
	}

	return negativeStages;
}

u16 GetTeamSpeedAverage(u8 bank)
{
	u8 firstId, lastId;
	struct Pokemon* party = LoadPartyRange(bank, &firstId, &lastId);

	u8 totalNum = 0;
	u16 sum = 0;

	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		if (party[i].species == SPECIES_NONE
		|| GetMonData(&party[i], MON_DATA_IS_EGG, NULL))
			continue;

		++totalNum;
		sum += GetMonData(&party[i], MON_DATA_SPEED, NULL);
	}

	return sum / totalNum;
}

u16 GetPokemonOnSideSpeedAverage(u8 bank)
{
	u16 speed1 = 0;
	u16 speed2 = 0;
	u8 numBattlersAlive = 0;

	if (BATTLER_ALIVE(bank))
	{
		speed1 = SpeedCalc(bank);
		++numBattlersAlive;
	}

	if (IS_DOUBLE_BATTLE
	&&  BATTLER_ALIVE(PARTNER(bank)))
	{
		speed2 = SpeedCalc(PARTNER(bank));
		++numBattlersAlive;
	}

	return (speed1 + speed2) / numBattlersAlive;
}*/

u16 GetBattleMonMove(u8 bank, u8 i)
{
	u16 move;

	if (i >= MAX_MON_MOVES)
		return MOVE_NONE;

	#ifdef REALLY_SMART_AI
		move = gBattleMons[bank].moves[i];
	#else
		if (SIDE(bank) == B_SIDE_PLAYER && !(gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
			move = BATTLE_HISTORY->usedMoves[bank][i];
		else if (SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(bank) == B_POSITION_PLAYER_LEFT)
			move = BATTLE_HISTORY->usedMoves[bank][i];
		else
			move = gBattleMons[bank].moves[i];
	#endif

	if (IsDynamaxed(bank))
	{
		gBattleStruct->ai.zMoveHelper = move; //Store the original move in memory for damage calcs later
		move = GetMaxMove(bank, i);
	}

	return move;
}

u8 GetAIAbility(u8 bankAtk, u8 bankDef, u16 move)
{
	u8 ability = ABILITY_NONE;

	if (!ShouldAIDelayMegaEvolution(bankAtk, bankDef, move))
		ability = GetBankMegaFormAbility(bankAtk);

	if (ability == ABILITY_NONE)
		return ABILITY(bankAtk);

	return ability;
}

u8 GetPredictedAIAbility(u8 bankAtk, u8 bankDef)
{
	u16 predictedUserMove = IsValidMovePrediction(bankAtk, bankDef);
	if (predictedUserMove != MOVE_NONE)
		return GetAIAbility(bankAtk, bankDef, predictedUserMove);
	else
		return ABILITY(bankAtk);
}

u16 GetAIChosenMove(u8 bankAtk, u8 bankDef)
{
	u16 move = gChosenMoveByBattler[bankAtk];
	if (move == MOVE_NONE)
		move = IsValidMovePrediction(bankAtk, bankDef);
	else if (gBattleStruct->zMoveData.toBeUsed[bankAtk]) //Pokemon chose a Z-Move
		move = ReplaceWithZMoveRuntime(bankAtk, move);
	else if (IsDynamaxed(bankAtk) || gBattleStruct->dynamaxData.toBeUsed[bankAtk])
	{
		u16 maxMove = GetMaxMoveByMove(bankAtk, move);
		if (maxMove != MOVE_NONE)
			move = maxMove;
	}

	return move;
}

bool8 IsTrapped(u8 bank, bool8 switching)
{
	if (IsOfType(bank, TYPE_GHOST)
	|| (switching && ITEM_EFFECT(bank) == HOLD_EFFECT_SHED_SHELL)
	|| (!switching && ABILITY(bank) == ABILITY_RUNAWAY)
	|| (!switching && ITEM_EFFECT(bank) == HOLD_EFFECT_CAN_ALWAYS_RUN))
		return FALSE;
	else
	{
		if (gBattleMons[bank].status2 & (STATUS2_ESCAPE_PREVENTION | STATUS2_WRAPPED)
		|| (ABILITY_ON_OPPOSING_FIELD(bank, ABILITY_SHADOWTAG) && ABILITY(bank) != ABILITY_SHADOWTAG)
		|| (ABILITY_ON_OPPOSING_FIELD(bank, ABILITY_ARENATRAP) && CheckGrounding(bank) == GROUNDED)
		|| (ABILITY_ON_OPPOSING_FIELD(bank, ABILITY_MAGNETPULL) && IsOfType(bank, TYPE_STEEL))
		|| gStatuses3[bank] & (STATUS3_ROOTED | STATUS3_SKY_DROP_TARGET)
		|| IsFairyLockActive())
			return TRUE;
	}

	return FALSE;
}
/*
bool8 BankHasMonToSwitchTo(u8 bank)
{
	if (IS_SINGLE_BATTLE)
	{
		if (ViableMonCountFromBank(bank) <= 1)
			return FALSE;
	}
	else //Double
	{
		if (SIDE(bank) == B_SIDE_PLAYER)
		{
			if (IsTagBattle())
			{
				if (ViableMonCountFromBankLoadPartyRange(bank) <= 1)
					return FALSE;
			}
			else //Regular Double Battle
			{
				if (ViableMonCountFromBankLoadPartyRange(bank) <= 2)
					return FALSE;
			}
		}
		else //Opponent
		{
			if (IsTwoOpponentBattle())
			{
				if (ViableMonCountFromBankLoadPartyRange(bank) <= 1)
					return FALSE;
			}
			else //Regular Double Battle
			{
				if (ViableMonCountFromBankLoadPartyRange(bank) <= 2)
					return FALSE;
			}
		}
	}

	return TRUE;
}

bool8 IsTakingSecondaryDamage(u8 bank)
{
	u8 ability = ABILITY(bank);

	if (gStatuses3[bank] & STATUS3_PERISH_SONG)
		return TRUE;

	if (ability != ABILITY_MAGICGUARD)
	{
		if (TakesDamageFromSandstorm(bank)
		||  TakesDamageFromHail(bank)
		||  gWishFutureKnock.futureSightCounter[bank] == 1
		||  gStatuses3[bank] & STATUS3_LEECHSEED
		||  (gBattleMons[bank].status1 & STATUS1_PSN_ANY && ability != ABILITY_POISONHEAL)
		||  gBattleMons[bank].status1 & STATUS1_BURN
		||  ((gBattleMons[bank].status1 & STATUS1_SLEEP) > 1 && gBattleMons[bank].status2 & STATUS2_NIGHTMARE)
		||  gBattleMons[bank].status2 & (STATUS2_CURSED | STATUS2_WRAPPED)
		||	(BankSideHasSeaOfFire(bank) && !IsOfType(bank, TYPE_FIRE))
		||  (BankSideHasGMaxVineLash(bank) && !IsOfType(bank, TYPE_GRASS))
		||  (BankSideHasGMaxWildfire(bank) && !IsOfType(bank, TYPE_FIRE))
		||  (BankSideHasGMaxCannonade(bank) && !IsOfType(bank, TYPE_WATER))
		||  BankSideHasGMaxVolcalith(bank))
			return TRUE;
	}

	return FALSE;
}

bool8 WillFaintFromSecondaryDamage(u8 bank)
{
	u8 hp = gBattleMons[bank].hp + GetAmountToRecoverBy(bank, 0, MOVE_PROTECT); //Assume leftover etc. healing first
	u8 ability = ABILITY(bank);

	if (gStatuses3[bank] & STATUS3_PERISH_SONG
	&&  gDisableStructs[bank].perishSongTimer == 0)
		return TRUE;

	if (ability != ABILITY_MAGICGUARD)
	{
		u32 damage = 0;
		if (gWishFutureKnock.weatherDuration != 1)
			damage += GetSandstormDamage(bank)
				    + GetHailDamage(bank); //Weather's not about to end

		if ((gBattleMons[bank].status1 & STATUS1_SLEEP) != 1)
			damage += GetNightmareDamage(bank); //Sleep's not about to end

		if (!(gBattleStruct->brokeFreeMessage & gBitTable[bank]))
			damage += GetTrapDamage(bank); //Trapping isn't about to end

		if (damage
		+  GetLeechSeedDamage(bank)
		+  GetPoisonDamage(bank)
		+  GetBurnDamage(bank)
		+  GetCurseDamage(bank)
		+  GetSeaOfFireDamage(bank) //Sea of Fire runs on last turn
		+  GetGMaxVineLashDamage(bank)
		+  GetGMaxWildfireDamage(bank)
		+  GetGMaxCannonadeDamage(bank)
		+  GetGMaxVolcalithDamage(bank) >= hp)
			return TRUE;
	}

	return FALSE;
}
*/
u16 CalcSecondaryEffectChance(u8 bank, u16 move)
{
	u16 chance = gBattleMoves[move].secondaryEffectChance;

	if (ABILITY(bank) == ABILITY_SERENEGRACE || BankSideHasRainbow(bank))
		chance *= 2;

	return chance;
}

u16 CalcAIAccuracy(u16 move, u8 bankAtk, u8 bankDef)
{
	u16 acc = AccuracyCalc(move, bankAtk, bankDef);

	if (gBattleMoves[move].effect == EFFECT_RECHARGE)
		acc = 49; //Set recharge moves to a pretty low number only higher than OHKO moves
	else if (gBattleMoves[move].effect == EFFECT_RAMPAGE || gBattleMoves[move].effect == EFFECT_ROLLOUT)
		acc -= 1; //Don't rampage if you don't have to
	else if (move == MOVE_FINALGAMBIT)
		acc -= 5; //Don't kill yourself if you don't have to

	return acc;
}

bool8 ShouldAIDelayMegaEvolution(u8 bankAtk, u8 bankDef, u16 move)
{
	u8 atkAbility = ABILITY(bankAtk);
	u8 megaAbility = GetBankMegaFormAbility(bankAtk);

	if (BATTLER_SEMI_INVULNERABLE(bankAtk))
		return TRUE; //Can't Mega Evolve this turn

	if (atkAbility == megaAbility //Ability isn't changing
	||  megaAbility == ABILITY_NONE) //Can't Mega evolve or ability suppressed
		return FALSE;

	switch (atkAbility) {
		case ABILITY_SPEED_BOOST:
		case ABILITY_MOODY:
			switch (move) {
				case MOVE_PROTECT:
				case MOVE_DETECT:
				case MOVE_SPIKYSHIELD:
				case MOVE_KINGSSHIELD:
					return TRUE; //Delay Mega Evolution if using Protect for Speed Boost benefits
			}
			break;
	}

	return FALSE;
}

bool8 BadIdeaToPutToSleep(u8 bankDef, u8 bankAtk)
{
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 defAbility = ABILITY(bankDef);

	return !CanBePutToSleep(bankDef, TRUE)
		|| gStatuses3[bankDef] & STATUS3_YAWN
		|| defItemEffect == HOLD_EFFECT_CURE_SLP
		|| defItemEffect == HOLD_EFFECT_CURE_STATUS
		|| defAbility == ABILITY_EARLYBIRD
		|| defAbility == ABILITY_SHEDSKIN
		|| (defAbility == ABILITY_SYNCHRONIZE && CanBePutToSleep(bankAtk, TRUE))
		|| (defAbility == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1)
		|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)) && ABILITY(PARTNER(bankDef)) == ABILITY_HEALER);
}

bool8 BadIdeaToPoison(u8 bankDef, u8 bankAtk)
{
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 defAbility = ABILITY(bankDef);
	u8 atkAbility = ABILITY(bankAtk);

	return !CanBePoisoned(bankDef, bankAtk, TRUE)
		||  defItemEffect == HOLD_EFFECT_CURE_PSN
		||  defItemEffect == HOLD_EFFECT_CURE_STATUS
		||  defAbility == ABILITY_SHEDSKIN
		||  defAbility == ABILITY_POISONHEAL
		||  defAbility == ABILITY_MAGICGUARD
		||  defAbility == ABILITY_QUICKFEET
		|| (defAbility == ABILITY_SYNCHRONIZE && CanBePoisoned(bankAtk, bankDef, TRUE) && !GoodIdeaToPoisonSelf(bankAtk))
		|| (defAbility == ABILITY_MARVELSCALE && PhysicalMoveInMoveset(bankAtk))
		|| (defAbility == ABILITY_NATURALCURE && CAN_SWITCH_OUT(bankDef))
		|| (defAbility == ABILITY_TOXICBOOST && PhysicalMoveInMoveset(bankDef))
		|| (defAbility == ABILITY_GUTS && PhysicalMoveInMoveset(bankDef))
		|| (atkAbility == ABILITY_POISONTOUCH && ContactMovesThatAffectTargetInMoveset(bankAtk, bankDef)) //Just poison it using attacker's ability
		|| (defAbility == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1)
		|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)) && ABILITY(PARTNER(bankDef)) == ABILITY_HEALER)
		||  MoveInMoveset(MOVE_FACADE, bankDef)
		||  MoveInMoveset(MOVE_PSYCHOSHIFT, bankDef);
}

bool8 GoodIdeaToPoisonSelf(u8 bankAtk)
{
	u8 atkAbility = ABILITY(bankAtk);

	return CanBePoisoned(bankAtk, bankAtk, FALSE)
		&&  (atkAbility == ABILITY_MARVELSCALE
		 ||  atkAbility == ABILITY_POISONHEAL
		 ||  atkAbility == ABILITY_QUICKFEET
		 ||  atkAbility == ABILITY_MAGICGUARD
		 || (atkAbility == ABILITY_TOXICBOOST && PhysicalMoveInMoveset(bankAtk))
		 || (atkAbility == ABILITY_GUTS && PhysicalMoveInMoveset(bankAtk))
		 ||  MoveInMoveset(MOVE_FACADE, bankAtk)
		 ||  MoveInMoveset(MOVE_PSYCHOSHIFT, bankAtk));
}

bool8 BadIdeaToParalyze(u8 bankDef, u8 bankAtk)
{
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 defAbility = ABILITY(bankDef);

	return !CanBeParalyzed(bankDef, TRUE)
	   ||  defItemEffect == HOLD_EFFECT_CURE_PAR
	   ||  defItemEffect == HOLD_EFFECT_CURE_STATUS
	   ||  defAbility == ABILITY_SHEDSKIN
	   ||  defAbility == ABILITY_QUICKFEET
	   || (defAbility == ABILITY_SYNCHRONIZE && CanBeParalyzed(bankAtk, TRUE) && !GoodIdeaToParalyzeSelf(bankAtk))
	   || (defAbility == ABILITY_MARVELSCALE && PhysicalMoveInMoveset(bankAtk))
	   || (defAbility == ABILITY_NATURALCURE && CAN_SWITCH_OUT(bankDef))
	   || (defAbility == ABILITY_GUTS && PhysicalMoveInMoveset(bankDef))
	   || (defAbility == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1)
	   || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)) && ABILITY(PARTNER(bankDef)) == ABILITY_HEALER)
	   ||  MoveInMoveset(MOVE_FACADE, bankDef)
	   ||  MoveInMoveset(MOVE_PSYCHOSHIFT, bankDef);
}

bool8 GoodIdeaToParalyzeSelf(u8 bankAtk)
{
	u8 atkAbility = ABILITY(bankAtk);

	return CanBeParalyzed(bankAtk, FALSE)
		&&  (atkAbility == ABILITY_MARVELSCALE
		 ||  atkAbility == ABILITY_QUICKFEET
		 || (atkAbility == ABILITY_GUTS && PhysicalMoveInMoveset(bankAtk))
		 || MoveInMoveset(MOVE_FACADE, bankAtk)
		 || MoveInMoveset(MOVE_PSYCHOSHIFT, bankAtk));
}

bool8 BadIdeaToBurn(u8 bankDef, u8 bankAtk)
{
	u8 defItemEffect = ITEM_EFFECT(bankDef);
	u8 defAbility = ABILITY(bankDef);

	return !CanBeBurned(bankDef, TRUE)
		||  defItemEffect == HOLD_EFFECT_CURE_BRN
		||  defItemEffect == HOLD_EFFECT_CURE_STATUS
		||  defAbility == ABILITY_SHEDSKIN
		||  defAbility == ABILITY_MAGICGUARD
		||  defAbility == ABILITY_QUICKFEET
		|| (defAbility == ABILITY_SYNCHRONIZE && CanBeBurned(bankAtk, TRUE) && !GoodIdeaToBurnSelf(bankAtk))
		|| (defAbility == ABILITY_MARVELSCALE && PhysicalMoveInMoveset(bankAtk))
		|| (defAbility == ABILITY_NATURALCURE && CAN_SWITCH_OUT(bankDef))
		|| (defAbility == ABILITY_FLAREBOOST && SpecialMoveInMoveset(bankDef))
		|| (defAbility == ABILITY_GUTS && PhysicalMoveInMoveset(bankDef))
		|| (defAbility == ABILITY_HYDRATION && gBattleWeather & WEATHER_RAIN_ANY && gWishFutureKnock.weatherDuration != 1)
		|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)) && ABILITY(PARTNER(bankDef)) == ABILITY_HEALER)
		||  MoveInMoveset(MOVE_FACADE, bankDef)
		||  MoveInMoveset(MOVE_PSYCHOSHIFT, bankDef);
}

bool8 GoodIdeaToBurnSelf(u8 bankAtk)
{
	u8 atkAbility = ABILITY(bankAtk);

	return CanBeBurned(bankAtk, FALSE)
		&&  (atkAbility == ABILITY_QUICKFEET
		 ||  atkAbility == ABILITY_HEATPROOF
		 ||  atkAbility == ABILITY_MAGICGUARD
		 || (atkAbility == ABILITY_FLAREBOOST && SpecialMoveInMoveset(bankAtk))
		 || (atkAbility == ABILITY_GUTS && PhysicalMoveInMoveset(bankAtk))
		 || MoveInMoveset(MOVE_FACADE, bankAtk)
		 || MoveInMoveset(MOVE_PSYCHOSHIFT, bankAtk));
}

bool8 BadIdeaToFreeze(u8 bankDef, u8 bankAtk)
{
	u8 defAbility = ABILITY(bankDef);
	u8 defItemEffect = ITEM_EFFECT(bankDef);

	return !CanBeFrozen(bankDef, TRUE)
		|| defItemEffect != HOLD_EFFECT_CURE_FRZ
		|| defItemEffect != HOLD_EFFECT_CURE_STATUS
		|| (defAbility == ABILITY_SYNCHRONIZE && CanBeFrozen(bankAtk, TRUE))
		|| (defAbility == ABILITY_NATURALCURE && CAN_SWITCH_OUT(bankDef)) //Don't waste a one-time freeze
		|| UnfreezingMoveInMoveset(bankDef);
}

bool8 GoodIdeaToLowerAttack(u8 bankDef, u8 bankAtk, u16 move)
{
	u8 defAbility = ABILITY(bankDef);
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	return STAT_STAGE(bankDef, STAT_STAGE_ATK) > 4 && PhysicalMoveInMoveset(bankDef)
		&& defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		&& defAbility != ABILITY_WHITESMOKE
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_HYPERCUTTER;
}

bool8 GoodIdeaToLowerDefense(u8 bankDef, u8 bankAtk, u16 move)
{
	u8 defAbility = ABILITY(bankDef);

	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	return STAT_STAGE(bankDef, STAT_STAGE_DEF) > 4
		&& PhysicalMoveInMoveset(bankAtk)
		&& defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		&& defAbility != ABILITY_WHITESMOKE
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_BIGPECKS;
}

bool8 GoodIdeaToLowerSpAtk(u8 bankDef, u8 bankAtk, u16 move)
{
	u8 defAbility = ABILITY(bankDef);

	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	return STAT_STAGE(bankDef, STAT_STAGE_SPATK) > 4 && SpecialMoveInMoveset(bankDef)
		&& defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_WHITESMOKE;
}

bool8 GoodIdeaToLowerSpDef(u8 bankDef, u8 bankAtk, u16 move)
{
	u8 defAbility = ABILITY(bankDef);
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	return STAT_STAGE(bankDef, STAT_STAGE_SPDEF) > 4 && SpecialMoveInMoveset(bankAtk)
		&& defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_WHITESMOKE;
}

bool8 GoodIdeaToLowerSpeed(u8 bankDef, u8 bankAtk, u16 move)
{
	u8 defAbility = ABILITY(bankDef);
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	return SpeedCalc(bankAtk) <= SpeedCalc(bankDef)
		&& defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_WHITESMOKE;
}

bool8 GoodIdeaToLowerAccuracy(u8 bankDef, u8 bankAtk, u16 move)
{
	u8 defAbility = ABILITY(bankDef);
	if (!MoveWouldHitFirst(move, bankAtk, bankDef) && CanKnockOut(bankAtk, bankDef))
		return FALSE; //Don't bother lowering stats if can kill enemy.

	return defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		&& defAbility != ABILITY_WHITESMOKE
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_KEENEYE;
}

bool8 GoodIdeaToLowerEvasion(u8 bankDef, u8 bankAtk, u16 move)
{
	u8 defAbility = ABILITY(bankDef);

	return (STAT_STAGE(bankDef, STAT_STAGE_EVASION) > 6 || MoveInMovesetWithAccuracyLessThan(bankAtk, bankDef, 90, TRUE))
		&& defAbility != ABILITY_CONTRARY
		&& defAbility != ABILITY_CLEARBODY
		//&& defAbility != ABILITY_FULLMETALBODY
		&& defAbility != ABILITY_WHITESMOKE;
}

//Move Prediction Code

u16 IsValidMovePrediction(u8 bankAtk, u8 bankDef)
{
	if (gBattleStruct->ai.movePredictions[bankAtk][bankDef] == MOVE_PREDICTION_SWITCH)
		return MOVE_NONE;
	else
		return gBattleStruct->ai.movePredictions[bankAtk][bankDef];
}
/*
bool8 IsPredictedToSwitch(u8 bankAtk, u8 bankDef)
{
	return gBattleStruct->ai.movePredictions[bankAtk][bankDef] == MOVE_PREDICTION_SWITCH;
}

void StoreMovePrediction(u8 bankAtk, u8 bankDef, u16 move)
{
	gBattleStruct->ai.movePredictions[bankAtk][bankDef] = move;
}

void StoreSwitchPrediction(u8 bankAtk, u8 bankDef)
{
	gBattleStruct->ai.movePredictions[bankAtk][bankDef] = MOVE_PREDICTION_SWITCH;
}

bool8 IsMovePredictionSemiInvulnerable(u8 bankAtk, u8 bankDef)
{
	u16 move = IsValidMovePrediction(bankAtk, bankDef);

	if (move != MOVE_NONE)
	{
		u8 effect = gBattleMoves[move].effect;
		return effect == EFFECT_SEMI_INVULNERABLE;

	}

	return FALSE;
}

bool8 IsMovePredictionHealingMove(u8 bankAtk, u8 bankDef)
{
	u16 move = IsValidMovePrediction(bankAtk, bankDef);

	if (move != MOVE_NONE)
	{
		u8 effect = gBattleMoves[move].effect;
		return effect == EFFECT_RESTORE_HP
			|| effect == EFFECT_MORNING_SUN
			|| effect == EFFECT_SWALLOW
			|| effect == EFFECT_WISH;
	}

	return FALSE;
}

bool8 IsPredictedToUsePursuitableMove(u8 bankAtk, u8 bankDef)
{
	u16 move = IsValidMovePrediction(bankAtk, bankDef);

	if (move != MOVE_NONE)
	{
		u8 effect = gBattleMoves[move].effect;
		return effect == EFFECT_BATON_PASS && move != MOVE_BATONPASS;
	}

	return FALSE;
}

bool8 IsMovePredictionPhazingMove(u8 bankAtk, u8 bankDef)
{
	u16 move = IsValidMovePrediction(bankAtk, bankDef);

	if (IsDynamaxed(bankDef))
		return FALSE; //Dynamax Pokemon can't be phazed out

	if (move != MOVE_NONE)
	{
		u8 effect = gBattleMoves[move].effect;
		return effect == EFFECT_ROAR || effect == EFFECT_HAZE || effect == EFFECT_REMOVE_TARGET_STAT_CHANGES;
	}

	return FALSE;
}
*/
//bankAtk is the protector
bool8 CanMovePredictionProtectAgainstMove(u8 bankAtk, u8 bankDef, u16 move)
{
	u16 protectMove = IsValidMovePrediction(bankAtk, bankDef);

	if (move != MOVE_NONE && gBattleMoves[protectMove].effect == EFFECT_PROTECT)
	{
		return DoesProtectionMoveBlockMove(bankDef, bankAtk, move, protectMove); //The "target" is the one not using Protection move and attacking
	}

	return FALSE;
}
/*

bool8 DamagingMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (SPLIT(move) != SPLIT_STATUS
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_COUNTER
			&& gBattleMoves[move].effect != EFFECT_MIRROR_COAT)
				return TRUE;
		}
	}

	return FALSE;
}*/

bool8 PhysicalMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);
	int i;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalcMoveSplit(bank, move) == SPLIT_PHYSICAL
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_COUNTER)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 SpecialMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);
	int i;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalcMoveSplit(bank, move) == SPLIT_SPECIAL
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_MIRROR_COAT)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 MoveSplitInMoveset(u8 bank, u8 moveSplit)
{
	switch (moveSplit) {
		case SPLIT_PHYSICAL:
			return PhysicalMoveInMoveset(bank);
		case SPLIT_SPECIAL:
			return SpecialMoveInMoveset(bank);
		default:
			return !PhysicalMoveInMoveset(bank) && !SpecialMoveInMoveset(bank);
	}
}
/*
bool8 PhysicalMoveInMonMoveset(struct Pokemon* mon, u8 moveLimitations)
{
	u16 move;

	moveLimitations = CheckMoveLimitationsFromParty(mon, 0, moveLimitations);
	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalcMoveSplitFromParty(mon, move) == SPLIT_PHYSICAL
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_COUNTER)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 SpecialMoveInMonMoveset(struct Pokemon* mon, u8 moveLimitations)
{
	u16 move;

	moveLimitations = CheckMoveLimitationsFromParty(mon, 0, moveLimitations);
	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalcMoveSplitFromParty(mon, move) == SPLIT_SPECIAL
			&& gBattleMoves[move].power != 0
			&& gBattleMoves[move].effect != EFFECT_MIRROR_COAT)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 MagicCoatableMovesInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].flags & FLAG_MAGIC_COAT_AFFECTED)
				return TRUE;
		}
	}
	return FALSE;
}
*/
bool8 HasProtectionMoveInMoveset(u8 bank, u8 checkType)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);
	int i;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[gBattleMons[bank].moves[i]].effect == EFFECT_PROTECT)
			{
				switch (move) {
					case MOVE_PROTECT:
					case MOVE_SPIKYSHIELD:
					case MOVE_KINGSSHIELD:
						return TRUE;

					case MOVE_QUICKGUARD:
						if (checkType & CHECK_QUICK_GUARD)
							return TRUE;
						break;

					case MOVE_WIDEGUARD:
						if (checkType & CHECK_WIDE_GUARD)
							return TRUE;
						break;

					case MOVE_CRAFTYSHIELD:
						if (checkType & CHECK_CRAFTY_SHIELD)
							return TRUE;
						break;

					case MOVE_MATBLOCK:
						if (checkType & CHECK_MAT_BLOCK
						&&  gDisableStructs[bank].isFirstTurn)
							return TRUE;
						break;
				}
			}
		}
	}

	return FALSE;
}
/*
// AI function to check if any move is of given type
bool8 MoveTypeInMoveset(u8 bank, u8 moveType)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (GetMoveTypeSpecial(bank, move) == moveType)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 DamagingMoveTypeInMoveset(u8 bank, u8 moveType)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (GetMoveTypeSpecial(bank, move) == moveType
			&&  SPLIT(move) != SPLIT_STATUS)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 DamagingAllHitMoveTypeInMoveset(u8 bank, u8 moveType)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (GetMoveTypeSpecial(bank, move) == moveType
			&&  SPLIT(move) != SPLIT_STATUS
			&&  gBattleMoves[move].target & MOVE_TARGET_FOES_AND_ALLY)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 DamagingSpreadMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (SPLIT(move) != SPLIT_STATUS
			&&  gBattleMoves[move].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY))
				return TRUE;
		}
	}

	return FALSE;
}

// AI Function to check if bank has a snatchable move in moveset
bool8 HasSnatchableMove(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].flags & FLAG_SNATCH_AFFECTED)
				return TRUE;
		}
	}

	return FALSE;
}

// AI function to see if bank has a move with specific move effect
bool8 MoveEffectInMoveset(u8 moveEffect, u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].effect == moveEffect)
				return TRUE;
		}
	}

	return FALSE;
}

//AI function to check if bank has a status move in their moveset
bool8 StatusMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CalcMoveSplit(bank, move) == SPLIT_STATUS)
				return TRUE;
		}
	}

	return FALSE;
}
*/
bool8 MoveInMovesetWithAccuracyLessThan(u8 bankAtk, u8 bankDef, u8 acc, bool8 ignoreStatusMoves)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);
	int i;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (ignoreStatusMoves && SPLIT(move) == SPLIT_STATUS)
				continue;

			if (gBattleMoves[move].accuracy == 0 //Always hits
			||  gBattleMoves[move].target & (MOVE_TARGET_USER | MOVE_TARGET_OPPONENTS_FIELD))
				continue;

			if (AccuracyCalc(move, bankAtk, bankDef) < acc)
				return TRUE;
		}
	}

	return FALSE;
}
/*
bool8 FlinchingMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].effect == EFFECT_FLINCH_HIT)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 HealingMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			u8 effect = gBattleMoves[move].effect;
			if (effect == EFFECT_RESTORE_HP
			||  effect == EFFECT_MORNING_SUN
			||  effect == EFFECT_SWALLOW
			||  effect == EFFECT_WISH)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 HealingMoveInMonMoveset(struct Pokemon* mon)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitationsFromParty(mon, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			u8 effect = gBattleMoves[move].effect;
			if (effect == EFFECT_RESTORE_HP
			||  effect == EFFECT_MORNING_SUN
			||  effect == EFFECT_SWALLOW
			||  effect == EFFECT_WISH)
				return TRUE;
		}
	}

	return FALSE;
}

bool8 SoundMoveInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CheckSoundMove(move))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 MoveThatCanHelpAttacksHitInMoveset(u8 bank)
{
	u16 move;
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bank, i);
		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			switch (gBattleMoves[move].effect) {
				case EFFECT_ACCURACY_UP:
				case EFFECT_ACCURACY_UP_2:
				case EFFECT_EVASION_DOWN:
				case EFFECT_EVASION_DOWN_2:
					return TRUE;

				case EFFECT_LOCK_ON:
					if (move != MOVE_LASERFOCUS)
						return TRUE;
					break;
			}

			switch (move) {
				case MOVE_HONECLAWS:
				case MOVE_COIL:
				case MOVE_DEFOG:
					return TRUE;
			}
		}
	}

	return FALSE;
}

bool8 DamagingMoveThaCanBreakThroughSubstituteInMoveset(u8 bankAtk, u8 bankDef)
{
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (SPLIT(move) != SPLIT_STATUS //Damaging move
			&& !MoveBlockedBySubstitute(move, bankAtk, bankDef))
				return TRUE;
		}
	}

	return FALSE;
}
*/
bool8 ContactMovesThatAffectTargetInMoveset(u8 bankAtk, u8 bankDef)
{
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);
	int i ;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CheckContact(move, bankAtk)
			&& !(AI_SpecialTypeCalc(move, bankDef, bankAtk) & MOVE_RESULT_NO_EFFECT))
				return TRUE;
		}
	}

	return FALSE;
}
/*
bool8 UnfreezingMoveInMoveset(u8 bank)
{
	u8 moveLimitations = CheckMoveLimitations(bank, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bank, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (CheckTableForMove(move, gMovesCanUnfreezeAttacker))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 SleepMoveInMovesetWithLowAccuracy(u8 bankAtk, u8 bankDef)
{
	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);

	for (int i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			if (gBattleMoves[move].effect == EFFECT_SLEEP && AccuracyCalc(move, bankAtk, bankDef) < 85)
				return TRUE;
		}
	}

	return FALSE;
}

static bool8 WallsFoe(u8 bankAtk, u8 bankDef)
{
	u32 attack, spAttack;
	bool8 cantWall = FALSE;

	APPLY_STAT_MOD(attack, &gBattleMons[bankAtk], gBattleMons[bankAtk].attack, STAT_STAGE_ATK);
	APPLY_STAT_MOD(spAttack, &gBattleMons[bankAtk], gBattleMons[bankAtk].spAttack, STAT_STAGE_SPATK);

	if (PhysicalMoveInMoveset(bankAtk) && gBattleMons[bankDef].defense <= attack)
		cantWall = TRUE;
	else if (SpecialMoveInMoveset(bankAtk) && gBattleMons[bankDef].spDefense <= spAttack)
		cantWall = TRUE;

	return !cantWall;
}

static bool8 ShouldAIFreeChoiceLockWithDynamax(u8 bankAtk, u8 bankDef)
{
	if (CanDynamax(bankAtk)
	&& (ITEM_EFFECT(bankAtk) == ITEM_EFFECT_CHOICE_BAND || ABILITY(bankAtk) == ABILITY_GORILLATACTICS)
	&& CHOICED_MOVE(bankAtk) != MOVE_NONE && CHOICED_MOVE(bankAtk) != 0xFFFF) //AI is locked into some move
	{
		u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, MOVE_LIMITATION_ZEROMOVE | MOVE_LIMITATION_PP);
		struct AIScript aiScriptData = {0};
		PopulateAIScriptStructWithBaseAttackerData(&aiScriptData, bankAtk);
		PopulateAIScriptStructWithBaseDefenderData(&aiScriptData, bankDef);
		for (int i = 0; i < MAX_MON_MOVES; ++i)
		{
			u16 move = GetMaxMove(bankAtk, i);
			if (move == MOVE_NONE)
				break;

			if (!(gBitTable[i] & moveLimitations))
			{
				if (AIScript_Negatives(bankAtk, bankDef, move, 100, &aiScriptData) >= 100)
					return TRUE;
			}
		}
	}

	return FALSE; //AI still has no usable moves after Dynamaxing
}

static bool8 CalcOnlyBadMovesLeftInMoveset(u8 bankAtk, u8 bankDef)
{
	int i;
	u8 viability;
	u16 move;
	u32 dmg;
	u8 numDamageMoves = 0;
	bool8 hasSuperEffectiveMove = FALSE;

	if (AI_THINKING_STRUCT->aiFlags == AI_SCRIPT_CHECK_BAD_MOVE) //Only basic AI
		return FALSE; //The dumb AI doesn't get to switch like this

	u8 moveLimitations = CheckMoveLimitations(bankAtk, 0, 0xFF);
	struct AIScript aiScriptData = {0};
	PopulateAIScriptStructWithBaseAttackerData(&aiScriptData, bankAtk);
	PopulateAIScriptStructWithBaseDefenderData(&aiScriptData, bankDef);
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		move = GetBattleMonMove(bankAtk, i);

		if (move == MOVE_NONE)
			break;

		if (!(gBitTable[i] & moveLimitations))
		{
			viability = AIScript_Negatives(bankAtk, bankDef, move, 100, &aiScriptData);
			if (viability >= 100)
			{
				if (SPLIT(move) == SPLIT_STATUS)
					return FALSE; //Has viable status move

				if (gBattleMoves[move].effect == EFFECT_KNOCK_OFF
				&&  CanKnockOffItem(bankDef))
					return FALSE; //Using Knock-Off is good even if it does minimal damage

				if (TypeCalc(move, bankAtk, bankDef, NULL, FALSE) & MOVE_RESULT_SUPER_EFFECTIVE)
					hasSuperEffectiveMove = TRUE;

				++numDamageMoves; //By default can't be a status move here
			}
		}
	}

	if (IsDoubleBattle() || !MoveEffectInMoveset(EFFECT_PROTECT, bankAtk)) //Single battle protector usually protect to stall
	{
		if (numDamageMoves == 0)
		{
			if (IS_BEHIND_SUBSTITUTE(bankAtk)
			&& !DamagingMoveThaCanBreakThroughSubstituteInMoveset(bankDef, bankAtk)
			&& IsTakingSecondaryDamage(bankDef))
				return FALSE; //Don't switch out on a foe who's taking damage even if you literally have no good moves

			if (ShouldAIFreeChoiceLockWithDynamax(bankAtk, bankDef)) //AI is locked into some move
			{
				gBattleStruct->ai.dynamaxMonId[SIDE(bankAtk)] = gBattlerPartyIndexes[bankAtk]; //Dynamax this mon and free the choice lock
				gBattleStruct->ai.shouldFreeChoiceLockWithDynamax[bankAtk][bankDef] = TRUE;
				return FALSE;
			}

			return TRUE; //Legit no moves left
		}
		else if (IS_BEHIND_SUBSTITUTE(bankAtk)
		&& !DamagingMoveThaCanBreakThroughSubstituteInMoveset(bankDef, bankAtk))
		{
			return FALSE; //Don't switch if hiding legitimately behind substitute
		}
		else if (!MoveEffectInMoveset(EFFECT_BATON_PASS, bankAtk) //U-Turn/Volt Switch switch on their own
		&& (ABILITY(bankDef) == ABILITY_MAGICGUARD || !(gBattleMons[bankDef].status2 & STATUS2_WRAPPED)) //Only switch if the enemy isn't taking trap damage
		&& (!IsTrapped(bankDef, TRUE) || !(gStatuses3[bankDef] & STATUS3_PERISH_SONG) || gDisableStructs[bankDef].perishSongTimer == 0) //Don't let a trapped foe escape from Perish Song
        ) //Only 1 viable damaging move left
		{
			move = GetStrongestMove(bankAtk, bankDef); //Assume the usuable damage move is the strongest move
			dmg = GetFinalAIMoveDamage(move, bankAtk, bankDef, 1, NULL);

			if (dmg < gBattleMons[bankDef].hp) //Move doesn't KO
			{
				u16 leftoversRecovery = GetAmountToRecoverBy(bankDef, bankAtk, MOVE_PROTECT);
				if (leftoversRecovery != 0  //Target can heal itself with ability/item
				||  HealingMoveInMoveset(bankDef)) //Target can heal itself with move
				{
					if (dmg * 3 >= gBattleMons[bankDef].hp + ((u32) leftoversRecovery * 2)) //Damage deals a third or more of remaining health after leftovers recovery
						return FALSE; //Don't switch if can 2-3HKO
				}
				else
				{
					if (dmg * 3 >= gBattleMons[bankDef].hp) //Damage deals a third or more of remaining health
						return FALSE; //Don't switch if can 3HKO
				}

				if (ShouldAIFreeChoiceLockWithDynamax(bankAtk, bankDef)) //AI is locked into some move
				{
					gBattleStruct->ai.dynamaxMonId[SIDE(bankAtk)] = gBattlerPartyIndexes[bankAtk]; //Dynamax this mon and free the choice lock
					gBattleStruct->ai.shouldFreeChoiceLockWithDynamax[bankAtk][bankDef] = TRUE;
					return FALSE;
				}

				u8 backupActiveBattler = gActiveBattler;
				gActiveBattler = bankAtk;
				if (GetMostSuitableMonToSwitchIntoScore() & SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE)
				{
					gActiveBattler = backupActiveBattler;
					if (hasSuperEffectiveMove)
						return FALSE; //Don't switch to another Pokemon just because they have a Super-Effective move if you have one too.

					if (GetMostSuitableMonToSwitchIntoScore() == SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE
					&& WallsFoe(bankAtk, bankDef))
						return FALSE; //Tough it out
				}
				else if (GetMostSuitableMonToSwitchIntoScore() < SWITCHING_INCREASE_HAS_SUPER_EFFECTIVE_MOVE)
				{
					gActiveBattler = backupActiveBattler;
					return FALSE;
				}
				else if (GetMostSuitableMonToSwitchIntoScore() == SWITCHING_INCREASE_WALLS_FOE)
				{
					gActiveBattler = backupActiveBattler;
					//Check if current mon can't wall
					bool8 canWall = WallsFoe(bankAtk, bankDef);

					if (canWall) //Current mon can wall
						return FALSE; //Tough it out
				}
				else
					gActiveBattler = backupActiveBattler;
			}
			else
				return FALSE; //Don't switch if you can KO the opponent

			return TRUE;
		}
	}

	return FALSE; //Has some moves to use
}

bool8 OnlyBadMovesLeftInMoveset(u8 bankAtk, u8 bankDef)
{
	if (gBattleStruct->ai.onlyBadMovesLeft[bankAtk][bankDef] == 0xFF)
		gBattleStruct->ai.onlyBadMovesLeft[bankAtk][bankDef] = CalcOnlyBadMovesLeftInMoveset(bankAtk, bankDef);

	return gBattleStruct->ai.onlyBadMovesLeft[bankAtk][bankDef];
}
*/
u16 TryReplaceMoveWithZMove(u8 bankAtk, u8 bankDef, u16 move)
{
	if (IsAnyMaxMove(move))
		return move;

	if (!gBattleStruct->zMoveData.used[bankAtk] && SPLIT(move) != SPLIT_STATUS
	&& ShouldAIUseZMove(bankAtk, bankDef, move))
	{
		u8 moveIndex = FindMovePositionInMoveset(move, bankAtk);
		if (moveIndex < MAX_MON_MOVES)
		{
			u16 zMove = CanUseZMove(bankAtk, moveIndex, move);
			if (zMove != MOVE_NONE)
			{
				gBattleStruct->ai.zMoveHelper = move; //Store the original move in memory for damage calcs later
				move = zMove;
			}
		}
	}
	else if (IsDynamaxed(bankAtk) || (!gBattleStruct->dynamaxData.used[bankAtk] && ShouldAIDynamax(bankAtk, bankDef)))
	{
		u16 maxMove;
		if (IsRaidBattle() && bankAtk == BANK_RAID_BOSS && IsRaidBossUsingRegularMove(bankAtk, move))
			return move; //This turn the raid boss isn't using a Max Move

		maxMove = GetMaxMoveByMove(bankAtk, move);
		if (maxMove != MOVE_NONE)
		{
			gBattleStruct->ai.zMoveHelper = move; //Store the original move in memory for damage calcs later
			move = maxMove;
		}
	}

	return move;
}
/*
u8 GetAIMoveEffectForMaxMove(u16 move, u8 bankAtk, u8 bankDef)
{
	u8 maxEffect = gBattleMoves[move].z_move_effect;
	u8 moveEffect = 0;

	if (move == MOVE_MAX_GUARD)
		return EFFECT_PROTECT;

	switch (maxEffect) {
		case MAX_EFFECT_RAISE_TEAM_ATTACK:
		case MAX_EFFECT_RAISE_TEAM_DEFENSE:
		case MAX_EFFECT_RAISE_TEAM_SPEED:
		case MAX_EFFECT_RAISE_TEAM_SP_ATK:
		case MAX_EFFECT_RAISE_TEAM_SP_DEF:
			if (STAT_STAGE(bankAtk, (maxEffect - MAX_EFFECT_RAISE_TEAM_ATTACK) + 1) < STAT_STAGE_MAX
			|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankAtk)) && STAT_STAGE(PARTNER(bankAtk), (maxEffect - MAX_EFFECT_RAISE_TEAM_ATTACK) + 1) < STAT_STAGE_MAX))
				moveEffect = (maxEffect - MAX_EFFECT_RAISE_TEAM_ATTACK) + EFFECT_ATTACK_UP;
			break;

		case MAX_EFFECT_LOWER_ATTACK:
		case MAX_EFFECT_LOWER_DEFENSE:
		case MAX_EFFECT_LOWER_SPEED:
		case MAX_EFFECT_LOWER_SP_ATK:
		case MAX_EFFECT_LOWER_SP_DEF:
			if (STAT_STAGE(bankDef, (maxEffect - MAX_EFFECT_LOWER_ATTACK) + 1) > STAT_STAGE_MIN)
				moveEffect = (maxEffect - MAX_EFFECT_LOWER_ATTACK) + EFFECT_ATTACK_DOWN;
			break;

		case MAX_EFFECT_SUN:
			if (!(gBattleWeather & (WEATHER_SUN_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
				moveEffect = EFFECT_SUNNY_DAY;
			break;
		case MAX_EFFECT_RAIN:
			if (!(gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
				moveEffect = EFFECT_RAIN_DANCE;
			break;
		case MAX_EFFECT_SANDSTORM:
			if (!(gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
				moveEffect = EFFECT_SANDSTORM;
			break;
		case MAX_EFFECT_HAIL:
			if (!(gBattleWeather & (WEATHER_HAIL_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
				moveEffect = EFFECT_HAIL;
			break;

		case MAX_EFFECT_ELECTRIC_TERRAIN:
		case MAX_EFFECT_GRASSY_TERRAIN:
		case MAX_EFFECT_MISTY_TERRAIN:
		case MAX_EFFECT_PSYCHIC_TERRAIN:
			if (!(gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN)
			&& gTerrainType != (maxEffect - MAX_EFFECT_ELECTRIC_TERRAIN) + 1)
				moveEffect = EFFECT_SET_TERRAIN;
			break;

		case MAX_EFFECT_VINE_LASH:
			//TODO AI
			break;

		case MAX_EFFECT_WILDFIRE:
			//TODO AI
			break;

		case MAX_EFFECT_CANNONADE:
			//TODO AI
			break;

		case MAX_EFFECT_EFFECT_SPORE_FOES:
			//TODO AI
			break;
		case MAX_EFFECT_POISON_PARALYZE_FOES:
			//TODO AI
			break;
		case MAX_EFFECT_PARALYZE_FOES:
			if (CanBeParalyzed(bankDef, TRUE))
				moveEffect = EFFECT_PARALYZE;
			break;
		case MAX_EFFECT_POISON_FOES:
			if (CanBePoisoned(bankDef, bankAtk, TRUE))
				moveEffect = EFFECT_POISON;
			break;
		case MAX_EFFECT_CONFUSE_FOES_PAY_DAY:
		case MAX_EFFECT_CONFUSE_FOES:
			if (CanBeConfused(bankDef, TRUE))
				moveEffect = EFFECT_CONFUSE;
			break;

		case MAX_EFFECT_CRIT_PLUS:
			if (!(gBattleMons[bankAtk].status2 & STATUS2_FOCUS_ENERGY))
				moveEffect = EFFECT_FOCUS_ENERGY;
			break;

		case MAX_EFFECT_MEAN_LOOK:
			if (!IsTrapped(bankDef, TRUE))
				moveEffect = EFFECT_MEAN_LOOK;
			break;

		case MAX_EFFECT_AURORA_VEIL:
			if (gBattleStruct->AuroraVeilTimers[SIDE(bankAtk)] == 0)
				moveEffect = EFFECT_REFLECT;
			break;

		case MAX_EFFECT_INFATUATE_FOES:
			if (CanBeInfatuated(bankDef, bankAtk))
				moveEffect = EFFECT_ATTRACT;
			break;

		case MAX_EFFECT_RECYCLE_BERRIES:
			if (SAVED_CONSUMED_ITEMS(bankAtk) != ITEM_NONE
			&& IsBerry(SAVED_CONSUMED_ITEMS(bankAtk))
			&& ITEM(bankAtk) == ITEM_NONE)
				moveEffect = EFFECT_RECYCLE;
			break;

		case MAX_EFFECT_STEALTH_ROCK:
			if (gSideTimers[SIDE(bankDef)].srAmount == 0)
				moveEffect = EFFECT_SPIKES;
			break;
		case MAX_EFFECT_STEELSURGE:
			if (gSideTimers[SIDE(bankDef)].steelsurge == 0)
				moveEffect = EFFECT_SPIKES;
			break;

		case MAX_EFFECT_DEFOG:
			if (gSideStatuses[SIDE(bankDef)] & (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN | SIDE_STATUS_SAFEGUARD | SIDE_STATUS_MIST)
			|| gBattleStruct->AuroraVeilTimers[SIDE(bankDef)] != 0
			|| gSideStatuses[SIDE(bankAtk)] & SIDE_STATUS_SPIKES
			|| !(gSideStatuses[SIDE(bankDef)] & SIDE_STATUS_SPIKES))
				moveEffect = EFFECT_RAPID_SPIN;
			break;

		case MAX_EFFECT_HEAL_TEAM:
			moveEffect = EFFECT_RESTORE_HP;
			break;

		case MAX_EFFECT_SPITE:
			moveEffect = EFFECT_SPITE;
			break;

		case MAX_EFFECT_GRAVITY:
			if (!IsGravityActive())
				moveEffect = EFFECT_FIELD_EFFECTS;
			break;

		case MAX_EFFECT_VOLCAITH_FOES:
			break;

		case MAX_EFFECT_SANDBLAST_FOES:
		case MAX_EFFECT_FIRE_SPIN_FOES:
			moveEffect = EFFECT_TRAP;
			break;

		case MAX_EFFECT_YAWN_FOE:
			if (CanBePutToSleep(bankDef, TRUE) && !(gStatuses3[bankDef] & STATUS3_YAWN))
				moveEffect = EFFECT_YAWN;
			break;

		case MAX_EFFECT_LOWER_EVASIVENESS_FOES:
			if (STAT_STAGE(bankDef, STAT_STAGE_EVASION) > STAT_STAGE_MIN)
				moveEffect = EFFECT_EVASION_DOWN;
			break;

		case MAX_EFFECT_AROMATHERAPY:
			if (PartyMemberStatused(bankAtk, FALSE))
				moveEffect = EFFECT_HEAL_BELL;
			break;

		case MAX_EFFECT_TORMENT_FOES:
			if (CanBeTormented(bankDef))
				moveEffect = EFFECT_TORMENT;
			break;

		case MAX_EFFECT_LOWER_SPEED_2_FOES:
			if (STAT_STAGE(bankDef, STAT_STAGE_SPEED) > STAT_STAGE_MIN)
				moveEffect = EFFECT_SPEED_DOWN_2;
			break;
	}

	return moveEffect;
}

bool8 GetHealthPercentage(u8 bank)
{
	return (gBattleMons[bank].hp * 100) / gBattleMons[bank].maxHP;
}

bool8 TeamFullyHealedMinusBank(u8 bank)
{
	u8 firstId, lastId;

	struct Pokemon* party = LoadPartyRange(bank, &firstId, &lastId);

	for (int i = firstId; i < lastId; ++i)
	{
		if (party[i].species == SPECIES_NONE
		|| GetMonData(&party[i], REQ_EGG, NULL)
		|| i == gBattlerPartyIndexes[bank])
			continue;

		if (party[i].hp < party[i].maxHP)
			return FALSE;
	}
	return TRUE;
}

bool8 AnyStatIsRaised(u8 bank)
{
	for (u8 statId = STAT_STAGE_ATK; statId < BATTLE_STATS_NO; ++statId)
	{
		if (STAT_STAGE(bank, statId) > 6)
			return TRUE;
	}

	return FALSE;
}

bool8 AnyUsefulStatIsRaised(u8 bank)
{
	bool8 storedPowerInMoveset = MoveInMoveset(MOVE_STOREDPOWER, bank) || MoveInMoveset(MOVE_STOREDPOWER, bank);

	for (u8 statId = STAT_STAGE_ATK; statId < BATTLE_STATS_NO; ++statId)
	{
		if (STAT_STAGE(bank, statId) > 6)
		{
			if (storedPowerInMoveset)
				return TRUE;

			switch (statId) {
				case STAT_STAGE_ATK:
					if (PhysicalMoveInMoveset(bank))
						return TRUE;
					break;
				case STAT_STAGE_DEF:
					if (MoveSplitOnTeam(FOE(bank), SPLIT_PHYSICAL))
						return TRUE;
					break;
				case STAT_STAGE_SPATK:
					if (SpecialMoveInMoveset(bank))
						return TRUE;
					break;
				case STAT_STAGE_SPDEF:
					if (MoveSplitOnTeam(FOE(bank), SPLIT_SPECIAL))
						return TRUE;
					break;
				case STAT_STAGE_SPEED:
				case STAT_STAGE_ACC:
				case STAT_STAGE_EVASION:
					return TRUE;
			}
		}
	}

	if (gBattleResources->flags->flags[bank] & 1
	&&  DamagingMoveTypeInMoveset(bank, TYPE_FIRE))
		return TRUE;

	return FALSE;
}

bool8 AnyUsefulOffseniveStatIsRaised(u8 bank)
{
	for (u8 statId = STAT_STAGE_ATK; statId < BATTLE_STATS_NO; ++statId)
	{
		if (STAT_STAGE(bank, statId) > 6)
		{
			switch (statId) {
				case STAT_STAGE_ATK:
					if (PhysicalMoveInMoveset(bank))
						return TRUE;
					break;
				case STAT_STAGE_SPATK:
					if (SpecialMoveInMoveset(bank))
						return TRUE;
					break;
				case STAT_STAGE_SPEED:
					return TRUE;
			}
		}
	}

	return FALSE;
}

bool8 PartyMemberStatused(u8 bank, bool8 checkSoundproof)
{
	struct Pokemon* party;
	if (SIDE(bank) == B_SIDE_PLAYER)
		party = gPlayerParty;
	else
		party = gEnemyParty;

	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		if (checkSoundproof && GetMonAbility(&party[i]) == ABILITY_SOUNDPROOF)
			continue;

		if (party[i].condition != STATUS1_NONE)
			return TRUE;
	}

	return FALSE;
}
*/

bool8 ShouldUseFakeOut(u8 bankAtk, u8 bankDef)
{
	if (gDisableStructs[bankAtk].isFirstTurn
	&& IsUsefulToFlinchTarget(bankDef)
	&& ABILITY(bankDef) != ABILITY_INNERFOCUS
	&& !MoveBlockedBySubstitute(MOVE_FAKEOUT, bankAtk, bankDef))
	{
		if (IS_DOUBLE_BATTLE)
		{
			if (!CanMovePredictionProtectAgainstMove(bankDef, bankAtk, MOVE_FAKEOUT))
			{
				if (ITEM_EFFECT(bankAtk) == HOLD_EFFECT_CHOICE_BAND  //Don't lock the attacker into Fake Out
				&& ViableMonCountFromBank(bankAtk) <= 2) 	 		 //if they can't switch out afterwards.
				{
					if (ViableMonCountFromBank(bankDef) == 1
					&&  MoveKnocksOutXHits(MOVE_FAKEOUT, bankAtk, bankDef, 1))
						return TRUE; //If the opponent also only has one Pokemon left, then kill it to win the battle
				}
				else
					return TRUE;
			}
		}
		else //Single Battle
		{
			if (ITEM_EFFECT(bankAtk) == HOLD_EFFECT_CHOICE_BAND  //Don't lock the attacker into Fake Out
			&& ViableMonCountFromBank(bankAtk) <= 1) 			 //if they can't switch out afterwards.
			{
				if (ViableMonCountFromBank(bankDef) == 1
				&&  MoveKnocksOutXHits(MOVE_FAKEOUT, bankAtk, bankDef, 1))
					return TRUE; //If the opponent also only has one Pokemon left, then kill it to win the battle
			}
			else
				return TRUE;
		}
	}

	return FALSE;
}

bool8 ShouldAIUseZMove(u8 bankAtk, u8 bankDef, u16 move)
{
	int i;

	u16 zMove = CanUseZMove(bankAtk, 0xFF, move);
	u16 defMovePrediction = IsValidMovePrediction(bankDef, bankAtk);

	if (IS_DOUBLE_BATTLE && bankDef == PARTNER(bankAtk))
		return FALSE; //No need to waste a Z-Move on your partner

	if (zMove != MOVE_NONE)
	{
		if (zMove != 0xFFFF) //Damaging Z-Move
		{
			u8 defAbility = ABILITY(bankDef);
			u16 defSpecies = SPECIES(bankDef);
			bool8 noMoldBreakers = NO_MOLD_BREAKERS(ABILITY(bankAtk), zMove);

			if (move == MOVE_FAKEOUT && ShouldUseFakeOut(bankAtk, bankDef))
				return FALSE; //Prefer actual Fake Out over Breakneck Blitz

			if (MoveBlockedBySubstitute(zMove, bankAtk, bankDef)
			|| (defMovePrediction == MOVE_SUBSTITUTE
			 && !MoveWouldHitFirst(zMove, bankAtk, bankDef)
			 && !MoveIgnoresSubstitutes(zMove, ABILITY(bankAtk))))
				return FALSE; //Don't use a Z-Move on a Substitute or if the enemy is going to go first and use Substitute

			#ifdef SPECIES_MIMIKYU
			if (noMoldBreakers && defAbility == ABILITY_DISGUISE && defSpecies == SPECIES_MIMIKYU)
				return FALSE; //Don't waste a Z-Move busting Mimikyu's disguise
			#endif
			#ifdef SPECIES_EISCUE
			if (noMoldBreakers && defAbility == ABILITY_ICEFACE && defSpecies == SPECIES_EISCUE && SPLIT(zMove) == SPLIT_PHYSICAL)
				return FALSE; //Don't waste a Z-Move busting Eiscue's Ice Face
			#endif

			if (defMovePrediction == MOVE_PROTECT || defMovePrediction == MOVE_KINGSSHIELD || defMovePrediction == MOVE_SPIKYSHIELD || defMovePrediction == MOVE_OBSTRUCT
			|| (IsDynamaxed(bankDef) && SPLIT(defMovePrediction) == SPLIT_STATUS))
				return FALSE; //Don't waste a Z-Move on a Protect

			if (IsRaidBattle() && gBattleStruct->dynamaxData.raidShieldsUp && SIDE(bankAtk) == B_SIDE_PLAYER && SIDE(bankDef) == B_SIDE_OPPONENT) //Partner AI on Raid Pokemon with shields up
			{
				u16 bankAtkPartner, partnerMove;

				if (gBattleStruct->dynamaxData.shieldCount - gBattleStruct->dynamaxData.shieldsDestroyed <= 2 //Less than 3 shields left
				&& gBattleStruct->dynamaxData.stormLevel < 3) //The Raid boss hasn't almost won
					return FALSE; //Don't waste a Z-Move breaking a shield

				bankAtkPartner = PARTNER(bankAtk);
				partnerMove = GetAIChosenMove(bankAtkPartner, bankDef);

				if (SPLIT(partnerMove) == SPLIT_STATUS
				|| MoveWouldHitFirst(partnerMove, bankAtkPartner, bankAtk)
				|| (gChosenMoveByBattler[bankAtkPartner] != MOVE_NONE && gBattleStruct->moveTarget[bankAtkPartner] != bankDef)) //Not targeting raid Pokemon
					return FALSE; //Don't waste a Z-Move if partner can't destroy shield first
			}

			//These moves should always be turned into Z-Moves, regardless if they KO or not
			switch (gBattleMoves[move].effect) {
				case EFFECT_RECHARGE:
				case EFFECT_SEMI_INVULNERABLE:
				case EFFECT_SKY_ATTACK:
				case EFFECT_SKULL_BASH:
				case EFFECT_SOLARBEAM:
				case EFFECT_LASTRESORT_SKYDROP:
					return TRUE;
			}

			if (MoveKnocksOutXHits(move, bankAtk, bankDef, 1))
				return FALSE; //If the base move can KO, don't turn it into a Z-Move

			return TRUE;
		}
		else //Status Move
		{
			u8 zEffect = gBattleMoves[move].z_move_effect;

			switch (zEffect) {
				case Z_EFFECT_NONE:
					return FALSE;
				case Z_EFFECT_RESET_STATS:
					for (i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
					{
						if (i == STAT_STAGE_ATK && !PhysicalMoveInMoveset(bankAtk)) //Only reset lowered Attack if useful
							continue;
						else if (i == STAT_STAGE_ATK && !SpecialMoveInMoveset(bankAtk)) //Only reset lowered Special Attack if useful
							continue;

						if (STAT_STAGE(bankAtk, i) < 6)
							return TRUE; //Reset any negative stats
					}
					break;
				case Z_EFFECT_ALL_STATS_UP_1:
					if (!StatsMaxed(bankAtk))
						return TRUE;
					break;
				case Z_EFFECT_BOOST_CRITS:
					if (!(gBattleMons[bankAtk].status2 & STATUS2_FOCUS_ENERGY))
						return TRUE;
					break;
				case Z_EFFECT_FOLLOW_ME:
					if (IS_DOUBLE_BATTLE)
						return TRUE;
					break;
				case Z_EFFECT_ATK_UP_1:
				case Z_EFFECT_ATK_UP_2:
				case Z_EFFECT_ATK_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_ATK)
					&& MoveSplitInMoveset(bankAtk, SPLIT_PHYSICAL))
						return TRUE;
					break;
				case Z_EFFECT_DEF_UP_1:
				case Z_EFFECT_DEF_UP_2:
				case Z_EFFECT_DEF_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_DEF))
						return TRUE;
					break;
				case Z_EFFECT_SPATK_UP_1:
				case Z_EFFECT_SPATK_UP_2:
				case Z_EFFECT_SPATK_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_SPATK)
					&& MoveSplitInMoveset(bankAtk, SPLIT_SPECIAL))
						return TRUE;
					break;
				case Z_EFFECT_SPDEF_UP_1:
				case Z_EFFECT_SPDEF_UP_2:
				case Z_EFFECT_SPDEF_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_SPDEF))
						return TRUE;
					break;
				case Z_EFFECT_SPD_UP_1:
				case Z_EFFECT_SPD_UP_2:
				case Z_EFFECT_SPD_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_SPEED))
						return TRUE;
					break;
				case Z_EFFECT_ACC_UP_1:
				case Z_EFFECT_ACC_UP_2:
				case Z_EFFECT_ACC_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_ACC)
					&&  MoveInMovesetWithAccuracyLessThan(bankAtk, bankDef, 90, FALSE))
						return TRUE;
					break;
				case Z_EFFECT_EVSN_UP_1:
				case Z_EFFECT_EVSN_UP_2:
				case Z_EFFECT_EVSN_UP_3:
					if (STAT_CAN_RISE(bankAtk, STAT_STAGE_EVASION))
						return TRUE;
					break;
				default: //Recover HP
					return TRUE;
			}
		}
	}

	return FALSE;
}
/*
static bool8 MonCanTriggerWeatherAbilityWithMaxMove(struct Pokemon* mon)
{
	if (WEATHER_HAS_EFFECT)
	{
		switch (GetMonAbility(mon)) {
			case ABILITY_SWIFTSWIM:
			case ABILITY_RAINDISH:
			case ABILITY_DRYSKIN:
				return MonCanUseMaxMoveWithEffect(mon, MAX_EFFECT_RAIN);
			case ABILITY_CHLOROPHYLL:
			case ABILITY_SOLARPOWER:
			case ABILITY_FLOWERGIFT:
				return MonCanUseMaxMoveWithEffect(mon, MAX_EFFECT_SUN);
			case ABILITY_SANDRUSH:
			case ABILITY_SANDFORCE:
				return MonCanUseMaxMoveWithEffect(mon, MAX_EFFECT_SANDSTORM);
			case ABILITY_SLUSHRUSH:
			case ABILITY_ICEBODY:
			case ABILITY_ICEFACE:
				return MonCanUseMaxMoveWithEffect(mon, MAX_EFFECT_HAIL);
			case ABILITY_SURGESURFER:
				return MonCanUseMaxMoveWithEffect(mon, MAX_EFFECT_ELECTRIC_TERRAIN);
		}
	}

	return FALSE;
}

void CalcAIDynamaxMon(u8 bank)
{
	u8 bestMonId = 0xFF;

	if (gBattleTypeFlags & BATTLE_TYPE_DYNAMAX && !gBattleStruct->dynamaxData.used[bank])
	{
		struct Pokemon* party;
		u8 bestMonScore, bestStatAmount, firstId, lastId, battlerIn1, battlerIn2, i;

		party = LoadPartyRange(bank, &firstId, &lastId);
		if (IS_SINGLE_BATTLE)
			battlerIn1 = battlerIn2 = bank;
		else
		{
			if (gAbsentBattlerFlags & gBitTable[bank])
				battlerIn1 = battlerIn2 = PARTNER(bank);
			else if (gAbsentBattlerFlags & gBitTable[PARTNER(bank)])
				battlerIn1 = battlerIn2 = bank;
			else
			{
				battlerIn1 = bank;
				battlerIn2 = PARTNER(bank);
			}
		}

		for (i = 0, bestMonScore = 0, bestStatAmount = 0; i < PARTY_SIZE; ++i) //Do entire party at once, even for Multi Battles
		{
			struct Pokemon* mon = &party[i];
			u8 updateScore, itemEffect, ability;
			u32 bestMonStat, attack, spAttack;

			if (mon->species == SPECIES_NONE
			|| GetMonData(mon, MON_DATA_HP, NULL) == 0
			|| GetMonData(mon, MON_DATA_IS_EGG, NULL)
			|| !MonCanDynamax(mon))
				continue;

			updateScore = 0;
			bestMonStat = 0;
			if (gBattlerPartyIndexes[battlerIn1] == i || gBattlerPartyIndexes[battlerIn2] == i)
			{
				u8 checkBank;
				if (gBattlerPartyIndexes[battlerIn1] == i)
					checkBank = battlerIn1;
				else
					checkBank = battlerIn2;

				itemEffect = ITEM_EFFECT(checkBank);
				ability = ABILITY(checkBank);
				attack = gBattleMons[checkBank].attack;
				spAttack = gBattleMons[checkBank].spAttack;
				APPLY_QUICK_STAT_MOD(attack, STAT_STAGE(checkBank, STAT_STAGE_ATK));
				APPLY_QUICK_STAT_MOD(spAttack, STAT_STAGE(checkBank, STAT_STAGE_SPATK));
			}
			else
			{
				itemEffect = GetMonItemEffect(mon);
				ability = GetMonAbility(mon);
				attack = mon->attack;
				spAttack = mon->spAttack;
			}

			if (PhysicalMoveInMonMoveset(mon, MOVE_LIMITATION_ZEROMOVE | MOVE_LIMITATION_PP))
			{
				if (SpecialMoveInMonMoveset(mon, MOVE_LIMITATION_ZEROMOVE | MOVE_LIMITATION_PP) && spAttack > attack)
					bestMonStat = spAttack;
				else
					bestMonStat = attack;
			}
			else if (SpecialMoveInMonMoveset(mon, MOVE_LIMITATION_ZEROMOVE | MOVE_LIMITATION_PP)) //Only set if mon has Special move
				bestMonStat = spAttack;

			if (itemEffect == ITEM_EFFECT_WEAKNESS_POLICY)
				updateScore = 4;
			else if (itemEffect == ITEM_EFFECT_CHOICE_BAND || ability == ABILITY_GORILLATACTICS)
				updateScore = 3;
			else if (MonCanTriggerWeatherAbilityWithMaxMove(mon))
				updateScore = 2;
			else if (bestMonStat > 0 //Has an actual attacking move
			&& itemEffect != ITEM_EFFECT_EJECT_BUTTON && itemEffect != ITEM_EFFECT_EJECT_PACK) //And probably won't be forced out by its item
				updateScore = 1;

			if (MoveInMonMovesetThatCanChangeByGigantamaxing(mon))
				++updateScore; //This is even better

			if (updateScore >= bestMonScore)
			{
				if (updateScore == bestMonScore
				&& bestMonStat <= bestStatAmount)
					continue; //Stats aren't better so check next mon

				if (bestMonStat > 0) //Mon has attacking moves
				{
					bestMonId = i;
					bestMonScore = updateScore;
					bestStatAmount = bestMonStat;
				}
			}
		}
	}

	gBattleStruct->ai.dynamaxMonId[SIDE(bank)] = bestMonId;
}

void CalcShouldAIDynamax(u8 bankAtk, u8 bankDef)
{
	gBattleStruct->ai.dynamaxPotential[bankAtk][bankDef] = FALSE;

	if (bankAtk != bankDef && CanDynamax(bankAtk))
	{
		u8 partnerBank = PARTNER(bankAtk);

		if (BATTLER_SEMI_INVULNERABLE(bankAtk))
			return; //Can't Dynamax out of view

		if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(partnerBank) && gBattleStruct->dynamaxData.toBeUsed[partnerBank])
			return; //Don't Dynamax if the other mon is going to

		if (IS_DOUBLE_BATTLE && bankDef == PARTNER(bankAtk))
			return; //No need to Dynamax against your partner

		if (IsRaidBattle() && bankAtk == BANK_RAID_BOSS)
			return; //Raid Boss is always Dynamaxed

		if (!IsMockBattle() && SIDE(bankAtk) == B_SIDE_PLAYER)
		{
			if (IsTagBattle())
			{
				if (GetBattlerPosition(bankAtk) == B_POSITION_PLAYER_LEFT)
					return; //Never predict that the player will Dynamax
			}
			else //Player is in control
				return; //Never predict that the player will Dynamax
		}

		if (gBattleStruct->ai.dynamaxMonId[SIDE(bankAtk)] == gBattlerPartyIndexes[bankAtk])
		{
			if (AI_THINKING_STRUCT->aiFlags > AI_SCRIPT_CHECK_BAD_MOVE || SIDE(bankAtk) == B_SIDE_PLAYER) //Smart AI or partners only
			{
				u16 predictedMove = IsValidMovePrediction(bankAtk, bankDef);
				if (predictedMove != MOVE_NONE
				&&  MoveWouldHitFirst(predictedMove, bankAtk, bankDef)
				&&  MoveKnocksOutXHits(predictedMove, bankAtk, bankDef, 1))
					return; //Just KO the opponent normally

				if ((IS_SINGLE_BATTLE && ViableMonCountFromBank(bankAtk) > 1)
				||  (IS_DOUBLE_BATTLE && ViableMonCountFromBank(bankAtk) > 2)) //Could use Dynamax on another Pokemon
				{
					predictedMove = IsValidMovePrediction(bankDef, bankAtk);
					if (predictedMove != MOVE_NONE
					&&  MoveWouldHitFirst(predictedMove, bankDef, bankAtk)
					&&  MoveKnocksOutAfterDynamax(predictedMove, bankDef, bankAtk))
						return; //Don't Dynamax is foe will just KO you
				}

				OnlyBadMovesLeftInMoveset(bankAtk, bankDef); //Force calculation

				if ((ITEM_EFFECT(bankAtk) == ITEM_EFFECT_CHOICE_BAND || ABILITY(bankAtk) == ABILITY_GORILLATACTICS)
				&& !gBattleStruct->ai.shouldFreeChoiceLockWithDynamax[bankAtk][bankDef]) //There are good moves left
					return; //Save Dynamax for when you really need it
			}

			gBattleStruct->ai.dynamaxPotential[bankAtk][bankDef] = TRUE;
			ClearStrongestMoveAndCanKnockOut(bankAtk, bankDef); //All moves now are treated like Max Moves so wipe old data
		}
	}
}
*/
bool8 ShouldAIDynamax(u8 bankAtk, u8 bankDef)
{
	return gBattleStruct->ai.dynamaxPotential[bankAtk][bankDef];
}

u8 AdjustMoveLimitationFlagsForAI(u8 bankAtk, u8 bankDef)
{
	if (ShouldAIDynamax(bankAtk, bankDef)) //AI will Dynamax this turn
		return MOVE_LIMITATION_ZEROMOVE | MOVE_LIMITATION_PP | MOVE_LIMITATION_TAUNT;

	return 0xFF; //All flags by default
}

void IncreaseViability(s16* viability, u8 amount)
{
	*viability = MathMin(*viability + amount, 255);
}

void DecreaseViability(s16* viability, u16 amount)
{
	if (IS_DOUBLE_BATTLE) //Double decreases in doubles
		amount *= 2; //Assumes amount won't be too big to cause overflow

	if (amount > *viability)
		*viability = 0;
	else
		*viability -= amount;
}
