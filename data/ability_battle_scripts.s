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
#include "constants/hold_effects.h"
#include "battle_string_ids.h"
	.include "asm/macros/battle_script.inc"
@ Define these here since misc_constants.inc conflicts with the C headers
	.set NULL, 0
	.set FALSE, 0
	.set TRUE, 1
	.section script_data, "aw", %progbits
	.align 2

BattleScript_NewWeatherAbilityActivates::
	call BattleScript_NewWeatherAbilityActivatesCall
	end3

BattleScript_NewWeatherAbilityActivatesCall::
	call BattleScript_AbilityPopUp
	printfromtable gWeatherAbilityStrings
	waitstate
	playanimation2 BS_SCRIPTING sB_ANIM_ARG1 0x0
	call BattleScript_AbilityPopUpRevert
	call BattleScript_HandleWeatherFormChanges
	return

BattleScript_AirLock::
	call BattleScript_AbilityPopUp
	printstring STRINGID_CUSTOMSTRING
	waitmessage 0x40
	call BattleScript_AbilityPopUpRevert
	call BattleScript_HandleWeatherFormChanges
	end3

BattleScript_PauseIntimidateActivates::
	call BattleScript_AbilityPopUp
	setbyte TARGET_BANK 0x0

BattleScript_IntimidateActivatesLoop::
	setstatchanger2 STAT_ATK | DECREASE_1
	trygetintimidatetarget BattleScript_IntimidateActivatesReturn
	jumpifbehindsubstitute BS_TARGET IntimidateActivatesLoopIncrement
	statbuffchange STAT_TARGET | STAT_NOT_PROTECT_AFFECTED | STAT_BS_PTR IntimidateActivatesLoopIncrement
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_IntimidatePrevented
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	jumpifhelditemeffect BS_TARGET HOLD_EFFECT_ADRENALINE_ORB BattleScript_AdrenalineOrb
	goto IntimidateActivatesLoopIncrement
	
BattleScript_AdrenalineOrb::
	call BattleScript_AdrenalineOrbCall
	goto IntimidateActivatesLoopIncrement

BattleScript_AdrenalineOrbCall::
	setstatchanger2 STAT_SPEED| INCREASE_1
	statbuffchange STAT_TARGET | STAT_NOT_PROTECT_AFFECTED | STAT_BS_PTR, IntimidateActivatesLoopIncrement
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 IntimidateActivatesLoopIncrement
	playanimation BS_TARGET B_ANIM_ITEM_EFFECT 0x0
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	setword BATTLE_STRING_LOADER AdrenalineOrbString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	removeitem BS_TARGET
	return

BattleScript_IntimidatePrevented::
	pause DELAY_HALFSECOND
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

IntimidateActivatesLoopIncrement::
	jumpifword NOTANDS gBattleTypeFlags BATTLE_TYPE_DOUBLE BattleScript_IntimidateActivatesReturn
	addbyte TARGET_BANK 0x1
	trygetintimidatetarget BattleScript_IntimidateActivatesReturn
	callasm TryReactiveIntimidatePopUp
	goto BattleScript_IntimidateActivatesLoop

BattleScript_IntimidateActivatesReturn::
	callasm TryRemoveIntimidateAbilityPopUp @;In case the battle scripting bank is changed
	callasm RemoveIntimidateActive
	return
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


BattleScript_Frisk::
	call BattleScript_AbilityPopUp
	printstring STRINGID_CUSTOMSTRING
	waitmessage 0x40
	callasm TryLoadSecondFriskTargetDoubles
	printstring STRINGID_CUSTOMSTRING
	waitmessage 0x40
	
BattleScript_FriskEnd::
	call BattleScript_AbilityPopUpRevert
	end3	

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_NeutralizingGas::
	call BattleScript_AbilityPopUp
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	callasm UndoAbilityEffectsForNeutralizingGas
	call BattleScript_HandleWeatherFormChanges
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SwitchInAbilityMsg::
	call BattleScript_AbilityPopUp
	printstring STRINGID_CUSTOMSTRING
	waitmessage 0x40
	call BattleScript_AbilityPopUpRevert
