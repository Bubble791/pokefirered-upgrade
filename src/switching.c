#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "pokemon.h"
#include "dynamax.h"
#include "event_data.h"
#include "util.h"
#include "mega.h"
#include "set_z_effect.h"
#include "item.h"
#include "random.h"
#include "party_menu.h"
#include "frontier.h"
#include "battle_ai_script_commands.h"
#include "constants/species.h"
#include "constants/abilities.h"
#include "constants/battle_ai.h"
#include "constants/battle_move_effects.h"
#include "constants/hold_effects.h"
#include "constants/battle_script_commands.h"
#include "constants/party_menu.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "battle_controllers.h"
#include "battle_message.h"

enum SwitchInStates
{
	SwitchIn_HandleAICooldown,
	SwitchIn_CamomonsReveal,
	SwitchIn_HealingWish,
	SwitchIn_ZHealingWish,
	SwitchIn_Spikes,
	SwitchIn_StealthRock,
	SwitchIn_Steelsurge,
	SwitchIn_ToxicSpikes,
	SwitchIn_StickyWeb,
	SwitchIn_EmergencyExit,
	SwitchIn_PrimalReversion,
	SwitchIn_Truant,
	SwitchIn_Abilities,
	SwitchIn_Items,
	SwitchIn_AirBalloon,
	SwitchIn_TotemPokemon,
	SwitchIn_LastPokemonMusic,
	SwitchIn_TrainerMessage,
	SwitchIn_PreEnd,
	SwitchIn_EjectPack,
	SwitchIn_End,
};

static bool8 TryRemovePrimalWeather(u8 bank, u8 ability);
static bool8 TryRemoveNeutralizingGas(u8 ability);
static bool8 TryRemoveUnnerve(u8 bank);
static bool8 TryActivateFlowerGift(u8 leavingBank);


//This file's functions:
static bool8 TryDoForceSwitchOut(void);
static bool8 PPIsMaxed(u8);
static u8 GetStealthRockDivisor(void);

extern const u8 gText_CamomonsTypeReveal[];
extern const u8 gText_CamomonsTypeRevealDualType[];

extern const u8 gText_NeutralizingGasEnd[];

bool8 HandleSpecialSwitchOutAbilities(u8 bank, ability_t ability)
{
	return TryRemovePrimalWeather(bank, ability)
		|| TryRemoveNeutralizingGas(ability)
		|| TryRemoveUnnerve(bank)
		|| TryActivateFlowerGift(bank);
}

static bool8 TryRemovePrimalWeather(u8 bank, ability_t ability)
{
	int i;
	gBattleStringLoader = NULL;

	switch (ability) {
		case ABILITY_PRIMORDIALSEA:
			if (gBattleWeather & WEATHER_RAIN_PRIMAL)
				gBattleStringLoader = PrimalRainEndString;
			break;
		case ABILITY_DESOLATELAND:
			if (gBattleWeather & WEATHER_SUN_PRIMAL)
				gBattleStringLoader = PrimalSunEndString;
			break;
		case ABILITY_DELTASTREAM:
			if (gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL)
				gBattleStringLoader = PrimalAirCurrentEndString;
	}

	if (gBattleStringLoader != NULL)
	{
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (i == bank) continue;
			if (ABILITY(i) == ability) break;
		}

		if (i == gBattlersCount)
		{
			gBattleWeather = 0;
			gWishFutureKnock.weatherDuration = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_PrimalWeatherEnd;
			return TRUE;
		}
	}

	return FALSE;
}

static bool8 TryRemoveNeutralizingGas(ability_t ability)
{
    int i;

	if (ability == ABILITY_NEUTRALIZINGGAS)
	{
		if (!gBattleStruct->printedNeutralizingGasOverMsg)
		{
			BattleScriptPushCursor();
			gBattleStringLoader = gText_NeutralizingGasEnd;
			gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			gBattleStruct->printedNeutralizingGasOverMsg = TRUE;
			return TRUE;
		}

		for (i = 0; i < gBattlersCount; ++i)
		{
			u8 bank = gBattlerByTurnOrder[i];

			if (gBattleStruct->neutralizingGasBlockedAbilities[bank] != ABILITY_NONE)
			{
				u8 ability = *GetAbilityLocationIgnoreNeutralizingGas(bank) = gBattleStruct->neutralizingGasBlockedAbilities[bank]; //Restore ability
				gBattleStruct->neutralizingGasBlockedAbilities[bank] = ABILITY_NONE;
				gBattleStruct->SlowStartTimers[bank] = 0;
				gDisableStructs[gBattlerTarget].truantCounter = 0;

				//Some abilities don't reactivate
				switch (ability) {
					case ABILITY_UNNERVE:
						break;
					case ABILITY_IMPOSTER: //Never gets another chance
						gStatuses3[bank] |= STATUS3_INTIMIDATE_POKES;
						break;
					default:
						gStatuses3[bank] &= ~STATUS3_INTIMIDATE_POKES;
				}

				if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, bank, 0, 0, 0))
					return TRUE;
			}
		}
	}

	gBattleStruct->printedNeutralizingGasOverMsg = FALSE; //Reset for next time
	return FALSE;
}

