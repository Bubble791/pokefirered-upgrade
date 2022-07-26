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
#include "constants/game_stat.h"
#include "constants/hold_effects.h"
#include "battle_string_ids.h"
	.include "asm/macros/battle_script.inc"
@ Define these here since misc_constants.inc conflicts with the C headers
	.set NULL, 0
	.set FALSE, 0
	.set TRUE, 1
	.section script_data, "aw", %progbits
	.align 2
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StickyHoldActivatesRet::
	pause 0x10
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER ItemCantBeRemovedString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityWasSuppressed::
	setword BATTLE_STRING_LOADER AbilitySuppressedString
	printstring 0x184
	waitmessage DELAY_1SECOND
	call BattleScript_TryRemoveIllusion
	callasm TryRemovePrimalWeatherAfterAbilityChange
	call BattleScript_HandleWeatherFormChanges @;Try to revert Cherrim and Castform
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveRaiseStatTeam::
	jumpifhasnohp BS_ATTACKER BattleScript_MaxMoveRaiseStatTeam_CheckPartner
	callasm SetMaxMoveStatRaiseEffect
	seteffectprimary

BattleScript_MaxMoveRaiseStatTeam_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetAttackerPartner
	jumpifhasnohp BS_ATTACKER BattleScript_MaxMoveSetAttackerBackAndReturn
	callasm SetMaxMoveStatRaiseEffect
	seteffectprimary
BattleScript_MaxMoveSetAttackerBackAndReturn::
	callasm SetAttackerPartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveLowerStatFoes::
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveLowerStatFoes_CheckPartner
	callasm SetMaxMoveStatLowerEffect
	seteffectprimary

BattleScript_MaxMoveLowerStatFoes_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	callasm SetMaxMoveStatLowerEffect
	seteffectprimary
BattleScript_MaxMoveSetTargetBackAndReturn::
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EatEffectBankBerry::
	copybyte sBATTLER gEffectBattler
	call BattleScript_CheekPouch
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveSetWeather::
	playanimation2 BS_SCRIPTING sB_ANIM_ARG1 0x0
	printfromtable gMoveWeatherChangeStringIds 
	waitmessage DELAY_1SECOND 
	call BattleScript_HandleWeatherFormChanges
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveSetTerrain::
	call BattleScript_SetTerrain
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveEffectSporeFoes::
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveEffectSporeFoes_CheckPartner
	callasm PickRandomGMaxBefuddleEffect
	seteffectsecondary

BattleScript_MaxMoveEffectSporeFoes_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	callasm PickRandomGMaxBefuddleEffect
	seteffectsecondary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveParalyzeFoes::
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveParalyzeFoes_CheckPartner
	callasm SetGMaxVoltCrashEffect
	seteffectsecondary

BattleScript_MaxMoveParalyzeFoes_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	callasm SetGMaxVoltCrashEffect
	seteffectsecondary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMovePayDayConfuseFoes::
	setmoveeffect MOVE_EFFECT_PAYDAY
	seteffectprimary
	goto BattleScript_MaxMoveConfuseFoes

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveCritUp::
	jumpifhasnohp BS_ATTACKER BS_MaxMoveCritUp_CheckPartner
	jumpifmaxchistrikecapped BS_ATTACKER BS_MaxMoveCritUp_CheckPartner
	callasm SetGMaxChiStrikeEffect
	printfromtable gFocusEnergyUsedStringIds
	waitmessage DELAY_1SECOND

BS_MaxMoveCritUp_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetAttackerPartner
	jumpifhasnohp BS_ATTACKER BS_MaxMoveCritUp_RestoreOriginalAttacker
	jumpifmaxchistrikecapped BS_ATTACKER BS_MaxMoveCritUp_RestoreOriginalAttacker
	callasm SetGMaxChiStrikeEffect
	printfromtable gFocusEnergyUsedStringIds
	waitmessage DELAY_1SECOND

