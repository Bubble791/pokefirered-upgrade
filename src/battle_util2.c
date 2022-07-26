#include "global.h"
#include "bg.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "item.h"
#include "pokemon.h"
#include "malloc.h"
#include "trainer_tower.h"
#include "frontier.h"
#include "build_pokemon.h"
#include "battle_message.h"
#include "util.h"
#include "battle_controllers.h"
#include "mail_data.h"
#include "string_util.h"
#include "dynamax.h"
#include "event_data.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/species.h"
#include "constants/battle_move_effects.h"

static void TryRemoveUnburdenBoost(u8 bank);

void AllocateBattleResources(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
        InitTrainerTowerBattleStruct();
    if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
    {
        s32 i;

        for (i = 0; i < 4; ++i)
            gPokedudeBattlerStates[i] = AllocZeroed(sizeof(struct PokedudeBattlerState));
    }
    gBattleStruct = AllocZeroed(sizeof(*gBattleStruct));
    gBattleResources = AllocZeroed(sizeof(*gBattleResources));
    gBattleResources->secretBase = AllocZeroed(sizeof(*gBattleResources->secretBase));
    gBattleResources->flags = AllocZeroed(sizeof(*gBattleResources->flags));
    gBattleResources->battleScriptsStack = AllocZeroed(sizeof(*gBattleResources->battleScriptsStack));
    gBattleResources->battleCallbackStack = AllocZeroed(sizeof(*gBattleResources->battleCallbackStack));
    gBattleResources->beforeLvlUp = AllocZeroed(sizeof(*gBattleResources->beforeLvlUp));
    gBattleResources->ai = AllocZeroed(sizeof(*gBattleResources->ai));
    gBattleResources->battleHistory = AllocZeroed(sizeof(*gBattleResources->battleHistory));
    gBattleResources->AI_ScriptsStack = AllocZeroed(sizeof(*gBattleResources->AI_ScriptsStack));
    gLinkBattleSendBuffer = AllocZeroed(BATTLE_BUFFER_LINK_SIZE);
    gLinkBattleRecvBuffer = AllocZeroed(BATTLE_BUFFER_LINK_SIZE);
    gBattleAnimMons_BgTilesBuffer = AllocZeroed(0x2000);
    gBattleAnimMons_BgTilemapBuffer = AllocZeroed(0x1000);
    SetBgTilemapBuffer(1, gBattleAnimMons_BgTilemapBuffer);
    SetBgTilemapBuffer(2, gBattleAnimMons_BgTilemapBuffer);
}

void FreeBattleResources(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
        FreeTrainerTowerBattleStruct();
    if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
    {
        s32 i;

        for (i = 0; i < 4; ++i)
        {
            FREE_AND_SET_NULL(gPokedudeBattlerStates[i]);
        }
    }
    if (gBattleResources != NULL)
    {
        FREE_AND_SET_NULL(gBattleStruct);
        FREE_AND_SET_NULL(gBattleResources->secretBase);
        FREE_AND_SET_NULL(gBattleResources->flags);
        FREE_AND_SET_NULL(gBattleResources->battleScriptsStack);
        FREE_AND_SET_NULL(gBattleResources->battleCallbackStack);
        FREE_AND_SET_NULL(gBattleResources->beforeLvlUp);
        FREE_AND_SET_NULL(gBattleResources->ai);
        FREE_AND_SET_NULL(gBattleResources->battleHistory);
        FREE_AND_SET_NULL(gBattleResources->AI_ScriptsStack);
        FREE_AND_SET_NULL(gBattleResources);
        FREE_AND_SET_NULL(gLinkBattleSendBuffer);
        FREE_AND_SET_NULL(gLinkBattleRecvBuffer);
        FREE_AND_SET_NULL(gBattleAnimMons_BgTilesBuffer);
        FREE_AND_SET_NULL(gBattleAnimMons_BgTilemapBuffer);
    }
}

void AdjustFriendshipOnBattleFaint(u8 battlerId)
{
    u8 opposingBattlerId, opposingBattlerId2;

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        opposingBattlerId = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        opposingBattlerId2 = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
        if (gBattleMons[opposingBattlerId2].level > gBattleMons[opposingBattlerId].level)
            opposingBattlerId = opposingBattlerId2;
    }
    else
    {
        opposingBattlerId = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
    }
    if (gBattleMons[opposingBattlerId].level > gBattleMons[battlerId].level)
    {
        if (gBattleMons[opposingBattlerId].level - gBattleMons[battlerId].level > 29)
            AdjustFriendship(&gPlayerParty[gBattlerPartyIndexes[battlerId]], FRIENDSHIP_EVENT_FAINT_LARGE);
        else
            AdjustFriendship(&gPlayerParty[gBattlerPartyIndexes[battlerId]], FRIENDSHIP_EVENT_FAINT_SMALL);
    }
    else
    {
        AdjustFriendship(&gPlayerParty[gBattlerPartyIndexes[battlerId]], FRIENDSHIP_EVENT_FAINT_SMALL);
    }
}

ability_t GetBankAbility(u8 bank)
{
	if (IsAbilitySuppressed(bank))
		return ABILITY_NONE;

	return gBattleMons[bank].ability;
}

ability_t GetRecordedAbility(u8 bank)
{
    u16 species = species;
    u8 ability1, ability2, hiddenAbility;

	if (IsAbilitySuppressed(bank))
		return ABILITY_NONE;

	if (BATTLE_HISTORY->abilities[bank] != ABILITY_NONE)
		return BATTLE_HISTORY->abilities[bank];

	ability1 = TryRandomizeAbility(gBaseStats[species].ability1, species);
	ability2 = TryRandomizeAbility(gBaseStats[species].ability2, species);
	hiddenAbility = TryRandomizeAbility(gBaseStats[species].hiddenAbility, species);

	if (ability1 == ability2 && hiddenAbility == ABILITY_NONE)
		return ability1;

	if (ability1 == ability2 && ability1 == hiddenAbility)
		return ability1;

	return ABILITY_NONE; //We don't know which ability the target has
}

ability_t CopyAbility(u8 bank)
{
	if (IsAbilitySuppressed(bank))
		return gBattleStruct->SuppressedAbilities[bank];
	else if (gBattleStruct->neutralizingGasBlockedAbilities[bank])
		return gBattleStruct->neutralizingGasBlockedAbilities[bank];
	else if (gBattleStruct->DisabledMoldBreakerAbilities[bank])
		return gBattleStruct->DisabledMoldBreakerAbilities[bank];
	else
		return gBattleMons[bank].ability;
}

ability_t* GetAbilityLocation(u8 bank)
{
	if (IsAbilitySuppressed(bank))
		return &gBattleStruct->SuppressedAbilities[bank];
	else if (gBattleStruct->neutralizingGasBlockedAbilities[bank])
		return &gBattleStruct->neutralizingGasBlockedAbilities[bank];
	else if (gBattleStruct->DisabledMoldBreakerAbilities[bank])
		return &gBattleStruct->DisabledMoldBreakerAbilities[bank];
	else
		return &gBattleMons[bank].ability;
}

ability_t* GetAbilityLocationIgnoreNeutralizingGas(u8 bank)
{
	if (IsAbilitySuppressed(bank))
		return &gBattleStruct->SuppressedAbilities[bank];
	else if (gBattleStruct->DisabledMoldBreakerAbilities[bank])
		return &gBattleStruct->DisabledMoldBreakerAbilities[bank];
	else
		return &gBattleMons[bank].ability;
}