static bool8 TryRemoveUnnerve(u8 bank)
{
	u8 side = SIDE(bank);
	bool8 ret = FALSE;
    int i;

	if (ABILITY(bank) == ABILITY_UNNERVE)
	{
		*GetAbilityLocation(bank) = ABILITY_NONE; //Temporarily remove Unnerve so Berries can activate

		for (i = 0; i < gBattlersCount; ++i)
		{
			u8 bank = gBattlerByTurnOrder[i];
			if (SIDE(bank) == side) //Ignore Berries on the side of the Unnerver
				continue;

			if (IsBerry(ITEM(bank)))
			{
				if (ItemBattleEffects(ItemEffects_EndTurn, bank, TRUE, FALSE))
				{
					ret = TRUE;
					break;
				}
			}
		}

		*GetAbilityLocation(bank) = ABILITY_UNNERVE; //Restore Unnerve so loop can continue when we return to this function
	}

	return ret;
}

static bool8 TryActivateFlowerGift(u8 leavingBank)
{
	u32 i = 0;
    u8 bank;

	if (ABILITY(leavingBank) == ABILITY_AIR_LOCK
	||  ABILITY(leavingBank) == ABILITY_CLOUD_NINE)
		gBattleMons[leavingBank].ability = ABILITY_NONE; //Remove ability because we can't have these anymore

	for (bank = gBattlerByTurnOrder[i]; i < gBattlersCount; ++i, bank = gBattlerByTurnOrder[i])
	{
		if (bank == leavingBank)
			continue; //Don't do this form change if you're the bank switching out

		if ((ABILITY(bank) == ABILITY_FLOWERGIFT || ABILITY(bank) == ABILITY_FORECAST)) //Just in case someone with Air Lock/Cloud Nine switches out
		{
			gStatuses3[bank] &= ~STATUS3_INTIMIDATE_POKES;

			if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, bank, 0, 0, 0))
				return TRUE;
		}
	}

	return FALSE;
}

void TryReactivateCentifernoSandblast(u32* status2)
{
    int i;

	for (i = 0; i < gBattlersCount; ++i)
	{
		//Check if special Fire Spin or Sand Tomb were undone by the switch, and they reactivate them
		if (gBattleStruct->sandblastCentiferno[i])
			gBattleMons[i].status2 |= (status2[i] & STATUS2_WRAPPED);
	}
}

