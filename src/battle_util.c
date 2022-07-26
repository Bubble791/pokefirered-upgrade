#include "global.h"
#include "item.h"
#include "text.h"
#include "util.h"
#include "link.h"
#include "berry.h"
#include "random.h"
#include "pokemon.h"
#include "string_util.h"
#include "field_weather.h"
#include "event_data.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "battle_message.h"
#include "constants/battle_anim.h"
#include "battle_controllers.h"
#include "battle_string_ids.h"
#include "battle_ai_script_commands.h"
#include "dynamax.h"
#include "set_z_effect.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/weather.h"
#include "constants/abilities.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/hold_effects.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_script_commands.h"

static u8 ConfusionBerries(u8 bank, u8 flavour, bool8 moveTurn, bool8 doPluck);
static u8 StatRaiseBerries(u8 bank, u8 stat, bool8 moveTurn, bool8 doPluck);
static u8 RaiseStatsContactItem(u8 bank, u8 stat, bool8 doPluck);
static u8 KeeMaranagaBerryFunc(u8 bank, u8 stat, u8 split, bool8 doPluck);

const u8 gStatusConditionString_MentalState[] = _("mental");
const u8 gStatusConditionString_TauntProblem[] = _("Taunt");
const u8 gStatusConditionString_DisableProblem[] = _("Disable");
const u8 gStatusConditionString_EncoreProblem[] = _("Encore");
const u8 gStatusConditionString_Confusion[] = _("Confusion");

const u8 gText_AirLockActivate[] = _("天气消失了!");
const u8 gText_PressureActivate[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}增在施压!");
const u8 gText_MoldBreakerActivate[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}打破常规!");
const u8 gText_TeravoltActivate[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}散发出爆炸的波导!");
const u8 gText_TurboblazeActivate[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}散发出炽烈的波导!");
const u8 gText_SlowStartActivate[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}无法逃跑!");
const u8 gText_UnnerveActivate[] = _("{B_SIDE}太紧张了\n无法使用树果!");
const u8 gText_DarkAuraActivate[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}散发出黑色的波导!");
const u8 gText_FairyAuraActivate[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}散发着童话般的波导！");
const u8 gText_AuraBreakActivate[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}反转了所有宝可梦的波导!");
const u8 gText_ComatoseActivate[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}昏昏欲睡!");
const u8 gText_AnticipationActivate[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}正在颤抖!");
const u8 gText_ForewarnActivate[] = _("他惊动了\n{B_EFF_NAME_WITH_PREFIX}的{B_BUFF1}!");
const u8 gText_FriskActivate[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}搜索了{B_DEF_NAME_WITH_PREFIX}\n发现了一个{B_LAST_ITEM}!");
const u8 gText_FogIsDeep[] = _("雾很蒙.");
const u8 gText_PrimordialSeaActivate[] = _("雨下的很大!");
const u8 gText_DesolateLandActivate[] = _("阳光变得很刺眼!");
const u8 gText_DeltaStream[] = _("神秘的强风保护着\n飞行属性宝可梦!");
const u8 gText_SlowStartEnd[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}齐心协力!");
const u8 gText_FlashFirePowerUp[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的火属性技能\n伤害提高了!");
const u8 gText_AbilityCuredStatusProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}解决了{B_BUFF1}的问题!");
const u8 gText_HealerCuredStatusProblem[] = _("{B_EFF_NAME_WITH_PREFIX}解决了{B_BUFF1} 的问题!");
const u8 gText_MagicBounce[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}将{B_CURRENT_MOVE}反弹了!");
const u8 gText_StartedSchooling[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}建立了一所学校!");
const u8 gText_StoppedSchooling[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}已停止上学!");
const u8 gText_GorillaTacticsMoveMenuBlockage[] = _("由于{B_CURRENT_MOVE}无法选择\n{B_LAST_ABILITY}!\p");
const u8 gText_IceFaceTookDamage[] = _("结冻头受到了\n来自{B_DEF_NAME_WITH_PREFIX}的伤害!");
const u8 gText_IceFaceBusted[] = _("{B_DEF_NAME_WITH_PREFIX}的头受到了伤害!");
const u8 gText_ScreenCleanerActivate[] = _("场上的所有屏障\n被消除了!");
const u8 gText_NeutralizingGasActivate[] = _("化学变化气体充满了场地!");
const u8 gText_NeutralizingGasEnd[] = _("化学变化气体的效果消失了!");
const u8 gText_PkmnHurtsWith[] = _("{B_ATK_NAME_WITH_PREFIX} 受伤了!");

const u8 SpikesLayString[] = _("尖刺分布在{B_FD35}队伍附近!");
const u8 ToxicSpikesLayString[] = _("毒刺分布在{B_FD35}队伍附近!");
const u8 StealthRockLayString[] = _("尖石漂浮在{B_FD35}队伍附近!");
const u8 StickyWebLayString[] = _("黏黏网分布在{B_FD35}队伍附近!");
const u8 gText_SteelsurgeLay[] = _("锋利的铁片分布在{B_FD35}队伍附近!");
const u8 RemovedEntryHazardsString[] = _("{B_FD36}被吹走了!");
const u8 RemovedEntryHazardsTargetSideString[] = _("{B_FD35}被吹走了!");
const u8 gText_HazardsBlownAway[] = _("{B_ATK_NAME_WITH_PREFIX}被吹走了!");
const u8 gText_BattleCircusDescriptionElectricTerrain[] = _("战场上充满了电！\p站在电属性的宝可梦无法入睡\n并且加强电属性技能!");
const u8 gText_BattleCircusDescriptionGrassyTerrain[] = _("战场上长满了草！\p站在草属性宝可梦将\n加强草属性技能并每回合\p都能治疗宝可梦!");
const u8 gText_BattleCircusDescriptionMistyTerrain[] = _("战场上雾很大！\p妖精属性宝可梦将\n加强妖精属性技能\p并不受额外状态干扰!");
const u8 gText_BattleCircusDescriptionPsychicTerrain[] = _("战场上变得很奇怪！\p超能力属性宝可梦将\n加强超能力属性技能\p并不会被先攻技能击中!");
const u8 gText_BattleCircusDescriptionRain[] = _("正在下着大雨");
const u8 gText_BattleCircusDescriptionSun[] = _("太阳很晴朗");
const u8 gText_BattleCircusDescriptionSandstorm[] = _("沙暴还在持续中");
const u8 gText_BattleCircusDescriptionHail[] = _("冰雹还在持续中");
const u8 gText_BattleCircusDescriptionFog[] = _("在迷雾的掩护下.");
const u8 gText_BattleCircusDescriptionTrickRoom[] = _("戏法空间的效果\p速度慢的宝可梦先行动!");
const u8 gText_BattleCircusDescriptionMagicRoom[] = _("魔法空间的效果!\p大多数道具无效");
const u8 gText_BattleCircusDescriptionWonderRoom[] = _("奇妙空间的效果!\p宝可梦的防御特防被交换");
const u8 gText_BattleCircusDescriptionGravity[] = _("在重力的影响下!\p所有空中宝可梦将落到地上!");
const u8 gText_BattleCircusDescriptionIonDeluge[] = _("空气中充满了离子！\n所有普通的技能都将变成电气模式！\p所有空中宝可梦将落到地上！");
const u8 gText_BattleCircusDescriptionFairyLock[] = _("战场被锁定\n宝可梦将无法脱离战斗");
const u8 gText_BattleCircusDescriptionMudSport[] = _("战场上充满了水泥\n电属性的技能将减弱");
const u8 gText_BattleCircusDescriptionWaterSport[] = _("空气中充满了水分\n火属性的技能将减弱");
const u8 gText_BattleCircusDescriptionInverseBattle[] = _("在反转对战中!\p所有属性相克将反转!");
const u8 gText_BattleCircusDescriptionDynamax[] = _("开启开关！\n所有的宝可梦都将能够极巨化！\p但是，宝可梦不能进行Mega进化，\n原始回复，Z爆发或使用Z招式！");
const u8 gText_BattleCircusDescriptionTradeMon[] = _("交换宝可梦\n双方将随机获得对手的\p其中一只宝可梦");
const u8 gText_BattleCircusDescriptionSafeguard[] = _("这个战斗的规定\p宝可梦不能对其他宝可梦\n施加状态条件！");
const u8 gText_BattleCircusDescriptionMist[] = _("雾气环绕双方战场!\p宝可梦的属性不会降低了!");
const u8 gText_BattleCircusDescriptionSeaOfFire[] = _("火海环绕在双方战场!\p所有飞火属性宝可梦\n每回合都会受到攻击!");
const u8 gText_BattleCircusDescriptionRainbow[] = _("彩虹环绕在双方战场\n所有技能有几率效果翻倍!");
const u8 gText_BattleCircusDescriptionConfused[] = _("所有宝可梦混乱了");
const u8 gText_BattleCircusDescriptionTaunt[] = _("所有宝可梦都被嘲讽了\n宝可梦将无法使用技能");
const u8 gText_BattleCircusDescriptionTorment[] = _("所有宝可梦都触发了\n无理取闹的效果!\p技能不能连续使用两次!");
const u8 gText_BattleCircusDescriptionHealBlock[] = _("所有宝可梦都触发了\n回复阻挡的效果!\p宝可梦无法治疗!");
const u8 gText_BattleCircusDescriptionThroatChop[] = _("所有宝可梦都触发了\n地狱突刺的效果!\p不能使用声音的技能!");
const u8 gText_BattleCircusDescriptionAlwaysCrit[] = _("所有宝可梦都触发了\n磨砺的效果!\p致命一击的几率提高了!");
const u8 gText_BattleCircusDescriptionNoCrits[] = _("没有宝可梦能致命一击!");
const u8 gText_BattleCircusDescriptionAbilitySuppression[] = _("所有宝可梦的能力减弱!");

const u8 gText_RainbowDisappeared[] = _("{B_FD36}的彩虹消失了!");
const u8 gText_SeaOfFireDisappeared[] = _("{B_FD36}周围的火海消失了!");
const u8 gText_SwampDisappeared[] = _("{B_FD36}周围的沼泽消失了!");
const u8 gText_WhiteHerbFling[] = _("The {B_LAST_ITEM}恢复了\n{B_SCR_ACTIVE_NAME_WITH_PREFIX}的状态!");
const u8 gText_SqueezedByOctolock[] = _("{B_ATK_NAME_WITH_PREFIX}被蛸固固定了!");
const u8 gText_NoRetreatTrapped[] = _("{B_ATK_NAME_WITH_PREFIX}不能逃跑\n因为他使用了{B_CURRENT_MOVE}!");
const u8 gText_TarShotAffected[] = _("{B_DEF_NAME_WITH_PREFIX}火属性招式能力下降!");
const u8 gText_TargetWasCaughtInSnapTrap[] = _("{B_DEF_NAME_WITH_PREFIX}被捕兽夹捆着了!");
const u8 gText_HurtBySeaOfFire[] = _("{B_ATK_NAME_WITH_PREFIX}被火海烧伤了!");
const u8 gText_BallFetchRetrievedItem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}找到了\n{B_LAST_ITEM}!");
const u8 gText_PerishBodyActivated[] = _("场地上的精灵都将在\n三个回合后晕倒!");

const u8 gText_HurtByGMaxVineLash[] = _("{B_ATK_NAME_WITH_PREFIX}受到\n超极巨灰飞鞭灭的攻击!");
const u8 gText_GMaxVineLashEnded[] = _("{B_FD36}周围的藤蔓消失了!");

const u8 gText_HurtByGMaxWildfire[] = _("{B_ATK_NAME_WITH_PREFIX}正在被超极巨地狱灭焰\n的附加效果燃烧!");
const u8 gText_GMaxWildfireEnded[] = _("超级巨狱焰周围\n{B_FD36}队伍消失了!");

const u8 gText_HurtByGMaxCannonade[] = _("{B_ATK_NAME_WITH_PREFIX}被\n超极巨水炮轰灭\n的漩涡伤害!");
const u8 gText_GMaxCannonadeEnded[] = _("{B_FD36}周围的漩涡消失了!");

const u8 gText_HurtByGMaxVolcalith[] = _("{B_ATK_NAME_WITH_PREFIX}超级巨炎石喷发伤害了!");
const u8 gText_GMaxVolcalithEnded[] = _("超级巨炎石喷发周围\n{B_FD36}队伍消失了!");
const u8 gText_TrappedByGMaxCentiferno[] = _("{B_SIDE}队伍陷入了一个\n炽热的漩涡中!");
const u8 gText_CouldntFullyProtect[] = _("{B_DEF_NAME_WITH_PREFIX}不能保护自己而受伤了!");
const u8 gText_TauntWoreOff[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的挑衅消失了!");
const u8 gText_TargetWrappedInMistyTerrain[] = _("{B_DEF_NAME_WITH_PREFIX}被包围在大雾中!");
const u8 gText_TargetWrappedInElectricTerrain[] = _("{B_DEF_NAME_WITH_PREFIX}被雷雨影响了!");
const u8 gText_SideStatusWoreOff[] = _("{B_FD38}队伍的{B_BUFF1}\n逐渐变淡!");

static const ability_t gTraceBannedAbilities[] =
	{
		ABILITY_TRACE,
		ABILITY_FORECAST,
		ABILITY_FLOWERGIFT,
		ABILITY_MULTITYPE,
		ABILITY_ILLUSION,
		ABILITY_ZENMODE,
		ABILITY_IMPOSTER,
		ABILITY_STANCECHANGE,
		ABILITY_POWEROFALCHEMY,
		ABILITY_RECEIVER,
		ABILITY_SCHOOLING,
		ABILITY_COMATOSE,
		ABILITY_SHIELDSDOWN,
		ABILITY_DISGUISE /*,ABILITY_RKSSYSTEM*/,
		ABILITY_BATTLEBOND,
		ABILITY_POWERCONSTRUCT,
		//ABILITY_NEUTRALIZINGGAS,
		ABILITY_ICEFACE,
		//ABILITY_HUNGERSWITCH,
		/*ABILITY_GULPMISSILE,*/ ABILITY_TABLES_TERMIN,
};

static const u16 sSoundMovesTable[] =
{
    MOVE_GROWL, MOVE_ROAR, MOVE_SING, MOVE_SUPERSONIC, MOVE_SCREECH, MOVE_SNORE,
    MOVE_UPROAR, MOVE_METAL_SOUND, MOVE_GRASS_WHISTLE, MOVE_HYPER_VOICE, 0xFFFF
};

static const ability_t gNeutralizingGasBannedAbilities[] =
{
    ABILITY_MULTITYPE,
    ABILITY_STANCECHANGE,
    ABILITY_SCHOOLING,
    ABILITY_COMATOSE,
    ABILITY_SHIELDSDOWN,
    ABILITY_DISGUISE,
    //ABILITY_RKSSYSTEM,
    ABILITY_BATTLEBOND,
    ABILITY_POWERCONSTRUCT,
    //ABILITY_NEUTRALIZINGGAS,
    ABILITY_ICEFACE,
    //ABILITY_HUNGERSWITCH,
    //ABILITY_GULPMISSILE,
    ABILITY_TABLES_TERMIN,
};

u8 GetBattlerForBattleScript(u8 caseId)
{
    u8 ret = 0;

    switch (caseId) 
    {
        case BS_GET_TARGET:
			ret = gBattlerTarget;
			break;
		case BS_GET_ATTACKER:
			ret = gBattlerAttacker;
			break;
		case BS_GET_EFFECT_BANK:
			ret = gEffectBattler;
			break;
		case BS_GET_BANK_0:
			ret = 0;
			break;
		case BS_GET_SCRIPTING_BANK:
			ret = gBattleScripting.battler;
			break;
		case BS_GET_FAINTED_BANK:
			ret = gBattlerFainted;
			break;
		case 5:
			ret = gBattlerFainted;
			break;
		case BS_GET_PLAYER1:
			ret = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
			break;
		case BS_GET_OPPONENT1:
			ret = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
			break;
		case BS_GET_PLAYER2:
			ret = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
			break;
		case BS_GET_OPPONENT2:
			ret = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
			break;
		case BS_GET_SWITCHING_BANK:
			ret = gBankSwitching;
			break;
		case 4:
		case 6:
		case 8:
		case 9:
			break;
	}

    return ret;
}

void PressurePPLose(u8 target, u8 attacker, u16 move)
{
    s32 i;

    if (gBattleMons[target].ability == ABILITY_PRESSURE)
    {
        for (i = 0; i < MAX_MON_MOVES && gBattleMons[attacker].moves[i] != move; ++i);
        if (i != MAX_MON_MOVES)
        {
            if (gBattleMons[attacker].pp[i])
                --gBattleMons[attacker].pp[i];
            if (!(gBattleMons[attacker].status2 & STATUS2_TRANSFORMED)
             && !(gDisableStructs[attacker].mimickedMoves & gBitTable[i]))
            {
                gActiveBattler = attacker;
                BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + i, 0, 1, &gBattleMons[gActiveBattler].pp[i]);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
    }
}

void PressurePPLoseOnUsingImprison(u8 attacker)
{
    s32 i, j;
    s32 imprisonPos = 4;
    u8 atkSide = GetBattlerSide(attacker);

    for (i = 0; i < gBattlersCount; ++i)
    {
        if (atkSide != GetBattlerSide(i) && gBattleMons[i].ability == ABILITY_PRESSURE)
        {
            for (j = 0; j < MAX_MON_MOVES && gBattleMons[attacker].moves[j] != MOVE_IMPRISON; ++j);
            if (j != MAX_MON_MOVES)
            {
                imprisonPos = j;
                if (gBattleMons[attacker].pp[j])
                    --gBattleMons[attacker].pp[j];
            }
        }
    }
    if (imprisonPos != 4
     && !(gBattleMons[attacker].status2 & STATUS2_TRANSFORMED)
     && !(gDisableStructs[attacker].mimickedMoves & gBitTable[imprisonPos]))
    {
        gActiveBattler = attacker;
        BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + imprisonPos, 0, 1, &gBattleMons[gActiveBattler].pp[imprisonPos]);
        MarkBattlerForControllerExec(gActiveBattler);
    }
}

void PressurePPLoseOnUsingPerishSong(u8 attacker)
{
    s32 i, j;
    s32 perishSongPos = 4;

    for (i = 0; i < gBattlersCount; ++i)
    {
        if (gBattleMons[i].ability == ABILITY_PRESSURE && i != attacker)
        {
            for (j = 0; j < MAX_MON_MOVES && gBattleMons[attacker].moves[j] != MOVE_PERISH_SONG; ++j);
            if (j != MAX_MON_MOVES)
            {
                perishSongPos = j;
                if (gBattleMons[attacker].pp[j])
                    --gBattleMons[attacker].pp[j];
            }
        }
    }
    if (perishSongPos != MAX_MON_MOVES
     && !(gBattleMons[attacker].status2 & STATUS2_TRANSFORMED)
     && !(gDisableStructs[attacker].mimickedMoves & gBitTable[perishSongPos]))
    {
        gActiveBattler = attacker;
        BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + perishSongPos, 0, 1, &gBattleMons[gActiveBattler].pp[perishSongPos]);
        MarkBattlerForControllerExec(gActiveBattler);
    }
}

void MarkAllBattlersForControllerExec(void)
{
    s32 i;

    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        for (i = 0; i < gBattlersCount; ++i)
            gBattleControllerExecFlags |= gBitTable[i] << 0x1C;
    else
        for (i = 0; i < gBattlersCount; ++i)
            gBattleControllerExecFlags |= gBitTable[i];
}

void MarkBattlerForControllerExec(u8 battlerId)
{
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        gBattleControllerExecFlags |= gBitTable[battlerId] << 0x1C;
    else
        gBattleControllerExecFlags |= gBitTable[battlerId];
}

void MarkBattlerReceivedLinkData(u8 battlerId)
{
    s32 i;

    for (i = 0; i < GetLinkPlayerCount(); ++i)
        gBattleControllerExecFlags |= gBitTable[battlerId] << (i << 2);

    gBattleControllerExecFlags &= ~(0x10000000 << battlerId);
}

void CancelMultiTurnMoves(u8 battler)
{
    gBattleMons[battler].status2 &= ~(STATUS2_MULTIPLETURNS);
    gBattleMons[battler].status2 &= ~(STATUS2_LOCK_CONFUSE);
    gBattleMons[battler].status2 &= ~(STATUS2_UPROAR);
    gBattleMons[battler].status2 &= ~(STATUS2_BIDE);
    gStatuses3[battler] &= ~(STATUS3_SEMI_INVULNERABLE);
    gDisableStructs[battler].rolloutTimer = 0;
    gDisableStructs[battler].furyCutterCounter = 0;
}

bool8 WasUnableToUseMove(u8 battler)
{
    if (gProtectStructs[battler].prlzImmobility
     || gProtectStructs[battler].targetNotAffected
     || gProtectStructs[battler].usedImprisonedMove
     || gProtectStructs[battler].loveImmobility
     || gProtectStructs[battler].usedDisabledMove
     || gProtectStructs[battler].usedTauntedMove
     || gProtectStructs[battler].flag2Unknown
     || gProtectStructs[battler].flinchImmobility
     || gProtectStructs[battler].confusionSelfDmg)
        return TRUE;
    else
        return FALSE;
}

void PrepareStringBattle(u16 stringId, u8 battler)
{
    gActiveBattler = battler;
    BtlController_EmitPrintString(0, stringId);
    MarkBattlerForControllerExec(gActiveBattler);
}

void ResetSentPokesToOpponentValue(void)
{
    s32 i;
    u32 bits = 0;

    gSentPokesToOpponent[0] = 0;
    gSentPokesToOpponent[1] = 0;
    for (i = 0; i < gBattlersCount; i += 2)
        bits |= gBitTable[gBattlerPartyIndexes[i]];
    for (i = 1; i < gBattlersCount; i += 2)
        gSentPokesToOpponent[(i & BIT_FLANK) >> 1] = bits;
}

void OpponentSwitchInResetSentPokesToOpponentValue(u8 battler)
{
    s32 i = 0;
    u32 bits = 0;

    if (GetBattlerSide(battler) == B_SIDE_OPPONENT)
    {
        u8 flank = ((battler & BIT_FLANK) >> 1);
        gSentPokesToOpponent[flank] = 0;

        for (i = 0; i < gBattlersCount; i += 2)
            if (!(gAbsentBattlerFlags & gBitTable[i]))
                bits |= gBitTable[gBattlerPartyIndexes[i]];
        gSentPokesToOpponent[flank] = bits;
    }
}

void UpdateSentPokesToOpponentValue(u8 battler)
{
    if (GetBattlerSide(battler) == B_SIDE_OPPONENT)
    {
        OpponentSwitchInResetSentPokesToOpponentValue(battler);
    }
    else
    {
        s32 i;
        
        for (i = 1; i < gBattlersCount; ++i)
            gSentPokesToOpponent[(i & BIT_FLANK) >> 1] |= gBitTable[gBattlerPartyIndexes[battler]];
    }
}

void BattleScriptPush(const u8 *bsPtr)
{
    gBattleResources->battleScriptsStack->ptr[gBattleResources->battleScriptsStack->size++] = bsPtr;
}

void BattleScriptPushCursor(void)
{
    gBattleResources->battleScriptsStack->ptr[gBattleResources->battleScriptsStack->size++] = gBattlescriptCurrInstr;
}

void BattleScriptPop(void)
{
    gBattlescriptCurrInstr = gBattleResources->battleScriptsStack->ptr[--gBattleResources->battleScriptsStack->size];
}

u8 TrySetCantSelectMoveBattleScript(void)
{
    u8 holdEffect;
    u8 limitations = 0;
    u16 move = gBattleMons[gActiveBattler].moves[gBattleBufferB[gActiveBattler][2]];
    u16 *choicedMove = &gBattleStruct->choicedMove[gActiveBattler];

    if (gDisableStructs[gActiveBattler].disabledMove == move && move != MOVE_NONE)
    {
        gBattleScripting.battler = gActiveBattler;
        gCurrentMove = move;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingDisabledMove;
        limitations = 1;
    }
    if (move == gLastMoves[gActiveBattler] && move != MOVE_STRUGGLE && (gBattleMons[gActiveBattler].status2 & STATUS2_TORMENT))
    {
        CancelMultiTurnMoves(gActiveBattler);
        gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingTormentedMove;
        ++limitations;
    }
    if (gDisableStructs[gActiveBattler].tauntTimer && !gBattleMoves[move].power)
    {
        gCurrentMove = move;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingNotAllowedMoveTaunt;
        ++limitations;
    }
    if (GetImprisonedMovesCount(gActiveBattler, move))
    {
        gCurrentMove = move;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingImprisonedMove;
        ++limitations;
    }
    if (gBattleMons[gActiveBattler].item == ITEM_ENIGMA_BERRY)
        holdEffect = gEnigmaBerries[gActiveBattler].holdEffect;
    else
        holdEffect = ItemId_GetHoldEffect(gBattleMons[gActiveBattler].item);
    gPotentialItemEffectBattler = gActiveBattler;
    if (holdEffect == HOLD_EFFECT_CHOICE_BAND && *choicedMove && *choicedMove != 0xFFFF && *choicedMove != move)
    {
        gCurrentMove = *choicedMove;
        gLastUsedItem = gBattleMons[gActiveBattler].item;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingNotAllowedMoveChoiceItem;
        ++limitations;
    }
    if (!gBattleMons[gActiveBattler].pp[gBattleBufferB[gActiveBattler][2]])
    {
        gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingMoveWithNoPP;
        ++limitations;
    }
    return limitations;
}

