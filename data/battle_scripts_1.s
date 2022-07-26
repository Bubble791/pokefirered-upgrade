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

gBattleScriptsForMoveEffects::
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectSleep
	.4byte BattleScript_EffectPoisonHit
	.4byte BattleScript_EffectAbsorb
	.4byte BattleScript_EffectBurnHit
	.4byte BattleScript_EffectFreezeHit
	.4byte BattleScript_EffectParalyzeHit
	.4byte BattleScript_EffectExplosion
	.4byte BattleScript_EffectDreamEater
	.4byte BattleScript_EffectMirrorMove
	.4byte BattleScript_EffectAttackUp
	.4byte BattleScript_EffectDefenseUp
	.4byte BattleScript_EffectSpeUp
	.4byte BattleScript_EffectSpecialAttackUp
	.4byte BattleScript_EffectRaiseUserSpDef1
	.4byte BattleScript_EffectRaiseUserAcc1
	.4byte BattleScript_EffectEvasionUp
	.4byte BattleScript_EffectNeverMiss
	.4byte BattleScript_EffectAttackDown
	.4byte BattleScript_EffectDefenseDown
	.4byte BattleScript_EffectSpeedDown
	.4byte BattleScript_EffectLowerTargetSpAtk1
	.4byte BattleScript_EffectLowerTargetSpDef1
	.4byte BattleScript_EffectAccuracyDown
	.4byte BattleScript_EffectEvasionDown
	.4byte BattleScript_EffectHaze
	.4byte BattleScript_EffectBide
	.4byte BattleScript_EffectRampage
	.4byte BattleScript_EffectRoar
	.4byte BattleScript_EffectMultiHit
	.4byte BattleScript_EffectConversion
	.4byte BattleScript_EffectFlinchHit
	.4byte BattleScript_EffectRecover
	.4byte BattleScript_EffectToxic
	.4byte BattleScript_EffectPayDay
	.4byte BattleScript_EffectLightScreen
	.4byte BattleScript_EffectTriAttack
	.4byte BattleScript_EffectRest
	.4byte BattleScript_EffectOHKO
	.4byte BattleScript_EffectRazorWind
	.4byte BattleScript_EffectSuperFang
	.4byte BattleScript_EffectDragonRage
	.4byte BattleScript_EffectTrap
	.4byte BattleScript_EffectHighCritRatio
	.4byte BattleScript_EffectDoubleHit
	.4byte BattleScript_EffectRecoilIfMiss
	.4byte BattleScript_EffectMist
	.4byte BattleScript_EffectFocusEnergy
	.4byte BattleScript_EffectRecoil
	.4byte BattleScript_EffectConfuse
	.4byte BattleScript_EffectAttackUp2
	.4byte BattleScript_EffectDefenseUp2
	.4byte BattleScript_EffectSpeedUp2
	.4byte BattleScript_EffectSpecialAttackUp2
	.4byte BattleScript_EffectSpecialDefenseUp2
	.4byte BattleScript_EffectRaiseUserAcc2
	.4byte BattleScript_EffectRaiseUserEvsn2
	.4byte BattleScript_EffectTransform
	.4byte BattleScript_EffectAttackDown2
	.4byte BattleScript_EffectDefenseDown2
	.4byte BattleScript_EffectSpeedDown2
	.4byte BattleScript_EffectLowerTargetSpAtk2
	.4byte BattleScript_EffectSpecialDefenseDown2
	.4byte BattleScript_EffectLowerTargetAcc2
	.4byte BattleScript_EffectLowerTargetEvsn2
	.4byte BattleScript_EffectReflect
	.4byte BattleScript_EffectPoison
	.4byte BattleScript_EffectParalyze
	.4byte BattleScript_EffectAttackDownHit
	.4byte BattleScript_EffectDefenseDownHit
	.4byte BattleScript_EffectSpeedDownHit
	.4byte BattleScript_EffectSpecialAttackDownHit
	.4byte BattleScript_EffectSpecialDefenseDownHit
	.4byte BattleScript_EffectAccuracyDownHit
	.4byte BattleScript_EffectLowerTargetEvsn1Chance
	.4byte BattleScript_EffectSkyAttack
	.4byte BattleScript_EffectConfuseHit
	.4byte BattleScript_EffectTwineedle
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectSubstitute
	.4byte BattleScript_EffectRecharge
	.4byte BattleScript_EffectRage
	.4byte BattleScript_EffectMimic
	.4byte BattleScript_EffectMetronome
	.4byte BattleScript_EffectLeechSeed
	.4byte BattleScript_EffectSplash
	.4byte BattleScript_EffectDisable
	.4byte BattleScript_EffectLevelDamage
	.4byte BattleScript_EffectPsywave
	.4byte BattleScript_EffectCounter
	.4byte BattleScript_EffectEncore
	.4byte BattleScript_EffectPainSplit
	.4byte BattleScript_EffectSnore
	.4byte BattleScript_EffectConversion2
	.4byte BattleScript_EffectLockOn
	.4byte BattleScript_EffectSketch
	.4byte BattleScript_EffectRaiseAttackerSpd1Chance
	.4byte BattleScript_EffectSleepTalk
	.4byte BattleScript_EffectDestinyBond
	.4byte BattleScript_EffectFlail
	.4byte BattleScript_EffectSpite
	.4byte BattleScript_EffectFalseSwipe
	.4byte BattleScript_EffectHealBell
	.4byte BattleScript_EffectPriorityMove
	.4byte BattleScript_EffectTripleKick
	.4byte BattleScript_EffectThief
	.4byte BattleScript_EffectMeanLook
	.4byte BattleScript_EffectNightmare
	.4byte BattleScript_EffectMinimize
	.4byte BattleScript_EffectCurse
	.4byte BattleScript_EffectRaiseAttackerSpAtk1Chance
	.4byte BattleScript_EffectProtect
	.4byte BattleScript_EffectSpikes
	.4byte BattleScript_EffectForesight
	.4byte BattleScript_EffectPerishSong
	.4byte BattleScript_EffectSandstorm
	.4byte BattleScript_EffectProtect
	.4byte BattleScript_EffectRollout
	.4byte BattleScript_EffectSwagger
	.4byte BattleScript_EffectFuryCutter
	.4byte BattleScript_EffectAttract
	.4byte BattleScript_EffectReturn
	.4byte BattleScript_EffectPresent
	.4byte BattleScript_EffectReturn
	.4byte BattleScript_EffectSafeguard
	.4byte BattleScript_EffectThawHit
	.4byte BattleScript_EffectMagnitude
	.4byte BattleScript_EffectBatonPass
	.4byte BattleScript_EffectPursuit
	.4byte BattleScript_EffectRapidSpin
	.4byte BattleScript_EffectSonicboom
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectMorningSun
	.4byte BattleScript_EffectMorningSun
	.4byte BattleScript_EffectMorningSun
	.4byte BattleScript_EffectAttackerRaiseDef2Chance
	.4byte BattleScript_EffectRainDance
	.4byte BattleScript_EffectSunnyDay
	.4byte BattleScript_EffectDefenseUpHit
	.4byte BattleScript_EffectAttackUpHit
	.4byte BattleScript_EffectAllStatsUpHit
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectBellyDrum
	.4byte BattleScript_EffectPsychUp
	.4byte BattleScript_EffectMirrorCoat
	.4byte BattleScript_EffectSkullBash
	.4byte BattleScript_EffectTwister
	.4byte BattleScript_EffectEarthquake
	.4byte BattleScript_EffectFutureSight
	.4byte BattleScript_EffectGust
	.4byte BattleScript_EffectFlinchMinimizeHit
	.4byte BattleScript_EffectSolarbeam
	.4byte BattleScript_EffectThunder
	.4byte BattleScript_EffectTeleport
	.4byte BattleScript_EffectBeatUp
	.4byte BattleScript_EffectFly
	.4byte BattleScript_EffectDefenseCurl
	.4byte BattleScript_EffectSoftboiled
	.4byte BattleScript_EffectFakeOut
	.4byte BattleScript_EffectUproar
	.4byte BattleScript_EffectStockpile
	.4byte BattleScript_EffectSpitUp
	.4byte BattleScript_EffectSwallow
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectHail
	.4byte BattleScript_EffectTorment
	.4byte BattleScript_EffectFlatter
	.4byte BattleScript_EffectSetBurn
	.4byte BattleScript_EffectMemento
	.4byte BattleScript_EffectFacade
	.4byte BattleScript_EffectFocusPunch
	.4byte BattleScript_EffectSmellingsalt
	.4byte BattleScript_EffectFollowMe
	.4byte BattleScript_EffectNaturePower
	.4byte BattleScript_EffectCharge
	.4byte BattleScript_EffectTaunt
	.4byte BattleScript_EffectHelpingHand
	.4byte BattleScript_EffectTrick
	.4byte BattleScript_EffectRolePlay
	.4byte BattleScript_EffectWish
	.4byte BattleScript_EffectAssist
	.4byte BattleScript_EffectIngrain
	.4byte BattleScript_EffectSuperpower
	.4byte BattleScript_EffectMagicCoat
	.4byte BattleScript_EffectRecycle
	.4byte BattleScript_EffectRevenge
	.4byte BattleScript_EffectBrickBreak
	.4byte BattleScript_EffectYawn
	.4byte BattleScript_EffectKnockOff
	.4byte BattleScript_EffectEndeavor
	.4byte BattleScript_EffectEruption
	.4byte BattleScript_EffectSkillSwap
	.4byte BattleScript_EffectImprison
	.4byte BattleScript_EffectRefresh
	.4byte BattleScript_EffectGrudge
	.4byte BattleScript_EffectSnatch
	.4byte BattleScript_EffectLowKick
	.4byte BattleScript_EffectSecretPower
	.4byte BattleScript_EffectDoubleEdge
	.4byte BattleScript_EffectTeeterDance
	.4byte BattleScript_EffectTeeterDance
	.4byte BattleScript_EffectMudSport
	.4byte BattleScript_EffectPoisonFang
	.4byte BattleScript_EffectWeatherBall
	.4byte BattleScript_EffectOverheat
	.4byte BattleScript_EffectTickle
	.4byte BattleScript_EffectCosmicPower
	.4byte BattleScript_EffectExtremeEvoBoost
	.4byte BattleScript_EffectBulkUp
	.4byte BattleScript_EffectBadPoisonChance
	.4byte BattleScript_EffectWaterSport
	.4byte BattleScript_EffectCalmMind
	.4byte BattleScript_EffectDragonDance
	.4byte BattleScript_EffectStatSwapSplitters
	.4byte BS_214_Blank
	.4byte BS_214_Blank @215
	.4byte BS_214_Blank @216
	.4byte BS_214_Blank @217
	.4byte BS_218_MeFirst
	.4byte BS_219_DestroyBerry
	.4byte BS_220_NaturalGift
	.4byte BS_221_SmackDown
	.4byte BS_222_DamageResetTargetStatChanges
	.4byte BS_223_RelicSong
	.4byte BS_224_Blank
	.4byte BS_225_Blank
	.4byte BS_226_Terrain
	.4byte BS_227_Pledges
	.4byte BS_228_FieldEffects
	.4byte BS_229_Fling
	.4byte BS_230_Feint
	.4byte BS_231_AttackBlockers
	.4byte BS_232_TypeChangers
	.4byte BS_233_HealTarget
	.4byte BS_234_TopsyTurvyElectrify
	.4byte BS_235_FairyLockHappyHourCelebrateHoldHands
	.4byte BS_236_InstructAfterYouQuash
	.4byte BS_237_SuckerPunch
	.4byte BS_238_Blank
	.4byte BS_239_TeamEffectsAndMagnetRise
	.4byte BS_240_Camouflage
	.4byte BS_241_FlameBurst
	.4byte BS_242_LastResortSkyDrop
	.4byte BS_243_DamageSetTerrain
	.4byte BS_244_Blank
	.4byte BS_245_Blank
	.4byte BS_246_Blank
	.4byte BS_247_Blank
	.4byte BS_248_Blank
	.4byte BS_249_Blank
	.4byte BS_250_Blank
	.4byte BS_251_Blank
	.4byte BS_252_Blank
	.4byte BS_253_MaxMove
	.4byte BS_254_Synchronoise

BattleScript_EffectHit::
	jumpifnotmove MOVE_SURF, BattleScript_HitFromAtkCanceler
	jumpifnostatus3 BS_TARGET, STATUS3_UNDERWATER, BattleScript_HitFromAtkCanceler
	orword gHitMarker, HITMARKER_IGNORE_UNDERWATER
	setbyte sDMG_MULTIPLIER, 2
BattleScript_HitFromAtkCanceler::
	attackcanceler
BattleScript_HitFromAccCheck::
	accuracycheck BattleScript_PrintMoveMissed, ACC_CURR_MOVE
BattleScript_HitFromAtkString::
	attackstring
	ppreduce
BattleScript_HitFromCritCalc::
	critcalc
	damagecalc
	typecalc
	adjustnormaldamage
BattleScript_HitFromAtkAnimation::
	attackanimation
	waitanimation
	effectivenesssound
	hitanimation BS_TARGET
	waitstate
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	critmessage
	waitmessage 0x40
Resultmessage::
	resultmessage
	waitmessage 0x40
	seteffectwithchance
BS_MOVE_FAINT::
	tryfaintmon BS_TARGET, 0, NULL
BattleScript_MoveEnd::
	setbyte sMOVEEND_STATE, 0
BattleScript_Atk49::
	moveend 0, 0
BattleScript_MoveEnd3::
	end

BattleScript_MakeMoveMissed::
	orbyte gMoveResultFlags, MOVE_RESULT_MISSED
BattleScript_PrintMoveMissed::
	attackstring
	ppreduce
BattleScript_MoveMissedPause::
	pause 0x20
BattleScript_MoveMissed::
	effectivenesssound
	resultmessage
BattleScript_MoveMissed3::
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectSleep::
	attackcanceler
	attackstring @;Protean always activates!
	ppreduce
	jumpifbehindsubstitute BS_TARGET FAILED
	trysetsleep BS_TARGET BS_StatusMoveFail
	accuracycheck BattleScript_MoveMissedPause 0x0
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_SLEEP
	seteffectprimary
	goto BattleScript_MoveEnd

BS_StatusMoveFail::
	pause DELAY_HALFSECOND
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_AlreadyAsleep::
	pause 0x20
	printstring STRINGID_PKMNALREADYASLEEP
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_WasntAffected::
	pause 0x20
	printstring STRINGID_PKMNWASNTAFFECTED
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_CantMakeAsleep::
	pause 0x20
	printfromtable gUproarAwakeStringIds
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectPoisonHit::
	setmoveeffect MOVE_EFFECT_POISON
	goto BattleScript_HitFromAtkCanceler

BattleScript_AbsorbUpdateHp::
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	jumpifmovehadnoeffect BattleScript_AbsorbTryFainting
	printfromtable gLeechSeedDrainStringIds
	waitmessage 0x40
BattleScript_AbsorbTryFainting::
	tryfaintmon BS_ATTACKER, 0, NULL
	tryfaintmon BS_TARGET, 0, NULL
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BattleScript_EffectAbsorb::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	jumpifmove MOVE_STRENGTHSAP StrengthSapBS
	call STANDARD_DAMAGE
	negativedamage

DrainHPBSP2::
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	jumpifability BS_TARGET ABILITY_LIQUIDOOZE BattleScript_AbsorbLiquidOoze
	setbyte cMULTISTRING_CHOOSER 0x0
	goto BattleScript_AbsorbUpdateHp
	
StrengthSapBS::
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	jumpifstat BS_TARGET EQUALS STAT_ATK STAT_MIN FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	callasm StrengthSapFunc
	setstatchanger2 STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR DrainHPBSP2
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 DrainHPBSP2
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	goto DrainHPBSP2

BattleScript_AbsorbLiquidOoze::
	jumpifmovehadnoeffect BS_MOVE_FAINT
	copybyte sBATTLER TARGET_BANK
	call BattleScript_AbilityPopUp
	manipulatedamage 0x0 @;ATK80_DMG_CHANGE_SIGN
	setbyte cMULTISTRING_CHOOSER, 0x1
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printfromtable gLeechSeedDrainStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	tryfaintmon BS_ATTACKER 0x0 0x0
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectBurnHit::
	setmoveeffect MOVE_EFFECT_BURN
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectFreezeHit::
	setmoveeffect MOVE_EFFECT_FREEZE
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectParalyzeHit::
	setmoveeffect MOVE_EFFECT_PARALYSIS
	goto BattleScript_HitFromAtkCanceler

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectExplosion::
	attackcanceler
	attackstring
	ppreduce
	faintifabilitynotdamp
	setatkhptozero
	waitstate
	accuracycheck BattleScript_PrintMoveMissed 0x0
	goto BattleScript_HitFromCritCalc

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectDreamEater::
	attackcanceler
	jumpifstatus BS_TARGET STATUS1_SLEEP EatTheDreams
	jumpifability BS_TARGET ABILITY_COMATOSE EatTheDreams
	attackstring
	ppreduce
	waitmessage DELAY_1SECOND
	goto BattleScript_WasntAffected @;Target wasn't affected

EatTheDreams::
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	negativedamage
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	jumpifability BS_TARGET ABILITY_LIQUIDOOZE BattleScript_AbsorbLiquidOoze
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring 0x3C
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectMirrorMove::
	attackcanceler
	jumpifmove MOVE_COPYCAT CopycatBS
	attackstringnoprotean
	pause DELAY_1SECOND
	trymirrormove
	ppreduce
	orbyte gMoveResultFlags OUTCOME_FAILED
	printstring 0xE7
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

CopycatBS::
	attackstringnoprotean
	ppreduce
	callasm CopycatFunc
	attackanimation
	waitanimation
	setbyte sB_ANIM_TURN 0x0
	setbyte sB_ANIM_TARGETS_HIT 0x0
	jumptocalledmove 0x1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectAttackUp::
	setstatchanger2 STAT_ATK | INCREASE_1
	jumpifmove MOVE_HONECLAWS HoneClawsBS
	jumpifnotbattletype BATTLE_TYPE_DOUBLE BattleScript_EffectStatUp
	jumpifmove MOVE_HOWL HowlBS @;Only difference is that it raises partner's attack in Doubles too
	goto BattleScript_EffectStatUp

HoneClawsBS:
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BS_TARGET LESSTHAN STAT_ATK STAT_MAX HoneClaws_Atk
	jumpifstat BS_TARGET EQUALS STAT_ACC STAT_MAX BattleScript_CantRaiseMultipleStats

HoneClaws_Atk:
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_ACC, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	@;setstatchanger STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN HoneClaws_Acc
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 HoneClaws_Acc
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

HoneClaws_Acc:
	setstatchanger2 STAT_ACC | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

HowlBS:
	attackcanceler
	attackstring
	ppreduce
	statbuffchange STAT_ATTACKER | STAT_BS_PTR HowlTryPartnerBS
	jumpifbyte NOTEQUALS cMULTISTRING_CHOOSER 0x2 HowlAttackAnimation
	pause DELAY_HALFSECOND
	setbyte sB_ANIM_TARGETS_HIT 0x0
	goto HowlPrintAttackerString

HowlAttackAnimation:
	attackanimation
	waitanimation
	setbyte sB_ANIM_TARGETS_HIT 0x1
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER B_ANIM_STATS_CHANGE sB_ANIM_ARG1
HowlPrintAttackerString:
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

HowlTryPartnerBS:
	callasm SetTargetPartner
	jumpifbehindsubstitute BS_TARGET BattleScript_MoveEnd
	jumpifhasnohp BS_TARGET BattleScript_MoveEnd
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_MoveEnd
	jumpifbyte NOTEQUALS cMULTISTRING_CHOOSER 0x2 HowlSecondAttackAnimation
	pause DELAY_HALFSECOND
	swapattackerwithtarget @;So the proper string is shown
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND	
	swapattackerwithtarget
	goto BattleScript_MoveEnd

HowlSecondAttackAnimation:
	bicbyte gMoveResultFlags, MOVE_RESULT_NO_EFFECT
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
HowlPrintPartnerString:
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectDefenseUp::
	attackcanceler
	attackstring
	ppreduce
	setstatchanger2 STAT_DEF | INCREASE_1
	jumpifmove MOVE_FLOWERSHIELD FlowerShieldBS
	jumpifmove MOVE_MAGNETICFLUX MagneticFluxBS
	jumpifmove MOVE_AROMATICMIST RaiseUserDef1_AromaticMist
	goto BattleScript_EffectStatUpAfterStart @0x81D6BA1

FlowerShieldBS:
	pause DELAY_HALFSECOND
	setbyte gBattleCommunication 0x0
BattleScript_FlowerShieldLoop:
	flowershieldlooper 0x0 BattleScript_FlowerShieldStatBoost BattleScript_FlowerShieldDidntWork
	goto BattleScript_MoveEnd

BattleScript_FlowerShieldStatBoost:
	setstatchanger2 STAT_DEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_FlowerShieldLoop
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_FlowerShieldCantRaiseStats
	attackanimation
	waitanimation
	setbyte sB_ANIM_TARGETS_HIT 0x1
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_FlowerShieldLoop

BattleScript_FlowerShieldCantRaiseStats:
	swapattackerwithtarget @;So the correct string prints
	printfromtable gStatUpStringIds
	swapattackerwithtarget
	waitmessage DELAY_1SECOND
	goto BattleScript_FlowerShieldLoop

BattleScript_FlowerShieldDidntWork:
	printfromtable gFlowerShieldStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_FlowerShieldLoop

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MagneticFluxBS:
	pause DELAY_HALFSECOND
	setbyte gBattleCommunication 0x0
BattleScript_MagneticFluxLoop:
	flowershieldlooper 0x1 BattleScript_MagneticFluxStatBoost BattleScript_MagneticFluxDidntWork
	goto BattleScript_MoveEnd

BattleScript_MagneticFluxStatBoost:
	jumpifstatcanberaised BS_TARGET STAT_DEF MagneticFlux_Def
	jumpifstatcanberaised BS_TARGET STAT_SPDEF MagneticFlux_Def
	goto MagneticFluxStatsWontGoHigher

MagneticFlux_Def:
	attackanimation
	waitanimation
	setbyte sB_ANIM_TARGETS_HIT 0x1
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_TARGET, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_DEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR MagneticFlux_SpDef
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 MagneticFlux_SpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

MagneticFlux_SpDef:
	setstatchanger2 STAT_SPDEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_MagneticFluxLoop
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MagneticFluxLoop
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MagneticFluxLoop

MagneticFluxStatsWontGoHigher:
	setbyte cMULTISTRING_CHOOSER 0x3
	swapattackerwithtarget @;So the correct string plays
	printfromtable gFlowerShieldStringIds
	swapattackerwithtarget
	waitmessage DELAY_1SECOND
	goto BattleScript_MagneticFluxLoop

BattleScript_MagneticFluxDidntWork:
	printfromtable gFlowerShieldStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MagneticFluxLoop
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RaiseUserDef1_AromaticMist:
	jumpifbyte NOTANDS gBattleTypeFlags BATTLE_TYPE_DOUBLE FAILED
	callasm SetTargetPartner
	jumpifstatus3condition EQUALS, STATUS3_SEMI_INVULNERABLE, 0x0, FAILED
	jumpifhasnohp BS_TARGET FAILED
	jumpifprotectedbycraftyshield BS_TARGET FAILED
	setbyte sSTAT_ANIM_PLAYED 0x0
	jumpifstatcanberaised BS_TARGET STAT_SPDEF AromaticMistRaiseSpDef
	goto FAILED
	
AromaticMistRaiseSpDef:
	attackanimation
	waitanimation
	setstatchanger2 STAT_SPDEF | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR FAILED
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 FAILED
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BattleScript_EffectSpeUp::
	setstatchanger2 STAT_SPEED | INCREASE_1
	goto BattleScript_EffectStatUp
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectSpecialAttackUp::
	attackcanceler
	attackstring
	ppreduce
	setstatchanger2 STAT_SPATK | INCREASE_1
	jumpifmove MOVE_GROWTH GrowthBS
	jumpifmove MOVE_WORKUP WorkUpBS
	jumpifmove MOVE_ROTOTILLER RototillerBS
	jumpifmove MOVE_GEARUP GearUpBS
	goto BattleScript_EffectStatUpAfterStart @0x81D6BA1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GrowthBS::
	jumpifstat BS_ATTACKER LESSTHAN STAT_ATK STAT_MAX Growth_RaiseAtk
	jumpifstat BS_ATTACKER EQUALS STAT_SPATK STAT_MAX BattleScript_CantRaiseMultipleStats

Growth_RaiseAtk::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_ATK | INCREASE_1
	callasm ModifyGrowthInSun
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN Growth_RaiseSpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 Growth_RaiseSpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

Growth_RaiseSpAtk::
	setstatchanger2 STAT_SPATK | INCREASE_1
	callasm ModifyGrowthInSun
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

WorkUpBS::
	jumpifstat BS_ATTACKER LESSTHAN STAT_ATK STAT_MAX WorkUp_RaiseAtk
	jumpifstat BS_ATTACKER EQUALS STAT_SPATK STAT_MAX BattleScript_CantRaiseMultipleStats

