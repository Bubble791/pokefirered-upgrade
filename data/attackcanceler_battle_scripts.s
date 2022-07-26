#include "constants/moves.h"
#include "constants/battle.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_script_commands.h"
#include "constants/battle_anim.h"
#include "constants/items.h"
#include "constants/abilities.h"
#include "constants/species.h"
#include "constants/pokemon.h"
#include "constants/songs.h"
#include "constants/trainers.h"
#include "constants/game_stat.h"
#include "battle_string_ids.h"
	.include "asm/macros/battle_script.inc"
@ Define these here since misc_constants.inc conflicts with the C headers
	.set NULL, 0
	.set FALSE, 0
	.set TRUE, 1
	.section script_data, "aw", %progbits
	.align 2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PrintCustomString::
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MagicBounce::
	attackstring
	ppreduce
	pause 0x10
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_MagicBounce
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND	
	call BattleScript_AbilityPopUpRevert
	orword HIT_MARKER 0x800C00
	various BS_ATTACKER 0x1
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveUsedFlinched::
	printstring STRINGID_PKMNFLINCHED
	waitmessage DELAY_1SECOND
	jumpifability BS_ATTACKER ABILITY_STEADFAST SteadfastBoost
	goto BattleScript_MoveEnd

SteadfastBoost::
	jumpifstat BS_ATTACKER EQUALS STAT_SPEED STAT_MAX BattleScript_MoveEnd
	copyarray sBATTLER gBattlerAttacker 0x1
	call BattleScript_AbilityPopUp
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BS_ATTACKER STAT_ANIM_SPD STAT_ANIM_UP
	setbyte sSTATCHANGER STAT_SPEED | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	copyarray 0x2023FDB gBattlerAttacker 0x1 @;gBattlescripting->bank
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveUsedDevolvedForgot::
	setword BATTLE_STRING_LOADER gText_DevolvedForgotMove
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TruantLoafingAround::
	setbyte cMULTISTRING_CHOOSER 0x0
	call BattleScript_AbilityPopUp
	printfromtable gInobedientStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveUsedGravityPrevents::
	orbyte gMoveResultFlags OUTCOME_FAILED
	setword BATTLE_STRING_LOADER GravityAttackCancelString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto 0x81D694E

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveUsedHealBlockPrevents::
	orbyte gMoveResultFlags OUTCOME_FAILED
	setword BATTLE_STRING_LOADER HealBlockAttackCancelString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveUsedThroatChopPrevents::
	orbyte gMoveResultFlags OUTCOME_FAILED
	setword BATTLE_STRING_LOADER ThroatChopAttackCancelString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveUsedFailedPrimalWeather::
	orbyte gMoveResultFlags OUTCOME_FAILED
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	jumpifhalfword ANDS gBattleWeather weather_harsh_sun HarshSunEvaportionBS
	setword BATTLE_STRING_LOADER MoveFizzledInHeavyRainString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

HarshSunEvaportionBS::
	setword BATTLE_STRING_LOADER MoveEvaporatedInHarshSunString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveUsedPsychicTerrainPrevents::
	orbyte gMoveResultFlags OUTCOME_FAILED
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER PsychicTerrainAttackCancelString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveUsedPowderPrevents::
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	playanimation BS_ATTACKER B_ANIM_POWDER_EXPLOSION 0x0
	jumpifability BS_ATTACKER ABILITY_MAGICGUARD SkipPowderDamage
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER

SkipPowderDamage::
	setword BATTLE_STRING_LOADER PowderExplosionString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	tryfaintmon BS_ATTACKER 0x0 0x0
	orbyte gMoveResultFlags 0x1
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StanceChangeToBlade::
	call BattleScript_FlushMessageBox
	call BattleScript_AbilityPopUp
	playanimation BS_ATTACKER B_ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER String_SwitchedToBladeForm
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_StanceChangeToShield::
	call BattleScript_FlushMessageBox
	call BattleScript_AbilityPopUp
	playanimation BS_ATTACKER B_ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER String_SwitchedToShieldForm
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ZMoveActivateStatus::
	call BattleScript_TryRemoveIllusion
	setword BATTLE_STRING_LOADER ZPowerSurroundsString
	printstring STRINGID_CUSTOMSTRING
	playanimation BS_ATTACKER B_ANIM_ZMOVE_ACTIVATE 0x0
	callasm SetZEffect
	setword BATTLE_STRING_LOADER ZMoveUnleashedString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

BattleScript_ZMoveActivateDamaging::
	call BattleScript_TryRemoveIllusion
	setword BATTLE_STRING_LOADER ZPowerSurroundsString
	printstring STRINGID_CUSTOMSTRING
	playanimation BS_ATTACKER B_ANIM_ZMOVE_ACTIVATE 0x0
	setword BATTLE_STRING_LOADER ZMoveUnleashedString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

BattleScript_TryRemoveIllusion::
	jumpifstatus3condition BS_SCRIPTING STATUS3_ILLUSION 0x1 RemoveIllusionReturn
	@;remove illusion counter
	clearspecialstatusbit BS_SCRIPTING STATUS3_ILLUSION
	callasm ClearScriptingBankDisguisedAs
	reloadhealthbar BS_SCRIPTING
	playanimation BS_SCRIPTING B_ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER IllusionWoreOffString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
RemoveIllusionReturn::
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DarkTypePreventsPrankster::
	orbyte gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	printstring 27 @;STRINGID_IT_DOESNT_AFFECT
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveUsedSkyBattlePrevents::
	orbyte gMoveResultFlags OUTCOME_FAILED
	setword BATTLE_STRING_LOADER SkyBattleAttackCancelString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_CantUseSignatureMove::
	attackstring
	pause DELAY_HALFSECOND
	orbyte gMoveResultFlags OUTCOME_FAILED
	setword BATTLE_STRING_LOADER CantUseHyperspaceFuryString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_HoopaCantUseHyperspaceFury::
	attackstring
	pause DELAY_HALFSECOND
	orbyte gMoveResultFlags OUTCOME_FAILED
	setword BATTLE_STRING_LOADER WrongHoopaFormString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveUsedDynamaxPrevents::
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	orbyte gMoveResultFlags OUTCOME_FAILED
	setword BATTLE_STRING_LOADER gText_DynamaxAttackCancel
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveUsedRaidBattlePrevents::
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	orbyte gMoveResultFlags OUTCOME_FAILED
	setword BATTLE_STRING_LOADER gText_RaidBattleAttackCancel
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaidBattleStatNullification::
	call BattleScript_FlushMessageBox
	playanimation BS_SCRIPTING B_ANIM_RAID_BATTLE_ENERGY_BURST 0x0
	setword BATTLE_STRING_LOADER gText_RaidBattleStatNullification
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	updatestatusicon BS_SCRIPTING
	return
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TargetSleepHeal::
	setword BATTLE_STRING_LOADER SlappedAwakeString
	printstring 0x184
	waitmessage DELAY_1SECOND
	updatestatusicon BS_TARGET
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TargetBurnHeal::
	setword BATTLE_STRING_LOADER BurnCuredString
	printstring 0x184
	waitmessage DELAY_1SECOND
	updatestatusicon BS_TARGET
	return