u8 CheckMoveLimitations(u8 bank, u8 unusableMoves, u8 check)
{
    ability_t ability = ABILITY(bank);
	u8 holdEffect = ITEM_EFFECT(bank);
	u16 choicedMove = CHOICED_MOVE(bank);
	int i;

	gPotentialItemEffectBattler = bank;
	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = gBattleMons[bank].moves[i];
		if (IsDynamaxed(bank))
			move = GetMaxMoveByMove(bank, move);

		if (IsUnusableMove(move, bank, check, gBattleMons[bank].pp[i], ability, holdEffect, choicedMove))
			unusableMoves |= gBitTable[i];
	}

	return unusableMoves;
}

bool8 AreAllMovesUnusable(void)
{
    u8 unusable = CheckMoveLimitations(gActiveBattler, 0, 0xFF);

    if (unusable == 0xF) // All moves are unusable.
    {
        gProtectStructs[gActiveBattler].noValidMoves = 1;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_NoMovesLeft;
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            gBattleBufferB[gActiveBattler][3] = GetBattlerAtPosition((BATTLE_OPPOSITE(GetBattlerPosition(gActiveBattler))) | (Random() & 2));
        else 
            gBattleBufferB[gActiveBattler][3] = GetBattlerAtPosition(BATTLE_OPPOSITE(GetBattlerPosition(gActiveBattler)));
    }
    else
    {
        gProtectStructs[gActiveBattler].noValidMoves = 0;
    }
    return (unusable == 0xF);
}

u8 GetImprisonedMovesCount(u8 battlerId, u16 move)
{
    s32 i;
    u8 imprisonedMoves = 0;
    u8 battlerSide = GetBattlerSide(battlerId);

    for (i = 0; i < gBattlersCount; ++i)
    {
        if (battlerSide != GetBattlerSide(i) && gStatuses3[i] & STATUS3_IMPRISONED_OTHERS)
        {
            s32 j;

            for (j = 0; j < MAX_MON_MOVES && move != gBattleMons[i].moves[j]; ++j);
            if (j < MAX_MON_MOVES)
                ++imprisonedMoves;
        }
    }
    return imprisonedMoves;
}

enum
{
    ENDTURN_ORDER,
    ENDTURN_REFLECT,
    ENDTURN_LIGHT_SCREEN,
    ENDTURN_MIST,
    ENDTURN_SAFEGUARD,
    ENDTURN_WISH,
    ENDTURN_RAIN,
    ENDTURN_SANDSTORM,
    ENDTURN_SUN,
    ENDTURN_HAIL,
    ENDTURN_FIELD_COUNT,
};

enum
{
    ENDTURN_INGRAIN,
    ENDTURN_ABILITIES,
    ENDTURN_ITEMS1,
    ENDTURN_LEECH_SEED,
    ENDTURN_POISON,
    ENDTURN_BAD_POISON,
    ENDTURN_BURN,
    ENDTURN_NIGHTMARES,
    ENDTURN_CURSE,
    ENDTURN_WRAP,
    ENDTURN_UPROAR,
    ENDTURN_THRASH,
    ENDTURN_DISABLE,
    ENDTURN_ENCORE,
    ENDTURN_LOCK_ON,
    ENDTURN_CHARGE,
    ENDTURN_TAUNT,
    ENDTURN_YAWN,
    ENDTURN_ITEMS2,
    ENDTURN_BATTLER_COUNT
};

enum EndTurnEffects
{
	ET_Order,
	ET_General_Counter_Decrement,
	ET_Weather,
	ET_Weather_Damage_Health_Abilities,
	ET_Future_Sight,
	ET_Wish,
	ET_Block_A,
	ET_Aqua_Ring,
	ET_Ingrain,
	ET_Leech_Seed,
	ET_Item_Effects2,
	ET_Switch_Out_Abilities2,
	ET_Poison,
	ET_Item_Effects3,
	ET_Switch_Out_Abilities3,
	ET_Burn,
	ET_Item_Effects4,
	ET_Switch_Out_Abilities4,
	ET_Nightmare,
	ET_Item_Effects5,
	ET_Switch_Out_Abilities5,
	ET_Curse,
	ET_Item_Effects6,
	ET_Switch_Out_Abilities6,
	ET_Trap_Damage,
	ET_Octolock,
	ET_Item_Effects7,
	ET_Switch_Out_Abilities7,
	ET_Taunt_Timer,
	ET_Encore_Timer,
	ET_Disable_Timer,
	ET_Lock_On_Timer,
	ET_Charge_Timer,
	ET_Magnet_Rise_Timer,
	ET_Telekinesis_Timer,
	ET_Heal_Block_Timer,
	ET_Embargo_Timer,
	ET_Yawn,
	ET_Item_Effects8,
	ET_Perish_Song,
	ET_Roost,
	ET_Reflect,
	ET_Light_Screen,
	ET_Safeguard,
	ET_Mist,
	ET_Tailwind_Timer,
	ET_Lucky_Chant_Timer,
	ET_Rainbow_Timer,
	ET_Sea_Of_Fire_Timer,
	ET_Swamp_Timer,
	ET_G_Max_VineLash_Timer,
	ET_G_Max_Wildfire_Timer,
	ET_G_Max_Cannonade_Timer,
	ET_G_Max_Volcalith_Timer,
	ET_Aurora_Veil,
	ET_Trick_Room_Timer,
	ET_Gravity_Timer,
	ET_Water_Sport_Timer,
	ET_Mud_Sport_Timer,
	ET_Wonder_Room_Timer,
	ET_Magic_Room_Timer,
	ET_Seeds,
	ET_Terrain_Timer,
	ET_Block_B,
	ET_Form_Change,
	ET_Reactivate_Overworld_Weather,
	ET_End
};

enum Block_A
{
	ET_Sea_Of_Fire,
	ET_G_Max_VineLash,
	ET_G_Max_Wildfire,
	ET_G_Max_Cannonade,
	ET_G_Max_Volcalith,
	ET_Grassy_Terrain,
	ET_Hydration_ShedSkin_Healer,
	ET_Item_Effects,
	ET_Switch_Out_Abilities,
	MAX_CASES_BLOCK_A,
};

enum Block_B
{
	ET_Uproar,
	ET_SpeedBoost_Moody_BadDreams_SlowStart,
	ET_Orbz,
	ET_Harvest_Pickup,
	MAX_CASES_BLOCK_B,
};

u32 GetLeechSeedDamage(u8 bank)
{
	u32 damage = 0;

	if (gStatuses3[bank] & STATUS3_LEECHSEED
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
		damage = MathMax(1, GetBaseMaxHP(bank) / 8);

	return damage;
}

u32 GetPoisonDamage(u8 bank)
{
	u32 damage = 0;
	ability_t ability = ABILITY(bank);

	if (ability != ABILITY_MAGICGUARD
	&& ability != ABILITY_POISONHEAL)
	{
		if (gBattleMons[bank].status1 & STATUS1_POISON)
		{
			damage = MathMax(1, GetBaseMaxHP(bank) / 8);
		}
		else if (gBattleMons[bank].status1 & STATUS1_TOXIC_POISON)
		{
			damage = MathMax(1, GetBaseMaxHP(bank) / 16);
			damage *= (gBattleMons[gActiveBattler].status1 & 0xF00) >> 8;
		}
	}
	else if (ability == ABILITY_POISONHEAL)
		damage = MathMax(1, GetBaseMaxHP(bank) / 8);

	return damage;
}

u32 GetBurnDamage(u8 bank)
{
	u32 damage = 0;
	ability_t ability = ABILITY(bank);

	if (gBattleMons[bank].status1 & STATUS1_BURN
	&& ability != ABILITY_MAGICGUARD)
	{
		if (ability == ABILITY_HEATPROOF)
		{
			#ifdef OLD_BURN_DAMAGE
				damage = MathMax(1, GetBaseMaxHP(bank) / 16);
			#else
				damage = MathMax(1, GetBaseMaxHP(bank) / 32);
			#endif
		}
		else
		{
			#ifdef OLD_BURN_DAMAGE
				damage = MathMax(1, GetBaseMaxHP(bank) / 8);
			#else
				damage = MathMax(1, GetBaseMaxHP(bank) / 16);
			#endif
		}
	}

	return damage;
}

u32 GetNightmareDamage(u8 bank)
{
	u32 damage = 0;

	if (gBattleMons[bank].status2 & STATUS2_NIGHTMARE
	&& gBattleMons[bank].status1 & STATUS1_SLEEP
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 4);
	}

	return damage;
}

u32 GetCurseDamage(u8 bank)
{
	u32 damage = 0;

	if (gBattleMons[bank].status2 & STATUS2_CURSED
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 4);
	}

	return damage;
}

u32 GetTrapDamage(u8 bank)
{
	u32 damage = 0;

	if (gBattleMons[bank].status2 & STATUS2_WRAPPED
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		if ((gBattleStruct->sandblastCentiferno[gActiveBattler] & 2) //Trapped by this move and user held Binding Band
		|| ITEM_EFFECT(gBattleStruct->wrappedBy[bank]) == HOLD_EFFECT_BINDING_BAND)
			damage = MathMax(1, GetBaseMaxHP(bank) / 6);
		else
			damage = MathMax(1, GetBaseMaxHP(bank) / 8);
	}

	return damage;
}

u32 GetSeaOfFireDamage(u8 bank)
{
	u32 damage = 0;

	if (BankSideHasSeaOfFire(bank)
	&& !IsOfType(bank, TYPE_FIRE)
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 8);
	}

	return damage;
}

u32 GetGMaxVineLashDamage(u8 bank)
{
	u32 damage = 0;

	if (BankSideHasGMaxVineLash(bank)
	&& !IsOfType(bank, TYPE_GRASS)
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 6);
	}

	return damage;
}

u32 GetGMaxWildfireDamage(u8 bank)
{
	u32 damage = 0;

	if (BankSideHasGMaxWildfire(bank)
	&& !IsOfType(bank, TYPE_FIRE)
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 6);
	}

	return damage;
}

u32 GetGMaxCannonadeDamage(u8 bank)
{
	u32 damage = 0;

	if (BankSideHasGMaxCannonade(bank)
	&& !IsOfType(bank, TYPE_WATER)
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 6);
	}

	return damage;
}

u32 GetGMaxVolcalithDamage(u8 bank)
{
	u32 damage = 0;

	if (BankSideHasGMaxVolcalith(bank)
	&& ABILITY(bank) != ABILITY_MAGICGUARD)
	{
		damage = MathMax(1, GetBaseMaxHP(bank) / 6);
	}

	return damage;
}

#define TURNBASED_MAX_CASE ET_End

