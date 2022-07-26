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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryConfuseHealRet::
	call BattleScript_TryPrintRipenAbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	printstring 0x12A
	waitmessage DELAY_1SECOND
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_SCRIPTING
	datahpupdate BS_SCRIPTING
	printstring 0x144
	waitmessage DELAY_1SECOND
	setmoveeffect MOVE_EFFECT_CONFUSION
	seteffectprimaryscriptingbank
	call BattleScript_CheekPouch
	return

BattleScript_BerryConfuseHealEnd2::
	call BattleScript_BerryConfuseHealRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryHealHP_RemoveBerryRet::
	call BattleScript_TryPrintRipenAbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	
BattleScript_ItemHealHP_RemoveItem_SkipAnim::
	playanimation BS_SCRIPTING B_ANIM_HEALING_SPARKLES 0x0
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_SCRIPTING
	datahpupdate BS_SCRIPTING
	printstring 0x12A
	waitmessage DELAY_1SECOND
	call BattleScript_CheekPouch
	return

BattleScript_BerryHealHP_RemoveBerryEnd2::
	call BattleScript_BerryHealHP_RemoveBerryRet
	end2
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ItemHealHP_RemoveItemRet::
	playanimation BS_SCRIPTING B_ANIM_ITEM_EFFECT 0x0
	goto BattleScript_ItemHealHP_RemoveItem_SkipAnim

BattleScript_ItemHealHP_RemoveItemEnd2::
	call BattleScript_ItemHealHP_RemoveItemRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryPPHealRet::
	waitstate
	call BattleScript_TryPrintRipenAbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	printstring 0x12B
	waitmessage DELAY_1SECOND
	call BattleScript_CheekPouch
	return
	
BattleScript_BerryPPHealEnd2::
	call BattleScript_BerryPPHealRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryFocusEnergyRet::
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	printstring 0x146
	waitmessage DELAY_1SECOND
	call BattleScript_CheekPouch
	return
	
BattleScript_BerryFocusEnergyEnd2::
	call BattleScript_BerryFocusEnergyRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryStatRaiseRet::
	call BattleScript_TryPrintRipenAbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	setbyte cMULTISTRING_CHOOSER 0x4
	call BattleScript_StatUp
	call BattleScript_CheekPouch
	return

BattleScript_BerryStatRaiseEnd2::
	call BattleScript_BerryStatRaiseRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryCureParRet::
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	updatestatusicon BS_SCRIPTING
	printstring STRINGID_PKMNSITEMCUREDPARALYSIS
	waitmessage DELAY_1SECOND
	call BattleScript_CheekPouch
	return

BattleScript_BerryCurePrlzEnd2::
	call BattleScript_BerryCureParRet
	end2
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


BattleScript_BerryCurePsnRet::
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	updatestatusicon BS_SCRIPTING
	printstring 0x124
	waitmessage DELAY_1SECOND
	call BattleScript_CheekPouch
	return

BattleScript_BerryCurePsnEnd2::
	call BattleScript_BerryCurePsnRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryCureBrnRet::
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	updatestatusicon BS_SCRIPTING
	printstring 0x125
	waitmessage DELAY_1SECOND
	call BattleScript_CheekPouch
	return

BattleScript_BerryCureBrnEnd2::
	call BattleScript_BerryCureBrnRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryCureFrzRet::
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	updatestatusicon BS_SCRIPTING
	printstring 0x126
	waitmessage DELAY_1SECOND
	call BattleScript_CheekPouch
	return

BattleScript_BerryCureFrzEnd2::
	call BattleScript_BerryCureFrzRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryCureSlpRet::
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	updatestatusicon BS_SCRIPTING
	printstring 0x127
	waitmessage DELAY_1SECOND
	call BattleScript_CheekPouch
	return

BattleScript_BerryCureSlpEnd2::
	call BattleScript_BerryCureSlpRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryCureConfusionRet::
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	printstring 0x128
	waitmessage DELAY_1SECOND
	call BattleScript_CheekPouch
	return

BattleScript_BerryCureConfusionEnd2::
	call BattleScript_BerryCureConfusionRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BerryCureChosenStatusRet::
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	updatestatusicon BS_SCRIPTING
	printfromtable gBerryEffectStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_CheekPouch
	return

