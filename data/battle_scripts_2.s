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
#include "battle_string_ids.h"
	.include "asm/macros/battle_script.inc"
@ Define these here since misc_constants.inc conflicts with the C headers
	.set NULL, 0
	.set FALSE, 0
	.set TRUE, 1
	.section script_data, "aw", %progbits
	.align 2

gBattlescriptsForBallThrow::
	.4byte BattleScript_ThrowBall
	.4byte BattleScript_ThrowSafariBall

gBattlescriptsForUsingItem::
	.4byte BattleScript_PlayerUseItem
	.4byte BattleScript_AIUseFullRestoreOrHpHeal
	.4byte BattleScript_AIUseFullRestoreOrHpHeal
	.4byte BattleScript_AIUseStatRestore
	.4byte BattleScript_AIUseXstat
	.4byte BattleScript_AIUseGuardSpec

gBattlescriptsForRunningByItem::
	.4byte BattleScript_UseFluffyTail
	.4byte BattleScript_UsePokeFlute

gBattlescriptsForSafariActions::
	.4byte BattleScript_WatchesCarefully
	.4byte BattleScript_ThrowRock
	.4byte BattleScript_ThrowBait
	.4byte BattleScript_LeftoverWallyPrepToThrow

BattleScript_ThrowBall::
	jumpifbattletype BATTLE_TYPE_OLD_MAN_TUTORIAL, BattleScript_OldManThrowBall
	jumpifbattletype BATTLE_TYPE_POKEDUDE, BattleScript_PokedudeThrowBall
	printstring STRINGID_PLAYERUSEDITEM
	handleballthrow

BattleScript_OldManThrowBall::
	printstring STRINGID_OLDMANUSEDITEM
	handleballthrow

BattleScript_PokedudeThrowBall::
	printstring STRINGID_POKEDUDEUSED
	handleballthrow

BattleScript_ThrowSafariBall::
	printstring STRINGID_PLAYERUSEDITEM
	updatestatusicon BS_ATTACKER
	handleballthrow

BattleScript_SafariNoIncGameStat::
	trysetcaughtmondexflags BattleScript_CaughtPokemonSkipNewDex
	printstring STRINGID_PKMNDATAADDEDTODEX
	waitstate
	setbyte gBattleCommunication, 0
	displaydexinfo
BattleScript_CaughtPokemonSkipNewDex::
	printstring STRINGID_GIVENICKNAMECAPTURED
	waitstate
	setbyte gBattleCommunication, 0
	trygivecaughtmonnick BattleScript_CaughtPokemonSkipNickname
	givecaughtmon
	printfromtable gCaughtMonStringIds
	waitmessage 64
	goto BattleScript_CaughtPokemonDone

BattleScript_CaughtPokemonSkipNickname::
	givecaughtmon
BattleScript_CaughtPokemonDone::
	setbyte gBattleOutcome, B_OUTCOME_CAUGHT
	finishturn

BattleScript_OldMan_Pokedude_CaughtMessage::
	printstring STRINGID_GOTCHAPKMNCAUGHT2
	setbyte gBattleOutcome, B_OUTCOME_CAUGHT
	flee
	finishturn

BattleScript_ShakeBallThrow::
	printfromtable gBallEscapeStringIds
	waitmessage 64
	jumpifnotbattletype BATTLE_TYPE_SAFARI, BattleScript_CatchFailEnd
	jumpifbyte CMP_NOT_EQUAL, gNumSafariBalls, 0, BattleScript_CatchFailEnd
	printstring STRINGID_OUTOFSAFARIBALLS
	waitmessage 64
	setbyte gBattleOutcome, B_OUTCOME_NO_SAFARI_BALLS
BattleScript_CatchFailEnd::
	finishaction

BattleScript_TrainerBallBlock::
	waitmessage 64
	printstring STRINGID_TRAINERBLOCKEDBALL
	waitmessage 64
	printstring STRINGID_DONTBEATHIEF
	waitmessage 64
	finishaction

BattleScript_GhostBallDodge::
	waitmessage 64
	printstring STRINGID_ITDODGEDBALL
	waitmessage 64
	finishaction

BattleScript_PlayerUseItem::
	moveendcase 15
	end

