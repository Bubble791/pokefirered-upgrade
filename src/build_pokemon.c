#include "global.h"
#include "battle.h"
#include "util.h"
#include "item.h"
#include "random.h"
#include "pokemon.h"
#include "menu.h"
#include "malloc.h"
#include "window.h"
#include "sound.h"
#include "wild_encounter.h"
#include "event_data.h"
#include "frontier.h"
#include "constants/abilities.h"
#include "constants/hold_effects.h"
#include "constants/battle_move_effects.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/species.h"

EWRAM_DATA u16 gLastFishingSpecies = 0;
EWRAM_DATA u8 gFishingStreak = 0;

u8 TryRandomizeAbility(ability_t ability, u16 species)
{
	u32 newAbility = ability;

	#ifdef FLAG_ABILITY_RANDOMIZER
	if (FlagGet(FLAG_ABILITY_RANDOMIZER) && !FlagGet(FLAG_BATTLE_FACILITY))
	{
		u32 id = MathMax(1, T1_READ_32(gSaveBlock2->playerTrainerId)); //0 id would mean Pokemon wouldn't have ability

		do
		{
			newAbility = newAbility * id * species;
			newAbility = MathMax(1, newAbility % ABILITIES_COUNT);
		}
		while (CheckTableForAbility(newAbility, gRandomizerAbilityBanList));
	}
	#endif

	return newAbility;
}

u32 CheckShinyMon(struct Pokemon* mon)
{
	u16 chance = 0;
	u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);

	#ifdef FLAG_SHINY_CREATION
	if (FlagGet(FLAG_SHINY_CREATION))
	{
		chance = 4097;
	}
	else
	#endif
	{
		#ifdef ITEM_SHINY_CHARM
		if (CheckBagHasItem(ITEM_SHINY_CHARM, 1) > 0)
			chance = 3; //Tries an extra two times
		#endif

		if (gFishingByte) //Currently fishing
			chance += (1 + 2 * MathMin(gFishingStreak, 20)); //Tries an extra 2 times per Pokemon in the streak up to 41 times
	}

	if (RandRange(0, 4097) < chance)		//Nominal 1/4096
	{
		//Force shiny
		u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
		u16 sid = HIHALF(otId);
		u16 tid = LOHALF(otId);

		u8 shinyRange = RandRange(0,8);
		u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
		ability_t ability = personality & 1;
		u8 nature = GetNatureFromPersonality(personality);
		u8 gender = GetGenderFromSpeciesAndPersonality(species, personality);
		u8 letter = GetUnownLetterByPersonalityLoByte(personality);
		bool8 abilityMatters = !mon->box.hiddenAbility;

		do
		{
			personality = Random32();
			personality = (((shinyRange ^ (sid ^ tid)) ^ LOHALF(personality)) << 16) | LOHALF(personality);

			if (abilityMatters)
			{
				personality &= ~(1);
				personality |= ability; //Either 0 or 1
			}
		} while (GetNatureFromPersonality(personality) != nature || GetGenderFromSpeciesAndPersonality(species, personality) != gender
		#ifdef SPECIES_UNOWN
		|| (species == SPECIES_UNOWN && GetUnownLetterByPersonalityLoByte(personality) != letter)
		#endif
		); //Keep all other values the same
	}

	return personality;
};

u8 GetOpenWorldBadgeCount(void)
{
	u8 badgeCount = 0;

	if (FlagGet(FLAG_SYS_GAME_CLEAR)) //0x82C
		return 9;

	if (FlagGet(FLAG_BADGE08_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE07_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE06_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE05_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE04_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE03_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE02_GET))
		++badgeCount;
	if (FlagGet(FLAG_BADGE01_GET))
		++badgeCount;

	return badgeCount;
}