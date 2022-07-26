#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_ai_script_commands.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_main.h"
#include "battle_message.h"
#include "battle_scripts.h"
#include "battle_setup.h"
#include "battle_string_ids.h"
#include "berry.h"
#include "data.h"
#include "dns.h"
#include "decompress.h"
#include "event_data.h"
#include "evolution_scene.h"
#include "graphics.h"
#include "help_system.h"
#include "item.h"
#include "link.h"
#include "link_rfu.h"
#include "load_save.h"
#include "m4a.h"
#include "party_menu.h"
#include "pokeball.h"
#include "pokedex.h"
#include "quest_log.h"
#include "random.h"
#include "mega.h"
#include "dynamax.h"
#include "frontier.h"
#include "roamer.h"
#include "field_weather.h"
#include "set_z_effect.h"
#include "safari_zone.h"
#include "scanline_effect.h"
#include "task.h"
#include "trig.h"
#include "vs_seeker.h"
#include "util.h"
#include "constants/abilities.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_setup.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "constants/trainers.h"
#include "constants/weather.h"

static void SpriteCB_UnusedDebugSprite(struct Sprite *sprite);
static void HandleAction_UseMove(void);
static void HandleAction_Switch(void);
static void HandleAction_UseItem(void);
static void HandleAction_Run(void);
static void HandleAction_WatchesCarefully(void);
static void HandleAction_SafariZoneBallThrow(void);
static void HandleAction_ThrowBait(void);
static void HandleAction_ThrowRock(void);
static void HandleAction_SafariZoneRun(void);
static void HandleAction_OldManBallThrow(void);
static void HandleAction_TryFinish(void);
static void HandleAction_NothingIsFainted(void);
static void HandleAction_ActionFinished(void);
static void HandleEndTurn_ContinueBattle(void);
static void HandleEndTurn_BattleWon(void);
static void HandleEndTurn_BattleLost(void);
static void HandleEndTurn_MonFled(void);
static void HandleEndTurn_FinishBattle(void);
static void CB2_InitBattleInternal(void);
static void CB2_PreInitMultiBattle(void);
static void CB2_HandleStartMultiBattle(void);
static u8 CreateNPCTrainerParty(struct Pokemon *party, u16 trainerNum);
static void CB2_HandleStartBattle(void);
static void TryCorrectShedinjaLanguage(struct Pokemon *mon);
static void BattleMainCB1(void);
static void CB2_QuitPokedudeBattle(void);
static void SpriteCB_UnusedDebugSprite_Step(struct Sprite *sprite);
static void CB2_EndLinkBattle(void);
static void EndLinkBattleInSteps(void);
static void SpriteCB_MoveWildMonToRight(struct Sprite *sprite);
static void SpriteCB_WildMonShowHealthbox(struct Sprite *sprite);
static void SpriteCB_Unused_8011E28_Step(struct Sprite *sprite);
static void SpriteCB_AnimFaintOpponent(struct Sprite *sprite);
static void SpriteCb_BlinkVisible(struct Sprite *sprite);
static void oac_poke_ally_(struct Sprite *sprite);
static void SpriteCallbackDummy3(struct Sprite *sprite);
static void SpriteCB_BounceEffect(struct Sprite *sprite);
static void SpriteCB_PlayerThrowUpdate(struct Sprite *sprite);
static void BattleStartClearSetData(void);
static void BattleIntroGetMonsData(void);
static void TurnValuesCleanUp(bool8 var0);
static void SpecialStatusesClear(void);
static void BattleIntroPrepareBackgroundSlide(void);
static void BattleIntroDrawTrainersOrMonsSprites(void);
static void BattleIntroDrawPartySummaryScreens(void);
static void BattleIntroPrintTrainerWantsToBattle(void);
static void BattleIntroPrintWildMonAttacked(void);
static void BattleIntroPrintOpponentSendsOut(void);
static void BattleIntroPrintPlayerSendsOut(void);
static void BattleIntroRecordMonsToDex(void);
static void BattleIntroOpponentSendsOutMonAnimation(void);
static void BattleIntroPlayerSendsOutMonAnimation(void);
static void TryDoEventsBeforeFirstTurn(void);
static void HandleTurnActionSelectionState(void);
static void RunTurnActionsFunctions(void);
static void SetActionsAndBattlersTurnOrder(void);
static void CheckFocusPunch_ClearVarsBeforeTurnStarts(void);
static void HandleEndTurn_FinishBattle(void);
static void FreeResetData_ReturnToOvOrDoEvolutions(void);
static void ReturnFromBattleToOverworld(void);
static void TryEvolvePokemon(void);
static void WaitForEvoSceneToFinish(void);
static void TrySetupRaidBossRepeatedAttack(u8 actionFuncId);
static u8 GetWhoStrikesFirstUseLastBracketCalc(u8 bank1, u8 bank2);