BS_MaxMoveCritUp_RestoreOriginalAttacker::
	callasm SetAttackerPartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveTrap::
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveTrap_CheckPartner
	jumpifstatus2 BS_TARGET STATUS2_ESCAPE_PREVENTION BattleScript_MaxMoveTrap_CheckPartner
	jumpiftype BS_TARGET TYPE_GHOST BattleScript_MaxMoveTrap_CheckPartner
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	seteffectprimary
	printstring 0x8F @;STRINGID_TARGETCANTESCAPENOW
	waitmessage DELAY_1SECOND

BattleScript_MaxMoveTrap_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	jumpifstatus2 BS_TARGET STATUS2_ESCAPE_PREVENTION BattleScript_MaxMoveSetTargetBackAndReturn
	jumpiftype BS_TARGET TYPE_GHOST BattleScript_MaxMoveSetTargetBackAndReturn
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	seteffectprimary
	printstring 0x8F @;STRINGID_TARGETCANTESCAPENOW
	waitmessage DELAY_1SECOND
	callasm SetTargetFoePartner
	return
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveInfatuation::
	jumpifhasnohp BS_TARGET BS_MaxMoveInfatuation_CheckPartner
	tryinfatuating BS_TARGET BS_MaxMoveInfatuation_CheckPartner
	status2animation BS_TARGET STATUS2_INFATUATION
	printstring 0x45
	waitmessage DELAY_1SECOND
	jumpifhelditemeffect BS_TARGET HOLD_EFFECT_DESTINY_KNOT BS_MaxMoveInfatuation_CheckTarget1DestinyKnot

BS_MaxMoveInfatuation_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	tryinfatuating BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	status2animation BS_TARGET STATUS2_INFATUATION
	printstring 0x45
	waitmessage DELAY_1SECOND
	jumpifhelditemeffect BS_TARGET HOLD_EFFECT_DESTINY_KNOT BS_MaxMoveInfatuation_CheckTarget2DestinyKnot
	callasm SetTargetFoePartner
	return

BS_MaxMoveInfatuation_CheckTarget1DestinyKnot:	
	tryinfatuating BS_ATTACKER BS_MaxMoveInfatuation_CheckPartner
	call BS_MaxMoveInfatuation_ActivateDestinyKnot
	goto BS_MaxMoveInfatuation_CheckPartner

BS_MaxMoveInfatuation_CheckTarget2DestinyKnot::
	tryinfatuating BS_ATTACKER BS_MaxMoveInfatuation_CheckPartner
	call BS_MaxMoveInfatuation_ActivateDestinyKnot
	goto BattleScript_MaxMoveSetTargetBackAndReturn

BS_MaxMoveInfatuation_ActivateDestinyKnot::
	playanimation BS_TARGET B_ANIM_ITEM_EFFECT 0x0
	status2animation BS_ATTACKER STATUS2_INFATUATION
	copyarray sBATTLER gBattlerAttacker 0x1
	setword BATTLE_STRING_LOADER DestinyKnotString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveRecycle::
	recycleberry BS_ATTACKER BS_MaxMoveRecycle_CheckPartner
	printstring 0x140
	waitmessage DELAY_1SECOND

BS_MaxMoveRecycle_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	recycleberry BS_TARGET BattleScript_MaxMoveEffectEnd @;Partner
	callasm SetAttackerPartner @;So the string prints the correct thing
	printstring 0x140
	callasm SetAttackerPartner
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveHealTeam::
	jumpifhasnohp BS_ATTACKER BS_MaxMoveHealTeam_CheckPartner
	jumpifhealthcomparestomax BS_ATTACKER EQUALS BS_MaxMoveHealTeam_CheckPartner
	setdamagetobankhealthfraction BS_ATTACKER, -6, 0x1 @;1/6 of Actual Max HP
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	swapattackerwithtarget @;So the string prints the correct thing
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	swapattackerwithtarget
	waitmessage DELAY_1SECOND

BS_MaxMoveHealTeam_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetAttackerPartner
	jumpifhasnohp BS_ATTACKER BS_MaxMoveHealTeam_RestoreOriginalAttacker
	jumpifhealthcomparestomax BS_ATTACKER EQUALS BS_MaxMoveHealTeam_RestoreOriginalAttacker
	setdamagetobankhealthfraction BS_ATTACKER, -6, 0x1 @;1/6 of Actual Max HP
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	swapattackerwithtarget @;So the string prints the correct thing
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	swapattackerwithtarget
	waitmessage DELAY_1SECOND