u8 DoBattlerEndTurnEffects(void)
{
   int i, j;
	u8 effect = 0;

	if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
		return 0;

	gHitMarker |= (HITMARKER_GRUDGE | HITMARKER_x20);
	while (gBattleStruct->turnEffectsTracker <= TURNBASED_MAX_CASE)
	{
		u8 sideBank;
		gActiveBattler = gBattlerAttacker = gBattlerTarget = gBattlerByTurnOrder[gBattleStruct->turnEffectsBattlerId];

		switch (gBattleStruct->turnEffectsTracker) {
			case ET_Order:
				for (i = 0; i < gBattlersCount; ++i)
				{
					gBattlerByTurnOrder[i] = i;
				}
				for (i = 0; i < gBattlersCount - 1; ++i)
				{
					for (j = i + 1; j < gBattlersCount; ++j)
					{
						if (GetWhoStrikesFirst(gBattlerByTurnOrder[i], gBattlerByTurnOrder[j], TRUE))
							SwapTurnOrder(i, j);
					}
				}
				++gBattleStruct->turnEffectsTracker;

			case ET_General_Counter_Decrement:
				for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
				{
					if(gBattleStruct->ThroatChopTimers[i])
						--gBattleStruct->ThroatChopTimers[i];

					if(gBattleStruct->StompingTantrumTimers[i])
						--gBattleStruct->StompingTantrumTimers[i];

					if (gBattleStruct->StakeoutCounters[i])
						--gBattleStruct->StakeoutCounters[i];

					if (gBattleStruct->DestinyBondCounters[i])
						--gBattleStruct->DestinyBondCounters[i];
					
					if (gBattleStruct->ai.switchingCooldown[i])
						--gBattleStruct->ai.switchingCooldown[i];

					gBattleStruct->synchronizeTarget[i] = 0;
					gBattleMons[i].status2 &= ~(STATUS2_FLINCHED);
				}

				//These should be cleared earlier on, but just in case they aren't
				gBattleStruct->PledgeHelper = FALSE;
				gBattleStruct->PowderByte = 0;
				gBattleStruct->BeakBlastByte = 0;
				gBattleStruct->quashed = 0;
				gBattleStruct->playedFocusPunchMessage = 0;
				gBattleStruct->playedShellTrapMessage = 0;
				gBattleStruct->CustapQuickClawIndicator = 0;
				gBattleStruct->activatedCustapQuickClaw = 0;
				gBattleStruct->criticalCapture = 0;
				gBattleStruct->devolveForgotMove = 0;

				if (gBattleStruct->RetaliateCounters[0])
					--gBattleStruct->RetaliateCounters[0];

				if (gBattleStruct->RetaliateCounters[1])
					--gBattleStruct->RetaliateCounters[1];

				if (gBattleStruct->FairyLockTimer)
					--gBattleStruct->FairyLockTimer;

				if (gBattleStruct->EchoedVoiceCounter)
					--gBattleStruct->EchoedVoiceCounter;

				if (gBattleStruct->EchoedVoiceCounter == 0)
					gBattleStruct->EchoedVoiceDamageScale = 0;

				gSideStatuses[B_SIDE_PLAYER] &= ~(SIDE_STATUS_CRAFTY_SHIELD | SIDE_STATUS_MAT_BLOCK | SIDE_STATUS_QUICK_GUARD | SIDE_STATUS_WIDE_GUARD);
				gSideStatuses[B_SIDE_OPPONENT] &= ~(SIDE_STATUS_CRAFTY_SHIELD | SIDE_STATUS_MAT_BLOCK | SIDE_STATUS_QUICK_GUARD | SIDE_STATUS_WIDE_GUARD);

				gBattleStruct->NoMoreMovingThisTurn = 0;
				gBattleStruct->fusionFlareUsedPrior = FALSE;
				gBattleStruct->fusionBoltUsedPrior = FALSE;
				gBattleStruct->roundUsed = FALSE;

				if (IsRaidBattle() && !BATTLER_ALIVE(BANK_RAID_BOSS)) //Raid mon is dead so skip end turn effects
				{
					gBattleStruct->turnEffectsTracker = ET_End;
					goto END_TURN_SKIP;
				}

				++gBattleStruct->turnEffectsTracker;

			case ET_Weather:
				if (gBattleWeather & WEATHER_RAIN_ANY)
				{
					if (!(gBattleWeather & WEATHER_RAIN_PERMANENT)
					&& --gWishFutureKnock.weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_RAIN_ANY;
						gBattleCommunication[MULTISTRING_CHOOSER] = 2;
					}
					else if (gBattleWeather & WEATHER_RAIN_DOWNPOUR)
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;
					}
					else
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					}

					BattleScriptExecute(BattleScript_RainContinuesOrEnds);
					effect++;
				}
				else if (gBattleWeather & WEATHER_SUN_ANY)
				{
					if (!(gBattleWeather & WEATHER_SUN_PERMANENT)
					&& --gWishFutureKnock.weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_SUN_ANY;
						gBattlescriptCurrInstr = BattleScript_SunlightFaded;
					}
					else
					{
						gBattlescriptCurrInstr = BattleScript_SunlightContinues;
					}
					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				else if (gBattleWeather & WEATHER_SANDSTORM_ANY)
				{
					if (!(gBattleWeather & WEATHER_SANDSTORM_PERMANENT)
					&& --gWishFutureKnock.weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_SANDSTORM_ANY;
						gBattlescriptCurrInstr = BattleScript_SandStormHailEnds;
					}
					else
					{
						gBattlescriptCurrInstr = BattleScript_SandstormHailContinues;
					}

					gBattleScripting.animArg1 = B_ANIM_SANDSTORM_CONTINUES;
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				else if (gBattleWeather & WEATHER_HAIL_ANY)
				{
					if (!(gBattleWeather & WEATHER_HAIL_PERMANENT)
					&& --gWishFutureKnock.weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_HAIL_ANY;
						gBattlescriptCurrInstr = BattleScript_SandStormHailEnds;
					}
					else
					{
						gBattlescriptCurrInstr = BattleScript_SandstormHailContinues;
					}

					gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				else if (gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL)
				{
					BattleScriptExecute(BattleScript_MysteriousAirCurrentContinues);
					effect++;
				}
				else if (gBattleWeather & WEATHER_FOG_ANY)
				{
					if (!(gBattleWeather & WEATHER_FOG_PERMANENT)
					&& --gWishFutureKnock.weatherDuration == 0)
					{
						gBattleWeather &= ~WEATHER_FOG_ANY;
						gBattlescriptCurrInstr = BattleScript_FogEnded;
					}
					else
					{
						gBattlescriptCurrInstr = BattleScript_FogContinues;
					}
					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				gBattleStruct->turnEffectsBattlerId = gBattlersCount;
				break;

			case ET_Weather_Damage_Health_Abilities:
				if (BATTLER_ALIVE(gActiveBattler))
				{
					gLastUsedAbility = ABILITY(gActiveBattler);
					switch(gLastUsedAbility) {
						case ABILITY_RAINDISH:
						case ABILITY_DRYSKIN:
						case ABILITY_ICEBODY:
						case ABILITY_SOLARPOWER:
							if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
								effect++;
							break;
						case ABILITY_FORECAST:
							if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gActiveBattler, 0, 0, 0))
								effect++;
							break;
					}

					if (!effect)
					{
						if (gBattleWeather & WEATHER_HAIL_ANY)
						{
							if (TakesDamageFromHail(gActiveBattler))
								effect++;
						}
						else if (gBattleWeather & WEATHER_SANDSTORM_ANY)
						{
							if (TakesDamageFromSandstorm(gActiveBattler))
								effect++;
						}

						if (effect)
							BattleScriptExecute(BattleScript_WeatherDamage);
					}
				}
				break;

			case ET_Future_Sight:
				if (gWishFutureKnock.futureSightCounter[gActiveBattler]
				&& --gWishFutureKnock.futureSightCounter[gActiveBattler] == 0
				&& gBattleMons[gActiveBattler].hp)
				{
					if (gWishFutureKnock.futureSightMove[gActiveBattler] == MOVE_FUTURESIGHT)
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					else //Doom Desire
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;

					gCurrentMove = gWishFutureKnock.futureSightMove[gActiveBattler];
					gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gActiveBattler, gCurrentMove);

					gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
					gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
					gBattleTextBuff1[2] = gWishFutureKnock.futureSightMove[gActiveBattler];
					gBattleTextBuff1[3] = gWishFutureKnock.futureSightMove[gActiveBattler] >> 8;
					gBattleTextBuff1[4] = 0xFF;
					gBattlerAttacker = gWishFutureKnock.futureSightAttacker[gActiveBattler];
					gSpecialStatuses[gBattlerTarget].dmg = 0xFFFF;
					BattleScriptExecute(BattleScript_MonTookFutureAttack);
					effect++;
				}
				break;

			case ET_Wish:
				if(gBattleStruct->ElectrifyTimers[gActiveBattler]) //Cleared down here because necessary for Future Sight
					--gBattleStruct->ElectrifyTimers[gActiveBattler];

				if(gBattleStruct->LaserFocusTimers[gActiveBattler])
					--gBattleStruct->LaserFocusTimers[gActiveBattler];

				if (gWishFutureKnock.wishCounter[gActiveBattler]
				&& --gWishFutureKnock.wishCounter[gActiveBattler] == 0
				&& gBattleMons[gActiveBattler].hp)
				{
					BattleScriptExecute(BattleScript_WishComesTrue);
					effect++;
				}
				gBattleStruct->endTurnBlockState = 0; //For later
				break;

			case ET_Block_A:
				switch(gBattleStruct->endTurnBlockState) {
					case ET_Sea_Of_Fire:
						if (BATTLER_ALIVE(gActiveBattler)
						&&  BankSideHasSeaOfFire(gActiveBattler)
						&&	ABILITY(gActiveBattler) != ABILITY_MAGICGUARD
						&&	!IsOfType(gActiveBattler, TYPE_FIRE))
						{
							gBattleMoveDamage = GetSeaOfFireDamage(gActiveBattler);
							gBattleStringLoader = gText_HurtBySeaOfFire;
							gBattleScripting.animArg1 = B_ANIM_SEA_OF_FIRE;
							BattleScriptExecute(BattleScript_SeaOfFireDamage);
							effect++;
						}
						gBattleStruct->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
						break;

					case ET_G_Max_VineLash:
						if (BATTLER_ALIVE(gActiveBattler)
						&&  BankSideHasGMaxVineLash(gActiveBattler)
						&&	ABILITY(gActiveBattler) != ABILITY_MAGICGUARD
						&&	!IsOfType(gActiveBattler, TYPE_GRASS))
						{
							gBattleMoveDamage = GetGMaxVineLashDamage(gActiveBattler);
							gBattleStringLoader = gText_HurtByGMaxVineLash;
							gBattleScripting.animArg1 = B_ANIM_G_MAX_VINE_LASH;
							BattleScriptExecute(BattleScript_SeaOfFireDamage);
							effect++;
						}
						gBattleStruct->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
						break;

					case ET_G_Max_Wildfire:
						if (BATTLER_ALIVE(gActiveBattler)
						&&  BankSideHasGMaxWildfire(gActiveBattler)
						&&	ABILITY(gActiveBattler) != ABILITY_MAGICGUARD
						&&	!IsOfType(gActiveBattler, TYPE_FIRE))
						{
							gBattleMoveDamage = GetGMaxWildfireDamage(gActiveBattler);
							gBattleStringLoader = gText_HurtByGMaxWildfire;
							gBattleScripting.animArg1 = B_ANIM_G_MAX_WILDFIRE;
							BattleScriptExecute(BattleScript_SeaOfFireDamage);
							effect++;
						}
						gBattleStruct->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
						break;

					case ET_G_Max_Cannonade:
						if (BATTLER_ALIVE(gActiveBattler)
						&&  BankSideHasGMaxCannonade(gActiveBattler)
						&&	ABILITY(gActiveBattler) != ABILITY_MAGICGUARD
						&&	!IsOfType(gActiveBattler, TYPE_WATER))
						{
							gBattleMoveDamage = GetGMaxCannonadeDamage(gActiveBattler);
							gBattleStringLoader = gText_HurtByGMaxCannonade;
							gBattleScripting.animArg1 = B_ANIM_G_MAX_CANNONADE;
							BattleScriptExecute(BattleScript_SeaOfFireDamage);
							effect++;
						}
						gBattleStruct->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
						break;

					case ET_G_Max_Volcalith:
						if (BATTLER_ALIVE(gActiveBattler)
						&&  BankSideHasGMaxVolcalith(gActiveBattler)
						&&	ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
						{
							gBattleMoveDamage = GetGMaxVolcalithDamage(gActiveBattler);
							gBattleStringLoader = gText_HurtByGMaxVolcalith;
							gBattleScripting.animArg1 = B_ANIM_G_MAX_VOLCALITH;
							BattleScriptExecute(BattleScript_SeaOfFireDamage);
							effect++;
						}
						gBattleStruct->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
						break;

					case ET_Grassy_Terrain:
						if (gTerrainType == GRASSY_TERRAIN && CheckGrounding(gActiveBattler)
						&&  BATTLER_ALIVE(gActiveBattler)
						&&  !BATTLER_MAX_HP(gActiveBattler)
						&& !IsHealBlocked(gActiveBattler))
						{
							gBattleMoveDamage = -1 * MathMax(1, GetBaseMaxHP(gActiveBattler) / 16);
							BattleScriptExecute(BattleScript_GrassyTerrainHeal);
							effect++;
						}
						break;

					case ET_Item_Effects:
						if (BATTLER_ALIVE(gActiveBattler))
						{
							if (ItemBattleEffects(ItemEffects_EndTurn, gActiveBattler, FALSE, FALSE))
								effect++;
						}
						break;

					case ET_Hydration_ShedSkin_Healer:
						if (BATTLER_ALIVE(gActiveBattler))
						{
							switch(ABILITY(gActiveBattler)) {
								case ABILITY_SHEDSKIN:
								case ABILITY_HYDRATION:
								case ABILITY_HEALER:
								case ABILITY_EMERGENCYEXIT:
									if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
										effect++;
							}
						}
						break;
				}

				++gBattleStruct->endTurnBlockState;
				if (gBattleStruct->endTurnBlockState < MAX_CASES_BLOCK_A)
					goto SkipBankIncrement;

				gBattleStruct->endTurnBlockState = 0;
				break;

			case ET_Aqua_Ring:
				if (gStatuses3[gActiveBattler] & STATUS3_AQUA_RING
				&& BATTLER_ALIVE(gActiveBattler)
				&& !BATTLER_MAX_HP(gActiveBattler)
				&& !IsHealBlocked(gActiveBattler))
				{
					gBattleMoveDamage = GetBaseMaxHP(gActiveBattler) / 16;
					if (ITEM_EFFECT(gActiveBattler) == HOLD_EFFECT_BIG_ROOT)
						gBattleMoveDamage = (gBattleMoveDamage * 130) / 100;
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;

					gBattleMoveDamage *= -1;
					BattleScriptExecute(BattleScript_AquaRing);
					effect++;
				}
				break;

			case ET_Ingrain:
				if (gStatuses3[gActiveBattler] & STATUS3_ROOTED
				&& BATTLER_ALIVE(gActiveBattler)
				&& !BATTLER_MAX_HP(gActiveBattler)
				&& !IsHealBlocked(gActiveBattler))
				{
					gBattleMoveDamage = GetBaseMaxHP(gActiveBattler) / 16;
					if (ITEM_EFFECT(gActiveBattler) == HOLD_EFFECT_BIG_ROOT)
						gBattleMoveDamage = (gBattleMoveDamage * 13) / 10;
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;

					gBattleMoveDamage *= -1;
					BattleScriptExecute(BattleScript_IngrainTurnHeal);
					effect++;
				}
				break;

			case ET_Leech_Seed:
				if (gStatuses3[gActiveBattler] & STATUS3_LEECHSEED
				&& BATTLER_ALIVE(gActiveBattler)
				&& ABILITY(gActiveBattler) != ABILITY_MAGICGUARD
				&& gBattleMons[gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER].hp != 0)
				{
					gBattlerTarget = gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER; //funny how the 'target' is actually the bank that receives HP
					gBattleMoveDamage = GetLeechSeedDamage(gActiveBattler);
					gBattleScripting.animArg1 = gBattlerTarget;
					gBattleScripting.animArg2 = gBattlerAttacker;
					BattleScriptExecute(BattleScript_LeechSeedTurnDrain);
					effect++;
				}
				gBattleStruct->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			//One item effect checked is done after each time the Pokemon receives damage
			case ET_Item_Effects2:
			case ET_Item_Effects3:
			case ET_Item_Effects4:
			case ET_Item_Effects5:
			case ET_Item_Effects6:
			case ET_Item_Effects7:
			case ET_Item_Effects8:
				if (BATTLER_ALIVE(gActiveBattler))
				{
					if (ItemBattleEffects(ItemEffects_EndTurn, gActiveBattler, FALSE, FALSE))
						effect++;
				}
				break;
			
			//One ability checked is done after each time the Pokemon receives damage
			case ET_Switch_Out_Abilities2:
			case ET_Switch_Out_Abilities3:
			case ET_Switch_Out_Abilities4:
			case ET_Switch_Out_Abilities5:
			case ET_Switch_Out_Abilities6:
			case ET_Switch_Out_Abilities7:
				if (BATTLER_ALIVE(gActiveBattler))
				{
					switch(ABILITY(gActiveBattler)) {
						case ABILITY_EMERGENCYEXIT:
							if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
								effect++;
					}
				}
				break;

			case ET_Poison:
				if (BATTLER_ALIVE(gActiveBattler)
				&& ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
				{
					if (gBattleMons[gActiveBattler].status1 & (STATUS1_POISON | STATUS1_TOXIC_POISON))
					{
						if (gBattleMons[gActiveBattler].status1 & STATUS1_TOXIC_POISON
						&& (gBattleMons[gActiveBattler].status1 & 0xF00) != 0xF00) //not 16 turns
							gBattleMons[gActiveBattler].status1 += 0x100;

						gBattleMoveDamage = GetPoisonDamage(gActiveBattler);

						if (ABILITY(gActiveBattler) == ABILITY_POISONHEAL)
						{
							if (!BATTLER_MAX_HP(gActiveBattler) && !IsHealBlocked(gActiveBattler))
							{
								gBattleMoveDamage *= -1;
								gBattleScripting.battler = gActiveBattler;
								BattleScriptExecute(BattleScript_PoisonHeal);
							}
						}
						else
						{
							BattleScriptExecute(BattleScript_PoisonTurnDmg);
						}
						++effect;
					}
				}
				gBattleStruct->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			case ET_Burn:
				if (gBattleMons[gActiveBattler].status1 & STATUS1_BURN
				&& BATTLER_ALIVE(gActiveBattler)
				&& ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
				{
					gBattleMoveDamage = GetBurnDamage(gActiveBattler);

					BattleScriptExecute(BattleScript_BurnTurnDmg);
					effect++;
				}
				gBattleStruct->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			case ET_Nightmare:
				if (gBattleMons[gActiveBattler].status2 & STATUS2_NIGHTMARE)
				{
					if (gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP)
					{
						if (BATTLER_ALIVE(gActiveBattler)
						&&  ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
						{
							gBattleMoveDamage = GetNightmareDamage(gActiveBattler);
							BattleScriptExecute(BattleScript_NightmareTurnDmg);
							effect++;
						}
					}
					else
						gBattleMons[gActiveBattler].status2 &= ~STATUS2_NIGHTMARE;
				}
				gBattleStruct->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			case ET_Curse:
				if (gBattleMons[gActiveBattler].status2 & STATUS2_CURSED
				&&  BATTLER_ALIVE(gActiveBattler)
				&&  ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
				{
					gBattleMoveDamage = GetCurseDamage(gActiveBattler);
					BattleScriptExecute(BattleScript_CurseTurnDmg);
					effect++;
				}
				gBattleStruct->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			case ET_Trap_Damage:
				if (gBattleStruct->brokeFreeMessage & gBitTable[gActiveBattler]
				&& BATTLER_ALIVE(gActiveBattler))
				{
					//Broke free
					gBattleMons[gActiveBattler].status2 &= ~(STATUS2_WRAPPED);
					gBattleStruct->brokeFreeMessage &= ~(gBitTable[gActiveBattler]);
					gBattleStruct->sandblastCentiferno[gActiveBattler] = 0;

					gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
					gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
					gBattleTextBuff1[2] = gBattleStruct->wrappedMove[gActiveBattler];
					gBattleTextBuff1[3] = gBattleStruct->wrappedMove[gActiveBattler] >> 8;
					gBattleTextBuff1[4] = EOS;
					gBattlescriptCurrInstr = BattleScript_WrapEnds;
					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				else if ((gBattleMons[gActiveBattler].status2 & STATUS2_WRAPPED)
				&& gBattleMons[gActiveBattler].hp)
				{
					gBattleMons[gActiveBattler].status2 -= 0x2000;

					if (!(gBattleMons[gActiveBattler].status2 & STATUS2_WRAPPED))
					{
						gBattleMons[gActiveBattler].status2 |= STATUS2_WRAPPED; //Reactivate temporarily
						gBattleStruct->brokeFreeMessage |= gBitTable[gActiveBattler]; //Will play next turn
					}

					if (ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
					{
						//Damaged by wrap
						gBattleScripting.animArg1 = gBattleStruct->wrappedMove[gActiveBattler];
						gBattleScripting.animArg2 = gBattleStruct->wrappedMove[gActiveBattler] >> 8;
						gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
						gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
						gBattleTextBuff1[2] = gBattleStruct->wrappedMove[gActiveBattler];
						gBattleTextBuff1[3] = gBattleStruct->wrappedMove[gActiveBattler] >> 8;
						gBattleTextBuff1[4] = EOS;

						gBattlescriptCurrInstr = BattleScript_WrapTurnDmg;
						gBattleMoveDamage = GetTrapDamage(gActiveBattler);

						BattleScriptExecute(gBattlescriptCurrInstr);
						effect++;
					}
				}
				gBattleStruct->turnDamageTaken[gActiveBattler] = gBattleMoveDamage; //For Emergency Exit
				break;

			case ET_Octolock:
				if (gBattleMons[gActiveBattler].status2 & STATUS2_ESCAPE_PREVENTION
				&& BATTLER_ALIVE(gActiveBattler) && gBattleStruct->trappedByOctolock)
				{
					gBattleScripting.animArg1 = MOVE_OCTOLOCK & 0xFF;
					gBattleScripting.animArg2 = MOVE_OCTOLOCK >> 8;
					BattleScriptExecute(BattleScript_OctolockTurnDmg);
					effect++;
				}
				break;

			case ET_Taunt_Timer:
				if (gDisableStructs[gActiveBattler].tauntTimer > 0 && --gDisableStructs[gActiveBattler].tauntTimer == 0)
				{
					gBattleScripting.battler = gActiveBattler;
					gBattleStringLoader = gText_TauntWoreOff;
					BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
					effect++;
				}
				break;

			case ET_Encore_Timer:
				if (gDisableStructs[gActiveBattler].encoreTimer && BATTLER_ALIVE(gActiveBattler))
				{
					if (gBattleMons[gActiveBattler].moves[gDisableStructs[gActiveBattler].encoredMovePos] != gDisableStructs[gActiveBattler].encoredMove)
					{  // pokemon does not have the encored move anymore
						gDisableStructs[gActiveBattler].encoredMove = 0;
						gDisableStructs[gActiveBattler].encoreTimer = 0;
					}
					else if (--gDisableStructs[gActiveBattler].encoreTimer == 0
						  || gBattleMons[gActiveBattler].pp[gDisableStructs[gActiveBattler].encoredMovePos] == 0)
					{
						gDisableStructs[gActiveBattler].encoredMove = 0;
						gDisableStructs[gActiveBattler].encoreTimer = 0;
						BattleScriptExecute(BattleScript_EncoredNoMore);
						effect++;
					}
				}
				break;

			case ET_Disable_Timer:
				if (gDisableStructs[gActiveBattler].disableTimer && gBattleMons[gActiveBattler].hp)
				{
					int movenum;

					for (movenum = 0; movenum < 4; movenum++)
					{
						if (gDisableStructs[gActiveBattler].disabledMove == gBattleMons[gActiveBattler].moves[movenum])
							break;
					}

					if (movenum == 4)
					{ // pokemon does not have the disabled move anymore
						gDisableStructs[gActiveBattler].disabledMove = 0;
						gDisableStructs[gActiveBattler].disableTimer = 0;
					}
					else if (--gDisableStructs[gActiveBattler].disableTimer == 0)
					{  // disable ends
						gDisableStructs[gActiveBattler].disabledMove = 0;
						BattleScriptExecute(BattleScript_DisabledNoMore);
						effect++;
					}
				}
				break;

			case ET_Lock_On_Timer:
				if (gStatuses3[gActiveBattler] & STATUS3_ALWAYS_HITS)
					gStatuses3[gActiveBattler] -= 0x8;
				break;

			case ET_Charge_Timer:
				if (gDisableStructs[gActiveBattler].chargeTimer && --gDisableStructs[gActiveBattler].chargeTimer == 0)
					gStatuses3[gActiveBattler] &= ~STATUS3_CHARGED_UP;
				break;

			case ET_Magnet_Rise_Timer:
				if (gBattleStruct->MagnetRiseTimers[gActiveBattler] > 0
				&& BATTLER_ALIVE(gActiveBattler)
				&& --gBattleStruct->MagnetRiseTimers[gActiveBattler] == 0)
				{
					BattleScriptExecute(BattleScript_MagnetRiseEnd);
					effect++;
				}
				break;

			case ET_Telekinesis_Timer:
				if (gBattleStruct->TelekinesisTimers[gActiveBattler] > 0
				&& BATTLER_ALIVE(gActiveBattler)
				&& --gBattleStruct->TelekinesisTimers[gActiveBattler] == 0)
				{
					BattleScriptExecute(BattleScript_TelekinesisEnd);
					effect++;
				}
				break;

			case ET_Heal_Block_Timer:
				if (gBattleStruct->HealBlockTimers[gActiveBattler] > 0
				&& BATTLER_ALIVE(gActiveBattler)
				&& --gBattleStruct->HealBlockTimers[gActiveBattler] == 0)
				{
					BattleScriptExecute(BattleScript_HealBlockEnd);
					effect++;
				}
				break;

			case ET_Embargo_Timer:
				if (gBattleStruct->EmbargoTimers[gActiveBattler] > 0
				&& BATTLER_ALIVE(gActiveBattler)
				&& --gBattleStruct->EmbargoTimers[gActiveBattler] == 0)
				{
					BattleScriptExecute(BattleScript_EmbargoEnd);
					effect++;
				}
				break;

			case ET_Yawn:
				if (gStatuses3[gActiveBattler] & STATUS3_YAWN && BATTLER_ALIVE(gActiveBattler))
				{
					gStatuses3[gActiveBattler] -= 0x800;

					if (!(gStatuses3[gActiveBattler] & STATUS3_YAWN))
					{
						if (CanBePutToSleep(gActiveBattler, FALSE))
						{
							if (!BATTLER_SEMI_INVULNERABLE(gActiveBattler)) //Semi-Invulnerability is removed when it tries to attack
								CancelMultiTurnMoves(gActiveBattler);
							gBattleMons[gActiveBattler].status1 |= (Random() % 3) + 2;
							BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
							MarkBattlerForControllerExec(gActiveBattler);
							gEffectBattler = gActiveBattler;
							BattleScriptExecute(BattleScript_YawnMakesAsleep);
							effect++;
						}
						else if (CheckGrounding(gActiveBattler) && gTerrainType == MISTY_TERRAIN)
						{
							gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
							BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
							effect++;
						}
						else if (CheckGrounding(gActiveBattler) && gTerrainType == ELECTRIC_TERRAIN)
						{
							gBattleStringLoader = gText_TargetWrappedInElectricTerrain;
							BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
							effect++;
						}
					}
				}
				break;

			case ET_Perish_Song:
				if (gStatuses3[gActiveBattler] & STATUS3_PERISH_SONG && BATTLER_ALIVE(gActiveBattler))
				{
					gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
					gBattleTextBuff1[1] = B_TXT_BUFF2;
					gBattleTextBuff1[2] = 1;
					gBattleTextBuff1[3] = 1;
					gBattleTextBuff1[4] = gDisableStructs[gActiveBattler].perishSongTimer;
					gBattleTextBuff1[5] = 0xFF;

					if (gDisableStructs[gActiveBattler].perishSongTimer == 0)
					{
						gStatuses3[gActiveBattler] &= ~(STATUS3_PERISH_SONG);
						gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
						gBattlescriptCurrInstr = BattleScript_PerishSongTakesLife;
					}
					else
					{
						gDisableStructs[gActiveBattler].perishSongTimer--;
						gBattlescriptCurrInstr = BattleScript_PerishSongCountGoesDown;
					}

					BattleScriptExecute(gBattlescriptCurrInstr);
					effect++;
				}
				break;

			case ET_Roost:
				if (gBattleStruct->RoostCounter & gBitTable[gActiveBattler])
				{
					gBattleStruct->RoostCounter &= ~(gBitTable[gActiveBattler]);
					if (gBattleMons[gActiveBattler].type1 == TYPE_NORMAL)
						gBattleMons[gActiveBattler].type1 = TYPE_FLYING;
					if (gBattleMons[gActiveBattler].type2 == TYPE_NORMAL)
						gBattleMons[gActiveBattler].type2 = TYPE_FLYING;
					if (gBattleMons[gActiveBattler].type3 == TYPE_ROOSTLESS)
						gBattleMons[gActiveBattler].type3 = TYPE_FLYING;
				}
				else
				{
					if (gBattleMons[gActiveBattler].type1 == TYPE_ROOSTLESS)
						gBattleMons[gActiveBattler].type1 = TYPE_FLYING;
					if (gBattleMons[gActiveBattler].type2 == TYPE_ROOSTLESS)
						gBattleMons[gActiveBattler].type2 = TYPE_FLYING;
					if (gBattleMons[gActiveBattler].type3 == TYPE_ROOSTLESS)
						gBattleMons[gActiveBattler].type3 = TYPE_FLYING;
				}
				break;

//These Timers all fall through to eachother for quicker game speed
			case ET_Reflect:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					sideBank = gBattleStruct->turnEffectsBattlerId;
					if (gSideStatuses[sideBank] & SIDE_STATUS_REFLECT && --gSideTimers[sideBank].reflectTimer == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = sideBank;
						gSideStatuses[sideBank] &= ~SIDE_STATUS_REFLECT;
						gBattleStringLoader = gText_SideStatusWoreOff;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
						gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
						gBattleTextBuff1[2] = MOVE_REFLECT;
						gBattleTextBuff1[3] = MOVE_REFLECT >> 8;
						gBattleTextBuff1[4] = EOS;
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_Light_Screen:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					sideBank = gBattleStruct->turnEffectsBattlerId;
					if (gSideStatuses[sideBank] & SIDE_STATUS_LIGHTSCREEN && --gSideTimers[sideBank].lightscreenTimer == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = sideBank;
						gSideStatuses[sideBank] &= ~SIDE_STATUS_LIGHTSCREEN;
						gBattleStringLoader = gText_SideStatusWoreOff;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
						gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
						gBattleTextBuff1[2] = MOVE_LIGHTSCREEN;
						gBattleTextBuff1[3] = MOVE_LIGHTSCREEN >> 8;
						gBattleTextBuff1[4] = EOS;
						effect++;
					}
					break;
				}
				else

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_Aurora_Veil:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					if (gBattleStruct->AuroraVeilTimers[gBattleStruct->turnEffectsBattlerId]
					&& --gBattleStruct->AuroraVeilTimers[gBattleStruct->turnEffectsBattlerId] == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = gBattleStruct->turnEffectsBattlerId;
						gBattleStringLoader = gText_SideStatusWoreOff;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
						gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
						gBattleTextBuff1[2] = MOVE_AURORAVEIL & 0xFF;
						gBattleTextBuff1[3] = MOVE_AURORAVEIL >> 8;
						gBattleTextBuff1[4] = EOS;
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_Safeguard:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					sideBank = SIDE(gBattleStruct->turnEffectsBattlerId);
					if (gSideStatuses[sideBank] & SIDE_STATUS_SAFEGUARD && --gSideTimers[sideBank].safeguardTimer == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = sideBank;
						gSideStatuses[sideBank] &= ~SIDE_STATUS_SAFEGUARD;
						BattleScriptExecute(BattleScript_SafeguardEnds);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_Mist:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					sideBank = SIDE(gBattleStruct->turnEffectsBattlerId);
					if ((gSideStatuses[sideBank] & SIDE_STATUS_MIST) && --gSideTimers[sideBank].mistTimer == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = sideBank;
						gSideStatuses[sideBank] &= ~SIDE_STATUS_MIST;
						gBattleStringLoader = gText_SideStatusWoreOff;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
						gBattleTextBuff1[1] = B_TXT_COPY_VAR_1;
						gBattleTextBuff1[2] = MOVE_MIST;
						gBattleTextBuff1[3] = MOVE_MIST >> 8;
						gBattleTextBuff1[4] = EOS;
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_Tailwind_Timer:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					if (gBattleStruct->TailwindTimers[gBattleStruct->turnEffectsBattlerId]
					&& --gBattleStruct->TailwindTimers[gBattleStruct->turnEffectsBattlerId] == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = gBattleStruct->turnEffectsBattlerId;
						BattleScriptExecute(BattleScript_TailwindEnd);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_Lucky_Chant_Timer:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					if (gBattleStruct->LuckyChantTimers[gBattleStruct->turnEffectsBattlerId]
					&& --gBattleStruct->LuckyChantTimers[gBattleStruct->turnEffectsBattlerId] == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = gBattleStruct->turnEffectsBattlerId;
						BattleScriptExecute(BattleScript_LuckyChantEnd);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_Rainbow_Timer:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					if (gBattleStruct->RainbowTimers[gBattleStruct->turnEffectsBattlerId] > 0
					&& --gBattleStruct->RainbowTimers[gBattleStruct->turnEffectsBattlerId] == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = gBattleStruct->turnEffectsBattlerId;
						BattleScriptExecute(BattleScript_RainbowEnd);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_Sea_Of_Fire_Timer:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					if (gBattleStruct->SeaOfFireTimers[gBattleStruct->turnEffectsBattlerId] > 0
					&& --gBattleStruct->SeaOfFireTimers[gBattleStruct->turnEffectsBattlerId] == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = gBattleStruct->turnEffectsBattlerId;
						BattleScriptExecute(BattleScript_SeaOfFireEnd);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_Swamp_Timer:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					if (gBattleStruct->SwampTimers[gBattleStruct->turnEffectsBattlerId] > 0
					&& --gBattleStruct->SwampTimers[gBattleStruct->turnEffectsBattlerId] == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = gBattleStruct->turnEffectsBattlerId;
						BattleScriptExecute(BattleScript_SwampEnd);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_G_Max_VineLash_Timer:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					if (gBattleStruct->maxVineLashTimers[gBattleStruct->turnEffectsBattlerId] > 0
					&& --gBattleStruct->maxVineLashTimers[gBattleStruct->turnEffectsBattlerId] == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = gBattleStruct->turnEffectsBattlerId;
						gBattleStringLoader = gText_GMaxVineLashEnded;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_G_Max_Wildfire_Timer:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					if (gBattleStruct->maxWildfireTimers[gBattleStruct->turnEffectsBattlerId] > 0
					&& --gBattleStruct->maxWildfireTimers[gBattleStruct->turnEffectsBattlerId] == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = gBattleStruct->turnEffectsBattlerId;
						gBattleStringLoader = gText_GMaxWildfireEnded;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_G_Max_Cannonade_Timer:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					if (gBattleStruct->maxCannonadeTimers[gBattleStruct->turnEffectsBattlerId] > 0
					&& --gBattleStruct->maxCannonadeTimers[gBattleStruct->turnEffectsBattlerId] == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = gBattleStruct->turnEffectsBattlerId;
						gBattleStringLoader = gText_GMaxCannonadeEnded;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_G_Max_Volcalith_Timer:
				if (gBattleStruct->turnEffectsBattlerId < 2)
				{
					if (gBattleStruct->maxVolcalithTimers[gBattleStruct->turnEffectsBattlerId] > 0
					&& --gBattleStruct->maxVolcalithTimers[gBattleStruct->turnEffectsBattlerId] == 0)
					{
						gBattlerAttacker = gBattlerTarget = gActiveBattler = gBattleStruct->turnEffectsBattlerId;
						gBattleStringLoader = gText_GMaxVolcalithEnded;
						BattleScriptExecute(BattleScript_PrintCustomStringEnd2);
						effect++;
					}
					break;
				}

				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;

				
			case ET_Trick_Room_Timer:
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;
				if (gBattleStruct->TrickRoomTimer > 0 && --gBattleStruct->TrickRoomTimer == 0)
				{
					BattleScriptExecute(BattleScript_TrickRoomEnd);
					return ++effect;
				}
				
			case ET_Water_Sport_Timer:
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;
				if (gBattleStruct->WaterSportTimer > 0 && --gBattleStruct->WaterSportTimer == 0)
				{
					BattleScriptExecute(BattleScript_WaterSportEnd);
					return ++effect;
				}
				
			case ET_Mud_Sport_Timer:
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;
				if (gBattleStruct->MudSportTimer > 0 && --gBattleStruct->MudSportTimer == 0)
				{
					BattleScriptExecute(BattleScript_MudSportEnd);
					return ++effect;
				}
				
			case ET_Wonder_Room_Timer:
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;
				if (gBattleStruct->WonderRoomTimer > 0 && --gBattleStruct->WonderRoomTimer == 0)
				{
					BattleScriptExecute(BattleScript_WonderRoomEnd);
					return ++effect;
				}
				
			case ET_Magic_Room_Timer:
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;
				if (gBattleStruct->MagicRoomTimer > 0 && --gBattleStruct->MagicRoomTimer == 0)
				{
					BattleScriptExecute(BattleScript_MagicRoomEnd);
					return ++effect;
				}
				
			case ET_Gravity_Timer:
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;
				if (gBattleStruct->GravityTimer > 0 && --gBattleStruct->GravityTimer == 0)
				{
					BattleScriptExecute(BattleScript_GravityEnd);
					return ++effect;
				}
				
			case ET_Seeds: ;
				if (ITEM_EFFECT(gActiveBattler) == HOLD_EFFECT_SEEDS)
				{
					if (ItemBattleEffects(ItemEffects_SwitchIn, gActiveBattler, FALSE, FALSE))
						++effect;
				}
				break;

			case ET_Terrain_Timer:
				gBattleStruct->endTurnBlockState = 0; //For later
				++gBattleStruct->turnEffectsTracker;
				gBattleStruct->turnEffectsBattlerId = 0;
				if (gBattleStruct->TerrainTimer && --gBattleStruct->TerrainTimer == 0)
				{
					BattleScriptExecute(BattleScript_TerrainEnd);
					return ++effect;
				}
				
			case ET_Block_B:
				switch(gBattleStruct->endTurnBlockState) {
					case ET_Uproar:
						if (gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR
						&& BATTLER_ALIVE(gActiveBattler))
						{
							gBattleMons[gActiveBattler].status2 -= 0x10;  // uproar timer goes down
							if (WasUnableToUseMove(gActiveBattler))
							{
								CancelMultiTurnMoves(gActiveBattler);
								gBattleCommunication[MULTISTRING_CHOOSER] = 1;
							}
							else if (gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR)
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = 0;
								gBattleMons[gActiveBattler].status2 |= STATUS2_MULTIPLETURNS;
							}
							else
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = 1;
								CancelMultiTurnMoves(gActiveBattler);
							}

							BattleScriptExecute(BattleScript_PrintUproarOverTurns);
							++effect;
						}
						break;

					case ET_SpeedBoost_Moody_BadDreams_SlowStart:
						if (BATTLER_ALIVE(gActiveBattler))
						{
							gLastUsedAbility = ABILITY(gActiveBattler);
							switch(gLastUsedAbility) {
								case ABILITY_SPEED_BOOST:
								case ABILITY_TRUANT:
								case ABILITY_MOODY:
								case ABILITY_BADDREAMS:
								case ABILITY_SLOWSTART:
									if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
										effect++;
									break;
							}
						}
						break;

					case ET_Orbz:
						if (BATTLER_ALIVE(gActiveBattler)
						&&  gBattleMons[gActiveBattler].status1 == STATUS1_NONE)
						{
							u8 itemEffect = ITEM_EFFECT(gActiveBattler);

							switch (itemEffect) {
								case HOLD_EFFECT_TOXIC_ORB:
									if (CanBePoisoned(gActiveBattler, gActiveBattler, FALSE))
									{
										gLastUsedItem = ITEM(gActiveBattler);
										RecordItemEffectBattle(gActiveBattler, itemEffect);
										gBattleMons[gActiveBattler].status1 |= STATUS1_TOXIC_POISON;
										BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
										MarkBattlerForControllerExec(gActiveBattler);

										gEffectBattler = gActiveBattler;
										BattleScriptExecute(BattleScript_ToxicOrb);
										++effect;
									}
									break;
								case HOLD_EFFECT_FLAME_ORB:
									if (CanBeBurned(gActiveBattler, FALSE))
									{
										gLastUsedItem = ITEM(gActiveBattler);
										RecordItemEffectBattle(gActiveBattler, itemEffect);
										gBattleMons[gActiveBattler].status1 |= STATUS1_BURN;
										BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
										MarkBattlerForControllerExec(gActiveBattler);

										gEffectBattler = gActiveBattler;
										BattleScriptExecute(BattleScript_FlameOrb);
										++effect;
									}
									break;
								case HOLD_EFFECT_STICKY_BARB:
									if (ABILITY(gActiveBattler) != ABILITY_MAGICGUARD)
									{
										gLastUsedItem = ITEM(gBattleScripting.battler = gActiveBattler);
										RecordItemEffectBattle(gActiveBattler, itemEffect);

										gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gActiveBattler) / 8);
										BattleScriptExecute(BattleScript_BlackSludgeHurt);
										++effect;
									}
									break;
							}
						}
						break;

					case ET_Harvest_Pickup:
						if (BATTLER_ALIVE(gActiveBattler) && ITEM(gActiveBattler) == ITEM_NONE)
						{
							switch(ABILITY(gActiveBattler)) {
								case ABILITY_HARVEST:
								case ABILITY_PICKUP:
								case ABILITY_BALLFETCH:
									if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
										effect++;
							}
						}
						break;
				}

				++gBattleStruct->endTurnBlockState;
				if (gBattleStruct->endTurnBlockState < MAX_CASES_BLOCK_B)
					goto SkipBankIncrement;
				else
				{
					gBattleStruct->endTurnBlockState = 0;
					break;
				}

			case ET_Form_Change:
				if (BATTLER_ALIVE(gActiveBattler) && !IS_TRANSFORMED(gActiveBattler))
				{
					struct Pokemon* mon = GetBankPartyData(gActiveBattler);
					u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
					u16 newSpecies = SPECIES_NONE;
					ability_t ability = ABILITY(gActiveBattler);
					u8 itemEffect = ITEM_EFFECT(gActiveBattler);
					bool8 changedForm = FALSE;
					bool8 reloadType = FALSE;
					bool8 reloadStats = FALSE;
					bool8 reloadAbility = FALSE;
					const u8* battleScript = NULL;

					switch(ability) {
						case ABILITY_FORECAST:
							if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
								effect++;
							break;

						case ABILITY_ZENMODE:
							if (gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 2)
							{
								#if (defined SPECIES_DARMANITAN && defined SPECIES_DARMANITANZEN)
								if (species == SPECIES_DARMANITAN)
								{
									newSpecies = SPECIES_DARMANITANZEN;
									changedForm = TRUE;
									reloadType = TRUE;
									reloadStats = TRUE;
									battleScript = BattleScript_TransformedEnd2;
								}
								#endif
								#if (defined SPECIES_DARMANITAN_G && defined SPECIES_DARMANITAN_G_ZEN)
								if (species == SPECIES_DARMANITAN_G)
								{
									newSpecies = SPECIES_DARMANITAN_G_ZEN;
									changedForm = TRUE;
									reloadType = TRUE;
									reloadStats = TRUE;
									battleScript = BattleScript_TransformedEnd2;
								}
								#endif
							}
							else //gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 2
							{
								//Revert if back above half health
								#if (defined SPECIES_DARMANITAN && defined SPECIES_DARMANITANZEN)
								if (species == SPECIES_DARMANITANZEN)
								{
									newSpecies = SPECIES_DARMANITAN;
									changedForm = TRUE;
									reloadType = TRUE;
									reloadStats = TRUE;
									battleScript = BattleScript_TransformedEnd2;
								}
								#endif
								#if (defined SPECIES_DARMANITAN_G && defined SPECIES_DARMANITAN_G_ZEN)
								if (species == SPECIES_DARMANITAN_G_ZEN)
								{
									newSpecies = SPECIES_DARMANITAN_G;
									changedForm = TRUE;
									reloadType = TRUE;
									reloadStats = TRUE;
									battleScript = BattleScript_TransformedEnd2;
								}
								#endif
							}
							break;

						#if (defined SPECIES_ZYGARDE && defined SPECIES_ZYGARDE_10 && defined SPECIES_ZYGARDE_COMPLETE)
						case ABILITY_POWERCONSTRUCT:
							if ((species == SPECIES_ZYGARDE || species == SPECIES_ZYGARDE_10)
							&& gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 2)
							{
								newSpecies = SPECIES_ZYGARDE_COMPLETE;
								changedForm = TRUE;
								reloadType = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_PowerConstruct;
							}
							break;
						#endif

						#if (defined SPECIES_WISHIWASHI && defined SPECIES_WISHIWASHI_S)
						case ABILITY_SCHOOLING:
							if (species == SPECIES_WISHIWASHI && gBattleMons[gActiveBattler].level >= 20
							&& gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP / 4)
							{
								newSpecies = SPECIES_WISHIWASHI_S;
								changedForm = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_StartedSchooling;
							}
							else if (species == SPECIES_WISHIWASHI_S
								&&  (gBattleMons[gActiveBattler].level < 20
								  || gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 4))
							{
								newSpecies = SPECIES_WISHIWASHI;
								changedForm = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_StoppedSchooling;
							}
							break;
						#endif

						#ifdef SPECIES_MINIOR_SHIELD
						case ABILITY_SHIELDSDOWN:
							if (species == SPECIES_MINIOR_SHIELD
							&& gBattleMons[gActiveBattler].hp <= gBattleMons[gActiveBattler].maxHP / 2)
							{
								newSpecies = GetMiniorCoreSpecies(mon);
								changedForm = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_ShieldsDownToCore;
							}
							else if (CheckTableForSpecies(species, gMiniorCores)
							&& gBattleMons[gActiveBattler].hp > (gBattleMons[gActiveBattler].maxHP / 2))
							{
								newSpecies = SPECIES_MINIOR_SHIELD;
								changedForm = TRUE;
								reloadStats = TRUE;
								battleScript = BattleScript_ShieldsDownToMeteor;
							}
							break;
						#endif

						#if (defined SPECIES_CHERRIM && defined SPECIES_CHERRIM_SUN)
						case ABILITY_FLOWERGIFT:
							switch (species) {
								case SPECIES_CHERRIM:
									if (WEATHER_HAS_EFFECT
									&& (gBattleWeather & WEATHER_SUN_ANY)
									&& itemEffect != HOLD_EFFECT_UTILITY_UMBRELLA)
									{
										newSpecies = SPECIES_CHERRIM_SUN;
										changedForm = TRUE;
										battleScript = BattleScript_FlowerGiftEnd2;
									}
									break;
								case SPECIES_CHERRIM_SUN:
									if (!WEATHER_HAS_EFFECT
									|| !(gBattleWeather & WEATHER_SUN_ANY)
									//|| ability != ABILITY_FLOWERGIFT //Not neccessary
									|| itemEffect == HOLD_EFFECT_UTILITY_UMBRELLA)
									{
										newSpecies = SPECIES_CHERRIM;
										changedForm = TRUE;
										battleScript = BattleScript_FlowerGiftEnd2;
									}
									break;
							}
						#endif
							break;

						#if (defined SPECIES_MORPEKO && defined SPECIES_MORPEKO_HANGRY)
						case ABILITY_HUNGERSWITCH:
							if (species == SPECIES_MORPEKO)
							{
								newSpecies = SPECIES_MORPEKO_HANGRY;
								changedForm = TRUE;
								battleScript = BattleScript_FlowerGiftEnd2;
							}
							else if (species == SPECIES_MORPEKO_HANGRY)
							{
								newSpecies = SPECIES_MORPEKO;
								changedForm = TRUE;
								battleScript = BattleScript_FlowerGiftEnd2;
							}
						#endif
							break;
					}

					if (changedForm)
					{
						gBattleScripting.battler = gActiveBattler;
						DoFormChange(gActiveBattler, newSpecies, reloadType, reloadStats, reloadAbility);
						BattleScriptExecute(battleScript);
						++effect;
					}
				}
				break;

			case ET_Reactivate_Overworld_Weather:
				gBattleStruct->turnEffectsBattlerId = gBattlersCount;
				if (gBattleWeather == 0 && AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, 0, 0, 0xFF, 0))
				{
					++effect;
					return effect;
				}

				if (gTerrainType == 0 && TryActivateOWTerrain())
				{
					++effect;
					return effect;
				}
				break;

			case ET_End:
			END_TURN_SKIP:
				gBattleStruct->turnEffectsBattlerId = gBattlersCount;
				gBattleStruct->endTurnDone = TRUE;
				gBattleStruct->megaData.state = 0;
				gBattleStruct->calculatedAIPredictions = FALSE;
				gBattleStruct->batonPassing = FALSE;
				gBattleStruct->NoMoreMovingThisTurn = 0; //May be set during end turn Emergency Exit
				gBattleStruct->ragePowdered = 0;
				gBattleStruct->dynamaxData.attackAgain = FALSE;
				gBattleStruct->dynamaxData.repeatedAttacks = 0;
				gBattleStruct->ai.sideSwitchedThisRound = 0;

				if (gBattleStruct->IonDelugeTimer) //Cleared down here b/c necessary for future attacks
					--gBattleStruct->IonDelugeTimer;

				for (i = 0; i < gBattlersCount; ++i)
				{
					gBattleStruct->pickupStack[i] = 0xFF;
					gBattleStruct->leftoverHealingDone[i] = FALSE;
					gBattleStruct->statRoseThisRound[i] = FALSE;
					gBattleStruct->statFellThisRound[i] = FALSE;
					gBattleStruct->turnDamageTaken[i] = 0;
					gBattleStruct->ai.calculatedAISwitchings[i] = FALSE;
					gBattleStruct->recalculatedBestDoublesKillingScores[i] = FALSE;
					gBattleStruct->ai.fightingStyle[i] = 0xFF;
					gBattleStruct->ai.megaPotential[i] = NULL;

					if (gBattleStruct->metronomeItemBonus[i] > 0)
						--gBattleStruct->metronomeItemBonus[i];

					if (gBattleStruct->dynamaxData.timer[i] > 0)
					{
						if (--gBattleStruct->dynamaxData.timer[i] == 0)
							gBattleStruct->dynamaxData.timer[i] = -1; //Indicator to revert
					}

					for (j = 0; j < gBattlersCount; ++j)
					{
						int k;
						gBattleStruct->ai.strongestMove[i][j] = 0xFFFF;
						gBattleStruct->ai.canKnockOut[i][j] = 0xFF;
						gBattleStruct->ai.can2HKO[i][j] = 0xFF;
						gBattleStruct->ai.onlyBadMovesLeft[i][j] = 0xFF;
						gBattleStruct->ai.shouldFreeChoiceLockWithDynamax[i][j] = FALSE;
						gBattleStruct->ai.dynamaxPotential[i][j] = FALSE;

						for (k = 0; k < MAX_MON_MOVES; ++k)
						{
							gBattleStruct->ai.damageByMove[i][j][k] = 0xFFFFFFFF;
							gBattleStruct->ai.moveKnocksOut1Hit[i][j][k] = 0xFF;
							gBattleStruct->ai.moveKnocksOut2Hits[i][j][k] = 0xFF;
						}
					}
				}
		}
		gBattleStruct->turnEffectsBattlerId++;

	SkipBankIncrement:
		if (gBattleStruct->turnEffectsBattlerId >= gBattlersCount)
		{
			gBattleStruct->turnEffectsTracker++;
			gBattleStruct->turnEffectsBattlerId = 0;
		}

		if (effect)
			return effect;
	}

	gHitMarker &= ~(HITMARKER_GRUDGE | HITMARKER_x20);
	return 0;
}

bool8 HandleWishPerishSongOnTurnEnd(void)
{
    	u8 bank;

	if (IsRaidBattle()
	#ifdef FLAG_RAID_BATTLE_NO_FORCE_END
	&& !FlagGet(FLAG_RAID_BATTLE_NO_FORCE_END) //These battles can't be force ended
	&& BATTLER_ALIVE(BANK_RAID_BOSS) //Don't force out if battle is over
	#endif
	&& gBattleResults.battleTurnCounter + 1 >= 10) //10 Turns have passed
	{
		gBattleStringLoader = gText_RaidBattleKO4;

		if (BATTLER_ALIVE(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)))
			gBattleScripting.battler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
		else
			gBattleScripting.battler = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
		BattleScriptExecute(BattleScript_LoseRaidBattle);
		gBattleResults.battleTurnCounter -= 1; //To prevent infinite loop
		return TRUE;
	}

	if (!RAID_BATTLE_END)
	{
		for (bank = 0; bank < gBattlersCount; ++bank)
		{
			if (gBattleStruct->dynamaxData.timer[bank] == -1) //Revert
			{
				TryRevertBankGigantamax(bank);
				gBattleStruct->dynamaxData.timer[bank] = 0;
				gBattleScripting.battler = bank;
				BattleScriptExecute(BattleScript_DynamaxEnd);
				return TRUE;
			}
		}
	}

	return FALSE;
}

#define FAINTED_ACTIONS_MAX_CASE 11

bool8 HandleFaintedMonActions(void)
{
	int i, j;

	if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
		return FALSE;

	do
	{
		switch (gBattleStruct->faintedActionsState) {
			case 0:
				gBattleStruct->faintedActionsBattlerId = 0;
				gBattleStruct->faintedActionsState++;
				for (i = 0; i < gBattlersCount; i++)
				{
					if (gAbsentBattlerFlags & gBitTable[i] && !HasNoMonsToSwitch(i))
						gAbsentBattlerFlags &= ~(gBitTable[i]);
				}


			case 1:
				do
				{
					gBattlerFainted = gBattlerTarget = gBattleStruct->faintedActionsBattlerId;
					if (gBattleMons[gBattleStruct->faintedActionsBattlerId].hp == 0
					&& !(gBattleStruct->givenExpMons & gBitTable[gBattlerPartyIndexes[gBattleStruct->faintedActionsBattlerId]])
					&& !(gAbsentBattlerFlags & gBitTable[gBattleStruct->faintedActionsBattlerId]))
					{
						BattleScriptExecute(BattleScript_GiveExp);
						gBattleStruct->faintedActionsState = 2;
						return TRUE;
					}
				} while (++gBattleStruct->faintedActionsBattlerId != gBattlersCount);

				gBattleStruct->faintedActionsState = 3;
				break;

			case 2:
				OpponentSwitchInResetSentPokesToOpponentValue(gBattlerFainted);
				if (++gBattleStruct->faintedActionsBattlerId >= gBattlersCount)
					gBattleStruct->faintedActionsState = 3;
				else
					gBattleStruct->faintedActionsState = 1;
				break;

			case 3:
				gBattleStruct->faintedActionsBattlerId = 0;
				gBattleStruct->faintedActionsState++;


			case 4:
				do
				{
					gBattlerFainted = gBattlerTarget = gBattleStruct->faintedActionsBattlerId;
					BackupSwitchingBank();
					if (gBattleMons[gBattleStruct->faintedActionsBattlerId].hp == 0)
					{
						if (IsCatchableRaidBattle()
						&& gBattleStruct->faintedActionsBattlerId == BANK_RAID_BOSS)
							continue; //Don't end the battle if the raid mon is KOed

						if (RAID_BATTLE_END
						&& SIDE(gBattleStruct->faintedActionsBattlerId) == B_SIDE_PLAYER
						&& ViableMonCountFromBank(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)) >= 1)
							continue; //Don't end the raid battle if the player is KOd but partner is still alive to help catch mon

						if (!(gAbsentBattlerFlags & gBitTable[gBattleStruct->faintedActionsBattlerId])) //Bank was just emptied
						{
							gAbsentBattlerFlags |= gBitTable[gBattleStruct->faintedActionsBattlerId]; //Makes the game realize the target is dead for now, so no attacking it
							gBattleStruct->AbsentBattlerHelper |= gBitTable[gBattleStruct->faintedActionsBattlerId]; //Record which Pokemon need replacements
						}

						if (gBattleStruct->AbsentBattlerHelper & gBitTable[gBattleStruct->faintedActionsBattlerId])
						{
							if (gBattleStruct->endTurnDone
							||  ViableMonCountFromBank(gBattleStruct->faintedActionsBattlerId) == 0)
							{
								gAbsentBattlerFlags &= ~(gBitTable[gBattleStruct->faintedActionsBattlerId]);

								//Not wild battle
								//In a double or frontier battle, the player is not asked if they want to switch Pokemon
								if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))
								{
									gBattleStruct->handleSetSwitchIns |= gBitTable[gBattleStruct->faintedActionsBattlerId];
									BattleScriptExecute(BattleScript_HandleFaintedMonDoublesInitial);
								}
								else
								{
									BattleScriptExecute(BattleScript_HandleFaintedMonSingles);
									gBattleStruct->doSwitchInEffects |= gBitTable[gBattleStruct->faintedActionsBattlerId];
									gBattleStruct->ai.switchingCooldown[gBattleStruct->faintedActionsBattlerId] = 1; //AI shouldn't switch out again until after the next time an end turn is reached
									gBattleStruct->handlingFaintSwitching = TRUE;

									for (i = 0; i < gBattlersCount; ++i)
									{
										if (gBattleStruct->AbsentBattlerHelper & gBitTable[i]
										&&  !(gBattleStruct->doSwitchInEffects & gBitTable[i])
										&&  HITMARKER_FAINTED(i))
										{
											if (++gBattleStruct->faintedActionsBattlerId >= gBattlersCount)
												gBattleStruct->faintedActionsState = 7;
											return TRUE; //Still more Pokemon that need to switch in (eg after Explosion)
										}
									}

									if (gBattlersCount & 1) //Odd number
										gAbsentBattlerFlags |= gBitTable[gBattlersCount]; //So switching doesn't crash in Raids

									if (++gBattleStruct->faintedActionsBattlerId >= gBattlersCount)
										gBattleStruct->faintedActionsState = 7;
									return TRUE;
								}

								gBattleStruct->faintedActionsState = 5;
								return TRUE;
							}
						}
					}
				} while (++gBattleStruct->faintedActionsBattlerId < gBattlersCount);

				if (gBattleStruct->handleSetSwitchIns)
				{
					gBattleStruct->faintedActionsBattlerId = 0;
					gBattleStruct->faintedActionsState = 6;
				}
				else if (gBattleStruct->doSwitchInEffects)
					gBattleStruct->faintedActionsState = 7;
				else
					gBattleStruct->faintedActionsState = FAINTED_ACTIONS_MAX_CASE - 1;
				break;

			case 5:
				if (++gBattleStruct->faintedActionsBattlerId >= gBattlersCount)
				{
					if (gBattleStruct->handleSetSwitchIns)
					{
						gBattleStruct->faintedActionsBattlerId = 0;
						gBattleStruct->faintedActionsState = 6;
					}
					else if (gBattleStruct->doSwitchInEffects)
						gBattleStruct->faintedActionsState = 7;
					else
						gBattleStruct->faintedActionsState = FAINTED_ACTIONS_MAX_CASE - 1;
				}
				else
					gBattleStruct->faintedActionsState = 4;
				break;

			case 6:
				do
				{
					gBattlerFainted = gBattlerTarget = gBattleStruct->faintedActionsBattlerId;
					BackupSwitchingBank();
					if (gBattleStruct->handleSetSwitchIns & gBitTable[gBattleStruct->faintedActionsBattlerId])
					{
						++gBattleStruct->faintedActionsBattlerId;
						gAbsentBattlerFlags &= ~(gBitTable[gBattleStruct->faintedActionsBattlerId]);
						gBattleStruct->ai.switchingCooldown[gBattlerFainted] = 1; //AI shouldn't switch out again until after the next time an end turn is reached
						BattleScriptExecute(BattleScript_HandleFaintedMonDoublesPart2);
						return TRUE;
					}
				} while (++gBattleStruct->faintedActionsBattlerId < gBattlersCount);


			case 7:
				//Recalc turn order for switch-in abilities
				for (i = 0; i < gBattlersCount; ++i)
				{
					if (gBattleStruct->doSwitchInEffects & gBitTable[i])
						gBattleStruct->handleSetSwitchIns |= gBitTable[i];

					if (!BATTLER_ALIVE(i))
						gAbsentBattlerFlags |= gBitTable[i];

					gBattlerByTurnOrder[i] = i;
				}
				gBattleStruct->doSwitchInEffects = FALSE;

				for (i = 0; i < gBattlersCount - 1; ++i)
				{
					for (j = i + 1; j < gBattlersCount; ++j)
					{
						if (GetWhoStrikesFirst(gBattlerByTurnOrder[i], gBattlerByTurnOrder[j], 1))
							SwapTurnOrder(i, j);
					}
				}

				gBattleStruct->faintedActionsBattlerId = 0;
				gBattleStruct->faintedActionsState++;


			case 8:
				do
					{
						gBattlerFainted = gBattlerTarget = gBattleStruct->faintedActionsBattlerId;
						BackupSwitchingBank();
						if (gBattleStruct->handleSetSwitchIns & gBitTable[gBattlerFainted])
						{
							if (ABILITY(gBattlerFainted) == ABILITY_TRUANT)
								gDisableStructs[gBattlerFainted].truantCounter = 1; //So it gets unset during the switch in effects

							gBattleStruct->handleSetSwitchIns &= ~(gBitTable[gBattlerFainted]);
							BattleScriptExecute(BattleScript_HandleFaintedMonDoublesSwitchInEffects);
							return TRUE;
						}
					} while (++gBattleStruct->faintedActionsBattlerId < gBattlersCount);
				gBattleStruct->faintedActionsState++;


			case 9:
				if (gBattleStruct->restartEndTurnSwitching) //Died from entry hazards
				{
					gBattleStruct->restartEndTurnSwitching = FALSE;
					gBattleStruct->faintedActionsState = 0;
				}
				else
					gBattleStruct->faintedActionsState++;
				break;

			case 10:
				gBattleStruct->endTurnDone = FALSE;
				gBattleStruct->faintedActionsState++;
				break;

			case FAINTED_ACTIONS_MAX_CASE:
				gBattleStruct->handlingFaintSwitching = FALSE;
				break;
		}
	} while (gBattleStruct->faintedActionsState != FAINTED_ACTIONS_MAX_CASE);

	return FALSE;
}

void TryClearRageStatuses(void)
{
    s32 i;

    for (i = 0; i < gBattlersCount; ++i)
        if ((gBattleMons[i].status2 & STATUS2_RAGE) && gChosenMoveByBattler[i] != MOVE_RAGE)
            gBattleMons[i].status2 &= ~(STATUS2_RAGE);
}


u8 AbilityBattleEffects(u8 caseID, u8 bank, ability_t ability, u8 special, u16 moveArg)
{
    int i;
	u8 effect = 0;
	u16 speciesAtk;
	u16 speciesDef;
	u32 pidAtk;
	u32 pidDef;
	u16 move;
	u8 moveType;
	u8 side;
	u8 target1;

    if (gBattleTypeFlags & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_OLD_MAN_TUTORIAL))
		return FALSE;
	
	if (gBattlerAttacker >= gBattlersCount)
		gBattlerAttacker = bank;
	
	speciesAtk = gBattleMons[gBattlerAttacker].species;
	pidAtk = gBattleMons[gBattlerAttacker].personality;

	speciesDef = gBattleMons[gBattlerTarget].species;
	pidDef = gBattleMons[gBattlerTarget].personality;

	if (special)
		gLastUsedAbility = special;
	else
		gLastUsedAbility = ABILITY(bank);
	
	if (moveArg)
		move = moveArg;
	else
		move = gCurrentMove;

	moveType = gBattleStruct->dynamicMoveType;

    #ifndef NO_GHOST_BATTLES
		if ((gBattleTypeFlags & (BATTLE_TYPE_GHOST_UNVEILED | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST)
		{
			if (SIDE(bank) == B_SIDE_OPPONENT)
				return FALSE; //Ghost's abilities don't activate

			switch (gLastUsedAbility) {		//All of these abilities either use or make changes to
				case ABILITY_INTIMIDATE:	//the unidentified Ghost. In FR, only Intimidate and
				case ABILITY_TRACE:			//Trace were included in this list. It has thus been
				case ABILITY_DOWNLOAD:		//been expanded to support newer abilities.
				case ABILITY_UNNERVE:
				case ABILITY_ANTICIPATION:
				case ABILITY_FOREWARN:
				case ABILITY_FRISK:
				case ABILITY_IMPOSTER:
					return FALSE;
			}
		}
	#endif

	if (gBattleStruct->skipCertainSwitchInAbilities)
	{
		gBattleStruct->skipCertainSwitchInAbilities = FALSE;

		switch (gLastUsedAbility) {
			case ABILITY_INTIMIDATE:
			case ABILITY_DOWNLOAD:
			case ABILITY_FOREWARN:
			case ABILITY_IMPOSTER:
			case ABILITY_ANTICIPATION:
			case ABILITY_FRISK:
				gStatuses3[bank] |= STATUS3_INTIMIDATE_POKES;
				break;
			case ABILITY_TRACE: //Trace is the only ability that activates after a U-Turn + faint switch-in
				return FALSE;
		}
	}

    switch (caseID)
    {
    case ABILITYEFFECT_ON_SWITCHIN: // 0
        gBattleScripting.battler = bank;

        if (gStatuses3[bank] & STATUS3_INTIMIDATE_POKES
		&& gLastUsedAbility != ABILITYEFFECT_SWITCH_IN_WEATHER)
			break;

        switch (gLastUsedAbility)
        {
        case ABILITYEFFECT_SWITCH_IN_WEATHER:
            switch (GetCurrentWeather())
            {
            case WEATHER_RAIN:
            case WEATHER_RAIN_THUNDERSTORM:
                if (!(gBattleWeather & WEATHER_RAIN_ANY))
				{
					gBattleWeather = (WEATHER_RAIN_TEMPORARY | WEATHER_RAIN_PERMANENT);
					gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
					effect++;
				}
				break;
            case WEATHER_DOWNPOUR:
                if (!(gBattleWeather & WEATHER_RAIN_ANY))
                {
                    gBattleWeather = (WEATHER_RAIN_DOWNPOUR | WEATHER_RAIN_PERMANENT);
					gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
                    effect++;
                }
                break;
            case WEATHER_SANDSTORM:
                if (!(gBattleWeather & WEATHER_SANDSTORM_ANY))
				{
					gBattleWeather = (WEATHER_SANDSTORM_PERMANENT | WEATHER_SANDSTORM_TEMPORARY);
					gBattleScripting.animArg1 = B_ANIM_SANDSTORM_CONTINUES;
					effect++;
				}
				break;
            case WEATHER_DROUGHT:
                if (!(gBattleWeather & WEATHER_SUN_ANY))
				{
					gBattleWeather = (WEATHER_SUN_PERMANENT | WEATHER_SUN_TEMPORARY);
					gBattleScripting.animArg1 = B_ANIM_SUN_CONTINUES;
					effect++;
				}
				break;
            
            #ifdef HAIL_IN_BATTLE
				case WEATHER_STEADY_SNOW:
					if (!(gBattleWeather & WEATHER_HAIL_ANY))
					{
						gBattleWeather = (WEATHER_HAIL_PERMANENT | WEATHER_HAIL_TEMPORARY);
						gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
						effect++;
					}
					break;
			#endif

            #ifdef FOG_IN_BATTLE
				#ifdef FOG_IN_BATTLE_1
				case WEATHER_FOG_HORIZONTAL:
				#endif
				#ifdef FOG_IN_BATTLE_2
				case WEATHER_FOG_DIAGONAL:
				#endif
				#ifdef FOG_IN_BATTLE_3
				case WEATHER_FOG_3:
				#endif

				#if defined FOG_IN_BATTLE_1 || defined FOG_IN_BATTLE_2 || defined FOG_IN_BATTLE_3 //So dumb people don't cause compilation errors
					if (!(gBattleWeather & WEATHER_FOG_ANY) && !gBattleStruct->fogBlownAwayByDefog)
					{
						gBattleStringLoader = gText_FogIsDeep;
						gBattleWeather = (WEATHER_FOG_TEMPORARY | WEATHER_FOG_PERMANENT);
						gBattleScripting.animArg1 = B_ANIM_FOG_CONTINUES;
						effect++;
					}
				#endif
			#endif
            }

            if (effect)
			{
				if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_WEATHER)
					gBattleWeather |= WEATHER_CIRCUS; //Can't be removed

				gBattleCommunication[MULTISTRING_CHOOSER] = GetCurrentWeather();
				BattleScriptPushCursorAndCallback(BattleScript_OverworldWeatherStarts);
			}
			break;

        case ABILITY_DRIZZLE:
            if (!(gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
			{
				effect = ActivateWeatherAbility(WEATHER_RAIN_PERMANENT | WEATHER_RAIN_TEMPORARY,
												HOLD_EFFECT_DAMP_ROCK, bank, B_ANIM_RAIN_CONTINUES, 0, FALSE);
			}

			break;
        case ABILITY_SAND_STREAM:
            if (!(gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
			{
				effect = ActivateWeatherAbility(WEATHER_SANDSTORM_PERMANENT | WEATHER_SANDSTORM_TEMPORARY,
												HOLD_EFFECT_SMOOTH_ROCK, bank, B_ANIM_SANDSTORM_CONTINUES, 1, FALSE);
			}

			break;
        case ABILITY_DROUGHT:
            if (!(gBattleWeather & (WEATHER_SUN_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
			{
				effect = ActivateWeatherAbility(WEATHER_SUN_PERMANENT | WEATHER_SUN_TEMPORARY,
												HOLD_EFFECT_HEAT_ROCK, bank, B_ANIM_SUN_CONTINUES, 2, FALSE);
			}

			break;
        case ABILITY_SNOWWARNING:
			if (!(gBattleWeather & (WEATHER_HAIL_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
			{
				gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
				effect = ActivateWeatherAbility(WEATHER_HAIL_PERMANENT | WEATHER_HAIL_TEMPORARY,
												HOLD_EFFECT_ICY_ROCK, bank, B_ANIM_HAIL_CONTINUES, 3, FALSE);
			}

			break;

		case ABILITY_PRIMORDIALSEA:
			if (!(gBattleWeather & (WEATHER_RAIN_PRIMAL | WEATHER_CIRCUS)))
			{
				gBattleWeather = (WEATHER_RAIN_PERMANENT | WEATHER_RAIN_TEMPORARY |  WEATHER_RAIN_PRIMAL);
				gWishFutureKnock.weatherDuration = 0;
				gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
				gBattleStringLoader = gText_PrimordialSeaActivate;
				gBattleCommunication[MULTISTRING_CHOOSER] = 4;
				BattleScriptPushCursorAndCallback(BattleScript_NewWeatherAbilityActivates);
				effect++;
			}

			break;

		case ABILITY_DESOLATELAND:
			if (!(gBattleWeather & (WEATHER_SUN_PRIMAL | WEATHER_CIRCUS)))
			{
				gBattleWeather = (WEATHER_SUN_PERMANENT | WEATHER_SUN_TEMPORARY |  WEATHER_SUN_PRIMAL);
				gWishFutureKnock.weatherDuration = 0;
				gBattleScripting.animArg1 = B_ANIM_SUN_CONTINUES;
				gBattleStringLoader = gText_DesolateLandActivate;
				gBattleCommunication[MULTISTRING_CHOOSER] = 4;
				BattleScriptPushCursorAndCallback(BattleScript_NewWeatherAbilityActivates);
				effect++;
			}

			break;

		case ABILITY_DELTASTREAM:
			if (!(gBattleWeather & (WEATHER_AIR_CURRENT_PRIMAL | WEATHER_CIRCUS)))
			{
				gBattleWeather = (WEATHER_AIR_CURRENT_PRIMAL);
				gWishFutureKnock.weatherDuration = 0;
				gBattleScripting.animArg1 = B_ANIM_STRONG_WINDS_CONTINUE;
				gBattleStringLoader = gText_DeltaStream;
				gBattleCommunication[MULTISTRING_CHOOSER] = 4;
				BattleScriptPushCursorAndCallback(BattleScript_NewWeatherAbilityActivates);
				effect++;
			}

			break;

        case ABILITY_INTIMIDATE:
            if (!(gBattleMons[FOE(bank)].status2 & STATUS2_SUBSTITUTE) || !(gBattleMons[PARTNER(FOE(bank))].status2 & STATUS2_SUBSTITUTE))
			{
				BattleScriptPushCursorAndCallback(BattleScript_IntimidateActivatesEnd3);
				gBattleStruct->intimidateBattler = bank;
				gBattleStruct->intimidateActive = bank + 1;
				effect++;
			}
			break;
        case ABILITY_FORECAST:
            effect = CastformDataTypeChange(bank);
			if (effect != 0)
			{
				BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
				gBattleStruct->formToChangeInto = effect - 1;
			}
			break;
        case ABILITY_TRACE:
        {
            u8 target2;
			side = (GetBattlerPosition(bank) ^ BIT_SIDE) & BIT_SIDE; // side of the opposing pokemon
			target1 = GetBattlerAtPosition(side);
			target2 = GetBattlerAtPosition(side + BIT_FLANK);
			if (IS_DOUBLE_BATTLE)
			{
				if (*GetAbilityLocation(target1) != ABILITY_NONE && gBattleMons[target1].hp != 0
				&& *GetAbilityLocation(target2) != ABILITY_NONE && gBattleMons[target2].hp != 0)
				{
					gActiveBattler = GetBattlerAtPosition(((Random() & 1) * 2) | side);
					effect++;
				}
				else if (*GetAbilityLocation(target1) != ABILITY_NONE && gBattleMons[target1].hp != 0)
				{
					gActiveBattler = target1;

					effect++;
				}
				else if (*GetAbilityLocation(target2) != ABILITY_NONE && gBattleMons[target2].hp != 0)
				{
					gActiveBattler = target2;
					effect++;
				}
			}
			else //Single Battle
			{
				if (*GetAbilityLocation(target1) && gBattleMons[target1].hp)
				{
					gActiveBattler = target1;
					effect++;
				}
			}

			if (effect)
			{
				if (!CheckTableForAbility(*GetAbilityLocation(gActiveBattler), gTraceBannedAbilities))
				{
					gBattlerAttacker = bank;
					*GetAbilityLocation(bank) = *GetAbilityLocation(gActiveBattler);
					gLastUsedAbility = *GetAbilityLocation(gActiveBattler);
					BattleScriptPushCursorAndCallback(BattleScript_TraceActivates);

					PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gActiveBattler, gBattlerPartyIndexes[gActiveBattler])
					PREPARE_ABILITY_BUFFER(gBattleTextBuff2, gLastUsedAbility)
				}
				else
					--effect;
			}
        }
			break;

        case ABILITY_CLOUD_NINE:
        case ABILITY_AIR_LOCK:
            gBattleStringLoader = gText_AirLockActivate;
			BattleScriptPushCursorAndCallback(BattleScript_AirLock);
			effect++;
			break;
        case ABILITY_PRESSURE:
			gBattleStringLoader = gText_PressureActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_MOLDBREAKER:
			gBattleStringLoader = gText_MoldBreakerActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_TERAVOLT:
			gBattleStringLoader = gText_TeravoltActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_TURBOBLAZE:
			gBattleStringLoader = gText_TurboblazeActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_SLOWSTART:
			gBattleStruct->SlowStartTimers[bank] = 5;
			gBattleStringLoader = gText_SlowStartActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_UNNERVE:
			gBattlerAttacker = bank;
			gBattleStringLoader = gText_UnnerveActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_DARKAURA:
			gBattleStringLoader = gText_DarkAuraActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_FAIRYAURA:
			gBattleStringLoader = gText_FairyAuraActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_AURABREAK:
			gBattleStringLoader = gText_AuraBreakActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;

		case ABILITY_COMATOSE:
			gBattleStringLoader = gText_ComatoseActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;
        case ABILITY_ANTICIPATION:
			for (i = 0; i < 4; ++i)
			{
				if (gBattleMons[FOE(bank)].hp)
				{
					move = gBattleMons[FOE(bank)].moves[i];

					if (gBattleMoves[move].effect == EFFECT_OHKO)
					{
						++effect;
						break;
					}

					moveType = gBattleMoves[move].type;
					if (move == MOVE_HIDDEN_POWER)
						moveType = GetExceptionMoveType(FOE(bank), move);

					if (MOVE_RESULT_SUPER_EFFECTIVE &
						NewTypeCalc(move, FOE(bank), bank, 0, 0))
					{
						++effect;
						break;
					}
				}

				if (IS_DOUBLE_BATTLE
				&&  gBattleMons[PARTNER(FOE(bank))].hp)
				{
					move = gBattleMons[PARTNER(FOE(bank))].moves[i];

					if (gBattleMoves[move].effect == EFFECT_OHKO)
					{
						++effect;
						break;
					}

					moveType = gBattleMoves[move].type;
					if (move == MOVE_HIDDEN_POWER)
						moveType = GetExceptionMoveType(PARTNER(FOE(bank)), move);

					if (MOVE_RESULT_SUPER_EFFECTIVE &
						NewTypeCalc(move, PARTNER(FOE(bank)), bank, 0, 0))
					{
						++effect;
						break;
					}
				}
			}

			if (effect)
			{
				gBattleStringLoader = gText_AnticipationActivate;
				BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			}

			break;

        case ABILITY_FOREWARN: 
        {
			u16 strongestMove = MOVE_NONE;
			u8 maxPower = 0;
			u8 strongestTarget = FOE(bank);
			u16 power;

			for (i = 0; i < MAX_MON_MOVES; ++i)
			{
				if (gBattleMons[FOE(bank)].hp != 0)
				{
					move = gBattleMons[FOE(bank)].moves[i];
					if (move != MOVE_NONE)
					{
						power = CalcMovePowerForForewarn(move);

						if (strongestMove == MOVE_NONE)
						{
							strongestMove = move;
							maxPower = power;
							strongestTarget = FOE(bank);
						}
						else if (power > maxPower
						|| (power == maxPower && Random() & 1))
						{
							maxPower = power;
							strongestMove = move;
							strongestTarget = FOE(bank);
						}
					}
				}

				if (IS_DOUBLE_BATTLE
				&&  gBattleMons[PARTNER(FOE(bank))].hp)
				{
					move = gBattleMons[PARTNER(FOE(bank))].moves[i];
					if (move != MOVE_NONE)
					{
						power = CalcMovePowerForForewarn(move);

						if (strongestMove == MOVE_NONE)
						{
							strongestMove = move;
							maxPower = power;
							strongestTarget = PARTNER(FOE(bank));
						}
						else if (power > maxPower
						|| (power == maxPower && Random() & 1))
						{
							maxPower = power;
							strongestMove = move;
							strongestTarget = PARTNER(FOE(bank));
						}
					}
				}
			}

			gBattlerAttacker = bank;
			gEffectBattler = strongestTarget;
			PREPARE_MOVE_BUFFER(gBattleTextBuff1, strongestMove);
			gBattleStringLoader = gText_ForewarnActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
        }
			break;
        case ABILITY_FRISK: 
			{
                u8 foe = SIDE(bank) ^ BIT_SIDE;
                u8 partner = PARTNER(foe);
                u8 skipFoe = 0xFF;

                //Deal with a Red Card switch-in.
                if (gForceSwitchHelper == Force_Switch_Red_Card)
                {
                    skipFoe = gBattleStruct->originalTargetBackup;

                    if (IS_DOUBLE_BATTLE && partner == skipFoe) //The second Frisk target should be skipped
                    {
                        //Make the skipped target the first one
                        u8 temp = partner;
                        partner = foe;
                        foe = temp;
                    }
                }

                if (foe != skipFoe && BATTLER_ALIVE(foe) && ITEM(foe))
                {
                    gLastUsedItem = ITEM(foe);
                    gBattlerTarget = foe;
                    gBattleScripting.battler = bank;
                    gBattleStringLoader = gText_FriskActivate;
                    RecordItemEffectBattle(foe, ITEM_EFFECT(foe));
                    BattleScriptPushCursorAndCallback(BattleScript_Frisk);
                    effect++;
                }
                else if (IS_DOUBLE_BATTLE && BATTLER_ALIVE(partner) && ITEM(partner) != ITEM_NONE)
                {
                    gLastUsedItem = ITEM(partner);
                    gBattlerTarget = partner;
                    gBattleScripting.battler = bank;
                    gBattleStringLoader = gText_FriskActivate;
                    RecordItemEffectBattle(partner, ITEM_EFFECT(partner));
                    BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
                    effect++;
                }
            }
			break;
        case ABILITY_IMMUNITY:
			effect = ImmunityAbilityCheck(bank, STATUS1_PSN_ANY, gText_Poison);
			break;

		//case ABILITY_PASTELVEIL:
		//	effect = ImmunityAbilityCheck(bank, STATUS1_PSN_ANY, gText_Poison);
		//	break;

		case ABILITY_LIMBER:
			effect = ImmunityAbilityCheck(bank, STATUS1_PARALYSIS, gText_Paralysis);
			break;

		case ABILITY_INSOMNIA:
		case ABILITY_VITAL_SPIRIT:
			effect = ImmunityAbilityCheck(bank, STATUS1_SLEEP, gText_Sleep);
			break;

		case ABILITY_WATER_VEIL:
		case ABILITY_WATERBUBBLE:
			effect = ImmunityAbilityCheck(bank, STATUS1_BURN, gText_Burn);
			break;

		case ABILITY_MAGMA_ARMOR:
			effect = ImmunityAbilityCheck(bank, STATUS1_FREEZE, gText_Ice);
			break;

        case ABILITY_DOWNLOAD:
        {
			u8 statId;
			u32 opposingBank = BATTLE_OPPOSITE(bank);
            u32 opposingSpDef;

			u32 opposingDef = gBattleMons[opposingBank].defense;
			APPLY_STAT_MOD(opposingDef, &gBattleMons[opposingBank], opposingDef, STAT_DEF);
			opposingSpDef = gBattleMons[opposingBank].spDefense;
			APPLY_STAT_MOD(opposingSpDef, &gBattleMons[opposingBank], opposingSpDef, STAT_SPDEF);

			if (IS_DOUBLE_BATTLE)
			{
				u32 opposingPartnerDef = 0;
				u32 opposingPartnerSpDef = 0;
				opposingBank = PARTNER(opposingBank);
				if (gBattleMons[opposingBank].hp)
				{
					opposingPartnerDef = gBattleMons[opposingBank].defense;
					APPLY_STAT_MOD(opposingPartnerDef, &gBattleMons[opposingBank], opposingPartnerDef, STAT_DEF);
					opposingPartnerSpDef = gBattleMons[opposingBank].spDefense;
					APPLY_STAT_MOD(opposingPartnerSpDef, &gBattleMons[opposingBank], opposingPartnerSpDef, STAT_SPDEF);
				}

				opposingDef += opposingPartnerDef;
				opposingSpDef += opposingPartnerSpDef;
			}

			if (opposingDef < opposingSpDef)
				statId = STAT_ATK;
			else
				statId = STAT_SPATK;

			if (STAT_STAGE(bank, statId) < STAT_STAGE_MAX)
			{
				gBattlerAttacker = bank;
				STAT_STAGE(bank, statId)++;
				gBattleScripting.statChanger = statId | INCREASE_1;
				PREPARE_STAT_BUFFER(gBattleTextBuff1, statId);
				PREPARE_STAT_ROSE(gBattleTextBuff2);
				BattleScriptPushCursorAndCallback(BattleScript_AttackerAbilityStatRaiseEnd3);
				effect++;
			}
        }
			break;
        case ABILITY_IMPOSTER:
        {
			u8 transformBank = BATTLE_OPPOSITE(bank);

			if (IsRaidBattle() && SIDE(bank) == B_SIDE_PLAYER)
				transformBank = BANK_RAID_BOSS;

			if (BATTLER_ALIVE(transformBank)
			&& !(gBattleMons[transformBank].status2 & (STATUS2_TRANSFORMED | STATUS2_SUBSTITUTE))
			&& !(gStatuses3[transformBank] & (STATUS3_SEMI_INVULNERABLE | STATUS3_ILLUSION))
			&& !IS_TRANSFORMED(bank)
			&& !(IsRaidBattle() && transformBank == BANK_RAID_BOSS && gBattleStruct->dynamaxData.raidShieldsUp))
			{
				gBattlerAttacker = bank;
				gBattlerTarget = transformBank;
				TransformPokemon(bank, gBattlerTarget);
				BattleScriptPushCursorAndCallback(BattleScript_ImposterActivates);
				effect++;
			}
        }
			break;
        case ABILITY_SCHOOLING:
			if (!(gBattleMons[bank].status2 & STATUS2_TRANSFORMED))
			{
				if (SPECIES(bank) == SPECIES_WISHIWASHI && gBattleMons[bank].level >= 20
				&&  gBattleMons[bank].hp > (gBattleMons[bank].maxHP / 4))
				{
					DoFormChange(bank, SPECIES_WISHIWASHI_S, FALSE, TRUE, FALSE);
					BattleScriptPushCursorAndCallback(BattleScript_StartedSchoolingEnd3);
					++effect;
				}
				else if (SPECIES(bank) == SPECIES_WISHIWASHI_S
				&& (gBattleMons[bank].level < 20 ||
					gBattleMons[bank].hp <= (gBattleMons[bank].maxHP / 4)))
				{
					DoFormChange(bank, SPECIES_WISHIWASHI, FALSE, TRUE, FALSE);
					BattleScriptPushCursorAndCallback(BattleScript_StoppedSchoolingEnd3);
					++effect;
				}
			}
			break;
    
        case ABILITY_SHIELDSDOWN:
			if (!(gBattleMons[bank].status2 & STATUS2_TRANSFORMED))
			{
				if (SPECIES(bank) == SPECIES_MINIOR_SHIELD
				&& gBattleMons[bank].hp <= (gBattleMons[bank].maxHP / 2))
				{
					DoFormChange(bank, GetBankPartyData(bank)->box.personality % 7, FALSE, TRUE, FALSE); //Get Minior Colour
					BattleScriptPushCursorAndCallback(BattleScript_ShieldsDownToCoreEnd3);
					++effect;
				}
				else if ((SPECIES(bank) == SPECIES_MINIOR_RED ||
						  SPECIES(bank) == SPECIES_MINIOR_BLUE ||
						  SPECIES(bank) == SPECIES_MINIOR_ORANGE ||
						  SPECIES(bank) == SPECIES_MINIOR_YELLOW ||
						  SPECIES(bank) == SPECIES_MINIOR_INDIGO ||
						  SPECIES(bank) == SPECIES_MINIOR_GREEN ||
						  SPECIES(bank) == SPECIES_MINIOR_VIOLET)
				&& gBattleMons[bank].hp > (gBattleMons[bank].maxHP / 2))
				{
					DoFormChange(bank, SPECIES_MINIOR_SHIELD, FALSE, TRUE, FALSE);
					BattleScriptPushCursorAndCallback(BattleScript_ShieldsDownToMeteorEnd3);
					++effect;
				}
			}
			break;
        
        case ABILITY_FLOWERGIFT:
			#if (defined SPECIES_CHERRIM && defined SPECIES_CHERRIM_SUN)
			if (!(gBattleMons[bank].status2 & STATUS2_TRANSFORMED))
			{
				switch(SPECIES(bank)) {
					case SPECIES_CHERRIM:
						if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
						&& ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA)
						{
							DoFormChange(bank, SPECIES_CHERRIM_SUN, FALSE, FALSE, FALSE);
							BattleScriptPushCursorAndCallback(BattleScript_TransformedEnd3);
							++effect;
						}
						break;

					case SPECIES_CHERRIM_SUN:
						if (!WEATHER_HAS_EFFECT || !(gBattleWeather & WEATHER_SUN_ANY)
						|| ITEM_EFFECT(bank) == HOLD_EFFECT_UTILITY_UMBRELLA)
						{
							DoFormChange(bank, SPECIES_CHERRIM, FALSE, FALSE, FALSE);
							BattleScriptPushCursorAndCallback(BattleScript_TransformedEnd3);
							++effect;
						}
				}
			}
			#endif
			break;
        
        case ABILITY_ELECTRICSURGE:
			effect = TryActivateTerrainAbility(ELECTRIC_TERRAIN, B_ANIM_ELECTRIC_SURGE, bank);
			break;

		case ABILITY_GRASSYSURGE:
			effect = TryActivateTerrainAbility(GRASSY_TERRAIN, B_ANIM_GRASSY_SURGE, bank);
			break;

		case ABILITY_MISTYSURGE:
			effect = TryActivateTerrainAbility(MISTY_TERRAIN, B_ANIM_MISTY_SURGE, bank);
			break;

		case ABILITY_PSYCHICSURGE:
			effect = TryActivateTerrainAbility(PSYCHIC_TERRAIN, B_ANIM_PSYCHIC_SURGE, bank);
			break;
        
        case ABILITY_INTREPIDSWORD:
			if (STAT_STAGE(bank, STAT_STAGE_ATK) < STAT_STAGE_MAX)
			{
				gBattlerAttacker = bank;
				STAT_STAGE(bank, STAT_STAGE_ATK)++;
				gBattleScripting.statChanger = STAT_STAGE_ATK | INCREASE_1;
				PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_ATK);
				PREPARE_STAT_ROSE(gBattleTextBuff2);
				BattleScriptPushCursorAndCallback(BattleScript_AttackerAbilityStatRaiseEnd3);
				effect++;
			}
			break;
        
        case ABILITY_DAUNTLESSSHIELD:
			if (STAT_STAGE(bank, STAT_STAGE_DEF) < STAT_STAGE_MAX)
			{
				gBattlerAttacker = bank;
				STAT_STAGE(bank, STAT_STAGE_DEF)++;
				gBattleScripting.statChanger = STAT_STAGE_DEF | INCREASE_1;
				PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_DEF);
				PREPARE_STAT_ROSE(gBattleTextBuff2);
				BattleScriptPushCursorAndCallback(BattleScript_AttackerAbilityStatRaiseEnd3);
				effect++;
			}
			break;
        
        case ABILITY_SCREENCLEANER:
			RemoveScreensFromSide(B_SIDE_PLAYER);
			RemoveScreensFromSide(B_SIDE_OPPONENT);
			gBattleStringLoader = gText_ScreenCleanerActivate;
			BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
			effect++;
			break;
        
        case ABILITY_MIMICRY:
        {
			const u8* script = TryActivateMimicryForBank(bank);
			if (script != NULL)
			{
				BattleScriptPushCursorAndCallback(BattleScript_End3); //Pop back down to end
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = script;
				effect++;
			}
        }
			break;
        
        case ABILITY_NEUTRALIZINGGAS:
			for (i = 0; i < gBattlersCount; ++i)
			{
				if (!IsAbilitySuppressed(i) //Gastro Acid has higher priority
				&& ABILITY(i) != ABILITY_NONE
				&& !CheckTableForAbility(ABILITY(i), gNeutralizingGasBannedAbilities))
				{
					ability_t* abilityLoc = GetAbilityLocation(i);
					gBattleStruct->neutralizingGasBlockedAbilities[i] = *abilityLoc;
					*abilityLoc = 0;
					gBattleStruct->SlowStartTimers[i] = 0;
				}
			}

			gBattleStringLoader = gText_NeutralizingGasActivate;
			BattleScriptPushCursorAndCallback(BattleScript_NeutralizingGas);
			effect++;
		}

        switch (gLastUsedAbility) { //These abilities should always activate if they can
			case ABILITY_NONE: //So Unnerve activates the first time when Neutralizing Gas leaves the field
			case ABILITY_FORECAST:
			case ABILITY_FLOWERGIFT:
			case ABILITY_TRACE:
			case ABILITYEFFECT_SWITCH_IN_WEATHER:
				break;
			default:
				gStatuses3[bank] |= STATUS3_INTIMIDATE_POKES;
		}

		break;

    case ABILITYEFFECT_ENDTURN: // 1
        if (BATTLER_ALIVE(bank))
        {
            gBattlerAttacker = gActiveBattler = gBattleScripting.battler = bank;
            switch (gLastUsedAbility)
            {
            case ABILITY_RAIN_DISH:
                if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY)
					&& ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA
					&& !BATTLER_MAX_HP(bank))
					{
						BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 16);
						gBattleMoveDamage *= -1;
						effect++;
					}
					break;
            case ABILITY_DRYSKIN:
					if (WEATHER_HAS_EFFECT && ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA)
					{
						if (gBattleWeather & WEATHER_RAIN_ANY && !BATTLER_MAX_HP(bank))
						{
							gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 8);
							gBattleMoveDamage *= -1;
							BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
							effect++;
						}
						else if (gBattleWeather & WEATHER_SUN_ANY)
						{
							gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 8);
							BattleScriptPushCursorAndCallback(BattleScript_DrySkinDamage);
							effect++;
						}
					}
					break;
            
            case ABILITY_ICEBODY:
					if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_HAIL_ANY) && !BATTLER_MAX_HP(bank))
					{
						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 16);
						gBattleMoveDamage *= -1;
						BattleScriptExecute(BattleScript_RainDishActivates);
						effect++;
					}
					break;
            
            case ABILITY_SOLARPOWER:
					if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_SUN_ANY)
					&& ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA)
					{
						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 8);
						BattleScriptExecute(BattleScript_SolarPowerDamage);
						effect++;
					}
					break;
            
            case ABILITY_SHED_SKIN:
					if ((gBattleMons[bank].status1 & STATUS1_ANY) && Random() % 3 == 0)
					{
						ClearBankStatus(bank);
						BattleScriptPushCursorAndCallback(BattleScript_AbilityCuredStatusEnd3);
						effect++;
					}
					break;
            case ABILITY_HYDRATION:
					if (WEATHER_HAS_EFFECT && (gBattleWeather & WEATHER_RAIN_ANY)
					&& ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA
					&& gBattleMons[bank].status1 & STATUS1_ANY)
					{
						ClearBankStatus(bank);
						BattleScriptPushCursorAndCallback(BattleScript_AbilityCuredStatusEnd3);
						effect++;
					}
					break;

            case ABILITY_HEALER:
					if (IS_DOUBLE_BATTLE
					&& BATTLER_ALIVE(PARTNER(bank))
					&& gBattleMons[PARTNER(bank)].status1
					&& Random() % 100 < 30)
					{
						gEffectBattler = PARTNER(bank);
						ClearBankStatus(gEffectBattler);
						BattleScriptPushCursorAndCallback(BattleScript_Healer);
						effect++;
					}
					break;
                    
            case ABILITY_SPEED_BOOST:
					if (STAT_STAGE(bank, STAT_SPEED) < STAT_STAGE_MAX && gDisableStructs[bank].isFirstTurn != 2)
					{
						gBattleMons[bank].statStages[STAT_SPEED - 1]++;
						gBattleScripting.statChanger = STAT_SPEED | INCREASE_1;
						gBattleScripting.animArg1 = 0x11;
						gBattleScripting.animArg2 = 0;
						PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_SPEED);
						PREPARE_STAT_ROSE(gBattleTextBuff2);
						BattleScriptPushCursorAndCallback(BattleScript_AttackerAbilityStatRaiseEnd3);
						effect++;
					}
					break;
            
            case ABILITY_MOODY:
                {
					u8 statToRaise = 0;
					u8 statToLower = 0;
					const u8* scriptPtr;

					if (MainStatsMinned(bank))
					{	//Raise One Stat
						statToRaise = RandRange(STAT_ATK, STAT_SPDEF + 1); //Attack, Defense, Sp. Atk, Sp.Def, Speed
						gBattleScripting.statChanger = statToRaise | INCREASE_2;
						scriptPtr = BattleScript_MoodySingleStat;
					}
					else if (MainStatsMaxed(bank))
					{	//Lower One Stat
						statToLower = RandRange(STAT_ATK, STAT_SPDEF + 1); //Attack, Defense, Sp. Atk, Sp.Def, Speed
						gBattleScripting.statChanger = statToLower | DECREASE_1;
						scriptPtr = BattleScript_MoodySingleStat;
					}
					else
					{	//Raise One Stat and Lower Another
						if (!(AllMainStatsButOneAreMinned(bank)))
						{	//At least two non min stats
							do
							{
								statToRaise = RandRange(STAT_ATK, STAT_SPDEF + 1); //Attack, Defense, Sp. Atk, Sp.Def, Speed
							} while (STAT_STAGE(bank, statToRaise) == STAT_STAGE_MAX);
						}
						else
						{	//If all stats but one are at min, then raise one of the min ones so the
							//non min one canned be lowered.
							do
							{
								statToRaise = RandRange(STAT_ATK, STAT_SPDEF + 1); //Attack, Defense, Sp. Atk, Sp.Def, Speed
							} while (STAT_STAGE(bank, statToRaise) != 0);
						}

						do
						{
							statToLower = RandRange(STAT_ATK, STAT_SPDEF + 1); //Attack, Defense, Sp. Atk, Sp.Def, Speed
						} while (statToLower == statToRaise || STAT_STAGE(bank, statToLower) == 0);

						gBattleScripting.statChanger = statToRaise | INCREASE_2;
						gBattleCommunication[MOVE_EFFECT_BYTE] = statToLower; //Save stat to lower in move effect byte
						scriptPtr = BattleScript_MoodyRegular;
					}

					BattleScriptPushCursorAndCallback(scriptPtr);
					effect++;
                }
					break;
            case ABILITY_BADDREAMS:
					if (gBattleMons[FOE(bank)].status1 & STATUS1_SLEEP
					|| ABILITY(FOE(bank)) == ABILITY_COMATOSE
					|| gBattleMons[PARTNER(FOE(bank))].status1 & STATUS1_SLEEP
					|| ABILITY(PARTNER(FOE(bank))) == ABILITY_COMATOSE)
					{
						BattleScriptPushCursorAndCallback(BattleScript_BadDreams);
						++effect;
					}
					break;
            case ABILITY_TRUANT:
					if (!(gBattleMons[bank].status1 & STATUS1_SLEEP))
						gDisableStructs[bank].truantCounter ^= 1;
					else
						gDisableStructs[bank].truantCounter = 0; //Being asleep resets the Truant counter
					break;
            
            case ABILITY_HARVEST:
					if (ItemId_GetPocket(SAVED_CONSUMED_ITEMS(bank)) == POCKET_BERRY_POUCH)
					{
						if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA) //Yeah...that'll never happen
						{
							//100% chance
						}
						else //No Sun
						{
							if (Random() % 100 < 50)
								break;
						}

						BattleScriptPushCursorAndCallback(BattleScript_Harvest);
						++effect;
					}
					break;
            
            case ABILITY_PICKUP:
                {
					u8 itemBank = GetTopOfPickupStackNotIncludingBank(bank);

					if (itemBank != 0xFF)
					{
						RemoveBankFromPickupStack(itemBank);
						if (CONSUMED_ITEMS(itemBank)) //This shouldn't be empty but just in case
						{
							gBattleMons[bank].item = CONSUMED_ITEMS(itemBank);
							BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[bank].item);
							MarkBattlerForControllerExec(bank);
							gLastUsedItem = CONSUMED_ITEMS(itemBank);
							CONSUMED_ITEMS(itemBank) = 0;
							RecordItemEffectBattle(bank, ITEM_EFFECT(bank));
							BattleScriptPushCursorAndCallback(BattleScript_Pickup);
							++effect;
							break;
						}
					}
                }
					break;

            case ABILITY_SLOWSTART:
					if (gBattleStruct->SlowStartTimers[bank] > 0 && --gBattleStruct->SlowStartTimers[bank] == 0)
					{
						gBattleStringLoader = gText_SlowStartEnd;
						BattleScriptPushCursorAndCallback(BattleScript_SwitchInAbilityMsg);
						++effect;
					}
					break;
            case ABILITY_BALLFETCH:
					if (gBattleStruct->failedThrownPokeBall != ITEM_NONE
					&& SIDE(bank) == B_SIDE_PLAYER
					&& ITEM(bank) == ITEM_NONE)
					{
						gLastUsedItem = ITEM(bank) = gBattleStruct->failedThrownPokeBall;
						gBattleStruct->failedThrownPokeBall = ITEM_NONE;

						gActiveBattler = bank;
						BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
						MarkBattlerForControllerExec(gActiveBattler);

						BattleScriptPushCursorAndCallback(BattleScript_BallFetch);
						++effect;
					}
					break;

				case ABILITY_FORECAST:
					effect = CastformDataTypeChange(bank);
					if (effect)
					{
						BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
						gBattleStruct->formToChangeInto = effect - 1;
					}
					break;

                //case ABILITY_WIMPOUT:
				case ABILITY_EMERGENCYEXIT:
					if (!(gStatuses3[bank] & (STATUS3_SKY_DROP_ANY))
					&& gBattleStruct->turnDamageTaken[bank] > 0
					&& gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2
					&& gBattleMons[bank].hp + gBattleStruct->turnDamageTaken[bank] > gBattleMons[bank].maxHP / 2) //Fell from end turn damage
					{
						gBattleScripting.battler = gBankSwitching = bank;
						BattleScriptPushCursorAndCallback(BattleScript_EmergencyExitEnd3);
						effect = 1;
					}

					gBattleStruct->turnDamageTaken[bank] = 0; //Reset to prevent accidental triggering

            }
        }
        break;

    case ABILITYEFFECT_MOVES_BLOCK: // 2
        if (gBattlerAttacker != bank) //Can't block against self
			{
				switch (gLastUsedAbility) {
					case ABILITY_SOUNDPROOF:
						if (CheckSoundMove(move))
							effect = 1;
						break;

					case ABILITY_BULLETPROOF:
						if (CheckTableForMove(move, gBallBombMoves))
							effect = 1;
						break;

					case ABILITY_OVERCOAT:
						if (CheckTableForMove(move, gPowderMoves))
							effect = 1;
						break;

					case ABILITY_DAZZLING: //Cannot use
					case ABILITY_QUEENLYMAJESTY: //Cannot use
						if (PriorityCalc(gBattlerAttacker, B_ACTION_USE_MOVE, move) > 0)
							effect = 2;
						break;

					case ABILITY_TELEPATHY:
						if (IS_DOUBLE_BATTLE
						&& gBattlerAttacker == PARTNER(bank))
							effect = 1;
						break;
				}

				if (effect)
				{
					if (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)
						gHitMarker |= HITMARKER_NO_PPDEDUCT;
					gBattleScripting.battler = bank;

					if (effect == 1)
						gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless_PPLoss;
					else
						gBattlescriptCurrInstr = BattleScript_AttackerCantUseMove;
				}
			}
			break;
    case ABILITYEFFECT_MOVES_BLOCK_PARTNER: //0x14
			if (gBattlerAttacker != PARTNER(bank)) //Can't block against partner
			{
				switch (gLastUsedAbility) {
					case ABILITY_DAZZLING:
					case ABILITY_QUEENLYMAJESTY:
						if (PriorityCalc(gBattlerAttacker, B_ACTION_USE_MOVE, move) > 0)
							effect = 1;
						break;
				}

				if (effect)
				{
					if (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)
						gHitMarker |= HITMARKER_NO_PPDEDUCT;
					gBattleScripting.battler = bank;
					gBattlescriptCurrInstr = BattleScript_AttackerCantUseMove;
				}
			}
			break;
    case ABILITYEFFECT_ABSORBING: // 3
    {
        u8 statId = 0;
		
        if (move == MOVE_NONE) break;

		gBattleScripting.battler = bank;
        switch (gLastUsedAbility) {
				case ABILITY_VOLT_ABSORB:
					if (moveType == TYPE_ELECTRIC)
						effect = 1;
					break;

				case ABILITY_WATER_ABSORB:
				case ABILITY_DRYSKIN:
					if (moveType == TYPE_WATER)
						effect = 1;
					break;

				case ABILITY_MOTORDRIVE:
					if (moveType == TYPE_ELECTRIC)
						effect = 2, statId = STAT_SPEED;
					break;

				case ABILITY_LIGHTNING_ROD:
					if (moveType == TYPE_ELECTRIC)
						effect = 2, statId = STAT_SPATK;
					break;

				case ABILITY_STORMDRAIN:
					if (moveType == TYPE_WATER)
						effect = 2, statId = STAT_SPATK;
					break;

				case ABILITY_SAPSIPPER:
					if (moveType == TYPE_GRASS)
					{
						effect = 2, statId = STAT_ATK;
					}
					break;

				case ABILITY_FLASH_FIRE:
					if (moveType == TYPE_FIRE && !(gBattleMons[bank].status1 & STATUS1_FREEZE))
						effect = 3;
					break;
			}

        switch (effect) {
				case 1: // Restore HP ability
					if (BATTLER_MAX_HP(bank) || IsHealBlocked(bank))
					{
						if ((gProtectStructs[gBattlerAttacker].notFirstStrike))
							gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless;
						else
							gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless_PPLoss;
					}
					else
					{
						if (gProtectStructs[gBattlerAttacker].notFirstStrike)
							gBattlescriptCurrInstr = BattleScript_MoveHPDrain;
						else
							gBattlescriptCurrInstr = BattleScript_MoveHPDrain_PPLoss;

						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 4);
						gBattleMoveDamage *= -1;
					}
					break;

				case 2: // Boost Stat ability
					if (gBattleMons[bank].statStages[statId - 1] >= STAT_STAGE_MAX)
					{
						if ((gProtectStructs[gBattlerAttacker].notFirstStrike))
							gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless;
						else
							gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless_PPLoss;
					}
					else
					{
						if (gProtectStructs[gBattlerAttacker].notFirstStrike)
							gBattlescriptCurrInstr = BattleScript_MoveStatDrain;
						else
							gBattlescriptCurrInstr = BattleScript_MoveStatDrain_PPLoss;

						gBattleScripting.statChanger = statId | INCREASE_1;
					}
					
					break;

				case 3: //Flash Fire
					if (!(gBattleResources->flags->flags[bank] & RESOURCE_FLAG_FLASH_FIRE))
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					else
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;

					if (gProtectStructs[gBattlerAttacker].notFirstStrike)
						gBattlescriptCurrInstr = BattleScript_FlashFireBoost;
					else
						gBattlescriptCurrInstr = BattleScript_FlashFireBoost_PPLoss;
			}
    }
        break;
    case ABILITYEFFECT_MOVE_END: // Think contact abilities.
        if (SheerForceCheck() && gLastUsedAbility != ABILITY_ILLUSION) //Sheer Force negates all these abilities
				break;

		gBattleScripting.battler = bank;

        switch (gLastUsedAbility)
        {
        case ABILITY_COLOR_CHANGE:
            if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& move != MOVE_STRUGGLE
				&& SPLIT(move) != SPLIT_STATUS
				&& !IsOfType(bank, moveType)
				&& BATTLER_ALIVE(bank)
				&& gBattlerAttacker != bank)
				{
					SET_BATTLER_TYPE(bank, moveType);
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AbilityChangedTypeContact;
					effect++;
				}
				break;
        case ABILITY_IRONBARBS:
        case ABILITY_ROUGH_SKIN:
            if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBattlerAttacker)
				&& gBattlerAttacker != bank
				&& ABILITY(gBattlerAttacker) != ABILITY_MAGICGUARD
				&& CheckContact(move, gBattlerAttacker))
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBattlerAttacker) / 8);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
					effect++;
				}
				break;
        case ABILITY_EFFECT_SPORE:
            if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBattlerAttacker)
				&& gBattlerAttacker != bank
				&& CheckContact(move, gBattlerAttacker)
				&& IsAffectedByPowder(gBattlerAttacker)
				&& Random() % 10 == 0)
				{
					do
					{
						gBattleCommunication[MOVE_EFFECT_BYTE] = Random() & 3;
					} while (gBattleCommunication[MOVE_EFFECT_BYTE] == 0);

					switch (gBattleCommunication[MOVE_EFFECT_BYTE]) {
						case MOVE_EFFECT_SLEEP:
							if (CanBePutToSleep(gBattlerAttacker, TRUE))
								++effect;
							break;
						case MOVE_EFFECT_POISON:
							if (CanBePoisoned(gBattlerAttacker, bank, TRUE))
								++effect;
							break;
						case MOVE_EFFECT_BURN: //Gets changed to Paralysis
							gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
							if (CanBeParalyzed(gBattlerAttacker, TRUE))
								++effect;
							break;
					}

					if (effect)
					{
						gBattleCommunication[MOVE_EFFECT_BYTE] |= MOVE_EFFECT_AFFECTS_USER;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_AbilityApplySecondaryEffect;
						gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked earlier
					}
					else
						gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
				}
				break;
        case ABILITY_POISON_POINT:
            if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBattlerAttacker)
				&& gBattlerAttacker != bank
				&& CheckContact(move, gBattlerAttacker)
				&& CanBePoisoned(gBattlerAttacker, gBattlerTarget, TRUE)
				&& Random() % 3 == 0)
				{
					gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_POISON;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AbilityApplySecondaryEffect;
					gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked earlier
					effect++;
				}
				break;
        case ABILITY_STATIC:
            if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBattlerAttacker)
				&& gBattlerAttacker != bank
				&& CheckContact(move, gBattlerAttacker)
				&& CanBeParalyzed(gBattlerAttacker, TRUE)
				&& Random() % 3 == 0)
				{
					gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_PARALYSIS;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AbilityApplySecondaryEffect;
					gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked earlier
					effect++;
				}
				break;
        case ABILITY_FLAME_BODY:
            if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBattlerAttacker)
				&& gBattlerAttacker != bank
				&& CheckContact(move, gBattlerAttacker)
				&& CanBeBurned(gBattlerAttacker, TRUE)
				&& Random() % 3 == 0)
				{
					gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_BURN;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AbilityApplySecondaryEffect;
					gHitMarker |= HITMARKER_IGNORE_SAFEGUARD; //Safeguard checked earlier
					effect++;
				}
				break;

        case ABILITY_CUTE_CHARM:
            if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBattlerAttacker)
				&& BATTLER_ALIVE(bank)
				&& gBattlerAttacker != bank
				&& CheckContact(move, gBattlerAttacker)
				&& (Random() % 3 == 0)
				&& ABILITY(gBattlerAttacker) != ABILITY_OBLIVIOUS
				&& ABILITY(gBattlerAttacker) != ABILITY_AROMAVEIL
				&& !(IS_DOUBLE_BATTLE && ABILITY(PARTNER(gBattlerAttacker)) == ABILITY_AROMAVEIL)
				&& GetGenderFromSpeciesAndPersonality(speciesAtk, pidAtk) != GetGenderFromSpeciesAndPersonality(speciesDef, pidDef)
				&& !(gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION)
				&& GetGenderFromSpeciesAndPersonality(speciesAtk, pidAtk) != MON_GENDERLESS
				&& GetGenderFromSpeciesAndPersonality(speciesDef, pidDef) != MON_GENDERLESS)
				{
					gBattleMons[gBattlerAttacker].status2 |= STATUS2_INFATUATED_WITH(gBattlerTarget);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_CuteCharmActivates;
					effect++;
				}
				break;
        
        case ABILITY_JUSTIFIED:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(bank)
				&& gBattlerAttacker != bank
				&& moveType == TYPE_DARK
				&& gBattleMons[bank].statStages[STAT_ATK - 1] < 12)
				{
					gBattleScripting.statChanger = STAT_ATK | INCREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;
        
        case ABILITY_RATTLED:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(bank)
				&& gBattlerAttacker != bank
				&& (moveType == TYPE_DARK || moveType == TYPE_BUG || moveType == TYPE_GHOST)
				&& gBattleMons[bank].statStages[STAT_SPEED - 1] < 12)
				{
					gBattleScripting.statChanger = STAT_SPEED | INCREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;
        
        case ABILITY_WEAKARMOR:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& CalcMoveSplit(gBattlerAttacker, gCurrentMove) == SPLIT_PHYSICAL
				&& BATTLER_ALIVE(bank)
				&& gBattlerAttacker != bank
				&& (gBattleMons[bank].statStages[STAT_SPEED - 1] < 12 || gBattleMons[bank].statStages[STAT_DEF - 1] > 0))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_WeakArmorActivates;
					effect++;
				}
				break;
        
        case ABILITY_CURSEDBODY:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBattlerAttacker)
				&& gBattlerAttacker != bank
				&& ABILITY(gBattlerAttacker) != ABILITY_AROMAVEIL
				&& !(IS_DOUBLE_BATTLE && ABILITY(PARTNER(gBattlerAttacker)) == ABILITY_AROMAVEIL)
				&& Random() % 3 == 0)
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_CursedBodyActivates;
					effect++;
				}
				break;
        case ABILITY_MUMMY:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBattlerAttacker)
				&& gBattlerAttacker != bank
				&& CheckContact(move, gBattlerAttacker))
				{
					switch (ABILITY(gBattlerAttacker)) {
						case ABILITY_MUMMY:
						case ABILITY_BATTLEBOND:
						case ABILITY_COMATOSE:
						case ABILITY_DISGUISE:
						case ABILITY_MULTITYPE:
						case ABILITY_POWERCONSTRUCT:
						case ABILITY_RKS_SYSTEM:
						case ABILITY_SCHOOLING:
						case ABILITY_SHIELDSDOWN:
						case ABILITY_STANCECHANGE:
						case ABILITY_ICEFACE:
							break;
						default:
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_MummyActivates;
							effect++;
							break;
					}
				}
				break;

			case ABILITY_WANDERING_SPIRIT:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& (BATTLER_ALIVE(gBattlerAttacker) || BATTLER_ALIVE(gBattlerTarget))
				&& gBattlerAttacker != bank
				&& CheckContact(move, gBattlerAttacker))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_WanderingSpiritActivates;
					effect++;
					break;
				}
				break;

			case ABILITY_ANGERPOINT:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gCritMultiplier > BASE_CRIT_MULTIPLIER
				&& BATTLER_ALIVE(bank)
				&& gBattleMons[bank].statStages[STAT_ATK - 1] < 12)
				{
					gBattleMons[bank].statStages[STAT_ATK - 1] = 12;
					PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_ATK);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AngerPointActivates;
					effect++;
				}
				break;

			case ABILITY_AFTERMATH:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBattlerAttacker)
				&& gBattlerAttacker != bank
				&& ABILITY(gBattlerAttacker) != ABILITY_MAGICGUARD
				&& CheckContact(move, gBattlerAttacker)
				&& !BATTLER_ALIVE(bank)
				&& !ABILITY_PRESENT(ABILITY_DAMP))
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBattlerAttacker) / 4);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
					effect++;
				}
				break;

			case ABILITY_BERSERK:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(bank)
				&& gBattleMons[bank].hp < gBattleMons[bank].maxHP / 2
				&& gBattleMons[bank].hp + gHpDealt > gBattleMons[bank].maxHP / 2 //Hp fell below half
				&& gBattleMons[bank].statStages[STAT_SPATK - 1] < 12)
				{
					gBattleScripting.statChanger = STAT_SPATK | INCREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;

			case ABILITY_TANGLINGHAIR:
			case ABILITY_GOOEY:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& CheckContact(move, gBattlerAttacker)
				&& BATTLER_ALIVE(gBattlerAttacker)
				&& gBattlerAttacker != bank
				&& (STAT_CAN_FALL(gBattlerAttacker, STAT_SPEED) || ABILITY(gBattlerAttacker) == ABILITY_MIRRORARMOR))
				{
					gBattleScripting.statChanger = STAT_SPEED | DECREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_GooeyActivates;
					effect++;
				}
				break;

			case ABILITY_INNARDSOUT:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(gBattlerAttacker)
				&& gBattlerAttacker != bank
				&& ABILITY(gBattlerAttacker) != ABILITY_MAGICGUARD
				&& !BATTLER_ALIVE(bank))
				{
					gBattleMoveDamage = MathMax(1, gHpDealt);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
					effect++;
				}
				break;

			case ABILITY_STAMINA:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(bank)
				&& gBattlerAttacker != bank
				&& gBattleMons[bank].statStages[STAT_DEF - 1] < 12)
				{
					gBattleScripting.statChanger = STAT_DEF | INCREASE_1;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;

			case ABILITY_WATERCOMPACTION:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(bank)
				&& gBattlerAttacker != bank
				&& moveType == TYPE_WATER
				&& gBattleMons[bank].statStages[STAT_DEF - 1] < 12)
				{
					gBattleScripting.statChanger = STAT_DEF | INCREASE_2;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;

			case ABILITY_STEAMENGINE:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& BATTLER_ALIVE(bank)
				&& gBattlerAttacker != bank
				&& (moveType == TYPE_WATER || moveType == TYPE_FIRE)
				&& STAT_STAGE(bank, STAT_SPEED) < STAT_STAGE_MAX)
				{
					gBattleScripting.statChanger = STAT_SPEED | INCREASE_6;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaise;
					effect++;
				}
				break;

			case ABILITY_ILLUSION:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gStatuses3[bank] & STATUS3_ILLUSION)
				{
					gBattleStruct->IllusionBroken |= gBitTable[bank];
					gStatuses3[bank] &= ~(STATUS3_ILLUSION);
					gBattleStruct->disguisedAs[bank] = 0;
					gActiveBattler = bank;
					EmitDataTransfer(0, &gStatuses3[gActiveBattler], 4, &gStatuses3[gActiveBattler]);
					MarkBattlerForControllerExec(gActiveBattler);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_IllusionBroken;
					effect++;
				}
				break;

			case ABILITY_COTTONDOWN:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank))
				{
				}
				break;

			case ABILITY_SANDSPIT:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBattlerAttacker != bank
				&& !(gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS)))
				{
					effect = ActivateWeatherAbility(WEATHER_SANDSTORM_PERMANENT | WEATHER_SANDSTORM_TEMPORARY,
													HOLD_EFFECT_SMOOTH_ROCK, bank, B_ANIM_SANDSTORM_CONTINUES, 1, TRUE);
				}
				break;

			case ABILITY_PERISH_BODY:
				if (MOVE_HAD_EFFECT
				&& TOOK_DAMAGE(bank)
				&& gBattlerAttacker != bank
				&& CheckContact(move, gBattlerAttacker))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_PerishBody;
					effect++;
				}
				break;
        
        }
        break;
    case ABILITYEFFECT_IMMUNITY: // 5
        for (bank = 0; bank < gBattlersCount; bank++)
        {
            switch (gBattleMons[bank].ability)
            {
            case ABILITY_IMMUNITY:
                if (gBattleMons[bank].status1 & (STATUS1_PSN_ANY))
                {
                    StringCopy(gBattleTextBuff1, gStatusConditionString_PoisonJpn);
                    effect = 1;
                }
                break;
            case ABILITY_OWN_TEMPO:
                if (gBattleMons[bank].status2 & STATUS2_CONFUSION)
                {
                    StringCopy(gBattleTextBuff1, gStatusConditionString_ConfusionJpn);
                    effect = 2;
                }
                break;
            case ABILITY_LIMBER:
                if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
                {
                    StringCopy(gBattleTextBuff1, gStatusConditionString_ParalysisJpn);
                    effect = 1;
                }
                break;
            case ABILITY_INSOMNIA:
            case ABILITY_VITAL_SPIRIT:
                if (gBattleMons[bank].status1 & STATUS1_SLEEP)
                {
                    gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
                    StringCopy(gBattleTextBuff1, gStatusConditionString_SleepJpn);
                    effect = 1;
                }
                break;
            case ABILITY_WATER_VEIL:
                if (gBattleMons[bank].status1 & STATUS1_BURN)
                {
                    StringCopy(gBattleTextBuff1, gStatusConditionString_BurnJpn);
                    effect = 1;
                }
                break;
            case ABILITY_MAGMA_ARMOR:
                if (gBattleMons[bank].status1 & STATUS1_FREEZE)
                {
                    StringCopy(gBattleTextBuff1, gStatusConditionString_IceJpn);
                    effect = 1;
                }
                break;
            case ABILITY_OBLIVIOUS:
                if (gBattleMons[bank].status2 & STATUS2_INFATUATION)
                {
                    if (gDisableStructs[bank].tauntTimer)
                        StringCopy(gBattleTextBuff1, gStatusConditionString_MentalState);
                    else
						StringCopy(gBattleTextBuff1, gStatusConditionString_LoveJpn);
					effect = 3;
                }
                else if (gDisableStructs[bank].tauntTimer)
				{
					StringCopy(gBattleTextBuff1, gStatusConditionString_TauntProblem);
					effect = 3;
				}
            }
            if (effect)
            {
                switch (effect)
                {
                case 1: // status cleared
                    gBattleMons[bank].status1 = 0;
                    break;
                case 2: // get rid of confusion
                    gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
                    break;
                case 3: // get rid of infatuation
                    gBattleMons[bank].status2 &= ~(STATUS2_INFATUATION);
                    break;
                }

               BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_AbilityCuredStatus;
				gActiveBattler = gBattleScripting.battler = bank;
                BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                MarkBattlerForControllerExec(gActiveBattler);
                return effect;
            }
        }
        break;
    case ABILITYEFFECT_FORECAST: // 6
        break;
    case ABILITYEFFECT_SYNCHRONIZE: // 7
        if (gLastUsedAbility == ABILITY_SYNCHRONIZE
			&& gBattleStruct->synchronizeTarget[bank] > 0)
			{
				if (bank != gBattleStruct->synchronizeTarget[bank] - 1) //Didn't status itself
				{
					gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
					gBattleStruct->synchronizeMoveEffect &= ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
					gBattleCommunication[MOVE_EFFECT_BYTE] = gBattleStruct->synchronizeMoveEffect;
					gBattleScripting.battler = bank;

					gBattleStruct->backupSynchronizeBanks[0] = gBattlerAttacker;
					gBattleStruct->backupSynchronizeBanks[1] = gBattlerTarget;

					gBattlerAttacker = bank;
					gBattlerTarget = gBattleStruct->synchronizeTarget[bank] - 1;
					gBattleStruct->synchronizeTarget[bank] = 0;

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_SynchronizeActivates;
					gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
					effect++;
				}

				gBattleStruct->synchronizeTarget[bank] = 0;
			}
			break;
    case ABILITYEFFECT_INTIMIDATE1: // 9
        break;
    case ABILITYEFFECT_TRACE: // 11
        break;
    case ABILITYEFFECT_INTIMIDATE2: // 10
        break;
    case ABILITYEFFECT_CHECK_OTHER_SIDE: // 12
            side = SIDE(bank);
			for (i = 0; i < gBattlersCount; i++)
			{
				if (SIDE(i) != side && ABILITY(i) == ability)
				{
					gLastUsedAbility = ability;
					effect = i + 1;
				}
			}
        break;
    case ABILITYEFFECT_CHECK_BATTLER_SIDE: // 13
            side = SIDE(bank);
			for (i = 0; i < gBattlersCount; i++)
			{
				if (SIDE(i) == side && ABILITY(i) == ability)
				{
					gLastUsedAbility = ability;
					effect = i + 1;
				}
			}
			break;
    case ABILITYEFFECT_FIELD_SPORT: // 14
        switch (gLastUsedAbility) {
				case 0xFD:
					effect = IsMudSportActive();
					break;
				case 0xFE:
					effect = IsWaterSportActive();
					break;
				default:
					for (i = 0; i < gBattlersCount; i++)
					{
						if (ABILITY(i) == ability)
						{
							gLastUsedAbility = ability;
							effect = i + 1;
						}
					}
					break;
			}
			break;
    case ABILITYEFFECT_CHECK_ON_FIELD: // 19
        for (i = 0; i < gBattlersCount; i++)
			{
				if (ABILITY(i) == ability && gBattleMons[i].hp)
				{
					gLastUsedAbility = ability;
					effect = i + 1;
				}
			}
			break;
    case ABILITYEFFECT_CHECK_FIELD_EXCEPT_BATTLER: // 15
        for (i = 0; i < gBattlersCount; i++)
			{
				if (ABILITY(i) == ability && i != bank)
				{
					gLastUsedAbility = ability;
					effect = i + 1;
				}
			}
			break;
    case ABILITYEFFECT_COUNT_OTHER_SIDE: // 16
        side = SIDE(bank);
			for (i = 0; i < gBattlersCount; i++)
			{
				if (SIDE(i) != side && ABILITY(i) == ability)
				{
					gLastUsedAbility = ability;
					effect++;
				}
			}
			break;
    case ABILITYEFFECT_COUNT_BATTLER_SIDE: // 17
        side = SIDE(bank);
			for (i = 0; i < gBattlersCount; i++)
			{
				if (SIDE(i) == side && ABILITY(i) == ability)
				{
					gLastUsedAbility = ability;
					effect++;
				}
			}
			break;
    case ABILITYEFFECT_COUNT_ON_FIELD: // 18
        for (i = 0; i < gBattlersCount; i++)
			{
				if (ABILITY(i) == ability && i != bank)
				{
					gLastUsedAbility = ability;
					effect++;
				}
			}
			break;
    }
    if (effect && caseID < ABILITYEFFECT_CHECK_OTHER_SIDE && gLastUsedAbility != 0xFF)
		RecordAbilityBattle(bank, gLastUsedAbility);

    return effect;
}