void atk52_switchineffects(void)
{
	int i;
	u8 arg = T2_READ_8(gBattlescriptCurrInstr + 1);
	ability_t ability;
	u8 itemEffect;

	if (arg == BS_GET_SCRIPTING_BANK)
		gBattleScripting.battler = gBattleStruct->SentInBackup; //Restore scripting backup b/c can get changed

	gActiveBattler = GetBattlerForBattleScript(arg);
	UpdateSentPokesToOpponentValue(gActiveBattler);
	gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
	gSpecialStatuses[gActiveBattler].flag40 = 0;
	ability = ABILITY(gActiveBattler);
	itemEffect = ITEM_EFFECT(gActiveBattler);

	if (gBattleMons[gActiveBattler].hp == 0)
		goto SWITCH_IN_END;

	if (!(gSideStatuses[SIDE(gActiveBattler)] & SIDE_STATUS_SPIKES) //Skip the entry hazards if there are none
	&& gBattleStruct->switchInEffectsState >= SwitchIn_Spikes
	&& gBattleStruct->switchInEffectsState <= SwitchIn_StickyWeb)
		gBattleStruct->switchInEffectsState = SwitchIn_PrimalReversion;

	switch (gBattleStruct->switchInEffectsState) {
		case SwitchIn_HandleAICooldown:
			if (SIDE(gActiveBattler) == B_SIDE_PLAYER) //Player switched in a Pokemon
			{
				//If the player switches out their Pokemon, allow the AI to immediately switch out if it wants to
				gBattleStruct->ai.switchingCooldown[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)] = 0;
				if (IS_DOUBLE_BATTLE)
					gBattleStruct->ai.switchingCooldown[GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)] = 0;
			}
			else
			{
				gBattleStruct->ai.switchingCooldown[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)] = 0;
				if (IS_DOUBLE_BATTLE)
					gBattleStruct->ai.switchingCooldown[GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)] = 0;
			}

			++gBattleStruct->switchInEffectsState;
			//Fallthrough

		case SwitchIn_CamomonsReveal:
			if (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS)
			{
				gBattleScripting.battler = gActiveBattler;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_CamomonsTypeRevealRet;

				if (gBattleMons[gActiveBattler].type1 == gBattleMons[gActiveBattler].type2)
					gBattleStringLoader = gText_CamomonsTypeReveal;
				else
					gBattleStringLoader = gText_CamomonsTypeRevealDualType;
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, gBattleMons[gActiveBattler].type1);
				PREPARE_TYPE_BUFFER(gBattleTextBuff2, gBattleMons[gActiveBattler].type2);
			}
			++gBattleStruct->switchInEffectsState;
			break;

		case SwitchIn_HealingWish:
			if (gBattleMons[gActiveBattler].hp != gBattleMons[gActiveBattler].maxHP
			|| gBattleMons[gActiveBattler].status1 != STATUS1_NONE)
			{
				if (gBattleStruct->HealingWishLoc & gBitTable[GetBattlerPosition(gActiveBattler)])
				{
					gBattleStruct->HealingWishLoc &= ~gBitTable[GetBattlerPosition(gActiveBattler)];
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_HealingWishHeal;
					gBattleMoveDamage = -1 * (gBattleMons[gActiveBattler].maxHP);
					gBattleMons[gActiveBattler].status1 = 0;
					BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
					gBattleScripting.battler = gActiveBattler;
					gBattlerAttacker = gActiveBattler;
					++gBattleStruct->switchInEffectsState;
					return;
				}
				else if (gBattleStruct->HealingWishLoc & (gBitTable[GetBattlerPosition(gActiveBattler)] << 4))
				{
					goto LUNAR_DANCE_ACTIVATE;
				}
			}
			else if (gBattleStruct->HealingWishLoc & (gBitTable[GetBattlerPosition(gActiveBattler)] << 4) && !PPIsMaxed(gActiveBattler))
			{
			LUNAR_DANCE_ACTIVATE:
				gBattleStruct->HealingWishLoc &= ~(gBitTable[GetBattlerPosition(gActiveBattler)] << 4);
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_LunarDanceHeal;

				gBattleMoveDamage = -1 * (gBattleMons[gActiveBattler].maxHP);
				gBattleMons[gActiveBattler].status1 = 0;
				BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);

				//PP Restored in Battle Script

				gBattleScripting.battler = gActiveBattler;
				gBattlerAttacker = gActiveBattler;
				++gBattleStruct->switchInEffectsState;
				return;
			}
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_ZHealingWish:
			if (gBattleMons[gActiveBattler].hp != gBattleMons[gActiveBattler].maxHP && gBattleStruct->zMoveData.healReplacement)
			{
				gBattleStruct->zMoveData.healReplacement = FALSE;
				gBattleMoveDamage = -1 * (gBattleMons[gActiveBattler].maxHP);
				gBattleScripting.battler = gActiveBattler;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_HealReplacementZMove;
				return;
			}
			else
				gBattleStruct->zMoveData.healReplacement = FALSE;

			gBattleStruct->DamageTaken[gActiveBattler] = 0;
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_Spikes:
			if (CheckGrounding(gActiveBattler)
			&& gSideTimers[SIDE(gActiveBattler)].spikesAmount > 0
			&& ability != ABILITY_MAGICGUARD
			&& itemEffect != HOLD_EFFECT_HEAVY_DUTY_BOOTS)
			{
				gBattleMoveDamage = CalcSpikesDamage(gActiveBattler);
				gBattleStruct->DamageTaken[gActiveBattler] += gBattleMoveDamage;

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SpikesHurt;
				gSideStatuses[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting.battler = gActiveBattler;
				gBattlerTarget = gActiveBattler;
				//gBattlerAttacker = FOE(gActiveBattler); //For EXP
				++gBattleStruct->switchInEffectsState;
				return;
			}
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_StealthRock:
			if (gSideTimers[SIDE(gActiveBattler)].srAmount > 0
			&& ability != ABILITY_MAGICGUARD
			&& itemEffect != HOLD_EFFECT_HEAVY_DUTY_BOOTS)
			{
				gBattleMoveDamage = CalcStealthRockDamage(gActiveBattler);
				gBattleStruct->DamageTaken[gActiveBattler] += gBattleMoveDamage;

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SRHurt;
				gSideStatuses[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting.battler = gActiveBattler;
				gBattlerTarget = gActiveBattler;
				//gBattlerAttacker = FOE(gActiveBattler); //For EXP
				++gBattleStruct->switchInEffectsState;
				return;
			}
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_Steelsurge:
			if (gSideTimers[SIDE(gActiveBattler)].steelsurge > 0
			&& ability != ABILITY_MAGICGUARD
			&& itemEffect != HOLD_EFFECT_HEAVY_DUTY_BOOTS)
			{
				gBattleMoveDamage = CalcSteelsurgeDamage(gActiveBattler);
				gBattleStruct->DamageTaken[gActiveBattler] += gBattleMoveDamage;

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SteelsurgeHurt;
				gSideStatuses[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattlerTarget = gBattleScripting.battler = gActiveBattler;
				//gBattlerAttacker = FOE(gActiveBattler); //For EXP
				++gBattleStruct->switchInEffectsState;
				return;
			}
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_ToxicSpikes:
			if (gSideTimers[SIDE(gActiveBattler)].tspikesAmount > 0
			&& CheckGrounding(gActiveBattler))
			{
				if (IsOfType(gActiveBattler, TYPE_POISON))
				{
					gSideTimers[SIDE(gActiveBattler)].tspikesAmount = 0;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TSAbsorb;
				}
				else if (itemEffect != HOLD_EFFECT_HEAVY_DUTY_BOOTS) //Pokemon with this item can still remove T-Spikes
				{
					if (gSideTimers[SIDE(gActiveBattler)].tspikesAmount == 1)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_TSPoison;
					}
					else
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_TSHarshPoison;
					}

					gSideStatuses[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				}

				gBattleScripting.battler = gActiveBattler;
				gBattlerTarget = gActiveBattler;
				//gBattlerAttacker = FOE(gActiveBattler); //For EXP
				++gBattleStruct->switchInEffectsState;
				return;
			}
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_StickyWeb:
			if (gSideTimers[SIDE(gActiveBattler)].stickyWeb
			&&  CheckGrounding(gActiveBattler)
			&& itemEffect != HOLD_EFFECT_HEAVY_DUTY_BOOTS)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_StickyWebSpeedDrop;
				gSideStatuses[SIDE(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
				gBattleScripting.battler = gActiveBattler;
				gBattlerTarget = gActiveBattler;
				//gBattlerAttacker = FOE(gActiveBattler); //For EXP
				++gBattleStruct->switchInEffectsState;
				return;
			}
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_EmergencyExit:
			if (ABILITY(gActiveBattler) == ABILITY_EMERGENCYEXIT)
			//||  ABILITY(gActiveBattler) == ABILITY_WIMPOUT)
			{
				if (gBattleMons[gActiveBattler].hp > 0
				&&  gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 2
				&&  gBattleMons[gActiveBattler].hp + gBattleStruct->DamageTaken[gActiveBattler] > gBattleMons[gActiveBattler].maxHP / 2)
				{
					BattleScriptPush(gBattlescriptCurrInstr + 2);
					gBattlescriptCurrInstr = BattleScript_EmergencyExit;
					return;
				}
			}
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_PrimalReversion:
		{
			const u8* script = DoPrimalReversion(gActiveBattler, 1);
			if(script)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = script;
				gBattleScripting.battler = gActiveBattler;
				gBattlerAttacker = gActiveBattler;
				++gBattleStruct->switchInEffectsState;
				return;
			}
		}
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_Truant:
			if (ABILITY(gActiveBattler) == ABILITY_TRUANT)
				gDisableStructs[gActiveBattler].truantCounter ^= 1;
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_Abilities:
			for (i = 0; i < gBattlersCount; ++i)
			{
				if (i != gActiveBattler
				&& ABILITY(i) == ABILITY_TRACE
				&& AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, i, 0, 0, 0))
					return;
			}

			if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gActiveBattler, 0, 0, 0))
				return;

			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_Items:
			if (ItemBattleEffects(ItemEffects_SwitchIn, gActiveBattler, TRUE, FALSE))
				return;

			if (ItemBattleEffects(ItemEffects_EndTurn, gActiveBattler, TRUE, FALSE))
				return;
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_AirBalloon:
			if (gActiveBattler < gBattlersCount && ITEM_EFFECT(gActiveBattler) == HOLD_EFFECT_AIR_BALLOON)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_AirBalloonSub;
				gBattleScripting.battler = gActiveBattler;
				gBattlerAttacker = gActiveBattler;
				++gBattleStruct->switchInEffectsState;
				return;
			}
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_TotemPokemon:
		{
			u8 totemBoostType = CanActivateTotemBoost(gActiveBattler);

			if (totemBoostType == TOTEM_SINGLE_BOOST)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_TotemRet;
				gBattlerAttacker = gBattleScripting.battler = gActiveBattler;
				++gBattleStruct->switchInEffectsState;
				return;
			}
			else if (totemBoostType == TOTEM_OMNIBOOST)
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_TotemOmniboostRet;
				gBattlerAttacker = gBattleScripting.battler = gActiveBattler;
				++gBattleStruct->switchInEffectsState;
				return;
			}
		}
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_LastPokemonMusic:
			++gBattleStruct->switchInEffectsState;
			#ifdef BGM_BATTLE_GYM_LEADER_LAST_POKEMON
			if (gBattleTypeFlags & BATTLE_TYPE_TRAINER
			&& !(gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_TOWER))
			&& gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_LEADER
			&& SIDE(gActiveBattler) == B_SIDE_OPPONENT //So player accepting offer to switching out doesn't trigger this yet
			&& ViableMonCount(gEnemyParty) <= 1)
			{
				PlayBGM(BGM_BATTLE_GYM_LEADER_LAST_POKEMON);
			}
			#endif
		

		case SwitchIn_TrainerMessage:
			++gBattleStruct->switchInEffectsState;
			if (ShouldDoTrainerSlide(gActiveBattler, gTrainerBattleOpponent_A, TRAINER_SLIDE_LAST_SWITCHIN))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_TrainerSlideMsgRet;
				return;
			}
		

		case SwitchIn_PreEnd:
		SWITCH_IN_END:
			gSideStatuses[SIDE(gActiveBattler)] &= ~(SIDE_STATUS_SPIKES_DAMAGED);

			for (i = 0; i < gBattlersCount; ++i)
			{
				if (gBattlerByTurnOrder[i] == gActiveBattler)
					gActionsByTurnOrder[i] = B_ACTION_FINISHED;
			}

			for (i = 0; i < gBattlersCount; i++)
				gBattleStruct->hpOnSwitchout[SIDE(i)] = gBattleMons[i].hp;

			if (T2_READ_8(gBattlescriptCurrInstr + 1) == 5)
			{
				u32 hitmarkerFaintBits = gHitMarker >> 0x1C;
				++gBattlerFainted;
				while (1)
				{
					if (hitmarkerFaintBits & gBitTable[gBattlerFainted] && !(gAbsentBattlerFlags & gBitTable[gBattlerFainted]))
						break;
					if (gBattlerFainted >= gBattlersCount)
						break;
					++gBattlerFainted;
				}
			}
			++gBattleStruct->switchInEffectsState;
		

		case SwitchIn_EjectPack:
			gBattleStruct->switchInEffectsState = 0;
			gBattlescriptCurrInstr += 2;

			for (i = 0; i < gBattlersCount; ++i)
			{
				if (ITEM_EFFECT(i) == HOLD_EFFECT_EJECT_PACK && ItemBattleEffects(ItemEffects_SwitchIn, i, TRUE, FALSE))  //Try to trigger Eject Packs after Intimidate
					return;
			}
		

		case SwitchIn_End:
			break;
	}
}

