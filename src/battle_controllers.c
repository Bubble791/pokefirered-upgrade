#include "global.h"
#include "battle.h"
#include "battle_main.h"
#include "battle_ai_script_commands.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "link.h"
#include "link_rfu.h"
#include "cable_club.h"
#include "party_menu.h"
#include "task.h"
#include "util.h"
#include "dynamax.h"
#include "mega.h"
#include "set_z_effect.h"
#include "frontier.h"
#include "malloc.h"
#include "constants/abilities.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/battle_move_effects.h"

static EWRAM_DATA u8 sLinkSendTaskId = 0;
static EWRAM_DATA u8 sLinkReceiveTaskId = 0;
static EWRAM_DATA u8 gUnknown_202286E = 0;
EWRAM_DATA struct UnusedControllerStruct gUnknown_2022870 = {0};
EWRAM_DATA u8 sBattleBuffersTransferData[0x100] = {0};

static void CreateTasksForSendRecvLinkBuffers(void);
static void InitLinkBtlControllers(void);
static void InitSinglePlayerBtlControllers(void);
static void SetBattlePartyIds(void);
static void Task_HandleSendLinkBuffersData(u8 taskId);
static void Task_HandleCopyReceivedLinkBuffersData(u8 taskId);

void HandleLinkBattleSetup(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        if (gWirelessCommType)
            SetWirelessCommType1();
        if (!gReceivedRemoteLinkPlayers)
            OpenLink();
        CreateTask(Task_WaitForReceivedRemoteLinkPlayers5SecondTimeout, 0);
        CreateTasksForSendRecvLinkBuffers();
    }
}

void SetUpBattleVars(void)
{
    s32 i;

    gBattleMainFunc = BattleDummy;
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
    {
        gBattlerControllerFuncs[i] = PlayerDummy;
        gBattlerPositions[i] = 0xFF;
        gActionSelectionCursor[i] = 0;
        gMoveSelectionCursor[i] = 0;
    }
    HandleLinkBattleSetup();
    gBattleControllerExecFlags = 0;
    ClearBattleAnimationVars();
    ClearBattleMonForms();

    memset(gBattleBufferA, 0x0, sizeof(gBattleBufferA)); //Clear both battle buffers
	memset(gBattleBufferB, 0x0, sizeof(gBattleBufferB));
    memset(gBattleMons, 0x0, sizeof(gBattleMons));

    gBattleStringLoader = NULL;
    gSeedHelper = NULL;
    gTerrainType = 0;
    gBankSwitching = 0;
    gDontRemoveTransformSpecies = 0;
    gForceSwitchHelper = 0;
    gFormCounter = 0;
    gMagicianHelper= 0;
    gBackupHWord = 0;
    gAbilityPopUpHelper = 0;
    gShakerData[0] = 0;
    gShakerData[1] = 0;
    
    if (IsRaidBattle())
	{
		gBattleTypeFlags |= BATTLE_TYPE_DYNAMAX;
		gBattleStruct->dynamaxData.timer[B_POSITION_OPPONENT_LEFT] = -2; //Don't revert
		gBattleStruct->dynamaxData.backupRaidMonItem = GetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, NULL); //For Frontier
	}

    BattleAI_HandleItemUseBeforeAISetup();
    gUnknown_2022B54 = 0;
    gUnknown_2023DDC = 0;
}

void InitBtlControllers(void)
{
    s32 i;

    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        InitLinkBtlControllers();
    else
        InitSinglePlayerBtlControllers();
    SetBattlePartyIds();
    if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
        for (i = 0; i < gBattlersCount; ++i)
            BufferBattlePartyCurrentOrderBySide(i, 0);
}

static void InitSinglePlayerBtlControllers(void)
{
    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        gBattleMainFunc = BeginBattleIntro;
        if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
        {
            gBattlerControllerFuncs[0] = SetControllerToPokedude;
            gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[1] = SetControllerToPokedude;
            gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
            gBattlersCount = 2;
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
                gBattlerControllerFuncs[0] = SetControllerToSafari;
            else if (gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_FIRST_BATTLE))
                gBattlerControllerFuncs[0] = SetControllerToOakOrOldMan;
            else
                gBattlerControllerFuncs[0] = SetControllerToPlayer;
            gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[1] = SetControllerToOpponent;
            gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
            gBattlersCount = 2;
        }
    }
    else
    {
        gBattleMainFunc = BeginBattleIntro;
        if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
        {
            gBattlerControllerFuncs[0] = SetControllerToPokedude;
            gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[1] = SetControllerToPokedude;
            gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
            gBattlerControllerFuncs[2] = SetControllerToPokedude;
            gBattlerPositions[2] = B_POSITION_PLAYER_RIGHT;
            gBattlerControllerFuncs[3] = SetControllerToPokedude;
            gBattlerPositions[3] = B_POSITION_OPPONENT_RIGHT;
            gBattlersCount = MAX_BATTLERS_COUNT;
        }
        else
        {
            gBattlerControllerFuncs[0] = SetControllerToPlayer;
            gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[1] = SetControllerToOpponent;
            gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
            gBattlerControllerFuncs[2] = SetControllerToPlayer;
            gBattlerPositions[2] = B_POSITION_PLAYER_RIGHT;
            gBattlerControllerFuncs[3] = SetControllerToOpponent;
            gBattlerPositions[3] = B_POSITION_OPPONENT_RIGHT;
            gBattlersCount = MAX_BATTLERS_COUNT;
        }
    }
}