BS_MaxMoveHealTeam_RestoreOriginalAttacker::
	callasm SetAttackerPartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMovePoisonFoes::
	jumpifhasnohp BS_TARGET BattleScript_MaxMovePoisonFoes_CheckPartner
	callasm SetGMaxMalodorEffect
	seteffectsecondary

BattleScript_MaxMovePoisonFoes_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	callasm SetGMaxMalodorEffect
	seteffectsecondary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveSetStealthRock::
	trysetspikes BattleScript_MaxMoveEffectEnd
	printstring 0x184
	waitmessage DELAY_1SECOND
BattleScript_MaxMoveEffectEnd::
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveDefog::
	rapidspinfree
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMovePoisonParalyzeFoes::
	jumpifhasnohp BS_TARGET BattleScript_MaxMovePoisonParalyzeFoes_CheckPartner
	callasm PickRandomGMaxStunshockEffect
	seteffectsecondary

BattleScript_MaxMovePoisonParalyzeFoes_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	callasm PickRandomGMaxStunshockEffect
	seteffectsecondary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveSpite::
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveEffectEnd
	printstring 0x8D
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveSetGravity::
	callasm DoFieldEffect
	printstring 0x184
	waitmessage DELAY_1SECOND
	callasm BringDownMons
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveSetYawn::
	callasm ActuallySetYawn
	printstring 0xB6 @;STRINGID_PKMNWASMADEDROWSY
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveTrapAndDamageFoes::
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveTrapAndDamageFoes_CheckPartner
	setmoveeffect MOVE_EFFECT_WRAP
	seteffectprimary

BattleScript_MaxMoveTrapAndDamageFoes_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	setmoveeffect MOVE_EFFECT_WRAP
	seteffectprimary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveLowerEvasivenessFoes::
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveLowerEvasivenessFoes_CheckPartner
	setmoveeffect MOVE_EFFECT_EVS_MINUS_1
	seteffectprimary

BattleScript_MaxMoveLowerEvasivenessFoes_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	setmoveeffect MOVE_EFFECT_EVS_MINUS_1
	seteffectprimary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveAromatherapy::
	healpartystatus
	waitstate
	printfromtable gPartyStatusHealStringIds
	updatestatusicon 0x4
	waitstate
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveConfuseFoes::
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveConfuseFoes_CheckPartner
	callasm SetGMaxSmiteEffect
	seteffectprimary

BattleScript_MaxMoveConfuseFoes_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	callasm SetGMaxSmiteEffect
	seteffectprimary
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveTormentFoes::
	settorment BattleScript_MaxMoveTormentFoes_TryPartner
	printstring 0xA9 @;STRINGID_PKMNSUBJECTEDTOTORMENT
	waitmessage DELAY_1SECOND

BattleScript_MaxMoveTormentFoes_TryPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	settorment BattleScript_MaxMoveSetTargetBackAndReturn
	printstring 0xA9 @;STRINGID_PKMNSUBJECTEDTOTORMENT
	waitmessage DELAY_1SECOND
	callasm SetTargetFoePartner
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MaxMoveLowerSpeed2Foes::
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveLowerSpeed2Foes_CheckPartner
	setmoveeffect MOVE_EFFECT_SPD_MINUS_2
	seteffectprimary

BattleScript_MaxMoveLowerSpeed2Foes_CheckPartner::
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_MaxMoveEffectEnd
	callasm SetTargetFoePartner
	jumpifhasnohp BS_TARGET BattleScript_MaxMoveSetTargetBackAndReturn
	setmoveeffect MOVE_EFFECT_SPD_MINUS_2
	seteffectprimary
	callasm SetTargetFoePartner
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_HealingWishHeal::
	playanimation BS_SCRIPTING B_ANIM_HEALING_WISH_HEAL 0x0
	setword BATTLE_STRING_LOADER HealingWishHealString
	goto Sparkles

BattleScript_LunarDanceHeal::
	callasm RestorePPLunarDance
	playanimation BS_SCRIPTING B_ANIM_LUNAR_DANCE_HEAL 0x0
	setword BATTLE_STRING_LOADER LunarDanceHealString

