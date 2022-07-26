#ifndef GUARD_BATTLE_UTIL_H
#define GUARD_BATTLE_UTIL_H

#include "global.h"

struct DamageCalc
{
	u8 bankAtk;
	u8 bankDef;
	struct Pokemon* monAtk;
	struct Pokemon* monDef;

	u16 atkSpecies;
	u16 defSpecies;

	u8 atkAbility;
	u8 defAbility;
	u8 atkPartnerAbility;
	u8 defPartnerAbility;

	u8 atkItemEffect;
	u8 atkItemQuality;
	u8 defItemEffect;
	u8 defItemQuality;

	u16 atkItem;

	u16 atkHP;
	u16 defHP;
	u16 atkMaxHP;
	u16 defMaxHP;
//	u32 attack;
//	u32 spAttack;
	u32 defense;
	u32 spDefense;
	u32 atkSpeed;
	u32 defSpeed;

	u8 atkBuff;
	u8 spAtkBuff;
	u8 defBuff;
	u8 spDefBuff;

	u32 atkStatus1;
	u32 defStatus1;
	u32 atkStatus3;
	u32 defStatus3;
	u16 defSideStatus;
	bool8 atkIsGrounded;
	bool8 defIsGrounded;

	u16 move;
	u8 moveType;
	u8 moveSplit;
	u8 resultFlags;
	u8 basePower;

	u8 specialFlags;
	bool8 attackerLoaded;
	bool8 defenderLoaded;
};

struct FlingStruct
{
	u8 power;
	u8 effect;
};

#define MOVE_LIMITATION_ZEROMOVE                (1 << 0)
#define MOVE_LIMITATION_PP                      (1 << 1)
#define MOVE_LIMITATION_DISABLED                (1 << 2)
#define MOVE_LIMITATION_TORMENTED               (1 << 3)
#define MOVE_LIMITATION_TAUNT                   (1 << 4)
#define MOVE_LIMITATION_IMPRISON                (1 << 5)
#define MOVE_LIMITATION_CHOICE		(1 << 6)
#define MOVE_LIMITATION_ENCORE		(1 << 7)

#define ABILITYEFFECT_ON_SWITCHIN                0x0
#define ABILITYEFFECT_ENDTURN                    0x1
#define ABILITYEFFECT_MOVES_BLOCK                0x2
#define ABILITYEFFECT_ABSORBING                  0x3
#define ABILITYEFFECT_MOVE_END                   0x4
#define ABILITYEFFECT_IMMUNITY                   0x5
#define ABILITYEFFECT_FORECAST                   0x6
#define ABILITYEFFECT_SYNCHRONIZE                0x7
#define ABILITYEFFECT_ATK_SYNCHRONIZE            0x8
#define ABILITYEFFECT_INTIMIDATE1                0x9
#define ABILITYEFFECT_INTIMIDATE2                0xA
#define ABILITYEFFECT_TRACE                      0xB
#define ABILITYEFFECT_CHECK_OTHER_SIDE           0xC
#define ABILITYEFFECT_CHECK_BATTLER_SIDE         0xD
#define ABILITYEFFECT_FIELD_SPORT                0xE
#define ABILITYEFFECT_CHECK_FIELD_EXCEPT_BATTLER 0xF // TODO: Is it correct? 
#define ABILITYEFFECT_COUNT_OTHER_SIDE           0x10
#define ABILITYEFFECT_COUNT_BATTLER_SIDE         0x11
#define ABILITYEFFECT_COUNT_ON_FIELD             0x12
#define ABILITYEFFECT_CHECK_ON_FIELD             0x13
#define ABILITYEFFECT_MOVES_BLOCK_PARTNER	   	 0x14
#define ABILITYEFFECT_SWITCH_IN_WEATHER          0xFF

#define ABILITY_ON_OPPOSING_FIELD(battlerId, abilityId)(AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, battlerId, abilityId, 0, 0))
#define ABILITY_ON_FIELD(abilityId)(AbilityBattleEffects(ABILITYEFFECT_CHECK_ON_FIELD, 0, abilityId, 0, 0))
#define ABILITY_ON_FIELD2(abilityId)(AbilityBattleEffects(ABILITYEFFECT_FIELD_SPORT, 0, abilityId, 0, 0))

#define ITEMEFFECT_ON_SWITCH_IN                 0x0
#define ITEMEFFECT_MOVE_END                     0x3
#define ITEMEFFECT_KINGSROCK_SHELLBELL          0x4