static void InitLinkBtlControllers(void)
{
    s32 i;
    u8 multiplayerId;

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        if (gBattleTypeFlags & BATTLE_TYPE_IS_MASTER)
        {
            gBattleMainFunc = BeginBattleIntro;
            gBattlerControllerFuncs[0] = SetControllerToPlayer;
            gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[1] = SetControllerToLinkOpponent;
            gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
            gBattlersCount = 2;
        }
        else
        {
            gBattlerControllerFuncs[1] = SetControllerToPlayer;
            gBattlerPositions[1] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[0] = SetControllerToLinkOpponent;
            gBattlerPositions[0] = B_POSITION_OPPONENT_LEFT;
            gBattlersCount = 2;
        }
    }
    else if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI) && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        if (gBattleTypeFlags & BATTLE_TYPE_IS_MASTER)
        {
            gBattleMainFunc = BeginBattleIntro;
            gBattlerControllerFuncs[0] = SetControllerToPlayer;
            gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[1] = SetControllerToLinkOpponent;
            gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
            gBattlerControllerFuncs[2] = SetControllerToPlayer;
            gBattlerPositions[2] = B_POSITION_PLAYER_RIGHT;
            gBattlerControllerFuncs[3] = SetControllerToLinkOpponent;
            gBattlerPositions[3] = B_POSITION_OPPONENT_RIGHT;
            gBattlersCount = MAX_BATTLERS_COUNT;
        }
        else
        {
            gBattlerControllerFuncs[1] = SetControllerToPlayer;
            gBattlerPositions[1] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[0] = SetControllerToLinkOpponent;
            gBattlerPositions[0] = B_POSITION_OPPONENT_LEFT;
            gBattlerControllerFuncs[3] = SetControllerToPlayer;
            gBattlerPositions[3] = B_POSITION_PLAYER_RIGHT;
            gBattlerControllerFuncs[2] = SetControllerToLinkOpponent;
            gBattlerPositions[2] = B_POSITION_OPPONENT_RIGHT;
            gBattlersCount = MAX_BATTLERS_COUNT;
        }
    }
    else
    {
        multiplayerId = GetMultiplayerId();
        if (gBattleTypeFlags & BATTLE_TYPE_IS_MASTER)
            gBattleMainFunc = BeginBattleIntro;
        for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
        {
            switch (gLinkPlayers[i].id)
            {
            case 0:
            case 3:
                BufferBattlePartyCurrentOrderBySide(gLinkPlayers[i].id, 0);
                break;
            case 1:
            case 2:
                BufferBattlePartyCurrentOrderBySide(gLinkPlayers[i].id, 1);
                break;
            }
            if (i == multiplayerId)
            {
                gBattlerControllerFuncs[gLinkPlayers[i].id] = SetControllerToPlayer;
                switch (gLinkPlayers[i].id)
                {
                case 0:
                case 3:
                    gBattlerPositions[gLinkPlayers[i].id] = B_POSITION_PLAYER_LEFT;
                    gBattlerPartyIndexes[gLinkPlayers[i].id] = B_POSITION_PLAYER_LEFT;
                    break;
                case 1:
                case 2:
                    gBattlerPositions[gLinkPlayers[i].id] = B_POSITION_PLAYER_RIGHT;
                    gBattlerPartyIndexes[gLinkPlayers[i].id] = 3;
                    break;
                }
            }
            else
            {
                if ((!(gLinkPlayers[i].id & 1) && !(gLinkPlayers[multiplayerId].id & 1)) || ((gLinkPlayers[i].id & 1) && (gLinkPlayers[multiplayerId].id & 1)))
                {
                    gBattlerControllerFuncs[gLinkPlayers[i].id] = SetControllerToLinkPartner;
                    switch (gLinkPlayers[i].id)
                    {
                    case 0:
                    case 3:
                        gBattlerPositions[gLinkPlayers[i].id] = B_POSITION_PLAYER_LEFT;
                        gBattlerPartyIndexes[gLinkPlayers[i].id] = B_POSITION_PLAYER_LEFT;
                        break;
                    case 1:
                    case 2:
                        gBattlerPositions[gLinkPlayers[i].id] = B_POSITION_PLAYER_RIGHT;
                        gBattlerPartyIndexes[gLinkPlayers[i].id] = B_POSITION_OPPONENT_RIGHT;
                        break;
                    }
                }
                else
                {
                    gBattlerControllerFuncs[gLinkPlayers[i].id] = SetControllerToLinkOpponent;
                    switch (gLinkPlayers[i].id)
                    {
                    case 0:
                    case 3:
                        gBattlerPositions[gLinkPlayers[i].id] = B_POSITION_OPPONENT_LEFT;
                        gBattlerPartyIndexes[gLinkPlayers[i].id] = B_POSITION_PLAYER_LEFT;
                        break;
                    case 1:
                    case 2:
                        gBattlerPositions[gLinkPlayers[i].id] = B_POSITION_OPPONENT_RIGHT;
                        gBattlerPartyIndexes[gLinkPlayers[i].id] = B_POSITION_OPPONENT_RIGHT;
                        break;
                    }
                }
            }
        }
        gBattlersCount = MAX_BATTLERS_COUNT;
    }
}

static void SetBattlePartyIds(void)
{
    s32 i, j;

    if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
    {
        for (i = 0; i < gBattlersCount; ++i)
        {
            for (j = 0; j < PARTY_SIZE; ++j)
            {
                if (i < 2)
                {
                    if (GET_BATTLER_SIDE2(i) == B_SIDE_PLAYER)
                    {
                        if (GetMonData(&gPlayerParty[j], MON_DATA_HP) != 0
                         && GetMonData(&gPlayerParty[j], MON_DATA_SPECIES2) != SPECIES_NONE
                         && GetMonData(&gPlayerParty[j], MON_DATA_SPECIES2) != SPECIES_EGG
                         && !GetMonData(&gPlayerParty[j], MON_DATA_IS_EGG))
                        {
                            gBattlerPartyIndexes[i] = j;
                            break;
                        }
                    }
                    else
                    {
                        if (GetMonData(&gEnemyParty[j], MON_DATA_HP) != 0
                         && GetMonData(&gEnemyParty[j], MON_DATA_SPECIES2) != SPECIES_NONE
                         && GetMonData(&gEnemyParty[j], MON_DATA_SPECIES2) != SPECIES_EGG
                         && !GetMonData(&gEnemyParty[j], MON_DATA_IS_EGG))
                        {
                            gBattlerPartyIndexes[i] = j;
                            break;
                        }
                    }
                }
                else
                {
                    if (GET_BATTLER_SIDE2(i) == B_SIDE_PLAYER)
                    {
                        if (GetMonData(&gPlayerParty[j], MON_DATA_HP) != 0
                         && GetMonData(&gPlayerParty[j], MON_DATA_SPECIES) != SPECIES_NONE  // Probably a typo by Game Freak. The rest use SPECIES2.
                         && GetMonData(&gPlayerParty[j], MON_DATA_SPECIES2) != SPECIES_EGG
                         && !GetMonData(&gPlayerParty[j], MON_DATA_IS_EGG)
                         && gBattlerPartyIndexes[i - 2] != j)
                        {
                            gBattlerPartyIndexes[i] = j;
                            break;
                        }
                    }
                    else
                    {
                        if (GetMonData(&gEnemyParty[j], MON_DATA_HP) != 0
                         && GetMonData(&gEnemyParty[j], MON_DATA_SPECIES2) != SPECIES_NONE
                         && GetMonData(&gEnemyParty[j], MON_DATA_SPECIES2) != SPECIES_EGG
                         && !GetMonData(&gEnemyParty[j], MON_DATA_IS_EGG)
                         && gBattlerPartyIndexes[i - 2] != j)
                        {
                            gBattlerPartyIndexes[i] = j;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void PrepareBufferDataTransfer(u8 bufferId, u8 *data, u16 size)
{
    s32 i;

    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        PrepareBufferDataTransferLink(bufferId, size, data);
    }
    else
    {
        switch (bufferId)
        {
        case 0:
            for (i = 0; i < size; ++data, ++i)
                gBattleBufferA[gActiveBattler][i] = *data;
            break;
        case 1:
            for (i = 0; i < size; ++data, ++i)
                gBattleBufferB[gActiveBattler][i] = *data;
            break;
        }
    }
}

static void CreateTasksForSendRecvLinkBuffers(void)
{
    sLinkSendTaskId = CreateTask(Task_HandleSendLinkBuffersData, 0);
    gTasks[sLinkSendTaskId].data[11] = 0;
    gTasks[sLinkSendTaskId].data[12] = 0;
    gTasks[sLinkSendTaskId].data[13] = 0;
    gTasks[sLinkSendTaskId].data[14] = 0;
    gTasks[sLinkSendTaskId].data[15] = 0;
    sLinkReceiveTaskId = CreateTask(Task_HandleCopyReceivedLinkBuffersData, 0);
    gTasks[sLinkReceiveTaskId].data[12] = 0;
    gTasks[sLinkReceiveTaskId].data[13] = 0;
    gTasks[sLinkReceiveTaskId].data[14] = 0;
    gTasks[sLinkReceiveTaskId].data[15] = 0;
    gUnknown_202286E = 0;
}

enum
{
    LINK_BUFF_BUFFER_ID,
    LINK_BUFF_ACTIVE_BATTLER,
    LINK_BUFF_ATTACKER,
    LINK_BUFF_TARGET,
    LINK_BUFF_SIZE_LO,
    LINK_BUFF_SIZE_HI,
    LINK_BUFF_ABSENT_BATTLER_FLAGS,
    LINK_BUFF_EFFECT_BATTLER,
    LINK_BUFF_DATA
};

void PrepareBufferDataTransferLink(u8 bufferId, u16 size, u8 *data)
{
    s32 alignedSize, i;

    alignedSize = size - size % 4 + 4;
    if (gTasks[sLinkSendTaskId].data[14] + alignedSize + LINK_BUFF_DATA + 1 > BATTLE_BUFFER_LINK_SIZE)
    {
        gTasks[sLinkSendTaskId].data[12] = gTasks[sLinkSendTaskId].data[14];
        gTasks[sLinkSendTaskId].data[14] = 0;
    }
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_BUFFER_ID] = bufferId;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_ACTIVE_BATTLER] = gActiveBattler;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_ATTACKER] = gBattlerAttacker;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_TARGET] = gBattlerTarget;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_SIZE_LO] = alignedSize;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_SIZE_HI] = (alignedSize & 0x0000FF00) >> 8;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_ABSENT_BATTLER_FLAGS] = gAbsentBattlerFlags;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_EFFECT_BATTLER] = gEffectBattler;
    for (i = 0; i < size; ++i)
        gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_DATA + i] = data[i];
    gTasks[sLinkSendTaskId].data[14] = gTasks[sLinkSendTaskId].data[14] + alignedSize + LINK_BUFF_DATA;
}

