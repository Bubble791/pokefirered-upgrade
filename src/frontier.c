#include "global.h"
#include "battle.h"
#include "battle_scripts.h"
#include "battle_string_ids.h"
#include "battle_controllers.h"
#include "util.h"
#include "item.h"
#include "random.h"
#include "pokemon.h"
#include "build_pokemon.h"
#include "frontier.h"
#include "text.h"
#include "window.h"
#include "event_data.h"
#include "constants/abilities.h"
#include "constants/hold_effects.h"
#include "constants/battle_move_effects.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/vars.h"
#include "constants/flags.h"
#include "constants/battle_ai.h"

EWRAM_DATA u16 gBattleTowerStreaks[NUM_TOWER_BATTLE_TYPES][NUM_FORMATS_OLD][2][2][2] = {0};
EWRAM_DATA struct BattleSandsStreak gBattleSandsStreaks[2] = {0};
EWRAM_DATA u16 gBattleMineStreaks[NUM_BATTLE_MINE_TIERS][2] = {0};
EWRAM_DATA u16 gBattleCircusStreaks[NUM_BATTLE_CIRCUS_TIERS][NUM_TOWER_BATTLE_TYPES][2][2][2] = {0};

#include "data/battle_frontier_trainers.h"

const u8 gBattleTowerTiers[] =
{
	BATTLE_FACILITY_STANDARD,
	BATTLE_FACILITY_NO_RESTRICTIONS,
	BATTLE_FACILITY_OU,
	BATTLE_FACILITY_UBER,
	BATTLE_FACILITY_LITTLE_CUP,
	BATTLE_FACILITY_MIDDLE_CUP,
	BATTLE_FACILITY_MONOTYPE,
	BATTLE_FACILITY_DYNAMAX_STANDARD,
};

const u8 gNumBattleTowerTiers = ARRAY_COUNT(gBattleTowerTiers);

const u8 gBattleMineFormat1Tiers[] =
{
	BATTLE_FACILITY_OU,
	BATTLE_FACILITY_CAMOMONS,
	BATTLE_FACILITY_BENJAMIN_BUTTERFREE,
};

const u8 gBattleMineFormat2Tiers[] =
{
	BATTLE_FACILITY_SCALEMONS,
	BATTLE_FACILITY_350_CUP,
	BATTLE_FACILITY_AVERAGE_MONS,
};

const u8 gBattleMineFormat3Tiers[] =
{
	BATTLE_FACILITY_LITTLE_CUP,
	BATTLE_FACILITY_LC_CAMOMONS,
};

const u8 gBattleMineTiers[] =
{
	BATTLE_MINE_FORMAT_1,
	BATTLE_MINE_FORMAT_2,
	BATTLE_MINE_FORMAT_3,
};

const u8 gNumBattleMineTiers = ARRAY_COUNT(gBattleMineTiers);

const u8 gBattleCircusTiers[] =
{
	BATTLE_FACILITY_STANDARD,
	BATTLE_FACILITY_NO_RESTRICTIONS,
	BATTLE_FACILITY_MONOTYPE,
	BATTLE_FACILITY_CAMOMONS,
	BATTLE_FACILITY_UBER_CAMOMONS,
	BATTLE_FACILITY_LC_CAMOMONS,
	BATTLE_FACILITY_MC_CAMOMONS,
	BATTLE_FACILITY_SCALEMONS,
	BATTLE_FACILITY_350_CUP,
	BATTLE_FACILITY_AVERAGE_MONS,
	BATTLE_FACILITY_BENJAMIN_BUTTERFREE,
	BATTLE_FACILITY_DYNAMAX_STANDARD,
	BATTLE_FACILITY_NATIONAL_DEX_OU,
};

const u8 gNumBattleCircusTiers = ARRAY_COUNT(gBattleCircusTiers);


bool8 InBattleSands(void)
{
	return (gBattleTypeFlags & BATTLE_TYPE_BATTLE_SANDS) != 0;
}

bool8 IsCamomonsTier(u8 tier)
{
	return tier == BATTLE_FACILITY_CAMOMONS
		|| tier == BATTLE_FACILITY_UBER_CAMOMONS
		|| tier == BATTLE_FACILITY_LC_CAMOMONS
		|| tier == BATTLE_FACILITY_MC_CAMOMONS;
}

bool8 IsLittleCupTier(u8 tier)
{
	return tier == BATTLE_FACILITY_LITTLE_CUP || tier == BATTLE_FACILITY_LC_CAMOMONS;
}

bool8 IsMiddleCupTier(u8 tier)
{
	return tier == BATTLE_FACILITY_MIDDLE_CUP || tier == BATTLE_FACILITY_MC_CAMOMONS;
}

bool8 IsAverageMonsBattle(void)
{
	return FlagGet(FLAG_BATTLE_FACILITY) && VarGet(VAR_BATTLE_FACILITY_TIER) == BATTLE_FACILITY_AVERAGE_MONS;
}