WorkUp_RaiseAtk::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN WorkUp_RaiseSpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 WorkUp_RaiseSpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

WorkUp_RaiseSpAtk::
	setstatchanger2 STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RototillerBS::
	pause DELAY_HALFSECOND
	setbyte gBattleCommunication 0x0
BattleScript_RototillerLoop::
	flowershieldlooper 0x0 BattleScript_RototillerStatBoost BattleScript_RototillerDidntWork
	goto BattleScript_MoveEnd

BattleScript_RototillerStatBoost::
	jumpifstatcanberaised BS_TARGET STAT_ATK Rototiller_Atk
	jumpifstatcanberaised BS_TARGET STAT_SPATK Rototiller_Atk
	goto RototillerStatsWontGoHigher

Rototiller_Atk::
	attackanimation
	waitanimation
	setbyte sB_ANIM_TARGETS_HIT 0x1
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_ATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR Rototiller_SpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 Rototiller_SpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	
Rototiller_SpAtk::
	setstatchanger2 STAT_SPATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_RototillerLoop
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_RototillerLoop
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_RototillerLoop

RototillerStatsWontGoHigher::
	setbyte cMULTISTRING_CHOOSER 0x3
	swapattackerwithtarget @;So the correct string plays
	printfromtable gFlowerShieldStringIds
	swapattackerwithtarget
	waitmessage DELAY_1SECOND
	goto BattleScript_RototillerLoop

BattleScript_RototillerDidntWork::
	printfromtable gFlowerShieldStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_RototillerLoop

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GearUpBS::
	pause DELAY_HALFSECOND
	setbyte gBattleCommunication 0x0
BattleScript_GearUpLoop::
	flowershieldlooper 0x1 BattleScript_GearUpStatBoost BattleScript_GearUpDidntWork
	goto BattleScript_MoveEnd

BattleScript_GearUpStatBoost::
	jumpifstatcanberaised BS_TARGET STAT_ATK GearUp_Atk
	jumpifstatcanberaised BS_TARGET STAT_SPATK GearUp_Atk
	goto GearUpStatsWontGoHigher

GearUp_Atk::
	attackanimation
	waitanimation
	setbyte sB_ANIM_TARGETS_HIT 0x1
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_ATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR GearUp_SpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 GearUp_SpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	
GearUp_SpAtk::
	setstatchanger2 STAT_SPATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_GearUpLoop
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_GearUpLoop
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_GearUpLoop

GearUpStatsWontGoHigher::
	setbyte cMULTISTRING_CHOOSER 0x3
	swapattackerwithtarget @;So the correct string plays
	printfromtable gFlowerShieldStringIds
	swapattackerwithtarget
	waitmessage DELAY_1SECOND
	goto BattleScript_GearUpLoop

BattleScript_GearUpDidntWork::
	printfromtable gFlowerShieldStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_GearUpLoop

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BattleScript_EffectRaiseUserSpDef1::
	setstatchanger2 STAT_SPDEF | INCREASE_1
	goto BattleScript_EffectStatUp

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BattleScript_EffectRaiseUserAcc1::
	setstatchanger2 STAT_ACC | INCREASE_1
	goto BattleScript_EffectStatUp

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectEvasionUp::
	jumpifmove MOVE_ACUPRESSURE AcupressureBS
	setstatchanger2 STAT_EVASION | INCREASE_1
	goto BattleScript_EffectStatUp

AcupressureBS:
	attackcanceler
	attackstring
	ppreduce
	callasm AcupressureFunc
	attackanimation
	waitanimation
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	setgraphicalstatchangevalues
	playanimation BS_TARGET 0x1 sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectNeverMiss::
	goto BattleScript_HitFromAtkCanceler

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectStatUp::
	attackcanceler
BattleScript_EffectStatUpAfterAtkCanceler::
	attackstring
	ppreduce
BattleScript_EffectStatUpAfterStart::
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_StatUpEnd
	jumpifbyte CMP_NOT_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_StatUpAttackAnim
BattleScript_EffectStatUpAfterStart3::
	pause 0x20
	goto BattleScript_StatUpPrintString

BattleScript_StatUpAttackAnim::
	attackanimation
	waitanimation
BattleScript_StatUpDoAnim::
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
BattleScript_StatUpPrintString::
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_StatUpEnd::
	goto BattleScript_MoveEnd

BattleScript_StatUp::
	playanimation BS_EFFECT_BATTLER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage 0x40
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BattleScript_EffectAttackDown::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	attackstring
	ppreduce
	accuracycheck BattleScript_MoveMissedPause 0x0
	setbyte sSTAT_ANIM_PLAYED 0x0
	jumpifmove MOVE_PLAYNICE PlayNiceBS
	jumpifmove MOVE_NOBLEROAR PlayNiceBS
	jumpifmove MOVE_TEARFULLOOK PlayNiceBS
	jumpifmove MOVE_VENOMDRENCH VenomDrenchBS
	setstatchanger2 STAT_ATK | DECREASE_1
	goto BattleScript_EffectStatDown2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

PlayNiceBS::
	jumpifstatcanbelowered BS_TARGET STAT_ATK PlayNice_Atk
	jumpifstatcanbelowered BS_TARGET STAT_SPATK PlayNice_Atk
	pause 0x10
	goto PlayNice_SkipAnim

PlayNice_Atk::
	attackanimation
	waitanimation
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE
	
PlayNice_SkipAnim::
	setbyte FORM_COUNTER 0x0
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK STAT_ANIM_DOWN
	setstatchanger2 STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR PlayNice_SpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x3 PlayNice_SpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x4 BattleScript_MoveEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

PlayNice_SpAtk::
	playstatchangeanimation BS_TARGET, STAT_ANIM_SPATK STAT_ANIM_DOWN
	setstatchanger2 STAT_SPATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_MoveEnd
	jumpifbyte GREATERTHAN cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

VenomDrenchBS::
	jumpifstatus BS_TARGET, STATUS1_POISON | STATUS1_TOXIC_POISON, DoVenomDrench
	goto BattleScript_NotAffected

DoVenomDrench::
	jumpifstatcanbelowered BS_TARGET STAT_ATK VenomDrench_Atk
	jumpifstatcanbelowered BS_TARGET STAT_SPATK VenomDrench_Atk
	jumpifstatcanbelowered BS_TARGET STAT_SPEED VenomDrench_Atk
	pause 0x10
	goto VenomDrench_SkipAnim

VenomDrench_Atk::
	attackanimation
	waitanimation
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK | STAT_ANIM_SPD, STAT_ANIM_DOWN | STAT_ANIM_ONLY_TRIPLE
	
VenomDrench_SkipAnim::
	setbyte FORM_COUNTER 0x0
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPD, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK, STAT_ANIM_DOWN
	setstatchanger2 STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR VenomDrench_SpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x3 VenomDrench_SpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x4 BattleScript_MoveEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

VenomDrench_SpAtk::
	playstatchangeanimation BS_TARGET, STAT_ANIM_SPATK | STAT_ANIM_SPD, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE
	playstatchangeanimation BS_TARGET, STAT_ANIM_SPATK, STAT_ANIM_DOWN
	setstatchanger2 STAT_SPATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR VenomDrench_Spd
	jumpifbyte GREATERTHAN cMULTISTRING_CHOOSER 0x2 VenomDrench_Spd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

VenomDrench_Spd::
	playstatchangeanimation BS_TARGET, STAT_ANIM_SPD, STAT_ANIM_DOWN
	setstatchanger2 STAT_SPEED| DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_MoveEnd
	jumpifbyte GREATERTHAN cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectDefenseDown::
	setstatchanger2 STAT_DEF | DECREASE_1
	goto BattleScript_EffectStatDown

BattleScript_EffectSpeedDown::
	setstatchanger2 STAT_SPEED| DECREASE_1
	jumpifmove MOVE_TARSHOT TarShotBS
	goto BattleScript_EffectStatDown

TarShotBS::
	jumpifcounter BS_TARGET TAR_SHOT_TIMERS NOTEQUALS 0 BattleScript_EffectStatDown @;Already set
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_PauseAndJumpToStatChangeTargetFail
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x3 BattleScript_MoveEnd
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	setcounter BS_TARGET TAR_SHOT_TIMERS 1
	setword BATTLE_STRING_LOADER gText_TarShotAffected
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_PauseAndJumpToStatChangeTargetFail::
	pause 0x20
	goto BattleScript_StatDownPrintString

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectLowerTargetSpAtk1::
	setstatchanger2 STAT_SPATK | DECREASE_1
	goto BattleScript_EffectStatDown

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectLowerTargetSpDef1::
	setstatchanger2 STAT_SPDEF | DECREASE_1
	goto BattleScript_EffectStatDown

BattleScript_EffectAccuracyDown::
	setstatchanger STAT_ACC, 1, TRUE
	goto BattleScript_EffectStatDown

BattleScript_EffectEvasionDown::
	setstatchanger STAT_EVASION, 1, TRUE
BattleScript_EffectStatDown::
	attackcanceler
	jumpifstatus2 BS_TARGET, STATUS2_SUBSTITUTE, BattleScript_ButItFailedAtkStringPpReduce
	accuracycheck BattleScript_PrintMoveMissed, ACC_CURR_MOVE
	attackstring
	ppreduce
BattleScript_EffectStatDown2::
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_StatDownEnd
	jumpifbyte CMP_LESS_THAN, cMULTISTRING_CHOOSER, 2, BattleScript_StatDownDoAnim
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 3, BattleScript_StatDownEnd
	pause 0x20
	goto BattleScript_StatDownPrintString

BattleScript_StatDownDoAnim::
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
BattleScript_StatDownPrintString::
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_StatDownEnd::
	goto BattleScript_MoveEnd

BattleScript_StatDown::
	playanimation BS_EFFECT_BATTLER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage 0x40
	return

BattleScript_EffectHaze::
	attackcanceler
	attackstring
	ppreduce
	attackanimation
	waitanimation
	normalisebuffs
	printstring STRINGID_STATCHANGESGONE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectBide::
	attackcanceler
	attackstring
	ppreduce
	attackanimation
	waitanimation
	orword gHitMarker, HITMARKER_CHARGING
	setbide
	goto BattleScript_MoveEnd

BattleScript_EffectRampage::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed, ACC_CURR_MOVE
	attackstring
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_EffectRampage2
	ppreduce
BattleScript_EffectRampage2::
	confuseifrepeatingattackends
	goto BattleScript_HitFromCritCalc

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectRoar::
	attackcanceler
	jumpifmove MOVE_DRAGONTAIL DragonTailBS
	jumpifmove MOVE_CIRCLETHROW DragonTailBS

RoarBS::
	attackstring @;Still activates Protean even if move fails
	ppreduce
	jumpifability BS_TARGET ABILITY_SUCTIONCUPS BattleScript_AbilityPreventsPhasingOut2
	jumpifstatus3condition BS_TARGET STATUS3_ROOTED 0x0 BattleScript_PrintMonIsRooted
	accuracycheck FAILED 0x0
	forcerandomswitch BS_TARGET BS_ATTACKER FAILED

DragonTailBS::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	jumpifbehindsubstitute BS_TARGET BattleScript_HitFromAtkString
	call STANDARD_DAMAGE
	jumpifhasnohp BS_TARGET BS_MOVE_FAINT
	jumpifmovehadnoeffect BS_MOVE_FAINT
	jumpifdynamaxed BS_TARGET BattleScript_DragonTailBlockedByDynamax
	jumpifstatus3condition BS_TARGET STATUS3_ROOTED 0x0 BattleScript_PrintMonIsRooted
	jumpifability BS_TARGET ABILITY_SUCTIONCUPS BattleScript_AbilityPreventsPhasingOutSkipFail
	jumpifcantswitch BS_TARGET | ATK4F_DONT_CHECK_STATUSES BS_MOVE_FAINT
	setbyte sMOVEEND_STATE 0x0
	moveend 0x6 0x0
	playanimation BS_TARGET B_DRAGON_TAIL_BLOW_AWAY_ANIM 0x0
	setbyte gForceSwitchHelper 0x1
	forcerandomswitch BS_TARGET BS_ATTACKER BattleScript_DragonTailResetForceSwitchHelper

BattleScript_DragonTailResetForceSwitchHelper::
	setbyte gForceSwitchHelper 0x0
	goto BattleScript_MoveEnd3

BattleScript_DragonTailBlockedByDynamax::
	setword BATTLE_STRING_LOADER gText_RaidBattleAttackCancel
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

BattleScript_AbilityPreventsPhasingOut2::
	orbyte gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE

BattleScript_AbilityPreventsPhasingOutSkipFail::
	pause 0x10
	copyarray sBATTLER TARGET_BANK 0x1
	call BattleScript_AbilityPopUp
	printstring STRINGID_PKMNANCHORSITSELFWITH
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectMultiHit::
	goto BattleScript_HitFromAtkCanceler
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed, ACC_CURR_MOVE
	attackstring
	ppreduce
	setmultihitcounter 0
	initmultihitstring
	setbyte sMULTIHIT_EFFECT, 0
BattleScript_MultiHitLoop::
	jumpifhasnohp BS_ATTACKER, BattleScript_MultiHitEnd
	jumpifhasnohp BS_TARGET, BattleScript_MultiHitPrintStrings
	jumpifhalfword CMP_EQUAL, gChosenMove, MOVE_SLEEP_TALK, BattleScript_DoMultiHit
	jumpifstatus BS_ATTACKER, STATUS1_SLEEP, BattleScript_MultiHitPrintStrings
BattleScript_DoMultiHit::
	movevaluescleanup
	copybyte cEFFECT_CHOOSER, sMULTIHIT_EFFECT
	critcalc
	damagecalc
	typecalc
	jumpifmovehadnoeffect BattleScript_MultiHitNoMoreHits
	adjustnormaldamage
	attackanimation
	waitanimation
	effectivenesssound
	hitanimation BS_TARGET
	waitstate
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	critmessage
	waitmessage 0x40
	printstring STRINGID_EMPTYSTRING3
	waitmessage 1
	addbyte gBattleScripting + 12, 1
	moveendto 16
	jumpifbyte CMP_COMMON_BITS, gMoveResultFlags, MOVE_RESULT_FOE_ENDURED, BattleScript_MultiHitPrintStrings
	decrementmultihit BattleScript_MultiHitLoop
	goto BattleScript_MultiHitPrintStrings

BattleScript_MultiHitNoMoreHits::
	pause 0x20
BattleScript_MultiHitPrintStrings::
	copyarray gBattleTextBuff1 sMULTIHIT_STRING 0x6
	printstring STRINGID_HITXTIMES
	waitmessage DELAY_1SECOND
	return
BattleScript_MultiHitEnd::
	seteffectwithchance
	tryfaintmon BS_TARGET, 0, NULL
	moveendcase 2
	moveendfrom 4
	end

BattleScript_EffectConversion::
	attackcanceler
	jumpifmove MOVE_REFLECTTYPE ReflectTypeBS

ConversionBS::
	tryconversiontypechange FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	printstring STRINGID_PKMNCHANGEDTYPE
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

ReflectTypeBS::
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	accuracycheck BattleScript_PrintMoveMissed 0x0
	tryconversiontypechange FAILED_PRE
	attackstring
	ppreduce	
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER ReflectTypeString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BattleScript_EffectFlinchHit::
BS_031_Flinch::
	setmoveeffect MOVE_EFFECT_FLINCH
	jumpifmove MOVE_THUNDERFANG ThunderFangBS
	jumpifmove MOVE_FIREFANG FireFangBS
	jumpifmove MOVE_ICEFANG IceFangBS
	goto BattleScript_HitFromAtkCanceler
	
ThunderFangBS::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	seteffectwithchance
	setmoveeffect MOVE_EFFECT_PARALYSIS
	seteffectwithchance
	goto BS_MOVE_FAINT

FireFangBS::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	seteffectwithchance
	setmoveeffect MOVE_EFFECT_BURN
	seteffectwithchance
	goto BS_MOVE_FAINT

IceFangBS::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	seteffectwithchance
	setmoveeffect MOVE_EFFECT_FREEZE
	seteffectwithchance
	goto BS_MOVE_FAINT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


BattleScript_EffectRestoreHp::
	attackcanceler
	attackstring
	ppreduce
	tryhealhalfhealth BattleScript_AlreadyAtFullHp, BS_ATTACKER
BattleScript_EffectRestoreHp3::
	attackanimation
	waitanimation
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring STRINGID_PKMNREGAINEDHEALTH
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectToxic::
	attackcanceler
	attackstring @;Protean always activates!
	ppreduce
	jumpifbehindsubstitute BS_TARGET FAILED
	trysetpoison BS_TARGET BS_StatusMoveFail
	accuracycheck BattleScript_MoveMissedPause 0x0
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_TOXIC
	seteffectprimary
	goto BattleScript_MoveEnd

BattleScript_AlreadyPoisoned::
	pause 0x40
	printstring STRINGID_PKMNALREADYPOISONED
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_ImmunityProtected::
	copybyte gEffectBattler, gBattlerTarget
	setbyte cMULTISTRING_CHOOSER, 0
	call BattleScript_PSNPrevention
	goto BattleScript_MoveEnd

BattleScript_EffectPayDay::
	setmoveeffect MOVE_EFFECT_PAYDAY
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectLightScreen::
	attackcanceler
	jumpifsideaffecting BS_ATTACKER SIDE_LIGHTSCREEN FAILED_PRE
	attackstring
	ppreduce
	setlightscreen
	goto ReflectBS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectTriAttack::
	setmoveeffect MOVE_EFFECT_TRI_ATTACK
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectRest::
	attackcanceler
	jumpifterrainandgrounded ELECTRIC_TERRAIN BS_ATTACKER FAILED_PRE
	jumpifterrainandgrounded MISTY_TERRAIN BS_ATTACKER FAILED_PRE
	attackstring
	ppreduce
	jumpifstatus BS_ATTACKER, STATUS1_SLEEP, BattleScript_RestIsAlreadyAsleep @;BattleScript_RestIsAlreadyAsleep
	trysetrest 0x0
	pause DELAY_HALFSECOND
	printfromtable gRestUsedStringIds @;gRestUsedStringIds
	waitmessage DELAY_1SECOND
	updatestatusicon BS_ATTACKER
	waitstate
	goto BattleScript_PresentHealTarget @;BattleScript_PresentHealTarget


BattleScript_RestCantSleep::
	pause 0x40
	printfromtable gUproarAwakeStringIds
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_RestIsAlreadyAsleep::
	pause 0x20
	printstring STRINGID_PKMNALREADYASLEEP2
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectOHKO::
	attackcanceler
	accuracycheck FAILED 0xFFFF
	attackstring
	ppreduce
	typecalc2
	jumpifmovehadnoeffect BattleScript_HitFromAtkAnimation
	tryKO BattleScript_KOFail
	trysetdestinybondtohappen
	goto BattleScript_HitFromAtkAnimation

BattleScript_KOFail::
	pause 0x40
	printfromtable gKOFailedStringIds
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectRazorWind::
	jumpifstatus2 BS_ATTACKER STATUS2_MULTIPLETURNS TwoTurnMovesSecondTurnBS
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING TwoTurnMovesSecondTurnBS
	setbyte sTWOTURN_STRINGID 0x0
	call BattleScript_FirstChargingTurn
	call BattleScript_CheckPowerHerb
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectSkyAttack::
	jumpifstatus2 BS_ATTACKER STATUS2_MULTIPLETURNS TwoTurnMovesSecondTurnBS
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING TwoTurnMovesSecondTurnBS
	setbyte sTWOTURN_STRINGID 0x3
	call BattleScript_FirstChargingTurn
	call BattleScript_CheckPowerHerb
	goto BattleScript_MoveEnd

BattleScript_FirstChargingTurn::
	attackcanceler
	printstring 0x130 @;Blank String
	ppreduce
	attackanimation
	waitanimation
	jumpifmove MOVE_FREEZESHOCK PrintFreezeShockString
	jumpifmove MOVE_ICEBURN PrintIceBurnString
	jumpifmove MOVE_SHADOWFORCE PrintShadowForceString
	jumpifmove MOVE_PHANTOMFORCE PrintShadowForceString
	jumpifmove MOVE_METEORBEAM PrintMeteorBeamString
	copyarray cMULTISTRING_CHOOSER sTWOTURN_STRINGID 0x1
	printfromtable gFirstTurnOfTwoStringIds
	return

PrintFreezeShockString::
	setword BATTLE_STRING_LOADER FreezeShockChargingString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

PrintIceBurnString::
	setword BATTLE_STRING_LOADER IceBurnChargingString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return
	
PrintShadowForceString::
	setword BATTLE_STRING_LOADER ShadowForceString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

PrintMeteorBeamString::
	setword BATTLE_STRING_LOADER gText_MeteorBeamCharge
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

BattleScript_CheckPowerHerb::
	waitmessage DELAY_1SECOND
	jumpifhelditemeffect BS_ATTACKER HOLD_EFFECT_POWER_HERB PowerHerbChargeBS
	jumpifraidboss BS_ATTACKER TwoTurnMovesRaidBossSkipCharge
	orword HIT_MARKER HITMARKER_CHARGING
	setmoveeffect MOVE_EFFECT_CHARGING | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	return

PowerHerbChargeBS::
	playanimation BS_ATTACKER B_ANIM_ITEM_EFFECT 0x0
	setword BATTLE_STRING_LOADER PowerHerbString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	removeitem BS_ATTACKER

TwoTurnMovesRaidBossSkipCharge::
	setbyte sB_ANIM_TARGETS_HIT 0x0
	setbyte sB_ANIM_TURN 0x1
	callasm ClearCalculatedSpreadMoveData @;So the damage can be calculated
	goto PowerHerbSkipBS

TwoTurnMovesSecondTurnBS::
	attackcanceler
	setmoveeffect MOVE_EFFECT_CHARGING
	setbyte sB_ANIM_TURN 0x1
	clearstatusfromeffect BS_ATTACKER

PowerHerbSkipBS:
	orword HIT_MARKER HITMARKER_NO_PPDEDUCT
	clearsemiinvulnerablebit
	jumpifmove MOVE_RAZORWIND RazorWindEffectBS
	jumpifmove MOVE_BOUNCE FreezeShockEffectBS
	jumpifmove MOVE_FREEZESHOCK FreezeShockEffectBS
	jumpifmove MOVE_ICEBURN IceBurnEffectBS
	goto BattleScript_HitFromAccCheck
	
RazorWindEffectBS:
	setmoveeffect MOVE_EFFECT_FLINCH
	goto BattleScript_HitFromAccCheck
	
FreezeShockEffectBS:
	setmoveeffect MOVE_EFFECT_PARALYSIS
	goto BattleScript_HitFromAccCheck
	
IceBurnEffectBS:
	setmoveeffect MOVE_EFFECT_BURN
	goto BattleScript_HitFromAccCheck

BattleScript_TwoTurnMovesSecondTurn::
	attackcanceler
	setmoveeffect MOVE_EFFECT_CHARGING
	setbyte sB_ANIM_TURN, 1
	clearstatusfromeffect BS_ATTACKER
	orword gHitMarker, HITMARKER_NO_PPDEDUCT
	jumpifnotmove MOVE_SKY_ATTACK, BattleScript_HitFromAccCheck
	setmoveeffect MOVE_EFFECT_FLINCH
	goto BattleScript_HitFromAccCheck

BattleScriptFirstChargingTurn::
	attackcanceler
	printstring STRINGID_EMPTYSTRING3
	ppreduce
	attackanimation
	waitanimation
	orword gHitMarker, HITMARKER_CHARGING
	setmoveeffect MOVE_EFFECT_CHARGING | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	copybyte cMULTISTRING_CHOOSER, sTWOTURN_STRINGID
	printfromtable gFirstTurnOfTwoStringIds
	waitmessage 0x40
	return

BattleScript_EffectSuperFang::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	damagetohalftargethp
	goto BattleScript_HitFromAtkAnimation

BattleScript_EffectDragonRage::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	setword gBattleMoveDamage 0x0
	setbyte gBattleMoveDamage 40
	adjustsetdamage
	goto BattleScript_HitFromAtkAnimation

BattleScript_EffectTrap::
BattleScript_DoWrapEffect::
	setmoveeffect MOVE_EFFECT_WRAP
	goto BattleScript_EffectHit

BattleScript_EffectHighCritRatio::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectDoubleHit::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectRecoilIfMiss::
	jumpifability BS_ATTACKER ABILITY_MAGICGUARD BattleScript_HitFromAtkCanceler
	attackcanceler
	accuracycheck HighJumpKickMiss 0x0
	typecalc2
	bicbyte gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	jumpifmovehadnoeffect HighJumpKickMiss
	goto BattleScript_HitFromAtkString

HighJumpKickMiss:
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	resultmessage
	waitmessage DELAY_1SECOND
	printstring 0x60
	waitmessage DELAY_1SECOND
	bicbyte gMoveResultFlags, MOVE_RESULT_MISSED
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	setdamagetobankhealthfraction BS_ATTACKER 2 0x0 @;50 % of Base Max HP
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER 0x0 0x0
	orbyte gMoveResultFlags, MOVE_RESULT_MISSED
	goto BattleScript_MoveEnd