BattleScript_End3::
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DrySkinDamage::
BattleScript_SolarPowerDamage::
	call BattleScript_AbilityPopUp
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_SCRIPTING
	datahpupdate BS_SCRIPTING
	setword BATTLE_STRING_LOADER HurtByAbilityString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	tryfaintmon BS_SCRIPTING 0x0 0x0
	end3
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Healer::
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER, gText_HealerCuredStatusProblem
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	updatestatusicon BS_EFFECT_BATTLER
	call BattleScript_AbilityPopUpRevert
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityCuredStatusEnd3::
	call BattleScript_AbilityCuredStatus
	end3
	
BattleScript_AbilityCuredStatus::
	call BattleScript_AbilityPopUp
	printstring STRINGID_PKMNSXCUREDITSYPROBLEM
	waitmessage 0x40
	updatestatusicon BS_SCRIPTING
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TerrainFromAbility::
	call BattleScript_AbilityPopUp
	callasm TransferTerrainData
	playanimation2 BS_SCRIPTING sB_ANIM_ARG1 0x0
	copyarray BATTLE_STRING_LOADER SEED_HELPER 0x4
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	setbyte SEED_HELPER 0
	callasm SeedRoomServiceLooper
	callasm TryActivateMimicry
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ImposterActivates::
	callasm TransferAbilityPopUpHelperAsImposter
	playanimation BS_SCRIPTING B_ANIM_LOAD_ABILITY_POP_UP 0x0
	playanimation BS_SCRIPTING 0xFF 0x0
	printfromtable gTransformUsedStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	clearspecialstatusbit BS_SCRIPTING STATUS3_INTIMIDATE_POKES
	tryactivateswitchinability BS_SCRIPTING
	end3


@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AttackerAbilityStatRaiseEnd3::
	call BattleScript_AttackerAbilityStatRaise
	end3	
	
BattleScript_AttackerAbilityStatRaise::
	call BattleScript_AbilityPopUp
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printstring STRINGID_PKMNSSTATCHANGED
	waitmessage 0x40
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityPopUp::
	callasm TransferAbilityPopUpHelper
	recordlastability BS_SCRIPTING
	playanimation BS_SCRIPTING B_ANIM_LOAD_ABILITY_POP_UP 0x0
	return

BattleScript_AbilityPopUpRevert::
	playanimation BS_SCRIPTING B_ANIM_DESTROY_ABILITY_POP_UP 0x0
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StartedSchoolingEnd3::
	call BattleScript_StartedSchoolingRet
	end3

BattleScript_StoppedSchoolingEnd3::
	call BattleScript_StartedSchoolingRet
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ShieldsDownToCoreEnd3::
	call BattleScript_ShieldsDownToCoreRet
	end3
	
BattleScript_ShieldsDownToMeteorEnd3::
	call BattleScript_ShieldsDownToMeteorRet
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TransformedEnd2::
	call BattleScript_AbilityTransformed
	end2

BattleScript_TransformedEnd3::
	call BattleScript_AbilityTransformed
	end3

BattleScript_AbilityTransformed::
	call BattleScript_AbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER TransformedString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EmergencyExit::
	jumpifbattletype BATTLE_TRAINER EmergencyExitSwitchBS

EmergencyExitFleeBS::
	getifcantrunfrombattle BS_SCRIPTING
	jumpifbyte EQUALS gBattleCommunication 0x1 EmergencyExitReturn
	call BattleScript_AbilityPopUp
	copyarray gBattlerAttacker sBATTLER 0x1
	printstring STRINGID_PKMNFLEDFROMBATTLE
	waitmessage DELAY_1SECOND
	setbyte gBattleOutcome 0x5 @;Teleported
	end