Sparkles::
	printstring 0x184
	waitmessage DELAY_1SECOND
	playanimation BS_SCRIPTING B_ANIM_HEALING_SPARKLES 0x0
	healthbarupdate BS_SCRIPTING
	datahpupdate BS_SCRIPTING
	updatestatusicon BS_SCRIPTING
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SpikesHurt::
	orword HIT_MARKER 0x100100
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	setword BATTLE_STRING_LOADER gText_HurtBySpikes
	printstring 0x184
	waitmessage DELAY_1SECOND
	tryfaintmon BS_TARGET 0x0 0x0
	tryfaintmon BS_TARGET TRUE BattleScript_DmgHazardsOnAttackerFainted
	return

BattleScript_DmgHazardsOnAttackerFainted::
	setbyte sGIVEEXP_STATE, 0x0
	getexp BS_ATTACKER
	setbyte sMOVEEND_STATE, 0x0
	moveend 0x0, 0x0
	callasm TryToStopNewMonFromSwitchingInAfterSRHurt
	goto BattleScript_HandleFaintedMonSingles

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
BattleScript_SRHurt::
	orword HIT_MARKER 0x100100
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	setword BATTLE_STRING_LOADER gText_HurtByStealthRock
	printstring 0x184
	waitmessage DELAY_1SECOND
	tryfaintmon BS_TARGET 0x0 0x0
	tryfaintmon BS_TARGET TRUE BattleScript_DmgHazardsOnAttackerFainted
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SteelsurgeHurt::
	orword HIT_MARKER 0x100100
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	setword BATTLE_STRING_LOADER gText_HurtBySteelsurge
	printstring 0x184
	waitmessage DELAY_1SECOND
	tryfaintmon BS_TARGET 0x0 0x0
	tryfaintmon BS_TARGET TRUE BattleScript_DmgHazardsOnAttackerFainted
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
BattleScript_TSPoison::
	setbyte POISONED_BY 0x2 @To-Do, modify PSN set script
	orword HIT_MARKER HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_SUBSTITUTE @;Ignore Shield Dust
	setmoveeffect MOVE_EFFECT_POISON
	seteffectprimary
	bicword HIT_MARKER HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_SUBSTITUTE
	setbyte POISONED_BY 0x0
	return
	
BattleScript_TSHarshPoison::
	setbyte POISONED_BY 0x2
	orword HIT_MARKER HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_SUBSTITUTE @;Ignore Shield Dust
	setmoveeffect MOVE_EFFECT_TOXIC
	seteffectprimary
	bicword HIT_MARKER HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_SUBSTITUTE
	setbyte POISONED_BY 0x0
	return

BattleScript_TSAbsorb::
	setword BATTLE_STRING_LOADER gText_AbsorbedToxicSpikes
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StickyWebSpeedDrop::
	setword BATTLE_STRING_LOADER gText_CaughtInStickyWeb
	printstring 0x184
	waitmessage DELAY_HALFSECOND
	orword HIT_MARKER, HITMARKER_NON_ATTACK_DMG | HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_SUBSTITUTE @;Ignore Shield Dust
	jumpifability BS_TARGET ABILITY_MIRRORARMOR BattleScript_MirrorArmorStickyWeb
	setstatchanger2 STAT_SPEED | DECREASE_1
	statbuffchange STAT_TARGET | STAT_NOT_PROTECT_AFFECTED | STAT_BS_PTR BattleScript_StickyWebSpeedDropReturn
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_StickyWebSpeedDropReturn
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
BattleScript_StickyWebSpeedDropReturn:
	bicword HIT_MARKER, HITMARKER_NON_ATTACK_DMG | HITMARKER_IGNORE_SAFEGUARD | HITMARKER_IGNORE_SUBSTITUTE
	return
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PrimalWeatherEnd::
	pause DELAY_HALFSECOND
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SuccessForceOut::
	jumpifbyte EQUALS gForceSwitchHelper 0x2 RedCardForceSwitch
	jumpifbyte NOTEQUALS gForceSwitchHelper 0x0 SkipRoarAnim
	attackanimation
	waitanimation

