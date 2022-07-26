#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "battle_string_ids.h"
#include "battle_controllers.h"
#include "util.h"
#include "item.h"
#include "random.h"
#include "pokemon.h"
#include "build_pokemon.h"
#include "battle_ai_script_commands.h"
#include "menu.h"
#include "malloc.h"
#include "window.h"
#include "dynamax.h"
#include "sound.h"
#include "pokedex.h"
#include "constants/abilities.h"
#include "constants/hold_effects.h"
#include "constants/battle_move_effects.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/species.h"

static u16 GetBasePower(struct DamageCalc* data);
static u16 AdjustBasePower(struct DamageCalc* data, u16 power);
static u16 GetZMovePower(u16 zMove);
static u16 GetMaxMovePower(void);
static u32 AdjustWeight(u32 weight, ability_t ability, u8 item_effect, u8 bank, bool8 check_nimble);

#define FLAG_IGNORE_TARGET 0x1
#define FLAG_CONFUSION_DAMAGE 0x2
#define FLAG_CHECKING_FROM_MENU 0x4
#define FLAG_AI_CALC 0x8

struct NaturalGiftStruct
{
	u16 berry;
	u8 type;
	u8 power;
};

#define ALTERNATE_SIZE(species, height, weight) {species, height * 10, weight * 10}

struct AlternateSize
{
	u16 species;
	u16 height;
	u16 weight;
};

const struct AlternateSize gAlternateSpeciesSizeTable[] =
{
	ALTERNATE_SIZE(SPECIES_VENUSAUR_MEGA, 2.4, 155.5),
	ALTERNATE_SIZE(SPECIES_CHARIZARD_MEGA_Y, 1.7, 100.5),
	ALTERNATE_SIZE(SPECIES_CHARIZARD_MEGA_X, 1.7, 110.5),
	ALTERNATE_SIZE(SPECIES_BLASTOISE_MEGA, 1.6, 101.1),
	ALTERNATE_SIZE(SPECIES_BEEDRILL_MEGA, 1.4, 40.5),
	ALTERNATE_SIZE(SPECIES_PIDGEOT, 2.2, 50.5),
	ALTERNATE_SIZE(SPECIES_RATTATA_A, 0.3, 3.8),
	ALTERNATE_SIZE(SPECIES_RATICATE_A, 0.7, 25.5),
	ALTERNATE_SIZE(SPECIES_RAICHU_A, 0.7, 21.0),
	ALTERNATE_SIZE(SPECIES_SANDSHREW_A, 0.7, 40.0),
	ALTERNATE_SIZE(SPECIES_SANDSLASH_A, 1.2, 55.0),
	ALTERNATE_SIZE(SPECIES_DIGLETT_A, 0.2, 1.0),
	ALTERNATE_SIZE(SPECIES_DUGTRIO_A, 0.7, 66.6),
	ALTERNATE_SIZE(SPECIES_PERSIAN_A, 1.1, 33.0),
	ALTERNATE_SIZE(SPECIES_ALAKAZAM_MEGA, 1.2, 105.8),
	ALTERNATE_SIZE(SPECIES_GEODUDE_A, 0.4, 20.3),
	ALTERNATE_SIZE(SPECIES_GRAVELER_A, 1.0, 110.0),
	ALTERNATE_SIZE(SPECIES_GOLEM_A, 1.7, 316.0),
	ALTERNATE_SIZE(SPECIES_SLOWBRO_MEGA, 2.0, 120.0),
	ALTERNATE_SIZE(SPECIES_GRIMER_A, 0.7, 42.0),
	ALTERNATE_SIZE(SPECIES_MUK_A, 1.0, 52.0),
	ALTERNATE_SIZE(SPECIES_GENGAR_MEGA, 1.4, 40.5),
	ALTERNATE_SIZE(SPECIES_EXEGGUTOR_A, 10.9, 415.6),
	ALTERNATE_SIZE(SPECIES_MAROWAK_A, 1.0, 34.0),
	ALTERNATE_SIZE(SPECIES_KANGASKHAN_MEGA, 2.2, 100.0),
	ALTERNATE_SIZE(SPECIES_PINSIR_MEGA, 1.7, 59.0),
	ALTERNATE_SIZE(SPECIES_GYARADOS_MEGA, 6.5, 305.0),
	ALTERNATE_SIZE(SPECIES_AERODACTYL_MEGA, 2.1, 79.0),
	ALTERNATE_SIZE(SPECIES_MEWTWO_MEGA_Y, 1.5, 33.0),
	ALTERNATE_SIZE(SPECIES_MEWTWO_MEGA_X, 2.3, 127.0),
	ALTERNATE_SIZE(SPECIES_STEELIX_MEGA, 10.5, 740.0),
	ALTERNATE_SIZE(SPECIES_SCIZOR_MEGA, 2.0, 125.0),
	ALTERNATE_SIZE(SPECIES_HERACROSS_MEGA, 1.7, 62.5),
	ALTERNATE_SIZE(SPECIES_HOUNDOOM_MEGA, 1.9, 49.5),
	ALTERNATE_SIZE(SPECIES_TYRANITAR_MEGA, 2.5, 255.0),
	ALTERNATE_SIZE(SPECIES_SCEPTILE_MEGA, 1.9, 55.2),
	ALTERNATE_SIZE(SPECIES_SWAMPERT_MEGA, 1.9, 102.0),
	ALTERNATE_SIZE(SPECIES_SABLEYE_MEGA, 0.5, 161.0),
	ALTERNATE_SIZE(SPECIES_MAWILE_MEGA, 1.0, 23.5),
	ALTERNATE_SIZE(SPECIES_AGGRON_MEGA, 2.2, 395.0),
	ALTERNATE_SIZE(SPECIES_MANECTRIC_MEGA, 1.8, 44.0),
	ALTERNATE_SIZE(SPECIES_SHARPEDO_MEGA, 2.5, 130.3),
	ALTERNATE_SIZE(SPECIES_CAMERUPT_MEGA, 2.5, 320.5),
	ALTERNATE_SIZE(SPECIES_ALTARIA_MEGA, 1.5, 20.6),
	ALTERNATE_SIZE(SPECIES_BANETTE_MEGA, 1.2, 13.0),
	ALTERNATE_SIZE(SPECIES_ABSOL_MEGA, 1.2, 49.0),
	ALTERNATE_SIZE(SPECIES_GLALIE_MEGA, 2.1, 350.2),
	ALTERNATE_SIZE(SPECIES_SALAMENCE_MEGA, 1.8, 112.6),
	ALTERNATE_SIZE(SPECIES_METAGROSS_MEGA, 2.5, 942.9),
	ALTERNATE_SIZE(SPECIES_LATIAS_MEGA, 1.8, 52.0),
	ALTERNATE_SIZE(SPECIES_LATIOS_MEGA, 2.3, 70.0),
	ALTERNATE_SIZE(SPECIES_KYOGRE_PRIMAL, 9.8, 430.0),
	ALTERNATE_SIZE(SPECIES_GROUDON_PRIMAL, 5.0, 999.7),
	ALTERNATE_SIZE(SPECIES_RAYQUAZA_MEGA, 10.8, 392.0),
	ALTERNATE_SIZE(SPECIES_LOPUNNY_MEGA, 1.3, 28.3),
	ALTERNATE_SIZE(SPECIES_LUCARIO_MEGA, 1.3, 57.5),
	ALTERNATE_SIZE(SPECIES_ABOMASNOW_MEGA, 2.7, 185.0),
	ALTERNATE_SIZE(SPECIES_GALLADE_MEGA, 1.6, 56.4),
	ALTERNATE_SIZE(SPECIES_GIRATINA_ORIGIN, 6.9, 650.0),
	ALTERNATE_SIZE(SPECIES_SHAYMIN_SKY, 0.4, 5.2),
	ALTERNATE_SIZE(SPECIES_AUDINO_MEGA, 1.5, 32.0),
	ALTERNATE_SIZE(SPECIES_TORNADUS_THERIAN, 1.4, 63.0),
	ALTERNATE_SIZE(SPECIES_THUNDURUS_THERIAN, 3.0, 61.0),
	ALTERNATE_SIZE(SPECIES_LANDORUS_THERIAN, 1.3, 68.0),
	ALTERNATE_SIZE(SPECIES_KYUREM_BLACK, 3.3, 325.0),
	ALTERNATE_SIZE(SPECIES_KYUREM_WHITE, 3.6, 325.0),
	ALTERNATE_SIZE(SPECIES_PUMPKABOO_M, 0.4, 5.0),
	ALTERNATE_SIZE(SPECIES_PUMPKABOO_L, 0.5, 7.5),
	ALTERNATE_SIZE(SPECIES_PUMPKABOO_XL, 0.8, 15.0),
	ALTERNATE_SIZE(SPECIES_GOURGEIST_M, 0.9, 12.5),
	ALTERNATE_SIZE(SPECIES_GOURGEIST_L, 1.1, 14.0),
	ALTERNATE_SIZE(SPECIES_GOURGEIST_XL, 1.7, 39.0),
	ALTERNATE_SIZE(SPECIES_ZYGARDE_10, 1.2, 33.5),
	ALTERNATE_SIZE(SPECIES_ZYGARDE_COMPLETE, 4.5, 610.0),
	ALTERNATE_SIZE(SPECIES_DIANCIE_MEGA, 1.1, 27.8),
	ALTERNATE_SIZE(SPECIES_HOOPA_UNBOUND, 6.5, 490.0),
	ALTERNATE_SIZE(SPECIES_LYCANROC_N, 1.1, 25.0),
	ALTERNATE_SIZE(SPECIES_WISHIWASHI_S, 8.2, 78.6),
	ALTERNATE_SIZE(SPECIES_MINIOR_RED, 0.3, 0.3),
	ALTERNATE_SIZE(SPECIES_MINIOR_BLUE, 0.3, 0.3),
	ALTERNATE_SIZE(SPECIES_MINIOR_ORANGE, 0.3, 0.3),
	ALTERNATE_SIZE(SPECIES_MINIOR_YELLOW, 0.3, 0.3),
	ALTERNATE_SIZE(SPECIES_MINIOR_INDIGO, 0.3, 0.3),
	ALTERNATE_SIZE(SPECIES_MINIOR_GREEN, 0.3, 0.3),
	ALTERNATE_SIZE(SPECIES_MINIOR_VIOLET, 0.3, 0.3),
	ALTERNATE_SIZE(SPECIES_NECROZMA_DAWN_WINGS, 4.2, 350.0),
	ALTERNATE_SIZE(SPECIES_NECROZMA_DUSK_MANE, 3.8, 460.0),
	ALTERNATE_SIZE(SPECIES_NECROZMA_ULTRA, 7.5, 230.0),
	ALTERNATE_SIZE(SPECIES_TABLES_TERMIN, 0, 0),
};

static const struct NaturalGiftStruct gNaturalGiftTable[] =
{
    {
        .berry = ITEM_CHERI_BERRY,
        .type = TYPE_FIRE,
        .power = 80,
    },
    {
        .berry = ITEM_CHESTO_BERRY,
        .type = TYPE_WATER,
        .power = 80,
    },
    {
        .berry = ITEM_PECHA_BERRY,
        .type = TYPE_ELECTRIC,
        .power = 80,
    },
    {
        .berry = ITEM_RAWST_BERRY,
        .type = TYPE_GRASS,
        .power = 80,
    },
    {
        .berry = ITEM_ASPEAR_BERRY,
        .type = TYPE_ICE,
        .power = 80,
    },
    {
        .berry = ITEM_LEPPA_BERRY,
        .type = TYPE_FIGHTING,
        .power = 80,
    },
    {
        .berry = ITEM_ORAN_BERRY,
        .type = TYPE_POISON,
        .power = 80,
    },
    {
        .berry = ITEM_PERSIM_BERRY,
        .type = TYPE_GROUND,
        .power = 80,
    },
    {
        .berry = ITEM_LUM_BERRY,
        .type = TYPE_FLYING,
        .power = 80,
    },
    {
        .berry = ITEM_SITRUS_BERRY,
        .type = TYPE_PSYCHIC,
        .power = 80,
    },
    {
        .berry = ITEM_FIGY_BERRY,
        .type = TYPE_BUG,
        .power = 80,
    },
    {
        .berry = ITEM_WIKI_BERRY,
        .type = TYPE_ROCK,
        .power = 80,
    },
    {
        .berry = ITEM_MAGO_BERRY,
        .type = TYPE_GHOST,
        .power = 80,
    },
    {
        .berry = ITEM_AGUAV_BERRY,
        .type = TYPE_DRAGON,
        .power = 80,
    },
    {
        .berry = ITEM_IAPAPA_BERRY,
        .type = TYPE_DARK,
        .power = 80,
    },
    {
        .berry = ITEM_RAZZ_BERRY,
        .type = TYPE_STEEL,
        .power = 80,
    },
    {
        .berry = ITEM_BLUK_BERRY,
        .type = TYPE_FIRE,
        .power = 90,
    },
    {
        .berry = ITEM_NANAB_BERRY,
        .type = TYPE_WATER,
        .power = 90,
    },
    {
        .berry = ITEM_WEPEAR_BERRY,
        .type = TYPE_ELECTRIC,
        .power = 90,
    },
    {
        .berry = ITEM_PINAP_BERRY,
        .type = TYPE_GRASS,
        .power = 90,
    },
    {
        .berry = ITEM_POMEG_BERRY,
        .type = TYPE_ICE,
        .power = 90,
    },
    {
        .berry = ITEM_KELPSY_BERRY,
        .type = TYPE_FIGHTING,
        .power = 90,
    },
    {
        .berry = ITEM_QUALOT_BERRY,
        .type = TYPE_POISON,
        .power = 90,
    },
    {
        .berry = ITEM_HONDEW_BERRY,
        .type = TYPE_GROUND,
        .power = 90,
    },
    {
        .berry = ITEM_GREPA_BERRY,
        .type = TYPE_FLYING,
        .power = 90,
    },
    {
        .berry = ITEM_TAMATO_BERRY,
        .type = TYPE_PSYCHIC,
        .power = 90,
    },
    {
        .berry = ITEM_CORNN_BERRY,
        .type = TYPE_BUG,
        .power = 90,
    },
    {
        .berry = ITEM_MAGOST_BERRY,
        .type = TYPE_ROCK,
        .power = 90,
    },
    {
        .berry = ITEM_RABUTA_BERRY,
        .type = TYPE_GHOST,
        .power = 90,
    },
    {
        .berry = ITEM_NOMEL_BERRY,
        .type = TYPE_DRAGON,
        .power = 90,
    },
    {
        .berry = ITEM_SPELON_BERRY,
        .type = TYPE_DARK,
        .power = 90,
    },
    {
        .berry = ITEM_PAMTRE_BERRY,
        .type = TYPE_STEEL,
        .power = 90,
    },
    {
        .berry = ITEM_WATMEL_BERRY,
        .type = TYPE_FIRE,
        .power = 100,
    },
    {
        .berry = ITEM_DURIN_BERRY,
        .type = TYPE_WATER,
        .power = 100,
    },
    {
        .berry = ITEM_BELUE_BERRY,
        .type = TYPE_ELECTRIC,
        .power = 100,
    },
    {
        .berry = ITEM_OCCA_BERRY,
        .type = TYPE_FIRE,
        .power = 80,
    },
    {
        .berry = ITEM_PASSHO_BERRY,
        .type = TYPE_WATER,
        .power = 80,
    },
    {
        .berry = ITEM_WACAN_BERRY,
        .type = TYPE_ELECTRIC,
        .power = 80,
    },
    {
        .berry = ITEM_RINDO_BERRY,
        .type = TYPE_GRASS,
        .power = 80,
    },
    {
        .berry = ITEM_YACHE_BERRY,
        .type = TYPE_ICE,
        .power = 80,
    },
    {
        .berry = ITEM_CHOPLE_BERRY,
        .type = TYPE_FIGHTING,
        .power = 80,
    },
    {
        .berry = ITEM_KEBIA_BERRY,
        .type = TYPE_POISON,
        .power = 80,
    },
    {
        .berry = ITEM_SHUCA_BERRY,
        .type = TYPE_GROUND,
        .power = 80,
    },
    {
        .berry = ITEM_COBA_BERRY,
        .type = TYPE_FLYING,
        .power = 80,
    },
    {
        .berry = ITEM_PAYAPA_BERRY,
        .type = TYPE_PSYCHIC,
        .power = 80,
    },
    {
        .berry = ITEM_TANGA_BERRY,
        .type = TYPE_BUG,
        .power = 80,
    },
    {
        .berry = ITEM_CHARTI_BERRY,
        .type = TYPE_ROCK,
        .power = 80,
    },
    {
        .berry = ITEM_KASIB_BERRY,
        .type = TYPE_GHOST,
        .power = 80,
    },
    {
        .berry = ITEM_HABAN_BERRY,
        .type = TYPE_DRAGON,
        .power = 80,
    },
    {
        .berry = ITEM_COLBUR_BERRY,
        .type = TYPE_DARK,
        .power = 80,
    },
    {
        .berry = ITEM_BABIRI_BERRY,
        .type = TYPE_STEEL,
        .power = 80,
    },
    {
        .berry = ITEM_CHILAN_BERRY,
        .type = TYPE_NORMAL,
        .power = 80,
    },
    {
        .berry = ITEM_LIECHI_BERRY,
        .type = TYPE_GRASS,
        .power = 100,
    },
    {
        .berry = ITEM_GANLON_BERRY,
        .type = TYPE_ICE,
        .power = 100,
    },
    {
        .berry = ITEM_SALAC_BERRY,
        .type = TYPE_FIGHTING,
        .power = 100,
    },
    {
        .berry = ITEM_PETAYA_BERRY,
        .type = TYPE_POISON,
        .power = 100,
    },
    {
        .berry = ITEM_APICOT_BERRY,
        .type = TYPE_GROUND,
        .power = 100,
    },
    {
        .berry = ITEM_LANSAT_BERRY,
        .type = TYPE_FLYING,
        .power = 100,
    },
    {
        .berry = ITEM_STARF_BERRY,
        .type = TYPE_PSYCHIC,
        .power = 100,
    },
    {
        .berry = ITEM_ENIGMA_BERRY,
        .type = TYPE_BUG,
        .power = 100,
    },
    {
        .berry = ITEM_ENIGMA_BERRY,
        .type = TYPE_BUG,
        .power = 100,
    },
    {
        .berry = ITEM_MICLE_BERRY,
        .type = TYPE_ROCK,
        .power = 100,
    },
    {
        .berry = ITEM_CUSTAP_BERRY,
        .type = TYPE_GHOST,
        .power = 100,
    },
    {
        .berry = ITEM_JABOCA_BERRY,
        .type = TYPE_DRAGON,
        .power = 100,
    },
    {
        .berry = ITEM_ROWAP_BERRY,
        .type = TYPE_DARK,
        .power = 100,
    },
    {
        .berry = ITEM_ROSELI_BERRY,
        .type = TYPE_FAIRY,
        .power = 100,
    },
    {
        .berry = ITEM_KEE_BERRY,
        .type = TYPE_FAIRY,
        .power = 100,
    },
    {
        .berry = ITEM_MARANGA_BERRY,
        .type = TYPE_DARK,
        .power = 100,
    },
    {
        .berry = ITEM_TABLES_TERMIN,
    },
};

