#ifndef GUARD_BATTLE_H
#define GUARD_BATTLE_H

#include <limits.h>
#include "global.h"
#include "constants/battle.h"
#include "battle_util.h"
#include "battle_script_commands.h"
#include "battle_main.h"
#include "battle_ai_switch_items.h"
#include "battle_gfx_sfx_util.h"
#include "battle_util2.h"
#include "battle_bg.h"
#include "mega.h"
#include "dynamax.h"
#include "constants/flags.h"

/*
    Banks are a name given to what could be called a 'battlerId' or 'monControllerId'.
    Each bank has a value consisting of two bits.
    0x1 bit is responsible for the side, 0 = player's side, 1 = opponent's side.
    0x2 bit is responsible for the id of sent out pokemon. 0 means it's the first sent out pokemon, 1 it's the second one. (Triple battle didn't exist at the time yet.)
*/

#define GET_BATTLER_POSITION(battler)((gBattlerPositions[battler]))
#define GET_BATTLER_SIDE(battler)((GetBattlerPosition(battler) & BIT_SIDE))
#define GET_BATTLER_SIDE2(battler)((GET_BATTLER_POSITION(battler) & BIT_SIDE))

#define SIDE(bank) GetBattlerSide(bank)
#define PARTNER(bank) (bank ^ BIT_FLANK)
#define FOE(bank) ((bank ^ BIT_SIDE) & BIT_SIDE)
#define ABILITY(bank) GetBankAbility(bank)
#define SPECIES(bank) gBattleMons[bank].species
#define ITEM(bank) gBattleMons[bank].item
#define ITEM_EFFECT(bank) GetBankItemEffect(bank)
#define ITEM_QUALITY(bank) ItemId_GetHoldEffectParam(gBattleMons[bank].item)
#define ITEM_POCKET(bank) GetPocketByItemId(gBattleMons[bank].item)
#define SPLIT(move) gBattleMoves[move].split
#define PINCH_BERRY_CHECK(bank) (gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 4 || (ABILITY(bank) == ABILITY_GLUTTONY && gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2))

#define SECOND_OPPONENT (VarGet(VAR_SECOND_OPPONENT))
#define BATTLER_MAX_HP(bank) (gBattleMons[bank].hp == gBattleMons[bank].maxHP)
#define STAT_STAGE(bank, stat) (gBattleMons[bank].statStages[stat - 1])
#define BATTLER_ALIVE(bank) (gBattleMons[bank].hp > 0)
#define IS_DOUBLE_BATTLE (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
#define IS_BATTLE_CIRCUS (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS)
#define BATTLER_SEMI_INVULNERABLE(bank) (gStatuses3[bank] & STATUS3_SEMI_INVULNERABLE)
#define IS_BLANK_TYPE(type) (type == TYPE_MYSTERY || type == TYPE_ROOSTLESS || type == TYPE_BLANK)
#define CHOICED_MOVE(bank) gBattleStruct->choicedMove[bank]
#define IS_BEHIND_SUBSTITUTE(bank) (gBattleMons[bank].status2 & STATUS2_SUBSTITUTE)
#define NO_MOLD_BREAKERS(ability, move) (ability != ABILITY_MOLDBREAKER && ability != ABILITY_TERAVOLT && ability != ABILITY_TURBOBLAZE && !CheckTableForMove(move, gMoldBreakerMoves))
#define IS_TRANSFORMED(bank) (gBattleMons[bank].status2 & STATUS2_TRANSFORMED)
#define TOOK_DAMAGE(bank) (gSpecialStatuses[bank].physicalDmg || gSpecialStatuses[bank].specialDmg)
#define MOVE_HAD_EFFECT (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
#define STAT_CAN_RISE(bank, stat) ((gBattleMons[bank].statStages[stat-1] < 12 && ABILITY(bank) != ABILITY_CONTRARY) || (ABILITY(bank) == ABILITY_CONTRARY && gBattleMons[bank].statStages[stat-1] > 0))
#define STAT_CAN_FALL(bank, stat) ((gBattleMons[bank].statStages[stat-1] > 0 && ABILITY(bank) != ABILITY_CONTRARY) || (ABILITY(bank) == ABILITY_CONTRARY && gBattleMons[bank].statStages[stat-1] < 12))
#define SAVED_CONSUMED_ITEMS(bank) gBattleStruct->SavedConsumedItems[gBattlerPartyIndexes[bank]]
#define CONSUMED_ITEMS(bank) gBattleStruct->usedHeldItems[bank]
#define ABILITY_PRESENT(ability) AbilityBattleEffects(ABILITYEFFECT_CHECK_ON_FIELD, 0, ability, 0, 0)
#define IS_SINGLE_BATTLE !IS_DOUBLE_BATTLE
#define MON_CAN_BATTLE(mon) (((GetMonData(mon, MON_DATA_SPECIES) && GetMonData(mon, MON_DATA_IS_EGG) != TRUE && GetMonData(mon, MON_DATA_HP))))
#define IS_GHOST_BATTLE ((gBattleTypeFlags & (BATTLE_TYPE_WILD_SCRIPTED | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST)
#define BANK_RAID_BOSS GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)
#define RAID_BATTLE_END (IsCatchableRaidBattle() && GetBankPartyData(BANK_RAID_BOSS)->hp == 0)

#define BATTLE_HISTORY ((struct BattleHistory*) (gBattleResources->battleHistory))

#define GET_STAT_BUFF_VALUE_WITH_SIGN(n)((n & 0xF8))                 // 0x80, the sign bit

#define TRAINER_OPPONENT_3FE        0x3FE
#define SECRET_BASE_OPPONENT        0x400

// Special Trainer Ids.
#define TRAINER_RECORD_MIXING_FRIEND        300
#define TRAINER_RECORD_MIXING_APPRENTICE    400
#define TRAINER_EREADER                     500
#define TRAINER_FRONTIER_BRAIN              1022
#define TRAINER_PLAYER                      1023
#define TRAINER_SECRET_BASE                 1024
#define TRAINER_LINK_OPPONENT               2048
#define TRAINER_OPPONENT_C00                3072
#define TRAINER_STEVEN_PARTNER              3075
#define TRAINER_CUSTOM_PARTNER              3076

#define B_ACTION_USE_MOVE                  0
#define B_ACTION_USE_ITEM                  1
#define B_ACTION_SWITCH                    2
#define B_ACTION_RUN                       3
#define B_ACTION_SAFARI_WATCH_CAREFULLY    4
#define B_ACTION_SAFARI_BALL               5
#define B_ACTION_SAFARI_BAIT          6
#define B_ACTION_SAFARI_GO_NEAR            7
#define B_ACTION_SAFARI_RUN                8
#define B_ACTION_OLDMAN_THROW              9
#define B_ACTION_EXEC_SCRIPT               10
#define B_ACTION_TRY_FINISH                11
#define B_ACTION_FINISHED                  12
#define B_ACTION_CANCEL_PARTNER            12 // when choosing an action
#define B_ACTION_NOTHING_FAINTED           13 // when choosing an action
#define B_ACTION_NONE                      0xFF

#define MAX_TRAINER_ITEMS 4