EWRAM_DATA u16 gBattle_BG0_X = 0;
EWRAM_DATA u16 gBattle_BG0_Y = 0;
EWRAM_DATA u16 gBattle_BG1_X = 0;
EWRAM_DATA u16 gBattle_BG1_Y = 0;
EWRAM_DATA u16 gBattle_BG2_X = 0;
EWRAM_DATA u16 gBattle_BG2_Y = 0;
EWRAM_DATA u16 gBattle_BG3_X = 0;
EWRAM_DATA u16 gBattle_BG3_Y = 0;
EWRAM_DATA u16 gBattle_WIN0H = 0;
EWRAM_DATA u16 gBattle_WIN0V = 0;
EWRAM_DATA u16 gBattle_WIN1H = 0;
EWRAM_DATA u16 gBattle_WIN1V = 0;
EWRAM_DATA u8 gDisplayedStringBattle[300] = {0};
EWRAM_DATA u8 gBattleTextBuff1[TEXT_BUFF_ARRAY_COUNT] = {0};
EWRAM_DATA u8 gBattleTextBuff2[TEXT_BUFF_ARRAY_COUNT] = {0};
EWRAM_DATA u8 gBattleTextBuff3[TEXT_BUFF_ARRAY_COUNT] = {0};
static EWRAM_DATA u32 gUnknown_2022AE8[25] = {0}; // Note: This shouldn't be removed without adjusting the size of gDisplayedStringBattle.
EWRAM_DATA u32 gBattleTypeFlags = 0;
EWRAM_DATA u8 gBattleTerrain = 0;
EWRAM_DATA u32 gUnknown_2022B54 = 0;
EWRAM_DATA struct MultiBattlePokemonTx gMultiPartnerParty[3] = {0};
EWRAM_DATA u8 *gBattleAnimMons_BgTilesBuffer = NULL;
EWRAM_DATA u8 *gBattleAnimMons_BgTilemapBuffer = NULL;
static EWRAM_DATA u16 *sUnknownDebugSpriteDataBuffer = NULL;
EWRAM_DATA u8 gBattleBufferA[MAX_BATTLERS_COUNT][0x200] = {0};
EWRAM_DATA u8 gBattleBufferB[MAX_BATTLERS_COUNT][0x200] = {0};
EWRAM_DATA u8 gActiveBattler = 0;
EWRAM_DATA u32 gBattleControllerExecFlags = 0;
EWRAM_DATA u8 gBattlersCount = 0;
EWRAM_DATA u16 gBattlerPartyIndexes[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerPositions[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gActionsByTurnOrder[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerByTurnOrder[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gCurrentTurnActionNumber = 0;
EWRAM_DATA u8 gCurrentActionFuncId = 0;
EWRAM_DATA struct BattlePokemon gBattleMons[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerSpriteIds[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gCurrMovePos = 0;
EWRAM_DATA u8 gChosenMovePos = 0;
EWRAM_DATA u16 gCurrentMove = 0;
EWRAM_DATA u16 gChosenMove = 0;
EWRAM_DATA u16 gCalledMove = 0;
EWRAM_DATA s32 gBattleMoveDamage = 0;
EWRAM_DATA s32 gHpDealt = 0;
EWRAM_DATA s32 gTakenDmg[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastUsedItem = 0;
EWRAM_DATA ability_t gLastUsedAbility = 0;
EWRAM_DATA u8 gBattlerAttacker = 0;
EWRAM_DATA u8 gBattlerTarget = 0;
EWRAM_DATA u8 gBattlerFainted = 0;
EWRAM_DATA u8 gEffectBattler = 0;
EWRAM_DATA u8 gPotentialItemEffectBattler = 0;
EWRAM_DATA u8 gAbsentBattlerFlags = 0;
EWRAM_DATA u8 gCritMultiplier = 0;
EWRAM_DATA u8 gMultiHitCounter = 0;
EWRAM_DATA const u8 *gBattlescriptCurrInstr = NULL;
EWRAM_DATA u32 gUnusedBattleMainVar = 0;
EWRAM_DATA u8 gChosenActionByBattler[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA const u8 *gSelectionBattleScripts[MAX_BATTLERS_COUNT] = {NULL};
EWRAM_DATA u16 gLastPrintedMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastLandedMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastHitByType[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastResultingMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLockedMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gLastHitBy[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gChosenMoveByBattler[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gMoveResultFlags = 0;
EWRAM_DATA u32 gHitMarker = 0;
static EWRAM_DATA u8 gUnknown_2023DD4[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gTakenDmgByBattler[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gUnknown_2023DDC = 0;
EWRAM_DATA u16 gSideStatuses[2] = {0};
EWRAM_DATA struct SideTimer gSideTimers[2] = {0};
EWRAM_DATA u32 gStatuses3[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct DisableStruct gDisableStructs[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gPauseCounterBattle = 0;
EWRAM_DATA u16 gPaydayMoney = 0;
EWRAM_DATA u16 gRandomTurnNumber = 0;
EWRAM_DATA u8 gBattleCommunication[BATTLE_COMMUNICATION_ENTRIES_COUNT] = {0};
EWRAM_DATA u8 gBattleOutcome = 0;
EWRAM_DATA struct ProtectStruct gProtectStructs[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct SpecialStatus gSpecialStatuses[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gBattleWeather = 0;
EWRAM_DATA struct WishFutureKnock gWishFutureKnock = {0};
EWRAM_DATA u16 gIntroSlideFlags = 0;
EWRAM_DATA u8 gSentPokesToOpponent[2] = {0};
EWRAM_DATA u16 gDynamicBasePower = 0;
EWRAM_DATA u16 gExpShareExp = 0;
EWRAM_DATA struct BattleEnigmaBerry gEnigmaBerries[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct BattleScripting gBattleScripting = {0};
EWRAM_DATA struct BattleStruct *gBattleStruct = NULL;
EWRAM_DATA u8 *gLinkBattleSendBuffer = NULL;
EWRAM_DATA u8 *gLinkBattleRecvBuffer = NULL;
EWRAM_DATA struct BattleResources *gBattleResources = NULL;
EWRAM_DATA u8 gActionSelectionCursor[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gMoveSelectionCursor[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerStatusSummaryTaskId[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerInMenuId = 0;
EWRAM_DATA bool8 gDoingBattleAnim = FALSE;
EWRAM_DATA u32 gTransformedPersonalities[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct BattleSpriteData *gBattleSpritesDataPtr = NULL;
EWRAM_DATA struct MonSpritesGfx *gMonSpritesGfxPtr = NULL;
EWRAM_DATA u16 gBattleMovePower = 0;
EWRAM_DATA u16 gMoveToLearn = 0;
EWRAM_DATA u8 gBattleMonForms[MAX_BATTLERS_COUNT] = {0};

//new
EWRAM_DATA u32 gBattleCircusFlags = 0;
EWRAM_DATA const u8* gBattleStringLoader = 0;
EWRAM_DATA u8* gSeedHelper = 0;
EWRAM_DATA u8 gAbilityPopUpHelper = 0;
EWRAM_DATA u8 gTerrainType = 0;
EWRAM_DATA u8 gBankSwitching = 0;
EWRAM_DATA bool8 gDontRemoveTransformSpecies = 0;
EWRAM_DATA u8 gForceSwitchHelper = 0;
EWRAM_DATA u8 gFormCounter = 0;
EWRAM_DATA u8 gMagicianHelper = 0;
EWRAM_DATA u8 gPoisonedBy = 0;
EWRAM_DATA u8 gFishingByte = 0;
EWRAM_DATA u16 gBackupHWord = 0;
EWRAM_DATA u8 gRaidBattleStars = 0;
EWRAM_DATA u8 gRainFadeHelper = 0;
EWRAM_DATA u8 gRaidBattleLevel = 0;
EWRAM_DATA u16 gRaidBattleSpecies = 0;
EWRAM_DATA u8 gShakerData[2] = {0};
EWRAM_DATA u8 gDexNavStartedBattle = 0;
EWRAM_DATA u8 gCurrentDexNavChain = 0;

void (*gPreBattleCallback1)(void);
void (*gBattleMainFunc)(void);
struct BattleResults gBattleResults;
u8 gLeveledUpInBattle;
void (*gBattlerControllerFuncs[MAX_BATTLERS_COUNT])(void);
u8 gHealthboxSpriteIds[MAX_BATTLERS_COUNT];
u8 gMultiUsePlayerCursor;
u8 gNumberOfMovesToChoose;
u8 gBattleControllerData[MAX_BATTLERS_COUNT];
struct BattleExtensionState ExtensionState;

static const struct ScanlineEffectParams sIntroScanlineParams16Bit =
{
    &REG_BG3HOFS, SCANLINE_EFFECT_DMACNT_16BIT, 1
};


const u8 gText_DynamaxUsable[] = _("Dynamax Energy gathered\naround you!");

const struct SpriteTemplate gUnknownDebugSprite =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_UnusedDebugSprite,
};

static const u8 sText_ShedinjaJpnName[] = _("ヌケニン"); // Nukenin

const struct OamData gOamData_BattlerOpponent =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0,
};

const struct OamData gOamData_BattlerPlayer =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 2,
    .affineParam = 0,
};

// not used
static const union AnimCmd gUnknown_824F020[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_JUMP(0),
};

// not used
static const union AnimCmd *const gUnknown_824F028[] =
{
    gUnknown_824F020,
};

// not used
static const union AffineAnimCmd gUnknown_824F02C[] =
{
    AFFINEANIMCMD_FRAME(-0x10, 0x0, 0, 4),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 0x3C),
    AFFINEANIMCMD_JUMP(1),
};

// not used
static const union AffineAnimCmd *const gUnknown_824F044[] =
{
    gUnknown_824F02C,
};

static const s8 sPlayerThrowXTranslation[] = { -32, -16, -16, -32, -32, 0, 0, 0 };

// format: attacking type, defending type, damage multiplier
// the multiplier is a (decimal) fixed-point number:
// 20 is ×2.0 TYPE_MUL_SUPER_EFFECTIVE
// 10 is ×1.0 TYPE_MUL_NORMAL
// 05 is ×0.5 TYPE_MUL_NOT_EFFECTIVE
// 00 is ×0.0 TYPE_MUL_NO_EFFECT
const u8 gTypeEffectiveness[336] =
{
    TYPE_NORMAL, TYPE_ROCK, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_NORMAL, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIRE, TYPE_FIRE, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIRE, TYPE_WATER, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIRE, TYPE_GRASS, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIRE, TYPE_ICE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIRE, TYPE_BUG, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIRE, TYPE_ROCK, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIRE, TYPE_DRAGON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIRE, TYPE_STEEL, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_WATER, TYPE_FIRE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_WATER, TYPE_WATER, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_WATER, TYPE_GRASS, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_WATER, TYPE_GROUND, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_WATER, TYPE_ROCK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_WATER, TYPE_DRAGON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ELECTRIC, TYPE_WATER, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ELECTRIC, TYPE_ELECTRIC, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ELECTRIC, TYPE_GRASS, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ELECTRIC, TYPE_GROUND, TYPE_MUL_NO_EFFECT,
    TYPE_ELECTRIC, TYPE_FLYING, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ELECTRIC, TYPE_DRAGON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_FIRE, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_WATER, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GRASS, TYPE_GRASS, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_POISON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_GROUND, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GRASS, TYPE_FLYING, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_BUG, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_ROCK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GRASS, TYPE_DRAGON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GRASS, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ICE, TYPE_WATER, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ICE, TYPE_GRASS, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ICE, TYPE_ICE, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ICE, TYPE_GROUND, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ICE, TYPE_FLYING, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ICE, TYPE_DRAGON, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ICE, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ICE, TYPE_FIRE, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIGHTING, TYPE_NORMAL, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIGHTING, TYPE_ICE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIGHTING, TYPE_POISON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIGHTING, TYPE_FLYING, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIGHTING, TYPE_PSYCHIC, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIGHTING, TYPE_BUG, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FIGHTING, TYPE_ROCK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIGHTING, TYPE_DARK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FIGHTING, TYPE_STEEL, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_POISON, TYPE_GRASS, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_POISON, TYPE_POISON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_POISON, TYPE_GROUND, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_POISON, TYPE_ROCK, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_POISON, TYPE_GHOST, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_POISON, TYPE_STEEL, TYPE_MUL_NO_EFFECT,
    TYPE_GROUND, TYPE_FIRE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GROUND, TYPE_ELECTRIC, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GROUND, TYPE_GRASS, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GROUND, TYPE_POISON, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GROUND, TYPE_FLYING, TYPE_MUL_NO_EFFECT,
    TYPE_GROUND, TYPE_BUG, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GROUND, TYPE_ROCK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GROUND, TYPE_STEEL, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FLYING, TYPE_ELECTRIC, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FLYING, TYPE_GRASS, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FLYING, TYPE_FIGHTING, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FLYING, TYPE_BUG, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_FLYING, TYPE_ROCK, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FLYING, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_PSYCHIC, TYPE_FIGHTING, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_PSYCHIC, TYPE_POISON, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_PSYCHIC, TYPE_PSYCHIC, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_PSYCHIC, TYPE_DARK, TYPE_MUL_NO_EFFECT,
    TYPE_PSYCHIC, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_BUG, TYPE_FIRE, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_BUG, TYPE_GRASS, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_BUG, TYPE_FIGHTING, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_BUG, TYPE_POISON, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_BUG, TYPE_FLYING, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_BUG, TYPE_PSYCHIC, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_BUG, TYPE_GHOST, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_BUG, TYPE_DARK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_BUG, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ROCK, TYPE_FIRE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ROCK, TYPE_ICE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ROCK, TYPE_FIGHTING, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ROCK, TYPE_GROUND, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_ROCK, TYPE_FLYING, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ROCK, TYPE_BUG, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_ROCK, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GHOST, TYPE_NORMAL, TYPE_MUL_NO_EFFECT,
    TYPE_GHOST, TYPE_PSYCHIC, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_GHOST, TYPE_DARK, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GHOST, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_GHOST, TYPE_GHOST, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_DRAGON, TYPE_DRAGON, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_DRAGON, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_DARK, TYPE_FIGHTING, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_DARK, TYPE_PSYCHIC, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_DARK, TYPE_GHOST, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_DARK, TYPE_DARK, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_DARK, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_STEEL, TYPE_FIRE, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_STEEL, TYPE_WATER, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_STEEL, TYPE_ELECTRIC, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_STEEL, TYPE_ICE, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_STEEL, TYPE_ROCK, TYPE_MUL_SUPER_EFFECTIVE,
    TYPE_STEEL, TYPE_STEEL, TYPE_MUL_NOT_EFFECTIVE,
    TYPE_FORESIGHT, TYPE_FORESIGHT, TYPE_MUL_NO_EFFECT,
    TYPE_NORMAL, TYPE_GHOST, TYPE_MUL_NO_EFFECT,
    TYPE_FIGHTING, TYPE_GHOST, TYPE_MUL_NO_EFFECT,
    TYPE_ENDTABLE, TYPE_ENDTABLE, TYPE_MUL_NO_EFFECT
};

const u8 gTypeNames[][TYPE_NAME_LENGTH + 1] =
{
    _("普通"),
    _("格斗"),
    _("飞行"),
    _("毒"),
    _("地面"),
    _("岩石"),
    _("虫"),
    _("幽灵"),
    _("钢"),
    _("???"),
    _("火"),
    _("水"),
    _("草"),
    _("电"),
    _("超能"),
    _("冰"),
    _("龙"),
    _("恶"),
    _("妖精"),
};

// This is a factor in how much money you get for beating a trainer.
const struct TrainerMoney gTrainerMoneyTable[] =
{
    { TRAINER_CLASS_LEADER, 25 },
    { TRAINER_CLASS_ELITE_FOUR, 25 },
    { TRAINER_CLASS_PKMN_PROF, 25 },
    { TRAINER_CLASS_RIVAL_EARLY, 4 },
    { TRAINER_CLASS_RIVAL_LATE, 9 },
    { TRAINER_CLASS_CHAMPION, 25 },
    { TRAINER_CLASS_YOUNGSTER, 4 },
    { TRAINER_CLASS_BUG_CATCHER, 3 },
    { TRAINER_CLASS_HIKER, 9 },
    { TRAINER_CLASS_BIRD_KEEPER, 6 },
    { TRAINER_CLASS_PICNICKER, 5 },
    { TRAINER_CLASS_SUPER_NERD, 6 },
    { TRAINER_CLASS_FISHERMAN, 9 },
    { TRAINER_CLASS_TEAM_ROCKET, 8 },
    { TRAINER_CLASS_LASS, 4 },
    { TRAINER_CLASS_BEAUTY, 18 },
    { TRAINER_CLASS_BLACK_BELT, 6 },
    { TRAINER_CLASS_CUE_BALL, 6 },
    { TRAINER_CLASS_CHANNELER, 8 },
    { TRAINER_CLASS_ROCKER, 6 },
    { TRAINER_CLASS_GENTLEMAN, 18 },
    { TRAINER_CLASS_BURGLAR, 22 },
    { TRAINER_CLASS_SWIMMER_M, 1 },
    { TRAINER_CLASS_ENGINEER, 12 },
    { TRAINER_CLASS_JUGGLER, 10 },
    { TRAINER_CLASS_SAILOR, 8 },
    { TRAINER_CLASS_COOLTRAINER, 9 },
    { TRAINER_CLASS_POKEMANIAC, 12 },
    { TRAINER_CLASS_TAMER, 10 },
    { TRAINER_CLASS_CAMPER, 5 },
    { TRAINER_CLASS_PSYCHIC, 5 },
    { TRAINER_CLASS_BIKER, 5 },
    { TRAINER_CLASS_GAMER, 18 },
    { TRAINER_CLASS_SCIENTIST, 12 },
    { TRAINER_CLASS_CRUSH_GIRL, 6 },
    { TRAINER_CLASS_TUBER, 1 },
    { TRAINER_CLASS_PKMN_BREEDER, 7 },
    { TRAINER_CLASS_PKMN_RANGER, 9 },
    { TRAINER_CLASS_AROMA_LADY, 7 },
    { TRAINER_CLASS_RUIN_MANIAC, 12 },
    { TRAINER_CLASS_LADY, 50 },
    { TRAINER_CLASS_PAINTER, 4 },
    { TRAINER_CLASS_TWINS, 3 },
    { TRAINER_CLASS_YOUNG_COUPLE, 7 },
    { TRAINER_CLASS_SIS_AND_BRO, 1 },
    { TRAINER_CLASS_COOL_COUPLE, 6 },
    { TRAINER_CLASS_CRUSH_KIN, 6 },
    { TRAINER_CLASS_SWIMMER_F, 1 },
    { TRAINER_CLASS_PLAYER, 1 },
    { TRAINER_CLASS_RS_LEADER, 25 },
    { TRAINER_CLASS_RS_ELITE_FOUR, 25 },
    { TRAINER_CLASS_RS_LASS, 4 },
    { TRAINER_CLASS_RS_YOUNGSTER, 4 },
    { TRAINER_CLASS_PKMN_TRAINER, 15 },
    { TRAINER_CLASS_RS_HIKER, 10 },
    { TRAINER_CLASS_RS_BEAUTY, 20 },
    { TRAINER_CLASS_RS_FISHERMAN, 10 },
    { TRAINER_CLASS_RS_LADY, 50 },
    { TRAINER_CLASS_TRIATHLETE, 10 },
    { TRAINER_CLASS_TEAM_AQUA, 5 },
    { TRAINER_CLASS_RS_TWINS, 3 },
    { TRAINER_CLASS_RS_SWIMMER_F, 2 },
    { TRAINER_CLASS_RS_BUG_CATCHER, 4 },
    { TRAINER_CLASS_SCHOOL_KID, 5 },
    { TRAINER_CLASS_RICH_BOY, 50 },
    { TRAINER_CLASS_SR_AND_JR, 4 },
    { TRAINER_CLASS_RS_BLACK_BELT, 8 },
    { TRAINER_CLASS_RS_TUBER_F, 1 },
    { TRAINER_CLASS_HEX_MANIAC, 6 },
    { TRAINER_CLASS_RS_PKMN_BREEDER, 10 },
    { TRAINER_CLASS_TEAM_MAGMA, 5 },
    { TRAINER_CLASS_INTERVIEWER, 12 },
    { TRAINER_CLASS_RS_TUBER_M, 1 },
    { TRAINER_CLASS_RS_YOUNG_COUPLE, 8 },
    { TRAINER_CLASS_GUITARIST, 8 },
    { TRAINER_CLASS_RS_GENTLEMAN, 20 },
    { TRAINER_CLASS_RS_CHAMPION, 50 },
    { TRAINER_CLASS_MAGMA_LEADER, 20 },
    { TRAINER_CLASS_BATTLE_GIRL, 6 },
    { TRAINER_CLASS_RS_SWIMMER_M, 2 },
    { TRAINER_CLASS_POKEFAN, 20 },
    { TRAINER_CLASS_EXPERT, 10 },
    { TRAINER_CLASS_DRAGON_TAMER, 12 },
    { TRAINER_CLASS_RS_BIRD_KEEPER, 8 },
    { TRAINER_CLASS_NINJA_BOY, 3 },
    { TRAINER_CLASS_PARASOL_LADY, 10 },
    { TRAINER_CLASS_BUG_MANIAC, 15 },
    { TRAINER_CLASS_RS_SAILOR, 8 },
    { TRAINER_CLASS_COLLECTOR, 15 },
    { TRAINER_CLASS_RS_PKMN_RANGER, 12 },
    { TRAINER_CLASS_MAGMA_ADMIN, 10 },
    { TRAINER_CLASS_RS_AROMA_LADY, 10 },
    { TRAINER_CLASS_RS_RUIN_MANIAC, 15 },
    { TRAINER_CLASS_RS_COOLTRAINER, 12 },
    { TRAINER_CLASS_RS_POKEMANIAC, 15 },
    { TRAINER_CLASS_KINDLER, 8 },
    { TRAINER_CLASS_RS_CAMPER, 4 },
    { TRAINER_CLASS_RS_PICNICKER, 4 },
    { TRAINER_CLASS_RS_PSYCHIC, 6 },
    { TRAINER_CLASS_RS_SIS_AND_BRO, 3 },
    { TRAINER_CLASS_OLD_COUPLE, 10 },
    { TRAINER_CLASS_AQUA_ADMIN, 10 },
    { TRAINER_CLASS_AQUA_LEADER, 20 },
    { TRAINER_CLASS_BOSS, 25 },
    { 0xFF, 5 },
};

#include "data/text/abilities.h"

static void (*const sTurnActionsFuncsTable[])(void) =
{
    [B_ACTION_USE_MOVE] = HandleAction_UseMove,
    [B_ACTION_USE_ITEM] = HandleAction_UseItem,
    [B_ACTION_SWITCH] = HandleAction_Switch,
    [B_ACTION_RUN] = HandleAction_Run,
    [B_ACTION_SAFARI_WATCH_CAREFULLY] = HandleAction_WatchesCarefully,
    [B_ACTION_SAFARI_BALL] = HandleAction_SafariZoneBallThrow,
    [B_ACTION_SAFARI_BAIT] = HandleAction_ThrowBait,
    [B_ACTION_SAFARI_GO_NEAR] = HandleAction_ThrowRock,
    [B_ACTION_SAFARI_RUN] = HandleAction_SafariZoneRun,
    [B_ACTION_OLDMAN_THROW] = HandleAction_OldManBallThrow,
    [B_ACTION_EXEC_SCRIPT] = HandleAction_RunBattleScript,
    [B_ACTION_TRY_FINISH] = HandleAction_TryFinish,
    [B_ACTION_FINISHED] = HandleAction_ActionFinished,
    [B_ACTION_NOTHING_FAINTED] = HandleAction_NothingIsFainted,
};

static void (*const sEndTurnFuncsTable[])(void) =
{
    [0] = HandleEndTurn_ContinueBattle, //B_OUTCOME_NONE?
    [B_OUTCOME_WON] = HandleEndTurn_BattleWon,
    [B_OUTCOME_LOST] = HandleEndTurn_BattleLost,
    [B_OUTCOME_DREW] = HandleEndTurn_BattleLost,
    [B_OUTCOME_RAN] = HandleEndTurn_RanFromBattle,
    [B_OUTCOME_PLAYER_TELEPORTED] = HandleEndTurn_FinishBattle,
    [B_OUTCOME_MON_FLED] = HandleEndTurn_MonFled,
    [B_OUTCOME_CAUGHT] = HandleEndTurn_FinishBattle,
    [B_OUTCOME_NO_SAFARI_BALLS] = HandleEndTurn_FinishBattle,
};

const u8 gConsumableItemEffects[] =
{
	HOLD_EFFECT_RESTORE_HP,
	HOLD_EFFECT_CURE_PAR,
	HOLD_EFFECT_CURE_SLP,
	HOLD_EFFECT_CURE_PSN,
	HOLD_EFFECT_CURE_BRN,
	HOLD_EFFECT_CURE_FRZ,
	HOLD_EFFECT_RESTORE_PP,
	HOLD_EFFECT_CURE_CONFUSION,
	HOLD_EFFECT_CURE_STATUS,
	HOLD_EFFECT_CONFUSE_SPICY,
	HOLD_EFFECT_CONFUSE_DRY,
	HOLD_EFFECT_CONFUSE_SWEET,
	HOLD_EFFECT_CONFUSE_BITTER,
	HOLD_EFFECT_CONFUSE_SOUR,
	HOLD_EFFECT_ATTACK_UP,
	HOLD_EFFECT_DEFENSE_UP,
	HOLD_EFFECT_SPEED_UP,
	HOLD_EFFECT_SP_ATTACK_UP,
	HOLD_EFFECT_SP_DEFENSE_UP,
	HOLD_EFFECT_CRITICAL_UP,
	HOLD_EFFECT_RANDOM_STAT_UP,
	HOLD_EFFECT_RESTORE_STATS,
	HOLD_EFFECT_CURE_ATTRACT,
	HOLD_EFFECT_WEAKNESS_POLICY,
	HOLD_EFFECT_GEM,
	HOLD_EFFECT_WEAKNESS_BERRY,
	HOLD_EFFECT_CUSTAP_BERRY,
	HOLD_EFFECT_ABSORB_BULB,
	HOLD_EFFECT_AIR_BALLOON,
	HOLD_EFFECT_CELL_BATTERY,
	HOLD_EFFECT_EJECT_BUTTON,
	HOLD_EFFECT_LUMINOUS_MOSS,
	HOLD_EFFECT_RED_CARD,
	HOLD_EFFECT_SNOWBALL,
	HOLD_EFFECT_SEEDS,
	HOLD_EFFECT_JABOCA_ROWAP_BERRY,
	HOLD_EFFECT_KEE_BERRY,
	HOLD_EFFECT_MARANGA_BERRY,
	HOLD_EFFECT_ADRENALINE_ORB,
	HOLD_EFFECT_POWER_HERB,
	HOLD_EFFECT_MICLE_BERRY,
	HOLD_EFFECT_ENIGMA_BERRY,
	HOLD_EFFECT_EJECT_PACK,
	HOLD_EFFECT_ROOM_SERVICE,
	HOLD_EFFECT_BLUNDER_POLICY,
	HOLD_EFFECT_HEAVY_DUTY_BOOTS,
	HOLD_EFFECT_UTILITY_UMBRELLA,
	HOLD_EFFECT_THROAT_SPRAY,
	0xFF,
};

const u8 gStatusConditionString_PoisonJpn[8] = _("どく$$$$$");
const u8 gStatusConditionString_SleepJpn[8] = _("ねむり$$$$");
const u8 gStatusConditionString_ParalysisJpn[8] = _("まひ$$$$$");
const u8 gStatusConditionString_BurnJpn[8] = _("やけど$$$$");
const u8 gStatusConditionString_IceJpn[8] = _("こおり$$$$");
const u8 gStatusConditionString_ConfusionJpn[8] = _("こんらん$$$");
const u8 gStatusConditionString_LoveJpn[8] = _("メロメロ$$$");

const u8 *const gStatusConditionStringsTable[7][2] =
{
    { gStatusConditionString_PoisonJpn, gText_Poison },
    { gStatusConditionString_SleepJpn, gText_Sleep },
    { gStatusConditionString_ParalysisJpn, gText_Paralysis },
    { gStatusConditionString_BurnJpn, gText_Burn },
    { gStatusConditionString_IceJpn, gText_Ice },
    { gStatusConditionString_ConfusionJpn, gText_Confusion },
    { gStatusConditionString_LoveJpn, gText_Love }
};

void CB2_InitBattle(void)
{
    MoveSaveBlocks_ResetHeap();
    AllocateBattleResources();
    AllocateBattleSpritesData();
    AllocateMonSpritesGfx();
    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        HandleLinkBattleSetup();
        SetMainCallback2(CB2_PreInitMultiBattle);
        gBattleCommunication[MULTIUSE_STATE] = 0;
    }
    else
    {
        CB2_InitBattleInternal();
        if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
        {
            if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                    SetHelpContext(HELPCONTEXT_TRAINER_BATTLE_DOUBLE);
                else
                    SetHelpContext(HELPCONTEXT_TRAINER_BATTLE_SINGLE);
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
            {
                SetHelpContext(HELPCONTEXT_SAFARI_BATTLE);
            }
            else
            {
                SetHelpContext(HELPCONTEXT_WILD_BATTLE);
            }
        }
    }
}

static void CB2_InitBattleInternal(void)
{
    s32 i;

    SetHBlankCallback(NULL);
    SetVBlankCallback(NULL);
    CpuFill32(0, (void *)VRAM, VRAM_SIZE);
    SetGpuReg(REG_OFFSET_MOSAIC, 0);
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 0xF0));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0x50, 0x51));
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    gBattle_WIN0H = WIN_RANGE(0, 0xF0);
    gBattle_WIN0V = WIN_RANGE(0x50, 0x51);
    ScanlineEffect_Clear();
    for (i = 0; i < 80; ++i)
    {
        gScanlineEffectRegBuffers[0][i] = 0xF0;
        gScanlineEffectRegBuffers[1][i] = 0xF0;
    }
    for (; i < 160; ++i)
    {
        gScanlineEffectRegBuffers[0][i] = 0xFF10;
        gScanlineEffectRegBuffers[1][i] = 0xFF10;
    }
    ScanlineEffect_SetParams(sIntroScanlineParams16Bit);
    ResetPaletteFade();
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    gBattle_BG2_X = 0;
    gBattle_BG2_Y = 0;
    gBattle_BG3_X = 0;
    gBattle_BG3_Y = 0;
    gBattleTerrain = BattleSetup_GetTerrainId();
    InitBattleBgsVideo();
    LoadBattleTextboxAndBackground();
    ResetSpriteData();
    ResetTasks();
    DrawBattleEntryBackground();
    FreeAllSpritePalettes();
    gReservedSpritePaletteCount = 4;
    SetVBlankCallback(VBlankCB_Battle);
    SetUpBattleVars();
    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        SetMainCallback2(CB2_HandleStartMultiBattle);
    else
        SetMainCallback2(CB2_HandleStartBattle);
    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        CreateNPCTrainerParty(&gEnemyParty[0], gTrainerBattleOpponent_A);
        SetWildMonHeldItem();
    }
    gMain.inBattle = TRUE;
    for (i = 0; i < PARTY_SIZE; ++i)
        AdjustFriendship(&gPlayerParty[i], FRIENDSHIP_EVENT_LEAGUE_BATTLE);
    gBattleCommunication[MULTIUSE_STATE] = 0;
}

#define BUFFER_PARTY_VS_SCREEN_STATUS(party, flags, i)              \
    for ((i) = 0; (i) < PARTY_SIZE; (i)++)                          \
    {                                                               \
        u16 species = GetMonData(&(party)[(i)], MON_DATA_SPECIES2); \
        u16 hp = GetMonData(&(party)[(i)], MON_DATA_HP);            \
        u32 status = GetMonData(&(party)[(i)], MON_DATA_STATUS);    \
                                                                    \
        if (species == SPECIES_NONE)                                \
            continue;                                               \
                                                                    \
        /* Is healthy mon? */                                       \
        if (species != SPECIES_EGG && hp != 0 && status == 0)       \
            (flags) |= 1 << (i) * 2;                                \
                                                                    \
        if (species == SPECIES_NONE) /* Redundant */                \
            continue;                                               \
                                                                    \
        /* Is Egg or statused? */                                   \
        if (hp != 0 && (species == SPECIES_EGG || status != 0))     \
            (flags) |= 2 << (i) * 2;                                \
                                                                    \
        if (species == SPECIES_NONE) /* Redundant */                \
            continue;                                               \
                                                                    \
        /* Is fainted? */                                           \
        if (species != SPECIES_EGG && hp == 0)                      \
            (flags) |= 3 << (i) * 2;                                \
    }

static void BufferPartyVsScreenHealth_AtStart(void)
{
    u16 flags = 0;
    s32 i;

    BUFFER_PARTY_VS_SCREEN_STATUS(gPlayerParty, flags, i);
    gBattleStruct->multiBuffer.linkPartnerHeader.vsScreenHealthFlagsLo = flags;
    *(&gBattleStruct->multiBuffer.linkPartnerHeader.vsScreenHealthFlagsHi) = flags >> 8;
}

static void SetPlayerBerryDataInBattleStruct(void)
{
    s32 i;
    struct BattleStruct *battleStruct = gBattleStruct;
    struct BattleEnigmaBerry *battleBerry = &battleStruct->multiBuffer.linkPartnerHeader.battleEnigmaBerry;

    if (IsEnigmaBerryValid() == TRUE)
    {
        for (i = 0; i < BERRY_NAME_LENGTH; ++i)
            battleBerry->name[i] = gSaveBlock1Ptr->enigmaBerry.berry.name[i];
        battleBerry->name[i] = EOS;
        for (i = 0; i < BERRY_ITEM_EFFECT_COUNT; ++i)
            battleBerry->itemEffect[i] = gSaveBlock1Ptr->enigmaBerry.itemEffect[i];
        battleBerry->holdEffect = gSaveBlock1Ptr->enigmaBerry.holdEffect;
        battleBerry->holdEffectParam = gSaveBlock1Ptr->enigmaBerry.holdEffectParam;
    }
    else
    {
        const struct Berry *berryData = GetBerryInfo(ItemIdToBerryType(ITEM_ENIGMA_BERRY));

        for (i = 0; i < BERRY_NAME_LENGTH; ++i)
            battleBerry->name[i] = berryData->name[i];
        battleBerry->name[i] = EOS;
        for (i = 0; i < BERRY_ITEM_EFFECT_COUNT; ++i)
            battleBerry->itemEffect[i] = 0;
        battleBerry->holdEffect = HOLD_EFFECT_NONE;
        battleBerry->holdEffectParam = 0;
    }
}

static void SetAllPlayersBerryData(void)
{
    s32 i, j;

    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        if (IsEnigmaBerryValid() == TRUE)
        {
            for (i = 0; i < BERRY_NAME_LENGTH; ++i)
            {
                gEnigmaBerries[0].name[i] = gSaveBlock1Ptr->enigmaBerry.berry.name[i];
                gEnigmaBerries[2].name[i] = gSaveBlock1Ptr->enigmaBerry.berry.name[i];
            }
            gEnigmaBerries[0].name[i] = EOS;
            gEnigmaBerries[2].name[i] = EOS;
            for (i = 0; i < BERRY_ITEM_EFFECT_COUNT; ++i)
            {
                gEnigmaBerries[0].itemEffect[i] = gSaveBlock1Ptr->enigmaBerry.itemEffect[i];
                gEnigmaBerries[2].itemEffect[i] = gSaveBlock1Ptr->enigmaBerry.itemEffect[i];
            }
            gEnigmaBerries[0].holdEffect = gSaveBlock1Ptr->enigmaBerry.holdEffect;
            gEnigmaBerries[2].holdEffect = gSaveBlock1Ptr->enigmaBerry.holdEffect;
            gEnigmaBerries[0].holdEffectParam = gSaveBlock1Ptr->enigmaBerry.holdEffectParam;
            gEnigmaBerries[2].holdEffectParam = gSaveBlock1Ptr->enigmaBerry.holdEffectParam;
        }
        else
        {
            const struct Berry *berryData = GetBerryInfo(ItemIdToBerryType(ITEM_ENIGMA_BERRY));

            for (i = 0; i < BERRY_NAME_LENGTH; ++i)
            {
                gEnigmaBerries[0].name[i] = berryData->name[i];
                gEnigmaBerries[2].name[i] = berryData->name[i];
            }
            gEnigmaBerries[0].name[i] = EOS;
            gEnigmaBerries[2].name[i] = EOS;
            for (i = 0; i < BERRY_ITEM_EFFECT_COUNT; ++i)
            {
                gEnigmaBerries[0].itemEffect[i] = 0;
                gEnigmaBerries[2].itemEffect[i] = 0;
            }
            gEnigmaBerries[0].holdEffect = HOLD_EFFECT_NONE;
            gEnigmaBerries[2].holdEffect = HOLD_EFFECT_NONE;
            gEnigmaBerries[0].holdEffectParam = 0;
            gEnigmaBerries[2].holdEffectParam = 0;
        }
    }
    else
    {
        s32 numPlayers;
        struct BattleEnigmaBerry *src;
        u8 battlerId;

        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            for (i = 0; i < 4; ++i)
            {
                src = (struct BattleEnigmaBerry *)(gBlockRecvBuffer[i] + 2);
                battlerId = gLinkPlayers[i].id;
                for (j = 0; j < BERRY_NAME_LENGTH; ++j)
                    gEnigmaBerries[battlerId].name[j] = src->name[j];
                gEnigmaBerries[battlerId].name[j] = EOS;
                for (j = 0; j < BERRY_ITEM_EFFECT_COUNT; ++j)
                    gEnigmaBerries[battlerId].itemEffect[j] = src->itemEffect[j];
                gEnigmaBerries[battlerId].holdEffect = src->holdEffect;
                gEnigmaBerries[battlerId].holdEffectParam = src->holdEffectParam;
            }
        }
        else
        {
            for (i = 0; i < 2; ++i)
            {
                src = (struct BattleEnigmaBerry *)(gBlockRecvBuffer[i] + 2);
                for (j = 0; j < BERRY_NAME_LENGTH; ++j)
                {
                    gEnigmaBerries[i].name[j] = src->name[j];
                    gEnigmaBerries[i + 2].name[j] = src->name[j];
                }
                gEnigmaBerries[i].name[j] = EOS;
                gEnigmaBerries[i + 2].name[j] = EOS;
                for (j = 0; j < BERRY_ITEM_EFFECT_COUNT; ++j)
                {
                    gEnigmaBerries[i].itemEffect[j] = src->itemEffect[j];
                    gEnigmaBerries[i + 2].itemEffect[j] = src->itemEffect[j];
                }
                gEnigmaBerries[i].holdEffect = src->holdEffect;
                gEnigmaBerries[i + 2].holdEffect = src->holdEffect;
                gEnigmaBerries[i].holdEffectParam = src->holdEffectParam;
                gEnigmaBerries[i + 2].holdEffectParam = src->holdEffectParam;
            }
        }
    }
}

static void LinkBattleComputeBattleTypeFlags(u8 numPlayers, u8 multiPlayerId)
{
    u8 found = 0;

    // If player 1 is playing the minimum version, player 1 is master.
    if (gBlockRecvBuffer[0][0] == 0x100)
    {
        if (multiPlayerId == 0)
            gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER | BATTLE_TYPE_TRAINER;
        else
            gBattleTypeFlags |= BATTLE_TYPE_TRAINER;
        ++found;
    }
    if (found == 0)
    {
        // If multiple different versions are being used, player 1 is master.
        s32 i;

        for (i = 0; i < numPlayers; ++i)
            if (gBlockRecvBuffer[0][0] != gBlockRecvBuffer[i][0])
                break;
        if (i == numPlayers)
        {
            if (multiPlayerId == 0)
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER | BATTLE_TYPE_TRAINER;
            else
                gBattleTypeFlags |= BATTLE_TYPE_TRAINER;
            ++found;
        }
        if (found == 0)
        {
            // Lowest index player with the highest game version is master.
            for (i = 0; i < numPlayers; ++i)
            {
                if (gBlockRecvBuffer[i][0] == 0x201 && i != multiPlayerId)
                    if (i < multiPlayerId)
                        break;
                if (gBlockRecvBuffer[i][0] > 0x201 && i != multiPlayerId)
                    break;
            }
            if (i == numPlayers)
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER | BATTLE_TYPE_TRAINER;
            else
                gBattleTypeFlags |= BATTLE_TYPE_TRAINER;
        }
    }
}

static void CB2_HandleStartBattle(void)
{
    u8 playerMultiplayerId;
    u8 enemyMultiplayerId;

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    playerMultiplayerId = GetMultiplayerId();
    gBattleStruct->multiplayerId = playerMultiplayerId;
    enemyMultiplayerId = playerMultiplayerId ^ BIT_SIDE;
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            ShowBg(0);
            ShowBg(1);
            ShowBg(2);
            ShowBg(3);
            BattleInterfaceSetWindowPals();
            gBattleCommunication[MULTIUSE_STATE] = 1;
        }
        if (gWirelessCommType)
            LoadWirelessStatusIndicatorSpriteGfx();
        break;
    case 1:
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            if (gReceivedRemoteLinkPlayers)
            {
                if (IsLinkTaskFinished())
                {
                    // 0x201
                    *(&gBattleStruct->multiBuffer.linkPartnerHeader.versionSignatureLo) = 1;
                    *(&gBattleStruct->multiBuffer.linkPartnerHeader.versionSignatureHi) = 2;
                    BufferPartyVsScreenHealth_AtStart();
                    SetPlayerBerryDataInBattleStruct();
                    SendBlock(bitmask_all_link_players_but_self(), &gBattleStruct->multiBuffer.linkPartnerHeader, sizeof(gBattleStruct->multiBuffer.linkPartnerHeader));
                    gBattleCommunication[MULTIUSE_STATE] = 2;
                }
                if (gWirelessCommType != 0)
                    CreateWirelessStatusIndicatorSprite(0, 0);
            }
        }
        else
        {
            gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER;
            gBattleCommunication[MULTIUSE_STATE] = 15;
            SetAllPlayersBerryData();
        }
        break;
    case 2:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            u8 taskId;

            ResetBlockReceivedFlags();
            LinkBattleComputeBattleTypeFlags(2, playerMultiplayerId);
            SetAllPlayersBerryData();
            taskId = CreateTask(InitLinkBattleVsScreen, 0);
            gTasks[taskId].data[1] = 270;
            gTasks[taskId].data[2] = 90;
            gTasks[taskId].data[5] = 0;
            gTasks[taskId].data[3] = gBattleStruct->multiBuffer.linkPartnerHeader.vsScreenHealthFlagsLo | (gBattleStruct->multiBuffer.linkPartnerHeader.vsScreenHealthFlagsHi << 8);
            gTasks[taskId].data[4] = gBlockRecvBuffer[enemyMultiplayerId][1];
            SetDeoxysStats();
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 3:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty, sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 4:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            memcpy(gEnemyParty, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 7:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty + 2, sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 8:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            memcpy(gEnemyParty + 2, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 11:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty + 4, sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 12:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            memcpy(gEnemyParty + 4, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon) * 2);
            TryCorrectShedinjaLanguage(&gEnemyParty[0]);
            TryCorrectShedinjaLanguage(&gEnemyParty[1]);
            TryCorrectShedinjaLanguage(&gEnemyParty[2]);
            TryCorrectShedinjaLanguage(&gEnemyParty[3]);
            TryCorrectShedinjaLanguage(&gEnemyParty[4]);
            TryCorrectShedinjaLanguage(&gEnemyParty[5]);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 15:
        InitBtlControllers();
        ++gBattleCommunication[MULTIUSE_STATE];
        gBattleCommunication[SPRITES_INIT_STATE1] = 0;
        gBattleCommunication[SPRITES_INIT_STATE2] = 0;
        break;
    case 16:
        if (BattleInitAllSprites(&gBattleCommunication[SPRITES_INIT_STATE1], &gBattleCommunication[SPRITES_INIT_STATE2]))
        {
            gPreBattleCallback1 = gMain.callback1;
            gMain.callback1 = BattleMainCB1;
            SetMainCallback2(BattleMainCB2);
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                gBattleTypeFlags |= BATTLE_TYPE_LINK_ESTABLISHED;
            }
        }
        break;
    case 5:
    case 9:
    case 13:
        ++gBattleCommunication[MULTIUSE_STATE];
        gBattleCommunication[1] = 1;
    case 6:
    case 10:
    case 14:
        if (--gBattleCommunication[1] == 0)
            ++gBattleCommunication[MULTIUSE_STATE];
        break;
    }
}

static void PrepareOwnMultiPartnerBuffer(void)
{
    s32 i, j;
    u8 *nick, *cur;

    for (i = 0; i < 3; ++i)
    {
        gMultiPartnerParty[i].species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
        gMultiPartnerParty[i].heldItem = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM);
        nick = gMultiPartnerParty[i].nickname;
        GetMonData(&gPlayerParty[i], MON_DATA_NICKNAME, nick);
        gMultiPartnerParty[i].level = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
        gMultiPartnerParty[i].hp = GetMonData(&gPlayerParty[i], MON_DATA_HP);
        gMultiPartnerParty[i].maxhp = GetMonData(&gPlayerParty[i], MON_DATA_MAX_HP);
        gMultiPartnerParty[i].status = GetMonData(&gPlayerParty[i], MON_DATA_STATUS);
        gMultiPartnerParty[i].personality = GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY);
        gMultiPartnerParty[i].gender = GetMonGender(&gPlayerParty[i]);
        StripExtCtrlCodes(nick);
        if (GetMonData(&gPlayerParty[i], MON_DATA_LANGUAGE) != LANGUAGE_JAPANESE)
        {
            for (cur = nick, j = 0; cur[j] != EOS; ++j)
                ;
            while (j < 6)
                cur[j++] = 0;
            cur[j] = EOS;
        }
    }
    memcpy(gBattleStruct->multiBuffer.multiBattleMons, gMultiPartnerParty, sizeof(gMultiPartnerParty));
}

static void CB2_PreInitMultiBattle(void)
{
    s32 i;
    u8 playerMultiplierId;
    u8 r4 = 0xF;
    u16 *savedBattleTypeFlags;
    void (**savedCallback)(void);

    playerMultiplierId = GetMultiplayerId();
    gBattleStruct->multiplayerId = playerMultiplierId;
    savedCallback = &gBattleStruct->savedCallback;
    savedBattleTypeFlags = &gBattleStruct->savedBattleTypeFlags;

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        if (gReceivedRemoteLinkPlayers && IsLinkTaskFinished())
        {
            PrepareOwnMultiPartnerBuffer();
            SendBlock(bitmask_all_link_players_but_self(), gBattleStruct->multiBuffer.multiBattleMons, sizeof(gBattleStruct->multiBuffer.multiBattleMons));
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 1:
        if ((GetBlockReceivedStatus() & r4) == r4)
        {
            ResetBlockReceivedFlags();
            for (i = 0; i < 4; ++i)
            {
                if (i == playerMultiplierId)
                    continue;
                if ((!(gLinkPlayers[i].id & 1) && !(gLinkPlayers[playerMultiplierId].id & 1))
                 || (gLinkPlayers[i].id & 1 && gLinkPlayers[playerMultiplierId].id & 1))
                    memcpy(gMultiPartnerParty, gBlockRecvBuffer[i], sizeof(gMultiPartnerParty));
            }
            ++gBattleCommunication[MULTIUSE_STATE];
            *savedCallback = gMain.savedCallback;
            *savedBattleTypeFlags = gBattleTypeFlags;
            gMain.savedCallback = CB2_PreInitMultiBattle;
            ShowPartyMenuToShowcaseMultiBattleParty();
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            ++gBattleCommunication[MULTIUSE_STATE];
            if (gWirelessCommType)
                SetLinkStandbyCallback();
            else
                SetCloseLinkCallback();
        }
        break;
    case 3:
        if (gWirelessCommType)
        {
            if (IsLinkRfuTaskFinished())
            {
                gBattleTypeFlags = *savedBattleTypeFlags;
                gMain.savedCallback = *savedCallback;
                SetMainCallback2(CB2_InitBattleInternal);
            }
        }
        else if (!gReceivedRemoteLinkPlayers)
        {
            gBattleTypeFlags = *savedBattleTypeFlags;
            gMain.savedCallback = *savedCallback;
            SetMainCallback2(CB2_InitBattleInternal);
        }
        break;
    }
}

static void CB2_HandleStartMultiBattle(void)
{
    u8 playerMultiplayerId;
    s32 id;
    u8 taskId;

    playerMultiplayerId = GetMultiplayerId();
    gBattleStruct->multiplayerId = playerMultiplayerId;
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            ShowBg(0);
            ShowBg(1);
            ShowBg(2);
            ShowBg(3);
            BattleInterfaceSetWindowPals();
            gBattleCommunication[MULTIUSE_STATE] = 1;
        }
        if (gWirelessCommType)
            LoadWirelessStatusIndicatorSpriteGfx();
        break;
    case 1:
        if (gReceivedRemoteLinkPlayers)
        {
            if (IsLinkTaskFinished())
            {
                // 0x201
                *(&gBattleStruct->multiBuffer.linkPartnerHeader.versionSignatureLo) = 1;
                *(&gBattleStruct->multiBuffer.linkPartnerHeader.versionSignatureHi) = 2;
                BufferPartyVsScreenHealth_AtStart();
                SetPlayerBerryDataInBattleStruct();
                SendBlock(bitmask_all_link_players_but_self(), &gBattleStruct->multiBuffer.linkPartnerHeader, sizeof(gBattleStruct->multiBuffer.linkPartnerHeader));
                ++gBattleCommunication[MULTIUSE_STATE];
            }
            if (gWirelessCommType)
                CreateWirelessStatusIndicatorSprite(0, 0);
        }
        break;
    case 2:
        if ((GetBlockReceivedStatus() & 0xF) == 0xF)
        {
            ResetBlockReceivedFlags();
            LinkBattleComputeBattleTypeFlags(4, playerMultiplayerId);
            SetAllPlayersBerryData();
            SetDeoxysStats();
            memcpy(gDecompressionBuffer, gPlayerParty, sizeof(struct Pokemon) * 3);
            taskId = CreateTask(InitLinkBattleVsScreen, 0);
            gTasks[taskId].data[1] = 270;
            gTasks[taskId].data[2] = 90;
            gTasks[taskId].data[5] = 0;
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[4] = 0;
            for (id = 0; id < MAX_LINK_PLAYERS; ++id)
            {
                switch (gLinkPlayers[id].id)
                {
                case 0:
                    gTasks[taskId].data[3] |= gBlockRecvBuffer[id][1] & 0x3F;
                    break;
                case 1:
                    gTasks[taskId].data[4] |= gBlockRecvBuffer[id][1] & 0x3F;
                    break;
                case 2:
                    gTasks[taskId].data[3] |= (gBlockRecvBuffer[id][1] & 0x3F) << 6;
                    break;
                case 3:
                    gTasks[taskId].data[4] |= (gBlockRecvBuffer[id][1] & 0x3F) << 6;
                    break;
                }
            }
            ZeroPlayerPartyMons();
            ZeroEnemyPartyMons();
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        else
        {
            break;
        }
        // fall through
    case 3:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gDecompressionBuffer, sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 4:
        if ((GetBlockReceivedStatus() & 0xF) == 0xF)
        {
            ResetBlockReceivedFlags();
            for (id = 0; id < MAX_LINK_PLAYERS; ++id)
            {
                if (id == playerMultiplayerId)
                {
                    switch (gLinkPlayers[id].id)
                    {
                    case 0:
                    case 3:
                        memcpy(gPlayerParty, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                        break;
                    case 1:
                    case 2:
                        memcpy(gPlayerParty + 3, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                        break;
                    }
                }
                else
                {
                    if ((!(gLinkPlayers[id].id & 1) && !(gLinkPlayers[playerMultiplayerId].id & 1))
                     || ((gLinkPlayers[id].id & 1) && (gLinkPlayers[playerMultiplayerId].id & 1)))
                    {
                        switch (gLinkPlayers[id].id)
                        {
                        case 0:
                        case 3:
                            memcpy(gPlayerParty, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                            break;
                        case 1:
                        case 2:
                            memcpy(gPlayerParty + 3, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                            break;
                        }
                    }
                    else
                    {
                        switch (gLinkPlayers[id].id)
                        {
                        case 0:
                        case 3:
                            memcpy(gEnemyParty, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                            break;
                        case 1:
                        case 2:
                            memcpy(gEnemyParty + 3, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                            break;
                        }
                    }
                }
            }
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 7:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gDecompressionBuffer + sizeof(struct Pokemon) * 2, sizeof(struct Pokemon));
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 8:
        if ((GetBlockReceivedStatus() & 0xF) == 0xF)
        {
            ResetBlockReceivedFlags();
            for (id = 0; id < MAX_LINK_PLAYERS; ++id)
            {
                if (id == playerMultiplayerId)
                {
                    switch (gLinkPlayers[id].id)
                    {
                    case 0:
                    case 3:
                        memcpy(gPlayerParty + 2, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                        break;
                    case 1:
                    case 2:
                        memcpy(gPlayerParty + 5, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                        break;
                    }
                }
                else
                {
                    if ((!(gLinkPlayers[id].id & 1) && !(gLinkPlayers[playerMultiplayerId].id & 1))
                     || ((gLinkPlayers[id].id & 1) && (gLinkPlayers[playerMultiplayerId].id & 1)))
                    {
                        switch (gLinkPlayers[id].id)
                        {
                        case 0:
                        case 3:
                            memcpy(gPlayerParty + 2, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                            break;
                        case 1:
                        case 2:
                            memcpy(gPlayerParty + 5, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                            break;
                        }
                    }
                    else
                    {
                        switch (gLinkPlayers[id].id)
                        {
                        case 0:
                        case 3:
                            memcpy(gEnemyParty + 2, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                            break;
                        case 1:
                        case 2:
                            memcpy(gEnemyParty + 5, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                            break;
                        }
                    }
                }
            }
            TryCorrectShedinjaLanguage(&gPlayerParty[0]);
            TryCorrectShedinjaLanguage(&gPlayerParty[1]);
            TryCorrectShedinjaLanguage(&gPlayerParty[2]);
            TryCorrectShedinjaLanguage(&gPlayerParty[3]);
            TryCorrectShedinjaLanguage(&gPlayerParty[4]);
            TryCorrectShedinjaLanguage(&gPlayerParty[5]);
            TryCorrectShedinjaLanguage(&gEnemyParty[0]);
            TryCorrectShedinjaLanguage(&gEnemyParty[1]);
            TryCorrectShedinjaLanguage(&gEnemyParty[2]);
            TryCorrectShedinjaLanguage(&gEnemyParty[3]);
            TryCorrectShedinjaLanguage(&gEnemyParty[4]);
            TryCorrectShedinjaLanguage(&gEnemyParty[5]);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 11:
        InitBtlControllers();
        ++gBattleCommunication[MULTIUSE_STATE];
        gBattleCommunication[SPRITES_INIT_STATE1] = 0;
        gBattleCommunication[SPRITES_INIT_STATE2] = 0;
        break;
    case 12:
        if (BattleInitAllSprites(&gBattleCommunication[SPRITES_INIT_STATE1], &gBattleCommunication[SPRITES_INIT_STATE2]))
        {
            gPreBattleCallback1 = gMain.callback1;
            gMain.callback1 = BattleMainCB1;
            SetMainCallback2(BattleMainCB2);
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                gBattleTypeFlags |= BATTLE_TYPE_LINK_ESTABLISHED;
        }
        break;
    case 5:
    case 9:
        ++gBattleCommunication[0];
        gBattleCommunication[SPRITES_INIT_STATE1] = 1;
        // fall through
    case 6:
    case 10:
        if (--gBattleCommunication[SPRITES_INIT_STATE1] == 0)
            ++gBattleCommunication[0];
        break;
    }
}

void BattleMainCB2(void)
{
    AnimateSprites();
    BuildOamBuffer();
    RunTextPrinters();
    UpdatePaletteFade();
    RunTasks();

    #ifdef DNS_ENABLED
    DnsApplyFilters();
    #endif

    if (JOY_HELD(B_BUTTON) && gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
    {
        gSpecialVar_Result = gBattleOutcome = B_OUTCOME_DREW;
        ResetPaletteFadeControl();
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
        SetMainCallback2(CB2_QuitPokedudeBattle);
    }
}

void FreeRestoreBattleData(void)
{
    gMain.callback1 = gPreBattleCallback1;
    gScanlineEffect.state = 3;
    gMain.inBattle = FALSE;
    ZeroEnemyPartyMons();
    m4aSongNumStop(SE_LOW_HEALTH);
    FreeMonSpritesGfx();
    FreeBattleSpritesData();
    FreeBattleResources();
}

static void CB2_QuitPokedudeBattle(void)
{
    UpdatePaletteFade();
    if (!gPaletteFade.active)
    {
        FreeRestoreBattleData();
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static void SpriteCB_UnusedDebugSprite(struct Sprite *sprite)
{
    sprite->data[0] = 0;
    sprite->callback = SpriteCB_UnusedDebugSprite_Step;
}

static void SpriteCB_UnusedDebugSprite_Step(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        sUnknownDebugSpriteDataBuffer = AllocZeroed(0x1000);
        ++sprite->data[0];
        sprite->data[1] = 0;
        sprite->data[2] = 0x281;
        sprite->data[3] = 0;
        sprite->data[4] = 1;
        // fall through
    case 1:
        if (--sprite->data[4] == 0)
        {
            s32 i, r2, r0;

            sprite->data[4] = 2;
            r2 = sprite->data[1] + sprite->data[3] * 32;
            r0 = sprite->data[2] - sprite->data[3] * 32;
            for (i = 0; i <= 29; i += 2)
            {
                *(&sUnknownDebugSpriteDataBuffer[r2] + i) = 0x3D;
                *(&sUnknownDebugSpriteDataBuffer[r0] + i) = 0x3D;
            }
            if (++sprite->data[3] == 21)
            {
                ++sprite->data[0];
                sprite->data[1] = 32;
            }
        }
        break;
    case 2:
        if (--sprite->data[1] == 20)
        {
            if (sUnknownDebugSpriteDataBuffer != NULL)
            {
                memset(sUnknownDebugSpriteDataBuffer, 0, 0x1000);
                FREE_AND_SET_NULL(sUnknownDebugSpriteDataBuffer);
            }
            SetMainCallback2(CB2_InitBattle);
        }
        break;
    }
}

static u8 CreateNPCTrainerParty(struct Pokemon *party, u16 trainerNum)
{
    u32 nameHash = 0;
    u32 personalityValue;
    u8 fixedIV;
    s32 i, j;

    if (trainerNum == TRAINER_SECRET_BASE)
        return 0;
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
     && !(gBattleTypeFlags & (BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER)))
    {
        ZeroEnemyPartyMons();
        for (i = 0; i < gTrainers[trainerNum].partySize; ++i)
        {
            if (gTrainers[trainerNum].doubleBattle == TRUE)
                personalityValue = 0x80;
            else if (gTrainers[trainerNum].encounterMusic_gender & 0x80)
                personalityValue = 0x78;
            else
                personalityValue = 0x88;
            for (j = 0; gTrainers[trainerNum].trainerName[j] != EOS; ++j)
                nameHash += gTrainers[trainerNum].trainerName[j];
            switch (gTrainers[trainerNum].partyFlags)
            {
            case 0:
            {
                const struct TrainerMonNoItemDefaultMoves *partyData = gTrainers[trainerNum].party.NoItemDefaultMoves;

                for (j = 0; gSpeciesNames[partyData[i].species][j] != EOS; ++j)
                    nameHash += gSpeciesNames[partyData[i].species][j];
                personalityValue += nameHash << 8;
                fixedIV = partyData[i].iv * 31 / 255;
                CreateMon(&party[i], partyData[i].species, partyData[i].lvl, fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                break;
            }
            case F_TRAINER_PARTY_CUSTOM_MOVESET:
            {
                const struct TrainerMonNoItemCustomMoves *partyData = gTrainers[trainerNum].party.NoItemCustomMoves;

                for (j = 0; gSpeciesNames[partyData[i].species][j] != EOS; ++j)
                    nameHash += gSpeciesNames[partyData[i].species][j];
                personalityValue += nameHash << 8;
                fixedIV = partyData[i].iv * 31 / 255;
                CreateMon(&party[i], partyData[i].species, partyData[i].lvl, fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                for (j = 0; j < MAX_MON_MOVES; ++j)
                {
                    SetMonData(&party[i], MON_DATA_MOVE1 + j, &partyData[i].moves[j]);
                    SetMonData(&party[i], MON_DATA_PP1 + j, &gBattleMoves[partyData[i].moves[j]].pp);
                }
                break;
            }
            case F_TRAINER_PARTY_HELD_ITEM:
            {
                const struct TrainerMonItemDefaultMoves *partyData = gTrainers[trainerNum].party.ItemDefaultMoves;

                for (j = 0; gSpeciesNames[partyData[i].species][j] != EOS; ++j)
                    nameHash += gSpeciesNames[partyData[i].species][j];
                personalityValue += nameHash << 8;
                fixedIV = partyData[i].iv * 31 / 255;
                CreateMon(&party[i], partyData[i].species, partyData[i].lvl, fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);

                SetMonData(&party[i], MON_DATA_HELD_ITEM, &partyData[i].heldItem);
                break;
            }
            case F_TRAINER_PARTY_CUSTOM_MOVESET | F_TRAINER_PARTY_HELD_ITEM:
            {
                const struct TrainerMonItemCustomMoves *partyData = gTrainers[trainerNum].party.ItemCustomMoves;

                for (j = 0; gSpeciesNames[partyData[i].species][j] != EOS; ++j)
                    nameHash += gSpeciesNames[partyData[i].species][j];
                personalityValue += nameHash << 8;
                fixedIV = partyData[i].iv * 31 / 255;
                CreateMon(&party[i], partyData[i].species, partyData[i].lvl, fixedIV, TRUE, personalityValue, OT_ID_RANDOM_NO_SHINY, 0);
                SetMonData(&party[i], MON_DATA_HELD_ITEM, &partyData[i].heldItem);
                for (j = 0; j < MAX_MON_MOVES; ++j)
                {
                    SetMonData(&party[i], MON_DATA_MOVE1 + j, &partyData[i].moves[j]);
                    SetMonData(&party[i], MON_DATA_PP1 + j, &gBattleMoves[partyData[i].moves[j]].pp);
                }
                break;
            }
            }
        }
        gBattleTypeFlags |= gTrainers[trainerNum].doubleBattle;
    }
    return gTrainers[trainerNum].partySize;
}

// not used
UNUSED static void unused_80116CC(void)
{
    if (REG_VCOUNT < 0xA0 && REG_VCOUNT >= 0x6F)
        REG_BG0CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(24) | BGCNT_16COLOR | BGCNT_TXT256x512;
}

void VBlankCB_Battle(void)
{
    // Change gRngSeed every vblank.
    Random();
    SetGpuReg(REG_OFFSET_BG0HOFS, gBattle_BG0_X);
    SetGpuReg(REG_OFFSET_BG0VOFS, gBattle_BG0_Y);
    SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
    SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);
    SetGpuReg(REG_OFFSET_BG2HOFS, gBattle_BG2_X);
    SetGpuReg(REG_OFFSET_BG2VOFS, gBattle_BG2_Y);
    SetGpuReg(REG_OFFSET_BG3HOFS, gBattle_BG3_X);
    SetGpuReg(REG_OFFSET_BG3VOFS, gBattle_BG3_Y);
    SetGpuReg(REG_OFFSET_WIN0H, gBattle_WIN0H);
    SetGpuReg(REG_OFFSET_WIN0V, gBattle_WIN0V);
    SetGpuReg(REG_OFFSET_WIN1H, gBattle_WIN1H);
    SetGpuReg(REG_OFFSET_WIN1V, gBattle_WIN1V);
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    ScanlineEffect_InitHBlankDmaTransfer();
}

void SpriteCB_VsLetterDummy(struct Sprite *sprite)
{
}

static void SpriteCB_VsLetter(struct Sprite *sprite)
{
    if (sprite->data[0] != 0)
        sprite->x = sprite->data[1] + ((sprite->data[2] & 0xFF00) >> 8);
    else
        sprite->x = sprite->data[1] - ((sprite->data[2] & 0xFF00) >> 8);
    sprite->data[2] += 0x180;
    if (sprite->affineAnimEnded)
    {
        FreeSpriteTilesByTag(ANIM_SPRITES_START);
        FreeSpritePaletteByTag(ANIM_SPRITES_START);
        FreeSpriteOamMatrix(sprite);
        DestroySprite(sprite);
    }
}

void SpriteCB_VsLetterInit(struct Sprite *sprite)
{
    StartSpriteAffineAnim(sprite, 1);
    sprite->callback = SpriteCB_VsLetter;
    PlaySE(SE_MUGSHOT);
}

static void BufferPartyVsScreenHealth_AtEnd(u8 taskId)
{
    struct Pokemon *party1 = NULL;
    struct Pokemon *party2 = NULL;
    u8 multiplayerId = gBattleStruct->multiplayerId;
    u32 r7;
    s32 i;

    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        switch (gLinkPlayers[multiplayerId].id)
        {
        case 0:
        case 2:
            party1 = gPlayerParty;
            party2 = gEnemyParty;
            break;
        case 1:
        case 3:
            party1 = gEnemyParty;
            party2 = gPlayerParty;
            break;
        }
    }
    else
    {
        party1 = gPlayerParty;
        party2 = gEnemyParty;
    }
    r7 = 0;
    BUFFER_PARTY_VS_SCREEN_STATUS(party1, r7, i);
    gTasks[taskId].data[3] = r7;
    r7 = 0;
    BUFFER_PARTY_VS_SCREEN_STATUS(party2, r7, i);
    gTasks[taskId].data[4] = r7;
}

void CB2_InitEndLinkBattle(void)
{
    s32 i;
    u8 taskId;

    SetHBlankCallback(NULL);
    SetVBlankCallback(NULL);
    CpuFill32(0, (void *)VRAM, VRAM_SIZE);
    SetGpuReg(REG_OFFSET_MOSAIC, 0);
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 0xF0));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0x50, 0x51));
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    gBattle_WIN0H = WIN_RANGE(0, 0xF0);
    gBattle_WIN0V = WIN_RANGE(0x50, 0x51);
    ScanlineEffect_Clear();
    for (i = 0; i < 80; ++i)
    {
        gScanlineEffectRegBuffers[0][i] = 0xF0;
        gScanlineEffectRegBuffers[1][i] = 0xF0;
    }

    for (; i < 160; ++i)
    {
        gScanlineEffectRegBuffers[0][i] = 0xFF10;
        gScanlineEffectRegBuffers[1][i] = 0xFF10;
    }
    ResetPaletteFade();
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    gBattle_BG2_X = 0;
    gBattle_BG2_Y = 0;
    gBattle_BG3_X = 0;
    gBattle_BG3_Y = 0;
    InitBattleBgsVideo();
    LoadCompressedPalette(gBattleTextboxPalette, 0, 64);
    LoadBattleMenuWindowGfx();
    ResetSpriteData();
    ResetTasks();
    DrawBattleEntryBackground();
    SetGpuReg(REG_OFFSET_WINOUT, WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ | WININ_WIN0_CLR);
    FreeAllSpritePalettes();
    gReservedSpritePaletteCount = 4;
    SetVBlankCallback(VBlankCB_Battle);
    taskId = CreateTask(InitLinkBattleVsScreen, 0);
    gTasks[taskId].data[1] = 270;
    gTasks[taskId].data[2] = 90;
    gTasks[taskId].data[5] = 1;
    BufferPartyVsScreenHealth_AtEnd(taskId);
    SetMainCallback2(CB2_EndLinkBattle);
    gBattleCommunication[MULTIUSE_STATE] = 0;
}

static void CB2_EndLinkBattle(void)
{
    EndLinkBattleInSteps();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
    RunTasks();
}

static void EndLinkBattleInSteps(void)
{
    s32 i;

    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        gBattleCommunication[1] = 0xFF;
        ++gBattleCommunication[MULTIUSE_STATE];
        break;
    case 1:
        if (--gBattleCommunication[1] == 0)
        {
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            SetMainCallback2(gMain.savedCallback);
            TrySetQuestLogLinkBattleEvent();
            FreeMonSpritesGfx();
            FreeBattleSpritesData();
            FreeBattleResources();
        }
        break;
    }
}

u32 GetBattleBgAttribute(u8 arrayId, u8 caseId)
{
    u32 ret = 0;

    switch (caseId)
    {
    case 0:
        ret = gBattleBgTemplates[arrayId].bg;
        break;
    case 1:
        ret = gBattleBgTemplates[arrayId].charBaseIndex;
        break;
    case 2:
        ret = gBattleBgTemplates[arrayId].mapBaseIndex;
        break;
    case 3:
        ret = gBattleBgTemplates[arrayId].screenSize;
        break;
    case 4:
        ret = gBattleBgTemplates[arrayId].paletteMode;
        break;
    case 5:
        ret = gBattleBgTemplates[arrayId].priority;
        break;
    case 6:
        ret = gBattleBgTemplates[arrayId].baseTile;
        break;
    }
    return ret;
}

static void TryCorrectShedinjaLanguage(struct Pokemon *mon)
{
    u8 nickname[POKEMON_NAME_LENGTH + 1];
    u8 language = LANGUAGE_JAPANESE;

    if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_SHEDINJA
     && GetMonData(mon, MON_DATA_LANGUAGE) != language)
    {
        GetMonData(mon, MON_DATA_NICKNAME, nickname);
        if (StringCompareWithoutExtCtrlCodes(nickname, sText_ShedinjaJpnName) == 0)
            SetMonData(mon, MON_DATA_LANGUAGE, &language);
    }
}

#define sBattler            data[0]
#define sSpeciesId          data[2]

void SpriteCB_EnemyMon(struct Sprite *sprite)
{
    u32 selectedPalettes = 0x10000 << sprite->oam.paletteNum;

    sprite->callback = SpriteCB_MoveWildMonToRight;
    StartSpriteAnimIfDifferent(sprite, 0);
    if (!BeginNormalPaletteFade(selectedPalettes, 0, 10, 10, RGB(8, 8, 8))) //If a fade is already in progress,
		gPaletteFade_selectedPalettes |= selectedPalettes; //Then add second mon in wild doubles to the palettes to fade
}

static void SpriteCB_MoveWildMonToRight(struct Sprite *sprite)
{
    if ((gIntroSlideFlags & 1) == 0)
    {
        sprite->x2 += 2;
        if (sprite->x2 == 0)
        {
            sprite->callback = SpriteCB_WildMonShowHealthbox;
            PlayCry1(sprite->data[2], 25);
        }
    }
}

static void SpriteCB_WildMonShowHealthbox(struct Sprite *sprite)
{
    if (sprite->animEnded)
    {
		u32 selectedPalettes = 0x10000 << sprite->oam.paletteNum;

        StartHealthboxSlideIn(sprite->data[0]);
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[sprite->data[0]]);
        sprite->callback = SpriteCallbackDummy;
        StartSpriteAnimIfDifferent(sprite, 0);
        if (!BeginNormalPaletteFade(selectedPalettes, 0, 10, 0, RGB(8, 8, 8))) //If a fade is already in progress,
			gPaletteFade_selectedPalettes |= selectedPalettes; //Then add second mon in wild doubles to the palettes to unfade
    }
}

void SpriteCallbackDummy2(struct Sprite *sprite)
{
}

// not used
UNUSED static void SpriteCB_Unused_8011E28(struct Sprite *sprite)
{
    sprite->data[3] = 6;
    sprite->data[4] = 1;
    sprite->callback = SpriteCB_Unused_8011E28_Step;
}

// not used
static void SpriteCB_Unused_8011E28_Step(struct Sprite *sprite)
{
    --sprite->data[4];
    if (sprite->data[4] == 0)
    {
        sprite->data[4] = 8;
        sprite->invisible ^= 1;
        --sprite->data[3];
        if (sprite->data[3] == 0)
        {
            sprite->invisible = FALSE;
            sprite->callback = SpriteCallbackDummy2;
            gUnknown_2022AE8[0] = 0;
        }
    }
}

void SpriteCB_FaintOpponentMon(struct Sprite *sprite)
{
    u8 battler = sprite->sBattler;
    u16 species;
    u8 yOffset;

    if (gBattleSpritesDataPtr->battlerData[battler].transformSpecies != 0)
        species = gBattleSpritesDataPtr->battlerData[battler].transformSpecies;
    else
        species = sprite->sSpeciesId;
    GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_PERSONALITY);  // Unused return value.
    if (species == SPECIES_UNOWN)
    {
        u32 personalityValue = GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_PERSONALITY);
        u16 unownForm = ((((personalityValue & 0x3000000) >> 18) | ((personalityValue & 0x30000) >> 12) | ((personalityValue & 0x300) >> 6) | (personalityValue & 3)) % 0x1C);
        u16 unownSpecies;

        if (unownForm == 0)
            unownSpecies = SPECIES_UNOWN;  // Use the A Unown form.
        else
            unownSpecies = NUM_SPECIES + unownForm;  // Use one of the other Unown letters.
        yOffset = gMonFrontPicCoords[unownSpecies].y_offset;
    }
    else if (species == SPECIES_CASTFORM)
    {
        yOffset = gCastformFrontSpriteCoords[gBattleMonForms[battler]].y_offset;
    }
    else if (species > NUM_SPECIES)
    {
        yOffset = gMonFrontPicCoords[SPECIES_NONE].y_offset;
    }
    else
    {
        yOffset = gMonFrontPicCoords[species].y_offset;
    }
    sprite->data[3] = 8 - yOffset / 8;
    sprite->data[4] = 1;
    sprite->callback = SpriteCB_AnimFaintOpponent;
}

static void SpriteCB_AnimFaintOpponent(struct Sprite *sprite)
{
    s32 i;

    if (--sprite->data[4] == 0)
    {
        sprite->data[4] = 2;
        sprite->y2 += 8; // Move the sprite down.
        if (--sprite->data[3] < 0)
        {
            FreeSpriteOamMatrix(sprite);
            DestroySprite(sprite);
        }
        else // Erase bottom part of the sprite to create a smooth illusion of mon falling down.
        {
            u8 *dst = (u8 *)gMonSpritesGfxPtr->sprites[GetBattlerPosition(sprite->sBattler)] + (gBattleMonForms[sprite->sBattler] << 11) + (sprite->data[3] << 8);

            for (i = 0; i < 0x100; ++i)
                *(dst++) = 0;
            StartSpriteAnim(sprite, gBattleMonForms[sprite->sBattler]);
        }
    }
}

void SpriteCb_ShowAsMoveTarget(struct Sprite *sprite)
{
    sprite->data[3] = 8;
    sprite->data[4] = sprite->invisible;
    sprite->callback = SpriteCb_BlinkVisible;
}

static void SpriteCb_BlinkVisible(struct Sprite *sprite)
{
    --sprite->data[3];
    if (sprite->data[3] == 0)
    {
        sprite->invisible ^= 1;
        sprite->data[3] = 8;
    }
}

void SpriteCb_HideAsMoveTarget(struct Sprite *sprite)
{
    sprite->invisible = sprite->data[4];
    sprite->data[4] = FALSE;
    sprite->callback = SpriteCallbackDummy2;
}

void SpriteCB_AllyMon(struct Sprite *sprite)
{
    sprite->callback = oac_poke_ally_;
}

static void oac_poke_ally_(struct Sprite *sprite)
{
    if (!(gIntroSlideFlags & 1))
    {
        sprite->x2 -= 2;
        if (sprite->x2 == 0)
        {
            sprite->callback = SpriteCallbackDummy3;
            sprite->data[1] = 0;
        }
    }
}

void SpriteCB_SetToDummy3(struct Sprite *sprite)
{
    sprite->callback = SpriteCallbackDummy3;
}

static void SpriteCallbackDummy3(struct Sprite *sprite)
{
}

void SpriteCB_FaintSlideAnim(struct Sprite *sprite)
{
    if (!(gIntroSlideFlags & 1))
    {
        sprite->x2 += sprite->data[1];
        sprite->y2 += sprite->data[2];
    }
}

#define sSinIndex           data[0]
#define sDelta              data[1]
#define sAmplitude          data[2]
#define sBouncerSpriteId    data[3]
#define sWhich              data[4]

void DoBounceEffect(u8 battler, u8 which, s8 delta, s8 amplitude)
{
    u8 invisibleSpriteId;
    u8 bouncerSpriteId;

    switch (which)
    {
    case BOUNCE_HEALTHBOX:
    default:
        if (gBattleSpritesDataPtr->healthBoxesData[battler].healthboxIsBouncing)
            return;
        break;
    case BOUNCE_MON:
        if (gBattleSpritesDataPtr->healthBoxesData[battler].battlerIsBouncing)
            return;
        break;
    }
    invisibleSpriteId = CreateInvisibleSpriteWithCallback(SpriteCB_BounceEffect);
    if (which == BOUNCE_HEALTHBOX)
    {
        bouncerSpriteId = gHealthboxSpriteIds[battler];
        gBattleSpritesDataPtr->healthBoxesData[battler].healthboxBounceSpriteId = invisibleSpriteId;
        gBattleSpritesDataPtr->healthBoxesData[battler].healthboxIsBouncing = 1;
        gSprites[invisibleSpriteId].sSinIndex = 128; // 0
    }
    else
    {
        bouncerSpriteId = gBattlerSpriteIds[battler];
        gBattleSpritesDataPtr->healthBoxesData[battler].battlerBounceSpriteId = invisibleSpriteId;
        gBattleSpritesDataPtr->healthBoxesData[battler].battlerIsBouncing = 1;
        gSprites[invisibleSpriteId].sSinIndex = 192; // -1
    }
    gSprites[invisibleSpriteId].sDelta = delta;
    gSprites[invisibleSpriteId].sAmplitude = amplitude;
    gSprites[invisibleSpriteId].sBouncerSpriteId = bouncerSpriteId;
    gSprites[invisibleSpriteId].sWhich = which;
    gSprites[bouncerSpriteId].x2 = 0;
    gSprites[bouncerSpriteId].y2 = 0;
}

void EndBounceEffect(u8 battler, u8 which)
{
    u8 bouncerSpriteId;

    if (which == BOUNCE_HEALTHBOX)
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battler].healthboxIsBouncing)
            return;

        bouncerSpriteId = gSprites[gBattleSpritesDataPtr->healthBoxesData[battler].healthboxBounceSpriteId].sBouncerSpriteId;
        DestroySprite(&gSprites[gBattleSpritesDataPtr->healthBoxesData[battler].healthboxBounceSpriteId]);
        gBattleSpritesDataPtr->healthBoxesData[battler].healthboxIsBouncing = 0;
    }
    else
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battler].battlerIsBouncing)
            return;

        bouncerSpriteId = gSprites[gBattleSpritesDataPtr->healthBoxesData[battler].battlerBounceSpriteId].sBouncerSpriteId;
        DestroySprite(&gSprites[gBattleSpritesDataPtr->healthBoxesData[battler].battlerBounceSpriteId]);
        gBattleSpritesDataPtr->healthBoxesData[battler].battlerIsBouncing = 0;
    }
    gSprites[bouncerSpriteId].x2 = 0;
    gSprites[bouncerSpriteId].y2 = 0;
}

static void SpriteCB_BounceEffect(struct Sprite *sprite)
{
    u8 bouncerSpriteId = sprite->sBouncerSpriteId;
    s32 index;

    if (sprite->sWhich == BOUNCE_HEALTHBOX)
        index = sprite->sSinIndex;
    else
        index = sprite->sSinIndex;
    gSprites[bouncerSpriteId].y2 = Sin(index, sprite->sAmplitude) + sprite->sAmplitude;
    sprite->sSinIndex = (sprite->sSinIndex + sprite->sDelta) & 0xFF;
}

void SpriteCB_PlayerThrowInit(struct Sprite *sprite)
{
    StartSpriteAnim(sprite, 1);
    sprite->callback = SpriteCB_PlayerThrowUpdate;
}

void UpdatePlayerPosInThrowAnim(struct Sprite *sprite)
{
    if (sprite->animDelayCounter == 0)
        sprite->centerToCornerVecX = sPlayerThrowXTranslation[sprite->animCmdIndex];
}

static void SpriteCB_PlayerThrowUpdate(struct Sprite *sprite)
{
    UpdatePlayerPosInThrowAnim(sprite);
    if (sprite->animEnded)
        sprite->callback = SpriteCallbackDummy3;
}

void BattleDummy(void)
{
}

void BeginBattleIntro(void)
{
    BattleStartClearSetData();
    gBattleCommunication[1] = 0;
    gBattleMainFunc = BattleIntroGetMonsData;
}

static void BattleMainCB1(void)
{
    gBattleMainFunc();
    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
        gBattlerControllerFuncs[gActiveBattler]();
}

static void BattleStartClearSetData(void)
{
    s32 i;
    u32 j;
    u8 *dataPtr;

    TurnValuesCleanUp(FALSE);
    SpecialStatusesClear();
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
    {
        gStatuses3[i] = 0;
        dataPtr = (u8 *)&gDisableStructs[i];
        for (j = 0; j < sizeof(struct DisableStruct); ++j)
            dataPtr[j] = 0;
        gDisableStructs[i].isFirstTurn = 2;
        gUnknown_2023DD4[i] = 0;
        gLastMoves[i] = MOVE_NONE;
        gLastLandedMoves[i] = MOVE_NONE;
        gLastHitByType[i] = 0;
        gLastResultingMoves[i] = MOVE_NONE;
        gLastHitBy[i] = 0xFF;
        gLockedMoves[i] = MOVE_NONE;
        gLastPrintedMoves[i] = MOVE_NONE;
        gBattleResources->flags->flags[i] = 0;
    }
    for (i = 0; i < 2; ++i)
    {
        gSideStatuses[i] = 0;
        dataPtr = (u8 *)&gSideTimers[i];
        for (j = 0; j < sizeof(struct SideTimer); ++j)
            dataPtr[j] = 0;
    }
    gBattlerAttacker = 0;
    gBattlerTarget = 0;
    gBattleWeather = 0;
    dataPtr = (u8 *)&gWishFutureKnock;
    for (i = 0; i < sizeof(struct WishFutureKnock); ++i)
        dataPtr[i] = 0;
    gHitMarker = 0;
    if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_POKEDUDE)) && gSaveBlock2Ptr->optionsBattleSceneOff)
        gHitMarker |= HITMARKER_NO_ANIMATIONS;
    gBattleScripting.battleStyle = gSaveBlock2Ptr->optionsBattleStyle;
    gMultiHitCounter = 0;
    gBattleOutcome = 0;
    gBattleControllerExecFlags = 0;
    gPaydayMoney = 0;
    gBattleResources->battleScriptsStack->size = 0;
    gBattleResources->battleCallbackStack->size = 0;
    for (i = 0; i < BATTLE_COMMUNICATION_ENTRIES_COUNT; ++i)
        gBattleCommunication[i] = 0;
    gPauseCounterBattle = 0;
    gBattleMoveDamage = 0;
    gIntroSlideFlags = 0;
    gBattleScripting.animTurn = 0;
    gBattleScripting.animTargetsHit = 0;
    gLeveledUpInBattle = 0;
    gAbsentBattlerFlags = 0;
    gBattleStruct->runTries = 0;
    gBattleStruct->safariGoNearCounter = 0;
    gBattleStruct->safariPkblThrowCounter = 0;
    *(&gBattleStruct->safariCatchFactor) = gBaseStats[GetMonData(&gEnemyParty[0], MON_DATA_SPECIES)].catchRate * 100 / 1275;
    *(&gBattleStruct->safariEscapeFactor) = gBaseStats[GetMonData(&gEnemyParty[0], MON_DATA_SPECIES)].safariZoneFleeRate * 100 / 1275;
    if (gBattleStruct->safariEscapeFactor <= 1)
        gBattleStruct->safariEscapeFactor = 2;
    gBattleStruct->wildVictorySong = 0;
    gBattleStruct->moneyMultiplier = 1;
    for (i = 0; i < 8; ++i)
    {
        *((u8 *)gBattleStruct->lastTakenMove + i) = MOVE_NONE;
        *((u8 *)gBattleStruct->usedHeldItems + i) = ITEM_NONE;
        *((u8 *)gBattleStruct->choicedMove + i) = MOVE_NONE;
        *((u8 *)gBattleStruct->changedItems + i) = ITEM_NONE;
        *(i + 0 * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
        *(i + 1 * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
        *(i + 2 * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
        *(i + 3 * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    }
    *(gBattleStruct->AI_monToSwitchIntoId + 0) = PARTY_SIZE;
    *(gBattleStruct->AI_monToSwitchIntoId + 1) = PARTY_SIZE;
    *(&gBattleStruct->givenExpMons) = 0;
    for (i = 0; i < 11; ++i)
        gBattleResults.catchAttempts[i] = 0;
    gBattleResults.battleTurnCounter = 0;
    gBattleResults.playerFaintCounter = 0;
    gBattleResults.opponentFaintCounter = 0;
    gBattleResults.playerSwitchesCounter = 0;
    gBattleResults.numHealingItemsUsed = 0;
    gBattleResults.numRevivesUsed = 0;
    gBattleResults.playerMonWasDamaged = FALSE;
    gBattleResults.usedMasterBall = FALSE;
    gBattleResults.lastOpponentSpecies = SPECIES_NONE;
    gBattleResults.lastUsedMovePlayer = MOVE_NONE;
    gBattleResults.lastUsedMoveOpponent = MOVE_NONE;
    gBattleResults.playerMon1Species = SPECIES_NONE;
    gBattleResults.playerMon2Species = SPECIES_NONE;
    gBattleResults.caughtMonSpecies = SPECIES_NONE;
    for (i = 0; i < POKEMON_NAME_LENGTH; ++i)
    {
        gBattleResults.playerMon1Name[i] = 0;
        gBattleResults.playerMon2Name[i] = 0;
        gBattleResults.caughtMonNick[i] = 0;
    }
}

void SwitchInClearSetData(void)
{
    struct DisableStruct disableStructCopy = gDisableStructs[gActiveBattler];
    s32 i;
    u8 *ptr;

    if (gCurrentMove != MOVE_BATON_PASS)
    {
        for (i = 0; i < NUM_BATTLE_STATS - 1; ++i)
            gBattleMons[gActiveBattler].statStages[i] = 6;
        for (i = 0; i < gBattlersCount; ++i)
        {
            if ((gBattleMons[i].status2 & STATUS2_ESCAPE_PREVENTION) && gDisableStructs[i].battlerPreventingEscape == gActiveBattler)
                gBattleMons[i].status2 &= ~STATUS2_ESCAPE_PREVENTION;
            if ((gStatuses3[i] & STATUS3_ALWAYS_HITS) && gDisableStructs[i].battlerWithSureHit == gActiveBattler)
            {
                gStatuses3[i] &= ~STATUS3_ALWAYS_HITS;
                gDisableStructs[i].battlerWithSureHit = 0;
            }
        }
    }
    if (gCurrentMove == MOVE_BATON_PASS)
    {
        gBattleMons[gActiveBattler].status2 &= (STATUS2_CONFUSION | STATUS2_FOCUS_ENERGY | STATUS2_SUBSTITUTE | STATUS2_ESCAPE_PREVENTION | STATUS2_CURSED);
        gStatuses3[gActiveBattler] &= (STATUS3_LEECHSEED_BATTLER | STATUS3_LEECHSEED | STATUS3_ALWAYS_HITS | STATUS3_PERISH_SONG | STATUS3_ROOTED | STATUS3_MUDSPORT | STATUS3_AIR_BALLOON_BS);
        for (i = 0; i < gBattlersCount; ++i)
        {
            if (GetBattlerSide(gActiveBattler) != GetBattlerSide(i)
             && (gStatuses3[i] & STATUS3_ALWAYS_HITS) != 0
             && (gDisableStructs[i].battlerWithSureHit == gActiveBattler))
            {
                gStatuses3[i] &= ~(STATUS3_ALWAYS_HITS);
                gStatuses3[i] |= 0x10;
            }
        }
    }
    else
    {
        gBattleMons[gActiveBattler].status2 = 0;
        gStatuses3[gActiveBattler] = 0;
    }
    for (i = 0; i < gBattlersCount; ++i)
    {
        if (gBattleMons[i].status2 & STATUS2_INFATUATED_WITH(gActiveBattler))
            gBattleMons[i].status2 &= ~(STATUS2_INFATUATED_WITH(gActiveBattler));
        if ((gBattleMons[i].status2 & STATUS2_WRAPPED) && *(gBattleStruct->wrappedBy + i) == gActiveBattler)
            gBattleMons[i].status2 &= ~(STATUS2_WRAPPED);
    }
    gActionSelectionCursor[gActiveBattler] = 0;
    gMoveSelectionCursor[gActiveBattler] = 0;
    ptr = (u8 *)&gDisableStructs[gActiveBattler];
    for (i = 0; i < sizeof(struct DisableStruct); ++i)
        ptr[i] = 0;
    if (gCurrentMove == MOVE_BATON_PASS)
    {
        gDisableStructs[gActiveBattler].substituteHP = disableStructCopy.substituteHP;
        gDisableStructs[gActiveBattler].battlerWithSureHit = disableStructCopy.battlerWithSureHit;
        gDisableStructs[gActiveBattler].perishSongTimer = disableStructCopy.perishSongTimer;
        gDisableStructs[gActiveBattler].perishSongTimerStartValue = disableStructCopy.perishSongTimerStartValue;
        gDisableStructs[gActiveBattler].battlerPreventingEscape = disableStructCopy.battlerPreventingEscape;
    }
    gMoveResultFlags = 0;
    gDisableStructs[gActiveBattler].isFirstTurn = 2;
    gLastMoves[gActiveBattler] = MOVE_NONE;
    gLastLandedMoves[gActiveBattler] = MOVE_NONE;
    gLastHitByType[gActiveBattler] = 0;
    gLastResultingMoves[gActiveBattler] = MOVE_NONE;
    gLastPrintedMoves[gActiveBattler] = MOVE_NONE;
    gLastHitBy[gActiveBattler] = 0xFF;
    *(gBattleStruct->lastTakenMove + gActiveBattler * 2 + 0) = MOVE_NONE;
    *(gBattleStruct->lastTakenMove + gActiveBattler * 2 + 1) = MOVE_NONE;
    *(0 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(0 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    *(1 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(1 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    *(2 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(2 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    *(3 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(3 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    for (i = 0; i < gBattlersCount; ++i)
    {
        if (i != gActiveBattler)
        {
            *(gBattleStruct->lastTakenMove + i * 2 + 0) = MOVE_NONE;
            *(gBattleStruct->lastTakenMove + i * 2 + 1) = MOVE_NONE;
        }
        *(i * 8 + gActiveBattler * 2 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
        *(i * 8 + gActiveBattler * 2 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    }
    *((u8 *)(&gBattleStruct->choicedMove[gActiveBattler]) + 0) = MOVE_NONE;
    *((u8 *)(&gBattleStruct->choicedMove[gActiveBattler]) + 1) = MOVE_NONE;
    gBattleResources->flags->flags[gActiveBattler] = 0;
    gCurrentMove = MOVE_NONE;
}

void FaintClearSetData(void)
{
    s32 i;
    u8 *ptr;

    for (i = 0; i < NUM_BATTLE_STATS - 1; ++i)
        gBattleMons[gActiveBattler].statStages[i] = 6;
    gBattleMons[gActiveBattler].status2 = 0;
    gStatuses3[gActiveBattler] = 0;
    for (i = 0; i < gBattlersCount; ++i)
    {
        if ((gBattleMons[i].status2 & STATUS2_ESCAPE_PREVENTION) && gDisableStructs[i].battlerPreventingEscape == gActiveBattler)
            gBattleMons[i].status2 &= ~STATUS2_ESCAPE_PREVENTION;
        if (gBattleMons[i].status2 & STATUS2_INFATUATED_WITH(gActiveBattler))
            gBattleMons[i].status2 &= ~(STATUS2_INFATUATED_WITH(gActiveBattler));
        if ((gBattleMons[i].status2 & STATUS2_WRAPPED) && *(gBattleStruct->wrappedBy + i) == gActiveBattler)
            gBattleMons[i].status2 &= ~(STATUS2_WRAPPED);
    }
    gActionSelectionCursor[gActiveBattler] = 0;
    gMoveSelectionCursor[gActiveBattler] = 0;
    ptr = (u8 *)&gDisableStructs[gActiveBattler];
    for (i = 0; i < sizeof(struct DisableStruct); ++i)
        ptr[i] = 0;
    gProtectStructs[gActiveBattler].protected = FALSE;
    gProtectStructs[gActiveBattler].endured = FALSE;
    gProtectStructs[gActiveBattler].noValidMoves = FALSE;
    gProtectStructs[gActiveBattler].helpingHand = FALSE;
    gProtectStructs[gActiveBattler].bounceMove = FALSE;
    gProtectStructs[gActiveBattler].stealMove = FALSE;
    gProtectStructs[gActiveBattler].flag0Unknown = FALSE;
    gProtectStructs[gActiveBattler].prlzImmobility = FALSE;
    gProtectStructs[gActiveBattler].confusionSelfDmg = FALSE;
    gProtectStructs[gActiveBattler].targetNotAffected = FALSE;
    gProtectStructs[gActiveBattler].chargingTurn = FALSE;
    gProtectStructs[gActiveBattler].fleeFlag = 0;
    gProtectStructs[gActiveBattler].usedImprisonedMove = FALSE;
    gProtectStructs[gActiveBattler].loveImmobility = FALSE;
    gProtectStructs[gActiveBattler].usedDisabledMove = FALSE;
    gProtectStructs[gActiveBattler].usedTauntedMove = FALSE;
    gProtectStructs[gActiveBattler].flag2Unknown = FALSE;
    gProtectStructs[gActiveBattler].flinchImmobility = FALSE;
    gProtectStructs[gActiveBattler].notFirstStrike = FALSE;
    gDisableStructs[gActiveBattler].isFirstTurn = 2;
    gLastMoves[gActiveBattler] = MOVE_NONE;
    gLastLandedMoves[gActiveBattler] = MOVE_NONE;
    gLastHitByType[gActiveBattler] = MOVE_NONE;
    gLastResultingMoves[gActiveBattler] = MOVE_NONE;
    gLastPrintedMoves[gActiveBattler] = MOVE_NONE;
    gLastHitBy[gActiveBattler] = 0xFF;
    *((u8 *)(&gBattleStruct->choicedMove[gActiveBattler]) + 0) = MOVE_NONE;
    *((u8 *)(&gBattleStruct->choicedMove[gActiveBattler]) + 1) = MOVE_NONE;
    *(gBattleStruct->lastTakenMove + gActiveBattler * 2 + 0) = MOVE_NONE;
    *(gBattleStruct->lastTakenMove + gActiveBattler * 2 + 1) = MOVE_NONE;
    *(0 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(0 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    *(1 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(1 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    *(2 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(2 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    *(3 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
    *(3 * 2 + gActiveBattler * 8 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    for (i = 0; i < gBattlersCount; ++i)
    {
        if (i != gActiveBattler)
        {
            *(gBattleStruct->lastTakenMove + i * 2 + 0) = MOVE_NONE;
            *(gBattleStruct->lastTakenMove + i * 2 + 1) = MOVE_NONE;
        }
        *(i * 8 + gActiveBattler * 2 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 0) = 0;
        *(i * 8 + gActiveBattler * 2 + (u8 *)(gBattleStruct->lastTakenMoveFrom) + 1) = 0;
    }
    gBattleResources->flags->flags[gActiveBattler] = 0;
    gBattleMons[gActiveBattler].type1 = gBaseStats[gBattleMons[gActiveBattler].species].type1;
    gBattleMons[gActiveBattler].type2 = gBaseStats[gBattleMons[gActiveBattler].species].type2;
    gBattleMons[gActiveBattler].type3 =  TYPE_BLANK;
}

static void BattleIntroGetMonsData(void)
{
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        gActiveBattler = gBattleCommunication[1];
        BtlController_EmitGetMonData(0, REQUEST_ALL_BATTLE, 0);
        MarkBattlerForControllerExec(gActiveBattler);
        ++gBattleCommunication[MULTIUSE_STATE];
        break;
    case 1:
        if (gBattleControllerExecFlags == 0)
        {
            ++gBattleCommunication[1];
            if (gBattleCommunication[1] == gBattlersCount)
                gBattleMainFunc = BattleIntroPrepareBackgroundSlide;
            else
                gBattleCommunication[MULTIUSE_STATE] = 0;
        }
        break;
    }
}

static void BattleIntroPrepareBackgroundSlide(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        gActiveBattler = GetBattlerAtPosition(0);
        BtlController_EmitIntroSlide(0, gBattleTerrain);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattleMainFunc = BattleIntroDrawTrainersOrMonsSprites;
        gBattleCommunication[MULTIUSE_STATE] = 0;
        gBattleCommunication[SPRITES_INIT_STATE1] = 0;
    }
}

static void BattleIntroDrawTrainersOrMonsSprites(void)
{
    u8 *ptr;
    s32 i;

    if (!gBattleControllerExecFlags)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
        {
            if ((gBattleTypeFlags & BATTLE_TYPE_SAFARI)
             && GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
            {
                ptr = (u8 *)&gBattleMons[gActiveBattler];
                for (i = 0; i < sizeof(struct BattlePokemon); ++i)
                    ptr[i] = 0;
            }
            else
            {
                u16 *hpOnSwitchout;

                ptr = (u8 *)&gBattleMons[gActiveBattler];
                for (i = 0; i < sizeof(struct BattlePokemon); ++i)
                    ptr[i] = gBattleBufferB[gActiveBattler][4 + i];
                gBattleMons[gActiveBattler].type1 = gBaseStats[gBattleMons[gActiveBattler].species].type1;
                gBattleMons[gActiveBattler].type2 = gBaseStats[gBattleMons[gActiveBattler].species].type2;
                gBattleMons[gActiveBattler].type3 = TYPE_BLANK;
                gBattleMons[gActiveBattler].ability = GetMonAbility(GetBankPartyData(gActiveBattler));
                hpOnSwitchout = &gBattleStruct->hpOnSwitchout[GetBattlerSide(gActiveBattler)];
                *hpOnSwitchout = gBattleMons[gActiveBattler].hp;
                for (i = 0; i < NUM_BATTLE_STATS - 1; ++i)
                    gBattleMons[gActiveBattler].statStages[i] = 6;
                gBattleMons[gActiveBattler].status2 = 0;
            }
            if (GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_LEFT)
            {
                BtlController_EmitDrawTrainerPic(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
            {
                if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_LEFT)
                {
                    BtlController_EmitDrawTrainerPic(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
                if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT
                 && !(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER
                                       | BATTLE_TYPE_POKEDUDE
                                       | BATTLE_TYPE_LINK
                                       | BATTLE_TYPE_GHOST
                                       | BATTLE_TYPE_OLD_MAN_TUTORIAL
                                       | BATTLE_TYPE_LEGENDARY)))
                    HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), FLAG_SET_SEEN, gBattleMons[gActiveBattler].personality);
            }
            else
            {
                if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT)
                {
                    if (gBattleTypeFlags & (BATTLE_TYPE_GHOST | BATTLE_TYPE_GHOST_UNVEILED))
                    {
                        if (!IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE(gBattleTypeFlags))
                            HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), FLAG_SET_SEEN, gBattleMons[gActiveBattler].personality);
                    }
                    else if (!(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER
                                                | BATTLE_TYPE_POKEDUDE
                                                | BATTLE_TYPE_LINK
                                                | BATTLE_TYPE_GHOST
                                                | BATTLE_TYPE_OLD_MAN_TUTORIAL
                                                | BATTLE_TYPE_LEGENDARY)))
                    {
                        HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), FLAG_SET_SEEN, gBattleMons[gActiveBattler].personality);
                    }
                    BtlController_EmitLoadMonSprite(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI
             && (GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT || GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_RIGHT))
            {
                BtlController_EmitDrawTrainerPic(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
        gBattleMainFunc = BattleIntroDrawPartySummaryScreens;
    }
}

static void BattleIntroDrawPartySummaryScreens(void)
{
    s32 i;
    struct HpAndStatus hpStatus[PARTY_SIZE];

    if (!gBattleControllerExecFlags)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            for (i = 0; i < PARTY_SIZE; ++i)
            {
                if (GetMonData(&gEnemyParty[i], MON_DATA_SPECIES2) == SPECIES_NONE
                 || GetMonData(&gEnemyParty[i], MON_DATA_SPECIES2) == SPECIES_EGG)
                {
                    hpStatus[i].hp = 0xFFFF;
                    hpStatus[i].status = 0;
                }
                else
                {
                    hpStatus[i].hp = GetMonData(&gEnemyParty[i], MON_DATA_HP);
                    hpStatus[i].status = GetMonData(&gEnemyParty[i], MON_DATA_STATUS);
                }
            }
            gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
            BtlController_EmitDrawPartyStatusSummary(0, hpStatus, 0x80);
            MarkBattlerForControllerExec(gActiveBattler);
            for (i = 0; i < PARTY_SIZE; ++i)
            {
                if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) == SPECIES_NONE
                 || GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) == SPECIES_EGG)
                {
                    hpStatus[i].hp = 0xFFFF;
                    hpStatus[i].status = 0;
                }
                else
                {
                    hpStatus[i].hp = GetMonData(&gPlayerParty[i], MON_DATA_HP);
                    hpStatus[i].status = GetMonData(&gPlayerParty[i], MON_DATA_STATUS);
                }
            }
            gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
            BtlController_EmitDrawPartyStatusSummary(0, hpStatus, 0x80);
            MarkBattlerForControllerExec(gActiveBattler);

            gBattleMainFunc = BattleIntroPrintTrainerWantsToBattle;
        }
        else
        {
            // The struct gets set here, but nothing is ever done with it since
            // wild battles don't show the party summary.
            // Still, there's no point in having dead code.
            for (i = 0; i < PARTY_SIZE; ++i)
            {
                if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) == SPECIES_NONE
                 || GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) == SPECIES_EGG)
                {
                    hpStatus[i].hp = 0xFFFF;
                    hpStatus[i].status = 0;
                }
                else
                {
                    hpStatus[i].hp = GetMonData(&gPlayerParty[i], MON_DATA_HP);
                    hpStatus[i].status = GetMonData(&gPlayerParty[i], MON_DATA_STATUS);
                }
            }
            gBattleMainFunc = BattleIntroPrintWildMonAttacked;
        }
    }
}

static void BattleIntroPrintTrainerWantsToBattle(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        PrepareStringBattle(STRINGID_INTROMSG, gActiveBattler);
        gBattleMainFunc = BattleIntroPrintOpponentSendsOut;
    }
}

static void BattleIntroPrintWildMonAttacked(void)
{
    if (!gBattleControllerExecFlags)
    {
        gBattleMainFunc = BattleIntroPrintPlayerSendsOut;
        PrepareStringBattle(STRINGID_INTROMSG, 0);
        if (IS_BATTLE_TYPE_GHOST_WITH_SCOPE(gBattleTypeFlags))
        {
            gBattleScripting.battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
            BattleScriptExecute(BattleScript_SilphScopeUnveiled);
        }
    }
}

static void BattleIntroPrintOpponentSendsOut(void)
{
    if (!gBattleControllerExecFlags)
    {
        PrepareStringBattle(STRINGID_INTROSENDOUT, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
        gBattleMainFunc = BattleIntroOpponentSendsOutMonAnimation;
    }
}

static void BattleIntroOpponentSendsOutMonAnimation(void)
{
    if (!gBattleControllerExecFlags)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
        {
            if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_LEFT)
            {
                BtlController_EmitIntroTrainerBallThrow(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI && GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_RIGHT)
            {
                BtlController_EmitIntroTrainerBallThrow(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
        gBattleMainFunc = BattleIntroRecordMonsToDex;
    }
}

static void BattleIntroRecordMonsToDex(void)
{
    if (!gBattleControllerExecFlags)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT
             && !(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER
                                   | BATTLE_TYPE_POKEDUDE
                                   | BATTLE_TYPE_LINK
                                   | BATTLE_TYPE_GHOST
                                   | BATTLE_TYPE_OLD_MAN_TUTORIAL
                                   | BATTLE_TYPE_LEGENDARY)))
                HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), FLAG_SET_SEEN, gBattleMons[gActiveBattler].personality);
        gBattleMainFunc = BattleIntroPrintPlayerSendsOut;
    }
}

// not used
static void Unused_AutoProgressToIntro(void)
{
    if (!gBattleControllerExecFlags)
        gBattleMainFunc = BattleIntroPrintPlayerSendsOut;
}

void BattleIntroPrintPlayerSendsOut(void)
{
    if (!gBattleControllerExecFlags)
    {
        if (!(gBattleTypeFlags & BATTLE_TYPE_SAFARI))
            PrepareStringBattle(STRINGID_INTROSENDOUT, GetBattlerAtPosition(B_POSITION_PLAYER_LEFT));
        gBattleMainFunc = BattleIntroPlayerSendsOutMonAnimation;
    }
}

static void BattleIntroPlayerSendsOutMonAnimation(void)
{
    u32 position;

    if (!gBattleControllerExecFlags)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
        {
            if (GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_LEFT)
            {
                BtlController_EmitIntroTrainerBallThrow(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI && GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT)
            {
                BtlController_EmitIntroTrainerBallThrow(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
        gBattleStruct->switchInAbilitiesCounter = 0;
        gBattleStruct->switchInItemsCounter = 0;
        gBattleStruct->overworldWeatherDone = FALSE;
        gBattleMainFunc = TryDoEventsBeforeFirstTurn;
    }
}

// not used
static void Unused_AutoProgressToSwitchInAnims(void)
{
    if (!gBattleControllerExecFlags)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
        {
            if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
            {
                BtlController_EmitSwitchInAnim(0, gBattlerPartyIndexes[gActiveBattler], FALSE);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
        gBattleStruct->switchInAbilitiesCounter = 0;
        gBattleStruct->switchInItemsCounter = 0;
        gBattleStruct->overworldWeatherDone = FALSE;
        gBattleMainFunc = TryDoEventsBeforeFirstTurn;
    }
}

enum BattleBeginStates
{
	BackupPartyItems,
	GetTurnOrder,
	ThirdTypeRemoval,
	RaidBattleReveal,
	DynamaxUsableIndicator,
	NeutralizingGas,
	SwitchInAbilities,
	Intimidate,
	AmuletCoin_WhiteHerb,
	AirBalloon,
	TotemPokemon,
	StartTurnEnd,
};


const u8 gText_CamomonsTypeReveal[] = ("");
const u8 gText_CamomonsTypeRevealDualType[] = ("");
const u8 gText_RaidBattleReveal[] = ("");

static void SavePartyItems(void)
{
    int i;

	for (i = 0; i < PARTY_SIZE; ++i)
		gBattleStruct->itemBackup[i] = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, NULL);
}

bool8 TryActivateOWTerrain(void)
{
	bool8 effect = FALSE;
	u8 owTerrain = VarGet(VAR_TERRAIN);

	if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS)
	{
		//Can have at most one of these set at a time
		switch (gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN) {
			case BATTLE_CIRCUS_ELECTRIC_TERRAIN:
				owTerrain = ELECTRIC_TERRAIN;
				break;
			case BATTLE_CIRCUS_GRASSY_TERRAIN:
				owTerrain = GRASSY_TERRAIN;
				break;
			case BATTLE_CIRCUS_MISTY_TERRAIN:
				owTerrain = MISTY_TERRAIN;
				break;
			case BATTLE_CIRCUS_PSYCHIC_TERRAIN:
				owTerrain = PSYCHIC_TERRAIN;
				break;
		}
	}

	if (owTerrain != 0 && gTerrainType != owTerrain && !gBattleStruct->terrainForcefullyRemoved)
	{
		switch (owTerrain) {
			case ELECTRIC_TERRAIN:
				BattleScriptPushCursorAndCallback(BattleScript_ElectricTerrainBattleBegin);
				effect = TRUE;
				break;
			case GRASSY_TERRAIN:
				BattleScriptPushCursorAndCallback(BattleScript_GrassyTerrainBattleBegin);
				effect = TRUE;
				break;
			case MISTY_TERRAIN:
				BattleScriptPushCursorAndCallback(BattleScript_MistyTerrainBattleBegin);
				effect = TRUE;
				break;
			case PSYCHIC_TERRAIN:
				BattleScriptPushCursorAndCallback(BattleScript_PsychicTerrainBattleBegin);
				effect = TRUE;
		}

		if (effect)
			gTerrainType = owTerrain;
	}

	return effect;
}

u8 CanActivateTotemBoost(u8 bank)
{
	u16 val = VarGet(VAR_TOTEM + bank);
	u16 stat = val & 0x7;

	if (bank < gBattlersCount && stat != 0)
	{
		u8 raiseAmount = val & ~(0xF);

		if (val == 0xFFFF) //Omniboost
		{
			if (InBattleSands())
				VarSet(VAR_TOTEM + bank, 0); //Only first Pokemon gets boost in battle sands

			return TOTEM_OMNIBOOST;
		}
		else if (stat <= STAT_STAGE_EVASION
		&& ((raiseAmount >= INCREASE_1 && raiseAmount <= INCREASE_6)
		 || (raiseAmount >= DECREASE_1 && raiseAmount <= DECREASE_6)))
		{
			gBattleScripting.statChanger = stat | raiseAmount;
			if (InBattleSands())
				VarSet(VAR_TOTEM + bank, 0); //Only first Pokemon gets boost in battle sands

			return TOTEM_SINGLE_BOOST;
		}
	}

	return TOTEM_NO_BOOST;
}

static void TryPrepareTotemBoostInBattleSands(void)
{
	if (InBattleSands())
	{
		u8 playerId = 0;
		u8 enemyId = 1;
		u8 playerStat = RandRange(STAT_STAGE_ATK, BATTLE_STATS_NO);
		u8 enemyStat = RandRange(STAT_STAGE_ATK, BATTLE_STATS_NO);
		u8 increaseMax, increase;
        u8 currStreak;
        u8 contraryShiftEnemy;
        u8 contraryShiftPlayer;

		if (IS_DOUBLE_BATTLE)
		{
			playerId |= (Random() & BIT_FLANK);
			enemyId |= (Random() & BIT_FLANK);
		}

		//The farther the "player" gets, the higher chance a stat will be raised more than 1
		currStreak = GetCurrentBattleTowerStreak();
		if (currStreak < 10)
			increaseMax = 1;
		else if (currStreak < 20)
			increaseMax = 2;
		else if (currStreak < 30)
			increaseMax = 3;
		else if (currStreak < 60)
			increaseMax = 4;
		else if (currStreak < 50)
			increaseMax = 5;
		else
			increaseMax = 6;

		//Makes it so Contrary has no effect on the stat boost
		contraryShiftPlayer = (ABILITY(playerId) == ABILITY_CONTRARY) ? 0x80 : 0;
		contraryShiftEnemy = (ABILITY(enemyId) == ABILITY_CONTRARY) ? 0x80 : 0;

		increase = (Random() % increaseMax) + 1;
		VarSet(VAR_TOTEM + playerId, playerStat | (increase * 0x10 + contraryShiftPlayer));
		increase = (Random() % increaseMax) + 1;
		VarSet(VAR_TOTEM + enemyId, enemyStat | (increase * 0x10 + contraryShiftEnemy));
	}
}

void CleanUpExtraTurnValues(void)
{
	gBattleStruct->NoMoreMovingThisTurn = 0;
	gBattleStruct->OriginalAttackerTargetCount = 0;
	gBattleStruct->ParentalBondOn = FALSE;
	gBattleStruct->DancerInProgress = FALSE;
	gBattleStruct->MoveBounceInProgress = FALSE;
	gBattleStruct->zMoveData.active = FALSE;
	gBattleStruct->dynamaxData.active = FALSE;
	gBattleStruct->batonPassing = FALSE;
}

static void TryDoEventsBeforeFirstTurn(void)
{
    int i, j, k;
	u8* state = &(gBattleStruct->switchInAbilitiesCounter);
	u8* bank = &(gBattleStruct->switchInItemsCounter);

    if (!gBattleControllerExecFlags)
    {
        switch(*state)
        {
            case BackupPartyItems:
				SavePartyItems();
				++*state;
				break;
            
            case GetTurnOrder:
				gBattleStruct->skipBankStatAnim = 0xFF;
				for (i = 0; i < gBattlersCount; ++i)
				{
					gBattlerByTurnOrder[i] = i;
					ResetBestMonToSwitchInto(i);
				}

				for (i = 0; i < gBattlersCount - 1; ++i)
				{
					for (j = i + 1; j < gBattlersCount; ++j)
					{
						if (GetWhoStrikesFirst(gBattlerByTurnOrder[i], gBattlerByTurnOrder[j], 1))
							SwapTurnOrder(i, j);
					}
				}

				//OW Weather
				if (!gBattleStruct->overworldWeatherDone && AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, 0, 0, 0xFF, 0))
				{
					gBattleStruct->overworldWeatherDone = TRUE;
					return;
				}

				//OW Terrain
				if (TryActivateOWTerrain())
					return;

				//Primal Reversion
				for (; *bank < gBattlersCount; ++*bank)
				{
					const u8* script = DoPrimalReversion(gBattlerByTurnOrder[*bank], 0);

					if(script != NULL)
					{
						BattleScriptPushCursorAndCallback(script);
						gBattlerAttacker = gBattleScripting.battler = gBattlerByTurnOrder[*bank];
						++*bank;
						return;
					}
				}

				++*state;
				*bank = 0;
				break;
            
            case ThirdTypeRemoval:
				for (; *bank < gBattlersCount; ++*bank)
				{
					gBattleMons[*bank].type3 =  TYPE_BLANK;

					if (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) //The Pokemon takes on the types of its first two moves
					{
						#ifndef NO_GHOST_BATTLES
						if (IS_GHOST_BATTLE && SIDE(*bank) == B_SIDE_OPPONENT)
							continue;
						#endif

						UpdateTypesForCamomons(*bank);
						gBattleScripting.battler = *bank;
						BattleScriptPushCursorAndCallback(BattleScript_CamomonsTypeRevealEnd3);

						if (gBattleMons[*bank].type1 == gBattleMons[*bank].type2)
							gBattleStringLoader = gText_CamomonsTypeReveal;
						else
							gBattleStringLoader = gText_CamomonsTypeRevealDualType;
						PREPARE_TYPE_BUFFER(gBattleTextBuff1, gBattleMons[*bank].type1);
						PREPARE_TYPE_BUFFER(gBattleTextBuff2, gBattleMons[*bank].type2);
						++*bank;
						return;
					}
				}

				*bank = 0;
				++*state;
				break;
            
            case RaidBattleReveal:
				if (IsRaidBattle())
				{
					gAbsentBattlerFlags |= gBitTable[B_POSITION_OPPONENT_RIGHT]; //Because it's not there - causes bugs without
					gBattleScripting.battler = BANK_RAID_BOSS;
					gBattleStringLoader = gText_RaidBattleReveal;
					BattleScriptPushCursorAndCallback(BattleScript_RaidBattleStart);
				}
				++*state;
				break;
            
            case DynamaxUsableIndicator:
				gBattleScripting.battler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
				if (DynamaxEnabled(gBattleScripting.battler))
				{
					gBattleStringLoader = gText_DynamaxUsable;
					BattleScriptPushCursorAndCallback(BattleScript_DynamaxEnergySwirl);
				}
				++*state;
				break;
            
            case NeutralizingGas:
				for (; *bank < gBattlersCount; ++*bank)
				{
					if (ABILITY(gBattlerByTurnOrder[*bank]) == ABILITY_NEUTRALIZINGGAS
					&& AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gBattlerByTurnOrder[*bank], 0, 0, 0))
					{
						++*bank;
						return;
					}
				}

				*bank = 0;
				++*state;
				break;
            
            case SwitchInAbilities:
				for (; *bank < gBattlersCount; ++*bank)
				{
					if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gBattlerByTurnOrder[*bank], 0, 0, 0))
					{
						++*bank;
						return;
					}
				}

				*bank = 0; //Reset Bank for next loop
				++*state;
				break;
            
            case Intimidate:
			/*
				if (AbilityBattleEffects(ABILITYEFFECT_INTIMIDATE1, 0, 0, 0, 0))
					return;
				if (AbilityBattleEffects(ABILITYEFFECT_INTIMIDATE2, 0, 0, 0, 0))
					return;
			*/
				++*state;
				break;

            case AmuletCoin_WhiteHerb:
				for (; *bank < gBattlersCount; ++*bank)
				{
					if (ItemBattleEffects(ItemEffects_SwitchIn, gBattlerByTurnOrder[*bank], FALSE, FALSE))
					{
						++*bank;
						return;
					}
				}

				*bank = 0; //Reset Bank for next loop
				++*state;
				break;
            
            case AirBalloon:
				for (; *bank < gBattlersCount; ++*bank)
				{
					#ifndef NO_GHOST_BATTLES
					if (IS_GHOST_BATTLE && SIDE(gBattlerByTurnOrder[*bank]) == B_SIDE_OPPONENT)
						continue;
					#endif

					if (ITEM_EFFECT(gBattlerByTurnOrder[*bank]) == HOLD_EFFECT_AIR_BALLOON)
					{
						BattleScriptPushCursorAndCallback(BattleScript_AirBalloonFloat);
						gBattlerAttacker = gBattleScripting.battler = gBattlerByTurnOrder[*bank];
						RecordItemEffectBattle(gBattlerAttacker, HOLD_EFFECT_AIR_BALLOON);
						++*bank;
						return;
					}
				}

				*bank = 0; //Reset Bank for next loop
				TryPrepareTotemBoostInBattleSands();
				++*state;
				break;
            
            case TotemPokemon: ;
				if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_SANDS //The only battle facility to utilize totem boosts
				|| !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_OLD_MAN_TUTORIAL)))
				{
					for (; *bank < gBattlersCount; ++*bank)
					{
                        u8 totemBoostType;
						#ifndef NO_GHOST_BATTLES
						if (IS_GHOST_BATTLE && SIDE(*bank) == B_SIDE_OPPONENT)
							continue;
						#endif

						totemBoostType = CanActivateTotemBoost(*bank);
						if (totemBoostType == TOTEM_SINGLE_BOOST)
						{
							BattleScriptPushCursorAndCallback(BattleScript_Totem);
							gBattlerAttacker = gBattleScripting.battler = *bank;
							++*bank;
							return;
						}
						else if (totemBoostType == TOTEM_OMNIBOOST) //All stats
						{
							BattleScriptPushCursorAndCallback(BattleScript_TotemOmniboost);
							gBattlerAttacker = gBattleScripting.battler = *bank;
							++*bank;
							return;
						}
					}
				}

				*bank = 0; //Reset Bank for next loop
				++*state;
				break;
            
            case StartTurnEnd:
				for (i = 0; i < MAX_BATTLERS_COUNT; i++)
				{
					gBattleStruct->monToSwitchIntoId[i] = PARTY_SIZE;
					gChosenActionByBattler[i] = 0xFF;
					gChosenMoveByBattler[i] = 0;
					gBattleStruct->ai.fightingStyle[i] = 0xFF;

					for (j = 0; j < MAX_BATTLERS_COUNT; ++j)
					{
						gBattleStruct->ai.strongestMove[i][j] = 0xFFFF;
						gBattleStruct->ai.canKnockOut[i][j] = 0xFF;
						gBattleStruct->ai.can2HKO[i][j] = 0xFF;
						gBattleStruct->ai.strongestMove[i][j] = 0xFFFF;
						gBattleStruct->ai.canKnockOut[i][j] = 0xFF;
						gBattleStruct->ai.can2HKO[i][j] = 0xFF;

						for (k = 0; k < MAX_MON_MOVES; ++k)
						{
							gBattleStruct->ai.damageByMove[i][j][k] = 0xFFFFFFFF;
							gBattleStruct->ai.moveKnocksOut1Hit[i][j][k] = 0xFF;
							gBattleStruct->ai.moveKnocksOut2Hits[i][j][k] = 0xFF;
						}
					}
				}

				TurnValuesCleanUp(0);
				SpecialStatusesClear();
				gBattleStruct->absentBattlerFlags = gAbsentBattlerFlags;
				gBattleMainFunc = HandleTurnActionSelectionState;
				ResetSentPokesToOpponentValue();
				for (i = 0; i < 8; i++)
					gBattleCommunication[i] = 0;

				for (i = 0; i < gBattlersCount; i++)
				{
					gBattleMons[i].status2 &= ~8;
					gBattleStruct->pickupStack[i] = 0xFF;
					gBattleStruct->statRoseThisRound[i] = FALSE;
					gBattleStruct->statFellThisTurn[i] = FALSE;
					gBattleStruct->statFellThisRound[i] = FALSE;
				}

				gBattleStruct->turnEffectsTracker = 0;
				gBattleStruct->turnEffectsBattlerId = 0;
				gBattleStruct->wishPerishSongState = 0;
				gBattleStruct->wishPerishSongBattlerId = 0;
				gBattleScripting.atk49_state = 0;
				gBattleStruct->faintedActionsState = 0;
				gBattleStruct->turnCountersTracker = 0;
				gMoveResultFlags = 0;
				gRandomTurnNumber = Random();
				*state = 0;

        }
        
    }
}

static void HandleEndTurn_ContinueBattle(void)
{
    s32 i;

    if (!gBattleControllerExecFlags)
    {
        gBattleMainFunc = BattleTurnPassed;
        for (i = 0; i < BATTLE_COMMUNICATION_ENTRIES_COUNT; ++i)
            gBattleCommunication[i] = 0;
        for (i = 0; i < gBattlersCount; ++i)
        {
            gBattleMons[i].status2 &= ~(STATUS2_FLINCHED);
            if ((gBattleMons[i].status1 & STATUS1_SLEEP) && (gBattleMons[i].status2 & STATUS2_MULTIPLETURNS))
                CancelMultiTurnMoves(i);
        }
        gBattleStruct->turnEffectsTracker = 0;
        gBattleStruct->turnEffectsBattlerId = 0;
        gBattleStruct->wishPerishSongState = 0;
        gBattleStruct->wishPerishSongBattlerId = 0;
        gBattleStruct->turnCountersTracker = 0;
        gMoveResultFlags = 0;
    }
}

void BattleTurnPassed(void)
{
    s32 i;

    TurnValuesCleanUp(TRUE);
    if (gBattleOutcome == 0)
    {
        if (DoBattlerEndTurnEffects())
            return;
    }
    if (HandleFaintedMonActions())
        return;
    gBattleStruct->faintedActionsState = 0;
    if (HandleWishPerishSongOnTurnEnd())
        return;
    TurnValuesCleanUp(FALSE);
    gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING);
    gHitMarker &= ~(HITMARKER_UNABLE_TO_USE_MOVE);
    gHitMarker &= ~(HITMARKER_PLAYER_FAINTED);
    gHitMarker &= ~(HITMARKER_NON_ATTACK_DMG);
    gBattleScripting.animTurn = 0;
    gBattleScripting.animTargetsHit = 0;
    gBattleScripting.atk49_state = 0;
    gBattleMoveDamage = 0;
    gMoveResultFlags = 0;
    for (i = 0; i < 5; ++i)
        gBattleCommunication[i] = 0;
    if (gBattleOutcome != 0)
    {
        gCurrentActionFuncId = B_ACTION_FINISHED;
        gBattleMainFunc = RunTurnActionsFunctions;
        return;
    }
    if (gBattleResults.battleTurnCounter < 0xFF)
        ++gBattleResults.battleTurnCounter;
    for (i = 0; i < gBattlersCount; ++i)
    {
        gChosenActionByBattler[i] = B_ACTION_NONE;
        gChosenMoveByBattler[i] = MOVE_NONE;
    }
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
        *(gBattleStruct->monToSwitchIntoId + i) = PARTY_SIZE;
    *(&gBattleStruct->absentBattlerFlags) = gAbsentBattlerFlags;
    gBattleMainFunc = HandleTurnActionSelectionState;
    gRandomTurnNumber = Random();
}

#define ABILITY_PREVENTING_ESCAPE 2

u8 IsRunningFromBattleImpossible(void)
{
    u8 itemEffect;
	u8 side;
	int i;

	itemEffect = ITEM_EFFECT(gActiveBattler);
	gPotentialItemEffectBattler = gActiveBattler;

	if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER))
		return FALSE;
	else if (AreAllKindsOfRunningPrevented())
		return TRUE;
	else if (itemEffect == HOLD_EFFECT_CAN_ALWAYS_RUN)
		return FALSE;
	else if (gBattleMons[gActiveBattler].ability == ABILITY_RUNAWAY)
		return FALSE;
	else if (IsOfType(gActiveBattler, TYPE_GHOST))
		return FALSE;

	side = SIDE(gActiveBattler);

	for (i = 0; i < gBattlersCount; i++)
	{
		if (ABILITY(gActiveBattler) != ABILITY_SHADOWTAG //Shadow Tag's not affected by Shadow Tag
		&& side != SIDE(i)
		&& ABILITY(i) == ABILITY_SHADOWTAG)
		{
			gBattleScripting.battler = i;
			gLastUsedAbility = ABILITY(i);
			gBattleCommunication[MULTISTRING_CHOOSER] = 2;
			return ABILITY_PREVENTING_ESCAPE;
		}

		if (side != SIDE(i)
		&& ABILITY(i) == ABILITY_ARENATRAP
		&& CheckGrounding(gActiveBattler))
		{
			gBattleScripting.battler = i;
			gLastUsedAbility = ABILITY(i);
			gBattleCommunication[MULTISTRING_CHOOSER] = 2;
			return ABILITY_PREVENTING_ESCAPE;
		}

		if (i != gActiveBattler
		&& ABILITY(i) == ABILITY_MAGNETPULL
		&& IsOfType(gActiveBattler, TYPE_STEEL))
		{
			gBattleScripting.battler = i;
			gLastUsedAbility = ABILITY(i);
			gBattleCommunication[MULTISTRING_CHOOSER] = 2;
			return ABILITY_PREVENTING_ESCAPE;
		}
	}

	if ((gBattleMons[gActiveBattler].status2 & (STATUS2_ESCAPE_PREVENTION | STATUS2_WRAPPED))
	|| (gStatuses3[gActiveBattler] & STATUS3_SKY_DROP_TARGET))
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		return TRUE;
	}

	if (!gBattleStruct->TeleportBit)
	{
		if (gStatuses3[gActiveBattler] & STATUS3_ROOTED)
		{
			gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			return 1;
		}
	}
	else
		gBattleStruct->TeleportBit = FALSE;

	if (IsFairyLockActive())
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		return TRUE;
	}

	if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
		return TRUE;
	}

	return 0;
}

