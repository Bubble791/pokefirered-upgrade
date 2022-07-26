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

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StartedSchooling::
	call BattleScript_StartedSchoolingRet
	end2

BattleScript_StartedSchoolingRet::
	call BattleScript_AbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_WISHIWASHI_FISH 0x0
	setword BATTLE_STRING_LOADER gText_StartedSchooling
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_StoppedSchooling::
	call BattleScript_StoppedSchoolingRet
	end2

BattleScript_StoppedSchoolingRet::
	call BattleScript_AbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_WISHIWASHI_FISH 0x0
	setword BATTLE_STRING_LOADER gText_StoppedSchooling
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


BattleScript_FlowerGiftRet::
	jumpifability BS_SCRIPTING ABILITY_FLOWERGIFT DoFlowerGiftChange
	goto SkipFlowerGiftPopUp

DoFlowerGiftChange::
	call BattleScript_AbilityPopUp

SkipFlowerGiftPopUp::
	playanimation BS_SCRIPTING B_ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER TransformedString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_FlowerGift::
	call BattleScript_FlowerGiftRet
	end3

BattleScript_FlowerGiftEnd2::
	call BattleScript_FlowerGiftRet
	end2


@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MysteriousAirCurrentContinues::
	setword BATTLE_STRING_LOADER MysteriousAirCurrentContinuesString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	playanimation 0x0 B_ANIM_STRONG_WINDS_CONTINUE 0x0
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SandstormHailContinues::
	printfromtable gSandstormHailContinuesStringIds
	waitmessage DELAY_1SECOND
	playanimation2 BS_ATTACKER, sB_ANIM_ARG1, 0x0
	end2

BattleScript_WeatherDamage::
	weatherdamage
	jumpifword EQUALS gBattleMoveDamage 0x0 BattleScript_WeatherDamage_End
	printfromtable gSandstormHailDmgStringIds
	waitmessage DELAY_1SECOND
	orword HIT_MARKER, HITMARKER_x20 | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG | HITMARKER_GRUDGE
	effectivenesssound
	hitanimation BS_ATTACKER
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER, FALSE, 0x0
	bicword HIT_MARKER, HITMARKER_x20 | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG | HITMARKER_GRUDGE
BattleScript_WeatherDamage_End::
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_FogEnded::
	setword BATTLE_STRING_LOADER FogEndedString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_FogContinues::
	setword BATTLE_STRING_LOADER FogContinuesString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	playanimation 0x0 B_ANIM_FOG_CONTINUES 0x0
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SeaOfFireDamage::
	playanimation2 BS_ATTACKER sB_ANIM_ARG1 0x0
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_DoTurnDmg

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_GrassyTerrainHeal::
	playanimation BS_ATTACKER B_ANIM_GRASSY_TERRAIN_HEAL 0x0
	orword HIT_MARKER 0x100
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	setword BATTLE_STRING_LOADER GrassyTerrainHealString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AquaRing::
	playanimation BS_ATTACKER B_ANIM_AQUA_RING_HEAL 0x0
	orword HIT_MARKER 0x100
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	setword BATTLE_STRING_LOADER AquaRingHealString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LeechSeedTurnDrain::
	playanimation BS_ATTACKER, B_ANIM_LEECH_SEED_DRAIN, sB_ANIM_ARG1
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	copyword gBattleMoveDamage gHpDealt
	callasm TryManipulateDamageForLeechSeedBigRoot
	jumpifability BS_ATTACKER, ABILITY_LIQUIDOOZE, BattleScript_LeechSeedTurnPrintLiquidOoze
	manipulatedamage 0x0
	setbyte cMULTISTRING_CHOOSER, 0x3
	goto BattleScript_LeechSeedTurnPrintAndUpdateHp

BattleScript_LeechSeedTurnPrintLiquidOoze::
	setbyte cMULTISTRING_CHOOSER, 0x4