bool8 Is350CupBattle(void)
{
	return FlagGet(FLAG_BATTLE_FACILITY) && VarGet(VAR_BATTLE_FACILITY_TIER) == BATTLE_FACILITY_350_CUP;
}

bool8 IsScaleMonsBattle(void)
{
	return FlagGet(FLAG_BATTLE_FACILITY) && VarGet(VAR_BATTLE_FACILITY_TIER) == BATTLE_FACILITY_SCALEMONS;
}

bool8 IsCamomonsBattle(void)
{
	return FlagGet(FLAG_BATTLE_FACILITY) && IsCamomonsTier(VarGet(VAR_BATTLE_FACILITY_TIER));
}

u8 GetCamomonsTypeByMon(struct Pokemon* mon, u8 whichType)
{
	if (whichType == 0)
	{
		return gBattleMoves[GetMonData(mon, MON_DATA_MOVE1, NULL)].type;
	}
	else
	{
		u16 move2 = GetMonData(mon, MON_DATA_MOVE2, NULL);
		if (move2 != MOVE_NONE)
			return gBattleMoves[move2].type;

		return gBattleMoves[GetMonData(mon, MON_DATA_MOVE1, NULL)].type;
	}
}

u8 ShouldReplaceTypesWithCamomons(void)
{
	return (gMain.inBattle && gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) || IsCamomonsBattle();
}

void UpdateTypesForCamomons(u8 bank)
{
	gBattleMons[bank].type1 = gBattleMoves[gBattleMons[bank].moves[0]].type;

	if (gBattleMons[bank].moves[1] != MOVE_NONE)
		gBattleMons[bank].type2 = gBattleMoves[gBattleMons[bank].moves[1]].type;
	else
		gBattleMons[bank].type2 = gBattleMons[bank].type1;
}

u32 GetAIFlagsInBattleFrontier(u8 bank)
{
	return AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_CHECK_GOOD_MOVE;
}

bool8 IsFrontierTrainerId(u16 trainerId)
{
	switch (trainerId) {
		case BATTLE_TOWER_TID:
		case BATTLE_TOWER_SPECIAL_TID:
		case FRONTIER_BRAIN_TID:
		case BATTLE_FACILITY_MULTI_TRAINER_TID:
		case RAID_BATTLE_MULTI_TRAINER_TID:
			return TRUE;
	} 
	
	return FALSE;
}
bool8 IsBenjaminButterfreeBattle(void)
{
	return (gBattleTypeFlags & BATTLE_TYPE_SECRET_BASE) != 0;
}

bool8 AreMegasZMovesBannedInTier(u8 tier)
{
	return tier == BATTLE_FACILITY_DYNAMAX_STANDARD;
}

bool8 IsMegaZMoveBannedBattle(void)
{
	return gBattleTypeFlags & BATTLE_TYPE_TRAINER //Excludes Raid Battles
	&& FlagGet(FLAG_BATTLE_FACILITY)
	&& (AreMegasZMovesBannedInTier(VarGet(VAR_BATTLE_FACILITY_TIER))
	 || (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_DYNAMAX));
}

bool8 IsFrontierSingles(u8 battleType)
{
	return battleType == BATTLE_FACILITY_SINGLE
		|| battleType == BATTLE_FACILITY_SINGLE_RANDOM;
}

bool8 IsFrontierDoubles(u8 battleType)
{
	return battleType == BATTLE_FACILITY_DOUBLE
		|| battleType == BATTLE_FACILITY_DOUBLE_RANDOM;
}

bool8 IsFrontierMulti(u8 battleType)
{
	return battleType == BATTLE_FACILITY_MULTI
		|| battleType == BATTLE_FACILITY_MULTI_RANDOM
		|| battleType == BATTLE_FACILITY_LINK_MULTI;
}

bool8 IsRandomBattleTowerBattle(void)
{
	u8 battleType = VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE);

	return battleType == BATTLE_FACILITY_SINGLE_RANDOM
		|| battleType == BATTLE_FACILITY_DOUBLE_RANDOM
		|| battleType == BATTLE_FACILITY_MULTI_RANDOM;
}

bool8 IsGSCupBattle(void)
{
	u8 battleType = VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE);

	return (IsFrontierDoubles(battleType) || IsFrontierMulti(battleType))
		&&  VarGet(VAR_BATTLE_FACILITY_TIER) == BATTLE_FACILITY_GS_CUP;
}

bool8 DuplicateItemsAreBannedInTier(u8 tier, u8 battleType)
{
	if (tier == BATTLE_FACILITY_STANDARD
	||  IsMiddleCupTier(tier)
	||  tier == BATTLE_FACILITY_MEGA_BRAWL
	||  tier == BATTLE_FACILITY_DYNAMAX_STANDARD)
		return TRUE;

	return !IsFrontierSingles(battleType) && tier == BATTLE_FACILITY_GS_CUP;
}