void BattleScriptExecute(const u8 *BS_ptr)
{
    gBattlescriptCurrInstr = BS_ptr;
    gBattleResources->battleCallbackStack->function[gBattleResources->battleCallbackStack->size++] = gBattleMainFunc;
    gBattleMainFunc = RunBattleScriptCommands_PopCallbacksStack;
    gCurrentActionFuncId = 0;
}

void BattleScriptPushCursorAndCallback(const u8 *BS_ptr)
{
    BattleScriptPushCursor();
    gBattlescriptCurrInstr = BS_ptr;
    gBattleResources->battleCallbackStack->function[gBattleResources->battleCallbackStack->size++] = gBattleMainFunc;
    gBattleMainFunc = RunBattleScriptCommands;
}

enum
{
    ITEM_NO_EFFECT,
    ITEM_STATUS_CHANGE,
    ITEM_EFFECT_OTHER,
    ITEM_PP_CHANGE,
    ITEM_HP_CHANGE,
    ITEM_STATS_CHANGE,
};

u8 ItemBattleEffects(u8 caseID, u8 bank, bool8 moveTurn, bool8 doPluck)
{
	int i = 0;
	u8 effect = ITEM_NO_EFFECT;
	u8 changedPP = 0;
	u8 bankHoldEffect, atkHoldEffect;
	u8 bankQuality, atkQuality;
	u16 atkItem;
	u8 moveSplit = CalcMoveSplit(gBattlerAttacker, gCurrentMove);

    #ifndef NO_GHOST_BATTLES
	if (IS_GHOST_BATTLE && SIDE(bank) == B_SIDE_OPPONENT)
		return 0; //Ghost's items don't activate
	#endif

    if (doPluck)
	{
		bankHoldEffect = ItemId_GetHoldEffect(gLastUsedItem);
		bankQuality = ItemId_GetHoldEffectParam(gLastUsedItem);
	}
	else
	{
		gLastUsedItem = ITEM(bank);

		if (IsBerry(gLastUsedItem)
		&& AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, bank, ABILITY_UNNERVE, 0, 0))
		{
			bankHoldEffect = 0;
			bankQuality = 0;
		}
		else
		{
			bankHoldEffect = ITEM_EFFECT(bank);
			bankQuality = ITEM_QUALITY(bank);
		}
	}

    atkItem = ITEM(gBattlerAttacker);
	atkHoldEffect = ItemId_GetHoldEffect(atkItem);
	atkQuality = ItemId_GetHoldEffectParam(atkItem);

    switch (caseID)
    {
    case ItemEffects_SwitchIn:
        switch (bankHoldEffect)
        {
        case HOLD_EFFECT_DOUBLE_PRIZE:
            gBattleStruct->moneyMultiplier = 2;
            break;
        case HOLD_EFFECT_RESTORE_STATS:
            for (i = 0; i < BATTLE_STATS_NO-1; i++) {
					if (gBattleMons[bank].statStages[i] < 6) {
						gBattleMons[bank].statStages[i] = 6;
						effect = ITEM_STATS_CHANGE;
					}
				}

				if (effect) {
					gBattleScripting.battler = bank;
					gPotentialItemEffectBattler = bank;
					gActiveBattler = gBattlerAttacker = bank;

					if (moveTurn)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_WhiteHerbRet;
					}
					else
						BattleScriptExecute(BattleScript_WhiteHerbEnd2);
				}
				break;
        case HOLD_EFFECT_SEEDS:
            {
				u8 stat = 0;

				if (bankQuality && bankQuality == gTerrainType)
				{
					switch (gTerrainType) {
						case ELECTRIC_TERRAIN:
						case GRASSY_TERRAIN:
							if (STAT_CAN_RISE(bank, STAT_STAGE_DEF))
							{
								stat = STAT_STAGE_DEF;
								effect = ITEM_STATS_CHANGE;
							}
							break;
						case MISTY_TERRAIN:
						case PSYCHIC_TERRAIN:
							if (STAT_CAN_RISE(bank, STAT_STAGE_SPDEF))
							{
								stat = STAT_STAGE_SPDEF;
								effect = ITEM_STATS_CHANGE;
							}
							break;
					}

					if (effect)
					{
						PREPARE_STAT_BUFFER(gBattleTextBuff1, stat);
						gBattleScripting.statChanger = INCREASE_1 | stat;
						gBattleScripting.animArg1 = 0xE + stat;
						gBattleScripting.animArg2 = 0;
						gBattleScripting.battler = gBattlerTarget = bank;

						if (moveTurn)
						{
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_ItemStatChangeRet;
						}
						else
							BattleScriptExecute(BattleScript_ItemStatChangeEnd2);
					}
				}
            }
				break;
        case HOLD_EFFECT_EJECT_PACK:
				if (gBattleStruct->statFellThisTurn[bank])
				{
					gBattleStruct->statFellThisTurn[bank] = FALSE;
					gBattleStruct->NoSymbiosisByte = TRUE;
					gActiveBattler = gBattleScripting.battler = gBankSwitching = bank;
					effect = ITEM_STATS_CHANGE;

					if (moveTurn)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_EjectPackRet;
					}
					else
						BattleScriptExecute(BattleScript_EjectPackEnd2);
				}
				break;
        case HOLD_EFFECT_ROOM_SERVICE:
				if (IsTrickRoomActive() && STAT_CAN_FALL(bank, STAT_STAGE_SPEED))
				{
					PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_SPEED);
					gBattleScripting.statChanger = DECREASE_1 | STAT_STAGE_SPEED;
					gBattleScripting.animArg1 = STAT_ANIM_MINUS1 + STAT_STAGE_SPEED - 1;
					gBattleScripting.animArg2 = 0;
					gBattleScripting.battler = gBattlerTarget = bank;
					effect = ITEM_STATS_CHANGE;

					if (moveTurn)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_ItemStatChangeRet;
					}
					else
						BattleScriptExecute(BattleScript_ItemStatChangeEnd2);
				}
				break;
        }
        break;
    case ItemEffects_EndTurn:
        gBattleScripting.battler = bank;

        if (gBattleMons[bank].hp)
        {
            switch (bankHoldEffect)
            {
            case HOLD_EFFECT_RESTORE_HP:
                if ((gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2) || (doPluck && !BATTLER_MAX_HP(bank)))
				{
					if (gLastUsedItem == ITEM_SITRUS_BERRY)
						gBattleMoveDamage = GetBaseMaxHP(bank) / 4;
					else
						gBattleMoveDamage = bankQuality;

					if (ABILITY(bank) == ABILITY_RIPEN && IsBerry(gLastUsedItem))
						gBattleMoveDamage *= 2;

					if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
						gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;

					gBattleMoveDamage *= -1;
					if (moveTurn || doPluck)
					{
						BattleScriptPushCursor();
						if (IsBerry(gLastUsedItem))
							gBattlescriptCurrInstr = BattleScript_BerryHealHP_RemoveBerryRet;
						else
							gBattlescriptCurrInstr = BattleScript_ItemHealHP_RemoveItemRet; //Berry Juice
					}
					else
					{
						if (IsBerry(gLastUsedItem))
							BattleScriptExecute(BattleScript_BerryHealHP_RemoveBerryEnd2);
						else
							BattleScriptExecute(BattleScript_ItemHealHP_RemoveItemEnd2);
					}
					effect = ITEM_HP_CHANGE;
				}
				break;
            case HOLD_EFFECT_RESTORE_PP:
                if (!moveTurn || doPluck)
				{
					u8 ppBonuses, maxPP;
					u16 move;
					struct Pokemon* poke = GetBankPartyData(bank);
					u8 restoreAmount = bankQuality;

					if (ABILITY(bank) == ABILITY_RIPEN)
						restoreAmount *= 2;

					for (i = 0; i < MAX_MON_MOVES; ++i)
					{
						move = GetMonData(poke, MON_DATA_MOVE1 + i, 0);
						changedPP = GetMonData(poke, MON_DATA_PP1 + i, 0);
						ppBonuses = GetMonData(poke, MON_DATA_PP_BONUSES, 0);

						if (doPluck)
						{
							maxPP = CalculatePPWithBonus(move, ppBonuses, i);
							if (IS_TRANSFORMED(bank) && maxPP > 5)
								maxPP = 5; //Can't restore past 5 PP if transformed

							if (changedPP < maxPP)
								break;
						}
						else if (move && changedPP == 0)
							break;
					}

					if (i < MAX_MON_MOVES)
					{
						maxPP = CalculatePPWithBonus(move, ppBonuses, i);
						if (IS_TRANSFORMED(bank) && maxPP > 5)
							maxPP = 5; //Can't restore past 5 PP if transformed

						if (changedPP + restoreAmount > maxPP)
							changedPP = maxPP;
						else
							changedPP = changedPP + restoreAmount;

						PREPARE_MOVE_BUFFER(gBattleTextBuff1, move);

						if (doPluck)
						{
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_BerryPPHealRet;
						}
						else
							BattleScriptExecute(BattleScript_BerryPPHealEnd2);


						gActiveBattler = bank;
						BtlController_EmitSetMonData(0, i + REQUEST_PPMOVE1_BATTLE, 0, 1, &changedPP);
						MarkBattlerForControllerExec(gActiveBattler);
						effect = ITEM_PP_CHANGE;
					}
				}
				break;
            case HOLD_EFFECT_RESTORE_STATS:
                for (i = 0; i < BATTLE_STATS_NO-1; ++i)
				{
					if (gBattleMons[bank].statStages[i] < 6)
					{
						gBattleMons[bank].statStages[i] = 6;
						effect = ITEM_STATS_CHANGE;
					}
				}

				if (effect)
				{
					gBattleScripting.battler = gActiveBattler = gPotentialItemEffectBattler = bank;
					if (moveTurn)
					{
						BattleScriptPushCursor();
						if (doPluck) //Fling
							gBattlescriptCurrInstr = BattleScript_WhiteHerbFling;
						else
							gBattlescriptCurrInstr = BattleScript_WhiteHerbRet;
					}
					else
						BattleScriptExecute(BattleScript_WhiteHerbEnd2);
				}
				break;
            case HOLD_EFFECT_LEFTOVERS:
                LEFTOVERS_HEAL:
				if (!moveTurn && !BATTLER_MAX_HP(bank)
				&&  !IsHealBlocked(bank)
				&&  !gBattleStruct->leftoverHealingDone[bank])
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 16);
					if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
						gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
					gBattleMoveDamage *= -1;
					BattleScriptExecute(BattleScript_ItemHealHP_End2);
					effect = ITEM_HP_CHANGE;
					RecordItemEffectBattle(bank, bankHoldEffect);
					gBattleStruct->leftoverHealingDone[bank] = TRUE;
				}
				break;

            case HOLD_EFFECT_CONFUSE_SPICY:
				effect = ConfusionBerries(bank, FLAVOR_SPICY, moveTurn, doPluck);
				break;

			case HOLD_EFFECT_CONFUSE_DRY:
				effect = ConfusionBerries(bank, FLAVOR_DRY, moveTurn, doPluck);
				break;

			case HOLD_EFFECT_CONFUSE_SWEET:
				effect = ConfusionBerries(bank, FLAVOR_SWEET, moveTurn, doPluck);
				break;

			case HOLD_EFFECT_CONFUSE_BITTER:
				effect = ConfusionBerries(bank, FLAVOR_BITTER, moveTurn, doPluck);
				break;

			case HOLD_EFFECT_CONFUSE_SOUR:
				effect = ConfusionBerries(bank, FLAVOR_SOUR, moveTurn, doPluck);
				break;

			case HOLD_EFFECT_ATTACK_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_ATK, moveTurn, doPluck);
				break;

			case HOLD_EFFECT_DEFENSE_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_DEF, moveTurn, doPluck);
				break;

			case HOLD_EFFECT_SPEED_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_SPEED, moveTurn, doPluck);
				break;

			case HOLD_EFFECT_SP_ATTACK_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_SPATK, moveTurn, doPluck);
				break;

			case HOLD_EFFECT_SP_DEFENSE_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_SPDEF, moveTurn, doPluck);
				break;

            case HOLD_EFFECT_CRITICAL_UP:
                if ((PINCH_BERRY_CHECK(bank) || doPluck)
				&& !(gBattleMons[bank].status2 & STATUS2_FOCUS_ENERGY))
				{
					gBattleMons[bank].status2 |= STATUS2_FOCUS_ENERGY;
					if (moveTurn || doPluck)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryFocusEnergyRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryFocusEnergyEnd2);
					effect = ITEM_EFFECT_OTHER;
				}
				break;
            case HOLD_EFFECT_RANDOM_STAT_UP:
                if ((PINCH_BERRY_CHECK(bank) || doPluck)
				&& !((StatsMaxed(bank) && ABILITY(bank) != ABILITY_CONTRARY) || (StatsMinned(bank) && ABILITY(bank) == ABILITY_CONTRARY)))
				{
					u8 buff = (ABILITY(bank) == ABILITY_RIPEN) ? INCREASE_4 : INCREASE_2;

					do
					{
						i = RandRange(STAT_STAGE_ATK, NUM_STATS);
					} while (STAT_STAGE(bank, i) >= STAT_STAGE_MAX);

					i -= 1; //So stat starts at 0

					gBattleTextBuff1[0] = 0xFD;
					gBattleTextBuff1[1] = 5;
					gBattleTextBuff1[2] = i + 1;
					gBattleTextBuff1[3] = EOS;

					gBattleTextBuff2[0] = 0xFD;
					gBattleTextBuff2[1] = 0;
					gBattleTextBuff2[2] = 0xD1;
					gBattleTextBuff2[3] = 0xD1 >> 8;
					gBattleTextBuff2[4] = 0;
					gBattleTextBuff2[5] = 0xD2;
					gBattleTextBuff2[6] = 0xD2 >> 8;
					gBattleTextBuff2[7] = EOS;

					gEffectBattler = bank;
					gBattleScripting.statChanger = (buff | 1) + i;
					gBattleScripting.animArg1 = STAT_ANIM_PLUS2 + i;
					gBattleScripting.animArg2 = 0;

					if (moveTurn || doPluck)
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryStatRaiseRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryStatRaiseEnd2);
					effect = ITEM_STATS_CHANGE;
				}
				break;
            case HOLD_EFFECT_CURE_PAR:
                if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
				{
					gBattleMons[bank].status1 &= ~(STATUS1_PARALYSIS);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureParRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCurePrlzEnd2);
					effect = ITEM_STATUS_CHANGE;
				}
				break;
            case HOLD_EFFECT_CURE_PSN:
                if (gBattleMons[bank].status1 & STATUS1_PSN_ANY)
                {
                    gBattleMons[bank].status1 &= ~(STATUS1_PSN_ANY | STATUS1_TOXIC_COUNTER);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCurePsnRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCurePsnEnd2);
					effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_BRN:
                if (gBattleMons[bank].status1 & STATUS1_BURN)
                {
                    gBattleMons[bank].status1 &= ~(STATUS1_BURN);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureBrnRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureBrnEnd2);
					effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_FRZ:
				if (gBattleMons[bank].status1 & STATUS1_FREEZE)
				{
					gBattleMons[bank].status1 &= ~(STATUS1_FREEZE);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureFrzRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureFrzEnd2);
					effect = ITEM_STATUS_CHANGE;
				}
				break;

			case HOLD_EFFECT_CURE_SLP:
				if (gBattleMons[bank].status1 & STATUS1_SLEEP)
				{
					gBattleMons[bank].status1 &= ~(STATUS1_SLEEP);
					gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureSlpRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureSlpEnd2);
					effect = ITEM_STATUS_CHANGE;
				}
				break;

			case HOLD_EFFECT_CURE_CONFUSION:
				if (gBattleMons[bank].status2 & STATUS2_CONFUSION)
				{
					gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureConfusionRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureConfusionEnd2);
					effect = ITEM_EFFECT_OTHER;
				}
				break;
            case HOLD_EFFECT_CURE_STATUS:
                if (gBattleMons[bank].status1 & STATUS1_ANY || gBattleMons[bank].status2 & STATUS2_CONFUSION)
                {
                    i = 0;
                    if (gBattleMons[bank].status1 & STATUS1_PSN_ANY)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_PoisonJpn);
                        ++i;
                    }
                    if (gBattleMons[bank].status1 & STATUS1_SLEEP)
                    {
                        gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
                        StringCopy(gBattleTextBuff1, gStatusConditionString_SleepJpn);
                        ++i;
                    }
                    if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_ParalysisJpn);
                        ++i;
                    }
                    if (gBattleMons[bank].status1 & STATUS1_BURN)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_BurnJpn);
                        ++i;
                    }
                    if (gBattleMons[bank].status1 & STATUS1_FREEZE)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_IceJpn);
                        ++i;
                    }
                    if (gBattleMons[bank].status2 & STATUS2_CONFUSION)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_ConfusionJpn);
                        ++i;
                    }
                    if (i < 2)
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					else
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;
					gBattleMons[bank].status1 = 0;
					gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureChosenStatusRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureChosenStatusEnd2);
					effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_ATTRACT:
                i = 0;
				if (gBattleMons[bank].status2 & STATUS2_INFATUATION)
				{
					gBattleMons[bank].status2 &= ~(STATUS2_INFATUATION);
					StringCopy(gBattleTextBuff1, gStatusConditionString_LoveJpn);
					++i;
				}
				if (gDisableStructs[bank].disabledMove)
				{
					gDisableStructs[bank].disabledMove = 0;
					gDisableStructs[bank].disableTimer = 0;
					StringCopy(gBattleTextBuff1, gStatusConditionString_DisableProblem);
					++i;
				}
				if (gDisableStructs[bank].encoredMove)
				{
					gDisableStructs[bank].encoredMove = 0; //Same as end turn clear
					gDisableStructs[bank].encoreTimer = 0;
					StringCopy(gBattleTextBuff1, gStatusConditionString_EncoreProblem);
					++i;
				}
				if (gDisableStructs[bank].tauntTimer > 0)
				{
					gDisableStructs[bank].tauntTimer = 0;
					StringCopy(gBattleTextBuff1, gStatusConditionString_TauntProblem);
					++i;
				}
				if (i == 0)
					break;

				else if (i >= 2)
					StringCopy(gBattleTextBuff1, gStatusConditionString_MentalState);

				if (moveTurn)
				{
					BattleScriptPushCursor();

					if (doPluck) //Fling
						gBattlescriptCurrInstr = BattleScript_HerbCureChosenStatusFling;
					else
						gBattlescriptCurrInstr = BattleScript_HerbCureChosenStatusRet;
				}
				else
					BattleScriptExecute(BattleScript_HerbCureChosenStatusEnd2);

				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				effect = ITEM_EFFECT_OTHER;
				break;
            case HOLD_EFFECT_MICLE_BERRY:
				if (PINCH_BERRY_CHECK(bank) || doPluck)
				{
					gBattleStruct->MicleBerryBits |= (1 << bank);

					if (moveTurn || doPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MicleBerryRet;
					}
					else
						BattleScriptExecute(BattleScript_MicleBerryEnd2);
					effect = ITEM_EFFECT_OTHER;
				}
				break;

			case HOLD_EFFECT_BLACK_SLUDGE:
				if (!moveTurn)
				{
					if (IsOfType(bank, TYPE_POISON))
						goto LEFTOVERS_HEAL;
					else if (!moveTurn && ABILITY(bank) != ABILITY_MAGICGUARD
					&& !gBattleStruct->leftoverHealingDone[bank])
					{
						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 8);
						BattleScriptExecute(BattleScript_BlackSludgeHurt);
						effect = ITEM_EFFECT_OTHER;
						gBattleStruct->leftoverHealingDone[bank] = TRUE;
					}
				}
            }
            if (effect)
			{
				if (GetPocketByItemId(gLastUsedItem) == POCKET_BERRY_POUCH)
					gBattleStruct->BelchCounters |= gBitTable[gBattlerPartyIndexes[bank]];

				gBattleScripting.battler = bank;
				gPotentialItemEffectBattler = bank;
				gActiveBattler = bank;
				switch (effect) {
					case ITEM_STATUS_CHANGE:
						BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bank].status1);
						MarkBattlerForControllerExec(gActiveBattler);
						break;
					case ITEM_PP_CHANGE:
						if (!(gBattleMons[bank].status2 & STATUS2_TRANSFORMED) && !(gDisableStructs[bank].mimickedMoves & gBitTable[i]))
							gBattleMons[bank].pp[i] = changedPP;
						break;
					case ITEM_HP_CHANGE:
						BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[bank].hp);
						MarkBattlerForControllerExec(gActiveBattler);
						break;
				}
			}
		}
        break;
    
    case ItemEffects_ContactTarget:
        if (doPluck)
			gBattleScripting.battler = bank;
		else
			gBattleScripting.battler = bank = gBattlerTarget;

		if (SheerForceCheck() && bankHoldEffect != HOLD_EFFECT_AIR_BALLOON) //Air Balloon still pops
			break;

		switch (bankHoldEffect) {
			case HOLD_EFFECT_ROCKY_HELMET:
				if (TOOK_DAMAGE(bank)
				&& MOVE_HAD_EFFECT
				&& ABILITY(gBattlerAttacker) != ABILITY_MAGICGUARD
				&& CheckContact(gCurrentMove, gBattlerAttacker)
				&& !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bank)
				&& gBattleMons[gBattlerAttacker].hp)
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBattlerAttacker) / 6);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_RockyHelmetDamage;
					RecordItemEffectBattle(bank, bankHoldEffect);
					gActiveBattler = gBattlerAttacker;
					effect = ITEM_HP_CHANGE;
				}
				break;

			case HOLD_EFFECT_WEAKNESS_POLICY:
				if (TOOK_DAMAGE(bank)
				&& gMoveResultFlags == MOVE_RESULT_SUPER_EFFECTIVE
				&& gBattleMons[bank].hp
				&& (STAT_CAN_RISE(bank, STAT_STAGE_ATK) || STAT_CAN_RISE(bank, STAT_STAGE_SPATK))
				&& !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bank))
				{
					gEffectBattler = bank;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_WeaknessPolicy;
					gActiveBattler = gBattlerTarget;
					effect = ITEM_STATS_CHANGE;
				}
				break;

			case HOLD_EFFECT_ABSORB_BULB:
				effect = RaiseStatsContactItem(bank, STAT_STAGE_SPATK, TYPE_WATER);
				break;

			case HOLD_EFFECT_AIR_BALLOON:
				if (TOOK_DAMAGE(bank) && MOVE_HAD_EFFECT)
				{
					gEffectBattler = bank;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AirBallooonPop;
					gActiveBattler = bank;
					effect = ITEM_EFFECT_OTHER;
				}
				break;

			case HOLD_EFFECT_CELL_BATTERY:
				effect = RaiseStatsContactItem(bank, STAT_STAGE_ATK, TYPE_ELECTRIC);
				break;

			case HOLD_EFFECT_LUMINOUS_MOSS:
				effect = RaiseStatsContactItem(bank, STAT_STAGE_SPDEF, TYPE_WATER);
				break;

			case HOLD_EFFECT_SNOWBALL:
				effect = RaiseStatsContactItem(bank, STAT_STAGE_ATK, TYPE_ICE);
				break;

			case HOLD_EFFECT_JABOCA_ROWAP_BERRY:
				if (!doPluck)
				{
					if (TOOK_DAMAGE(bank)
					&& MOVE_HAD_EFFECT
					&& gBattleMons[gBattlerAttacker].hp
					&& gBattleMons[bank].hp
					&& !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bank)
					&& moveSplit == bankQuality)
					{
						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBattlerAttacker) / 8);

						if (ABILITY(bank) == ABILITY_RIPEN)
							gBattleMoveDamage *= 2;

						gEffectBattler = gBattlerAttacker;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_JabocaRowapBerry;
						gActiveBattler = gBattlerAttacker;
						effect = ITEM_HP_CHANGE;
					}
				}
				break;

			case HOLD_EFFECT_KEE_BERRY:
				effect = KeeMaranagaBerryFunc(bank, STAT_STAGE_DEF, SPLIT_PHYSICAL, doPluck);
				break;

			case HOLD_EFFECT_MARANGA_BERRY:
				effect = KeeMaranagaBerryFunc(bank, STAT_STAGE_SPDEF, SPLIT_SPECIAL, doPluck);
				break;

			case HOLD_EFFECT_ENIGMA_BERRY:
				if (!doPluck
				&& TOOK_DAMAGE(bank)
				&& gMoveResultFlags == MOVE_RESULT_SUPER_EFFECTIVE
				&& BATTLER_ALIVE(bank)
				&& !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bank))
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 4);

					if (ABILITY(bank) == ABILITY_RIPEN)
						gBattleMoveDamage *= 2;

					if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
						gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
					gBattleMoveDamage *= -1;
					BattleScriptPushCursor();
					BattleScriptExecute(BattleScript_BerryHealHP_RemoveBerryRet);
					effect = ITEM_HP_CHANGE;
				}
				break;

			case HOLD_EFFECT_STICKY_BARB:
				if (TOOK_DAMAGE(bank)
				&& MOVE_HAD_EFFECT
				&& CheckContact(gCurrentMove, gBattlerAttacker)
				&& !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bank)
				&& gBattleMons[gBattlerAttacker].hp != 0
				&& gBattleMons[gBattlerAttacker].item == ITEM_NONE)
				{
					gEffectBattler = bank;

					gBattleMons[bank].item = 0;
					gBattleMons[gBattlerAttacker].item = gLastUsedItem;
					HandleUnburdenBoost(bank); //Give old holder's Unburden boost
					HandleUnburdenBoost(gBattlerAttacker); //Remove new holder's Unburden boost

					gActiveBattler = gBattlerAttacker;
					BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gLastUsedItem);
					MarkBattlerForControllerExec(gActiveBattler);

					gActiveBattler = bank;
					BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
					MarkBattlerForControllerExec(gActiveBattler);

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_StickyBarbTransfer;
					gActiveBattler = gBattlerAttacker;
					effect = ITEM_EFFECT_OTHER;
				}
				break;
		}

		if (effect && GetPocketByItemId(gLastUsedItem)== POCKET_BERRY_POUCH)
			gBattleStruct->BelchCounters |= gBitTable[gBattlerPartyIndexes[bank]];

		if (effect == REQUEST_HP_BATTLE)
		{
			gBattleScripting.battler = bank;
			gPotentialItemEffectBattler = bank;
			gActiveBattler = bank;
			BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].status1);
			MarkBattlerForControllerExec(gActiveBattler);
			break;
		}

        break;
    case ItemEffects_ContactAttacker:
		//These item effects must explicitly be listed in CMD49.c in order to work!
		if (!SheerForceCheck() || atkHoldEffect == HOLD_EFFECT_THROAT_SPRAY || atkHoldEffect == HOLD_EFFECT_BLUNDER_POLICY)
		{
			switch (atkHoldEffect)
			{
				case HOLD_EFFECT_FLINCH:
					if (ABILITY(gBattlerAttacker) == ABILITY_SERENE_GRACE || BankSideHasRainbow(gBattlerAttacker))
						bankQuality *= 2;
					if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
					&& ABILITY(gBattlerTarget) != ABILITY_STENCH
					&& TOOK_DAMAGE(gBattlerTarget)
					&& MOVE_HAD_EFFECT
					&& ((Random() % 100)) < bankQuality
					&& !CheckTableForMove(gCurrentMove, gFlinchChanceMoves)
					&& gBattleMons[gBattlerTarget].hp)
					{
						gBattleMons[gBattlerTarget].status2 |= STATUS2_FLINCHED;
					}
					break;
				case HOLD_EFFECT_SHELL_BELL:
					if (gBattleStruct->totalDamageGiven > 0
					&& bank != gBattlerTarget
					&& BATTLER_ALIVE(bank)
					&& !BATTLER_MAX_HP(bank))
					{
						if (gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2)
							gBattleStruct->lessThanHalfHPBeforeShellBell = TRUE; //For Emergency Exit

						gPotentialItemEffectBattler = bank;
						gBattleScripting.battler = bank;
						gBattleMoveDamage = MathMax(1, (gBattleStruct->totalDamageGiven / atkQuality)) * - 1;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_ItemHealHP_Ret;
						effect++;
					}
					break;
				case HOLD_EFFECT_LIFE_ORB:
                {
					u8 moveEffect = gBattleMoves[gCurrentMove].effect;

					if (gBattleStruct->AttackerDidDamageAtLeastOnce
					&& moveEffect != EFFECT_BIDE //Moves that deal direct damage aren't included
					&& moveEffect != EFFECT_COUNTER
					&& moveEffect != EFFECT_MIRROR_COAT
					&& moveEffect != EFFECT_ENDEAVOR
					&& moveEffect != EFFECT_SUPER_FANG
					&& moveEffect != EFFECT_LEVEL_DAMAGE
					&& moveEffect != EFFECT_PSYWAVE
					&& gCurrentMove != MOVE_FINALGAMBIT
					&& ABILITY(bank) != ABILITY_MAGICGUARD
					&& BATTLER_ALIVE(bank))
					{
						gBattleMoveDamage = MathMax(1, GetBaseMaxHP(bank) / 10);
						gBattleStruct->selfInflictedDamage += gBattleMoveDamage; //For Emergency Exit
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_LifeOrbDamage;
						effect++;
					}
                }
					break;
				case HOLD_EFFECT_THROAT_SPRAY:
                {
					bool8 throatSprayWork = FALSE;

					if (SPLIT(gCurrentMove) == SPLIT_STATUS)
					{
						if (MOVE_HAD_EFFECT)
							throatSprayWork = TRUE;
					}
					else //Damaging move
					{
						if (gBattleStruct->AttackerDidDamageAtLeastOnce && !SheerForceCheck())
							throatSprayWork = TRUE;
					}

					if (throatSprayWork
					&& BATTLER_ALIVE(gBattlerAttacker)
					&& !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
					&& CheckSoundMove(gCurrentMove)
					&& !ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), STAT_STAGE_SPATK, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0))
					{
						PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_SPATK);

						gEffectBattler = bank;
						gBattleScripting.animArg1 = STAT_ANIM_PLUS1 + STAT_STAGE_SPATK - 1;
						gBattleScripting.animArg2 = 0;

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_RaiseStatsItem;
						effect = ITEM_STATS_CHANGE;
					}
                }
					break;
				case HOLD_EFFECT_BLUNDER_POLICY:
					if (gBattleStruct->activateBlunderPolicy
					&& BATTLER_ALIVE(gBattlerAttacker)
					&& !ChangeStatBuffs(SET_STAT_BUFF_VALUE(2), STAT_STAGE_SPEED, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0))
					{
						PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_SPEED);

						gEffectBattler = gBattlerAttacker;
						gBattleScripting.animArg1 = STAT_ANIM_PLUS2 + STAT_STAGE_SPEED - 1;
						gBattleScripting.animArg2 = 0;

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_RaiseStatsItem;
						effect = ITEM_STATS_CHANGE;
					}

					gBattleStruct->activateBlunderPolicy = FALSE;
					break;
			}
		}
		break;
	}

	return effect;
}