static void Task_HandleSendLinkBuffersData(u8 taskId)
{
    u16 blockSize;
    s32 playerCount;

    switch (gTasks[taskId].data[11])
    {
    case 0:
        gTasks[taskId].data[10] = 100;
        ++gTasks[taskId].data[11];
        break;
    case 1:
        if (!--gTasks[taskId].data[10])
        {
            ++gTasks[taskId].data[11];
            if (gReceivedRemoteLinkPlayers)
                gTasks[taskId].data[11] = 3;
        }
        break;
    case 2:
        playerCount = GetLinkPlayerCount_2();
        if ((gBattleTypeFlags & BATTLE_TYPE_MULTI && playerCount > 3) || (!(gBattleTypeFlags & BATTLE_TYPE_MULTI) && playerCount > 1))
        {
            if (IsLinkMaster())
            {
                CheckShouldAdvanceLinkState();
                ++gTasks[taskId].data[11];
            }
            else
            {
                ++gTasks[taskId].data[11];
            }
        }
        break;
    case 3:
        if (gTasks[taskId].data[15] != gTasks[taskId].data[14])
        {
            if (!gTasks[taskId].data[13])
            {
                if (gTasks[taskId].data[15] > gTasks[taskId].data[14]
                 && gTasks[taskId].data[15] == gTasks[taskId].data[12])
                {
                    gTasks[taskId].data[12] = 0;
                    gTasks[taskId].data[15] = 0;
                }
                blockSize = (gLinkBattleSendBuffer[gTasks[taskId].data[15] + LINK_BUFF_SIZE_LO] | (gLinkBattleSendBuffer[gTasks[taskId].data[15] + LINK_BUFF_SIZE_HI] << 8)) + LINK_BUFF_DATA;
                SendBlock(bitmask_all_link_players_but_self(), &gLinkBattleSendBuffer[gTasks[taskId].data[15]], blockSize);
                ++gTasks[taskId].data[11];
            }
            else
            {
                --gTasks[taskId].data[13];
                break;
            }
        }
        break;
    case 4:
        if (IsLinkTaskFinished())
        {
            blockSize = gLinkBattleSendBuffer[gTasks[taskId].data[15] + LINK_BUFF_SIZE_LO] | (gLinkBattleSendBuffer[gTasks[taskId].data[15] + LINK_BUFF_SIZE_HI] << 8);
            gTasks[taskId].data[13] = 5;
            gTasks[taskId].data[15] = gTasks[taskId].data[15] + blockSize + LINK_BUFF_DATA;
            gTasks[taskId].data[11] = 3;
        }
        break;
    case 5:
        if (!--gTasks[taskId].data[13])
        {
            gTasks[taskId].data[13] = 5;
            gTasks[taskId].data[11] = 3;
        }
        break;
    }
}

void TryReceiveLinkBattleData(void)
{
    u8 i;
    s32 j;
    u8 *recvBuffer;

    if (gReceivedRemoteLinkPlayers && (gBattleTypeFlags & BATTLE_TYPE_LINK_ESTABLISHED) && (gLinkPlayers[0].linkType == 0x2211))
    {
        LinkRfu_DestroyIdleTask();
        for (i = 0; i < GetLinkPlayerCount(); ++i)
        {
            if (GetBlockReceivedStatus() & gBitTable[i])
            {
                ResetBlockReceivedFlag(i);
                recvBuffer = (u8 *)gBlockRecvBuffer[i];
                {
                    u8 *dest, *src;
                    u16 r6 = gBlockRecvBuffer[i][2];

                    if (gTasks[sLinkReceiveTaskId].data[14] + 9 + r6 > 0x1000)
                    {
                        gTasks[sLinkReceiveTaskId].data[12] = gTasks[sLinkReceiveTaskId].data[14];
                        gTasks[sLinkReceiveTaskId].data[14] = 0;
                    }
                    dest = &gLinkBattleRecvBuffer[gTasks[sLinkReceiveTaskId].data[14]];
                    src = recvBuffer;
                    for (j = 0; j < r6 + 8; ++j)
                        dest[j] = src[j];
                    gTasks[sLinkReceiveTaskId].data[14] = gTasks[sLinkReceiveTaskId].data[14] + r6 + 8;
                }
            }
        }
    }
}