bool8 RayquazaCanMegaEvolveInFrontierBattle(void)
{
	return IsGSCupBattle()
		|| VarGet(VAR_BATTLE_FACILITY_TIER) == BATTLE_FACILITY_NO_RESTRICTIONS
		|| IsScaleMonsBattle();
}

void CopyFrontierTrainerName(u8* dst, u16 trainerId, u8 battlerNum)
{
	int i;
	const u8* name = GetFrontierTrainerName(trainerId, battlerNum);

	for (i = 0; name[i] != EOS; ++i)
		dst[i] = name[i];

	dst[i] = EOS;
}

const u8* GetFrontierTrainerName(u16 trainerId, u8 battlerNum)
{
	const u8* name;
	//need todo

	return ReturnEmptyStringIfNull(name);
}

static u8 AdjustLevelForTier(u8 level, u8 tier)
{
	if (IsLittleCupTier(tier))
		return 5;

	if (IS_SINGLE_100_RECORD_TIER(tier))
		return 100;

	return level;
}

static void LoadProperStreakData(u8* facilityNum, u8* currentOrMax, u8* battleStyle, u8* tier, u8* partySize, u8* level)
{
	u32 i;

	switch (*facilityNum) {
		case IN_BATTLE_TOWER:
			switch (*tier) {
				case BATTLE_FACILITY_MONOTYPE:
					*tier = BATTLE_FACILITY_LITTLE_CUP; //Hijack Little Cup level 100 slot
					*level = 100;
					break;
				case BATTLE_FACILITY_DYNAMAX_STANDARD:
					*facilityNum = IN_BATTLE_CIRCUS; //Hijack Battle Circus Little Cup level 100 slot
					*tier = BATTLE_FACILITY_LC_CAMOMONS;
					*level = 100;
					return LoadProperStreakData(facilityNum, currentOrMax, battleStyle, tier, partySize, level);
			}

			for (i = 0; i < gNumBattleTowerTiers; ++i)
			{
				if (gBattleTowerTiers[i] == *tier)
				{
					*tier = i;
					break;
				}
			}

			if (i == gNumBattleTowerTiers)
				*tier = 0;
			break;

		case IN_BATTLE_SANDS:
			*tier = MathMin(*tier, gNumBattleTowerTiers);
			break;

		case IN_BATTLE_MINE:
			*tier = MathMin(*tier - BATTLE_MINE_FORMAT_1, 2);
			break;

		case IN_BATTLE_CIRCUS:
			for (i = 0; i < gNumBattleCircusTiers; ++i)
			{
				if (gBattleCircusTiers[i] == *tier)
				{
					*tier = i;
					break;
				}
			}

			if (i == gNumBattleCircusTiers)
				*tier = 0;
			break;
	}

	if (IsFrontierMulti(*battleStyle))
		*partySize *= 2; //Each player gets half the team

	*currentOrMax = MathMin(*currentOrMax, 1);
	*battleStyle = MathMin(*battleStyle, NUM_TOWER_BATTLE_TYPES);
	*partySize = (*partySize < 6) ? 0 : 1;
	*level = (*level < 100) ? 0 : 1;
}

u16 GetBattleTowerStreak(u8 currentOrMax, u16 inputBattleStyle, u16 inputTier, u16 partySize, u8 level)
{
	u8 facilityNum = BATTLE_FACILITY_NUM;
	u8 battleStyle = (inputBattleStyle == 0xFFFF) ? VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE) : inputBattleStyle;
	u8 tier = (inputTier == 0xFFFF) ? VarGet(VAR_BATTLE_FACILITY_TIER) : inputTier;
	u8 size = (partySize == 0xFFFF) ? VarGet(VAR_BATTLE_FACILITY_POKE_NUM) : partySize;
	level = (level == 0) ? VarGet(VAR_BATTLE_FACILITY_POKE_LEVEL) : level;
	level = AdjustLevelForTier(level, tier);

	LoadProperStreakData(&facilityNum, &currentOrMax, &battleStyle, &tier, &size, &level);

	switch (facilityNum) {
		case IN_BATTLE_TOWER:
		default:
			return gBattleTowerStreaks[battleStyle][tier][size][level][currentOrMax];
		case IN_BATTLE_SANDS:
			return gBattleSandsStreaks[currentOrMax].streakLength;
		case IN_BATTLE_MINE:
			return gBattleMineStreaks[tier][currentOrMax];
		case IN_BATTLE_CIRCUS:
			return gBattleCircusStreaks[tier][battleStyle][size][level][currentOrMax];
	}
}

u16 GetCurrentBattleTowerStreak(void)
{
	return GetBattleTowerStreak(CURR_STREAK, 0xFFFF, 0xFFFF, 0xFFFF, 0);
}

u8 GetFrontierTrainerFrontSpriteId(u16 trainerId, u8 battlerNum)
{
	return gTrainers[trainerId].trainerPic;
}