static u8 ConfusionBerries(u8 bank, u8 flavour, bool8 moveTurn, bool8 doPluck)
{
	u8 effect = 0;

	#ifdef OLD_CONFUSION_HEAL_BERRIES
	if ((gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2 && !moveTurn)
	|| (doPluck && !BATTLER_MAX_HP(bank)))
	{
		gBattleTextBuff1[0] = 0xFD;
		gBattleTextBuff1[1] = 8;
		gBattleTextBuff1[2] = flavour;
		gBattleTextBuff1[3] = EOS;

		gBattleMoveDamage = GetBaseMaxHP(bank) / 8;

		if (ABILITY(bank) == ABILITY_RIPEN)
			gBattleMoveDamage *= 2;

		if (gBattleMoveDamage == 0)
			gBattleMoveDamage = 1;
		if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
			gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
		gBattleMoveDamage *= -1;

		if (moveTurn || doPluck)
		{
			BattleScriptPushCursor();
			if (GetFlavorRelationByPersonality(gBattleMons[bank].personality, flavour) < 0)
				gBattlescriptCurrInstr = BattleScript_BerryConfuseHealRet;
			else
				gBattlescriptCurrInstr = BattleScript_BerryHealHP_RemoveBerryRet;
		}
		else
		{
			if (GetFlavorRelationByPersonality(gBattleMons[bank].personality, flavour) < 0)
				BattleScriptExecute(BattleScript_BerryConfuseHealEnd2);
			else
				BattleScriptExecute(BattleScript_BerryHealHP_RemoveBerryEnd2);
		}
		effect = ITEM_HP_CHANGE;
	}
	#else
	if (PINCH_BERRY_CHECK(bank) || (doPluck && !BATTLER_MAX_HP(bank)))
	{
		gBattleTextBuff1[0] = 0xFD;
		gBattleTextBuff1[1] = 8;
		gBattleTextBuff1[2] = flavour;
		gBattleTextBuff1[3] = EOS;

		#ifdef GEN_7_CONFUSION_HEAL_BERRIES
		gBattleMoveDamage = GetBaseMaxHP(bank) / 2;
		if (ABILITY(bank) == ABILITY_RIPEN)
			gBattleMoveDamage *= 2;
		#else
		gBattleMoveDamage = GetBaseMaxHP(bank) / 3;
		if (ABILITY(bank) == ABILITY_RIPEN)
			gBattleMoveDamage = (gBattleMoveDamage * 4) / 3; //So total winds up being 1.666
		#endif

		if (ABILITY(bank) == ABILITY_RIPEN)
			gBattleMoveDamage *= 2;

		if (gBattleMoveDamage == 0)
			gBattleMoveDamage = 1;
		if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
			gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
		gBattleMoveDamage *= -1;

		if (moveTurn || doPluck)
		{
			BattleScriptPushCursor();
			if (GetFlavorRelationByPersonality(gBattleMons[bank].personality, flavour) < 0)
				gBattlescriptCurrInstr = BattleScript_BerryConfuseHealRet;
			else
				gBattlescriptCurrInstr = BattleScript_BerryHealHP_RemoveBerryRet;
		}
		else
		{
			if (GetFlavorRelationByPersonality(gBattleMons[bank].personality, flavour) < 0)
				BattleScriptExecute(BattleScript_BerryConfuseHealEnd2);
			else
				BattleScriptExecute(BattleScript_BerryHealHP_RemoveBerryEnd2);
		}
		effect = ITEM_HP_CHANGE;
	}
	#endif
	return effect;
}