#define WEATHER_HAS_EFFECT ((!AbilityBattleEffects(ABILITYEFFECT_CHECK_ON_FIELD, 0, ABILITY_CLOUD_NINE, 0, 0) && !AbilityBattleEffects(ABILITYEFFECT_CHECK_ON_FIELD, 0, ABILITY_AIR_LOCK, 0, 0)))
#define WEATHER_HAS_EFFECT2 ((!AbilityBattleEffects(ABILITYEFFECT_FIELD_SPORT, 0, ABILITY_CLOUD_NINE, 0, 0) && !AbilityBattleEffects(ABILITYEFFECT_FIELD_SPORT, 0, ABILITY_AIR_LOCK, 0, 0)))

#define BS_GET_TARGET                   0
#define BS_GET_ATTACKER                 1
#define BS_GET_EFFECT_BANK              2
#define BS_GET_FAINTED_BANK             3
#define BS_GET_BANK_0                   7
#define BS_ATTACKER_WITH_PARTNER        4 // for atk98_status_icon_update
#define BS_GET_ATTACKER_SIDE            8 // for atk1E_jumpifability
#define BS_GET_NOT_ATTACKER_SIDE        9 // for atk1E_jumpifability
#define BS_GET_SCRIPTING_BANK           10
#define BS_GET_PLAYER1                  11
#define BS_GET_OPPONENT1                12
#define BS_GET_PLAYER2                  13
#define BS_GET_OPPONENT2                14
#define BS_GET_SWITCHING_BANK           15

extern const u16 gPowderMoves[];
extern const u16 gMoldBreakerMoves[];
extern const u16 gSubstituteBypassMoves[];
extern const u16 gMovesCanUnfreezeTarget[];
extern const u16 gMovesCanUnfreezeAttacker[];
extern const u16 gRaidBattleBannedMoves[];
extern const u16 gRaidBattleBannedRaidMonMoves[];
extern const u16 gGravityBannedMoves[];
extern const u16 gMovesWithQuietBGM[];
extern const u16 gFlinchChanceMoves[];
extern const u16 gRecklessBoostedMoves[];
extern const u16 gPunchingMoves[];
extern const u16 gSheerForceBoostedMoves[];
extern const u16 gAlwaysHitInRainMoves[];
extern const u16 gAlwaysCriticalMoves[];
extern const u16 gPulseAuraMoves[];
extern const u16 gBallBombMoves[];
extern const u16 gBitingMoves[];
extern const u16 gIgnoreStatChangesMoves[];
extern const u16 gMovesThatRequireRecharging[];
extern const u16 gAlwaysHitWhenMinimizedMoves[];
extern const u16 gTwoToFiveStrikesMoves[];
extern const u16 gTwoStrikesMoves[];
extern const u16 gHighCriticalChanceMoves[];
extern const u16 gSpecialAttackPhysicalDamageMoves[];
extern const u8 gText_SetAuroraVeil[];
extern const bool8 gMoldBreakerIgnoredAbilities[];
extern const u16 gSkyBattleBannedMoves[];
extern const u16 gIgnoreUndergoundMoves[];
extern const u16 gIgnoreInAirMoves[];
extern const u16 gIgnoreUnderwaterMoves[];
extern const u16 gMiniorCores[];
extern const u16 gPercent25RecoilMoves[];
extern const u16 gPercent33RecoilMoves[];
extern const u16 gPercent50RecoilMoves[];
extern const u16 gSoundMoves[];
extern const u16 gTypeChangeExceptionMoves[];
extern const u16 gDynamaxBannedMoves[];
extern const u16 gParentalBondBannedMoves[];
extern const u16 gMovesThatChangePhysicality[];
extern const u16 gSpecialWholeFieldMoves[];
extern const u16 gMovesThatLiftProtectTable[];
extern const u8 gTypeEffectiveness_2[NUMBER_OF_MON_TYPES][NUMBER_OF_MON_TYPES];

extern const u8 PrimalRainEndString[];
extern const u8 PrimalSunEndString[];
extern const u8 PrimalAirCurrentEndString[];
extern const u8 gText_RaidBattleKO4[];