BattleScript_AIUseFullRestoreOrHpHeal::
	printstring STRINGID_EMPTYSTRING3
	pause 48
	playse SE_USE_ITEM
	printstring STRINGID_TRAINER1USEDITEM
	waitmessage 64
	useitemonopponent
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring STRINGID_PKMNSITEMRESTOREDHEALTH
	waitmessage 64
	updatestatusicon BS_ATTACKER
	moveendcase 15
	finishaction

BattleScript_AIUseStatRestore::
	printstring STRINGID_EMPTYSTRING3
	pause 48
	playse SE_USE_ITEM
	printstring STRINGID_TRAINER1USEDITEM
	waitmessage 64
	useitemonopponent
	printfromtable gTrainerItemCuredStatusStringIds
	waitmessage 64
	updatestatusicon BS_ATTACKER
	moveendcase 15
	finishaction

BattleScript_AIUseXstat::
	printstring STRINGID_EMPTYSTRING3
	pause 48
	playse SE_USE_ITEM
	printstring STRINGID_TRAINER1USEDITEM
	waitmessage DELAY_1SECOND
	useitemonopponent
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	finishaction

BattleScript_AIUseGuardSpec::
	printstring STRINGID_EMPTYSTRING3
	pause 48
	playse SE_USE_ITEM
	printstring STRINGID_TRAINER1USEDITEM
	waitmessage 64
	useitemonopponent
	printfromtable gMistUsedStringIds
	waitmessage 64
	moveendcase 15
	finishaction

BattleScript_UseFluffyTail::
	playse SE_FLEE
	setbyte gBattleOutcome, B_OUTCOME_RAN
	finishturn

BattleScript_UsePokeFlute::
	checkpokeflute BS_ATTACKER
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 1, BattleScript_PokeFluteWakeUp
	printstring STRINGID_POKEFLUTECATCHY
	waitmessage 64
	goto BattleScript_PokeFluteEnd

BattleScript_PokeFluteWakeUp::
	printstring STRINGID_POKEFLUTE
	waitmessage 64
	fanfare MUS_POKE_FLUTE
	waitfanfare BS_ATTACKER
	printstring STRINGID_MONHEARINGFLUTEAWOKE
	waitmessage 64
	updatestatusicon BS_PLAYER2
	waitstate
BattleScript_PokeFluteEnd::
	finishaction

BattleScript_WatchesCarefully::
	printfromtable gSafariPokeblockResultStringIds
	waitmessage 64
	playanimation BS_OPPONENT1, B_ANIM_SAFARI_REACTION, NULL
	end2

BattleScript_ThrowRock::
	printstring STRINGID_THREWROCK
	waitmessage 64
	playanimation BS_ATTACKER, B_ANIM_ROCK_THROW, NULL
	end2

BattleScript_ThrowBait::
	printstring STRINGID_THREWBAIT
	waitmessage 64
	playanimation BS_ATTACKER, B_ANIM_BAIT_THROW, NULL
	end2

BattleScript_LeftoverWallyPrepToThrow::
	printstring STRINGID_RETURNMON
	waitmessage 64
	returnatktoball
	waitstate
	trainerslidein BS_TARGET
	waitstate
	printstring STRINGID_YOUTHROWABALLNOWRIGHT
	waitmessage 64
	end2

STANDARD_DAMAGE::
	attackstring
	ppreduce
STANDARD_DAMAGE2::
	critcalc
	damagecalc
	typecalc
	adjustnormaldamage
	attackanimation
	waitanimation
	effectivenesssound
	hitanimation BS_TARGET
	waitstate
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	critmessage
	waitmessage DELAY_1SECOND
	resultmessage
	waitmessage DELAY_1SECOND
	return

FailedPreBS::
BattleScript_ButItFailedAttackstring::
FAILED_PRE::
	attackstring
	ppreduce
FAILED::
BattleScript_ButItFailed::
	pause DELAY_HALFSECOND
	orbyte gMoveResultFlags OUTCOME_FAILED
	resultmessage
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_TeamProtectedBySweetVeil::
	pause 0x10
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_SweetVeilProtects
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

BattleScript_TeamProtectedByFlowerVeil::
	pause 0x10
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_FlowerVeilProtects
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

BattleScript_ProtectedByAbility::
	pause 0x10
	copyarray sBATTLER TARGET_BANK 0x1
	call BattleScript_AbilityPopUp
	printstring STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