static void TypeDamageModificationByDefTypes(ability_t atkAbility, u8 bankDef, u16 move, u8 moveType, u8* flags, u8 defType1, u8 defType2, u8 defType3);

void FutureSightDamageCalc(void)
{
	struct DamageCalc data = {0};
	data.bankAtk = gBattlerAttacker;
	data.bankDef = gBattlerTarget;
	data.move = gCurrentMove;
	gBattleMoveDamage = (CalculateBaseDamage(&data) * gCritMultiplier) / BASE_CRIT_MULTIPLIER;
	gBattleStruct->DamageTaken[gBattlerTarget] = gBattleMoveDamage;
}

s32 ConfusionDamageCalc(void)
{
	struct DamageCalc data = {0};
	data.bankAtk = data.bankDef = gBattlerAttacker;
	data.move = MOVE_POUND;
	data.specialFlags |= FLAG_CONFUSION_DAMAGE;
	gBattleMoveDamage = CalculateBaseDamage(&data);
	return gBattleMoveDamage;
}

u8 CalcPossibleCritChance(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* monAtk, struct Pokemon* monDef)
{
	ability_t atkAbility;
	ability_t defAbility;
	u16 atkSpecies;
	u32 atkStatus2;
	u32 defStatus1;

	u8 atkEffect = 0;
	u16 critChance = 0;

	if (monAtk != NULL)
	{
			atkAbility = GetMonAbility(monAtk);
			atkSpecies = monAtk->box.species;
			atkEffect = GetMonItemEffect(monAtk);
			atkStatus2 = 0;
	}
	else
	{
		atkAbility = ABILITY(bankAtk);
		atkEffect = ITEM_EFFECT(bankAtk);
		atkSpecies = gBattleMons[bankAtk].species;
		atkStatus2 = gBattleMons[bankAtk].status2;
	}

	if (monDef != NULL)
	{
		defAbility = GetMonAbility(monDef);
		defStatus1 = monDef->status;
	}
	else
	{
		defAbility = ABILITY(bankDef);
		defStatus1 = gBattleMons[bankDef].status1;
	}

	if (defAbility == ABILITY_BATTLE_ARMOR
	||  defAbility == ABILITY_SHELL_ARMOR
	||  CantScoreACrit(bankAtk, monAtk)
	||  gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_FIRST_BATTLE)
	||  gBattleStruct->LuckyChantTimers[SIDE(bankDef)])
		return FALSE;

	else if ((atkAbility == ABILITY_MERCILESS && (defStatus1 & STATUS1_PSN_ANY))
	|| (IsLaserFocused(bankAtk) && monAtk == NULL)
	|| CheckTableForMove(move, gAlwaysCriticalMoves))
		return TRUE;

	else {
		critChance  = 2 * ((atkStatus2 & STATUS2_FOCUS_ENERGY) != 0)
					+ gBattleStruct->chiStrikeCritBoosts[bankAtk]
					+ (CheckTableForMove(move, gHighCriticalChanceMoves))
					+ (atkEffect == HOLD_EFFECT_SCOPE_LENS)
					+ (atkAbility == ABILITY_SUPERLUCK)
					#ifdef SPECIES_CHANSEY
					+ 2 * (atkEffect == HOLD_EFFECT_LUCKY_PUNCH && atkSpecies == SPECIES_CHANSEY)
					#endif
					#ifdef SPECIES_FARFETCHD
					+ 2 * (atkEffect == HOLD_EFFECT_STICK && atkSpecies == SPECIES_FARFETCHD)
					#endif
					+ 2 * (move == MOVE_10000000_VOLT_THUNDERBOLT);

		#ifdef CRIT_CHANCE_GEN_6
			if (critChance >= 3)
				return TRUE;
			if (critChance >= 2)
				return 2; //50 % Chance
		#elif (defined CRIT_CHANCE_GEN_2_TO_5)
			if (critChance >= 4)
				return 2; //50 % Chance
		#else
			if (critChance >= 3)
				return TRUE;
			if (critChance >= 2)
				return 2; //50 % Chance
		#endif
	}
	return FALSE;
}

u32 AI_CalcPartyDmg(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* monAtk, struct DamageCalc* damageData)
{
	u8 resultFlags = NewTypeCalc(move, bankAtk, bankDef, monAtk, TRUE);
	u32 damage = 0;
	struct DamageCalc data = {0};

	if (gBattleMoves[move].effect != EFFECT_PAIN_SPLIT
	&& (SPLIT(move) == SPLIT_STATUS || resultFlags & MOVE_RESULT_NO_EFFECT))
		return 0;

	switch (gBattleMoves[move].effect) {
		case EFFECT_SUPER_FANG:
			return GetBaseCurrentHP(bankDef) / 2; //50 % of base HP
		case EFFECT_DRAGON_RAGE:
			return 40;
		case EFFECT_SONICBOOM:
			return 20;
		case EFFECT_LEVEL_DAMAGE:
			return monAtk->level;
		case EFFECT_PSYWAVE:
			return GetPsywaveDamage(50); //On average, 50 will be selected as the random number
		case EFFECT_MEMENTO: //Final Gambit
			return monAtk->hp;
		case EFFECT_ENDEAVOR:
			if (gBattleMons[bankDef].hp <= monAtk->hp)
				return 0;
			return gBattleMons[bankDef].hp - monAtk->hp;
		case EFFECT_PAIN_SPLIT:
		{
			u16 finalHp = MathMax(1, (monAtk->hp + gBattleMons[bankDef].hp) / 2);

			if (finalHp >= gBattleMons[bankDef].hp)
				return 0;
			return gBattleMons[bankDef].hp - finalHp;
		}
	}

	if (SPLIT(move) == SPLIT_STATUS) //At this point we don't care about Status moves anymore
		return 0;

	gBattleScripting.dmgMultiplier = 1;

	gCritMultiplier = CalcPossibleCritChance(bankAtk, bankDef, move, monAtk, NULL); //Return 0 if none, 1 if always, 2 if 50%
	if (gCritMultiplier != 0 && Random() % gCritMultiplier == 0)
		gCritMultiplier = CRIT_MULTIPLIER;
	else
		gCritMultiplier = BASE_CRIT_MULTIPLIER;

	if (damageData == NULL)
		damageData = &data;

	damageData->bankAtk = bankAtk;
	damageData->bankDef = bankDef;
	damageData->move = move;
	damageData->monAtk = monAtk;
	damageData->specialFlags |= FLAG_AI_CALC;
	damage = CalculateBaseDamage(damageData);

	gBattleMoveDamage = damage;
	NewTypeCalc(move, bankAtk, bankDef, monAtk, TRUE);
	damage = (gBattleMoveDamage * gCritMultiplier) / BASE_CRIT_MULTIPLIER;
	gCritMultiplier = BASE_CRIT_MULTIPLIER; //Reset

	damage = (damage * 96) / 100; //Roll 96% damage with party mons - be more idealistic

	if (CheckTableForMove(move, gTwoToFiveStrikesMoves) && GetMonAbility(monAtk) == ABILITY_SKILLLINK)
	{
		damage *= 5;
		return damage;
	}
	else if (CheckTableForMove(move, gTwoToFiveStrikesMoves) || gBattleMoves[move].effect == EFFECT_TRIPLE_KICK) //Three hits on average
	{
		damage *= 3;
		return damage;
	}
	else if (CheckTableForMove(move, gTwoStrikesMoves))
	{
		damage *= 2;
		return damage;
	}
	else if (GetMonAbility(monAtk) == ABILITY_PARENTALBOND && IsMoveAffectedByParentalBond(move, bankAtk))
	{
		#ifdef OLD_PARENTAL_BOND_DAMAGE
			damage = (damage * 150) / 100; //1.5x overall boost
		#else
			damage = (damage * 125) / 100; //1.25x overall boost
		#endif
		return damage;
	}

	//Multi hit moves skip these checks
	if (gBattleMoves[move].effect == EFFECT_FALSE_SWIPE
	|| (BATTLER_MAX_HP(bankDef) && ABILITY(bankDef) == ABILITY_STURDY && NO_MOLD_BREAKERS(GetMonAbility(monAtk), move))
	|| (BATTLER_MAX_HP(bankDef) && IsBankHoldingFocusSash(bankDef)))
		damage = MathMin(damage, gBattleMons[bankDef].hp - 1);

	return damage;
}

u32 AI_CalcMonDefDmg(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* monDef, struct DamageCalc* damageData)
{
	u8 resultFlags = NewAI_TypeCalc(move, bankAtk, monDef);
	u32 damage = 0;
	struct DamageCalc data = {0};

	if (gBattleMoves[move].effect != EFFECT_PAIN_SPLIT
	&& (SPLIT(move) == SPLIT_STATUS || resultFlags & MOVE_RESULT_NO_EFFECT))
		return 0;

	switch (gBattleMoves[move].effect) {
		case EFFECT_SUPER_FANG:
			return monDef->hp / 2; //50 % of base HP
		case EFFECT_DRAGON_RAGE:
			return 40;
		case EFFECT_SONICBOOM:
			return 20;
		case EFFECT_LEVEL_DAMAGE:
			return gBattleMons[bankAtk].level;
		case EFFECT_PSYWAVE:
			return GetPsywaveDamage(50); //On average, 50 will be selected as the random number
		case EFFECT_MEMENTO: //Final Gambit
			return gBattleMons[bankAtk].hp;
		case EFFECT_ENDEAVOR:
			if (monDef->hp <= gBattleMons[bankAtk].hp)
				return 0;
			return monDef->hp - gBattleMons[bankAtk].hp;
		case EFFECT_PAIN_SPLIT:
		{
			u16 finalHp = MathMax(1, (gBattleMons[bankAtk].hp + monDef->hp) / 2);

			if (finalHp >= monDef->hp)
				return 0;
			return monDef->hp - finalHp;
		}
	}

	gBattleScripting.dmgMultiplier = 1;

	gCritMultiplier = CalcPossibleCritChance(bankAtk, bankDef, move, NULL, monDef); //Return 0 if none, 1 if always, 2 if 50%
	if (gCritMultiplier != 0 && Random() % gCritMultiplier == 0)
		gCritMultiplier = CRIT_MULTIPLIER;
	else
		gCritMultiplier = BASE_CRIT_MULTIPLIER;

	if (damageData == NULL)
		damageData = &data;

	damageData->bankAtk = bankAtk;
	damageData->bankDef = bankDef;
	damageData->move = move;
	damageData->monDef = monDef;
	damageData->specialFlags |= FLAG_AI_CALC;
	damage = CalculateBaseDamage(damageData);

	gBattleMoveDamage = damage;
	NewAI_TypeCalc(move, bankAtk, monDef);
	damage = (gBattleMoveDamage * gCritMultiplier) / BASE_CRIT_MULTIPLIER;
	gCritMultiplier = BASE_CRIT_MULTIPLIER; //Reset

	damage = (damage * 96) / 100; //Roll 96% damage with party mons - be more idealistic

	if (CheckTableForMove(move, gTwoToFiveStrikesMoves) && ABILITY(bankAtk) == ABILITY_SKILLLINK)
	{
		damage *= 5;
		return damage;
	}
	else if (CheckTableForMove(move, gTwoToFiveStrikesMoves) || gBattleMoves[move].effect == EFFECT_TRIPLE_KICK) //Three hits on average
	{
		damage *= 3;
		return damage;
	}
	else if (CheckTableForMove(move, gTwoStrikesMoves))
	{
		damage *= 2;
		return damage;
	}
	else if (ABILITY(bankAtk) == ABILITY_PARENTALBOND && IsMoveAffectedByParentalBond(move, bankAtk))
	{
		#ifdef OLD_PARENTAL_BOND_DAMAGE
			damage = (damage * 150) / 100; //1.5x overall boost
		#else
			damage = (damage * 125) / 100; //1.25x overall boost
		#endif
		return damage;
	}

	//Multi hit moves skip these checks
	if (gBattleMoves[move].effect == EFFECT_FALSE_SWIPE
	|| (monDef->hp == monDef->maxHP && GetMonAbility(monDef) == ABILITY_STURDY && NO_MOLD_BREAKERS(ABILITY(bankAtk), move))
	|| (monDef->hp == monDef->maxHP && IsBankHoldingFocusSash(bankDef)))
		damage = MathMin(damage, monDef->hp - 1);

	return damage;
}



static void ModulateDmgByType(u8 multiplier, const u16 move, const u8 moveType, const u8 defType, const u8 bankDef, ability_t atkAbility, u8* flags, struct Pokemon* monDef, bool8 checkMonDef)
{
	if (IsInverseBattle())
	{
		switch (multiplier) {
			case TYPE_MUL_NO_EFFECT:
				multiplier = TYPE_MUL_SUPER_EFFECTIVE;
				break;

			case TYPE_MUL_NOT_EFFECTIVE:
				multiplier = TYPE_MUL_SUPER_EFFECTIVE;
				break;

			case TYPE_MUL_SUPER_EFFECTIVE:
				multiplier = TYPE_MUL_NOT_EFFECTIVE;
				break;
		}
	}

	if (!checkMonDef && multiplier == TYPE_MUL_NO_EFFECT)
	{
		if ((defType == TYPE_GHOST && (moveType == TYPE_NORMAL || moveType == TYPE_FIGHTING))
		&& (gBattleMons[bankDef].status2 & STATUS2_FORESIGHT || atkAbility == ABILITY_SCRAPPY))
			return; //Foresight breaks ghost immunity

		if (moveType == TYPE_PSYCHIC && defType == TYPE_DARK && (gStatuses3[bankDef] & STATUS3_MIRACLE_EYED))
			return; //Miracle Eye causes normal damage hits
	}
	else if (checkMonDef)
	{
		if (atkAbility == ABILITY_SCRAPPY
		&& (defType == TYPE_GHOST && (moveType == TYPE_NORMAL || moveType == TYPE_FIGHTING)))
			return; //Foresight breaks ghost immunity
	}

	if (move == MOVE_FREEZEDRY && defType == TYPE_WATER) //Always Super-Effective, even in Inverse Battles
		multiplier = TYPE_MUL_SUPER_EFFECTIVE;

	if (moveType == TYPE_FIRE && gBattleStruct->tarShotBits & gBitTable[bankDef]) //Fire always Super-Effective if covered in tar
		multiplier = TYPE_MUL_SUPER_EFFECTIVE;

	if (checkMonDef)
	{
		if (multiplier == TYPE_MUL_NO_EFFECT && GetMonItemEffect(monDef) == HOLD_EFFECT_RING_TARGET)
			multiplier = TYPE_MUL_NORMAL;
		else if (multiplier == TYPE_MUL_NO_EFFECT && moveType == TYPE_GROUND
		&& (CheckGroundingFromPartyData(monDef) || move == MOVE_THOUSANDARROWS))
			multiplier = TYPE_MUL_NORMAL;
	}
	else
	{
		if (multiplier == TYPE_MUL_NO_EFFECT && ITEM_EFFECT(bankDef) == HOLD_EFFECT_RING_TARGET)
			multiplier = TYPE_MUL_NORMAL;
		else if (multiplier == TYPE_MUL_NO_EFFECT && moveType == TYPE_GROUND
		&& (CheckGrounding(bankDef) || move == MOVE_THOUSANDARROWS))
			multiplier = TYPE_MUL_NORMAL;
	}

	switch (multiplier) {
		case TYPE_MUL_NO_EFFECT:
			*flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
			*flags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE | MOVE_RESULT_SUPER_EFFECTIVE);
			break;

		case TYPE_MUL_NOT_EFFECTIVE:
			if (SPLIT(move) != SPLIT_STATUS && !(*flags & MOVE_RESULT_NO_EFFECT))
			{
				if (*flags & MOVE_RESULT_SUPER_EFFECTIVE)
					*flags &= ~MOVE_RESULT_SUPER_EFFECTIVE;
				else
					*flags |= MOVE_RESULT_NOT_VERY_EFFECTIVE;
			}
			break;

		case TYPE_MUL_SUPER_EFFECTIVE:
			if (SPLIT(move) != SPLIT_STATUS && !(*flags & MOVE_RESULT_NO_EFFECT))
			{
				if (*flags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
					*flags &= ~MOVE_RESULT_NOT_VERY_EFFECTIVE;
				else
					*flags |= MOVE_RESULT_SUPER_EFFECTIVE;
			}
			break;
	}

	if (defType == TYPE_FLYING && multiplier == TYPE_MUL_SUPER_EFFECTIVE && gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL && move != MOVE_STEALTHROCK)
		multiplier = TYPE_MUL_NORMAL;

	if (multiplier != TYPE_MUL_NO_DATA && multiplier != TYPE_MUL_NORMAL)
	{
		if (multiplier == TYPE_MUL_NO_EFFECT)
			multiplier = 0; //Set to 0 properly

		gBattleMoveDamage = MathMax(1, (gBattleMoveDamage * multiplier) / 10);
	}
}

