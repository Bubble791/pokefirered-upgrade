#ifndef GUARD_BATTLE_AI_SCRIPT_COMMANDS_H
#define GUARD_BATTLE_AI_SCRIPT_COMMANDS_H

#include "global.h"

// return values for BattleAI_ChooseMoveOrAction
// 0 - 3 are move idx
#define AI_CHOICE_FLEE 4
#define AI_CHOICE_WATCH 5

#define AI_ACTION_DONE          0x0001
#define AI_ACTION_FLEE          0x0002
#define AI_ACTION_WATCH         0x0004
#define AI_ACTION_DO_NOT_ATTACK 0x0008
#define AI_ACTION_UNK5          0x0010
#define AI_ACTION_UNK6          0x0020
#define AI_ACTION_UNK7          0x0040
#define AI_ACTION_UNK8          0x0080

struct AIScript
{
	u16 atkSpecies;
	u16 defSpecies;
	u8 atkAbility;
	u8 defAbility;

	u8 atkItemEffect;
	u8 defItemEffect;
	u16 defItem;
	u16 atkItem;

	u8 atkItemQuality;
	u8 defItemQuality;
	u32 atkStatus1;
	u32 defStatus1;
	u32 atkStatus2;
	u32 defStatus2;
	u32 atkStatus3;
	u32 defStatus3;
	u8 atkGender;
	u8 defGender;

	u16 atkAttack;
	u16 atkDefense;
	u16 atkSpeed;
	u16 atkSpAtk;
	u16 atkSpDef;

	u16 defAttack;
	u16 defDefense;
	u16 defSpeed;
	u16 defSpAtk;
	u16 defSpDef;

	u8 bankAtkPartner;
	u8 bankDefPartner;
	u8 atkPartnerAbility;
	u8 defPartnerAbility;

	u8 foe1;
	u8 foe2;
	u16 partnerMove;
	bool8 partnerHandling;
};

void BattleAI_HandleItemUseBeforeAISetup(void);
void BattleAI_SetupAIData(u8);
void ClearBankMoveHistory(u8 bank);
void RecordAbilityBattle(u8 bank, u8 abilityId);
void ClearBankAbilityHistory(u8 bank);
void RecordItemEffectBattle(u8 bank, u8 itemEffect);
void ClearBankItemEffectHistory(u8 bank);
u8 BattleAI_ChooseMoveOrAction(void);

void ResetBestMonToSwitchInto(u8 bank);
u32 GetAIFlags(void);
bool8 IsPredictedToSwitch(u8 bankAtk, u8 bankDef);