void RestorePPLunarDance(void)
{
	u8 i, maxPP;
	gActiveBattler = gBattleScripting.battler;

	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		maxPP = 0;
		if (gBattleMons[gActiveBattler].moves[i] != MOVE_NONE)
		{
			maxPP = CalculatePPWithBonus(gBattleMons[gActiveBattler].moves[i], gBattleMons[gActiveBattler].ppBonuses, i);
			if (IS_TRANSFORMED(gActiveBattler) && maxPP > 5)
				maxPP = 5; //Can't restore past 5 PP if transformed
		}

		if (gBattleMons[gActiveBattler].pp[i] != maxPP)
		{
			gBattleMons[gActiveBattler].pp[i] = maxPP;
			BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + i, 0, 1, &gBattleMons[gActiveBattler].pp[i]); //Restore PP, one move at a time
			MarkBattlerForControllerExec(gActiveBattler);
			gBattlescriptCurrInstr -= 5;
			return;
		}
	}
}

//Ripped from PokeEmerald
void atk8F_forcerandomswitch(void)
{
	int i;
	u8 bankDef, firstMonId, lastMonId;
	u8 battler1PartyId = 0;
	u8 battler2PartyId = 0;
	u8 monsCount = 0;
	u8 validMons = 0;
	u8 minNeeded = 1;
	struct Pokemon* party = NULL;
	
	bankDef = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

	if (gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE)) //Trainer Battle or Wild Double Battle
	{
		party = LoadPartyRange(bankDef, &firstMonId, &lastMonId);

		if ((gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
		||  (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_FRONTIER && SIDE(bankDef) == B_SIDE_PLAYER)
		||  (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && SIDE(bankDef) == B_SIDE_OPPONENT)
		||  (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && SIDE(bankDef) == B_SIDE_PLAYER))
		{
			monsCount = 3;
			battler1PartyId = gBattlerPartyIndexes[bankDef];
			battler2PartyId = gBattlerPartyIndexes[PARTNER(bankDef)];
		}
		else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		{
			monsCount = 6;
			battler1PartyId = gBattlerPartyIndexes[bankDef];
			battler2PartyId = gBattlerPartyIndexes[PARTNER(bankDef)];
		}
		else //Single Battle
		{
			monsCount = 6;
			battler1PartyId = gBattlerPartyIndexes[bankDef]; // there is only one pokemon out in single battles
			battler2PartyId = gBattlerPartyIndexes[bankDef];
		}

		for (i = firstMonId; i < lastMonId; ++i)
		{
			if (i != battler1PartyId
			&&  i != battler2PartyId
			&& MON_CAN_BATTLE(&party[i]))
				validMons++;
		}

		if (validMons < minNeeded)
		{
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
		}
		else
		{
			if (TryDoForceSwitchOut())
			{
				do
				{
					i = Random() % monsCount;
					i += firstMonId;
				}
				while (i == battler1PartyId
					|| i == battler2PartyId
					|| !MON_CAN_BATTLE(&party[i]));
			}
			gBattleStruct->monToSwitchIntoId[bankDef] = i;

			if (!IsLinkDoubleBattle() && !IsTagBattle())
				UpdatePartyOwnerOnSwitch_NonMulti(bankDef);
			else if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_FRONTIER))
			{
				SwitchPartyOrderLinkMulti(bankDef, i, 0);
				SwitchPartyOrderLinkMulti(PARTNER(bankDef), i, 1);
			}
			else if (IsTagBattle())
				SwitchPartyOrderInGameMulti(bankDef, i);
		}
	}
	else //Regular Wild Battle
		TryDoForceSwitchOut();
}