u8 GetMoveTypeSpecial(u8 bankAtk, u16 move)
{
	ability_t atkAbility = ABILITY(bankAtk);
	u8 moveType = GetMoveTypeSpecialPreAbility(move, bankAtk, NULL);
	if (moveType != 0xFF)
		return moveType;

	return GetMoveTypeSpecialPostAbility(move, atkAbility, gBattleStruct->zMoveData.active || gBattleStruct->zMoveData.viewing);
}

u8 GetMoveTypeSpecialPreAbility(u16 move, u8 bankAtk, struct Pokemon* monAtk)
{
	u8 moveType = gBattleMoves[move].type;

	if (monAtk == NULL && gBattleStruct->ElectrifyTimers[bankAtk] > 0)
		return TYPE_ELECTRIC;

	if (CheckTableForMove(move, gTypeChangeExceptionMoves))
	{
		if (monAtk != NULL)
			return GetMonExceptionMoveType(monAtk, move);
		else
			return GetExceptionMoveType(bankAtk, move);
	}

	if (moveType == TYPE_NORMAL && monAtk == NULL && IsIonDelugeActive())
		return TYPE_ELECTRIC;

	return 0xFF;
}

u8 GetMoveTypeSpecialPostAbility(u16 move, ability_t atkAbility, bool8 zMoveActive)
{
	u8 moveType = gBattleMoves[move].type;
	bool8 moveTypeCanBeChanged = !zMoveActive || SPLIT(move) == SPLIT_STATUS;

	if (moveTypeCanBeChanged)
	{
		//Change Normal-type Moves
		if (moveType == TYPE_NORMAL)
		{
			switch (atkAbility) {
				case ABILITY_REFRIGERATE:
					return TYPE_ICE;
				case ABILITY_PIXILATE:
					return TYPE_FAIRY;
				case ABILITY_AERILATE:
					return TYPE_FLYING;
				case ABILITY_GALVANIZE:
					return TYPE_ELECTRIC;
			}
		}

		//Change non-Normal-type moves
		switch (atkAbility) {
			case ABILITY_NORMALIZE:
				return TYPE_NORMAL;
			case ABILITY_LIQUIDVOICE:
				if (CheckSoundMove(move)) //Change Sound Moves
					return TYPE_WATER;
				break;
		}
	}

	return moveType;
}

u8 GetMonMoveTypeSpecial(struct Pokemon* mon, u16 move)
{
	ability_t atkAbility = GetMonAbility(mon);
	u8 moveType = GetMoveTypeSpecialPreAbility(move, 0, mon);
	if (moveType != 0xFF)
		return moveType;

	return GetMoveTypeSpecialPostAbility(move, atkAbility, FALSE);
}

bool8 IsAnyMaxMove(u16 move)
{
	return move >= FIRST_MAX_MOVE && move <= LAST_G_MAX_MOVE;
}

static bool8 AbilityCanChangeTypeAndBoost(u16 move, ability_t atkAbility, u8 electrifyTimer, bool8 checkIonDeluge, bool8 zMoveActive)
{
	u8 moveType = gBattleMoves[move].type;
	bool8 moveTypeCanBeChanged = !zMoveActive || SPLIT(move) == SPLIT_STATUS;

	if (electrifyTimer > 0
	|| IsAnyMaxMove(move)
	|| CheckTableForMove(move, gTypeChangeExceptionMoves))
		return FALSE;

	//Check Normal-type Moves
	if (moveType == TYPE_NORMAL)
	{
		if (checkIonDeluge && IsIonDelugeActive())
			return FALSE;

		if (moveTypeCanBeChanged)
		{
			switch (atkAbility) {
				case ABILITY_REFRIGERATE:
				case ABILITY_PIXILATE:
				case ABILITY_AERILATE:
				case ABILITY_GALVANIZE:
					return TRUE;
			}
		}
	}

	//Check non-Normal-type moves
	return atkAbility == ABILITY_NORMALIZE && moveTypeCanBeChanged;
}

u8 GetExceptionMoveType(u8 bankAtk, u16 move)
{
	int i;
	u8 moveType = gBattleMoves[move].type;
	u16 item = ITEM(bankAtk);
	u8 effect = ITEM_EFFECT(bankAtk);
	u8 quality = ITEM_QUALITY(bankAtk);

	switch (move) {
		case MOVE_HIDDEN_POWER:
			moveType = ((gBattleMons[bankAtk].hpIV & 1))
					 | ((gBattleMons[bankAtk].attackIV & 1) << 1)
					 | ((gBattleMons[bankAtk].defenseIV & 1) << 2)
					 | ((gBattleMons[bankAtk].speedIV & 1) << 3)
					 | ((gBattleMons[bankAtk].spAttackIV & 1) << 4)
					 | ((gBattleMons[bankAtk].spDefenseIV & 1) << 5);

			moveType = (15 * moveType) / 63 + 1;
			if (moveType >= TYPE_MYSTERY)
				moveType++;
			break;

		case MOVE_WEATHER_BALL:
			if (WEATHER_HAS_EFFECT)
			{
				if (gBattleWeather & WEATHER_RAIN_ANY && effect != HOLD_EFFECT_UTILITY_UMBRELLA)
					moveType = TYPE_WATER;
				else if (gBattleWeather & WEATHER_SANDSTORM_ANY)
					moveType = TYPE_ROCK;
				else if (gBattleWeather & WEATHER_SUN_ANY && effect != HOLD_EFFECT_UTILITY_UMBRELLA)
					moveType = TYPE_FIRE;
				else if (gBattleWeather & WEATHER_HAIL_ANY)
					moveType = TYPE_ICE;
				else
					moveType = TYPE_NORMAL;
			}
			break;

		case MOVE_NATURALGIFT:
			if (IsBerry(item))
			{
				for (i = 0; gNaturalGiftTable[i].berry != ITEM_TABLES_TERMIN; ++i)
				{
					if (gNaturalGiftTable[i].berry == item)
					{
						moveType = gNaturalGiftTable[i].type;
						goto BREAK_NATURAL_GIFT;
					}
				}
				moveType = TYPE_MYSTERY; //If the berry isn't in the table, it has no type
			}
			else
				moveType = TYPE_MYSTERY; //If Natural Gift fails, it has no type
		BREAK_NATURAL_GIFT:
			break;

		case MOVE_JUDGMENT:
			if (effect == HOLD_EFFECT_PLATE)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		case MOVE_MULTIATTACK:
			if (effect == HOLD_EFFECT_MEMORY)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		case MOVE_TECHNOBLAST:
			if (effect == HOLD_EFFECT_DRIVE)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		//Based on https://bulbapedia.bulbagarden.net/wiki/Revelation_Dance_(move)
		case MOVE_REVELATIONDANCE: 
			{
                u8 atkType1 = gBattleMons[bankAtk].type1;
                u8 atkType2 = gBattleMons[bankAtk].type2;
                u8 atkType3 = gBattleMons[bankAtk].type3;
                if (gBattleStruct->DancerInProgress)
                {
                    if (atkType1 != TYPE_MYSTERY && atkType1 != TYPE_ROOSTLESS)
                        moveType = atkType1;
                    else if (atkType2 != TYPE_MYSTERY && atkType2 != TYPE_ROOSTLESS)
                        moveType = atkType2;
                    else
                        moveType = TYPE_NORMAL;
                }
                else
                {
                    if (atkType1 != TYPE_MYSTERY && atkType1 != TYPE_ROOSTLESS)
                        moveType = atkType1;
                    else if (atkType2 != TYPE_MYSTERY && atkType2 != TYPE_ROOSTLESS)
                        moveType = atkType2;
                    else if (atkType3 != TYPE_MYSTERY && atkType3 != TYPE_ROOSTLESS && atkType3 != TYPE_BLANK)
                        moveType = atkType3;
                    else
                        moveType = TYPE_MYSTERY;
                }
            }
			break;

		case MOVE_AURAWHEEL:
			if (SPECIES(bankAtk) == SPECIES_MORPEKO_HANGRY)
				moveType = TYPE_DARK;
			break;
		
		case MOVE_TERRAINPULSE:
			switch (gTerrainType) {
				case ELECTRIC_TERRAIN:
					moveType = TYPE_ELECTRIC;
					break;
				case GRASSY_TERRAIN:
					moveType = TYPE_GRASS;
					break;
				case MISTY_TERRAIN:
					moveType = TYPE_FAIRY;
					break;
				case PSYCHIC_TERRAIN:
					moveType = TYPE_PSYCHIC;
					break;
				default:
					moveType = TYPE_NORMAL;
					break;
			}
			break;
	}

	if (moveType == TYPE_NORMAL && IsIonDelugeActive())
		moveType = TYPE_ELECTRIC;

	return moveType;
}

u8 GetMonExceptionMoveType(struct Pokemon* mon, u16 move)
{
	int i;
	u8 moveType = gBattleMoves[move].type;
	ability_t ability = GetMonAbility(mon);
	u16 item = mon->box.heldItem;
	u8 effect = ItemId_GetHoldEffect(item);
	u8 quality = ItemId_GetHoldEffectParam(item);

	if (ability == ABILITY_KLUTZ)
		effect = 0;

	switch (move) {
		case MOVE_HIDDEN_POWER:
			moveType = CalcMonHiddenPowerType(mon);
			break;

		case MOVE_WEATHER_BALL:
			if (WEATHER_HAS_EFFECT)
			{
				if (gBattleWeather & WEATHER_RAIN_ANY && effect != HOLD_EFFECT_UTILITY_UMBRELLA)
					moveType = TYPE_WATER;
				else if (gBattleWeather & WEATHER_SANDSTORM_ANY)
					moveType = TYPE_ROCK;
				else if (gBattleWeather & WEATHER_SUN_ANY && effect != HOLD_EFFECT_UTILITY_UMBRELLA)
					moveType = TYPE_FIRE;
				else if (gBattleWeather & WEATHER_HAIL_ANY)
					moveType = TYPE_ICE;
				else
					moveType = TYPE_NORMAL;
			}
			break;

		case MOVE_NATURALGIFT:
			if (IsBerry(item))
			{
				for (i = 0; gNaturalGiftTable[i].berry != ITEM_TABLES_TERMIN; ++i)
				{
					if (gNaturalGiftTable[i].berry == item)
					{
						moveType = gNaturalGiftTable[i].type;
						goto NATURAL_GIFT_BREAK;
					}
				}
				moveType = TYPE_MYSTERY; //If the berry isn't in the table, it has no type
			}
			else
				moveType = TYPE_MYSTERY; //If Natural Gift fails, it has no type
		NATURAL_GIFT_BREAK:
			break;

		case MOVE_JUDGMENT:
			if (effect == HOLD_EFFECT_PLATE && ability != ABILITY_KLUTZ)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		case MOVE_MULTIATTACK:
			if (effect == HOLD_EFFECT_MEMORY && ability != ABILITY_KLUTZ)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		case MOVE_TECHNOBLAST:
			if (effect == HOLD_EFFECT_DRIVE && ability != ABILITY_KLUTZ)
				moveType = quality;
			else
				moveType = TYPE_NORMAL;
			break;

		case MOVE_REVELATIONDANCE:
			moveType = GetMonType(mon, 0);
			break;

		case MOVE_AURAWHEEL:
			#ifdef SPECIES_MORPEKO_HANGRY
			if (mon->box.species == SPECIES_MORPEKO_HANGRY)
				moveType = TYPE_DARK;
			else
			#endif
				moveType = TYPE_ELECTRIC;
			break;

		case MOVE_TERRAINPULSE:
			switch (gTerrainType) {
				case ELECTRIC_TERRAIN:
					moveType = TYPE_ELECTRIC;
					break;
				case GRASSY_TERRAIN:
					moveType = TYPE_GRASS;
					break;
				case MISTY_TERRAIN:
					moveType = TYPE_FAIRY;
					break;
				case PSYCHIC_TERRAIN:
					moveType = TYPE_PSYCHIC;
					break;
				default:
					moveType = TYPE_NORMAL;
					break;
			}
			break;
	}

	return moveType;
}

u8 CalcMonHiddenPowerType(struct Pokemon* mon)
{
	u8 moveType;

	moveType = ((mon->box.hpIV & 1)) |
			   ((mon->box.attackIV & 1) << 1) |
			   ((mon->box.defenseIV & 1) << 2) |
			   ((mon->box.speedIV & 1) << 3) |
			   ((mon->box.spAttackIV & 1) << 4) |
			   ((mon->box.spDefenseIV & 1) << 5);

	moveType = ((15 * moveType) / 63) + 1;
	if (moveType >= TYPE_MYSTERY)
		++moveType;

	return moveType;
}