void UpdatePartyOwnerOnSwitch_NonMulti(u8 battler)
{
    s32 i;
    u8 r4, r1;

    for (i = 0; i < 3; ++i)
        gBattlePartyCurrentOrder[i] = *(battler * 3 + i + (u8 *)(gBattleStruct->battlerPartyOrders));
    r4 = GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[battler]);
    r1 = GetPartyIdFromBattlePartyId(*(gBattleStruct->monToSwitchIntoId + battler));
    SwitchPartyMonSlots(r4, r1);
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        for (i = 0; i < 3; ++i)
        {
            *(battler * 3 + i + (u8 *)(gBattleStruct->battlerPartyOrders)) = gBattlePartyCurrentOrder[i];
            *(BATTLE_PARTNER(battler) * 3 + i + (u8 *)(gBattleStruct->battlerPartyOrders)) = gBattlePartyCurrentOrder[i];
        }
    }
    else
    {
        for (i = 0; i < 3; ++i)
            *(battler * 3 + i + (u8 *)(gBattleStruct->battlerPartyOrders)) = gBattlePartyCurrentOrder[i];
    }
}

enum
{
    STATE_BEFORE_ACTION_CHOSEN,
    STATE_WAIT_ACTION_CHOSEN,
    STATE_WAIT_ACTION_CASE_CHOSEN,
    STATE_WAIT_ACTION_CONFIRMED_STANDBY,
    STATE_WAIT_ACTION_CONFIRMED,
    STATE_SELECTION_SCRIPT,
    STATE_WAIT_SET_BEFORE_ACTION,
};