void ClearBattlerAbilityHistory(u8 bank)
{
	BATTLE_HISTORY->abilities[bank] = ABILITY_NONE;
}

u8 GetBankItemEffect(u8 bank)
{
	if (ABILITY(bank) != ABILITY_KLUTZ && !gBattleStruct->EmbargoTimers[bank] && !IsMagicRoomActive())
		return ItemId_GetHoldEffect(ITEM(bank));

	return 0;
}

u8 GetMonItemEffect(struct Pokemon* mon)
{
	if (GetMonAbility(mon) != ABILITY_KLUTZ && !IsMagicRoomActive())
		return ItemId_GetHoldEffect(GetMonData(mon, MON_DATA_HELD_ITEM, NULL));

	return 0;
}

u8 GetRecordedItemEffect(u8 bank)
{
	if (GetRecordedAbility(bank) != ABILITY_KLUTZ
	&& !gBattleStruct->EmbargoTimers[bank]
	&& !IsMagicRoomActive()
	&& ITEM(bank) != ITEM_NONE) //Can't have an effect if you have no item
		return gBattleStruct->ai.itemEffects[bank];

	return 0;
}

void ClearBattlerItemEffectHistory(u8 bank)
{
	gBattleStruct->ai.itemEffects[bank] = 0;
}

struct Pokemon* GetBankPartyData(u8 bank)
{
	u8 index = gBattlerPartyIndexes[bank];
	return (SIDE(bank) == B_SIDE_OPPONENT) ? &gEnemyParty[index] : &gPlayerParty[index];
}

u16 GetBaseCurrentHP(u8 bank)
{
	if (IsDynamaxed(bank))
	{
		//Ceiling
		return MathMax(1, gBattleMons[bank].hp / GetDynamaxHPBoost(bank) + (gBattleMons[bank].hp & 1));
	}

	return gBattleMons[bank].hp;
}

u16 GetBaseMaxHP(u8 bank)
{
	if (IsDynamaxed(bank))
	{
		//Ceiling
		return MathMax(1, gBattleMons[bank].maxHP / GetDynamaxHPBoost(bank) + (gBattleMons[bank].maxHP & 1));
	}

	return gBattleMons[bank].maxHP;
}

u8 GetBankFromPartyData(struct Pokemon* mon)
{
    int i;

	for (i = 0; i < gBattlersCount; ++i)
	{
		if (SIDE(i) == B_SIDE_OPPONENT)
		{
			if ((u32) (&gEnemyParty[gBattlerPartyIndexes[i]]) == (u32) mon)
				return i;
		}

		if (SIDE(i) == B_SIDE_PLAYER)
		{
			if ((u32) (&gPlayerParty[gBattlerPartyIndexes[i]]) == (u32) mon)
				return i;
		}
	}

	return PARTY_SIZE;
}

bool8 CanHitSemiInvulnerableTarget(u8 bankAtk, u8 bankDef, u16 move)
{
	if (ABILITY(bankAtk) == ABILITY_NOGUARD || ABILITY(bankDef) == ABILITY_NOGUARD)
		return TRUE;

	if (move == MOVE_TOXIC && IsOfType(bankAtk, TYPE_POISON))
		return TRUE;

	return gStatuses3[bankDef] & STATUS3_ALWAYS_HITS && gDisableStructs[bankDef].battlerWithSureHit == bankAtk;
}

bool8 CheckGrounding(u8 bank)
{
	if (BATTLER_SEMI_INVULNERABLE(bank)) //Apparently a thing
		return IN_AIR;

	if (IsGravityActive()
	|| GetBankItemEffect(bank) == HOLD_EFFECT_IRON_BALL
	|| (gStatuses3[bank] & (STATUS3_SMACKED_DOWN | STATUS3_ROOTED)))
		return GROUNDED;

	else if ((gStatuses3[bank] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS | STATUS3_ON_AIR))
		   || ITEM_EFFECT(bank) == HOLD_EFFECT_AIR_BALLOON
		   || ABILITY(bank) == ABILITY_LEVITATE
		   || gBattleMons[bank].type3 == TYPE_FLYING
		   || gBattleMons[bank].type1 == TYPE_FLYING
		   || gBattleMons[bank].type2 == TYPE_FLYING)
				return IN_AIR;

	return GROUNDED;
}

bool8 NonInvasiveCheckGrounding(u8 bank)
{
	if (BATTLER_SEMI_INVULNERABLE(bank)) //Apparently a thing
		return IN_AIR;

	if (IsGravityActive()
	|| GetRecordedItemEffect(bank) == HOLD_EFFECT_IRON_BALL
	|| (gStatuses3[bank] & (STATUS3_SMACKED_DOWN | STATUS3_ROOTED)))
		return GROUNDED;

	else if ((gStatuses3[bank] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS | STATUS3_ON_AIR))
		   || GetRecordedItemEffect(bank) == HOLD_EFFECT_AIR_BALLOON
		   || GetRecordedAbility(bank) == ABILITY_LEVITATE
		   || gBattleMons[bank].type3 == TYPE_FLYING
		   || gBattleMons[bank].type1 == TYPE_FLYING
		   || gBattleMons[bank].type2 == TYPE_FLYING)
				return IN_AIR;

	return GROUNDED;
}

bool8 CheckGroundingFromPartyData(struct Pokemon* mon)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);

	if (IsGravityActive()
	|| (ItemId_GetHoldEffect(item) == HOLD_EFFECT_IRON_BALL && GetMonAbility(mon) != ABILITY_KLUTZ))
		return GROUNDED;

	else if  (GetMonAbility(mon) == ABILITY_LEVITATE
		|| gBaseStats[species].type1 == TYPE_FLYING
		|| gBaseStats[species].type2 == TYPE_FLYING)
			return IN_AIR;
	return GROUNDED;
}

u8 ViableMonCountFromBank(u8 bank)
{
	return (SIDE(bank) == B_SIDE_PLAYER) ? ViableMonCount(gPlayerParty) : ViableMonCount(gEnemyParty);
}

u8 ViableMonCountFromBankLoadPartyRange(u8 bank)
{
	u8 count = 0;
	u8 firstMonId, lastMonId;
	struct Pokemon* party = LoadPartyRange(bank, &firstMonId, &lastMonId);
    int i;

	for (i = firstMonId; i < lastMonId; ++i)
	{
		if (party[i].box.species != SPECIES_NONE
		&& !GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
		&& party[i].hp != 0)
			++count;
	}

	return count;
}

bool8 CheckContact(u16 move, u8 bank)
{
	if (!(gBattleMoves[move].flags & FLAG_MAKES_CONTACT)
	|| ITEM_EFFECT(bank) == HOLD_EFFECT_PROTECTIVE_PADS
	|| ABILITY(bank) == ABILITY_LONGREACH)
		return FALSE;

	return TRUE;
}

bool8 CheckContactByMon(u16 move, struct Pokemon* mon)
{
	if (!(gBattleMoves[move].flags & FLAG_MAKES_CONTACT)
	|| GetMonItemEffect(mon) == HOLD_EFFECT_PROTECTIVE_PADS
	|| GetMonAbility(mon) == ABILITY_LONGREACH)
		return FALSE;

	return TRUE;
}