BattleScript_TargetStayedAwakeUsingAbility::
	pause 0x10
	copyarray sBATTLER TARGET_BANK 0x1
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_TargetStaysAwake
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd
	
BattleScript_PauseResultMessage::
	pause DELAY_HALFSECOND
	goto Resultmessage @;Resultmessage

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@new move effect@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SideStatusWoreOffRet::
	setword BATTLE_STRING_LOADER gText_DefogRemovedSideStatus
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BattleScript_EffectHealBell::
	attackcanceler
	attackstring
	ppreduce
	healpartystatus
	waitstate
	attackanimation
	waitanimation
	printfromtable gPartyStatusHealStringIds
	waitmessage DELAY_1SECOND
	jumpifmove MOVE_AROMATHERAPY AromatherapySapSipperCheckBS
	jumpifnotmove MOVE_HEALBELL BattleScript_PartyHealEnd
	jumpifbyte NOTANDS cMULTISTRING_CHOOSER 0x1 BattleScript_CheckHealBellMon2Unaffected
	copybyte FORM_COUNTER sBATTLER
	copybyte sBATTLER gBattlerAttacker
	call BattleScript_AbilityPopUp
	printstring STRINGID_PKMNSXBLOCKSY2
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	copybyte sBATTLER FORM_COUNTER

BattleScript_CheckHealBellMon2Unaffected::
	jumpifbyte NOTANDS cMULTISTRING_CHOOSER 0x2 BattleScript_PartyHealEnd
	call BattleScript_AbilityPopUp
	printstring STRINGID_PKMNSXBLOCKSY2
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert

BattleScript_PartyHealEnd::
	updatestatusicon 0x4
	waitstate
	goto BattleScript_MoveEnd

AromatherapySapSipperCheckBS::
	callasm TryActivatePartnerSapSipper
	goto BattleScript_PartyHealEnd
	
BattleScript_SapSipperAromatherapy::
	statbuffchange STAT_TARGET | STAT_BS_PTR SapSipperReturnBS
	call BattleScript_AbilityPopUp
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
SapSipperReturnBS::
	return

BattleScript_DefogAdditionalEffects::
	attackanimation @;Should only play after the Second Defog Check
	waitanimation
	jumpifweather weather_circus SkipRemoveFogBS
	jumpifweather weather_fog | weather_permament_fog RemoveFogBS
SkipRemoveFogBS:
	rapidspinfree
	goto BattleScript_MoveEnd

RemoveFogBS::
	sethalfword gBattleWeather 0x0
	setword BATTLE_STRING_LOADER RemoveFogString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	rapidspinfree
	goto BattleScript_MoveEnd
	
SecondDefogCheck::
	callasm DefogHelperFunc @;Automatically redirects to BattleScript_DefogAdditionalEffects if applicable
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

LifeDewBS::
	callasm TryFailLifeDew
	attackanimation
	waitanimation
	tryhealhalfhealth LifeDewAttackerFullHealthBS BS_ATTACKER
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND

LifeDewRestorePartnerHPBS::
	callasm SetTargetPartner
	jumpifhasnohp BS_TARGET BattleScript_MoveEnd
	jumpifcounter BS_TARGET HEAL_BLOCK_TIMERS NOTEQUALS 0x0 BattleScript_NoHealTargetAfterHealBlock
	accuracycheck LifeDewMissedPartnerBS 0x0
	tryhealhalfhealth LifeDewPartnerFullHealthBS BS_TARGET
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printstring STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

LifeDewAttackerFullHealthBS::
	printstring STRINGID_PKMNHPFULL @;STRINGID_PKMNHPFULL
	waitmessage DELAY_1SECOND
	goto LifeDewRestorePartnerHPBS

LifeDewMissedPartnerBS::
	orbyte gMoveResultFlags OUTCOME_MISSED
	goto BattleScript_MoveMissed3

BattleScript_LifeDewFail::
	pause DELAY_HALFSECOND
	printstring STRINGID_PKMNHPFULL @;STRINGID_PKMNHPFULL
	waitmessage DELAY_1SECOND
	callasm SetTargetPartner
LifeDewPartnerFullHealthBS::
	printstring STRINGID_PKMNHPFULL @;STRINGID_PKMNHPFULL
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectRecover::
	attackcanceler
	jumpifmove MOVE_PURIFY PurifyBS
	attackstring
	ppreduce
	jumpifmove MOVE_ROOST RoostBS
	jumpifmove MOVE_LIFEDEW LifeDewBS
	jumpifmove MOVE_JUNGLEHEALING LifeDewBS @TODO

