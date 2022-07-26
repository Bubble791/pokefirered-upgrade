#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "pokemon.h"
#include "dynamax.h"
#include "item.h"
#include "event_data.h"
#include "util.h"
#include "frontier.h"
#include "mega.h"
#include "set_z_effect.h"
#include "data.h"
#include "palette.h"
#include "decompress.h"
#include "sprite.h"
#include "overworld.h"
#include "random.h"
#include "battle_message.h"
#include "battle_interface.h"
#include "battle_controllers.h"
#include "constants/moves.h"
#include "constants/species.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/region_map_sections.h"

EWRAM_DATA const void *gPokeBackupPtr = NULL;

static bool8 IsBannedHeldItemForDynamax(u16 item);
static const u8* DoDynamax(u8 bank);
static const u8* DoGigantamax(u8 bank);
static bool8 IsItemDynamaxBand(u16 item);
static u16 FindTrainerDynamaxBand(u16 trainerId);
static u16 FindPlayerDynamaxBand(void);
static u16 FindBankDynamaxBand(u8 bank);
static u16 GetTypeBasedMaxMove(u8 moveType, u8 moveSplit);
static u16 GetGMaxMove(u8 moveType, u8 moveSplit, u16 species, bool8 canGigantamax);
static u8 GetRaidMapSectionId(void);
static u32 GetRaidRandomNumber(void);
static bool8 ShouldTryGigantamaxRaidMon(void);

#define HEALTHBOX_GFX_HP_BAR_GREEN 3
#define HEALTHBOX_GFX_HP_BAR_YELLOW 47
#define HEALTHBOX_GFX_HP_BAR_RED 56
#define B_HEALTHBAR_PIXELS 48

#include "data/raid_encounters.h"
#include "data/battle_tower_spreads.h"

struct DynamaxTrainerSlide
{
	u16 trainerId;
	const u8* dynamaxMsg;
};


const u8 gRaidBattleStarsByBadges[NUM_BADGE_OPTIONS][2] =
{
	[0] = {NO_RAID,         NO_RAID},
	[1] = {ONE_STAR_RAID, 	ONE_STAR_RAID},
	[2] = {ONE_STAR_RAID,   TWO_STAR_RAID},
	[3] = {TWO_STAR_RAID,   TWO_STAR_RAID},
	[4] = {TWO_STAR_RAID,   THREE_STAR_RAID},
	[5] = {THREE_STAR_RAID, THREE_STAR_RAID},
	[6] = {THREE_STAR_RAID, FOUR_STAR_RAID},
	[7] = {FOUR_STAR_RAID,  FOUR_STAR_RAID},
	[8] = {FOUR_STAR_RAID,  FIVE_STAR_RAID},
	[9] = {FIVE_STAR_RAID,  SIX_STAR_RAID}, //Beat Game
};

const u8 gRaidBattleLevelRanges[RAID_STAR_COUNT][2] =
{
	[ONE_STAR_RAID]   = {15, 20},
	[TWO_STAR_RAID]   = {25, 30},
	[THREE_STAR_RAID] = {35, 40},
	[FOUR_STAR_RAID]  = {50, 55},
	[FIVE_STAR_RAID]  = {60, 65},
	[SIX_STAR_RAID]   = {75, 90},
};

//The chance that each move is replaced with an Egg Move
const u8 gRaidBattleEggMoveChances[RAID_STAR_COUNT] =
{
	[ONE_STAR_RAID]   = 0,
	[TWO_STAR_RAID]   = 10,
	[THREE_STAR_RAID] = 30,
	[FOUR_STAR_RAID]  = 50,
	[FIVE_STAR_RAID]  = 70,
	[SIX_STAR_RAID]   = 70,
};

static const u8 sRaidBattleDropRates[MAX_RAID_DROPS] =
{	//In percent
	100,
	80,
	80,
	50,
	50,
	30,
	30,
	25,
	25,
	5,
	4,
	1,
};

struct GMaxMove
{
	u16 species;
	u8 moveType;
	u16 gmaxMove;
};

static const struct GMaxMove sGMaxMoveTable[] =
{
	{SPECIES_VENUSAUR_GIGA,	      TYPE_GRASS,      MOVE_G_MAX_VINE_LASH_P},
	{SPECIES_CHARIZARD_GIGA,      TYPE_FIRE,       MOVE_G_MAX_WILDFIRE_P},
	{SPECIES_BLASTOISE_GIGA,      TYPE_WATER,      MOVE_G_MAX_CANNONADE_P},
	{SPECIES_BUTTERFREE_GIGA,     TYPE_BUG,        MOVE_G_MAX_BEFUDDLE_P},
	{SPECIES_PIKACHU_GIGA,        TYPE_ELECTRIC,   MOVE_G_MAX_VOLT_CRASH_P},
	{SPECIES_MEOWTH_GIGA,         TYPE_NORMAL,     MOVE_G_MAX_GOLD_RUSH_P},
	{SPECIES_MACHAMP_GIGA,        TYPE_FIGHTING,   MOVE_G_MAX_CHI_STRIKE_P},
	{SPECIES_GENGAR_GIGA,         TYPE_GHOST,      MOVE_G_MAX_TERROR_P},
	{SPECIES_KINGLER_GIGA,        TYPE_WATER,      MOVE_G_MAX_FOAM_BURST_P},
	{SPECIES_LAPRAS_GIGA,         TYPE_ICE,        MOVE_G_MAX_RESONANCE_P},
	{SPECIES_EEVEE_GIGA,          TYPE_NORMAL,     MOVE_G_MAX_CUDDLE_P},
	{SPECIES_SNORLAX_GIGA,        TYPE_NORMAL,     MOVE_G_MAX_REPLENISH_P},
	{SPECIES_GARBODOR_GIGA,       TYPE_POISON,     MOVE_G_MAX_MALODOR_P},
	{SPECIES_MELMETAL_GIGA,       TYPE_STEEL,      MOVE_G_MAX_MELTDOWN_P},
	{SPECIES_RILLABOOM_GIGA,      TYPE_GRASS,      MOVE_G_MAX_DRUM_SOLO_P},
	{SPECIES_CINDERACE_GIGA,      TYPE_FIRE,       MOVE_G_MAX_FIREBALL_P},
	{SPECIES_INTELEON_GIGA,       TYPE_WATER,      MOVE_G_MAX_HYDROSNIPE_P},
	{SPECIES_CORVIKNIGHT_GIGA,    TYPE_FLYING,     MOVE_G_MAX_WIND_RAGE_P},
	{SPECIES_ORBEETLE_GIGA,       TYPE_PSYCHIC,    MOVE_G_MAX_GRAVITAS_P},
	{SPECIES_DREDNAW_GIGA,        TYPE_WATER,      MOVE_G_MAX_STONESURGE_P},
	{SPECIES_COALOSSAL_GIGA,      TYPE_ROCK,       MOVE_G_MAX_VOLCALITH_P},
	{SPECIES_FLAPPLE_GIGA,        TYPE_GRASS,      MOVE_G_MAX_TARTNESS_P},
	{SPECIES_APPLETUN_GIGA,       TYPE_GRASS,      MOVE_G_MAX_SWEETNESS_P},
	{SPECIES_SANDACONDA_GIGA,     TYPE_GROUND,     MOVE_G_MAX_SANDBLAST_P},
	{SPECIES_TOXTRICITY_GIGA,     TYPE_ELECTRIC,   MOVE_G_MAX_STUN_SHOCK_P},
	{SPECIES_CENTISKORCH_GIGA,    TYPE_FIRE,       MOVE_G_MAX_CENTIFERNO_P},
	{SPECIES_HATTERENE,           TYPE_FAIRY,      MOVE_G_MAX_SMITE_P},
	{SPECIES_GRIMMSNARL_GIGA,     TYPE_DARK,       MOVE_G_MAX_SNOOZE_P},
	{SPECIES_ALCREMIE_GIGA,       TYPE_FAIRY,      MOVE_G_MAX_FINALE_P},
	{SPECIES_COPPERAJAH_GIGA,     TYPE_STEEL,      MOVE_G_MAX_STEELSURGE_P},
	{SPECIES_DURALUDON_GIGA,      TYPE_DRAGON,     MOVE_G_MAX_DEPLETION_P},
	{SPECIES_URSHIFU_SINGLE_GIGA, TYPE_DARK,       MOVE_G_MAX_ONE_BLOW_P},
	{SPECIES_URSHIFU_RAPID_GIGA,  TYPE_WATER,      MOVE_G_MAX_RAPID_FLOW_P},
};