static void Task_HandleCopyReceivedLinkBuffersData(u8 taskId)
{
    u16 blockSize;
    u8 battlerId;
    u8 var;

    if (gTasks[taskId].data[15] != gTasks[taskId].data[14])
    {
        if (gTasks[taskId].data[15] > gTasks[taskId].data[14]
         && gTasks[taskId].data[15] == gTasks[taskId].data[12])
        {
            gTasks[taskId].data[12] = 0;
            gTasks[taskId].data[15] = 0;
        }
        battlerId = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_ACTIVE_BATTLER];
        blockSize = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_SIZE_LO] | (gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_SIZE_HI] << 8);
        switch (gLinkBattleRecvBuffer[gTasks[taskId].data[15] + 0])
        {
        case 0:
            if (gBattleControllerExecFlags & gBitTable[battlerId])
                return;
            memcpy(gBattleBufferA[battlerId], &gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_DATA], blockSize);
            MarkBattlerReceivedLinkData(battlerId);
            if (!(gBattleTypeFlags & BATTLE_TYPE_IS_MASTER))
            {
                gBattlerAttacker = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_ATTACKER];
                gBattlerTarget = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_TARGET];
                gAbsentBattlerFlags = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_ABSENT_BATTLER_FLAGS];
                gEffectBattler = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_EFFECT_BATTLER];
            }
            break;
        case 1:
            memcpy(gBattleBufferB[battlerId], &gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_DATA], blockSize);
            break;
        case 2:
            var = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_DATA];
            gBattleControllerExecFlags &= ~(gBitTable[battlerId] << (var * 4));
            break;
        }
        gTasks[taskId].data[15] = gTasks[taskId].data[15] + blockSize + LINK_BUFF_DATA;
    }
}

void BtlController_EmitGetMonData(u8 bufferId, u8 requestId, u8 monToCheck)
{
    sBattleBuffersTransferData[0] = CONTROLLER_GETMONDATA;
    sBattleBuffersTransferData[1] = requestId;
    sBattleBuffersTransferData[2] = monToCheck;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

// not used
static void BtlController_EmitGetRawMonData(u8 bufferId, u8 monId, u8 bytes)
{
    sBattleBuffersTransferData[0] = CONTROLLER_GETRAWMONDATA;
    sBattleBuffersTransferData[1] = monId;
    sBattleBuffersTransferData[2] = bytes;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitSetMonData(u8 bufferId, u8 requestId, u8 monToCheck, u8 bytes, void *data)
{
    s32 i;

    sBattleBuffersTransferData[0] = CONTROLLER_SETMONDATA;
    sBattleBuffersTransferData[1] = requestId;
    sBattleBuffersTransferData[2] = monToCheck;
    for (i = 0; i < bytes; ++i)
        sBattleBuffersTransferData[3 + i] = *(u8 *)(data++);
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 3 + bytes);
}

void BtlController_EmitSetRawMonData(u8 bufferId, u8 monId, u8 bytes, void *data)
{
    s32 i;

    sBattleBuffersTransferData[0] = CONTROLLER_SETRAWMONDATA;
    sBattleBuffersTransferData[1] = monId;
    sBattleBuffersTransferData[2] = bytes;
    for (i = 0; i < bytes; ++i)
        sBattleBuffersTransferData[3 + i] = *(u8 *)(data++);
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, bytes + 3);
}