//new typecalc
//This is the type calc that is callable as a function
//This calc now also contains the calc the AI uses
u8 NewTypeCalc(u16 move, u8 bankAtk, u8 bankDef, struct Pokemon* monAtk, bool8 CheckParty)
{
	u8 moveType;
	ability_t defAbility = ABILITY(bankDef);
	u8 defEffect = ITEM_EFFECT(bankDef);
	ability_t atkAbility;
	u8 atkType1, atkType2, atkType3;
	u8 flags = 0;

	if (move == MOVE_STRUGGLE)
		return 0;

    if (CheckParty)
	{
		atkAbility = GetMonAbility(monAtk);
		atkType1 = GetMonType(monAtk, 0);
		atkType2 = GetMonType(monAtk, 1);
		atkType3 = TYPE_BLANK;
		moveType = GetMonMoveTypeSpecial(monAtk, move);
	}
	else
	{
		atkAbility = ABILITY(bankAtk);
		atkType1 = gBattleMons[bankAtk].type1;
		atkType2 = gBattleMons[bankAtk].type2;
		atkType3 = gBattleMons[bankAtk].type3;
		moveType = GetMoveTypeSpecial(bankAtk, move);
	}

	//Check stab
	if (atkType1 == moveType || atkType2 == moveType || atkType3 == moveType)
	{
		if (atkAbility == ABILITY_ADAPTABILITY)
			gBattleMoveDamage *= 2;
		else
			gBattleMoveDamage = gBattleMoveDamage * 150 / 100;
	}

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND
	&& !CheckGrounding(bankDef)
	&& ((defAbility == ABILITY_LEVITATE && NO_MOLD_BREAKERS(atkAbility, move)) || defEffect == HOLD_EFFECT_AIR_BALLOON || (gStatuses3[bankDef] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS)))
	&& move != MOVE_THOUSANDARROWS)
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if (CheckTableForMove(move, gPowderMoves) && !IsAffectedByPowder(bankDef))
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if (move == MOVE_SKYDROP && IsOfType(bankDef, TYPE_FLYING))
	{
		flags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	//Regular Type Calc
	else
	{
		TypeDamageModification(atkAbility, bankDef, move, moveType, &flags);
	}

	//Wonder Guard Check
	if (defAbility == ABILITY_WONDER_GUARD
	&& NO_MOLD_BREAKERS(atkAbility, move)
	&& !(flags & MOVE_RESULT_MISSED)
	&& AttacksThisTurn(bankAtk, move) == 2
	&& (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[move].power
	&& SPLIT(move) != SPLIT_STATUS)
	{
		flags |= MOVE_RESULT_MISSED;
	}

	return flags;
}

void TypeDamageModification(ability_t atkAbility, u8 bankDef, u16 move, u8 moveType, u8* flags)
{
	return TypeDamageModificationByDefTypes(atkAbility, bankDef, move, moveType, flags, gBattleMons[bankDef].type1, gBattleMons[bankDef].type2, gBattleMons[bankDef].type3);
}

const u8 gTypeEffectiveness_2[NUMBER_OF_MON_TYPES][NUMBER_OF_MON_TYPES] = //need todo
{
	[TYPE_NORMAL]=
	{
		[TYPE_ROCK] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_STEEL] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GHOST] = TYPE_MUL_NO_EFFECT,
	},
	[TYPE_FIRE]=
	{
		[TYPE_FIRE] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_WATER] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GRASS] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_ICE] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_BUG] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_ROCK] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_DRAGON] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_STEEL] = TYPE_MUL_SUPER_EFFECTIVE,
	},
	[TYPE_WATER]=
	{
		[TYPE_FIRE] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_WATER] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GRASS] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GROUND] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_ROCK] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_DRAGON] = TYPE_MUL_NOT_EFFECTIVE,
	},
	[TYPE_ELECTRIC]=
	{
		[TYPE_WATER] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_ELECTRIC] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GRASS] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GROUND] = TYPE_MUL_NO_EFFECT,
		[TYPE_FLYING] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_DRAGON] = TYPE_MUL_NOT_EFFECTIVE,
	},
	[TYPE_GRASS]=
	{
		[TYPE_FIRE] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_WATER] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_GRASS] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_POISON] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GROUND] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_FLYING] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_BUG] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_ROCK] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_DRAGON] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_STEEL] = TYPE_MUL_NOT_EFFECTIVE,
	},
	[TYPE_ICE]=
	{
		[TYPE_WATER] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GRASS] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_ICE] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GROUND] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_FLYING] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_DRAGON] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_STEEL] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_FIRE] = TYPE_MUL_NOT_EFFECTIVE,
	},
	[TYPE_FIGHTING]=
	{
		[TYPE_NORMAL] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_ICE] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_POISON] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_FLYING] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_PSYCHIC] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_BUG] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_FAIRY] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_ROCK] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_DARK] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_STEEL] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_GHOST] = TYPE_MUL_NO_EFFECT,
	},
	[TYPE_POISON]=
	{
		[TYPE_GRASS] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_FAIRY] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_POISON] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GROUND] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_ROCK] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GHOST] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_STEEL] = TYPE_MUL_NO_EFFECT,
	},
	[TYPE_GROUND]=
	{
		[TYPE_FIRE] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_ELECTRIC] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_GRASS] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_POISON] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_FLYING] = TYPE_MUL_NO_EFFECT,
		[TYPE_BUG] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_ROCK] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_STEEL] = TYPE_MUL_SUPER_EFFECTIVE,
	},
	[TYPE_FLYING]=
	{
		[TYPE_ELECTRIC] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GRASS] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_FIGHTING] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_BUG] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_ROCK] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_STEEL] = TYPE_MUL_NOT_EFFECTIVE,
	},
	[TYPE_PSYCHIC]=
	{
		[TYPE_FIGHTING] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_POISON] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_PSYCHIC] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_DARK] = TYPE_MUL_NO_EFFECT,
		[TYPE_STEEL] = TYPE_MUL_NOT_EFFECTIVE,
	},
	[TYPE_BUG]=
	{
		[TYPE_FIRE] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GRASS] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_FIGHTING] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_POISON] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_FLYING] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_PSYCHIC] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_GHOST] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_DARK] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_STEEL] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_FAIRY] = TYPE_MUL_NOT_EFFECTIVE,
	},
	[TYPE_ROCK]=
	{
		[TYPE_FIRE] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_ICE] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_FIGHTING] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GROUND] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_FLYING] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_BUG] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_STEEL] = TYPE_MUL_NOT_EFFECTIVE,
	},
	[TYPE_GHOST]=
	{
		[TYPE_NORMAL] = TYPE_MUL_NO_EFFECT,
		[TYPE_PSYCHIC] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_DARK] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_GHOST] = TYPE_MUL_SUPER_EFFECTIVE,
	},
	[TYPE_DRAGON]=
	{
		[TYPE_DRAGON] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_STEEL] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_FAIRY] = TYPE_MUL_NO_EFFECT,
	},
	[TYPE_DARK]=
	{
		[TYPE_FIGHTING] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_PSYCHIC] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_GHOST] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_DARK] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_FAIRY] = TYPE_MUL_NOT_EFFECTIVE,
	},
	[TYPE_STEEL]=
	{
		[TYPE_STEEL] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_FIRE] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_WATER] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_ELECTRIC] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_ICE] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_ROCK] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_FAIRY] = TYPE_MUL_SUPER_EFFECTIVE,
	},
	[TYPE_FAIRY]=
	{
		[TYPE_POISON] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_STEEL] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_FIRE] = TYPE_MUL_NOT_EFFECTIVE,
		[TYPE_FIGHTING] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_DRAGON] = TYPE_MUL_SUPER_EFFECTIVE,
		[TYPE_DARK] = TYPE_MUL_SUPER_EFFECTIVE,
	},
};

static void TypeDamageModificationByDefTypes(ability_t atkAbility, u8 bankDef, u16 move, u8 moveType, u8* flags, u8 defType1, u8 defType2, u8 defType3)
{
	u8 multiplier1, multiplier2, multiplier3;

TYPE_LOOP:
	multiplier1 = gTypeEffectiveness_2[moveType][defType1];
	multiplier2 = gTypeEffectiveness_2[moveType][defType2];
	multiplier3 = gTypeEffectiveness_2[moveType][defType3];

	//If the multiplier is 0, that means normal damage. No effect is 1 (it is modified to 0 later).
	ModulateDmgByType(multiplier1, move, moveType, defType1, bankDef, atkAbility, flags, 0, FALSE);

	if (defType1 != defType2)
		ModulateDmgByType(multiplier2, move, moveType, defType2, bankDef, atkAbility, flags, 0, FALSE);

	if (defType3 != defType1 && defType3 != defType2)
		ModulateDmgByType(multiplier3, move, moveType, defType3, bankDef, atkAbility, flags, 0, FALSE);

	if (move == MOVE_FLYINGPRESS && moveType != TYPE_FLYING)
	{
		moveType = TYPE_FLYING;
		goto TYPE_LOOP;
	}
}

bool8 IsBankHoldingFocusSash(u8 bank)
{
	if (ITEM_EFFECT(bank) == HOLD_EFFECT_FOCUS_BAND
	&& itemid_get_x19(ITEM(bank)))
		return TRUE;

	return FALSE;
}

s32 CalculateBaseDamage(struct DamageCalc* data)
{
	u32 attack, spAttack;

	//Take variables off struct for easier access
	u8 bankAtk = data->bankAtk;
	u8 bankDef = data->bankDef;
	u16 move = data->move;

	u16 power = 0;
	u32 damage = 0;
	bool8 useMonAtk = data->monAtk != NULL;
	bool8 useMonDef = data->monDef != NULL;
	
	if (!data->attackerLoaded)
		PopulateDamageCalcStructWithBaseAttackerData(data);
	if (!data->defenderLoaded)
		PopulateDamageCalcStructWithBaseDefenderData(data);

//Load attacker Data
	if (useMonAtk)
	{
		struct Pokemon* monAtk = data->monAtk;

		switch (data->move) {
			case MOVE_BODYPRESS:
				attack = monAtk->defense;
				spAttack = monAtk->spDefense;
				break;
			default:
				attack = monAtk->attack;
				spAttack = monAtk->spAttack;
		}

		data->atkBuff = 0;
		data->spAtkBuff = 0;

		data->moveSplit = CalcMoveSplitFromParty(monAtk, move);
		data->moveType = GetMonMoveTypeSpecial(monAtk, move);

		/*if (useMonDef) //CAN'T AND SHOULD NOT HAPPEN
			data->resultFlags = AI_TypeCalc(move, monAtk, monDef);
		else*/
			data->resultFlags = NewTypeCalc(move, bankAtk, bankDef, monAtk, TRUE);
	}
	else //Load from bank
	{
		switch (data->move) {
			case MOVE_BODYPRESS:
				attack = gBattleMons[bankAtk].defense;
				spAttack = gBattleMons[bankAtk].spDefense;
				if (IsWonderRoomActive())
				{
					//For some strange reason in SwSh, Wonder Room swaps the defense boosts
					//when using Body Press, as opposed to the actual stat.
					data->atkBuff = STAT_STAGE(bankAtk, STAT_STAGE_SPDEF);
					data->spAtkBuff = STAT_STAGE(bankAtk, STAT_STAGE_DEF);
				}
				else
				{
					data->atkBuff = STAT_STAGE(bankAtk, STAT_STAGE_DEF);
					data->spAtkBuff = STAT_STAGE(bankAtk, STAT_STAGE_SPDEF);
				}
				break;
			default:
				attack = gBattleMons[bankAtk].attack;
				spAttack = gBattleMons[bankAtk].spAttack;
				data->atkBuff = STAT_STAGE(bankAtk, STAT_STAGE_ATK);
				data->spAtkBuff = STAT_STAGE(bankAtk, STAT_STAGE_SPATK);
		}

		data->moveSplit = CalcMoveSplit(bankAtk, move);
		data->moveType = GetMoveTypeSpecial(bankAtk, move);

		if (useMonDef)
			data->resultFlags = NewAI_TypeCalc(move, bankAtk, data->monDef);
		else if (data->specialFlags & FLAG_AI_CALC)
			data->resultFlags = AI_SpecialTypeCalc(move, bankAtk, bankDef); //Takes into account things like Illusion
		else
			data->resultFlags = NewTypeCalc(move, bankAtk, bankDef, NULL, FALSE);
	}

//Load target data
	if (useMonDef)
	{
		struct Pokemon* monDef = data->monDef;

		switch (data->move) {
			case MOVE_FOULPLAY:
				attack = monDef->attack;
				spAttack = monDef->spAttack;
				data->atkBuff = 6; //Party mon has no buffs
				data->spAtkBuff = 6;
				break;
		}
	}
	else //Load from bank
	{
		switch (data->move) {
			case MOVE_FOULPLAY:
				attack = gBattleMons[bankDef].attack;
				spAttack = gBattleMons[bankDef].spAttack;
				data->atkBuff = STAT_STAGE(bankDef, STAT_STAGE_ATK);
				data->spAtkBuff = STAT_STAGE(bankDef, STAT_STAGE_SPATK);
				break;
		}
	}

	data->basePower = gBattleMoves[move].power; //Save real base power for later

//Load correct move power
	if (!(data->specialFlags & FLAG_CONFUSION_DAMAGE))
	{
		power = GetBasePower(data);
		power = AdjustBasePower(data, power);
	}
	else //Self-confusion damage
	{
		gBattleMovePower = power = data->basePower;
		data->moveType = TYPE_MYSTERY;
		data->resultFlags = 0;
	}

//Attacker Ability Checks
	switch(data->atkAbility) {
		case ABILITY_HUGE_POWER:
//		case ABILITY_PUREPOWER:
		//2x Boost
			attack *= 2;
			break;

		case ABILITY_FLOWERGIFT:
		//1.5x Boost
			if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
			&& data->atkItemEffect != HOLD_EFFECT_UTILITY_UMBRELLA)
				attack = (attack * 15) / 10;
			break;

		case ABILITY_PLUS:
		case ABILITY_MINUS:
		//1.5x Boost
			if (data->atkPartnerAbility == ABILITY_PLUS || data->atkPartnerAbility == ABILITY_MINUS) //Double battle check prior
				spAttack = (spAttack * 15) / 10;
			break;

		case ABILITY_HUSTLE:
		//1.5x Boost
			attack = (attack * 15) / 10;
			break;

		case ABILITY_GUTS:
		//1.5x Boost
			if (data->atkStatus1 & STATUS1_ANY)
				attack = (attack * 15) / 10;
			break;

		case ABILITY_SOLARPOWER:
		//1.5x Boost
			if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
			&& data->atkItemEffect != HOLD_EFFECT_UTILITY_UMBRELLA)
				spAttack = (spAttack * 15) / 10;
			break;

		case ABILITY_SLOWSTART:
		//0.5x Boost
			if (useMonAtk)
				attack /= 2; //Pokemon in the party would start with half atk
			else if (gBattleStruct->SlowStartTimers[bankAtk])
				attack /= 2;
			break;

		case ABILITY_DEFEATIST:
		//0.5x Boost
			if (data->atkHP <= (data->atkMaxHP / 2))
			{
				attack /= 2;
				spAttack /= 2;
			}
			break;

		case ABILITY_FLASH_FIRE:
		//1.5x Boost
			if (!useMonAtk && data->moveType == TYPE_FIRE
			&& (gBattleResources->flags->flags[bankAtk] & RESOURCE_FLAG_FLASH_FIRE))
				spAttack = (spAttack * 15) / 10;
			break;

		case ABILITY_OVERGROW:
		//1.5x Boost
			if (data->moveType == TYPE_GRASS && data->atkHP <= data->atkMaxHP / 3)
			{
				attack = (attack * 15) / 10;
				spAttack = (spAttack * 15) / 10;
			}
			break;
		case ABILITY_BLAZE:
		//1.5x Boost
			if (data->moveType == TYPE_FIRE && data->atkHP <= data->atkMaxHP / 3)
			{
				attack = (attack * 15) / 10;
				spAttack = (spAttack * 15) / 10;
			}
			break;
		case ABILITY_TORRENT:
		//1.5x Boost
			if (data->moveType == TYPE_WATER && data->atkHP <= data->atkMaxHP / 3)
			{
				attack = (attack * 15) / 10;
				spAttack = (spAttack * 15) / 10;
			}
			break;
		case ABILITY_SWARM:
		//1.5x Boost
			if (data->moveType == TYPE_BUG && data->atkHP <= data->atkMaxHP / 3)
			{
				attack = (attack * 15) / 10;
				spAttack = (spAttack * 15) / 10;
			}
			break;

		case ABILITY_GORILLATACTICS:
		//1.5x Boost
			if (!IsDynamaxed(bankAtk))
				attack = (attack * 15) / 10;
			break;
	}

	switch (data->atkPartnerAbility) {
		case ABILITY_FLOWERGIFT:
			if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
			&& ITEM_EFFECT(PARTNER(bankAtk)) != HOLD_EFFECT_UTILITY_UMBRELLA)
				attack = (attack * 15) / 10;
			break;
	}

//Target Ability Checks
	switch (data->defAbility) {
		case ABILITY_MARVEL_SCALE:
		//1.5x Boost
			if (data->defStatus1 & STATUS1_ANY)
				data->defense = (data->defense * 15) / 10;
			break;

		case ABILITY_GRASSPELT:
		//1.5x Boost
			if (gTerrainType == GRASSY_TERRAIN)
				data->defense = (data->defense * 15) / 10;
			break;

		case ABILITY_THICK_FAT:
		//0.5x Decrement
			if (data->moveType == TYPE_FIRE || data->moveType == TYPE_ICE)
			{
				attack /= 2;
				spAttack /= 2;
			}
			break;

		case ABILITY_FURCOAT:
		//2x Boost
			data->defense *= 2;
			break;

		case ABILITY_PORTALPOWER:
		//0.75x Decrement
		#ifdef PORTAL_POWER
			if ((useMonAtk && !CheckContactByMon(move, data->monAtk))
			|| (!useMonAtk && !CheckContact(move, bankAtk)))
			{
				attack = (attack * 75) / 100;
				spAttack = (spAttack * 75) / 100;
			}
		#endif
			break;
	}

//Attacker Item Checks
	switch (data->atkItemEffect) {
		case HOLD_EFFECT_CHOICE_BAND:
		//1.5x Boost
			if (!IsDynamaxed(bankAtk))
			{
				switch (data->atkItemQuality) {
					case QUALITY_CHOICE_BAND:
						attack = (attack * 15) / 10;
						break;

					case QUALITY_CHOICE_SPECS:
						spAttack = (spAttack * 15) / 10;
						break;
				}
			}
			break;

		#ifdef SPECIES_CUBONE
		case HOLD_EFFECT_THICK_CLUB:
		//2x Boost
			if (data->atkSpecies == SPECIES_CUBONE
			#ifdef SPECIES_MAROWAK
			||  data->atkSpecies == SPECIES_MAROWAK
			#endif
			#ifdef SPECIES_MAROWAK_A
			||  data->atkSpecies == SPECIES_MAROWAK_A
			#endif
			)
				attack *= 2;
			break;
		#endif

		#ifdef NATIONAL_DEX_PIKACHU
		case HOLD_EFFECT_LIGHT_BALL:
		//2x Boost
			if (SpeciesToNationalPokedexNum(data->atkSpecies) == NATIONAL_DEX_PIKACHU)
			{
				attack *= 2;
				spAttack *= 2;
			}
			break;
		#endif

		#ifdef SPECIES_CLAMPERL
		case HOLD_EFFECT_DEEP_SEA_TOOTH:
		//2x Boost
			if (data->atkSpecies == SPECIES_CLAMPERL)
				spAttack *= 2;
			break;
		#endif

		#if (defined OLD_SOUL_DEW_EFFECT && defined SPECIES_LATIOS && defined SPECIES_LATIAS)
		case HOLD_EFFECT_SOUL_DEW:
		//1.5x Boost
			if (data->atkSpecies == SPECIES_LATIOS || data->atkSpecies == SPECIES_LATIAS)
				spAttack = (spAttack * 3) / 2;
			break;
		#endif
	}