BattleScript_BerryCureChosenStatusEnd2::
	call BattleScript_BerryCureChosenStatusRet
	end2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_CheekPouch::
	callasm CheeckPouchFunc
	jumpifbyte EQUALS FORM_COUNTER 0x0 CheeckPouchRet
	setbyte FORM_COUNTER 0x0
	call BattleScript_AbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_HEALING_SPARKLES 0x0
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE
	healthbarupdate BS_SCRIPTING
	datahpupdate BS_SCRIPTING
	setword BATTLE_STRING_LOADER CheekPouchString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
CheeckPouchRet::
	removeitem BS_SCRIPTING
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_TryPrintRipenAbilityPopUp::
	jumpifability BS_SCRIPTING ABILITY_RIPEN BattleScript_PrintRipenAbilityPopUp
	return

BattleScript_PrintRipenAbilityPopUp::
	call BattleScript_AbilityPopUp
	pause DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert	
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_HerbCureChosenStatusRet::
	playanimation BS_SCRIPTING B_ANIM_ITEM_EFFECT 0x0
	updatestatusicon BS_SCRIPTING
	printfromtable gBerryEffectStringIds
	waitmessage DELAY_1SECOND
	removeitem BS_SCRIPTING
	return

BattleScript_HerbCureChosenStatusEnd2::
	call BattleScript_HerbCureChosenStatusRet
	end2
	
BattleScript_HerbCureChosenStatusFling::
	playanimation BS_SCRIPTING B_ANIM_ITEM_EFFECT 0x0
	updatestatusicon BS_SCRIPTING
	printfromtable gBerryEffectStringIds
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaiseStatsItem::
	playanimation BS_SCRIPTING B_ANIM_ITEM_EFFECT 0x0
	setbyte cMULTISTRING_CHOOSER 0x4
	call BattleScript_StatUp
	removeitem BS_SCRIPTING
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ItemStatChangeEnd2::
	call BattleScript_ItemStatChangeRet
	end2

BattleScript_ItemStatChangeRet::
	statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN ItemStatChangeReturn
	playanimation BS_TARGET B_ANIM_ITEM_EFFECT 0x0
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	setbyte cMULTISTRING_CHOOSER 0x4
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	removeitem BS_TARGET
ItemStatChangeReturn::
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_WhiteHerbFling::
	playanimation BS_SCRIPTING B_ANIM_ITEM_EFFECT 0x0
	setword BATTLE_STRING_LOADER gText_WhiteHerbFling
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AirBallooonPop::
	setword BATTLE_STRING_LOADER AirBalloonPopString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	setcounter BS_TARGET INCINERATE_COUNTERS 0x1 @;Air Balloons can't be recycled
	removeitem BS_TARGET
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_WeaknessPolicy::
	playanimation BS_TARGET B_ANIM_ITEM_EFFECT 0x0
	setword BATTLE_STRING_LOADER WeaknessPolicyString
	printstring STRINGID_CUSTOMSTRING
	
WP_Atk::
	setbyte 0x2023FDF 0x0
	playstatchangeanimation BS_TARGET STAT_ANIM_ATK | STAT_ANIM_SPATK  STAT_ANIM_UP | STAT_ANIM_IGNORE_ABILITIES | STAT_ANIM_BY_TWO
	setstatchanger2 STAT_ATK | INCREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR WP_SpAtk
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 WP_SpAtk
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

WP_SpAtk::
	setstatchanger2 STAT_SPATK | INCREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR WP_Ret
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 WP_Ret
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND

WP_Ret::
	removeitem BS_TARGET
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RockyHelmetDamage::
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	setword BATTLE_STRING_LOADER RockyHelmetString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	tryfaintmon BS_ATTACKER 0x0 0x0
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_JabocaRowapBerry::
	call BattleScript_TryPrintRipenAbilityPopUp
	playanimation BS_TARGET B_ANIM_BERRY_EAT 0x0
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	setword BATTLE_STRING_LOADER RockyHelmetString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	tryfaintmon BS_ATTACKER 0x0 0x0
	call BattleScript_CheekPouch
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BlackSludgeHurt::
	playanimation BS_SCRIPTING B_ANIM_WAITING_WAGGLE 0x0
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_SCRIPTING
	datahpupdate BS_SCRIPTING
	setword BATTLE_STRING_LOADER BlackSludgeHurtString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	tryfaintmon BS_SCRIPTING 0x0 0x0
	end2
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MicleBerryRet::
	call BattleScript_TryPrintRipenAbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	setword BATTLE_STRING_LOADER MicleBerryString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	call BattleScript_CheekPouch
	return