EmergencyExitSwitchBS::
	jumpifcantswitch BS_SWITCHING | ATK4F_DONT_CHECK_STATUSES EmergencyExitReturn
	call BattleScript_AbilityPopUp
	playanimation BS_SWITCHING B_ANIM_CALL_BACK_POKEMON 0x0
	callasm MakeSwitchingBankInvisible
	call BattleScript_AbilityPopUpRevert
	openpartyscreen BS_SWITCHING EmergencyExitReturn
	switchoutabilities BS_SWITCHING
	waitstate
	switchhandleorder BS_SWITCHING 0x2
	returntoball BS_SWITCHING
	getswitchedmondata BS_SWITCHING
	switchindataupdate BS_SWITCHING
	hpthresholds BS_SWITCHING
	printstring STRINGID_SWITCHINMON
	switchinanim BS_SWITCHING 0x1
	waitstate
	switchineffects BS_SWITCHING
	callasm SetNoMoreMovingThisTurnSwitchingBank @;New Pokemon can't attack after being switched in

EmergencyExitReturn:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoodySingleStat::
	call BattleScript_AbilityPopUp
MoodySkipString::
	statbuffchange STAT_ATTACKER | STAT_NOT_PROTECT_AFFECTED | STAT_CERTAIN MoodyStatRaiseEnd
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
MoodyStatRaiseEnd::
	call BattleScript_AbilityPopUpRevert
	end3

BattleScript_MoodyRegular::
	call BattleScript_AbilityPopUp
	statbuffchange STAT_ATTACKER | STAT_NOT_PROTECT_AFFECTED | STAT_CERTAIN MoodyRegularP2
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	
MoodyRegularP2::
	callasm LoadMoodyStatToLower
	goto MoodySkipString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BadDreams::
	call BattleScript_AbilityPopUp
	setbyte SEED_HELPER 0x0
	jumpifbyte ANDS gBattlerAttacker 0x1 BadDreams_OpponentSidePlayer1

BadDreams_PlayerSideOpponent1::
	setbyte TARGET_BANK 0x1
	jumpifstatus BS_TARGET STATUS1_SLEEP BadDreams_callhurt
	jumpifability BS_TARGET ABILITY_COMATOSE BadDreams_callhurt
	addbyte SEED_HELPER 0x1
BadDreams_PlayerSideOpponent2::
	jumpifbyte NOTEQUALS NUM_POKEMON 0x4 BadDreams_end @not double battle
	setbyte TARGET_BANK 0x3
	jumpifstatus BS_TARGET STATUS1_SLEEP BadDreams_callhurt
	jumpifability BS_TARGET ABILITY_COMATOSE BadDreams_callhurt
	goto BadDreams_end

BadDreams_OpponentSidePlayer1::
	setbyte SEED_HELPER 0x2
	setbyte TARGET_BANK 0x0
	jumpifstatus BS_TARGET STATUS1_SLEEP BadDreams_callhurt
	jumpifability BS_TARGET ABILITY_COMATOSE BadDreams_callhurt
	addbyte SEED_HELPER 0x1
BadDreams_OpponentSidePlayer2::
	jumpifbyte NOTEQUALS NUM_POKEMON 0x4 BadDreams_end @not double battle
	setbyte TARGET_BANK 0x2
	jumpifstatus BS_TARGET STATUS1_SLEEP BadDreams_callhurt
	jumpifability BS_TARGET ABILITY_COMATOSE BadDreams_callhurt
	goto BadDreams_end

BadDreams_callhurt::
	call BadDreams_hurt
	jumpifbyte EQUALS SEED_HELPER 0x1 BadDreams_PlayerSideOpponent2
	jumpifbyte EQUALS SEED_HELPER 0x2 BadDreams_end
	jumpifbyte EQUALS SEED_HELPER 0x3 BadDreams_OpponentSidePlayer2
	jumpifbyte EQUALS SEED_HELPER 0x4 BadDreams_end
	goto BadDreams_end