// array entries for battle communication
#define MULTIUSE_STATE          0x0
#define CURSOR_POSITION         0x1
#define TASK_ID                 0x1 // task Id and cursor position share the same field
#define SPRITES_INIT_STATE1     0x1 // shares the Id as well
#define SPRITES_INIT_STATE2     0x2
#define MOVE_EFFECT_BYTE        0x3
#define ACTIONS_CONFIRMED_COUNT 0x4
#define MULTISTRING_CHOOSER     0x5
#define MSG_DISPLAY             0x7
#define BATTLE_COMMUNICATION_ENTRIES_COUNT  0x8

#define MOVE_TARGET_SELECTED          0x0
#define MOVE_TARGET_DEPENDS           0x1
#define MOVE_TARGET_USER_OR_PARTNER  0x2
#define MOVE_TARGET_RANDOM            0x4
#define MOVE_TARGET_BOTH              0x8
#define MOVE_TARGET_USER              0x10
#define MOVE_TARGET_FOES_AND_ALLY     0x20
#define MOVE_TARGET_OPPONENTS_FIELD   0x40

#define FLAG_MAKES_CONTACT          0x1
#define FLAG_PROTECT_AFFECTED       0x2
#define FLAG_MAGIC_COAT_AFFECTED    0x4
#define FLAG_SNATCH_AFFECTED        0x8
#define FLAG_MIRROR_MOVE_AFFECTED   0x10
#define FLAG_KINGS_ROCK_AFFECTED    0x20
#define FLAG_TRIAGE_AFFECTED     	0x40

#define SPLIT_PHYSICAL 0
#define SPLIT_SPECIAL 1
#define SPLIT_STATUS 2

#define MAX_NUM_RAID_SHIELDS 5

#define ARG_IN_FUTURE_ATTACK 3
#define ARG_IN_PURSUIT 4
#define ARG_ONLY_EMERGENCY_EXIT 5
#define ARG_DRAGON_TAIL 6
#define ARG_PARTING_SHOT 7

#ifdef OLD_CRIT_DAMAGE
	#define CRIT_MULTIPLIER 20 //2x
#else //Gen 6+ crit damage
	#define CRIT_MULTIPLIER 15 //1.5x
#endif

//Exported Constants
enum {IN_AIR, GROUNDED};

enum
{
	NO_TERRAIN,
	ELECTRIC_TERRAIN,
	GRASSY_TERRAIN,
	MISTY_TERRAIN,
	PSYCHIC_TERRAIN
};

enum
{
	Force_Switch_Regular,
	Force_Switch_Dragon_Tail,
	Force_Switch_Red_Card
};

enum ItemBattleEffectCases
{
	ItemEffects_SwitchIn,
	ItemEffects_EndTurn,
	ItemEffects_ContactTarget,
	ItemEffects_ContactAttacker
};

enum
{
	FAINT_SET_UP,
	FAINT_ATTACKER_ABILITIES,
	FAINT_ADVERSE_PROTECTION,
    FAINT_RAGE,
    FAINT_SYNCHRONIZE_TARGET,
	FAINT_BEAK_BLAST_BURN,
	FAINT_SYNCHRONIZE_ATTACKER,
    FAINT_MOVE_END_ABILITIES,
    FAINT_SYNCHRONIZE_ATTACKER_2,
	FAINT_ITEM_EFFECTS_CONTACT_TARGET,
	FAINT_COUNT,
};

enum
{
	CASTFORM_NO_CHANGE, //0
	CASTFORM_TO_NORMAL, //1
	CASTFORM_TO_FIRE,   //2
	CASTFORM_TO_WATER,  //3
	CASTFORM_TO_ICE,    //4
};

enum CastformForms
{
	CASTFORM_NORMAL,
	CASTFORM_SUN,
	CASTFORM_RAIN,
	CASTFORM_HAIL
};

//Exported Constants
enum
{
	CANCELLER_FLAGS,
	CANCELLER_RAID_BATTLE_NULLIFICATION,
	CANCELLER_ASLEEP,
	CANCELLER_FROZEN,
	CANCELLER_TRUANT,
	CANCELLER_RECHARGE,
	CANCELLER_FLINCH,
	CANCELLER_DEVOLVE,
	CANCELLER_RAID_BATTLES_FAILED_MOVES,
	CANCELLER_BANNED_SIGNATURE_MOVE,
	CANCELLER_GRAVITY,
	CANCELLER_DISABLED,
	CANCELLER_HEAL_BLOCKED,
	CANCELLER_THROAT_CHOP,
	CANCELLER_TAUNTED,
	CANCELLER_IMPRISONED,
	CANCELLER_CONFUSED,
	CANCELLER_PARALYSED,
	CANCELLER_GHOST,
	CANCELLER_IN_LOVE,
	CANCELLER_STANCE_CHANGE,
	CANCELLER_BIDE,
	CANCELLER_THAW,
	CANCELLER_Z_MOVES,
	CANCELLER_GRAVITY_Z_MOVES,

//Called Attacks Start Here
	CANCELLER_RAID_BATTLES_FAILED_MOVES_2,
	CANCELLER_BANNED_SIGNATURE_MOVE_2,
	CANCELLER_GRAVITY_2,
	CANCELLER_SKY_BATTLE,
	CANCELLER_HEAL_BLOCKED_2,
	CANCELLER_THROAT_CHOP_2,
	CANCELLER_STANCE_CHANGE_2,
	CANCELLER_DYNAMAX_FAILED_MOVES,
	CANCELLER_NATURAL_GIFT,
	CANCELLER_DANCER,
	CANCELLER_POWDER,
	CANCELLER_PRIMAL_WEATHER,
	CANCELLER_PSYCHIC_TERRAIN,
	CANCELLER_PRANKSTER,
	CANCELLER_EXPLODING_DAMP,
	CANCELLER_MULTIHIT_MOVES,
	CANCELLER_MULTI_TARGET_MOVES,
	CANCELLER_END,
};

struct TrainerSpotted {
    /* NPC state id */
    u8 id;

    /* Distance from trainer to player */
    u8 distance;

    /* The script the on the trainer NPC. */
    u8* script;
};

struct BattleExtensionState //Clear After Battle
{
    struct {
        u8 count;
		u8 approachingId;
		u8 firstTrainerNPCId; //Used in trainerbattle 0xB
		u8 secondTrainerNPCId;
        struct TrainerSpotted trainers[2];
    } spotted;

    void* partyBackup;
	struct Pokemon* skyBattlePartyBackup;
	u8* trainerBIntroSpeech;
	u8* trainerBDefeatSpeech;
	u8* trainerBVictorySpeech;
	u8* trainerBCantBattleSpeech;
	u8* trainerBRetAddress;

	u16   partnerTrainerId;
	u16   partnerBackSpriteId;
	u16   trainerBTrainerId;
	u8	  multiTaskStateHelper;
};

struct TerrainTableStruct
{
	u8 camouflageType;
	u8 secretPowerEffect;
	u16 secretPowerAnim;
	u16 naturePowerMove;
	u16 burmyForm;
};

enum TotemBoostType
{
	TOTEM_NO_BOOST,
	TOTEM_SINGLE_BOOST,
	TOTEM_OMNIBOOST,
};

enum
{
	TRAINER_SLIDE_LAST_SWITCHIN,
	TRAINER_SLIDE_LAST_LOW_HP,
	TRAINER_SLIDE_FIRST_DOWN,
};