static void HandleTurnActionSelectionState(void)
{
    s32 i;

    gBattleCommunication[ACTIONS_CONFIRMED_COUNT] = 0;
    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
    {
        u8 position = GetBattlerPosition(gActiveBattler);

        switch (gBattleCommunication[gActiveBattler])
        {
        case STATE_BEFORE_ACTION_CHOSEN: // Choose an action.
            *(gBattleStruct->monToSwitchIntoId + gActiveBattler) = PARTY_SIZE;
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI
             || (position & BIT_FLANK) == B_FLANK_LEFT
             || gBattleStruct->absentBattlerFlags & gBitTable[GetBattlerAtPosition(BATTLE_PARTNER(position))]
             || gBattleCommunication[GetBattlerAtPosition(BATTLE_PARTNER(position))] == STATE_WAIT_ACTION_CONFIRMED)
            {
                if (gBattleStruct->absentBattlerFlags & gBitTable[gActiveBattler])
                {
                    gChosenActionByBattler[gActiveBattler] = B_ACTION_NOTHING_FAINTED;
                    if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
                        gBattleCommunication[gActiveBattler] = STATE_WAIT_ACTION_CONFIRMED;
                    else
                        gBattleCommunication[gActiveBattler] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                }
                else
                {
                    if (gBattleMons[gActiveBattler].status2 & STATUS2_MULTIPLETURNS
                     || gBattleMons[gActiveBattler].status2 & STATUS2_RECHARGE)
                    {
                        gChosenActionByBattler[gActiveBattler] = B_ACTION_USE_MOVE;
                        gBattleCommunication[gActiveBattler] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                    }
                    else
                    {
                        BtlController_EmitChooseAction(0, gChosenActionByBattler[0], gBattleBufferB[0][1] | (gBattleBufferB[0][2] << 8));
                        MarkBattlerForControllerExec(gActiveBattler);
                        ++gBattleCommunication[gActiveBattler];
                    }
                }
            }
            break;
        case STATE_WAIT_ACTION_CHOSEN: // Try to perform an action.
            if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) | (0xF0000000) | (gBitTable[gActiveBattler] << 4) | (gBitTable[gActiveBattler] << 8) | (gBitTable[gActiveBattler] << 0xC))))
            {
                gChosenActionByBattler[gActiveBattler] = gBattleBufferB[gActiveBattler][1];
                switch (gBattleBufferB[gActiveBattler][1])
                {
                case B_ACTION_USE_MOVE:
                    if (AreAllMovesUnusable())
                    {
                        gBattleCommunication[gActiveBattler] = STATE_SELECTION_SCRIPT;
                        *(gBattleStruct->selectionScriptFinished + gActiveBattler) = FALSE;
                        *(gBattleStruct->stateIdAfterSelScript + gActiveBattler) = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                        *(gBattleStruct->moveTarget + gActiveBattler) = gBattleBufferB[gActiveBattler][3];
                        return;
                    }
                    else if (gDisableStructs[gActiveBattler].encoredMove != MOVE_NONE)
                    {
                        gChosenMoveByBattler[gActiveBattler] = gDisableStructs[gActiveBattler].encoredMove;
                        *(gBattleStruct->chosenMovePositions + gActiveBattler) = gDisableStructs[gActiveBattler].encoredMovePos;
                        gBattleCommunication[gActiveBattler] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                        return;
                    }
                    else
                    {
                        struct ChooseMoveStruct moveInfo;

                        moveInfo.species = gBattleMons[gActiveBattler].species;
                        moveInfo.monType1 = gBattleMons[gActiveBattler].type1;
                        moveInfo.monType2 = gBattleMons[gActiveBattler].type2;
                        for (i = 0; i < MAX_MON_MOVES; ++i)
                        {
                            moveInfo.moves[i] = gBattleMons[gActiveBattler].moves[i];
                            moveInfo.currentPp[i] = gBattleMons[gActiveBattler].pp[i];
                            moveInfo.maxPp[i] = CalculatePPWithBonus(gBattleMons[gActiveBattler].moves[i],
                                                                     gBattleMons[gActiveBattler].ppBonuses,
                                                                     i);
                        }
                        BtlController_EmitChooseMove(0, (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) != 0, FALSE, &moveInfo);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                    break;
                case B_ACTION_USE_ITEM:
                    if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_EREADER_TRAINER))
                    {
                        gSelectionBattleScripts[gActiveBattler] = BattleScript_ActionSelectionItemsCantBeUsed;
                        gBattleCommunication[gActiveBattler] = STATE_SELECTION_SCRIPT;
                        *(gBattleStruct->selectionScriptFinished + gActiveBattler) = FALSE;
                        *(gBattleStruct->stateIdAfterSelScript + gActiveBattler) = STATE_BEFORE_ACTION_CHOSEN;
                        return;
                    }
                    else
                    {
                        BtlController_EmitChooseItem(0, gBattleStruct->battlerPartyOrders[gActiveBattler]);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                    break;
                case B_ACTION_SWITCH:
                    *(gBattleStruct->battlerPartyIndexes + gActiveBattler) = gBattlerPartyIndexes[gActiveBattler];
                    if (gBattleMons[gActiveBattler].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION) || gStatuses3[gActiveBattler] & STATUS3_ROOTED)
                    {
                        BtlController_EmitChoosePokemon(0, PARTY_ACTION_CANT_SWITCH, 6, ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
                    }
                    else if ((i = ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_SHADOW_TAG))
                          || ((i = ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_ARENA_TRAP))
                              && !IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_FLYING)
                              && gBattleMons[gActiveBattler].ability != ABILITY_LEVITATE)
                          || ((i = AbilityBattleEffects(ABILITYEFFECT_CHECK_FIELD_EXCEPT_BATTLER, gActiveBattler, ABILITY_MAGNET_PULL, 0, 0))
                              && IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_STEEL)))
                    {
                        BtlController_EmitChoosePokemon(0, ((i - 1) << 4) | PARTY_ACTION_ABILITY_PREVENTS, 6, gLastUsedAbility, gBattleStruct->battlerPartyOrders[gActiveBattler]);
                    }
                    else
                    {
                        if (gActiveBattler == 2 && gChosenActionByBattler[0] == B_ACTION_SWITCH)
                            BtlController_EmitChoosePokemon(0, PARTY_ACTION_CHOOSE_MON, *(gBattleStruct->monToSwitchIntoId + 0), ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
                        else if (gActiveBattler == 3 && gChosenActionByBattler[1] == B_ACTION_SWITCH)
                            BtlController_EmitChoosePokemon(0, PARTY_ACTION_CHOOSE_MON, *(gBattleStruct->monToSwitchIntoId + 1), ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
                        else
                            BtlController_EmitChoosePokemon(0, PARTY_ACTION_CHOOSE_MON, 6, ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
                    }
                    MarkBattlerForControllerExec(gActiveBattler);
                    break;
                case B_ACTION_SAFARI_BALL:
                    if (IsPlayerPartyAndPokemonStorageFull())
                    {
                        gSelectionBattleScripts[gActiveBattler] = BattleScript_PrintFullBox;
                        gBattleCommunication[gActiveBattler] = STATE_SELECTION_SCRIPT;
                        *(gBattleStruct->selectionScriptFinished + gActiveBattler) = FALSE;
                        *(gBattleStruct->stateIdAfterSelScript + gActiveBattler) = STATE_BEFORE_ACTION_CHOSEN;
                        return;
                    }
                    break;
                case B_ACTION_CANCEL_PARTNER:
                    gBattleCommunication[gActiveBattler] = STATE_WAIT_SET_BEFORE_ACTION;
                    gBattleCommunication[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gActiveBattler)))] = STATE_BEFORE_ACTION_CHOSEN;
                    BtlController_EmitEndBounceEffect(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                    return;
                }
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
                 && !(gBattleTypeFlags & BATTLE_TYPE_LINK)
                 && gBattleBufferB[gActiveBattler][1] == B_ACTION_RUN)
                {
                    BattleScriptExecute(BattleScript_PrintCantRunFromTrainer);
                    gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
                }
                else if (IsRunningFromBattleImpossible() != BATTLE_RUN_SUCCESS
                      && gBattleBufferB[gActiveBattler][1] == B_ACTION_RUN)
                {
                    gSelectionBattleScripts[gActiveBattler] = BattleScript_PrintCantEscapeFromBattle;
                    gBattleCommunication[gActiveBattler] = STATE_SELECTION_SCRIPT;
                    *(gBattleStruct->selectionScriptFinished + gActiveBattler) = FALSE;
                    *(gBattleStruct->stateIdAfterSelScript + gActiveBattler) = STATE_BEFORE_ACTION_CHOSEN;
                    return;
                }
                else
                {
                    ++gBattleCommunication[gActiveBattler];
                }
            }
            break;
        case STATE_WAIT_ACTION_CASE_CHOSEN:
            if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) | (0xF0000000) | (gBitTable[gActiveBattler] << 4) | (gBitTable[gActiveBattler] << 8) | (gBitTable[gActiveBattler] << 0xC))))
            {
                switch (gChosenActionByBattler[gActiveBattler])
                {
                case B_ACTION_USE_MOVE:
                    switch (gBattleBufferB[gActiveBattler][1])
                    {
                    case 3 ... 9:
                        gChosenActionByBattler[gActiveBattler] = gBattleBufferB[gActiveBattler][1];
                        return;
                    default:
                        if ((gBattleBufferB[gActiveBattler][2] | (gBattleBufferB[gActiveBattler][3] << 8)) == 0xFFFF)
                        {
                            gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
                        }
                        else if (TrySetCantSelectMoveBattleScript())
                        {
                            gBattleCommunication[gActiveBattler] = STATE_SELECTION_SCRIPT;
                            *(gBattleStruct->selectionScriptFinished + gActiveBattler) = FALSE;
                            gBattleBufferB[gActiveBattler][1] = 0;
                            *(gBattleStruct->stateIdAfterSelScript + gActiveBattler) = STATE_WAIT_ACTION_CHOSEN;
                            return;
                        }
                        else
                        {
                            *(gBattleStruct->chosenMovePositions + gActiveBattler) = gBattleBufferB[gActiveBattler][2];
                            gChosenMoveByBattler[gActiveBattler] = gBattleMons[gActiveBattler].moves[*(gBattleStruct->chosenMovePositions + gActiveBattler)];
                            *(gBattleStruct->moveTarget + gActiveBattler) = gBattleBufferB[gActiveBattler][3];
                            MegaRetrieveData();
                            ++gBattleCommunication[gActiveBattler];
                        }
                        break;
                    }
                    break;
                case B_ACTION_USE_ITEM:
                    if ((gBattleBufferB[gActiveBattler][1] | (gBattleBufferB[gActiveBattler][2] << 8)) == 0)
                    {
                        gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
                    }
                    else
                    {
                        gLastUsedItem = (gBattleBufferB[gActiveBattler][1] | (gBattleBufferB[gActiveBattler][2] << 8));
                        ++gBattleCommunication[gActiveBattler];
                    }
                    break;
                case B_ACTION_SWITCH:
                    if (gBattleBufferB[gActiveBattler][1] == PARTY_SIZE)
                    {
                        gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
                    }
                    else
                    {
                        *(gBattleStruct->monToSwitchIntoId + gActiveBattler) = gBattleBufferB[gActiveBattler][1];
                        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                        {
                            *(gActiveBattler * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) &= 0xF;
                            *(gActiveBattler * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) |= (gBattleBufferB[gActiveBattler][2] & 0xF0);
                            *(gActiveBattler * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 1) = gBattleBufferB[gActiveBattler][3];
                            *((gActiveBattler ^ BIT_FLANK) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) &= (0xF0);
                            *((gActiveBattler ^ BIT_FLANK) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) |= (gBattleBufferB[gActiveBattler][2] & 0xF0) >> 4;
                            *((gActiveBattler ^ BIT_FLANK) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 2) = gBattleBufferB[gActiveBattler][3];
                        }
                        ++gBattleCommunication[gActiveBattler];
                    }
                    break;
                case B_ACTION_RUN:
                    gHitMarker |= HITMARKER_RUN;
                    ++gBattleCommunication[gActiveBattler];
                    break;
                case B_ACTION_SAFARI_WATCH_CAREFULLY:
                    ++gBattleCommunication[gActiveBattler];
                    break;
                case B_ACTION_SAFARI_BALL:
                    ++gBattleCommunication[gActiveBattler];
                    break;
                case B_ACTION_SAFARI_BAIT:
                case B_ACTION_SAFARI_GO_NEAR:
                    ++gBattleCommunication[gActiveBattler];
                    break;
                case B_ACTION_SAFARI_RUN:
                    gHitMarker |= HITMARKER_RUN;
                    ++gBattleCommunication[gActiveBattler];
                    break;
                case B_ACTION_OLDMAN_THROW:
                    ++gBattleCommunication[gActiveBattler];
                    break;
                }
            }
            break;
        case STATE_WAIT_ACTION_CONFIRMED_STANDBY:
            if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) | (0xF0000000) | (gBitTable[gActiveBattler] << 4) | (gBitTable[gActiveBattler] << 8) | (gBitTable[gActiveBattler] << 0xC))))
            {
                if (((gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_DOUBLE)) != BATTLE_TYPE_DOUBLE)
                 || (position & BIT_FLANK) != B_FLANK_LEFT
                 || (*(&gBattleStruct->absentBattlerFlags) & gBitTable[GetBattlerAtPosition(position ^ BIT_FLANK)]))
                    BtlController_EmitLinkStandbyMsg(0, 0);
                else
                    BtlController_EmitLinkStandbyMsg(0, 1);
                MarkBattlerForControllerExec(gActiveBattler);
                ++gBattleCommunication[gActiveBattler];
            }
            break;
        case STATE_WAIT_ACTION_CONFIRMED:
            if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) | (0xF0000000) | (gBitTable[gActiveBattler] << 4) | (gBitTable[gActiveBattler] << 8) | (gBitTable[gActiveBattler] << 0xC))))
                ++gBattleCommunication[ACTIONS_CONFIRMED_COUNT];
            break;
        case STATE_SELECTION_SCRIPT:
            if (*(gBattleStruct->selectionScriptFinished + gActiveBattler))
            {
                gBattleCommunication[gActiveBattler] = *(gBattleStruct->stateIdAfterSelScript + gActiveBattler);
            }
            else
            {
                gBattlerAttacker = gActiveBattler;
                gBattlescriptCurrInstr = gSelectionBattleScripts[gActiveBattler];
                if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) | (0xF0000000) | (gBitTable[gActiveBattler] << 4) | (gBitTable[gActiveBattler] << 8) | (gBitTable[gActiveBattler] << 0xC))))
                    gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
                gSelectionBattleScripts[gActiveBattler] = gBattlescriptCurrInstr;
            }
            break;
        case STATE_WAIT_SET_BEFORE_ACTION:
            if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) | (0xF0000000) | (gBitTable[gActiveBattler] << 4) | (gBitTable[gActiveBattler] << 8) | (gBitTable[gActiveBattler] << 0xC))))
                gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
            break;
        }
    }
    // Check if everyone chose actions.
    if (gBattleCommunication[ACTIONS_CONFIRMED_COUNT] == gBattlersCount)
        gBattleMainFunc = SetActionsAndBattlersTurnOrder;
}