static bool8 TryDoForceSwitchOut(void)
{
	u8 bankDef = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8 bankAtk = GetBattlerForBattleScript(gBattlescriptCurrInstr[2]);

	if (IsDynamaxed(bankDef)) //Can't force out a Dynamaxed mon
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
		return FALSE;
	}

	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		gBankSwitching = bankDef;
		gBattleStruct->battlerPartyIndexes[bankDef] = gBattlerPartyIndexes[bankDef];
		gBattlescriptCurrInstr = BattleScript_SuccessForceOut;
		return TRUE;
	}

	//If Wild Battle
	else if (gBattleMons[bankAtk].level < gBattleMons[bankDef].level)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
		return FALSE;
	}

	//Roar always fails in Wild Double Battles if used on the wild mon
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && SIDE(bankDef) == B_SIDE_OPPONENT)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
		return FALSE;
	}
	
	//Roar always fails in wild boss battles
	else if (AreAllKindsOfRunningPrevented())
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
		return FALSE;
	}	

	gBankSwitching = bankDef;
	gBattleStruct->battlerPartyIndexes[bankDef] = gBattlerPartyIndexes[bankDef];
	gBattlescriptCurrInstr = BattleScript_SuccessForceOut;
	return TRUE;
}

void SwitchPartyOrderInGameMulti(u8 bank, u8 monToSwitchIntoId)
{	//0x8013F6C in FR
	if (SIDE(bank) != B_SIDE_OPPONENT)
	{
		s32 i;

		// gBattleStruct->battlerPartyOrders[0][i]

		for (i = 0; i < 3; i++)
			gBattlePartyCurrentOrder[i] = gBattleStruct->battlerPartyOrders[0][i];

		SwitchPartyMonSlots(GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[bank]), GetPartyIdFromBattlePartyId(monToSwitchIntoId));

		for (i = 0; i < 3; i++)
			gBattleStruct->battlerPartyOrders[0][i] = gBattlePartyCurrentOrder[i];
	}
}

