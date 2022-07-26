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

BattleScript_CouldntFullyProtect::
	waitmessage DELAY_1SECOND
	setword gBattleStringLoader, gText_CouldntFullyProtect
	printstring STRINGID_CUSTOMSTRING
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PoisonTouch::
	setbyte POISONED_BY 0x1
	setbyte cEFFECT_CHOOSER 0x2
	seteffectsecondary @;Affected by Safeguard
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_KingsShield::
	setbyte sSTATCHANGER STAT_ATK | 0xA0
	
BattleScript_KingsShieldPostDecrementSet::
	swapattackerwithtarget @;Allows for abilities like Defiant and Mirror Armor to have their proper effect
	statbuffchange STAT_TARGET | STAT_NOT_PROTECT_AFFECTED | STAT_BS_PTR KingsShieldReturn
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 KingsShieldReturn
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

KingsShieldReturn::
	swapattackerwithtarget
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_SpikyShield::
	orword HIT_MARKER HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	setword BATTLE_STRING_LOADER SpikyShieldString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	tryfaintmon BS_ATTACKER 0x0 0x0
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BanefulBunker::
	statusanimation BS_ATTACKER
	updatestatusicon BS_ATTACKER
	setword BATTLE_STRING_LOADER BanefulBunkerPSNString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ObstructStatDecrement::
	setbyte sSTATCHANGER STAT_DEF | 0xA0
	goto BattleScript_KingsShieldPostDecrementSet

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RageIsBuilding::
	setbyte sSTATCHANGER STAT_ATK | 0x10
	statbuffchange STAT_TARGET | STAT_BS_PTR | STAT_CERTAIN RageReturn
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 RageReturn
	setgraphicalstatchangevalues
	playanimation BS_TARGET B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	jumpifability BS_TARGET ABILITY_CONTRARY RageContraryBS

RagePrintString::
	printstring STRINGID_PKMNRAGEBUILDING
	waitmessage DELAY_1SECOND
	
RageReturn::
	return
	
RageContraryBS:: @;Rage says "Attack fell!" if the target has Contrary. Only the rage string is printed if the stat rises
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	goto RagePrintString

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BeakBlastBurn::
	orword HIT_MARKER 0x2100
	swapattackerwithtarget
	setbyte POISONED_BY 0x1
	setbyte cEFFECT_CHOOSER 0x3
	seteffectprimary
	swapattackerwithtarget
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Magician::
	call BattleScript_AbilityPopUp 
	setmoveeffect MOVE_EFFECT_STEAL_ITEM
	setmoveeffect2
	callasm TryHideActiveAbilityPopUps
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Moxie::
	statbuffchange STAT_ATTACKER | STAT_BS_PTR MoxieReturnPostBuff
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x2 MoxieReturnPostBuff
	call BattleScript_AbilityPopUp
	playanimation BS_ATTACKER B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
MoxieReturnPostBuff::
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MindBlownDamage::
	orword HIT_MARKER 0x100 | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	setword BATTLE_STRING_LOADER MindBlownString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	tryfaintmon BS_ATTACKER 0x0 0x0
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ExplosionAnim::
	callasm PlayAttackAnimationForExplosion + 1
	waitanimation
	
BattleScript_FaintAttackerForExplosion::
	tryfaintmon BS_ATTACKER 0x0 0x0
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_LifeOrbDamage::
	orword HIT_MARKER 0x100 | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	setword BATTLE_STRING_LOADER LifeOrbString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	tryfaintmon BS_ATTACKER 0x0 0x0
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_Pickpocket::
	call BattleScript_AbilityPopUp
	setmoveeffect MOVE_EFFECT_STEAL_ITEM
	swapattackerwithtarget
	setmoveeffect2
	swapattackerwithtarget
	callasm TryActivateTargetEndTurnItemEffect
	callasm TryHideActiveAbilityPopUps
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PoisonedBy::
	jumpifbyte EQUALS POISONED_BY 0x1 PoisonTouchPSN
	statusanimation 0x2
	jumpifbyte EQUALS POISONED_BY 0x2 ToxicSpikesPSN
	jumpifbyte EQUALS POISONED_BY 0x3 ToxicOrbBadPSN
	jumpifbyte EQUALS POISONED_BY 0x4 BanefulBunkerPSN
	printfromtable gGotPoisonedStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_UpdateEffectStatusIconRet

PoisonTouchPSN::
	setbyte POISONED_BY 0x0
	call BattleScript_AbilityPopUp
	statusanimation 0x2
	printfromtable gGotPoisonedStringIds
	waitmessage DELAY_1SECOND
	call BattleScript_AbilityPopUpRevert
	goto BattleScript_UpdateEffectStatusIconRet

ToxicSpikesPSN::
	setbyte POISONED_BY 0x0
	setword BATTLE_STRING_LOADER ToxicSpikesPSNString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_UpdateEffectStatusIconRet