const u8 gText_MonDynamaxed[] = _("");
const u8 gText_SurroundedByGMaxVineLash[] = _("{B_SIDE}被藤蔓捆着了!");
const u8 gText_SurroundedByGMaxWildfire[] = _("{B_SIDE}被火包围了!");
const u8 gText_SurroundedByGMaxCannonade[] = _("{B_SIDE}被漩涡包围了!");
const u8 gText_SurroundedByGMaxVolcalith[] = _("{B_SIDE}队伍被石头包围了!");

static const u16 sDynamaxBandTable[] =
{
	ITEM_DYNAMAX_BAND,
};

static const u16 s4StarFrontierRaidBattleDrops[] =
{
	/*100 %*/ ITEM_PP_UP,
	/* 80 %*/ ITEM_HP_UP,
	/* 80 %*/ ITEM_POMEG_BERRY,
	/* 50 %*/ ITEM_LIECHI_BERRY,
	/* 50 %*/ ITEM_FULL_RESTORE,
	/* 30 %*/ ITEM_NORMAL_GEM,
	/* 30 %*/ ITEM_NONE,
	/* 25 %*/ ITEM_HEART_SCALE,
	/* 25 %*/ ITEM_NONE,
	/*  5 %*/ ITEM_LEFTOVERS,
	/*  4 %*/ ITEM_BOTTLE_CAP,
	/*  1 %*/ ITEM_GOLD_BOTTLE_CAP,
};

static const u16 s56StarFrontierRaidBattleDrops[] =
{
	/*100 %*/ ITEM_PP_UP,
	/* 80 %*/ ITEM_HP_UP,
	/* 80 %*/ ITEM_POMEG_BERRY,
	/* 50 %*/ ITEM_LIECHI_BERRY,
	/* 50 %*/ ITEM_OCCA_BERRY,
	/* 30 %*/ ITEM_NORMAL_GEM,
	/* 30 %*/ ITEM_HEART_SCALE,
	/* 25 %*/ ITEM_PP_MAX,
	/* 25 %*/ ITEM_LEFTOVERS,
	/*  5 %*/ ITEM_WISHING_PIECE,
	/*  4 %*/ ITEM_BOTTLE_CAP,
	/*  1 %*/ ITEM_GOLD_BOTTLE_CAP,
};

u16 GetDynamaxSpecies(u8 bank, bool8 checkGMaxInstead)
{
	u16 species = SPECIES(bank); //Prevents ditto too

	if (IsBannedHeldItemForDynamax(ITEM(bank)))
		return SPECIES_NONE;

	if (!checkGMaxInstead) //Checking regular Dynmax
	{
		if (IsBannedDynamaxSpecies(species))
			return SPECIES_NONE; //Certain Pokemon can't Dynamax

		return species; //Returning the species of the Pokemon is an indicator that they can Dynamax
	}
	else //Check Gigantamax
	{
		struct Pokemon* mon = GetBankPartyData(bank);
		return GetGigantamaxSpecies(mon->box.species, mon->box.gigantamax);
	}

	return SPECIES_NONE;
}

bool8 IsGMaxMove(u16 move)
{
	return move >= FIRST_G_MAX_MOVE && move <= LAST_G_MAX_MOVE;
}

bool8 CanDynamax(u8 bank)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DYNAMAX && !gBattleStruct->dynamaxData.used[bank])
		return GetDynamaxSpecies(bank, FALSE) != SPECIES_NONE;

	return FALSE;
}

bool8 CanGigantamax(u8 bank)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DYNAMAX && !gBattleStruct->dynamaxData.used[bank])
		return GetDynamaxSpecies(bank, TRUE) != SPECIES_NONE;

	return FALSE;
}

static const u8* DoDynamax(u8 bank)
{
	u16 dynamaxSpecies = GetDynamaxSpecies(bank, FALSE);

	if (dynamaxSpecies == SPECIES_NONE) //Can't Dynamax
		return NULL;

	gBattleScripting.battler = bank;
	gLastUsedItem = FindBankDynamaxBand(bank);

	return BattleScript_Dynamax;
}

static const u8* DoGigantamax(u8 bank)
{
	u16 gigantamaxSpecies = GetDynamaxSpecies(bank, TRUE);

	if (gigantamaxSpecies == SPECIES_NONE) //Can't Gigantamax
		return NULL;

	DoFormChange(bank, gigantamaxSpecies, FALSE, FALSE, FALSE);

	gBattleScripting.battler = bank;
	gLastUsedItem = FindBankDynamaxBand(bank);
	PREPARE_SPECIES_BUFFER(gBattleTextBuff1, gigantamaxSpecies);

	return BattleScript_Dynamax;
}

const u8* GetDynamaxScript(u8 bank)
{
	const u8* script = DoGigantamax(bank);
	if (script != NULL)
		return script;

	return DoDynamax(bank);
}

static bool8 IsBannedHeldItemForDynamax(u16 item)
{
	if (IsMegaZMoveBannedBattle())
		return FALSE; //These items have no effect so don't ban them

	return IsMegaStone(item)
		|| IsZCrystal(item)
		|| IsPrimalOrb(item);
}

bool8 IsGigantamaxSpecies(u16 species)
{
	const struct Evolution* evolutions = gEvolutionTable[species];
	u8 i;

	for (i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_GIGANTAMAX && evolutions[i].param == 0)
			return TRUE;
	}

	return FALSE;
}

bool8 IsDynamaxed(u8 bank)
{
	return gBattleStruct->dynamaxData.timer[bank] != 0;
}

bool8 IsGigantamaxed(u8 bank)
{
	return IsGigantamaxSpecies(SPECIES(bank));
}

bool8 HasDynamaxSymbol(u8 bank)
{
	return IsDynamaxed(bank) || IsGigantamaxed(bank);
}

//The following functions relate to raid battles:
bool8 IsRaidBattle(void)
{
	#ifdef FLAG_RAID_BATTLE
	return FlagGet(FLAG_RAID_BATTLE) && !(gBattleTypeFlags & BATTLE_TYPE_TRAINER);
	#else
	return FALSE;
	#endif
}

u8 GetDynamaxHPBoost(u8 bank)
{
	if (SPECIES(bank) == SPECIES_SHEDINJA)
		return 1;
	else if (IsRaidBattle() && bank == BANK_RAID_BOSS)
		return GetRaidBattleHPBoost();
	else
		return 2;
}

u8 GetMonDynamaxHPBoost(struct Pokemon* mon)
{
	if (GetMonData(mon, MON_DATA_SPECIES, NULL) == SPECIES_SHEDINJA)
		return 1;
	
	return 2;
}

u8 GetRaidBattleHPBoost(void)
{
	return 4;
}

bool8 ProtectedByMaxGuard(u8 bankDef, u16 move)
{
	if (gProtectStructs[bankDef].protected && IsDynamaxed(bankDef) && move != MOVE_FEINT && move != MOVE_MEAN_LOOK) //Mean Look is probably a GF bug
	{
		if (gBattleMoves[move].target & (MOVE_TARGET_DEPENDS | MOVE_TARGET_OPPONENTS_FIELD))
			return (gBattleMoves[move].flags & FLAG_PROTECT_AFFECTED) != 0;

		if (gBattleMoves[move].target & MOVE_TARGET_FOES_AND_ALLY && CheckTableForMove(gCurrentMove, gSpecialWholeFieldMoves))
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

u8 GetRaidBattleStatNullificationChance(u8 bank)
{
	u8 level = gBattleMons[bank].level;

	if (gDisableStructs[bank].isFirstTurn)
		return 0; //Don't use first attack with this

	#ifdef FLAG_BATTLE_FACILITY
	if (FlagGet(FLAG_BATTLE_FACILITY))
		level = MAX_LEVEL;
	#endif

	switch (level) {
		case 0 ... 19:
			return 0; //Never
		case 20 ... 40:
			return 20; //20 % chance before each attack
		case 41 ... 70:
			return 35; //35 % chance before each attack
		default:
			return 50; //50 % chance before each attack
	}
}

bool8 IsFrontierRaidBattle(void)
{
	#ifdef FLAG_BATTLE_FACILITY
	return IsRaidBattle() && FlagGet(FLAG_BATTLE_FACILITY);
	#else
	return FALSE;
	#endif
}

bool8 IsCatchableRaidBattle(void)
{
	return IsRaidBattle() && !FlagGet(FLAG_NO_CATCHING) && !FlagGet(FLAG_NO_CATCHING_AND_RUNNING);
}

bool8 IsBannedDynamaxSpecies(u16 species)
{
	switch (species) {
		case SPECIES_NONE:
		#ifdef SPECIES_ZACIAN
		case SPECIES_ZACIAN:
		#endif
		#ifdef SPECIES_ZAMAZENTA
		case SPECIES_ZAMAZENTA:
		#endif
		#ifdef SPECIES_ZACIAN_CROWNED
		case SPECIES_ZACIAN_CROWNED:
		#endif
		#ifdef SPECIES_ZAMAZENTA_CROWNED
		case SPECIES_ZAMAZENTA_CROWNED:
		#endif
		#ifdef SPECIES_ETERNATUS
		case SPECIES_ETERNATUS:
		#endif
			return TRUE;
	}

	if (IsMegaSpecies(species)
	||  IsRedPrimalSpecies(species)
	||  IsBluePrimalSpecies(species)
	||  IsUltraNecrozmaSpecies(species))
		return TRUE;

	return FALSE;
}

u16 GetGigantamaxBaseForm(u16 species)
{
	const struct Evolution* evolutions = gEvolutionTable[species];
	u8 i;

	for (i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_GIGANTAMAX && evolutions[i].param == 0)
			return evolutions[i].targetSpecies;
	}

	return SPECIES_NONE;
}

static u16 FindTrainerDynamaxBand(u16 trainerId)
{
	u8 i;
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK) || IsFrontierTrainerId(trainerId))
		return ITEM_DYNAMAX_BAND;

	for (i = 0; i < MAX_TRAINER_ITEMS; ++i)
	{
		if (IsItemDynamaxBand(gTrainers[trainerId].items[i]))
			return gTrainers[trainerId].items[i];
	}

	return ITEM_NONE;
}