bool8 CanKillAFoe(u8 bank);
bool8 CanKnockOut(u8 bankAtk, u8 bankDef);
bool8 GetCanKnockOut(u8 bankAtk, u8 bankDef);
bool8 Can2HKO(u8 bankAtk, u8 bankDef);
bool8 GetCan2HKO(u8 bankAtk, u8 bankDef);
bool8 CanKnockOutAfterHealing(u8 bankAtk, u8 bankDef, u16 healAmount, u8 numHits);
bool8 CanKnockOutWithoutMove(const u16 ignoredMove, const u8 bankAtk, const u8 bankDef, const bool8 ignoreFutureAttacks);
bool8 MoveKnocksOutPossiblyGoesFirstWithBestAccuracy(u16 move, u8 bankAtk, u8 bankDef, bool8 checkGoingFirst);
bool8 IsWeakestContactMoveWithBestAccuracy(u16 move, u8 bankAtk, u8 bankDef);
bool8 StrongestMoveGoesFirst(u16 move, u8 bankAtk, u8 bankDef);
bool8 CanKnockOutFromParty(struct Pokemon* monAtk, u8 bankDef, struct DamageCalc* damageData);
void UpdateBestDoubleKillingMoveScore(u8 bankAtk, u8 bankDef, u8 bankAtkPartner, u8 bankDefPartner, s8 bestMoveScores[MAX_BATTLERS_COUNT], u16* bestMove);
u8 GetDoubleKillingScore(u16 move, u8 bankAtk, u8 bankDef);
void TryRemoveDoublesKillingScore(u8 bankAtk, u8 bankDef, u16 chosenMove);
bool8 RangeMoveCanHurtPartner(u16 move, u8 bankAtk, u8 bankAtkPartner);
bool8 MoveKnocksOutXHits(u16 move, u8 bankAtk, u8 bankDef, u8 numHits);
bool8 MoveKnocksOutXHitsFromParty(u16 move, struct Pokemon* monAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData);
u16 CalcFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData);
u32 GetFinalAIMoveDamage(u16 move, u8 bankAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData);
u16 CalcFinalAIMoveDamageFromParty(u16 move, struct Pokemon* monAtk, u8 bankDef, u8 numHits, struct DamageCalc* damageData);
u16 CalcStrongestMove(const u8 bankAtk, const u8 bankDef, const bool8 onlySpreadMoves);
bool8 IsStrongestMove(const u16 currentMove, const u8 bankAtk, const u8 bankDef);
u16 GetStrongestMove(const u8 bankAtk, const u8 bankDef);
bool8 MoveWillHit(u16 move, u8 bankAtk, u8 bankDef);
bool8 MoveWouldHitFirst(u16 move, u16 bankAtk, u16 bankDef);
bool8 MoveWouldHitBeforeOtherMove(u16 moveAtk, u8 bankAtk, u16 moveDef, u8 bankDef);
bool8 IsUsefulToFlinchTarget(u8 bankDef);
bool8 IsBankAsleep(u8 bank);
bool8 IsBankIncapacitated(u8 bank);
bool8 WillFaintFromWeatherSoon(u8 bank);
bool8 WillTakeSignificantDamageFromEntryHazards(u8 bank, u8 healthFraction);
u8 CountBanksPositiveStatStages(u8 bank);
u8 CountBanksNegativeStatStages(u8 bank);
u16 GetTeamSpeedAverage(u8 bank);
u16 GetPokemonOnSideSpeedAverage(u8 bank);
u16 GetBattleMonMove(u8 bank, u8 index);
u8 GetAIAbility(u8 bankAtk, u8 bankDef, u16 move);
u8 GetPredictedAIAbility(u8 bankAtk, u8 bankDef);
u16 GetAIChosenMove(u8 bankAtk, u8 bankDef);
bool8 IsTrapped(u8 bank, bool8 switching);
bool8 BankHasMonToSwitchTo(u8 bank);
bool8 IsTakingSecondaryDamage(u8 bank);
bool8 WillFaintFromSecondaryDamage(u8 bank);
u16 CalcSecondaryEffectChance(u8 bank, u16 move);
u16 CalcAIAccuracy(u16 move, u8 bankAtk, u8 bankDef);
bool8 ShouldAIDelayMegaEvolution(u8 bankAtk, u8 bankDef, u16 move);