void SwapTurnOrder(u8 id1, u8 id2)
{
    u32 temp;

    SWAP(gActionsByTurnOrder[id1], gActionsByTurnOrder[id2], temp);
    SWAP(gBattlerByTurnOrder[id1], gBattlerByTurnOrder[id2], temp);
}

static void SetActionsAndBattlersTurnOrder(void)
{
    s32 turnOrderId = 0;
	s32 i, j;

	if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
	{
		for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
		{
			gActionsByTurnOrder[turnOrderId] = gChosenActionByBattler[gActiveBattler];
			gBattlerByTurnOrder[turnOrderId] = gActiveBattler;
			++turnOrderId;
		}
	}
	else
	{
		if (gBattleTypeFlags & BATTLE_TYPE_LINK)
		{
			for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
			{
				if (gChosenActionByBattler[gActiveBattler] == B_ACTION_RUN)
				{
					turnOrderId = 5;
					break;
				}
			}
		}
		else
		{
			if (gChosenActionByBattler[0] == B_ACTION_RUN)
			{
				gActiveBattler = 0;
				turnOrderId = 5;
			}
			else if (gChosenActionByBattler[0] == B_ACTION_USE_ITEM)
			{
				gBattleStruct->playerItemUsedCount = MathMin(gBattleStruct->playerItemUsedCount + 1, 0xFF);
			}

			if (gChosenActionByBattler[2] == B_ACTION_RUN)
			{
				gActiveBattler = 2;
				turnOrderId = 5;
			}
			else if (gChosenActionByBattler[2] == B_ACTION_USE_ITEM && !IsTagBattle())
			{
				gBattleStruct->playerItemUsedCount = MathMin(gBattleStruct->playerItemUsedCount + 1, 0xFF);
			}
		}

		if (turnOrderId == 5)
		{
			gActionsByTurnOrder[0] = gChosenActionByBattler[gActiveBattler];
			gBattlerByTurnOrder[0] = gActiveBattler;
			turnOrderId = 1;
			for (i = 0; i < gBattlersCount; i++)
			{
				if (i != gActiveBattler)
				{
					gActionsByTurnOrder[turnOrderId] = gChosenActionByBattler[i];
					gBattlerByTurnOrder[turnOrderId] = i;
					++turnOrderId;
				}
			}
			gBattleMainFunc = CheckFocusPunch_ClearVarsBeforeTurnStarts;
			gBattleStruct->focusPunchBattlerId = 0;
			return;
		}
		else
		{
			for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
			{
				if (gChosenActionByBattler[gActiveBattler] == B_ACTION_USE_ITEM || gChosenActionByBattler[gActiveBattler] == B_ACTION_SWITCH)
				{
					gActionsByTurnOrder[turnOrderId] = gChosenActionByBattler[gActiveBattler];
					gBattlerByTurnOrder[turnOrderId] = gActiveBattler;
					++turnOrderId;
				}
			}
			for (gActiveBattler = 0; gActiveBattler < gBattlersCount; gActiveBattler++)
			{
				if (gChosenActionByBattler[gActiveBattler] != B_ACTION_USE_ITEM && gChosenActionByBattler[gActiveBattler] != B_ACTION_SWITCH)
				{
					gActionsByTurnOrder[turnOrderId] = gChosenActionByBattler[gActiveBattler];
					gBattlerByTurnOrder[turnOrderId] = gActiveBattler;
					++turnOrderId;
				}
			}
			for (i = 0; i < gBattlersCount - 1; i++)
			{
				for (j = i + 1; j < gBattlersCount; j++)
				{
					u8 bank1 = gBattlerByTurnOrder[i];
					u8 bank2 = gBattlerByTurnOrder[j];
					if (gActionsByTurnOrder[i] != B_ACTION_USE_ITEM
						&& gActionsByTurnOrder[j] != B_ACTION_USE_ITEM
						&& gActionsByTurnOrder[i] != B_ACTION_SWITCH
						&& gActionsByTurnOrder[j] != B_ACTION_SWITCH)
					{
						if (GetWhoStrikesFirst(bank1, bank2, FALSE))
							SwapTurnOrder(i, j);
					}
				}
			}
		}
	}

	gBattleMainFunc = CheckFocusPunch_ClearVarsBeforeTurnStarts;
	gBattleStruct->focusPunchBattlerId = 0;
}