BattleScript_MoveMissedDoDamage::
	attackstring
	ppreduce
	pause 0x40
	resultmessage
	waitmessage 0x40
	jumpifbyte CMP_COMMON_BITS, gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE, BattleScript_MoveEnd
	printstring STRINGID_PKMNCRASHED
	waitmessage 0x40
	damagecalc
	typecalc
	adjustnormaldamage
	manipulatedamage 1
	bicbyte gMoveResultFlags, MOVE_RESULT_MISSED
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER, 0, NULL
	orbyte gMoveResultFlags, MOVE_RESULT_MISSED
	goto BattleScript_MoveEnd

BattleScript_EffectMist::
	attackcanceler
	jumpifsideaffecting BS_ATTACKER SIDE_MIST FAILED_PRE
	attackstring
	ppreduce
	setmist
	attackanimation
	waitanimation
	printfromtable gMistUsedStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectFocusEnergy::
	attackcanceler
	jumpifstatus2 BS_ATTACKER STATUS2_FOCUS_ENERGY FAILED_PRE
	attackstring
	ppreduce
	setfocusenergy
	attackanimation
	waitanimation
	printfromtable gFocusEnergyUsedStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectRecoil::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectConfuse::
	attackcanceler
	attackstring @;Protean always activates!
	ppreduce
	jumpifbehindsubstitute BS_TARGET FAILED
	canconfuse BS_TARGET BS_StatusMoveFail
	accuracycheck BattleScript_MoveMissedPause 0x0
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_CONFUSION
	seteffectprimary
	resultmessage
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_AlreadyConfused::
	pause 0x20
	printstring STRINGID_PKMNALREADYCONFUSED
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectAttackUp2::
	setstatchanger2 STAT_ATK | INCREASE_2
	goto BattleScript_EffectStatUp

BattleScript_EffectDefenseUp2::
	setstatchanger2 STAT_DEF | INCREASE_2
	jumpifmove MOVE_COTTONGUARD CottonGuardBS
	jumpifmove MOVE_STUFFCHEEKS StuffCheeksBS
	goto BattleScript_EffectStatUp
	
CottonGuardBS::
	setstatchanger2 STAT_DEF | INCREASE_3
	goto BattleScript_EffectStatUp

StuffCheeksBS::
	attackcanceler
	callasm FailIfAttackerIsntHoldingEdibleBerry
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm SetTempIgnoreAnimations @;So the berry animation doesn't play
	setmoveeffect MOVE_EFFECT_EAT_BERRY
	seteffectprimary
	callasm SetTempIgnoreAnimations @;So the attack animation doesn't play again
	setstatchanger2 STAT_DEF | INCREASE_2
	goto BattleScript_EffectStatUpAfterStart

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectSpeedUp2::
	setstatchanger2 STAT_SPEED| INCREASE_2
	jumpifnotmove MOVE_AUTOTOMIZE BattleScript_EffectStatUp

AutotomizeBS::
	attackcanceler
	attackstring
	ppreduce
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_StatUpEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_EffectStatUpAfterStart3
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	callasm IncreaseNimbleCounter
	setword BATTLE_STRING_LOADER BecameNimbleString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BattleScript_EffectSpecialAttackUp2::
	setstatchanger2 STAT_SPATK | INCREASE_2
	jumpifnotmove MOVE_TAILGLOW BattleScript_EffectStatUp
	setstatchanger2 STAT_SPATK | INCREASE_3
	goto BattleScript_EffectStatUp

BattleScript_EffectSpecialDefenseUp2::
	setstatchanger2 STAT_SPDEF | INCREASE_2
	goto BattleScript_EffectStatUp

BattleScript_EffectRaiseUserAcc2::
	setstatchanger2 STAT_ACC | INCREASE_2
	goto BattleScript_EffectStatUp

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectRaiseUserEvsn2::
	setstatchanger2 STAT_EVASION | INCREASE_2
	goto BattleScript_EffectStatUp

BattleScript_EffectTransform::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	jumpifstatus2 BS_ATTACKER STATUS2_TRANSFORMED FAILED_PRE
	jumpifstatus3condition BS_TARGET STATUS3_ILLUSION 0x0 FAILED_PRE
	attackstring
	ppreduce
	transformdataexecution
	attackanimation
	waitanimation
	printfromtable gTransformUsedStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectAttackDown2::
	setstatchanger2 STAT_ATK | DECREASE_2
	goto BattleScript_EffectStatDown

BattleScript_EffectDefenseDown2::
	setstatchanger2 STAT_DEF | DECREASE_2
	goto BattleScript_EffectStatDown

BattleScript_EffectSpeedDown2::
	setstatchanger2 STAT_SPEED| DECREASE_2
	goto BattleScript_EffectStatDown

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectLowerTargetSpAtk2::
	setstatchanger2 STAT_SPATK | DECREASE_2
	jumpifnotmove MOVE_CAPTIVATE BattleScript_EffectStatDown
	
CaptivateBS::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	jumpifability BS_TARGET ABILITY_OBLIVIOUS BattleScript_ObliviousPrevents
	callasm CaptivateFunc
	goto BattleScript_EffectStatDown2

BattleScript_ObliviousPrevents::
	pause 0x10
	copyarray sBATTLER TARGET_BANK 0x1
	call BattleScript_AbilityPopUp
	orbyte gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE
	printstring STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectSpecialDefenseDown2::
	setstatchanger2 STAT_SPDEF | DECREASE_2
	goto BattleScript_EffectStatDown

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectLowerTargetAcc2::
	setstatchanger2 STAT_ACC | DECREASE_2
	goto BattleScript_EffectStatDown

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectLowerTargetEvsn2::
	setstatchanger2 STAT_EVASION | DECREASE_2
	goto BattleScript_EffectStatDown
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectReflect::
	attackcanceler
	jumpifmove MOVE_AURORAVEIL AuroraVeilBS
	jumpifsideaffecting BS_ATTACKER SIDE_REFLECT FAILED_PRE
	attackstring
	ppreduce
	setreflect
	
ReflectBS::
	attackanimation
	waitanimation
	printfromtable gReflectLightScreenSafeguardStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

AuroraVeilBS::
	jumpifcounter BS_ATTACKER AURORA_VEIL_TIMERS NOTEQUALS 0 FAILED_PRE
	attackstring
	ppreduce
	callasm SetAuroraVeil
	goto ReflectBS

BattleScript_PrintReflectLightScreenSafeguardString::
	attackanimation
	waitanimation
	printfromtable gReflectLightScreenSafeguardStringIds
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectPoison::
	attackcanceler
	attackstring @;Protean always activates!
	ppreduce
	jumpifbehindsubstitute BS_TARGET FAILED
	jumpifmove MOVE_TOXICTHREAD ToxicThreadDo

PoisonChecks::
	trysetpoison BS_TARGET BS_StatusMoveFail
	accuracycheck BattleScript_MoveMissedPause 0x0
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_POISON
	seteffectprimary
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ToxicThreadDo::
	accuracycheck BattleScript_MoveMissedPause 0x0
	setstatchanger2 STAT_SPEED| DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR ToxicThreadPSN
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 ToxicThreadPSN

@ToxicThreadWork
	attackanimation
	waitanimation
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	setmoveeffect MOVE_EFFECT_POISON
	seteffectprimary
	goto BattleScript_MoveEnd

ToxicThreadPSN::
	bicbyte gMoveResultFlags, 1
	goto PoisonChecks

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectParalyze::
	attackcanceler
	attackstring @;Protean always activates!
	ppreduce
	jumpifbehindsubstitute BS_TARGET FAILED
	trysetparalysis BS_TARGET BS_StatusMoveFail
	accuracycheck BattleScript_MoveMissedPause 0x0
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_PARALYSIS
	seteffectprimary
	goto BattleScript_MoveEnd

BattleScript_AlreadyParalyzed::
	pause 0x20
	printstring STRINGID_PKMNISALREADYPARALYZED
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_LimberProtected::
	copybyte gEffectBattler, gBattlerTarget
	setbyte cMULTISTRING_CHOOSER, 0
	call BattleScript_PRLZPrevention
	goto BattleScript_MoveEnd

BattleScript_EffectAttackDownHit::
	setmoveeffect MOVE_EFFECT_ATK_MINUS_1
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectDefenseDownHit::
	setmoveeffect MOVE_EFFECT_DEF_MINUS_1
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectSpeedDownHit::
	setmoveeffect MOVE_EFFECT_SPD_MINUS_1
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectSpecialAttackDownHit::
	setmoveeffect MOVE_EFFECT_SP_ATK_MINUS_1
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectSpecialDefenseDownHit::
	jumpifmove MOVE_ACIDSPRAY AcidSprayBS
	jumpifmove MOVE_SEEDFLARE AcidSprayBS
	setmoveeffect MOVE_EFFECT_SP_DEF_MINUS_1
	goto BattleScript_HitFromAtkCanceler

AcidSprayBS::
	setmoveeffect MOVE_EFFECT_SP_DEF_MINUS_2
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectAccuracyDownHit::
	setmoveeffect MOVE_EFFECT_ACC_MINUS_1
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectLowerTargetEvsn1Chance::
	setmoveeffect MOVE_EFFECT_EVS_MINUS_1
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectConfuseHit::
BS_076_SetConfusionChance::
	setmoveeffect MOVE_EFFECT_CONFUSION
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectTwineedle::
	setmoveeffect MOVE_EFFECT_POISON
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectSubstitute::
	attackcanceler
	ppreduce
	attackstring
	waitstate
	jumpifstatus2 BS_ATTACKER, STATUS2_SUBSTITUTE, BattleScript_AlreadyHasSubstitute
	setsubstitute
	jumpifbyte CMP_NOT_EQUAL, cMULTISTRING_CHOOSER, 1, BattleScript_SubstituteAnim
	pause 0x20
	goto BattleScript_SubstituteString

BattleScript_SubstituteAnim::
	attackanimation
	waitanimation
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
BattleScript_SubstituteString::
	printfromtable gSubsituteUsedStringIds
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_AlreadyHasSubstitute::
	pause 0x20
	printstring STRINGID_PKMNHASSUBSTITUTE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectRecharge::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed, ACC_CURR_MOVE
	setmoveeffect MOVE_EFFECT_RECHARGE | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkString

BattleScript_MoveUsedMustRecharge::
	printstring STRINGID_PKMNMUSTRECHARGE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectRage::
	attackcanceler
	accuracycheck BattleScript_RageMiss, ACC_CURR_MOVE
	setmoveeffect MOVE_EFFECT_RAGE
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffectprimary
	goto BS_MOVE_FAINT

BattleScript_RageMiss::
	setmoveeffect MOVE_EFFECT_RAGE
	clearstatusfromeffect BS_ATTACKER
	goto BattleScript_PrintMoveMissed

BattleScript_EffectMimic::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	mimicattackcopy FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x16
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectMetronome::
	attackcanceler
	attackstring
	pause DELAY_HALFSECOND
	attackanimation
	waitanimation
	setbyte sB_ANIM_TURN 0x0
	setbyte sB_ANIM_TARGETS_HIT 0x0
	metronome
BattleScript_EffectLeechSeed::
	attackcanceler
	attackstring
	pause 0x20
	ppreduce
	jumpifstatus2 BS_TARGET, STATUS2_SUBSTITUTE, BattleScript_ButItFailed
	accuracycheck BattleScript_DoLeechSeed, ACC_CURR_MOVE
BattleScript_DoLeechSeed::
	setseeded
	attackanimation
	waitanimation
	printfromtable gLeechSeedStringIds
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectSplash::
	attackcanceler
	attackstring
	ppreduce
	attackanimation
	waitanimation
	incrementgamestat GAME_STAT_USED_SPLASH
	printstring STRINGID_BUTNOTHINGHAPPENED
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectDisable::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstringnoprotean
	ppreduce
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL BattleScript_ProtectedByAromaVeil
	disablelastusedattack BS_TARGET FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_PKMNMOVEWASDISABLED
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_ProtectedByAromaVeil::
	pause 0x10
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER gText_AromaVeilProtects
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

BattleScript_EffectLevelDamage::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed, ACC_CURR_MOVE
	attackstring
	ppreduce
	typecalc2
	bicbyte gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	dmgtolevel
	adjustsetdamage
	goto BattleScript_HitFromAtkAnimation

BattleScript_EffectPsywave::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed, ACC_CURR_MOVE
	attackstring
	ppreduce
	typecalc2
	bicbyte gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	psywavedamageeffect
	adjustsetdamage
	goto BattleScript_HitFromAtkAnimation

BattleScript_EffectCounter::
	attackcanceler
	jumpifmove MOVE_METALBURST MetalBurstBS
	
CounterBS::
	counterdamagecalculator FAILED_PRE
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	adjustsetdamage
	goto BattleScript_HitFromAtkAnimation

MetalBurstBS::
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	callasm MetalBurstDamageCalculator
	typecalc2
	bicbyte gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	adjustsetdamage
	goto BattleScript_HitFromAtkAnimation
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectEncore::
	attackcanceler	
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstringnoprotean
	ppreduce
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL BattleScript_ProtectedByAromaVeil
	trysetencore FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x87
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
BattleScript_EffectPainSplit::
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	painsplitdmgcalc FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	copyarray gBattleMoveDamage, sPAINSPLIT_HP, 0x4
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printstring STRINGID_SHAREDPAIN
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectSnore::
	attackcanceler
	jumpifstatus BS_ATTACKER STATUS1_SLEEP BattleScript_SnoreIsAsleep
	jumpifability BS_ATTACKER ABILITY_COMATOSE BattleScript_SnoreIsAsleep
	goto FAILED_PRE

BattleScript_SnoreIsAsleep::
	jumpifmove MOVE_SLEEPTALK BattleScript_DoSnore
	printstring STRINGID_PKMNFASTASLEEP
	waitmessage DELAY_1SECOND
	statusanimation BS_ATTACKER

BattleScript_DoSnore::
	attackstring
	ppreduce
	accuracycheck BattleScript_PrintMoveMissed 0x0
	setmoveeffect MOVE_EFFECT_FLINCH
	goto BattleScript_HitFromCritCalc

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectConversion2::
	attackcanceler
	attackstring
	ppreduce
	settypetorandomresistance BattleScript_ButItFailed
	attackanimation
	waitanimation
	printstring STRINGID_PKMNCHANGEDTYPE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectLockOn::
	attackcanceler
	attackstring
	ppreduce
	jumpifmove MOVE_LASERFOCUS LaserFocusBS
	
LockOnBS::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	accuracycheck FAILED_PRE 0x0
	attackstring
	ppreduce
	setalwayshitflag
	attackanimation
	waitanimation
	printstring 0x89
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

LaserFocusBS::
	callasm SetLaserFocusTimer
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER LaserFocusString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectSketch::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	attackstringnoprotean
	ppreduce
	copymovepermanently FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_PKMNSKETCHEDMOVE
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectRaiseAttackerSpd1Chance::
	setmoveeffect MOVE_EFFECT_SPD_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto BattleScript_HitFromAtkCanceler

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectSleepTalk::
	attackcanceler
	jumpifstatus BS_ATTACKER, STATUS1_SLEEP, BattleScript_SleepTalkIsAsleep
	jumpifability BS_ATTACKER ABILITY_COMATOSE BattleScript_SleepTalkIsAsleep
	goto FAILED_PRE

BattleScript_SleepTalkIsAsleep::
	printstring STRINGID_PKMNFASTASLEEP
	waitmessage 0x40
	statusanimation BS_ATTACKER
	attackstring
	ppreduce
	orword gHitMarker, HITMARKER_NO_PPDEDUCT
	trychoosesleeptalkmove BattleScript_SleepTalkUsingMove
	pause 0x40
	goto BattleScript_ButItFailed

BattleScript_SleepTalkUsingMove::
	attackanimation
	waitanimation
	setbyte sB_ANIM_TURN, 0
	setbyte sB_ANIM_TARGETS_HIT, 0
	jumptocalledmove 1
BattleScript_EffectDestinyBond::
	attackcanceler
	attackstringnoprotean
	ppreduce
	setdestinybond
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_PKMNTRYINGTOTAKEFOE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectFlail::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectSpite::
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	tryspiteppreduce FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_PKMNREDUCEDPP
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectPriorityMove::
BattleScript_EffectFalseSwipe::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectTripleKick::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	jumpifmove MOVE_TRIPLEAXEL BS_TripleAxel
	addbyte sTRIPLE_KICK_POWER 10
	goto BattleScript_HitFromCritCalc

BS_TripleAxel::
	addbyte sTRIPLE_KICK_POWER 20
	goto BattleScript_HitFromCritCalc

BattleScript_TripleKickLoop::
	jumpifhasnohp BS_ATTACKER, BattleScript_TripleKickEnd
	jumpifhasnohp BS_TARGET, BattleScript_TripleKickNoMoreHits
	jumpifhalfword CMP_EQUAL, gChosenMove, MOVE_SLEEP_TALK, BattleScript_DoTripleKickAttack
	jumpifstatus BS_ATTACKER, STATUS1_SLEEP, BattleScript_TripleKickNoMoreHits
BattleScript_DoTripleKickAttack::
	accuracycheck BattleScript_TripleKickNoMoreHits, ACC_CURR_MOVE
	movevaluescleanup
	addbyte sTRIPLE_KICK_POWER, 10
	addbyte gBattleScripting + 12, 1
	copyhword gDynamicBasePower, sTRIPLE_KICK_POWER
	critcalc
	damagecalc
	typecalc
	adjustnormaldamage
	jumpifmovehadnoeffect BattleScript_TripleKickNoMoreHits
	attackanimation
	waitanimation
	effectivenesssound
	hitanimation BS_TARGET
	waitstate
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	critmessage
	waitmessage 0x40
	printstring STRINGID_EMPTYSTRING3
	waitmessage 1
	moveendto 16
	jumpifbyte CMP_COMMON_BITS, gMoveResultFlags, MOVE_RESULT_FOE_ENDURED, BattleScript_TripleKickPrintStrings
	decrementmultihit BattleScript_TripleKickLoop
	goto BattleScript_TripleKickPrintStrings

BattleScript_TripleKickNoMoreHits::
	pause 0x20
	jumpifbyte CMP_EQUAL, gBattleScripting + 12, 0, BattleScript_TripleKickPrintStrings
	bicbyte gMoveResultFlags, MOVE_RESULT_MISSED
BattleScript_TripleKickPrintStrings::
	resultmessage
	waitmessage 0x40
	jumpifbyte CMP_EQUAL, gBattleScripting + 12, 0, BattleScript_TripleKickEnd
	jumpifbyte CMP_COMMON_BITS, gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE, BattleScript_TripleKickEnd
	copyarray gBattleTextBuff1, sMULTIHIT_STRING, 6
	printstring STRINGID_HITXTIMES
	waitmessage 0x40
BattleScript_TripleKickEnd::
	seteffectwithchance
	tryfaintmon BS_TARGET, 0, NULL
	moveendfrom 14
	end

BattleScript_EffectThief::
	setmoveeffect MOVE_EFFECT_STEAL_ITEM
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectMeanLook::
	jumpifmove MOVE_ANCHORSHOT DamageAndTrapBS
	jumpifmove MOVE_SPIRITSHACKLE DamageAndTrapBS
	jumpifmove MOVE_THOUSANDWAVES DamageAndTrapBS
	jumpifmove MOVE_JAWLOCK JawLockBS
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	jumpiftype BS_TARGET TYPE_GHOST FAILED_PRE
	jumpifstatus2 BS_TARGET STATUS2_ESCAPE_PREVENTION | STATUS2_SUBSTITUTE FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	seteffectprimary
	printstring STRINGID_TARGETCANTESCAPENOW @;STRINGID_TARGETCANTESCAPENOW
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
DamageAndTrapBS::
	jumpifstatus2 BS_TARGET STATUS2_ESCAPE_PREVENTION | STATUS2_SUBSTITUTE BattleScript_HitFromAtkCanceler
	jumpiftype BS_TARGET TYPE_GHOST BattleScript_HitFromAtkCanceler
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	jumpifhasnohp BS_TARGET BS_MOVE_FAINT
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffectprimary
	printstring STRINGID_TARGETCANTESCAPENOW
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

JawLockBS::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	jumpifhasnohp BS_TARGET BS_MOVE_FAINT
	jumpifmovehadnoeffect BS_MOVE_FAINT
	@;Make both banks lock each other
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	seteffectprimary
	swapattackerwithtarget
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE
	seteffectprimary
	swapattackerwithtarget
	setword BATTLE_STRING_LOADER gText_JawLockNoEscape
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectNightmare::
	attackcanceler
	jumpifstatus2 0x0 STATUS2_SUBSTITUTE | STATUS2_NIGHTMARE FAILED_PRE
	attackstring
	ppreduce
	jumpifstatus BS_TARGET STATUS1_SLEEP SetNightmareBS
	jumpifability BS_TARGET ABILITY_COMATOSE SetNightmareBS
	goto BattleScript_NotAffected

SetNightmareBS::
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_NIGHTMARE
	seteffectprimary
	printstring STRINGID_PKMNFELLINTONIGHTMARE
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectMinimize::
	attackcanceler
	setminimize
	setstatchanger2 STAT_EVASION | INCREASE_2
	goto BattleScript_EffectStatUpAfterAtkCanceler

BattleScript_EffectCurse::
	jumpiftype2 BS_ATTACKER, TYPE_GHOST, BattleScript_GhostCurse
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BS_ATTACKER, CMP_GREATER_THAN, STAT_SPEED, 0, BattleScript_CurseTrySpeed
	jumpifstat BS_ATTACKER, CMP_NOT_EQUAL, STAT_ATK, 12, BattleScript_CurseTrySpeed
	jumpifstat BS_ATTACKER, CMP_EQUAL, STAT_DEF, 12, BattleScript_ButItFailed
BattleScript_CurseTrySpeed::
	copybyte gBattlerTarget, gBattlerAttacker
	setbyte sB_ANIM_TURN, 1
	attackanimation
	waitanimation
	setstatchanger STAT_SPEED, 1, TRUE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CurseTryAttack
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_CurseTryAttack::
	setstatchanger STAT_ATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CurseTryDefence
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_CurseTryDefence::
	setstatchanger STAT_DEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CurseEnd
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_CurseEnd::
	goto BattleScript_MoveEnd

BattleScript_GhostCurse::
	jumpifbytenotequal gBattlerAttacker, gBattlerTarget, BattleScript_DoGhostCurse
	getmovetarget BS_ATTACKER
BattleScript_DoGhostCurse::
	attackcanceler
	attackstring
	ppreduce
	jumpifstatus2 BS_TARGET, STATUS2_SUBSTITUTE, BattleScript_ButItFailed
	accuracycheck BattleScript_ButItFailed, NO_ACC_CALC_CHECK_LOCK_ON
	cursetarget BattleScript_ButItFailed
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE
	setbyte sB_ANIM_TURN, 0
	attackanimation
	waitanimation
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring STRINGID_PKMNLAIDCURSE
	waitmessage 0x40
	tryfaintmon BS_ATTACKER, 0, NULL
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectRaiseAttackerSpAtk1Chance::
	setmoveeffect MOVE_EFFECT_SP_ATK_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto BattleScript_HitFromAtkCanceler

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectProtect::
	attackcanceler
	attackstringnoprotean
	ppreduce
	setprotectlike
	tryactivateprotean
	attackanimation
	waitanimation
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x3 ProtectPrintCustomMessage
	printfromtable gProtectLikeUsedStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
ProtectPrintCustomMessage::
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectSpikes::
	attackcanceler
	attackstringnoprotean
	ppreduce
	trysetspikes FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectForesight::
	jumpifmove MOVE_MIRACLEEYE MiracleEyeBS
	
ForesightBS::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	setforesight
	attackanimation
	waitanimation
	printstring 0x96
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
MiracleEyeBS::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	jumpifstatus3condition BS_TARGET STATUS3_MIRACLE_EYED 0x0 FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setspecialstatusbit BS_TARGET STATUS3_MIRACLE_EYED
	printstring 0x96
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectPerishSong::
	attackcanceler
	attackstringnoprotean
	ppreduce
	trysetperishsong FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xFE
	waitmessage DELAY_1SECOND
	setbyte sBATTLER 0x0
	
PerishSongLoopBS::
	jumpifability BS_SCRIPTING ABILITY_SOUNDPROOF BattleScript_PerishSongNotAffected
	
BattleScript_PerishSongLoopIncrement::
	addbyte sBATTLER 0x1
	jumpifarraynotequal sBATTLER NUM_POKEMON 0x1 PerishSongLoopBS
	goto BattleScript_MoveEnd
	
BattleScript_PerishSongNotAffected::
	copyarray TARGET_BANK sBATTLER 0x1
	call BattleScript_AbilityPopUp
	printstring STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_PerishSongLoopIncrement

BattleScript_EffectSandstorm::
	attackcanceler
	attackstring
	ppreduce
	setsandstorm
	goto BattleScript_MoveWeatherChange