u16 IsValidMovePrediction(u8 bankAtk, u8 bankDef);
bool8 IsPredictedToSwitch(u8 bankAtk, u8 bankDef);
void StoreMovePrediction(u8 bankAtk, u8 bankDef, u16 move);
void StoreSwitchPrediction(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionSemiInvulnerable(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionHealingMove(u8 bankAtk, u8 bankDef);
bool8 IsPredictedToUsePursuitableMove(u8 bankAtk, u8 bankDef);
bool8 IsMovePredictionPhazingMove(u8 bankAtk, u8 bankDef);
bool8 CanMovePredictionProtectAgainstMove(u8 bankAtk, u8 bankDef, u16 move);
void LoadBattlersAndFoes(u8* battlerIn1, u8* battlerIn2, u8* foe1, u8* foe2);
u32 WildMonIsSmart(u8 bank);
bool8 DamagingMoveInMoveset(u8 bank);
bool8 PhysicalMoveInMoveset(u8 bank);
bool8 SpecialMoveInMoveset(u8 bank);
bool8 MoveSplitInMoveset(u8 bank, u8 moveSplit);
bool8 PhysicalMoveInMonMoveset(struct Pokemon* mon, u8 moveLimitations);
bool8 SpecialMoveInMonMoveset(struct Pokemon* mon, u8 moveLimitations);
bool8 MagicCoatableMovesInMoveset(u8 bank);
bool8 HasProtectionMoveInMoveset(u8 bank, u8 checkType);
bool8 MoveTypeInMoveset(u8 bank, u8 moveType);
bool8 DamagingMoveTypeInMoveset(u8 bank, u8 moveType);
bool8 DamagingAllHitMoveTypeInMoveset(u8 bank, u8 moveType);
bool8 DamagingSpreadMoveInMoveset(u8 bank);
bool8 HasSnatchableMove(u8 bank);
bool8 MoveEffectInMoveset(u8 moveEffect, u8 bank);
bool8 StatusMoveInMoveset(u8 bank);
bool8 MoveInMovesetWithAccuracyLessThan(u8 bankAtk, u8 bankDef, u8 acc, bool8 ignoreStatusMoves);
bool8 FlinchingMoveInMoveset(u8 bank);
bool8 HealingMoveInMoveset(u8 bank);
bool8 HealingMoveInMonMoveset(struct Pokemon* mon);
bool8 SoundMoveInMoveset(u8 bank);
bool8 MoveThatCanHelpAttacksHitInMoveset(u8 bank);
bool8 DamagingMoveThaCanBreakThroughSubstituteInMoveset(u8 bankAtk, u8 bankDef);
bool8 ContactMovesThatAffectTargetInMoveset(u8 bankAtk, u8 bankDef);
bool8 UnfreezingMoveInMoveset(u8 bank);
bool8 SleepMoveInMovesetWithLowAccuracy(u8 bankAtk, u8 bankDef);
bool8 OnlyBadMovesLeftInMoveset(u8 bankAtk, u8 bankDef);
u16 TryReplaceMoveWithZMove(u8 bankAtk, u8 bankDef, u16 move);
u8 GetAIMoveEffectForMaxMove(u16 move, u8 bankAtk, u8 bankDef);

bool8 GetHealthPercentage(u8 bank);
bool8 TeamFullyHealedMinusBank(u8 bank);
bool8 AnyStatIsRaised(u8 bank);
bool8 AnyUsefulStatIsRaised(u8 bank);
bool8 AnyUsefulOffseniveStatIsRaised(u8 bank);
bool8 PartyMemberStatused(u8 bank, bool8 checkSoundproof);
bool8 ShouldAIUseZMove(u8 bank, u8 moveIndex, u16 move);
void CalcAIDynamaxMon(u8 bank);
void CalcShouldAIDynamax(u8 bankAtk, u8 bankDef);
bool8 ShouldAIDynamax(u8 bankAtk, u8 bankDef);
u8 AdjustMoveLimitationFlagsForAI(u8 bankAtk, u8 bankDef);
bool8 BadIdeaToPutToSleep(u8 bankDef, u8 bankAtk);
bool8 BadIdeaToPoison(u8 bankDef, u8 bankAtk);
bool8 GoodIdeaToPoisonSelf(u8 bankAtk);
bool8 BadIdeaToParalyze(u8 bankDef, u8 bankAtk);
bool8 GoodIdeaToParalyzeSelf(u8 bankAtk);
bool8 BadIdeaToBurn(u8 bankDef, u8 bankAtk);
bool8 GoodIdeaToBurnSelf(u8 bankAtk);
bool8 BadIdeaToFreeze(u8 bankDef, u8 bankAtk);

bool8 GoodIdeaToLowerAttack(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerDefense(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerSpAtk(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerSpDef(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerSpeed(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerAccuracy(u8 bankDef, u8 bankAtk, u16 move);
bool8 GoodIdeaToLowerEvasion(u8 bankDef, u8 bankAtk, u16 move);
#endif // GUARD_BATTLE_AI_SCRIPT_COMMANDS_H