BattleScript_MicleBerryEnd2::
	call BattleScript_MicleBerryRet
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_StickyBarbTransfer::
	callasm TransferLastUsedItem
	playanimation BS_SCRIPTING B_ANIM_ITEM_STEAL 0x0
	setword BATTLE_STRING_LOADER StickyBarbString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	removeitem BS_SCRIPTING
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EjectButton::
	jumpifcantswitch BS_SWITCHING | ATK4F_DONT_CHECK_STATUSES EjectButtonEnd
	playanimation BS_SWITCHING B_ANIM_ITEM_EFFECT 0x0
	setword BATTLE_STRING_LOADER EjectButtonString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	removeitem BS_SWITCHING
	playanimation BS_SWITCHING B_ANIM_CALL_BACK_POKEMON 0x0
	callasm MakeSwitchingBankInvisible
	openpartyscreen BS_SWITCHING EjectButtonEnd
	switchoutabilities BS_SWITCHING
	waitstate
	switchhandleorder BS_SWITCHING 0x2
	returntoball BS_SWITCHING
	getswitchedmondata BS_SWITCHING
	switchindataupdate BS_SWITCHING
	hpthresholds BS_SWITCHING
	printstring 0x3
	switchinanim BS_SWITCHING 0x1
	waitstate
	switchineffects BS_SWITCHING
	callasm SetNoMoreMovingThisTurnSwitchingBank @;New Pokemon can't attack after being switched in

EjectButtonEnd::
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EjectPackEnd2::
	call BattleScript_EjectPackRet
	end2
	
BattleScript_EjectPackRet::
	goto BattleScript_EjectButton

BattleScript_EjectPackCMD49::
	jumpifcantswitch BS_SWITCHING | ATK4F_DONT_CHECK_STATUSES EjectButtonEnd
	jumpifhasnohp BS_TARGET BattleScript_EjectPackGiveEXP
	goto BattleScript_EjectPackRet
	
BattleScript_EjectPackGiveEXP::
	setbyte sGIVEEXP_STATE 0x0
	getexp 0x0
	callasm SetSkipCertainSwitchInAbilities
	goto BattleScript_EjectPackRet
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RedCard::
	jumpifcantswitch BS_ATTACKER | ATK4F_DONT_CHECK_STATUSES RedCardRet
	playanimation BS_SCRIPTING B_ANIM_ITEM_EFFECT 0x0
	setword BATTLE_STRING_LOADER RedCardString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	jumpifdynamaxed BS_ATTACKER RedCard_Dynamax
	jumpifstatus3condition BS_ATTACKER STATUS3_ROOTED 0x0 RedCard_Ingrain
	jumpifability BS_ATTACKER ABILITY_SUCTION_CUPS RedCard_SuctionCups
	playanimation BS_ATTACKER B_DRAGON_TAIL_BLOW_AWAY_ANIM 0x0
	callasm ClearAttackerDidDamageOnce
	callasm TryRemovePrimalWeatherOnPivot	
	forcerandomswitch BS_ATTACKER BS_TARGET RedCardEnd

RedCardEnd::
	removeitem BS_SCRIPTING
	
RedCardRet::
	return

RedCard_Dynamax::
	setword BATTLE_STRING_LOADER gText_DynamaxItemBlock
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto RedCardEnd	

RedCard_Ingrain::
	setword BATTLE_STRING_LOADER RedCardIngrainString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto RedCardEnd

RedCard_SuctionCups::
	setword BATTLE_STRING_LOADER RedCardSuctionCupsString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto RedCardEnd

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_HangedOnFocusSash::
	playanimation BS_TARGET B_ANIM_ITEM_EFFECT 0x0
	printstring 0x12F
	waitmessage DELAY_1SECOND
	removeitem BS_TARGET
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Gems::
	callasm BufferAttackerItem
	pause DELAY_HALFSECOND
	playanimation BS_ATTACKER B_ANIM_ITEM_EFFECT 0x0
	setword BATTLE_STRING_LOADER GemString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	removeitem BS_ATTACKER
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_WeaknessBerryActivate::
	pause DELAY_HALFSECOND
	call BattleScript_TryPrintRipenAbilityPopUp
	playanimation BS_SCRIPTING B_ANIM_BERRY_EAT 0x0
	setword BATTLE_STRING_LOADER WeaknessBerryString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_HALFSECOND
	call BattleScript_CheekPouch
	return

