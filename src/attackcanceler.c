#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "battle_string_ids.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "util.h"
#include "item.h"
#include "random.h"
#include "pokemon.h"
#include "battle_ai_script_commands.h"
#include "menu.h"
#include "malloc.h"
#include "dynamax.h"
#include "string_util.h"
#include "window.h"
#include "sound.h"
#include "event_data.h"
#include "constants/abilities.h"
#include "constants/flags.h"
#include "constants/battle_move_effects.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"
#include "constants/pokemon.h"

static bool8 DoesTargetHaveAbilityImmunity(void);
static u8 IsMonDisobedient(void);

void atk00_attackcanceler(void)
{
	int i;

	if (gBattleOutcome != 0)
	{
		gCurrentActionFuncId = B_ACTION_FINISHED;
		return;
	}

	ResetBestMonToSwitchInto(gBattlerAttacker);

	if ((gBattleMons[gBattlerAttacker].hp == 0
	|| (gStatuses3[gBattlerAttacker] & STATUS3_SKY_DROP_TARGET)
	|| (gBattleStruct->NoMoreMovingThisTurn & gBitTable[gBattlerAttacker])) //From Smack Down/Gravity
		&& !(gHitMarker & HITMARKER_NO_ATTACKSTRING))
	{
		gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
		gBattlescriptCurrInstr = BattleScript_MoveEnd;
		return;
	}

	if (!NO_MOLD_BREAKERS(ABILITY(gBattlerAttacker), gCurrentMove) || gBattleStruct->dynamaxData.nullifiedStats) //There is a Mold Breaker
	{
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (i == gBattlerAttacker) continue;

			if (gMoldBreakerIgnoredAbilities[ABILITY(i)])
			{
				gBattleStruct->DisabledMoldBreakerAbilities[i] = gBattleMons[i].ability; //Temporarily disable all relevant abilities on the field
				gBattleMons[i].ability = ABILITY_NONE;
			}
		}
	}

	gBattleStruct->originalAttackerBackup = gBattlerAttacker;
	gBattleStruct->originalTargetBackup = gBattlerTarget;

	if (AtkCanceller_UnableToUseMove())
		return;
	else if (!BATTLER_ALIVE(gBattlerTarget)
	&& AttacksThisTurn(gBattlerAttacker, gCurrentMove) == 2 //Not charging move
	&&  !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_OPPONENTS_FIELD)) //Moves like Stealth Rock can still be used
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 2;
		return;
	}
	else if (!ProtectAffects(gCurrentMove, gBattlerAttacker, gBattlerTarget, FALSE)
	&& !MissesDueToSemiInvulnerability(gBattlerAttacker, gBattlerTarget, gCurrentMove)
	&& (AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK, gBattlerTarget, 0, 0, 0)
	|| (IS_DOUBLE_BATTLE && AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK_PARTNER, PARTNER(gBattlerTarget), 0, 0, 0))))
		return;

	else if (!gBattleStruct->ParentalBondOn
	&& gBattleStruct->OriginalAttackerTargetCount == 0
	&& ABILITY(gBattlerAttacker) == ABILITY_PARENTALBOND
	&& IsMoveAffectedByParentalBond(gCurrentMove, gBattlerAttacker)
	&& !(gAbsentBattlerFlags & gBitTable[gBattlerTarget]))
	{
		gBattleStruct->ParentalBondOn = 2;
		gMultiHitCounter = 2;
		PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0);
	}

	if (gBattleMons[gBattlerAttacker].pp[gCurrMovePos] == 0
	&& gCurrentMove != MOVE_STRUGGLE
	&& !(gHitMarker & (HITMARKER_x800000 | HITMARKER_NO_ATTACKSTRING))
	&& !(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)
	&& !gBattleStruct->DancerInProgress)
	{
		gBattlescriptCurrInstr = BattleScript_NoPPForMove;
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		return;
	}

	gHitMarker &= ~(HITMARKER_x800000);

	if (!(gHitMarker & HITMARKER_OBEYS) && !(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))
	{
		switch (IsMonDisobedient()) {
			case 0:
				break;
			case 2:
				gHitMarker |= HITMARKER_OBEYS;
				return;
			default:
				gMoveResultFlags |= MOVE_RESULT_MISSED;
				return;
		}
	}

	gHitMarker |= HITMARKER_OBEYS;

	if (MoveIgnoresSubstitutes(gCurrentMove, ABILITY(gBattlerAttacker)))
		gBattleStruct->bypassSubstitute = TRUE;

	if (gBattleStruct->MoveBounceInProgress == 2) //Bounce just ended
		gBattleStruct->MoveBounceInProgress = 0;

	if (!gBattleStruct->MoveBounceInProgress
	&& gBattleMoves[gCurrentMove].flags & FLAG_MAGIC_COAT_AFFECTED
	&& !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_FOES_AND_ALLY)) //Safety measure; no default moves allow this
	{
		if (gBattleMoves[gCurrentMove].target == MOVE_TARGET_OPPONENTS_FIELD)
		{
			if (gProtectStructs[SIDE(gBattlerAttacker) ^ BIT_SIDE].bounceMove)
			{
				PressurePPLose(gBattlerAttacker, SIDE(gBattlerAttacker) ^ BIT_SIDE, MOVE_MAGIC_COAT);
				gProtectStructs[SIDE(gBattlerAttacker) ^ BIT_SIDE].bounceMove = 0;
				gBattleStruct->MoveBounceInProgress = TRUE;
				gBattleStruct->moveWasBouncedThisTurn = TRUE;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MagicCoatBounce;
				return;
			}
			else if (gProtectStructs[PARTNER(SIDE(gBattlerAttacker)) ^ BIT_SIDE].bounceMove)
			{
				PressurePPLose(gBattlerAttacker, PARTNER(SIDE(gBattlerAttacker)) ^ BIT_SIDE, MOVE_MAGIC_COAT);
				gProtectStructs[PARTNER(SIDE(gBattlerAttacker)) ^ BIT_SIDE].bounceMove = 0;
				gBattleStruct->MoveBounceInProgress = TRUE;
				gBattleStruct->moveWasBouncedThisTurn = TRUE;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MagicCoatBounce;
				return;
			}
			else if (ABILITY(SIDE(gBattlerAttacker) ^ BIT_SIDE) == ABILITY_MAGICBOUNCE && !(gStatuses3[SIDE(gBattlerAttacker) ^ BIT_SIDE] & STATUS3_SEMI_INVULNERABLE))
			{
				gBattleStruct->MoveBounceInProgress = TRUE;
				gBattleStruct->moveWasBouncedThisTurn = TRUE;
				gBattleScripting.battler = SIDE(gBattlerAttacker) ^ BIT_SIDE;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MagicBounce;
				return;
			}
			else if (ABILITY(PARTNER(SIDE(gBattlerAttacker)) ^ BIT_SIDE) == ABILITY_MAGICBOUNCE && !(gStatuses3[PARTNER(SIDE(gBattlerAttacker)) ^ BIT_SIDE] & STATUS3_SEMI_INVULNERABLE))
			{
				gBattleStruct->MoveBounceInProgress = TRUE;
				gBattleStruct->moveWasBouncedThisTurn = TRUE;
				gBattleScripting.battler = PARTNER(SIDE(gBattlerAttacker) ^ BIT_SIDE);
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MagicBounce;
				return;
			}
		}
		else
		{
			if (gProtectStructs[gBattlerTarget].bounceMove)
			{
				PressurePPLose(gBattlerAttacker, gBattlerTarget, MOVE_MAGIC_COAT);
				gProtectStructs[gBattlerTarget].bounceMove = 0;
				gBattleStruct->MoveBounceInProgress = TRUE;
				gBattleStruct->moveWasBouncedThisTurn = TRUE;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MagicCoatBounce;
				return;
			}
			else if (ABILITY(gBattlerTarget) == ABILITY_MAGICBOUNCE && !(gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE))
			{
				gBattleStruct->MoveBounceInProgress = TRUE;
				gBattleStruct->moveWasBouncedThisTurn = TRUE;
				gLastUsedAbility = ABILITY_MAGICBOUNCE;
				RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
				gBattleScripting.battler = gBattlerTarget;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MagicBounce;
				return;
			}
		}
	}

	for (i = 0; i < gBattlersCount; i++)
	{
		if ((gProtectStructs[gBattlerByTurnOrder[i]].stealMove) && gBattleMoves[gCurrentMove].flags & FLAG_SNATCH_AFFECTED)
		{
			PressurePPLose(gBattlerAttacker, gBattlerByTurnOrder[i], MOVE_SNATCH);
			gProtectStructs[gBattlerByTurnOrder[i]].stealMove = 0;
			gBattleScripting.battler = gBattlerByTurnOrder[i];
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_SnatchedMove;
			return;
		}
	}

	if (gSpecialStatuses[gBattlerTarget].lightningRodRedirected)
	{
		gSpecialStatuses[gBattlerTarget].lightningRodRedirected = 0;
		gBattleScripting.battler = gBattlerTarget;
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_TookAttack;
	}
	else if (ProtectAffects(gCurrentMove, gBattlerAttacker, gBattlerTarget, FALSE)
	 && (gCurrentMove != MOVE_CURSE || IsOfType(gBattlerAttacker, TYPE_GHOST))
	 && ((!IsTwoTurnsMove(gCurrentMove) || (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))))
	{
		if (gBattleMoves[gCurrentMove].effect != EFFECT_RAMPAGE) //These are handled in CMD49
			CancelMultiTurnMoves(gBattlerAttacker);

		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gLastLandedMoves[gBattlerTarget] = 0;
		gLastHitByType[gBattlerTarget] = 0;
		gBattleCommunication[6] = 1;
		gBattlescriptCurrInstr++;
	}
	else
	{
		gBattlescriptCurrInstr++;
	}
}