//Target Item Checks
	switch (data->defItemEffect)
	{
		#if (defined OLD_SOUL_DEW_EFFECT && defined SPECIES_LATIOS && defined SPECIES_LATIAS)
		case HOLD_EFFECT_SOUL_DEW:
			if (data->defSpecies == SPECIES_LATIOS || data->defSpecies == SPECIES_LATIAS)
				data->spDefense = (data->spDefense * 3) / 2; //1.5
			break;
		#endif

		#ifdef SPECIES_DITTO
		case HOLD_EFFECT_METAL_POWDER:
			if (data->defSpecies == SPECIES_DITTO && (useMonDef || !IS_TRANSFORMED(bankDef)))
				data->defense *= 2;
			break;
		#endif

		#ifdef SPECIES_CLAMPERL
		case HOLD_EFFECT_DEEP_SEA_SCALE:
			if (data->defSpecies == SPECIES_CLAMPERL)
				data->spDefense *= 2;
			break;
		#endif

		case HOLD_EFFECT_EVIOLITE:
			if ((useMonDef && CanEvolve(data->monDef))
			|| (!useMonDef && CanEvolve(GetBankPartyData(bankDef))))
			{
				data->defense = (data->defense * 15) / 10;
				data->spDefense = (data->spDefense * 15) / 10;
			}
			break;

		case HOLD_EFFECT_ASSAULT_VEST:
			data->spDefense = (data->spDefense * 15) / 10;
			break;
	}

//Gym Badge Checks
	#ifdef BADGE_BOOSTS
	//1.1x Boosts
		if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER))
		&& gBattleTypeFlags & BATTLE_TYPE_TRAINER
		&& SIDE(bankAtk) == B_SIDE_PLAYER
		&& gTrainerBattleOpponent_A != TRAINER_SECRET_BASE)
		{
			if (FlagGet(FLAG_BADGE01_GET) && SIDE(bankAtk) == B_SIDE_PLAYER)
				attack = (11 * attack) / 10;

			if (FlagGet(FLAG_BADGE05_GET) && SIDE(bankDef) == B_SIDE_PLAYER)
				data->defense = (11 * data->defense) / 10;

			if (FlagGet(FLAG_BADGE07_GET) && SIDE(bankAtk) == B_SIDE_PLAYER)
				spAttack = (11 * spAttack) / 10;

			if (FlagGet(FLAG_BADGE07_GET) && SIDE(bankDef) == B_SIDE_PLAYER)
				data->spDefense = (11 * data->spDefense) / 10;
		}
	#endif

//Sandstorm Sp. Def Increase
	if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SANDSTORM_ANY)
	&& ((!useMonDef && IsOfType(bankDef, TYPE_ROCK)) || (useMonDef && IsMonOfType(data->monDef, TYPE_ROCK))))
		data->spDefense = (15 * data->spDefense) / 10;

//Old Exploding Check
	#ifdef OLD_EXPLOSION_BOOST
		if (move == MOVE_SELFDESTRUCT || move == MOVE_EXPLOSION)
			data->defense /= 2;
	#endif

//Stat Buffs - Attacker
	if (data->defAbility != ABILITY_UNAWARE && !useMonAtk)
	{
		if (gCritMultiplier > BASE_CRIT_MULTIPLIER)
		{
			if (data->atkBuff > 6)
				APPLY_QUICK_STAT_MOD(attack, data->atkBuff);

			if (data->spAtkBuff > 6)
				APPLY_QUICK_STAT_MOD(spAttack, data->spAtkBuff);
		}
		else
		{
			APPLY_QUICK_STAT_MOD(attack, data->atkBuff);
			APPLY_QUICK_STAT_MOD(spAttack, data->spAtkBuff);
		}
	}

//Stat Buffs - Target
	if (data->atkAbility != ABILITY_UNAWARE && !useMonDef && !CheckTableForMove(move, gIgnoreStatChangesMoves))
	{
		if (gCritMultiplier > BASE_CRIT_MULTIPLIER)
		{
			if (data->defBuff < 6)
				APPLY_QUICK_STAT_MOD(data->defense, data->defBuff);

			if (data->spDefBuff < 6)
				APPLY_QUICK_STAT_MOD(data->spDefense, data->spDefBuff);
		}
		else
		{
			APPLY_QUICK_STAT_MOD(data->defense, data->defBuff);
			APPLY_QUICK_STAT_MOD(data->spDefense, data->spDefBuff);
		}
	}

//Actual Calculation
	if (useMonAtk)
		damage = (2 * data->monAtk->level) / 5 + 2;
	else
		damage = (2 * gBattleMons[bankAtk].level) / 5 + 2;

	damage *= power;

	if (CheckTableForMove(move, gSpecialAttackPhysicalDamageMoves))
	{
		damage *= spAttack;
		damage /= MathMax(1, data->defense); //MathMax prevents underflow
	}
	else
	{
		switch (data->moveSplit) {
			default:
			case SPLIT_PHYSICAL:
					damage *= attack;
					damage /= MathMax(1, data->defense);
					break;
			case SPLIT_SPECIAL:
					damage *= spAttack;
					damage /= MathMax(1, data->spDefense);
					break;
		}
	}

	damage /= 50;
	damage += 2;

//Final Damage mods - Likely boosts placed first, followed by damage cuts
	//Specific Move Modifiers
	switch (move) {
		case MOVE_ASSURANCE:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			&& (gProtectStructs[bankDef].physicalDmg || gProtectStructs[bankDef].specialDmg))
				damage *= 2;
			break;

		case MOVE_MAGNITUDE:
		case MOVE_EARTHQUAKE:
			if (data->defStatus3 & STATUS3_UNDERGROUND)
				damage *= 2;
			break;

		case MOVE_SURF:
			if (data->defStatus3 & STATUS3_UNDERWATER)
				damage *= 2;
			break;

		case MOVE_KNOCKOFF:
			if ((!useMonDef && CanKnockOffItem(bankDef))
			|| (useMonDef && CanKnockOffMonItem(data->monDef, SIDE(bankDef))))
				damage = (damage * 15) / 10;
			break;

		case MOVE_SOLARBEAM:
		case MOVE_SOLARBLADE:
			if (WEATHER_HAS_EFFECT
			&& data->atkItemEffect != HOLD_EFFECT_UTILITY_UMBRELLA
			&& gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_SANDSTORM_ANY | WEATHER_HAIL_ANY | WEATHER_FOG_ANY | WEATHER_AIR_CURRENT_PRIMAL))
				damage /= 2; //Any weather except sun weakens Solar Beam
			break;

		case MOVE_DYNAMAXCANNON:
		case MOVE_BEHEMOTHBLADE:
		case MOVE_BEHEMOTHBASH:
			if (IsDynamaxed(bankDef))
				damage *= 2;
			break;
	}

	//Stomp Minimize Boost
	if (data->defStatus3 & STATUS3_MINIMIZED && CheckTableForMove(move, gAlwaysHitWhenMinimizedMoves))
		damage *= 2;

	//Me First Boost
	if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC)) && gBattleStruct->MeFirstByte)
		damage = (damage * 15) / 10;

	//Helping Hand Boost
	if (!(data->specialFlags & FLAG_AI_CALC) && gProtectStructs[bankAtk].helpingHand)
		damage = (damage * 15) / 10;

	//Weather Boost
	if (WEATHER_HAS_EFFECT && data->defItemEffect != HOLD_EFFECT_UTILITY_UMBRELLA)
	{
		if (gBattleWeather & WEATHER_RAIN_ANY)
		{
			switch (data->moveType) {
				case TYPE_FIRE:
					damage /= 2;
					break;
				case TYPE_WATER:
					damage = (damage * 15) / 10;
					break;
			}
		}
		else if (gBattleWeather & WEATHER_SUN_ANY)
		{
			switch (data->moveType) {
				case TYPE_FIRE:
					damage = (damage * 15) / 10;
					break;
				case TYPE_WATER:
					damage /= 2;
					break;
			}
		}
	}

	//Aura Abilities
	if ((data->moveType == TYPE_DARK
		&& (ABILITY_PRESENT(ABILITY_DARKAURA) || data->atkAbility == ABILITY_DARKAURA || data->defAbility == ABILITY_DARKAURA)) //Check all because may be party mon
	||  (data->moveType == TYPE_FAIRY
		&& (ABILITY_PRESENT(ABILITY_FAIRYAURA) || data->atkAbility == ABILITY_FAIRYAURA || data->defAbility == ABILITY_FAIRYAURA)))
	{
		if (ABILITY_PRESENT(ABILITY_AURABREAK) || data->atkAbility == ABILITY_AURABREAK || data->defAbility == ABILITY_AURABREAK)
			damage = (damage * 75) / 100;
		else
			damage = (damage * 4) / 3;
	}

	//Second Attacker Ability Checks
	switch (data->atkAbility) {
		case ABILITY_TINTEDLENS:
			if (data->resultFlags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
				damage *= 2;
			break;

		case ABILITY_SNIPER:
			if (gCritMultiplier > BASE_CRIT_MULTIPLIER)
				damage = (damage * 15) / 10;
			break;
	}

	//Second Attacker Item Checks
	switch (data->atkItemEffect) {
		case HOLD_EFFECT_EXPERT_BELT:
		//1.2x Boost
			if (data->resultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
				damage = (damage * 12) / 10;
			break;
	}

	//Second Target Ability Checks
	switch (data->defAbility) {
		case ABILITY_DRYSKIN:
		//1.5x Boost
			if (data->moveType == TYPE_FIRE)
				damage = (damage * 125) / 100;
			break;

		case ABILITY_SOLIDROCK:
		case ABILITY_FILTER:
		case ABILITY_PRISMARMOR:
		//0.75x Decrement
			if (data->resultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
				damage = (damage * 75) / 100;
			break;

		case ABILITY_HEATPROOF:
		case ABILITY_WATERBUBBLE:
		//0.5x Decrement
			if (data->moveType == TYPE_FIRE)
				damage /= 2;
			break;

		case ABILITY_MULTISCALE:
		case ABILITY_SHADOWSHIELD:
		//0.5x Decrement
			if (data->defHP >= data->defMaxHP)
				damage /= 2;
			break;

		case ABILITY_FLUFFY:
		//2x Boost / 0.5x Decrement
			if (data->moveType == TYPE_FIRE)
				damage *= 2;

			if ((useMonAtk && CheckContactByMon(move, data->monAtk))
			|| (!useMonAtk && CheckContact(move, bankAtk)))
				damage /= 2;
			break;

		case ABILITY_PUNKROCK:
		//0.5x Decrement
			if (CheckSoundMove(move))
				damage /= 2; //50 % reduction
			break;

		case ABILITY_ICESCALES:
		//0.5x Decrement
			if (data->moveSplit == SPLIT_SPECIAL)
				damage /= 2;
			break;
	}

	//Second Target Partner Ability Checks
	if (data->defPartnerAbility == ABILITY_FRIENDGUARD)
		damage = (damage * 75) / 100;

	//Second Target Item Checks
	switch (data->defItemEffect) {
		case HOLD_EFFECT_WEAKNESS_BERRY:
			if (!AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, bankDef, ABILITY_UNNERVE, 0, 0) && data->atkAbility != ABILITY_UNNERVE)
			{
				if ((data->resultFlags & MOVE_RESULT_SUPER_EFFECTIVE && data->defItemQuality == data->moveType)
				|| (data->defItemQuality == TYPE_NORMAL && data->moveType == TYPE_NORMAL)) //Chilan Berry
				{
					if (data->defAbility == ABILITY_RIPEN)
						damage /= 3;
					else
						damage /= 2;
				}
			}
			break;
	}

	//Screen Resistance
	if (gBattleMoves[move].effect != EFFECT_BRICK_BREAK) //Shatters the screens
	{
		switch (data->moveSplit) {
			case SPLIT_PHYSICAL:
				if ((data->defSideStatus & SIDE_STATUS_REFLECT || gBattleStruct->AuroraVeilTimers[SIDE(bankDef)])
				&& gCritMultiplier <= BASE_CRIT_MULTIPLIER
				&& data->atkAbility != ABILITY_INFILTRATOR)
				{
					if (IS_DOUBLE_BATTLE && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef) >= 2)
						damage = (damage * 2) / 3;
					else
						damage /= 2;
				}
				break;

			case SPLIT_SPECIAL:
				if ((data->defSideStatus & SIDE_STATUS_LIGHTSCREEN || gBattleStruct->AuroraVeilTimers[SIDE(bankDef)])
				&& gCritMultiplier <= BASE_CRIT_MULTIPLIER
				&& data->atkAbility != ABILITY_INFILTRATOR)
				{
					if (IS_DOUBLE_BATTLE && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef) >= 2)
						damage = (damage * 2) / 3;
					else
						damage /= 2;
				}
				break;
		}
	}

	//Burn Cut
	if (data->moveSplit == SPLIT_PHYSICAL
	&& (data->atkStatus1 & STATUS1_BURN)
	&& data->atkAbility != ABILITY_GUTS
	&& move != MOVE_FACADE)
		damage /= 2;

	//Parental Bond Second Strike
	if (gBattleStruct->ParentalBondOn == 1)
	{
		#ifdef OLD_PARENTAL_BOND_DAMAGE
			damage /= 2;
		#else
			damage /= 4;
		#endif
	}

	//Spread Move Cut
	if (IS_DOUBLE_BATTLE)
	{
		if (gBattleMoves[move].target & MOVE_TARGET_BOTH && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, bankAtk, bankDef) >= 2)
			damage = (damage * 75) / 100;

		else if (gBattleMoves[move].target & MOVE_TARGET_FOES_AND_ALLY && CountAliveMonsInBattle(BATTLE_ALIVE_EXCEPT_ACTIVE, bankAtk, bankDef) >= 2)
			damage = (damage * 75) / 100;
	}

	if (damage == 0)
		damage = 1;
	else if (damage > 0x7FFFFFFF) //Prevent overflow
		damage = 0x7FFFFFFF;

	return damage;
}