enum EnduranceListings {ENUDRE_REG, ENDURE_STURDY, ENDURE_FOCUS_SASH};

#define RELOAD_BATTLE_STATS(bank, partydata)				\
{															\
	if (gStatuses3[bank] & STATUS3_POWER_TRICK) {			\
		gBattleMons[bank].defense = partydata->attack;		\
		gBattleMons[bank].attack = partydata->defense;		\
	}														\
	else {													\
		gBattleMons[bank].attack = partydata->attack;		\
		gBattleMons[bank].defense = partydata->defense;		\
	}														\
	gBattleMons[bank].speed = partydata->speed;				\
	gBattleMons[bank].spAttack = partydata->spAttack;		\
	gBattleMons[bank].spDefense = partydata->spDefense;		\
	gBattleMons[bank].hp = partydata->hp;					\
	gBattleMons[bank].maxHP = partydata->maxHP;				\
}

#define APPLY_STAT_MOD(var, mon, stat, statIndex)                                   	\
{                                                                                   	\
    (var) = (stat) * (gStatStageRatios)[(mon)->statStages[(statIndex)-1]][0];         	\
    (var) = (var) / (gStatStageRatios)[(mon)->statStages[(statIndex)-1]][1];     \
}

#define APPLY_QUICK_STAT_MOD(var, buff)                 \
{                                                       \
    (var) = (var) * gStatStageRatios[buff][0];          \
    (var) = ((var) / gStatStageRatios[buff][1]);   \
}

struct TrainerMonNoItemDefaultMoves
{
    u16 iv;
    u8 lvl;
    u16 species;
};

struct TrainerMonItemDefaultMoves
{
    u16 iv;
    u8 lvl;
    u16 species;
    u16 heldItem;
};

struct TrainerMonNoItemCustomMoves
{
    u16 iv;
    u8 lvl;
    u16 species;
    u16 moves[4];
};

struct TrainerMonItemCustomMoves
{
    u16 iv;
    u8 lvl;
    u16 species;
    u16 heldItem;
    u16 moves[4];
};

union TrainerMonPtr
{
    const struct TrainerMonNoItemDefaultMoves *NoItemDefaultMoves;
    const struct TrainerMonNoItemCustomMoves *NoItemCustomMoves;
    const struct TrainerMonItemDefaultMoves *ItemDefaultMoves;
    const struct TrainerMonItemCustomMoves *ItemCustomMoves;
};

struct Trainer
{
    /*0x00*/ u8 partyFlags;
    /*0x01*/ u8 trainerClass;
    /*0x02*/ u8 encounterMusic_gender; // last bit is gender
    /*0x03*/ u8 trainerPic;
    /*0x04*/ u8 trainerName[12];
    /*0x10*/ u16 items[4];
    /*0x18*/ bool8 doubleBattle;
    /*0x1C*/ u32 aiFlags;
    /*0x20*/ u8 partySize;
    /*0x24*/ const union TrainerMonPtr party;
};

extern const struct Trainer gTrainers[];

struct ResourceFlags
{
    u32 flags[4];
};

#define RESOURCE_FLAG_FLASH_FIRE 1

struct DisableStruct
{
    /*0x00*/ u32 transformedMonPersonality;
    /*0x04*/ u16 disabledMove;
    /*0x06*/ u16 encoredMove;
    /*0x08*/ u8 protectUses;
    /*0x09*/ u8 stockpileCounter;
    /*0x0A*/ u8 substituteHP;
    /*0x0B*/ u8 disableTimer : 4;
    /*0x0B*/ u8 disableTimerStartValue : 4;
    /*0x0C*/ u8 encoredMovePos;
    /*0x0D*/ u8 unkD;
    /*0x0E*/ u8 encoreTimer : 4;
    /*0x0E*/ u8 encoreTimerStartValue : 4;
    /*0x0F*/ u8 perishSongTimer : 4;
    /*0x0F*/ u8 perishSongTimerStartValue : 4;
    /*0x10*/ u8 furyCutterCounter;
    /*0x11*/ u8 rolloutTimer : 4;
    /*0x11*/ u8 rolloutTimerStartValue : 4;
    /*0x12*/ u8 chargeTimer : 4;
    /*0x12*/ u8 chargeTimerStartValue : 4;
    /*0x13*/ u8 tauntTimer:4;
    /*0x13*/ u8 tauntTimer2:4;
    /*0x14*/ u8 battlerPreventingEscape;
    /*0x15*/ u8 battlerWithSureHit;
    /*0x16*/ u8 isFirstTurn;
    /*0x17*/ u8 unk17;
    /*0x18*/ u8 truantCounter : 1;
    /*0x18*/ u8 truantSwitchInHack : 1; // unused? 
    /*0x18*/ u8 unk18_a_2 : 2;
    /*0x18*/ u8 mimickedMoves : 4;
    /*0x19*/ u8 rechargeTimer;
    /*0x1A*/ u8 unk1A[2];
};

extern struct DisableStruct gDisableStructs[MAX_BATTLERS_COUNT];

struct ProtectStruct
{
    /* field_0 */
    u32 protected:1;
    u32 endured:1;
    u32 noValidMoves:1;
    u32 helpingHand:1;
    u32 bounceMove:1;
    u32 stealMove:1;
    u32 flag0Unknown:1;
    u32 prlzImmobility:1;
    /* field_1 */
    u32 confusionSelfDmg:1;
    u32 targetNotAffected:1;
    u32 chargingTurn:1;
    u32 fleeFlag:2; // for RunAway and Smoke Ball
    u32 usedImprisonedMove:1;
    u32 loveImmobility:1;
    u32 usedDisabledMove:1;
    /* field_2 */
    u32 usedTauntedMove:1;      // 0x1
    u32 flag2Unknown:1;         // 0x2
    u32 flinchImmobility:1;     // 0x4
    u32 notFirstStrike:1;       // 0x8
    u32 palaceUnableToUseMove : 1;           // 0x10
    u32 obstruct : 1;           // 0x20
    u32 flag_x40 : 1;           // 0x40
    u32 obstructDamage : 1;           // 0x80
    u32 field3 : 8;

    /* field_3 */
    u32 KingsShield : 1;
    u32 SpikyShield : 1;
	u32 BanefulBunker : 1;
    u32 kingsshield_damage : 1;
    u32 spikyshield_damage : 1;
    u32 banefulbunker_damage : 1;
    u32 enduredSturdy : 1;
    u32 Field3 : 1;

    u32 physicalDmg;
    u32 specialDmg;
    u8 physicalBattlerId;
    u8 specialBattlerId;
    u16 fieldE;
};

extern struct ProtectStruct gProtectStructs[MAX_BATTLERS_COUNT];

struct SpecialStatus
{
    u8 statLowered : 1;             // 0x1
    u8 lightningRodRedirected : 1;  // 0x2
    u8 restoredBattlerSprite: 1;    // 0x4
    u8 intimidatedMon : 1;          // 0x8
    u8 traced : 1;                  // 0x10
    u8 ppNotAffectedByPressure : 1;
    u8 flag40 : 1;
    u8 focusBanded : 1;
    u8 field1[3];
    s32 dmg;
    s32 physicalDmg;
    s32 specialDmg;
    u8 physicalBattlerId;
    u8 specialBattlerId;
    u8 field12;
    u8 field13;
};