BattleScript_EffectRollout::
	attackcanceler
	attackstring
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_RolloutCheckAccuracy
	ppreduce
BattleScript_RolloutCheckAccuracy::
	accuracycheck BattleScript_RolloutHit, ACC_CURR_MOVE
BattleScript_RolloutHit::
	typecalc2
	rolloutdamagecalculation
	goto BattleScript_HitFromCritCalc

BattleScript_EffectSwagger::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET BattleScript_MakeMoveMissed
	accuracycheck BattleScript_PrintMoveMissed 0x0
	jumpifconfusedandstatmaxed STAT_ATK FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setstatchanger2 STAT_ATK | INCREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR SwaggerTryConfuseBS
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 SwaggerTryConfuseBS
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	
SwaggerTryConfuseBS::
	canconfuse BS_TARGET BS_StatusMoveFail
	setmoveeffect MOVE_EFFECT_CONFUSION
	seteffectprimary
	goto BattleScript_MoveEnd

BattleScript_EffectFuryCutter::
	jumpifmove MOVE_ROUND RoundBS
	jumpifmove MOVE_ECHOEDVOICE EchoedVoiceBS

FuryCutterBS::
	attackcanceler
	accuracycheck FuryCutterResetBS 0x0
	call STANDARD_DAMAGE
	seteffectwithchance
	furycuttercalc
	goto BS_MOVE_FAINT
	
FuryCutterResetBS::
	furycuttercalc
	goto BattleScript_MakeMoveMissed

RoundBS::
	attackcanceler
	callasm RoundBSFunction
	accuracycheck MoveMissedSetRoundBit 0x0
	call STANDARD_DAMAGE
	callasm SetRoundUsed
	goto BS_MOVE_FAINT
	
MoveMissedSetRoundBit::
	callasm SetRoundUsed
	goto BattleScript_MakeMoveMissed

EchoedVoiceBS::
	attackcanceler
	callasm EchoedVoiceFunc
	goto BattleScript_HitFromAccCheck

BattleScript_EffectAttract::
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	attackstring
	ppreduce
	jumpifability BS_TARGET ABILITY_OBLIVIOUS BattleScript_ObliviousPrevents
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL BattleScript_ProtectedByAromaVeil
	tryinfatuating BS_TARGET BattleScript_NotAffected
	attackanimation
	waitanimation
	printstring 0x45
	waitmessage DELAY_1SECOND
	jumpifstatus2 BS_ATTACKER STATUS2_INFATUATION BattleScript_MoveEnd
	jumpifability BS_ATTACKER ABILITY_OBLIVIOUS BattleScript_MoveEnd
	jumpifabilitypresentattackerfield ABILITY_AROMAVEIL BattleScript_MoveEnd
	jumpifhelditemeffect BS_TARGET HOLD_EFFECT_DESTINY_KNOT AttractDestinyKnot
	goto BattleScript_MoveEnd
	
AttractDestinyKnot::
	tryinfatuating BS_ATTACKER BattleScript_MoveEnd
	playanimation BS_TARGET B_ANIM_ITEM_EFFECT 0x0
	status2animation BS_ATTACKER STATUS2_INFATUATION
	copyarray sBATTLER gBattlerAttacker 0x1
	setword BATTLE_STRING_LOADER DestinyKnotString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectReturn::
	goto BattleScript_HitFromAtkString

BattleScript_EffectPresent::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed, ACC_CURR_MOVE
	attackstring
	ppreduce
	typecalc
	presentdamagecalculation
BattleScript_EffectSafeguard::
	attackcanceler
	jumpifsideaffecting BS_ATTACKER SIDE_SAFEGUARD FAILED_PRE
	attackstring
	ppreduce
	setsafeguard
	goto BattleScript_PrintReflectLightScreenSafeguardString

BattleScript_EffectThawHit::
	attackcanceler
	jumpiftype BS_ATTACKER TYPE_FIRE DoBurnUp
	goto FAILED_PRE

DoBurnUp::
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	seteffectwithchance
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	jumpifmovehadnoeffect BattleScript_MoveEnd
	setbyte sMOVEEND_STATE 0x0
	moveend 0x0 0x0
	jumpifhasnohp BS_ATTACKER BurnUpEnd	
	callasm BurnUpFunc
	setword BATTLE_STRING_LOADER BurnUpString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND

BurnUpEnd:
	end

BattleScript_EffectMagnitude::
	attackcanceler
	attackstring
	ppreduce
	magnitudedamagecalculation
	pause 0x20
	printstring STRINGID_MAGNITUDESTRENGTH
	waitmessage 0x40
	accuracycheck BattleScript_PrintMoveMissed 0x0
	goto BattleScript_HitFromCritCalc

BattleScript_EffectPursuit::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectRapidSpin::
	jumpifmove MOVE_DEFOG DefogBS
	
RapidSpinBS::
	setmoveeffect MOVE_EFFECT_RAPIDSPIN | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkCanceler
	
DefogBS::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET SecondDefogCheck
	accuracycheck SecondDefogCheck 0x0
	attackstring
	ppreduce
	setstatchanger2 STAT_EVASION | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR SecondDefogCheck
	jumpifbyte LESSTHAN cMULTISTRING_CHOOSER 0x2 DefogLoweredStat
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x3 SecondDefogCheck
	pause DELAY_HALFSECOND
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	goto SecondDefogCheck
	
DefogLoweredStat::
	attackanimation
	waitanimation
	setbyte sB_ANIM_TARGETS_HIT 0x1 @;So animation doesn't play again
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_DefogAdditionalEffects

BattleScript_EffectSonicboom::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed, ACC_CURR_MOVE
	attackstring
	ppreduce
	typecalc2
	bicbyte gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	setword gBattleMoveDamage 0x0
	setbyte gBattleMoveDamage 20
	adjustsetdamage
	goto BattleScript_HitFromAtkAnimation

BattleScript_EffectMorningSun::
	attackcanceler
	attackstring
	ppreduce
	recoverbasedonsunlight BattleScript_AlreadyAtFullHp @;AlreadyAtFullHp
	goto BattleScript_EffectRestoreHp3 @;Part of Recover

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BattleScript_EffectAttackerRaiseDef2Chance::
	setmoveeffect MOVE_EFFECT_DEF_PLUS_2 | MOVE_EFFECT_AFFECTS_USER
	goto BattleScript_HitFromAtkCanceler

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectRainDance::
	attackcanceler
	attackstringnoprotean
	ppreduce
	setrain
	tryactivateprotean
BattleScript_MoveWeatherChange::
	attackanimation
	waitanimation
	printfromtable gMoveWeatherChangeStringIds
	waitmessage 0x40
	call BattleScript_HandleWeatherFormChanges
	goto BattleScript_MoveEnd

BattleScript_EffectSunnyDay::
	attackcanceler
	attackstringnoprotean
	ppreduce
	setsunny
	tryactivateprotean
	goto BattleScript_MoveWeatherChange

BattleScript_EffectDefenseUpHit::
	setmoveeffect MOVE_EFFECT_DEF_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto BattleScript_EffectHit

BattleScript_EffectAttackUpHit::
	jumpifmove MOVE_FELLSTINGER FellStingerBS
	setmoveeffect MOVE_EFFECT_ATK_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	goto BattleScript_HitFromAtkCanceler

FellStingerBS:: @;Add in Volt Switch Fix?
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	jumpifhasnohp BS_ATTACKER BS_MOVE_FAINT
	jumpifhasnohp BS_TARGET FellStingerKill
	goto BS_MOVE_FAINT

FellStingerKill::
	setbyte sSTAT_ANIM_PLAYED 0x0
	jumpifstat BS_ATTACKER EQUALS STAT_ATK STAT_MAX BattleScript_MoveEnd
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_ATK, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
	setstatchanger2 STAT_ATK | INCREASE_3
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN, BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
BattleScript_EffectAllStatsUpHit::
	setmoveeffect MOVE_EFFECT_ALL_STATS_UP | MOVE_EFFECT_AFFECTS_USER
	goto BattleScript_EffectHit

BattleScript_EffectBellyDrum::
	attackcanceler
	attackstringnoprotean
	ppreduce
	maxattackhalvehp FAILED
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	tryactivateprotean
	attackanimation
	waitanimation
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	playanimation BS_ATTACKER B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	jumpifability BS_ATTACKER ABILITY_CONTRARY BattleScript_PrintContraryBellyDrumLoweredAttack
	printstring STRINGID_PKMNCUTHPMAXEDATTACK
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
BattleScript_PrintContraryBellyDrumLoweredAttack::
	setword BATTLE_STRING_LOADER gText_BellyDrumMinimizedAttack
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectPsychUp::
	jumpifnotmove MOVE_SPECTRALTHIEF BattleScript_EffectPsychUp3

SpectralThiefBS:
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	tryspectralthiefsteal PlaySpectBoost
	setbyte sB_ANIM_TURN 0x1
	goto BattleScript_HitFromCritCalc

PlaySpectBoost:
	attackanimation
	waitanimation
	setbyte FORM_COUNTER 0x0
	callasm ToggleSpectralThiefByte
	playstatchangeanimation BS_ATTACKER, 0xFF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	callasm ToggleSpectralThiefByte
	setword BATTLE_STRING_LOADER SpectralThiefString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	setbyte sB_ANIM_TURN 0x1
	callasm ClearCalculatedSpreadMoveData @;So the damage can be calculated
	goto BattleScript_HitFromCritCalc

BattleScript_EffectPsychUp3::
	attackcanceler
	attackstring
	ppreduce
	copyfoestats BattleScript_ButItFailed
	attackanimation
	waitanimation
	printstring STRINGID_PKMNCOPIEDSTATCHANGES
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectMirrorCoat::
	attackcanceler
	mirrorcoatdamagecalculator FAILED_PRE
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	adjustsetdamage
	goto BattleScript_HitFromAtkAnimation

BattleScript_EffectSkullBash::
	jumpifstatus2 BS_ATTACKER STATUS2_MULTIPLETURNS TwoTurnMovesSecondTurnBS
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING TwoTurnMovesSecondTurnBS
	setbyte sTWOTURN_STRINGID, 0x2
	call BattleScript_FirstChargingTurn
	jumpifmove MOVE_METEORBEAM BS_MeteorBeam
	setstatchanger2 STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_CheckPowerHerb
	goto BattleScript_MoveEnd

BS_MeteorBeam::
	setstatchanger2 STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_CheckPowerHerb
	goto BattleScript_MoveEnd

BattleScript_EffectTwister::
	orword HIT_MARKER HITMARKER_IGNORE_ON_AIR
	goto BS_031_Flinch
BattleScript_FlinchEffect::
	setmoveeffect MOVE_EFFECT_FLINCH
	goto BattleScript_EffectHit

BattleScript_EffectEarthquake::
	orword HIT_MARKER HITMARKER_IGNORE_UNDERGROUND
	goto BattleScript_HitFromAtkCanceler
BattleScript_HitsAllWithUndergroundBonusLoop::
	movevaluescleanup
	jumpifnostatus3 BS_TARGET, STATUS3_UNDERGROUND, BattleScript_HitsAllNoUndergroundBonus
	orword gHitMarker, HITMARKER_IGNORE_UNDERGROUND
	setbyte sDMG_MULTIPLIER, 2
	goto BattleScript_DoHitAllWithUndergroundBonus

BattleScript_HitsAllNoUndergroundBonus::
	bicword gHitMarker, HITMARKER_IGNORE_UNDERGROUND
	setbyte sDMG_MULTIPLIER, 1
BattleScript_DoHitAllWithUndergroundBonus::
	accuracycheck BattleScript_HitAllWithUndergroundBonusMissed, ACC_CURR_MOVE
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
	waitmessage 0x40
	resultmessage
	waitmessage 0x40
	printstring STRINGID_EMPTYSTRING3
	waitmessage 1
	tryfaintmon BS_TARGET, 0, NULL
	moveendto 16
	jumpifnexttargetvalid BattleScript_HitsAllWithUndergroundBonusLoop
	end

BattleScript_HitAllWithUndergroundBonusMissed::
	pause 0x20
	typecalc
	effectivenesssound
	resultmessage
	waitmessage 0x40
	moveendto 16
	jumpifnexttargetvalid BattleScript_HitsAllWithUndergroundBonusLoop
	end

BattleScript_EffectFutureSight::
	attackcanceler
	attackstringnoprotean
	ppreduce
	trysetfutureattack FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printfromtable gFutureMoveUsedStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectGust::
	orword HIT_MARKER HITMARKER_IGNORE_ON_AIR
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectFlinchMinimizeHit::
	jumpifnostatus3 BS_TARGET, STATUS3_MINIMIZED, BattleScript_FlinchEffect
	setbyte sDMG_MULTIPLIER, 2
	goto BattleScript_FlinchEffect

BattleScript_EffectSolarbeam::
	jumpifabilitypresent ABILITY_CLOUDNINE, BSSolarbeamDecideTurn
	jumpifabilitypresent ABILITY_AIRLOCK, BSSolarbeamDecideTurn
	jumpifhelditemeffect BS_ATTACKER, HOLD_EFFECT_UTILITY_UMBRELLA, BSSolarbeamDecideTurn
	jumpifweather WEATHER_SUN_ANY, BSSolarbeamOnFirstTurn

BSSolarbeamDecideTurn::
	jumpifstatus2 BS_ATTACKER STATUS2_MULTIPLETURNS TwoTurnMovesSecondTurnBS
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING TwoTurnMovesSecondTurnBS
	setbyte sTWOTURN_STRINGID, 0x1
	call BattleScript_FirstChargingTurn
	call BattleScript_CheckPowerHerb
	goto BattleScript_MoveEnd

BSSolarbeamOnFirstTurn::
	orword HIT_MARKER HITMARKER_CHARGING
	setmoveeffect MOVE_EFFECT_CHARGING | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	ppreduce
	callasm ClearCalculatedSpreadMoveData @;So the damage can be calculated
	goto TwoTurnMovesSecondTurnBS

BattleScript_SolarbeamDecideTurn::
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_TwoTurnMovesSecondTurn
	jumpifword CMP_COMMON_BITS, gHitMarker, HITMARKER_NO_ATTACKSTRING, BattleScript_TwoTurnMovesSecondTurn
	setbyte sTWOTURN_STRINGID, 1
	call BattleScriptFirstChargingTurn
	goto BattleScript_MoveEnd

BattleScript_SolarbeamOnFirstTurn::
	orword gHitMarker, HITMARKER_CHARGING
	setmoveeffect MOVE_EFFECT_CHARGING | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	ppreduce
	goto BattleScript_TwoTurnMovesSecondTurn

BattleScript_EffectThunder::
	orword HIT_MARKER HITMARKER_IGNORE_ON_AIR
	jumpifmove MOVE_HURRICANE BS_076_SetConfusionChance
	setmoveeffect MOVE_EFFECT_PARALYSIS
	goto BattleScript_HitFromAtkCanceler	

BattleScript_EffectBeatUp::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	addbyte gBattleCommunication 0x1
	goto BattleScript_HitFromAtkString
BattleScript_BeatUpLoop::
	movevaluescleanup
	trydobeatup BattleScript_BeatUpEnd, BattleScript_ButItFailed
	printstring STRINGID_PKMNATTACK
	critcalc
	jumpifbyte CMP_NOT_EQUAL, gCritMultiplier, 2, BattleScript_BeatUpAttack
	manipulatedamage 2
BattleScript_BeatUpAttack::
	adjustnormaldamage
	attackanimation
	waitanimation
	effectivenesssound
	hitanimation BS_TARGET
	waitstate
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	critmessage
	waitmessage 0x40
	resultmessage
	waitmessage 0x40
	tryfaintmon BS_TARGET, 0, NULL
	moveendto 16
	goto BattleScript_BeatUpLoop

BattleScript_BeatUpEnd::
	end

BattleScript_EffectSemiInvulnerable::
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_SecondTurnSemiInvulnerable
	jumpifword CMP_COMMON_BITS, gHitMarker, HITMARKER_NO_ATTACKSTRING, BattleScript_SecondTurnSemiInvulnerable
	jumpifmove MOVE_FLY, BattleScript_FlyFirstTurn
	jumpifmove MOVE_DIVE, BattleScript_DiveFirstTurn
	jumpifmove MOVE_BOUNCE, BattleScript_BounceFirstTurn
	@ MOVE_DIG
	setbyte sTWOTURN_STRINGID, 5
	goto BattleScript_FirstTurnSemiInvulnerable

BattleScript_BounceFirstTurn::
	setbyte sTWOTURN_STRINGID, 7
	goto BattleScript_FirstTurnSemiInvulnerable

BattleScript_DiveFirstTurn::
	setbyte sTWOTURN_STRINGID, 6
	goto BattleScript_FirstTurnSemiInvulnerable

BattleScript_FlyFirstTurn::
	setbyte sTWOTURN_STRINGID, 4
BattleScript_FirstTurnSemiInvulnerable::
	call BattleScriptFirstChargingTurn
	setsemiinvulnerablebit
	goto BattleScript_MoveEnd

BattleScript_SecondTurnSemiInvulnerable::
	attackcanceler
	setmoveeffect MOVE_EFFECT_CHARGING
	setbyte sB_ANIM_TURN, 1
	clearstatusfromeffect BS_ATTACKER
	orword gHitMarker, HITMARKER_NO_PPDEDUCT
	jumpifnotmove MOVE_BOUNCE, BattleScript_SemiInvulnerableTryHit
	setmoveeffect MOVE_EFFECT_PARALYSIS
BattleScript_SemiInvulnerableTryHit::
	accuracycheck BattleScript_SemiInvulnerableMiss, ACC_CURR_MOVE
	clearsemiinvulnerablebit
	goto BattleScript_HitFromAtkString

BattleScript_SemiInvulnerableMiss::
	clearsemiinvulnerablebit
	goto BattleScript_PrintMoveMissed

BattleScript_EffectFly::
	jumpifstatus2 BS_ATTACKER STATUS2_MULTIPLETURNS BS_SecondTurnSemiInvulnerable
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING BS_SecondTurnSemiInvulnerable
	jumpifmove MOVE_FLY BS_FirstTurnFly
	jumpifmove MOVE_DIVE BS_FirstTurnDive
	jumpifmove MOVE_BOUNCE, BS_FirstTurnBounce
	jumpifmove MOVE_DIG BS_FirstTurnDig
	goto BS_FirstTurnSemiInvulnerable

BS_FirstTurnBounce::
	setbyte sTWOTURN_STRINGID 0x7
	goto BS_FirstTurnSemiInvulnerable

BS_FirstTurnDive::
	setbyte sTWOTURN_STRINGID 0x6
	goto BS_FirstTurnSemiInvulnerable

BS_FirstTurnFly::
	setbyte sTWOTURN_STRINGID 0x4
	goto BS_FirstTurnSemiInvulnerable

BS_FirstTurnDig::
	setbyte sTWOTURN_STRINGID 0x5
	
BS_FirstTurnSemiInvulnerable::
	call BattleScript_FirstChargingTurn
	call BattleScript_CheckPowerHerb
	setsemiinvulnerablebit
	goto BattleScript_MoveEnd

BS_SecondTurnSemiInvulnerable::
	attackcanceler
	setmoveeffect MOVE_EFFECT_CHARGING
	setbyte sB_ANIM_TURN, 0x1
	clearstatusfromeffect BS_ATTACKER
	orword HIT_MARKER HITMARKER_NO_PPDEDUCT
	jumpifmove MOVE_SKYDROP SkyDropDropBS
	jumpifnotmove MOVE_BOUNCE BS_SemiInvulnerableTryHit
	setmoveeffect MOVE_EFFECT_PARALYSIS

BS_SemiInvulnerableTryHit::
	clearsemiinvulnerablebit
	goto BattleScript_HitFromAccCheck

BattleScript_EffectDefenseCurl::
	attackcanceler
	attackstring
	ppreduce
	setdefensecurlbit
	setstatchanger2 STAT_DEF | INCREASE_1
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_DefenseCurlDoStatUpAnim
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_StatUpPrintString
	attackanimation
	waitanimation
BattleScript_DefenseCurlDoStatUpAnim::
	goto BattleScript_StatUpDoAnim

BattleScript_EffectSoftboiled::
	goto BattleScript_HitFromAtkCanceler

BattleScript_PresentHealTarget::
	attackanimation
	waitanimation
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printstring STRINGID_PKMNREGAINEDHEALTH
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_AlreadyAtFullHp::
	pause 0x20
	printstring STRINGID_PKMNHPFULL
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectFakeOut::
	attackcanceler
	jumpifnotfirstturn BattleScript_ButItFailedAtkStringPpReduce
	jumpifmove MOVE_FIRSTIMPRESSION BattleScript_HitFromAtkCanceler
	setmoveeffect MOVE_EFFECT_FLINCH | MOVE_EFFECT_CERTAIN
	goto BattleScript_EffectHit

BattleScript_ButItFailedAtkStringPpReduce::
	attackstring
BattleScript_ButItFailedPpReduce::
	ppreduce
	pause 0x20
	orbyte gMoveResultFlags, MOVE_RESULT_FAILED
	resultmessage
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_NotAffected::
	pause 0x20
	orbyte gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE
	resultmessage
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectUproar::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed, ACC_CURR_MOVE
	setmoveeffect MOVE_EFFECT_UPROAR | MOVE_EFFECT_AFFECTS_USER
	attackstring
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_UproarHit
	ppreduce
BattleScript_UproarHit::
	nop
	goto BattleScript_HitFromCritCalc

BattleScript_EffectStockpile::
	attackcanceler
	attackstring
	ppreduce
	stockpile
	attackanimation
	waitanimation
	printfromtable gStockpileUsedStringIds
	waitmessage 0x40
	jumpifmovehadnoeffect BattleScript_MoveEnd

StockpileRaiseDef::
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN StockpileRaiseSpDef
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 StockpileRaiseSpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

StockpileRaiseSpDef::
	setstatchanger2 STAT_SPDEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectSpitUp::
	attackcanceler
	jumpifbyte CMP_EQUAL, gBattleCommunication + 6, 1, BattleScript_SpitUpNoDamage
	attackstringnoprotean
	ppreduce
	accuracycheck BattleScript_PrintMoveMissed, ACC_CURR_MOVE
	stockpiletobasedamage BattleScript_SpitUpFail
	tryactivateprotean
	call STANDARD_DAMAGE2
	seteffectwithchance
	prefaintmoveendeffects 0x0
	faintpokemonaftermove

SpitUpCompareStats::
	jumpifstat BS_TARGET GREATERTHAN STAT_DEF STAT_MIN SpitUpLowerDef
	jumpifstat BS_TARGET EQUALS STAT_SPDEF STAT_MIN StockpileWearOff

SpitUpLowerDef::
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_DOWN | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_DEF
	callasm ModifyPostStockpileBoostDecrement
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN SpitUpLowerSpDef
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 SpitUpLowerSpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	
SpitUpLowerSpDef::
	setstatchanger2 STAT_SPDEF
	callasm ModifyPostStockpileBoostDecrement
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN StockpileWearOff
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 StockpileWearOff
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

StockpileWearOff::
	callasm RemoveStockpileBoosts
	setword BATTLE_STRING_LOADER StockpileWoreOffString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_SpitUpFail::
	pause 0x20
	printstring STRINGID_FAILEDTOSPITUP
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_SpitUpNoDamage::
	attackstring
	ppreduce
	pause 0x40
	stockpiletobasedamage BattleScript_SpitUpFail
	resultmessage
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectSwallow::
	attackcanceler
	attackstringnoprotean
	ppreduce
	stockpiletohpheal BattleScript_SwallowFail
	tryactivateprotean
	attackanimation
	waitanimation
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring 0x4B
	waitmessage DELAY_1SECOND
	goto SpitUpCompareStats

BattleScript_SwallowFail::
	pause 0x20
	printfromtable gSwallowFailStringIds
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectHail::
	attackcanceler
	attackstringnoprotean
	ppreduce
	sethail
	tryactivateprotean
	goto BattleScript_MoveWeatherChange

BattleScript_EffectTorment::
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL BattleScript_ProtectedByAromaVeil
	settorment FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_PKMNSUBJECTEDTOTORMENT
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectFlatter::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET BattleScript_MakeMoveMissed
	accuracycheck BattleScript_PrintMoveMissed 0x0
	jumpifconfusedandstatmaxed STAT_SPATK FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setstatchanger2 STAT_SPATK | INCREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR SwaggerTryConfuseBS
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 SwaggerTryConfuseBS
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto SwaggerTryConfuseBS
BattleScript_FlatterTryConfuse::
	jumpifability BS_TARGET, ABILITY_OWN_TEMPO, BattleScript_OwnTempoPrevents
	jumpifsideaffecting BS_TARGET, SIDE_STATUS_SAFEGUARD, BattleScript_SafeguardProtected
	setmoveeffect MOVE_EFFECT_CONFUSION
	seteffectprimary
	goto BattleScript_MoveEnd

BattleScript_EffectSetBurn::
	attackcanceler
	attackstring @;Protean always activates!
	ppreduce
	jumpifbehindsubstitute BS_TARGET FAILED
	trysetburn BS_TARGET BS_StatusMoveFail
	accuracycheck BattleScript_MoveMissedPause 0x0
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_BURN
	seteffectprimary
	goto BattleScript_MoveEnd