void UpdateHPForDynamax(void)
{
	u32 newCurrentHp;
	u8 hpBoost;

	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}
	gActiveBattler = gBattleScripting.battler;

	#ifdef SPECIES_SHEDINJA
	if (SPECIES(gActiveBattler) == SPECIES_SHEDINJA) //Shedinja doesn't get a Dynamax HP boost
	{
		gBattleMoveDamage = 0;
		return;
	}
	#endif

	hpBoost = GetDynamaxHPBoost(gActiveBattler);

	if (IsDynamaxed(gActiveBattler))
	{
		gBattleMons[gActiveBattler].maxHP = MathMin(gBattleMons[gActiveBattler].maxHP * hpBoost, 0xFFFF);
		newCurrentHp = MathMin(gBattleMons[gActiveBattler].hp * hpBoost, 0xFFFF);

		gBattleMoveDamage = (newCurrentHp - gBattleMons[gActiveBattler].hp) * - 1;
	}
	else
	{
		//Get ceiling of HP divided by boost
		gBattleMons[gActiveBattler].maxHP = MathMax(gBattleMons[gActiveBattler].maxHP / hpBoost + (gBattleMons[gActiveBattler].maxHP & 1), 1);
		gBattleMons[gActiveBattler].hp = MathMax(gBattleMons[gActiveBattler].hp / hpBoost + (gBattleMons[gActiveBattler].hp & 1), 1);
	}

	BtlController_EmitSetMonData(0, REQUEST_MAX_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].maxHP);
	MarkBattlerForControllerExec(gActiveBattler);
}

bool8 IsRaidBossUsingRegularMove(u8 bank, u16 baseMove)
{
	return SPLIT(baseMove) == SPLIT_STATUS
		|| baseMove == MOVE_STRUGGLE
		|| (gRaidBattleStars < 4 && (gRandomTurnNumber & 3) == 0) //25 % chance to use regular damaging move
		|| (gRaidBattleStars >= 4 && (gRandomTurnNumber % 100 >= 90)) //Harder foes have a lower chance of using regular moves
		|| gBattleMons[bank].status2 & (STATUS2_RECHARGE | STATUS2_MULTIPLETURNS);
}

bool8 DoesDynamaxUsageStopMegaEvolution(u8 bank)
{
	return gBattleStruct->dynamaxData.used[bank]
		&& gBattleStruct->dynamaxData.partyIndex[SIDE(bank)] & gBitTable[gBattlerPartyIndexes[bank]];
}

static u16 GetTypeBasedMaxMove(u8 moveType, u8 moveSplit)
{
	if (moveType < TYPE_FIRE)
		return MOVE_MAX_STRIKE_P + (moveType * 2) + moveSplit;
	else if (moveType >= TYPE_FAIRY)
		return MOVE_MAX_STARFALL_P + ((moveType - TYPE_FAIRY) * 2) + moveSplit;
	else
		return MOVE_MAX_STRIKE_P + ((moveType - 1) * 2) + moveSplit;
}

static u8 GetMaxMoveType(u16 move, u8 bank, struct Pokemon* mon)
{
	u8 moveType = GetMoveTypeSpecialPreAbility(move, bank, mon);
	u16 maxMove ;

	if (moveType == 0xFF) //No overridden type
	{
		u8 moveSplit, ability;

		if (mon != NULL)
		{
			moveSplit = CalcMoveSplitFromParty(mon, move);
			ability = GetMonAbility(mon);
		}
		else
		{
			moveSplit = CalcMoveSplit(bank, move);
			ability = ABILITY(bank);
		}

		//Try to modify Max Move's type using ability
		maxMove = GetTypeBasedMaxMove(gBattleMoves[move].type, moveSplit);
		moveType = GetMoveTypeSpecialPostAbility(maxMove, ability, FALSE);
	}

	return moveType;
}

u16 GetGigantamaxSpecies(u16 species, bool8 canGigantamax)
{
	u32 i;
	const struct Evolution* evolutions = gEvolutionTable[species];

	if (canGigantamax) //Mon can Gigantamax
	{
		for (i = 0; i < EVOS_PER_MON; ++i)
		{
			if (evolutions[i].method == EVO_GIGANTAMAX)
			{
				//Ignore reversion information
				if (evolutions[i].param == 0) continue;

				//Any value other than 0 indicates G-Max potential
				return evolutions[i].targetSpecies;
			}
		}
	}

	return SPECIES_NONE;
}

static u16 GetGMaxMove(u8 moveType, u8 moveSplit, u16 species, bool8 canGigantamax)
{
	if (canGigantamax)
	{
		if (!IsGigantamaxSpecies(species))
			species = GetGigantamaxSpecies(species, canGigantamax);

		if (species != SPECIES_NONE)
		{
			u32 i;
			for (i = 0; i < ARRAY_COUNT(sGMaxMoveTable); ++i)
			{
				if (sGMaxMoveTable[i].species == species
				&&  sGMaxMoveTable[i].moveType == moveType)
				{
					return sGMaxMoveTable[i].gmaxMove + moveSplit;
				}
			}
		}
	}

	return MOVE_NONE;
}

u16 GetMaxMove(u8 bank, u8 moveIndex)
{
	u16 baseMove = gBattleMons[bank].moves[moveIndex];
	return GetMaxMoveByMove(bank, baseMove);
}

u16 GetMaxMoveByMove(u8 bank, u16 baseMove)
{
	u8 moveSplit = CalcMoveSplit(bank, baseMove);
	u8 moveType;
	u16 maxMove;

	if (baseMove == MOVE_NONE)
		return MOVE_NONE;

	if (!IsRaidBattle() || bank != BANK_RAID_BOSS) //Raid mon's don't care about these rules
	{
		if (IsBannedDynamaxSpecies(SPECIES(bank))
		|| IsBannedHeldItemForDynamax(ITEM(bank))
		|| DoesZMoveUsageStopDynamaxing(bank)) //No using Z-Move and Dynamaxing
			return MOVE_NONE;
	}

	if (moveSplit == SPLIT_STATUS)
		return MOVE_MAX_GUARD;

	moveType = GetMaxMoveType(baseMove, bank, NULL);
	maxMove = GetGMaxMove(moveType, moveSplit, SPECIES(bank), GetBankPartyData(bank)->box.gigantamax);
	if (maxMove != MOVE_NONE)
		return maxMove;

	return GetTypeBasedMaxMove(moveType, moveSplit);
}

//In theory, this function will do nothing as the regular forms revert should
//should take care of the reversion. This is to prevent bugs if the player
//gives themselves a Gigantmax to start the battle.
void GigantamaxRevert(struct Pokemon* party)
{
	u32 i;

	for (i = 0; i < PARTY_SIZE; ++i)
		TryRevertGigantamax(&party[i]);
}

void TryRevertGigantamax(struct Pokemon* mon)
{
	u16 baseSpecies = GetGigantamaxBaseForm(mon->box.species);

	if (baseSpecies != SPECIES_NONE)
	{
		mon->box.species = baseSpecies;
		mon->box.gigantamax = TRUE; //If encountered in the wild, now can permanently Gigantamax
		CalculateMonStats(mon);
	}
}

void TryRevertBankGigantamax(u8 bank)
{
	struct Pokemon* mon = GetBankPartyData(bank);
	u16 baseSpecies = GetGigantamaxBaseForm(GetMonData(mon, MON_DATA_SPECIES, NULL));

	if (baseSpecies != SPECIES_NONE) //Bank is gigantamaxed - can't check timer because already reset
	{
		if (mon->box.backupSpecies != SPECIES_NONE)
			baseSpecies = mon->box.backupSpecies;

		DoFormChange(bank, baseSpecies, FALSE, FALSE, FALSE);
		mon->box.gigantamax = TRUE; //If encountered in the wild, now can permanently Gigantamax
	}
}