static u16 GetBasePower(struct DamageCalc* data)
{
	int i;
	u16 power = data->basePower;

	//Take variables off struct for easier access
	u8 bankAtk = data->bankAtk;
	u8 bankDef = data->bankDef;
	u16 move = data->move;

	bool8 useMonAtk = data->monAtk != NULL;
	bool8 useMonDef = data->monDef != NULL;

	if (gBattleStruct->zMoveData.active) //Only active at runtime
	{
		if (gBattleStruct->ai.zMoveHelper == MOVE_NONE) //Would be set by moves like Mirror Move & Me First
			gBattleStruct->ai.zMoveHelper = gBattleMons[bankAtk].moves[gBattleStruct->chosenMovePositions[bankAtk]];

		return GetZMovePower(move);
	}
	else if (gBattleStruct->dynamaxData.active) //Only active at runtime
	{
		gBattleStruct->ai.zMoveHelper = gBattleMons[bankAtk].moves[gBattleStruct->chosenMovePositions[bankAtk]];
		return GetMaxMovePower();
	}
	else if (IsZMove(move)) //Only used in AI calcs
	{
		return GetZMovePower(move);
	}
	else if (IsAnyMaxMove(move)) //Only used in AI calcs
	{
		return GetMaxMovePower();
	}

	switch (move) {
		case MOVE_ACROBATICS:
			if (data->atkItem == ITEM_NONE)
				power *= 2;
			break;

		case MOVE_AVALANCHE:
		case MOVE_REVENGE:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			&& !useMonAtk
			&& ((gProtectStructs[bankAtk].physicalDmg && gProtectStructs[bankAtk].physicalBattlerId == bankDef)
			 || (gProtectStructs[bankAtk].specialDmg && gProtectStructs[bankAtk].specialBattlerId == bankDef)))
				power *= 2;
			break;

		case MOVE_BRINE:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defHP < data->defMaxHP / 2)
				power *= 2;
			break;

		case MOVE_ECHOEDVOICE:
			power = MathMin(200, power + (40 * gBattleStruct->EchoedVoiceDamageScale));
			break;

		case MOVE_FACADE:
			if (data->atkStatus1 & STATUS1_ANY)
				power *= 2;
			break;

		case MOVE_FURYCUTTER:
			if (!useMonAtk)
			{
				for (i = 0; i < gDisableStructs[bankAtk].furyCutterCounter; ++i)
					power *= 2;
			}
			break;

		case MOVE_FUSIONBOLT:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			&& !useMonAtk
			&& gBattleStruct->fusionFlareUsedPrior
			&& !IsFirstAttacker(bankAtk))
				power *= 2;
			break;

		case MOVE_FUSIONFLARE:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			&& !useMonAtk
			&& gBattleStruct->fusionBoltUsedPrior
			&& !IsFirstAttacker(bankAtk))
				power *= 2;
			break;

		case MOVE_HEX:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus1 & STATUS1_ANY)
				power *= 2;
			break;

		case MOVE_PAYBACK:
			if (!(data->specialFlags & (FLAG_IGNORE_TARGET | FLAG_CHECKING_FROM_MENU))
			&& !useMonAtk
			&& !useMonDef
			&& BankMovedBefore(bankDef, bankAtk))
				power *= 2;
			break;

		case MOVE_RETALIATE:
			if (gBattleStruct->RetaliateCounters[SIDE(bankAtk)]) //Bank should be accurate for party too
				power *= 2;
			break;

		case MOVE_ROUND:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			&& !useMonAtk
			&& gBattleStruct->roundUsed)
				power *= 2;
			break;

		case MOVE_SMELLINGSALTS:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus1 & STATUS1_PARALYSIS)
				power *= 2;
			break;

		case MOVE_STOMPINGTANTRUM:
			if (!useMonAtk && gBattleStruct->StompingTantrumTimers[bankAtk])
				power *= 2;
			break;

		case MOVE_VENOSHOCK:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus1 & STATUS1_PSN_ANY)
				power *= 2;
			break;

		case MOVE_WAKEUPSLAP:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus1 & STATUS1_SLEEP)
				power *= 2;
			break;

		case MOVE_WEATHERBALL:
			if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_ANY && !(gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL))
				power *= 2;
			break;

		case MOVE_WHIRLPOOL:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus3 & STATUS3_UNDERWATER)
				power *= 2;
			break;

		case MOVE_GUST:
		case MOVE_TWISTER:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET)
			&& data->defStatus3 & (STATUS3_ON_AIR | STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET))
				power *= 2;
			break;

		case MOVE_ROLLOUT:
		case MOVE_ICEBALL:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC)) && !useMonAtk)
			{
				if (gBattleMons[bankAtk].status2 & STATUS2_DEFENSE_CURL)
					power *= 2;

				if (gBattleMons[bankAtk].status2 & STATUS2_MULTIPLETURNS) //Rollout has started
				{
					for (i = 1; i < (5 - gDisableStructs[bankAtk].rolloutTimer); ++i)
						power *= 2;
				}
			}
			break;

		case MOVE_GRASSPLEDGE:
		case MOVE_FIREPLEDGE:
		case MOVE_WATERPLEDGE:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC)) && !useMonAtk && gBattleStruct->PledgeHelper)
				power = 150;
			break;

		case MOVE_ELECTROBALL:
		{
			u32 ratio;
			if (!(data->specialFlags & FLAG_IGNORE_TARGET))
			{
				ratio = data->atkSpeed / data->defSpeed;

				if (ratio >= 4)
					power = 150;
				else if (ratio >= 3)
					power = 120;
				else if (ratio >= 2)
					power = 80;
				else
					power = 60;
			}
		}
			break;

		case MOVE_GYROBALL:	;
			if (!(data->specialFlags & FLAG_IGNORE_TARGET))
			{
				if (data->atkSpeed == 0)
					power = 1;
				else
					power = MathMin(150, (data->defSpeed * 25) / data->atkSpeed + 1);
			}
			break;

		case MOVE_PUNISHMENT:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET) && !useMonDef)
				power = MathMin(200, 60 + 20 * CountBoosts(bankDef));
			break;

		case MOVE_LOWKICK:
		case MOVE_GRASSKNOT:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET))
			{
				u32 weight = GetActualSpeciesWeight(data->defSpecies, data->defAbility, data->defItemEffect, bankDef, !useMonDef) / 10;

				if (weight >= 200)
					power = 120;
				else if (weight >= 100)
					power = 100;
				else if (weight >= 50)
					power = 80;
				else if (weight >= 25)
					power = 60;
				else if (weight >= 10)
					power = 40;
				else
					power = 20;
			}
			break;

		case MOVE_HEAVYSLAM:
		case MOVE_HEATCRASH:	;
			if (!(data->specialFlags & FLAG_IGNORE_TARGET))
			{
				u32 atkWeight, defWeight, weightRatio;

				defWeight = GetActualSpeciesWeight(data->defSpecies, data->defAbility, data->defItemEffect, bankDef, !useMonDef);
				atkWeight = GetActualSpeciesWeight(data->atkSpecies, data->atkAbility, data->atkItemEffect, bankAtk, !useMonAtk);
				weightRatio = atkWeight / defWeight;

				switch (weightRatio) {
					case 0:
					case 1:
						power = 40;
						break;
					case 2:
						power = 60;
						break;
					case 3:
						power = 80;
						break;
					case 4:
						power = 100;
						break;
					default:
						power = 120;
				}
			}
			break;

		case MOVE_POWERTRIP:
		case MOVE_STOREDPOWER:
			if (!useMonAtk)
				power = 20 * (1 + CountBoosts(bankAtk));
			break;

		case MOVE_FLING: ;
			power = GetFlingPower(data->atkItem, data->atkSpecies, data->atkAbility, bankAtk, useMonAtk);
			break;

		case MOVE_ERUPTION:
		case MOVE_WATERSPOUT:
			power = MathMax(1, (150 * data->atkHP) / data->atkMaxHP);
			break;

		case MOVE_REVERSAL:
		case MOVE_FLAIL:
		{
			u16 percentage = (48 * data->atkHP) / data->atkMaxHP;

			if (percentage <= 1)
				power = 200;
			else if (percentage <= 4)
				power = 150;
			else if (percentage <= 9)
				power = 100;
			else if (percentage <= 16)
				power = 80;
			else if (percentage <= 32)
				power = 40;
			else
				power = 20;
		}
			break;

		#ifdef SPECIES_ASHGRENINJA
		case MOVE_WATERSHURIKEN:
			if (data->atkSpecies == SPECIES_ASHGRENINJA && data->atkAbility == ABILITY_BATTLEBOND)
				power = 20;
			break;
		#endif

		case MOVE_CRUSHGRIP:
		case MOVE_WRINGOUT:
		case MOVE_DRAGONENERGY:
			if (!(data->specialFlags & FLAG_IGNORE_TARGET))
				power = MathMax(1, (data->defHP * 120) / data->defMaxHP);
			break;

		case MOVE_TRUMPCARD:
		{
			u8 index, pp;
			bool8 decrement = FALSE;

			//Load proper index of move
			if (useMonAtk)
			{
				index = FindMovePositionInMonMoveset(move, data->monAtk);
				pp = data->monAtk->box.pp[index];
				decrement = TRUE;
			}
			else if (data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC))
			{
				index = FindMovePositionInMoveset(move, bankAtk);
				pp = gBattleMons[bankAtk].pp[index];
				decrement = TRUE;
			}
			else
			{
				index = gBattleStruct->chosenMovePositions[bankAtk];
				pp = gBattleMons[bankAtk].pp[index];
			}

			if (decrement && pp > 0) //PP will be decremented at runtime so imitate this
			{
				pp -= 1; //One PP will be used on execution

				if (pp > 0 && data->defAbility == ABILITY_PRESSURE
				&& !(data->specialFlags & FLAG_IGNORE_TARGET))
					pp -= 1; //Decrement one more PP for Pressure usage
			}

			if (index < MAX_MON_MOVES)
			{
				switch (pp) {
					case 0:
						power = 200;
						break;
					case 1:
						power = 80;
						break;
					case 2:
						power = 60;
						break;
					case 3:
						power = 50;
						break;
					default:
						power = 40;
				}
			}
		}
			break;

		case MOVE_SPITUP:
			if (!useMonAtk)
				power = 100 * gDisableStructs[bankAtk].stockpileCounter; //Will be 0 if no stockpile
			break;

		case MOVE_NATURALGIFT:
			power = 0; //Would be at 1 before
			if (IsBerry(data->atkItem))
			{
				for (i = 0; gNaturalGiftTable[i].berry != ITEM_TABLES_TERMIN; ++i)
				{
					if (gNaturalGiftTable[i].berry == data->atkItem)
					{
						power = gNaturalGiftTable[i].power;
						break;
					}
				}
			}
			break;

		case MOVE_PURSUIT:
			if (data->specialFlags & FLAG_AI_CALC)
			{
				if (!useMonAtk
				&& !useMonDef
				&& IsPredictedToSwitch(bankDef, bankAtk))
					power *= 2;
			}
			else if (!(data->specialFlags & FLAG_CHECKING_FROM_MENU))
				power *= gBattleScripting.dmgMultiplier;
			break;

		case MOVE_RETURN:
			if ((gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER))
			|| IsFrontierRaidBattle())
				power = (10 * 255) / 25;
			else if (useMonAtk)
				power = (10 * (data->monAtk->box.friendship)) / 25;
			else
				power = (10 * (gBattleMons[bankAtk].friendship)) / 25;
			break;

		case MOVE_FRUSTRATION:
			if ((gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER))
			|| IsFrontierRaidBattle())
				power = (10 * 255) / 25;
			else if (useMonAtk)
				power = (10 * (255 - data->monAtk->box.friendship)) / 25;
			else
				power = (10 * (255 - gBattleMons[bankAtk].friendship)) / 25;
			break;

		case MOVE_BEATUP:
			if (useMonAtk || (data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC)))
				power = (gBaseStats[data->atkSpecies].baseAttack / 10) + 5;
			else
			{
				struct Pokemon* party;
				if (SIDE(bankAtk) == B_SIDE_PLAYER)
					party = gPlayerParty;
				else
					party = gEnemyParty;

				power = (gBaseStats[party[gBattleCommunication[0] - 1].box.species].baseAttack / 10) + 5;
			}
			break;

		case MOVE_HIDDENPOWER:
		#ifdef OLD_HIDDEN_POWER_BP
			if (useMonAtk)
			{
				power = ((data->monAtk->hpIV & 2) >> 1) |
						((data->monAtk->attackIV & 2)) |
						((data->monAtk->defenseIV & 2) << 1) |
						((data->monAtk->speedIV & 2) << 2) |
						((data->monAtk->spAttackIV & 2) << 3) |
						((data->monAtk->spDefenseIV & 2) << 4);
			}
			else
			{
				power = ((gBattleMons[bankAtk].hpIV & 2) >> 1) |
						((gBattleMons[bankAtk].attackIV & 2)) |
						((gBattleMons[bankAtk].defenseIV & 2) << 1) |
						((gBattleMons[bankAtk].speedIV & 2) << 2) |
						((gBattleMons[bankAtk].spAttackIV & 2) << 3) |
						((gBattleMons[bankAtk].spDefenseIV & 2) << 4);
			}

			power = ((40 * power) / 63) + 30;
		#endif
			break;

		case MOVE_MAGNITUDE:
		case MOVE_PRESENT:
			if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC)) && !useMonAtk)
				power = gDynamicBasePower;
			break;

		case MOVE_BOLTBEAK:
		case MOVE_FISHIOUSREND:
			if (BankMovedBeforeIgnoreSwitch(bankAtk, bankDef))
				power *= 2;
			break;

		case MOVE_GRAVAPPLE:
			if (IsGravityActive())
				power = (power * 15) / 10; //1.5x boost in Gravity
			break;

		case MOVE_TERRAINPULSE:
			if (gTerrainType && data->atkIsGrounded)
				power *= 2;
			break;

		case MOVE_RISINGVOLTAGE:
			if (gTerrainType == ELECTRIC_TERRAIN && data->defIsGrounded)
				power *= 2;
			break;

		case MOVE_EXPANDINGFORCE:
			if (gTerrainType == PSYCHIC_TERRAIN && data->atkIsGrounded)
				power = (power * 15) / 10;
			break;

		case MOVE_MISTYEXPLOSION:
			if (gTerrainType == MISTY_TERRAIN && data->atkIsGrounded)
				power = (power * 15) / 10;
			break;

		case MOVE_LASHOUT:
			if (gBattleStruct->statFellThisRound[bankAtk])
				power *= 2;
			break;

		default:
			if (gBattleMoves[move].effect == EFFECT_TRIPLE_KICK)
			{
				if (!(data->specialFlags & (FLAG_CHECKING_FROM_MENU | FLAG_AI_CALC)) && !useMonAtk)
					power = gBattleScripting.tripleKickPower;
			}
			break;
	}

	gBattleMovePower = power;
	return power;
}