RecoverBS::
	tryhealhalfhealth BattleScript_AlreadyAtFullHp BS_ATTACKER @;BattleScript_AlreadyAtFullHp
	
RecoverAnimBS::
	attackanimation
	waitanimation
	
RecoverHealthUpdateBS::
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectTeleport::
	attackcanceler
	callasm SetCorrectTeleportBattleScript

BattleScript_TeleportFlee::
	jumpifbattletype BATTLE_TYPE_DOUBLE, FAILED_PRE
	callasm SetTeleportBit
	getifcantrunfrombattle BS_ATTACKER
	jumpifbyte EQUALS gBattleCommunication 0x1 FAILED_PRE
	jumpifbyte EQUALS gBattleCommunication 0x2 FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring STRINGID_PKMNFLEDFROMBATTLE
	waitmessage DELAY_1SECOND
	setbyte gBattleOutcome 0x5 @;Teleported
	goto BattleScript_MoveEnd

BattleScript_TeleportSwitch::
	jumpifcantswitch BS_ATTACKER | ATK4F_DONT_CHECK_STATUSES, FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	copybyte gBankSwitching gBattlerAttacker
	goto BatonPassSwitchOutBS

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectBatonPass::
	attackcanceler
	copyarray BACKUP_HWORD gCurrentMove 2
	jumpifnotmove MOVE_BATONPASS UTurnBS
	jumpifcantswitch BS_ATTACKER | ATK4F_DONT_CHECK_STATUSES FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm SetBatonPassSwitchingBit
	copybyte gBankSwitching gBattlerAttacker

BatonPassSwitchOutBS::
	copyarray gCurrentMove BACKUP_HWORD 2
	callasm ClearAttackerDidDamageOnce
	callasm ClearTargetStatFellThisTurn @;So Eject Pack doesn't activate
	openpartyscreen BS_SWITCHING FAILED
	switchoutabilities BS_SWITCHING
	waitstate
	switchhandleorder BS_SWITCHING 0x2
	returntoball BS_SWITCHING
	callasm TryRemovePrimalWeatherSwitchingBank
	getswitchedmondata BS_SWITCHING
	switchindataupdate BS_SWITCHING
	hpthresholds BS_SWITCHING
	printstring 0x3
	switchinanim BS_SWITCHING 0x1
	waitstate
	switchineffects BS_SWITCHING
	callasm ClearBatonPassSwitchingBit
	copyarray gCurrentMove BACKUP_HWORD 2
	jumpifmove MOVE_PARTINGSHOT PartingShotEndBS
	jumpifnotmove MOVE_BATONPASS BattleScript_MoveEnd3 @;U-Turn & Volt Switch
	goto BattleScript_MoveEnd

UTurnBS::
	accuracycheck BattleScript_PrintMoveMissed 0x0
	jumpifmove MOVE_PARTINGSHOT PartingShotBS
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffectwithchance
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	setbyte sMOVEEND_STATE 0x0
	moveend BS_TARGET 0x0
	
UTurnCheckSwitchBS::
	copybyte gBankSwitching gBattlerAttacker
	jumpifcantswitch BS_SWITCHING | ATK4F_DONT_CHECK_STATUSES, BattleScript_MoveEnd3
	jumpifnoviablemonsleft BS_TARGET BattleScript_MoveEnd3
	jumpifhasnohp BS_SWITCHING BattleScript_MoveEnd3
	jumpifhasnohp BS_TARGET UTurnGiveEXPBS

UTurnSwitchBS::
	copybyte sBATTLER gBattlerAttacker
	setword BATTLE_STRING_LOADER UTurnString
	printstring 0x184
	waitmessage DELAY_1SECOND
	goto BattleScript_SwitchOutAttackCheckPursuit
	
UTurnGiveEXPBS::
	setbyte sGIVEEXP_STATE 0x0
	getexp 0x0
	callasm SetSkipCertainSwitchInAbilities
	goto UTurnSwitchBS
	