BattleScript_LeechSeedTurnPrintAndUpdateHp::
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printfromtable gLeechSeedStringIds @;gLeechSeedStringIds
	waitmessage DELAY_1SECOND
	tryfaintmon BS_ATTACKER 0x0 0x0
	tryfaintmon BS_TARGET 0x0 0x0
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PoisonHeal::
	call BattleScript_AbilityPopUp
	playanimation BS_ATTACKER B_ANIM_HEALING_SPARKLES 0x0
	orword HIT_MARKER 0x100
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring 0x4B @;STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_YawnMakesAsleep::
	statusanimation BS_EFFECT_BATTLER
	printstring STRINGID_PKMNFELLASLEEP @;STRINGID_PKMNFELLASLEEP
	waitmessage DELAY_HALFSECOND
	updatestatusicon BS_EFFECT_BATTLER
	waitstate
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MagnetRiseEnd::
	clearspecialstatusbit BS_ATTACKER STATUS3_LEVITATING
	setword BATTLE_STRING_LOADER MagnetRiseEndString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
    end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TelekinesisEnd::
	clearspecialstatusbit BS_ATTACKER STATUS3_TELEKINESIS
	setword BATTLE_STRING_LOADER TelekinesisStringEndString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
    end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_HealBlockEnd::
	setword BATTLE_STRING_LOADER HealBlockEndString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EmbargoEnd::
	setword BATTLE_STRING_LOADER EmbargoEndString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TailwindEnd::
	setword BATTLE_STRING_LOADER gText_TailwindWoreOff
	goto PrintTimerString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LuckyChantEnd::
	setword BATTLE_STRING_LOADER gText_LuckyChantWoreOff
	goto PrintTimerString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RainbowEnd::
	setword BATTLE_STRING_LOADER gText_RainbowDisappeared
	goto PrintTimerString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SeaOfFireEnd::
	setword BATTLE_STRING_LOADER gText_SeaOfFireDisappeared
	goto PrintTimerString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SwampEnd::
	setword BATTLE_STRING_LOADER gText_SwampDisappeared
	goto PrintTimerString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TrickRoomEnd::
	setword BATTLE_STRING_LOADER TrickRoomEndString
	goto PrintTimerString


BattleScript_WaterSportEnd::
	setword BATTLE_STRING_LOADER WaterSportEndString
	goto PrintTimerString


BattleScript_MudSportEnd::
	setword BATTLE_STRING_LOADER MudSportEndString
	goto PrintTimerString


BattleScript_WonderRoomEnd::
	setword BATTLE_STRING_LOADER WonderRoomEndString
	goto PrintTimerString


BattleScript_MagicRoomEnd::
	setword BATTLE_STRING_LOADER MagicRoomEndString
	goto PrintTimerString


BattleScript_GravityEnd::
	setword BATTLE_STRING_LOADER GravityEndString
	goto PrintTimerString


BattleScript_TerrainEnd::
	setbyte gTerrainType 0x0
	callasm TransferTerrainData
	playanimation 0x0 B_ANIM_LOAD_DEFAULT_BG 0x0
	setword BATTLE_STRING_LOADER TerrainEndString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	callasm TryActivateMimicry
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

PrintTimerString::
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end2
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ToxicOrb::
	statusanimation BS_EFFECT_BATTLER
	setword BATTLE_STRING_LOADER ToxicOrbString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	updatestatusicon BS_EFFECT_BATTLER
	waitstate
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_FlameOrb::
	statusanimation BS_EFFECT_BATTLER
	setword BATTLE_STRING_LOADER FlameOrbString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	updatestatusicon BS_EFFECT_BATTLER
	waitstate
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PowerConstruct::
	setword BATTLE_STRING_LOADER PresenceOfManyString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_HALFSECOND
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	playanimation BS_ATTACKER B_ANIM_ZYGARDE_CELL_SWIRL 0x0
	reloadhealthbar BS_ATTACKER
	setword BATTLE_STRING_LOADER PowerConstructCompleteString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ShieldsDownToCore::
	call BattleScript_ShieldsDownToCoreRet
	end2

BattleScript_ShieldsDownToCoreRet::
	call BattleScript_AbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER ToCoreString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_ShieldsDownToMeteor::
	call BattleScript_ShieldsDownToMeteorRet
	end2

BattleScript_ShieldsDownToMeteorRet::
	call BattleScript_AbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER ToMeteorString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MonTookFutureAttack::
	printstring STRINGID_PKMNTOOKATTACK @;STRINGID_PKMNTOOKATTACK
	waitmessage 0x10
	jumpifmove MOVE_DOOMDESIRE BattleScript_CheckDoomDesireMiss
	accuracycheck BattleScript_FutureAttackMiss MOVE_FUTURESIGHT
	goto BattleScript_CalcDamage

BattleScript_CheckDoomDesireMiss::
	accuracycheck BattleScript_FutureAttackMiss MOVE_DOOMDESIRE