BadDreams_hurt::
	jumpifability BS_TARGET ABILITY_MAGICGUARD BadDreams_return
	setdamagetobankhealthfraction BS_TARGET 8 0x0 @;1/8 of base HP
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	setword BATTLE_STRING_LOADER BadDreamsHurtString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	tryfaintmon BS_TARGET 0x0 0x0
	BadDreams_return:
	addbyte SEED_HELPER 0x1
	return

BadDreams_end:
	setbyte SEED_HELPER 0x0
	callasm TryHideActiveAbilityPopUps
	end3
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Harvest::
	tryrecycleitem HarvestBSEnd
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER HarvestString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	callasm HarvestActivateBerry

HarvestBSEnd:
	setbyte FORM_COUNTER 0x0
	end3
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Pickup::
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER PickupString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BallFetch::
	call BattleScript_AbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_SNATCH_MOVE 0x0
	setword BATTLE_STRING_LOADER gText_BallFetchRetrievedItem
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EmergencyExitEnd3::
	call BattleScript_EmergencyExit
	end3

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AttackerCantUseMove::
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER CantUseMoveString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveStatDrain_PPLoss::
	ppreduce
BattleScript_MoveStatDrain::
	orbyte gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE
	attackstring
	pause DELAY_HALFSECOND
	call BattleScript_TargetAbilityStatRaise
	goto BattleScript_MoveEnd

BattleScript_TargetAbilityStatRaise::
	call BattleScript_AbilityPopUp
	statbuffchange BS_TARGET, BattleScript_TargetAbilityStatRaiseReturn
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_TargetAbilityStatRaiseReturn
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printstring STRINGID_PKMNSSTATCHANGED4
	waitmessage DELAY_1SECOND
	
BattleScript_TargetAbilityStatRaiseReturn:
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_WeakArmorActivates::
	jumpifstat BS_TARGET GREATERTHAN STAT_DEF STAT_MIN WeakArmorModDef
	jumpifstat BS_TARGET EQUALS STAT_SPEED STAT_MAX WeakArmorReturn

WeakArmorModDef::
	call BattleScript_AbilityPopUp
	setbyte sSTATCHANGER STAT_DEF | DECREASE_1
	statbuffchange BS_TARGET | STAT_CERTAIN WeakArmorModSpd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 WeakArmorModSpd
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	
WeakArmorModSpd::
	jumpifstat BS_TARGET EQUALS STAT_SPEED STAT_MAX WeakArmorRevertPopUp
	setbyte sSTATCHANGER, STAT_SPEED | INCREASE_2
	statbuffchange BS_TARGET | STAT_CERTAIN WeakArmorRevertPopUp
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 WeakArmorRevertPopUp
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	
WeakArmorRevertPopUp::
	call BattleScript_AbilityPopUpRevert

WeakArmorReturn:
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_CursedBodyActivates::
	disablelastusedattack BS_ATTACKER CursedBodyReturn
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER CursedBodyString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	
CursedBodyReturn:
	return
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MummyActivates::
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	copyarray sBATTLER gBattlerAttacker 0x1
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	setability BS_ATTACKER ABILITY_MUMMY
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER MummyString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_WanderingSpiritActivates::
	swapattackerwithtarget
	tryswapabilities BattleScript_WanderingSpiritActivatesReturn
	callasm TransferAbilityPopUpHelperAsWanderingSpirit
	playanimation BS_SCRIPTING B_ANIM_LOAD_ABILITY_POP_UP 0x0
	call BattleScript_AbilityPopUpRevert

	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	copyarray sBATTLER TARGET_BANK 0x1
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	printstring STRINGID_PKMNSWAPPEDABILITIES
	waitmessage DELAY_1SECOND
	tryactivateswitchinability BS_ATTACKER
	tryactivateswitchinability BS_TARGET

BattleScript_WanderingSpiritActivatesReturn:
	swapattackerwithtarget
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
BattleScript_GooeyActivates::
	call BattleScript_AbilityPopUp
	swapattackerwithtarget @;Allows for abilities like Defiant and Mirror Armor to have their proper effect
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_TARGET STAT_ANIM_SPD STAT_ANIM_DOWN
	setbyte sSTATCHANGER, STAT_SPEED | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR GooeyReturn
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 GooeyReturn
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert

GooeyReturn:
	swapattackerwithtarget
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_IllusionBroken::
	callasm TransferIllusionBroken
	call BattleScript_AbilityPopUp
	reloadhealthbar BS_TARGET
	playanimation BS_TARGET B_ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER IllusionWoreOffString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return
	
BattleScript_IllusionBrokenFaint::
	callasm TransferIllusionBroken
	call BattleScript_AbilityPopUp
	reloadhealthbar BS_TARGET
	callasm CycleScriptingBankHealthBetween0And1
	playanimation BS_TARGET B_ANIM_TRANSFORM 0x0
	callasm CycleScriptingBankHealthBetween0And1
	setword BATTLE_STRING_LOADER IllusionWoreOffString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AngerPointActivates::
	call BattleScript_AbilityPopUp
	setbyte sSTATCHANGER, STAT_ATK | INCREASE_1
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	setword BATTLE_STRING_LOADER AngerPointString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SynchronizeActivates::
	pause 0x10
	call BattleScript_AbilityPopUp
	jumpifstatus BS_TARGET, STATUS1_ANY, SynchronizeNoEffect
	jumpifsideaffecting BS_TARGET SIDE_SAFEGUARD SynchronizeSafeguard
	seteffectprimary
	jumpifstatus BS_TARGET, STATUS1_ANY, SynchronizeReturn

SynchronizeNoEffect::
	setword BATTLE_STRING_LOADER SynchronizeNoEffectString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto SynchronizeReturn
	
SynchronizeSafeguard:
	printstring 0x50
	waitmessage DELAY_1SECOND

SynchronizeReturn::
	call BattleScript_AbilityPopUpRevert
	callasm RestoreBanksFromSynchronize
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PerishBody::
	callasm ActivatePerishBody
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_PerishBodyActivated
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	jumpifhasnohp BS_ATTACKER BattleScript_PerishBodyReturn

BattleScript_PerishBodyReturn::
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityChangedType:: @;For Protean
	pause DELAY_HALFSECOND
	callasm DoProteanTypeChange
	call BattleScript_AbilityPopUp
	printstring STRINGID_PKMNCHANGEDTYPE @;STRINGID_PKMNCHANGEDTYPE - uses ATTACKER
	waitmessage DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	return
	
BattleScript_AbilityChangedTypeContact::
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_AbilityChangedType
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityApplySecondaryEffect::
	waitstate
	copybyte FORM_COUNTER sBATTLER
	call BattleScript_AbilityPopUp
	waitstate
	seteffectsecondary
	copybyte sBATTLER FORM_COUNTER
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MimicryTransformed::
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_AbilityChangedType
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_MimicryReturnedToNormal::
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_MimicryReturnedToNormal
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DisguiseTookDamage::
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER MimikyuDisguisedTookDamageString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DisguiseTransform::
	playanimation BS_TARGET B_ANIM_TRANSFORM 0x0
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_SCRIPTING
	datahpupdate BS_SCRIPTING
	bicword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	setword BATTLE_STRING_LOADER DisguiseBustedString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_IceFaceTookDamage::
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_IceFaceTookDamage
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_IceFaceTransform::
	playanimation BS_TARGET B_ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER gText_IceFaceBusted
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AvoidedMoveWithAbility::
	call BattleScript_AbilityPopUp
	printstring STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SoulHeart::
	call BattleScript_AbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	setword BATTLE_STRING_LOADER AbilityRaisedStatString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_EnduredSturdy::
	pause 0x10
	call BattleScript_AbilityPopUp
	printstring STRINGID_PKMNENDUREDHIT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Receiver::
	playanimation BS_SCRIPTING B_ANIM_LOAD_ABILITY_POP_UP 0x0
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER ReceiverString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	clearspecialstatusbit BS_SCRIPTING STATUS3_INTIMIDATE_POKES
	tryactivateswitchinability BS_SCRIPTING
	return
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PartnerAbilityNoStatLoss::
	pause 0x10
	callasm TryRemoveIntimidateAbilityPopUp
	callasm TryHideActiveAbilityPopUps @;Basically Mirror Armor
	copybyte sBATTLER gBattleCommunication
	callasm SetBattleScriptingBankForPartnerAbilityNoStatLoss
	call BattleScript_AbilityPopUp
	copybyte sBATTLER gBattleCommunication
	printstring 0xCE
	waitmessage DELAY_1SECOND
	callasm SetBattleScriptingBankForPartnerAbilityNoStatLoss
	call BattleScript_AbilityPopUpRevert
	copybyte sBATTLER gBattleCommunication
	setbyte cMULTISTRING_CHOOSER 0x4
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Symbiosis::
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER SymbiosisString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MirrorArmorReflectsStatLoss::
	copybyte FORM_COUNTER sBATTLER
	callasm TryHideActiveAbilityPopUps @;Basically hide Gooey
	copybyte sBATTLER FORM_COUNTER
	call BattleScript_AbilityPopUp
	jumpifbehindsubstitute BS_ATTACKER BattleScript_AbilityNoSpecificStatLossPrintMessage