static u16 AdjustBasePower(struct DamageCalc* data, u16 power)
{
	//Take variables off struct for easier access
	u8 bankAtk = data->bankAtk;
	u8 bankDef = data->bankDef;
	u16 move = data->move;

	bool8 useMonAtk = data->monAtk != NULL;
	bool8 useMonDef = data->monDef != NULL;

	if (data->specialFlags & (FLAG_CONFUSION_DAMAGE))
		return power;

	//Check attacker ability boost
	switch(data->atkAbility) {
		case ABILITY_TECHNICIAN:
		//1.5x Boost
			if (data->basePower <= 60)
				power = (power * 15) / 10;
			break;

		case ABILITY_RIVALRY:
		{
		//1.25x / 0.75x Boost
			u8 attackerGender, targetGender;
			if (useMonAtk)
				attackerGender = GetGenderFromSpeciesAndPersonality(data->atkSpecies, data->monAtk->box.personality);
			else
				attackerGender = GetGenderFromSpeciesAndPersonality(data->atkSpecies, gBattleMons[bankAtk].personality);

			if (useMonDef)
				targetGender = GetGenderFromSpeciesAndPersonality(data->defSpecies, data->monDef->box.personality);
			else
				targetGender = GetGenderFromSpeciesAndPersonality(data->defSpecies, gBattleMons[bankDef].personality);

			if (attackerGender != 0xFF && targetGender != 0xFF)
			{
				if (attackerGender == targetGender)
					power = (power * 125) / 100;
				else
					power = (power * 75) / 100;
			}
		}
			break;

		case ABILITY_RECKLESS:
		//1.2x Boost
			if (CheckTableForMove(move, gRecklessBoostedMoves))
				power = (power * 12) / 10;
			break;

		case ABILITY_IRONFIST:
		//1.2x Boost
			if (CheckTableForMove(move, gPunchingMoves))
				power = (power * 12) / 10;
			break;

		case ABILITY_TOXICBOOST:
		//1.5x Boost
			if (data->atkStatus1 & STATUS1_PSN_ANY && data->moveSplit == SPLIT_PHYSICAL)
				power = (power * 15) / 10;
			break;

		case ABILITY_FLAREBOOST:
		//1.5x Boost
			if (data->atkStatus1 & STATUS1_BURN && data->moveSplit == SPLIT_SPECIAL)
				power = (power * 15) / 10;
			break;

		case ABILITY_SANDFORCE:
		//1.3x Boost
			if (gBattleWeather & WEATHER_SANDSTORM_ANY
			&& (data->moveType == TYPE_ROCK || data->moveType == TYPE_GROUND || data->moveType == TYPE_STEEL))
				power = (power * 13) / 10;
			break;

		case ABILITY_SHEERFORCE:
		//1.3x Boost
			if (CheckTableForMove(move, gSheerForceBoostedMoves))
				power = (power * 13) / 10;
			break;

		case ABILITY_AERILATE:
		case ABILITY_PIXILATE:
		case ABILITY_REFRIGERATE:
		case ABILITY_GALVANIZE:
		case ABILITY_NORMALIZE:
		//1.2x / 1.3x Boost
			if ((!useMonAtk && AbilityCanChangeTypeAndBoost(move, data->atkAbility, gBattleStruct->ElectrifyTimers[bankAtk], TRUE, (gBattleStruct->zMoveData.active || gBattleStruct->zMoveData.viewing)))
			||   (useMonAtk && AbilityCanChangeTypeAndBoost(move, data->atkAbility, 0, FALSE, FALSE)))
			{
				#ifdef OLD_ATE_BOOST
					power = (power * 13) / 10;
				#else
					power = (power * 12) / 10;
				#endif
			}
			break;

		case ABILITY_MEGALAUNCHER:
		//1.5x Boost
			if (CheckTableForMove(move, gPulseAuraMoves))
				power = (power * 15) / 10;
			break;

		case ABILITY_STRONGJAW:
		//1.5x Boost
			if (CheckTableForMove(move, gBitingMoves))
				power = (power * 15) / 10;
			break;

		case ABILITY_TOUGHCLAWS:
		//1.3x Boost
			if (gBattleMoves[move].flags & FLAG_MAKES_CONTACT
			&& data->atkItemEffect != HOLD_EFFECT_PROTECTIVE_PADS)
				power = (power * 13) / 10;
			break;

		case ABILITY_NEUROFORCE:
		//1.25x Boost
			if (data->resultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
				power = (power * 125) / 100;
			break;

		case ABILITY_STAKEOUT:
		//2x Boost
			if (!useMonAtk && gBattleStruct->StakeoutCounters[bankDef])
				power *= 2;
			break;

		case ABILITY_STEELWORKER:
		case ABILITY_STEELY_SPIRIT:
		//1.5x Boost
			if (data->moveType == TYPE_STEEL)
				power = (power * 15) / 10;
			break;

		case ABILITY_WATERBUBBLE:
		//2x Boost
			if (data->moveType == TYPE_WATER)
				power *= 2; //2x Boost
			break;

		case ABILITY_ANALYTIC:
		//1.3x Boost
			if (BankMovedBefore(bankDef, bankAtk))
				power = (power * 13) / 10;
			break;

		case ABILITY_PUNKROCK:
		//1.3x Boost
			if (CheckSoundMove(move))
				power = (power * 13) / 10;
			break;
	}

	//Check attacker partner ability boost
	switch (data->atkPartnerAbility) {
		case ABILITY_BATTERY:
		//1.3x Boost
			if (data->moveSplit == SPLIT_SPECIAL)
				power = (power * 13) / 10;
			break;

		case ABILITY_STEELY_SPIRIT:
		//1.5x Boost
			if (data->moveType == TYPE_STEEL)
				power = (power * 15) / 10;
			break;

		case ABILITY_POWERSPOT:
		//1.3x Boost
			power = (power * 13) / 10;
			break;
	}

	//Check attacker item effect boost
	switch (data->atkItemEffect) {
		case HOLD_EFFECT_MUSCLE_BAND:
			if (data->moveSplit == SPLIT_PHYSICAL)
				power = (power * 11) / 10;
			break;

		case HOLD_EFFECT_WISE_GLASSES:
			if (data->moveSplit == SPLIT_SPECIAL)
				power = (power * 11) / 10;
			break;

		case HOLD_EFFECT_PLATE:
		case HOLD_EFFECT_TYPE_BOOSTERS:
		//1.2x Boost
			if (data->moveType == data->atkItemQuality)
				power = (power * 12) / 10;
			break;

		#ifdef SPECIES_DIALGA
		case HOLD_EFFECT_ADAMANT_ORB:
		//1.2x Boost
			if (data->atkSpecies == SPECIES_DIALGA && (data->moveType == TYPE_STEEL || data->moveType == TYPE_DRAGON))
				power = (power * 12) / 10;
			break;
		#endif

		#ifdef SPECIES_PALKIA
		case HOLD_EFFECT_LUSTROUS_ORB:
		//1.2x Boost
			if (data->atkSpecies == SPECIES_PALKIA && (data->moveType == TYPE_WATER || data->moveType == TYPE_DRAGON))
				power = (power * 12) / 10;
			break;
		#endif

		#if (defined SPECIES_GIRATINA && defined SPECIES_GIRATINA_ORIGIN)
		case HOLD_EFFECT_GRISEOUS_ORB:
		//1.2x Boost
			if ((data->atkSpecies == SPECIES_GIRATINA || data->atkSpecies == SPECIES_GIRATINA_ORIGIN)
			&& (data->moveType == TYPE_GHOST || data->moveType == TYPE_DRAGON))
				power = (power * 12) / 10;
			break;
		#endif

		#if (!(defined OLD_SOUL_DEW_EFFECT) && defined SPECIES_LATIOS && defined SPECIES_LATIAS)
		case HOLD_EFFECT_SOUL_DEW:
		//1.2x Boost
			if ((data->atkSpecies == SPECIES_LATIOS || data->atkSpecies == SPECIES_LATIAS)
			&& (data->moveType == TYPE_PSYCHIC || data->moveType == TYPE_DRAGON))
				power = (power * 12) / 10;
			break;
		#endif

		case HOLD_EFFECT_GEM: //This check is specifically meant for the AI, as the Gem would usually be consumed by now
							  //If the gBattleStruct->GemHelper is active and the user still has a gem, that means it received another through Symbiosis which is ignored
		//1.3x / 1.5x Boost
			if (data->moveType == data->atkItemQuality && !gBattleStruct->GemHelper)
			{
				#ifdef OLD_GEM_BOOST
					power = (power * 15) / 10;
				#else
					power = (power * 13) / 10;
				#endif
			}
			break;

		case HOLD_EFFECT_METRONOME:
			//1.2x - 2.0x Boost
			if (!useMonAtk)
			{
				u16 boost = MathMin(200, 100 + gBattleStruct->MetronomeCounter[bankAtk]);
				power = (power * boost) / 100;
			}
			break;

		case HOLD_EFFECT_LIFE_ORB:
			//1.3x Boost
			power = (power * 13) / 10;
			break;
	}

	//Gem activated at runtime
	if (gBattleStruct->GemHelper)
	{
		//1.3x / 1.5x Boost
		#ifdef OLD_GEM_BOOST
			power = (power * 15) / 10;
		#else
			power = (power * 13) / 10;
		#endif
	}

	//Charge check - 2x Boost
	if (data->atkStatus3 & STATUS3_CHARGED_UP && data->moveType == TYPE_ELECTRIC)
		power *= 2;

	#ifdef OLD_TERRAIN_BOOST
		#define TERRAIN_BOOST 15 //1.5x in Gen 6 & 7
	#else
		#define TERRAIN_BOOST 13 //1.3x in Gen 8
	#endif

	//Terrain Checks
	switch (gTerrainType) {
		case ELECTRIC_TERRAIN:
		//1.5x Boost
			if (data->atkIsGrounded && data->moveType == TYPE_ELECTRIC)
				power = (power * TERRAIN_BOOST) / 10;
			break;

		case GRASSY_TERRAIN:
		//1.5x / 0.5 Boost
			if (data->atkIsGrounded && data->moveType == TYPE_GRASS)
				power = (power * TERRAIN_BOOST) / 10;

			if ((move == MOVE_MAGNITUDE || move == MOVE_EARTHQUAKE || move == MOVE_BULLDOZE)
			&& !(data->defStatus3 & STATUS3_SEMI_INVULNERABLE))
				power /= 2;
			break;

		case MISTY_TERRAIN:
		//0.5x Boost
			if (data->defIsGrounded && data->moveType == TYPE_DRAGON)
				power /= 2;
			break;

		case PSYCHIC_TERRAIN:
		//1.5x Boost
			if (data->atkIsGrounded && data->moveType == TYPE_PSYCHIC)
				power = (power * TERRAIN_BOOST) / 10;
			break;
	}

	//Sport Checks
	switch (data->moveType) {
		case TYPE_FIRE:
			if (IsWaterSportActive())
				power /= 3;
			break;
		case TYPE_ELECTRIC:
			if (IsMudSportActive())
				power /= 3;
			break;
	}

	return power;
}

static u16 GetZMovePower(u16 zMove)
{
	u16 power = 1;

	switch (zMove) {
		case MOVE_CATASTROPIKA:
		case MOVE_10000000_VOLT_THUNDERBOLT:
		case MOVE_STOKED_SPARKSURFER:
		case MOVE_EXTREME_EVOBOOST:
		case MOVE_PULVERIZING_PANCAKE:
		case MOVE_GENESIS_SUPERNOVA:
		case MOVE_SINISTER_ARROW_RAID:
		case MOVE_MALICIOUS_MOONSAULT:
		case MOVE_OCEANIC_OPERETTA:
		case MOVE_SPLINTERED_STORMSHARDS:
		case MOVE_LETS_SNUGGLE_FOREVER:
		case MOVE_CLANGOROUS_SOULBLAZE:
		case MOVE_GUARDIAN_OF_ALOLA:
		case MOVE_SEARING_SUNRAZE_SMASH:
		case MOVE_MENACING_MOONRAZE_MAELSTROM:
		case MOVE_LIGHT_THAT_BURNS_THE_SKY:
		case MOVE_SOUL_STEALING_7_STAR_STRIKE:
			power = gBattleMoves[zMove].power;
			break;
		default:
			//An issue arose in the case of the AI calculating Z-Move power. To fix this,
			//gBattleStruct->ai.zMoveHelper stores the original move to helper turn it into a Z-Move.
			power = gBattleMoves[gBattleStruct->ai.zMoveHelper].z_move_power;
	}

	return power;
}

//Requires that the base move be loaded into gBattleStruct->ai.zMoveHelper
static u16 GetMaxMovePower(void)
{
	return gDynamaxMovePowers[gBattleStruct->ai.zMoveHelper];
}

void PopulateDamageCalcStructWithBaseAttackerData(struct DamageCalc* data)
{
	u8 bankAtk = data->bankAtk;
	bool8 useMonAtk = data->monAtk != NULL;

	if (useMonAtk)
	{
		struct Pokemon* monAtk = data->monAtk;

		data->atkSpecies = monAtk->box.species;
		data->atkAbility = GetMonAbility(monAtk);
		data->atkPartnerAbility = ABILITY_NONE;
		data->atkItemEffect = GetMonItemEffect(monAtk);
		data->atkItem = monAtk->box.heldItem;
		data->atkItemQuality = ItemId_GetHoldEffectParam(monAtk->box.heldItem);
		data->atkHP = monAtk->hp;
		data->atkMaxHP = monAtk->maxHP;
		data->atkSpeed = SpeedCalcMon(SIDE(bankAtk), monAtk);
		data->atkStatus1 = monAtk->status;
		data->atkStatus3 = 0;
		data->atkIsGrounded = CheckGroundingFromPartyData(monAtk);
	}
	else //Load from bank
	{
		data->atkSpecies = SPECIES(bankAtk);
		data->atkAbility = ABILITY(bankAtk);
		if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankAtk)))
			data->atkPartnerAbility = ABILITY(PARTNER(bankAtk));
		else
			data->atkPartnerAbility = 0;
		data->atkItem = ITEM(bankAtk);
		data->atkItemEffect = ITEM_EFFECT(bankAtk);
		data->atkItemQuality = ITEM_QUALITY(bankAtk);

		data->atkHP = gBattleMons[bankAtk].hp;
		data->atkMaxHP = gBattleMons[bankAtk].maxHP;
		data->atkSpeed = SpeedCalc(bankAtk);
		data->atkStatus1 = gBattleMons[bankAtk].status1;
		data->atkStatus3 = gStatuses3[bankAtk];
		data->atkIsGrounded = CheckGrounding(bankAtk);
	}
	
	data->attackerLoaded = TRUE;
}

void PopulateDamageCalcStructWithBaseDefenderData(struct DamageCalc* data)
{
	bool8 useMonDef = data->monDef != NULL;
	u8 bankDef = data->bankDef;

	if (useMonDef)
	{
		struct Pokemon* monDef = data->monDef;

		data->defSpecies = monDef->box.species;
		data->defAbility = GetMonAbility(monDef);
		data->defPartnerAbility = ABILITY_NONE;
		data->defItemEffect = GetMonItemEffect(monDef);
		data->defItemQuality = ItemId_GetHoldEffectParam(monDef->box.heldItem);
		data->defHP = monDef->hp;
		data->defMaxHP = monDef->maxHP;
		data->defSpeed = SpeedCalcMon(SIDE(bankDef), monDef);
		data->defStatus1 = monDef->status;
		data->defStatus3 = 0;
		data->defSideStatus = gSideStatuses[SIDE(bankDef)];
		data->defIsGrounded = CheckGroundingFromPartyData(monDef);

		data->defBuff = 0;
		data->spDefBuff = 0;

		if (IsWonderRoomActive())
		{
			data->defense = monDef->spDefense;
			data->spDefense = monDef->defense;
		}
		else
		{
			data->defense = monDef->defense;
			data->spDefense = monDef->spDefense;
		}
	}
	else //Load from bank
	{
		data->defSpecies = SPECIES(bankDef);
		data->defAbility = ABILITY(bankDef);
		if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(PARTNER(bankDef)))
			data->defPartnerAbility = ABILITY(PARTNER(bankDef));
		else
			data->defPartnerAbility = 0;
		data->defItemEffect = ITEM_EFFECT(bankDef);
		data->defItemQuality = ITEM_QUALITY(bankDef);
		data->defHP = gBattleMons[bankDef].hp;
		data->defMaxHP = gBattleMons[bankDef].maxHP;
		data->defSpeed = SpeedCalc(bankDef);
		data->defStatus1 = gBattleMons[bankDef].status1;
		data->defStatus3 = gStatuses3[bankDef];
		data->defSideStatus = gSideStatuses[SIDE(bankDef)];
		data->defIsGrounded = CheckGrounding(bankDef);

		data->defBuff = STAT_STAGE(bankDef, STAT_STAGE_DEF);
		data->spDefBuff = STAT_STAGE(bankDef, STAT_STAGE_SPDEF);

		if (IsWonderRoomActive())
		{
			data->defense = gBattleMons[bankDef].spDefense;
			data->spDefense = gBattleMons[bankDef].defense;
		}
		else
		{
			data->defense = gBattleMons[bankDef].defense;
			data->spDefense = gBattleMons[bankDef].spDefense;
		}
	}

	data->defenderLoaded = TRUE;
}

u32 GetActualSpeciesWeight(u16 species, ability_t ability, u8 itemEffect, u8 bank, bool8 checkNimble)
{
	u32 weight = TryGetAlternateSpeciesSize(species, 1); //Eg. Mega Form
	if (weight == 0)
		weight = GetPokedexHeightWeight(SpeciesToNationalPokedexNum(species), 1);

	return AdjustWeight(weight, ability, itemEffect, bank, checkNimble);
}

static u32 AdjustWeight(u32 weight, ability_t ability, u8 item_effect, u8 bank, bool8 check_nimble)
{
	int i;

	switch (ability) {
		case ABILITY_LIGHTMETAL:
			weight /= 2;
			break;
		case ABILITY_HEAVYMETAL:
			weight *= 2;
	}

	if (item_effect == HOLD_EFFECT_FLOAT_STONE)
		weight /= 2;

	if (check_nimble) {
		for (i = 0; i < gBattleStruct->NimbleCounters[bank]; ++i) {
			if (weight > 2200)
				weight -= 2200;
			else {
				weight = 1;
				break;
			}
		}
	}
	return weight;
}

u16 TryGetAlternateSpeciesSize(u16 species, u8 type)
{
	int i;
	for (i = 0; gAlternateSpeciesSizeTable[i].species != SPECIES_TABLES_TERMIN; ++i)
	{
		if (gAlternateSpeciesSizeTable[i].species == species)
		{
			switch (type) {
				case 0:
					return gAlternateSpeciesSizeTable[i].height;

				case 1:
					return gAlternateSpeciesSizeTable[i].weight;
			}
		}
	}

	return 0;
}