BattleScript_EffectWillOWisp::
	attackcanceler
	attackstring
	ppreduce
	jumpifstatus2 BS_TARGET, STATUS2_SUBSTITUTE, BattleScript_ButItFailed
	jumpifstatus BS_TARGET, STATUS1_BURN, BattleScript_AlreadyBurned
	jumpiftype BS_TARGET, TYPE_FIRE, BattleScript_NotAffected
	jumpifability BS_TARGET, ABILITY_WATER_VEIL, BattleScript_WaterVeilPrevents
	jumpifstatus BS_TARGET, STATUS1_ANY, BattleScript_ButItFailed
	accuracycheck BattleScript_ButItFailed, ACC_CURR_MOVE
	jumpifsideaffecting BS_TARGET, SIDE_STATUS_SAFEGUARD, BattleScript_SafeguardProtected
	attackanimation
	waitanimation
	setmoveeffect MOVE_EFFECT_BURN
	seteffectprimary
	goto BattleScript_MoveEnd

BattleScript_WaterVeilPrevents::
	copybyte gEffectBattler, gBattlerTarget
	setbyte cMULTISTRING_CHOOSER, 0
	call BattleScript_BRNPrevention
	goto BattleScript_MoveEnd

BattleScript_AlreadyBurned::
	pause 0x20
	printstring STRINGID_PKMNALREADYHASBURN
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectMemento::
	attackcanceler
	jumpifmove MOVE_FINALGAMBIT FinalGambitBS
	jumpifmove MOVE_HEALINGWISH HealingWishBS
	jumpifmove MOVE_LUNARDANCE HealingWishBS
	
MementoBS::
	jumpifbyte EQUALS, gBattleCommunication + 6, 0x1, BattleScript_MementoNoReduceStats
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK | STAT_ANIM_SPATK, STAT_ANIM_DOWN | STAT_ANIM_BY_TWO | STAT_ANIM_ONLY_MULTIPLE
	
Memento_Atk::
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK, STAT_ANIM_DOWN | STAT_ANIM_BY_TWO
	setstatchanger2 STAT_ATK | DECREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR MementoPrintAtkMsg
MementoPrintAtkMsg::
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x3 Memento_SpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x4 MementoFaintUser
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

Memento_SpAtk::
	playstatchangeanimation BS_TARGET, STAT_ANIM_SPATK, STAT_ANIM_DOWN | STAT_ANIM_BY_TWO
	setstatchanger2 STAT_SPATK | DECREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR MementoPrintSpAtkMsg
MementoPrintSpAtkMsg::
	jumpifbyte GREATERTHAN cMULTISTRING_CHOOSER 0x2 MementoFaintUser
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	
MementoFaintUser::
	setatkhptozero
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER 0x0 0x0
	goto BattleScript_MoveEnd
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

HealingWishBS::
	jumpifcantswitch BS_ATTACKER | ATK4F_DONT_CHECK_STATUSES FAILED_PRE
	attackstring
	ppreduce
	setatkhptozero
	attackanimation
	waitanimation
	callasm SetHealingWishLunarDanceFunc
	setatkhptozero
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER 0x0 0x0
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

FinalGambitBS::
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	typecalc2
	bicbyte gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	callasm FinalGambitDamageCalc
	adjustsetdamage
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
	seteffectwithchance
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	jumpifmovehadnoeffect BattleScript_MoveEnd
	setatkhptozero
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER 0x0 0x0
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MementoSkipStatDown1::
	playstatchangeanimation BS_TARGET, BIT_SPATK, ATK48_STAT_NEGATIVE | ATK48_STAT_BY_TWO
	setstatchanger STAT_SPATK, 2, TRUE
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_MementoSkipStatDown2
	jumpifbyte CMP_GREATER_THAN, cMULTISTRING_CHOOSER, 1, BattleScript_MementoSkipStatDown2
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_MementoSkipStatDown2::
	tryfaintmon BS_ATTACKER, 0, NULL
	goto BattleScript_MoveEnd

BattleScript_MementoSubstituteInvulnerable::
	printstring STRINGID_BUTNOEFFECT
	waitmessage 0x40
	goto BattleScript_MementoSkipStatDown2

BattleScript_MementoNoReduceStats::
	attackstring
	ppreduce
	jumpifattackandspecialattackcannotfall BattleScript_MementoNoReduceStatsEnd
BattleScript_MementoNoReduceStatsEnd::
	setatkhptozero
	pause 0x40
	effectivenesssound
	resultmessage
	waitmessage 0x40
	tryfaintmon BS_ATTACKER, 0, NULL
	goto BattleScript_MoveEnd

BattleScript_EffectFacade::
	goto BattleScript_EffectHit

BattleScript_FacadeDoubleDmg::
	setbyte sDMG_MULTIPLIER, 2
	goto BattleScript_EffectHit

BattleScript_EffectFocusPunch::
	jumpifmove MOVE_BEAKBLAST BeakBlastDoBS
	jumpifmove MOVE_SHELLTRAP ShellTrapDoBS
	goto BattleScript_EffectFocusPunch2
	
BeakBlastDoBS::
	callasm ClearBeakBlastBit
	goto BattleScript_HitFromAtkCanceler

ShellTrapDoBS::
	attackcanceler
	callasm TryLaunchShellTrap
	goto BattleScript_HitFromAccCheck
BattleScript_EffectFocusPunch2::
	attackcanceler
	jumpifnodamage BattleScript_HitFromAccCheck
	ppreduce
	printstring STRINGID_PKMNLOSTFOCUS
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectSmellingsalt::
	jumpifbehindsubstitute BS_TARGET BattleScript_HitFromAtkCanceler
	setmoveeffect MOVE_EFFECT_REMOVE_PARALYSIS | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkCanceler

BattleScript_SmellingSaltBuff::
	setbyte sDMG_MULTIPLIER, 2
	goto BattleScript_EffectHit

BattleScript_EffectFollowMe::
	attackcanceler
	attackstring
	ppreduce
	setforcedtarget
	attackanimation
	waitanimation
	printstring STRINGID_PKMNCENTERATTENTION
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectNaturePower::
	attackcanceler
	attackstring
	pause 0x20
	callterrainattack
	printstring STRINGID_NATUREPOWERTURNEDINTO
	waitmessage 0x40
	return

BattleScript_EffectCharge::
	attackcanceler
	attackstring
	ppreduce
	setcharge
	attackanimation
	waitanimation
	printstring STRINGID_PKMNCHARGINGPOWER
	waitmessage 0x40
	setmoveeffect MOVE_EFFECT_SP_DEF_PLUS_1 | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	goto BattleScript_MoveEnd

BattleScript_EffectTaunt::
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	jumpifability BS_TARGET ABILITY_OBLIVIOUS BattleScript_ObliviousPrevents
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL BattleScript_ProtectedByAromaVeil
	settaunt FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_PKMNFELLFORTAUNT
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectHelpingHand::
	attackcanceler
	attackstringnoprotean
	ppreduce
	trysethelpinghand FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_PKMNREADYTOHELP @;STRINGID_PKMNREADYTOHELP
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectTrick::
	jumpifmove MOVE_BESTOW BestowBS

TrickBS::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	tryswapitems FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_PKMNSWITCHEDITEMS @;STRINGID_PKMNSWITCHEDITEMS
	waitmessage DELAY_1SECOND
	printfromtable gItemSwapStringIds @;gItemSwapStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_HandleWeatherFormChanges @;BattleScript_WeatherFormChanges - In case of Utility Umbrella
	goto BattleScript_MoveEnd

BestowBS::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	callasm BestowItem
	callasm TransferLastUsedItem
	tryactivateprotean
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER BestowString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_HandleWeatherFormChanges @;BattleScript_WeatherFormChanges - In case of Utility Umbrella
	goto BattleScript_MoveEnd

BattleScript_EffectRolePlay::
	attackcanceler
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	trycopyability FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	copyarray sBATTLER gBattlerAttacker 0x1
	playanimation BS_SCRIPTING B_ANIM_LOAD_ABILITY_POP_UP 0x0
	call BattleScript_AbilityPopUpRevert
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	printstring STRINGID_PKMNCOPIEDFOE @;STRINGID_PKMNCOPIEDFOE
	waitmessage DELAY_1SECOND
	copybyte sBATTLER gBattlerAttacker
	call BattleScript_TryRemoveIllusion
	callasm TryRemovePrimalWeatherAfterAbilityChange
	call BattleScript_HandleWeatherFormChanges @;Try to revert Cherrim and Castform
	tryactivateswitchinability BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_EffectWish::
	attackcanceler
	attackstringnoprotean
	ppreduce
	trywish 0, BattleScript_ButItFailed
	tryactivateprotean
	attackanimation
	waitanimation
	goto BattleScript_MoveEnd

BattleScript_EffectAssist::
	attackcanceler
	attackstringnoprotean
	assistattackselect BattleScript_ButItFailedPpReduce
	attackanimation
	waitanimation
	setbyte sB_ANIM_TURN, 0
	setbyte sB_ANIM_TARGETS_HIT, 0
	jumptocalledmove 1
BattleScript_EffectIngrain::
	attackcanceler
	jumpifmove MOVE_AQUARING AquaRingBS

IngrainBS::
	attackcanceler
	attackstringnoprotean
	ppreduce
	trysetroots FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xB3
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
AquaRingBS::
	jumpifstatus3condition BS_ATTACKER STATUS3_AQUA_RING 0x0 FAILED_PRE
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER AquaRingSetString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	setspecialstatusbit BS_ATTACKER STATUS3_AQUA_RING
	goto BattleScript_MoveEnd

BattleScript_EffectSuperpower::
	jumpifhalfword EQUALS gCurrentMove MOVE_CLOSECOMBAT CloseCombatBS
	jumpifhalfword EQUALS gCurrentMove MOVE_DRAGONASCENT CloseCombatBS
	jumpifhalfword EQUALS gCurrentMove MOVE_HAMMERARM HammerArmBS
	jumpifhalfword EQUALS gCurrentMove MOVE_ICEHAMMER HammerArmBS
	jumpifhalfword EQUALS gCurrentMove MOVE_CLANGINGSCALES ClangingScalesBS
	jumpifhalfword EQUALS gCurrentMove MOVE_VCREATE VCreateBS
	jumpifhalfword EQUALS gCurrentMove MOVE_HYPERSPACEHOLE HyperspaceHoleBS
	jumpifhalfword EQUALS gCurrentMove MOVE_HYPERSPACEFURY HyperspaceFuryBS
	setmoveeffect MOVE_EFFECT_ATK_DEF_DOWN | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkCanceler

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

CloseCombatBS::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	jumpifstat BS_ATTACKER GREATERTHAN STAT_DEF STAT_MIN CC_LowerDef
	jumpifstat BS_ATTACKER EQUALS STAT_SPDEF STAT_MIN BS_MOVE_FAINT

CC_LowerDef::
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_DOWN | STAT_ANIM_IGNORE_ABILITIES
CC_SkipTo::
	setstatchanger2 STAT_DEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN CC_LowerSpDef
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 CC_LowerSpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

CC_LowerSpDef::
	setstatchanger2 STAT_SPDEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BS_MOVE_FAINT
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BS_MOVE_FAINT
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

HammerArmBS::
	setmoveeffect MOVE_EFFECT_SPD_MINUS_1 | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkCanceler

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ClangingScalesBS::
	setmoveeffect MOVE_EFFECT_DEF_MINUS_1 | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkCanceler

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

VCreateBS::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	jumpifstat BS_ATTACKER GREATERTHAN STAT_SPEED STAT_MIN VC_LowerSpd
	jumpifstat BS_ATTACKER GREATERTHAN STAT_DEF STAT_MIN VC_LowerSpd
	jumpifstat BS_ATTACKER EQUALS STAT_SPDEF STAT_MIN BS_MOVE_FAINT

VC_LowerSpd::
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF | STAT_ANIM_SPD, STAT_ANIM_DOWN | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_SPEED| DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN VC_LowerDef
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 VC_LowerDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

VC_LowerDef::
	setbyte sSTAT_ANIM_PLAYED 0x0
	goto CC_SkipTo

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

HyperspaceHoleBS::
	attackcanceler
	goto FeintSkipBS

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

HyperspaceFuryBS::
	attackcanceler
	setmoveeffect MOVE_EFFECT_DEF_MINUS_1 | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto FeintSkipBS

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectMagicCoat::
	attackcanceler
	attackstringnoprotean
	ppreduce
	trysetmagiccoat BattleScript_ButItFailedAtkStringPpReduce
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_PKMNSHROUDEDITSELF
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectRecycle::
	attackcanceler
	jumpifnotmove MOVE_BELCH RecycleBS
	callasm BelchFunction
	goto BattleScript_HitFromAccCheck
	
RecycleBS:
	jumpifcounter BS_ATTACKER INCINERATE_COUNTERS NOTEQUALS 0x0 FAILED_PRE
	attackstringnoprotean
	ppreduce
	tryrecycleitem FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x140
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectRevenge::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectBrickBreak::
	setmoveeffect MOVE_EFFECT_BREAK_SCREENS
	removelightscreenreflect @;For the anim only
	goto BattleScript_HitFromAtkCanceler
BattleScript_BrickBreakAnim::
	attackanimation
	waitanimation
	jumpifbyte CMP_LESS_THAN, sB_ANIM_TURN, 2, BattleScript_BrickBreakDoHit
	printstring STRINGID_THEWALLSHATTERED
	waitmessage 0x40
BattleScript_BrickBreakDoHit::
	typecalc2
	effectivenesssound
	hitanimation BS_TARGET
	waitstate
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	critmessage
	waitmessage 0x40
	resultmessage
	waitmessage 0x40
	seteffectwithchance
	tryfaintmon BS_TARGET, 0, NULL
	goto BattleScript_MoveEnd

BattleScript_EffectYawn::
	attackcanceler
	attackstring
	ppreduce
	jumpifbehindsubstitute BS_TARGET FAILED
	setyawn BS_StatusMoveFail
	accuracycheck FAILED 0x0
	callasm ActuallySetYawn
	attackanimation
	waitanimation
	printstring STRINGID_PKMNWASMADEDROWSY
	waitmessage 0x40
	goto BattleScript_MoveEnd


BattleScript_PrintBankAbilityMadeIneffective::
	copybyte sBATTLER, sBATTLER_WITH_ABILITY
BattleScript_PrintAbilityMadeIneffective::
	pause 0x20
	printstring STRINGID_PKMNSXMADEITINEFFECTIVE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectKnockOff::
	setmoveeffect MOVE_EFFECT_KNOCK_OFF
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectEndeavor::
	attackcanceler
	attackstring
	ppreduce
	setdamagetohealthdifference BattleScript_ButItFailed
	copyword gHpDealt, gBattleMoveDamage
	accuracycheck BattleScript_MoveMissedPause, ACC_CURR_MOVE
	typecalc2
	jumpifmovehadnoeffect BattleScript_HitFromAtkAnimation
	bicbyte gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	copyword gBattleMoveDamage, gHpDealt
	adjustsetdamage
	goto BattleScript_HitFromAtkAnimation

BattleScript_EffectEruption::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectSkillSwap::
	jumpifmove MOVE_WORRYSEED WorrySeedBS
	jumpifmove MOVE_GASTROACID GastroAcidBS
	jumpifmove MOVE_ENTRAINMENT EntrainmentBS
	jumpifmove MOVE_COREENFORCER CoreEnforcerBS
	jumpifmove MOVE_SIMPLEBEAM SimpleBeamBS
	
SkillSwapBS::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstringnoprotean
	ppreduce
	tryswapabilities FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	copyarray sBATTLER gBattlerAttacker 0x1
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert

	copyarray sBATTLER TARGET_BANK 0x1
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	
	printstring 0xB8
	waitmessage DELAY_1SECOND
	tryactivateswitchinability BS_ATTACKER
	tryactivateswitchinability BS_TARGET
	goto BattleScript_MoveEnd

GastroAcidBS::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstringnoprotean
	ppreduce
	callasm AbilityChangeBSFunc
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	copybyte sBATTLER TARGET_BANK
	call BattleScript_TryRemoveIllusion
	callasm TryRemovePrimalWeatherAfterAbilityChange
	call BattleScript_HandleWeatherFormChanges @;Try to revert Cherrim and Castform
	goto BattleScript_MoveEnd

WorrySeedBS::
EntrainmentBS::
SimpleBeamBS::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstringnoprotean
	ppreduce
	callasm AbilityChangeBSFunc
	tryactivateprotean
	attackanimation
	waitanimation

	playanimation BS_TARGET B_ANIM_LOAD_ABILITY_POP_UP 0x0
	call BattleScript_AbilityPopUpRevert
	copyarray sBATTLER TARGET_BANK 0x1
	call BattleScript_AbilityPopUp
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert

	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	copyarray sBATTLER TARGET_BANK 0x1
	call BattleScript_TryRemoveIllusion
	callasm TryRemovePrimalWeatherAfterAbilityChange
	call BattleScript_HandleWeatherFormChanges @;Try to revert Cherrim and Castform
	tryactivateswitchinability BS_TARGET
	goto BattleScript_MoveEnd

CoreEnforcerBS::
	setmoveeffect MOVE_EFFECT_SUPPRESS_ABILITY | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkCanceler


BattleScript_EffectImprison::
	attackcanceler
	attackstringnoprotean
	ppreduce
	tryimprison BattleScript_ButItFailed
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_PKMNSEALEDOPPONENTMOVE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectRefresh::
	attackcanceler
	jumpifmove MOVE_PSYCHOSHIFT PsychoShiftBS
	
RefreshBS::
	attackstringnoprotean
	ppreduce
	cureifburnedparalysedorpoisoned FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0xA7 @;STRINGID_PKMNSTATUSNORMAL
	waitmessage DELAY_1SECOND
	updatestatusicon BS_ATTACKER
	goto BattleScript_MoveEnd
	
PsychoShiftBS::
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	jumpifstatus BS_TARGET, STATUS1_ANY, FAILED_PRE
	accuracycheck FAILED_PRE 0x0
	attackstringnoprotean
	ppreduce
	jumpifsideaffecting BS_TARGET SIDE_SAFEGUARD BattleScript_SafeguardProtected @;Protected By Safeguard
	callasm LoadStatustoPsychoShiftTransfer
	cureprimarystatus BS_ATTACKER FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	seteffectprimary
	updatestatusicon BS_ATTACKER
	setword BATTLE_STRING_LOADER PsychoShiftString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectGrudge::
	attackcanceler
	attackstringnoprotean
	ppreduce
	trysetgrudge FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_PKMNWANTSGRUDGE
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectSnatch::
	attackcanceler
	attackstringnoprotean
	ppreduce
	trysetsnatch FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	pause DELAY_HALFSECOND
	printstring STRINGID_PKMNWAITSFORTARGET
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_EffectLowKick::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectSecretPower::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectDoubleEdge::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectTeeterDance::
	goto BattleScript_HitFromAtkCanceler

BattleScript_TeeterDanceLoop::
	movevaluescleanup
	setmoveeffect MOVE_EFFECT_CONFUSION
	jumpifbyteequal gBattlerAttacker, gBattlerTarget, BattleScript_TeeterDanceLoopIncrement
	jumpifability BS_TARGET, ABILITY_OWN_TEMPO, BattleScript_TeeterDanceOwnTempoPrevents
	jumpifstatus2 BS_TARGET, STATUS2_SUBSTITUTE, BattleScript_TeeterDanceSubstitutePrevents
	jumpifstatus2 BS_TARGET, STATUS2_CONFUSION, BattleScript_TeeterDanceAlreadyConfused
	accuracycheck BattleScript_TeeterDanceMissed, ACC_CURR_MOVE
	jumpifsideaffecting BS_TARGET, SIDE_STATUS_SAFEGUARD, BattleScript_TeeterDanceSafeguardProtected
	attackanimation
	waitanimation
	seteffectprimary
	resultmessage
	waitmessage 0x40
BattleScript_TeeterDanceLoopIncrement::
	moveendto 16
	addbyte gBattlerTarget, 1
	jumpifbytenotequal gBattlerTarget, gBattlersCount, BattleScript_TeeterDanceLoop
	end

BattleScript_TeeterDanceOwnTempoPrevents::
	pause 0x20
	printstring STRINGID_PKMNPREVENTSCONFUSIONWITH
	waitmessage 0x40
	goto BattleScript_TeeterDanceLoopIncrement

BattleScript_TeeterDanceSafeguardProtected::
	pause 0x20
	printstring STRINGID_PKMNUSEDSAFEGUARD
	waitmessage 0x40
	goto BattleScript_TeeterDanceLoopIncrement

BattleScript_TeeterDanceSubstitutePrevents::
	pause 0x20
	printstring STRINGID_BUTITFAILED
	waitmessage 0x40
	goto BattleScript_TeeterDanceLoopIncrement

BattleScript_TeeterDanceAlreadyConfused::
	pause 0x20
	printstring STRINGID_PKMNALREADYCONFUSED
	waitmessage 0x40
	goto BattleScript_TeeterDanceLoopIncrement

BattleScript_TeeterDanceMissed::
	resultmessage
	waitmessage 0x40
	goto BattleScript_TeeterDanceLoopIncrement

BattleScript_EffectMudSport::
BattleScript_EffectWaterSport::
	attackcanceler
	attackstringnoprotean
	ppreduce
	settypebasedhalvers BattleScript_ButItFailed
	attackanimation
	waitanimation
	printfromtable gSportsUsedStringIds
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectPoisonFang::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectWeatherBall::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectOverheat::
	setmoveeffect MOVE_EFFECT_SP_ATK_TWO_DOWN | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectTickle::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	setbyte sSTAT_ANIM_PLAYED 0x0
	jumpifstatcanbelowered BS_TARGET STAT_ATK Tickle_Atk
	jumpifstatcanbelowered BS_TARGET STAT_DEF Tickle_Atk
	pause 0x10
	goto Tickle_SkipAnim

Tickle_Atk::
	attackanimation
	waitanimation
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK | STAT_ANIM_DEF, STAT_ANIM_DOWN | STAT_ANIM_ONLY_MULTIPLE
	
Tickle_SkipAnim::
	setbyte FORM_COUNTER 0x0
	playstatchangeanimation BS_TARGET, STAT_ANIM_ATK STAT_ANIM_DOWN
	setstatchanger2 STAT_ATK | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR Tickle_Def
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x3 Tickle_Def
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x4 BattleScript_MoveEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

Tickle_Def::
	playstatchangeanimation BS_TARGET, STAT_ANIM_DEF STAT_ANIM_DOWN
	setstatchanger2 STAT_DEF | DECREASE_1
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_MoveEnd
	jumpifbyte GREATERTHAN cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TickleDoMoveAnim::
	accuracycheck BattleScript_ButItFailed, ACC_CURR_MOVE
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_TARGET, BIT_ATK | BIT_DEF, ATK48_STAT_NEGATIVE | ATK48_ONLY_MULTIPLE
	playstatchangeanimation BS_TARGET, BIT_ATK, ATK48_STAT_NEGATIVE
	setstatchanger STAT_ATK, 1, TRUE
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_TickleTryLowerDef
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_TickleTryLowerDef
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_TickleTryLowerDef::
	playstatchangeanimation BS_TARGET, BIT_DEF, ATK48_STAT_NEGATIVE
	setstatchanger STAT_DEF, 1, TRUE
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_TickleEnd
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_TickleEnd
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_TickleEnd::
	goto BattleScript_MoveEnd

BattleScript_CantLowerMultipleStats::
	pause 0x20
	orbyte gMoveResultFlags, MOVE_RESULT_FAILED
	printstring STRINGID_STATSWONTDECREASE2
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectCosmicPower::
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BS_TARGET LESSTHAN STAT_DEF STAT_MAX CosmicPower_Def
	jumpifstat BS_TARGET EQUALS STAT_SPDEF STAT_MAX BattleScript_CantRaiseMultipleStats

CosmicPower_Def::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN CosmicPower_SpDef
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 CosmicPower_SpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

CosmicPower_SpDef::
	setstatchanger2 STAT_SPDEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectExtremeEvoBoost::
	attackcanceler
	jumpifmove MOVE_NORETREAT NoRetreatBS
	attackstring
	ppreduce
	jumpifmove MOVE_CLANGOROUSSOUL ClangorousSoulBS
	jumpifstat BS_ATTACKER LESSTHAN STAT_ATK STAT_MAX ExtremeEvoboost_Atk
	jumpifstat BS_ATTACKER LESSTHAN STAT_DEF STAT_MAX ExtremeEvoboost_Atk
	jumpifstat BS_ATTACKER LESSTHAN STAT_SPEED STAT_MAX ExtremeEvoboost_Atk
	jumpifstat BS_ATTACKER LESSTHAN STAT_SPATK STAT_MAX ExtremeEvoboost_Atk
	jumpifstat BS_ATTACKER EQUALS STAT_SPDEF STAT_MAX BattleScript_CantRaiseMultipleStats