void BtlController_EmitLoadMonSprite(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_LOADMONSPRITE;
    sBattleBuffersTransferData[1] = CONTROLLER_LOADMONSPRITE;
    sBattleBuffersTransferData[2] = CONTROLLER_LOADMONSPRITE;
    sBattleBuffersTransferData[3] = CONTROLLER_LOADMONSPRITE;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitSwitchInAnim(u8 bufferId, u8 partyId, bool8 dontClearSubstituteBit)
{
    sBattleBuffersTransferData[0] = CONTROLLER_SWITCHINANIM;
    sBattleBuffersTransferData[1] = partyId;
    sBattleBuffersTransferData[2] = dontClearSubstituteBit;
    sBattleBuffersTransferData[3] = 5;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitReturnMonToBall(u8 bufferId, u8 arg1)
{
    sBattleBuffersTransferData[0] = CONTROLLER_RETURNMONTOBALL;
    sBattleBuffersTransferData[1] = arg1;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitDrawTrainerPic(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_DRAWTRAINERPIC;
    sBattleBuffersTransferData[1] = CONTROLLER_DRAWTRAINERPIC;
    sBattleBuffersTransferData[2] = CONTROLLER_DRAWTRAINERPIC;
    sBattleBuffersTransferData[3] = CONTROLLER_DRAWTRAINERPIC;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitTrainerSlide(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_TRAINERSLIDE;
    sBattleBuffersTransferData[1] = CONTROLLER_TRAINERSLIDE;
    sBattleBuffersTransferData[2] = CONTROLLER_TRAINERSLIDE;
    sBattleBuffersTransferData[3] = CONTROLLER_TRAINERSLIDE;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitTrainerSlideBack(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_TRAINERSLIDEBACK;
    sBattleBuffersTransferData[1] = CONTROLLER_TRAINERSLIDEBACK;
    sBattleBuffersTransferData[2] = CONTROLLER_TRAINERSLIDEBACK;
    sBattleBuffersTransferData[3] = CONTROLLER_TRAINERSLIDEBACK;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitFaintAnimation(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_FAINTANIMATION;
    sBattleBuffersTransferData[1] = CONTROLLER_FAINTANIMATION;
    sBattleBuffersTransferData[2] = CONTROLLER_FAINTANIMATION;
    sBattleBuffersTransferData[3] = CONTROLLER_FAINTANIMATION;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

// not used
static void BtlController_EmitPaletteFade(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_PALETTEFADE;
    sBattleBuffersTransferData[1] = CONTROLLER_PALETTEFADE;
    sBattleBuffersTransferData[2] = CONTROLLER_PALETTEFADE;
    sBattleBuffersTransferData[3] = CONTROLLER_PALETTEFADE;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

// not used
static void BtlController_EmitSuccessBallThrowAnim(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_SUCCESSBALLTHROWANIM;
    sBattleBuffersTransferData[1] = CONTROLLER_SUCCESSBALLTHROWANIM;
    sBattleBuffersTransferData[2] = CONTROLLER_SUCCESSBALLTHROWANIM;
    sBattleBuffersTransferData[3] = CONTROLLER_SUCCESSBALLTHROWANIM;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitBallThrowAnim(u8 bufferId, u8 caseId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_BALLTHROWANIM;
    sBattleBuffersTransferData[1] = caseId;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 2);
}

// not used
static void BtlController_EmitPause(u8 bufferId, u8 toWait, void *data)
{
    s32 i;

    sBattleBuffersTransferData[0] = CONTROLLER_PAUSE;
    sBattleBuffersTransferData[1] = toWait;
    for (i = 0; i < toWait * 3; ++i)
        sBattleBuffersTransferData[2 + i] = *(u8 *)(data++);
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, toWait * 3 + 2);
}

void BtlController_EmitMoveAnimation(u8 bufferId, u16 move, u8 turnOfMove, u16 movePower, s32 dmg, u8 friendship, struct DisableStruct *disableStructPtr, u8 hit)
{
    sBattleBuffersTransferData[0] = CONTROLLER_MOVEANIMATION;
    sBattleBuffersTransferData[1] = move;
    sBattleBuffersTransferData[2] = (move & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = turnOfMove;
    sBattleBuffersTransferData[4] = movePower;
    sBattleBuffersTransferData[5] = (movePower & 0xFF00) >> 8;
    sBattleBuffersTransferData[6] = dmg;
    sBattleBuffersTransferData[7] = (dmg & 0x0000FF00) >> 8;
    sBattleBuffersTransferData[8] = (dmg & 0x00FF0000) >> 16;
    sBattleBuffersTransferData[9] = (dmg & 0xFF000000) >> 24;
    sBattleBuffersTransferData[10] = friendship;
    sBattleBuffersTransferData[11] = hit; // multihit in pokeem
    if (WEATHER_HAS_EFFECT2)
    {
        sBattleBuffersTransferData[12] = gBattleWeather;
        sBattleBuffersTransferData[13] = (gBattleWeather & 0xFF00) >> 8;
    }
    else
    {
        sBattleBuffersTransferData[12] = 0;
        sBattleBuffersTransferData[13] = 0;
    }
    sBattleBuffersTransferData[14] = 0;
    sBattleBuffersTransferData[15] = 0;
    memcpy(&sBattleBuffersTransferData[16], disableStructPtr, sizeof(struct DisableStruct));
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 16 + sizeof(struct DisableStruct));
}

static u8* StringCopyBattleStringLoader(u8 *dest, const u8 *src)
{
    s32 i;
    s32 limit = 0x96;

    for (i = 0; i < limit; i++)
    {
        dest[i] = src[i];

        if (dest[i] == 0xFF)
            return &dest[i];
    }

    dest[i] = 0xFF;
    return &dest[i];
}

void BtlController_EmitPrintString(u8 bufferId, u16 stringID)
{
    s32 i;
    struct BattleMsgData *stringInfo;

    sBattleBuffersTransferData[0] = CONTROLLER_PRINTSTRING;
    sBattleBuffersTransferData[1] = gBattleOutcome;
    sBattleBuffersTransferData[2] = stringID;
    sBattleBuffersTransferData[3] = (stringID & 0xFF00) >> 8;

    stringInfo = (struct BattleMsgData *)(&sBattleBuffersTransferData[4]);
    stringInfo->currentMove = gCurrentMove;
    stringInfo->originallyUsedMove = gChosenMove;
    stringInfo->lastItem = gLastUsedItem;
    stringInfo->lastAbility = gLastUsedAbility;
    stringInfo->scrActive = gBattleScripting.battler;
    stringInfo->bakScriptPartyIdx = gBattleStruct->scriptPartyIdx;
    stringInfo->hpScale = gBattleStruct->hpScale;
    stringInfo->itemEffectBattler = gPotentialItemEffectBattler;
    stringInfo->moveType = gBattleMoves[gCurrentMove].type;
    stringInfo->zMoveActive = gBattleStruct->zMoveData.active;
	stringInfo->dynamaxActive = gBattleStruct->dynamaxData.active;

    if (gBattleStringLoader !=  NULL)
		StringCopyBattleStringLoader(stringInfo->battleStringLoader, gBattleStringLoader);

    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
        stringInfo->abilities[i] = *GetAbilityLocation(i);

    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; ++i)
    {
        stringInfo->textBuffs[0][i] = gBattleTextBuff1[i];
        stringInfo->textBuffs[1][i] = gBattleTextBuff2[i];
        stringInfo->textBuffs[2][i] = gBattleTextBuff3[i];
    }
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, sizeof(struct BattleMsgData) + 4);
}

void BtlController_EmitPrintSelectionString(u8 bufferId, u16 stringID)
{
    s32 i;
    struct BattleMsgData *stringInfo;

    sBattleBuffersTransferData[0] = CONTROLLER_PRINTSTRINGPLAYERONLY;
    sBattleBuffersTransferData[1] = CONTROLLER_PRINTSTRINGPLAYERONLY;
    sBattleBuffersTransferData[2] = stringID;
    sBattleBuffersTransferData[3] = (stringID & 0xFF00) >> 8;
    stringInfo = (struct BattleMsgData *)(&sBattleBuffersTransferData[4]);
    stringInfo->currentMove = gCurrentMove;
    stringInfo->originallyUsedMove = gChosenMove;
    stringInfo->lastItem = gLastUsedItem;
    stringInfo->lastAbility = gLastUsedAbility;
    stringInfo->scrActive = gBattleScripting.battler;
    stringInfo->bakScriptPartyIdx = gBattleStruct->scriptPartyIdx;
    stringInfo->zMoveActive = gBattleStruct->zMoveData.active;
	stringInfo->dynamaxActive = gBattleStruct->dynamaxData.active;
	if (gBattleStringLoader !=  NULL)
		StringCopyBattleStringLoader(stringInfo->battleStringLoader, gBattleStringLoader);

    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
        stringInfo->abilities[i] = *GetAbilityLocation(i);
    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; ++i)
    {
        stringInfo->textBuffs[0][i] = gBattleTextBuff1[i];
        stringInfo->textBuffs[1][i] = gBattleTextBuff2[i];
        stringInfo->textBuffs[2][i] = gBattleTextBuff3[i];
    }
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, sizeof(struct BattleMsgData) + 4);
}

void BtlController_EmitChooseAction(u8 bufferId, u8 arg1, u16 arg2)
{
    sBattleBuffersTransferData[0] = CONTROLLER_CHOOSEACTION;
    sBattleBuffersTransferData[1] = arg1;
    sBattleBuffersTransferData[2] = arg2;
    sBattleBuffersTransferData[3] = (arg2 & 0xFF00) >> 8;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

// not used
static void BtlController_EmitUnknownYesNoBox(u8 bufferId, u32 arg1) // TODO: Does the function name make sense for pokefirered? 
{
    sBattleBuffersTransferData[0] = CONTROLLER_UNKNOWNYESNOBOX;
    sBattleBuffersTransferData[1] = arg1;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 2);
}

struct ChooseMoveStructOld
{
    u16 moves[4];		//0x0
    u8 currentPp[4];	//0x8
    u8 maxPp[4];		//0xC
    u16 species;		//0x10
    u8 monType1;		//0x12
    u8 monType2;		//0x13
};

const u8 gMoveEffectsThatIgnoreWeaknessResistance[] = 
{
    EFFECT_BIDE,
    EFFECT_SUPER_FANG,
    EFFECT_DRAGON_RAGE,
    EFFECT_SONICBOOM,
    EFFECT_LEVEL_DAMAGE,
    EFFECT_PSYWAVE,
    EFFECT_MEMENTO,
    EFFECT_ENDEAVOR,
    EFFECT_PAIN_SPLIT,
    EFFECT_COUNTER,
    EFFECT_MIRROR_COAT,
    MOVE_EFFECT_TABLES_TERMIN
};

void BtlController_EmitChooseMove(u8 bufferId, bool8 isDoubleBattle, bool8 NoPpNumber, struct ChooseMoveStruct *movePpData)
{
    u32 i, j;
    const struct Evolution* evolutions;

    struct ChooseMoveStruct* tempMoveStruct = AllocZeroed(sizeof(struct ChooseMoveStruct)); //Make space for new expanded data
	memcpy(tempMoveStruct, movePpData, sizeof(struct ChooseMoveStructOld)); //Copy the old data
	tempMoveStruct->monType1 = gBattleMons[gActiveBattler].type1;
	tempMoveStruct->monType2 = gBattleMons[gActiveBattler].type2;
	tempMoveStruct->monType3 = gBattleMons[gActiveBattler].type3;
	tempMoveStruct->ability = ABILITY(gActiveBattler);

    //Fix Transformed Move PP
	if (IS_TRANSFORMED(gActiveBattler))
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (tempMoveStruct->maxPp[i] > 5)
				tempMoveStruct->maxPp[i] = 5;
		}
	}

	tempMoveStruct->dynamaxed = IsDynamaxed(gActiveBattler);
	tempMoveStruct->dynamaxDone = gBattleStruct->dynamaxData.used[gActiveBattler];
	if ((!gBattleStruct->dynamaxData.used[gActiveBattler] || IsDynamaxed(gActiveBattler))
	&& DynamaxEnabled(gActiveBattler)
	&& MonCanDynamax(GetBankPartyData(gActiveBattler))
	&& !BATTLER_SEMI_INVULNERABLE(gActiveBattler))
	{
		for (i = 0; i < MAX_MON_MOVES; ++i)
			tempMoveStruct->possibleMaxMoves[i] = GetMaxMove(gActiveBattler, i);
	}

    gBattleScripting.dmgMultiplier = 1;

    for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u8 foe = (IS_DOUBLE_BATTLE && !BATTLER_ALIVE(FOE(gActiveBattler))) ? PARTNER(FOE(gActiveBattler)) : FOE(gActiveBattler);
		u16 originalMove = gBattleMons[gActiveBattler].moves[i];
		u16 move = (tempMoveStruct->dynamaxed) ? tempMoveStruct->possibleMaxMoves[i] : originalMove;

		tempMoveStruct->moveTypes[i] = GetMoveTypeSpecial(gActiveBattler, move);

		if (IS_DOUBLE_BATTLE && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, gActiveBattler, foe) >= 2) //Because target can vary, display only attacker's modifiers
		{
			tempMoveStruct->movePowers[i] = CalcVisualBasePower(gActiveBattler, gActiveBattler, move, TRUE);
			tempMoveStruct->moveAcc[i] = VisualAccuracyCalc_NoTarget(move, gActiveBattler);

			if (tempMoveStruct->possibleMaxMoves[i] != MOVE_NONE)
			{
				gBattleStruct->ai.zMoveHelper = originalMove;
				tempMoveStruct->maxMovePowers[i] = CalcVisualBasePower(gActiveBattler, gActiveBattler, tempMoveStruct->possibleMaxMoves[i], TRUE);
				gBattleStruct->ai.zMoveHelper = MOVE_NONE;
			}

			for (j = 0; j < gBattlersCount; ++j)
			{
				if (SPLIT(move) != SPLIT_STATUS)
				{
					u8 moveResult;

					if (j == gActiveBattler || j == PARTNER(gActiveBattler))
					{
						tempMoveStruct->moveResults[GetBattlerPosition(j)][i] = 0;
						continue;
					}

					moveResult = VisualTypeCalc(move, gActiveBattler, j);

					if (!(moveResult & MOVE_RESULT_NO_EFFECT)
					&& (CheckTableForMoveEffect(move, gMoveEffectsThatIgnoreWeaknessResistance) || gBattleMoves[move].effect == EFFECT_OHKO))
						moveResult = 0; //These moves can have no effect, but are neither super nor not very effective
					tempMoveStruct->moveResults[GetBattlerPosition(j)][i] = moveResult;
				}
				else
					tempMoveStruct->moveResults[GetBattlerPosition(j)][i] = 0;
			}
		}
		else //Single Battle or single target
		{
			tempMoveStruct->movePowers[i] = CalcVisualBasePower(gActiveBattler, foe, move, FALSE);
			tempMoveStruct->moveAcc[i] = VisualAccuracyCalc(move, gActiveBattler, foe);

			if (tempMoveStruct->possibleMaxMoves[i] != MOVE_NONE)
			{
				gBattleStruct->ai.zMoveHelper = originalMove;
				tempMoveStruct->maxMovePowers[i] = CalcVisualBasePower(gActiveBattler, foe, tempMoveStruct->possibleMaxMoves[i], FALSE);
				gBattleStruct->ai.zMoveHelper = MOVE_NONE;
			}

			if (SPLIT(move) != SPLIT_STATUS)
			{
				u8 moveResult = VisualTypeCalc(move, gActiveBattler, foe);

				if (!(moveResult & MOVE_RESULT_NO_EFFECT)
				&& (CheckTableForMoveEffect(move, gMoveEffectsThatIgnoreWeaknessResistance) || gBattleMoves[move].effect == EFFECT_OHKO))
					moveResult = 0; //These moves can have no effect, but are neither super nor not very effective

				tempMoveStruct->moveResults[GetBattlerPosition(foe)][i] = moveResult;
			}
			else
				tempMoveStruct->moveResults[GetBattlerPosition(foe)][i] = 0;
		}
	}

	tempMoveStruct->megaDone = gBattleStruct->megaData.done[gActiveBattler];
	tempMoveStruct->ultraDone = gBattleStruct->ultraData.done[gActiveBattler];
	if (!IS_TRANSFORMED(gActiveBattler) && !tempMoveStruct->dynamaxed)
	{
		if (!gBattleStruct->megaData.done[gActiveBattler])
		{
			evolutions = CanMegaEvolve(gActiveBattler, FALSE);
			if (evolutions != NULL)
			{
				if (!BankMegaEvolved(gActiveBattler, FALSE)
				&& MegaEvolutionEnabled(gActiveBattler)
				&& !BATTLER_SEMI_INVULNERABLE(gActiveBattler) //No Mega Evolving while not on screen
				&& !DoesZMoveUsageStopMegaEvolution(gActiveBattler) //No Mega Evolving if you've used a Z-Move (*cough* *cough* Rayquaza)
				&& !DoesDynamaxUsageStopMegaEvolution(gActiveBattler))
				{
					tempMoveStruct->canMegaEvolve = TRUE;
					tempMoveStruct->megaVariance = evolutions->unknown;
				}
			}
		}

		if (!gBattleStruct->ultraData.done[gActiveBattler])
		{
			evolutions = CanMegaEvolve(gActiveBattler, TRUE); //Check Ultra Burst
			if (evolutions != NULL)
			{
				if (!BankMegaEvolved(gActiveBattler, TRUE) //Check Ultra Burst
				&& !BATTLER_SEMI_INVULNERABLE(gActiveBattler) //No Ultra Bursting while not on screen
				&& !DoesDynamaxUsageStopMegaEvolution(gActiveBattler))
				{
					tempMoveStruct->canMegaEvolve = TRUE;
					tempMoveStruct->megaVariance = evolutions->unknown;
				}
			}
		}
	}

	tempMoveStruct->zMoveUsed = gBattleStruct->zMoveData.used[gActiveBattler];
	tempMoveStruct->zPartyIndex = gBattleStruct->zMoveData.partyIndex[SIDE(gActiveBattler)];
	if (!gBattleStruct->zMoveData.used[gActiveBattler] && !IsMegaZMoveBannedBattle() && !IsMega(gActiveBattler) && !IsBluePrimal(gActiveBattler) && !IsRedPrimal(gActiveBattler))
	{
		u8 limitations = CheckMoveLimitations(gActiveBattler, 0, MOVE_LIMITATION_PP);

		for (i = 0; i < MAX_MON_MOVES; ++i)
		{
			if (!(limitations & gBitTable[i])) //Don't display a Z-Move if the base move has no PP
				tempMoveStruct->possibleZMoves[i] = CanUseZMove(gActiveBattler, i, 0);
		}
	}

	sBattleBuffersTransferData[0] = CONTROLLER_CHOOSEMOVE;
	sBattleBuffersTransferData[1] = isDoubleBattle;
	sBattleBuffersTransferData[2] = NoPpNumber;
	sBattleBuffersTransferData[3] = 0;
	for (i = 0; i < sizeof(*tempMoveStruct); ++i)
		sBattleBuffersTransferData[4 + i] = ((u8*)(tempMoveStruct))[i];
	PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, sizeof(*tempMoveStruct) + 4);

	Free(tempMoveStruct);
}

