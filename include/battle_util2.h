#ifndef GUARD_BATTLE_UTIL2_H
#define GUARD_BATTLE_UTIL2_H

#include "global.h"

void AllocateBattleResources(void);
void FreeBattleResources(void);
void AdjustFriendshipOnBattleFaint(u8 bank);

ability_t GetBankAbility(u8 bank);
ability_t GetRecordedAbility(u8 bank);
ability_t CopyAbility(u8 bank);
ability_t* GetAbilityLocation(u8 bank);
ability_t* GetAbilityLocationIgnoreNeutralizingGas(u8 bank);
void ClearBattlerAbilityHistory(u8 bank);
u8 GetBankItemEffect(u8 bank);
u8 GetMonItemEffect(struct Pokemon* mon);
u8 GetRecordedItemEffect(u8 bank);
void ClearBattlerItemEffectHistory(u8 bank);
struct Pokemon* GetBankPartyData(u8 bank);
u16 GetBaseCurrentHP(u8 bank);
u16 GetBaseMaxHP(u8 bank);
u8 GetBankFromPartyData(struct Pokemon* mon);
bool8 CanHitSemiInvulnerableTarget(u8 bankAtk, u8 bankDef, u16 move);
bool8 CheckGrounding(u8 bank);
bool8 NonInvasiveCheckGrounding(u8 bank);
bool8 CheckGroundingFromPartyData(struct Pokemon* mon);
u8 ViableMonCountFromBank(u8 bank);
u8 ViableMonCountFromBankLoadPartyRange(u8 bank);
bool8 CheckContact(u16 move, u8 bank);
bool8 CheckContactByMon(u16 move, struct Pokemon* mon);
bool8 CheckHealingMove(u16 move);
bool8 CheckSoundMove(u16 move);
bool8 SheerForceCheck(void);
bool8 IsOfType(u8 bank, u8 type);
bool8 LiftProtect(u8 bank);
bool8 ProtectsAgainstZMoves(u16 move, u8 bankAtk, u8 bankDef);
bool8 StatsMaxed(u8 bank);
bool8 MainStatsMaxed(u8 bank);
bool8 StatsMinned(u8 bank);
bool8 MainStatsMinned(u8 bank);
bool8 AnyStatGreaterThan(u8 bank, u8 amount);
u8 CountBoosts(u8 bank);
bool8 IsUnusableMove(u16 move, u8 bank, u8 check, u8 pp, u8 ability, u8 holdEffect, u16 choicedMove);
u8 CheckMoveLimitationsFromParty(struct Pokemon* mon, u8 unusableMoves, u8 check);
bool8 IsMoveRedirectedByFollowMe(u16 move, u8 bankAtk, u8 defSide);
bool8 IsMoveRedirectionPrevented(u16 move, u8 atkAbility);
bool8 IsBattlerAlive(u8 bank);
struct Pokemon* LoadPartyRange(u8 bank, u8* firstMonId, u8* lastMonId);

bool8 IsUproarBeingMade(void);
u8 GetIllusionPartyNumber(u8 bank);
struct Pokemon* GetIllusionPartyData(u8 bank);
bool8 BankMovedBefore(u8 bank1, u8 bank2);
bool8 BankMovedBeforeIgnoreSwitch(u8 bank1, u8 bank2);
bool8 IsFirstAttacker(u8 bank);
bool8 CanTransferItem(u16 species, u16 item);
bool8 CanFling(u16 item, u16 species, u8 ability, u8 bankOnSide, u8 embargoTimer);
bool8 SymbiosisCanActivate(u8 giverBank, u8 receiverBank);
bool8 CanKnockOffItem(u8 bank);
bool8 CanKnockOffMonItem(struct Pokemon* mon, u8 side);
bool8 IsAffectedByPowder(u8 bank);
bool8 IsAffectedByPowderByDetails(u8 type1, u8 type2, u8 type3, u8 ability, u8 itemEffect);
bool8 MoveIgnoresSubstitutes(u16 move, u8 atkAbility);
bool8 MoveBlockedBySubstitute(u16 move, u8 bankAtk, u8 bankDef);
bool8 MonMoveBlockedBySubstitute(u16 move, struct Pokemon* monAtk, u8 bankDef);
bool8 IsMockBattle(void);
bool8 IsMoveAffectedByParentalBond(u16 move, u8 bankAtk);
u8 CalcMoveSplit(u8 bank, u16 move);
u8 CalcMoveSplitFromParty(struct Pokemon* mon, u16 move);
u8 FindMovePositionInMoveset(u16 move, u8 bank);
bool8 MoveInMoveset(u16 move, u8 bank);
bool8 IsZMove(const u16 move);
void ResetVarsForAbilityChange(u8 bank);
void HandleUnburdenBoost(u8 bank);
void AddBankToPickupStack(const u8 bank);
void RemoveBankFromPickupStack(const u8 bank);
u8 GetTopOfPickupStackNotIncludingBank(const u8 bank);
void RemoveScreensFromSide(const u8 side);