static void TurnValuesCleanUp(bool8 var0)
{
    s32 i;
    u8 *dataPtr;

    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
    {
        if (var0)
        {
            gProtectStructs[gActiveBattler].protected = FALSE;
            gProtectStructs[gActiveBattler].endured = FALSE;
        }
        else
        {
            dataPtr = (u8 *)(&gProtectStructs[gActiveBattler]);
            for (i = 0; i < sizeof(struct ProtectStruct); ++i)
                dataPtr[i] = 0;
            if (gDisableStructs[gActiveBattler].isFirstTurn)
                --gDisableStructs[gActiveBattler].isFirstTurn;
            if (gDisableStructs[gActiveBattler].rechargeTimer)
            {
                --gDisableStructs[gActiveBattler].rechargeTimer;
                if (gDisableStructs[gActiveBattler].rechargeTimer == 0)
                    gBattleMons[gActiveBattler].status2 &= ~(STATUS2_RECHARGE);
            }
        }

        if (gDisableStructs[gActiveBattler].substituteHP == 0)
            gBattleMons[gActiveBattler].status2 &= ~(STATUS2_SUBSTITUTE);
    }
    gSideTimers[0].followmeTimer = 0;
    gSideTimers[1].followmeTimer = 0;
    
    CleanUpExtraTurnValues();
}

static void SpecialStatusesClear(void)
{
    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
    {
        s32 i;
        u8 *dataPtr = (u8 *)(&gSpecialStatuses[gActiveBattler]);

        for (i = 0; i < sizeof(struct SpecialStatus); ++i)
            dataPtr[i] = 0;
    }
}

static void CheckFocusPunch_ClearVarsBeforeTurnStarts(void)
{
    if (!(gHitMarker & HITMARKER_RUN))
    {
        while (gBattleStruct->focusPunchBattlerId < gBattlersCount)
        {
            gActiveBattler = gBattlerAttacker = gBattleStruct->focusPunchBattlerId;
            ++gBattleStruct->focusPunchBattlerId;
            if (gChosenMoveByBattler[gActiveBattler] == MOVE_FOCUS_PUNCH
             && !(gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP)
             && !(gDisableStructs[gBattlerAttacker].truantCounter)
             && !(gProtectStructs[gActiveBattler].noValidMoves))
            {
                BattleScriptExecute(BattleScript_FocusPunchSetUp);
                return;
            }
        }
    }
    TryClearRageStatuses();
    gCurrentTurnActionNumber = 0;
    {
        // something stupid needed to match
        u8 zero;

        gCurrentActionFuncId = gActionsByTurnOrder[(zero = 0)];
    }
    gDynamicBasePower = 0;
    gBattleStruct->dynamicMoveType = 0;
    gBattleMainFunc = RunTurnActionsFunctions;
    gBattleCommunication[3] = 0;
    gBattleCommunication[4] = 0;
    gBattleScripting.multihitMoveEffect = 0;
    gBattleResources->battleScriptsStack->size = 0;
}

enum MegaStates
{
	Mega_Check,
	Mega_CalcTurnOrder,
	Mega_SwitchInAbilities,
	Mega_Intimidate,
	Mega_End
};


static void RunTurnActionsFunctions(void)
{
    int i, j;
	u8 effect, savedActionFuncId;
	u8* megaBank = &(gBattleStruct->megaData.activeBank);

	if (gBattleOutcome != 0)
		gCurrentActionFuncId = B_ACTION_FINISHED;
	
	savedActionFuncId = gCurrentActionFuncId;

	if (!gBattleStruct->activatedCustapQuickClaw)
	{
		for (i = 0; i < gBattlersCount; ++i) //Loop through all battlers and play Quick Claw anim for each
		{
			if (gBattleStruct->CustapQuickClawIndicator & gBitTable[i])
			{
				gBattleStruct->CustapQuickClawIndicator &= ~(gBitTable[i]);

				if (gActionsByTurnOrder[i] != B_ACTION_USE_ITEM)
				{
					gBattleScripting.battler = i;
					gLastUsedItem = ITEM(i);
					if (ITEM_EFFECT(i) != HOLD_EFFECT_CUSTAP_BERRY)
						RecordItemEffectBattle(i, ITEM_EFFECT(i));

					BattleScriptExecute(BattleScript_QuickClaw);
					gCurrentActionFuncId = savedActionFuncId;
					return;
				}
			}
		}
	}
	
	gBattleStruct->activatedCustapQuickClaw = TRUE; //So the animation only plays once
    //Try to Mega Evolve/Ultra Burst Pokemon
	switch (gBattleStruct->megaData.state) {
		case Mega_Check:
			for (i = *megaBank; i < gBattlersCount; ++i, ++*megaBank)
			{
				u8 bank = gActiveBattler = gBattlerByTurnOrder[i];
				if (gBattleStruct->megaData.chosen[bank]
				&& !gBattleStruct->megaData.done[bank]
				&& !DoesZMoveUsageStopMegaEvolution(bank)
				&& (gCurrentActionFuncId == B_ACTION_USE_MOVE
				 || (gCurrentActionFuncId == B_ACTION_SWITCH && gChosenMoveByBattler[bank] == MOVE_PURSUIT)))
				{
					const u8* script = DoMegaEvolution(bank);
					if (script != NULL)
					{
						if (!(gBattleTypeFlags & BATTLE_TYPE_MEGA_BRAWL)) //As many mons can Mega Evolve as you want
							gBattleStruct->megaData.done[bank] = TRUE;

						gBattleStruct->megaData.chosen[bank] = 0;
						gBattleStruct->megaData.megaEvoInProgress = TRUE;
						gBattleStruct->megaData.script = script;
						if (!(gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_MULTI | BATTLE_TYPE_MEGA_BRAWL))
						&& SIDE(bank) == B_SIDE_PLAYER)
						{
							gBattleStruct->megaData.chosen[PARTNER(bank)] = 0;
							gBattleStruct->megaData.done[PARTNER(bank)] = TRUE;
						}
						else if (!(gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI | BATTLE_TYPE_MEGA_BRAWL))
						&& SIDE(bank) == B_SIDE_OPPONENT)
						{
							gBattleStruct->megaData.chosen[PARTNER(bank)] = 0;
							gBattleStruct->megaData.done[PARTNER(bank)] = TRUE;
						}
						RecordItemEffectBattle(bank, HOLD_EFFECT_MEGA_STONE);
						BattleScriptExecute(gBattleStruct->megaData.script);
						gCurrentActionFuncId = savedActionFuncId;
						return;
					}
				}
				else if (gBattleStruct->ultraData.chosen[bank] && !gBattleStruct->ultraData.done[bank])
				{
					const u8* script = DoMegaEvolution(bank);
					if (script != NULL)
					{
						if (!(gBattleTypeFlags & BATTLE_TYPE_MEGA_BRAWL)) //As many mons can Mega Evolve as you want
							gBattleStruct->ultraData.done[bank] = TRUE;

						gBattleStruct->ultraData.chosen[bank] = 0;
						gBattleStruct->megaData.megaEvoInProgress = TRUE;
						gBattleStruct->megaData.script = script;
						if (!(gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_MULTI | BATTLE_TYPE_MEGA_BRAWL))
						&& SIDE(bank) == B_SIDE_PLAYER)
						{
							gBattleStruct->ultraData.chosen[PARTNER(bank)] = 0;
							gBattleStruct->ultraData.done[PARTNER(bank)] = TRUE;
						}
						else if (!(gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI | BATTLE_TYPE_MEGA_BRAWL))
						&& SIDE(bank) == B_SIDE_OPPONENT)
						{
							gBattleStruct->ultraData.chosen[PARTNER(bank)] = 0;
							gBattleStruct->ultraData.done[PARTNER(bank)] = TRUE;
						}
						RecordItemEffectBattle(bank, HOLD_EFFECT_MEGA_STONE);
						BattleScriptExecute(gBattleStruct->megaData.script);
						gCurrentActionFuncId = savedActionFuncId;
						return;
					}
				}
			}
			if (gBattleStruct->megaData.megaEvoInProgress)
				++gBattleStruct->megaData.state;
			else
				gBattleStruct->megaData.state = Mega_End;
			return;

		case Mega_CalcTurnOrder:
			for (i = 0; i < gBattlersCount - 1; ++i)
			{
				for (j = i + 1; j < gBattlersCount; ++j)
				{
					u8 bank1 = gBattlerByTurnOrder[i];
					u8 bank2 = gBattlerByTurnOrder[j];
					if (gActionsByTurnOrder[i] != B_ACTION_USE_ITEM
						&& gActionsByTurnOrder[j] != B_ACTION_USE_ITEM
						&& gActionsByTurnOrder[i] != B_ACTION_SWITCH
						&& gActionsByTurnOrder[j] != B_ACTION_SWITCH
						&& gActionsByTurnOrder[i] != B_ACTION_FINISHED
						&& gActionsByTurnOrder[j] != B_ACTION_FINISHED)
					{
						if (GetWhoStrikesFirst(bank1, bank2, FALSE))
							SwapTurnOrder(i, j);
					}
				}
			}
			*megaBank = 0; //Reset the bank for the next loop
			++gBattleStruct->megaData.state;
			return;

		case Mega_SwitchInAbilities:
			while (*megaBank < gBattlersCount) {
				if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gBattlerByTurnOrder[*megaBank], 0, 0, 0))
					++effect;
				++*megaBank;

				if (effect) return;
			}
			*megaBank = 0;
			++gBattleStruct->megaData.state;
			return;

		case Mega_Intimidate:
			gBattleStruct->megaData.script = 0;
			gBattleStruct->megaData.state = Mega_End;
			gBattleStruct->megaData.activeBank = 0;
			gBattleStruct->megaData.megaEvoInProgress = FALSE;
			//Fallthrough
		case Mega_End:
			if (gCurrentActionFuncId != B_ACTION_USE_MOVE && gCurrentActionFuncId != B_ACTION_EXEC_SCRIPT) //Necessary because of Mega Evolving before Pursuit
				gBattleStruct->megaData.state = 0; //Reset since not everyone may have had a chance to Mega Evolve
	}

	*megaBank = 0;

	if (gCurrentActionFuncId == B_ACTION_USE_MOVE)
	{
		//Try to Dynamax/Gigantamax Pokemon
		for (i = 0; i < gBattlersCount; ++i)
		{
			u8 bank = gActiveBattler = gBattlerByTurnOrder[i];

			if (gBattleStruct->dynamaxData.toBeUsed[bank]
			&& !gBattleStruct->dynamaxData.used[bank]
			&& !DoesZMoveUsageStopMegaEvolution(bank)) //Same for Dynamax
			{
				const u8* script = GetDynamaxScript(bank);
				if (script != NULL)
				{
					gBattleStruct->dynamaxData.toBeUsed[bank] = FALSE;
					gBattleStruct->dynamaxData.used[bank] = TRUE;
					gBattleStruct->dynamaxData.timer[bank] = 3; //Dynamax lasts for 3 turns
					gBattleStruct->dynamaxData.partyIndex[SIDE(bank)] = gBitTable[gBattlerPartyIndexes[bank]];

					if (SIDE(bank) == B_SIDE_PLAYER)
					{
						gBattleStruct->dynamaxData.toBeUsed[PARTNER(bank)] = FALSE;
						gBattleStruct->dynamaxData.used[PARTNER(bank)] = TRUE;
					}
					else
					{
						gBattleStruct->dynamaxData.toBeUsed[PARTNER(bank)] = FALSE;
						gBattleStruct->dynamaxData.used[PARTNER(bank)] = TRUE;
					}

					BattleScriptExecute(script);
					gCurrentActionFuncId = savedActionFuncId;

					if (IS_BEHIND_SUBSTITUTE(bank))
					{
						BattleScriptPushCursor();
						gBattlerTarget = bank;
						gBattleMons[gActiveBattler].status2 &= ~(STATUS2_SUBSTITUTE);
						gDisableStructs[gActiveBattler].substituteHP = 0;
						gBattlescriptCurrInstr = BattleScript_SubstituteFade;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
					}
					return;
				}
			}
		}
	
		while (gBattleStruct->focusPunchBattlerId < gBattlersCount)
		{
            u16 chosenMove;
			gActiveBattler = gBattlerByTurnOrder[gBattleStruct->focusPunchBattlerId];
			++gBattleStruct->focusPunchBattlerId;
			chosenMove = gChosenMoveByBattler[gActiveBattler];
			if ((chosenMove == MOVE_FOCUSPUNCH || chosenMove == MOVE_BEAKBLAST || chosenMove == MOVE_SHELLTRAP)
			&& !(gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP)
			&& !(gDisableStructs[gActiveBattler].truantCounter)
			&& !(gProtectStructs[gActiveBattler].noValidMoves)
			&& !IsDynamaxed(gActiveBattler))
			{
				gBattlerAttacker = gBattleScripting.battler = gActiveBattler;
				if (chosenMove == MOVE_BEAKBLAST && !(gBattleStruct->BeakBlastByte & gBitTable[gActiveBattler]))
				{
					gBattleStruct->BeakBlastByte |= gBitTable[gActiveBattler];
					BattleScriptExecute(BattleScript_BeakBlastSetUp);
				}
				else if (chosenMove == MOVE_SHELLTRAP && !(gBattleStruct->playedShellTrapMessage & gBitTable[gActiveBattler]))
				{
					gBattleStruct->playedShellTrapMessage |= gBitTable[gActiveBattler];
					BattleScriptExecute(BattleScript_ShellTrapSetUp);
				}
				else if (!(gBattleStruct->playedFocusPunchMessage & gBitTable[gActiveBattler]))
				{
					gBattleStruct->playedFocusPunchMessage |= gBitTable[gActiveBattler];
					BattleScriptExecute(BattleScript_FocusPunchSetUp);
				}
				return;
			}
		}
	}

	gBattleStruct->savedTurnActionNumber = gCurrentTurnActionNumber;
	sTurnActionsFuncsTable[gCurrentActionFuncId]();

	TrySetupRaidBossRepeatedAttack(savedActionFuncId);

	if (gCurrentTurnActionNumber >= gBattlersCount) // everyone did their actions, turn finished
	{
		gHitMarker &= ~(HITMARKER_NON_ATTACK_DMG);
		gBattleMainFunc = sEndTurnFuncsTable[gBattleOutcome & 0x7F];
	}
	else
	{
		if (gBattleStruct->savedTurnActionNumber != gCurrentTurnActionNumber) // action turn has been done, clear hitmarker bits for another bank
		{
			gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING);
			gHitMarker &= ~(HITMARKER_UNABLE_TO_USE_MOVE);
		}
	}
}