SkipRoarAnim::
	switchoutabilities BS_SWITCHING
	returntoball BS_SWITCHING
	waitstate
	jumpifword ANDS gBattleTypeFlags BATTLE_TRAINER ForceSwitch
	jumpifword ANDS gBattleTypeFlags BATTLE_TYPE_DOUBLE ForceSwitch @Wild Double Battles force switches
	setbyte gBattleOutcome B_OUTCOME_PLAYER_TELEPORTED
	finishaction

ForceSwitch::
	callasm TryRemovePrimalWeatherSwitchingBank
	getswitchedmondata BS_SWITCHING
	switchindataupdate BS_SWITCHING
	switchinanim BS_SWITCHING 0x0
	waitstate
	printstring STRINGID_PKMNWASDRAGGEDOUT
	callasm MoldBreakerRemoveAbilitiesOnForceSwitchIn
	switchineffects BS_SWITCHING
	callasm RestoreAllOriginalMoveData
	jumpifmove MOVE_DRAGONTAIL BattleScript_DragonTailResetForceSwitchHelper
	jumpifmove MOVE_CIRCLETHROW BattleScript_DragonTailResetForceSwitchHelper
	setbyte gForceSwitchHelper 0x0
	goto BattleScript_MoveEnd

RedCardForceSwitch::
	switchoutabilities BS_SWITCHING
	returntoball BS_SWITCHING
	waitstate
	jumpifword ANDS gBattleTypeFlags BATTLE_TRAINER ForceSwitchRedCard
	jumpifword ANDS gBattleTypeFlags BATTLE_TYPE_DOUBLE ForceSwitchRedCard @Wild Double Battles force switches
	setbyte gBattleOutcome B_OUTCOME_PLAYER_TELEPORTED
	end

ForceSwitchRedCard::
	getswitchedmondata BS_SWITCHING
	switchindataupdate BS_SWITCHING
	switchinanim BS_SWITCHING 0x0
	waitstate
	printstring STRINGID_PKMNWASDRAGGEDOUT
	callasm MoldBreakerRemoveAbilitiesOnForceSwitchIn
	switchineffects BS_SWITCHING
	callasm MoldBreakerRestoreAbilitiesOnForceSwitchIn
	setbyte gForceSwitchHelper 0x0
	callasm RestoreAllOriginalMoveData
	copybyte sBATTLER, TARGET_BANK
	removeitem BS_SCRIPTING
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ActionSwitch::
	call BattleScript_FlushMessageBox
	copybyte gBankSwitching gBattlerAttacker
	callasm SetSwitchingBankSwitchingCooldownTo2
	callasm BackupSwitchingBank
	hpthresholds2 BS_SWITCHING
	atknameinbuff1
	printstring STRINGID_RETURNMON
	setbyte sDMG_MULTIPLIER 0x2
	jumpifbattletype BATTLE_TYPE_DOUBLE BattleScript_PursuitSwitchDmgSetMultihit
	setbyte gMultiHitCounter 0x1
	goto BattleScript_PursuitSwitchDmgLoop

BattleScript_PursuitSwitchDmgSetMultihit::
	setbyte gMultiHitCounter 0x2

BattleScript_PursuitSwitchDmgLoop::
	jumpifnopursuitswitchdmg BattleScript_DoSwitchOut
	swapattackerwithtarget
	trysetdestinybondtohappen
	call BattleScript_PursuitDmgOnSwitchOut
	swapattackerwithtarget

BattleScript_DoSwitchOut::
	decrementmultihit BattleScript_PursuitSwitchDmgLoop
	switchoutabilities BS_SWITCHING
	waitstate
	returnatktoball
	waitstate
	drawpartystatussummary BS_SWITCHING
	switchhandleorder BS_SWITCHING 0x1
	getswitchedmondata BS_SWITCHING
	switchindataupdate BS_SWITCHING
	hpthresholds BS_SWITCHING
	printstring STRINGID_SWITCHINMON
	hidepartystatussummary BS_SWITCHING
	switchinanim BS_SWITCHING 0x0
	waitstate
	switchineffects BS_SWITCHING
	
HandleActionSwitchEnd::
BattleScript_EntryHazardsHurtReturn::
	end2
	