void ClearBankStatus(u8 bank);
bool8 DoesSleepClausePrevent(u8 bank);
bool8 CanBeGeneralStatused(u8 bank, bool8 checkFlowerVeil);
bool8 CanBePutToSleep(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeYawned(u8 bank);
bool8 CanBePoisoned(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil);
bool8 CanBeParalyzed(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeBurned(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeFrozen(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeConfused(u8 bank, u8 checkSafeguard);
bool8 CanBeTormented(u8 bank);
bool8 CanBeInfatuated(u8 bankDef, u8 bankAtk);

bool8 IsTrickRoomActive(void);
bool8 IsMagicRoomActive(void);
bool8 IsWonderRoomActive(void);
bool8 IsGravityActive(void);
bool8 IsIonDelugeActive(void);
bool8 IsFairyLockActive(void);
bool8 IsMudSportActive(void);
bool8 IsWaterSportActive(void);
bool8 IsInverseBattle(void);
bool8 BankSideHasSafeguard(u8 bank);
bool8 BankSideHasMist(u8 bank);
bool8 BankSideHasSeaOfFire(u8 bank);
bool8 BankSideHasRainbow(u8 bank);
bool8 BankSideHasSwamp(u8 bank);
bool8 SideHasSwamp(u8 side);
bool8 BankSideHasGMaxVineLash(u8 bank);
bool8 BankSideHasGMaxWildfire(u8 bank);
bool8 BankSideHasGMaxCannonade(u8 bank);
bool8 BankSideHasGMaxVolcalith(u8 bank);
bool8 IsConfused(u8 bank);
bool8 IsTaunted(u8 bank);
bool8 IsTormented(u8 bank);
bool8 IsHealBlocked(u8 bank);
bool8 CantUseSoundMoves(u8 bank);
bool8 IsLaserFocused(u8 bank);
bool8 IsAbilitySuppressed(u8 bank);
bool8 CantScoreACrit(u8 bank, struct Pokemon* mon);
u16 TryFixDynamaxTransformSpecies(u8 bank, u16 species);

bool8 CheckTableForAbility(u8 ability, const u8 table[]);

bool8 IsTwoOpponentBattle(void);
bool8 BankSideHasTwoTrainers(u8 bank);
bool8 IsTagBattle(void);
bool8 IsPartnerAttacker(void);

bool8 TryDoBenjaminButterfree(u8 scriptOffset);
s32 GetPsywaveDamage(u8 randDamage);
bool8 TrySpitePPReduce(u8 bank, u8 lostPP);
bool8 SetSunnyWeather(void);
bool8 SetRainyWeather(void);
bool8 SetHailWeather(void);
bool8 SetSandstormWeather(void);
bool8 SandstormHurts(u8 bank);
bool8 HailHurts(u8 bank);
bool8 TakesDamageFromSandstorm(u8 bank);
bool8 TakesDamageFromHail(u8 bank);
u32 GetSandstormDamage(u8 bank);
u32 GetHailDamage(u8 bank);
u16 GetNaturePowerMove(void);
void RecycleItem(u8 bank);
u8 GetCamouflageType(void);
void BufferAttackerItem(void);
void MoveValuesCleanUp(void);
u8 GetNextMultiTarget(void);
s32 BracketCalc(u8 bank);
void LoadRaidShieldGfx(void);
void LoadMegaGraphics(u8 state);
void CreateMegaIndicatorAfterAnim(void);
void TryLoadIndicatorForEachBank(void);
void DestroyMegaIndicator(u8 bank);
void TryLoadMegaTriggers(void);
void TryLoadZTrigger(void);
void TryLoadDynamaxTrigger(void);
void DestroyRaidShieldSprite(void);
u16 CalcVisualBasePower(u8 bankAtk, u8 bankDef, u16 move, bool8 ignoreDef);

//jumpifspecies BANK SPECIES ROM_OFFSET
void atkFF22_jumpifspecies(void);

//cureprimarystatus BANK
void atkFF02_cureprimarystatus(void);

//jumpifpartnerattack BANK MOVE ROM_OFFSET
void atkFF03_jumpifpartnerattack(void);

//setterrain ROM_ADDRESS
void atkFF06_setterrain(void);

//jumpifhelditemeffect BANK ITEM_EFFECT ROM_OFFSET
void atkFF07_jumpifhelditemeffect(void);

//counterclear BANK COUNTER_ID FAIL_OFFSET
void atkFF08_counterclear(void);

//jumpifcounter BANK COUNTER_ID PREDICATE BYTE_TO_COMPARE ROM_OFFSET
void atkFF09_jumpifcounter(void);

//setability BANK ABILITY
void atkFF0A_setability(void);

//jumpiftargetpartner ROM_OFFSET
void atkFF0C_jumpiftargetpartner(void);

//setcounter BANK COUNTER_ID AMOUNT
void atkFF0E_setcounter(void);

//jumpifgrounded BANK ROM_OFFSET
void atkFF0F_jumpifgrounded(void);

//jumpifhelditem BANK ITEM ROM_OFFSET
void atkFF10_jumpifhelditem(void);

//reloadhealthbar BANK
//void atkFF11_reloadhealthbar(void);

//jumpifhealthcomparestomax BANK PREDICATE ROM_OFFSET
void atkFF12_jumpifhealthcomparestomax(void);

//atkFF13_setdamagetobankhealthfraction BANK PERCENT
void atkFF13_setdamagetobankhealthfraction(void);

//jumpiftypepresent TYPE ROM_OFFSET
void atkFF14_jumpiftypepresent(void);

//jumpifstatcanbelowered BANK STAT ROM_ADDRESS
void atkFF15_jumpifstatcanbemodified(void);

//jumpifnoviablemonsleft BANK ROM_ADDRESS
void atkFF16_jumpifnoviablemonsleft(void);

//setsidestatus BANK STATUS
void atkFF17_setsidestatus(void);

//clearsidestatus BANK STATUS
void atkFF18_clearsidestatus(void);

//formchange BANK ORIGIN_SPECIES TARGET_SPECIES RELOAD_TYPE RELOAD_STATS ROM_ADDRESS
void atkFF19_formchange(void);

//jumpifabilitypresentattackerfield ABILITY ROM_OFFSET
void atkFF1A_jumpifabilitypresentattackerfield(void);

//tryactivateswitchinability
void atkFF1B_tryactivateswitchinability(void);

//flowershieldlooper MOVE_EFFECT SUCCESS_ADDRESS FAIL_ADDRESS
void atkFF1F_flowershieldlooper(void);

//jumpifprotectedbycraftyshield BANK ROM_ADDRESS
void atkFF20_jumpifprotectedbycraftyshield(void);

//tryspectralthiefsteal SUCCESS_ADDRESS
void atkFF21_tryspectralthiefsteal(void);

// faintpokemonaftermove NULL_BYTE NULL_WORD
void atkFF23_faintpokemonaftermove(void);

//jumpifattackeralreadydiddamage ROM_ADDRESS
void atkFF24_jumpifattackeralreadydiddamage(void);

//jumpifterrainandgrounded TERRAIN_ID BANK ROM_ADDRESS
void atkFF25_jumpifterrainandgrounded(void);

//attackstringnoprotean
void atkFF26_attackstringnoprotean(void);

//tryactivateprotean
void atkFF27_tryactivateprotean(void);

//jumpifweight BANK PREDICATE WEIGHT ROM_OFFSET
void atkFF28_jumpifweight(void);

//trysetsleep BANK FAIL_ADDRESS
void atkFF29_trysetsleep(void);

//trysetparalysis BANK FAIL_ADDRESS
void atkFF2A_trysetparalysis(void);

//trysetburn BANK FAIL_ADDRESS
void atkFF2B_trysetburn(void);

//trysetpoison BANK FAIL_ADDRESS
void atkFF2C_trysetpoison(void);

//addindicatorforattackerswitchineffects
void atkFF2D_addindicatorforattackerswitchineffects(void);

//setmoveeffect2
void atkFF2E_setmoveeffect2(void);

//jumpifdynamaxed BANK ROM_OFFSET
void atkFF30_jumpifdynamaxed(void);

//jumpifraidboss BANK ROM_OFFSET
void atkFF31_jumpifraidboss(void);

//recycleberry BANKish FAIL_OFFSET
void atkFF32_recycleberry(void);

//seteffectprimaryscriptingbank
void atkFF33_SetEffectPrimaryScriptingBank(void);

//canconfuse BANK FAIL_ADDRESS
void atkFF34_canconfuse(void);

//jumpifmaxchistrikecapped BANK FAIL_ADDRESS
void atkFF35_jumpifmaxchistrikecapped(void);

#endif // GUARD_BATTLE_UTIL_H