u8 AI_SpecialTypeCalc(u16 move, u8 bankAtk, u8 bankDef)
{
	u8 moveType;
	ability_t atkAbility = GetAIAbility(bankAtk, bankDef, move);
	ability_t defAbility = GetAIAbility(bankDef, bankAtk, IsValidMovePrediction(bankDef, bankAtk));
	u8 defEffect = ITEM_EFFECT(bankDef);
	u8 atkType1, atkType2, atkType3, defType1, defType2, defType3;
	u8 flags = 0;

	if (move == MOVE_STRUGGLE)
		return 0;

	atkType1 = gBattleMons[bankAtk].type1;
	atkType2 = gBattleMons[bankAtk].type2;
	atkType3 = gBattleMons[bankAtk].type3;
	moveType = GetMoveTypeSpecial(bankAtk, move);

	if (atkAbility == ABILITY_PROTEAN)
		atkType1 = atkType2 = atkType3 = moveType;

	if (gStatuses3[bankDef] & STATUS3_ILLUSION && gDisableStructs[bankDef].isFirstTurn) //Under illusion and haven't figured it out yet
	{
		struct Pokemon* illusionMon = GetIllusionPartyData(bankDef);
		u16 fakeSpecies = GetMonData(illusionMon, MON_DATA_SPECIES, NULL);
		defAbility = GetMonAbility(illusionMon);
		defType1 = gBaseStats[fakeSpecies].type1;
		defType2 = gBaseStats[fakeSpecies].type2;
	}
	else
	{
		defType1 = gBattleMons[bankDef].type1;
		defType2 = gBattleMons[bankDef].type2;
	}
	defType3 = gBattleMons[bankDef].type3; //Same type 3 - eg switched in on Forest's curse

	//Check STAB
	if (atkType1 == moveType || atkType2 == moveType || atkType3 == moveType)
	{
		if (atkAbility == ABILITY_ADAPTABILITY)
			gBattleMoveDamage *= 2;
		else
			gBattleMoveDamage = (gBattleMoveDamage * 150) / 100;
	}

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND
	&& !CheckGrounding(bankDef)
	&& ((defAbility == ABILITY_LEVITATE && NO_MOLD_BREAKERS(atkAbility, move)) || defEffect == HOLD_EFFECT_AIR_BALLOON || (gStatuses3[bankDef] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS)))
	&& move != MOVE_THOUSANDARROWS)
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if (CheckTableForMove(move, gPowderMoves) && !IsAffectedByPowderByDetails(defType1, defType2, defType3, defAbility, defEffect))
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if (move == MOVE_SKYDROP && (defType1 == TYPE_FLYING || defType2 == TYPE_FLYING || defType3 == TYPE_FLYING))
	{
		flags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else //Regular Type Calc
		TypeDamageModificationByDefTypes(atkAbility, bankDef, move, moveType, &flags, defType1, defType2, defType3);

	//Wonder Guard Check
	if (defAbility == ABILITY_WONDER_GUARD
	&& NO_MOLD_BREAKERS(atkAbility, move)
	&& !(flags & MOVE_RESULT_MISSED)
	&& AttacksThisTurn(bankAtk, move) == 2
	&& (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[move].power
	&& SPLIT(move) != SPLIT_STATUS)
	{
		flags |= MOVE_RESULT_MISSED;
	}

	if (flags & MOVE_RESULT_NO_EFFECT)
		gBattleMoveDamage = 0;

	return flags;
}

u32 NewAI_CalcDmg(const u8 bankAtk, const u8 bankDef, const u16 move, struct DamageCalc* damageData)
{
	u8 resultFlags = AI_SpecialTypeCalc(move, bankAtk, bankDef);
	u32 damage = 0;
	struct DamageCalc data = {0};

	if (gBattleMoves[move].effect != EFFECT_PAIN_SPLIT
	&& (SPLIT(move) == SPLIT_STATUS || resultFlags & MOVE_RESULT_NO_EFFECT))
		return 0;

	switch (gBattleMoves[move].effect) {
		case EFFECT_SUPER_FANG:
			return GetBaseCurrentHP(bankDef) / 2; //50 % of base HP
		case EFFECT_DRAGON_RAGE:
			return 40;
		case EFFECT_SONICBOOM:
			return 20;
		case EFFECT_LEVEL_DAMAGE:
			return gBattleMons[bankAtk].level;
		case EFFECT_PSYWAVE:
			return GetPsywaveDamage(50); //On average, 50 will be selected as the random number
		case EFFECT_MEMENTO: //Final Gambit
			return gBattleMons[bankAtk].hp;
		case EFFECT_ENDEAVOR:
		{
			s32 dmg;
			dmg = GetBaseCurrentHP(bankDef) - GetBaseCurrentHP(bankAtk);
			if (dmg <= 0)
				return 0;
			return dmg;
		}
		case EFFECT_PAIN_SPLIT:
		{
			u16 finalHp = (GetBaseCurrentHP(bankAtk) + GetBaseCurrentHP(bankDef)) / 2;

			if (finalHp >= GetBaseCurrentHP(bankDef))
				return 0;
			return GetBaseCurrentHP(bankDef) - finalHp;
		}
	}

	if (SPLIT(move) == SPLIT_STATUS) //At this point we don't care about Status moves anymore
		return 0;

	gBattleScripting.dmgMultiplier = 1;

	gCritMultiplier = CalcPossibleCritChance(bankAtk, bankDef, move, NULL, NULL); //Return 0 if none, 1 if always, 2 if 50%
	if (gCritMultiplier != 0 && Random() % gCritMultiplier == 0)
		gCritMultiplier = CRIT_MULTIPLIER;
	else
		gCritMultiplier = BASE_CRIT_MULTIPLIER;

	if (damageData == NULL)
		damageData = &data;

	damageData->bankAtk = bankAtk;
	damageData->bankDef = bankDef;
	damageData->move = move;
	damageData->specialFlags |= FLAG_AI_CALC;
	damage = CalculateBaseDamage(damageData);

	gBattleMoveDamage = MathMin(0x7FFFFFFF, damage);
	AI_SpecialTypeCalc(move, bankAtk, bankDef);
	damage = (gBattleMoveDamage * gCritMultiplier) / BASE_CRIT_MULTIPLIER;
	gCritMultiplier = BASE_CRIT_MULTIPLIER; //Reset

	damage = (damage * 93) / 100; //Roll 93% damage - about halfway between min & max damage

	if (CheckTableForMove(move, gTwoToFiveStrikesMoves) && ABILITY(bankAtk) == ABILITY_SKILLLINK)
	{
		damage *= 5;
		return damage;
	}
	else if (CheckTableForMove(move, gTwoToFiveStrikesMoves) || gBattleMoves[move].effect == EFFECT_TRIPLE_KICK) //Three hits on average
	{
		damage *= 3;
		return damage;
	}
	else if (CheckTableForMove(move, gTwoStrikesMoves))
	{
		damage *= 2;
		return damage;
	}
	else if (ABILITY(bankAtk) == ABILITY_PARENTALBOND && IsMoveAffectedByParentalBond(move, bankAtk))
	{
		#ifdef OLD_PARENTAL_BOND_DAMAGE
			damage = (damage * 150) / 100; //1.5x overall boost
		#else
			damage = (damage * 125) / 100; //1.25x overall boost
		#endif
		return damage;
	}

	//Multi hit moves skip these checks
	if (gBattleMoves[move].effect == EFFECT_FALSE_SWIPE
	|| (BATTLER_MAX_HP(bankDef) && ABILITY(bankDef) == ABILITY_STURDY && NO_MOLD_BREAKERS(ABILITY(bankAtk), move))
	|| (BATTLER_MAX_HP(bankDef) && IsBankHoldingFocusSash(bankDef)))
		damage = MathMin(damage, gBattleMons[bankDef].hp - 1);

	return damage;
}

//The function allows the AI to do type calculations from a move onto one of their partied mons
u8 NewAI_TypeCalc(u16 move, u8 bankAtk, struct Pokemon* monDef) 
{
	u8 flags = 0;

	ability_t defAbility = GetMonAbility(monDef);
	u8 defEffect = ItemId_GetHoldEffectParam(monDef->box.heldItem);
	u8 defType1 = GetMonType(monDef, 0);
	u8 defType2 = GetMonType(monDef, 1);

	ability_t atkAbility = ABILITY(bankAtk);
	u8 atkType1 = gBattleMons[bankAtk].type1;
	u8 atkType2 = gBattleMons[bankAtk].type2;
	u8 atkType3 = gBattleMons[bankAtk].type3;
	u8 moveType;

	if (move == MOVE_STRUGGLE)
		return 0;

	moveType = GetMoveTypeSpecial(bankAtk, move);

	//Check stab
	if (atkType1 == moveType || atkType2 == moveType || atkType3 == moveType)
	{
		if (atkAbility == ABILITY_ADAPTABILITY)
			gBattleMoveDamage *= 2;
		else
			gBattleMoveDamage = (gBattleMoveDamage * 150 / 100);
	}

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND
	&& !CheckGroundingFromPartyData(monDef)
	&& ((defAbility == ABILITY_LEVITATE && NO_MOLD_BREAKERS(atkAbility, move)) || (defEffect == HOLD_EFFECT_AIR_BALLOON && defAbility != ABILITY_KLUTZ))
	&& move != MOVE_THOUSANDARROWS)
	{
		flags = MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE;
	}
	else if (CheckTableForMove(move, gPowderMoves)
	&& (defAbility == ABILITY_OVERCOAT || defEffect == HOLD_EFFECT_SAFETY_GOGGLES || defType1 == TYPE_GRASS || defType2 == TYPE_GRASS))
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if (move == MOVE_SKYDROP && (defType1 == TYPE_FLYING || defType2 == TYPE_FLYING))
	{
		flags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else
	{
		TypeDamageModificationPartyMon(atkAbility, monDef, move, moveType, &flags);
	}

	if (defAbility == ABILITY_WONDER_GUARD
	&& (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[move].power)
	{
		flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	}

	return flags;
}

//The TypeCalc for showing move effectiveness on the move menu
u8 VisualTypeCalc(u16 move, u8 bankAtk, u8 bankDef)
{
	u8 moveType;
	ability_t defAbility = GetRecordedAbility(bankDef);
	u8 defEffect = GetRecordedItemEffect(bankDef);
	ability_t atkAbility;
	u8 defType1, defType2, defType3;
	u8 flags = 0;
	struct Pokemon* monIllusion;

	if (move == MOVE_STRUGGLE)
		return 0;

	atkAbility = ABILITY(bankAtk);
	moveType = GetMoveTypeSpecial(bankAtk, move);

	monIllusion = GetIllusionPartyData(bankDef);
	if (monIllusion != GetBankPartyData(bankDef)) //Under illusion
	{
		defType1 = GetMonType(monIllusion, 0);
		defType2 = GetMonType(monIllusion, 1);
		defType3 = TYPE_BLANK;
	}
	else
	{
		defType1 = gBattleMons[bankDef].type1;
		defType2 = gBattleMons[bankDef].type2;
		defType3 = gBattleMons[bankDef].type3;
	}

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND
	&& !NonInvasiveCheckGrounding(bankDef)
	&& ((defAbility == ABILITY_LEVITATE && NO_MOLD_BREAKERS(atkAbility, move)) || defEffect == HOLD_EFFECT_AIR_BALLOON || (gStatuses3[bankDef] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS)))
	&& move != MOVE_THOUSANDARROWS)
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if (CheckTableForMove(move, gPowderMoves) && !IsAffectedByPowderByDetails(defType1, defType2, defType3, defAbility, defEffect))
	{
		flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else if (move == MOVE_SKYDROP && (defType1 == TYPE_FLYING || defType2 == TYPE_FLYING || defType3 == TYPE_FLYING))
	{
		flags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
	}
	else //Regular Type Calc
		TypeDamageModificationByDefTypes(atkAbility, bankDef, move, moveType, &flags, defType1, defType2, defType3);

	//Wonder Guard Check
	if (defAbility == ABILITY_WONDER_GUARD
	&& NO_MOLD_BREAKERS(atkAbility, move)
	&& !(flags & MOVE_RESULT_MISSED)
	&& (!(flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[move].power
	&& SPLIT(move) != SPLIT_STATUS)
		flags |= MOVE_RESULT_MISSED;

	return flags;
}

void TypeDamageModificationPartyMon(ability_t atkAbility, struct Pokemon* monDef, u16 move, u8 moveType, u8* flags)
{
	u8 defType1, defType2, multiplier1, multiplier2;

	defType1 = GetMonType(monDef, 0);
	defType2 = GetMonType(monDef, 1);

TYPE_LOOP_AI:
	multiplier1 = gTypeEffectiveness_2[moveType][defType1];
	multiplier2 = gTypeEffectiveness_2[moveType][defType2];

	ModulateDmgByType(multiplier1, move, moveType, defType1, 0, atkAbility, flags, monDef, TRUE);

	if (defType1 != defType2)
		ModulateDmgByType(multiplier2, move, moveType, defType2, 0, atkAbility, flags, monDef, TRUE);

	if (move == MOVE_FLYINGPRESS && moveType != TYPE_FLYING)
	{
		moveType = TYPE_FLYING;
		goto TYPE_LOOP_AI;
	}
}

u16 CalcVisualBasePower(u8 bankAtk, u8 bankDef, u16 move, bool8 ignoreDef)
{
	struct DamageCalc data = {0};
	u16 power = 0;

	data.bankAtk = bankAtk;
	data.bankDef = bankDef;
	data.move = move;
	data.specialFlags = FLAG_CHECKING_FROM_MENU;
	
//Load attacker Data
	PopulateDamageCalcStructWithBaseAttackerData(&data);
	data.moveSplit = CalcMoveSplit(bankAtk, move);
	data.moveType = GetMoveTypeSpecial(bankAtk, move);

//Load target data
	if (ignoreDef)
		data.specialFlags |= FLAG_IGNORE_TARGET;
	else
		PopulateDamageCalcStructWithBaseDefenderData(&data);

	data.resultFlags = NewTypeCalc(move, bankAtk, bankDef, NULL, FALSE);

//Load correct move power
	data.basePower = gBattleMoves[move].power;
	power = GetBasePower(&data);
	power = AdjustBasePower(&data, power);

	//Not really a base power thing, but stick them in anayways
	switch (move) {
		case MOVE_SOLARBEAM:
		case MOVE_SOLARBLADE:
			if (WEATHER_HAS_EFFECT
			&& data.atkItemEffect != HOLD_EFFECT_UTILITY_UMBRELLA
			&& gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_SANDSTORM_ANY | WEATHER_HAIL_ANY | WEATHER_FOG_ANY | WEATHER_AIR_CURRENT_PRIMAL))
				power /= 2; //Any weather except sun weakens Solar Beam
			break;
		case MOVE_DYNAMAXCANNON:
		case MOVE_BEHEMOTHBLADE:
		case MOVE_BEHEMOTHBASH:
			if (IsDynamaxed(bankDef))
				power *= 2;
			break;
	}

	return power;
}

static u32 BoostSpeedInWeather(ability_t ability, u8 itemEffect, u32 speed)
{
	if (WEATHER_HAS_EFFECT) {
		switch (ability) {
			case ABILITY_SWIFT_SWIM:
				if (gBattleWeather & WEATHER_RAIN_ANY && itemEffect != HOLD_EFFECT_UTILITY_UMBRELLA)
					speed *= 2;
				break;
			case ABILITY_CHLOROPHYLL:
				if (gBattleWeather & WEATHER_SUN_ANY && itemEffect != HOLD_EFFECT_UTILITY_UMBRELLA)
					speed *= 2;
				break;
			case ABILITY_SANDRUSH:
				if (gBattleWeather & WEATHER_SANDSTORM_ANY)
					speed *= 2;
				break;
			case ABILITY_SLUSHRUSH:
				if (gBattleWeather & WEATHER_HAIL_ANY)
					speed *= 2;
				break;
		}
	}

	return speed;
}

static u32 BoostSpeedByItemEffect(u8 itemEffect, u8 itemQuality, u16 species, u32 speed, bool8 isDynamaxed)
{
	switch (itemEffect) {
		case HOLD_EFFECT_MACHO_BRACE:
			speed /= 2;
			break;
		case HOLD_EFFECT_CHOICE_BAND:
			if (!isDynamaxed && itemQuality == QUALITY_CHOICE_SCARF)
				speed = (speed * 15) / 10;
			break;
		case HOLD_EFFECT_IRON_BALL:
			speed /= 2;
			break;
		case HOLD_EFFECT_QUICK_POWDER:
			if (species == SPECIES_DITTO)
				speed *= 2;
	}

	return speed;
}

u32 SpeedCalc(u8 bank)
{
	u32 speed;
	ability_t ability;
	u8 itemEffect, itemQuality;

	if (!BATTLER_ALIVE(bank))
		return 0;

	ability = ABILITY(bank);
	itemEffect = ITEM_EFFECT(bank);
	itemQuality = ITEM_QUALITY(bank);

	//Calculate adjusted speed stat
	speed = (gBattleMons[bank].speed * gStatStageRatios[gBattleMons[bank].statStages[STAT_STAGE_SPEED-1]][0]) / gStatStageRatios[gBattleMons[bank].statStages[STAT_STAGE_SPEED-1]][1];

	//Check for abilities that alter speed
	speed = BoostSpeedInWeather(ability, itemEffect, speed);

	switch (ability) {
		case ABILITY_UNBURDEN:
			if (gBattleStruct->UnburdenBoosts & gBitTable[bank] && ITEM(bank) == ITEM_NONE)
				speed *= 2;
			break;
		case ABILITY_SLOWSTART:
			if (gBattleStruct->SlowStartTimers[bank])
				speed /= 2;
			break;
		case ABILITY_SURGESURFER:
			if (gTerrainType == ELECTRIC_TERRAIN)
				speed *= 2;
			break;
	}

	speed = BoostSpeedByItemEffect(itemEffect, itemQuality, SPECIES(bank), speed, IsDynamaxed(bank));

	if (gBattleStruct->TailwindTimers[SIDE(bank)])
		speed *= 2;
	if (BankSideHasSwamp(bank))
		speed /= 4;

	#ifdef BADGE_BOOSTS
		if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER))
		&& FlagGet(FLAG_BADGE03_GET)
		&& gBattleTypeFlags & BATTLE_TYPE_TRAINER
		&& SIDE(bank) == B_SIDE_PLAYER
		&& gTrainerBattleOpponent_A != 0x400)
			speed = (speed * 110) / 100;
	#endif

	if (gBattleMons[bank].status1 & STATUS1_ANY && ability == ABILITY_QUICKFEET)
		speed *= 2;
	else if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
	{
		#ifndef OLD_PARALYSIS_SPD_DROP
			speed /= 2;
		#else
			speed /= 4;
		#endif
	}

	return speed;
}


u32 SpeedCalcMon(u8 side, struct Pokemon* mon)
{
	u8 itemEffect;
	ability_t ability;
	u32 speed;
	u8 itemQuality;

	if (GetMonData(mon, MON_DATA_IS_EGG, NULL))
		return 0;

	ability = GetMonAbility(mon);
	speed = mon->speed;

	if (ability != ABILITY_KLUTZ)
		itemEffect = ItemId_GetHoldEffect(mon->box.heldItem);
	else
		itemEffect = 0;

	itemQuality = ItemId_GetHoldEffectParam(mon->box.heldItem);

	//Check for abilities that alter speed
	speed = BoostSpeedInWeather(ability, itemEffect, speed);

	switch (ability) {
		case ABILITY_SLOWSTART:
			speed /= 2;
			break;
		case ABILITY_SURGESURFER:
			if (gTerrainType == ELECTRIC_TERRAIN)
				speed *= 2;
			break;
	}

	//Check for items that alter speed
	speed = BoostSpeedByItemEffect(itemEffect, itemQuality, mon->box.species, speed, FALSE);

	//Check other things that alter speed
	if (gBattleStruct->TailwindTimers[side])
		speed *= 2;
	if (SideHasSwamp(side))
		speed /= 4;

	if (mon->status & STATUS1_ANY && ability == ABILITY_QUICKFEET)
		speed *= 2;
	else if (mon->status & STATUS1_PARALYSIS)
	{
		#ifndef OLD_PARALYSIS_SPD_DROP
			speed /= 2;
		#else
			speed /= 4;
		#endif
	}

	return speed;
}