bool8 CheckHealingMove(u16 move)
{
	return gBattleMoves[move].flags & FLAG_TRIAGE_AFFECTED;
}

bool8 CheckSoundMove(u16 move)
{
	return CheckTableForMove(move, gSoundMoves);
}

bool8 SheerForceCheck(void)
{
	return ABILITY(gBattlerAttacker) == ABILITY_SHEERFORCE && CheckTableForMove(gCurrentMove, gSheerForceBoostedMoves);
}

bool8 IsOfType(u8 bank, u8 type)
{
	u8 type1 = gBattleMons[bank].type1;
	u8 type2 = gBattleMons[bank].type2;
	u8 type3 = gBattleMons[bank].type3;

	if (!IS_BLANK_TYPE(type1) && type1 == type)
		return TRUE;

	if (!IS_BLANK_TYPE(type2) && type2 == type)
		return TRUE;

	if (!IS_BLANK_TYPE(type3) && type3 == type)
		return TRUE;

	return FALSE;
}

bool8 LiftProtect(u8 bank)
{
	if ((gProtectStructs[bank].protected && !IsDynamaxed(bank))
	|| gProtectStructs[bank].KingsShield
	|| gProtectStructs[bank].SpikyShield
	|| gProtectStructs[bank].BanefulBunker
	|| gProtectStructs[bank].obstruct
	|| gSideStatuses[SIDE(bank)] & (SIDE_STATUS_CRAFTY_SHIELD | SIDE_STATUS_MAT_BLOCK | SIDE_STATUS_QUICK_GUARD | SIDE_STATUS_WIDE_GUARD))
	{
		if (!IsDynamaxed(bank))
			gProtectStructs[bank].protected = 0; //Max Guard is not affected by Feint

		gProtectStructs[bank].KingsShield = 0;
		gProtectStructs[bank].SpikyShield = 0;
		gProtectStructs[bank].BanefulBunker = 0;
		gProtectStructs[bank].obstruct = 0;
		gSideStatuses[SIDE(bank)] &= ~(SIDE_STATUS_CRAFTY_SHIELD | SIDE_STATUS_MAT_BLOCK | SIDE_STATUS_QUICK_GUARD | SIDE_STATUS_WIDE_GUARD);
		return TRUE;
	}

	return FALSE;
}

bool8 ProtectsAgainstZMoves(u16 move, u8 bankAtk, u8 bankDef)
{
	if (gProtectStructs[bankDef].protected
	|| gProtectStructs[bankDef].SpikyShield
	|| gProtectStructs[bankDef].BanefulBunker
	|| gProtectStructs[bankDef].obstruct)
	{
		return TRUE;
	}
	else if ((gProtectStructs[bankDef].KingsShield || (gSideStatuses[SIDE(bankDef)] & SIDE_STATUS_MAT_BLOCK))
		 && SPLIT(move) != SPLIT_STATUS)
	{
		return TRUE;
	}
	else if (gSideStatuses[SIDE(bankDef)] & SIDE_STATUS_CRAFTY_SHIELD && bankAtk != bankDef && SPLIT(move) == SPLIT_STATUS)
	{
		return TRUE;
	}
	else if (gSideStatuses[SIDE(bankDef)] & (SIDE_STATUS_QUICK_GUARD) && PriorityCalc(bankAtk, B_ACTION_USE_MOVE, move) > 0)
	{
		return TRUE;
	}
	else if (gSideStatuses[SIDE(bankDef)] & (SIDE_STATUS_WIDE_GUARD)
		&& (gBattleMoves[move].target == MOVE_TARGET_BOTH || gBattleMoves[move].target == MOVE_TARGET_FOES_AND_ALLY))
	{
		return TRUE;
	}

	return FALSE;
}

bool8 StatsMaxed(u8 bank)
{
	u8 i;
	for (i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) < STAT_STAGE_MAX)
			return FALSE;
	}

	return TRUE;
}

bool8 MainStatsMaxed(u8 bank)
{
	u8 i;
	for (i = STAT_STAGE_ATK; i <= STAT_SPDEF; ++i)
	{
		if (STAT_STAGE(bank, i) < STAT_STAGE_MAX)
			return FALSE;
	}

	return TRUE;
}

bool8 StatsMinned(u8 bank)
{
	u8 i;
	for (i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) > 0)
			return FALSE;
	}
	return TRUE;
}

bool8 MainStatsMinned(u8 bank)
{
	u8 i;
	for (i = STAT_STAGE_ATK; i <= STAT_SPDEF; ++i)
	{
		if (STAT_STAGE(bank, i) > 0)
			return FALSE;
	}
	return TRUE;
}

bool8 AnyStatGreaterThan(u8 bank, u8 amount)
{
	u8 i;
	for (i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) > amount)
			return TRUE;
	}

	return FALSE;
}

u8 CountBoosts(u8 bank)
{
	u8 sum = 0;
	u8 i;

	for (i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		if (STAT_STAGE(bank, i) > 6)
			sum += STAT_STAGE(bank, i) - 6;
	}

	return sum;
}

bool8 IsUnusableMove(u16 move, u8 bank, u8 check, u8 pp, u8 ability, u8 holdEffect, u16 choicedMove)
{
	bool8 isMaxMove = IsAnyMaxMove(move);

	if (move == MOVE_NONE && check & MOVE_LIMITATION_ZEROMOVE)
		return TRUE;
	else if (pp == 0 && check & MOVE_LIMITATION_PP)
		return TRUE;
	else if (!isMaxMove && move == gDisableStructs[bank].disabledMove && check & MOVE_LIMITATION_DISABLED)
		return TRUE;
	else if (!isMaxMove && move == gLastMoves[bank] && check & MOVE_LIMITATION_TORMENTED && IsTormented(bank))
		return TRUE;
	else if (IsTaunted(bank) && check & MOVE_LIMITATION_TAUNT && SPLIT(move) == SPLIT_STATUS)
		return TRUE;
	else if (!isMaxMove && GetImprisonedMovesCount(bank, move) && check & MOVE_LIMITATION_IMPRISON)
		return TRUE;
	else if (gDisableStructs[bank].encoreTimer && gDisableStructs[bank].encoredMove != move && check & MOVE_LIMITATION_ENCORE)
		return TRUE;
	else if (!isMaxMove
		 && (holdEffect == HOLD_EFFECT_CHOICE_BAND || ability == ABILITY_GORILLATACTICS)
		 && choicedMove != 0 && choicedMove != 0xFFFF && choicedMove != move
		 && check & MOVE_LIMITATION_CHOICE)
	{
		return TRUE;
	}
	else if (holdEffect == HOLD_EFFECT_ASSAULT_VEST && SPLIT(move) == SPLIT_STATUS)
		return TRUE;
	#ifdef FLAG_SKY_BATTLE
	else if (FlagGet(FLAG_SKY_BATTLE) && CheckTableForMove(move, gSkyBattleBannedMoves))
		return TRUE;
	#endif
	else if (IsGravityActive() && CheckTableForMove(move, gGravityBannedMoves))
		return TRUE;
	else if (CantUseSoundMoves(bank) && CheckSoundMove(move))
		return TRUE;
	else if (IsHealBlocked(bank) && CheckHealingMove(move))
		return TRUE;
	else if (IsRaidBattle() && bank != BANK_RAID_BOSS && CheckTableForMove(move, gRaidBattleBannedMoves))
		return TRUE;

	return FALSE;
}