BattleScript_MirrorArmorReflectsStatLoss_StatChange::
	statbuffchange STAT_ATTACKER | STAT_NOT_PROTECT_AFFECTED | STAT_BS_PTR BattleScript_MirrorArmorReflectsStatLossReturn
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MirrorArmorReflectsStatLossAttackerStatCantGoLower
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

BattleScript_MirrorArmorReflectsStatLossReturn::
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_MirrorArmorReflectsStatLossAttackerStatCantGoLower::
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	setbyte cMULTISTRING_CHOOSER 0x3
	goto BattleScript_MirrorArmorReflectsStatLossReturn

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MirrorArmorStickyWeb::
	copybyte sBATTLER TARGET_BANK
	call BattleScript_AbilityPopUp
	callasm ChooseTargetForMirrorArmorStickyWeb
	jumpifarrayequal TARGET_BANK gBattlerAttacker 1 BattleScript_AbilityNoSpecificStatLossPrintMessage @;No target on other side of field
	setstatchanger2 STAT_SPEED| DECREASE_1
	goto BattleScript_MirrorArmorReflectsStatLoss_StatChange

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MirrorArmorReflectsIntimidate::
	pause 0x10
	copybyte FORM_COUNTER sBATTLER
	callasm TryRemoveIntimidateAbilityPopUp
	copybyte sBATTLER FORM_COUNTER
	call BattleScript_AbilityPopUp
	copybyte gBattlerAttacker sBATTLER
	callasm LoadIntimidateBankIntoTarget
	swapattackerwithtarget
	bicword HIT_MARKER HITMARKER_SWAP_ATTACKER_TARGET
	jumpifbehindsubstitute BS_ATTACKER BattleScript_AbilityNoSpecificStatLossPrintMessage
	swapattackerwithtarget
	bicword HIT_MARKER HITMARKER_SWAP_ATTACKER_TARGET
	statbuffchange STAT_TARGET | STAT_NOT_PROTECT_AFFECTED | STAT_BS_PTR BattleScript_MirrorArmorReflectsIntimidateReturn
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MirrorArmorAttackerStatCantGoLower
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	jumpifhelditemeffect BS_TARGET HOLD_EFFECT_ADRENALINE_ORB BattleScript_MirrorArmorAdrenalineOrb

BattleScript_MirrorArmorReflectsIntimidateReturn::
	swapattackerwithtarget @;Places the initial target in the target bank
	bicword HIT_MARKER HITMARKER_SWAP_ATTACKER_TARGET
	copybyte sBATTLER FORM_COUNTER
	call BattleScript_AbilityPopUpRevert
	return
	
BattleScript_MirrorArmorAdrenalineOrb::
	call BattleScript_AdrenalineOrbCall
	goto BattleScript_MirrorArmorReflectsIntimidateReturn