BattleScript_SwitchOutAttackCheckPursuit::
	jumpifhasnohp BS_TARGET BattleScript_UTurnSwitchOutAnim
	
	setbyte sDMG_MULTIPLIER 0x2
	jumpifbattletype BATTLE_TYPE_DOUBLE BattleScript_PursuitSwitchDmgSetMultihitUTurn
	setbyte gMultiHitCounter 0x1
	goto BattleScript_PursuitSwitchDmgLoopUTurn

BattleScript_PursuitSwitchDmgSetMultihitUTurn::
	setbyte gMultiHitCounter 0x2

BattleScript_PursuitSwitchDmgLoopUTurn::
	jumpifnopursuitswitchdmg BattleScript_DoSwitchOutUTurn
	swapattackerwithtarget
	trysetdestinybondtohappen
	call BattleScript_PursuitDmgOnSwitchOutUTurn
	swapattackerwithtarget

BattleScript_DoSwitchOutUTurn::
	decrementmultihit BattleScript_PursuitSwitchDmgLoopUTurn
	
BattleScript_UTurnSwitchOutAnim::
	playanimation BS_SWITCHING B_ANIM_CALL_BACK_POKEMON 0x0
	callasm MakeSwitchingBankInvisible @;So the sprite stays hidden
	goto BatonPassSwitchOutBS
	
BattleScript_PursuitDmgOnSwitchOutUTurn::
	pause DELAY_HALFSECOND
	setbyte gForceSwitchHelper 0x0
	callasm MoldBreakerRemoveAbilitiesOnForceSwitchIn
	call STANDARD_DAMAGE
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	setbyte sMOVEEND_STATE, 0x0
	moveend 0x4 0x0
	various BS_TARGET 4
	jumpifbyte EQUALS gBattleCommunication 0x0 BattleScript_PursuitDmgOnSwitchOutUTurnRet
	setbyte sGIVEEXP_STATE 0x0
	getexp BS_TARGET
	goto BattleScript_MoveEnd
	
BattleScript_PursuitDmgOnSwitchOutUTurnRet::
	return
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
PartingShotBS::
	attackstring
	ppreduce
	setbyte sB_ANIM_TARGETS_HIT 0x0
	setbyte sSTAT_ANIM_PLAYED 0x0
	jumpifstatcanbelowered BS_TARGET STAT_ATK PartingShot_LowerAtk
	jumpifstatcanbelowered BS_TARGET STAT_SPATK PartingShot_LowerAtk
	pause 0x10
	goto PartingShot_SkipAnim
	
PartingShot_LowerAtk::
	attackanimation
	waitanimation
	setbyte sB_ANIM_TARGETS_HIT 0x1 @;So we know parting shot worked
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE

PartingShot_SkipAnim::
	setbyte FORM_COUNTER 0x0
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK STAT_ANIM_DOWN
	setstatchanger2 STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR PartingShot_LowerSpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x3 PartingShot_LowerSpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x4 BattleScript_MoveEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

PartingShot_LowerSpAtk::
	playstatchangeanimation BS_TARGET, STAT_ANIM_SPATK STAT_ANIM_DOWN
	setstatchanger2 STAT_SPATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR CheckPartingShotFail
	jumpifbyte GREATERTHAN cMULTISTRING_CHOOSER 0x2 CheckPartingShotFail
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	
CheckPartingShotFail::
	jumpifbyte EQUALS sB_ANIM_TARGETS_HIT 0x0 BattleScript_MoveEnd @;Anim didn't play means no stats were lowered
	goto UTurnCheckSwitchBS

PartingShotEndBS:
	setbyte sMOVEEND_STATE, 0x0
	moveend 0x7 0x0
	end
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

PurifyBS::
	jumpifstatus BS_TARGET,STATUS1_ANY,PurifyHeal
	goto FAILED_PRE

PurifyHeal::
	attackstringnoprotean
	ppreduce
	cureprimarystatus BS_TARGET FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER PurifyString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	updatestatusicon BS_TARGET
	tryhealhalfhealth BattleScript_AlreadyAtFullHp 0x1
	playanimation BS_ATTACKER B_ANIM_HEALING_SPARKLES 0x0
	goto RecoverHealthUpdateBS

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RoostBS::
	tryhealhalfhealth BattleScript_AlreadyAtFullHp 0x1 @;BattleScript_AlreadyAtFullHp
	callasm SetRoostFunc
	goto RecoverAnimBS