static void HandleEndTurn_BattleWon(void)
{
    gCurrentActionFuncId = 0;
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        gBattleTextBuff1[0] = gBattleOutcome;
        gBattlerAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        gBattlescriptCurrInstr = BattleScript_LinkBattleWonOrLost;
        gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
    }
    else if (gBattleTypeFlags & (BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_BATTLE_TOWER))
    {
        BattleStopLowHpSound();
        PlayBGM(MUS_VICTORY_TRAINER);
        gBattlescriptCurrInstr = BattleScript_BattleTowerTrainerBattleWon;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && !(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        u16 id = gTrainerBattleOpponent_A;
		u8 specialMus = FALSE;
		u8 loop = FALSE;

        BattleStopLowHpSound();
        gBattlescriptCurrInstr = BattleScript_LocalTrainerBattleWon;
    VICTORY_MUSIC_SELECTION:
        switch (gTrainers[id].trainerClass)
        {
        case TRAINER_CLASS_LEADER:
        case TRAINER_CLASS_CHAMPION:
            PlayBGM(MUS_VICTORY_GYM_LEADER);
            break;
        case TRAINER_CLASS_BOSS:
        case TRAINER_CLASS_TEAM_ROCKET:
        case TRAINER_CLASS_COOLTRAINER:
        case TRAINER_CLASS_ELITE_FOUR:
        case TRAINER_CLASS_GENTLEMAN:
        default:
            PlayBGM(MUS_VICTORY_TRAINER);
            break;
        }
        if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && !specialMus && !loop) {
			id = VarGet(VAR_SECOND_OPPONENT);
			loop = TRUE;
			goto VICTORY_MUSIC_SELECTION;
		}
    }
    else
    {
        if (IsRaidBattle())
		#ifdef UNBOUND
			PlayBGM(BGM_VICTORY_GYM);
		#else
			PlayBGM(MUS_VICTORY_GYM_LEADER);
		#endif

        gBattlescriptCurrInstr = BattleScript_PayDayMoneyAndPickUpItems;
    }
    
    gSpecialVar_Result = 0;
    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void HandleEndTurn_BattleLost(void)
{
    gCurrentActionFuncId = 0;
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        gBattleTextBuff1[0] = gBattleOutcome;
        gBattlerAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        gBattlescriptCurrInstr = BattleScript_LinkBattleWonOrLost;
        gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
    }
    else
    {
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && GetTrainerBattleMode() == TRAINER_BATTLE_EARLY_RIVAL)
        {
            if (GetRivalBattleFlags() & RIVAL_BATTLE_HEAL_AFTER)
                gBattleCommunication[MULTISTRING_CHOOSER] = 1; // Dont do white out text
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = 2; // Do white out text
            gBattlerAttacker = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        }
        else
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        }
        gBattlescriptCurrInstr = BattleScript_LocalBattleLost;
    }
    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

void HandleEndTurn_RanFromBattle(void)
{
    gCurrentActionFuncId = 0;
    switch (gProtectStructs[gBattlerAttacker].fleeFlag)
    {
    default:
        gBattlescriptCurrInstr = BattleScript_GotAwaySafely;
        break;
    case 1:
        gBattlescriptCurrInstr = BattleScript_SmokeBallEscape;
        break;
    case 2:
        gBattlescriptCurrInstr = BattleScript_RanAwayUsingMonAbility;
        break;
    }
    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void HandleEndTurn_MonFled(void)
{
    gCurrentActionFuncId = 0;
    PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker]);
    gBattlescriptCurrInstr = BattleScript_WildMonFled;
    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void NaturalCureHeal(void)
{
    int bank;
	for (bank = 0; bank < gBattlersCount; ++bank)
	{
		if (ABILITY(bank) == ABILITY_NATURALCURE && gBattleMons[bank].status1)
			GetBankPartyData(bank)->status = 0;
	}
}

bool8 IsConsumable(u16 item)
{
	u8 effect = ItemId_GetHoldEffect(item);
    u32 i;
	for (i = 0; gConsumableItemEffects[i] != 0xFF; ++i) {
		if (effect == gConsumableItemEffects[i])
			return TRUE;
	}

	return FALSE;
}


static void RestoreNonConsumableItems(void)
{
	u16 none = ITEM_NONE;
	u16* items = gBattleStruct->itemBackup;
	#ifdef FLAG_KEEP_CONSUMABLE_ITEMS
	bool8 keepConsumables = FlagGet(FLAG_KEEP_CONSUMABLE_ITEMS);
	#else
	bool8 keepConsumables = FALSE;
	#endif

	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
        int i;
	    for (i = 0; i < PARTY_SIZE; ++i)
		{
			if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER
			||  keepConsumables
			||  items[i] == ITEM_NONE
			||  !IsConsumable(items[i]))
			{
				SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &items[i]);
			}
			else if (gPlayerParty[i].box.heldItem != items[i] //The player consumed their item, and then picked up another one
			&& IsConsumable(items[i]))
			{
				SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &none);
			}
		}
	}
}

static void RevertDynamax(void)
{
    int i;
	for (i = 0; i < gBattlersCount; ++i)
		EndBattleDynamaxRevert(i);
}

static void RecalcAllStats(void)
{
    int i;
	for (i = 0; i < PARTY_SIZE; ++i)
	{
		if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE
		&& !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, NULL))
			CalculateMonStats(&gPlayerParty[i]); //Only recalc Pokemon that need to be recalced
	}
}

static void BringBackTheDead(void)
{ //Used after Multi Battles that you lost, but your partner won
    int i;
	if (ViableMonCount(gPlayerParty) == 0)
	{
		for (i = 0; i < PARTY_SIZE; ++i)
		{
			if (gPlayerParty[i].box.species != 0 && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, 0))
			{
				gPlayerParty[i].hp = 1;
				break;
			}
		}
	}
}

static void EndPartnerBattlePartyRestore(void)
{
	int i;
	u8 counter = 0;
	struct Pokemon* backup = ExtensionState.partyBackup;

	if (ExtensionState.partyBackup != NULL)
	{
		if (gSelectedOrderFromParty[0] == 0)
		{ 	//Special 0x2F was not used
			memcpy(&gPlayerParty[3], ExtensionState.partyBackup, sizeof(struct Pokemon) * 3);
		}
		else
		{ 	//Special 0x2F was used
			struct Pokemon* foughtMons = Alloc(sizeof(struct Pokemon) * 3);
			if (foughtMons != NULL)
			{
				memcpy(foughtMons, gPlayerParty, sizeof(struct Pokemon) * 3);
				memset(gPlayerParty, 0x0, sizeof(struct Pokemon) * 6);
				for (i = 0; i < 3; ++i)
				{
					if (gSelectedOrderFromParty[i] != 0)
						memcpy(&gPlayerParty[gSelectedOrderFromParty[i] - 1], &foughtMons[i], sizeof(struct Pokemon));
				}
			}

			for (i = 0; i < PARTY_SIZE; ++i)
			{
				if (!FlagGet(FLAG_BATTLE_FACILITY))
					gSelectedOrderFromParty[i] = 0; //Reset for next battle

				if (gPlayerParty[i].box.species == 0)
					memcpy(&gPlayerParty[i], &backup[counter++], sizeof(struct Pokemon));
			}
		}
		Free(ExtensionState.partyBackup);
	}
}


//TO DO, restore party order like above
static void EndSkyBattlePartyRestore(void)
{
	int i;
	u8 counter = 0;
	struct Pokemon* backup = ExtensionState.skyBattlePartyBackup;

	if (backup != NULL)
	{
		struct Pokemon tempTeam[PARTY_SIZE] = {0};

		for (i = 0; i < PARTY_SIZE; ++i)
		{
			if (gSelectedOrderFromParty[i] != 0)
			{
				tempTeam[gSelectedOrderFromParty[i] - 1] = gPlayerParty[i];
				gSelectedOrderFromParty[i] = 0;
			}
		}

		for (i = 0; i < PARTY_SIZE; ++i)
		{
			if (tempTeam[i].box.species == SPECIES_NONE)
				tempTeam[i] = backup[counter++];
		}

		memcpy(gPlayerParty, tempTeam, sizeof(struct Pokemon) * PARTY_SIZE);
		Free(backup);
	}
}

const u16 gEndBattleFlagClearTable[] =
{
#ifdef FLAG_CATCH_TRAINERS_POKEMON
	FLAG_CATCH_TRAINERS_POKEMON,
#endif
#ifdef FLAG_ACTIVATE_TUTORIAL
	FLAG_ACTIVATE_TUTORIAL,
#endif
#ifdef FLAG_INVERSE
	FLAG_INVERSE,
#endif
#ifdef FLAG_SKY_BATTLE
	FLAG_SKY_BATTLE,
#endif
#ifdef FLAG_NO_CATCHING
	FLAG_NO_CATCHING,
#endif
#ifdef FLAG_NO_RUNNING
	FLAG_NO_RUNNING,
#endif
#ifdef FLAG_NO_CATCHING_AND_RUNNING
	FLAG_NO_CATCHING_AND_RUNNING,
#endif
#ifdef FLAG_WILD_CUSTOM_MOVES
	FLAG_WILD_CUSTOM_MOVES,
#endif
#ifdef FLAG_SMART_WILD
	FLAG_SMART_WILD,
#endif
#ifdef FLAG_SCALE_WILD_BOSS_LEVEL
	FLAG_SCALE_WILD_BOSS_LEVEL,
#endif
#ifdef FLAG_DOUBLE_WILD_BATTLE
	FLAG_DOUBLE_WILD_BATTLE,
#endif
#ifdef FLAG_SHINY_CREATION
	FLAG_SHINY_CREATION,
#endif
#ifdef FLAG_CAMOMONS_BATTLE
	FLAG_CAMOMONS_BATTLE,
#endif
#ifdef FLAG_BENJAMIN_BUTTERFREE_BATTLE
	FLAG_BENJAMIN_BUTTERFREE_BATTLE,
#endif
#ifdef FLAG_DYNAMAX_BATTLE
	FLAG_DYNAMAX_BATTLE,
#endif
#ifdef FLAG_RAID_BATTLE
	FLAG_RAID_BATTLE,
#endif
#ifdef FLAG_RAID_BATTLE_NO_FORCE_END
	FLAG_RAID_BATTLE_NO_FORCE_END,
#endif
#ifdef FLAG_KEEP_CONSUMABLE_ITEMS
	FLAG_KEEP_CONSUMABLE_ITEMS,
#endif
#ifdef FLAG_HIDDEN_ABILITY
	FLAG_HIDDEN_ABILITY,
#endif
#ifdef FLAG_GIGANTAMAXABLE
	FLAG_GIGANTAMAXABLE,
#endif
#ifdef FLAG_TAG_BATTLE
	FLAG_TAG_BATTLE,
#endif
#ifdef FLAG_TWO_OPPONENTS
	FLAG_TWO_OPPONENTS,
#endif
};

static void EndBattleFlagClear(void)
{
    u16 backup;
    u32 i;
    u16 inducer;

	for (i = 0; i < ARRAY_COUNT(gEndBattleFlagClearTable); ++i)
		FlagClear(gEndBattleFlagClearTable[i]);

	#ifdef VAR_STATUS_INDUCER
	inducer = VarGet(VAR_STATUS_INDUCER);
	if (inducer & 0xFF00) //Temporary status inducer
	{
		u8 status = inducer & 0xFF;
		u8 amount = ((inducer & 0xFF00) >> 8) - 1; //Subtract num battles by 1

		if (amount == 0) //Time's up
			VarSet(VAR_STATUS_INDUCER, 0);
		else
			VarSet(VAR_STATUS_INDUCER, status | (amount << 8));
	}
	#endif

	//Reset Totem Vars
	VarSet(VAR_TOTEM + 0, 0);	//Bank B_POSITION_PLAYER_LEFT's Stat
	VarSet(VAR_TOTEM + 1, 0);	//Bank B_POSITION_OPPONENT_LEFT's Stat
	VarSet(VAR_TOTEM + 2, 0);	//Bank B_POSITION_PLAYER_RIGHT's Stat
	VarSet(VAR_TOTEM + 3, 0);	//Bank B_POSITION_OPPONENT_RIGHT's Stat

	VarSet(VAR_TERRAIN, 0);
	VarSet(VAR_BATTLE_FACILITY_TRAINER1_NAME, 0xFFFF);
	VarSet(VAR_BATTLE_FACILITY_TRAINER2_NAME, 0xFFFF);
	#ifdef VAR_BATTLE_TRANSITION_LOGO
	VarSet(VAR_BATTLE_TRANSITION_LOGO, 0);
	#endif
	gFishingByte = FALSE;

	//Handle DexNav Chain
	if (gDexNavStartedBattle
	&& gCurrentDexNavChain < 100
	&& (gBattleOutcome == B_OUTCOME_WON || gBattleOutcome == B_OUTCOME_CAUGHT))
		++gCurrentDexNavChain;
	else
		gCurrentDexNavChain = 0;
	gDexNavStartedBattle = FALSE;

	backup = gTrainerBattleOpponent_B;
	memset(&ExtensionState, 0x0, sizeof(struct BattleExtensionState));
	gTrainerBattleOpponent_B = backup;
	gBattleCircusFlags = 0;
}

static void HealPokemonInFrontier(void)
{
    int i;
	//This helps with the issue of multi battle teams in the Battle Tower are not healed
	if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
		for (i = 0; i < PARTY_SIZE; ++i)
		{
			struct Pokemon* mon = &gPlayerParty[i];

			if (GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE)
				HealMon(mon);
		}
	}
}

static void HandleEndTurn_FinishBattle(void)
{
    if (gCurrentActionFuncId == B_ACTION_TRY_FINISH || gCurrentActionFuncId == B_ACTION_FINISHED)
    {
        if (!(gBattleTypeFlags & (BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_SAFARI | BATTLE_TYPE_FIRST_BATTLE | BATTLE_TYPE_LINK)))
        {
            for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
            {
                if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
                {
                    if (gBattleResults.playerMon1Species == SPECIES_NONE)
                    {
                        gBattleResults.playerMon1Species = gBattleMons[gActiveBattler].species;
                        StringCopy(gBattleResults.playerMon1Name, gBattleMons[gActiveBattler].nickname);
                    }
                    else
                    {
                        gBattleResults.playerMon2Species = gBattleMons[gActiveBattler].species;
                        StringCopy(gBattleResults.playerMon2Name, gBattleMons[gActiveBattler].nickname);
                    }
                }
            }
        }
        NaturalCureHeal();
        RestoreNonConsumableItems();
        RevertDynamax();
        FormsRevert(gPlayerParty);
        MegaRevert(gPlayerParty);
        GigantamaxRevert(gPlayerParty);
        UpdateBurmy();
        EndPartnerBattlePartyRestore();
        EndSkyBattlePartyRestore();
        RecalcAllStats();
        BringBackTheDead();
        EndBattleFlagClear();
        HealPokemonInFrontier();
        gTerrainType = 0; //Reset now b/c normal reset is after BG is loaded
        CalculatePlayerPartyCount(); //Party size can change after multi battle is over

        #ifdef CHANGE_MAP_WEATHER_BATTLE_END
        {
            u8 weather = gBattleWeather;
            if (weather == WEATHER_RAIN_ANY)
            {
                SetSav1Weather(WEATHER_RAIN);
            }
        }
        #endif

        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
            ClearRematchStateByTrainerId();
        BeginFastPaletteFade(3);
        FadeOutMapMusic(5);
        gBattleMainFunc = FreeResetData_ReturnToOvOrDoEvolutions;
        gCB2_AfterEvolution = BattleMainCB2;
    }
    else if (!gBattleControllerExecFlags)
    {
        gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
    }
}

static void FreeResetData_ReturnToOvOrDoEvolutions(void)
{
    if (!gPaletteFade.active)
    {
        ResetSpriteData();
        if (gLeveledUpInBattle == 0 || gBattleOutcome != B_OUTCOME_WON)
            gBattleMainFunc = ReturnFromBattleToOverworld;
        else
            gBattleMainFunc = TryEvolvePokemon;
        FreeAllWindowBuffers();
        if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
        {
            FreeMonSpritesGfx();
            FreeBattleSpritesData();
            FreeBattleResources();
        }
    }
}

static void TryEvolvePokemon(void)
{
    s32 i;

    while (gLeveledUpInBattle != 0)
    {
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            if (gLeveledUpInBattle & gBitTable[i])
            {
                u16 species;
                u8 levelUpBits = gLeveledUpInBattle;

                levelUpBits &= ~(gBitTable[i]);
                gLeveledUpInBattle = levelUpBits;
                species = GetEvolutionTargetSpecies(&gPlayerParty[i], EVO_MODE_NORMAL, levelUpBits);
                if (species != SPECIES_NONE)
                {
                    gBattleMainFunc = WaitForEvoSceneToFinish;
                    EvolutionScene(&gPlayerParty[i], species, 0x81, i);
                    return;
                }
            }
        }
    }
    gBattleMainFunc = ReturnFromBattleToOverworld;
}

static void WaitForEvoSceneToFinish(void)
{
    if (gMain.callback2 == BattleMainCB2)
        gBattleMainFunc = TryEvolvePokemon;
}

static void ReturnFromBattleToOverworld(void)
{
    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        RandomlyGivePartyPokerus(gPlayerParty);
        PartySpreadPokerus(gPlayerParty);
    }
    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK) || !gReceivedRemoteLinkPlayers)
    {
        gSpecialVar_Result = gBattleOutcome;
        gMain.inBattle = FALSE;
        gMain.callback1 = gPreBattleCallback1;
        if (gBattleTypeFlags & BATTLE_TYPE_ROAMER)
        {
            UpdateRoamerHPStatus(&gEnemyParty[0]);
#ifdef BUGFIX
            if ((gBattleOutcome == B_OUTCOME_WON) || gBattleOutcome == B_OUTCOME_CAUGHT)
#else
            if ((gBattleOutcome & B_OUTCOME_WON) || gBattleOutcome == B_OUTCOME_CAUGHT) // Bug: When Roar is used by roamer, gBattleOutcome is B_OUTCOME_PLAYER_TELEPORTED (5).
#endif                                                                                  // & with B_OUTCOME_WON (1) will return TRUE and deactivates the roamer.
                SetRoamerInactive();
        }
        m4aSongNumStop(SE_LOW_HEALTH);
        SetMainCallback2(gMain.savedCallback);
    }
}

void RunBattleScriptCommands_PopCallbacksStack(void)
{
    if (gCurrentActionFuncId == B_ACTION_TRY_FINISH || gCurrentActionFuncId == B_ACTION_FINISHED)
    {
        if (gBattleResources->battleCallbackStack->size != 0)
            --gBattleResources->battleCallbackStack->size;
        gBattleMainFunc = gBattleResources->battleCallbackStack->function[gBattleResources->battleCallbackStack->size];
    }
    else
    {
        if (!gBattleControllerExecFlags)
            gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
    }
}

void RunBattleScriptCommands(void)
{
    if (!gBattleControllerExecFlags)
        gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
}

static void HandleAction_UseMove(void)
{
    u8 side;
    u8 moveType;
    int i, j;
    ability_t atkAbility;
    u8 moveTarget;
    u8 selectedTarget;

    if (!gBattleStruct->PledgeHelper) //Don't recalculate during pledge execution
	{
		//Recalculate turn order before each attack
		for (i = gCurrentTurnActionNumber; i < gBattlersCount - 1; ++i)
		{
			for (j = i + 1; j < gBattlersCount; ++j)
			{
				u8 bank1 = gBattlerByTurnOrder[i];
				u8 bank2 = gBattlerByTurnOrder[j];
				if (gActionsByTurnOrder[i] != B_ACTION_USE_ITEM
					&& gActionsByTurnOrder[j] != B_ACTION_USE_ITEM
					&& gActionsByTurnOrder[i] != B_ACTION_SWITCH
					&& gActionsByTurnOrder[j] != B_ACTION_SWITCH
					&& gActionsByTurnOrder[i] != B_ACTION_FINISHED
					&& gActionsByTurnOrder[j] != B_ACTION_FINISHED
					&& !(gBitTable[bank1] & gBattleStruct->quashed)
					&& !(gBitTable[bank2] & gBattleStruct->quashed))
				{
					if (GetWhoStrikesFirstUseLastBracketCalc(bank1, bank2))
						SwapTurnOrder(i, j);
				}
			}
		}
	}

    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];

    if (gBattleStruct->absentBattlerFlags & gBitTable[gBattlerAttacker])
    {
        gCurrentActionFuncId = B_ACTION_FINISHED;
        return;
    }
    
    gHpDealt = 0;
	gCritMultiplier = BASE_CRIT_MULTIPLIER;
	gBattleScripting.dmgMultiplier = 1;
	gBattleStruct->atkCancellerTracker = 0;
	gMoveResultFlags = 0;
	gMultiHitCounter = 0;
	gBattleStruct->OriginalAttackerTargetCount = 0;
	gBattleStruct->ParentalBondOn = FALSE;
	gBattleStruct->DancerInProgress = FALSE;
	gBattleStruct->MoveBounceInProgress = FALSE;
	gBattleStruct->breakDisguiseSpecialDmg = FALSE;
	gBattleStruct->zMoveData.active = FALSE;
	gBattleStruct->batonPassing = FALSE;
	gBattleStruct->dynamaxData.nullifiedStats = FALSE;
	gBattleStruct->dynamaxData.attackAgain = FALSE;
	gBattleCommunication[MOVE_EFFECT_BYTE] = 0; //Remove secondary effects
	gBattleCommunication[6] = 0;
	gCurrMovePos = gChosenMovePos = gBattleStruct->chosenMovePositions[gBattlerAttacker];

	gBattleStruct->totalDamageGiven = 0;
	gBattleStruct->selfInflictedDamage = 0;
	gBattleStruct->lessThanHalfHPBeforeShellBell = FALSE;
	//Clear spread move things
	gBattleStruct->doneDoublesSpreadHit = FALSE;
	gBattleStruct->calculatedSpreadMoveData = FALSE;
	gBattleStruct->calculatedSpreadMoveAccuracy = FALSE;

    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
	{
		gBattleStruct->DamageTaken[i] = 0;
		gBattleStruct->turnDamageTaken[i] = 0;
		gBattleStruct->criticalMultiplier[i] = 0;
		gBattleStruct->ResultFlags[i] = 0;
		gBattleStruct->missStringId[i] = 0;
		gBattleStruct->noResultString[i] = 0;
		gBattleStruct->statFellThisTurn[i] = FALSE;
	}

	if (IsRaidBattle())
		gBattleStruct->dynamaxData.turnStartHP = gBattleMons[BANK_RAID_BOSS].hp;
//Get Move to be Used
	if (gProtectStructs[gBattlerAttacker].noValidMoves)
	{
		gProtectStructs[gBattlerAttacker].noValidMoves = 0;
		gCurrentMove = gChosenMove = MOVE_STRUGGLE;
		gHitMarker |= HITMARKER_NO_PPDEDUCT;
		gBattleStruct->moveTarget[gBattlerAttacker] = GetMoveTarget(MOVE_STRUGGLE, 0);
	}
	else if (gBattleMons[gBattlerAttacker].status2 & STATUS2_RECHARGE)
	{
		gChosenMoveByBattler[gBattlerAttacker] = gCurrentMove = gChosenMove = gLockedMoves[gBattlerAttacker];
	}
	else if (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)
	{
		gChosenMoveByBattler[gBattlerAttacker] = gCurrentMove = gChosenMove = gLockedMoves[gBattlerAttacker];

		if (FindMovePositionInMoveset(gLockedMoves[gBattlerAttacker], gBattlerAttacker) == 4) //The Pokemon doesn't know the move it's locked into
		{
			CancelMultiTurnMoves(gBattlerAttacker);
			gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBattlerAttacker, gCurrentMove);
			gBattlerTarget = gBattleStruct->moveTarget[gBattlerAttacker];
			gBattlescriptCurrInstr = BattleScript_NoTargetMoveFailed;
			gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
			return;
		}
	}
	// Encore forces you to use the same move
	else if (gDisableStructs[gBattlerAttacker].encoredMove != MOVE_NONE
		  && gDisableStructs[gBattlerAttacker].encoredMove == gBattleMons[gBattlerAttacker].moves[gDisableStructs[gBattlerAttacker].encoredMovePos]
		  && !gBattleStruct->zMoveData.toBeUsed[gBattlerAttacker] //If a Z-Move was chosen, it can still be used
		  && !gBattleStruct->dynamaxData.active)
	{
		gChosenMove = gBattleMons[gBattlerAttacker].moves[gCurrMovePos];
		if (gChosenMove != gDisableStructs[gBattlerAttacker].encoredMove) //The encored move wasn't chosen
			gBattleStruct->moveTarget[gBattlerAttacker] = GetMoveTarget(gDisableStructs[gBattlerAttacker].encoredMove, 0); //Get correct target

		gCurrentMove = gChosenMove = gDisableStructs[gBattlerAttacker].encoredMove;
		gCurrMovePos = gChosenMovePos = gDisableStructs[gBattlerAttacker].encoredMovePos;
	}
	// Check if the encored move wasn't overwritten
	else if (gDisableStructs[gBattlerAttacker].encoredMove != MOVE_NONE
		  && gDisableStructs[gBattlerAttacker].encoredMove != gBattleMons[gBattlerAttacker].moves[gDisableStructs[gBattlerAttacker].encoredMovePos]
		  && !gBattleStruct->zMoveData.toBeUsed[gBattlerAttacker] //If a Z-Move was chosen, it can still be used
		  && !gBattleStruct->dynamaxData.active)
	{
		gCurrMovePos = gChosenMovePos = gDisableStructs[gBattlerAttacker].encoredMovePos;
		gCurrentMove = gChosenMove = gBattleMons[gBattlerAttacker].moves[gCurrMovePos];
		gDisableStructs[gBattlerAttacker].encoredMove = MOVE_NONE;
		gDisableStructs[gBattlerAttacker].encoredMovePos = 0;
		gDisableStructs[gBattlerAttacker].encoreTimer = 0;
		gBattleStruct->moveTarget[gBattlerAttacker] = GetMoveTarget(gCurrentMove, 0);
	}
	else if (gBattleMons[gBattlerAttacker].moves[gCurrMovePos] != gChosenMoveByBattler[gBattlerAttacker])
	{
		gCurrentMove = gChosenMove = gBattleMons[gBattlerAttacker].moves[gCurrMovePos];
		gBattleStruct->moveTarget[gBattlerAttacker] = GetMoveTarget(gCurrentMove, 0);
	}
	else
	{
		gCurrentMove = gChosenMove = gBattleMons[gBattlerAttacker].moves[gCurrMovePos];
	}

    if (gBattleMons[gBattlerAttacker].hp)
	{
		if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
			gBattleResults.lastUsedMovePlayer = gCurrentMove;
		else
			gBattleResults.lastUsedMoveOpponent = gCurrentMove;
	}

    if (gBattleStruct->zMoveData.toBeUsed[gBattlerAttacker])
	{
		gBattleStruct->zMoveData.active = TRUE;

		if (SPLIT(gCurrentMove) != SPLIT_STATUS)
		{
			u16 zmove = GetSpecialZMove(gCurrentMove, SPECIES(gBattlerAttacker), ITEM(gBattlerAttacker));
			if (zmove != MOVE_NONE && zmove != 0xFFFF) //There's a special Z-Move
				gCurrentMove = zmove;
			else if (zmove != 0xFFFF) //This check is needed b/c in Benjamin Butterfree you can select a special Z-Move but then lose it before it activates
				gCurrentMove = GetTypeBasedZMove(gBattleMons[gBattlerAttacker].moves[gCurrMovePos], gBattlerAttacker);
			else
			{
				gBattleStruct->zMoveData.active = FALSE;
				gBattleStruct->zMoveData.toBeUsed[gBattlerAttacker] = FALSE;
			}
		}
		else
		{
			//This check is needed b/c in Benjamin Butterfree you can select a special Z-Move but then lose it before it activates
			if (GetSpecialZMove(gCurrentMove, SPECIES(gBattlerAttacker), ITEM(gBattlerAttacker)) == 0xFFFF)
			{
				gBattleStruct->zMoveData.active = FALSE;
				gBattleStruct->zMoveData.toBeUsed[gBattlerAttacker] = FALSE;
			}
		}
	}
	else if (IsDynamaxed(gBattlerAttacker))
	{
		if (IsRaidBattle() && gBattlerAttacker == BANK_RAID_BOSS)
		{
			u8 split = SPLIT(gCurrentMove);
			bool8 isBannedMove = CheckTableForMove(gCurrentMove, gRaidBattleBannedRaidMonMoves)
							  || CheckTableForMove(gCurrentMove, gRaidBattleBannedMoves)
							  || IsUnusableMove(gCurrentMove, gBattlerAttacker, 0xFF, 1, ABILITY(gBattlerAttacker), ITEM_EFFECT(gBattlerAttacker), CHOICED_MOVE(gBattlerAttacker));

			if (isBannedMove && split != SPLIT_STATUS) //Use banned status move - don't use Max Guard
				goto TURN_MOVE_INTO_MAX_MOVE;
			else if (IsRaidBossUsingRegularMove(gBattlerAttacker, gCurrentMove))
			{
				//Samll chance to use regular damaging move
				//Raid wild Pokemon shouldn't be using Max Guard
			}
			else
				goto TURN_MOVE_INTO_MAX_MOVE;
		}
		else if (gCurrentMove != MOVE_STRUGGLE)
		{
			TURN_MOVE_INTO_MAX_MOVE:
			gBattleStruct->dynamaxData.active = TRUE;
			gCurrentMove = GetMaxMove(gBattlerAttacker, gCurrMovePos);
		}
	}

	gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBattlerAttacker, gCurrentMove);
	moveType = gBattleStruct->dynamicMoveType;