u8 CheckMoveLimitationsFromParty(struct Pokemon* mon, u8 unusableMoves, u8 check)
{
	u8 holdEffect = GetMonItemEffect(mon);
	int i;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);

		if (move == MOVE_NONE && check & MOVE_LIMITATION_ZEROMOVE)
			unusableMoves |= gBitTable[i];
		else if (GetMonData(mon, MON_DATA_PP1 + i, NULL) == 0 && check & MOVE_LIMITATION_PP)
			unusableMoves |= gBitTable[i];
		else if (holdEffect == HOLD_EFFECT_ASSAULT_VEST && SPLIT(move) == SPLIT_STATUS)
			unusableMoves |= gBitTable[i];
		#ifdef FLAG_SKY_BATTLE
		else if (FlagGet(FLAG_SKY_BATTLE) && CheckTableForMove(move, gSkyBattleBannedMoves))
			unusableMoves |= gBitTable[i];
		#endif
		else if (IsGravityActive() && CheckTableForMove(move, gGravityBannedMoves))
			unusableMoves |= gBitTable[i];
		else if (IsRaidBattle() && CheckTableForMove(move, gRaidBattleBannedMoves))
			unusableMoves |= gBitTable[i];
	}

	return unusableMoves;
}

bool8 IsMoveRedirectedByFollowMe(u16 move, u8 bankAtk, u8 defSide)
{	
	if (SIDE(bankAtk) == SIDE(gSideTimers[defSide].followmeTarget)
	|| IsMoveRedirectionPrevented(move, ABILITY(bankAtk))
	|| gSideTimers[defSide].followmeTimer == 0
	|| !BATTLER_ALIVE(gSideTimers[defSide].followmeTarget)
	|| (gBattleStruct->ragePowdered & gBitTable[defSide] && !IsAffectedByPowder(bankAtk)))
		return FALSE;

	return TRUE;
}

bool8 IsMoveRedirectionPrevented(u16 move, u8 atkAbility)
{
	return move == MOVE_SKYDROP
		|| move == MOVE_SNIPESHOT
//		|| atkAbility == ABILITY_PROPELLERTAIL
		|| atkAbility == ABILITY_STALWART;
}

bool8 IsBattlerAlive(u8 bank)
{
	if (!BATTLER_ALIVE(bank)
	||	bank >= gBattlersCount
	||	gAbsentBattlerFlags & gBitTable[bank])
	{
		return FALSE;
	}

	return TRUE;
}

struct Pokemon* LoadPartyRange(u8 bank, u8* firstMonId, u8* lastMonId)
{
	struct Pokemon* party;

	party = (SIDE(bank) == B_SIDE_OPPONENT) ? gEnemyParty : gPlayerParty;

	if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && SIDE(bank) == B_SIDE_PLAYER)
	{
		*firstMonId = 0;
		if (GetBattlerPosition(bank) == B_POSITION_PLAYER_RIGHT)
			*firstMonId = 3;

		*lastMonId = *firstMonId + 3;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
	{
		//Two Human Trainers vs Two AI Trainers
		if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
		{
			if (SIDE(bank) == B_SIDE_PLAYER)
			{
				*firstMonId = 0;
				if (GetLinkTrainerFlankId(GetBattlerMultiplayerId(bank)) == TRUE)
					*firstMonId = 3;
			}
			else //B_SIDE_OPPONENT
			{
				if (bank == B_POSITION_OPPONENT_LEFT)
					*firstMonId = 0;
				else
					*firstMonId = 3;
			}
		}
		//Two Human Trainers vs Two Human Trainers
		else
		{
			*firstMonId = 0;
			if (GetLinkTrainerFlankId(GetBattlerMultiplayerId(bank)) == TRUE)
				*firstMonId = 3;
		}

		*lastMonId = *firstMonId + 3;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && SIDE(bank) == B_SIDE_OPPONENT)
	{
		*firstMonId = 0;
		if (GetBattlerPosition(bank) == B_POSITION_OPPONENT_RIGHT)
			*firstMonId = 3;

		*lastMonId = *firstMonId + 3;
	}
	else //Single Battle // Regular Double Battle // Regular Double Against Two Trainers + PlayerSide // Wild Double Battle
	{
		*firstMonId = 0;
		*lastMonId = PARTY_SIZE;
	}

	return party;
}

bool8 IsUproarBeingMade(void)
{
	u32 i;
	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gBattleMons[i].status2 & STATUS2_UPROAR)
			return TRUE;
	}

	return FALSE;
}

//Change to loop through battle modified party indexes
u8 GetIllusionPartyNumber(u8 bank)
{
	u8 firstMonId, lastMonId;
	u32 i;
	struct Pokemon* party;

	if (gStatuses3[bank] & STATUS3_ILLUSION)
	{
		//Wild Pokemon can't diguise themselves
		if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(bank) == B_SIDE_OPPONENT)
			return gBattlerPartyIndexes[bank];
		
		//Check for a saved party number first.
		//This is necessary for when in doubles the last alive Pokemon is KOd.
		//If the party was checked each time, the Pokemon could change who it was
		//disguised as mid-Illusion.
		if (gBattleStruct->disguisedAs[bank] > 0)
			return gBattleStruct->disguisedAs[bank] - 1;

		party = LoadPartyRange(bank, &firstMonId, &lastMonId);

		for (i = lastMonId - 1; i >= firstMonId; --i) //Loop through party in reverse order
		{
			if (i == gBattlerPartyIndexes[bank]) //Finsihed checking mons after
				return gBattlerPartyIndexes[bank];

			if (party[i].box.species == SPECIES_NONE
			|| party[i].hp == 0
			|| GetMonData(&party[i], MON_DATA_IS_EGG, NULL))
				continue;

			gBattleStruct->disguisedAs[bank] = i + 1;
			return i;
		}
	}

	return gBattlerPartyIndexes[bank];
}

struct Pokemon* GetIllusionPartyData(u8 bank)
{
	u8 firstMonId, lastMonId;
	struct Pokemon* party = LoadPartyRange(bank, &firstMonId, &lastMonId);

	return &party[GetIllusionPartyNumber(bank)];
}

bool8 BankMovedBefore(u8 bank1, u8 bank2)
{
	u32 i;
	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gBattlerByTurnOrder[i] == bank1)
			return TRUE;
		else if (gBattlerByTurnOrder[i] == bank2)
			return FALSE;
	}

	return FALSE; //Should never be reached
}

bool8 BankMovedBeforeIgnoreSwitch(u8 bank1, u8 bank2)
{
	u32 i;
	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gChosenActionByBattler[gBattlerByTurnOrder[i]] == B_ACTION_SWITCH)
			continue;

		if (gBattlerByTurnOrder[i] == bank1)
			return TRUE;
		else if (gBattlerByTurnOrder[i] == bank2)
			return FALSE;
	}

	return FALSE; //Should never be reached
}

bool8 IsFirstAttacker(u8 bank)
{
	u8 i;
	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gActionsByTurnOrder[i] == B_ACTION_USE_ITEM
		||  gActionsByTurnOrder[i] == B_ACTION_SWITCH)
			continue;

		return gBattlerByTurnOrder[i] == bank;
	}

	return FALSE; //Should never be reached
}