BanefulBunkerPSN::
	setbyte POISONED_BY 0x0
	setword BATTLE_STRING_LOADER BanefulBunkerPSNString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_UpdateEffectStatusIconRet

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BadPoisonedBy::
	statusanimation 0x2
	jumpifbyte EQUALS POISONED_BY 0x1 PoisonTouchPSN
	jumpifbyte EQUALS POISONED_BY 0x2 ToxicSpikesBadPSN
	jumpifbyte EQUALS POISONED_BY 0x3 ToxicOrbBadPSN
	jumpifbyte EQUALS POISONED_BY 0x4 BanefulBunkerPSN
	printstring 0x2C
	waitmessage DELAY_1SECOND
	goto BattleScript_UpdateEffectStatusIconRet

ToxicSpikesBadPSN::
	setbyte POISONED_BY 0x0
	setword BATTLE_STRING_LOADER ToxicSpikesBadPSNString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_UpdateEffectStatusIconRet

ToxicOrbBadPSN::
	setbyte POISONED_BY 0x0
	setword BATTLE_STRING_LOADER ToxicOrbString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_UpdateEffectStatusIconRet

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BurnedBy::
	statusanimation 0x2
	jumpifbyte EQUALS POISONED_BY 0x1 BeakBlastBurnBS
	jumpifbyte EQUALS POISONED_BY 0x3 FlameOrbBurnBS
	printfromtable gGotBurnedStringIds
	waitmessage DELAY_1SECOND
	goto BattleScript_UpdateEffectStatusIconRet

BeakBlastBurnBS::
	setbyte POISONED_BY 0x0
	setword BATTLE_STRING_LOADER BeakBlastString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_UpdateEffectStatusIconRet

FlameOrbBurnBS::
	setbyte POISONED_BY 0x0
	setword BATTLE_STRING_LOADER FlameOrbString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	goto BattleScript_UpdateEffectStatusIconRet
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_ItemSteal::
	callasm TransferLastUsedItem
	playanimation BS_TARGET B_ANIM_ITEM_STEAL 0x0
	printstring STRINGID_PKMNSTOLEITEM
	waitmessage DELAY_1SECOND
	call BattleScript_HandleWeatherFormChanges @;BattleScript_WeatherFormChanges - In case of Utility Umbrella
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DancerActivated::
	call BattleScript_AbilityPopUp
	pause 0x40
	call BattleScript_AbilityPopUpRevert
	jumptocalledmove 0xFF

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_PluckEat::
	setword BATTLE_STRING_LOADER PluckString
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	callasm PluckBerryEat
	callasm ClearDoingPluckItemEffect
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaidShields::
	setword BATTLE_STRING_LOADER gText_RaidPokemonGettingDesperate
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	callasm CreateRaidShieldSprites
	setword BATTLE_STRING_LOADER gText_RaidShield
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BrokenRaidBarrier::
	setword BATTLE_STRING_LOADER gText_RaidShieldBroke
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	orword HIT_MARKER, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	tryfaintmon BS_TARGET 0x0 0x0
	jumpifhasnohp BS_TARGET BattleScript_BrokenRaidBarrierEnd
	setbyte sSTAT_ANIM_PLAYED 0x0
	playstatchangeanimation BS_TARGET, STAT_ANIM_DEF | STAT_ANIM_SPDEF, STAT_ANIM_DOWN | STAT_ANIM_BY_TWO | STAT_ANIM_ONLY_MULTIPLE

BattleScript_BrokenRaidBarrier_Def::
	playstatchangeanimation BS_TARGET, STAT_ANIM_DEF, STAT_ANIM_DOWN | STAT_ANIM_BY_TWO
	setbyte sSTATCHANGER STAT_DEF | DECREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_BrokenRaidBarrierPrintDefMsg
BattleScript_BrokenRaidBarrierPrintDefMsg::
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x3 BattleScript_BrokenRaidBarrier_SpDef
	jumpifbyte EQUALS cMULTISTRING_CHOOSER 0x4 BattleScript_BrokenRaidBarrierEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

BattleScript_BrokenRaidBarrier_SpDef::
	playstatchangeanimation BS_TARGET, STAT_ANIM_SPDEF, STAT_ANIM_DOWN | STAT_ANIM_BY_TWO
	setbyte sSTATCHANGER STAT_SPDEF | DECREASE_2
	statbuffchange STAT_TARGET | STAT_BS_PTR BattleScript_BrokenRaidBarrierPrintSpDefMsg
BattleScript_BrokenRaidBarrierPrintSpDefMsg::
	jumpifbyte GREATERTHAN cMULTISTRING_CHOOSER 0x2 BattleScript_BrokenRaidBarrierEnd
	printfromtable gStatDownStringIds
	waitmessage DELAY_1SECOND

BattleScript_BrokenRaidBarrierEnd::
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaidBattleStatIncrease::
	setgraphicalstatchangevalues
	playanimation BS_SCRIPTING B_ANIM_STATS_CHANGE sB_ANIM_ARG1
	setword BATTLE_STRING_LOADER gText_RaidBattleStatBoost
	printstring STRINGID_CUSTOMSTRING
	waitmessage DELAY_1SECOND
	return