static u8 StatRaiseBerries(u8 bank, u8 stat, bool8 moveTurn, bool8 doPluck)
{
	u8 effect = 0;
	u8 backupUser = gBattlerAttacker;
	u8 buff = 1;
	u8 animId = STAT_ANIM_PLUS1;
	gBattlerAttacker = bank;

	if (ABILITY(bank) == ABILITY_RIPEN)
	{
		buff *= 2;
		animId = STAT_ANIM_PLUS2;
	}

	if ((PINCH_BERRY_CHECK(bank) || doPluck) && STAT_CAN_RISE(bank, stat)
	&& BATTLER_ALIVE(bank)
	&& !ChangeStatBuffs(SET_STAT_BUFF_VALUE(buff), stat, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0))
	{
		PREPARE_STAT_BUFFER(gBattleTextBuff1, stat);
		PREPARE_STRING_BUFFER(gBattleTextBuff2, STRINGID_STATROSE);

		gEffectBattler = bank;
		gBattleScripting.statChanger = INCREASE_1 | stat;
		gBattleScripting.animArg1 = animId + stat - 1;
		gBattleScripting.animArg2 = 0;
		if (moveTurn || doPluck)
		{
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_BerryStatRaiseRet;
		}
		else
			BattleScriptExecute(BattleScript_BerryStatRaiseEnd2);
		effect = ITEM_STATS_CHANGE;
	}

	gBattlerAttacker = backupUser;
	return effect;
}