extern struct SpecialStatus gSpecialStatuses[MAX_BATTLERS_COUNT];

struct SideTimer
{
    /*0x00*/ u8 reflectTimer;
    /*0x01*/ u8 reflectBattlerId;
    /*0x02*/ u8 lightscreenTimer;
    /*0x03*/ u8 lightscreenBattlerId;
    /*0x04*/ u8 mistTimer;
    /*0x05*/ u8 mistBattlerId;
    /*0x06*/ u8 safeguardTimer;
    /*0x07*/ u8 safeguardBattlerId;
    /*0x08*/ u8 followmeTimer;
    /*0x09*/ u8 followmeTarget;
    /*0x0A*/ u8 spikesAmount : 2;
			 u8 tspikesAmount : 2;
			 u8 srAmount : 1;
			 u8 stickyWeb : 1;
    /*0x0B*/ u8 steelsurge: 1;
};

extern struct SideTimer gSideTimers[];

struct WishFutureKnock
{
    u8 futureSightCounter[MAX_BATTLERS_COUNT];
    u8 futureSightAttacker[MAX_BATTLERS_COUNT];
    u32 futureSightPartyIndex[MAX_BATTLERS_COUNT]; //was s32 futureSightDmg[BATTLE_BANKS_COUNT];
    u16 futureSightMove[MAX_BATTLERS_COUNT];
    u8 wishCounter[MAX_BATTLERS_COUNT];
    u8 wishMonId[MAX_BATTLERS_COUNT];
    u8 weatherDuration;
    u8 knockedOffMons[2];
};

extern struct WishFutureKnock gWishFutureKnock;

struct AI_ThinkingStruct
{
    u8 aiState;
    u8 movesetIndex;
    u16 moveConsidered;
    s8 score[4];
    u32 funcResult;
    u32 aiFlags;
    u8 aiAction;
    u8 aiLogicId;
    u8 filler12[6];
    u8 simulatedRNG[4];
};

extern u8 gActiveBattler;
extern u8 gBattlerTarget;
extern u8 gAbsentBattlerFlags;

extern struct BattlePokemon gBattleMons[MAX_BATTLERS_COUNT];

struct UsedMoves
{
    u16 moves[MAX_BATTLERS_COUNT];
    u16 unknown[MAX_BATTLERS_COUNT];
};

struct BattleHistory
{
    /*0x00*/ u16 usedMoves[2][8]; // 0xFFFF means move not used (confuse self hit, etc)
    /*0x20*/ u8 abilities[MAX_BATTLERS_COUNT / 2];
    /*0x22*/ u8 itemEffects[MAX_BATTLERS_COUNT / 2];
    /*0x24*/ u16 trainerItems[MAX_BATTLERS_COUNT];
    /*0x2C*/ u8 itemsNo;
};

struct BattleScriptsStack
{
    const u8 *ptr[8];
    u8 size;
};

struct BattleCallbacksStack
{
    void (*function[8])(void);
    u8 size;
};

struct StatsArray
{
    u16 stats[NUM_STATS];
};

struct BattleResources
{
    struct SecretBaseRecord *secretBase;
    struct ResourceFlags *flags;
    struct BattleScriptsStack *battleScriptsStack;
    struct BattleCallbacksStack *battleCallbackStack;
    struct StatsArray *beforeLvlUp;
    struct AI_ThinkingStruct *ai;
    struct BattleHistory *battleHistory;
    struct BattleScriptsStack *AI_ScriptsStack;
};

extern struct BattleResources *gBattleResources;

struct BattleResults
{
    u8 playerFaintCounter;    // 0x0
    u8 opponentFaintCounter;  // 0x1
    u8 playerSwitchesCounter; // 0x2
    u8 numHealingItemsUsed;   // 0x3
    u8 numRevivesUsed;        // 0x4
    u8 playerMonWasDamaged:1; // 0x5
    u8 usedMasterBall:1;      // 0x5
    u8 caughtMonBall:4;       // 0x5
    u8 shinyWildMon:1;        // 0x5
    u8 unk5_7:1;              // 0x5
    u16 playerMon1Species;    // 0x6
    u8 playerMon1Name[11];    // 0x8
    u8 battleTurnCounter;     // 0x13
    u8 playerMon2Name[11];    // 0x14
    u8 pokeblockThrows;       // 0x1F
    u16 lastOpponentSpecies;  // 0x20
    u16 lastUsedMovePlayer;   // 0x22
    u16 lastUsedMoveOpponent; // 0x24
    u16 playerMon2Species;    // 0x26
    u16 caughtMonSpecies;     // 0x28
    u8 caughtMonNick[10];     // 0x2A
    u8 filler34[2];
    u8 catchAttempts[11];     // 0x36
};

extern struct BattleResults gBattleResults;

struct LinkPartnerHeader
{
    u8 versionSignatureLo;
    u8 versionSignatureHi;
    u8 vsScreenHealthFlagsLo;
    u8 vsScreenHealthFlagsHi;
    struct BattleEnigmaBerry battleEnigmaBerry;
};