bool8 HasRaidShields(u8 bank)
{
	return GetBattlerPosition(bank) == B_POSITION_OPPONENT_LEFT
		&& gBattleStruct->dynamaxData.raidShieldsUp;
}

static u8 GetRaidShieldHealthRatio(u8 bank)
{
	switch (gBattleMons[bank].level) {
		case 0 ... 19:
			return 1; //Never
		case 20 ... 40:
			return 2; //Every 1/2 health lost
		case 41 ... 70:
			return 3; //Every 1/3 health lost
		default:
			return 4; //Every 1/4 health lost
	}

}

u16 GetNextRaidShieldHP(u8 bank)
{
	u8 i, healthRatio;
	u16 cutOff, prevCutOff;

	if (HasRaidShields(bank) || !BATTLER_ALIVE(bank))
		return 0;

	healthRatio = GetRaidShieldHealthRatio(bank);
	cutOff = 0;

	for (i = 1; i <= healthRatio; ++i) //Up to max HP
	{
		prevCutOff = cutOff;
		cutOff = (gBattleMons[bank].maxHP / healthRatio) * i;

		if (i == healthRatio)
			cutOff = gBattleMons[bank].maxHP; //Fix Math Errors

		if (gBattleMons[bank].hp > prevCutOff && gBattleMons[bank].hp <= cutOff)
			return prevCutOff;
	}

	return 0;
}

bool8 IsMaxGuardUp(u8 bank)
{
	return gProtectStructs[bank].protected && IsDynamaxed(bank);
}

void CreateRaidShieldSprites(void)
{
	//Base stat total determines how many shields
	//Level determines how often the shields appear

	u8 i, numShields;
	u8 bank = BANK_RAID_BOSS;
	u16 baseStatTotal = GetBaseStatsTotal(SPECIES(bank));

	#ifdef FLAG_RAID_BATTLE_NO_FORCE_END
	if (!FlagGet(FLAG_RAID_BATTLE_NO_FORCE_END)) //Less shields for battle that ends in 10 turns
	#endif
	{
		switch (baseStatTotal) {
			case 0 ... 349:
				numShields = 1;
				break;
			case 350 ... 494:
				numShields = 2;
				break;
			case 495 ... 568:
				numShields = 3;
				break;
			default: //569+
				numShields = 4;
		}
	}
	#ifdef FLAG_RAID_BATTLE_NO_FORCE_END
	else
	{
		switch (baseStatTotal) {
			case 0 ... 349:
				numShields = 1;
				break;
			case 350 ... 494:
				numShields = 2;
				break;
			case 495 ... 568:
				numShields = 3;
				break;
			case 569 ... 599:
				numShields = 4;
				break;
			default:
				numShields = MAX_NUM_RAID_SHIELDS;
		}
	}
	#endif

	gBattleStruct->dynamaxData.shieldCount = numShields;
	LoadRaidShieldGfx();

	for (i = 0; i < MAX_NUM_RAID_SHIELDS; ++i)
		gBattleStruct->dynamaxData.shieldSpriteIds[i] = 0;

	for (i = 0; i < numShields - gBattleStruct->dynamaxData.shieldsDestroyed; ++i)
	{
		u8 spriteId = CreateSpriteAtEnd(&gRaidShieldSpriteTemplate, 0, 0, 0xFF);
		gBattleStruct->dynamaxData.shieldSpriteIds[i] = spriteId + 1;
		gSprites[spriteId].data[0] = bank;
		gSprites[spriteId].data[1] = -9 * i;
	}
}

static u16 FindPlayerDynamaxBand(void)
{
	u8 i;
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))
		return ITEM_DYNAMAX_BAND;

	for (i = 0; i < ARRAY_COUNT(sDynamaxBandTable); ++i)
	{
		if (CheckBagHasItem(sDynamaxBandTable[i], 1))
			return sDynamaxBandTable[i];
	}

	#ifdef DEBUG_DYNAMAX
		return ITEM_DYNAMAX_BAND; //Give player Dynamax Band if they have none
	#endif

	return ITEM_NONE;
}

static u16 FindBankDynamaxBand(u8 bank)
{
	#ifdef DEBUG_DYNAMAX
		if (bank + 1)
			return ITEM_DYNAMAX_BAND;
	#endif

	if (SIDE(bank) == B_SIDE_OPPONENT)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
		{
			if (GetBattlerPosition(bank) == B_POSITION_OPPONENT_LEFT)
				return FindTrainerDynamaxBand(gTrainerBattleOpponent_A);
			else
				return FindTrainerDynamaxBand(SECOND_OPPONENT);
		}
		else
			return FindTrainerDynamaxBand(gTrainerBattleOpponent_A);
	}
	else //SIDE_PLAYER
	{
		if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
		{
			if (GetBattlerPosition(bank) == B_POSITION_PLAYER_RIGHT)
				return FindTrainerDynamaxBand(VarGet(VAR_PARTNER));
			else
				return FindPlayerDynamaxBand();
		}
		else
			return FindPlayerDynamaxBand();
	}
}

static bool8 IsItemDynamaxBand(u16 item)
{
	u8 i;
	for (i = 0; i < ARRAY_COUNT(sDynamaxBandTable); ++i)
	{
		if (item == sDynamaxBandTable[i])
			return TRUE;
	}

	return FALSE;
}

bool8 DynamaxEnabled(u8 bank)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DYNAMAX)
	{
		if (FindBankDynamaxBand(bank) == ITEM_NONE)
		{
			#ifdef DEBUG_DYNAMAX
				return TRUE;
			#else
				return FALSE;
			#endif
		}

		return TRUE;
	}

	return FALSE;
}

bool8 HasBankDynamaxedAlready(u8 bank)
{
	if ((SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
	||  (SIDE(bank) == B_SIDE_OPPONENT && gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
	{
		return gBattleStruct->dynamaxData.used[bank];
	}

	return gBattleStruct->dynamaxData.used[bank]
		|| (IS_DOUBLE_BATTLE && gBattleStruct->dynamaxData.used[PARTNER(bank)]);
}

static u16 GetMonMaxMove(struct Pokemon* mon, u16 baseMove)
{
	u8 moveType = GetMaxMoveType(baseMove, 0, mon);
	u8 moveSplit = CalcMoveSplitFromParty(mon, baseMove);
	u16 maxMove = GetGMaxMove(moveType, moveSplit, mon->box.species, mon->box.gigantamax);
	if (maxMove != MOVE_NONE)
		return maxMove;

	return GetTypeBasedMaxMove(moveType, moveSplit);
}

bool8 MonCanUseMaxMoveWithEffect(struct Pokemon* mon, u8 maxEffect)
{
	u32 i;
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);

	if (IsBannedDynamaxSpecies(species)
	|| IsBannedHeldItemForDynamax(item))
		return FALSE;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
		if (move == MOVE_NONE)
			break;

		if (SPLIT(move) == SPLIT_STATUS)
			continue;

		move = GetMonMaxMove(mon, move);
		if (move != MOVE_NONE && gBattleMoves[move].z_move_effect == maxEffect)
			return TRUE;
	}

	return FALSE;
}

bool8 MonCanDynamax(struct Pokemon* mon)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DYNAMAX)
	{
		u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
		u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);

		if (IsBannedDynamaxSpecies(species)
		|| IsBannedHeldItemForDynamax(item))
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

bool8 MoveInMonMovesetThatCanChangeByGigantamaxing(struct Pokemon* mon)
{
	u32 i;
	if (mon->box.gigantamax)
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			u16 move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);

			if (IsGMaxMove(GetMonMaxMove(mon, move)))
				return TRUE;
		}
	}

	return FALSE;
}

bool8 PlayerHasNoMonsLeftThatCanDynamax(void)
{
	u8 i, firstMonId, lastMonId;
	struct Pokemon* party = LoadPartyRange(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), &firstMonId, &lastMonId);

	for (i = firstMonId; i < lastMonId; ++i)
	{
		if (MON_CAN_BATTLE(&party[i])
		&& MonCanDynamax(&party[i]))
			return FALSE;
	}

	return TRUE;
}

void TryFadeBankPaletteForDynamax(u8 bank, u16 paletteOffset)
{
	if (IsDynamaxed(bank)
	|| (IsRaidBattle() && bank == BANK_RAID_BOSS)) //So it stays lit up when you try to catch it
	{
		BlendPalette(paletteOffset, 16, 4, RGB(31, 0, 12)); //Dynamax Pinkish-Red
		CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
	}
}