u8 AtkCanceller_UnableToUseMove(void)
{
	int i, j;
	u8 effect = 0;

	do
	{
		switch (gBattleStruct->atkCancellerTracker) {
		case CANCELLER_FLAGS: // flags clear
			gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_DESTINY_BOND);
			gStatuses3[gBattlerAttacker] &= ~(STATUS3_GRUDGE);
			gBattleScripting.tripleKickPower = 0;
			gBattleStruct->ai.zMoveHelper = 0;
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_ASLEEP: // check being asleep
			if (gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP)
			{
				u8 toSub;
				if (ABILITY(gBattlerAttacker) == ABILITY_EARLY_BIRD)
					toSub = 2;
				else
					toSub = 1;

				if ((gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP) < toSub)
					gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_SLEEP);
				else
					gBattleMons[gBattlerAttacker].status1 -= toSub;

				if (gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP)
				{
					if (gCurrentMove != MOVE_SNORE && gCurrentMove != MOVE_SLEEP_TALK)
					{
						if (BATTLER_SEMI_INVULNERABLE(gBattlerAttacker)) //Caused by Yawn
							CancelMultiTurnMoves(gActiveBattler = gBattlerAttacker);

						gBattlescriptCurrInstr = BattleScript_MoveUsedIsAsleep;
						gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
						effect = 2;
					}
					else
						effect = 2;
				}
				else
				{
					gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_NIGHTMARE);
					BattleScriptPushCursor();
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					gBattlescriptCurrInstr = BattleScript_MoveUsedWokeUp;
					effect = 2;
				}
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_FROZEN:
			if (gBattleMons[gBattlerAttacker].status1 & STATUS1_FREEZE)
			{
				if (Random() % 5)
				{
					if (!CheckTableForMove(gCurrentMove, gMovesCanUnfreezeAttacker) || gMoveResultFlags & MOVE_RESULT_FAILED) // unfreezing via a move effect happens in case 13
					{
						gBattlescriptCurrInstr = BattleScript_MoveUsedIsFrozen;
						gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					}
					else
					{
						gBattleStruct->atkCancellerTracker++;
						break;
					}
				}
				else // unfreeze
				{
					gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_FREEZE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MoveUsedUnfroze;
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				}
				effect = 2;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_TRUANT: // truant
			if (ABILITY(gBattlerAttacker) == ABILITY_TRUANT && gDisableStructs[gBattlerAttacker].truantCounter)
			{
				CancelMultiTurnMoves(gBattlerAttacker);
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				gBattleCommunication[MULTISTRING_CHOOSER] = 4;
				gBattleScripting.battler = gBattlerAttacker;
				gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
				gMoveResultFlags |= MOVE_RESULT_MISSED;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_RECHARGE: // recharge
			if (gBattleMons[gBattlerAttacker].status2 & STATUS2_RECHARGE)
			{
				gBattleStruct->MetronomeCounter[gBattlerAttacker] = MathMin(100, gBattleStruct->MetronomeCounter[gBattlerAttacker] + 20);
				gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_RECHARGE);
				gDisableStructs[gBattlerAttacker].rechargeTimer = 0;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedMustRecharge;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_FLINCH: // flinch
			if (gBattleMons[gBattlerAttacker].status2 & STATUS2_FLINCHED && !IsDynamaxed(gBattlerAttacker))
			{
				gProtectStructs[gBattlerAttacker].flinchImmobility = 1;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedFlinched;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_RAID_BATTLE_NULLIFICATION:
			if (IsRaidBattle()
			&& GetBattlerPosition(gBattlerAttacker) == B_POSITION_OPPONENT_LEFT
			&& Random() % 100 < GetRaidBattleStatNullificationChance(gBattlerAttacker))
			{
				for (i = 0; i < gBattlersCount; ++i)
				{
					if (i == gBattlerAttacker || !BATTLER_ALIVE(i))
						continue;

					for (j = STAT_STAGE_ATK; j < BATTLE_STATS_NO; ++j)
						STAT_STAGE(i, j) = 6;
				}

				gBattleStruct->dynamaxData.nullifiedStats = TRUE;
				gSpecialStatuses[gBattlerTarget].lightningRodRedirected = 0; //The target will still be changed, but this is the best I can do

				gActiveBattler = gBattlerAttacker;
				gBattleMons[gActiveBattler].status1 = 0; //Clear non-volatile status condition
				BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
				MarkBattlerForControllerExec(gActiveBattler);

				BattleScriptPushCursor();
				gBattleScripting.battler = gBattlerAttacker;
				gBattlescriptCurrInstr = BattleScript_RaidBattleStatNullification;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_DEVOLVE:
			if (gBattleStruct->devolveForgotMove & gBitTable[gBattlerAttacker])
			{
				gBattleStruct->devolveForgotMove &= ~(gBitTable[gBattlerAttacker]);
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedDevolvedForgot;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_RAID_BATTLES_FAILED_MOVES:
		case CANCELLER_RAID_BATTLES_FAILED_MOVES_2:
			if (IsRaidBattle()
			&& (CheckTableForMove(gCurrentMove, gRaidBattleBannedMoves)
			 || (gCurrentMove == MOVE_TRANSFORM && gBattlerTarget == BANK_RAID_BOSS && gBattleStruct->dynamaxData.raidShieldsUp))
			&& !gBattleStruct->zMoveData.active) //Raid Battles stop status Z-Moves, so there will be a second check later on
			{
				gBattleScripting.battler = gBattlerAttacker;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedRaidBattlePrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}

			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_DYNAMAX_FAILED_MOVES:
			if ((IsDynamaxed(gBattlerTarget)
			 && CheckTableForMove(gCurrentMove, gDynamaxBannedMoves)
			 && !gBattleStruct->zMoveData.active) //Dynamax Pokemon stop status Z-Moves, so there will be a second check later on
			|| (IsRaidBattle() && GetBattlerPosition(gBattlerAttacker) == B_POSITION_OPPONENT_LEFT && CheckTableForMove(gCurrentMove, gRaidBattleBannedRaidMonMoves)))
			{
				gBattleScripting.battler = gBattlerAttacker;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedDynamaxPrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}

			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_BANNED_SIGNATURE_MOVE:
		case CANCELLER_BANNED_SIGNATURE_MOVE_2:
		{
			u16 species = SPECIES(gBattlerAttacker);

			switch (gCurrentMove) {
				case MOVE_DARKVOID:
					if (!gBattleStruct->zMoveData.active || gBattleStruct->zMoveData.effectApplied) //Allows for Z-Dark Void to still have Z-Effect
					{
						#ifdef SPECIES_DARKRAI
						if (species != SPECIES_DARKRAI && !gBattleStruct->MoveBounceInProgress)
							effect = 1;
						#endif
					}
					break;
				case MOVE_HYPERSPACEFURY:
					#ifdef SPECIES_HOOPA_UNBOUND
					if (species != SPECIES_HOOPA_UNBOUND)
					{
						#ifdef SPECIES_HOOPA
						if (species == SPECIES_HOOPA)
							effect = 2;
						else
						#endif
							effect = 1;
					}
					#endif
					break;
				case MOVE_AURAWHEEL:
					#ifdef SPECIES_MORPEKO
					if (species != SPECIES_MORPEKO && species != SPECIES_MORPEKO_HANGRY)
						effect = 1;
					#endif
					break;
			}

			if (effect)
			{
				gBattleScripting.battler = gBattlerAttacker;
				CancelMultiTurnMoves(gBattlerAttacker);

				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;

				if (effect == 2)
					gBattlescriptCurrInstr = BattleScript_HoopaCantUseHyperspaceFury;
				else
					gBattlescriptCurrInstr = BattleScript_CantUseSignatureMove;
			}
			gBattleStruct->atkCancellerTracker++;
		}
			break;

		case CANCELLER_DISABLED: // disabled move
			if (gDisableStructs[gBattlerAttacker].disabledMove == gCurrentMove
			&& gDisableStructs[gBattlerAttacker].disabledMove != 0
			&& !gBattleStruct->zMoveData.active
			&& !IsAnyMaxMove(gCurrentMove))
			{
				gProtectStructs[gBattlerAttacker].usedDisabledMove = 1;
				gBattleScripting.battler = gBattlerAttacker;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedIsDisabled;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_HEAL_BLOCKED:
		case CANCELLER_HEAL_BLOCKED_2:
			if (IsHealBlocked(gBattlerAttacker)
			&& CheckHealingMove(gCurrentMove)
			&& !gBattleStruct->zMoveData.active
			&& !IsAnyMaxMove(gCurrentMove))
			{
				gBattleScripting.battler = gBattlerAttacker;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedHealBlockPrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_THROAT_CHOP:
		case CANCELLER_THROAT_CHOP_2:
			if (CantUseSoundMoves(gBattlerAttacker)
			&& CheckSoundMove(gCurrentMove)
			&& !gBattleStruct->zMoveData.active
			&& !IsAnyMaxMove(gCurrentMove))
			{
				gBattleScripting.battler = gBattlerAttacker;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedThroatChopPrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_GRAVITY:
		case CANCELLER_GRAVITY_2:
			if (IsGravityActive()
			&& CheckTableForMove(gCurrentMove, gGravityBannedMoves)
			&& !gBattleStruct->zMoveData.active //Gravity stops Z-Moves, so there will be a second check later on
			&& !IsAnyMaxMove(gCurrentMove))
			{
				gBattleScripting.battler = gBattlerAttacker;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedGravityPrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}

			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_TAUNTED: // taunt
			if (IsTaunted(gBattlerAttacker)
			&& SPLIT(gCurrentMove) == SPLIT_STATUS
			&& !gBattleStruct->zMoveData.active
			&& !IsAnyMaxMove(gCurrentMove))
			{
				gProtectStructs[gBattlerAttacker].usedTauntedMove = 1;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedIsTaunted;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_IMPRISONED: // imprisoned
			if (GetImprisonedMovesCount(gBattlerAttacker, gCurrentMove)
			&& !gBattleStruct->zMoveData.active
			&& !IsAnyMaxMove(gCurrentMove))
			{
				gProtectStructs[gBattlerAttacker].usedImprisonedMove = 1;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedIsImprisoned;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_CONFUSED: // confusion
			if (IsConfused(gBattlerAttacker))
			{
				if (gBattleMons[gBattlerAttacker].status2 & STATUS2_CONFUSION)
					gBattleMons[gBattlerAttacker].status2--;

				if (IsConfused(gBattlerAttacker))
				{
					#ifdef OLD_CONFUSION_CHANCE
					if (Random() & 1) //50 %
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
						BattleScriptPushCursor();
					}
					#else
					if (Random() % 100 > 33) //33 %
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
						BattleScriptPushCursor();
					}
					#endif
					else // confusion dmg
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;
						gBattlerTarget = gBattlerAttacker;
						gBattleMoveDamage = ConfusionDamageCalc();
						gProtectStructs[gBattlerAttacker].confusionSelfDmg = 1;
						gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
						gBattleStruct->breakDisguiseSpecialDmg = TRUE;
					}
					gBattlescriptCurrInstr = BattleScript_MoveUsedIsConfused;
				}
				else // snapped out of confusion
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MoveUsedIsConfusedNoMore;
				}
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_PARALYSED: // paralysis
			if ((gBattleMons[gBattlerAttacker].status1 & STATUS1_PARALYSIS) && Random() % 4 == 0)
			{
				gProtectStructs[gBattlerAttacker].prlzImmobility = 1;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedIsParalyzed;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_GHOST: //Lavender Town Ghost battle
		#ifndef NO_GHOST_BATTLES
			if ((gBattleTypeFlags & (BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST)
			{
				if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
					gBattlescriptCurrInstr = BattleScript_TooScaredToMove;
				else
					gBattlescriptCurrInstr = BattleScript_GhostGetOut;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
		#endif
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_IN_LOVE: // infatuation
			if (gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION)
			{
				gBattleScripting.battler = CountTrailingZeroBits((gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION) >> 0x10);
				if (Random() & 1)
				{
					BattleScriptPushCursor();
				}
				else
				{
					BattleScriptPush(BattleScript_MoveUsedIsInLoveCantAttack);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					gProtectStructs[gBattlerAttacker].loveImmobility = 1;
					CancelMultiTurnMoves(gBattlerAttacker);
				}
				gBattlescriptCurrInstr = BattleScript_MoveUsedIsInLove;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_STANCE_CHANGE:
		case CANCELLER_STANCE_CHANGE_2:
			#if (defined SPECIES_AEGISLASH && defined SPECIES_AEGISLASH_BLADE)
			if (ABILITY(gBattlerAttacker) == ABILITY_STANCECHANGE && !(gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED))
			{
				switch (gBattleMons[gBattlerAttacker].species)
				{
					case SPECIES_AEGISLASH:
						if (SPLIT(gCurrentMove) != SPLIT_STATUS)
						{
							DoFormChange(gBattlerAttacker, SPECIES_AEGISLASH_BLADE, FALSE, TRUE, FALSE);
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_StanceChangeToBlade;
							gBattleScripting.battler = gBattlerAttacker;
							effect = 1;
						}
						break;

					case SPECIES_AEGISLASH_BLADE:
						if (gCurrentMove == MOVE_KINGSSHIELD)
						{
							DoFormChange(gBattlerAttacker, SPECIES_AEGISLASH, FALSE, TRUE, FALSE);
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_StanceChangeToShield;
							gBattleScripting.battler = gBattlerAttacker;
							effect = 1;
						}
						break;
				}
			}
			#endif
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_BIDE: // bide
			if (gBattleMons[gBattlerAttacker].status2 & STATUS2_BIDE)
			{
				gBattleMons[gBattlerAttacker].status2 -= 0x100;
				if (gBattleMons[gBattlerAttacker].status2 & STATUS2_BIDE)
				{
					gBattlescriptCurrInstr = BattleScript_BideStoringEnergy;
				}
				else
				{
					gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);
					if (gTakenDmg[gBattlerAttacker])
					{
						gCurrentMove = MOVE_BIDE;
						gBattleScripting.bideDmg = gTakenDmg[gBattlerAttacker] * 2;
						gBattlerTarget = gTakenDmgByBattler[gBattlerAttacker];
						if (gAbsentBattlerFlags & gBitTable[gBattlerTarget])
							gBattlerTarget = GetMoveTarget(MOVE_BIDE, 1);
						gBattlescriptCurrInstr = BattleScript_BideAttack;
					}
					else
					{
						gBattlescriptCurrInstr = BattleScript_BideNoEnergyToAttack;
					}
				}
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_THAW: // move thawing
			if (gBattleMons[gBattlerAttacker].status1 & STATUS1_FREEZE)
			{
				if (CheckTableForMove(gCurrentMove, gMovesCanUnfreezeAttacker)
				&& !(gMoveResultFlags & MOVE_RESULT_FAILED)) //When Burn Up fails, it can't unfreeze
				{
					gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_FREEZE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MoveUsedUnfroze;
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
				}
				effect = 2;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_Z_MOVES:
			if (gBattleStruct->zMoveData.active)
			{
				RecordItemEffectBattle(gBattlerAttacker, HOLD_EFFECT_Z_CRYSTAL);
				gBattleStruct->zMoveData.used[gBattlerAttacker] = TRUE;
				gBattleStruct->zMoveData.toBeUsed[gBattlerAttacker] = 0;
				gBattleStruct->zMoveData.partyIndex[gBattlerAttacker] |= gBitTable[gBattlerPartyIndexes[gBattlerAttacker]]; //Stops Rayquaza from Mega Evolving

				if (IsDoubleBattle())
				{
					if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER && !(gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_MULTI))) //In team Battles, both players can use Z-moves
					{
						gBattleStruct->zMoveData.used[PARTNER(gBattlerAttacker)] = TRUE;
						gBattleStruct->zMoveData.toBeUsed[PARTNER(gBattlerAttacker)] = 0;
					}
					else if (SIDE(gBattlerAttacker) == B_SIDE_OPPONENT && !(gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI)))
					{
						gBattleStruct->zMoveData.used[PARTNER(gBattlerAttacker)] = TRUE;
						gBattleStruct->zMoveData.toBeUsed[PARTNER(gBattlerAttacker)] = 0;
					}
				}

				gBattleScripting.battler = gBattlerAttacker;
				if (SPLIT(gCurrentMove) == SPLIT_STATUS)
				{
					if (!gBattleStruct->zMoveData.effectApplied)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_ZMoveActivateStatus;
						gBattleStruct->zMoveData.effect = gBattleMoves[gCurrentMove].z_move_effect;
						gBattleStruct->zMoveData.effectApplied = TRUE;
					}
					else
					{
						gBattleStruct->atkCancellerTracker++;
						break;
					}
				}
				else
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_ZMoveActivateDamaging;
				}
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_GRAVITY_Z_MOVES:
			if (IsGravityActive() && CheckTableForMove(gCurrentMove, gGravityBannedMoves) && gBattleStruct->zMoveData.active) //Gravity stops Z-Moves after they apply their effect
			{
				gBattleScripting.battler = gBattlerAttacker;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedGravityPrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_SKY_BATTLE:
		#ifdef FLAG_SKY_BATTLE
			if (FlagGet(FLAG_SKY_BATTLE) && CheckTableForMove(gCurrentMove, gSkyBattleBannedMoves))
			{
				gBattleScripting.battler = gBattlerAttacker;
				CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedSkyBattlePrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
		#endif
			gBattleStruct->atkCancellerTracker++;
			break;

		//Natural Gift fails before it can take Powder damage
		case CANCELLER_NATURAL_GIFT:
			if (gCurrentMove == MOVE_NATURALGIFT &&
			  (ABILITY(gBattlerAttacker) == ABILITY_KLUTZ
			 || !IsBerry(ITEM(gBattlerAttacker))
			 || IsMagicRoomActive()
			 || gBattleStruct->EmbargoTimers[gBattlerAttacker]))
			{
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 2;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_DANCER:
			if (gBattleStruct->DancerInProgress)
			{
				if ((gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)
				&&  gCurrentMove != gLockedMoves[gBattlerAttacker])
				{
					gBattlescriptCurrInstr = BattleScript_ButItFailed - 2;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					effect = 1;
				}
				else if (gDisableStructs[gBattlerAttacker].encoredMove
				&& gCurrentMove != gDisableStructs[gBattlerAttacker].encoredMove)
				{
					gBattlescriptCurrInstr = BattleScript_ButItFailed - 2;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					effect = 1;
				}
				else if (gBattleStruct->choicedMove[gBattlerAttacker]
				&& gCurrentMove != gBattleStruct->choicedMove[gBattlerAttacker])
				{
					gBattlescriptCurrInstr = BattleScript_ButItFailed - 2;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					effect = 1;
				}
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_POWDER:
			if ((gBattleStruct->PowderByte & gBitTable[gBattlerAttacker]) && (gBattleStruct->dynamicMoveType == TYPE_FIRE))
			{
				gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 4;
				gBattlescriptCurrInstr = BattleScript_MoveUsedPowderPrevents;
				gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_PRIMAL_WEATHER:
			if (WEATHER_HAS_EFFECT && SPLIT(gCurrentMove) != SPLIT_STATUS) //Damaging moves only
			{
				if ((gBattleStruct->dynamicMoveType == TYPE_FIRE && gBattleWeather & WEATHER_RAIN_PRIMAL)
				||  (gBattleStruct->dynamicMoveType == TYPE_WATER && gBattleWeather & WEATHER_SUN_PRIMAL))
				{
					CancelMultiTurnMoves(gBattlerAttacker);
					gBattlescriptCurrInstr = BattleScript_MoveUsedFailedPrimalWeather;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					effect = 1;
				}
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_PSYCHIC_TERRAIN:
			if (gTerrainType == PSYCHIC_TERRAIN
			&& CheckGrounding(gBattlerTarget)
			&& gBattlerAttacker != gBattlerTarget
			&& PriorityCalc(gBattlerAttacker, B_ACTION_USE_MOVE, gCurrentMove) > 0
			&& !ProtectAffects(gCurrentMove, gBattlerAttacker, gBattlerTarget, FALSE)
			&& !MissesDueToSemiInvulnerability(gBattlerAttacker, gBattlerTarget, gCurrentMove))
			{
				if (IS_SINGLE_BATTLE || !(gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY))) //Don't cancel moves that can hit two targets b/c one target might not be protected
					CancelMultiTurnMoves(gBattlerAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedPsychicTerrainPrevents;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_PRANKSTER:
			#ifndef OLD_PRANKSTER
			if (ABILITY(gBattlerAttacker) == ABILITY_PRANKSTER
			&& SPLIT(gCurrentMove) == SPLIT_STATUS
			&& AttacksThisTurn(gBattlerAttacker, gCurrentMove) == 2
			&& !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_OPPONENTS_FIELD)
			&& gBattlerAttacker != gBattlerTarget
			&& IsOfType(gBattlerTarget, TYPE_DARK)
			&& gCurrentMove != MOVE_GRAVITY
			&& !ProtectAffects(gCurrentMove, gBattlerAttacker, gBattlerTarget, FALSE)
			&& !MissesDueToSemiInvulnerability(gBattlerAttacker, gBattlerTarget, gCurrentMove))
			{
				if (IS_SINGLE_BATTLE || !(gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY))) //Don't cancel moves that can hit two targets b/c one target might not be protected
					CancelMultiTurnMoves(gBattlerAttacker);
				gBattleScripting.battler = gBattlerTarget;
				gBattlescriptCurrInstr = BattleScript_DarkTypePreventsPrankster;
				effect = 1;
			}
			#endif
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_EXPLODING_DAMP:
			if (gBattleMoves[gCurrentMove].effect == EFFECT_EXPLOSION)
			{
				u8 bank;

				for (bank = 0; bank < gBattlersCount; ++bank)
				{
					if (ABILITY(bank) == ABILITY_DAMP)
						break;
				}

				if (bank != gBattlersCount)
				{
					gBattleScripting.battler = bank;
					gBattlescriptCurrInstr = BattleScript_DampStopsExplosion;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					effect = 1;
				}
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_MULTIHIT_MOVES:
			if (CheckTableForMove(gCurrentMove, gTwoToFiveStrikesMoves))
			{
				ability_t ability = ABILITY(gBattlerAttacker);

				if (gCurrentMove == MOVE_SURGINGSTRIKES)
				{
					gMultiHitCounter = 3;
				}
				else if (ability == ABILITY_SKILLLINK)
				{
					gMultiHitCounter = 5;
				}
				#ifdef SPECIES_ASHGRENINJA
				else if (ability == ABILITY_BATTLEBOND
				&& gCurrentMove == MOVE_WATERSHURIKEN
				&& SPECIES(gBattlerAttacker) == SPECIES_ASHGRENINJA)
				{
					gMultiHitCounter = 3;
				}
				else
				#endif
				{
					gMultiHitCounter = Random() & 3;
					if (gMultiHitCounter > 1)
						gMultiHitCounter = (Random() & 3) + 2;
					else
						gMultiHitCounter += 2;
				}

				PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0)
			}
			else if (CheckTableForMove(gCurrentMove, gTwoStrikesMoves))
			{
				gMultiHitCounter = 2;
				PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0)

				if (gCurrentMove == MOVE_DRAGONDARTS
				&& CanTargetPartner(gBattlerTarget)
				&& TargetFullyImmuneToCurrMove(gBattlerTarget))
				{
					//Smart target to partner
					gBattlerTarget = PARTNER(gBattlerTarget);
				}
			}
			else if (gBattleMoves[gCurrentMove].effect == EFFECT_TRIPLE_KICK)
			{
				gMultiHitCounter = 3;
				PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0)
			}
			else if (gBattleMoves[gCurrentMove].effect == EFFECT_BEAT_UP)
			{
				struct Pokemon* party;
                int i;

				if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
					party = gPlayerParty;
				else
					party = gEnemyParty;

				for (i = 0; i < PARTY_SIZE; ++i)
				{
					if (GetMonData(&party[i], MON_DATA_HP, 0)
					&& GetMonData(&party[i], MON_DATA_SPECIES, 0) != SPECIES_NONE
					&& !GetMonData(&party[i], MON_DATA_IS_EGG, 0)
					&& !GetMonData(&party[i], MON_DATA_STATUS, 0))
						++gMultiHitCounter;
				}

				gBattleCommunication[0] = 0; //For later
				PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0)
			}

			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_MULTI_TARGET_MOVES:
			if (IS_DOUBLE_BATTLE)
			{
				const u8* backupScript = gBattlescriptCurrInstr; //Script can get overwritten by ability blocking

				if (gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY)
				&& !CheckTableForMove(gCurrentMove, gSpecialWholeFieldMoves))
				{
					u8 priority = PriorityCalc(gBattlerAttacker, B_ACTION_USE_MOVE, gCurrentMove);

					for (i = 0; i < gBattlersCount; ++i)
					{
						if (i != gBattlerAttacker && BATTLER_ALIVE(i)
						&& ((gBattleMoves[gCurrentMove].target & MOVE_TARGET_FOES_AND_ALLY) || i != PARTNER(gBattlerAttacker)) //Skip partner when not all-hitting move
						&& !ProtectAffects(gCurrentMove, gBattlerAttacker, i, FALSE)
						&& !MissesDueToSemiInvulnerability(gBattlerAttacker, i, gCurrentMove))
						{
							if (AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK, i, 0, 0, 0)
							||  AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK_PARTNER, PARTNER(i), 0, 0, 0)
							|| (gTerrainType == PSYCHIC_TERRAIN && CheckGrounding(i) && priority > 0))
							{
								gBattleStruct->ResultFlags[i] = 0;
								gBattleStruct->noResultString[i] = TRUE;
							}
							else if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, i, 0, 0, gCurrentMove))
							{
								gBattleStruct->ResultFlags[i] = 0;
								gBattleStruct->noResultString[i] = 2; //Indicator to factor in Accuracy checks
							}
							else
								gBattleStruct->ResultFlags[i] = NewTypeCalc(gCurrentMove, gBattlerAttacker, i, NULL, FALSE);
						}
						else
						{
							gBattleStruct->ResultFlags[i] = MOVE_RESULT_NO_EFFECT; //You can't strike these targets
							gBattleStruct->noResultString[i] = TRUE;
						}
					}
				}

				gBattlescriptCurrInstr = backupScript; //Restore original script
				gActiveBattler = gBattlerAttacker;
				gBattleStruct->foeSpreadTargets = CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, gBattlerAttacker, FOE(gBattlerAttacker));
				gBattleStruct->allSpreadTargets = CountAliveMonsInBattle(BATTLE_ALIVE_EXCEPT_ACTIVE, gBattlerAttacker, FOE(gBattlerAttacker));
			}
			gBattleStruct->atkCancellerTracker++;
			break;

		case CANCELLER_END:
			break;
		}

	} while (gBattleStruct->atkCancellerTracker != CANCELLER_END && effect == 0);

	if (effect == 2)
	{
		gActiveBattler = gBattlerAttacker;
		BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
		MarkBattlerForControllerExec(gActiveBattler);
	}
	return effect;
}

static u8 IsMonDisobedient(void)
{
	s32 rnd;
	s32 calc;
	u8 obedienceLevel = 0;

	if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_FRONTIER))
		return 0;
	else if (IsFrontierRaidBattle())
		return 0;
	else if (gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER) && GetBattlerPosition(gBattlerAttacker) == B_POSITION_PLAYER_RIGHT)
		return 0;
	else if (gBattleTypeFlags & (BATTLE_TYPE_MOCK_BATTLE) && GetBattlerPosition(gBattlerAttacker) == B_POSITION_PLAYER_LEFT)
		return 0;
	else if (SIDE(gBattlerAttacker) == B_SIDE_OPPONENT)
		return 0;
	else if (gBattleStruct->InstructInProgress || gBattleStruct->DancerInProgress || gBattleStruct->zMoveData.active || IsAnyMaxMove(gCurrentMove))
		return 0;

	#ifndef OBEDIENCE_CHECK_FOR_PLAYER_ORIGINAL_POKEMON
	if (!IsOtherTrainer(gBattleMons[gBattlerAttacker].otId, gBattleMons[gBattlerAttacker].otName))
		return 0;
	#endif

	#ifdef OBEDIENCE_BY_BADGE_AMOUNT
		u8 badgeCount = 0;

		if (FlagGet(FLAG_BADGE08_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE07_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE06_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE05_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE04_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE03_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE02_GET))
			++badgeCount;
		if (FlagGet(FLAG_BADGE01_GET))
			++badgeCount;

		switch(badgeCount) {
			case 0:
				obedienceLevel = BASE_OBEDIENCE_LEVEL;
				break;
			case 1:
				obedienceLevel = BADGE_1_OBEDIENCE_LEVEL;
				break;
			case 2:
				obedienceLevel = BADGE_2_OBEDIENCE_LEVEL;
				break;
			case 3:
				obedienceLevel = BADGE_3_OBEDIENCE_LEVEL;
				break;
			case 4:
				obedienceLevel = BADGE_4_OBEDIENCE_LEVEL;
				break;
			case 5:
				obedienceLevel = BADGE_5_OBEDIENCE_LEVEL;
				break;
			case 6:
				obedienceLevel = BADGE_6_OBEDIENCE_LEVEL;
				break;
			case 7:
				obedienceLevel = BADGE_7_OBEDIENCE_LEVEL;
				break;
			default:
				return 0;
		}

	#else
		if (FlagGet(FLAG_BADGE08_GET))
			return 0;
		else if (FlagGet(FLAG_BADGE07_GET))
			obedienceLevel = BADGE_7_OBEDIENCE_LEVEL;
		else if (FlagGet(FLAG_BADGE06_GET))
			obedienceLevel = BADGE_6_OBEDIENCE_LEVEL;
		else if (FlagGet(FLAG_BADGE05_GET))
			obedienceLevel = BADGE_5_OBEDIENCE_LEVEL;
		else if (FlagGet(FLAG_BADGE04_GET))
			obedienceLevel = BADGE_4_OBEDIENCE_LEVEL;
		else if (FlagGet(FLAG_BADGE03_GET))
			obedienceLevel = BADGE_3_OBEDIENCE_LEVEL;
		else if (FlagGet(FLAG_BADGE02_GET))
			obedienceLevel = BADGE_2_OBEDIENCE_LEVEL;
		else if (FlagGet(FLAG_BADGE01_GET))
			obedienceLevel = BADGE_1_OBEDIENCE_LEVEL;
		else
			obedienceLevel = BASE_OBEDIENCE_LEVEL;
	#endif

	if (gBattleMons[gBattlerAttacker].level <= obedienceLevel)
		return 0;
	rnd = (Random() & 255);
	calc = (gBattleMons[gBattlerAttacker].level + obedienceLevel) * rnd >> 8;
	if (calc < obedienceLevel)
		return 0;

	// is not obedient
	if (gCurrentMove == MOVE_RAGE)
		gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_RAGE);
	if (gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP && (gCurrentMove == MOVE_SNORE || gCurrentMove == MOVE_SLEEP_TALK))
	{
		gBattlescriptCurrInstr = BattleScript_IgnoresWhileAsleep;
		return 1;
	}

	rnd = (Random() & 255);
	calc = (gBattleMons[gBattlerAttacker].level + obedienceLevel) * rnd >> 8;
	if (calc < obedienceLevel && gCurrentMove != MOVE_FOCUS_PUNCH && gCurrentMove != MOVE_BEAKBLAST && gCurrentMove != MOVE_SHELLTRAP)
	{
		calc = CheckMoveLimitations(gBattlerAttacker, gBitTable[gCurrMovePos], 0xFF);
		if (calc == 0xF) // all moves cannot be used
		{
			gBattleCommunication[MULTISTRING_CHOOSER] = Random() & 3;
			gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
			return 1;
		}
		else // use a random move
		{
			do
			{
				gCurrMovePos = gChosenMovePos = Random() & 3;
			} while (gBitTable[gCurrMovePos] & calc);

			gCalledMove = gBattleMons[gBattlerAttacker].moves[gCurrMovePos];
			gBattlescriptCurrInstr = BattleScript_IgnoresAndUsesRandomMove;
			gBattlerTarget = GetMoveTarget(gCalledMove, 0);
			gHitMarker |= HITMARKER_x200000;
			return 2;
		}
	}
	else
	{
		obedienceLevel = gBattleMons[gBattlerAttacker].level - obedienceLevel;

		calc = (Random() & 255);
		if (calc < obedienceLevel && CanBePutToSleep(gBattlerAttacker, FALSE))
		{
			// try putting asleep
			int i;
			for (i = 0; i < gBattlersCount; i++)
			{
				if (gBattleMons[i].status2 & STATUS2_UPROAR)
					break;
			}
			if (i == gBattlersCount)
			{
				gBattlescriptCurrInstr = BattleScript_IgnoresAndFallsAsleep;
				return 1;
			}
		}
		calc -= obedienceLevel;
		if (calc < obedienceLevel)
		{
			gBattleMoveDamage = ConfusionDamageCalc();
			gBattlerTarget = gBattlerAttacker;
			gBattlescriptCurrInstr = BattleScript_IgnoresAndHitsItself;
			gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
			return 2;
		}
		else
		{
			gBattleCommunication[MULTISTRING_CHOOSER] = Random() & 3;
			gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
			return 1;
		}
	}
}

bool8 CanTargetPartner(u8 bankDef)
{
	return IS_DOUBLE_BATTLE
		&& BATTLER_ALIVE(PARTNER(bankDef))
		&& bankDef != PARTNER(gBattlerAttacker);
}

bool8 TargetFullyImmuneToCurrMove(u8 bankDef)
{
	return NewTypeCalc(gCurrentMove, gBattlerAttacker, bankDef, 0, FALSE) & MOVE_RESULT_NO_EFFECT
		 || ProtectAffects(gCurrentMove, gBattlerAttacker, bankDef, FALSE)
		 || BATTLER_SEMI_INVULNERABLE(bankDef)
		 || DoesTargetHaveAbilityImmunity();
}

//For Dragon Darts
static bool8 DoesTargetHaveAbilityImmunity(void)
{
	BattleScriptPushCursor(); //Backup the current script

	if (AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK, gBattlerTarget, 0, 0, 0)
	||  AbilityBattleEffects(ABILITYEFFECT_ABSORBING, gBattlerTarget, 0, 0, 0))
	{
		BattleScriptPop(); //Restory the original script
		return TRUE;
	}

	return FALSE;
}