struct BattleStruct
{
    u8 turnEffectsTracker;
    u8 turnEffectsBattlerId;
    u8 filler2; // unused
    u8 turnCountersTracker;
    u8 wrappedMove[MAX_BATTLERS_COUNT * 2]; // Leftover from Ruby's ewram access.
    u8 moveTarget[MAX_BATTLERS_COUNT];
    u8 expGetterMonId;
    u8 field_11; // unused
    u8 wildVictorySong;
    u8 dynamicMoveType;
    u8 wrappedBy[MAX_BATTLERS_COUNT];
    u16 assistPossibleMoves[PARTY_SIZE * MAX_MON_MOVES]; // 6 mons, each of them knowing 4 moves
    u8 focusPunchBattlerId;
    u8 battlerPreventingSwitchout;
    u8 moneyMultiplier;
    u8 savedTurnActionNumber;
    u8 switchInAbilitiesCounter;
    u8 faintedActionsState;
    u8 faintedActionsBattlerId;
    // balign 2
    u16 expValue;
    u8 scriptPartyIdx; // for printing the nickname
    u8 sentInPokes;
    bool8 selectionScriptFinished[MAX_BATTLERS_COUNT];
    u8 battlerPartyIndexes[MAX_BATTLERS_COUNT];
    u8 monToSwitchIntoId[MAX_BATTLERS_COUNT];
    u8 battlerPartyOrders[MAX_BATTLERS_COUNT][3];
    u8 runTries;
    u8 caughtMonNick[POKEMON_NAME_LENGTH + 1];
    u8 field_78; // unused
    u8 safariGoNearCounter;
    u8 safariPkblThrowCounter;
    u8 safariEscapeFactor;
    u8 safariCatchFactor;
    u8 linkBattleVsSpriteId_V;
    u8 linkBattleVsSpriteId_S;
    u8 formToChangeInto;
    u8 chosenMovePositions[MAX_BATTLERS_COUNT];
    u8 stateIdAfterSelScript[MAX_BATTLERS_COUNT];
    u8 field_88; // unused
    u8 field_89; // unused
    u8 field_8A; // unused
    u8 playerPartyIdx;
    u8 field_8C; // unused
    u8 field_8D; // unused
    u8 stringMoveType;
    u8 expGetterBattlerId;
    u8 field_90; // unused
    u8 absentBattlerFlags;
    u8 AI_monToSwitchIntoId[2];
    u8 simulatedInputState[4];  // used by Oak/Old Man/Pokedude controllers
    u8 lastTakenMove[MAX_BATTLERS_COUNT * 2 * 2]; // ask gamefreak why they declared it that way
    u16 hpOnSwitchout[2];
    u8 abilityPreventingSwitchout;
    u8 hpScale;
    u16 savedBattleTypeFlags;
    void (*savedCallback)(void);
    u8 synchronizeMoveEffect;
    u8 multiplayerId;
    u8 overworldWeatherDone;
    u8 atkCancellerTracker;
    u16 usedHeldItems[MAX_BATTLERS_COUNT];
    u8 chosenItem[4]; // why is this an u8?
    u8 AI_itemType[2];
    u8 AI_itemFlags[2];
    u16 choicedMove[MAX_BATTLERS_COUNT];
    u16 changedItems[MAX_BATTLERS_COUNT];
    u8 intimidateBattler;
    u8 switchInItemsCounter;
    u8 field_DA; // battle tower related
    u8 turnSideTracker;
    u8 fillerDC[0xDF-0xDC];
    u8 givenExpMons;
    u16 lastTakenMoveFrom[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; // 4 sub-arrays containing the moves that bank was hit by, by each bank //used by Mirror Move
    u16 castformPalette[MAX_BATTLERS_COUNT][16];
    u8 wishPerishSongState;
    u8 wishPerishSongBattlerId;
    u8 field_182;
    // align 4
    union {
        struct LinkPartnerHeader linkPartnerHeader;
        struct MultiBattlePokemonTx multiBattleMons[3];
    } multiBuffer;
    //u8 padding_1E4[0x1C];

    //New battle struct data
//Field Counters
	u8 MudSportTimer;
	u8 WaterSportTimer;
	u8 GravityTimer;
	u8 TrickRoomTimer;
	u8 MagicRoomTimer;
	u8 WonderRoomTimer;
	u8 FairyLockTimer;
	u8 IonDelugeTimer;
	//u8 TerrainType;
	u8 TerrainTimer;

	//Team Counters
	u8 SeaOfFireTimers[NUM_BATTLE_SIDES];
	u8 SwampTimers[NUM_BATTLE_SIDES];
	u8 RainbowTimers[NUM_BATTLE_SIDES];
	u8 RetaliateCounters[NUM_BATTLE_SIDES];
	u8 LuckyChantTimers[NUM_BATTLE_SIDES];
	u8 TailwindTimers[NUM_BATTLE_SIDES];
	u8 AuroraVeilTimers[NUM_BATTLE_SIDES];
	u8 maxVineLashTimers[NUM_BATTLE_SIDES];
	u8 maxWildfireTimers[NUM_BATTLE_SIDES];
	u8 maxCannonadeTimers[NUM_BATTLE_SIDES];
	u8 maxVolcalithTimers[NUM_BATTLE_SIDES];
	u8 ragePowdered;

	//Personal Counters
	u8 TelekinesisTimers[MAX_BATTLERS_COUNT];
	u8 MagnetRiseTimers[MAX_BATTLERS_COUNT];
	u8 HealBlockTimers[MAX_BATTLERS_COUNT];
	u8 LaserFocusTimers[MAX_BATTLERS_COUNT];
	u8 ThroatChopTimers[MAX_BATTLERS_COUNT];
	u8 EmbargoTimers[MAX_BATTLERS_COUNT];
	u8 ElectrifyTimers[MAX_BATTLERS_COUNT];
	u8 SlowStartTimers[MAX_BATTLERS_COUNT];
	u8 StakeoutCounters[MAX_BATTLERS_COUNT];
	u8 StompingTantrumTimers[MAX_BATTLERS_COUNT];
	u8 NimbleCounters[MAX_BATTLERS_COUNT];
	u8 DestinyBondCounters[MAX_BATTLERS_COUNT];
	u8 MetronomeCounter[MAX_BATTLERS_COUNT];
	u8 metronomeItemBonus[MAX_BATTLERS_COUNT]; //Used to help with multi-turn attacks
	u8 IncinerateCounters[MAX_BATTLERS_COUNT];
	u8 LastUsedTypes[MAX_BATTLERS_COUNT];
	u8 lastTargeted[MAX_BATTLERS_COUNT];
	u8 usedMoveIndices[MAX_BATTLERS_COUNT];
	u8 DisabledMoldBreakerAbilities[MAX_BATTLERS_COUNT];
	u8 SuppressedAbilities[MAX_BATTLERS_COUNT];
	u8 neutralizingGasBlockedAbilities[MAX_BATTLERS_COUNT];
	u8 skyDropAttackersTarget[MAX_BATTLERS_COUNT]; //skyDropAttackersTarget[gBattlerAttacker] = gBattlerTarget
	u8 skyDropTargetsAttacker[MAX_BATTLERS_COUNT]; //skyDropTargetsAttacker[gBattlerTarget] = gBattlerAttacker
	u8 pickupStack[MAX_BATTLERS_COUNT];
	u8 synchronizeTarget[MAX_BATTLERS_COUNT]; //Bank + 1 that statused given bank
	u8 leftoverHealingDone[MAX_BATTLERS_COUNT]; //Leftovers already restored health this turn or Sticky Barb did damage
	u8 statRoseThisRound[MAX_BATTLERS_COUNT];
	u8 statFellThisTurn[MAX_BATTLERS_COUNT];
	u8 statFellThisRound[MAX_BATTLERS_COUNT];
	u8 recalculatedBestDoublesKillingScores[MAX_BATTLERS_COUNT];
	s8 lastBracketCalc[MAX_BATTLERS_COUNT];
	u8 chiStrikeCritBoosts[MAX_BATTLERS_COUNT]; //~0x2017A4B
	u8 sandblastCentiferno[MAX_BATTLERS_COUNT]; //Records if any banks are trapped by G-Max Centiferno or G-Max Sandblast
	u8 disguisedAs[MAX_BATTLERS_COUNT]; //The party index + 1 the mon with Illusion is disguised as

	//Bit Fields for Banks
	u8 MicleBerryBits;
	u8 UnburdenBoosts;
	u8 BeakBlastByte;
	u8 playedFocusPunchMessage;
	u8 playedShellTrapMessage;
	u8 RoostCounter;
	u8 CustapQuickClawIndicator; //0x2017632
	u8 HealingWishLoc;
	u8 PowderByte;
	u8 quashed;
	u8 tarShotBits;
	u8 trappedByOctolock;
	u8 trappedByNoRetreat;
	u8 AbsentBattlerHelper;
	u8 activeAbilityPopUps;
	u8 NoMoreMovingThisTurn;
	u8 handleSetSwitchIns;
	u8 brokeFreeMessage;
	u8 doSwitchInEffects;
	u8 devolveForgotMove;
	u8 hiddenAnimBattlerSprites;

	//Bit Fields for Party
	u8 BelchCounters;
	u8 IllusionBroken;

	//Other Helpers
	u8 switchOutAbilitiesState; //For tracking effects that happen on switch-out
	u8 switchInEffectsState; //For tracking effects that happen on switch-in
	u8 preFaintEffectsState; //For tracking effects that happen right before the target faints
	u8 faintEffectsState; //For tracking effects that happen after the target faints
	u8 endTurnBlockState; //For tracking sub-blocks utilized by the end turn function
	u8 SentInBackup;
	u8 OriginalAttackerTargetCount;
	u8 MoveBounceTargetCount;
	u8 EchoedVoiceCounter;
	u8 EchoedVoiceDamageScale;
	u8 DancerBankCount;
	u8 CurrentTurnAttacker : 4;
	u8 CurrentTurnTarget : 4;
	u8 targetsToBringDown;
	u8 backupMoveEffect;
	u8 savedObjId;
	u8 lastFainted;
	s8 intimidateActive;
	u8 backupAbility;
	u8 switchOutBankLooper;
	u8 skipBankStatAnim;
	u8 maxGoldrushUses;
	u8 playerItemUsedCount; //~0x2017A82
	u8 originalAttackerBackup : 2;
	u8 originalTargetBackup : 2;

	//Booleans
	bool8 MoveBounceInProgress : 2;
	bool8 moveWasBouncedThisTurn : 1;
	bool8 AttackerDidDamageAtLeastOnce : 1;
	bool8 PledgeHelper : 3;
	bool8 ParentalBondOn : 2;
	bool8 MeFirstByte : 1;
	bool8 ReceiverActivated : 1;
	bool8 GemHelper : 1;
	bool8 fusionFlareUsedPrior : 1;
	bool8 fusionBoltUsedPrior : 1;
	bool8 endTurnDone : 1;
	bool8 HappyHourByte : 1;
	bool8 attackAnimationPlayed : 1;
	bool8 DancerInProgress : 1;
	bool8 DancerByte : 1;
	bool8 InstructInProgress : 1;
	bool8 NoSymbiosisByte : 1;
	bool8 SpectralThiefActive : 1;
	bool8 MultiHitOn : 1;
	bool8 secondaryEffectApplied : 1;
	bool8 bypassSubstitute : 1;
	bool8 criticalCapture : 1;
	bool8 criticalCaptureSuccess : 1;
	bool8 trainerSlideLowHpMsgDone : 1;
	bool8 TeleportBit : 1;
	bool8 restartEndTurnSwitching : 1;
	bool8 skipCertainSwitchInAbilities : 1;
	bool8 roundUsed : 1; //0x2017653
	bool8 activatedCustapQuickClaw : 1;
	bool8 calculatedAIPredictions : 1;
	bool8 batonPassing : 1;
	bool8 activateBlunderPolicy : 1;
	bool8 tempIgnoreAnimations : 1;
	bool8 firstFailedPokeBallStored : 1;
	bool8 fogBlownAwayByDefog : 1;
	bool8 terrainForcefullyRemoved : 1;
	bool8 printedNeutralizingGasOverMsg : 1;
	bool8 doneDoublesSpreadHit : 1; //For when the HP bars all go down during spread moves
	bool8 calculatedSpreadMoveData : 1; //After the damage has been calculated for all Pokemon hit by a spread move
	bool8 calculatedSpreadMoveAccuracy : 1;  //After the accuracy has been calculated for all Pokemon hit by a spread move
	bool8 breakDisguiseSpecialDmg : 1;
	bool8 handlingFaintSwitching : 1;
	bool8 doingPluckItemEffect : 1;
	bool8 usedXSpDef : 1; //Needed because it's hooked into the X Sp. Atk
	bool8 lessThanHalfHPBeforeShellBell : 1; //For Emergency Exit

	//Other
	u16 LastUsedMove;
	u16 NewWishHealthSave;
	u32 totalDamageGiven;
	u32 selfInflictedDamage; //For Emergency Exit
	u8 DancerTurnOrder[MAX_BATTLERS_COUNT];
	u8 PayDayByPartyIndices[PARTY_SIZE];
	u16 SavedConsumedItems[PARTY_SIZE];
	u8 expHelper[MAX_BATTLERS_COUNT];
	u8 megaIndicatorObjIds[MAX_BATTLERS_COUNT];
	u8 abilityPopUpIds[MAX_BATTLERS_COUNT][2];
	u8 backupSynchronizeBanks[2];
	u16 failedThrownPokeBall;
	u32 maxGoldrushMoney;
	u16 itemBackup[PARTY_SIZE];
	u8 hiddenHealthboxFlags[MAX_SPRITES / 8]; //~2017AD8

	//Things for Spread Moves
	s32 DamageTaken[MAX_BATTLERS_COUNT]; //~0x2017AC8
	s32 turnDamageTaken[MAX_BATTLERS_COUNT]; //Specifically for multi-hit moves and Emergency Exit
	u8 criticalMultiplier[MAX_BATTLERS_COUNT];
	u8 ResultFlags[MAX_BATTLERS_COUNT];
	u8 missStringId[MAX_BATTLERS_COUNT];
	u8 EnduranceHelper[MAX_BATTLERS_COUNT];
	bool8 noResultString[MAX_BATTLERS_COUNT];
	u8 foeSpreadTargets;
	u8 allSpreadTargets;

	struct
	{
		u8 chosen[4];
		u8 done[4];
		u8 state;
		u8 activeBank;
		const u8* script;
		bool8 megaEvoInProgress : 1; //Used to tell the game whether or not the turn order should be recalculated
	} megaData;

	struct
	{
		u8 chosen[4];
		u8 done[4];
	} ultraData;

	struct
	{
		u8 toBeUsed[4];
		u8 used[4];
		u8 partyIndex[2]; //Index of party member who used Z-Move
		u8 effect;
		u8 state;
		bool8 active : 1;
		bool8 effectApplied : 1; //Used so moves like Sleep Talk don't give 2 z-effects, clear in CMD49
		bool8 healReplacement : 1;
		bool8 viewing : 1;
		bool8 runningZEffect : 1;
		bool8 viewingDetails : 1; //Not actually related to Z-Moves, I just felt like putting it here
	} zMoveData;

	struct DynamaxData
	{
		bool8 toBeUsed[MAX_BATTLERS_COUNT];
		bool8 used[MAX_BATTLERS_COUNT];
		s8 timer[MAX_BATTLERS_COUNT]; //Negative number means permanent
		u8 partyIndex[NUM_BATTLE_SIDES];
		u8 shieldSpriteIds[MAX_NUM_RAID_SHIELDS]; //Shields for raid battles
		u8 shieldCount;					//The amount of shields created in a Raid Battle
		u8 shieldsDestroyed;			//The amount of shields destroyed in a Raid Battle
		u8 stormLevel;					//The number of Pokemon the raid boss has KO'd.
		u8 repeatedAttacks;				//The amount of times the raid boss took another attack
		bool8 active : 1;
		bool8 viewing : 1;
		bool8 raidShieldsUp : 1;
		bool8 attackAgain : 1;
		bool8 nullifiedStats : 1;
		u8 backupMoveSelectionCursorPos;
		u16 turnStartHP;
		u16 backupRaidMonItem;
	} dynamaxData;

	struct 
	{
		u16 zMoveHelper;
		bool8 sideSwitchedThisRound;
		u8 switchingCooldown[MAX_BATTLERS_COUNT]; //~0x2017B5B
		u8 itemEffects[MAX_BATTLERS_COUNT];
		u16 movePredictions[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //movePredictions[bankAtk][bankDef]
		u16 strongestMove[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //strongestMove[bankAtk][bankDef]
		bool8 moveKnocksOut1Hit[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT][MAX_MON_MOVES]; //moveKnocksOut1Hit[bankAtk][bankDef][monMoveIndex]
		bool8 moveKnocksOut2Hits[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT][MAX_MON_MOVES]; //moveKnocksOut2Hits[bankAtk][bankDef][monMoveIndex]
		u32 damageByMove[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT][MAX_MON_MOVES]; //damageByMove[bankAtk][bankDef][monMoveIndex]
		u16 bestDoublesKillingMoves[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //bestDoublesKillingMoves[bankAtk][bankDef]
		s8 bestDoublesKillingScores[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //bestDoublesKillingScores[bankAtk][bankDef][bankDef / bankDefPartner / bankAtkPartner]
		bool8 canKnockOut[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //canKnockOut[bankAtk][bankDef]
		bool8 can2HKO[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //can2HKO[bankAtk][bankDef]
		u8 bestMonIdToSwitchInto[MAX_BATTLERS_COUNT][2]; //bestMonIdToSwitchInto[bankAtk][first or second choice]
		s8 bestMonIdToSwitchIntoScores[MAX_BATTLERS_COUNT][2];//bestMonIdToSwitchIntoScores[bankAtk][first or second choice]
		u8 calculatedAISwitchings[MAX_BATTLERS_COUNT]; //calculatedAISwitchings[bankSwitch]
		u8 fightingStyle[MAX_BATTLERS_COUNT]; //fightingStyle[bankAtk]
		u8 dynamaxMonId[NUM_BATTLE_SIDES]; //dynamaxMonId[SIDE(bankAtk)] //2017C1E
		bool8 onlyBadMovesLeft[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //onlyBadMovesLeft[bankAtk][bankDef]
		bool8 shouldFreeChoiceLockWithDynamax[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //shouldFreeChoiceLockWithDynamax[bankAtk][bankDef]
		bool8 dynamaxPotential[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //dynamaxPotential[bankAtk][bankDef]
		const void* megaPotential[MAX_BATTLERS_COUNT]; //aiMegaPotential[bankAtk] - stores evolution data of attacker
	} ai;
}; // size == 0x200 bytes

extern struct BattleStruct *gBattleStruct;

#define GET_MOVE_TYPE(move, typeArg)                        \
{                                                           \
    if (gBattleStruct->dynamicMoveType)                     \
        typeArg = gBattleStruct->dynamicMoveType & 0x3F;    \
    else                                                    \
        typeArg = gBattleMoves[move].type;                  \
}

#define IS_TYPE_PHYSICAL(moveType)(moveType < TYPE_MYSTERY)
#define IS_TYPE_SPECIAL(moveType)(moveType > TYPE_MYSTERY)
#define TARGET_TURN_DAMAGED ((gSpecialStatuses[gBattlerTarget].physicalDmg != 0 || gSpecialStatuses[gBattlerTarget].specialDmg != 0))
#define IS_BATTLER_OF_TYPE(battlerId, type)((gBattleMons[battlerId].type1 == type || gBattleMons[battlerId].type2 == type))
#define SET_BATTLER_TYPE(battlerId, type)   \
{                                           \
    gBattleMons[battlerId].type1 = type;    \
    gBattleMons[battlerId].type2 = type;    \
}

#define GET_STAT_BUFF_ID(n)((n & 7))               // first four bits 0x1, 0x2, 0x4, 0x8
#define GET_STAT_BUFF_VALUE2(n)((n & 0xF0))         //need todo
#define GET_STAT_BUFF_VALUE(n)(((n >> 4) & 7))      // 0x10, 0x20, 0x40
#define STAT_BUFF_NEGATIVE 0x80                     // 0x80, the sign bit

#define SET_STAT_BUFF_VALUE(n)(((s8)(((s8)(n) << 4)) & 0xF0))

#define SET_STATCHANGER(statId, stage, goesDown)(gBattleScripting.statChanger = (statId) + (stage << 4) + (goesDown << 7))

struct BattleScripting
{
    s32 painSplitHp;
    s32 bideDmg;
    u8 multihitString[6];
    u8 dmgMultiplier;
    u8 twoTurnsMoveStringId;
    u8 animArg1;
    u8 animArg2;
    u16 tripleKickPower;
    u8 atk49_state;
    u8 battlerWithAbility;
    u8 multihitMoveEffect;
    u8 battler;
    u8 animTurn;
    u8 animTargetsHit;
    u8 statChanger;
    bool8 statAnimPlayed;
    u8 atk23_state;
    u8 battleStyle;
    u8 atk6C_state;
    u8 learnMoveState;
    u8 field_20;
    u8 reshowMainState;
    u8 reshowHelperState;
    u8 field_23;
};

enum
{
    BACK_PIC_RED,
    BACK_PIC_LEAF,
    BACK_PIC_RS_BRENDAN,
    BACK_PIC_RS_MAY,
    BACK_PIC_POKEDUDE,
    BACK_PIC_OLDMAN
};

struct BattleSpriteInfo
{
    /*0x0*/ u16 invisible : 1; // 0x1
            u16 lowHpSong : 1; // 0x2
            u16 behindSubstitute : 1; // 0x4
            u16 substituteOffScreen : 1; // 0x8
            u16 hpNumbersNoBars : 1; // 0x10
    /*0x2*/ u16 transformSpecies;
};

struct BattleAnimationInfo
{
    u16 animArg; // to fill up later
    u8 field_2;
    u8 field_3;
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 field_7;
    u8 ballThrowCaseId;
    u8 healthboxSlideInStarted : 1;
    u8 field_9_x2 : 1;
    u8 field_9_x1C : 3;
    u8 field_9_x20 : 1;
    u8 field_9_x40 : 1;
    u8 field_9_x80 : 1;
    u8 field_A;
    u8 field_B;
    s16 field_C;
    u8 field_E;
    u8 field_F;
};

struct BattleHealthboxInfo
{
    u8 partyStatusSummaryShown : 1; // x1
    u8 healthboxIsBouncing : 1; // x2
    u8 battlerIsBouncing : 1; // x4
    u8 ballAnimActive : 1; // 0x8
    u8 statusAnimActive : 1; // x10
    u8 animFromTableActive : 1; // x20
    u8 specialAnimActive : 1; // x40
    u8 triedShinyMonAnim : 1; // x80
    u8 finishedShinyMonAnim : 1; // x1
    u8 opponentDrawPartyStatusSummaryDelay : 5; // x2
    u8 healthboxBounceSpriteId;
    u8 battlerBounceSpriteId;
    u8 animationState;
    u8 partyStatusDelayTimer;
    u8 matrixNum;
    u8 shadowSpriteId;
    u8 soundTimer;
    u8 introEndDelay;
};

struct BattleBarInfo
{
    u8 healthboxSpriteId;
    s32 maxValue;
    s32 oldValue;
    s32 receivedValue;
    s32 currValue;
};

struct BattleSpriteData
{
    struct BattleSpriteInfo *battlerData;
    struct BattleHealthboxInfo *healthBoxesData;
    struct BattleAnimationInfo *animationData;
    struct BattleBarInfo *battleBars;
};

extern struct BattleSpriteData *gBattleSpritesDataPtr;

#define BATTLE_BUFFER_LINK_SIZE 0x1000

extern u8 *gLinkBattleSendBuffer;
extern u8 *gLinkBattleRecvBuffer;

// Move this somewhere else

#include "sprite.h"

struct MonSpritesGfx
{
    void* firstDecompressed; // ptr to the decompressed sprite of the first pokemon
    void* sprites[MAX_BATTLERS_COUNT];
    struct SpriteTemplate templates[MAX_BATTLERS_COUNT];
    struct SpriteFrameImage images[MAX_BATTLERS_COUNT][4];
    u8 field_F4[0x80]; // unused
    u8 *barFontGfx;
    void *field_178; // freed but never allocated
    u16 *multiUseBuffer;
};

struct PokedudeBattlerState
{
    u8 action_idx;
    u8 move_idx;
    u8 timer;
    u8 msg_idx;
    u8 saved_bg0y;
};

extern const struct TerrainTableStruct gTerrainTable[];

extern u16 gBattle_BG0_X;
extern u16 gBattle_BG0_Y;
extern u16 gBattle_BG1_X;
extern u16 gBattle_BG1_Y;
extern u16 gBattle_BG2_X;
extern u16 gBattle_BG2_Y;
extern u16 gBattle_BG3_X;
extern u16 gBattle_BG3_Y;
extern u16 gBattle_WIN0H;
extern u16 gBattle_WIN0V;
extern u16 gBattle_WIN1H;
extern u16 gBattle_WIN1V;
extern struct BattleSpritesGfx *gMonSpritesGfx;
extern u8 gBattleOutcome;
extern u16 gLastUsedItem;
extern u32 gBattleTypeFlags;
extern struct MonSpritesGfx *gMonSpritesGfxPtr;
extern u16 gTrainerBattleOpponent_A;
extern u16 gTrainerBattleOpponent_B;
extern u16 gMoveToLearn;
extern u16 gBattleMovePower;
extern struct BattleEnigmaBerry gEnigmaBerries[MAX_BATTLERS_COUNT];
extern u16 gCurrentMove;
extern u16 gChosenMove;
extern u16 gCalledMove;
extern u8 gCritMultiplier;
extern u16 gBattleWeather;
extern u8 gLastUsedAbility;
extern u8 gBattlerInMenuId;
extern u8 gPotentialItemEffectBattler;
extern u8 gBattlersCount;
extern u16 gBattlerPartyIndexes[MAX_BATTLERS_COUNT];
extern s32 gBattleMoveDamage;
extern u16 gIntroSlideFlags;
extern u32 gTransformedPersonalities[MAX_BATTLERS_COUNT];
extern u8 gBattlerPositions[MAX_BATTLERS_COUNT];
extern u8 gHealthboxSpriteIds[MAX_BATTLERS_COUNT];
extern u8 gBattleOutcome;
extern u8 gBattleMonForms[MAX_BATTLERS_COUNT];
extern void (*gBattlerControllerFuncs[MAX_BATTLERS_COUNT])(void);
extern u32 gBattleControllerExecFlags;
extern u8 gBattleBufferA[MAX_BATTLERS_COUNT][0x200];
extern u8 gBattleBufferB[MAX_BATTLERS_COUNT][0x200];
extern u8 gActionSelectionCursor[MAX_BATTLERS_COUNT];
extern void (*gPreBattleCallback1)(void);
extern bool8 gDoingBattleAnim;
extern struct PokedudeBattlerState *gPokedudeBattlerStates[MAX_BATTLERS_COUNT];
extern u8 *gBattleAnimMons_BgTilesBuffer;
extern u8 *gBattleAnimMons_BgTilemapBuffer;
extern void (*gBattleMainFunc)(void);
extern u8 gMoveSelectionCursor[MAX_BATTLERS_COUNT];
extern u32 gUnknown_2022B54;
extern u8 gUnknown_2023DDC;
extern u8 gBattlerAttacker;
extern u8 gEffectBattler;
extern u8 gMultiHitCounter;
extern struct BattleScripting gBattleScripting;
extern u8 gBattlerFainted;
extern u32 gStatuses3[MAX_BATTLERS_COUNT];
extern u8 gSentPokesToOpponent[2];
extern const u8 *gBattlescriptCurrInstr;
extern const u8 *gSelectionBattleScripts[MAX_BATTLERS_COUNT];
extern u16 gLastMoves[MAX_BATTLERS_COUNT];
extern u8 gBattlerByTurnOrder[MAX_BATTLERS_COUNT];
extern u8 gBattleCommunication[BATTLE_COMMUNICATION_ENTRIES_COUNT];
extern u16 gSideStatuses[2];
extern u32 gHitMarker;
extern u16 gChosenMoveByBattler[MAX_BATTLERS_COUNT];
extern u8 gMoveResultFlags;
extern s32 gTakenDmg[MAX_BATTLERS_COUNT];
extern u8 gTakenDmgByBattler[MAX_BATTLERS_COUNT];
extern u8 gCurrentActionFuncId;
extern u8 gCurrMovePos;
extern u8 gChosenMovePos;
extern u8 gBattleControllerData[MAX_BATTLERS_COUNT];
extern u8 gBattlerStatusSummaryTaskId[MAX_BATTLERS_COUNT];
extern u16 gDynamicBasePower;
extern u16 gLastLandedMoves[MAX_BATTLERS_COUNT];
extern u8 gLastHitBy[MAX_BATTLERS_COUNT];
extern u8 gMultiUsePlayerCursor;
extern u8 gNumberOfMovesToChoose;
extern u16 gLastHitByType[MAX_BATTLERS_COUNT];
extern s32 gHpDealt;
extern u16 gPauseCounterBattle;
extern u16 gPaydayMoney;
extern u16 gLockedMoves[MAX_BATTLERS_COUNT];
extern u8 gCurrentTurnActionNumber;
extern u16 gExpShareExp;
extern u8 gLeveledUpInBattle;
extern u16 gLastResultingMoves[MAX_BATTLERS_COUNT];
extern u16 gLastPrintedMoves[MAX_BATTLERS_COUNT];
extern u8 gActionsByTurnOrder[MAX_BATTLERS_COUNT];
extern u8 gChosenActionByBattler[MAX_BATTLERS_COUNT];
extern u8 gBattleTerrain;
extern struct MultiBattlePokemonTx gMultiPartnerParty[3];
extern u16 gRandomTurnNumber;
extern u8 sBattleBuffersTransferData[0x100];

//new
extern u32 gBattleCircusFlags;
extern const u8* gBattleStringLoader;
extern u8 gAbilityPopUpHelper;
extern u8 gTerrainType;
extern u8 gBankSwitching;
extern bool8 gDontRemoveTransformSpecies;
extern u8 gForceSwitchHelper;
extern u8 gFormCounter;
extern u8 gMagicianHelper;
extern u8* gSeedHelper;
extern u8 gFishingByte;
extern u8 gPoisonedBy;
extern u16 gBackupHWord;
extern u8 gRaidBattleStars;
extern u8 gRaidBattleLevel;
extern u8 gRainFadeHelper;
extern u16 gRaidBattleSpecies;
extern u8 gShakerData[2];

#endif // GUARD_BATTLE_H