s16 GetBattlerXCoord(u8 bank)
{
	if (IsRaidBattle() && SIDE(bank) == B_SIDE_OPPONENT)
		return sBattlerCoords[FALSE][B_POSITION_OPPONENT_LEFT].x; //Only 1 Pokemon so Single battle position

	return sBattlerCoords[IS_DOUBLE_BATTLE][GetBattlerPosition(bank)].x;
}

s16 GetBattlerYCoord(u8 bank)
{
	if (IsRaidBattle() && SIDE(bank) == B_SIDE_OPPONENT)
		return sBattlerCoords[FALSE][B_POSITION_OPPONENT_LEFT].y; //Only 1 Pokemon so Single battle position

	return sBattlerCoords[IS_DOUBLE_BATTLE][GetBattlerPosition(bank)].y;
}

void HandleDeadRaidMonAndDeadPlayer(void)
{
	if (RAID_BATTLE_END && !BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)))
		gBattleStruct->absentBattlerFlags &= ~gBitTable[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]; //So the player can still catch even if they have no Pokemon left
}

u8 GetBattleHealthbarColour(u8 filledPixelsCount, u8 bank)
{
	if (IsRaidBattle() && bank == BANK_RAID_BOSS)
	{
		//Raid Boss always has their HP bar red
		return HEALTHBOX_GFX_HP_BAR_RED;
	}
	else
	{
		if (filledPixelsCount > (B_HEALTHBAR_PIXELS * 50 / 100)) // more than 50 % hp
			return HEALTHBOX_GFX_HP_BAR_GREEN;
		else if (filledPixelsCount > (B_HEALTHBAR_PIXELS * 20 / 100)) // more than 20% hp
			return HEALTHBOX_GFX_HP_BAR_YELLOW;
		else
			return HEALTHBOX_GFX_HP_BAR_RED; // 20 % or less
	}
}

void EndBattleDynamaxRevert(u8 bank)
{
	if (BATTLER_ALIVE(bank) && IsDynamaxed(bank))
	{
		u16 hp, maxHP;
		struct Pokemon* mon = GetBankPartyData(bank);

		hp = GetBaseCurrentHP(bank);
		maxHP = GetBaseMaxHP(bank);
		SetMonData(mon, MON_DATA_HP, &hp);
		SetMonData(mon, MON_DATA_MAX_HP, &maxHP);
		gBattleStruct->dynamaxData.timer[bank] = 0;
	}
}

void TryBoostDynamaxHPAfterLevelUp(u8 bank) //Should only be called once all battle mon stats are updated
{
	struct Pokemon* mon;
	gActiveBattler = bank;

	#ifdef SPECIES_SHEDINJA
	if (SPECIES(gActiveBattler) == SPECIES_SHEDINJA) //Shedinja doesn't get a Dynamax HP boost
		return;
	#endif

	if (IsDynamaxed(gActiveBattler))
	{
		u8 hpBoost = GetDynamaxHPBoost(gActiveBattler);
		mon = GetBankPartyData(gActiveBattler);

		gBattleMons[gActiveBattler].hp = MathMin(gBattleMons[gActiveBattler].hp + (mon->maxHP * hpBoost - gBattleMons[gActiveBattler].maxHP), 0xFFFF); //Calculate proper new hp
		gBattleMons[gActiveBattler].maxHP = MathMin(mon->maxHP * hpBoost, 0xFFFF);

		//This is gonna bite me in the butt later
		mon->hp = gBattleMons[gActiveBattler].hp;
		mon->maxHP = gBattleMons[gActiveBattler].maxHP;

		/*
			It should be done this way, but the game crashes when trying to learn a new move, so I'll deal with
			this if I ever do link Raid battles.
		*/
		//BtlController_EmitSetMonData(0, REQUEST_ALL_BATTLE, 0, sizeof(gBattleMons[gActiveBattler]), &gBattleMons[gActiveBattler]);
		//MarkBattlerForControllerExec(gActiveBattler);
	}
}

//Called from Battle Script
void UpdateCurrentHealthForDynamaxEnd(void)
{
	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	gActiveBattler = gBattleScripting.battler;

	if (!IsDynamaxed(gActiveBattler))
	{
		BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
		MarkBattlerForControllerExec(gActiveBattler);
	}
}

//Called from Battle Script
void ClearBallAnimActiveBit(void)
{
	gBattleSpritesDataPtr->healthBoxesData[gBattleScripting.battler].ballAnimActive = FALSE;
}