static u8 RaiseStatsContactItem(u8 bank, u8 stat, u8 moveType) 
{
	u8 effect = 0;
	u8 backupUser = gBattlerAttacker;
	gBattlerAttacker = bank;

	if (TOOK_DAMAGE(bank)
	&& gBattleStruct->dynamicMoveType == moveType
	&& BATTLER_ALIVE(bank)
	&& !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bank)
	&& !ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), stat, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0))
	{
		PREPARE_STAT_BUFFER(gBattleTextBuff1, stat);

		gEffectBattler = bank;
		gBattleScripting.animArg1 = STAT_ANIM_PLUS1 + stat - 1;
		gBattleScripting.animArg2 = 0;

		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_RaiseStatsItem;
		effect = ITEM_STATS_CHANGE;
	}

	gBattlerAttacker = backupUser;
	return effect;
}

static u8 KeeMaranagaBerryFunc(u8 bank, u8 stat, u8 split, bool8 doPluck) 
{
	u8 effect = 0;
	u8 backupUser = gBattlerAttacker;
	u8 buff = 1;
	u8 animId = STAT_ANIM_PLUS1;
	gBattlerAttacker = bank;

	if (ABILITY(bank) == ABILITY_RIPEN)
	{
		buff *= 2;
		animId = STAT_ANIM_PLUS2;
	}

	if (((TOOK_DAMAGE(bank) && CalcMoveSplit(gBattlerAttacker, gCurrentMove) == split && !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bank)) || doPluck)
	&& BATTLER_ALIVE(bank)
	&& !ChangeStatBuffs(SET_STAT_BUFF_VALUE(buff), stat, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0))
	{
		gEffectBattler = gBattlerAttacker;
		gBattleScripting.animArg1 = animId + stat - 1;
		gBattleScripting.animArg2 = 0;

		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_BerryStatRaiseRet;
		effect = ITEM_STATS_CHANGE;
	}

	gBattlerAttacker = backupUser;
	return effect;
}