BattleScript_PursuitDmgOnSwitchOut::
	pause DELAY_HALFSECOND
	setbyte gForceSwitchHelper 0x0
	callasm MoldBreakerRemoveAbilitiesOnForceSwitchIn
	call STANDARD_DAMAGE
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	setbyte sMOVEEND_STATE, 0x0
	moveend 0x4 0x0
	jumpifhasnohp BS_TARGET BattleScript_PursuitGiveExp
	return

BattleScript_PursuitGiveExp::
	setbyte sGIVEEXP_STATE 0x0
	getexp BS_TARGET
	swapattackerwithtarget
	goto HandleActionSwitchEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_HandleFaintedMonSingles::
	ifwildbattleend BattleScript_LinkBattleHandleFaint
	jumpifbyte NOTEQUALS, gBattleOutcome, 0, BattleScript_FaintedMonEnd2
	jumpifbattletype BATTLE_TRAINER, BattleScript_FaintedMonTryChooseAnother2
	jumpifword NOTANDS, HIT_MARKER, HITMARKER_PLAYER_FAINTED, BattleScript_FaintedMonTryChooseAnother2
	jumpifbattletype BATTLE_TYPE_DOUBLE BattleScript_FaintedMonTryChooseAnother2
	printstring STRINGID_USENEXTPKMN
	setbyte gBattleCommunication, 0
	yesnobox
	jumpifbyte EQUALS, gBattleCommunication + 1, 0, BattleScript_FaintedMonTryChooseAnother2
	jumpifplayerran BattleScript_FaintedMonEnd2
	printstring STRINGID_CANTESCAPE2
BattleScript_FaintedMonTryChooseAnother2::
	openpartyscreen BS_FAINTED, BattleScript_FaintedMonEnd2
	switchhandleorder BS_FAINTED, 2
	jumpifnotbattletype BATTLE_TRAINER, BattleScript_FaintedMonChooseAnother2
	jumpifbattletype BATTLE_TYPE_LINK, BattleScript_FaintedMonChooseAnother2
	jumpifbattletype BATTLE_TYPE_FRONTIER, BattleScript_FaintedMonChooseAnother2
	jumpifbattletype BATTLE_TYPE_DOUBLE, BattleScript_FaintedMonChooseAnother2
	jumpifbyte EQUALS, sBATTLE_STYLE, 1, BattleScript_FaintedMonChooseAnother2
	jumpifword ANDS, HIT_MARKER, HITMARKER_PLAYER_FAINTED, BattleScript_TryDoAIShiftSwitch
	jumpifcantswitch BS_PLAYER1, BattleScript_FaintedMonChooseAnother2
	printstring STRINGID_ENEMYABOUTTOSWITCHPKMN
	setbyte gBattleCommunication, 0
	yesnobox
	jumpifbyte EQUALS, gBattleCommunication + 1, 1, BattleScript_FaintedMonChooseAnother2
	callasm SetSwitchingBankToPlayer0
	setatktoplayer0
	openpartyscreen BS_ATTACKER | OPEN_PARTY_ALLOW_CANCEL, BattleScript_FaintedMonChooseAnother2
	switchhandleorder BS_ATTACKER, 2
	jumpifbyte EQUALS, gBattleCommunication, 6, BattleScript_FaintedMonChooseAnother2
	atknameinbuff1
	resetintimidatetracebits BS_ATTACKER
	hpthresholds2 BS_ATTACKER
	printstring 2 @;STRINGID_RETURNMON
	switchoutabilities BS_ATTACKER
	waitstate
	returnatktoball
	waitstate
	drawpartystatussummary BS_ATTACKER
	getswitchedmondata BS_ATTACKER
	switchindataupdate BS_ATTACKER
	hpthresholds BS_ATTACKER
	printstring STRINGID_SWITCHINMON
	hidepartystatussummary BS_ATTACKER
	switchinanim BS_ATTACKER, 0
	waitstate
	addindicatorforattackerswitchineffects
	resetsentmonsvalue
BattleScript_FaintedMonChooseAnother2::
	drawpartystatussummary BS_FAINTED
	getswitchedmondata BS_FAINTED
	switchindataupdate BS_FAINTED