ExtremeEvoboost_Atk::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_DEF | STAT_ANIM_SPD | STAT_ANIM_SPATK | STAT_ANIM_SPDEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
	setstatchanger2 STAT_ATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ExtremeEvoboost_Def
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 ExtremeEvoboost_Def
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ExtremeEvoboost_Def::
	setstatchanger2 STAT_DEF | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ExtremeEvoboost_SpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 ExtremeEvoboost_SpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ExtremeEvoboost_SpAtk::
	setstatchanger2 STAT_SPATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ExtremeEvoboost_SpDef
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 ExtremeEvoboost_SpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ExtremeEvoboost_SpDef::
	setstatchanger2 STAT_SPDEF | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ExtremeEvoboost_Spd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 ExtremeEvoboost_Spd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ExtremeEvoboost_Spd::
	setstatchanger2 STAT_SPEED| INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ClangorousSoulBS::
	callasm FailClangorousSoulIfLowHP
	jumpifstat BS_ATTACKER LESSTHAN STAT_ATK STAT_MAX ClangorusSoul_Atk
	jumpifstat BS_ATTACKER LESSTHAN STAT_DEF STAT_MAX ClangorusSoul_Atk
	jumpifstat BS_ATTACKER LESSTHAN STAT_SPEED STAT_MAX ClangorusSoul_Atk
	jumpifstat BS_ATTACKER LESSTHAN STAT_SPATK STAT_MAX ClangorusSoul_Atk
	jumpifstat BS_ATTACKER EQUALS STAT_SPDEF STAT_MAX BattleScript_CantRaiseMultipleStats

ClangorusSoul_Atk::
	attackanimation
	waitanimation
	call BattleScript_AllStatsUp
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	setdamagetobankhealthfraction BS_ATTACKER 3 0x0 @;33 % of Base Max HP
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER 0x0 0x0
	goto BattleScript_MoveEnd

NoRetreatBS::
	callasm FailIfTrappedByNoRetreat
	attackstring
	ppreduce
	attackanimation
	waitanimation
	call BattleScript_AllStatsUp
	setmoveeffect MOVE_EFFECT_PREVENT_ESCAPE | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	jumpifstatus2 BS_ATTACKER STATUS2_ESCAPE_PREVENTION PrintNoRetreatMessage @;May not be affected if Ghost or already trapped
	goto BattleScript_MoveEnd

PrintNoRetreatMessage::
	setword BATTLE_STRING_LOADER gText_NoRetreatTrapped
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_CosmicPowerDoMoveAnim::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_DEF | BIT_SPDEF, 0
	setstatchanger STAT_DEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CosmicPowerTrySpDef
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_CosmicPowerTrySpDef
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_CosmicPowerTrySpDef::
	setstatchanger STAT_SPDEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CosmicPowerEnd
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_CosmicPowerEnd
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_CosmicPowerEnd::
	goto BattleScript_MoveEnd

BattleScript_EffectSkyUppercut::
	orword gHitMarker, HITMARKER_IGNORE_ON_AIR
	goto BattleScript_EffectHit

BattleScript_EffectBulkUp::
	attackcanceler
	attackstring
	ppreduce
	jumpifmove MOVE_COIL CoilBS
	
BulkUpBS::
	jumpifstat BS_TARGET LESSTHAN STAT_ATK STAT_MAX BulkUp_Atk
	jumpifstat BS_TARGET EQUALS STAT_DEF STAT_MAX BattleScript_CantRaiseMultipleStats

BulkUp_Atk::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_DEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BulkUp_Def
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BulkUp_Def
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

BulkUp_Def::
	setstatchanger2 STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

CoilBS::
	jumpifstat BS_ATTACKER LESSTHAN STAT_ATK STAT_MAX Coil_Atk
	jumpifstat BS_ATTACKER LESSTHAN STAT_DEF STAT_MAX Coil_Atk
	jumpifstat BS_ATTACKER EQUALS STAT_ACC STAT_MAX BattleScript_CantRaiseMultipleStats

Coil_Atk::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_DEF | STAT_ANIM_ACC, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN Coil_Def
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 Coil_Def
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

Coil_Def::
	setstatchanger2 STAT_DEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN Coil_Acc
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 Coil_Acc
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

Coil_Acc::
	setstatchanger2 STAT_ACC | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BulkUpDoMoveAnim::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_ATK | BIT_DEF, 0
	setstatchanger STAT_ATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_BulkUpTryDef
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_BulkUpTryDef
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_BulkUpTryDef::
	setstatchanger STAT_DEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_BulkUpEnd
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_BulkUpEnd
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_BulkUpEnd::
	goto BattleScript_MoveEnd

BattleScript_EffectBadPoisonChance::
	setmoveeffect MOVE_EFFECT_TOXIC
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectCalmMind::
	attackcanceler
	jumpifhalfword EQUALS gCurrentMove MOVE_QUIVERDANCE QuiverDanceBS
	jumpifhalfword EQUALS gCurrentMove MOVE_GEOMANCY GeomancyBS

CalmMindBS::
	attackstring
	ppreduce
	jumpifstat BS_TARGET LESSTHAN STAT_SPATK STAT_MAX CalmMind_SpAtk
	jumpifstat BS_TARGET EQUALS STAT_SPDEF STAT_MAX BattleScript_CantRaiseMultipleStats

CalmMind_SpAtk::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_SPATK | STAT_ANIM_SPDEF, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN CalmMind_SpDef
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 CalmMind_SpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

CalmMind_SpDef::
	setstatchanger2 STAT_SPDEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

QuiverDanceBS::
	attackstring
	ppreduce
	jumpifstat BS_ATTACKER LESSTHAN STAT_SPATK STAT_MAX QuiverDance_RaiseSpAtk1
	jumpifstat BS_ATTACKER LESSTHAN STAT_SPDEF STAT_MAX QuiverDance_RaiseSpAtk1
	jumpifstat BS_ATTACKER EQUALS STAT_SPEED STAT_MAX BattleScript_CantRaiseMultipleStats

QuiverDance_RaiseSpAtk1::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_SPATK | STAT_ANIM_SPDEF | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_SPATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN QuiverDance_RaiseSpDef1
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 QuiverDance_RaiseSpDef1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

QuiverDance_RaiseSpDef1:
	setstatchanger2 STAT_SPDEF | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN QuiverDance_RaiseSpd1
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 QuiverDance_RaiseSpd1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

QuiverDance_RaiseSpd1:
	setstatchanger2 STAT_SPEED| INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GeomancyBS::
	attackcanceler
	jumpifstatus2 BS_ATTACKER STATUS2_MULTIPLETURNS Geomancy_PowerUp
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING Geomancy_PowerUp

Geomancy_Charge::	
	printstring 0x130 @;Blank String
	ppreduce
	attackanimation
	waitanimation
	jumpifhelditemeffect BS_ATTACKER HOLD_EFFECT_POWER_HERB Geomancy_PowerHerb
	jumpifraidboss BS_ATTACKER Geomancy_RaidBossSkipCharge
	orword HIT_MARKER HITMARKER_CHARGING
	setmoveeffect MOVE_EFFECT_CHARGING | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	setword BATTLE_STRING_LOADER GeomancyChargeString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

Geomancy_PowerUp::
	setmoveeffect MOVE_EFFECT_CHARGING
	setbyte sB_ANIM_TURN 0x1
	clearstatusfromeffect BS_ATTACKER
	
Geomancy_PowerHerbSkip::
	setmoveeffect 0x0
	attackstring
	jumpifstat BS_ATTACKER LESSTHAN STAT_SPATK STAT_MAX Geomancy_RaiseSpAtk2
	jumpifstat BS_ATTACKER LESSTHAN STAT_SPDEF STAT_MAX Geomancy_RaiseSpAtk2
	jumpifstat BS_ATTACKER EQUALS STAT_SPEED STAT_MAX BattleScript_CantRaiseMultipleStats

Geomancy_RaiseSpAtk2::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_SPATK | STAT_ANIM_SPDEF | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
	setstatchanger2 STAT_SPATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN Geomancy_RaiseSpDef2
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 Geomancy_RaiseSpDef2
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

Geomancy_RaiseSpDef2::
	setstatchanger2 STAT_SPDEF | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN Geomancy_RaiseSpd2
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 Geomancy_RaiseSpd2
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

Geomancy_RaiseSpd2::
	setstatchanger2 STAT_SPEED| INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

Geomancy_PowerHerb::
	playanimation BS_ATTACKER B_ANIM_ITEM_EFFECT 0x0
	setword BATTLE_STRING_LOADER PowerHerbString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	removeitem BS_ATTACKER

Geomancy_RaidBossSkipCharge::
	setbyte sB_ANIM_TARGETS_HIT 0x0
	setbyte sB_ANIM_TURN 0x1
	callasm ClearCalculatedSpreadMoveData @;So the damage can be calculated
	goto Geomancy_PowerHerbSkip

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_CalmMindDoMoveAnim::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_SPATK | BIT_SPDEF, 0
	setstatchanger STAT_SPATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CalmMindTrySpDef
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_CalmMindTrySpDef
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_CalmMindTrySpDef::
	setstatchanger STAT_SPDEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CalmMindEnd
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_CalmMindEnd
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_CalmMindEnd::
	goto BattleScript_MoveEnd

BattleScript_CantRaiseMultipleStats::
	pause 0x20
	orbyte gMoveResultFlags, MOVE_RESULT_FAILED
	printstring STRINGID_STATSWONTINCREASE2
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectDragonDance::
	attackcanceler
	attackstring
	ppreduce
	jumpifhalfword EQUALS gCurrentMove MOVE_SHIFTGEAR ShiftGearBS
	jumpifhalfword EQUALS gCurrentMove MOVE_SHELLSMASH ShellSmashBS

DragonDanceBS::
	jumpifstat BS_ATTACKER LESSTHAN STAT_ATK STAT_MAX DragonDance_Atk
	jumpifstat BS_ATTACKER EQUALS STAT_SPEED STAT_MAX BattleScript_CantRaiseMultipleStats

DragonDance_Atk::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN DragonDance_Spd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 DragonDance_Spd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

DragonDance_Spd::
	setstatchanger2 STAT_SPEED| INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ShiftGearBS::
	jumpifstat BS_ATTACKER LESSTHAN STAT_ATK STAT_MAX ShiftGear_Atk
	jumpifstat BS_ATTACKER EQUALS STAT_SPEED STAT_MAX BattleScript_CantRaiseMultipleStats

ShiftGear_Atk::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
	setstatchanger2 STAT_ATK | INCREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ShiftGear_Spd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 ShiftGear_Spd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ShiftGear_Spd::
	setstatchanger2 STAT_SPEED| INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN BattleScript_MoveEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 BattleScript_MoveEnd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ShellSmashBS::
	jumpifstat BS_ATTACKER GREATERTHAN STAT_DEF STAT_MIN ShellSmash_DropDef
	jumpifstat BS_ATTACKER EQUALS STAT_SPDEF STAT_MIN ShellSmash_BoostStats

ShellSmash_DropDef::
	attackanimation
	waitanimation
	setbyte sB_ANIM_TARGETS_HIT 0x1 @;Prevent the attack animation from playing again
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_DOWN | STAT_ANIM_IGNORE_ABILITIES
	setstatchanger2 STAT_DEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ShellSmash_DropSpDef
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 ShellSmash_DropSpDef
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ShellSmash_DropSpDef::
	setstatchanger2 STAT_SPDEF | DECREASE_1
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ShellSmash_BoostStats
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 ShellSmash_BoostStats
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ShellSmash_BoostStats::
	jumpifstat BS_ATTACKER LESSTHAN STAT_ATK STAT_MAX ShellSmash_SharpAtk
	jumpifstat BS_ATTACKER LESSTHAN STAT_SPATK STAT_MAX ShellSmash_SharpAtk
	jumpifstat BS_ATTACKER EQUALS STAT_SPEED STAT_MAX BattleScript_CantRaiseMultipleStats

ShellSmash_SharpAtk::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_ATTACKER, STAT_ANIM_ATK | STAT_ANIM_SPATK | STAT_ANIM_SPD, STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
	setstatchanger2 STAT_ATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ShellSmash_SharpSpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 ShellSmash_SharpSpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

ShellSmash_SharpSpAtk::
	setstatchanger2 STAT_SPATK | INCREASE_2
	statbuffchange STAT_ATTACKER | STAT_BS_PTR | STAT_CERTAIN ShiftGear_Spd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 ShiftGear_Spd
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto ShiftGear_Spd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DragonDanceDoMoveAnim::
	attackanimation
	waitanimation
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_ATK | BIT_SPEED, 0
	setstatchanger STAT_ATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_DragonDanceTrySpeed
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_DragonDanceTrySpeed
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_DragonDanceTrySpeed::
	setstatchanger STAT_SPEED, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_DragonDanceEnd
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_DragonDanceEnd
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_DragonDanceEnd::
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectStatSwapSplitters::
	attackcanceler
	attackstringnoprotean
	ppreduce
	accuracycheck BattleScript_MoveMissedPause 0x0
	callasm SetStatSwapSplit
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_214_Blank::
	goto BattleScript_HitFromAtkCanceler

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_218_MeFirst::
	attackcanceler
	attackstringnoprotean
	accuracycheck FAILED_PRE + 1 0x0
	callasm MeFirstFunc + 1
	attackanimation
	waitanimation
	setbyte sB_ANIM_TURN 0x0
	setbyte sB_ANIM_TARGETS_HIT 0x0
	jumptocalledmove 0x1

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_219_DestroyBerry::
	jumpifmove MOVE_INCINERATE IncinerateBS
	setmoveeffect MOVE_EFFECT_EAT_BERRY | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkCanceler
	
IncinerateBS::
	setmoveeffect MOVE_EFFECT_BURN_BERRY | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkCanceler

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_220_NaturalGift::
	attackcanceler
	accuracycheck NaturalGiftMiss 0x0
	call STANDARD_DAMAGE
	seteffectwithchance
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	setbyte sMOVEEND_STATE 0x0
	moveend 0x0 0x0
	removeitem BS_ATTACKER
	end

NaturalGiftMiss::
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	effectivenesssound
	resultmessage
	waitmessage DELAY_1SECOND
	removeitem BS_ATTACKER
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_221_SmackDown::
	setmoveeffect MOVE_EFFECT_BRING_DOWN | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkCanceler

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_222_DamageResetTargetStatChanges::
	setmoveeffect MOVE_EFFECT_RESET_STAT_CHANGES | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkCanceler
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_223_RelicSong::
	setmoveeffect MOVE_EFFECT_SLEEP
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	seteffectwithchance
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	callasm GotoMoveEndIfMoveDidntDamageAtLeastOnce
	jumpifnoviablemonsleft BS_TARGET BattleScript_MoveEnd
	jumpifspecies BS_ATTACKER SPECIES_MELOETTA TransformToPirouetteBS
	jumpifspecies BS_ATTACKER SPECIES_MELOETTA_PIROUETTE TransformToAriaBS
	goto BattleScript_MoveEnd
	
TransformToPirouetteBS::
	setbyte sMOVEEND_STATE 0x0
	moveend 0x0 0x0
	jumpifhasnohp BS_ATTACKER RelicSongEndBS
	formchange BS_ATTACKER SPECIES_MELOETTA SPECIES_MELOETTA_PIROUETTE TRUE TRUE FALSE RelicSongEndBS
	goto MeloettaTransformAnim

TransformToAriaBS::
	setbyte sMOVEEND_STATE 0x0
	moveend 0x0 0x0
	jumpifhasnohp BS_ATTACKER RelicSongEndBS
	formchange BS_ATTACKER SPECIES_MELOETTA_PIROUETTE SPECIES_MELOETTA TRUE TRUE FALSE RelicSongEndBS
	
MeloettaTransformAnim::
	playanimation BS_ATTACKER B_ANIM_TRANSFORM 0x0
	setword BATTLE_STRING_LOADER TransformedString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	
RelicSongEndBS::
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_224_Blank:
BS_225_Blank::
	goto BattleScript_HitFromAtkCanceler

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_226_Terrain::
	attackcanceler
	attackstringnoprotean
	ppreduce
	setterrain FAILED
	callasm TransferTerrainData
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	setbyte SEED_HELPER 0
	callasm SeedRoomServiceLooper
	copybyte gBattlerAttacker TARGET_BANK @;Restore original target
	callasm TryActivateMimicry
	goto BattleScript_MoveEnd
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_227_Pledges::
	attackcanceler
	callasm LoadPledgeScript	
	goto BattleScript_HitFromAccCheck

BattleScript_PledgeReady::
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER PledgeReadyString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
BattleScript_PledgeCombined::
	setword BATTLE_STRING_LOADER PledgeCombinedString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	accuracycheck BattleScript_PrintMoveMissed+ 1 0x0
	call STANDARD_DAMAGE + 1
	callasm SetPledgeEffect
	@;playanimation2 BS_TARGET sB_ANIM_ARG1 0x0
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_228_FieldEffects::
	jumpifmove MOVE_PLASMAFISTS PlasmaFistsBS
	attackcanceler
	attackstring
	ppreduce
	callasm DoFieldEffect
	attackanimation
	waitanimation
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	jumpifmove MOVE_GRAVITY GravityBringDownBS
	setbyte SEED_HELPER 0
	callasm SeedRoomServiceLooper
	copybyte gBattlerAttacker TARGET_BANK @;Restore original target
	goto BattleScript_MoveEnd
	
GravityBringDownBS::
	callasm BringDownMons
	goto BattleScript_MoveEnd
	
PlasmaFistsBS::
	setmoveeffect MOVE_EFFECT_ION_DELUGE | MOVE_EFFECT_CERTAIN
	goto BattleScript_HitFromAtkCanceler
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_229_Fling::
	attackcanceler
	attackstring
	ppreduce
	callasm TryFling
	pause DELAY_HALFSECOND
	setword BATTLE_STRING_LOADER FlingString
	printstring STRINGID_CUSTOMSTRING
	accuracycheck FlingMissBS 0x0
	callasm TransferLastUsedItem
	call STANDARD_DAMAGE
	seteffectwithchance
	callasm TrySetAlternateFlingEffect
	callasm ClearDoingPluckItemEffect
	removeitem BS_ATTACKER
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	goto BattleScript_MoveEnd

FlingMissBS::
	pause DELAY_HALFSECOND
	effectivenesssound
	resultmessage
	waitmessage DELAY_1SECOND
	removeitem BS_ATTACKER
	goto BattleScript_MoveEnd
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_230_Feint::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET BattleScript_HitFromAccCheck
	
FeintSkipBS::
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	seteffectwithchance
	jumpifmovehadnoeffect BS_MOVE_FAINT
	callasm LiftProtectTarget
	setword BATTLE_STRING_LOADER ProtectLiftedString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BS_MOVE_FAINT	

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_231_AttackBlockers:
	attackcanceler
	jumpifmove MOVE_THROATCHOP ThroatChopBS
	jumpifmove MOVE_EMBARGO EmbargoBS
	jumpifmove MOVE_POWDER PowderBS
	jumpifmove MOVE_TELEKINESIS TelekinesisBS
	
HealBlockBS::
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	jumpifcounter BS_TARGET HEAL_BLOCK_TIMERS NOTEQUALS 0x0 FAILED_PRE
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL BattleScript_ProtectedByAromaVeil
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	callasm SetHealBlockTimer
	setword BATTLE_STRING_LOADER HealBlockSetString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

EmbargoBS::
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	jumpifcounter BS_TARGET EMBARGO_TIMERS NOTEQUALS 0x0 FAILED_PRE
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setcounter BS_TARGET EMBARGO_TIMERS 5
	setword BATTLE_STRING_LOADER EmbargoSetString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

PowderBS::
	jumpifcounter BS_TARGET POWDER_TIMERS NOTEQUALS 0x0 FAILED_PRE
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setcounter BS_TARGET POWDER_TIMERS 1
	setword BATTLE_STRING_LOADER PowderSetString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

TelekinesisBS::
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	callasm CheckTelekinesisFail
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setcounter BS_TARGET TELEKINESIS_TIMERS 3
	setspecialstatusbit BS_TARGET STATUS3_TELEKINESIS
	setword BATTLE_STRING_LOADER TelekinesisSetString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

ThroatChopBS::
	jumpifbehindsubstitute BS_TARGET BattleScript_HitFromAccCheck
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	callasm SetThroatChopTimer
	seteffectwithchance
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_232_TypeChangers::
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	attackstring @;Activates Protean even if it fails
	ppreduce
	jumpifmove MOVE_SOAK SoakBS
	callasm ChangeTargetTypeFunc
	attackanimation
	waitanimation
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
SoakBS::
	attackanimation
	callasm ChangeTargetTypeFunc
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_233_HealTarget::
	attackcanceler
	jumpifmove MOVE_POLLENPUFF PollenPuffBS

HealPulseBS::
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	attackstringnoprotean
	ppreduce
	jumpifcounter BS_TARGET HEAL_BLOCK_TIMERS NOTEQUALS 0x0 BattleScript_NoHealTargetAfterHealBlock
	accuracycheck BattleScript_MoveMissedPause 0x0
	callasm HealTargetFunc
	tryactivateprotean
	attackanimation
	waitanimation
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printstring 0x4B
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_NoHealTargetAfterHealBlock::
	pause DELAY_HALFSECOND
	orbyte gMoveResultFlags OUTCOME_FAILED
	setword BATTLE_STRING_LOADER HealBlockTargetString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

PollenPuffBS::
	jumpiftargetpartner HealPulseBS
	goto BattleScript_HitFromAccCheck

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_234_TopsyTurvyElectrify::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	attackstring
	ppreduce
	jumpifmove MOVE_ELECTRIFY ElectrifyBS
	callasm TopsyTurvyFunc
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER TopsyTurvyString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
ElectrifyBS::
	jumpifcounter BS_TARGET ELECTRIFY_TIMERS NOTEQUALS 0x0 FAILED
	setcounter BS_TARGET ELECTRIFY_TIMERS 1
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER ElectrifyString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_235_FairyLockHappyHourCelebrateHoldHands::
	attackcanceler
	attackstringnoprotean
	ppreduce
	callasm DoFairyLockHappyHourFunc
	jumpifmove MOVE_HOLDHANDS HoldHandsBS
	
HappyHourAnimBS::
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
HoldHandsBS::
	callasm SetTargetPartner
	accuracycheck FAILED 0x0
	goto HappyHourAnimBS
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_236_InstructAfterYouQuash::
	attackcanceler
	attackstringnoprotean
	ppreduce
	jumpifmove MOVE_INSTRUCT InstructBS
	jumpifmove MOVE_QUASH QuashBS
	
AfterYouBS::
	callasm AfterYouFunc
	accuracycheck FAILED 0x0
	tryactivateprotean
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER AfterYouString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
QuashBS::
	callasm QuashFunc
	accuracycheck FAILED 0x0
	tryactivateprotean
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER QuashString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
InstructBS::
	accuracycheck BattleScript_MoveMissedPause 0x0
	callasm TryExecuteInstruct
	attackanimation
	waitanimation
	callasm InitiateInstruct
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_237_SuckerPunch::
	attackcanceler
	callasm TrySuckerPunch
	goto BattleScript_HitFromAccCheck
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_238_Blank::
	goto BattleScript_HitFromAtkCanceler
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_239_TeamEffectsAndMagnetRise::
	attackcanceler
	jumpifmove MOVE_MAGNETRISE MagnetRiseBS
	attackstringnoprotean
	ppreduce
	callasm TailwindLuckyChantFunc
	tryactivateprotean
	attackanimation
	waitanimation
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
MagnetRiseBS::
	attackstringnoprotean
	ppreduce
	callasm TrySetMagnetRise
	tryactivateprotean
	attackanimation
	waitanimation
	setword BATTLE_STRING_LOADER MagnetRiseSetString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_240_Camouflage::
	attackcanceler
	attackstringnoprotean
	ppreduce
	settypetoterrain FAILED
	tryactivateprotean
	attackanimation
	waitanimation
	printstring 0x49 @;STRINGID_PKMNCHANGEDTYPE
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_241_FlameBurst::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffectwithchance
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	callasm FlameBurstFunc
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_SCRIPTING
	datahpupdate BS_SCRIPTING
	setword BATTLE_STRING_LOADER FlameBurstString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	tryfaintmon BS_SCRIPTING 0x0 0x0
	goto BattleScript_MoveEnd
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_242_LastResortSkyDrop::
	jumpifmove MOVE_LASTRESORT LastResortBS
	jumpifstatus2 BS_ATTACKER STATUS2_MULTIPLETURNS SkyDropDropBS
	jumpifword ANDS HIT_MARKER HITMARKER_NO_ATTACKSTRING SkyDropDropBS
	
	attackcanceler
	jumpifbehindsubstitute BS_TARGET FAILED_PRE
	jumpifstatus3condition EQUALS, STATUS3_SEMI_INVULNERABLE, 0x0, FAILED_PRE
	jumpifweight BS_TARGET GREATERTHAN 1999 FAILED_PRE @;199.9 kg
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	attackanimation
	waitanimation
	setsemiinvulnerablebit
	various BS_TARGET 0x0 @;Cancel Multi Turn Moves
	setword BATTLE_STRING_LOADER SkyDropUpString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	orword HIT_MARKER HITMARKER_CHARGING
	setmoveeffect MOVE_EFFECT_CHARGING | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	goto BattleScript_MoveEnd

