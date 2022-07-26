#include "global.h"
#include "gflib.h"
#include "battle_string_ids.h"
#include "battle.h"
#include "battle_anim.h"
#include "strings.h"
#include "battle_message.h"
#include "link.h"
#include "event_scripts.h"
#include "event_data.h"
#include "item.h"
#include "battle_tower.h"
#include "trainer_tower.h"
#include "battle_setup.h"
#include "field_specials.h"
#include "new_menu_helpers.h"
#include "set_z_effect.h"
#include "battle_controllers.h"
#include "frontier.h"
#include "graphics.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/trainers.h"

struct BattleWindowText
{
    u8 fillValue;
    u8 fontId;
    u8 x;
    u8 y;
    u8 letterSpacing;
    u8 lineSpacing;
    u8 speed;
    u8 fgColor;
    u8 bgColor;
    u8 shadowColor;
};

static EWRAM_DATA u8 sBattlerAbilities[MAX_BATTLERS_COUNT] = {};
static EWRAM_DATA struct BattleMsgData *sBattleMsgDataPtr = NULL;

static void ChooseMoveUsedParticle(u8 *textPtr);
static void ChooseTypeOfMoveUsedString(u8 *textPtr);


static const u8 sText_Empty1[] = _("");
static const u8 sText_Trainer1LoseText[] = _("{B_TRAINER1_LOSE_TEXT}");
static const u8 sText_Trainer2Class[] = _("{B_TRAINER2_CLASS}");
static const u8 sText_Trainer1RecallPkmn1[] = _("{B_TRAINER1_NAME}：回来吧！{B_OPPONENT_MON1_NAME}！");
static const u8 sText_Trainer1WinText[] = _("{B_TRAINER1_WIN_TEXT}");
static const u8 sText_Trainer1RecallPkmn2[] = _("{B_TRAINER1_NAME}：回来吧，{B_OPPONENT_MON2_NAME}！");
static const u8 sText_Trainer1RecallBoth[] = _("{B_TRAINER1_NAME}：回来吧！\n{B_OPPONENT_MON1_NAME}和{B_OPPONENT_MON2_NAME}。");
static const u8 sText_Trainer2Name[] = _("{B_TRAINER2_NAME}");
static const u8 sText_PkmnGainedEXP[] = _("{B_BUFF1}获得了{B_BUFF2}\n{B_TRAINER2_LOSE_TEXT}点经验值！\p");
static const u8 sText_EmptyString4[] = _("");
static const u8 sText_ABoosted[] = _(" 大量的");
static const u8 sText_PkmnGrewToLv[] = _("{B_BUFF1} grew to\nLV. {B_BUFF2}!{WAIT_SE}\p");
static const u8 sText_PkmnLearnedMove[] = _("{B_BUFF1}掌握了\n{B_BUFF2}！{WAIT_SE}\p");
static const u8 sText_TryToLearnMove1[] = _("{B_BUFF1}想要\n掌握{B_BUFF2}。\p");
static const u8 sText_TryToLearnMove2[] = _("但是，{B_BUFF1}掌握四个绝招\n已经相当吃力了。\p");
static const u8 sText_TryToLearnMove3[] = _("是否忘记一个绝招\n来掌握{B_BUFF2}？");
static const u8 sText_PkmnForgotMove[] = _("{B_BUFF1}忘记了\n{B_BUFF2}。\p");
static const u8 sText_StopLearningMove[] = _("{PAUSE 32}停止了学习\n{B_BUFF2}?");
static const u8 sText_DidNotLearnMove[] = _("{B_BUFF1}没有掌握\n{B_BUFF2}。\p");
static const u8 sText_UseNextPkmn[] = _("使用下一只宝可梦?");
static const u8 sText_AttackMissed[] = _("{B_ATK_NAME_WITH_PREFIX}的\n攻击没有命中！");
static const u8 sText_PkmnProtectedItself[] = _("{B_DEF_NAME_WITH_PREFIX}\n保护了它自己！");
static const u8 sText_AvoidedDamage[] = _("{B_DEF_NAME_WITH_PREFIX}避开了\n{B_DEF_ABILITY}的伤害！");
static const u8 sText_PkmnMakesGroundMiss[] = _("{B_DEF_NAME_WITH_PREFIX}以{B_DEF_ABILITY}\n抵抗了地上系绝招！");
static const u8 sText_PkmnAvoidedAttack[] = _("{B_DEF_NAME_WITH_PREFIX}避开了\n攻击！");
static const u8 sText_ItDoesntAffect[] = _("这对{B_DEF_NAME_WITH_PREFIX}\n没有任何效果……");
static const u8 sText_AttackerFainted[] = _("{B_ATK_NAME_WITH_PREFIX}\n倒下了!\p");
static const u8 sText_TargetFainted[] = _("{B_DEF_NAME_WITH_PREFIX}\n倒下了!\p");
static const u8 sText_PlayerGotMoney[] = _("{B_PLAYER_NAME}获得了{B_BUFF1}$\n作为赏金！\p");
static const u8 sText_PlayerWhiteout[] = _("{B_PLAYER_NAME}没有\n可以作战的宝可梦了！\p");
static const u8 sText_PlayerPanicked[] = _("{B_PLAYER_NAME}被击败了还失去了{B_BUFF1}$……\p…………\p{B_PLAYER_NAME}眼前一片黑！{WAIT_SE}");
static const u8 sText_PlayerWhiteoutAgainstTrainer[] = _("{B_PLAYER_NAME}没有\n可以作战的宝可梦了！\p玩家败给了\n{B_TRAINER1_CLASS}{B_TRAINER1_NAME}！{WAIT_SE}");
static const u8 sText_PlayerPaidAsPrizeMoney[] = _("{B_PLAYER_NAME}支付了{B_BUFF1}$作为赏金\n……\p…………\p{B_PLAYER_NAME}眼前一片黑！{WAIT_SE}");
static const u8 sText_PlayerWhiteout2[] = _("{B_PLAYER_NAME}眼前一片黑！{WAIT_SE}");
static const u8 sText_PreventsEscape[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}以{B_SCR_ACTIVE_ABILITY}\n阻止了逃跑！\p");
static const u8 sText_CantEscape2[] = _("无法逃跑！\p");
static const u8 sText_AttackerCantEscape[] = _("{B_ATK_NAME_WITH_PREFIX}无法逃跑！");
static const u8 sText_HitXTimes[] = _("击中{B_BUFF1}次！");
static const u8 sText_PkmnFellAsleep[] = _("{B_EFF_NAME_WITH_PREFIX}\n睡着了！");
static const u8 sText_PkmnMadeSleep[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n使{B_EFF_NAME_WITH_PREFIX}睡着了！");
static const u8 sText_PkmnAlreadyAsleep[] = _("{B_DEF_NAME_WITH_PREFIX}\n已经睡着了！");
static const u8 sText_PkmnAlreadyAsleep2[] = _("{B_ATK_NAME_WITH_PREFIX}\n已经睡着了！");
static const u8 sText_PkmnWasntAffected[] = _("{B_DEF_NAME_WITH_PREFIX}\n没有受到影响！");
static const u8 sText_PkmnWasPoisoned[] = _("{B_EFF_NAME_WITH_PREFIX}\n中毒了！");
static const u8 sText_PkmnPoisonedBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n使{B_EFF_NAME_WITH_PREFIX}中毒了！");
static const u8 sText_PkmnHurtByPoison[] = _("{B_ATK_NAME_WITH_PREFIX}因中毒\n受到了伤害！");
static const u8 sText_PkmnAlreadyPoisoned[] = _("{B_DEF_NAME_WITH_PREFIX}已经\n中毒了。");
static const u8 sText_PkmnBadlyPoisoned[] = _("{B_EFF_NAME_WITH_PREFIX}中了\n猛毒！");
static const u8 sText_PkmnEnergyDrained[] = _("{B_DEF_NAME_WITH_PREFIX}的\n能量被吸收了！");
static const u8 sText_PkmnWasBurned[] = _("{B_EFF_NAME_WITH_PREFIX}烧伤了！");
static const u8 sText_PkmnBurnedBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n使{B_EFF_NAME_WITH_PREFIX}烧伤了！");
static const u8 sText_PkmnHurtByBurn[] = _("{B_ATK_NAME_WITH_PREFIX}因烧伤\n受到了伤害！");
static const u8 sText_PkmnAlreadyHasBurn[] = _("{B_DEF_NAME_WITH_PREFIX}已经\n烧伤了。");
static const u8 sText_PkmnWasFrozen[] = _("{B_EFF_NAME_WITH_PREFIX}被\n冻成了冰块！");
static const u8 sText_PkmnFrozenBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n使{B_EFF_NAME_WITH_PREFIX}冻成了冰块！");
static const u8 sText_PkmnIsFrozen[] = _("{B_ATK_NAME_WITH_PREFIX}\n冻成了冰块！");
static const u8 sText_PkmnWasDefrosted[] = _("{B_DEF_NAME_WITH_PREFIX}\n解冻了！");
static const u8 sText_PkmnWasDefrosted2[] = _("{B_ATK_NAME_WITH_PREFIX}\n解冻了！");
static const u8 sText_PkmnWasDefrostedBy[] = _("{B_ATK_NAME_WITH_PREFIX}因{B_CURRENT_MOVE}\n解冻了！");
static const u8 sText_PkmnWasParalyzed[] = _("{B_EFF_NAME_WITH_PREFIX}麻痹了！\n可能无法使出绝招！");
static const u8 sText_PkmnWasParalyzedBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n麻痹了{B_EFF_NAME_WITH_PREFIX}！\l可能无法使出绝招！");
static const u8 sText_PkmnIsParalyzed[] = _("{B_ATK_NAME_WITH_PREFIX}因麻痹\n而无法动弹！");
static const u8 sText_PkmnIsAlreadyParalyzed[] = _("{B_DEF_NAME_WITH_PREFIX}已经\n麻痹了！");
static const u8 sText_PkmnHealedParalysis[] = _("{B_DEF_NAME_WITH_PREFIX}的麻痹\n恢复了！");
static const u8 sText_PkmnDreamEaten[] = _("{B_DEF_NAME_WITH_PREFIX}的\n梦被吃掉了！");
static const u8 sText_StatsWontIncrease[] = _("{B_ATK_NAME_WITH_PREFIX}的{B_BUFF1}\n无法继续提升！");
static const u8 sText_StatsWontDecrease[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_BUFF1}\n无法继续下降！");
static const u8 sText_TeamStoppedWorking[] = _("我方的{B_BUFF1}\n失去了作用！");
static const u8 sText_FoeStoppedWorking[] = _("对方的{B_BUFF1}\n失去了作用！");
static const u8 sText_PkmnIsConfused[] = _("{B_ATK_NAME_WITH_PREFIX}\n混乱了！");
static const u8 sText_PkmnHealedConfusion[] = _("{B_ATK_NAME_WITH_PREFIX}解除了\n混乱！");
static const u8 sText_PkmnWasConfused[] = _("{B_EFF_NAME_WITH_PREFIX}开始\n混乱了！");
static const u8 sText_PkmnAlreadyConfused[] = _("{B_DEF_NAME_WITH_PREFIX}已经\n混乱了！");
static const u8 sText_PkmnFellInLove[] = _("{B_DEF_NAME_WITH_PREFIX}\n被迷倒了！");
static const u8 sText_PkmnInLove[] = _("{B_ATK_NAME_WITH_PREFIX}被{B_SCR_ACTIVE_NAME_WITH_PREFIX}\n迷倒了！");
static const u8 sText_PkmnImmobilizedByLove[] = _("{B_ATK_NAME_WITH_PREFIX}因迷倒\n而无法攻击！");
static const u8 sText_PkmnBlownAway[] = _("{B_DEF_NAME_WITH_PREFIX}被\n吹飞了！");
static const u8 sText_PkmnChangedType[] = _("{B_ATK_NAME_WITH_PREFIX}转变\n成了{B_BUFF1}的属性！");
static const u8 sText_PkmnFlinched[] = _("{B_ATK_NAME_WITH_PREFIX} flinched!");
static const u8 sText_PkmnRegainedHealth[] = _("{B_DEF_NAME_WITH_PREFIX}恢复了\n体力！");
static const u8 sText_PkmnHPFull[] = _("{B_DEF_NAME_WITH_PREFIX}的\nHP满了！");
static const u8 sText_PkmnRaisedSpDef[] = _("{B_ATK_PREFIX2}的{B_CURRENT_MOVE}\n提升了特防！");
static const u8 sText_PkmnRaisedSpDefALittle[] = _("{B_ATK_PREFIX2}的{B_CURRENT_MOVE}\n提升了一点点特防！");
static const u8 sText_PkmnRaisedDef[] = _("{B_ATK_PREFIX2}的{B_CURRENT_MOVE}\n提升了防御！");
static const u8 sText_PkmnRaisedDefALittle[] = _("{B_ATK_PREFIX2}的{B_CURRENT_MOVE}\n提升了一点点防御！");
static const u8 sText_PkmnCoveredByVeil[] = _("{B_ATK_PREFIX2}的场上被\n神秘的护身包围了！");
static const u8 sText_PkmnUsedSafeguard[] = _("{B_DEF_NAME_WITH_PREFIX}的场上被神秘护身\n保护了！");
static const u8 sText_PkmnSafeguardExpired[] = _("{B_ATK_PREFIX3}的场上不再受\n神秘护身的保护了！");
static const u8 sText_PkmnWentToSleep[] = _("{B_ATK_NAME_WITH_PREFIX}\n睡起了觉！");
static const u8 sText_PkmnSleptHealthy[] = _("{B_ATK_NAME_WITH_PREFIX}恢复健康后，\n居然睡起来了！");
static const u8 sText_PkmnWhippedWhirlwind[] = _("{B_ATK_NAME_WITH_PREFIX}刮起了\n一阵旋风！");
static const u8 sText_PkmnTookSunlight[] = _("{B_ATK_NAME_WITH_PREFIX}吸收了\n阳光！");
static const u8 sText_PkmnLoweredHead[] = _("{B_ATK_NAME_WITH_PREFIX}缩起了\n脑袋！");
static const u8 sText_PkmnIsGlowing[] = _("{B_ATK_NAME_WITH_PREFIX}正在发光！");
static const u8 sText_PkmnFlewHigh[] = _("{B_ATK_NAME_WITH_PREFIX}飞上了\n高空！");
static const u8 sText_PkmnDugHole[] = _("{B_ATK_NAME_WITH_PREFIX}钻入了地下！");
static const u8 sText_PkmnHidUnderwater[] = _("{B_ATK_NAME_WITH_PREFIX}潜入了\n水底！");
static const u8 sText_PkmnSprangUp[] = _("{B_ATK_NAME_WITH_PREFIX}跃上了高空！");
static const u8 sText_PkmnSqueezedByBind[] = _("{B_DEF_NAME_WITH_PREFIX}因{B_ATK_NAME_WITH_PREFIX}的绑紧\n而捆住了！");
static const u8 sText_PkmnTrappedInVortex[] = _("{B_DEF_NAME_WITH_PREFIX}被困在了\n漩涡里！");
static const u8 sText_PkmnTrappedBySandTomb[] = _("{B_DEF_NAME_WITH_PREFIX}被困于\n流沙地狱之中！");
static const u8 sText_PkmnWrappedBy[] = _("{B_DEF_NAME_WITH_PREFIX}被\n{B_ATK_NAME_WITH_PREFIX}捆绑了！");
static const u8 sText_PkmnClamped[] = _("{B_ATK_NAME_WITH_PREFIX} CLAMPED\n{B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnHurtBy[] = _("{B_ATK_NAME_WITH_PREFIX}因\n{B_BUFF1}受到伤害");
static const u8 sText_PkmnFreedFrom[] = _("{B_ATK_NAME_WITH_PREFIX}摆脱了\n{B_BUFF1}！");
static const u8 sText_PkmnCrashed[] = _("强力的冲击使{B_ATK_NAME_WITH_PREFIX}\n摔倒了！");
const u8 gBattleText_MistShroud[] = _("{B_ATK_PREFIX2}把身体\n包裹在了白雾中！");
static const u8 sText_PkmnProtectedByMist[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}受到\n白雾的保护！");
const u8 gBattleText_GetPumped[] = _("{B_ATK_NAME_WITH_PREFIX}终于\n鼓起精神了！");
static const u8 sText_PkmnHitWithRecoil[] = _("{B_ATK_NAME_WITH_PREFIX}受到\n反作用力的伤害！");
static const u8 sText_PkmnProtectedItself2[] = _("{B_ATK_NAME_WITH_PREFIX}保护了\n自己！");
static const u8 sText_PkmnBuffetedBySandstorm[] = _("{B_ATK_NAME_WITH_PREFIX}被沙暴\n击伤了！");
static const u8 sText_PkmnPeltedByHail[] = _("{B_ATK_NAME_WITH_PREFIX}被冰雹\n击伤了！");
static const u8 sText_PkmnsXWoreOff[] = _("{B_ATK_PREFIX1}的{B_BUFF1}\n消失了！");
static const u8 sText_PkmnSeeded[] = _("{B_DEF_NAME_WITH_PREFIX} was seeded!");
static const u8 sText_PkmnEvadedAttack[] = _("{B_DEF_NAME_WITH_PREFIX}避开了\n攻击！");
static const u8 sText_PkmnSappedByLeechSeed[] = _("寄生种子\n抢夺了{B_ATK_NAME_WITH_PREFIX}的体力！");
static const u8 sText_PkmnFastAsleep[] = _("{B_ATK_NAME_WITH_PREFIX}\n呼呼大睡。");
static const u8 sText_PkmnWokeUp[] = _("{B_ATK_NAME_WITH_PREFIX}醒过来了！");
static const u8 sText_PkmnUproarKeptAwake[] = _("但是{B_SCR_ACTIVE_NAME_WITH_PREFIX}的吵闹\n让它睡不着！");
static const u8 sText_PkmnWokeUpInUproar[] = _("{B_ATK_NAME_WITH_PREFIX} woke up\nin the UPROAR!");
static const u8 sText_PkmnCausedUproar[] = _("{B_ATK_NAME_WITH_PREFIX}发出了\n吵闹！");
static const u8 sText_PkmnMakingUproar[] = _("{B_ATK_NAME_WITH_PREFIX}正在\n吵闹中！");
static const u8 sText_PkmnCalmedDown[] = _("{B_ATK_NAME_WITH_PREFIX}平静下来了！");
static const u8 sText_PkmnCantSleepInUproar[] = _("但是{B_DEF_NAME_WITH_PREFIX}无法\n在吵闹的环境下睡着！");
static const u8 sText_PkmnStockpiled[] = _("{B_ATK_NAME_WITH_PREFIX}储存了\n{B_BUFF1}次！");
static const u8 sText_PkmnCantStockpile[] = _("{B_ATK_NAME_WITH_PREFIX}无法\n继续储存了！");
static const u8 sText_PkmnCantSleepInUproar2[] = _("但是{B_DEF_NAME_WITH_PREFIX}无法\n在吵闹的环境下睡着！");
static const u8 sText_UproarKeptPkmnAwake[] = _("但是吵闹的环境下\n{B_DEF_NAME_WITH_PREFIX}睡不着！");
static const u8 sText_PkmnStayedAwakeUsing[] = _("{B_DEF_NAME_WITH_PREFIX}使用了{B_DEF_ABILITY}\n保持清醒！");
static const u8 sText_PkmnStoringEnergy[] = _("{B_ATK_NAME_WITH_PREFIX}积攒了\n能量！");
static const u8 sText_PkmnUnleashedEnergy[] = _("{B_ATK_NAME_WITH_PREFIX}放出了\n能量！");
static const u8 sText_PkmnFatigueConfusion[] = _("{B_ATK_NAME_WITH_PREFIX}累了，\n混乱了！");
static const u8 sText_PkmnPickedUpItem[] = _("{B_PLAYER_NAME}拾起了\n{B_BUFF1}$！\p");
static const u8 sText_PkmnUnaffected[] = _("{B_DEF_NAME_WITH_PREFIX}\n没受到影响！");
static const u8 sText_PkmnTransformedInto[] = _("{B_ATK_NAME_WITH_PREFIX}变身为\n{B_BUFF1}!");
static const u8 sText_PkmnMadeSubstitute[] = _("{B_ATK_NAME_WITH_PREFIX}制造出了\n一个替身！");
static const u8 sText_PkmnHasSubstitute[] = _("{B_ATK_NAME_WITH_PREFIX}已经\n有一个替身了！");
static const u8 sText_SubstituteDamaged[] = _("替身替{B_DEF_NAME_WITH_PREFIX}\n承受了伤害！\p");
static const u8 sText_PkmnSubstituteFaded[] = _("{B_DEF_NAME_WITH_PREFIX}的\n替身消失了！\p");
static const u8 sText_PkmnMustRecharge[] = _("{B_ATK_NAME_WITH_PREFIX} must\nrecharge!");
static const u8 sText_PkmnRageBuilding[] = _("{B_DEF_NAME_WITH_PREFIX}已经\n愤怒了！");
static const u8 sText_PkmnMoveWasDisabled[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_BUFF1}\n已经没用了！");
static const u8 sText_PkmnMoveDisabledNoMore[] = _("{B_ATK_NAME_WITH_PREFIX}恢复了正常！");
static const u8 sText_PkmnGotEncore[] = _("{B_DEF_NAME_WITH_PREFIX}中了\n再来一次！");
static const u8 sText_PkmnEncoreEnded[] = _("{B_ATK_NAME_WITH_PREFIX}'s ENCORE\nended!");
static const u8 sText_PkmnTookAim[] = _("{B_ATK_NAME_WITH_PREFIX}瞄准了\n{B_DEF_NAME_WITH_PREFIX}！");
static const u8 sText_PkmnSketchedMove[] = _("{B_ATK_NAME_WITH_PREFIX}写生了\n{B_BUFF1}！");
static const u8 sText_PkmnTryingToTakeFoe[] = _("{B_ATK_NAME_WITH_PREFIX}试图\n与对手同归于尽！");
static const u8 sText_PkmnTookFoe[] = _("{B_DEF_NAME_WITH_PREFIX}与{B_ATK_NAME_WITH_PREFIX}同归于尽！");
static const u8 sText_PkmnReducedPP[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_BUFF1}减少了{B_BUFF2}点！");
static const u8 sText_PkmnStoleItem[] = _("{B_ATK_NAME_WITH_PREFIX}偷走了\n{B_DEF_NAME_WITH_PREFIX}的{B_LAST_ITEM}！");
static const u8 sText_TargetCantEscapeNow[] = _("{B_DEF_NAME_WITH_PREFIX}现在不能逃跑！");
static const u8 sText_PkmnFellIntoNightmare[] = _("{B_DEF_NAME_WITH_PREFIX}开始\n做恶梦了！");
static const u8 sText_PkmnLockedInNightmare[] = _("{B_ATK_NAME_WITH_PREFIX}被困在\n恶梦里了！");
static const u8 sText_PkmnLaidCurse[] = _("{B_ATK_NAME_WITH_PREFIX}耗费了自身一半的hp\n并对{B_DEF_NAME_WITH_PREFIX}施加了咒语！");
static const u8 sText_PkmnAfflictedByCurse[] = _("{B_ATK_NAME_WITH_PREFIX}因咒语\n而受到了伤害！");
static const u8 sText_SpikesScattered[] = _("满地星布满了\n对方的场地！");
static const u8 sText_PkmnHurtBySpikes[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}被\n满地星击伤了！");
static const u8 sText_PkmnIdentified[] = _("{B_ATK_NAME_WITH_PREFIX}识破了\n{B_DEF_NAME_WITH_PREFIX}！");
static const u8 sText_PkmnPerishCountFell[] = _("{B_ATK_NAME_WITH_PREFIX}的灭亡回合数\n只剩下{B_BUFF1}了！");
static const u8 sText_PkmnBracedItself[] = _("{B_ATK_NAME_WITH_PREFIX}撑住了！");
static const u8 sText_PkmnEnduredHit[] = _("{B_DEF_NAME_WITH_PREFIX}忍耐住了\n伤害！");
static const u8 sText_MagnitudeStrength[] = _("{B_BUFF1}级地震！");
static const u8 sText_PkmnCutHPMaxedAttack[] = _("{B_ATK_NAME_WITH_PREFIX}耗费了自身一半的HP\n将攻击最大化！");
static const u8 sText_PkmnCopiedStatChanges[] = _("{B_ATK_NAME_WITH_PREFIX}复制了\n{B_DEF_NAME_WITH_PREFIX}的能力变化！");
static const u8 sText_PkmnGotFree[] = _("{B_ATK_NAME_WITH_PREFIX}摆脱了\n{B_DEF_NAME_WITH_PREFIX}的{B_BUFF1}！");
static const u8 sText_PkmnShedLeechSeed[] = _("{B_ATK_NAME_WITH_PREFIX}摆脱了\n寄生种子！");
static const u8 sText_PkmnBlewAwaySpikes[] = _("{B_ATK_NAME_WITH_PREFIX}清除了\n满地星！");
static const u8 sText_PkmnFledFromBattle[] = _("{B_ATK_NAME_WITH_PREFIX}逃离了战斗！");
static const u8 sText_PkmnForesawAttack[] = _("{B_ATK_NAME_WITH_PREFIX} foresaw\nan attack!");
static const u8 sText_PkmnTookAttack[] = _("{B_DEF_NAME_WITH_PREFIX}受到了\n{B_BUFF1}的攻击！");
static const u8 sText_PkmnChoseXAsDestiny[] = _("{B_ATK_NAME_WITH_PREFIX}准备和\n{B_CURRENT_MOVE}一同倒下！");
static const u8 sText_PkmnAttack[] = _("{B_BUFF1}攻击！");
static const u8 sText_PkmnCenterAttention[] = _("{B_ATK_NAME_WITH_PREFIX} became the\ncenter of attention!");
static const u8 sText_PkmnChargingPower[] = _("{B_ATK_NAME_WITH_PREFIX}开始\n蓄力！");
static const u8 sText_NaturePowerTurnedInto[] = _("自然能力变成\n{B_CURRENT_MOVE}！");
static const u8 sText_PkmnStatusNormal[] = _("{B_ATK_NAME_WITH_PREFIX}的状态\n恢复原状！");
static const u8 sText_PkmnSubjectedToTorment[] = _("{B_DEF_NAME_WITH_PREFIX}被假指控\n影响了！");
static const u8 sText_PkmnTighteningFocus[] = _("{B_ATK_NAME_WITH_PREFIX}\n集中了力量！");
static const u8 sText_PkmnFellForTaunt[] = _("{B_DEF_NAME_WITH_PREFIX}被挑拨了！");
static const u8 sText_PkmnReadyToHelp[] = _("{B_ATK_NAME_WITH_PREFIX}准备与{B_DEF_NAME_WITH_PREFIX}\n互相帮助！");
static const u8 sText_PkmnSwitchedItems[] = _("{B_ATK_NAME_WITH_PREFIX}和对手\n交换了携带的物品！");
static const u8 sText_PkmnObtainedX[] = _("{B_ATK_NAME_WITH_PREFIX}获得了\n{B_BUFF1}。");
static const u8 sText_PkmnObtainedX2[] = _("{B_DEF_NAME_WITH_PREFIX}获得了\n{B_BUFF2}。");
static const u8 sText_PkmnObtainedXYObtainedZ[] = _("{B_ATK_NAME_WITH_PREFIX}获得了\n{B_BUFF1}。\p{B_DEF_NAME_WITH_PREFIX}获得了\n{B_BUFF2}。");
static const u8 sText_PkmnCopiedFoe[] = _("{B_ATK_NAME_WITH_PREFIX}模仿了\n{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}！");
static const u8 sText_PkmnMadeWish[] = _("{B_ATK_NAME_WITH_PREFIX}许下愿望！");
static const u8 sText_PkmnWishCameTrue[] = _("{B_BUFF1}的愿望\n成真了！");
static const u8 sText_PkmnPlantedRoots[] = _("{B_ATK_NAME_WITH_PREFIX}把根扎入地下！");
static const u8 sText_PkmnAbsorbedNutrients[] = _("{B_ATK_NAME_WITH_PREFIX}从根茎中\n得到养分！");
static const u8 sText_PkmnAnchoredItself[] = _("{B_DEF_NAME_WITH_PREFIX}用根茎\n牢牢的固定了自己！");
static const u8 sText_PkmnWasMadeDrowsy[] = _("{B_ATK_NAME_WITH_PREFIX}使\n{B_DEF_NAME_WITH_PREFIX}昏昏欲睡！");
static const u8 sText_PkmnKnockedOff[] = _("{B_ATK_NAME_WITH_PREFIX}打破了\n{B_DEF_NAME_WITH_PREFIX}的{B_LAST_ITEM}！");
static const u8 sText_PkmnSwappedAbilities[] = _("{B_ATK_NAME_WITH_PREFIX}与对手\n交换了特性！");
static const u8 sText_PkmnSealedOpponentMove[] = _("{B_ATK_NAME_WITH_PREFIX}封住了\n对手的绝招！");
static const u8 sText_PkmnWantsGrudge[] = _("{B_ATK_NAME_WITH_PREFIX}向对手\n施加了怨恨！");
static const u8 sText_PkmnLostPPGrudge[] = _("{B_ATK_NAME_WITH_PREFIX}的{B_BUFF1}因怨恨失去了\n所有的PP！");
static const u8 sText_PkmnShroudedItself[] = _("{B_ATK_NAME_WITH_PREFIX}把自己\n笼罩在{B_CURRENT_MOVE}之中！");
static const u8 sText_PkmnMoveBounced[] = _("{B_ATK_NAME_WITH_PREFIX}的{B_CURRENT_MOVE}\n被魔术外衣反弹了！");
static const u8 sText_PkmnWaitsForTarget[] = _("{B_ATK_NAME_WITH_PREFIX}等待着对手\n来复制绝招！");
static const u8 sText_PkmnSnatchedMove[] = _("{B_DEF_NAME_WITH_PREFIX}抢夺了\n{B_SCR_ACTIVE_NAME_WITH_PREFIX}的绝招！");

static const u8 sText_ElectricityWeakened[] = _("电系的威力\n变弱了！");
static const u8 sText_FireWeakened[] = _("火系的威力\n减弱了！");
static const u8 sText_XFoundOneY[] = _("{B_ATK_NAME_WITH_PREFIX}找到了\n一个{B_LAST_ITEM}！");
static const u8 sText_SoothingAroma[] = _("一片沁人心田的香气\n在空气中弥漫开来！");
static const u8 sText_ItemsCantBeUsedNow[] = _("道具暂时不能用了！{PAUSE 64}");
static const u8 sText_ForXCommaYZ[] = _("对于{B_SCR_ACTIVE_NAME_WITH_PREFIX}，\n{B_LAST_ITEM} {B_BUFF1}");
static const u8 sText_PkmnUsedXToGetPumped[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}用\n{B_LAST_ITEM}来变得激动！");
static const u8 sText_PkmnLostFocus[] = _("{B_ATK_NAME_WITH_PREFIX}失去了目标\n而且无法动弹！");
static const u8 sText_PkmnWasDraggedOut[] = _("{B_DEF_NAME_WITH_PREFIX}被\n拖入了战斗！\p");
static const u8 sText_TheWallShattered[] = _("墙壁被劈碎了！");
static const u8 sText_ButNoEffect[] = _("但这没有产生任何效果！");
static const u8 sText_PkmnHasNoMovesLeft[] = _("{B_ACTIVE_NAME_WITH_PREFIX}没有\n可用的绝招了！\p");
static const u8 sText_PkmnMoveIsDisabled[] = _("{B_ACTIVE_NAME_WITH_PREFIX}的{B_CURRENT_MOVE}\n没用了！\p");
static const u8 sText_PkmnCantUseMoveTorment[] = _("{B_ACTIVE_NAME_WITH_PREFIX}在假指控下\n无法使出相同的绝招！\p");
static const u8 sText_PkmnCantUseMoveTaunt[] = _("{B_ACTIVE_NAME_WITH_PREFIX}在假指控下\n无法使用{B_CURRENT_MOVE}！\p");
static const u8 sText_PkmnCantUseMoveSealed[] = _("{B_ACTIVE_NAME_WITH_PREFIX}无法使用\n被封住的{B_CURRENT_MOVE}！\p");
static const u8 sText_PkmnMadeItRain[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nmade it rain!");
static const u8 sText_PkmnRaisedSpeed[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n使速度提升！");
static const u8 sText_PkmnProtectedBy[] = _("{B_DEF_NAME_WITH_PREFIX}被\n{B_DEF_ABILITY}保护了！");
static const u8 sText_PkmnPreventsUsage[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n阻止了{B_ATK_NAME_WITH_PREFIX}\l使用{B_CURRENT_MOVE}！");
static const u8 sText_PkmnRestoredHPUsing[] = _("{B_DEF_NAME_WITH_PREFIX}以{B_DEF_ABILITY}\n回复了HP！");
static const u8 sText_PkmnsXMadeYUseless[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n使{B_CURRENT_MOVE}不起作用！");
static const u8 sText_PkmnChangedTypeWith[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n让它变成了{B_BUFF1}属性！");
static const u8 sText_PkmnPreventsParalysisWith[] = _("{B_EFF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n防止了麻痹！");
static const u8 sText_PkmnPreventsRomanceWith[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n防止了迷人！");
static const u8 sText_PkmnPreventsPoisoningWith[] = _("{B_EFF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n防止了中毒！");
static const u8 sText_PkmnPreventsConfusionWith[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n防止了混乱！");
static const u8 sText_PkmnRaisedFirePowerWith[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n提升了炎系的威力！");
static const u8 sText_PkmnAnchorsItselfWith[] = _("{B_DEF_NAME_WITH_PREFIX}以{B_DEF_ABILITY}\n固定住了自己！");
static const u8 sText_PkmnCutsAttackWith[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n降低了{B_DEF_NAME_WITH_PREFIX}的攻击！");
static const u8 sText_PkmnPreventsStatLossWith[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n阻止了能力的下降！");
static const u8 sText_PkmnHurtsWith[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n伤害了{B_ATK_NAME_WITH_PREFIX}！");
static const u8 sText_PkmnTraced[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} TRACED\n{B_BUFF1}'s {B_BUFF2}!");
static const u8 sText_PkmnsXPreventsBurns[] = _("{B_EFF_NAME_WITH_PREFIX}的{B_EFF_ABILITY}\n防止了烧伤！");
static const u8 sText_PkmnsXBlocksY[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n阻拦了{B_CURRENT_MOVE}！");
static const u8 sText_PkmnsXBlocksY2[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n阻拦了{B_CURRENT_MOVE}！");
static const u8 sText_PkmnsXRestoredHPALittle2[] = _("{B_ATK_NAME_WITH_PREFIX}的{B_ATK_ABILITY}\n回复了一点点HP");
static const u8 sText_PkmnsXWhippedUpSandstorm[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n引发了沙雹！");
static const u8 sText_PkmnsXIntensifiedSun[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n引发了强烈的阳光！");
static const u8 sText_PkmnsXPreventsYLoss[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n阻止了{B_BUFF1}降低！");
static const u8 sText_PkmnsXInfatuatedY[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n让{B_ATK_NAME_WITH_PREFIX}着迷了!");
static const u8 sText_PkmnsXMadeYIneffective[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n使{B_CURRENT_MOVE}无效！");
static const u8 sText_PkmnsXCuredYProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n治好了{B_BUFF1}！");
static const u8 sText_ItSuckedLiquidOoze[] = _("但它吸收了\n黏液！");
static const u8 sText_PkmnTransformed[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}转变了！");
static const u8 sText_PkmnsXTookAttack[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n承受住了攻击！");
const u8 gText_PkmnsXPreventsSwitching[] = _("{B_BUFF1}的{B_LAST_ABILITY}\n防止了交换！\p");
static const u8 sText_PreventedFromWorking[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_DEF_ABILITY}\n阻止了{B_SCR_ACTIVE_NAME_WITH_PREFIX}的\l{B_BUFF1}！");
static const u8 sText_PkmnsXMadeItIneffective[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n使其失去效果！");
static const u8 sText_PkmnsXPreventsFlinching[] = _("{B_EFF_NAME_WITH_PREFIX}的{B_EFF_ABILITY}\n防止了害怕！");
static const u8 sText_PkmnsXPreventsYsZ[] = _("{B_ATK_NAME_WITH_PREFIX}的{B_ATK_ABILITY}\n阻止了{B_DEF_NAME_WITH_PREFIX}的\l{B_DEF_ABILITY}！");
static const u8 sText_PkmnsXCuredItsYProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n治好了{B_BUFF1}！");
static const u8 sText_PkmnsXHadNoEffectOnY[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_SCR_ACTIVE_ABILITY}\n对{B_EFF_NAME_WITH_PREFIX}无效！");
static const u8 sText_TooScaredToMove[] = _("{B_ATK_NAME_WITH_PREFIX}因过于恐惧而无法动弹！");
static const u8 sText_GetOutGetOut[] = _("GHOST: Get out…… Get out……");
static const u8 sText_StatSharply[] = _("急剧");
const u8 gBattleText_Rose[] = _("提升！");
static const u8 sText_StatHarshly[] = _("大幅");
static const u8 sText_StatFell[] = _("降低！");
static const u8 sText_PkmnsStatChanged[] = _("{B_ATK_NAME_WITH_PREFIX}的{B_BUFF1}\n{B_BUFF2}");
const u8 gText_PkmnsStatChanged2[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_BUFF1}\n{B_BUFF2}");
static const u8 sText_UsingXTheYOfZN[] = _("因为{B_LAST_ITEM}，{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_BUFF1}\n{B_BUFF2}");
static const u8 sText_PkmnsStatChanged3[] = _("{B_ATK_NAME_WITH_PREFIX}的{B_BUFF1}\n{B_BUFF2}");
static const u8 sText_PkmnsStatChanged4[] = _("{B_DEF_NAME_WITH_PREFIX}的{B_BUFF1}\n{B_BUFF2}");
static const u8 sText_StatsWontIncrease2[] = _("{B_ATK_NAME_WITH_PREFIX}的能力无法\n再提升了！");
static const u8 sText_StatsWontDecrease2[] = _("{B_DEF_NAME_WITH_PREFIX}的能力无法\n再降低了！");
static const u8 sText_CriticalHit[] = _("击中要害！");
static const u8 sText_OneHitKO[] = _("一击必杀！");
static const u8 sText_123Poof[] = _("{PAUSE 32}1, {PAUSE 15}2, and{PAUSE 15}… {PAUSE 15}… {PAUSE 15}… {PAUSE 15}{PLAY_SE SE_BALL_BOUNCE_1}Poof!\p");
static const u8 sText_AndEllipsis[] = _("And…\p");
static const u8 sText_HMMovesCantBeForgotten[] = _("HM moves can't be\nforgotten now.\p");
static const u8 sText_NotVeryEffective[] = _("收效甚微……");
static const u8 sText_SuperEffective[] = _("效果拔群！");
static const u8 sText_GotAwaySafely[] = _("{PLAY_SE SE_FLEE}顺利地逃跑了！\p");
static const u8 sText_PkmnFledUsingIts[] = _("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX}以{B_LAST_ITEM}\n逃跑了！\p");
static const u8 sText_PkmnFledUsing[] = _("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} fled\nusing {B_ATK_ABILITY}!\p");
static const u8 sText_WildPkmnFled[] = _("{PLAY_SE SE_FLEE}Wild {B_BUFF1} fled!");
static const u8 sText_PlayerDefeatedLinkTrainer[] = _("Player defeated\n{B_LINK_OPPONENT1_NAME}!");
static const u8 sText_TwoLinkTrainersDefeated[] = _("Player beat {B_LINK_OPPONENT1_NAME}\nand {B_LINK_OPPONENT2_NAME}!");
static const u8 sText_PlayerLostAgainstLinkTrainer[] = _("Player lost against\n{B_LINK_OPPONENT1_NAME}!");
static const u8 sText_PlayerLostToTwo[] = _("Player lost to {B_LINK_OPPONENT1_NAME}\nand {B_LINK_OPPONENT2_NAME}!");
static const u8 sText_PlayerBattledToDrawLinkTrainer[] = _("Player battled to a draw against\n{B_LINK_OPPONENT1_NAME}!");
static const u8 sText_PlayerBattledToDrawVsTwo[] = _("Player battled to a draw against\n{B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!");
static const u8 sText_WildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} fled!");
static const u8 sText_TwoWildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} and\n{B_LINK_OPPONENT2_NAME} fled!");
static const u8 sText_NoRunningFromTrainers[] = _("与训练师的对战时\n不能逃跑！\p");
static const u8 sText_CantEscape[] = _("无法逃跑！\p");
static const u8 sText_EmptyString5[] = _("");
static const u8 sText_ButNothingHappened[] = _("但是什么事也没发生！");
static const u8 sText_ButItFailed[] = _("但是失败了!");
static const u8 sText_ItHurtConfusion[] = _("也不知为什么\n攻击了自己！");
static const u8 sText_MirrorMoveFailed[] = _("学舌术的效果不好决定！");
static const u8 sText_StartedToRain[] = _("开始下雨了！");
static const u8 sText_DownpourStarted[] = _("雨下大了！");
static const u8 sText_RainContinues[] = _("雨继续下着。");
static const u8 sText_DownpourContinues[] = _("滂沱大雨继续下着。");
static const u8 sText_RainStopped[] = _("雨停了。");
static const u8 sText_SandstormBrewed[] = _("扬起了沙暴！");
static const u8 sText_SandstormRages[] = _("飞沙走石。");
static const u8 sText_SandstormSubsided[] = _("沙暴停了下来。");
static const u8 sText_SunlightGotBright[] = _("阳光变强了！");
static const u8 sText_SunlightStrong[] = _("阳光很强烈。");
static const u8 sText_SunlightFaded[] = _("阳光柔和下来了。");
static const u8 sText_StartedHail[] = _("开始下冰雹了！");
static const u8 sText_HailContinues[] = _("冰雹持续下着。");
static const u8 sText_HailStopped[] = _("冰雹停了。");
static const u8 sText_FailedToSpitUp[] = _("但是什么也没\n喷出！");
static const u8 sText_FailedToSwallow[] = _("但是没东西可以\n吞下！");
static const u8 sText_WindBecameHeatWave[] = _("风变成了\n热风！");
static const u8 sText_StatChangesGone[] = _("所有的能力变化\n都还原了！");
static const u8 sText_CoinsScattered[] = _("硬币撒的到处都是！");
static const u8 sText_TooWeakForSubstitute[] = _("但是体力不够！\n造不出替身！");
static const u8 sText_SharedPain[] = _("平分了\n双方的体力！");
static const u8 sText_BellChimed[] = _("铃声响起来了！");
static const u8 sText_FaintInThree[] = _("所有受到影响的宝可梦\n都会在三回合内倒下！");
static const u8 sText_NoPPLeft[] = _("没有PP的绝招\n无法使出！\p");
static const u8 sText_ButNoPPLeft[] = _("但是没有PP的绝招\n无法使出！");
static const u8 sText_PkmnIgnoresAsleep[] = _("{B_ATK_NAME_WITH_PREFIX}没办法在\n睡着的时候听从指挥！");
static const u8 sText_PkmnIgnoredOrders[] = _("{B_ATK_NAME_WITH_PREFIX}不听从\n指挥！");
static const u8 sText_PkmnBeganToNap[] = _("{B_ATK_NAME_WITH_PREFIX}开始打盹了！");
static const u8 sText_PkmnLoafing[] = _("{B_ATK_NAME_WITH_PREFIX}在\n一旁偷懒！");
static const u8 sText_PkmnWontObey[] = _("{B_ATK_NAME_WITH_PREFIX}无视了\n指挥！");
static const u8 sText_PkmnTurnedAway[] = _("{B_ATK_NAME_WITH_PREFIX}不听话！");
static const u8 sText_PkmnPretendNotNotice[] = _("{B_ATK_NAME_WITH_PREFIX}在一旁\n装傻！");
static const u8 sText_EnemyAboutToSwitchPkmn[] = _("{B_TRAINER1_CLASS}{B_TRAINER1_NAME}要\n派出{B_BUFF2}了。\p{B_PLAYER_NAME}要交换\n宝可梦吗？");
static const u8 sText_PkmnLearnedMove2[] = _("{B_ATK_NAME_WITH_PREFIX}掌握了\n{B_BUFF1}！");
static const u8 sText_PlayerDefeatedLinkTrainerTrainer1[] = _("玩家战胜了\n{B_TRAINER1_CLASS}{B_TRAINER1_NAME}！\p");
static const u8 sText_ThrewARock[] = _("{B_PLAYER_NAME}向{B_OPPONENT_MON1_NAME}\n丢石头！");
static const u8 sText_ThrewSomeBait[] = _("{B_PLAYER_NAME}向{B_OPPONENT_MON1_NAME}\n丢饵食！");
static const u8 sText_PkmnWatchingCarefully[] = _("{B_OPPONENT_MON1_NAME} is watching\ncarefully!");
static const u8 sText_PkmnIsAngry[] = _("{B_OPPONENT_MON1_NAME}很生气！");
static const u8 sText_PkmnIsEating[] = _("{B_OPPONENT_MON1_NAME}正在吃饵食！");
static const u8 sText_OutOfSafariBalls[] = _("{PLAY_SE SE_DING_DONG} ANNOUNCER: You're out of\nSAFARI BALLS! Game over!\p");
static const u8 sText_WildPkmnAppeared[] = _("野生的{B_OPPONENT_MON1_NAME}出现了!\p");
static const u8 sText_WildPkmnAppeared2[] = _("野生的{B_OPPONENT_MON1_NAME}出现了!\p");
static const u8 sText_WildPkmnAppearedPause[] = _("野生的{B_OPPONENT_MON1_NAME}出现了!{PAUSE 64}");
static const u8 sText_TwoWildPkmnAppeared[] = _("野生的{B_OPPONENT_MON1_NAME}和\n{B_OPPONENT_MON2_NAME}出现了!\p");
static const u8 sText_GhostAppearedCantId[] = _("幽灵出来了！\p可是……\n图鉴里查不到幽灵的资料！\p");
static const u8 sText_TheGhostAppeared[] = _("幽灵出现了！\p");
static const u8 sText_SilphScopeUnveil[] = _("希鲁夫观测镜识破了幽灵的\n真实身份！");
static const u8 sText_TheGhostWas[] = _("幽灵原来是嘎拉嘎拉！\p\n");
static const u8 sText_Trainer1WantsToBattle[] = _("与{B_TRAINER1_CLASS}{B_TRAINER1_NAME}\n的战斗开始了！\p");
static const u8 sText_LinkTrainerWantsToBattle[] = _("与{B_LINK_OPPONENT1_NAME}\n的战斗开始了！");
static const u8 sText_TwoLinkTrainersWantToBattle[] = _("{B_LINK_OPPONENT1_NAME}和{B_LINK_OPPONENT2_NAME}\n出面挑战！");
static const u8 sText_Trainer1SentOutPkmn[] = _("{B_TRAINER1_CLASS}{B_TRAINER1_NAME}派上了\n{B_OPPONENT_MON1_NAME}！{PAUSE 64}！");
static const u8 sText_Trainer1SentOutTwoPkmn[] = _("{B_TRAINER1_CLASS}{B_TRAINER1_NAME}派上了\n{B_OPPONENT_MON1_NAME}和{B_OPPONENT_MON2_NAME}！{PAUSE 64}！");
static const u8 sText_Trainer1SentOutPkmn2[] = _("{B_TRAINER1_CLASS}{B_TRAINER1_NAME}派上了\n{B_BUFF1}！");
static const u8 sText_LinkTrainerSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME}派上了\n{B_OPPONENT_MON1_NAME}！");
static const u8 sText_LinkTrainerSentOutTwoPkmn[] = _("{B_LINK_OPPONENT1_NAME}派上了\n{B_OPPONENT_MON1_NAME}和{B_OPPONENT_MON2_NAME}！");
static const u8 sText_TwoLinkTrainersSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME}派上了{B_LINK_OPPONENT_MON1_NAME}！\n{B_LINK_OPPONENT2_NAME}派上了{B_LINK_OPPONENT_MON2_NAME}！");
static const u8 sText_LinkTrainerSentOutPkmn2[] = _("{B_LINK_OPPONENT1_NAME}派上了\n{B_BUFF1}！");
static const u8 sText_LinkTrainerMultiSentOutPkmn[] = _("{B_LINK_SCR_TRAINER_NAME}派上了\n{B_BUFF1}！");
static const u8 sText_GoPkmn[] = _("Go! {B_PLAYER_MON1_NAME}!");
static const u8 sText_GoTwoPkmn[] = _("Go! {B_PLAYER_MON1_NAME} and\n{B_PLAYER_MON2_NAME}!");
static const u8 sText_GoPkmn2[] = _("Go! {B_BUFF1}!");
static const u8 sText_DoItPkmn[] = _("Do it! {B_BUFF1}!");
static const u8 sText_GoForItPkmn[] = _("加油了！{B_BUFF1}！");
static const u8 sText_YourFoesWeakGetEmPkmn[] = _("对手很弱了！\n机会来了，{B_BUFF1}！");
static const u8 sText_LinkPartnerSentOutPkmnGoPkmn[] = _("{B_LINK_PARTNER_NAME} sent out {B_LINK_PLAYER_MON2_NAME}!\nGo! {B_LINK_PLAYER_MON1_NAME}!");
static const u8 sText_PkmnThatsEnough[] = _("够了，{B_BUFF1}，\n辛苦你了！回来吧！");
static const u8 sText_PkmnComeBack[] = _("回来吧，{B_BUFF1}！");
static const u8 sText_PkmnOkComeBack[] = _("{B_BUFF1}，好样的！\n回来吧！");
const u8 sText_PkmnGoodComeBack[] = _("{B_BUFF1}，干得漂亮！\n回来吧！");
static const u8 sText_Trainer1WithdrewPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\n收回了{B_BUFF1}！");
static const u8 sText_LinkTrainer1WithdrewPkmn[] = _("{B_LINK_OPPONENT1_NAME}收回了\n{B_BUFF1}！");
static const u8 sText_LinkTrainer2WithdrewPkmn[] = _("{B_LINK_SCR_TRAINER_NAME}收回了\n{B_BUFF1}！");
static const u8 sText_WildPkmnPrefix[] = _("野生的");
static const u8 sText_FoePkmnPrefix[] = _("对方");
static const u8 sText_FoePkmnPrefix2[] = _("对方");
static const u8 sText_AllyPkmnPrefix[] = _("我方");
static const u8 sText_FoePkmnPrefix3[] = _("对方");
static const u8 sText_AllyPkmnPrefix2[] = _("我方");
static const u8 sText_FoePkmnPrefix4[] = _("对方");
static const u8 sText_AllyPkmnPrefix3[] = _("我方");
static const u8 sText_AttackerUsedX[] = _("{B_ATK_NAME_WITH_PREFIX}使用\n{B_BUFF2}");
static const u8 sText_ExclamationMark[] = _("!");
static const u8 sText_ExclamationMark2[] = _("!");
static const u8 sText_ExclamationMark3[] = _("!");
static const u8 sText_ExclamationMark4[] = _("!");
static const u8 sText_ExclamationMark5[] = _("!");

static const u8 sText_HP2[] = _("HP");
static const u8 sText_Attack2[] = _("攻击");
static const u8 sText_Defense2[] = _("防御");
static const u8 sText_Speed[] = _("速度");
static const u8 sText_SpAtk2[] = _("特攻");
static const u8 sText_SpDef2[] = _("特防");
static const u8 sText_Accuracy[] = _("命中率");
static const u8 sText_Evasiveness[] = _("回避率");

const u8 sText_TheOpposingNoCaps[] = _("对方");
const u8 sText_Your[] = _("我方");

const u8 *const gStatNamesTable[] = {
    sText_HP2,
    sText_Attack2,
    sText_Defense2,
    sText_Speed,
    sText_SpAtk2,
    sText_SpDef2,
    sText_Accuracy,
    sText_Evasiveness
};

static const u8 sText_PokeblockWasTooSpicy[] = _("was too spicy!"); //
static const u8 sText_PokeblockWasTooDry[] = _("was too dry!");
static const u8 sText_PokeblockWasTooSweet[] = _("was too sweet!");
static const u8 sText_PokeblockWasTooBitter[] = _("was too bitter!");
static const u8 sText_PokeblockWasTooSour[] = _("was too sour!");

const u8 *const gPokeblockWasTooXStringTable[] = {
    sText_PokeblockWasTooSpicy,
    sText_PokeblockWasTooDry,
    sText_PokeblockWasTooSweet,
    sText_PokeblockWasTooBitter,
    sText_PokeblockWasTooSour
};

static const u8 sText_PlayerUsedItem[] = _("{B_PLAYER_NAME}使用了\n{B_LAST_ITEM}!");
static const u8 sText_OldManUsedItem[] = _("老爷爷使用了\n{B_LAST_ITEM}！");
static const u8 sText_PokedudeUsedItem[] = _("宝可梦老大哥使用了\n{B_LAST_ITEM}！");
static const u8 sText_Trainer1UsedItem[] = _("{B_TRAINER1_CLASS}{B_TRAINER1_NAME}\n使用了{B_LAST_ITEM}！");
static const u8 sText_TrainerBlockedBall[] = _("训练师把球打飞了！");
static const u8 sText_DontBeAThief[] = _("不要做小偷！");
static const u8 sText_ItDodgedBall[] = _("球被避开了！\n这只宝可梦不能收服！");
static const u8 sText_YouMissedPkmn[] = _("宝可梦逃脱了！");
static const u8 sText_PkmnBrokeFree[] = _("哦，不！\n宝可梦从球里蹦出来了！");
static const u8 sText_ItAppearedCaught[] = _("哎呀！\n我还以为抓住了！");
static const u8 sText_AarghAlmostHadIt[] = _("可恶！\n差一点就收服了！");
static const u8 sText_ShootSoClose[] = _("真可惜！\n就差一点点了！");
const u8 gUnknown_83FD78A[] = _("よけられた!\nこいつは つかまりそうにないぞ!");
static const u8 sText_GotchaPkmnCaught[] = _("成功了！\n收服{B_OPPONENT_MON1_NAME}了！{WAIT_SE}{PLAY_BGM MUS_CAUGHT}");
static const u8 sText_GotchaPkmnCaught2[] = _("成功了！\n收服{B_OPPONENT_MON1_NAME}了！{WAIT_SE}{PLAY_BGM MUS_CAUGHT}");
static const u8 sText_GiveNicknameCaptured[] = _("要起一个昵称\n给{B_OPPONENT_MON1_NAME}吗？");
static const u8 sText_PkmnSentToPC[] = _("{B_OPPONENT_MON1_NAME}被传送到\n{B_PC_CREATOR_NAME}电脑了！");
static const u8 sText_Someones[] = _("某人的");
static const u8 sText_Bills[] = _("正辉的");
static const u8 sText_PkmnDataAddedToDex[] = _("{B_OPPONENT_MON1_NAME}的资料被\n记录在宝可梦图鉴上了。\p");
static const u8 sText_ItIsRaining[] = _("现在在下雨。");
static const u8 sText_SandstormIsRaging[] = _("沙暴很猛烈。");
static const u8 sText_BoxIsFull[] = _("寄存箱满了！\n没办法继续收服了！\p");
static const u8 sText_EnigmaBerry[] = _("谜之果");
static const u8 sText_BerrySuffix[] = _(" 果");
static const u8 gUnknown_83FD8B6[] = _("ナゾ");
static const u8 sText_PkmnsItemCuredParalysis[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_LAST_ITEM}\n治愈了麻痹状态！");
static const u8 sText_PkmnsItemCuredPoison[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ncured poison!");
static const u8 sText_PkmnsItemHealedBurn[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_LAST_ITEM}\n治愈了烧伤状态！");
static const u8 sText_PkmnsItemDefrostedIt[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ndefrosted it!");
static const u8 sText_PkmnsItemWokeIt[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_LAST_ITEM}\n治愈了睡眠状态！");
static const u8 sText_PkmnsItemSnappedOut[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_LAST_ITEM}\n治愈了混乱状态！");
static const u8 sText_PkmnsItemCuredProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_LAST_ITEM}\n治愈了{B_BUFF1}状态！");
static const u8 sText_PkmnsItemNormalizedStatus[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_LAST_ITEM}\n恢复了其能力！");
static const u8 sText_PkmnsItemRestoredHealth[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_LAST_ITEM}\n恢复体力了！");
static const u8 sText_PkmnsItemRestoredPP[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_LAST_ITEM}\n回复了{B_BUFF1}的PP！");
static const u8 sText_PkmnsItemRestoredStatus[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_LAST_ITEM}\n使其能力恢复了！");
static const u8 sText_PkmnsItemRestoredHPALittle[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}的{B_LAST_ITEM}\n使其HP回复了一点！");
static const u8 sText_ItemAllowsOnlyYMove[] = _("受{B_LAST_ITEM}的影响，\n只有{B_COPY_VAR_1}有用！\p");
static const u8 sText_PkmnHungOnWithX[] = _("{B_DEF_NAME_WITH_PREFIX}因{B_LAST_ITEM}\n而撑住了！");
const u8 gText_EmptyString3[] = _("");
static const u8 sText_PlayedFluteCatchyTune[] = _("{B_PLAYER_NAME}吹奏了{B_LAST_ITEM}。\p悠扬的笛声响起来了！");
static const u8 sText_PlayedThe[] = _("{B_PLAYER_NAME}吹奏了\n{B_LAST_ITEM}。");
static const u8 sText_PkmnHearingFluteAwoke[] = _("宝可梦听到笛声\n醒过来了！");
static const u8 sText_YouThrowABallNowRight[] = _("你说，现在该丢球了对吗？\n我……我会全力以赴的！");
const u8 gText_ForPetesSake[] = _("大木：哎，真没办法……\n我孙子他还是那么容易冲动啊。\p{B_PLAYER_NAME}。\p你以前从未接触过宝可梦对战，\n对吧？\p宝可梦对战就是训练师们\n使用各自的宝可梦来\l比试胜负。\p");
const u8 gText_TheTrainerThat[] = _("在对战中把对手训练师的\n宝可梦的HP降为0的话，\l即可取得胜利。\p");
const u8 gText_TryBattling[] = _("但是与其这样纸上谈兵，\n还是亲自尝试对战看看吧。\p相信你会从中得到很多经验的。\p");
const u8 gText_InflictingDamageIsKey[] = _("大木：给对手造成的损伤，\n是对战的关键所在。\p");
const u8 gText_LoweringStats[] = _("大木：降低对手的能力\n可使对战对你更有利哦。\p");
const u8 gText_KeepAnEyeOnHP[] = _("大木：请把目光转移到你的\n宝可梦的HP上。\p一旦HP降为0，\n它就会倒下。\p");
const u8 gText_OakNoRunningFromATrainer[] = _("大木：不行！与训练师之间的\n宝可梦对战不能逃！\p");
const u8 gText_WinEarnsPrizeMoney[] = _("大木：嗯！非常好！\p赢了的话可以获得赏金，\n你的宝可梦也会成长！\p多多与其他训练师对战来使\n宝可梦变强吧！\p");
const u8 gText_HowDissapointing[] = _("大木：呃……\n真遗憾啊……\p如果你赢了，可以获得赏金，\n你的宝可梦也会成长。\p但是如果输了，{B_PLAYER_NAME}，你就要\n付给对手赏金……\p可是，看在你这次还没有经验，\n赏金我替你付吧。\p是不是输掉了觉得很不甘心？\n还是下次再努力吧。\p多多与野生宝可梦对战，\n让自己的宝可梦变强吧。\p");

const u8 sText_TheOpposingCapsNoSpace[] = _("");
const u8 sText_YourCaps[] = _("");

const u8 DisguiseBustedString[] = _("sss");
const u8 HarvestString[] = _("sss");
const u8 PickupString[] = _("sss");
const u8 TransformedString[] = _("sss");
const u8 HurtByAbilityString[] = _("sss");
const u8 gText_MimicryReturnedToNormal[] = _("sss");
const u8 gText_AbilityChangedType[] = _("sss");
const u8 BadDreamsHurtString[] = _("sss");
const u8 CantUseMoveString[] = _("sss");
const u8 CursedBodyString[] = _("sss");
const u8 MummyString[] = _("sss");
const u8 SynchronizeNoEffectString[] = _("sss");
const u8 IllusionWoreOffString[] = _("sss");
const u8 AngerPointString[] = _("sss");
const u8 MimikyuDisguisedTookDamageString[] = _("sss");

const u8 gText_DevolvedForgotMove[] = _("sss");
const u8 CantUseHyperspaceFuryString[] = _("sss");
const u8 gText_DynamaxAttackCancel[] = _("sss");
const u8 gText_RaidBattleAttackCancel[] = _("sss");
const u8 GravityAttackCancelString[] = _("sss");
const u8 HealBlockAttackCancelString[] = _("sss");
const u8 ThroatChopAttackCancelString[] = _("sss");
const u8 MoveEvaporatedInHarshSunString[] = _("sss");
const u8 ZPowerSurroundsString[] = _("sss");
const u8 ZMoveUnleashedString[] = _("sss");
const u8 gText_RaidBattleStatNullification[] = _("sss");
const u8 MoveFizzledInHeavyRainString[] = _("sss");
const u8 String_SwitchedToBladeForm[] = _("sss");
const u8 PowderExplosionString[] = _("sss");
const u8 String_SwitchedToShieldForm[] = _("sss");
const u8 SkyBattleAttackCancelString[] = _("sss");
const u8 PsychicTerrainAttackCancelString[] = _("sss");
const u8 WrongHoopaFormString[] = _("sss");
const u8 SlappedAwakeString[] = _("sss");
const u8 BurnCuredString[] = _("sss");
const u8 DestinyKnotString[] = _("sss");
const u8 FreezeShockChargingString[] = _("sss");
const u8 IceBurnChargingString[] = _("sss");
const u8 ShadowForceString[] = _("sss");
const u8 gText_MeteorBeamCharge[] = _("sss");
const u8 PowerHerbString[] = _("sss");
const u8 ReflectTypeString[] = _("sss");
const u8 BecameNimbleString[] = _("sss");
const u8 gText_AromaVeilProtects[] = _("sss");
const u8 LaserFocusString[] = _("sss");
const u8 gText_JawLockNoEscape[] = _("sss");
const u8 gText_BellyDrumMinimizedAttack[] = _("{B_ATK_NAME_WITH_PREFIX}消减了自己的生命值\n并最小化攻击!");
const u8 BurnUpString[] = _("sss");
const u8 SpectralThiefString[] = _("sss");
const u8 SkyDropUpString[] = _("sss");
const u8 FlameBurstString[] = _("sss");
const u8 MagnetRiseSetString[] = _("sss");
const u8 QuashString[] = _("sss");
const u8 AfterYouString[] = _("sss");
const u8 ElectrifyString[] = _("sss");
const u8 TelekinesisSetString[] = _("sss");
const u8 TopsyTurvyString[] = _("sss");
const u8 PowderSetString[] = _("sss");
const u8 EmbargoSetString[] = _("sss");
const u8 HealBlockSetString[] = _("sss");
const u8 ProtectLiftedString[] = _("sss");
const u8 FlingString[] = _("sss");
const u8 StockpileWoreOffString[] = _("sss");
const u8 PsychoShiftString[] = _("sss");
const u8 GeomancyChargeString[] = _("sss");
const u8 BestowString[] = _("sss");
const u8 AquaRingSetString[] = _("sss");
const u8 PledgeReadyString[] = _("sss");
const u8 PledgeCombinedString[] = _("sss");
const u8 gText_TargetStaysAwake[] = _("sss");
const u8 HealBlockTargetString[] = _("sss");
const u8 gText_DefogRemovedSideStatus[] = _("sss");
const u8 AdrenalineOrbString[] = _("sss");
const u8 AbilityRaisedStatString[] = _("sss");
const u8 ReceiverString[] = _("sss");
const u8 SymbiosisString[] = _("sss");
const u8 BlackSludgeHurtString[] = _("sss");
const u8 gText_Devolved[] = _("sss");
const u8 gText_DevolutionsStatsReset[] = _("sss");
const u8 AirBalloonPopString[] = _("sss");
const u8 MicleBerryString[] = _("sss");
const u8 StickyBarbString[] = _("sss");
const u8 CheekPouchString[] = _("sss");
const u8 EjectButtonString[] = _("sss");
const u8 gText_SweetVeilProtects[] = _("sss");
const u8 RemoveFogString[] = _("sss");
const u8 UTurnString[] = _("sss");
const u8 PurifyString[] = _("sss");
const u8 ScriptingBattlerFainted[] = _("sss");

const u8 MegaReactingString[] = _("sss");
const u8 MegaEvolutionCompleteString[] = _("sss");
const u8 FerventWishString[] = _("sss");
const u8 UltraBurstGlowingString[] = _("sss");
const u8 UltraBurstCompleteString[] = _("sss");
const u8 StatsResetZMoveString[] = _("sss");
const u8 StatRaisedZMoveString[] = _("sss");
const u8 StatsRaisedZMoveString[] = _("sss");
const u8 CritBoostedZMoveString[] = _("sss");
const u8 HPRestoredZMoveString[] = _("sss");
const u8 HPWillRestoreZMoveString[] = _("sss");
const u8 HPSwitchInRestoredZMoveString[] = _("sss");
const u8 AirBalloonEntryString[] = _("sss");
const u8 TotemAuraFlared[] = _("sss");
const u8 PrimalReversionString[] = _("sss");
const u8 ElectricTerrainBattleBeginString[] = _("sss");
const u8 GrassyTerrainBattleBeginString[] = _("sss");
const u8 MistyTerrainBattleBeginString[] = _("sss");
const u8 PsychicTerrainBattleBeginString[] = _("sss");
const u8 gText_ItemIncreasedSpeedBracket[] = _("sss");
const u8 gText_RaidBattleStormStarted[] = _("sss");
const u8 BeakBlastWarmUpString[] = _("sss");
const u8 ShellTrapSetString[] = _("sss");
const u8 StringNull[] = _("");

const u8 ToxicSpikesBadPSNString[] = _("sss");
const u8 LifeOrbString[] = _("sss");
const u8 ToxicSpikesPSNString[] = _("sss");
const u8 ToxicOrbString[] = _("sss");
const u8 MindBlownString[] = _("sss");
const u8 SpikyShieldString[] = _("sss");
const u8 BeakBlastString[] = _("sss");
const u8 FlameOrbString[] = _("sss");
const u8 gText_RaidPokemonGettingDesperate[] = _("sss");
const u8 gText_RaidBattleStatBoost[] = _("sss");
const u8 PluckString[] = _("sss");
const u8 gText_RaidShield[] = _("sss");
const u8 gText_RaidShieldBroke[] = _("{B_DEF_NAME_WITH_PREFIX} couldn't\nfully protect itself and got hurt!");
const u8 BanefulBunkerPSNString[] = _("{B_DEF_NAME_WITH_PREFIX} couldn't\nfully protect itself and got hurt!");

const u8 TrainerBLoseString[] = _("sss");
const u8 TrainerBVictoryString[] = _("sss");
const u8 TrainerAWinString[] = _("sss");
const u8 ToCoreString[] = _("sss");
const u8 ToMeteorString[] = _("sss");
const u8 RockyHelmetString[] = _("sss");
const u8 WeaknessPolicyString[] = _("sss");
const u8 gText_DynamaxItemBlock[] = _("sss");
const u8 RedCardIngrainString[] = _("sss");
const u8 RedCardSuctionCupsString[] = _("sss");
const u8 GemString[] = _("sss");
const u8 RedCardString[] = _("sss");
const u8 gText_RaidMonRanAway[] = _("sss");
const u8 WeaknessBerryString[] = _("sss");
const u8 MysteriousAirCurrentContinuesString[] = _("sss");
const u8 FogEndedString[] = _("sss");
const u8 FogContinuesString[] = _("sss");
const u8 GrassyTerrainHealString[] = _("sss");
const u8 AquaRingHealString[] = _("sss");
const u8 MagnetRiseEndString[] = _("sss");
const u8 TelekinesisStringEndString[] = _("sss");
const u8 HealBlockEndString[] = _("sss");
const u8 EmbargoEndString[] = _("sss");
const u8 gText_TailwindWoreOff[] = _("sss");
const u8 gText_LuckyChantWoreOff[] = _("sss");
const u8 WaterSportEndString[] = _("sss");
const u8 MudSportEndString[] = _("sss");
const u8 PresenceOfManyString[] = _("sss");
const u8 gText_DynamaxEnded[] = _("sss");
const u8 sText_PlayerForfeitedMatch[] = _("sss");
const u8 sText_QuestionForfeitMatch[] = _("sss");
const u8 PowerConstructCompleteString[] = _("sss");
const u8 BattleText_TwoInGameTrainersDefeated[] = _("sss");

const u8 gText_FlowerVeilProtects[] = _("sss");
const u8 gText_TargetStaysAwak[] = _("sss");
const u8 gText_HurtBySpikes[] = _("{B_DEF_NAME_WITH_PREFIX}被尖刺攻击了!");
const u8 gText_HurtByStealthRock[] = _("漂浮的尖石伤害了{B_DEF_NAME_WITH_PREFIX}!");
const u8 gText_HurtBySteelsurge[] = _("锋利的铁片伤害了{B_DEF_NAME_WITH_PREFIX}!");
const u8 gText_AbsorbedToxicSpikes[] = _("毒刺从{B_DEF_NAME_WITH_PREFIX}的周围消失了!");
const u8 gText_CaughtInStickyWeb[] = _("{B_DEF_NAME_WITH_PREFIX}被黏黏网粘着了!");
const u8 HealingWishHealString[] = _("sss");
const u8 ItemCantBeRemovedString[] = _("sss");
const u8 LunarDanceHealString[] = _("sss");

const u8 PrimalRainEndString[] = _("Primal rain end!");
const u8 PrimalSunEndString[] = _("Primal sun end!");
const u8 PrimalAirCurrentEndString[] = _("Air current end!");

//const u8 gText_CouldntFullyProtect[] = _("nothing");

const u8 *const gBattleStringsTable[] = {
    [STRINGID_TRAINER1LOSETEXT - 12]              = sText_Trainer1LoseText,
    [STRINGID_PKMNGAINEDEXP - 12]                 = sText_PkmnGainedEXP,
    [STRINGID_PKMNGREWTOLV - 12]                  = sText_PkmnGrewToLv,
    [STRINGID_PKMNLEARNEDMOVE - 12]               = sText_PkmnLearnedMove,
    [STRINGID_TRYTOLEARNMOVE1 - 12]               = sText_TryToLearnMove1,
    [STRINGID_TRYTOLEARNMOVE2 - 12]               = sText_TryToLearnMove2,
    [STRINGID_TRYTOLEARNMOVE3 - 12]               = sText_TryToLearnMove3,
    [STRINGID_PKMNFORGOTMOVE - 12]                = sText_PkmnForgotMove,
    [STRINGID_STOPLEARNINGMOVE - 12]              = sText_StopLearningMove,
    [STRINGID_DIDNOTLEARNMOVE - 12]               = sText_DidNotLearnMove,
    [STRINGID_PKMNLEARNEDMOVE2 - 12]              = sText_PkmnLearnedMove2,
    [STRINGID_ATTACKMISSED - 12]                  = sText_AttackMissed,
    [STRINGID_PKMNPROTECTEDITSELF - 12]           = sText_PkmnProtectedItself,
    [STRINGID_STATSWONTINCREASE2 - 12]            = sText_StatsWontIncrease2,
    [STRINGID_AVOIDEDDAMAGE - 12]                 = sText_AvoidedDamage,
    [STRINGID_ITDOESNTAFFECT - 12]                = sText_ItDoesntAffect,
    [STRINGID_ATTACKERFAINTED - 12]               = sText_AttackerFainted,
    [STRINGID_TARGETFAINTED - 12]                 = sText_TargetFainted,
    [STRINGID_PLAYERGOTMONEY - 12]                = sText_PlayerGotMoney,
    [STRINGID_PLAYERWHITEOUT - 12]                = sText_PlayerWhiteout,
    [STRINGID_PLAYERWHITEOUT2 - 12]               = sText_PlayerPanicked,
    [STRINGID_PREVENTSESCAPE - 12]                = sText_PreventsEscape,
    [STRINGID_HITXTIMES - 12]                     = sText_HitXTimes,
    [STRINGID_PKMNFELLASLEEP - 12]                = sText_PkmnFellAsleep,
    [STRINGID_PKMNMADESLEEP - 12]                 = sText_PkmnMadeSleep,
    [STRINGID_PKMNALREADYASLEEP - 12]             = sText_PkmnAlreadyAsleep,
    [STRINGID_PKMNALREADYASLEEP2 - 12]            = sText_PkmnAlreadyAsleep2,
    [STRINGID_PKMNWASNTAFFECTED - 12]             = sText_PkmnWasntAffected,
    [STRINGID_PKMNWASPOISONED - 12]               = sText_PkmnWasPoisoned,
    [STRINGID_PKMNPOISONEDBY - 12]                = sText_PkmnPoisonedBy,
    [STRINGID_PKMNHURTBYPOISON - 12]              = sText_PkmnHurtByPoison,
    [STRINGID_PKMNALREADYPOISONED - 12]           = sText_PkmnAlreadyPoisoned,
    [STRINGID_PKMNBADLYPOISONED - 12]             = sText_PkmnBadlyPoisoned,
    [STRINGID_PKMNENERGYDRAINED - 12]             = sText_PkmnEnergyDrained,
    [STRINGID_PKMNWASBURNED - 12]                 = sText_PkmnWasBurned,
    [STRINGID_PKMNBURNEDBY - 12]                  = sText_PkmnBurnedBy,
    [STRINGID_PKMNHURTBYBURN - 12]                = sText_PkmnHurtByBurn,
    [STRINGID_PKMNWASFROZEN - 12]                 = sText_PkmnWasFrozen,
    [STRINGID_PKMNFROZENBY - 12]                  = sText_PkmnFrozenBy,
    [STRINGID_PKMNISFROZEN - 12]                  = sText_PkmnIsFrozen,
    [STRINGID_PKMNWASDEFROSTED - 12]              = sText_PkmnWasDefrosted,
    [STRINGID_PKMNWASDEFROSTED2 - 12]             = sText_PkmnWasDefrosted2,
    [STRINGID_PKMNWASDEFROSTEDBY - 12]            = sText_PkmnWasDefrostedBy,
    [STRINGID_PKMNWASPARALYZED - 12]              = sText_PkmnWasParalyzed,
    [STRINGID_PKMNWASPARALYZEDBY - 12]            = sText_PkmnWasParalyzedBy,
    [STRINGID_PKMNISPARALYZED - 12]               = sText_PkmnIsParalyzed,
    [STRINGID_PKMNISALREADYPARALYZED - 12]        = sText_PkmnIsAlreadyParalyzed,
    [STRINGID_PKMNHEALEDPARALYSIS - 12]           = sText_PkmnHealedParalysis,
    [STRINGID_PKMNDREAMEATEN - 12]                = sText_PkmnDreamEaten,
    [STRINGID_STATSWONTINCREASE - 12]             = sText_StatsWontIncrease,
    [STRINGID_STATSWONTDECREASE - 12]             = sText_StatsWontDecrease,
    [STRINGID_TEAMSTOPPEDWORKING - 12]            = sText_TeamStoppedWorking,
    [STRINGID_FOESTOPPEDWORKING - 12]             = sText_FoeStoppedWorking,
    [STRINGID_PKMNISCONFUSED - 12]                = sText_PkmnIsConfused,
    [STRINGID_PKMNHEALEDCONFUSION - 12]           = sText_PkmnHealedConfusion,
    [STRINGID_PKMNWASCONFUSED - 12]               = sText_PkmnWasConfused,
    [STRINGID_PKMNALREADYCONFUSED - 12]           = sText_PkmnAlreadyConfused,
    [STRINGID_PKMNFELLINLOVE - 12]                = sText_PkmnFellInLove,
    [STRINGID_PKMNINLOVE - 12]                    = sText_PkmnInLove,
    [STRINGID_PKMNIMMOBILIZEDBYLOVE - 12]         = sText_PkmnImmobilizedByLove,
    [STRINGID_PKMNBLOWNAWAY - 12]                 = sText_PkmnBlownAway,
    [STRINGID_PKMNCHANGEDTYPE - 12]               = sText_PkmnChangedType,
    [STRINGID_PKMNFLINCHED - 12]                  = sText_PkmnFlinched,
    [STRINGID_PKMNREGAINEDHEALTH - 12]            = sText_PkmnRegainedHealth,
    [STRINGID_PKMNHPFULL - 12]                    = sText_PkmnHPFull,
    [STRINGID_PKMNRAISEDSPDEF - 12]               = sText_PkmnRaisedSpDef,
    [STRINGID_PKMNRAISEDDEF - 12]                 = sText_PkmnRaisedDef,
    [STRINGID_PKMNCOVEREDBYVEIL - 12]             = sText_PkmnCoveredByVeil,
    [STRINGID_PKMNUSEDSAFEGUARD - 12]             = sText_PkmnUsedSafeguard,
    [STRINGID_PKMNSAFEGUARDEXPIRED - 12]          = sText_PkmnSafeguardExpired,
    [STRINGID_PKMNWENTTOSLEEP - 12]               = sText_PkmnWentToSleep,
    [STRINGID_PKMNSLEPTHEALTHY - 12]              = sText_PkmnSleptHealthy,
    [STRINGID_PKMNWHIPPEDWHIRLWIND - 12]          = sText_PkmnWhippedWhirlwind,
    [STRINGID_PKMNTOOKSUNLIGHT - 12]              = sText_PkmnTookSunlight,
    [STRINGID_PKMNLOWEREDHEAD - 12]               = sText_PkmnLoweredHead,
    [STRINGID_PKMNISGLOWING - 12]                 = sText_PkmnIsGlowing,
    [STRINGID_PKMNFLEWHIGH - 12]                  = sText_PkmnFlewHigh,
    [STRINGID_PKMNDUGHOLE - 12]                   = sText_PkmnDugHole,
    [STRINGID_PKMNSQUEEZEDBYBIND - 12]            = sText_PkmnSqueezedByBind,
    [STRINGID_PKMNTRAPPEDINVORTEX - 12]           = sText_PkmnTrappedInVortex,
    [STRINGID_PKMNWRAPPEDBY - 12]                 = sText_PkmnWrappedBy,
    [STRINGID_PKMNCLAMPED - 12]                   = sText_PkmnClamped,
    [STRINGID_PKMNHURTBY - 12]                    = sText_PkmnHurtBy,
    [STRINGID_PKMNFREEDFROM - 12]                 = sText_PkmnFreedFrom,
    [STRINGID_PKMNCRASHED - 12]                   = sText_PkmnCrashed,
    [STRINGID_PKMNSHROUDEDINMIST - 12]            = gBattleText_MistShroud,
    [STRINGID_PKMNPROTECTEDBYMIST - 12]           = sText_PkmnProtectedByMist,
    [STRINGID_PKMNGETTINGPUMPED - 12]             = gBattleText_GetPumped,
    [STRINGID_PKMNHITWITHRECOIL - 12]             = sText_PkmnHitWithRecoil,
    [STRINGID_PKMNPROTECTEDITSELF2 - 12]          = sText_PkmnProtectedItself2,
    [STRINGID_PKMNBUFFETEDBYSANDSTORM - 12]       = sText_PkmnBuffetedBySandstorm,
    [STRINGID_PKMNPELTEDBYHAIL - 12]              = sText_PkmnPeltedByHail,
    [STRINGID_PKMNSEEDED - 12]                    = sText_PkmnSeeded,
    [STRINGID_PKMNEVADEDATTACK - 12]              = sText_PkmnEvadedAttack,
    [STRINGID_PKMNSAPPEDBYLEECHSEED - 12]         = sText_PkmnSappedByLeechSeed,
    [STRINGID_PKMNFASTASLEEP - 12]                = sText_PkmnFastAsleep,
    [STRINGID_PKMNWOKEUP - 12]                    = sText_PkmnWokeUp,
    [STRINGID_PKMNUPROARKEPTAWAKE - 12]           = sText_PkmnUproarKeptAwake,
    [STRINGID_PKMNWOKEUPINUPROAR - 12]            = sText_PkmnWokeUpInUproar,
    [STRINGID_PKMNCAUSEDUPROAR - 12]              = sText_PkmnCausedUproar,
    [STRINGID_PKMNMAKINGUPROAR - 12]              = sText_PkmnMakingUproar,
    [STRINGID_PKMNCALMEDDOWN - 12]                = sText_PkmnCalmedDown,
    [STRINGID_PKMNCANTSLEEPINUPROAR - 12]         = sText_PkmnCantSleepInUproar,
    [STRINGID_PKMNSTOCKPILED - 12]                = sText_PkmnStockpiled,
    [STRINGID_PKMNCANTSTOCKPILE - 12]             = sText_PkmnCantStockpile,
    [STRINGID_PKMNCANTSLEEPINUPROAR2 - 12]        = sText_PkmnCantSleepInUproar2,
    [STRINGID_UPROARKEPTPKMNAWAKE - 12]           = sText_UproarKeptPkmnAwake,
    [STRINGID_PKMNSTAYEDAWAKEUSING - 12]          = sText_PkmnStayedAwakeUsing,
    [STRINGID_PKMNSTORINGENERGY - 12]             = sText_PkmnStoringEnergy,
    [STRINGID_PKMNUNLEASHEDENERGY - 12]           = sText_PkmnUnleashedEnergy,
    [STRINGID_PKMNFATIGUECONFUSION - 12]          = sText_PkmnFatigueConfusion,
    [STRINGID_PKMNPICKEDUPITEM - 12]              = sText_PkmnPickedUpItem,
    [STRINGID_PKMNUNAFFECTED - 12]                = sText_PkmnUnaffected,
    [STRINGID_PKMNTRANSFORMEDINTO - 12]           = sText_PkmnTransformedInto,
    [STRINGID_PKMNMADESUBSTITUTE - 12]            = sText_PkmnMadeSubstitute,
    [STRINGID_PKMNHASSUBSTITUTE - 12]             = sText_PkmnHasSubstitute,
    [STRINGID_SUBSTITUTEDAMAGED - 12]             = sText_SubstituteDamaged,
    [STRINGID_PKMNSUBSTITUTEFADED - 12]           = sText_PkmnSubstituteFaded,
    [STRINGID_PKMNMUSTRECHARGE - 12]              = sText_PkmnMustRecharge,
    [STRINGID_PKMNRAGEBUILDING - 12]              = sText_PkmnRageBuilding,
    [STRINGID_PKMNMOVEWASDISABLED - 12]           = sText_PkmnMoveWasDisabled,
    [STRINGID_PKMNMOVEISDISABLED - 12]            = sText_PkmnMoveIsDisabled,
    [STRINGID_PKMNMOVEDISABLEDNOMORE - 12]        = sText_PkmnMoveDisabledNoMore,
    [STRINGID_PKMNGOTENCORE - 12]                 = sText_PkmnGotEncore,
    [STRINGID_PKMNENCOREENDED - 12]               = sText_PkmnEncoreEnded,
    [STRINGID_PKMNTOOKAIM - 12]                   = sText_PkmnTookAim,
    [STRINGID_PKMNSKETCHEDMOVE - 12]              = sText_PkmnSketchedMove,
    [STRINGID_PKMNTRYINGTOTAKEFOE - 12]           = sText_PkmnTryingToTakeFoe,
    [STRINGID_PKMNTOOKFOE - 12]                   = sText_PkmnTookFoe,
    [STRINGID_PKMNREDUCEDPP - 12]                 = sText_PkmnReducedPP,
    [STRINGID_PKMNSTOLEITEM - 12]                 = sText_PkmnStoleItem,
    [STRINGID_TARGETCANTESCAPENOW - 12]           = sText_TargetCantEscapeNow,
    [STRINGID_PKMNFELLINTONIGHTMARE - 12]         = sText_PkmnFellIntoNightmare,
    [STRINGID_PKMNLOCKEDINNIGHTMARE - 12]         = sText_PkmnLockedInNightmare,
    [STRINGID_PKMNLAIDCURSE - 12]                 = sText_PkmnLaidCurse,
    [STRINGID_PKMNAFFLICTEDBYCURSE - 12]          = sText_PkmnAfflictedByCurse,
    [STRINGID_SPIKESSCATTERED - 12]               = sText_SpikesScattered,
    [STRINGID_PKMNHURTBYSPIKES - 12]              = sText_PkmnHurtBySpikes,
    [STRINGID_PKMNIDENTIFIED - 12]                = sText_PkmnIdentified,
    [STRINGID_PKMNPERISHCOUNTFELL - 12]           = sText_PkmnPerishCountFell,
    [STRINGID_PKMNBRACEDITSELF - 12]              = sText_PkmnBracedItself,
    [STRINGID_PKMNENDUREDHIT - 12]                = sText_PkmnEnduredHit,
    [STRINGID_MAGNITUDESTRENGTH - 12]             = sText_MagnitudeStrength,
    [STRINGID_PKMNCUTHPMAXEDATTACK - 12]          = sText_PkmnCutHPMaxedAttack,
    [STRINGID_PKMNCOPIEDSTATCHANGES - 12]         = sText_PkmnCopiedStatChanges,
    [STRINGID_PKMNGOTFREE - 12]                   = sText_PkmnGotFree,
    [STRINGID_PKMNSHEDLEECHSEED - 12]             = sText_PkmnShedLeechSeed,
    [STRINGID_PKMNBLEWAWAYSPIKES - 12]            = sText_PkmnBlewAwaySpikes,
    [STRINGID_PKMNFLEDFROMBATTLE - 12]            = sText_PkmnFledFromBattle,
    [STRINGID_PKMNFORESAWATTACK - 12]             = sText_PkmnForesawAttack,
    [STRINGID_PKMNTOOKATTACK - 12]                = sText_PkmnTookAttack,
    [STRINGID_PKMNATTACK - 12]                    = sText_PkmnAttack,
    [STRINGID_PKMNCENTERATTENTION - 12]           = sText_PkmnCenterAttention,
    [STRINGID_PKMNCHARGINGPOWER - 12]             = sText_PkmnChargingPower,
    [STRINGID_NATUREPOWERTURNEDINTO - 12]         = sText_NaturePowerTurnedInto,
    [STRINGID_PKMNSTATUSNORMAL - 12]              = sText_PkmnStatusNormal,
    [STRINGID_PKMNHASNOMOVESLEFT - 12]            = sText_PkmnHasNoMovesLeft,
    [STRINGID_PKMNSUBJECTEDTOTORMENT - 12]        = sText_PkmnSubjectedToTorment,
    [STRINGID_PKMNCANTUSEMOVETORMENT - 12]        = sText_PkmnCantUseMoveTorment,
    [STRINGID_PKMNTIGHTENINGFOCUS - 12]           = sText_PkmnTighteningFocus,
    [STRINGID_PKMNFELLFORTAUNT - 12]              = sText_PkmnFellForTaunt,
    [STRINGID_PKMNCANTUSEMOVETAUNT - 12]          = sText_PkmnCantUseMoveTaunt,
    [STRINGID_PKMNREADYTOHELP - 12]               = sText_PkmnReadyToHelp,
    [STRINGID_PKMNSWITCHEDITEMS - 12]             = sText_PkmnSwitchedItems,
    [STRINGID_PKMNCOPIEDFOE - 12]                 = sText_PkmnCopiedFoe,
    [STRINGID_PKMNMADEWISH - 12]                  = sText_PkmnMadeWish,
    [STRINGID_PKMNWISHCAMETRUE - 12]              = sText_PkmnWishCameTrue,
    [STRINGID_PKMNPLANTEDROOTS - 12]              = sText_PkmnPlantedRoots,
    [STRINGID_PKMNABSORBEDNUTRIENTS - 12]         = sText_PkmnAbsorbedNutrients,
    [STRINGID_PKMNANCHOREDITSELF - 12]            = sText_PkmnAnchoredItself,
    [STRINGID_PKMNWASMADEDROWSY - 12]             = sText_PkmnWasMadeDrowsy,
    [STRINGID_PKMNKNOCKEDOFF - 12]                = sText_PkmnKnockedOff,
    [STRINGID_PKMNSWAPPEDABILITIES - 12]          = sText_PkmnSwappedAbilities,
    [STRINGID_PKMNSEALEDOPPONENTMOVE - 12]        = sText_PkmnSealedOpponentMove,
    [STRINGID_PKMNCANTUSEMOVESEALED - 12]         = sText_PkmnCantUseMoveSealed,
    [STRINGID_PKMNWANTSGRUDGE - 12]               = sText_PkmnWantsGrudge,
    [STRINGID_PKMNLOSTPPGRUDGE - 12]              = sText_PkmnLostPPGrudge,
    [STRINGID_PKMNSHROUDEDITSELF - 12]            = sText_PkmnShroudedItself,
    [STRINGID_PKMNMOVEBOUNCED - 12]               = sText_PkmnMoveBounced,
    [STRINGID_PKMNWAITSFORTARGET - 12]            = sText_PkmnWaitsForTarget,
    [STRINGID_PKMNSNATCHEDMOVE - 12]              = sText_PkmnSnatchedMove,
    [STRINGID_PKMNMADEITRAIN - 12]                = sText_PkmnMadeItRain,
    [STRINGID_PKMNRAISEDSPEED - 12]               = sText_PkmnRaisedSpeed,
    [STRINGID_PKMNPROTECTEDBY - 12]               = sText_PkmnProtectedBy,
    [STRINGID_PKMNPREVENTSUSAGE - 12]             = sText_PkmnPreventsUsage,
    [STRINGID_PKMNRESTOREDHPUSING - 12]           = sText_PkmnRestoredHPUsing,
    [STRINGID_PKMNCHANGEDTYPEWITH - 12]           = sText_PkmnChangedTypeWith,
    [STRINGID_PKMNPREVENTSPARALYSISWITH - 12]     = sText_PkmnPreventsParalysisWith,
    [STRINGID_PKMNPREVENTSROMANCEWITH - 12]       = sText_PkmnPreventsRomanceWith,
    [STRINGID_PKMNPREVENTSPOISONINGWITH - 12]     = sText_PkmnPreventsPoisoningWith,
    [STRINGID_PKMNPREVENTSCONFUSIONWITH - 12]     = sText_PkmnPreventsConfusionWith,
    [STRINGID_PKMNRAISEDFIREPOWERWITH - 12]       = sText_PkmnRaisedFirePowerWith,
    [STRINGID_PKMNANCHORSITSELFWITH - 12]         = sText_PkmnAnchorsItselfWith,
    [STRINGID_PKMNCUTSATTACKWITH - 12]            = sText_PkmnCutsAttackWith,
    [STRINGID_PKMNPREVENTSSTATLOSSWITH - 12]      = sText_PkmnPreventsStatLossWith,
    [STRINGID_PKMNHURTSWITH - 12]                 = sText_PkmnHurtsWith,
    [STRINGID_PKMNTRACED - 12]                    = sText_PkmnTraced,
    [STRINGID_STATSHARPLY - 12]                   = sText_StatSharply,
    [STRINGID_STATROSE - 12]                      = gBattleText_Rose,
    [STRINGID_STATHARSHLY - 12]                   = sText_StatHarshly,
    [STRINGID_STATFELL - 12]                      = sText_StatFell,
    [STRINGID_PKMNSSTATCHANGED - 12]              = sText_PkmnsStatChanged,
    [STRINGID_PKMNSSTATCHANGED2 - 12]             = gText_PkmnsStatChanged2,
    [STRINGID_PKMNSSTATCHANGED3 - 12]             = sText_PkmnsStatChanged3,
    [STRINGID_PKMNSSTATCHANGED4 - 12]             = sText_PkmnsStatChanged4,
    [STRINGID_CRITICALHIT - 12]                   = sText_CriticalHit,
    [STRINGID_ONEHITKO - 12]                      = sText_OneHitKO,
    [STRINGID_123POOF - 12]                       = sText_123Poof,
    [STRINGID_ANDELLIPSIS - 12]                   = sText_AndEllipsis,
    [STRINGID_NOTVERYEFFECTIVE - 12]              = sText_NotVeryEffective,
    [STRINGID_SUPEREFFECTIVE - 12]                = sText_SuperEffective,
    [STRINGID_GOTAWAYSAFELY - 12]                 = sText_GotAwaySafely,
    [STRINGID_WILDPKMNFLED - 12]                  = sText_WildPkmnFled,
    [STRINGID_NORUNNINGFROMTRAINERS - 12]         = sText_NoRunningFromTrainers,
    [STRINGID_CANTESCAPE - 12]                    = sText_CantEscape,
    [STRINGID_DONTLEAVEBIRCH - 12]                = sText_EmptyString5,
    [STRINGID_BUTNOTHINGHAPPENED - 12]            = sText_ButNothingHappened,
    [STRINGID_BUTITFAILED - 12]                   = sText_ButItFailed,
    [STRINGID_ITHURTCONFUSION - 12]               = sText_ItHurtConfusion,
    [STRINGID_MIRRORMOVEFAILED - 12]              = sText_MirrorMoveFailed,
    [STRINGID_STARTEDTORAIN - 12]                 = sText_StartedToRain,
    [STRINGID_DOWNPOURSTARTED - 12]               = sText_DownpourStarted,
    [STRINGID_RAINCONTINUES - 12]                 = sText_RainContinues,
    [STRINGID_DOWNPOURCONTINUES - 12]             = sText_DownpourContinues,
    [STRINGID_RAINSTOPPED - 12]                   = sText_RainStopped,
    [STRINGID_SANDSTORMBREWED - 12]               = sText_SandstormBrewed,
    [STRINGID_SANDSTORMRAGES - 12]                = sText_SandstormRages,
    [STRINGID_SANDSTORMSUBSIDED - 12]             = sText_SandstormSubsided,
    [STRINGID_SUNLIGHTGOTBRIGHT - 12]             = sText_SunlightGotBright,
    [STRINGID_SUNLIGHTSTRONG - 12]                = sText_SunlightStrong,
    [STRINGID_SUNLIGHTFADED - 12]                 = sText_SunlightFaded,
    [STRINGID_STARTEDHAIL - 12]                   = sText_StartedHail,
    [STRINGID_HAILCONTINUES - 12]                 = sText_HailContinues,
    [STRINGID_HAILSTOPPED - 12]                   = sText_HailStopped,
    [STRINGID_FAILEDTOSPITUP - 12]                = sText_FailedToSpitUp,
    [STRINGID_FAILEDTOSWALLOW - 12]               = sText_FailedToSwallow,
    [STRINGID_WINDBECAMEHEATWAVE - 12]            = sText_WindBecameHeatWave,
    [STRINGID_STATCHANGESGONE - 12]               = sText_StatChangesGone,
    [STRINGID_COINSSCATTERED - 12]                = sText_CoinsScattered,
    [STRINGID_TOOWEAKFORSUBSTITUTE - 12]          = sText_TooWeakForSubstitute,
    [STRINGID_SHAREDPAIN - 12]                    = sText_SharedPain,
    [STRINGID_BELLCHIMED - 12]                    = sText_BellChimed,
    [STRINGID_FAINTINTHREE - 12]                  = sText_FaintInThree,
    [STRINGID_NOPPLEFT - 12]                      = sText_NoPPLeft,
    [STRINGID_BUTNOPPLEFT - 12]                   = sText_ButNoPPLeft,
    [STRINGID_PLAYERUSEDITEM - 12]                = sText_PlayerUsedItem,
    [STRINGID_OLDMANUSEDITEM - 12]                = sText_OldManUsedItem,
    [STRINGID_TRAINERBLOCKEDBALL - 12]            = sText_TrainerBlockedBall,
    [STRINGID_DONTBEATHIEF - 12]                  = sText_DontBeAThief,
    [STRINGID_ITDODGEDBALL - 12]                  = sText_ItDodgedBall,
    [STRINGID_YOUMISSEDPKMN - 12]                 = sText_YouMissedPkmn,
    [STRINGID_PKMNBROKEFREE - 12]                 = sText_PkmnBrokeFree,
    [STRINGID_ITAPPEAREDCAUGHT - 12]              = sText_ItAppearedCaught,
    [STRINGID_AARGHALMOSTHADIT - 12]              = sText_AarghAlmostHadIt,
    [STRINGID_SHOOTSOCLOSE - 12]                  = sText_ShootSoClose,
    [STRINGID_GOTCHAPKMNCAUGHT - 12]              = sText_GotchaPkmnCaught,
    [STRINGID_GOTCHAPKMNCAUGHT2 - 12]             = sText_GotchaPkmnCaught2,
    [STRINGID_GIVENICKNAMECAPTURED - 12]          = sText_GiveNicknameCaptured,
    [STRINGID_PKMNSENTTOPC - 12]                  = sText_PkmnSentToPC,
    [STRINGID_PKMNDATAADDEDTODEX - 12]            = sText_PkmnDataAddedToDex,
    [STRINGID_ITISRAINING - 12]                   = sText_ItIsRaining,
    [STRINGID_SANDSTORMISRAGING - 12]             = sText_SandstormIsRaging,
    [STRINGID_CANTESCAPE2 - 12]                   = sText_CantEscape2,
    [STRINGID_PKMNIGNORESASLEEP - 12]             = sText_PkmnIgnoresAsleep,
    [STRINGID_PKMNIGNOREDORDERS - 12]             = sText_PkmnIgnoredOrders,
    [STRINGID_PKMNBEGANTONAP - 12]                = sText_PkmnBeganToNap,
    [STRINGID_PKMNLOAFING - 12]                   = sText_PkmnLoafing,
    [STRINGID_PKMNWONTOBEY - 12]                  = sText_PkmnWontObey,
    [STRINGID_PKMNTURNEDAWAY - 12]                = sText_PkmnTurnedAway,
    [STRINGID_PKMNPRETENDNOTNOTICE - 12]          = sText_PkmnPretendNotNotice,
    [STRINGID_ENEMYABOUTTOSWITCHPKMN - 12]        = sText_EnemyAboutToSwitchPkmn,
    [STRINGID_THREWROCK - 12]                     = sText_ThrewARock,
    [STRINGID_THREWBAIT - 12]                     = sText_ThrewSomeBait,
    [STRINGID_PKMNWATCHINGCAREFULLY - 12]         = sText_PkmnWatchingCarefully,
    [STRINGID_PKMNANGRY - 12]                     = sText_PkmnIsAngry,
    [STRINGID_PKMNEATING - 12]                    = sText_PkmnIsEating,
    [STRINGID_DUMMY288 - 12]                      = sText_Empty1,
    [STRINGID_DUMMY289 - 12]                      = sText_Empty1,
    [STRINGID_OUTOFSAFARIBALLS - 12]              = sText_OutOfSafariBalls,
    [STRINGID_PKMNSITEMCUREDPARALYSIS - 12]       = sText_PkmnsItemCuredParalysis,
    [STRINGID_PKMNSITEMCUREDPOISON - 12]          = sText_PkmnsItemCuredPoison,
    [STRINGID_PKMNSITEMHEALEDBURN - 12]           = sText_PkmnsItemHealedBurn,
    [STRINGID_PKMNSITEMDEFROSTEDIT - 12]          = sText_PkmnsItemDefrostedIt,
    [STRINGID_PKMNSITEMWOKEIT - 12]               = sText_PkmnsItemWokeIt,
    [STRINGID_PKMNSITEMSNAPPEDOUT - 12]           = sText_PkmnsItemSnappedOut,
    [STRINGID_PKMNSITEMCUREDPROBLEM - 12]         = sText_PkmnsItemCuredProblem,
    [STRINGID_PKMNSITEMRESTOREDHEALTH - 12]       = sText_PkmnsItemRestoredHealth,
    [STRINGID_PKMNSITEMRESTOREDPP - 12]           = sText_PkmnsItemRestoredPP,
    [STRINGID_PKMNSITEMRESTOREDSTATUS - 12]       = sText_PkmnsItemRestoredStatus,
    [STRINGID_PKMNSITEMRESTOREDHPALITTLE - 12]    = sText_PkmnsItemRestoredHPALittle,
    [STRINGID_ITEMALLOWSONLYYMOVE - 12]           = sText_ItemAllowsOnlyYMove,
    [STRINGID_PKMNHUNGONWITHX - 12]               = sText_PkmnHungOnWithX,
    [STRINGID_EMPTYSTRING3 - 12]                  = gText_EmptyString3,
    [STRINGID_PKMNSXPREVENTSBURNS - 12]           = sText_PkmnsXPreventsBurns,
    [STRINGID_PKMNSXBLOCKSY - 12]                 = sText_PkmnsXBlocksY,
    [STRINGID_PKMNSXRESTOREDHPALITTLE2 - 12]      = sText_PkmnsXRestoredHPALittle2,
    [STRINGID_PKMNSXWHIPPEDUPSANDSTORM - 12]      = sText_PkmnsXWhippedUpSandstorm,
    [STRINGID_PKMNSXPREVENTSYLOSS - 12]           = sText_PkmnsXPreventsYLoss,
    [STRINGID_PKMNSXINFATUATEDY - 12]             = sText_PkmnsXInfatuatedY,
    [STRINGID_PKMNSXMADEYINEFFECTIVE - 12]        = sText_PkmnsXMadeYIneffective,
    [STRINGID_PKMNSXCUREDYPROBLEM - 12]           = sText_PkmnsXCuredYProblem,
    [STRINGID_ITSUCKEDLIQUIDOOZE - 12]            = sText_ItSuckedLiquidOoze,
    [STRINGID_PKMNTRANSFORMED - 12]               = sText_PkmnTransformed,
    [STRINGID_ELECTRICITYWEAKENED - 12]           = sText_ElectricityWeakened,
    [STRINGID_FIREWEAKENED - 12]                  = sText_FireWeakened,
    [STRINGID_PKMNHIDUNDERWATER - 12]             = sText_PkmnHidUnderwater,
    [STRINGID_PKMNSPRANGUP - 12]                  = sText_PkmnSprangUp,
    [STRINGID_HMMOVESCANTBEFORGOTTEN - 12]        = sText_HMMovesCantBeForgotten,
    [STRINGID_XFOUNDONEY - 12]                    = sText_XFoundOneY,
    [STRINGID_PLAYERDEFEATEDTRAINER1 - 12]        = sText_PlayerDefeatedLinkTrainerTrainer1,
    [STRINGID_SOOTHINGAROMA - 12]                 = sText_SoothingAroma,
    [STRINGID_ITEMSCANTBEUSEDNOW - 12]            = sText_ItemsCantBeUsedNow,
    [STRINGID_FORXCOMMAYZ - 12]                   = sText_ForXCommaYZ,
    [STRINGID_USINGXTHEYOFZN - 12]                = sText_UsingXTheYOfZN,
    [STRINGID_PKMNUSEDXTOGETPUMPED - 12]          = sText_PkmnUsedXToGetPumped,
    [STRINGID_PKMNSXMADEYUSELESS - 12]            = sText_PkmnsXMadeYUseless,
    [STRINGID_PKMNTRAPPEDBYSANDTOMB - 12]         = sText_PkmnTrappedBySandTomb,
    [STRINGID_EMPTYSTRING4 - 12]                  = sText_EmptyString4,
    [STRINGID_ABOOSTED - 12]                      = sText_ABoosted,
    [STRINGID_PKMNSXINTENSIFIEDSUN - 12]          = sText_PkmnsXIntensifiedSun,
    [STRINGID_PKMNMAKESGROUNDMISS - 12]           = sText_PkmnMakesGroundMiss,
    [STRINGID_YOUTHROWABALLNOWRIGHT - 12]         = sText_YouThrowABallNowRight,
    [STRINGID_PKMNSXTOOKATTACK - 12]              = sText_PkmnsXTookAttack,
    [STRINGID_PKMNCHOSEXASDESTINY - 12]           = sText_PkmnChoseXAsDestiny,
    [STRINGID_PKMNLOSTFOCUS - 12]                 = sText_PkmnLostFocus,
    [STRINGID_USENEXTPKMN - 12]                   = sText_UseNextPkmn,
    [STRINGID_PKMNFLEDUSINGITS - 12]              = sText_PkmnFledUsingIts,
    [STRINGID_PKMNFLEDUSING - 12]                 = sText_PkmnFledUsing,
    [STRINGID_PKMNWASDRAGGEDOUT - 12]             = sText_PkmnWasDraggedOut,
    [STRINGID_PREVENTEDFROMWORKING - 12]          = sText_PreventedFromWorking,
    [STRINGID_PKMNSITEMNORMALIZEDSTATUS - 12]     = sText_PkmnsItemNormalizedStatus,
    [STRINGID_TRAINER1USEDITEM - 12]              = sText_Trainer1UsedItem,
    [STRINGID_BOXISFULL - 12]                     = sText_BoxIsFull,
    [STRINGID_PKMNAVOIDEDATTACK - 12]             = sText_PkmnAvoidedAttack,
    [STRINGID_PKMNSXMADEITINEFFECTIVE - 12]       = sText_PkmnsXMadeItIneffective,
    [STRINGID_PKMNSXPREVENTSFLINCHING - 12]       = sText_PkmnsXPreventsFlinching,
    [STRINGID_PKMNALREADYHASBURN - 12]            = sText_PkmnAlreadyHasBurn,
    [STRINGID_STATSWONTDECREASE2 - 12]            = sText_StatsWontDecrease2,
    [STRINGID_PKMNSXBLOCKSY2 - 12]                = sText_PkmnsXBlocksY2,
    [STRINGID_PKMNSXWOREOFF - 12]                 = sText_PkmnsXWoreOff,
    [STRINGID_PKMNRAISEDDEFALITTLE - 12]          = sText_PkmnRaisedDefALittle,
    [STRINGID_PKMNRAISEDSPDEFALITTLE - 12]        = sText_PkmnRaisedSpDefALittle,
    [STRINGID_THEWALLSHATTERED - 12]              = sText_TheWallShattered,
    [STRINGID_PKMNSXPREVENTSYSZ - 12]             = sText_PkmnsXPreventsYsZ,
    [STRINGID_PKMNSXCUREDITSYPROBLEM - 12]        = sText_PkmnsXCuredItsYProblem,
    [STRINGID_ATTACKERCANTESCAPE - 12]            = sText_AttackerCantEscape,
    [STRINGID_PKMNOBTAINEDX - 12]                 = sText_PkmnObtainedX,
    [STRINGID_PKMNOBTAINEDX2 - 12]                = sText_PkmnObtainedX2,
    [STRINGID_PKMNOBTAINEDXYOBTAINEDZ - 12]       = sText_PkmnObtainedXYObtainedZ,
    [STRINGID_BUTNOEFFECT - 12]                   = sText_ButNoEffect,
    [STRINGID_PKMNSXHADNOEFFECTONY - 12]          = sText_PkmnsXHadNoEffectOnY,
    [STRINGID_OAKPLAYERWON - 12]                  = gText_WinEarnsPrizeMoney,
    [STRINGID_OAKPLAYERLOST - 12]                 = gText_HowDissapointing,
    [STRINGID_PLAYERLOSTAGAINSTENEMYTRAINER - 12] = sText_PlayerWhiteoutAgainstTrainer,
    [STRINGID_PLAYERPAIDPRIZEMONEY - 12]          = sText_PlayerPaidAsPrizeMoney,
    [STRINGID_PKMNTRANSFERREDSOMEONESPC - 12]     = Text_MonSentToBoxInSomeonesPC,
    [STRINGID_PKMNTRANSFERREDBILLSPC - 12]        = Text_MonSentToBoxInBillsPC,
    [STRINGID_PKMNBOXSOMEONESPCFULL - 12]         = Text_MonSentToBoxSomeonesBoxFull,
    [STRINGID_PKMNBOXBILLSPCFULL - 12]            = Text_MonSentToBoxBillsBoxFull,
    [STRINGID_POKEDUDEUSED - 12]                  = sText_PokedudeUsedItem,
    [STRINGID_POKEFLUTECATCHY - 12]               = sText_PlayedFluteCatchyTune,
    [STRINGID_POKEFLUTE - 12]                     = sText_PlayedThe,
    [STRINGID_MONHEARINGFLUTEAWOKE - 12]          = sText_PkmnHearingFluteAwoke,
    [STRINGID_TRAINER2CLASS - 12]                 = sText_Trainer2Class,
    [STRINGID_TRAINER2NAME - 12]                  = sText_Trainer2Name,
    [STRINGID_PLAYERWHITEDOUT - 12]               = sText_PlayerWhiteout2,
    [STRINGID_MONTOOSCAREDTOMOVE - 12]            = sText_TooScaredToMove,
    [STRINGID_GHOSTGETOUTGETOUT - 12]             = sText_GetOutGetOut,
    [STRINGID_SILPHSCOPEUNVEILED - 12]            = sText_SilphScopeUnveil,
    [STRINGID_GHOSTWASMAROWAK - 12]               = sText_TheGhostWas,
    [STRINGID_TRAINER1MON1COMEBACK - 12]          = sText_Trainer1RecallPkmn1,
    [STRINGID_TRAINER1WINTEXT - 12]               = sText_Trainer1WinText,
    [STRINGID_TRAINER1MON2COMEBACK - 12]          = sText_Trainer1RecallPkmn2,
    [STRINGID_TRAINER1MON1AND2COMEBACK - 12]      = sText_Trainer1RecallBoth
};

const u16 gMissStringIds[] = {
	STRINGID_PKMNAVOIDEDATTACK,
	STRINGID_PKMNPROTECTEDITSELF,
	STRINGID_PKMNAVOIDEDATTACK,
	STRINGID_AVOIDEDDAMAGE,
	STRINGID_PKMNMAKESGROUNDMISS,
	STRINGID_CUSTOMSTRING, //Crafty Shield
	STRINGID_CUSTOMSTRING, //Mat Block
	STRINGID_CUSTOMSTRING, //Quick Guard
	STRINGID_CUSTOMSTRING, //Wide Guard
	STRINGID_CUSTOMSTRING, //Raid Shield
};

const u16 gNoEscapeStringIds[] = {
    STRINGID_CANTESCAPE,
    STRINGID_DONTLEAVEBIRCH,
    STRINGID_PREVENTSESCAPE,
    STRINGID_CANTESCAPE2,
    STRINGID_ATTACKERCANTESCAPE
};

const u16 gMoveWeatherChangeStringIds[] = {
    STRINGID_STARTEDTORAIN,
    STRINGID_DOWNPOURSTARTED,
    STRINGID_BUTITFAILED,
    STRINGID_SANDSTORMBREWED,
    STRINGID_SUNLIGHTGOTBRIGHT,
    STRINGID_STARTEDHAIL
};

const u16 gSandstormHailContinuesStringIds[] = {
    STRINGID_SANDSTORMRAGES,
    STRINGID_HAILCONTINUES
};

const u16 gSandstormHailDmgStringIds[] = {
    STRINGID_PKMNBUFFETEDBYSANDSTORM,
    STRINGID_PKMNPELTEDBYHAIL
};

const u16 gSandstormHailEndStringIds[] = {
    STRINGID_SANDSTORMSUBSIDED,
    STRINGID_HAILSTOPPED
};

const u16 gRainContinuesStringIds[] = {
    STRINGID_RAINCONTINUES,
    STRINGID_DOWNPOURCONTINUES,
    STRINGID_RAINSTOPPED
};

const u16 gProtectLikeUsedStringIds[] = {
    STRINGID_PKMNPROTECTEDITSELF2,
    STRINGID_PKMNBRACEDITSELF,
    STRINGID_BUTITFAILED
};


const u16 gLeechSeedStringIds[] = {
    STRINGID_PKMNSEEDED,
    STRINGID_PKMNEVADEDATTACK,
    STRINGID_ITDOESNTAFFECT,
    STRINGID_PKMNSAPPEDBYLEECHSEED,
    STRINGID_ITSUCKEDLIQUIDOOZE
};

const u16 gRestUsedStringIds[] = {
    STRINGID_PKMNWENTTOSLEEP,
    STRINGID_PKMNSLEPTHEALTHY
};

const u16 gUproarOverTurnStringIds[] = {
    STRINGID_PKMNMAKINGUPROAR,
    STRINGID_PKMNCALMEDDOWN
};

const u16 gStockpileUsedStringIds[] = {
    STRINGID_PKMNSTOCKPILED,
    STRINGID_PKMNCANTSTOCKPILE
};

const u16 gWokeUpStringIds[] = {
    STRINGID_PKMNWOKEUP,
    STRINGID_PKMNWOKEUPINUPROAR
};

const u16 gSwallowFailStringIds[] = {
    STRINGID_FAILEDTOSWALLOW,
    STRINGID_PKMNHPFULL
};

const u16 gUproarAwakeStringIds[] = {
    STRINGID_PKMNCANTSLEEPINUPROAR2,
    STRINGID_UPROARKEPTPKMNAWAKE,
    STRINGID_PKMNSTAYEDAWAKEUSING
};

const u16 gStatUpStringIds[] = {
    STRINGID_PKMNSSTATCHANGED,
    STRINGID_PKMNSSTATCHANGED2,
    STRINGID_STATSWONTINCREASE,
    STRINGID_EMPTYSTRING3,
    STRINGID_USINGXTHEYOFZN,
    STRINGID_PKMNUSEDXTOGETPUMPED
};

const u16 gStatDownStringIds[] = {
    STRINGID_PKMNSSTATCHANGED3,
    STRINGID_PKMNSSTATCHANGED4,
    STRINGID_STATSWONTDECREASE,
    STRINGID_EMPTYSTRING3
};

const u16 gFirstTurnOfTwoStringIds[] = {
    STRINGID_PKMNWHIPPEDWHIRLWIND,
    STRINGID_PKMNTOOKSUNLIGHT,
    STRINGID_PKMNLOWEREDHEAD,
    STRINGID_PKMNISGLOWING,
    STRINGID_PKMNFLEWHIGH,
    STRINGID_PKMNDUGHOLE,
    STRINGID_PKMNHIDUNDERWATER,
    STRINGID_PKMNSPRANGUP
};

const u16 gWrappedStringIds[] = {
    STRINGID_PKMNSQUEEZEDBYBIND,
    STRINGID_PKMNWRAPPEDBY,
    STRINGID_PKMNTRAPPEDINVORTEX,
    STRINGID_PKMNCLAMPED,
    STRINGID_PKMNTRAPPEDINVORTEX,
    STRINGID_PKMNTRAPPEDBYSANDTOMB
};

const u16 gMistUsedStringIds[] = {
    STRINGID_PKMNSHROUDEDINMIST,
    STRINGID_BUTITFAILED
};

const u16 gFocusEnergyUsedStringIds[] = {
    STRINGID_PKMNGETTINGPUMPED,
    STRINGID_BUTITFAILED
};

const u16 gTransformUsedStringIds[] = {
    STRINGID_PKMNTRANSFORMEDINTO,
    STRINGID_BUTITFAILED
};

const u16 gSubsituteUsedStringIds[] = {
    STRINGID_PKMNMADESUBSTITUTE,
    STRINGID_TOOWEAKFORSUBSTITUTE
};

const u16 gGotPoisonedStringIds[] = {
    STRINGID_PKMNWASPOISONED,
    STRINGID_PKMNPOISONEDBY
};

const u16 gGotParalyzedStringIds[] = {
    STRINGID_PKMNWASPARALYZED,
    STRINGID_PKMNWASPARALYZEDBY
};

const u16 gFellAsleepStringIds[] = {
    STRINGID_PKMNFELLASLEEP,
    STRINGID_PKMNMADESLEEP
};

const u16 gGotBurnedStringIds[] = {
    STRINGID_PKMNWASBURNED,
    STRINGID_PKMNBURNEDBY
};

const u16 gGotFrozenStringIds[] = {
    STRINGID_PKMNWASFROZEN,
    STRINGID_PKMNFROZENBY
};

const u16 gGotDefrostedStringIds[] = {
    STRINGID_PKMNWASDEFROSTED2,
    STRINGID_PKMNWASDEFROSTEDBY
};

const u16 gKOFailedStringIds[] = {
    STRINGID_ATTACKMISSED,
    STRINGID_PKMNUNAFFECTED
};

const u16 gAttractUsedStringIds[] = {
    STRINGID_PKMNFELLINLOVE,
    STRINGID_PKMNSXINFATUATEDY
};

const u16 gLeechSeedDrainStringIds[] = {
    STRINGID_PKMNENERGYDRAINED,
    STRINGID_ITSUCKEDLIQUIDOOZE
};

const u16 gSportsUsedStringIds[] = {
    STRINGID_ELECTRICITYWEAKENED,
    STRINGID_FIREWEAKENED
};

const u16 gPartyStatusHealStringIds[] = {
    STRINGID_BELLCHIMED,
    STRINGID_BELLCHIMED,
    STRINGID_BELLCHIMED,
    STRINGID_BELLCHIMED,
    STRINGID_SOOTHINGAROMA
};

const u16 gFutureMoveUsedStringIds[] = {
    STRINGID_PKMNFORESAWATTACK,
    STRINGID_PKMNCHOSEXASDESTINY
};

const u16 gBallEscapeStringIds[] = {
    STRINGID_PKMNBROKEFREE,
    STRINGID_ITAPPEAREDCAUGHT,
    STRINGID_AARGHALMOSTHADIT,
    STRINGID_SHOOTSOCLOSE
};

const u16 gWeatherContinuesStringIds[] = {
	STRINGID_ITISRAINING,		//No Weather
	STRINGID_ITISRAINING, 		//Sunnny Weather with Clouds in Water
	STRINGID_ITISRAINING,		//Regular Weather
	STRINGID_ITISRAINING,   	//Rainy Weather
	STRINGID_STARTEDHAIL, 		//Light Snow
	STRINGID_ITISRAINING,		//Thunderstorm
	STRINGID_CUSTOMSTRING, 		//Steady Mist
	STRINGID_STARTEDHAIL, 		//Steady Snowing
	STRINGID_SANDSTORMISRAGING,	//Sandstorm
	STRINGID_CUSTOMSTRING, 		//Mist from Top Right Corner
	STRINGID_CUSTOMSTRING, 		//Dense Bright Mist
	STRINGID_ITISRAINING,		//Cloudy
	STRINGID_SUNLIGHTSTRONG, 	//Drought
	STRINGID_DOWNPOURSTARTED, 	//Downpour
	STRINGID_CUSTOMSTRING, 		//Underwater Mist
	STRINGID_ITISRAINING		//???
};

const u16 gInobedientStringIds[] = {
    STRINGID_PKMNLOAFING,
    STRINGID_PKMNWONTOBEY,
    STRINGID_PKMNTURNEDAWAY,
    STRINGID_PKMNPRETENDNOTNOTICE
};

const u16 gSafariPokeblockResultStringIds[] = {
    STRINGID_PKMNWATCHINGCAREFULLY,
    STRINGID_PKMNANGRY,
    STRINGID_PKMNEATING
};

const u16 gTrainerItemCuredStatusStringIds[] = {
    STRINGID_PKMNSITEMSNAPPEDOUT,
    STRINGID_PKMNSITEMCUREDPARALYSIS,
    STRINGID_PKMNSITEMDEFROSTEDIT,
    STRINGID_PKMNSITEMHEALEDBURN,
    STRINGID_PKMNSITEMCUREDPOISON,
    STRINGID_PKMNSITEMWOKEIT
};

const u16 gBerryEffectStringIds[] = {
    STRINGID_PKMNSITEMCUREDPROBLEM,
    STRINGID_PKMNSITEMNORMALIZEDSTATUS
};

const u16 gBRNPreventionStringIds[] = {
    STRINGID_PKMNSXPREVENTSBURNS,
    STRINGID_PKMNSXPREVENTSYSZ,
    STRINGID_PKMNSXHADNOEFFECTONY
};

const u16 gPRLZPreventionStringIds[] = {
    STRINGID_PKMNPREVENTSPARALYSISWITH,
    STRINGID_PKMNSXPREVENTSYSZ,
    STRINGID_PKMNSXHADNOEFFECTONY
};

const u16 gPSNPreventionStringIds[] = {
    STRINGID_PKMNPREVENTSPOISONINGWITH,
    STRINGID_PKMNSXPREVENTSYSZ,
    STRINGID_PKMNSXHADNOEFFECTONY
};

const u16 gItemSwapStringIds[] = {
    STRINGID_PKMNOBTAINEDX,
    STRINGID_PKMNOBTAINEDX2,
    STRINGID_PKMNOBTAINEDXYOBTAINEDZ
};

const u16 gFlashFireStringIds[] =
{
	STRINGID_PKMNRAISEDFIREPOWERWITH, 
    STRINGID_ITDOESNTAFFECT
};

const u16 gCaughtMonStringIds[] = {
    STRINGID_PKMNTRANSFERREDSOMEONESPC,
    STRINGID_PKMNTRANSFERREDBILLSPC,
    STRINGID_PKMNBOXSOMEONESPCFULL,
    STRINGID_PKMNBOXBILLSPCFULL
};

const u16 gDoubleBattleRecallStrings[] = {
    STRINGID_TRAINER1MON1COMEBACK,
    STRINGID_TRAINER1MON1COMEBACK,
    STRINGID_TRAINER1MON2COMEBACK,
    STRINGID_TRAINER1MON1AND2COMEBACK
};

const u16 gTrappingMoves[] = {
    MOVE_BIND,
    MOVE_WRAP,
    MOVE_FIRE_SPIN,
    MOVE_CLAMP,
    MOVE_WHIRLPOOL,
    MOVE_SAND_TOMB,
    0xFFFF
};

const u8 gText_PkmnIsEvolving[] = _("哎呀？\n{STR_VAR_1}的情形！");
const u8 gText_CaptureRaidMon[] = _("{B_OPPONENT_MON1_NAME} is weak!\nThrow a Ball now!");
const u8 gText_CongratsPkmnEvolved[] = _("恭喜！{STR_VAR_1}\n进化成{STR_VAR_2}！{WAIT_SE}\p");
const u8 gText_PkmnStoppedEvolving[] = _("咦？{STR_VAR_1}\n的变化停止了！\p");
const u8 gText_EllipsisQuestionMark[] = _("……？\p");
const u8 gText_WhatWillPkmnDo[] = _("{B_ACTIVE_NAME_WITH_PREFIX}\n要做什么呢？");
const u8 gText_WhatWillPlayerThrow[] = _("{B_PLAYER_NAME}\n要丢出什么呢？");
const u8 gText_WhatWillOldManDo[] = _("老爷爷\n要做什么呢？");
const u8 gText_LinkStandby[] = _("{PAUSE 16}Link standby…");
const u8 gText_BattleMenu[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}战斗{CLEAR_TO 56}背包\n宝可梦{CLEAR_TO 56}逃跑");
const u8 gText_BattleMenu2[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}战斗{CLEAR_TO 56}背包\n宝可梦{CLEAR_TO 56}返回");
const u8 gUnknown_83FE747[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}BALL{CLEAR_TO 56}BAIT\nROCK{CLEAR_TO 56}RUN");
const u8 gText_BattleMenuRaidEnd[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Catch{CLEAR_TO 56}End");
const u8 gText_MoveInterfacePP[] = _("PP ");
const u8 gText_MoveInterfaceType[] = _("属性/");
const u8 gUnknown_83FE770[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}");
const u8 gUnknown_83FE779[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}どの わざを\nわすれさせたい?");
const u8 gText_BattleYesNoChoice[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}是\n否");
const u8 gText_BattleSwitchWhich[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Switch\nwhich?");
const u8 gUnknown_83FE7B6[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}");
const u8 gUnknown_83FE7BF[] = _("{RIGHT_ARROW_2}");
const u8 gUnknown_83FE7C2[] = _("{PLUS}");
const u8 gUnknown_83FE7C5[] = _("-");

const u8 gUnknown_83FE7C7[] = _("{SIZE 0}Max{SIZE 2} HP");
const u8 gUnknown_83FE7D4[] = _("ATTACK ");
const u8 gUnknown_83FE7DC[] = _("DEFENSE");
const u8 gUnknown_83FE7E4[] = _("SP. ATK");
const u8 gUnknown_83FE7EC[] = _("SP. DEF");

const u8 *const gUnknown_83FE7F4[] = {
    gUnknown_83FE7C7,
    gUnknown_83FE7E4,
    gUnknown_83FE7D4,
    gUnknown_83FE7EC,
    gUnknown_83FE7DC,
    sText_Speed
};

const u8 gText_SafariBalls[] = _("{HIGHLIGHT 2}SAFARI BALLS"); //
const u8 gText_HighlightRed_Left[] = _("{HIGHLIGHT 2}Left: ");
const u8 gText_HighlightRed[] = _("{HIGHLIGHT 2}");
const u8 gText_Sleep[] = _("sleep");
const u8 gText_Poison[] = _("poison");
const u8 gText_Burn[] = _("burn");
const u8 gText_Paralysis[] = _("paralysis");
const u8 gText_Ice[] = _("ice");
const u8 gText_Confusion[] = _("confusion");
const u8 gText_Love[] = _("love");
const u8 gUnknown_83FE859[] = _("  ");
const u8 gUnknown_83FE85C[] = _("\n");
const u8 gUnknown_83FE85E[] = _("\n");
const u8 gUnknown_83FE860[] = _(" is");
const u8 gUnknown_83FE864[] = _(" is");
const u8 gText_BadEgg[] = _("Bad EGG");
const u8 gUnknown_83FE870[] = _("ミツル");
const u8 gText_Win[] = _("{HIGHLIGHT 0}Win");
const u8 gText_Loss[] = _("{HIGHLIGHT 0}Loss");
const u8 gText_Draw[] = _("{HIGHLIGHT 0}Draw");
static const u8 sText_SpaceIs[] = _(" is");
static const u8 sText_ApostropheS[] = _("'s");
const u8 gUnknown_83FE892[] = _("a NORMAL move");
const u8 gUnknown_83FE8A0[] = _("a FIGHTING move");
const u8 gUnknown_83FE8B0[] = _("a FLYING move");
const u8 gUnknown_83FE8BE[] = _("a POISON move");
const u8 gUnknown_83FE8CC[] = _("a GROUND move");
const u8 gUnknown_83FE8DA[] = _("a ROCK move");
const u8 gUnknown_83FE8E6[] = _("a BUG move");
const u8 gUnknown_83FE8F1[] = _("a GHOST move");
const u8 gUnknown_83FE8FE[] = _("a STEEL move");
const u8 gUnknown_83FE90B[] = _("a ??? move");
const u8 gUnknown_83FE916[] = _("a FIRE move");
const u8 gUnknown_83FE922[] = _("a WATER move");
const u8 gUnknown_83FE92F[] = _("a GRASS move");
const u8 gUnknown_83FE93C[] = _("an ELECTRIC move");
const u8 gUnknown_83FE94D[] = _("a PSYCHIC move");
const u8 gUnknown_83FE95C[] = _("an ICE move");
const u8 gUnknown_83FE968[] = _("a DRAGON move");
const u8 gUnknown_83FE976[] = _("a DARK move");
const u8 gText_TimeBoard[] = _("TIME BOARD");
const u8 gText_ClearTime[] = _("CLEAR TIME"); // Unused
const u8 gText_XMinYZSec[] = _("{STR_VAR_1}MIN. {STR_VAR_2}.{STR_VAR_3}SEC.");
const u8 gUnknown_83FE9A9[] = _("1F");
const u8 gUnknown_83FE9AC[] = _("2F");
const u8 gUnknown_83FE9AF[] = _("3F");
const u8 gUnknown_83FE9B2[] = _("4F");
const u8 gUnknown_83FE9B5[] = _("5F");
const u8 gUnknown_83FE9B8[] = _("6F");
const u8 gUnknown_83FE9BB[] = _("7F");
const u8 gUnknown_83FE9BE[] = _("8F");

const u8 *const gTrainerTowerChallengeTypeTexts[NUM_TOWER_CHALLENGE_TYPES] = {
    gOtherText_Single,
    gOtherText_Double,
    gOtherText_Knockout,
    gOtherText_Mixed
};

const u8 gUnknown_83FE9D4[] = _("{PLAY_SE SE_FLEE}{B_TRAINER1_CLASS} {B_TRAINER1_NAME} fled!"); //
static const u8 sText_PlayerLostAgainstTrainer1[] = _("Player lost against\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
static const u8 sText_PlayerBattledToDrawTrainer1[] = _("Player battled to a draw against\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");

static const u8 *const sATypeMove_Table[] = {
    gUnknown_83FE892,
    gUnknown_83FE8A0,
    gUnknown_83FE8B0,
    gUnknown_83FE8BE,
    gUnknown_83FE8CC,
    gUnknown_83FE8DA,
    gUnknown_83FE8E6,
    gUnknown_83FE8F1,
    gUnknown_83FE8FE,
    gUnknown_83FE90B,
    gUnknown_83FE916,
    gUnknown_83FE922,
    gUnknown_83FE92F,
    gUnknown_83FE93C,
    gUnknown_83FE94D,
    gUnknown_83FE95C,
    gUnknown_83FE968,
    gUnknown_83FE976
};

static const u16 sGrammarMoveUsedTable[] = {
    MOVE_SWORDS_DANCE,
    MOVE_STRENGTH,
    MOVE_GROWTH,
    MOVE_HARDEN,
    MOVE_MINIMIZE,
    MOVE_SMOKESCREEN,
    MOVE_WITHDRAW,
    MOVE_DEFENSE_CURL,
    MOVE_EGG_BOMB,
    MOVE_SMOG,
    MOVE_BONE_CLUB,
    MOVE_FLASH,
    MOVE_SPLASH,
    MOVE_ACID_ARMOR,
    MOVE_BONEMERANG,
    MOVE_REST,
    MOVE_SHARPEN,
    MOVE_SUBSTITUTE,
    MOVE_MIND_READER,
    MOVE_SNORE,
    MOVE_PROTECT,
    MOVE_SPIKES,
    MOVE_ENDURE,
    MOVE_ROLLOUT,
    MOVE_SWAGGER,
    MOVE_SLEEP_TALK,
    MOVE_HIDDEN_POWER,
    MOVE_PSYCH_UP,
    MOVE_EXTREME_SPEED,
    MOVE_FOLLOW_ME,
    MOVE_TRICK,
    MOVE_ASSIST,
    MOVE_INGRAIN,
    MOVE_KNOCK_OFF,
    MOVE_CAMOUFLAGE,
    MOVE_ASTONISH,
    MOVE_ODOR_SLEUTH,
    MOVE_GRASS_WHISTLE,
    MOVE_SHEER_COLD,
    MOVE_MUDDY_WATER,
    MOVE_IRON_DEFENSE,
    MOVE_BOUNCE,
    MOVE_NONE,

    MOVE_TELEPORT,
    MOVE_RECOVER,
    MOVE_BIDE,
    MOVE_AMNESIA,
    MOVE_FLAIL,
    MOVE_TAUNT,
    MOVE_BULK_UP,
    MOVE_NONE,

    MOVE_MEDITATE,
    MOVE_AGILITY,
    MOVE_MIMIC,
    MOVE_DOUBLE_TEAM,
    MOVE_BARRAGE,
    MOVE_TRANSFORM,
    MOVE_STRUGGLE,
    MOVE_SCARY_FACE,
    MOVE_CHARGE,
    MOVE_WISH,
    MOVE_BRICK_BREAK,
    MOVE_YAWN,
    MOVE_FEATHER_DANCE,
    MOVE_TEETER_DANCE,
    MOVE_MUD_SPORT,
    MOVE_FAKE_TEARS,
    MOVE_WATER_SPORT,
    MOVE_CALM_MIND,
    MOVE_NONE,

    MOVE_POUND,
    MOVE_SCRATCH,
    MOVE_VICE_GRIP,
    MOVE_WING_ATTACK,
    MOVE_FLY,
    MOVE_BIND,
    MOVE_SLAM,
    MOVE_HORN_ATTACK,
    MOVE_WRAP,
    MOVE_THRASH,
    MOVE_TAIL_WHIP,
    MOVE_LEER,
    MOVE_BITE,
    MOVE_GROWL,
    MOVE_ROAR,
    MOVE_SING,
    MOVE_PECK,
    MOVE_ABSORB,
    MOVE_STRING_SHOT,
    MOVE_EARTHQUAKE,
    MOVE_FISSURE,
    MOVE_DIG,
    MOVE_TOXIC,
    MOVE_SCREECH,
    MOVE_METRONOME,
    MOVE_LICK,
    MOVE_CLAMP,
    MOVE_CONSTRICT,
    MOVE_POISON_GAS,
    MOVE_BUBBLE,
    MOVE_SLASH,
    MOVE_SPIDER_WEB,
    MOVE_NIGHTMARE,
    MOVE_CURSE,
    MOVE_FORESIGHT,
    MOVE_CHARM,
    MOVE_ATTRACT,
    MOVE_ROCK_SMASH,
    MOVE_UPROAR,
    MOVE_SPIT_UP,
    MOVE_SWALLOW,
    MOVE_TORMENT,
    MOVE_FLATTER,
    MOVE_ROLE_PLAY,
    MOVE_ENDEAVOR,
    MOVE_TICKLE,
    MOVE_COVET,
    MOVE_NONE
};

// code
void BufferStringBattle(u16 stringId)
{
    s32 i;
    const u8 *stringPtr = NULL;
    bool8 zMoveActive;

    sBattleMsgDataPtr = (struct BattleMsgData*)(&gBattleBufferA[gActiveBattler][4]);
    gLastUsedItem = sBattleMsgDataPtr->lastItem;
    gLastUsedAbility = sBattleMsgDataPtr->lastAbility;
    gBattleScripting.battler = sBattleMsgDataPtr->scrActive;

    *(&gBattleStruct->scriptPartyIdx) = sBattleMsgDataPtr->bakScriptPartyIdx;
    *(&gBattleStruct->hpScale) = sBattleMsgDataPtr->hpScale;
    gPotentialItemEffectBattler = sBattleMsgDataPtr->itemEffectBattler;
    *(&gBattleStruct->stringMoveType) = sBattleMsgDataPtr->moveType;

    gBattleStringLoader = sBattleMsgDataPtr->battleStringLoader;
	zMoveActive = sBattleMsgDataPtr->zMoveActive;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        sBattlerAbilities[i] = sBattleMsgDataPtr->abilities[i];
    }
    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; i++)
    {
        gBattleTextBuff1[i] = sBattleMsgDataPtr->textBuffs[0][i];
        gBattleTextBuff2[i] = sBattleMsgDataPtr->textBuffs[1][i];
        gBattleTextBuff3[i] = sBattleMsgDataPtr->textBuffs[2][i];
    }

    switch (stringId)
    {
    case STRINGID_INTROMSG: // first battle msg
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                {
                    stringPtr = sText_TwoLinkTrainersWantToBattle;
                }
                else
                {
                    if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                        stringPtr = sText_Trainer1WantsToBattle;
                    else
                        stringPtr = sText_LinkTrainerWantsToBattle;
                }
            }
            else
            {
                stringPtr = sText_Trainer1WantsToBattle;
            }
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_GHOST_UNVEILED)
                    stringPtr = sText_TheGhostAppeared;
                else
                    stringPtr = sText_GhostAppearedCantId;
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_LEGENDARY)
                stringPtr = sText_WildPkmnAppeared2;
            else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) // interesting, looks like they had something planned for wild double battles
                stringPtr = sText_TwoWildPkmnAppeared;
            else if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
                stringPtr = sText_WildPkmnAppearedPause;
            else
                stringPtr = sText_WildPkmnAppeared;
        }
        break;
    case STRINGID_INTROSENDOUT: // poke first send-out
        if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_LinkPartnerSentOutPkmnGoPkmn;
                else
                    stringPtr = sText_GoTwoPkmn;
            }
            else
            {
                stringPtr = sText_GoPkmn;
            }
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_TwoLinkTrainersSentOutPkmn;
                else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                    stringPtr = sText_LinkTrainerSentOutTwoPkmn;
                else
                    stringPtr = sText_Trainer1SentOutTwoPkmn;
            }
            else
            {
                if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
                    stringPtr = sText_Trainer1SentOutPkmn;
                else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                    stringPtr = sText_Trainer1SentOutPkmn;
                else
                    stringPtr = sText_LinkTrainerSentOutPkmn;
            }
        }
        break;
    case STRINGID_RETURNMON: // sending poke to ball msg
        if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        {
            if (*(&gBattleStruct->hpScale) == 0)
                stringPtr = sText_PkmnThatsEnough;
            else if (*(&gBattleStruct->hpScale) == 1 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = sText_PkmnComeBack;
            else if (*(&gBattleStruct->hpScale) == 2)
                stringPtr = sText_PkmnOkComeBack;
            else
                stringPtr = sText_PkmnGoodComeBack;
        }
        else
        {
            if (gTrainerBattleOpponent_A == TRAINER_LINK_OPPONENT)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_LinkTrainer2WithdrewPkmn;
                else
                    stringPtr = sText_LinkTrainer1WithdrewPkmn;
            }
            else
            {
                stringPtr = sText_Trainer1WithdrewPkmn;
            }
        }
        break;
    case STRINGID_SWITCHINMON: // switch-in msg
        if (GetBattlerSide(gBattleScripting.battler) == B_SIDE_PLAYER)
        {
            if (*(&gBattleStruct->hpScale) == 0 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = sText_GoPkmn2;
            else if (*(&gBattleStruct->hpScale) == 1)
                stringPtr = sText_DoItPkmn;
            else if (*(&gBattleStruct->hpScale) == 2)
                stringPtr = sText_GoForItPkmn;
            else
                stringPtr = sText_YourFoesWeakGetEmPkmn;
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_LinkTrainerMultiSentOutPkmn;
                else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                    stringPtr = sText_Trainer1SentOutPkmn2;
                else
                    stringPtr = sText_LinkTrainerSentOutPkmn2;
            }
            else
            {
                stringPtr = sText_Trainer1SentOutPkmn2;
            }
        }
        break;
    case STRINGID_USEDMOVE: // pokemon used a move msg
        ChooseMoveUsedParticle(gBattleTextBuff1); // buff1 doesn't appear in the string, leftover from japanese move names

        if (sBattleMsgDataPtr->currentMove >= MOVES_COUNT)
            StringCopy(gBattleTextBuff2, sATypeMove_Table[*(&gBattleStruct->stringMoveType)]);
        else
            StringCopy(gBattleTextBuff2, gMoveNames[sBattleMsgDataPtr->currentMove]);

        ChooseTypeOfMoveUsedString(gBattleTextBuff2);
        stringPtr = sText_AttackerUsedX;
        break;
    case STRINGID_BATTLEEND: // battle end
        if (gBattleTextBuff1[0] & B_OUTCOME_LINK_BATTLE_RAN)
        {
            gBattleTextBuff1[0] &= ~(B_OUTCOME_LINK_BATTLE_RAN);
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTextBuff1[0] == B_OUTCOME_LOST || gBattleTextBuff1[0] == B_OUTCOME_DREW)
                stringPtr = sText_GotAwaySafely;
            else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                stringPtr = sText_TwoWildFled;
            else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                stringPtr = gUnknown_83FE9D4;
            else
                stringPtr = sText_WildFled;
        }
        else
        {
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_TwoLinkTrainersDefeated;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostToTwo;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawVsTwo;
                    break;
                }
            }
            else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainerTrainer1;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstTrainer1;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawTrainer1;
                    break;
                }
            }
            else
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainer;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstLinkTrainer;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawLinkTrainer;
                    break;
                }
            }
        }
        break;
    case STRINGID_CUSTOMSTRING: //need todo
		stringPtr = gBattleStringLoader;
		if (stringPtr == (u8*) 0)
			goto DEFAULT_STRING_LOAD;
		break;

    default: // load a string from the table
    DEFAULT_STRING_LOAD:
        if (stringId >= BATTLESTRINGS_COUNT + BATTLESTRINGS_ID_ADDER)
        {
            gDisplayedStringBattle[0] = EOS;
            return;
        }
        else
        {
            stringPtr = gBattleStringsTable[stringId - BATTLESTRINGS_ID_ADDER];
        }
        break;
    }

    BattleStringExpandPlaceholdersToDisplayedString(stringPtr);
}

u32 BattleStringExpandPlaceholdersToDisplayedString(const u8* src)
{
    BattleStringExpandPlaceholders(src, gDisplayedStringBattle);
}

static const u8* TryGetStatusString(u8 *src)
{
    u32 i;
    u8 status[] = _("$$$$$$$");
    u32 chars1, chars2;
    u8* statusPtr;

    statusPtr = status;
    for (i = 0; i < 8; i++)
    {
        if (*src == EOS)
            break;
        *statusPtr = *src;
        src++;
        statusPtr++;
    }

    chars1 = *(u32*)(&status[0]);
    chars2 = *(u32*)(&status[4]);

    for (i = 0; i < NELEMS(gStatusConditionStringsTable); i++)
    {
        if (chars1 == *(u32*)(&gStatusConditionStringsTable[i][0][0])
            && chars2 == *(u32*)(&gStatusConditionStringsTable[i][0][4]))
            return gStatusConditionStringsTable[i][1];
    }
    return NULL;
}

#define HANDLE_NICKNAME_STRING_CASE(battlerId, monIndex)                \
    if (GetBattlerSide(battlerId) != B_SIDE_PLAYER)                     \
    {                                                                   \
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
            toCpy = sText_FoePkmnPrefix;                                \
        else                                                            \
            toCpy = sText_WildPkmnPrefix;                               \
        while (*toCpy != EOS)                                           \
        {                                                               \
            dst[dstId] = *toCpy;                                        \
            dstId++;                                                    \
            toCpy++;                                                    \
        }                                                               \
        GetMonData(&gEnemyParty[monIndex], MON_DATA_NICKNAME, text);    \
    }                                                                   \
    else                                                                \
    {                                                                   \
        GetMonData(&gPlayerParty[monIndex], MON_DATA_NICKNAME, text);   \
    }                                                                   \
    StringGetEnd10(text);                                               \
    toCpy = text;

u32 BattleStringExpandPlaceholders(const u8 *src, u8 *dst)
{
    u32 dstId = 0; // if they used dstId, why not use srcId as well?
    const u8 *toCpy = NULL;
    u8 text[30];
    u8 multiplayerId;
    s32 i;

    multiplayerId = GetMultiplayerId();

    while (*src != EOS)
    {
        if (*src == PLACEHOLDER_BEGIN)
        {
            src++;
            switch (*src)
            {
            case B_TXT_BUFF1:
                if (gBattleTextBuff1[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gStringVar1);
                    toCpy = gStringVar1;
                }
                else
                {
                    toCpy = TryGetStatusString(gBattleTextBuff1);
                    if (toCpy == NULL)
                        toCpy = gBattleTextBuff1;
                }
                break;
            case B_TXT_BUFF2:
                if (gBattleTextBuff2[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff2, gStringVar2);
                    toCpy = gStringVar2;
                }
                else
                    toCpy = gBattleTextBuff2;
                break;
            case B_TXT_BUFF3:
                if (gBattleTextBuff3[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff3, gStringVar3);
                    toCpy = gStringVar3;
                }
                else
                    toCpy = gBattleTextBuff3;
                break;
            case B_TXT_COPY_VAR_1:
                toCpy = gStringVar1;
                break;
            case B_TXT_COPY_VAR_2:
                toCpy = gStringVar2;
                break;
            case B_TXT_COPY_VAR_3:
                toCpy = gStringVar3;
                break;
            case B_TXT_PLAYER_MON1_NAME: // first player poke name
                GetMonData(GetIllusionPartyData(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)),
						   MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON1_NAME: // first enemy poke name
                GetMonData(GetIllusionPartyData(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)),
						   MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_PLAYER_MON2_NAME: // second player poke name
                GetMonData(GetIllusionPartyData(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)),
						   MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON2_NAME: // second enemy poke name
                GetMonData(GetIllusionPartyData(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)),
						   MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON1_NAME: // link first player poke name
                GetMonData(GetIllusionPartyData(gLinkPlayers[multiplayerId].id),
						   MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON1_NAME: // link first opponent poke name
                GetMonData(GetIllusionPartyData(gLinkPlayers[multiplayerId].id ^ 1),
						   MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON2_NAME: // link second player poke name
                GetMonData(GetIllusionPartyData(gLinkPlayers[multiplayerId].id ^ 2),
						   MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON2_NAME: // link second opponent poke name
                GetMonData(GetIllusionPartyData(gLinkPlayers[multiplayerId].id ^ 3),
						   MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX_MON1: // attacker name with prefix, only battlerId 0/1
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker,
                                            gBattlerPartyIndexes[GetBattlerAtPosition(GET_BATTLER_SIDE(gBattlerAttacker))])
                break;
            case B_TXT_ATK_PARTNER_NAME: // attacker partner name
                GetMonData(GetIllusionPartyData(gBattlerAttacker ^ BIT_FLANK), MON_DATA_NICKNAME, text);

                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX: // attacker name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker])
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX: // target name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerTarget, gBattlerPartyIndexes[gBattlerTarget])
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX: // effect battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gEffectBattler, gBattlerPartyIndexes[gEffectBattler])
                break;
            case B_TXT_ACTIVE_NAME_WITH_PREFIX: // active battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gActiveBattler, gBattlerPartyIndexes[gActiveBattler])
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX: // scripting active battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler, gBattlerPartyIndexes[gBattleScripting.battler])
                break;
            case B_TXT_CURRENT_MOVE: // current move name
                if (sBattleMsgDataPtr->currentMove >= MOVES_COUNT)
                    toCpy = (const u8 *)&sATypeMove_Table[gBattleStruct->stringMoveType];
                else
                    toCpy = gMoveNames[sBattleMsgDataPtr->currentMove];
                break;
            case B_TXT_LAST_MOVE: // originally used move name
                if (sBattleMsgDataPtr->originallyUsedMove >= MOVES_COUNT)
                    toCpy = (const u8 *)&sATypeMove_Table[gBattleStruct->stringMoveType];
                else
                {
                    BufferMoveNameBattle(sBattleMsgDataPtr->originallyUsedMove, text);
                    toCpy = text;
                }
                break;
            case B_TXT_LAST_ITEM: // last used item
                CopyItemName(gLastUsedItem, text);
                toCpy = text;
                break;
            case B_TXT_LAST_ABILITY: // last used ability
                toCpy = GetAbilityName(gLastUsedAbility);
                break;
            case B_TXT_ATK_ABILITY: // attacker ability
                toCpy = GetAbilityName(sBattlerAbilities[gBattlerAttacker]);
                break;
            case B_TXT_DEF_ABILITY: // target ability
                toCpy = GetAbilityName(sBattlerAbilities[gBattlerTarget]);
                break;
            case B_TXT_SCR_ACTIVE_ABILITY: // scripting active ability
                toCpy = GetAbilityName(sBattlerAbilities[gBattleScripting.battler]);
                break;
            case B_TXT_EFF_ABILITY: // effect battlerId ability
                toCpy = GetAbilityName(sBattlerAbilities[gEffectBattler]);
                break;
            case B_TXT_TRAINER1_CLASS: // trainer class name
                if (gTrainerBattleOpponent_A == SECRET_BASE_OPPONENT)
                    toCpy = gTrainerClassNames[GetSecretBaseTrainerNameIndex()];
                else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                    toCpy = gTrainerClassNames[GetUnionRoomTrainerClass()];
                else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
                    toCpy = gTrainerClassNames[GetBattleTowerTrainerClassNameId()];
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                    toCpy = gTrainerClassNames[GetTrainerTowerOpponentClass()];
                else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
                    toCpy = gTrainerClassNames[GetEreaderTrainerClassId()];
                else
                    toCpy = gTrainerClassNames[gTrainers[gTrainerBattleOpponent_A].trainerClass];
                
                if (toCpy[3] == 0x8) //Expanded Trainer Class Names
					toCpy = T1_READ_PTR(toCpy);
                break;
            case B_TXT_TRAINER1_NAME: // trainer1 name
                if (gTrainerBattleOpponent_A == SECRET_BASE_OPPONENT)
                {
                    for (i = 0; i < (s32)NELEMS(gBattleResources->secretBase->trainerName); i++)
                        text[i] = gBattleResources->secretBase->trainerName[i];
                    text[i] = EOS;
                    toCpy = text;
                }
                if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                {
                    toCpy = gLinkPlayers[multiplayerId ^ BIT_SIDE].name;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
                {
                    GetBattleTowerTrainerName(text);
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerOpponentName(text);
                    toCpy = text;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
                {
                    CopyEReaderTrainerName5(text);
                    toCpy = text;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER || IsFrontierTrainerId(gTrainerBattleOpponent_A))
				{
					CopyFrontierTrainerName(text, gTrainerBattleOpponent_A, 0);
					toCpy = text;
				}
                else
                {
                    if (gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_RIVAL_EARLY
                     || gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_RIVAL_LATE
                     || gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_CHAMPION)
                        toCpy = GetExpandedPlaceholder(PLACEHOLDER_ID_RIVAL);
                    else
                        toCpy = gTrainers[gTrainerBattleOpponent_A].trainerName;
                }
                break;
            case B_TXT_LINK_PLAYER_NAME: // link player name
                toCpy = gLinkPlayers[multiplayerId].name;
                break;
            case B_TXT_LINK_PARTNER_NAME: // link partner name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_PARTNER(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT1_NAME: // link opponent 1 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT2_NAME: // link opponent 2 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(
                    BATTLE_PARTNER(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id)))].name;
                break;
            case B_TXT_LINK_SCR_TRAINER_NAME: // link scripting active name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(gBattleScripting.battler)].name;
                break;
            case B_TXT_PLAYER_NAME: // player name
                toCpy = gSaveBlock2Ptr->playerName;
                break;
            case B_TXT_TRAINER1_LOSE_TEXT: // trainerA lose text
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerOpponentLoseText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerALoseText();
                }
                break;
            case B_TXT_TRAINER1_WIN_TEXT: // trainerA win text
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerOpponentWinText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerWonSpeech();
                }
                break;
            case B_TXT_TRAINER2_LOSE_TEXT:
                GetTrainerTowerOpponentLoseText(gStringVar4, 1);
                toCpy = gStringVar4;
                break;
            case B_TXT_TRAINER2_WIN_TEXT:
                GetTrainerTowerOpponentWinText(gStringVar4, 1);
                toCpy = gStringVar4;
                break;
            case B_TXT_26: // ?
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler, *(&gBattleStruct->scriptPartyIdx))
                break;
            case B_TXT_PC_CREATOR_NAME: // lanette pc
                if (FlagGet(FLAG_SYS_NOT_SOMEONES_PC))
                    toCpy = sText_Bills;
                else
                    toCpy = sText_Someones;
                break;
            case B_TXT_ATK_PREFIX2:
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_DEF_PREFIX2:
                if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_ATK_PREFIX1:
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_DEF_PREFIX1:
                if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_ATK_PREFIX3:
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            case B_TXT_DEF_PREFIX3:
                if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            case B_TXT_AFFECTS_ATTACKER_SIDE:
				if (SIDE(gBattlerAttacker) == B_SIDE_OPPONENT)
					toCpy = sText_TheOpposingNoCaps;
				else
					toCpy = sText_Your;
				break;
			case B_TXT_AFFECTS_TARGET_SIDE_CAPS:
				if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
					toCpy = sText_TheOpposingCapsNoSpace;
				else
					toCpy = sText_YourCaps;
				break;
			case B_TXT_AFFECTS_ATTACKER_SIDE_CAPS:
				if (SIDE(gBattlerAttacker) == B_SIDE_OPPONENT)
					toCpy = sText_TheOpposingCapsNoSpace;
				else
					toCpy = sText_YourCaps;
				break;
            case B_TXT_AFFECTS_TARGET_SIDE:
				if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
					toCpy = sText_TheOpposingNoCaps;
				else
					toCpy = sText_Your;
				break;
            }

            // missing if (toCpy != NULL) check
            while (*toCpy != EOS)
            {
                dst[dstId] = *toCpy;
                dstId++;
                toCpy++;
            }
            if (*src == B_TXT_TRAINER1_LOSE_TEXT || *src == B_TXT_TRAINER1_WIN_TEXT
             || *src == B_TXT_TRAINER2_LOSE_TEXT || *src == B_TXT_TRAINER2_WIN_TEXT)
            {
                dst[dstId] = EXT_CTRL_CODE_BEGIN;
                dstId++;
                dst[dstId] = 9;
                dstId++;
            }
        }
        else
        {
            dst[dstId] = *src;
            dstId++;
        }
        src++;
    }

    dst[dstId] = *src;
    dstId++;

    return dstId;
}

void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst)
{
    u32 srcId = 1;
    u32 value = 0;
    u8 text[12];
    u16 hword;

    *dst = EOS;
    while (src[srcId] != B_BUFF_EOS)
    {
        switch (src[srcId])
        {
        case B_BUFF_STRING: // battle string
            hword = T1_READ_16(&src[srcId + 1]);
            StringAppend(dst, gBattleStringsTable[hword - BATTLESTRINGS_ID_ADDER]);
            srcId += 3;
            break;
        case B_BUFF_NUMBER: // int to string
            switch (src[srcId + 1])
            {
            case 1:
                value = src[srcId + 3];
                break;
            case 2:
                value = T1_READ_16(&src[srcId + 3]);
                break;
            case 4:
                value = T1_READ_32(&src[srcId + 3]);
                break;
            }
            ConvertIntToDecimalStringN(dst, value, STR_CONV_MODE_LEFT_ALIGN, src[srcId + 2]);
            srcId += src[srcId + 1] + 3;
            break;
        case B_BUFF_MOVE: // move name
            StringAppend(dst, gMoveNames[T1_READ_16(&src[srcId + 1])]);
            srcId += 3;
            break;
        case B_BUFF_TYPE: // type name
            StringAppend(dst, gTypeNames[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_MON_NICK_WITH_PREFIX: // poke nick with prefix
            if (GetBattlerSide(src[srcId + 1]) == B_SIDE_PLAYER)
            {
                GetMonData(&gPlayerParty[src[srcId + 2]], MON_DATA_NICKNAME, text);
            }
            else
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                    StringAppend(dst, sText_FoePkmnPrefix);
                else
                    StringAppend(dst, sText_WildPkmnPrefix);

                GetMonData(&gEnemyParty[src[srcId + 2]], MON_DATA_NICKNAME, text);
            }
            StringGetEnd10(text);
            StringAppend(dst, text);
            srcId += 3;
            break;
        case B_BUFF_STAT: // stats
            StringAppend(dst, gStatNamesTable[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_SPECIES: // species name
            GetSpeciesName(dst, T1_READ_16(&src[srcId + 1]));
            srcId += 3;
            break;
        case B_BUFF_MON_NICK: // poke nick without prefix
            if (GetBattlerSide(src[srcId + 1]) == B_SIDE_PLAYER)
                GetMonData(&gPlayerParty[src[srcId + 2]], MON_DATA_NICKNAME, dst);
            else
                GetMonData(&gEnemyParty[src[srcId + 2]], MON_DATA_NICKNAME, dst);
            StringGetEnd10(dst);
            srcId += 3;
            break;
        case B_BUFF_NEGATIVE_FLAVOR: // flavor table
            StringAppend(dst, gPokeblockWasTooXStringTable[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_ABILITY: // ability names
            StringAppend(dst, gAbilityNames[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_ITEM: // item name
            hword = T1_READ_16(&src[srcId + 1]);
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (hword == ITEM_ENIGMA_BERRY)
                {
                    if (gLinkPlayers[gBattleStruct->multiplayerId].id == gPotentialItemEffectBattler)
                    {
                        StringCopy(dst, gEnigmaBerries[gPotentialItemEffectBattler].name);
                        StringAppend(dst, sText_BerrySuffix);
                    }
                    else
                    {
                        StringAppend(dst, sText_EnigmaBerry);
                    }
                }
                else
                {
                    CopyItemName(hword, dst);
                }
            }
            else
            {
                CopyItemName(hword, dst);
            }
            srcId += 3;
            break;
        }
    }
}

// Loads one of two text strings into the provided buffer. This is functionally
// unused, since the value loaded into the buffer is not read; it loaded one of
// two particles (either "は" or "の") which works in tandem with ChooseTypeOfMoveUsedString
// below to effect changes in the meaning of the line.
static void ChooseMoveUsedParticle(u8* textBuff)
{
    s32 counter = 0;
    u32 i = 0;

    while (counter != MAX_MON_MOVES)
    {
        if (sGrammarMoveUsedTable[i] == 0)
            counter++;
        if (sGrammarMoveUsedTable[i++] == sBattleMsgDataPtr->currentMove)
            break;
    }

    if (counter >= 0)
    {
        if (counter <= 2)
            StringCopy(textBuff, sText_SpaceIs); // is
        else if (counter <= MAX_MON_MOVES)
            StringCopy(textBuff, sText_ApostropheS); // 's
    }
}

// Appends "!" to the text buffer `dst`. In the original Japanese this looked
// into the table of moves at sGrammarMoveUsedTable and varied the line accordingly.
//
// sText_ExclamationMark was a plain "!", used for any attack not on the list.
// It resulted in the translation "<NAME>'s <ATTACK>!".
//
// sText_ExclamationMark2 was "を つかった！". This resulted in the translation
// "<NAME> used <ATTACK>!", which was used for all attacks in English.
//
// sText_ExclamationMark3 was "した！". This was used for those moves whose
// names were verbs, such as Recover, and resulted in translations like "<NAME>
// recovered itself!".
//
// sText_ExclamationMark4 was "を した！" This resulted in a translation of
// "<NAME> did an <ATTACK>!".
//
// sText_ExclamationMark5 was " こうげき！" This resulted in a translation of
// "<NAME>'s <ATTACK> attack!".
static void ChooseTypeOfMoveUsedString(u8* dst)
{
    s32 counter = 0;
    s32 i = 0;

    while (*dst != EOS)
        dst++;

    while (counter != MAX_MON_MOVES)
    {
        if (sGrammarMoveUsedTable[i] == MOVE_NONE)
            counter++;
        if (sGrammarMoveUsedTable[i++] == sBattleMsgDataPtr->currentMove)
            break;
    }

    switch (counter)
    {
    case 0:
        StringCopy(dst, sText_ExclamationMark);
        break;
    case 1:
        StringCopy(dst, sText_ExclamationMark2);
        break;
    case 2:
        StringCopy(dst, sText_ExclamationMark3);
        break;
    case 3:
        StringCopy(dst, sText_ExclamationMark4);
        break;
    case 4:
        StringCopy(dst, sText_ExclamationMark5);
        break;
    }
}

static const struct BattleWindowText sTextOnWindowsInfo_Normal[] = {
    {PIXEL_FILL(0xf), 2, 0x02, 2, 0, 2, 1, 0x1, 0xf, 0x6},
    {PIXEL_FILL(0xf), 2, 0x02, 2, 0, 2, 0, 0x1, 0xf, 0x6},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 2, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 1, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 1, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 1, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 1, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xc, 0xe, 0xb},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 1, 0x0a, 2, 0, 2, 0, 0xc, 0xe, 0xb},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 2, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 1, 0x00, 2, 0, 2, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 0, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0x0), 0, 0x20, 0, 0, 0, 0, 0x1, 0x0, 0x2},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 1, 2, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0xe), 2, 0x00, 2, 0, 0, 0, 0xd, 0xe, 0xf},
    {PIXEL_FILL(0x0), 2, 0x00, 2, 0, 0, 0, 0x1, 0x0, 0x6},
    {PIXEL_FILL(0x0), 2, 0x00, 2, 0, 0, 0, 0x1, 0x0, 0x6},
    {PIXEL_FILL(0x0), 2, 0x00, 2, 0, 0, 0, 0x1, 0x0, 0x6},
    {PIXEL_FILL(0x1), 4, 0x00, 1, 0, 1, 1, 0x2, 0x1, 0x3}
};

const u8 gUnknown_83FEC90[] = {0x04, 0x05, 0x02, 0x02};

// windowId: Upper 2 bits are text flags
//   x40: Use NPC context-defined font
//   x80: Inhibit window clear
void BattlePutTextOnWindow(const u8 *text, u8 windowId) {
    bool32 copyToVram;
    struct TextPrinterTemplate printerTemplate;
    u8 speed;
    int x;
    u8 context;

    u8 textFlags = windowId & 0xC0;
    windowId &= 0x3F;
    if (!(textFlags & 0x80))
        FillWindowPixelBuffer(windowId, sTextOnWindowsInfo_Normal[windowId].fillValue);
    if (textFlags & 0x40) {
        context = ContextNpcGetTextColor();
        printerTemplate.fontId = gUnknown_83FEC90[context];
    }
    else {
        printerTemplate.fontId = sTextOnWindowsInfo_Normal[windowId].fontId;
    }
    switch (windowId)
    {
    case 15 ... 20:
        x = (48 - GetStringWidth(sTextOnWindowsInfo_Normal[windowId].fontId, text,
                                 sTextOnWindowsInfo_Normal[windowId].letterSpacing)) / 2;
        break;
    case 21 ... 23:
        x = (64 - GetStringWidth(sTextOnWindowsInfo_Normal[windowId].fontId, text,
                                 sTextOnWindowsInfo_Normal[windowId].letterSpacing)) / 2;
        break;
    default:
        x = sTextOnWindowsInfo_Normal[windowId].x;
        break;
    }
    if (x < 0)
        x = 0;
    printerTemplate.currentChar = text;
    printerTemplate.windowId = windowId;
    printerTemplate.x = x;
    printerTemplate.y = sTextOnWindowsInfo_Normal[windowId].y;
    printerTemplate.currentX = printerTemplate.x;
    printerTemplate.currentY = printerTemplate.y;
    printerTemplate.letterSpacing = sTextOnWindowsInfo_Normal[windowId].letterSpacing;
    printerTemplate.lineSpacing = sTextOnWindowsInfo_Normal[windowId].lineSpacing;
    printerTemplate.unk = 0;
    printerTemplate.fgColor = sTextOnWindowsInfo_Normal[windowId].fgColor;
    printerTemplate.bgColor = sTextOnWindowsInfo_Normal[windowId].bgColor;
    printerTemplate.shadowColor = sTextOnWindowsInfo_Normal[windowId].shadowColor;
    if (windowId == 24)
        gTextFlags.useAlternateDownArrow = FALSE;
    else
        gTextFlags.useAlternateDownArrow = TRUE;

    if ((gBattleTypeFlags & BATTLE_TYPE_LINK) || ((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) && windowId != 24))
        gTextFlags.autoScroll = TRUE;
    else
        gTextFlags.autoScroll = FALSE;

    if (windowId == 0 || windowId == 24)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            speed = 1;
        else
            speed = GetTextSpeedSetting();
        gTextFlags.canABSpeedUpPrint = TRUE;
    }
    else
    {
        speed = sTextOnWindowsInfo_Normal[windowId].speed;
        gTextFlags.canABSpeedUpPrint = FALSE;
    }

    AddTextPrinter(&printerTemplate, speed, NULL);
    if (!(textFlags & 0x80))
    {
        PutWindowTilemap(windowId);
        CopyWindowToVram(windowId, COPYWIN_BOTH);
    }
}

bool8 BattleStringShouldBeColored(u16 stringId)
{
    if (stringId == STRINGID_TRAINER1LOSETEXT || stringId == STRINGID_TRAINER2CLASS || stringId == STRINGID_TRAINER1WINTEXT || stringId == STRINGID_TRAINER2NAME)
        return TRUE;
    return FALSE;
}

void SetPpNumbersPaletteInMoveSelection(void)
{
    struct ChooseMoveStruct *chooseMoveStruct = (struct ChooseMoveStruct*)(&gBattleBufferA[gActiveBattler][4]);
    const u16 *palPtr = gUnknown_8D2FBB4;
    u8 var = GetCurrentPpToMaxPpState(chooseMoveStruct->currentPp[gMoveSelectionCursor[gActiveBattler]],
                                      chooseMoveStruct->maxPp[gMoveSelectionCursor[gActiveBattler]]);

    gPlttBufferUnfaded[92] = palPtr[(var * 2) + 0];
    gPlttBufferUnfaded[91] = palPtr[(var * 2) + 1];

    CpuCopy16(&gPlttBufferUnfaded[92], &gPlttBufferFaded[92], sizeof(u16));
    CpuCopy16(&gPlttBufferUnfaded[91], &gPlttBufferFaded[91], sizeof(u16));
}

u8 GetCurrentPpToMaxPpState(u8 currentPp, u8 maxPp)
{
    if (maxPp == currentPp)
    {
        return 3;
    }
    else if (maxPp <= 2)
    {
        if (currentPp > 1)
            return 3;
        else
            return 2 - currentPp;
    }
    else if (maxPp <= 7)
    {
        if (currentPp > 2)
            return 3;
        else
            return 2 - currentPp;
    }
    else
    {
        if (currentPp == 0)
            return 2;
        if (currentPp <= maxPp / 4)
            return 1;
        if (currentPp > maxPp / 2)
            return 3;
    }

    return 0;
}

const u8* GetAbilityName(const u8 ability)
{
	const u8* ptr = gAbilityNames[ability];

	if (ptr[3] == 0x8 || ptr[3] == 0x9) //Expanded Ability Names
		ptr = T1_READ_PTR(ptr);

	return ptr;
}

void BufferMoveNameBattle(u16 move, u8* dst)
{
	if (IsZMove(move))
	{
		//Load elongated move names for Z-Moves
		StringCopy(dst, GetZMoveName(move));
	}
	else if (IsGMaxMove(move))
	{
		dst[0] = CHAR_G,
		dst[1] = CHAR_HYPHEN,
		dst[2] = CHAR_M,
		dst[3] = CHAR_a,
		dst[4] = CHAR_x,
		dst[5] = CHAR_SPACE, //Space
		StringCopy(&dst[6], gMoveNames[move]);
	}
	else if (IsAnyMaxMove(move))
	{
		dst[0] = CHAR_M,
		dst[1] = CHAR_a,
		dst[2] = CHAR_x,
		dst[3] = CHAR_SPACE, //Space
		StringCopy(&dst[4], gMoveNames[move]);
	}
	else
		StringCopy(dst, gMoveNames[move]);
}