bool8 CanTransferItem(u16 species, u16 item)
{
	u16 dexNum = SpeciesToNationalPokedexNum(species);
	u8 effect = ItemId_GetHoldEffect(item);
	const struct Evolution* evolutions = gEvolutionTable[species];
	int i;

	if (ItemIsMail(item))
		return FALSE;

	switch (effect) {
		case HOLD_EFFECT_Z_CRYSTAL:
			return FALSE;

		#ifdef NATIONAL_DEX_GIRATINA
		case HOLD_EFFECT_GRISEOUS_ORB:
			if (dexNum == NATIONAL_DEX_GIRATINA)
				return FALSE;
			break;
		#endif

		#ifdef NATIONAL_DEX_ARCEUS
		case HOLD_EFFECT_PLATE:
			if (dexNum == NATIONAL_DEX_ARCEUS)
				return FALSE;
			break;
		#endif

		#ifdef NATIONAL_DEX_SILVALLY
		case HOLD_EFFECT_MEMORY:
			if (dexNum == NATIONAL_DEX_SILVALLY)
				return FALSE;
			break;
		#endif

		#ifdef NATIONAL_DEX_GENESECT
		case HOLD_EFFECT_DRIVE:
			if (dexNum == NATIONAL_DEX_GENESECT)
				return FALSE;
			break;
		#endif

		case HOLD_EFFECT_MEGA_STONE:
			for (i = 0; i < EVOS_PER_MON; ++i)
			{
				if ((evolutions[i].method == MEGA_EVOLUTION && evolutions[i].param == item) //Can Mega Evolve
				||  (evolutions[i].method == MEGA_EVOLUTION && evolutions[i].param == 0)) //Is Mega
					return FALSE;
			}
			break;

		case HOLD_EFFECT_PRIMAL_ORB:
			for (i = 0; i < EVOS_PER_MON; ++i)
			{
				if ((evolutions[i].method == MEGA_EVOLUTION && evolutions[i].unknown == MEGA_VARIANT_PRIMAL && evolutions[i].param == item) //Can Primal Evolve
				||  (evolutions[i].method == MEGA_EVOLUTION && evolutions[i].unknown == MEGA_VARIANT_PRIMAL && evolutions[i].param == 0)) //Is Primal
					return FALSE;
			}
			break;
	}

	return TRUE;
}

//Make sure the input bank is any bank on the specific mon's side
bool8 CanFling(u16 item, u16 species, u8 ability, u8 bankOnSide, u8 embargoTimer)
{
	u8 itemEffect = ItemId_GetHoldEffect(item);

	if (item == ITEM_NONE
	|| ability == ABILITY_KLUTZ
	|| IsMagicRoomActive()
	|| embargoTimer != 0
	|| !CanTransferItem(species, item)
	|| itemEffect == HOLD_EFFECT_PRIMAL_ORB
	|| itemEffect == HOLD_EFFECT_GEM
	|| itemEffect == HOLD_EFFECT_ABILITY_CAPSULE
	|| (IsBerry(item) && AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, bankOnSide, ABILITY_UNNERVE, 0, 0))
	|| GetPocketByItemId(item) == POCKET_POKE_BALLS)
		return FALSE;

	return TRUE;
}

bool8 SymbiosisCanActivate(u8 giverBank, u8 receiverBank)
{
	u16 item = ITEM(giverBank);

	if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	||  ABILITY(giverBank) != ABILITY_SYMBIOSIS
	||  ITEM(receiverBank) != ITEM_NONE
	||  ITEM(giverBank) == ITEM_NONE
	|| !CanTransferItem(gBattleMons[giverBank].species, item)
	|| !CanTransferItem(gBattleMons[receiverBank].species, item))
		return FALSE;

	return TRUE;
}

//Sticky Hold also, but the boost ignores it
bool8 CanKnockOffItem(u8 bank)
{
	u16 item = ITEM(bank);

	if (item == ITEM_NONE)
		return FALSE;

	if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(bank) == B_SIDE_PLAYER) //Wild mons can't knock off items
		return FALSE;

	if (!CanTransferItem(SPECIES(bank), item))
		return FALSE;

	return TRUE;
}

bool8 CanKnockOffMonItem(struct Pokemon* mon, u8 side)
{
	u16 item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);

	if (item == ITEM_NONE)
		return FALSE;

	if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && side == B_SIDE_PLAYER) //Wild mons can't knock off items
		return FALSE;

	if (!CanTransferItem(GetMonData(mon, MON_DATA_SPECIES, NULL), item))
		return FALSE;

	return TRUE;
}

bool8 IsAffectedByPowder(u8 bank)
{
	return IsAffectedByPowderByDetails(gBattleMons[bank].type1, gBattleMons[bank].type2, gBattleMons[bank].type3, ABILITY(bank), ITEM_EFFECT(bank));
}

bool8 IsAffectedByPowderByDetails(u8 type1, u8 type2, u8 type3, u8 ability, u8 itemEffect)
{
	return ability != ABILITY_OVERCOAT
		&& itemEffect != HOLD_EFFECT_SAFETY_GOGGLES
		&& type1 != TYPE_GRASS
		&& type2 != TYPE_GRASS
		&& type3 != TYPE_GRASS;
}

bool8 MoveIgnoresSubstitutes(u16 move, u8 atkAbility)
{
	return CheckSoundMove(move)
		|| (atkAbility == ABILITY_INFILTRATOR && move != MOVE_TRANSFORM && move != MOVE_SKYDROP)
		|| CheckTableForMove(move, gSubstituteBypassMoves);
}

bool8 MoveBlockedBySubstitute(u16 move, u8 bankAtk, u8 bankDef)
{
	return IS_BEHIND_SUBSTITUTE(bankDef) && !MoveIgnoresSubstitutes(move, ABILITY(bankAtk));
}

bool8 MonMoveBlockedBySubstitute(u16 move, struct Pokemon* monAtk, u8 bankDef)
{
	return IS_BEHIND_SUBSTITUTE(bankDef) && !MoveIgnoresSubstitutes(move, GetMonAbility(monAtk));
}

bool8 IsMockBattle(void)
{
	return (gBattleTypeFlags & BATTLE_TYPE_MOCK_BATTLE) != 0;
}

bool8 IsMoveAffectedByParentalBond(u16 move, u8 bankAtk)
{
	if (SPLIT(move) != SPLIT_STATUS
	&& !IsAnyMaxMove(move)
	&& !CheckTableForMove(move, gParentalBondBannedMoves)
	&& !IsTwoTurnsMove(move)
	&& gBattleMoves[move].effect != EFFECT_OHKO
	&& gBattleMoves[move].effect != EFFECT_MULTI_HIT
	&& gBattleMoves[move].effect != EFFECT_TRIPLE_KICK
	&& gBattleMoves[move].effect != EFFECT_DOUBLE_HIT)
	{
		if (IS_DOUBLE_BATTLE)
		{
			switch (gBattleMoves[move].target) {
				case MOVE_TARGET_BOTH:
					if (CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, 0, FOE(bankAtk)) >= 2) //Check for single target
						return FALSE;
					break;
				case MOVE_TARGET_FOES_AND_ALLY:
					if (CountAliveMonsInBattle(BATTLE_ALIVE_EXCEPT_ACTIVE, bankAtk, 0) >= 2) //Count mons on both sides; ignore attacker
						return FALSE;
					break;
			}
		}

		return TRUE;
	}
	
	return FALSE;
}