void BtlController_EmitChooseItem(u8 bufferId, u8 *arg1)
{
    s32 i;

    sBattleBuffersTransferData[0] = CONTROLLER_OPENBAG;
    for (i = 0; i < 3; ++i)
        sBattleBuffersTransferData[1 + i] = arg1[i];
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitChoosePokemon(u8 bufferId, u8 caseId, u8 arg2, u8 abilityId, u8 *arg4)
{
    s32 i;

    sBattleBuffersTransferData[0] = CONTROLLER_CHOOSEPOKEMON;
    sBattleBuffersTransferData[1] = caseId;
    sBattleBuffersTransferData[2] = arg2;
    sBattleBuffersTransferData[3] = abilityId;
    for (i = 0; i < 3; ++i)
        sBattleBuffersTransferData[4 + i] = arg4[i];
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 8);  // Only 7 bytes were written.
}

// not used
static void BtlController_EmitCmd23(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_23;
    sBattleBuffersTransferData[1] = CONTROLLER_23;
    sBattleBuffersTransferData[2] = CONTROLLER_23;
    sBattleBuffersTransferData[3] = CONTROLLER_23;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitHealthBarUpdate(u8 bufferId, u16 hpValue)
{
    sBattleBuffersTransferData[0] = CONTROLLER_HEALTHBARUPDATE;
    sBattleBuffersTransferData[1] = 0;
    sBattleBuffersTransferData[2] = (s16)hpValue;
    sBattleBuffersTransferData[3] = ((s16)hpValue & 0xFF00) >> 8;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitExpUpdate(u8 bufferId, u8 partyId, u16 expPoints)
{
    sBattleBuffersTransferData[0] = CONTROLLER_EXPUPDATE;
    sBattleBuffersTransferData[1] = partyId;
    sBattleBuffersTransferData[2] = (s16)expPoints;
    sBattleBuffersTransferData[3] = ((s16)expPoints & 0xFF00) >> 8;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitStatusIconUpdate(u8 bufferId, u32 status1, u32 status2)
{
    sBattleBuffersTransferData[0] = CONTROLLER_STATUSICONUPDATE;
    sBattleBuffersTransferData[1] = status1;
    sBattleBuffersTransferData[2] = (status1 & 0x0000FF00) >> 8;
    sBattleBuffersTransferData[3] = (status1 & 0x00FF0000) >> 16;
    sBattleBuffersTransferData[4] = (status1 & 0xFF000000) >> 24;
    sBattleBuffersTransferData[5] = status2;
    sBattleBuffersTransferData[6] = (status2 & 0x0000FF00) >> 8;
    sBattleBuffersTransferData[7] = (status2 & 0x00FF0000) >> 16;
    sBattleBuffersTransferData[8] = (status2 & 0xFF000000) >> 24;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 9);
}

void BtlController_EmitStatusAnimation(u8 bufferId, bool8 status2, u32 status)
{
    sBattleBuffersTransferData[0] = CONTROLLER_STATUSANIMATION;
    sBattleBuffersTransferData[1] = status2;
    sBattleBuffersTransferData[2] = status;
    sBattleBuffersTransferData[3] = (status & 0x0000FF00) >> 8;
    sBattleBuffersTransferData[4] = (status & 0x00FF0000) >> 16;
    sBattleBuffersTransferData[5] = (status & 0xFF000000) >> 24;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 6);
}

// not used
static void BtlController_EmitStatusXor(u8 bufferId, u8 b)
{
    sBattleBuffersTransferData[0] = CONTROLLER_STATUSXOR;
    sBattleBuffersTransferData[1] = b;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitDataTransfer(u8 bufferId, u16 size, void *data)
{
    s32 i;

    sBattleBuffersTransferData[0] = CONTROLLER_DATATRANSFER;
    sBattleBuffersTransferData[1] = CONTROLLER_DATATRANSFER;
    sBattleBuffersTransferData[2] = size;
    sBattleBuffersTransferData[3] = (size & 0xFF00) >> 8;
    for (i = 0; i < size; ++i)
        sBattleBuffersTransferData[4 + i] = *(u8 *)(data++);
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, size + 4);
}

// not used
static void BtlController_EmitDMA3Transfer(u8 bufferId, void *dst, u16 size, void *data)
{
    s32 i;

    sBattleBuffersTransferData[0] = CONTROLLER_DMA3TRANSFER;
    sBattleBuffersTransferData[1] = (u32)(dst);
    sBattleBuffersTransferData[2] = ((u32)(dst) & 0x0000FF00) >> 8;
    sBattleBuffersTransferData[3] = ((u32)(dst) & 0x00FF0000) >> 16;
    sBattleBuffersTransferData[4] = ((u32)(dst) & 0xFF000000) >> 24;
    sBattleBuffersTransferData[5] = size;
    sBattleBuffersTransferData[6] = (size & 0xFF00) >> 8;
    for (i = 0; i < size; ++i)
        sBattleBuffersTransferData[7 + i] = *(u8 *)(data++);
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, size + 7);
}