static bool8 PPIsMaxed(u8 bank)
{
	int i;
	for (i = 0; i < 4; ++i)
	{
		if (gBattleMons[bank].moves[i] == MOVE_NONE) break;

		if (gBattleMons[bank].pp[i] < CalculatePPWithBonus(gBattleMons[bank].moves[i], gBattleMons[bank].ppBonuses, i))
			return FALSE;
	}
	return TRUE;
}

bool8 HasNoMonsToSwitch(u8 battler)
{
	struct Pokemon* party;
	u8 firstMonId, lastMonId, battlerIn1, battlerIn2;
	int i;

	if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
		return FALSE;

	party = LoadPartyRange(battler, &firstMonId, &lastMonId);

	battlerIn1 = battler;
	if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(GetBattlerPosition(battler) ^ BIT_FLANK)])
		battlerIn2 = battler;
	else
		battlerIn2 = GetBattlerAtPosition(GetBattlerPosition(battler) ^ BIT_FLANK);

	for (i = firstMonId; i < lastMonId; ++i)
	{
		if (GetMonData(&party[i], MON_DATA_HP, 0) != 0
		&& GetMonData(&party[i], MON_DATA_SPECIES, 0) != SPECIES_NONE
		&& GetMonData(&party[i], MON_DATA_SPECIES2, 0) != SPECIES_EGG
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, 0)
		&& i != gBattlerPartyIndexes[battlerIn1]
		&& i != gBattlerPartyIndexes[battlerIn2])
			break;
	}

	return (i == lastMonId);
}