BattleScript_MirrorArmorAttackerStatCantGoLower::
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	setbyte cMULTISTRING_CHOOSER 0x3
	goto BattleScript_MirrorArmorReflectsIntimidateReturn

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BattleScript_DefiantCompetitive::
	statbuffchange STAT_TARGET | STAT_BS_PTR DefiantReturn
	
BattleScript_DefiantCompetitiveCall::
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 DefiantReturn
	pause DELAY_HALFSECOND
	setbyte sSTAT_ANIM_PLAYED 0x0
	callasm TryRemoveIntimidateAbilityPopUp
	callasm TryHideActiveAbilityPopUps @;Basically Mirror Armor
	copybyte sBATTLER TARGET_BANK
	call BattleScript_AbilityPopUp
	setgraphicalstatchangevalues
	playanimation BS_SCRIPTING B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert

DefiantReturn::
	return

BattleScript_FaintRaidAttacker::
	playfaintcry BS_ATTACKER
	playanimation BS_ATTACKER B_ANIM_POWDER_EXPLOSION 0x0
	cleareffectsonfaint BS_ATTACKER
	callasm ClearPlayerRechargeMultipleTurns @;So the game doesn't lock
	callasm FinishTurn
	return

BattleScript_FaintRaidTarget::
	playfaintcry BS_TARGET
	playanimation BS_TARGET B_ANIM_POWDER_EXPLOSION 0x0
	cleareffectsonfaint BS_TARGET
	callasm ClearPlayerRechargeMultipleTurns @;So the game doesn't lock
	callasm FinishTurn
	return

BattleScript_FaintScriptingBank::
	playfaintcry BS_SCRIPTING
	pause 0x30
	dofaintanimation BS_SCRIPTING
	setword BATTLE_STRING_LOADER ScriptingBattlerFainted
	printstring STRINGID_CUSTOMSTRING
	cleareffectsonfaint BS_SCRIPTING
	printstring 0x130
	trytrainerslidefirstdownmsg BS_SCRIPTING
	return

BattleScript_FaintRaidScriptingBank::
	playfaintcry BS_SCRIPTING
	playanimation BS_SCRIPTING B_ANIM_POWDER_EXPLOSION 0x0
	cleareffectsonfaint BS_SCRIPTING
	callasm ClearPlayerRechargeMultipleTurns @;So the game doesn't lock
	callasm FinishTurn
	return

BattleScript_SuccessBallThrow::
	jumpifhalfword EQUALS gLastUsedItem ITEM_SAFARI_BALL BattleScript_PrintCaughtMonInfo
	incrementgamestat GAME_STAT_POKEMON_CAPTURES

BattleScript_PrintCaughtMonInfo:
	printstring STRINGID_GOTCHAPKMNCAUGHT
	copyarray sBATTLER TARGET_BANK 0x1
	callasm MakeScriptingBankInvisible
	setbyte sGIVEEXP_STATE, 0x0
	getexp BS_TARGET
	@;sethword gBattle_BG2_X, 0x0
	goto BattleScript_SafariNoIncGameStat
	
BattleScript_BenjaminButterfreeDevolution::
	callasm RestoreEffectBankHPStatsAndRemoveBackupSpecies
	reloadhealthbar BS_EFFECT_BATTLER
	playanimation BS_EFFECT_BATTLER B_ANIM_TRANSFORM 0x0
	playfaintcry BS_EFFECT_BATTLER
	setword BATTLE_STRING_LOADER gText_Devolved
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	cureprimarystatus BS_EFFECT_BATTLER 0x0
	reloadhealthbar BS_EFFECT_BATTLER
	setword BATTLE_STRING_LOADER gText_DevolutionsStatsReset
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	callasm SetSkipCertainSwitchInAbilities  @;Skip certain abilities if you fainted from recoil and KO'd the Foe
	tryactivateswitchinability BS_EFFECT_BATTLER
	return