BattleScript_CalcDamage::
	critcalc
	callasm TryUseGemFutureSight
	callasm FutureSightDamageCalc
	typecalc
	adjustnormaldamage2
	callasm TryActivateWeakenessBerryFutureSight
	jumpifmove MOVE_DOOMDESIRE BattleScript_FutureHitAnimDoomDesire
	playanimation BS_ATTACKER B_ANIM_FUTURE_SIGHT_HIT 0x0
	goto BattleScript_DoFutureAttackHit

BattleScript_FutureHitAnimDoomDesire::
	playanimation BS_ATTACKER B_ANIM_DOOM_DESIRE_HIT 0x0

BattleScript_DoFutureAttackHit::
	effectivenesssound
	hitanimation BS_TARGET
	waitstate
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	resultmessage
	waitmessage DELAY_1SECOND
	prefaintmoveendeffects 0x3
	faintpokemonaftermove
	ifwildbattleend BattleScript_FutureAttackEnd

BattleScript_FutureAttackEnd::
	setbyte sMOVEEND_STATE 0x0
	moveend 0x3 0x0
	setbyte gMoveResultFlags 0
	end2
	
BattleScript_FutureAttackMiss::
	pause DELAY_HALFSECOND
	setbyte gMoveResultFlags 0
	orbyte gMoveResultFlags OUTCOME_FAILED
	resultmessage
	waitmessage DELAY_1SECOND
	setbyte gMoveResultFlags 0
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@;Affected by Mist and Abilities
BattleScript_OctolockTurnDmg::
	playanimation BS_ATTACKER, B_ANIM_TURN_TRAP, sB_ANIM_ARG1
	setword BATTLE_STRING_LOADER gText_SqueezedByOctolock
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE

BattleScript_OctolockTurnDmg_Def::
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_DEF, STAT_ANIM_DOWN
	setstatchanger2 STAT_DEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_NOT_PROTECT_AFFECTED BattleScript_OctolockTurnDmgPrintDefMsg
BattleScript_OctolockTurnDmgPrintDefMsg:
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x3 BattleScript_OctolockTurnDmg_SpDef
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x4 BattleScript_OctolockTurnDmgEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

BattleScript_OctolockTurnDmg_SpDef::
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_SPDEF, STAT_ANIM_DOWN
	setstatchanger2 STAT_SPDEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_NOT_PROTECT_AFFECTED BattleScript_OctolockTurnDmgPrintSpDefMsg
BattleScript_OctolockTurnDmgPrintSpDefMsg::
	jumpifbyte GREATERTHAN cMULTISTRING_CHOOSER 0x2 BattleScript_OctolockTurnDmgEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

BattleScript_OctolockTurnDmgEnd::
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DynamaxEnd::
	callasm UpdateHPForDynamax
	waitstate
	callasm UpdateCurrentHealthForDynamaxEnd
	waitstate
	reloadhealthbar BS_SCRIPTING
	callasm SetAndTransferDontRemoveTransformSpecies
	jumpifstatus2 BS_SCRIPTING STATUS2_TRANSFORMED BattleScript_DynamaxEnd_SpecialTransformAnim
	playanimation BS_SCRIPTING B_ANIM_TRANSFORM 0x0
BattleScript_DynamaxEnd_Rejoin::
	waitanimation
	callasm ClearAndTransferDontRemoveTransformSpecies
	setword BATTLE_STRING_LOADER gText_DynamaxEnded
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end2

BattleScript_DynamaxEnd_SpecialTransformAnim::
	copybyte gBattlerAttacker sBATTLER
	copybyte TARGET_BANK sBATTLER
	playanimation BS_SCRIPTING 0xff 0x0
	goto BattleScript_DynamaxEnd_Rejoin

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LoseRaidBattle::
	playanimation BS_SCRIPTING B_ANIM_RAID_BATTLE_STORM 0x0
	playanimation BS_SCRIPTING B_DRAGON_TAIL_BLOW_AWAY_ANIM 0x0
	callasm SetScriptingBankToItsPartner
	playanimation BS_SCRIPTING B_DRAGON_TAIL_BLOW_AWAY_ANIM 0x0
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	setbyte gBattleOutcome 0x5 @;Teleported
	end2