u8 CalcMoveSplit(u8 bank, u16 move)
{
	if (CheckTableForMove(move, gMovesThatChangePhysicality)
	&&  SPLIT(move) != SPLIT_STATUS)
	{
		u32 attack = gBattleMons[bank].attack;
		u32 spAttack = gBattleMons[bank].spAttack;

		attack = attack * gStatStageRatios[STAT_STAGE(bank, STAT_STAGE_ATK)][0];
		attack = attack / gStatStageRatios[STAT_STAGE(bank, STAT_STAGE_ATK)][1];

		spAttack = spAttack * gStatStageRatios[STAT_STAGE(bank, STAT_STAGE_SPATK)][0];
		spAttack = spAttack / gStatStageRatios[STAT_STAGE(bank, STAT_STAGE_SPATK)][1];

		if (spAttack >= attack)
			return SPLIT_SPECIAL;
		else
			return SPLIT_PHYSICAL;
	}

	#ifdef OLD_MOVE_SPLIT
		if (gBattleMoves[move].type < TYPE_FIRE)
			return SPLIT_PHYSICAL;
		else
			return SPLIT_SPECIAL;
	#else
		return SPLIT(move);
	#endif
}

u8 CalcMoveSplitFromParty(struct Pokemon* mon, u16 move)
{
	if (CheckTableForMove(move, gMovesThatChangePhysicality))
	{
		if (mon->spAttack >= mon->attack)
			return SPLIT_SPECIAL;
		else
			return SPLIT_PHYSICAL;
	}

	return SPLIT(move);
}

u8 FindMovePositionInMoveset(u16 move, u8 bank)
{
	int i;

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (gBattleMons[bank].moves[i] == move)
			break;
	}

	return i;
}

bool8 MoveInMoveset(u16 move, u8 bank)
{
	return FindMovePositionInMoveset(move, bank) < MAX_MON_MOVES;
}

bool8 IsZMove(const u16 move)
{
	return move >= FIRST_Z_MOVE && move <= LAST_Z_MOVE;
}

void ResetVarsForAbilityChange(u8 bank)
{
	gBattleStruct->SlowStartTimers[bank] = 0;
	gDisableStructs[gBattlerTarget].truantCounter = 0;
	gStatuses3[bank] &= ~(STATUS3_INTIMIDATE_POKES);
	TryRemoveUnburdenBoost(bank);
}

void HandleUnburdenBoost(u8 bank)
{
	//1. Boost bit is only set if the Pokemon has Unburden.
	//2. Losing Unburden clears the boost, even if the Pokemon gets Unburden back.
	//3. If Unburden is suppressed, the boost isn't lost if Unburden returns,
	//   but a new one can't be gained during the suppression.
	if (ABILITY(bank) == ABILITY_UNBURDEN && ITEM(bank) == ITEM_NONE)
		gBattleStruct->UnburdenBoosts |= gBitTable[bank];
	else if (*GetAbilityLocation(bank) != ABILITY_UNBURDEN || ITEM(bank) != ITEM_NONE)
		gBattleStruct->UnburdenBoosts &= ~gBitTable[bank];
}

static void TryRemoveUnburdenBoost(u8 bank)
{
	if (*GetAbilityLocation(bank) != ABILITY_UNBURDEN || ITEM(bank) != ITEM_NONE)
		gBattleStruct->UnburdenBoosts &= ~gBitTable[bank];
}

void AddBankToPickupStack(const u8 bank)
{
	u32 i, j;
	u8 newStack[MAX_BATTLERS_COUNT];

	for (i = 0, j = 0; i < gBattlersCount; ++i)
	{
		if (gBattleStruct->pickupStack[i] != bank && gBattleStruct->pickupStack[i] != 0xFF)
			newStack[j++] = gBattleStruct->pickupStack[i];
	}

	newStack[j++] = bank;

	while (j < gBattlersCount)
		newStack[j++] = 0xFF;

	for (i = 0; i < gBattlersCount; ++i)
		gBattleStruct->pickupStack[i] = newStack[i];
}

void RemoveBankFromPickupStack(const u8 bank)
{
	u32 i, j;
	u8 newStack[MAX_BATTLERS_COUNT];

	for (i = 0, j = 0; i < gBattlersCount; ++i)
	{
		if (gBattleStruct->pickupStack[i] != bank && gBattleStruct->pickupStack[i] != 0xFF)
			newStack[j++] = gBattleStruct->pickupStack[i];
	}

	while (j < gBattlersCount)
		newStack[j++] = 0xFF;

	for (i = 0; i < gBattlersCount; ++i)
		gBattleStruct->pickupStack[i] = newStack[i];
}

u8 GetTopOfPickupStackNotIncludingBank(const u8 bank)
{
	u32 i;

	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gBattleStruct->pickupStack[i] == 0xFF)
			break;
	}

	if (i == 0 //Stack is empty
	|| (i == 1 && gBattleStruct->pickupStack[0] == bank)) //Stack only contains ignored bank
		return 0xFF;

	if (gBattleStruct->pickupStack[i - 1] == bank)
		return gBattleStruct->pickupStack[i - 2];

	return gBattleStruct->pickupStack[i - 1];
}

void RemoveScreensFromSide(const u8 side)
{
	gSideStatuses[side] &= ~(SIDE_STATUS_REFLECT);
	gSideStatuses[side] &= ~(SIDE_STATUS_LIGHTSCREEN);
	gSideTimers[side].reflectTimer = 0;
	gSideTimers[side].lightscreenTimer = 0;
	gBattleStruct->AuroraVeilTimers[side] = 0;
}

void ClearBankStatus(u8 bank)
{
	if (gBattleMons[bank].status1 & (STATUS1_POISON | STATUS1_TOXIC_POISON))
		StringCopy(gBattleTextBuff1, gText_Poison);
	else if (gBattleMons[bank].status1 & STATUS1_SLEEP)
		StringCopy(gBattleTextBuff1, gText_Sleep);
	else if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
		StringCopy(gBattleTextBuff1, gText_Paralysis);
	else if (gBattleMons[bank].status1 & STATUS1_BURN)
		StringCopy(gBattleTextBuff1, gText_Burn);
	else if (gBattleMons[bank].status1 & STATUS1_FREEZE)
		StringCopy(gBattleTextBuff1, gText_Ice);

	gBattleMons[bank].status1 = 0;
	gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
	gActiveBattler = bank;
	BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bank].status1);
	MarkBattlerForControllerExec(gActiveBattler);
}