void ClearSwitchBytes(u8 bank)
{
	gBattleStruct->TelekinesisTimers[bank] = 0;
	gBattleStruct->HealBlockTimers[bank] = 0;
	gBattleStruct->LaserFocusTimers[bank] = 0;
	gBattleStruct->ThroatChopTimers[bank] = 0;
	gBattleStruct->StompingTantrumTimers[bank] = 0;
	gBattleStruct->MetronomeCounter[bank] = 0;
	gBattleStruct->DestinyBondCounters[bank] = 0;
	gBattleStruct->SlowStartTimers[bank] = 0;
	gBattleStruct->IncinerateCounters[bank] = 0;
	gBattleStruct->EmbargoTimers[bank] = 0;
	gBattleStruct->DisabledMoldBreakerAbilities[bank] = 0;
	gBattleStruct->SuppressedAbilities[bank] = 0;
	gBattleStruct->neutralizingGasBlockedAbilities[bank] = 0;
	gBattleStruct->lastTargeted[bank] = 0;
	gBattleStruct->usedMoveIndices[bank] = 0;
	gBattleStruct->synchronizeTarget[bank] = 0;
	gBattleStruct->statFellThisTurn[bank] = FALSE;
	gBattleStruct->dynamaxData.timer[bank] = 0;
	gBattleStruct->zMoveData.toBeUsed[bank] = 0; //Force switch or fainted before Z-Move could be used
	gBattleStruct->chiStrikeCritBoosts[bank] = 0;
	gBattleStruct->sandblastCentiferno[bank] = 0;
	gBattleStruct->disguisedAs[bank] = 0;

	gProtectStructs[bank].KingsShield = 0;	//Necessary because could be sent away with Roar
	gProtectStructs[bank].SpikyShield = 0;
	gProtectStructs[bank].BanefulBunker = 0;
	gProtectStructs[bank].obstruct = 0;
	gProtectStructs[bank].enduredSturdy = 0;
	
	DestroyMegaIndicator(bank);
	ClearBattlerAbilityHistory(bank);
	ClearBattlerItemEffectHistory(bank);
}

void ClearSwitchBits(u8 bank)
{
	gBattleStruct->PowderByte &= ~(gBitTable[bank]);
	gBattleStruct->quashed &= ~(gBitTable[bank]);
	gBattleStruct->BeakBlastByte &= ~(gBitTable[bank]);
	gBattleStruct->tarShotBits &= ~(gBitTable[bank]);
	gBattleStruct->trappedByOctolock &= ~(gBitTable[bank]);
	gBattleStruct->trappedByNoRetreat &= ~(gBitTable[bank]);
	gBattleStruct->UnburdenBoosts &= ~(gBitTable[bank]);
	gBattleStruct->IllusionBroken &= ~(gBitTable[bank]);
	gBattleStruct->brokeFreeMessage &= ~(gBitTable[bank]);
	gBattleStruct->CustapQuickClawIndicator &= ~(gBitTable[bank]);
	gBattleStruct->devolveForgotMove &= ~(gBitTable[bank]);
}