SkyDropDropBS::
	attackcanceler
	attackstring
	pause DELAY_HALFSECOND
	setmoveeffect MOVE_EFFECT_CHARGING
	setbyte sB_ANIM_TURN 0x1
	clearstatusfromeffect BS_ATTACKER
	clearsemiinvulnerablebit
	makevisible BS_TARGET
	copyarray sBATTLER TARGET_BANK 0x1
	setword BATTLE_STRING_LOADER FreedFromSkyDropString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_HitFromCritCalc
	
LastResortBS::
	attackcanceler
	callasm LastResortFunc
	goto BattleScript_HitFromAccCheck
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_243_DamageSetTerrain::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	seteffectwithchance
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	call BattleScript_SetTerrain
	goto BattleScript_MoveEnd

BattleScript_SetTerrain::
	setterrain BattleScript_SetTerrainReturn
	callasm TransferTerrainData
	playanimation2 BS_ATTACKER sB_ANIM_ARG1 0x0
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	setbyte SEED_HELPER 0
	copyhword BACKUP_HWORD gBattlerAttacker @;Backup original atatcker + target
	callasm SeedRoomServiceLooper
	copyhword gBattlerAttacker BACKUP_HWORD @;Restore original attacker + target
	callasm TryActivateMimicry
BattleScript_SetTerrainReturn:
	return

BS_244_Blank:
BS_245_Blank:
BS_246_Blank:
BS_247_Blank:
BS_248_Blank:
BS_249_Blank:
BS_250_Blank:
BS_251_Blank:
BS_252_Blank::
	goto BattleScript_HitFromAtkCanceler
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_253_MaxMove::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	call STANDARD_DAMAGE
	jumpifmovehadnoeffect BS_MOVE_FAINT
	setmaxmoveeffect
	prefaintmoveendeffects 0x0
	faintpokemonaftermove
	goto BattleScript_MoveEnd

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_254_Synchronoise::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed 0x0
	attackstring
	ppreduce
	callasm SynchronoiseFunc
	goto BattleScript_HitFromCritCalc
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BS_255_Blank::
	goto BattleScript_HitFromAtkCanceler

BattleScript_EffectCamouflage::
	attackcanceler
	attackstring
	ppreduce
	settypetoterrain BattleScript_ButItFailed
	attackanimation
	waitanimation
	printstring STRINGID_PKMNCHANGEDTYPE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_FaintAttacker::
	playfaintcry BS_ATTACKER
	pause 0x30
	dofaintanimation BS_ATTACKER
	printstring STRINGID_ATTACKERFAINTED
	cleareffectsonfaint BS_ATTACKER
	printstring STRINGID_EMPTYSTRING3
	trytrainerslidefirstdownmsg BS_ATTACKER
	return

BattleScript_FaintTarget::
	playfaintcry BS_TARGET
	pause 0x30
	dofaintanimation BS_TARGET
	printstring STRINGID_TARGETFAINTED
	cleareffectsonfaint BS_TARGET
	printstring STRINGID_EMPTYSTRING3
	trytrainerslidefirstdownmsg BS_TARGET
	return

BattleScript_GiveExp::
	setbyte sGIVEEXP_STATE, 0
	getexp BS_TARGET
	end2

BattleScript_HandleFaintedMon::
	ifwildbattleend BattleScript_LinkBattleHandleFaint
	jumpifbyte CMP_NOT_EQUAL, gBattleOutcome, 0, BattleScript_FaintedMonEnd
	jumpifbattletype BATTLE_TYPE_TRAINER, BattleScript_FaintedMonTryChooseAnother
	jumpifword CMP_NO_COMMON_BITS, gHitMarker, HITMARKER_PLAYER_FAINTED, BattleScript_FaintedMonTryChooseAnother
	printstring STRINGID_USENEXTPKMN
	setbyte gBattleCommunication, 0
	yesnobox
	jumpifbyte CMP_EQUAL, gBattleCommunication + 1, 0, BattleScript_FaintedMonTryChooseAnother
	jumpifplayerran BattleScript_FaintedMonEnd
	printstring STRINGID_CANTESCAPE2
BattleScript_FaintedMonTryChooseAnother::
	openpartyscreen BS_FAINTED, BattleScript_FaintedMonEnd
	switchhandleorder BS_FAINTED, 2
	jumpifnotbattletype BATTLE_TYPE_TRAINER, BattleScript_FaintedMonChooseAnother
	jumpifbattletype BATTLE_TYPE_LINK, BattleScript_FaintedMonChooseAnother
	jumpifbattletype BATTLE_TYPE_BATTLE_TOWER, BattleScript_FaintedMonChooseAnother
	jumpifbattletype BATTLE_TYPE_DOUBLE, BattleScript_FaintedMonChooseAnother
	jumpifword CMP_COMMON_BITS, gHitMarker, HITMARKER_PLAYER_FAINTED, BattleScript_FaintedMonChooseAnother
	jumpifbyte CMP_EQUAL, sBATTLE_STYLE, 1, BattleScript_FaintedMonChooseAnother
	jumpifcantswitch BS_PLAYER1, BattleScript_FaintedMonChooseAnother
	printstring STRINGID_ENEMYABOUTTOSWITCHPKMN
	setbyte gBattleCommunication, 0
	yesnobox
	jumpifbyte CMP_EQUAL, gBattleCommunication + 1, 1, BattleScript_FaintedMonChooseAnother
	setatktoplayer0
	openpartyscreen BS_ATTACKER | OPEN_PARTY_ALLOW_CANCEL, BattleScript_FaintedMonChooseAnother
	switchhandleorder BS_ATTACKER, 2
	jumpifbyte CMP_EQUAL, gBattleCommunication, 6, BattleScript_FaintedMonChooseAnother
	atknameinbuff1
	resetintimidatetracebits BS_ATTACKER
	hpthresholds2 BS_ATTACKER
	printstring STRINGID_RETURNMON
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
	switchineffects BS_ATTACKER
	resetsentmonsvalue
BattleScript_FaintedMonChooseAnother::
	drawpartystatussummary BS_FAINTED
	getswitchedmondata BS_FAINTED
	switchindataupdate BS_FAINTED
	hpthresholds BS_FAINTED
	printstring STRINGID_SWITCHINMON
	hidepartystatussummary BS_FAINTED
	switchinanim BS_FAINTED, 0
	waitstate
	various7 BS_ATTACKER
	switchineffects BS_FAINTED
	jumpifbattletype BATTLE_TYPE_DOUBLE, BattleScript_FaintedMonEnd
	cancelallactions
BattleScript_FaintedMonEnd::
	end2

BattleScript_LinkBattleHandleFaint::
	openpartyscreen BS_UNKNOWN_5, BattleScript_LinkBattleHandleFaintStart
BattleScript_LinkBattleHandleFaintStart::
	switchhandleorder BS_FAINTED, 0
	openpartyscreen BS_UNKNOWN_6, BattleScript_LinkBattleFaintedMonEnd
	switchhandleorder BS_FAINTED, 0
BattleScript_LinkBattleFaintedMonLoop::
	switchhandleorder BS_FAINTED, 2
	drawpartystatussummary BS_FAINTED
	getswitchedmondata BS_FAINTED
	switchindataupdate BS_FAINTED
	hpthresholds BS_FAINTED
	printstring STRINGID_SWITCHINMON
	hidepartystatussummary BS_FAINTED
	switchinanim BS_FAINTED, 0
	waitstate
	switchineffects 5
	jumpifbytenotequal gBattlerFainted, gBattlersCount, BattleScript_LinkBattleFaintedMonLoop
BattleScript_LinkBattleFaintedMonEnd::
	end2

BattleScript_LocalTrainerBattleWon::
	printstring STRINGID_PLAYERDEFEATEDTRAINER1
	trainerslidein BS_ATTACKER
	waitstate
	printstring STRINGID_TRAINER1LOSETEXT
BattleScript_LocalTrainerBattleWon2::
	getmoneyreward BattleScript_LocalTrainerBattleWonGotMoney
BattleScript_LocalTrainerBattleWonGotMoney::
	printstring STRINGID_PLAYERGOTMONEY
	waitmessage 0x40
BattleScript_PayDayMoneyAndPickUpItems::
	givepaydaymoney
	pickup
	end2

BattleScript_LocalBattleLost::
	jumpifbattletype BATTLE_TYPE_TRAINER_TOWER, BattleScript_BattleTowerLost
	jumpifbattletype BATTLE_TYPE_EREADER_TRAINER, BattleScript_EReaderOrSecretBaseTrainerEnd
	jumpifhalfword CMP_EQUAL, gTrainerBattleOpponent_A, TRAINER_SECRET_BASE, BattleScript_EReaderOrSecretBaseTrainerEnd
	jumpifbyte CMP_NOT_EQUAL, cMULTISTRING_CHOOSER, 0, BattleScript_RivalBattleLost
BattleScript_LocalBattleLostPrintWhiteOut::
	jumpifbattletype BATTLE_TYPE_TRAINER, BattleScript_LocalBattleLostEnd
	printstring STRINGID_PLAYERWHITEOUT
	waitmessage 0x40
	getmoneyreward BattleScript_LocalBattleLostPrintTrainersWinText
	printstring STRINGID_PLAYERWHITEOUT2
	waitmessage 0x40
	goto BattleScript_EReaderOrSecretBaseTrainerEnd

BattleScript_LocalBattleLostEnd::
	printstring STRINGID_PLAYERLOSTAGAINSTENEMYTRAINER
	waitmessage 0x40
	getmoneyreward BattleScript_LocalBattleLostPrintTrainersWinText
	printstring STRINGID_PLAYERPAIDPRIZEMONEY
	waitmessage 0x40
BattleScript_EReaderOrSecretBaseTrainerEnd::
	end2

BattleScript_LocalBattleLostPrintTrainersWinText::
	printstring STRINGID_PLAYERWHITEDOUT
	waitmessage 0x40
	end2

BattleScript_RivalBattleLost::
	jumpifhasnohp BS_ATTACKER, BattleScript_RivalBattleLostSkipMonRecall
	printstring STRINGID_TRAINER1MON1COMEBACK
	waitmessage 0x40
	returnatktoball
	waitstate
BattleScript_RivalBattleLostSkipMonRecall::
	trainerslidein BS_ATTACKER
	waitstate
	printstring STRINGID_TRAINER1WINTEXT
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_LocalBattleLostPrintWhiteOut
	end2

BattleScript_BattleTowerLost::
	various8 BS_ATTACKER
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 0, BattleScript_BattleTowerLostLostSkipMonRecall
	printfromtable gDoubleBattleRecallStrings
	waitmessage 0x40
	returnopponentmon1toball BS_ATTACKER
	waitstate
	returnopponentmon2toball BS_ATTACKER
	waitstate
BattleScript_BattleTowerLostLostSkipMonRecall::
	trainerslidein BS_ATTACKER
	waitstate
	printstring STRINGID_TRAINER1WINTEXT
	jumpifnotbattletype BATTLE_TYPE_DOUBLE, BattleScript_BattleTowerLostLostSkipDouble
	printstring STRINGID_TRAINER2NAME
BattleScript_BattleTowerLostLostSkipDouble::
	end2

BattleScript_LinkBattleWonOrLost::
	printstring STRINGID_BATTLEEND
	waitmessage 0x40
	flee
	waitmessage 0x40
	end2

BattleScript_BattleTowerTrainerBattleWon::
	printstring STRINGID_PLAYERDEFEATEDTRAINER1
	trainerslidein BS_ATTACKER
	waitstate
	jumpifnotbattletype BATTLE_TYPE_TRAINER_TOWER, BattleScript_BattleTowerEtcTrainerBattleWonSkipText
	printstring STRINGID_TRAINER1LOSETEXT
	jumpifnotbattletype BATTLE_TYPE_DOUBLE, BattleScript_BattleTowerEtcTrainerBattleWonSkipText
	printstring STRINGID_TRAINER2CLASS
BattleScript_BattleTowerEtcTrainerBattleWonSkipText::
	pickup
BattleScript_BattleTowerEtcTrainerBattleWonSkipText2::
	end2

BattleScript_SmokeBallEscape::
	playanimation BS_ATTACKER, B_ANIM_SMOKEBALL_ESCAPE, NULL
	printstring STRINGID_PKMNFLEDUSINGITS
	waitmessage 0x40
	end2

BattleScript_GotAwaySafely::
	printstring STRINGID_GOTAWAYSAFELY
	waitmessage 0x40
	end2

BattleScript_WildMonFled::
	printstring STRINGID_WILDPKMNFLED
	waitmessage 0x40
	end2

BattleScript_PrintCantRunFromTrainer::
	jumpifbattletype BATTLE_TYPE_FIRST_BATTLE, BattleScript_LeftoverBirchString
	printstring STRINGID_NORUNNINGFROMTRAINERS
	end2

BattleScript_LeftoverBirchString::
	printstring STRINGID_DONTLEAVEBIRCH
	end2

BattleScript_PrintFailedToRunString::
	printfromtable gNoEscapeStringIds
	waitmessage 0x40
	end2

BattleScript_PrintCantEscapeFromBattle::
	printselectionstringfromtable gNoEscapeStringIds
	endselectionscript

BattleScript_PrintFullBox::
	printselectionstring STRINGID_BOXISFULL
	endselectionscript

BattleScript_PursuitSwitchCheckTwice::
	setmultihit 2
BattleScript_PursuitSwitchLoop::
	jumpifnopursuitswitchdmg BattleScript_DoSwitchOut
	swapattackerwithtarget
	trysetdestinybondtohappen
	call BattleScript_PursuitDmgOnSwitchOut
	swapattackerwithtarget
	goto BattleScript_DoSwitchOut

BattleScript_Pausex20::
	pause 0x20
	return

BattleScript_LevelUp::
	fanfare MUS_LEVEL_UP
	printstring STRINGID_PKMNGREWTOLV
	setbyte sLVLBOX_STATE, 0
	drawlvlupbox
	handlelearnnewmove BattleScript_LearnedNewMove, BattleScript_LearnMoveReturn, 1
	goto BattleScript_AskToLearnMove

BattleScript_LearnMoveLoop::
	handlelearnnewmove BattleScript_LearnedNewMove, BattleScript_LearnMoveReturn, 0
BattleScript_AskToLearnMove::
	buffermovetolearn
	printstring STRINGID_TRYTOLEARNMOVE1
	printstring STRINGID_TRYTOLEARNMOVE2
	printstring STRINGID_TRYTOLEARNMOVE3
	waitstate
	setbyte sLEARNMOVE_STATE, 0
	yesnoboxlearnmove BattleScript_ForgotAndLearnedNewMove
	printstring STRINGID_STOPLEARNINGMOVE
	waitstate
	setbyte sLEARNMOVE_STATE, 0
	yesnoboxstoplearningmove BattleScript_AskToLearnMove
	printstring STRINGID_DIDNOTLEARNMOVE
	goto BattleScript_LearnMoveLoop

BattleScript_ForgotAndLearnedNewMove::
	printstring STRINGID_123POOF
	printstring STRINGID_PKMNFORGOTMOVE
	printstring STRINGID_ANDELLIPSIS
BattleScript_LearnedNewMove::
	buffermovetolearn
	fanfare MUS_LEVEL_UP
	printstring STRINGID_PKMNLEARNEDMOVE
	waitmessage 0x40
	updatechoicemoveonlvlup BS_ATTACKER
	goto BattleScript_LearnMoveLoop

BattleScript_LearnMoveReturn::
	return

BattleScript_RainContinuesOrEnds::
	printfromtable gRainContinuesStringIds
	waitmessage 0x40
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_RainContinuesOrEndsEnd
	playanimation BS_ATTACKER, B_ANIM_RAIN_CONTINUES, NULL
BattleScript_RainContinuesOrEndsEnd::
	end2

BattleScript_DamagingWeatherContinues::
	printfromtable gSandstormHailContinuesStringIds
	waitmessage 0x40
	playanimation2 BS_ATTACKER, sB_ANIM_ARG1, NULL
	setbyte gBattleCommunication, 0
BattleScript_DamagingWeatherLoop::
	copyarraywithindex gBattlerAttacker, gBattlerByTurnOrder, gBattleCommunication, 1
	weatherdamage
	jumpifword CMP_EQUAL, gBattleMoveDamage, NULL, BattleScript_DamagingWeatherContinuesEnd
	printfromtable gSandstormHailDmgStringIds
	waitmessage 0x40
	orword gHitMarker, HITMARKER_x20 | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG | HITMARKER_GRUDGE
	effectivenesssound
	hitanimation BS_ATTACKER
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER, 0, NULL
	ifwildbattleend BattleScript_DamagingWeatherContinuesEnd
BattleScript_DamagingWeatherContinuesEnd::
	jumpifbyte CMP_NOT_EQUAL, gBattleOutcome, 0, BattleScript_WeatherDamageEndedBattle
	addbyte gBattleCommunication, 1
	jumpifbytenotequal gBattleCommunication, gBattlersCount, BattleScript_DamagingWeatherLoop
BattleScript_WeatherDamageEndedBattle::
	bicword gHitMarker, HITMARKER_x20 | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG | HITMARKER_GRUDGE
	end2

BattleScript_SandStormHailEnds::
	printfromtable gSandstormHailEndStringIds
	waitmessage 0x40
	end2

BattleScript_SunlightContinues::
	printstring STRINGID_SUNLIGHTSTRONG
	waitmessage 0x40
	playanimation BS_ATTACKER, B_ANIM_SUN_CONTINUES, NULL
	end2

BattleScript_SunlightFaded::
	printstring STRINGID_SUNLIGHTFADED
	waitmessage 0x40
	end2

BattleScript_OverworldWeatherStarts::
	printfromtable gWeatherContinuesStringIds
	waitmessage DELAY_1SECOND
	playanimation2 BS_ATTACKER sB_ANIM_ARG1 0x0
	end3

BattleScript_SideStatusWoreOff::
	printstring STRINGID_PKMNSXWOREOFF
	waitmessage 0x40
	end2

BattleScript_SafeguardProtected::
	pause 0x20
	printstring STRINGID_PKMNUSEDSAFEGUARD
	waitmessage 0x40
	end2

BattleScript_SafeguardEnds::
	pause 0x20
	printstring STRINGID_PKMNSAFEGUARDEXPIRED
	waitmessage 0x40
	end2

BattleScript_LeechSeedLiquidOoze::
	setbyte cMULTISTRING_CHOOSER, 4
	goto BattleScript_LeechSeedTurnPrintAndUpdateHp

BattleScript_BideStoringEnergy::
	printstring STRINGID_PKMNSTORINGENERGY
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_BideAttack::
	attackcanceler
	setmoveeffect MOVE_EFFECT_CHARGING
	clearstatusfromeffect BS_ATTACKER
	printstring STRINGID_PKMNUNLEASHEDENERGY
	waitmessage 0x40
	accuracycheck BattleScript_MoveMissed, ACC_CURR_MOVE
	typecalc
	bicbyte gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	copyword gBattleMoveDamage, sBIDE_DMG
	adjustsetdamage
	setbyte sB_ANIM_TURN, 1
	attackanimation
	waitanimation
	effectivenesssound
	hitanimation BS_TARGET
	waitstate
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	resultmessage
	waitmessage 0x40
	tryfaintmon BS_TARGET, 0, NULL
	goto BattleScript_MoveEnd

BattleScript_BideNoEnergyToAttack::
	attackcanceler
	setmoveeffect MOVE_EFFECT_CHARGING
	clearstatusfromeffect BS_ATTACKER
	printstring STRINGID_PKMNUNLEASHEDENERGY
	waitmessage 0x40
	goto BattleScript_ButItFailed

BattleScript_MistProtected::
	pause 0x20
	printstring STRINGID_PKMNPROTECTEDBYMIST
	waitmessage 0x40
	return

BattleScript_MoveUsedIsDisabled::
	printstring STRINGID_PKMNMOVEISDISABLED
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_SelectingDisabledMove::
	printselectionstring STRINGID_PKMNMOVEISDISABLED
	endselectionscript

BattleScript_DisabledNoMore::
	printstring STRINGID_PKMNMOVEDISABLEDNOMORE
	waitmessage 0x40
	end2

BattleScript_EncoredNoMore::
	printstring STRINGID_PKMNENCOREENDED
	waitmessage 0x40
	end2

BattleScript_DestinyBondTakesLife::
	printstring STRINGID_PKMNTOOKFOE
	waitmessage 0x40
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER, 0, NULL
	return

BattleScript_SpikesOnAttacker::
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	call BattleScript_PrintHurtBySpikes
	tryfaintmon BS_ATTACKER, 0, NULL
	tryfaintmon BS_ATTACKER, 1, BattleScript_SpikesOnAttackerFainted
	return

BattleScript_SpikesOnAttackerFainted::
	setbyte sGIVEEXP_STATE, 0
	getexp BS_ATTACKER
	moveendall
	goto BattleScript_HandleFaintedMon

BattleScript_SpikesOnTarget::
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	call BattleScript_PrintHurtBySpikes
	tryfaintmon BS_TARGET, 0, NULL
	tryfaintmon BS_TARGET, 1, BattleScript_SpikesOnTargetFainted
	return

BattleScript_SpikesOnTargetFainted::
	setbyte sGIVEEXP_STATE, 0
	getexp BS_TARGET
	moveendall
	goto BattleScript_HandleFaintedMon

BattleScript_SpikesOnFaintedBattler::
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_FAINTED
	datahpupdate BS_FAINTED
	call BattleScript_PrintHurtBySpikes
	tryfaintmon BS_FAINTED, 0, NULL
	tryfaintmon BS_FAINTED, 1, BattleScript_SpikesOnFaintedBattlerFainted
	return

BattleScript_SpikesOnFaintedBattlerFainted::
	setbyte sGIVEEXP_STATE, 0
	getexp BS_FAINTED
	moveendall
	goto BattleScript_HandleFaintedMon

BattleScript_PrintHurtBySpikes::
	printstring STRINGID_PKMNHURTBYSPIKES
	waitmessage 0x40
	return

BattleScript_PerishSongTakesLife::
	printstring STRINGID_PKMNPERISHCOUNTFELL
	waitmessage 0x40
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER, 0, NULL
	end2

BattleScript_PerishSongCountGoesDown::
	printstring STRINGID_PKMNPERISHCOUNTFELL
	waitmessage 0x40
	end2

BattleScript_AllStatsUp::
	jumpifstat BS_ATTACKER, CMP_LESS_THAN, STAT_ATK, 12, BattleScript_AllStatsUpAtk
	jumpifstat BS_ATTACKER, CMP_LESS_THAN, STAT_DEF, 12, BattleScript_AllStatsUpAtk
	jumpifstat BS_ATTACKER, CMP_LESS_THAN, STAT_SPEED, 12, BattleScript_AllStatsUpAtk
	jumpifstat BS_ATTACKER, CMP_LESS_THAN, STAT_SPATK, 12, BattleScript_AllStatsUpAtk
	jumpifstat BS_ATTACKER, CMP_EQUAL, STAT_SPDEF, 12, BattleScript_AllStatsUpRet
BattleScript_AllStatsUpAtk::
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_ATK | BIT_DEF | BIT_SPEED | BIT_SPATK | BIT_SPDEF, 0
	setstatchanger STAT_ATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_AllStatsUpDef
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_AllStatsUpDef::
	setstatchanger STAT_DEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_AllStatsUpSpeed
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_AllStatsUpSpeed::
	setstatchanger STAT_SPEED, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_AllStatsUpSpAtk
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_AllStatsUpSpAtk::
	setstatchanger STAT_SPATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_AllStatsUpSpDef
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_AllStatsUpSpDef::
	setstatchanger STAT_SPDEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_AllStatsUpRet
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_AllStatsUpRet::
	return

BattleScript_RapidSpinAway::
	rapidspinfree
	return

BattleScript_WrapFree::
	printstring STRINGID_PKMNGOTFREE
	waitmessage 0x40
	copybyte gBattlerTarget, sBATTLER
	return

BattleScript_LeechSeedFree::
	printstring STRINGID_PKMNSHEDLEECHSEED
	waitmessage 0x40
	return

BattleScript_SpikesFree::
	printstring STRINGID_PKMNBLEWAWAYSPIKES
	waitmessage 0x40
	return

BattleScript_FutureAttackAnimate::
	adjustnormaldamage2
	jumpifbyte CMP_NOT_EQUAL, cMULTISTRING_CHOOSER, 0, BattleScript_FutureHitAnimDoomDesire
	playanimation BS_ATTACKER, B_ANIM_FUTURE_SIGHT_HIT, NULL
	goto BattleScript_DoFutureAttackHit

BattleScript_NoMovesLeft::
	printselectionstring STRINGID_PKMNHASNOMOVESLEFT
	endselectionscript

BattleScript_SelectingMoveWithNoPP::
	printselectionstring STRINGID_NOPPLEFT
	endselectionscript

BattleScript_NoPPForMove::
	attackstring
	pause 0x20
	printstring STRINGID_BUTNOPPLEFT
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_SelectingTormentedMove::
	printselectionstring STRINGID_PKMNCANTUSEMOVETORMENT
	endselectionscript

	printstring STRINGID_PKMNCANTUSEMOVETORMENT
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_SelectingNotAllowedMoveTaunt::
	printselectionstring STRINGID_PKMNCANTUSEMOVETAUNT
	endselectionscript

