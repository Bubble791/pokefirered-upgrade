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
#include "string_util.h"
#include "event_data.h"
#include "constants/abilities.h"
#include "constants/hold_effects.h"
#include "constants/battle_move_effects.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/trainers.h"

static const u16 sKeystoneTable[] =
{
	ITEM_MEGA_RING,
};

static bool8 IsItemKeystone(u16 item);
static u16 FindTrainerKeystone(u16 trainerId);
static u16 FindPlayerKeystone(void);
static u16 FindBankKeystone(u8 bank);

const struct Evolution* CanMegaEvolve(u8 bank, bool8 CheckUBInstead)
{
    struct Pokemon* mon = GetBankPartyData(bank);
	const struct Evolution* evolutions = gEvolutionTable[mon->box.species];
	int i, j;

	if (!CheckUBInstead && !MegaEvolutionEnabled(bank)) //Ultra Burst doesn't need Mega Ring
		return NULL;

	if (IsMegaZMoveBannedBattle()
	|| (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(bank) == B_SIDE_OPPONENT)) //Wild mons (like Rayquaza) can't Mega Evolve
		return NULL;

	for (i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_MEGA)
		{
			//Ignore reversion information
			if (evolutions[i].param == 0) continue;

			//Check for held item
			if ((!CheckUBInstead && evolutions[i].unknown == MEGA_VARIANT_STANDARD) || (CheckUBInstead && evolutions[i].unknown == MEGA_VARIANT_ULTRA_BURST))
			{
				if (evolutions[i].param == mon->box.heldItem)
				{
					return &evolutions[i];
				}
			}
			else if (evolutions[i].unknown == MEGA_VARIANT_WISH && !CheckUBInstead
			&& !IsZCrystal(mon->box.heldItem)) //If Mega Rayquaza holds a Z-Crystal it can't Mega Evolve
			{
				if (!(gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
				|| RayquazaCanMegaEvolveInFrontierBattle())
				{
					//Check learned moves
					for (j = 0; j < MAX_MON_MOVES; ++j)
					{
						if (evolutions[i].param == mon->box.moves[j])
							return &evolutions[i];
					}
				}
			}
		}
	}

	return NULL;
}

//Assumes Wish Evolution isn't important
u16 GetMegaSpecies(u16 species, u16 item, const u16* moves)
{
	const struct Evolution* evolutions = gEvolutionTable[species];
	int i, j;

	for (i = 0; i < EVOS_PER_MON; ++i) {
		if (evolutions[i].method == EVO_MEGA)
		{
			//Ignore reversion information
			if (evolutions[i].param == 0) continue;

			//Check for held item
			if (evolutions[i].unknown == MEGA_VARIANT_STANDARD || evolutions[i].unknown == MEGA_VARIANT_ULTRA_BURST)
			{
				if (evolutions[i].param == item)
					return evolutions[i].targetSpecies;
			}
			else if (evolutions[i].unknown == MEGA_VARIANT_WISH && moves != NULL)
			{
				for (j = 0; j < MAX_MON_MOVES; ++j)
				{
					if (evolutions[i].param == moves[j])
						return evolutions[i].targetSpecies;
				}
			}
		}
	}

	return SPECIES_NONE;
}

ability_t GetBankMegaFormAbility(u8 bank)
{
	const struct Evolution* evos;

	if (!IsAbilitySuppressed(bank))
	{
		evos = CanMegaEvolve(bank, FALSE);
		if (evos != NULL)
		{
			return TryRandomizeAbility(gBaseStats[evos->targetSpecies].ability1, evos->targetSpecies); //Megas can only have 1 ability
		}

		evos = CanMegaEvolve(bank, TRUE);
		if (evos != NULL)
		{
			return TryRandomizeAbility(gBaseStats[evos->targetSpecies].ability2, evos->targetSpecies); //Ultra Necrozma only has 1 ability
		}
	}

	return ABILITY_NONE;
}