// not used
static void BtlController_EmitPlayBGM(u8 bufferId, u16 songId, void *unusedDumbDataParameter)
{
    s32 i;

    sBattleBuffersTransferData[0] = CONTROLLER_31;
    sBattleBuffersTransferData[1] = songId;
    sBattleBuffersTransferData[2] = (songId & 0xFF00) >> 8;
    for (i = 0; i < songId; ++i)
        sBattleBuffersTransferData[3 + i] = *(u8 *)(unusedDumbDataParameter++);
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, songId + 3);
}

// not used
static void BtlController_EmitCmd32(u8 bufferId, u16 size, void *data)
{
    s32 i;

    sBattleBuffersTransferData[0] = CONTROLLER_32;
    sBattleBuffersTransferData[1] = size;
    sBattleBuffersTransferData[2] = (size & 0xFF00) >> 8;
    for (i = 0; i < size; ++i)
        sBattleBuffersTransferData[3 + i] = *(u8 *)(data++);
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, size + 3);
}

void BtlController_EmitTwoReturnValues(u8 bufferId, u8 arg1, u16 arg2)
{
    sBattleBuffersTransferData[0] = CONTROLLER_TWORETURNVALUES;
    sBattleBuffersTransferData[1] = arg1;
    sBattleBuffersTransferData[2] = arg2;
    sBattleBuffersTransferData[3] = (arg2 & 0xFF00) >> 8;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitChosenMonReturnValue(u8 bufferId, u8 b, u8 *c)
{
    s32 i;

    sBattleBuffersTransferData[0] = CONTROLLER_CHOSENMONRETURNVALUE;
    sBattleBuffersTransferData[1] = b;
    for (i = 0; i < 3; ++i)
        sBattleBuffersTransferData[2 + i] = c[i];
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 5);
}

