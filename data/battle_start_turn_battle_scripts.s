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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AirBalloonFloat::
	call BattleScript_AirBalloonSub
	end3

BattleScript_AirBalloonSub::
	jumpifstatus3condition BS_ATTACKER STATUS3_AIR_BALLOON_BS 0x0 AirBalloonFloatEnd
	setspecialstatusbit BS_ATTACKER STATUS3_AIR_BALLOON_BS
	setword BATTLE_STRING_LOADER AirBalloonEntryString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
AirBalloonFloatEnd::
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Totem::
	call BattleScript_TotemRet
	end3

BattleScript_TotemOmniboost::
	call BattleScript_TotemOmniboostRet
	end3

BattleScript_TotemRet::
	playanimation BS_ATTACKER B_ANIM_TOTEM_BOOST 0x0
	setword BATTLE_STRING_LOADER TotemAuraFlared
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	statbuffchange STAT_ATTACKER | STAT_BS_PTR TotemEnd
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 TotemEnd
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER 0x1 sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
TotemEnd::
	return

BattleScript_TotemOmniboostRet::
	playanimation BS_ATTACKER B_ANIM_TOTEM_BOOST 0x0
	setword BATTLE_STRING_LOADER TotemAuraFlared
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AllStatsUp
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Primal::
	call BattleScript_PrimalSub
	end3

BattleScript_PrimalSub::
	jumpifspecies BS_ATTACKER SPECIES_GROUDON_PRIMAL PGroudonAnim
	jumpifspecies BS_ATTACKER SPECIES_KYOGRE_PRIMAL PKyogreAnim
	playanimation BS_ATTACKER B_ANIM_TRANSFORM 0x0

RegularPrimalAnim::
	goto PrimalStringDisplay

PGroudonAnim::
	playanimation BS_ATTACKER B_ANIM_RED_PRIMAL_REVERSION 0x0
	goto PrimalStringDisplay
	
PKyogreAnim::
	playanimation BS_ATTACKER B_ANIM_BLUE_PRIMAL_REVERSION 0x0
	
PrimalStringDisplay::
	callasm UpdatePrimalAbility
	reloadhealthbar BS_ATTACKER
	setword BATTLE_STRING_LOADER PrimalReversionString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ElectricTerrainBattleBegin::
	setword BATTLE_STRING_LOADER ElectricTerrainBattleBeginString
	printstring STRINGID_CUSTOMSTRING
	playanimation 0x0 B_ELECTRIC_TERRAIN_ACTIVE_ANIM 0x0
	end3

BattleScript_GrassyTerrainBattleBegin::
	setword BATTLE_STRING_LOADER GrassyTerrainBattleBeginString
	printstring STRINGID_CUSTOMSTRING
	playanimation 0x0 B_GRASSY_TERRAIN_ACTIVE_ANIM 0x0
	end3

BattleScript_MistyTerrainBattleBegin::
	setword BATTLE_STRING_LOADER MistyTerrainBattleBeginString
	printstring STRINGID_CUSTOMSTRING
	playanimation 0x0 B_MISTY_TERRAIN_ACTIVE_ANIM 0x0
	end3

BattleScript_PsychicTerrainBattleBegin::
	setword BATTLE_STRING_LOADER PsychicTerrainBattleBeginString
	printstring STRINGID_CUSTOMSTRING
	playanimation 0x0 B_PSYCHIC_TERRAIN_ACTIVE_ANIM 0x0
	end3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_QuickClaw::
	setword BATTLE_STRING_LOADER StringNull
	printstring STRINGID_CUSTOMSTRING
	setword BATTLE_STRING_LOADER gText_ItemIncreasedSpeedBracket
	jumpifhelditemeffect 0xA HOLD_EFFECT_QUICK_CLAW QuickClawBS
	playanimation 0xA B_ANIM_BERRY_EAT 0x0
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_HALFSECOND
	call BattleScript_CheekPouch
	end3

QuickClawBS::
	playanimation 0xA B_ANIM_ITEM_EFFECT 0x0
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_HALFSECOND
	end3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_FocusPunchSetUp::
	printstring STRINGID_EMPTYSTRING3
	waitmessage 0xA
	playanimation 0xA B_ANIM_FOCUS_PUNCH_SETUP 0x0
	printstring STRINGID_PKMNTIGHTENINGFOCUS
	waitmessage DELAY_1SECOND
	end3

BattleScript_BeakBlastSetUp::
	printstring STRINGID_EMPTYSTRING3
	waitmessage 0xA
	playanimation 0xA B_ANIM_BEAK_BLAST_WARM_UP 0x0
	setword BATTLE_STRING_LOADER BeakBlastWarmUpString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end3

BattleScript_ShellTrapSetUp::
	printstring STRINGID_EMPTYSTRING3
	waitmessage 0xA
	playanimation 0xA B_ANIM_SHELL_TRAP_SET 0x0
	setword BATTLE_STRING_LOADER ShellTrapSetString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end3


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_NoTargetMoveFailed::
	attackcanceler
	goto FAILED - 2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_CamomonsTypeRevealEnd3::
	call BattleScript_CamomonsTypeRevealRet
	end3