BattleScript_FaintedMonChooseAnotherRejoin::
	hpthresholds BS_FAINTED
	printstring STRINGID_SWITCHINMON
	hidepartystatussummary BS_FAINTED
	switchinanim BS_FAINTED, 0
	waitstate
	resetplayerfaintedflag BS_ATTACKER
	callasm ClearSwitchInEffectsState
BattleScript_FaintedMonEnd2::
	end2

BattleScript_TryDoAIShiftSwitch::
	jumpifcantswitch BS_OPPONENT1, BattleScript_FaintedMonChooseAnother2
	drawpartystatussummary BS_FAINTED
	getswitchedmondata BS_FAINTED @;Load player data first, so AI can work off of it
	switchindataupdate BS_FAINTED
	callasm ShouldDoAIShiftSwitch @;Jumps to BattleScript_FaintedMonChooseAnotherRejoin if fails
	callasm SetAttackerAndSwitchingBankToOpponent0
	openpartyscreen BS_ATTACKER | OPEN_PARTY_ALLOW_CANCEL, BattleScript_FaintedMonChooseAnotherRejoin
	switchhandleorder BS_ATTACKER, 2
	jumpifbyte EQUALS, gBattleCommunication, 6, BattleScript_FaintedMonChooseAnotherRejoin
	atknameinbuff1
	resetintimidatetracebits BS_ATTACKER
	hpthresholds2 BS_ATTACKER
	printstring 2 @;STRINGID_RETURNMON
	switchoutabilities BS_ATTACKER
	waitstate
	returnatktoball
	waitstate
	drawpartystatussummary BS_ATTACKER
	getswitchedmondata BS_ATTACKER
	switchindataupdate BS_ATTACKER
	hpthresholds BS_ATTACKER
	printstring 3 @;STRINGID_SWITCHINMON
	hidepartystatussummary BS_ATTACKER
	switchinanim BS_ATTACKER, 0
	waitstate
	addindicatorforattackerswitchineffects
	resetsentmonsvalue
	copybyte sBATTLER gBattlerFainted
	callasm FaintedBankNameInBuff1
	goto BattleScript_FaintedMonChooseAnotherRejoin

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_HandleFaintedMonDoublesInitial::
	ifwildbattleend BattleScript_LinkBattleHandleFaintDoubles
	jumpifbyte NOTEQUALS gBattleOutcome 0 BattleScript_FaintedMonCancelSwitchIn
	jumpifnoviablemonsleft BS_FAINTED BattleScript_FaintedMonCancelSwitchIn
	openpartyscreen BS_FAINTED BattleScript_FaintedMonCancelSwitchIn
	switchhandleorder BS_FAINTED 0x2
	getswitchedmondata BS_FAINTED
	goto BattleScript_FaintedMonEnd2
	
BattleScript_FaintedMonCancelSwitchIn::
	callasm RemoveSwitchInForFaintedBank
	end2
	
BattleScript_LinkBattleHandleFaintDoubles::
	openpartyscreen 0x5, .+4
	switchhandleorder BS_FAINTED, 0x0
	openpartyscreen 0x6 BattleScript_FaintedMonEnd2
	switchhandleorder BS_FAINTED, 0x0	
	switchhandleorder BS_FAINTED, 0x2
	getswitchedmondata BS_FAINTED
	end2

BattleScript_HandleFaintedMonDoublesPart2::
	drawpartystatussummary BS_FAINTED
	switchindataupdate BS_FAINTED
	hpthresholds BS_FAINTED
	printstring 0x3 @;STRINGID_SWITCHINMON
	hidepartystatussummary BS_FAINTED
	switchinanim BS_FAINTED, FALSE
	waitstate
	resetplayerfaintedflag BS_ATTACKER
	end2

BattleScript_HandleFaintedMonDoublesSwitchInEffects::
	switchineffects BS_FAINTED
	end2

.global BattleScript_DoPlayerAndFoeSwitchInEffects
BattleScript_DoPlayerAndFoeSwitchInEffects::
	setbyte gBattlerAttacker 0x0
	switchineffects BS_ATTACKER
	switchineffects BS_FAINTED
	end2