//Get Move Target
	atkAbility = ABILITY(gBattlerAttacker);
	moveTarget = gBattleMoves[gCurrentMove].target;
	side = SIDE(gBattlerAttacker) ^ BIT_SIDE;
	selectedTarget = gBattleStruct->moveTarget[gBattlerAttacker];

	if ((moveTarget == MOVE_TARGET_SELECTED || moveTarget == MOVE_TARGET_RANDOM)
	&& IsMoveRedirectedByFollowMe(gCurrentMove, gBattlerAttacker, side))
	{
		gBattlerTarget = gSideTimers[side].followmeTarget;
	}
	else if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		  &&  gSideTimers[side].followmeTimer == 0
		  && !IsMoveRedirectionPrevented(gCurrentMove, atkAbility)
		  && (SPLIT(gCurrentMove) != SPLIT_STATUS || gBattleMoves[gCurrentMove].target != MOVE_TARGET_USER)
		  && !(gBattleMoves[gCurrentMove].target & (MOVE_TARGET_FOES_AND_ALLY | MOVE_TARGET_BOTH)))
	{ //Try Ability Redirection
		switch (moveType) {
			case TYPE_WATER:
				if (ABILITY(selectedTarget) != ABILITY_STORMDRAIN)
				{
					if (ABILITY(SIDE(gBattlerAttacker) ^ BIT_SIDE) == ABILITY_STORMDRAIN)
					{
						gBattlerTarget = SIDE(gBattlerAttacker) ^ BIT_SIDE;
						gSpecialStatuses[gBattlerTarget].lightningRodRedirected = 1;
					}
					else if (ABILITY(PARTNER(SIDE(gBattlerAttacker) ^ BIT_SIDE)) == ABILITY_STORMDRAIN)
					{
						gBattlerTarget = PARTNER(SIDE(gBattlerAttacker) ^ BIT_SIDE);
						gSpecialStatuses[gBattlerTarget].lightningRodRedirected = 1;
					}
					else if (ABILITY(PARTNER(gBattlerAttacker)) == ABILITY_STORMDRAIN)
					{
						gBattlerTarget = PARTNER(gBattlerAttacker);
						gSpecialStatuses[gBattlerTarget].lightningRodRedirected = 1;
					}
				}

				break;

			case TYPE_ELECTRIC:
				if (ABILITY(selectedTarget) != ABILITY_LIGHTNINGROD)
				{
					if (ABILITY(SIDE(gBattlerAttacker) ^ BIT_SIDE) == ABILITY_LIGHTNINGROD)
					{
						gBattlerTarget = SIDE(gBattlerAttacker) ^ BIT_SIDE;
						gSpecialStatuses[gBattlerTarget].lightningRodRedirected = 1;
					}
					else if (ABILITY(PARTNER(SIDE(gBattlerAttacker) ^ BIT_SIDE)) == ABILITY_LIGHTNINGROD)
					{
						gBattlerTarget = PARTNER(SIDE(gBattlerAttacker) ^ BIT_SIDE);
						gSpecialStatuses[gBattlerTarget].lightningRodRedirected = 1;
					}
					else if (ABILITY(PARTNER(gBattlerAttacker)) == ABILITY_LIGHTNINGROD)
					{
						gBattlerTarget = PARTNER(gBattlerAttacker);
						gSpecialStatuses[gBattlerTarget].lightningRodRedirected = 1;
					}
				}
				break;
		}

		if (!gSpecialStatuses[gBattlerTarget].lightningRodRedirected)
		{
			if (gBattleMoves[gCurrentMove].target & MOVE_TARGET_RANDOM
			&& !IsAnyMaxMove(gCurrentMove))
				goto CHOOSE_RANDOM_TARGET_DOUBLES;
			else
				goto CHOOSE_REGULAR_TARGET_DOUBLES;
		}
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
		  && gBattleMoves[gCurrentMove].target & MOVE_TARGET_RANDOM
		  && !IsAnyMaxMove(gCurrentMove))
	{
	CHOOSE_RANDOM_TARGET_DOUBLES:
		if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
		{
			if (IsRaidBattle() || Random() & 1)
				gBattlerTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
			else
				gBattlerTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
		}
		else
		{
			if (Random() & 1)
				gBattlerTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
			else
				gBattlerTarget = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
		}

		if (gAbsentBattlerFlags & gBitTable[gBattlerTarget]
		&& SIDE(gBattlerAttacker) != SIDE(gBattlerTarget))
		{
			gBattlerTarget = GetBattlerAtPosition(PARTNER(gBattlerTarget));
		}
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
		 && gBattleMoves[gCurrentMove].target & MOVE_TARGET_FOES_AND_ALLY)
	{
		while ((gBattlerTarget = GetNextMultiTarget()) != 0xFF && gBattleMons[gBattlerTarget].hp == 0)
		{
			++gBattleStruct->OriginalAttackerTargetCount;
		}

		if (gBattlerTarget == 0xFF) //No targets left
			gBattlerTarget = FOE(gBattlerAttacker); //Doesn't matter who, as long as not attacker
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
	CHOOSE_REGULAR_TARGET_DOUBLES:
		gBattlerTarget = selectedTarget;
		if (gAbsentBattlerFlags & gBitTable[gBattlerTarget])
		{
			if (SIDE(gBattlerAttacker) != SIDE(gBattlerTarget))
			{
				gBattlerTarget = PARTNER(gBattlerTarget);
			}
			else //Targeted Partner
			{
				gBattlerTarget = GetBattlerAtPosition(GetBattlerPosition(gBattlerAttacker) ^ BIT_SIDE);
				if (gAbsentBattlerFlags & gBitTable[gBattlerTarget])
					gBattlerTarget = PARTNER(gBattlerTarget);
			}
		}
	}
	else
		gBattlerTarget = selectedTarget;

	// choose battlescript
	if (gStatuses3[gBattlerAttacker] & STATUS3_SKY_DROP_ATTACKER
	&& gBattleMons[gBattleStruct->skyDropAttackersTarget[gBattlerAttacker]].hp == 0)
	{
		gStatuses3[gBattlerAttacker] &= ~(STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET | STATUS3_ON_AIR);
		gBattleStruct->skyDropTargetsAttacker[gBattlerTarget] = 0;
		gBattleStruct->skyDropAttackersTarget[gBattlerAttacker] = 0;
		gBattlescriptCurrInstr = BattleScript_NoTargetMoveFailed;
	}
	else if (gBattleMons[gBattlerTarget].hp == 0
	&&  AttacksThisTurn(gBattlerAttacker, gCurrentMove) == 2 //Not charging move
	&&  !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_OPPONENTS_FIELD)) //Moves like Stealth Rock can still be used
		gBattlescriptCurrInstr = BattleScript_NoTargetMoveFailed;
	else
		gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];

    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void TrySetupRaidBossRepeatedAttack(u8 actionFuncId)
{
	if (IsRaidBattle() && gBattleStruct->dynamaxData.attackAgain && gBattleStruct->dynamaxData.repeatedAttacks < 2 && actionFuncId == B_ACTION_FINISHED)
	{
		u8 i, moveLimitations, viableMoves, curPos;
		gBattleStruct->dynamaxData.attackAgain = FALSE;

		gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber - 1]; //Get original attacker

		if (gBattlerAttacker != BANK_RAID_BOSS //Just in case the player KOs the partner and sets the bit
		|| CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, gBattlerAttacker, FOE(gBattlerAttacker)) == 0) //Don't attack again if no one left to hit
			return;

		moveLimitations = CheckMoveLimitations(gBattlerAttacker, 0, 0xFF);

		for (i = 0, viableMoves = 0; i < MAX_MON_MOVES; ++i)
		{
			if (gBattleMons[gBattlerAttacker].moves[i] == MOVE_NONE)
				continue;

			if (!(gBitTable[i] & moveLimitations)) //Move is usable
			{
				++viableMoves;
				break; //We only need at least one viable
			}
		}

		if (viableMoves == 0)
			return; //Don't force another move if the raid boss can't use one

		++gBattleStruct->dynamaxData.repeatedAttacks;
		gCurrentActionFuncId = gActionsByTurnOrder[--gCurrentTurnActionNumber] = B_ACTION_USE_MOVE;

		do
		{
			curPos = gBattleStruct->chosenMovePositions[gBattlerAttacker] = Random() & 3;
			gCurrentMove = gBattleMons[gBattlerAttacker].moves[curPos]; //Choose a new move
		} while (gCurrentMove == MOVE_NONE || (gBitTable[curPos] & moveLimitations));

		gBattleStruct->moveTarget[gBattlerAttacker] = GetMoveTarget(gCurrentMove, FALSE);

		gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING);
		gHitMarker &= ~(HITMARKER_UNABLE_TO_USE_MOVE);
	}
}


static void HandleAction_Switch(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gActionSelectionCursor[gBattlerAttacker] = 0;
    gMoveSelectionCursor[gBattlerAttacker] = 0;
    PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gBattlerAttacker, *(gBattleStruct->battlerPartyIndexes + gBattlerAttacker));
    gBattleScripting.battler = gBattlerAttacker;
    gBattlescriptCurrInstr = BattleScript_ActionSwitch;
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
    if (gBattleResults.playerSwitchesCounter < 255)
        ++gBattleResults.playerSwitchesCounter;
}

static void HandleAction_UseItem(void)
{
    gBattlerAttacker = gBattlerTarget = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    ClearFuryCutterDestinyBondGrudge(gBattlerAttacker);
    gLastUsedItem = gBattleBufferB[gBattlerAttacker][1] | (gBattleBufferB[gBattlerAttacker][2] << 8);
    if (GetPocketByItemId(gLastUsedItem) == POCKET_POKE_BALLS) // is ball
    {
        u8 ballType = ItemId_GetType(gLastUsedItem);
		if (ballType == BALL_TYPE_SAFARI_BALL)
			gBattlescriptCurrInstr = gBattlescriptsForBallThrow[1];
		else
            gBattlescriptCurrInstr = gBattlescriptsForBallThrow[0];
    }
    else if (gLastUsedItem == ITEM_POKE_DOLL || gLastUsedItem == ITEM_FLUFFY_TAIL)
    {
        gBattlescriptCurrInstr = gBattlescriptsForRunningByItem[0];
    }
    else if (gLastUsedItem == ITEM_POKE_FLUTE)
    {
        gBattlescriptCurrInstr = gBattlescriptsForRunningByItem[1];
    }
    else if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
    {
        gBattlescriptCurrInstr = gBattlescriptsForUsingItem[0];
    }
    else
    {
        gBattleScripting.battler = gBattlerAttacker;
        switch (*(gBattleStruct->AI_itemType + (gBattlerAttacker >> 1)))
        {
        case AI_ITEM_FULL_RESTORE:
        case AI_ITEM_HEAL_HP:
            break;
        case AI_ITEM_CURE_CONDITION:
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            if (*(gBattleStruct->AI_itemFlags + gBattlerAttacker / 2) & 1)
            {
                if (*(gBattleStruct->AI_itemFlags + gBattlerAttacker / 2) & 0x3E)
                    gBattleCommunication[MULTISTRING_CHOOSER] = 5;
            }
            else
            {
                while (!(*(gBattleStruct->AI_itemFlags + gBattlerAttacker / 2) & 1))
                {
                    *(gBattleStruct->AI_itemFlags + gBattlerAttacker / 2) >>= 1;
                    ++gBattleCommunication[MULTISTRING_CHOOSER];
                }
            }
            break;
        case AI_ITEM_X_STAT:
            gBattleCommunication[MULTISTRING_CHOOSER] = 4;
            if (*(gBattleStruct->AI_itemFlags + (gBattlerAttacker >> 1)) & 0x80)
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = 5;
            }
            else
            {
                PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_ATK);
                PREPARE_STRING_BUFFER(gBattleTextBuff2, CHAR_X);
                while (!((*(gBattleStruct->AI_itemFlags + (gBattlerAttacker >> 1))) & 1))
                {
                    *(gBattleStruct->AI_itemFlags + gBattlerAttacker / 2) >>= 1;
                    ++gBattleTextBuff1[2];
                }
                gBattleScripting.animArg1 = gBattleTextBuff1[2] + 14;
                gBattleScripting.animArg2 = 0;
            }
            break;
        case AI_ITEM_GUARD_SPECS:
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            break;
        }

        gBattlescriptCurrInstr = gBattlescriptsForUsingItem[*(gBattleStruct->AI_itemType + gBattlerAttacker / 2)];
    }
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

bool8 TryRunFromBattle(u8 bank)
{
    bool8 effect = FALSE;
	u8 itemEffect;
	u8 speedVar;

	itemEffect = ITEM_EFFECT(bank);
	gPotentialItemEffectBattler = bank;

	if (AreAllKindsOfRunningPrevented())
	{
		return FALSE;
	}
	else if (IsRaidBattle() && RAID_BATTLE_END)
	{
		++effect; //So you can always run at the end of a Raid
	}
	else if (IsOfType(bank, TYPE_GHOST))
	{
		++effect;
	}
	else if (itemEffect == HOLD_EFFECT_CAN_ALWAYS_RUN)
	{
		gLastUsedItem = ITEM(bank);
		gProtectStructs[bank].fleeFlag = 1;
		++effect;
	}
	else if (ABILITY(bank) == ABILITY_RUNAWAY)
	{
		gLastUsedAbility = ABILITY_RUNAWAY;
		gProtectStructs[bank].fleeFlag = 2;
		++effect;
	}
	#ifndef NO_GHOST_BATTLES
	else if ((gBattleTypeFlags & (BATTLE_TYPE_WILD_SCRIPTED | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST)
	{
		if (SIDE(bank) == B_SIDE_PLAYER)
			++effect;
	}
	#endif
	else if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_TOWER) && gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		++effect;
	}
	else
	{
		if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
		{
		SINGLE_FLEE_CALC:
			if (gBattleMons[bank].speed < gBattleMons[BATTLE_OPPOSITE(bank)].speed)
			{
				speedVar = ((gBattleMons[bank].speed * 128) / (gBattleMons[BATTLE_OPPOSITE(bank)].speed)) + (gBattleStruct->runTries * 30);
				if (speedVar > (Random() & 0xFF))
					++effect;
			}
			else // same speed or faster
			{
				++effect;
			}
		}
		else //Wild Double
		{
			if (gBattleMons[BATTLE_OPPOSITE(bank)].hp)
				goto SINGLE_FLEE_CALC;

			if (gBattleMons[bank].speed < gBattleMons[PARTNER(BATTLE_OPPOSITE(bank))].speed)
			{
				speedVar = ((gBattleMons[bank].speed * 128) / (gBattleMons[PARTNER(BATTLE_OPPOSITE(bank))].speed)) + (gBattleStruct->runTries * 30);
				if (speedVar > (Random() & 0xFF))
					++effect;
			}
			else // same speed or faster
			{
				++effect;
			}
		}

		gBattleStruct->runTries++;
	}

	if (effect)
	{
		gCurrentTurnActionNumber = gBattlersCount;
		gBattleOutcome = B_OUTCOME_RAN;
	}

	return effect;
}

static void HandleAction_Run(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];

    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        gCurrentTurnActionNumber = gBattlersCount;
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
        {
            if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
            {
                if (gChosenActionByBattler[gActiveBattler] == B_ACTION_RUN)
                    gBattleOutcome |= B_OUTCOME_LOST;
            }
            else
            {
                if (gChosenActionByBattler[gActiveBattler] == B_ACTION_RUN)
                    gBattleOutcome |= B_OUTCOME_WON;
            }
        }
        gBattleOutcome |= B_OUTCOME_LINK_BATTLE_RAN;
    }
    else
    {
        if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
        {
            if (!TryRunFromBattle(gBattlerAttacker)) // failed to run away
            {
                ClearFuryCutterDestinyBondGrudge(gBattlerAttacker);
                gBattleCommunication[MULTISTRING_CHOOSER] = 3;
                gBattlescriptCurrInstr = BattleScript_PrintFailedToRunString;
                gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
            }
        }
        else
        {
            if (gBattleMons[gBattlerAttacker].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION))
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = 4;
                gBattlescriptCurrInstr = BattleScript_PrintFailedToRunString;
                gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
            }
            else
            {
                gCurrentTurnActionNumber = gBattlersCount;
                gBattleOutcome = B_OUTCOME_MON_FLED;
            }
        }
    }
}

static void HandleAction_WatchesCarefully(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    if (gBattleStruct->safariGoNearCounter != 0)
    {
        --gBattleStruct->safariGoNearCounter;
        if (gBattleStruct->safariGoNearCounter == 0)
        {
            *(&gBattleStruct->safariCatchFactor) = gBaseStats[GetMonData(gEnemyParty, MON_DATA_SPECIES)].catchRate * 100 / 1275;
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        }
        else
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        }
    }
    else
    {
        if (gBattleStruct->safariPkblThrowCounter != 0)
        {
            --gBattleStruct->safariPkblThrowCounter;
            if (gBattleStruct->safariPkblThrowCounter == 0)
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            else
                gBattleCommunication[5] = 2;
        }
        else
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        }
    }
    gBattlescriptCurrInstr = gBattlescriptsForSafariActions[0];
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_SafariZoneBallThrow(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    --gNumSafariBalls;
    gLastUsedItem = ITEM_SAFARI_BALL;
    gBattlescriptCurrInstr = gBattlescriptsForBallThrow[ITEM_SAFARI_BALL];
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_ThrowBait(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattleStruct->safariPkblThrowCounter += Random() % 5 + 2;
    if (gBattleStruct->safariPkblThrowCounter > 6)
        gBattleStruct->safariPkblThrowCounter = 6;
    gBattleStruct->safariGoNearCounter = 0;
    gBattleStruct->safariCatchFactor >>= 1;
    if (gBattleStruct->safariCatchFactor <= 2)
        gBattleStruct->safariCatchFactor = 3;
    gBattlescriptCurrInstr = gBattlescriptsForSafariActions[2];
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_ThrowRock(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattleStruct->safariGoNearCounter += Random() % 5 + 2;
    if (gBattleStruct->safariGoNearCounter > 6)
        gBattleStruct->safariGoNearCounter = 6;
    gBattleStruct->safariPkblThrowCounter = 0;
    gBattleStruct->safariCatchFactor <<= 1;
    if (gBattleStruct->safariCatchFactor > 20)
        gBattleStruct->safariCatchFactor = 20;
    gBattlescriptCurrInstr = gBattlescriptsForSafariActions[1];
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_SafariZoneRun(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    PlaySE(SE_FLEE);
    gCurrentTurnActionNumber = gBattlersCount;
    gBattleOutcome = B_OUTCOME_RAN;
}

static void HandleAction_OldManBallThrow(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker])
    gBattlescriptCurrInstr = gBattlescriptsForSafariActions[3];
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
    gActionsByTurnOrder[1] = B_ACTION_FINISHED;
}

static void HandleAction_TryFinish(void)
{
    u8 i;
    
    if (!HandleFaintedMonActions())
    {
        gBattleStruct->faintedActionsState = 0;
        gCurrentActionFuncId = B_ACTION_FINISHED;
    }
}

static void HandleAction_NothingIsFainted(void)
{
    ++gCurrentTurnActionNumber;
    gCurrentActionFuncId = gActionsByTurnOrder[gCurrentTurnActionNumber];
    gHitMarker &= ~(HITMARKER_DESTINYBOND | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_ATTACKSTRING_PRINTED
                    | HITMARKER_NO_PPDEDUCT | HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_ON_AIR
                    | HITMARKER_IGNORE_UNDERGROUND | HITMARKER_IGNORE_UNDERWATER | HITMARKER_NON_ATTACK_DMG
                    | HITMARKER_OBEYS | HITMARKER_x10 | HITMARKER_SYNCHRONISE_EFFECT
                    | HITMARKER_CHARGING | HITMARKER_x4000000);
}

static void HandleAction_ActionFinished(void)
{
    ++gCurrentTurnActionNumber;
    gCurrentActionFuncId = gActionsByTurnOrder[gCurrentTurnActionNumber];
    SpecialStatusesClear();
    gHitMarker &= ~(HITMARKER_DESTINYBOND | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_ATTACKSTRING_PRINTED
                    | HITMARKER_NO_PPDEDUCT | HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_ON_AIR
                    | HITMARKER_IGNORE_UNDERGROUND | HITMARKER_IGNORE_UNDERWATER | HITMARKER_NON_ATTACK_DMG
                    | HITMARKER_OBEYS | HITMARKER_x10 | HITMARKER_SYNCHRONISE_EFFECT
                    | HITMARKER_CHARGING | HITMARKER_x4000000);
    gCurrentMove = MOVE_NONE;
    gBattleMoveDamage = 0;
    gMoveResultFlags = 0;
    gBattleScripting.animTurn = 0;
    gBattleScripting.animTargetsHit = 0;
    gLastLandedMoves[gBattlerAttacker] = 0;
    gLastHitByType[gBattlerAttacker] = 0;
    gBattleStruct->dynamicMoveType = 0;
    gDynamicBasePower = 0;
    gBattleScripting.atk49_state = 0;
    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
    gBattleCommunication[ACTIONS_CONFIRMED_COUNT] = 0;
    gBattleScripting.multihitMoveEffect = 0;
    gBattleResources->battleScriptsStack->size = 0;
}


enum SpeedWarResults
{
	FirstMon,
	SecondMon,
	SpeedTie,
};

u8 GetWhoStrikesFirst(u8 bank1, u8 bank2, bool8 ignoreMovePriorities)
{
	s8 bank1Priority, bank2Priority;
	s32 bank1Bracket, bank2Bracket;
	u32 bank1Spd, bank2Spd;
    u32 temp;
//Priority Calc
	if(!ignoreMovePriorities)
	{
		bank1Priority = PriorityCalc(bank1, gChosenActionByBattler[bank1], ReplaceWithZMoveRuntime(bank1, gBattleMons[bank1].moves[gBattleStruct->chosenMovePositions[bank1]]));
		bank2Priority = PriorityCalc(bank2, gChosenActionByBattler[bank2], ReplaceWithZMoveRuntime(bank2, gBattleMons[bank2].moves[gBattleStruct->chosenMovePositions[bank2]]));
		if (bank1Priority > bank2Priority)
			return FirstMon;
		else if (bank1Priority < bank2Priority)
			return SecondMon;
	}

//BracketCalc
	bank1Bracket = gBattleStruct->lastBracketCalc[bank1] = BracketCalc(bank1);
	bank2Bracket = gBattleStruct->lastBracketCalc[bank2] = BracketCalc(bank2);

	if (bank1Bracket > bank2Bracket)
		return FirstMon;
	else if (bank1Bracket < bank2Bracket)
		return SecondMon;

//SpeedCalc
	bank1Spd = SpeedCalc(bank1);
	bank2Spd = SpeedCalc(bank2);
	
	if (IsTrickRoomActive())
	{
		temp = bank2Spd;
		bank2Spd = bank1Spd;
		bank1Spd = temp;
	}

	if (bank1Spd > bank2Spd)
		return FirstMon;
	else if (bank1Spd < bank2Spd)
		return SecondMon;

	return SpeedTie;
}

static u8 GetWhoStrikesFirstUseLastBracketCalc(u8 bank1, u8 bank2)
{
	s8 bank1Priority, bank2Priority;
	s8 bank1Bracket, bank2Bracket;
	u32 bank1Spd, bank2Spd;
    u32 temp;
	//Priority Calc
	bank1Priority = PriorityCalc(bank1, gChosenActionByBattler[bank1], ReplaceWithZMoveRuntime(bank1, gBattleMons[bank1].moves[gBattleStruct->chosenMovePositions[bank1]]));
	bank2Priority = PriorityCalc(bank2, gChosenActionByBattler[bank2], ReplaceWithZMoveRuntime(bank2, gBattleMons[bank2].moves[gBattleStruct->chosenMovePositions[bank2]]));
	if (bank1Priority > bank2Priority)
		return FirstMon;
	else if (bank1Priority < bank2Priority)
		return SecondMon;

	//BracketCalc
	bank1Bracket = gBattleStruct->lastBracketCalc[bank1];
	bank2Bracket = gBattleStruct->lastBracketCalc[bank2];
	if (bank1Bracket > bank2Bracket)
		return FirstMon;
	else if (bank1Bracket < bank2Bracket)
		return SecondMon;

	//SpeedCalc
	bank1Spd = SpeedCalc(bank1);
	bank2Spd = SpeedCalc(bank2);
	
	if (IsTrickRoomActive())
	{
		temp = bank2Spd;
		bank2Spd = bank1Spd;
		bank1Spd = temp;
	}

	if (bank1Spd > bank2Spd)
		return FirstMon;
	else if (bank1Spd < bank2Spd)
		return SecondMon;

	return SpeedTie;
}

s32 BracketCalc(u8 bank)
{
	u8 itemEffect = ITEM_EFFECT(bank);
	u8 itemQuality = ITEM_QUALITY(bank);
	ability_t ability = ABILITY(bank);

	gBattleStruct->CustapQuickClawIndicator &= ~(gBitTable[bank]); //Reset the Quick Claw counter just in case
	if (BATTLER_ALIVE(bank))
	{
		switch (itemEffect) {
			case HOLD_EFFECT_QUICK_CLAW:
				if (gRandomTurnNumber % 100 < itemQuality)
				{
					gBattleStruct->CustapQuickClawIndicator |= gBitTable[bank];
					return 1;
				}
				break;

			case HOLD_EFFECT_CUSTAP_BERRY:
				if (!AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, bank, ABILITY_UNNERVE, 0, 0)
				&& PINCH_BERRY_CHECK(bank))
				{
					gBattleStruct->CustapQuickClawIndicator |= gBitTable[bank];
					return 1;
				}
				break;

			case HOLD_EFFECT_LAGGING_TAIL:
				return -2;
		}

		if (ability == ABILITY_STALL)
			return -1;
	}

	return 0;
}