const u8* DoMegaEvolution(u8 bank)
{
	struct Pokemon* mon = GetBankPartyData(bank);
	const struct Evolution* evolutions = CanMegaEvolve(bank, FALSE);

	if (evolutions == NULL) //Check Ultra Burst if no Mega
		evolutions = CanMegaEvolve(bank, TRUE);

	if (evolutions != NULL)
	{
		u16 species = mon->box.species;
		DoFormChange(bank, evolutions->targetSpecies, TRUE, TRUE, TRUE);

		gBattleScripting.battler = bank;
		gLastUsedItem = mon->box.heldItem;

		//[BUFFER][00]'s [LAST_ITEM]\nis reacting to [PLAYER_NAME]'s [BUFFER][01]!
		PREPARE_SPECIES_BUFFER(gBattleTextBuff1, species);
		PREPARE_ITEM_BUFFER(gBattleTextBuff2, FindBankKeystone(bank));

		if (evolutions->unknown == MEGA_VARIANT_WISH)
			return BattleScript_MegaWish;
		else if (evolutions->unknown == MEGA_VARIANT_ULTRA_BURST)
			return BattleScript_UltraBurst;
		else
			return BattleScript_MegaEvolution;
	}
	else
		return NULL;
}


const u8* DoPrimalReversion(u8 bank, u8 caseId)
{
	struct Pokemon* mon = GetBankPartyData(bank);
	const struct Evolution* evolutions = gEvolutionTable[mon->box.species];
	u16 item = mon->box.heldItem;

	if (item != ITEM_NONE)
	{
        u8 i;
		for (i = 0; i < EVOS_PER_MON; ++i)
		{
			if (evolutions[i].method == EVO_MEGA && evolutions[i].unknown == MEGA_VARIANT_PRIMAL && evolutions[i].param == item)
			{
				DoFormChange(bank, evolutions[i].targetSpecies, TRUE, TRUE, TRUE);

				switch (caseId) {
					case 0:
						return BattleScript_Primal;
					default:
						return BattleScript_PrimalSub;
				}
			}
		}
	}
	return NULL;
}

//In theory, this function will do nothing as the regular forms revert should
//should take care of the reversion. This is to prevent bugs if the player
//gives themselves a Mega or Primal to start the battle.
void MegaRevert(struct Pokemon* party)
{
	int i;

	for (i = 0; i < PARTY_SIZE; ++i)
		TryRevertMega(&party[i]);
}

void TryRevertMega(struct Pokemon* mon)
{
	const struct Evolution* evolutions = gEvolutionTable[mon->box.species];
    u8 i;
	for (i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_MEGA && evolutions[i].param == 0)
		{
			mon->box.species = evolutions[i].targetSpecies;
			CalculateMonStats(mon);
		}
	}
}

static bool8 IsItemKeystone(u16 item)
{
    u8 i;
	for (i = 0; i < ARRAY_COUNT(sKeystoneTable); ++i)
	{
		if (item == sKeystoneTable[i])
			return TRUE;
	}
	return FALSE;
}

static u16 FindTrainerKeystone(u16 trainerId)
{
    u8 i;
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK) || IsFrontierTrainerId(trainerId))
		return ITEM_MEGA_RING;

	for (i = 0; i < MAX_TRAINER_ITEMS; ++i)
	{
		if (IsItemKeystone(gTrainers[trainerId].items[i]))
			return gTrainers[trainerId].items[i];
	}

	return ITEM_NONE;
}

static u16 FindPlayerKeystone(void)
{
    u8 i;
	for (i = 0; i < ARRAY_COUNT(sKeystoneTable); ++i)
	{
		if (CheckBagHasItem(sKeystoneTable[i], 1))
			return sKeystoneTable[i];
	}

	#ifdef DEBUG_MEGA
		return ITEM_MEGA_RING; //Give player Mega Ring if they have none
	#endif

	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))
		return ITEM_MEGA_RING;

	return ITEM_NONE;
}