void PartyMenuSwitchingUpdate(void)
{
	int i;

	if (IsOfType(gActiveBattler, TYPE_GHOST)
	||  ITEM_EFFECT(gActiveBattler) == HOLD_EFFECT_SHED_SHELL)
		goto SKIP_SWITCH_BLOCKING_CHECK;

	gBattleStruct->battlerPartyIndexes[gActiveBattler] = gBattlerPartyIndexes[gActiveBattler];
	if ((gBattleMons[gActiveBattler].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION))
	|| (gStatuses3[gActiveBattler] & (STATUS3_ROOTED | STATUS3_SKY_DROP_TARGET))
	|| IsFairyLockActive())
	{
		BtlController_EmitChoosePokemon(0, PARTY_ACTION_CANT_SWITCH, 6, ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
	}
	else if (((i = ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_SHADOWTAG)) && ABILITY(gActiveBattler) != ABILITY_SHADOWTAG)
		 ||  ((i = ABILITY_ON_OPPOSING_FIELD(gActiveBattler, ABILITY_ARENATRAP)) && CheckGrounding(gActiveBattler))
		 ||  ((i = AbilityBattleEffects(ABILITYEFFECT_CHECK_FIELD_EXCEPT_BATTLER, gActiveBattler, ABILITY_MAGNETPULL, 0, 0))
				 && IsOfType(gActiveBattler, TYPE_STEEL)))
	{
		BtlController_EmitChoosePokemon(0, ((i - 1) << 4) | PARTY_ACTION_ABILITY_PREVENTS, 6, gLastUsedAbility, gBattleStruct->battlerPartyOrders[gActiveBattler]);
	}
	else
	{
	SKIP_SWITCH_BLOCKING_CHECK:
		if (gActiveBattler == B_POSITION_PLAYER_RIGHT && gChosenActionByBattler[0] == B_ACTION_SWITCH)
			BtlController_EmitChoosePokemon(0, PARTY_ACTION_CHOOSE_MON, gBattleStruct->monToSwitchIntoId[0], ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
		else if (gActiveBattler == B_POSITION_OPPONENT_RIGHT && gChosenActionByBattler[1] == B_ACTION_SWITCH)
			BtlController_EmitChoosePokemon(0, PARTY_ACTION_CHOOSE_MON, gBattleStruct->monToSwitchIntoId[1], ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
		else
			BtlController_EmitChoosePokemon(0, PARTY_ACTION_CHOOSE_MON, 6, ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
	}
	MarkBattlerForControllerExec(gActiveBattler);
}

u32 CalcSpikesDamage(u8 bank)
{
	u32 dmg;
	if (ITEM_EFFECT(bank) == HOLD_EFFECT_HEAVY_DUTY_BOOTS)
		return 0;

	dmg = (5 - gSideTimers[SIDE(bank)].spikesAmount) * 2;
	return MathMax(1, gBattleMons[bank].maxHP / dmg);
}

u32 CalcSpikesDamagePartyMon(struct Pokemon* mon, u8 side)
{
	u32 dmg;
	if (GetMonItemEffect(mon) == HOLD_EFFECT_HEAVY_DUTY_BOOTS)
		return 0;

	dmg = (5 - gSideTimers[side].spikesAmount) * 2;
	return MathMax(1, GetMonData(mon, MON_DATA_MAX_HP, NULL) / dmg);
}

u32 CalcStealthRockDamage(u8 bank)
{
	u8 flags;
	u8 divisor = 8;
	gBattleMoveDamage = 40;

	if (ITEM_EFFECT(bank) == HOLD_EFFECT_HEAVY_DUTY_BOOTS)
		return 0;

	TypeDamageModification(0, bank, MOVE_STEALTHROCK, TYPE_ROCK, &flags);
	divisor = GetStealthRockDivisor();

	return MathMax(1, gBattleMons[bank].maxHP / divisor);
}

u32 CalcStealthRockDamagePartyMon(struct Pokemon* mon)
{
	u8 flags;
	u8 divisor = 8;
	gBattleMoveDamage = 40;

	if (GetMonItemEffect(mon) == HOLD_EFFECT_HEAVY_DUTY_BOOTS)
		return 0;

	TypeDamageModificationPartyMon(0, mon, MOVE_STEALTHROCK, TYPE_ROCK, &flags);
	divisor = GetStealthRockDivisor();

	return MathMax(1, GetMonData(mon, MON_DATA_MAX_HP, NULL) / divisor);
}

u32 CalcSteelsurgeDamage(u8 bank)
{
	u8 flags;
	u8 divisor = 8;
	gBattleMoveDamage = 40;

	if (ITEM_EFFECT(bank) == HOLD_EFFECT_HEAVY_DUTY_BOOTS)
		return 0;

	TypeDamageModification(0, bank, MOVE_IRONHEAD, TYPE_STEEL, &flags);
	divisor = GetStealthRockDivisor();

	return MathMax(1, gBattleMons[bank].maxHP / divisor);
}

u32 CalcSteelsurgeDamagePartyMon(struct Pokemon* mon)
{
	u8 flags;
	u8 divisor = 8;
	gBattleMoveDamage = 40;

	if (GetMonItemEffect(mon) == HOLD_EFFECT_HEAVY_DUTY_BOOTS)
		return 0;

	TypeDamageModificationPartyMon(0, mon, MOVE_IRONHEAD, TYPE_STEEL, &flags);
	divisor = GetStealthRockDivisor();

	return MathMax(1, GetMonData(mon, MON_DATA_MAX_HP, NULL) / divisor);
}

static u8 GetStealthRockDivisor(void)
{
	u8 divisor = 1;

	switch (gBattleMoveDamage) {
		case 5:
			divisor = 64;
			break;
		case 10:
			divisor = 32;
			break;
		case 20:
			divisor = 16;
			break;
		case 40:
			divisor = 8;
			break;
		case 80:
			divisor = 4;
			break;
		case 160:
			divisor = 2;
			break;
		case 320:
			divisor = 1;
	}

	return divisor;
}

bool8 WillFaintFromEntryHazards(struct Pokemon* mon, u8 side)
{
	u16 hp = GetMonData(mon, MON_DATA_HP, NULL);
	u32 dmg = 0;

	if (gSideStatuses[side] & SIDE_STATUS_SPIKES
	&& GetMonAbility(mon) != ABILITY_MAGICGUARD
	&& ItemId_GetHoldEffect(GetMonData(mon, MON_DATA_HELD_ITEM, NULL)) != HOLD_EFFECT_HEAVY_DUTY_BOOTS)
	{
		if (gSideTimers[side].srAmount > 0)
			dmg += CalcStealthRockDamagePartyMon(mon);

		if (gSideTimers[side].steelsurge > 0)
			dmg += CalcSteelsurgeDamagePartyMon(mon);

		if (gSideTimers[side].spikesAmount > 0)
			dmg += CalcSpikesDamagePartyMon(mon, side);

		if (dmg >= hp)
			return TRUE;
	}

	return FALSE;
}