void atkFF2F_setmaxmoveeffect(void)
{
	gBattlescriptCurrInstr += 1;

	if (IsRaidBattle()
	&& gBattlerTarget == BANK_RAID_BOSS
	&& gBattleStruct->dynamaxData.raidShieldsUp)
		return; //No special effect when move is blocked by shields

	gHitMarker |= HITMARKER_IGNORE_SUBSTITUTE;

	switch (gBattleMoves[gCurrentMove].z_move_effect) { //Stored here for simplicity
		case MAX_EFFECT_RAISE_TEAM_ATTACK:
		case MAX_EFFECT_RAISE_TEAM_DEFENSE:
		case MAX_EFFECT_RAISE_TEAM_SPEED:
		case MAX_EFFECT_RAISE_TEAM_SP_ATK:
		case MAX_EFFECT_RAISE_TEAM_SP_DEF:
			if (BATTLER_ALIVE(gBattlerAttacker) || BATTLER_ALIVE(PARTNER(gBattlerAttacker)))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveRaiseStatTeam;
			}
			break;

		case MAX_EFFECT_LOWER_ATTACK:
		case MAX_EFFECT_LOWER_DEFENSE:
		case MAX_EFFECT_LOWER_SPEED:
		case MAX_EFFECT_LOWER_SP_ATK:
		case MAX_EFFECT_LOWER_SP_DEF:
			if (BATTLER_ALIVE(gBattlerTarget) || BATTLER_ALIVE(PARTNER(gBattlerTarget)))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveLowerStatFoes;
			}
			break;

		case MAX_EFFECT_SUN:
			if (SetSunnyWeather())
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetWeather;
			}
			break;
		case MAX_EFFECT_RAIN:
			if (SetRainyWeather())
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetWeather;
			}
			break;
		case MAX_EFFECT_SANDSTORM:
			if (SetSandstormWeather())
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetWeather;
			}
			break;
		case MAX_EFFECT_HAIL:
			if (SetHailWeather())
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetWeather;
			}
			break;

		case MAX_EFFECT_ELECTRIC_TERRAIN:
		case MAX_EFFECT_GRASSY_TERRAIN:
		case MAX_EFFECT_MISTY_TERRAIN:
		case MAX_EFFECT_PSYCHIC_TERRAIN:
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MaxMoveSetTerrain;
			break;

		case MAX_EFFECT_VINE_LASH:
			if (!BankSideHasGMaxVineLash(gBattlerTarget))
			{
				gBattleStruct->maxVineLashTimers[SIDE(gBattlerTarget)] = 4;
				gBattleStringLoader = gText_SurroundedByGMaxVineLash;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			}
			break;

		case MAX_EFFECT_WILDFIRE:
			if (!BankSideHasGMaxWildfire(gBattlerTarget))
			{
				gBattleStruct->maxWildfireTimers[SIDE(gBattlerTarget)] = 4;
				gBattleStringLoader = gText_SurroundedByGMaxWildfire;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			}
			break;

		case MAX_EFFECT_CANNONADE:
			if (!BankSideHasGMaxCannonade(gBattlerTarget))
			{
				gBattleStruct->maxCannonadeTimers[SIDE(gBattlerTarget)] = 4;
				gBattleStringLoader = gText_SurroundedByGMaxCannonade;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			}
			break;

		case MAX_EFFECT_EFFECT_SPORE_FOES:
			if (BATTLER_ALIVE(gBattlerTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerTarget))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveEffectSporeFoes;
			}
			break;

		case MAX_EFFECT_PARALYZE_FOES:
			if (BATTLER_ALIVE(gBattlerTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerTarget))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveParalyzeFoes;
			}
			break;

		case MAX_EFFECT_CONFUSE_FOES_PAY_DAY:
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MaxMovePayDayConfuseFoes;
			break;

		case MAX_EFFECT_CRIT_PLUS:
			if ((BATTLER_ALIVE(gBattlerAttacker) && gBattleStruct->chiStrikeCritBoosts[gBattlerAttacker] < 3) //Capped at 3
			|| ((IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerAttacker))) && gBattleStruct->chiStrikeCritBoosts[PARTNER(gBattlerAttacker)] < 3))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveCritUp;
			}
			break;

		case MAX_EFFECT_MEAN_LOOK:
			if ((BATTLER_ALIVE(gBattlerTarget) && !IsOfType(gBattlerTarget, TYPE_GHOST) && !(gBattleMons[gBattlerTarget].status2 & STATUS2_ESCAPE_PREVENTION))
			|| (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerTarget)) && !IsOfType(PARTNER(gBattlerTarget), TYPE_GHOST) && !(gBattleMons[PARTNER(gBattlerTarget)].status2 & STATUS2_ESCAPE_PREVENTION)))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveTrap;
			}
			break;

		case MAX_EFFECT_AURORA_VEIL:
			if (!gBattleStruct->AuroraVeilTimers[SIDE(gBattlerAttacker)])
			{
				if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_LIGHT_CLAY)
					gBattleStruct->AuroraVeilTimers[SIDE(gBattlerAttacker)] = 8;
				else
					gBattleStruct->AuroraVeilTimers[SIDE(gBattlerAttacker)] = 5;

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				gBattleStringLoader = gText_SetAuroraVeil;
			}
			break;

		case MAX_EFFECT_INFATUATE_FOES:
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MaxMoveInfatuation;
			break;

		case MAX_EFFECT_RECYCLE_BERRIES:
			if ((Random() & 1) == 0 //50 % of the time
			&& (BATTLER_ALIVE(gBattlerAttacker) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerAttacker)))) //Someone on this side is alive
			&& ((ITEM(gBattlerAttacker) == ITEM_NONE && IsBerry(SAVED_CONSUMED_ITEMS(gBattlerAttacker))) //Check for a Berry to restore
			 || (IS_DOUBLE_BATTLE && ITEM(PARTNER(gBattlerAttacker)) == ITEM_NONE && IsBerry(SAVED_CONSUMED_ITEMS(PARTNER(gBattlerAttacker))))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveRecycle;
			}
			break;

		case MAX_EFFECT_POISON_FOES:
			if (BATTLER_ALIVE(gBattlerTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerTarget))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMovePoisonFoes;
			}
			break;

		case MAX_EFFECT_STEALTH_ROCK:
			if (gSideTimers[SIDE(gBattlerTarget)].srAmount == 0)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetStealthRock;
			}
			break;

		case MAX_EFFECT_DEFOG:
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MaxMoveDefog;
			break;

		case MAX_EFFECT_POISON_PARALYZE_FOES:
			if (BATTLER_ALIVE(gBattlerTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerTarget))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMovePoisonParalyzeFoes;
			}
			break;

		case MAX_EFFECT_HEAL_TEAM:
			if ((BATTLER_ALIVE(gBattlerAttacker) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerAttacker))))
			&& (!BATTLER_MAX_HP(gBattlerAttacker) || (IS_DOUBLE_BATTLE && !BATTLER_MAX_HP(PARTNER(gBattlerAttacker)))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveHealTeam;
			}
			break;

		case MAX_EFFECT_SPITE:
			if (BATTLER_ALIVE(gBattlerTarget) && TrySpitePPReduce(gBattlerTarget, 2))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSpite;
			}
			break;

		case MAX_EFFECT_GRAVITY:
			if (!IsGravityActive())
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetGravity;
			}
			break;

		case MAX_EFFECT_VOLCAITH_FOES:
			if (!BankSideHasGMaxVolcalith(gBattlerTarget))
			{
				gBattleStruct->maxVolcalithTimers[SIDE(gBattlerTarget)] = 4;
				gBattleStringLoader = gText_SurroundedByGMaxVolcalith;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			}
			break;

		case MAX_EFFECT_SANDBLAST_FOES:
		case MAX_EFFECT_FIRE_SPIN_FOES:
			if ((BATTLER_ALIVE(gBattlerTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerTarget))))
			&&  (!(gBattleMons[gBattlerTarget].status2 & STATUS2_WRAPPED) || (IS_DOUBLE_BATTLE && !(gBattleMons[PARTNER(gBattlerTarget)].status2 & STATUS2_WRAPPED))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveTrapAndDamageFoes;
			}
			break;

		case MAX_EFFECT_YAWN_FOE:
			if (BATTLER_ALIVE(gBattlerTarget)
			&& CanBeYawned(gBattlerTarget)
			&& !(gStatuses3[gBattlerTarget] & STATUS3_YAWN)
			&& (Random() & 1) == 0) //50 % chance target is put to sleep
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetYawn;
			}
			break;

		case MAX_EFFECT_LOWER_EVASIVENESS_FOES:
			if (BATTLER_ALIVE(gBattlerTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerTarget))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveLowerEvasivenessFoes;
			}
			break;

		case MAX_EFFECT_AROMATHERAPY:
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MaxMoveAromatherapy;
			break;

		case MAX_EFFECT_CONFUSE_FOES:
			if ((BATTLER_ALIVE(gBattlerTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerTarget))))
			&&  (CanBeConfused(gBattlerTarget, TRUE) || (IS_DOUBLE_BATTLE && CanBeConfused(PARTNER(gBattlerTarget), TRUE)))) //Is it worth it to push the script
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveConfuseFoes;
			}
			break;

		case MAX_EFFECT_TORMENT_FOES:
			if ((BATTLER_ALIVE(gBattlerTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerTarget))))
			&&  (CanBeTormented(gBattlerTarget) || (IS_DOUBLE_BATTLE && CanBeTormented(PARTNER(gBattlerTarget))))) //Is it worth it to push the script
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveTormentFoes;
			}
			break;

		case MAX_EFFECT_LOWER_SPEED_2_FOES:
			if (BATTLER_ALIVE(gBattlerTarget) || (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(gBattlerTarget))))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveLowerSpeed2Foes;
			}
			break;

		case MAX_EFFECT_STEELSURGE:
			if (gSideTimers[SIDE(gBattlerTarget)].steelsurge == 0)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MaxMoveSetStealthRock;
			}
			break;
	}
}


void SetMaxMoveStatRaiseEffect(void)
{
	u8 statId = (gBattleMoves[gCurrentMove].z_move_effect - MAX_EFFECT_RAISE_TEAM_ATTACK);
	gBattleCommunication[MOVE_EFFECT_BYTE] = (MOVE_EFFECT_ATK_PLUS_1 + statId) | MOVE_EFFECT_AFFECTS_USER;
}

void SetMaxMoveStatLowerEffect(void)
{
	u8 statId = (gBattleMoves[gCurrentMove].z_move_effect - MAX_EFFECT_LOWER_ATTACK);
	gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_ATK_MINUS_1 + statId;
}

void PickRandomGMaxBefuddleEffect(void)
{
	do
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = Random() & 3;
	} while (gBattleCommunication[MOVE_EFFECT_BYTE] == 0);

	switch (gBattleCommunication[MOVE_EFFECT_BYTE]) {
		case MOVE_EFFECT_SLEEP:
			if (CanBePutToSleep(gBattlerTarget, TRUE))
				gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
			break;
		case MOVE_EFFECT_POISON:
			if (CanBePoisoned(gBattlerTarget, gBattlerAttacker, TRUE))
				gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
			break;
		case MOVE_EFFECT_BURN: //Gets changed to Paralysis
			gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
			if (CanBeParalyzed(gBattlerTarget, TRUE))
				gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
			break;
	}
}

void SetGMaxVoltCrashEffect(void)
{
	gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
	if (CanBeParalyzed(gBattlerTarget, TRUE))
		gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
}

void SetGMaxMalodorEffect(void)
{
	gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_POISON;
	if (CanBePoisoned(gBattlerTarget, gBattlerAttacker, TRUE))
		gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
}

void SetGMaxChiStrikeEffect(void)
{
	gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	if (BATTLER_ALIVE(gBattlerAttacker) && gBattleStruct->chiStrikeCritBoosts[gBattlerAttacker] < 3) //Capped at 3
		++gBattleStruct->chiStrikeCritBoosts[gBattlerAttacker];
}

void PickRandomGMaxStunshockEffect(void)
{
	if ((Random() & 1) == 0)
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_POISON;
		if (CanBePoisoned(gBattlerTarget, gBattlerAttacker, TRUE))
			gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
	}
	else
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
		if (CanBeParalyzed(gBattlerTarget, TRUE))
			gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
	}
}

void SetGMaxSmiteEffect(void)
{
	gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_CONFUSION;
	if (CanBeConfused(gBattlerTarget, TRUE))
		gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked on line above
}

bool8 IsMaxMoveWithWeatherEffect(u16 move)
{
	if (IsAnyMaxMove(move))
	{
		switch (gBattleMoves[move].z_move_effect) {
			case MAX_EFFECT_SUN:
			case MAX_EFFECT_RAIN:
			case MAX_EFFECT_SANDSTORM:
			case MAX_EFFECT_HAIL:
				return TRUE;
		}
	}

	return FALSE;
}

