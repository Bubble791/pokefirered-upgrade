#ifndef GUARD_DYNAMAX_H
#define GUARD_DYNAMAX_H

enum MaxMoveEffect
{
	MAX_EFFECT_NONE,
	MAX_EFFECT_RAISE_TEAM_ATTACK,
	MAX_EFFECT_RAISE_TEAM_DEFENSE,
	MAX_EFFECT_RAISE_TEAM_SPEED,
	MAX_EFFECT_RAISE_TEAM_SP_ATK,
	MAX_EFFECT_RAISE_TEAM_SP_DEF,
	MAX_EFFECT_LOWER_ATTACK,
	MAX_EFFECT_LOWER_DEFENSE,
	MAX_EFFECT_LOWER_SPEED,
	MAX_EFFECT_LOWER_SP_ATK,
	MAX_EFFECT_LOWER_SP_DEF,
	MAX_EFFECT_SUN,
	MAX_EFFECT_RAIN,
	MAX_EFFECT_SANDSTORM,
	MAX_EFFECT_HAIL,
	MAX_EFFECT_ELECTRIC_TERRAIN,
	MAX_EFFECT_GRASSY_TERRAIN,
	MAX_EFFECT_MISTY_TERRAIN,
	MAX_EFFECT_PSYCHIC_TERRAIN,
	MAX_EFFECT_VINE_LASH,
	MAX_EFFECT_WILDFIRE,
	MAX_EFFECT_CANNONADE,
	MAX_EFFECT_EFFECT_SPORE_FOES,
	MAX_EFFECT_PARALYZE_FOES,
	MAX_EFFECT_CONFUSE_FOES_PAY_DAY,
	MAX_EFFECT_CRIT_PLUS,
	MAX_EFFECT_MEAN_LOOK,
	MAX_EFFECT_AURORA_VEIL,
	MAX_EFFECT_INFATUATE_FOES,
	MAX_EFFECT_RECYCLE_BERRIES,
	MAX_EFFECT_POISON_FOES,
	MAX_EFFECT_STEALTH_ROCK,
	MAX_EFFECT_DEFOG,
	MAX_EFFECT_POISON_PARALYZE_FOES,
	MAX_EFFECT_HEAL_TEAM,
	MAX_EFFECT_SPITE,
	MAX_EFFECT_GRAVITY,
	MAX_EFFECT_VOLCAITH_FOES,
	MAX_EFFECT_SANDBLAST_FOES,
	MAX_EFFECT_YAWN_FOE,
	MAX_EFFECT_LOWER_EVASIVENESS_FOES,
	MAX_EFFECT_AROMATHERAPY,
	MAX_EFFECT_CONFUSE_FOES,
	MAX_EFFECT_STEELSURGE,
	MAX_EFFECT_TORMENT_FOES,
	MAX_EFFECT_LOWER_SPEED_2_FOES,
	MAX_EFFECT_FIRE_SPIN_FOES,
};

enum
{
	RAID_ABILITY_HIDDEN,
	RAID_ABILITY_1,
	RAID_ABILITY_2,
	RAID_ABILITY_RANDOM_1_2,
	RAID_ABILITY_RANDOM_ALL,
};

enum RaidStars
{
	NO_RAID,
	ONE_STAR_RAID,
	TWO_STAR_RAID,
	THREE_STAR_RAID,
	FOUR_STAR_RAID,
	FIVE_STAR_RAID,
	SIX_STAR_RAID,
	RAID_STAR_COUNT,
};

#define MAX_NUM_DROPS 12
#define NUM_BADGE_OPTIONS 10
#define MAX_RAID_DROPS 12
#define FIRST_RAID_BATTLE_FLAG 0x1800

struct Raid
{
	u16 species;
	u16 item;
	u8 ability;
	u16 drops[MAX_NUM_DROPS];
};

struct RaidData
{
	struct Raid* data;
	u16 amount;
};

u16 GetDynamaxSpecies(u8 bank, bool8 checkGMaxInstead);
bool8 IsBannedDynamaxSpecies(u16 species);
bool8 CanDynamax(u8 bank);
bool8 CanGigantamax(u8 bank);
const u8* GetDynamaxScript(u8 bank);
void GigantamaxRevert(struct Pokemon* party);
void TryRevertGigantamax(struct Pokemon* mon);
void TryRevertBankGigantamax(u8 bank);
u16 GetGigantamaxSpecies(u16 species, bool8 canGigantamax);
u16 GetGigantamaxBaseForm(u16 species);
bool8 DynamaxEnabled(u8 bank);
bool8 HasBankDynamaxedAlready(u8 bank);
bool8 IsGigantamaxSpecies(u16 species);
bool8 IsDynamaxed(u8 bank);
bool8 IsRaidBattle(void);
bool8 IsGigantamaxed(u8 bank);
bool8 HasDynamaxSymbol(u8 bank);
bool8 DoesDynamaxUsageStopMegaEvolution(u8 bank);
u16 GetMaxMove(u8 bank, u8 moveIndex);
u16 GetMaxMoveByMove(u8 bank, u16 baseMove);
bool8 MonCanUseMaxMoveWithEffect(struct Pokemon* mon, u8 maxEffect);
bool8 MonCanDynamax(struct Pokemon* mon);
bool8 MoveInMonMovesetThatCanChangeByGigantamaxing(struct Pokemon* mon);
bool8 PlayerHasNoMonsLeftThatCanDynamax(void);
u8 GetDynamaxHPBoost(u8 bank);
u8 GetMonDynamaxHPBoost(struct Pokemon* mon);
u8 GetRaidBattleHPBoost(void);
bool8 IsAnyMaxMove(u16 move);
bool8 IsGMaxMove(u16 move);
void EndBattleDynamaxRevert(u8 bank);
void TryBoostDynamaxHPAfterLevelUp(u8 bank);
bool8 IsMaxMoveWithWeatherEffect(u16 move);
bool8 IsMaxMoveWithTerrainEffect(u16 move);
bool8 IsMaxMoveWithStatusEffect(u16 move);
bool8 IsMaxMoveWithConfusionEffect(u16 move);
bool8 IsMaxMoveWithTrapDamageEffect(u16 move);
bool8 IsMaxMoveWithEffect(u16 move, u8 effect);
bool8 IsMaxGuardUp(u8 bank);
bool8 ProtectedByMaxGuard(u8 bankDef, u16 move);
void TryFadeBankPaletteForDynamax(u8 bank, u16 paletteOffset);
u8 GetRaidBattleStatNullificationChance(u8 bank);
bool8 IsFrontierRaidBattle(void);
bool8 IsCatchableRaidBattle(void);
bool8 IsRaidBossUsingRegularMove(u8 bank, u16 baseMove);
u16 GetNextRaidShieldHP(u8 bank);
bool8 HasRaidShields(u8 bank);
void CreateRaidShieldSprites(void);
u8 GetRaidBattleRepeatedAttackChance(u8 bank);
bool8 ShouldCreateRaidShields(u8 bank);
u8 GetRaidBattleKOStatIncreaseAmount(u8 bank);

#define RAID_BATTLE_END (IsCatchableRaidBattle() && GetBankPartyData(BANK_RAID_BOSS)->hp == 0)

#endif // GUARD_DYNAMAX_H