bool8 DoesSleepClausePrevent(u8 bank)
{
	int i;
	u8 firstId, lastId;
	struct Pokemon* party;

	if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{
		switch (VarGet(VAR_BATTLE_FACILITY_TIER)) 
		{
			case BATTLE_FACILITY_OU:
			case BATTLE_FACILITY_UBER:
			case BATTLE_FACILITY_LITTLE_CUP:
			case BATTLE_FACILITY_MONOTYPE:
			case BATTLE_FACILITY_CAMOMONS:
			case BATTLE_FACILITY_UBER_CAMOMONS:
			case BATTLE_FACILITY_LC_CAMOMONS:
			case BATTLE_FACILITY_SCALEMONS:
			case BATTLE_FACILITY_350_CUP:
			case BATTLE_FACILITY_AVERAGE_MONS:
			case BATTLE_FACILITY_BENJAMIN_BUTTERFREE:
			case BATTLE_FACILITY_NATIONAL_DEX_OU: ;
				
				party = LoadPartyRange(bank, &firstId, &lastId);

				for (i = 0; i < PARTY_SIZE; ++i)
				{
					if (GetMonData(&party[i], MON_DATA_HP, NULL) != 0
					&& !GetMonData(&party[i], MON_DATA_IS_EGG, NULL)
					&& GetMonData(&party[i], MON_DATA_STATUS, NULL) & STATUS1_SLEEP) //Someone on team is already asleep
						return TRUE;
				}
		}
	}

	return FALSE;
}

bool8 CanBeGeneralStatused(u8 bank, bool8 checkFlowerVeil)
{
	#ifdef SPECIES_MINIOR_SHIELD
	if (ABILITY(bank) == ABILITY_SHIELDSDOWN
	&&  GetBankPartyData(bank)->box.species == SPECIES_MINIOR_SHIELD) //Prevents Ditto from getting this benefit
		return FALSE;
	#endif

	switch (ABILITY(bank)) {
		case ABILITY_COMATOSE:
			return FALSE;

		case ABILITY_LEAFGUARD:
			if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA)
				return FALSE;
			break;

		case ABILITY_FLOWERVEIL:
			if (checkFlowerVeil && IsOfType(bank, TYPE_GRASS))
				return FALSE;
	}

	if (checkFlowerVeil && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL && IsOfType(bank, TYPE_GRASS) && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD))
		return FALSE;

	if (gTerrainType == MISTY_TERRAIN && CheckGrounding(bank))
		return FALSE;

	if (gBattleMons[bank].status1 != STATUS1_NONE)
		return FALSE;

	if (checkFlowerVeil && gSideStatuses[SIDE(bank)] & SIDE_STATUS_SAFEGUARD && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD))
		return FALSE;

	return TRUE;
}

bool8 CanBePutToSleep(u8 bank, bool8 checkFlowerVeil)
{
	if (!CanBeGeneralStatused(bank, checkFlowerVeil))
		return FALSE;

	switch (ABILITY(bank)) {
		case ABILITY_INSOMNIA:
		case ABILITY_VITAL_SPIRIT:
		case ABILITY_SWEETVEIL:
			return FALSE;
	}

	if (gTerrainType == ELECTRIC_TERRAIN && CheckGrounding(bank))
		return FALSE;

	if (DoesSleepClausePrevent(bank))
		return FALSE;

	return TRUE;
}

bool8 CanBeYawned(u8 bank)
{
	#ifdef SPECIES_MINIOR_SHIELD
	if (ABILITY(bank) == ABILITY_SHIELDSDOWN
	&&  GetBankPartyData(bank)->box.species == SPECIES_MINIOR_SHIELD) //Prevents Ditto from getting this benefit
		return FALSE;
	#endif

	if (ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL && IsOfType(bank, TYPE_GRASS) && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD))
		return FALSE;

	if (gTerrainType == ELECTRIC_TERRAIN && CheckGrounding(bank))
		return FALSE;

	if (gBattleMons[bank].status1 != STATUS1_NONE)
		return FALSE;

	if (gSideStatuses[SIDE(bank)] & SIDE_STATUS_SAFEGUARD && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD))
		return FALSE;

	switch (ABILITY(bank)) {
		case ABILITY_INSOMNIA:
		case ABILITY_VITAL_SPIRIT:
		case ABILITY_SWEETVEIL:
		case ABILITY_COMATOSE:
			return FALSE;
		case ABILITY_LEAFGUARD:
			if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA)
				return FALSE;
			break;
		case ABILITY_FLOWERVEIL:
			if (IsOfType(bank, TYPE_GRASS))
				return FALSE;
			break;
	}

	return TRUE;
}

bool8 CanBePoisoned(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil)
{
	if (!CanBeGeneralStatused(bankDef, checkFlowerVeil))
		return FALSE;

	switch (ABILITY(bankDef)) {
		case ABILITY_IMMUNITY:
		case ABILITY_PASTELVEIL:
			return FALSE;
	}

	if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(bankDef)) == ABILITY_PASTELVEIL)
		return FALSE;

	if (ABILITY(bankAtk) != ABILITY_CORROSION)
	{
		if (IsOfType(bankDef, TYPE_POISON) || IsOfType(bankDef, TYPE_STEEL))
			return FALSE;
	}

	return TRUE;
}

bool8 CanBeParalyzed(u8 bank, bool8 checkFlowerVeil)
{
	if (!CanBeGeneralStatused(bank, checkFlowerVeil))
		return FALSE;

	if (IsOfType(bank, TYPE_ELECTRIC))
		return FALSE;

	switch (ABILITY(bank)) {
		case ABILITY_LIMBER:
			return FALSE;
	}

	return TRUE;
}

bool8 CanBeBurned(u8 bank, bool8 checkFlowerVeil)
{
	if (!CanBeGeneralStatused(bank, checkFlowerVeil))
		return FALSE;

	if (IsOfType(bank, TYPE_FIRE))
		return FALSE;

	switch (ABILITY(bank)) {
		case ABILITY_WATER_VEIL:
		case ABILITY_WATERBUBBLE:
			return FALSE;
	}

	return TRUE;
}

bool8 CanBeFrozen(u8 bank, bool8 checkFlowerVeil)
{
	if (!CanBeGeneralStatused(bank, checkFlowerVeil))
		return FALSE;

	if (IsOfType(bank, TYPE_ICE))
		return FALSE;

	switch (ABILITY(bank)) {
		case ABILITY_MAGMA_ARMOR:
			return FALSE;
	}

	if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA)
		return FALSE;

	return TRUE;
}

bool8 CanBeConfused(u8 bank, u8 checkSafeguard)
{
	if (IsConfused(bank))
		return FALSE;

	if (gTerrainType == MISTY_TERRAIN && CheckGrounding(bank))
		return FALSE;

	if (ABILITY(bank) == ABILITY_OWN_TEMPO)
		return FALSE;

	if (checkSafeguard && gSideStatuses[SIDE(bank)] & SIDE_STATUS_SAFEGUARD && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD))
		return FALSE;

	return TRUE;
}

bool8 CanBeTormented(u8 bank)
{
	return !(gBattleMons[bank].status2 & STATUS2_TORMENT) && !IsDynamaxed(bank);
}

bool8 CanBeInfatuated(u8 bankDef, u8 bankAtk)
{
	struct Pokemon *monAttacker, *monTarget;
	u16 speciesAttacker, speciesTarget;
	u32 personalityAttacker, personalityTarget;

	monAttacker = GetBankPartyData(bankAtk);
	monTarget = GetBankPartyData(bankDef);

	speciesAttacker = monAttacker->box.species;
	personalityAttacker = monAttacker->box.personality;

	speciesTarget = monTarget->box.species;
	personalityTarget = monTarget->box.personality;

	return BATTLER_ALIVE(bankDef)
		&& !(gBattleMons[bankDef].status2 & STATUS2_INFATUATION)
		&& ABILITY(bankDef) != ABILITY_OBLIVIOUS
		&& GetGenderFromSpeciesAndPersonality(speciesAttacker, personalityAttacker) != GetGenderFromSpeciesAndPersonality(speciesTarget, personalityTarget)
		&& GetGenderFromSpeciesAndPersonality(speciesAttacker, personalityAttacker) != MON_GENDERLESS
		&& GetGenderFromSpeciesAndPersonality(speciesTarget, personalityTarget) != MON_GENDERLESS
		&& !AbilityBattleEffects(ABILITYEFFECT_CHECK_BATTLER_SIDE, bankDef, ABILITY_AROMAVEIL, 0, 0);
}