bool8 IsMaxMoveWithTerrainEffect(u16 move)
{
	if (IsAnyMaxMove(move))
	{
		switch (gBattleMoves[move].z_move_effect) {
			case MAX_EFFECT_ELECTRIC_TERRAIN:
			case MAX_EFFECT_GRASSY_TERRAIN:
			case MAX_EFFECT_MISTY_TERRAIN:
			case MAX_EFFECT_PSYCHIC_TERRAIN:
				return TRUE;
		}
	}

	return FALSE;
}

bool8 IsMaxMoveWithStatusEffect(u16 move)
{
	if (IsAnyMaxMove(move))
	{
		switch (gBattleMoves[move].z_move_effect) {
			case MAX_EFFECT_EFFECT_SPORE_FOES:
			case MAX_EFFECT_POISON_FOES:
			case MAX_EFFECT_PARALYZE_FOES:
			case MAX_EFFECT_POISON_PARALYZE_FOES:
				return TRUE;
		}
	}

	return FALSE;
}

bool8 IsMaxMoveWithConfusionEffect(u16 move)
{
	if (IsAnyMaxMove(move))
	{
		switch (gBattleMoves[move].z_move_effect) {
			case MAX_EFFECT_CONFUSE_FOES:
			case MAX_EFFECT_CONFUSE_FOES_PAY_DAY:
				return TRUE;
		}
	}

	return FALSE;
}

bool8 IsMaxMoveWithTrapDamageEffect(u16 move)
{
	if (IsAnyMaxMove(move))
	{
		switch (gBattleMoves[move].z_move_effect) {
			case MAX_EFFECT_SANDBLAST_FOES:
			case MAX_EFFECT_FIRE_SPIN_FOES:
				return TRUE;
		}
	}

	return FALSE;
}

bool8 IsMaxMoveWithEffect(u16 move, u8 effect)
{
	return IsAnyMaxMove(move) && gBattleMoves[move].z_move_effect == effect;
}

bool8 ShouldCreateRaidShields(u8 bank)
{
	u8 i, healthRatio;

	if (HasRaidShields(bank) || !BATTLER_ALIVE(bank))
		return FALSE;

	healthRatio = GetRaidShieldHealthRatio(bank);

	for (i = 1; i <= healthRatio; ++i)
	{
		u16 cutOff = (gBattleMons[bank].maxHP / healthRatio) * i;

		if (i == healthRatio)
			cutOff = gBattleMons[bank].maxHP; //Fix Math Errors

		if (gBattleMons[bank].hp <= cutOff + (gBattleMons[bank].maxHP / 16) //Give some leeway for throwing up a shield
		&&  gBattleStruct->dynamaxData.turnStartHP > cutOff)
			return TRUE;
	}

	return FALSE;
}

u8 GetRaidBattleKOStatIncreaseAmount(u8 bank)
{
	u8 level = gBattleMons[bank].level;
	if (FlagGet(FLAG_BATTLE_FACILITY))
		level = MAX_LEVEL;

	switch (level) {
		case 0 ... 19:
			return 0; //Never
		case 20 ... 40:
			return 1;
		case 41 ... 70:
			return 2;
		default:
			return 3;
	}
}

u8 GetRaidBattleRepeatedAttackChance(u8 bank)
{
	u8 level = gBattleMons[bank].level;
	if (FlagGet(FLAG_BATTLE_FACILITY))
		level = MAX_LEVEL;

	switch (level) {
		case 0 ... 19:
			return 0; //Never
		case 20 ... 40:
			return 30; //30 % of the time after KO or Status Move
		case 41 ... 70:
			return 50; //50 % of the time after KO or Status Move
		default:
			return 70; //70 % of the time after KO or Status Move
	}
}

static u8 GetRaidMapSectionId(void)
{
	u8 currRegionMapSecId = GetCurrentRegionMapSectionId();

	return currRegionMapSecId - MAPSEC_DYNAMIC;
}

static u32 GetRaidRandomNumber(void)
{
	//Make sure no values are 0
	u8 dayOfWeek = (gClock.dayOfWeek == 0) ? 8 : gClock.dayOfWeek;
	u8 hour = (gClock.hour == 0) ? 24 : gClock.hour;
	u8 day = (gClock.day == 0) ? 32 : gClock.day;
	u8 month = (gClock.month == 0) ? 13 : gClock.month;

	u8 lastMapGroup = (gSaveBlock1Ptr->dynamicWarp.mapGroup == 0) ? 0xFF : gSaveBlock1Ptr->dynamicWarp.mapGroup;
	u8 lastMapNum = (gSaveBlock1Ptr->dynamicWarp.mapNum == 0) ? 0xFF : gSaveBlock1Ptr->dynamicWarp.mapNum;
	u8 lastWarpId = (gSaveBlock1Ptr->dynamicWarp.warpId == 0) ? 0xFF : gSaveBlock1Ptr->dynamicWarp.warpId;
	u16 lastPos = (gSaveBlock1Ptr->dynamicWarp.x + gSaveBlock1Ptr->dynamicWarp.y == 0) ? 0xFFFF : (u16) (gSaveBlock1Ptr->dynamicWarp.x + gSaveBlock1Ptr->dynamicWarp.y);
	#ifdef VAR_RAID_NUMBER_OFFSET
	u16 offset = VarGet(VAR_RAID_NUMBER_OFFSET); //Setting this var changes all the raid spawns for the current hour (helps with better Wishing Piece)
	#else
	u16 offset = 0;
	#endif

	return ((hour * (day + month) * lastMapGroup * (lastMapNum + lastWarpId + lastPos)) + ((hour * (day + month)) ^ dayOfWeek) + offset) ^ T1_READ_32(gSaveBlock2Ptr->playerTrainerId);
}

static bool8 ShouldTryGigantamaxRaidMon(void)
{
	return gRaidBattleStars >= 6 //6-star Raid
		&& (GetRaidRandomNumber() % 100 >= 95 || GetRaidRandomNumber() % 100 < 20); //25% chance
}

void DetermineRaidStars(void)
{
	u8 numBadges = GetOpenWorldBadgeCount();
	u8 min, max;
	u32 randomNum;
	if (FlagGet(FLAG_BATTLE_FACILITY))
		numBadges = 9; //Battle Frontier Demo

	min = gRaidBattleStarsByBadges[numBadges][0];
	max = gRaidBattleStarsByBadges[numBadges][1];
	randomNum = GetRaidRandomNumber();

	if (min == max)
		gRaidBattleStars = min;
	else
		gRaidBattleStars = (randomNum % ((max + 1) - min)) + min;
}

//Must call DetermineRaidStars first
void DetermineRaidSpecies(void)
{
	u16 index, altSpecies;
	u8 numStars = gRaidBattleStars;
	const struct RaidData* raid = &gRaidsByMapSection[GetRaidMapSectionId()][numStars];

	if (FlagGet(FLAG_BATTLE_FACILITY)) //Battle Tower Demo
	{
		const struct BattleTowerSpread* spread;

		if (GetRaidRandomNumber() % 100 >= 90) //10 % chance
		{
			index = GetRaidRandomNumber() % gNumFrontierLegendarySpreads;
			spread = &gFrontierLegendarySpreads[index];
		}
		else
		{
			index = GetRaidRandomNumber() % gNumFrontierSpreads;
			spread = &gFrontierSpreads[index];
		}

		gRaidBattleSpecies = spread->species; //Set initial species

		altSpecies = GetMegaSpecies(spread->species, spread->item, spread->moves); //Try Mega Evolve the mon
		if (altSpecies != SPECIES_NONE)
			gRaidBattleSpecies = altSpecies; //Update with Mega Evolved form
		else
		{
			//Because of the amount of Pokemon that can appear in the Frontier, always Gigantamax
			//Pokemon that are able to.
			altSpecies = GetGigantamaxSpecies(spread->species, TRUE);
			if (altSpecies != SPECIES_NONE)
				gRaidBattleSpecies = altSpecies; //Update with Gigantamax form
		}

		gPokeBackupPtr = spread; //Save spread pointer for later
	}
	else if (raid->data != NULL)
	{
		index = GetRaidRandomNumber() % raid->amount;
		gRaidBattleSpecies = raid->data[index].species;

		if (ShouldTryGigantamaxRaidMon())
		{
			altSpecies = GetGigantamaxSpecies(raid->data[index].species, TRUE);
			if (altSpecies != SPECIES_NONE)
				gRaidBattleSpecies = altSpecies; //Update with Gigantamax form
		}
	}
	else
		gRaidBattleSpecies = SPECIES_NONE;
}