void BtlController_EmitOneReturnValue(u8 bufferId, u16 arg1)
{
    sBattleBuffersTransferData[0] = CONTROLLER_ONERETURNVALUE;
    sBattleBuffersTransferData[1] = arg1;
    sBattleBuffersTransferData[2] = (arg1 & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitOneReturnValue_Duplicate(u8 bufferId, u16 b)
{
    sBattleBuffersTransferData[0] = CONTROLLER_ONERETURNVALUE_DUPLICATE;
    sBattleBuffersTransferData[1] = b;
    sBattleBuffersTransferData[2] = (b & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

// not used
static void BtlController_EmitCmd37(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_37;
    sBattleBuffersTransferData[1] = CONTROLLER_37;
    sBattleBuffersTransferData[2] = CONTROLLER_37;
    sBattleBuffersTransferData[3] = CONTROLLER_37;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

// not used
static void BtlController_EmitCmd38(u8 bufferId, u8 b)
{
    sBattleBuffersTransferData[0] = CONTROLLER_38;
    sBattleBuffersTransferData[1] = b;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 2);
}

// not used
static void BtlController_EmitCmd39(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_39;
    sBattleBuffersTransferData[1] = CONTROLLER_39;
    sBattleBuffersTransferData[2] = CONTROLLER_39;
    sBattleBuffersTransferData[3] = CONTROLLER_39;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

// not used
static void BtlController_EmitCmd40(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_40;
    sBattleBuffersTransferData[1] = CONTROLLER_40;
    sBattleBuffersTransferData[2] = CONTROLLER_40;
    sBattleBuffersTransferData[3] = CONTROLLER_40;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitHitAnimation(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_HITANIMATION;
    sBattleBuffersTransferData[1] = CONTROLLER_HITANIMATION;
    sBattleBuffersTransferData[2] = CONTROLLER_HITANIMATION;
    sBattleBuffersTransferData[3] = CONTROLLER_HITANIMATION;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitCmd42(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_42;
    sBattleBuffersTransferData[1] = CONTROLLER_42;
    sBattleBuffersTransferData[2] = CONTROLLER_42;
    sBattleBuffersTransferData[3] = CONTROLLER_42;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitPlaySE(u8 bufferId, u16 songId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_EFFECTIVENESSSOUND;
    sBattleBuffersTransferData[1] = songId;
    sBattleBuffersTransferData[2] = (songId & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitPlayFanfare(u8 bufferId, u16 songId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_PLAYFANFAREORBGM;
    sBattleBuffersTransferData[1] = songId;
    sBattleBuffersTransferData[2] = (songId & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitFaintingCry(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_FAINTINGCRY;
    sBattleBuffersTransferData[1] = CONTROLLER_FAINTINGCRY;
    sBattleBuffersTransferData[2] = CONTROLLER_FAINTINGCRY;
    sBattleBuffersTransferData[3] = CONTROLLER_FAINTINGCRY;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitIntroSlide(u8 bufferId, u8 terrainId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_INTROSLIDE;
    sBattleBuffersTransferData[1] = terrainId;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitIntroTrainerBallThrow(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_INTROTRAINERBALLTHROW;
    sBattleBuffersTransferData[1] = CONTROLLER_INTROTRAINERBALLTHROW;
    sBattleBuffersTransferData[2] = CONTROLLER_INTROTRAINERBALLTHROW;
    sBattleBuffersTransferData[3] = CONTROLLER_INTROTRAINERBALLTHROW;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitDrawPartyStatusSummary(u8 bufferId, struct HpAndStatus* hpAndStatus, u8 param)
{
    s32 i;

    sBattleBuffersTransferData[0] = CONTROLLER_DRAWPARTYSTATUSSUMMARY;
    sBattleBuffersTransferData[1] = param & 0x7F;
    sBattleBuffersTransferData[2] = (param & 0x80) >> 7;
    sBattleBuffersTransferData[3] = CONTROLLER_DRAWPARTYSTATUSSUMMARY;
    for (i = 0; i < (s32)(sizeof(struct HpAndStatus) * PARTY_SIZE); ++i)
        sBattleBuffersTransferData[4 + i] = *(i + (u8 *)(hpAndStatus));
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, sizeof(struct HpAndStatus) * PARTY_SIZE + 4);
}

void BtlController_EmitHidePartyStatusSummary(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_HIDEPARTYSTATUSSUMMARY;
    sBattleBuffersTransferData[1] = CONTROLLER_HIDEPARTYSTATUSSUMMARY;
    sBattleBuffersTransferData[2] = CONTROLLER_HIDEPARTYSTATUSSUMMARY;
    sBattleBuffersTransferData[3] = CONTROLLER_HIDEPARTYSTATUSSUMMARY;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitEndBounceEffect(u8 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_ENDBOUNCE;
    sBattleBuffersTransferData[1] = CONTROLLER_ENDBOUNCE;
    sBattleBuffersTransferData[2] = CONTROLLER_ENDBOUNCE;
    sBattleBuffersTransferData[3] = CONTROLLER_ENDBOUNCE;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitSpriteInvisibility(u8 bufferId, bool8 isInvisible)
{
    sBattleBuffersTransferData[0] = CONTROLLER_SPRITEINVISIBILITY;
    sBattleBuffersTransferData[1] = isInvisible;
    sBattleBuffersTransferData[2] = CONTROLLER_SPRITEINVISIBILITY;
    sBattleBuffersTransferData[3] = CONTROLLER_SPRITEINVISIBILITY;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitBattleAnimation(u8 bufferId, u8 animationId, u16 argument)
{
    sBattleBuffersTransferData[0] = CONTROLLER_BATTLEANIMATION;
    sBattleBuffersTransferData[1] = animationId;
    sBattleBuffersTransferData[2] = argument;
    sBattleBuffersTransferData[3] = (argument & 0xFF00) >> 8;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitLinkStandbyMsg(u8 bufferId, u8 arg1)
{
    sBattleBuffersTransferData[0] = CONTROLLER_LINKSTANDBYMSG;
    sBattleBuffersTransferData[1] = arg1;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitResetActionMoveSelection(u8 bufferId, u8 caseId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_RESETACTIONMOVESELECTION;
    sBattleBuffersTransferData[1] = caseId;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitCmd55(u8 bufferId, u8 battleOutcome)
{
    sBattleBuffersTransferData[0] = CONTROLLER_55;
    sBattleBuffersTransferData[1] = battleOutcome;
    PrepareBufferDataTransfer(bufferId, sBattleBuffersTransferData, 2);
}