BattleScript_LoseFrontierRaidBattle::
	setword BATTLE_STRING_LOADER gText_RaidBattleKO4
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	setbyte gBattleOutcome 0x5 @;Teleported
	end2

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Victory::
	jumpifword ANDS gBattleTypeFlags BATTLE_TYPE_TWO_OPPONENTS BeatTwoPeeps
	printstring 0x141
	goto PostBeatString
	
BeatTwoPeeps::
	setword BATTLE_STRING_LOADER BattleText_TwoInGameTrainersDefeated
	printstring STRINGID_CUSTOMSTRING

PostBeatString::
	trainerslidein 0x1
	waitstate
	jumpifword ANDS gBattleTypeFlags BATTLE_TYPE_EREADER_TRAINER BattleScript_BattleTowerEtcTrainerBattleWonSkipText @Just Pickup Calc
	printstring 0xC
	jumpifword NOTANDS gBattleTypeFlags BATTLE_TYPE_TWO_OPPONENTS CheckJumpLocForEndBattle
	callasm TrainerSlideOut+1
	waitstate
	trainerslidein 0x3
	waitstate
	setword BATTLE_STRING_LOADER TrainerBLoseString
	printstring STRINGID_CUSTOMSTRING

CheckJumpLocForEndBattle::
	jumpifword ANDS gBattleTypeFlags, BATTLE_TYPE_FRONTIER, BattleScript_BattleTowerEtcTrainerBattleWonSkipText2 @No Money Give
	jumpifword NOTANDS gBattleTypeFlags BATTLE_TYPE_TRAINER_TOWER BattleScript_LocalTrainerBattleWon2 @Give Money
	jumpifword NOTANDS gBattleTypeFlags BATTLE_TYPE_DOUBLE BattleScript_BattleTowerEtcTrainerBattleWonSkipText @Just Pickup Calc
	printstring 0x177 @Buffer Trainer Tower Win Text
	goto BattleScript_BattleTowerEtcTrainerBattleWonSkipText @Just Pickup Calc
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PrintPlayerForfeited::
	setword BATTLE_STRING_LOADER sText_PlayerForfeitedMatch
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end2
	
BattleScript_PrintPlayerForfeitedLinkBattle::
	setword BATTLE_STRING_LOADER sText_PlayerForfeitedMatch
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	flee
	waitmessage DELAY_1SECOND
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LostMultiBattleTower::
	returnopponentmon1toball BS_ATTACKER
	waitstate
	callasm ReturnOpponentMon2
	waitstate
	trainerslidein BS_ATTACKER
	waitstate
	setword BATTLE_STRING_LOADER TrainerAWinString
	printstring STRINGID_CUSTOMSTRING
	callasm TrainerSlideOut
	waitstate
	trainerslidein 0x3
	waitstate
	setword BATTLE_STRING_LOADER TrainerBVictoryString
	printstring STRINGID_CUSTOMSTRING
	flee
	waitmessage DELAY_1SECOND
	end2
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LostBattleTower::
	returnopponentmon1toball BS_ATTACKER
	waitstate
	callasm ReturnOpponentMon2
	waitstate
	trainerslidein BS_ATTACKER
	waitstate
	setword BATTLE_STRING_LOADER TrainerAWinString
	printstring STRINGID_CUSTOMSTRING
	flee
	waitmessage DELAY_1SECOND
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AskIfWantsToForfeitMatch::
	setword BATTLE_STRING_LOADER sText_QuestionForfeitMatch
	printstring STRINGID_CUSTOMSTRING
	callasm DisplayForfeitYesNoBox
	callasm HandleForfeitYesNoBox
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RanAwayUsingMonAbility::
	printstring 0x130 @;STRINGID_EMPTYSTRING3
	call BattleScript_AbilityPopUp
	printstring 0xDF @;STRINGID_GOTAWAYSAFELY
	waitmessage DELAY_1SECOND
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaidMonRanAway::
	setword BATTLE_STRING_LOADER gText_RaidMonRanAway
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaidMonEscapeBall::
	printfromtable gBallEscapeStringIds @;gBallEscapeStringIds
	waitmessage DELAY_1SECOND
	copybyte sBATTLER TARGET_BANK
	callasm MakeScriptingBankInvisible
	setword BATTLE_STRING_LOADER gText_RaidMonRanAway
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	setbyte gBattleOutcome 0x1 @B_OUTCOME_WON
	finishaction

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PrintCustomStringEnd2::
	call BattleScript_PrintCustomString
	end2

BattleScript_PrintCustomStringEnd3::
	call BattleScript_PrintCustomString
	end3