void DetermineRaidLevel(void)
{
	u8 numStars = gRaidBattleStars;
	u8 min = gRaidBattleLevelRanges[numStars][0];
	u8 max = gRaidBattleLevelRanges[numStars][1];
	u32 randomNum = GetRaidRandomNumber();

	if (FlagGet(FLAG_BATTLE_FACILITY)) //Battle Frontier Demo
		gRaidBattleLevel = VarGet(VAR_BATTLE_FACILITY_POKE_LEVEL);
	else if (min == max)
		gRaidBattleLevel = min;
	else
		gRaidBattleLevel = (randomNum % (max - min)) + min;
}

u8 GetRandomRaidLevel(void)
{
	u8 numStars = gRaidBattleStars;

	if (FlagGet(FLAG_BATTLE_FACILITY)) //Battle Frontier Demo
		return VarGet(VAR_BATTLE_FACILITY_POKE_LEVEL);

	return RandRange(gRaidBattleLevelRanges[numStars][0], gRaidBattleLevelRanges[numStars][1]);
}

u8 GetRaidRecommendedLevel(void)
{
	u8 numStars = gRaidBattleStars;

	if (FlagGet(FLAG_BATTLE_FACILITY)) //Battle Frontier Demo
		return VarGet(VAR_BATTLE_FACILITY_POKE_LEVEL);

	return gRaidBattleLevelRanges[numStars][1] + 5; //Max level + 5
}

void DetermineRaidPartners(bool8* checkedPartners, u8 maxPartners)
{
	u32 i, index;
	u8 numStars = gRaidBattleStars;
	u16 numMarked = 0;
	u16 numViable = 0;
	u32 randomNum = GetRaidRandomNumber();

	for (i = 1; i < /*1000*/ 0xFFFFFFFF; ++i)
	{
		if (randomNum == 0) //0 causes an infinite loop
			randomNum = 0xFFFFFFFF;

		randomNum ^= i;
		index = randomNum % gNumRaidPartners;

		if (checkedPartners[index] == 0)
		{
			++numMarked;

			if (gRaidPartners[index].spreads[numStars] != NULL)
			{
				checkedPartners[index] = TRUE;
				++numViable;
			}
			else
				checkedPartners[index] = 0xFF;
		}

		if (numViable >= maxPartners) //Found at least 3 partners
			return;

		if (numMarked >= gNumRaidPartners)
			break;
	}

	if (numMarked < gNumRaidPartners) //Couldn't mark off everyone
	{
		for (i = 0; i < gNumRaidPartners; ++i)
		{
			if (gRaidPartners[i].spreads[numStars] != NULL)
			{
				checkedPartners[i] = TRUE;
				++numViable;
			}

			if (numViable >= maxPartners) //Found at least 3 partners
				return;
		}
	}
}

u8 GetRaidSpeciesAbilityNum(u16 species)
{
	u32 i = 0;
	u8 numStars = gRaidBattleStars;
	const struct RaidData* raid = &gRaidsByMapSection[GetRaidMapSectionId()][numStars];

	if (IsGigantamaxSpecies(species))
		return RAID_ABILITY_RANDOM_ALL; //Gigantamax Pokemon can have any one of their abilities

	if (raid->data != NULL)
	{
		for (i = 0; i < raid->amount; ++i)
		{
			if (raid->data[i].species == species) //Max one species per dataset
				return raid->data[i].ability;
		}
	}

	return RAID_ABILITY_RANDOM_1_2;
}

u8 GetRaidEggMoveChance(void)
{
	return gRaidBattleEggMoveChances[gRaidBattleStars];
}

bool8 ShouldShowOpponentBattlerSprite(struct Pokemon* mon)
{
	return mon->hp > 0 || IsCatchableRaidBattle();
}

bool8 HasRaidBattleAlreadyBeenDone(void)
{
	return FlagGet(FIRST_RAID_BATTLE_FLAG + GetRaidMapSectionId());
}

void sp119_SetRaidBattleFlag(void)
{
	FlagSet(FIRST_RAID_BATTLE_FLAG + GetRaidMapSectionId());
}

//Input: Var8000 - 0: This Flag Only
//				   1: All Flags
void sp11A_ClearRaidBattleFlag(void)
{
	u32 i;
	switch (gSpecialVar_0x8000) {
		case 0: //Clear this area's flag
			FlagClear(FIRST_RAID_BATTLE_FLAG + GetRaidMapSectionId());
			break;
		case 1: //Clear all flags:
			for (i = 0; i < KANTO_MAPSEC_COUNT; ++i)
				FlagClear(FIRST_RAID_BATTLE_FLAG + i);
			break;
	}
}

void sp11B_AllRaidBattlesCompleted(void)
{
	u32 i;
	gSpecialVar_Result = FALSE;

	for (i = 0; i < KANTO_MAPSEC_COUNT; ++i)
	{
		if (gRaidsByMapSection[i] != NULL)
		{
			if (!FlagGet(FIRST_RAID_BATTLE_FLAG + i))
				return;
		}
	}

	gSpecialVar_Result = TRUE; //All Raid Battles available were completed
}

extern const u16 gItemsByType[];
u16 GetRaidRewardAmount(u16 item)
{
	if (GetPocketByItemId(item) == POCKET_POKE_BALLS)
		return Random() % 5 + 1; //1 - 5

	if (IsBerry(item))
		return Random() % 5 + 1; //1 - 5

	switch (gItemsByType[item]) {
		case ITEM_TYPE_HEALTH_RECOVERY:
		case ITEM_TYPE_STATUS_RECOVERY:
			return Random() % 5 + 1; //1 - 5
		case ITEM_TYPE_PP_RECOVERY:
		case ITEM_TYPE_STAT_BOOST_DRINK:
			return Random() % 3 + 1; //1 - 3
		case ITEM_TYPE_STAT_BOOST_WING:
			return Random() % 21 + 10; //10 - 30
		case ITEM_TYPE_SHARD:
			return Random() % 10 + 1; //1 - 10
		default:
			return 1;
	}
}

static u16 ModifyFrontierRaidDropItem(u16 item)
{
	switch (item) {
		case ITEM_HP_UP: //HP UP to Carbos
			item += RandRange(0, NUM_STATS);
			if (item >= ITEM_RARE_CANDY) //For some reason it comes before Zinc
				item = ITEM_ZINC;
			break;
		case ITEM_POMEG_BERRY:
			item += RandRange(0, NUM_STATS); //Pomeg - Tamato
			break;
		case ITEM_LIECHI_BERRY:
			item += RandRange(0, NUM_STATS + 1); //Liechi - Starf
			break;
	}

	return item;
}

//Input: VAR_TEMP_0 = 0
void sp11C_GiveRaidBattleRewards(void)
{
	u32 i = 0;
	u8 numStars = gRaidBattleStars;
	const struct RaidData* raid = &gRaidsByMapSection[GetRaidMapSectionId()][numStars];
	gSpecialVar_Result = FALSE; //Not done giving rewards

	if (raid->data != NULL)
	{
		for (i = 0; i < raid->amount; ++i)
		{
			if (raid->data[i].species == gRaidBattleSpecies) //Max one species per dataset
			{
				for (; VarGet(VAR_TEMP_0) < MAX_RAID_DROPS; ++*(GetVarPointer(VAR_TEMP_0)))
				{
					u8 dropNum = VarGet(VAR_TEMP_0);

					if (raid->data[i].drops[dropNum] != ITEM_NONE
					&& Random32() % 100 < sRaidBattleDropRates[dropNum])
					{
						gSpecialVar_LastTalked = 0xFD; //So no event objects disappear
						gSpecialVar_0x8000 = raid->data[i].drops[(*(GetVarPointer(VAR_TEMP_0)))++];
						gSpecialVar_0x8001 = GetRaidRewardAmount(gSpecialVar_0x8000);
						return;
					}
				}
			}
		}
	}
	else if (FlagGet(FLAG_BATTLE_FACILITY))
	{
		for (; VarGet(VAR_TEMP_0) < MAX_RAID_DROPS; ++*(GetVarPointer(VAR_TEMP_0)))
		{
			const u16* table = (gRaidBattleStars <= 4) ? s4StarFrontierRaidBattleDrops : s56StarFrontierRaidBattleDrops;

			if (table[VarGet(VAR_TEMP_0)] != ITEM_NONE
			&& Random() % 100 < sRaidBattleDropRates[VarGet(VAR_TEMP_0)])
			{
				gSpecialVar_LastTalked = 0xFD; //So no event objects disappear
				gSpecialVar_0x8000 = table[(*(GetVarPointer(VAR_TEMP_0)))++];
				gSpecialVar_0x8000 = ModifyFrontierRaidDropItem(gSpecialVar_0x8000);
				gSpecialVar_0x8001 = GetRaidRewardAmount(gSpecialVar_0x8000);
				return;
			}
		}
	}

	gSpecialVar_Result = TRUE; //Done giving rewards
}