bool8 CheckTableForAbility(u8 ability, const u8 table[])
{
    u32 i;
	for (i = 0; table[i] != ABILITY_TABLES_TERMIN; ++i)
	{
		if (ability == table[i])
			return TRUE;
	}

	return FALSE;
}

bool8 IsTrickRoomActive(void)
{
	return gBattleStruct->TrickRoomTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TRICK_ROOM);
}

bool8 IsMagicRoomActive(void)
{
	return gBattleStruct->MagicRoomTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_MAGIC_ROOM);
}

bool8 IsWonderRoomActive(void)
{
	return gBattleStruct->WonderRoomTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_WONDER_ROOM);
}

bool8 IsGravityActive(void)
{
	return gBattleStruct->GravityTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_GRAVITY);
}

bool8 IsIonDelugeActive(void)
{
	return gBattleStruct->IonDelugeTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_ION_DELUGE);
}

bool8 IsFairyLockActive(void)
{
	return gBattleStruct->FairyLockTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_FAIRY_LOCK);
}

bool8 IsMudSportActive(void)
{
	return gBattleStruct->MudSportTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_MUD_SPORT);
}

bool8 IsWaterSportActive(void)
{
	return gBattleStruct->WaterSportTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_WATER_SPORT);
}

bool8 IsInverseBattle(void)
{
	return
		#ifdef FLAG_INVERSE
		FlagGet(FLAG_INVERSE) ||
		#endif
		(IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_INVERSE);
}

bool8 BankSideHasSafeguard(u8 bank)
{
	return gSideStatuses[SIDE(bank)] & SIDE_STATUS_SAFEGUARD
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_SAFEGUARD);
}

bool8 BankSideHasMist(u8 bank)
{
	return gSideStatuses[SIDE(bank)] & SIDE_STATUS_MIST
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_MIST);
}

bool8 BankSideHasSeaOfFire(u8 bank)
{
	return gBattleStruct->SeaOfFireTimers[SIDE(bank)]
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_SEA_OF_FIRE);
}

bool8 BankSideHasRainbow(u8 bank)
{
	return gBattleStruct->RainbowTimers[SIDE(bank)]
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_RAINBOW);
}

bool8 BankSideHasSwamp(u8 bank)
{
	return SideHasSwamp(SIDE(bank));
}

bool8 SideHasSwamp(u8 side)
{
	return gBattleStruct->SwampTimers[side];
}

bool8 BankSideHasGMaxVineLash(u8 bank)
{
	return gBattleStruct->maxVineLashTimers[SIDE(bank)] > 0;
}

bool8 BankSideHasGMaxWildfire(u8 bank)
{
	return gBattleStruct->maxWildfireTimers[SIDE(bank)] > 0;
}

bool8 BankSideHasGMaxCannonade(u8 bank)
{
	return gBattleStruct->maxCannonadeTimers[SIDE(bank)] > 0;
}

bool8 BankSideHasGMaxVolcalith(u8 bank)
{
	return gBattleStruct->maxVolcalithTimers[SIDE(bank)] > 0;
}

bool8 IsConfused(u8 bank)
{
	return (gBattleMons[bank].status2 & STATUS2_CONFUSION) != 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_CONFUSED && ABILITY(bank) != ABILITY_OWN_TEMPO);
}

bool8 IsTaunted(u8 bank)
{
	return gDisableStructs[bank].tauntTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TAUNT && ABILITY(bank) != ABILITY_OBLIVIOUS);
}

bool8 IsTormented(u8 bank)
{
	return (gBattleMons[bank].status2 & STATUS2_TORMENT) != 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TORMENT);
}

bool8 IsHealBlocked(u8 bank)
{
	return gBattleStruct->HealBlockTimers[bank] > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_HEAL_BLOCK);
}

bool8 CantUseSoundMoves(u8 bank)
{
	return gBattleStruct->ThroatChopTimers[bank] > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_THROAT_CHOP);
}

bool8 IsLaserFocused(u8 bank)
{
	return gBattleStruct->LaserFocusTimers[bank] > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_ALWAYS_CRIT);
}

bool8 IsAbilitySuppressed(u8 bank)
{
	return (gStatuses3[bank] & STATUS3_ABILITY_SUPPRESS) != 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_ABILITY_SUPPRESSION);
}

bool8 CantScoreACrit(u8 bank, struct Pokemon* mon)
{
	if (mon != NULL)
		return FALSE;

	return (gStatuses3[bank] & STATUS3_CANT_SCORE_A_CRIT) != 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_NO_CRITS);
}

u16 TryFixDynamaxTransformSpecies(u8 bank, u16 species)
{
	if (gDontRemoveTransformSpecies && gBattleSpritesDataPtr->battlerData[bank].transformSpecies != SPECIES_NONE)
		species = gBattleSpritesDataPtr->battlerData[bank].transformSpecies;

	return species;
}

bool8 IsTwoOpponentBattle(void)
{
	if ((gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS)) == (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS))
		return TRUE;

	return FALSE;
}

bool8 BankSideHasTwoTrainers(u8 bank)
{
	u8 side = SIDE(bank);

	return ((side == B_SIDE_OPPONENT && IsTwoOpponentBattle())
		 || (side == B_SIDE_PLAYER && IsTagBattle()));
}

bool8 IsTagBattle(void)
{
	u32 flags = (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_INGAME_PARTNER);

	if ((gBattleTypeFlags & flags) == flags)
		return TRUE;
	else
		return FALSE;
}

bool8 IsPartnerAttacker(void)
{
	return gBattlerAttacker == B_POSITION_PLAYER_RIGHT;
}

/////////////////////////////////////////////////////////////////////////CMD49
static const u8 gTargetsByBank[4][4] =
{
	{B_POSITION_OPPONENT_LEFT, B_POSITION_OPPONENT_RIGHT, B_POSITION_PLAYER_RIGHT, 0xFF},	//Bank 0 - Player Left
	{B_POSITION_PLAYER_LEFT, B_POSITION_PLAYER_RIGHT, B_POSITION_OPPONENT_RIGHT, 0xFF}, 	//Bank 1 - Opponent Left
	{B_POSITION_OPPONENT_LEFT, B_POSITION_OPPONENT_RIGHT, B_POSITION_PLAYER_LEFT, 0xFF}, 	//Bank 2 - Player Right
	{B_POSITION_PLAYER_LEFT, B_POSITION_PLAYER_RIGHT, B_POSITION_OPPONENT_LEFT, 0xFF}  		//Bank 3 - Opponent Right
};

u8 GetNextMultiTarget(void)
{
	u8 pos = gTargetsByBank[gBattlerAttacker][gBattleStruct->OriginalAttackerTargetCount];
	if (pos != 0xFF)
		return GetBattlerAtPosition(pos);

	return pos;
}