static u16 FindBankKeystone(u8 bank)
{
	if (gBattleTypeFlags & BATTLE_TYPE_LINK)
		return ITEM_MEGA_RING; //You can always Mega Evolve in a link battle

	#ifdef DEBUG_MEGA
		if (bank + 1)
			return ITEM_MEGA_RING;
	#endif

	if (SIDE(bank) == B_SIDE_OPPONENT)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
		{
			if (GetBattlerPosition(bank) == B_POSITION_OPPONENT_LEFT)
				return FindTrainerKeystone(gTrainerBattleOpponent_A);
			else
				return FindTrainerKeystone(SECOND_OPPONENT);
		}
		else
			return FindTrainerKeystone(gTrainerBattleOpponent_A);
	}
	else //SIDE_PLAYER
	{
		if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
		{
			if (GetBattlerPosition(bank) == B_POSITION_PLAYER_RIGHT)
				return FindTrainerKeystone(VarGet(VAR_PARTNER));
			else
				return FindPlayerKeystone();
		}
		else
			return FindPlayerKeystone();
	}
}

bool8 MegaEvolutionEnabled(u8 bank)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_LINK))
		return TRUE;

	if (FindBankKeystone(bank) == ITEM_NONE)
	{
		#ifdef DEBUG_MEGA
			return TRUE;
		#else
			return FALSE;
		#endif
	}
	else
		return TRUE;
}

bool8 BankMegaEvolved(u8 bank, bool8 checkUB)
{
	if ((SIDE(bank) == B_SIDE_PLAYER && gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
	||  (SIDE(bank) == B_SIDE_OPPONENT && gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
	{
		if (checkUB)
		{
			if (gBattleStruct->ultraData.done[bank])
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if (gBattleStruct->megaData.done[bank])
				return TRUE;
			else
				return FALSE;
		}
	}

	if (checkUB)
	{
		if (gBattleStruct->ultraData.done[bank] || gBattleStruct->ultraData.done[PARTNER(bank)])
			return TRUE;
	}
	else
	{
		if (gBattleStruct->megaData.done[bank] || (gBattleStruct->megaData.done[PARTNER(bank)]))
			return TRUE;
	}

	return FALSE;
}

bool8 IsMegaSpecies(u16 species)
{
    u8 i;
	const struct Evolution* evolutions = gEvolutionTable[species];

	for (i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_MEGA
		&& (evolutions[i].unknown == MEGA_VARIANT_STANDARD || evolutions[i].unknown == MEGA_VARIANT_WISH)
		&& evolutions[i].param == 0)
			return TRUE;
	}

	return FALSE;
}

//No better way to check for these next two
bool8 IsBluePrimalSpecies(u16 species)
{
	#ifdef SPECIES_KYOGRE_PRIMAL
	return species == SPECIES_KYOGRE_PRIMAL;
	#else
	return FALSE;
	#endif
}

bool8 IsRedPrimalSpecies(u16 species)
{
	#ifdef SPECIES_GROUDON_PRIMAL
	return species == SPECIES_GROUDON_PRIMAL;
	#else
	return FALSE;
	#endif
}

bool8 IsMega(u8 bank)
{
	return(IsMegaSpecies(GetBankPartyData(bank)->box.species));
}

bool8 IsBluePrimal(u8 bank)
{
	return IsBluePrimalSpecies(GetBankPartyData(bank)->box.species);
}

bool8 IsRedPrimal(u8 bank)
{
	return IsRedPrimalSpecies(GetBankPartyData(bank)->box.species);
}

bool8 IsUltraNecrozma(u8 bank)
{
	return IsUltraNecrozmaSpecies(GetBankPartyData(bank)->box.species);
}

bool8 IsUltraNecrozmaSpecies(u16 species)
{
    u8 i;
	const struct Evolution* evolutions = gEvolutionTable[species];

	for (i = 0; i < EVOS_PER_MON; ++i)
	{
		if (evolutions[i].method == EVO_MEGA && evolutions[i].unknown == MEGA_VARIANT_ULTRA_BURST && evolutions[i].param == 0)
			return TRUE;
	}

	return FALSE;
}

bool8 HasMegaSymbol(u8 bank)
{
	return IsMega(bank) || IsBluePrimal(bank) || IsRedPrimal(bank) || IsUltraNecrozma(bank);
}

const u8* GetTrainerName(u8 bank)
{
	u8 battlerNum = 0;
	u16 trainerId = 0xFFFF;
	u8 multiplayerId = GetMultiplayerId();

	u16 linkPartner = GetBattlerMultiplayerId(BATTLE_PARTNER(gLinkPlayers[multiplayerId].id));
	u16 linkOpponent1 = GetBattlerMultiplayerId(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id));
	u16 linkOpponent2 = GetBattlerMultiplayerId(BATTLE_PARTNER(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id)));

	switch (GetBattlerPosition(bank)) {
		case B_POSITION_PLAYER_LEFT:
			if (InBattleSands())
			{
				trainerId = BATTLE_FACILITY_MULTI_TRAINER_TID;
				battlerNum = 2; //Name stored in partner var
			}
			break;

		case B_POSITION_OPPONENT_LEFT:
			if (gBattleTypeFlags & BATTLE_TYPE_LINK)
				trainerId = linkOpponent1;
			else
				trainerId = gTrainerBattleOpponent_A;
			break;

		case B_POSITION_PLAYER_RIGHT:
			battlerNum = 2;
			if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
			{
				trainerId = VarGet(VAR_PARTNER);
			}
			else if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
				trainerId = linkPartner;
			else if (InBattleSands())
			{
				trainerId = BATTLE_FACILITY_MULTI_TRAINER_TID;
				battlerNum = 2; //Name stored in partner var
			}
			else
				battlerNum = 0;
			break;

		case B_POSITION_OPPONENT_RIGHT:
			battlerNum = 1;
			if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
				trainerId = linkOpponent2;
			else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
				trainerId = linkOpponent1;
			else if (!(gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
			{
				trainerId = gTrainerBattleOpponent_A;
				battlerNum = 0;
			}
			else
				trainerId = SECOND_OPPONENT;
			break;
	}

	if (trainerId == 0xFFFF)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_LINK)
			return gLinkPlayers[multiplayerId].name;
		else
			return gSaveBlock2Ptr->playerName;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
	{
		return gLinkPlayers[trainerId].name;
	}
	else
	{
		u8 class = gTrainers[trainerId].trainerClass;
		u8* name = NULL;

		if (trainerId < RAID_BATTLE_MULTI_TRAINER_TID)
			name = TryGetRivalNameByTrainerClass(class);

		if (name == NULL)
		{
			if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER
			|| IsFrontierTrainerId(trainerId))
				return GetFrontierTrainerName(trainerId, battlerNum);

			return gTrainers[trainerId].trainerName;
		}

		return name;
	}
}