BattleScript_CamomonsTypeRevealRet::
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaidBattleStart::
	playanimation BS_SCRIPTING B_ANIM_DYNAMAX_START 0x0
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	setword BATTLE_STRING_LOADER gText_RaidBattleStormStarted
	call BattleScript_RaidBattleStorm
	end3
	
BattleScript_RaidBattleStorm::
	playanimation BS_SCRIPTING B_ANIM_RAID_BATTLE_STORM 0x0
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

BattleScript_DynamaxEnergySwirl::
	playanimation BS_SCRIPTING B_ANIM_DYNAMAX_ENERGY_SWIRL 0x0
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MegaEvolution::
	setword BATTLE_STRING_LOADER MegaReactingString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_HALFSECOND

MegaAnimBS::
	playanimation BS_SCRIPTING B_ANIM_MEGA_EVOLUTION 0x0
	reloadhealthbar BS_SCRIPTING
	setword BATTLE_STRING_LOADER MegaEvolutionCompleteString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end3

BattleScript_MegaWish::
	setword BATTLE_STRING_LOADER FerventWishString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_HALFSECOND
	goto MegaAnimBS

BattleScript_UltraBurst::
	setword BATTLE_STRING_LOADER UltraBurstGlowingString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_HALFSECOND
	playanimation BS_SCRIPTING B_ANIM_ULTRA_BURST 0x0
	reloadhealthbar BS_SCRIPTING
	setword BATTLE_STRING_LOADER UltraBurstCompleteString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end3

BattleScript_Dynamax::
	call BattleScript_FlushMessageBox
	callasm UpdateHPForDynamax
	playanimation BS_SCRIPTING B_ANIM_CALL_BACK_POKEMON 0x0
	waitanimation
	pause DELAY_1SECOND
	pause DELAY_HALFSECOND
	returntoball BS_SCRIPTING
	call BattleScript_TryRevertCramorant

BattleScript_Dynamax_Rejoin::
	waitstate
	callasm TryDoDynamaxTrainerSlide
	callasm SetAndTransferDontRemoveTransformSpecies
	callasm BackupScriptingBankMoveSelectionCursor @;Prevents the move selection cursor from being reset by the switch-in anim
	switchinanim BS_SCRIPTING 0x1 @;Play the switch-in animation
	waitanimation
	callasm RestoreScriptingBankMoveSelectionCursor
	callasm ClearAndTransferDontRemoveTransformSpecies
	playanimation BS_SCRIPTING B_ANIM_DYNAMAX_START 0x0
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_SCRIPTING
	datahpupdate BS_SCRIPTING
	bicword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE
	setword BATTLE_STRING_LOADER gText_MonDynamaxed
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	end3

BattleScript_TryRevertCramorant::
	formchange BS_SCRIPTING SPECIES_CRAMORANT_GULPING SPECIES_CRAMORANT TRUE TRUE FALSE BattleScript_TryRevertGorgingCramorant
BattleScript_TryRevertGorgingCramorant::
	formchange BS_SCRIPTING SPECIES_CRAMORANT_GORGING SPECIES_CRAMORANT TRUE TRUE FALSE BattleScript_Dynamax_Rejoin
	goto BattleScript_Dynamax_Rejoin

BattleScript_TrainerSlideMsgRet::
	handletrainerslidemsg BS_SCRIPTING 0
	callasm TrainerSlideInScriptingBank
	printstring 0x184
	waitstate
	callasm TrainerSlideOutScriptingBank
	handletrainerslidemsg BS_SCRIPTING 1
	waitstate
	return
	
BattleScript_TrainerSlideMsgEnd2::
	call BattleScript_TrainerSlideMsgRet
	end2

BattleScript_StatsResetZMove::
	setword BATTLE_STRING_LOADER StatsResetZMoveString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

BattleScript_AllStatsUpZMove::
	setword BATTLE_STRING_LOADER StatsRaisedZMoveString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

BattleScript_BoostCritsZMove::
	setword BATTLE_STRING_LOADER CritBoostedZMoveString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

BattleScript_FollowMeZMove::
	printstring 0xA4
	waitmessage DELAY_1SECOND
	return

BattleScript_RecoverHPZMove::
	healthbarupdate 0xA
	datahpupdate 0xA
	setword BATTLE_STRING_LOADER HPRestoredZMoveString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return
	
BattleScript_StatUpZMove::
	playanimation 0xA 0x1 0x2023FD4
	setword BATTLE_STRING_LOADER StatRaisedZMoveString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

BattleScript_SetUpHealReplacementZMove::
	setword BATTLE_STRING_LOADER HPWillRestoreZMoveString
	printstring 0x184
	waitmessage DELAY_1SECOND
	return

BattleScript_HealReplacementZMove::
	playanimation 0xA B_ANIM_HEALING_WISH_HEAL 0x0
	setword BATTLE_STRING_LOADER HPSwitchInRestoredZMoveString
	printstring 0x184
	waitmessage DELAY_1SECOND
	playanimation 0xA B_ANIM_HEALING_SPARKLES 0x0
	healthbarupdate 0xA
	datahpupdate 0xA
	return