BattleScript_MoveUsedIsTaunted::
	printstring STRINGID_PKMNCANTUSEMOVETAUNT
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_WishComesTrue::
	trywish 1, BattleScript_WishButFullHp
	playanimation BS_TARGET, B_ANIM_WISH_HEAL, NULL
	printstring STRINGID_PKMNWISHCAMETRUE
	waitmessage 0x40
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printstring STRINGID_PKMNREGAINEDHEALTH
	waitmessage 0x40
	end2

BattleScript_WishButFullHp::
	printstring STRINGID_PKMNWISHCAMETRUE
	waitmessage 0x40
	pause 0x20
	printstring STRINGID_PKMNHPFULL
	waitmessage 0x40
	end2

BattleScript_IngrainTurnHeal::
	playanimation BS_ATTACKER, B_ANIM_INGRAIN_HEAL, NULL
	printstring STRINGID_PKMNABSORBEDNUTRIENTS
	waitmessage 0x40
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	end2

BattleScript_PrintMonIsRooted::
	pause 0x20
	printstring STRINGID_PKMNANCHOREDITSELF
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_AtkDefDown::
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_ATK | BIT_DEF, ATK48_STAT_NEGATIVE | ATK48_ONLY_MULTIPLE | ATK48_DONT_CHECK_LOWER
	playstatchangeanimation BS_ATTACKER, BIT_ATK, ATK48_STAT_NEGATIVE | ATK48_DONT_CHECK_LOWER
	setstatchanger STAT_ATK, 1, TRUE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, BattleScript_AtkDefDownAtkFail
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_AtkDefDownAtkFail
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_AtkDefDownAtkFail::
	playstatchangeanimation BS_ATTACKER, BIT_DEF, ATK48_STAT_NEGATIVE | ATK48_DONT_CHECK_LOWER
	setstatchanger STAT_DEF, 1, TRUE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, BattleScript_AtkDefDownDefFail
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_AtkDefDownDefFail
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_AtkDefDownDefFail::
	return

BattleScript_KnockedOff::
	playanimation BS_TARGET, B_ANIM_ITEM_KNOCKOFF, NULL
	printstring STRINGID_PKMNKNOCKEDOFF
	waitmessage 0x40
	return

BattleScript_MoveUsedIsImprisoned::
	printstring STRINGID_PKMNCANTUSEMOVESEALED
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_SelectingImprisonedMove::
	printselectionstring STRINGID_PKMNCANTUSEMOVESEALED
	endselectionscript

BattleScript_GrudgeTakesPp::
	printstring STRINGID_PKMNLOSTPPGRUDGE
	waitmessage 0x40
	return

BattleScript_MagicCoatBounce::
	attackstring
	ppreduce
	pause 0x20
	printstring STRINGID_PKMNMOVEBOUNCED
	waitmessage 0x40
	orword gHitMarker, HITMARKER_ATTACKSTRING_PRINTED | HITMARKER_NO_PPDEDUCT | HITMARKER_x800000
	setmagiccoattarget BS_ATTACKER
	return

BattleScript_SnatchedMove::
	attackstring
	ppreduce
	snatchsetbattlers
	playanimation BS_TARGET, B_ANIM_SNATCH_MOVE, NULL
	printstring STRINGID_PKMNSNATCHEDMOVE
	waitmessage 0x40
	orword gHitMarker, HITMARKER_ATTACKSTRING_PRINTED | HITMARKER_NO_PPDEDUCT | HITMARKER_x800000
	swapattackerwithtarget
	return

BattleScript_EnduredMsg::
	printstring STRINGID_PKMNENDUREDHIT
	waitmessage 0x40
	return

BattleScript_OneHitKOMsg::
	printstring STRINGID_ONEHITKO
	waitmessage 0x40
	return

BattleScript_SAtkDown2::
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_SPATK, ATK48_STAT_NEGATIVE | ATK48_STAT_BY_TWO | ATK48_DONT_CHECK_LOWER
	setstatchanger STAT_SPATK, 2, TRUE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, BattleScript_SAtkDown2End
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 2, BattleScript_SAtkDown2End
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_SAtkDown2End::
	return

BattleScript_MoveUsedIsAsleep::
	printstring STRINGID_PKMNFASTASLEEP
	waitmessage 0x40
	statusanimation BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_MoveUsedWokeUp::
	bicword gHitMarker, HITMARKER_x10
	printfromtable gWokeUpStringIds
	waitmessage 0x40
	updatestatusicon BS_ATTACKER
	return

BattleScript_MonWokeUpInUproar::
	printstring STRINGID_PKMNWOKEUPINUPROAR
	waitmessage 0x40
	updatestatusicon BS_ATTACKER
	end2

BattleScript_PoisonTurnDmg::
	printstring STRINGID_PKMNHURTBYPOISON
	waitmessage 0x40
BattleScript_DoStatusTurnDmg::
	statusanimation BS_ATTACKER
BattleScript_DoTurnDmg::
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER, 0, NULL
	ifwildbattleend BattleScript_DoTurnDmgEnd
BattleScript_DoTurnDmgEnd::
	end2

BattleScript_BurnTurnDmg::
	printstring STRINGID_PKMNHURTBYBURN
	waitmessage 0x40
	goto BattleScript_DoStatusTurnDmg

BattleScript_MoveUsedIsFrozen::
	printstring STRINGID_PKMNISFROZEN
	waitmessage 0x40
	statusanimation BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_MoveUsedUnfroze::
	printfromtable gGotDefrostedStringIds
	waitmessage 0x40
	updatestatusicon BS_ATTACKER
	return

BattleScript_DefrostedViaFireMove::
	printstring STRINGID_PKMNWASDEFROSTED
	waitmessage 0x40
	updatestatusicon BS_TARGET
	return

BattleScript_MoveUsedIsParalyzed::
	printstring STRINGID_PKMNISPARALYZED
	waitmessage 0x40
	statusanimation BS_ATTACKER
	cancelmultiturnmoves BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_PrintUproarOverTurns::
	printfromtable gUproarOverTurnStringIds
	waitmessage 0x40
	end2

BattleScript_ThrashConfuses::
	chosenstatusanimation BS_ATTACKER, 1, STATUS2_CONFUSION
	printstring STRINGID_PKMNFATIGUECONFUSION
	waitmessage 0x40
	end2

BattleScript_MoveUsedIsConfused::
	printstring STRINGID_PKMNISCONFUSED
	waitmessage DELAY_1SECOND
	chosenstatusanimation BS_ATTACKER TRUE STATUS2_CONFUSION
	jumpifbyte CMP_EQUAL, cMULTISTRING_CHOOSER, 0, BattleScript_MoveUsedIsConfusedRet
BattleScript_DoSelfConfusionDmg::
	cancelmultiturnmoves BS_ATTACKER
	adjustnormaldamage2
	printstring STRINGID_ITHURTCONFUSION
	waitmessage 0x40
	effectivenesssound
	hitanimation BS_ATTACKER
	waitstate
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	resultmessage
	waitmessage 0x40
	tryfaintmon BS_ATTACKER, 0, NULL
	goto BattleScript_MoveEnd

BattleScript_MoveUsedIsConfusedRet::
	return

BattleScript_MoveUsedIsConfusedNoMore::
	printstring STRINGID_PKMNHEALEDCONFUSION
	waitmessage 0x40
	return

BattleScript_PrintPayDayMoneyString::
	printstring STRINGID_PKMNPICKEDUPITEM
	waitmessage 0x40
	return

BattleScript_WrapTurnDmg::
	playanimation BS_ATTACKER, B_ANIM_TURN_TRAP, sB_ANIM_ARG1
	printstring STRINGID_PKMNHURTBY
	waitmessage 0x40
	goto BattleScript_DoTurnDmg

BattleScript_WrapEnds::
	printstring STRINGID_PKMNFREEDFROM
	waitmessage 0x40
	end2

BattleScript_MoveUsedIsInLove::
	printstring STRINGID_PKMNINLOVE
	waitmessage 0x40
	status2animation BS_ATTACKER, STATUS2_INFATUATION
	return

BattleScript_MoveUsedIsInLoveCantAttack::
	printstring STRINGID_PKMNIMMOBILIZEDBYLOVE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_NightmareTurnDmg::
	printstring STRINGID_PKMNLOCKEDINNIGHTMARE
	waitmessage 0x40
	status2animation BS_ATTACKER, STATUS2_NIGHTMARE
	goto BattleScript_DoTurnDmg

BattleScript_CurseTurnDmg::
	printstring STRINGID_PKMNAFFLICTEDBYCURSE
	waitmessage 0x40
	status2animation BS_ATTACKER, STATUS2_CURSED
	goto BattleScript_DoTurnDmg

BattleScript_TargetPRLZHeal::
	printstring STRINGID_PKMNHEALEDPARALYSIS
	waitmessage 0x40
	updatestatusicon BS_TARGET
	return

BattleScript_TooScaredToMove::
	printstring STRINGID_MONTOOSCAREDTOMOVE
	waitmessage 0x40
	playanimation BS_ATTACKER, B_ANIM_MON_SCARED, NULL
	goto BattleScript_MoveEnd

BattleScript_GhostGetOutGetOut::
	printstring STRINGID_GHOSTGETOUTGETOUT
	playanimation BS_ATTACKER, B_ANIM_GHOST_GET_OUT, NULL
	goto BattleScript_MoveEnd

BattleScript_SilphScopeUnveiled::
	pause 0x20
	printstring STRINGID_SILPHSCOPEUNVEILED
	waitstate
	playanimation BS_OPPONENT1, B_ANIM_SILPH_SCOPED, NULL
	pause 0x20
	printstring STRINGID_GHOSTWASMAROWAK
	waitmessage 0x40
	end2

BattleScript_MoveEffectSleep::
	statusanimation BS_EFFECT_BATTLER
	printfromtable gFellAsleepStringIds
	waitmessage 0x40
BattleScript_UpdateEffectStatusIconRet::
	updatestatusicon BS_EFFECT_BATTLER
	waitstate
	return

BattleScript_MoveEffectPoison::
	statusanimation BS_EFFECT_BATTLER
	printfromtable gGotPoisonedStringIds
	waitmessage 0x40
	goto BattleScript_UpdateEffectStatusIconRet

BattleScript_MoveEffectBurn::
	statusanimation BS_EFFECT_BATTLER
	printfromtable gGotBurnedStringIds
	waitmessage 0x40
	goto BattleScript_UpdateEffectStatusIconRet

BattleScript_MoveEffectFreeze::
	statusanimation BS_EFFECT_BATTLER
	printfromtable gGotFrozenStringIds
	waitmessage 0x40
	goto BattleScript_UpdateEffectStatusIconRet

BattleScript_MoveEffectParalysis::
	statusanimation BS_EFFECT_BATTLER
	printfromtable gGotParalyzedStringIds
	waitmessage 0x40
	goto BattleScript_UpdateEffectStatusIconRet

BattleScript_MoveEffectUproar::
	printstring STRINGID_PKMNCAUSEDUPROAR
	waitmessage 0x40
	return

BattleScript_MoveEffectToxic::
	statusanimation BS_EFFECT_BATTLER
	printstring STRINGID_PKMNBADLYPOISONED
	waitmessage 0x40
	goto BattleScript_UpdateEffectStatusIconRet

BattleScript_MoveEffectPayDay::
	printstring STRINGID_COINSSCATTERED
	waitmessage 0x40
	return

BattleScript_MoveEffectWrap::
	printfromtable gWrappedStringIds
	waitmessage 0x40
	return

BattleScript_MoveEffectConfusion::
	chosenstatusanimation BS_EFFECT_BATTLER, 1, STATUS2_CONFUSION
	printstring STRINGID_PKMNWASCONFUSED
	waitmessage 0x40
	return

BattleScript_MoveEffectRecoil::
	jumpifmove MOVE_STRUGGLE, BattleScript_DoRecoil
	jumpifability BS_ATTACKER, ABILITY_ROCK_HEAD, BattleScript_RecoilEnd
BattleScript_DoRecoil::
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring STRINGID_PKMNHITWITHRECOIL
	waitmessage 0x40
	tryfaintmon BS_ATTACKER, 0, NULL
BattleScript_RecoilEnd::
	return

BattleScript_DrizzleActivates::
	pause 0x20
	printstring STRINGID_PKMNMADEITRAIN
	waitstate
	playanimation BS_BATTLER_0, B_ANIM_RAIN_CONTINUES, NULL
	call BattleScript_HandleWeatherFormChanges
	end3

BattleScript_SpeedBoostActivates::
	playanimation BS_ATTACKER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printstring STRINGID_PKMNRAISEDSPEED
	waitmessage 0x40
	end3

BattleScript_TraceActivates::
	callasm TransferAbilityPopUpHelperAsTrace
	playanimation BS_SCRIPTING B_ANIM_LOAD_ABILITY_POP_UP 0x0
	call BattleScript_AbilityPopUpRevert
	clearspecialstatusbit BS_SCRIPTING STATUS3_INTIMIDATE_POKES
	call BattleScript_AbilityPopUp
	printstring STRINGID_PKMNTRACED
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	tryactivateswitchinability BS_SCRIPTING
	end3

BattleScript_RainDishActivates::
	call BattleScript_AbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_HEALING_SPARKLES 0x0
	orword gHitMarker HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_SCRIPTING
	datahpupdate BS_SCRIPTING
	printstring STRINGID_PKMNSXRESTOREDHPALITTLE2
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	end3

BattleScript_SandstreamActivates::
	pause 0x20
	printstring STRINGID_PKMNSXWHIPPEDUPSANDSTORM
	waitstate
	playanimation BS_BATTLER_0, B_ANIM_SANDSTORM_CONTINUES, NULL
	call BattleScript_HandleWeatherFormChanges
	end3

BattleScript_ShedSkinActivates::
	printstring STRINGID_PKMNSXCUREDYPROBLEM
	waitmessage 0x40
	updatestatusicon BS_ATTACKER
	end3

BattleScript_HandleWeatherFormChanges::
	setbyte sBATTLER, 0
BattleScript_WeatherFormChangesLoop::
	trycastformdatachange
	addbyte sBATTLER, 1
	jumpifbytenotequal sBATTLER, gBattlersCount, BattleScript_WeatherFormChangesLoop
	return

BattleScript_CastformChange::
	call BattleScript_CastformChangeRet
	end3

BattleScript_CastformChangeRet::
	jumpifability BS_SCRIPTING ABILITY_FORECAST DoCastformTransform
	goto CastformChangeSkipAbilityPopUp @;Reverting back b/c no longer has ability
	
DoCastformTransform::
	call BattleScript_AbilityPopUp
	
CastformChangeSkipAbilityPopUp::
	docastformchangeanimation
	waitstate
	printstring STRINGID_PKMNTRANSFORMED @;STRINGID_PKMNTRANSFORMED
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	return

BattleScript_DoCastformChangeAnim::
	docastformchangeanimation
	waitstate
	printstring STRINGID_PKMNTRANSFORMED
	waitmessage 0x40
	return

BattleScript_IntimidateActivatesEnd3::
	call BattleScript_DoIntimidateActivationAnim
	end3

BattleScript_DoIntimidateActivationAnim::
	pause 0x20
BattleScript_IntimidateActivates::
	setbyte gBattlerTarget, 0
	setstatchanger STAT_ATK, 1, TRUE
BattleScript_IntimidateActivationAnimLoop::
	trygetintimidatetarget BattleScript_IntimidateEnd
	jumpifstatus2 BS_TARGET, STATUS2_SUBSTITUTE, BattleScript_IntimidateFail
	jumpifability BS_TARGET, ABILITY_CLEAR_BODY, BattleScript_IntimidateAbilityFail
	jumpifability BS_TARGET, ABILITY_HYPER_CUTTER, BattleScript_IntimidateAbilityFail
	jumpifability BS_TARGET, ABILITY_WHITE_SMOKE, BattleScript_IntimidateAbilityFail
	statbuffchange STAT_CHANGE_BS_PTR | STAT_CHANGE_NOT_PROTECT_AFFECTED, BattleScript_IntimidateFail
	jumpifbyte CMP_GREATER_THAN, cMULTISTRING_CHOOSER, 1, BattleScript_IntimidateFail
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printstring STRINGID_PKMNCUTSATTACKWITH
	waitmessage 0x40
BattleScript_IntimidateFail::
	addbyte gBattlerTarget, 1
	goto BattleScript_IntimidateActivationAnimLoop

BattleScript_IntimidateEnd::
	return

BattleScript_IntimidateAbilityFail::
	pause 0x20
	printstring STRINGID_PREVENTEDFROMWORKING
	waitmessage 0x40
	goto BattleScript_IntimidateFail

BattleScript_DroughtActivates::
	pause 0x20
	printstring STRINGID_PKMNSXINTENSIFIEDSUN
	waitstate
	playanimation BS_BATTLER_0, B_ANIM_SUN_CONTINUES, NULL
	call BattleScript_HandleWeatherFormChanges
	end3

BattleScript_TookAttack::
	attackstring
	pause 0x10
	call BattleScript_AbilityPopUp
	printstring STRINGID_PKMNSXTOOKATTACK
	waitmessage DELAY_HALFSECOND
	call BattleScript_AbilityPopUpRevert
	orword HIT_MARKER HITMARKER_ATTACKSTRING_PRINTED
	return

BattleScript_SturdyPreventsOHKO::
	pause 0x10
	call BattleScript_AbilityPopUp
	printstring 0x1B @;STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

BattleScript_DampStopsExplosion::
	attackstring
	ppreduce
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUp
	setword BATTLE_STRING_LOADER CantUseMoveString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

BattleScript_MoveHPDrain_PPLoss::
	ppreduce
BattleScript_MoveHPDrain::
	attackstring
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUp
	playanimation BS_TARGET B_ANIM_HEALING_SPARKLES 0x0
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printstring STRINGID_PKMNREGAINEDHEALTH
	waitmessage DELAY_1SECOND
	orbyte gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

BattleScript_MonMadeMoveUseless_PPLoss::
	ppreduce
BattleScript_MonMadeMoveUseless::
	orbyte gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE
	attackstring
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUp
	printstring STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

BattleScript_FlashFireBoost_PPLoss::
	ppreduce
BattleScript_FlashFireBoost::
	attackstring
	pause DELAY_HALFSECOND
	call BattleScript_AbilityPopUp
	callasm ApplyFlashFireBoost
	printfromtable gFlashFireStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

BattleScript_AbilityPreventsPhasingOut::
	pause 0x20
	printstring STRINGID_PKMNANCHORSITSELFWITH
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_AbilityNoStatLoss::
	pause 0x10
	callasm TryRemoveIntimidateAbilityPopUp
	callasm TryHideActiveAbilityPopUps @;Basically Mirror Armor
	copybyte sBATTLER gBattleCommunication
	call BattleScript_AbilityPopUp
	printstring 0xCE
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	setbyte cMULTISTRING_CHOOSER 0x4
	return

BattleScript_BRNPrevention::
	pause 0x20
	printfromtable gBRNPreventionStringIds
	waitmessage 0x40
	return

BattleScript_PRLZPrevention::
	pause 0x20
	printfromtable gPRLZPreventionStringIds
	waitmessage 0x40
	return

BattleScript_PSNPrevention::
	pause 0x20
	printfromtable gPSNPreventionStringIds
	waitmessage 0x40
	return

BattleScript_ObliviousPreventsAttraction::
	pause 0x20
	printstring STRINGID_PKMNPREVENTSROMANCEWITH
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_FlinchPrevention::
	pause 0x20
	printstring STRINGID_PKMNSXPREVENTSFLINCHING
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_OwnTempoPrevents::
	pause 0x20
	printstring STRINGID_PKMNPREVENTSCONFUSIONWITH
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_SoundproofProtected::
	attackstring
	ppreduce
	pause 0x20
	printstring STRINGID_PKMNSXBLOCKSY
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_AbilityNoSpecificStatLoss::
	pause 0x10
	copybyte FORM_COUNTER sBATTLER
	callasm TryRemoveIntimidateAbilityPopUp
	callasm TryHideActiveAbilityPopUps @;Basically Mirror Armor
	copybyte sBATTLER FORM_COUNTER
	call BattleScript_AbilityPopUp
	
BattleScript_AbilityNoSpecificStatLossPrintMessage::
	printstring 0x135
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	setbyte cMULTISTRING_CHOOSER 0x3
	return

BattleScript_StickyHoldActivates::
	pause 0x10
	call BattleScript_AbilityPopUp
	printstring 0x1B @;STRINGID_ITDOESNTAFFECT
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_MoveEnd

BattleScript_ColorChangeActivates::
	printstring STRINGID_PKMNCHANGEDTYPEWITH
	waitmessage 0x40
	return

BattleScript_RoughSkinActivates::
	call BattleScript_AbilityPopUp
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring STRINGID_PKMNHURTSWITH
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	tryfaintmon BS_ATTACKER 0x0 0x0
	return

BattleScript_CuteCharmActivates::
	call BattleScript_AbilityPopUp
	status2animation BS_ATTACKER, STATUS2_INFATUATION
	printstring STRINGID_PKMNSXINFATUATEDY
	waitmessage 0x40
	call BattleScript_AbilityPopUpRevert
	jumpifstatus2 BS_TARGET STATUS2_INFATUATION CuteCharmActivatesReturn
	jumpifability BS_TARGET ABILITY_OBLIVIOUS CuteCharmActivatesReturn
	jumpifabilitypresenttargetfield ABILITY_AROMAVEIL CuteCharmActivatesReturn
	jumpifhelditemeffect BS_ATTACKER HOLD_EFFECT_DESTINY_KNOT CuteCharmDestinyKnot

CuteCharmActivatesReturn::
	return

CuteCharmDestinyKnot::
	tryinfatuating BS_TARGET CuteCharmActivatesReturn
	playanimation BS_ATTACKER B_ANIM_ITEM_EFFECT 0x0
	status2animation BS_TARGET STATUS2_INFATUATION
	copyarray sBATTLER TARGET_BANK 0x1
	setword BATTLE_STRING_LOADER DestinyKnotString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

BattleScript_ApplySecondaryEffect::
	waitstate
	seteffectsecondary
	return

BattleScript_IgnoresWhileAsleep::
	printstring STRINGID_PKMNIGNORESASLEEP
	waitmessage 0x40
	moveendto 16
	end

BattleScript_IgnoresAndUsesRandomMove::
	printstring STRINGID_PKMNIGNOREDORDERS
	waitmessage 0x40
	jumptocalledmove 0
BattleScript_MoveUsedLoafingAround::
	call BattleScript_FlushMessageBox
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x4 BattleScript_TruantLoafingAround
	printfromtable gInobedientStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_MoveEnd

BattleScript_IgnoresAndFallsAsleep::
	printstring STRINGID_PKMNBEGANTONAP
	waitmessage 0x40
	setmoveeffect MOVE_EFFECT_SLEEP | MOVE_EFFECT_AFFECTS_USER
	seteffectprimary
	moveendto 16
	end

BattleScript_IgnoresAndHitsItself::
	printstring STRINGID_PKMNWONTOBEY
	waitmessage 0x40
	goto BattleScript_DoSelfConfusionDmg

BattleScript_SubstituteFade::
	playanimation BS_TARGET, B_ANIM_SUBSTITUTE_FADE, NULL
	printstring STRINGID_PKMNSUBSTITUTEFADED
	return


BattleScript_WhiteHerbEnd2::
	call BattleScript_WhiteHerbRet
	end2

BattleScript_WhiteHerbRet::
	playanimation BS_SCRIPTING, B_ANIM_ITEM_EFFECT, NULL
	printstring STRINGID_PKMNSITEMRESTOREDSTATUS
	waitmessage 0x40
	removeitem BS_SCRIPTING
	return

BattleScript_ItemHealHP_RemoveItem::
	playanimation BS_ATTACKER, B_ANIM_ITEM_EFFECT, NULL
	printstring STRINGID_PKMNSITEMRESTOREDHEALTH
	waitmessage 0x40
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	removeitem BS_ATTACKER
	end2

BattleScript_ItemHealHP_End2::
	call BattleScript_ItemHealHP_Ret
	end2

BattleScript_ItemHealHP_Ret::
	playanimation BS_ATTACKER, B_ANIM_ITEM_EFFECT, NULL
	printstring STRINGID_PKMNSITEMRESTOREDHPALITTLE
	waitmessage 0x40
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	return

BattleScript_SelectingNotAllowedMoveChoiceItem::
	printselectionstring STRINGID_ITEMALLOWSONLYYMOVE
	endselectionscript

BattleScript_HangedOnMsg::
	playanimation BS_TARGET, B_ANIM_HANGED_ON, NULL
	printstring STRINGID_PKMNHUNGONWITHX
	waitmessage 0x40
	return

BattleScript_ActionSelectionItemsCantBeUsed::
	printselectionstring STRINGID_ITEMSCANTBEUSEDNOW
	endselectionscript

BattleScript_FlushMessageBox::
	printstring STRINGID_EMPTYSTRING3
	return