void ClearFuryCutterDestinyBondGrudge(u8 battlerId)
{
    gDisableStructs[battlerId].furyCutterCounter = 0;
    gBattleMons[battlerId].status2 &= ~(STATUS2_DESTINY_BOND);
    gStatuses3[battlerId] &= ~(STATUS3_GRUDGE);
}

void HandleAction_RunBattleScript(void) // identical to RunBattleScriptCommands
{
    if (!gBattleControllerExecFlags)
        gBattleScriptingCommandsTable[*gBattlescriptCurrInstr]();
}

u8 GetMoveTarget(u16 move, u8 setTarget)
{
    u8 targetBattler = 0;
    u8 moveTarget;
    u8 side;

    if (setTarget)
        moveTarget = setTarget - 1;
    else
        moveTarget = gBattleMoves[move].target;
    switch (moveTarget)
    {
    case MOVE_TARGET_SELECTED:
        side = GetBattlerSide(gBattlerAttacker) ^ BIT_SIDE;
        if (gSideTimers[side].followmeTimer && gBattleMons[gSideTimers[side].followmeTarget].hp)
            targetBattler = gSideTimers[side].followmeTarget;
        else
        {
            side = GetBattlerSide(gBattlerAttacker);
            do
            {
                targetBattler = Random() % gBattlersCount;
            } while (targetBattler == gBattlerAttacker || side == GetBattlerSide(targetBattler) || gAbsentBattlerFlags & gBitTable[targetBattler]);
            if (gBattleMoves[move].type == TYPE_ELECTRIC
             && AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIDE, gBattlerAttacker, ABILITY_LIGHTNING_ROD, 0, 0)
             && gBattleMons[targetBattler].ability != ABILITY_LIGHTNING_ROD)
            {
                targetBattler ^= BIT_FLANK;
                RecordAbilityBattle(targetBattler, gBattleMons[targetBattler].ability);
                gSpecialStatuses[targetBattler].lightningRodRedirected = 1;
            }
        }
        break;
    case MOVE_TARGET_DEPENDS:
    case MOVE_TARGET_BOTH:
    case MOVE_TARGET_FOES_AND_ALLY:
    case MOVE_TARGET_OPPONENTS_FIELD:
        targetBattler = GetBattlerAtPosition((GetBattlerPosition(gBattlerAttacker) & BIT_SIDE) ^ BIT_SIDE);
        if (gAbsentBattlerFlags & gBitTable[targetBattler])
            targetBattler ^= BIT_FLANK;
        break;
    case MOVE_TARGET_RANDOM:
        side = GetBattlerSide(gBattlerAttacker) ^ BIT_SIDE;
        if (gSideTimers[side].followmeTimer && gBattleMons[gSideTimers[side].followmeTarget].hp)
            targetBattler = gSideTimers[side].followmeTarget;
        else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && moveTarget & MOVE_TARGET_RANDOM)
        {
            if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
            {
                if (Random() & 1)
                    targetBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
                else
                    targetBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
            }
            else
            {
                if (Random() & 1)
                    targetBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
                else
                    targetBattler = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
            }
            if (gAbsentBattlerFlags & gBitTable[targetBattler])
                targetBattler ^= BIT_FLANK;
        }
        else
            targetBattler = GetBattlerAtPosition((GetBattlerPosition(gBattlerAttacker) & BIT_SIDE) ^ BIT_SIDE);
        break;
    case MOVE_TARGET_USER_OR_PARTNER:
    case MOVE_TARGET_USER:
        targetBattler = gBattlerAttacker;
        break;
    }
    *(gBattleStruct->moveTarget + gBattlerAttacker) = targetBattler;
    return targetBattler;
}

static bool32 IsNotEventLegalMewOrDeoxys(u8 battlerId)
{
    if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT
     || (GetMonData(&gPlayerParty[gBattlerPartyIndexes[battlerId]], MON_DATA_SPECIES, NULL) != SPECIES_DEOXYS
     && GetMonData(&gPlayerParty[gBattlerPartyIndexes[battlerId]], MON_DATA_SPECIES, NULL) != SPECIES_MEW))
        return TRUE;
    return GetMonData(&gPlayerParty[gBattlerPartyIndexes[battlerId]], MON_DATA_EVENT_LEGAL, NULL);
}

bool8 IsLinkDoubleBattle(void)
{
	u32 flags = (BATTLE_TYPE_MULTI | BATTLE_TYPE_TRAINER | BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLE);

	if ((gBattleTypeFlags & flags) == flags)
		return TRUE;
	else
		return FALSE;
}

bool8 AreAllKindsOfRunningPrevented(void)
{
	if (IsRaidBattle() && !RAID_BATTLE_END)
		return TRUE;
	#ifdef FLAG_NO_RUNNING
	else if (FlagGet(FLAG_NO_RUNNING))
		return TRUE;
	#endif
	#ifdef FLAG_NO_CATCHING_AND_RUNNING
	else if (FlagGet(FLAG_NO_CATCHING_AND_RUNNING))
		return TRUE;
	#endif

	return FALSE;
}