void HandleEndTurn_RanFromBattle(void);
u8 GetBattlerForBattleScript(u8 caseId);
void PressurePPLose(u8 target, u8 attacker, u16 move);
void PressurePPLoseOnUsingImprison(u8 attacker);
void PressurePPLoseOnUsingPerishSong(u8 attacker);
void MarkAllBattlersForControllerExec(void);
void MarkBattlerForControllerExec(u8 battlerId);
void MarkBattlerReceivedLinkData(u8 battlerId);
void CancelMultiTurnMoves(u8 battler);
bool8 WasUnableToUseMove(u8 battler);
void PrepareStringBattle(u16 stringId, u8 battler);
void ResetSentPokesToOpponentValue(void);
void OpponentSwitchInResetSentPokesToOpponentValue(u8 battler);
void UpdateSentPokesToOpponentValue(u8 battler);
void BattleScriptPush(const u8 *bsPtr);
void BattleScriptPushCursor(void);
void BattleScriptPop(void);
u8 TrySetCantSelectMoveBattleScript(void);
u8 CheckMoveLimitations(u8 battlerId, u8 unusableMoves, u8 check);
bool8 AreAllMovesUnusable(void);
u8 GetImprisonedMovesCount(u8 battlerId, u16 move);
u8 DoBattlerEndTurnEffects(void);
bool8 HandleWishPerishSongOnTurnEnd(void);
bool8 HandleFaintedMonActions(void);
void TryClearRageStatuses(void);
u8 AtkCanceller_UnableToUseMove(void);
bool8 HasNoMonsToSwitch(u8 battler);
u8 CastformDataTypeChange(u8);
u8 AbilityBattleEffects(u8 caseID, u8 battler, u8 ability, u8 special, u16 moveArg);
void BattleScriptExecute(const u8 *BS_ptr);
void BattleScriptPushCursorAndCallback(const u8 *BS_ptr);
u8 ItemBattleEffects(u8 caseID, u8 bank, bool8 moveTurn, bool8 doPluck);
void ClearFuryCutterDestinyBondGrudge(u8 battlerId);
void HandleAction_RunBattleScript(void);
u8 GetMoveTarget(u16 move, u8 setTarget);
const u8 *ReturnEmptyStringIfNull(const u8 *string);
bool8 IsTwoTurnsMove(u16 move);
void TransferAbilityPopUpHelper(void);
void TransferAbilityPopUp(u8 bank, u8 ability);
u8 ActivateWeatherAbility(u16 flags, u16 item, u8 bank, u8 animArg, u8 stringIndex, bool8 moveTurn);
u8 CalcMovePowerForForewarn(u16 move);
u8 NewTypeCalc(u16 move, u8 bankAtk, u8 bankDef, struct Pokemon* monAtk, bool8 CheckParty);
u8 GetMoveTypeSpecial(u8 bankAtk, u16);
u8 GetMoveTypeSpecialPreAbility(u16 move, u8 bankAtk, struct Pokemon* monAtk);
u8 GetMoveTypeSpecialPostAbility(u16 move, u8 atkAbility, bool8 zMoveActive);
u8 GetMonMoveTypeSpecial(struct Pokemon* monAtk, u16 move);
u8 GetExceptionMoveType(u8 bankAtk, u16);
u8 GetMonExceptionMoveType(struct Pokemon*, u16);
u8 CalcMonHiddenPowerType(struct Pokemon* mon);
s8 PriorityCalc(u8 bank, u8 action, u16 move);
void TypeDamageModification(u8 atkAbility, u8 bankDef, u16 move, u8 moveType, u8* flags);
bool8 ImmunityAbilityCheck(u8 bank, u32 status, const u8* string);
void TransformPokemon(u8 bankAtk, u8 bankDef);
void EmitDataTransfer(u8 bufferId, void* dst, u16 size, void* data);
u8 TryActivateTerrainAbility(u8 terrain, u8 anim, u8 bank);
const u8* TryActivateMimicryForBank(u8 bank);
bool8 AllMainStatsButOneAreMinned(u8 bank);
s32 ConfusionDamageCalc(void);
void RestoreAllOriginalMoveData(void);
void RestoreOriginalAttackerAndTarget(void);
void SetBatonPassSwitchingBit(void);
void ClearBatonPassSwitchingBit(void);

void DoFormChange(u8 bank, u16 species, bool8 reloadType, bool8 reloadStats, bool8 reloadAbility);
void SwitchOutFormsRevert(u8 bank);
void FormsRevert(struct Pokemon* party);
bool8 TryFormRevert(struct Pokemon* mon);
void UpdateBurmy(void);
u16 GetMiniorCoreFromPersonality(u32 personality);
u16 GetMiniorCoreSpecies(struct Pokemon* mon);
void HoldItemFormChange(struct Pokemon* mon, u16 item);
void HoopaShayminPCRevertCheck(struct Pokemon* mon, bool8 recalcStats);

bool8 JumpIfMoveAffectedByProtect(u16 move, u8 bankAtk, u8 bankDef, bool8 actuallyJump);
bool8 ProtectAffects(u16 move, u8 bankAtk, u8 bankDef, bool8 set);
bool8 DoesProtectionMoveBlockMove(u8 bankAtk, u8 bankDef, u16 atkMove, u16 protectMove);
bool8 MissesDueToSemiInvulnerability(u8 bankAtk, u8 bankDef, u16 move);
u32 AccuracyCalc(u16 move, u8 bankAtk, u8 bankDef);
u32 VisualAccuracyCalc(u16 move, u8 bankAtk, u8 bankDef);
u32 VisualAccuracyCalc_NoTarget(u16 move, u8 bankAtk);
void JumpIfMoveFailed(u8 adder, u16 move);
bool8 AccuracyCalcHelper(u16 move, u8 bankDef);