u8* TryGetRivalNameByTrainerClass(u8 class)
{
	#ifdef OVERWRITE_RIVAL
		if (class == TRAINER_CLASS_RIVAL_EARLY || class == TRAINER_CLASS_RIVAL_LATE || class == TRAINER_CLASS_CHAMPION)
			return GetExpandedPlaceholder(PLACEHOLDER_ID_RIVAL);
		else
	#endif
			return NULL;
}

void MegaRetrieveData(void)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_OLD_MAN_TUTORIAL)) //Poke Dude's controller wasn't updated so these need to be manually set to 0
	{
		gBattleStruct->megaData.chosen[gActiveBattler] = 0;
		gBattleStruct->ultraData.chosen[gActiveBattler] = 0;
		gBattleStruct->zMoveData.toBeUsed[gActiveBattler] = 0;
		gBattleStruct->dynamaxData.toBeUsed[gActiveBattler] = 0;
	}
	else
	{
		gBattleStruct->megaData.chosen[gActiveBattler] |= gBattleBufferB[gActiveBattler][4];
		gBattleStruct->ultraData.chosen[gActiveBattler] |= gBattleBufferB[gActiveBattler][5];
		gBattleStruct->zMoveData.toBeUsed[gActiveBattler] |= gBattleBufferB[gActiveBattler][6];
		gBattleStruct->dynamaxData.toBeUsed[gActiveBattler] |= gBattleBufferB[gActiveBattler][7];
	}
}