bool8 CanTargetPartner(u8 bankDef);
bool8 TargetFullyImmuneToCurrMove(u8 bankDef);

void atk00_attackcanceler(void);
u8 GetExceptionMoveType(u8 bankAtk, u16 move);

u8 ChangeStatBuffs(s8 statValue, u8 statId, u8 flags, const u8* BS_ptr);
bool8 DefiantActivation(void);

void ClearSwitchBytes(u8 bank);
void ClearSwitchBits(u8 bank);
void TryReactivateCentifernoSandblast(u32* status2);
bool8 IsBankHoldingFocusSash(u8 bank);
bool8 HandleSpecialSwitchOutAbilities(u8 bank, u8 ability);
s32 CalculateBaseDamage(struct DamageCalc* data);
void AI_CalcDmg(u8 attacker, u8 defender);
u32 NewAI_CalcDmg(const u8 bankAtk, const u8 bankDef, const u16 move, struct DamageCalc* damageData);
u32 AI_CalcPartyDmg(u8 bankAtk, u8 bankDef, u16 move, struct Pokemon* monAtk, struct DamageCalc* damageData);
u8 AI_TypeCalc(u16 move, u16 targetSpecies, u8 targetAbility);
u8 AI_SpecialTypeCalc(u16 move, u8 bankAtk, u8 bankDef);
u8 VisualTypeCalc(u16 move, u8 bankAtk, u8 bankDef);
void TypeDamageModification(u8 atkAbility, u8 bankDef, u16, u8 moveType, u8* flags);
void TypeDamageModificationPartyMon(u8 atkAbility, struct Pokemon* monDef, u16 move, u8 moveType, u8* flags);
u8 GetMoveTypeSpecial(u8 bankAtk, u16);
u8 GetMoveTypeSpecialPreAbility(u16 move, u8 bankAtk, struct Pokemon* monAtk);
u8 GetMoveTypeSpecialPostAbility(u16 move, u8 atkAbility, bool8 zMoveActive);
u8 GetMonMoveTypeSpecial(struct Pokemon* monAtk, u16 move);
u8 GetExceptionMoveType(u8 bankAtk, u16);
u8 GetMonExceptionMoveType(struct Pokemon*, u16);
void PopulateDamageCalcStructWithBaseAttackerData(struct DamageCalc* data);
void PopulateDamageCalcStructWithBaseDefenderData(struct DamageCalc* data);
u32 GetActualSpeciesWeight(u16 species, u8 ability, u8 itemEffect, u8 bank, bool8 checkNimble);
void AdjustDamage(bool8 checkFalseSwipe);
u32 SpeedCalcMon(u8 side, struct Pokemon* mon);
u32 SpeedCalc(u8 bank);
bool8 SetMoveEffect2(void);
bool8 ShouldDoTrainerSlide(u8 bank, u16 trainerId, u8 caseId);
u16 TryGetAlternateSpeciesSize(u16 species, u8 type);
u8 GetFlingPower(u16 item, u16 species, u8 ability, u8 bank, bool8 partyCheck);
void atkFF1C_handletrainerslidemsg(void);
void atkFF1D_trytrainerslidefirstdownmsg(void);
void atkFF1E_trainerslideout(void);
u8 GetOpenWorldBadgeCount(void);
void DestroyMegaIndicator(u8 bank);
void TryReactivateCentifernoSandblast(u32* backupStatus2);
u32 CalcSpikesDamage(u8 bank);
u32 CalcSpikesDamagePartyMon(struct Pokemon* mon, u8 side);
u32 CalcStealthRockDamage(u8 bank);
u32 CalcStealthRockDamagePartyMon(struct Pokemon* mon);
u32 CalcSteelsurgeDamage(u8 bank);
u32 CalcSteelsurgeDamagePartyMon(struct Pokemon* mon);
bool8 WillFaintFromEntryHazards(struct Pokemon* mon, u8 side);
bool8 IsLinkDoubleBattle(void);
bool8 AreAllKindsOfRunningPrevented(void);
u8 CanActivateTotemBoost(u8 bank);
void atk8F_forcerandomswitch(void);
void atk52_switchineffects(void);
void SwitchPartyOrderInGameMulti(u8 bank, u8 monToSwitchIntoId);
bool8 IsTerrainMoveIndoors(void);
void HandleFormChange(void);
u8 GetCatchingBattler(void);
bool8 TryActivateOWTerrain(void);
void BackupSwitchingBank(void);
void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst);
const u8* GetAbilityName(const u8 ability);
void BufferMoveNameBattle(u16 move, u8* dst);
#endif // GUARD_BATTLE_UTIL_H
