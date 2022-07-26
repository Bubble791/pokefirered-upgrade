#include "global.h"
#include "gflib.h"
#include "item.h"
#include "util.h"
#include "random.h"
#include "pokedex.h"
#include "money.h"
#include "pokemon_icon.h"
#include "mail.h"
#include "event_data.h"
#include "strings.h"
#include "pokemon_special_anim.h"
#include "pokemon_storage_system.h"
#include "pokemon_summary_screen.h"
#include "task.h"
#include "naming_screen.h"
#include "overworld.h"
#include "party_menu.h"
#include "trainer_pokemon_sprites.h"
#include "field_specials.h"
#include "battle.h"
#include "battle_message.h"
#include "battle_anim.h"
#include "battle_ai_script_commands.h"
#include "battle_scripts.h"
#include "battle_string_ids.h"
#include "battle_scripts.h"
#include "field_player_avatar.h"
#include "dynamax.h"
#include "script.h"
#include "dns.h"
#include "mega.h"
#include "constants/daycare.h"
#include "frontier.h"
#include "set_z_effect.h"
#include "reshow_battle_screen.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "constants/battle_anim.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_script_commands.h"
#include "constants/items.h"
#include "constants/hold_effects.h"
#include "constants/songs.h"
#include "constants/moves.h"
#include "constants/abilities.h"
#include "constants/pokemon.h"
#include "constants/maps.h"
#include "constants/species.h"
#include "constants/game_stat.h"
#include "constants/items.h"

#include "data/move_tables.h"

#define DEFENDER_IS_PROTECTED ((gProtectStructs[gBattlerTarget].protected) && (gBattleMoves[gCurrentMove].flags & FLAG_PROTECT_AFFECTED))

extern const u8 *const gBattleScriptsForMoveEffects[];

static void sub_8026480(void);
static bool8 sub_80264D0(void);
static void DrawLevelUpWindow1(void);
static void DrawLevelUpWindow2(void);
static bool8 sub_8026648(void);
static void PutMonIconOnLvlUpBox(void);
static void PutLevelAndGenderOnLvlUpBox(void);
static bool8 DoesBankNegateDamage(u8 bank, u16 move);
static u16 ChoosePickupItem(u8 level);

static void SpriteCB_MonIconOnLvlUpBox(struct Sprite *sprite);

static void atk01_accuracycheck(void);
static void atk02_attackstring(void);
static void atk03_ppreduce(void);
static void atk04_critcalc(void);
static void atk05_damagecalc(void);
static void atk06_typecalc(void);
static void atk07_adjustnormaldamage(void);
static void atk08_adjustnormaldamage2(void);
static void atk09_attackanimation(void);
static void atk0A_waitanimation(void);
static void atk0B_healthbarupdate(void);
static void atk0C_datahpupdate(void);
static void atk0D_critmessage(void);
static void atk0E_effectivenesssound(void);
static void atk0F_resultmessage(void);
static void atk10_printstring(void);
static void atk11_printselectionstring(void);
static void atk12_waitmessage(void);
static void atk13_printfromtable(void);
static void atk14_printselectionstringfromtable(void);
static void atk15_seteffectwithchance(void);
static void atk16_seteffectprimary(void);
static void atk17_seteffectsecondary(void);
static void atk18_clearstatusfromeffect(void);
static void atk19_tryfaintmon(void);
static void atk1A_dofaintanimation(void);
static void atk1B_cleareffectsonfaint(void);
static void atk1C_jumpifstatus(void);
static void atk1D_jumpifstatus2(void);
static void atk1E_jumpifability(void);
static void atk1F_jumpifsideaffecting(void);
static void atk20_jumpifstat(void);
static void atk21_jumpifstatus3condition(void);
static void atk22_jumpiftype(void);
static void atk23_getexp(void);
static void atk24_ifwildbattleend(void);
static void atk25_movevaluescleanup(void);
static void atk26_setmultihit(void);
static void atk27_decrementmultihit(void);
static void atk28_goto(void);
static void atk29_jumpifbyte(void);
static void atk2A_jumpifhalfword(void);
static void atk2B_jumpifword(void);
static void atk2C_jumpifarrayequal(void);
static void atk2D_jumpifarraynotequal(void);
static void atk2E_setbyte(void);
static void atk2F_addbyte(void);
static void atk30_subbyte(void);
static void atk31_copyarray(void);
static void atk32_copyarraywithindex(void);
static void atk33_orbyte(void);
static void atk34_orhalfword(void);
static void atk35_orword(void);
static void atk36_bicbyte(void);
static void atk37_bichalfword(void);
static void atk38_bicword(void);
static void atk39_pause(void);
static void atk3A_waitstate(void);
static void atk3B_healthbar_update(void);
static void atk3C_return(void);
static void atk3D_end(void);
static void atk3E_end2(void);
static void atk3F_end3(void);
static void atk40_jumpifaffectedbyprotect(void);
static void atk41_call(void);
static void atk42_jumpiftype2(void);
static void atk43_jumpifabilitypresent(void);
static void atk44_endselectionscript(void);
static void atk45_playanimation(void);
static void atk46_playanimation2(void);
static void atk47_setgraphicalstatchangevalues(void);
static void atk48_playstatchangeanimation(void);
static void atk49_moveend(void);
static void atk4A_typecalc2(void);
static void atk4B_returnatktoball(void);
static void atk4C_getswitchedmondata(void);
static void atk4D_switchindataupdate(void);
static void atk4E_switchinanim(void);
static void atk4F_jumpifcantswitch(void);
static void atk50_openpartyscreen(void);
static void atk51_switchhandleorder(void);
static void atk53_trainerslidein(void);
static void atk54_playse(void);
static void atk55_fanfare(void);
static void atk56_playfaintcry(void);
static void atk57(void);
static void atk58_returntoball(void);
static void atk59_handlelearnnewmove(void);
static void atk5A_yesnoboxlearnmove(void);
static void atk5B_yesnoboxstoplearningmove(void);
static void atk5C_hitanimation(void);
static void atk5D_getmoneyreward(void);
static void atk5E(void);
static void atk5F_swapattackerwithtarget(void);
static void atk60_incrementgamestat(void);
static void atk61_drawpartystatussummary(void);
static void atk62_hidepartystatussummary(void);
static void atk63_jumptocalledmove(void);
static void atk64_statusanimation(void);
static void atk65_status2animation(void);
static void atk66_chosenstatusanimation(void);
static void atk67_yesnobox(void);
static void atk68_cancelallactions(void);
static void atk69_adjustsetdamage(void);
static void atk6A_removeitem(void);
static void atk6B_atknameinbuff1(void);
static void atk6C_drawlvlupbox(void);
static void atk6D_resetsentmonsvalue(void);
static void atk6E_setatktoplayer0(void);
static void atk6F_makevisible(void);
static void atk70_recordlastability(void);
static void atk71_buffermovetolearn(void);
static void atk72_jumpifplayerran(void);
static void atk73_hpthresholds(void);
static void atk74_hpthresholds2(void);
static void atk75_useitemonopponent(void);
static void atk76_various(void);
static void atk77_setprotectlike(void);
static void atk78_faintifabilitynotdamp(void);
static void atk79_setatkhptozero(void);
static void atk7A_jumpifnexttargetvalid(void);
static void atk7B_tryhealhalfhealth(void);
static void atk7C_trymirrormove(void);
static void atk7D_setrain(void);
static void atk7E_setreflect(void);
static void atk7F_setseeded(void);
static void atk80_manipulatedamage(void);
static void atk81_trysetrest(void);
static void atk82_jumpifnotfirstturn(void);
static void atk83_nop(void);
static void atk84_jumpifcantmakeasleep(void);
static void atk85_stockpile(void);
static void atk86_stockpiletobasedamage(void);
static void atk87_stockpiletohpheal(void);
static void atk88_negativedamage(void);
static void atk89_statbuffchange(void);
static void atk8A_normalisebuffs(void);
static void atk8B_setbide(void);
static void atk8C_confuseifrepeatingattackends(void);
static void atk8D_setmultihitcounter(void);
static void atk8E_initmultihitstring(void);
static void atk90_tryconversiontypechange(void);
static void atk91_givepaydaymoney(void);
static void atk92_setlightscreen(void);
static void atk93_tryKO(void);
static void atk94_damagetohalftargethp(void);
static void atk95_setsandstorm(void);
static void atk96_weatherdamage(void);
static void atk97_tryinfatuating(void);
static void atk98_updatestatusicon(void);
static void atk99_setmist(void);
static void atk9A_setfocusenergy(void);
static void atk9B_transformdataexecution(void);
static void atk9C_setsubstitute(void);
static void atk9D_mimicattackcopy(void);
static void atk9E_metronome(void);
static void atk9F_dmgtolevel(void);
static void atkA0_psywavedamageeffect(void);
static void atkA1_counterdamagecalculator(void);
static void atkA2_mirrorcoatdamagecalculator(void);
static void atkA3_disablelastusedattack(void);
static void atkA4_trysetencore(void);
static void atkA5_painsplitdmgcalc(void);
static void atkA6_settypetorandomresistance(void);
static void atkA7_setalwayshitflag(void);
static void atkA8_copymovepermanently(void);
static void atkA9_trychoosesleeptalkmove(void);
static void atkAA_setdestinybond(void);
static void atkAB_trysetdestinybondtohappen(void);
static void atkAC_remaininghptopower(void);
static void atkAD_tryspiteppreduce(void);
static void atkAE_healpartystatus(void);
static void atkAF_cursetarget(void);
static void atkB0_trysetspikes(void);
static void atkB1_setforesight(void);
static void atkB2_trysetperishsong(void);
static void atkB3_rolloutdamagecalculation(void);
static void atkB4_jumpifconfusedandstatmaxed(void);
static void atkB5_furycuttercalc(void);
static void atkB6_happinesstodamagecalculation(void);
static void atkB7_presentdamagecalculation(void);
static void atkB8_setsafeguard(void);
static void atkB9_magnitudedamagecalculation(void);
static void atkBA_jumpifnopursuitswitchdmg(void);
static void atkBB_setsunny(void);
static void atkBC_maxattackhalvehp(void);
static void atkBD_copyfoestats(void);
static void atkBE_rapidspinfree(void);
static void atkBF_setdefensecurlbit(void);
static void atkC0_recoverbasedonsunlight(void);
static void atkC1_hiddenpowercalc(void);
static void atkC2_selectfirstvalidtarget(void);
static void atkC3_trysetfutureattack(void);
static void atkC4_trydobeatup(void);
static void atkC5_setsemiinvulnerablebit(void);
static void atkC6_clearsemiinvulnerablebit(void);
static void atkC7_setminimize(void);
static void atkC8_sethail(void);
static void atkC9_jumpifattackandspecialattackcannotfall(void);
static void atkCA_setforcedtarget(void);
static void atkCB_setcharge(void);
static void atkCC_callterrainattack(void);
static void atkCD_cureifburnedparalysedorpoisoned(void);
static void atkCE_settorment(void);
static void atkCF_jumpifnodamage(void);
static void atkD0_settaunt(void);
static void atkD1_trysethelpinghand(void);
static void atkD2_tryswapitems(void);
static void atkD3_trycopyability(void);
static void atkD4_trywish(void);
static void atkD5_trysetroots(void);
static void atkD6_doubledamagedealtifdamaged(void);
static void atkD7_setyawn(void);
static void atkD8_setdamagetohealthdifference(void);
static void atkD9_scaledamagebyhealthratio(void);
static void atkDA_tryswapabilities(void);
static void atkDB_tryimprison(void);
static void atkDC_trysetgrudge(void);
static void atkDD_weightdamagecalculation(void);
static void atkDE_assistattackselect(void);
static void atkDF_trysetmagiccoat(void);
static void atkE0_trysetsnatch(void);
static void atkE1_trygetintimidatetarget(void);
static void atkE2_switchoutabilities(void);
static void atkE3_jumpifhasnohp(void);
static void atkE4_getsecretpowereffect(void);
static void atkE5_pickup(void);
static void atkE6_docastformchangeanimation(void);
static void atkE7_trycastformdatachange(void);
static void atkE8_settypebasedhalvers(void);
static void atkE9_setweatherballtype(void);
static void atkEA_tryrecycleitem(void);
static void atkEB_settypetoterrain(void);
static void atkEC_pursuitrelated(void);
static void atkED_snatchsetbattlers(void);
static void atkEE_removelightscreenreflect(void);
static void atkEF_handleballthrow(void);
static void atkF0_givecaughtmon(void);
static void atkF1_trysetcaughtmondexflags(void);
static void atkF2_displaydexinfo(void);
static void atkF3_trygivecaughtmonnick(void);
static void atkF4_subattackerhpbydmg(void);
static void atkF5_removeattackerstatus1(void);
static void atkF6_finishaction(void);
static void atkF7_finishturn(void);
static void atkF8_callasm(void);
static void atkF9_sethalfword(void);
static void atkFA_setword(void);
static void atkFB_setspecialstatusbit(void);
static void atkFC_clearspecialstatusbit(void);
static void atkFD_jumpifabilitypresenttargetfield(void);
static void atkFE_prefaintmoveendeffects(void);
static void atkFF_callsecondarytable(void);

void (* const gBattleScriptingCommandsTable[])(void) =
{
    atk00_attackcanceler,
    atk01_accuracycheck,
    atk02_attackstring,
    atk03_ppreduce,
    atk04_critcalc,
    atk05_damagecalc,
    atk06_typecalc,
    atk07_adjustnormaldamage,
    atk08_adjustnormaldamage2,
    atk09_attackanimation,
    atk0A_waitanimation,
    atk0B_healthbarupdate,
    atk0C_datahpupdate,
    atk0D_critmessage,
    atk0E_effectivenesssound,
    atk0F_resultmessage,
    atk10_printstring,
    atk11_printselectionstring,
    atk12_waitmessage,
    atk13_printfromtable,
    atk14_printselectionstringfromtable,
    atk15_seteffectwithchance,
    atk16_seteffectprimary,
    atk17_seteffectsecondary,
    atk18_clearstatusfromeffect,
    atk19_tryfaintmon,
    atk1A_dofaintanimation,
    atk1B_cleareffectsonfaint,
    atk1C_jumpifstatus,
    atk1D_jumpifstatus2,
    atk1E_jumpifability,
    atk1F_jumpifsideaffecting,
    atk20_jumpifstat,
    atk21_jumpifstatus3condition,
    atk22_jumpiftype,
    atk23_getexp,
    atk24_ifwildbattleend,
    atk25_movevaluescleanup,
    atk26_setmultihit,
    atk27_decrementmultihit,
    atk28_goto,
    atk29_jumpifbyte,
    atk2A_jumpifhalfword,
    atk2B_jumpifword,
    atk2C_jumpifarrayequal,
    atk2D_jumpifarraynotequal,
    atk2E_setbyte,
    atk2F_addbyte,
    atk30_subbyte,
    atk31_copyarray,
    atk32_copyarraywithindex,
    atk33_orbyte,
    atk34_orhalfword,
    atk35_orword,
    atk36_bicbyte,
    atk37_bichalfword,
    atk38_bicword,
    atk39_pause,
    atk3A_waitstate,
    atk3B_healthbar_update,
    atk3C_return,
    atk3D_end,
    atk3E_end2,
    atk3F_end3,
    atk40_jumpifaffectedbyprotect,
    atk41_call,
    atk42_jumpiftype2,
    atk43_jumpifabilitypresent,
    atk44_endselectionscript,
    atk45_playanimation,
    atk46_playanimation2,
    atk47_setgraphicalstatchangevalues,
    atk48_playstatchangeanimation,
    atk49_moveend,
    atk4A_typecalc2,
    atk4B_returnatktoball,
    atk4C_getswitchedmondata,
    atk4D_switchindataupdate,
    atk4E_switchinanim,
    atk4F_jumpifcantswitch,
    atk50_openpartyscreen,
    atk51_switchhandleorder,
    atk52_switchineffects,
    atk53_trainerslidein,
    atk54_playse,
    atk55_fanfare,
    atk56_playfaintcry,
    atk57,
    atk58_returntoball,
    atk59_handlelearnnewmove,
    atk5A_yesnoboxlearnmove,
    atk5B_yesnoboxstoplearningmove,
    atk5C_hitanimation,
    atk5D_getmoneyreward,
    atk5E,
    atk5F_swapattackerwithtarget,
    atk60_incrementgamestat,
    atk61_drawpartystatussummary,
    atk62_hidepartystatussummary,
    atk63_jumptocalledmove,
    atk64_statusanimation,
    atk65_status2animation,
    atk66_chosenstatusanimation,
    atk67_yesnobox,
    atk68_cancelallactions,
    atk69_adjustsetdamage,
    atk6A_removeitem,
    atk6B_atknameinbuff1,
    atk6C_drawlvlupbox,
    atk6D_resetsentmonsvalue,
    atk6E_setatktoplayer0,
    atk6F_makevisible,
    atk70_recordlastability,
    atk71_buffermovetolearn,
    atk72_jumpifplayerran,
    atk73_hpthresholds,
    atk74_hpthresholds2,
    atk75_useitemonopponent,
    atk76_various,
    atk77_setprotectlike,
    atk78_faintifabilitynotdamp,
    atk79_setatkhptozero,
    atk7A_jumpifnexttargetvalid,
    atk7B_tryhealhalfhealth,
    atk7C_trymirrormove,
    atk7D_setrain,
    atk7E_setreflect,
    atk7F_setseeded,
    atk80_manipulatedamage,
    atk81_trysetrest,
    atk82_jumpifnotfirstturn,
    atk83_nop,
    atk84_jumpifcantmakeasleep,
    atk85_stockpile,
    atk86_stockpiletobasedamage,
    atk87_stockpiletohpheal,
    atk88_negativedamage,
    atk89_statbuffchange,
    atk8A_normalisebuffs,
    atk8B_setbide,
    atk8C_confuseifrepeatingattackends,
    atk8D_setmultihitcounter,
    atk8E_initmultihitstring,
    atk8F_forcerandomswitch,
    atk90_tryconversiontypechange,
    atk91_givepaydaymoney,
    atk92_setlightscreen,
    atk93_tryKO,
    atk94_damagetohalftargethp,
    atk95_setsandstorm,
    atk96_weatherdamage,
    atk97_tryinfatuating,
    atk98_updatestatusicon,
    atk99_setmist,
    atk9A_setfocusenergy,
    atk9B_transformdataexecution,
    atk9C_setsubstitute,
    atk9D_mimicattackcopy,
    atk9E_metronome,
    atk9F_dmgtolevel,
    atkA0_psywavedamageeffect,
    atkA1_counterdamagecalculator,
    atkA2_mirrorcoatdamagecalculator,
    atkA3_disablelastusedattack,
    atkA4_trysetencore,
    atkA5_painsplitdmgcalc,
    atkA6_settypetorandomresistance,
    atkA7_setalwayshitflag,
    atkA8_copymovepermanently,
    atkA9_trychoosesleeptalkmove,
    atkAA_setdestinybond,
    atkAB_trysetdestinybondtohappen,
    atkAC_remaininghptopower,
    atkAD_tryspiteppreduce,
    atkAE_healpartystatus,
    atkAF_cursetarget,
    atkB0_trysetspikes,
    atkB1_setforesight,
    atkB2_trysetperishsong,
    atkB3_rolloutdamagecalculation,
    atkB4_jumpifconfusedandstatmaxed,
    atkB5_furycuttercalc,
    atkB6_happinesstodamagecalculation,
    atkB7_presentdamagecalculation,
    atkB8_setsafeguard,
    atkB9_magnitudedamagecalculation,
    atkBA_jumpifnopursuitswitchdmg,
    atkBB_setsunny,
    atkBC_maxattackhalvehp,
    atkBD_copyfoestats,
    atkBE_rapidspinfree,
    atkBF_setdefensecurlbit,
    atkC0_recoverbasedonsunlight,
    atkC1_hiddenpowercalc,
    atkC2_selectfirstvalidtarget,
    atkC3_trysetfutureattack,
    atkC4_trydobeatup,
    atkC5_setsemiinvulnerablebit,
    atkC6_clearsemiinvulnerablebit,
    atkC7_setminimize,
    atkC8_sethail,
    atkC9_jumpifattackandspecialattackcannotfall,
    atkCA_setforcedtarget,
    atkCB_setcharge,
    atkCC_callterrainattack,
    atkCD_cureifburnedparalysedorpoisoned,
    atkCE_settorment,
    atkCF_jumpifnodamage,
    atkD0_settaunt,
    atkD1_trysethelpinghand,
    atkD2_tryswapitems,
    atkD3_trycopyability,
    atkD4_trywish,
    atkD5_trysetroots,
    atkD6_doubledamagedealtifdamaged,
    atkD7_setyawn,
    atkD8_setdamagetohealthdifference,
    atkD9_scaledamagebyhealthratio,
    atkDA_tryswapabilities,
    atkDB_tryimprison,
    atkDC_trysetgrudge,
    atkDD_weightdamagecalculation,
    atkDE_assistattackselect,
    atkDF_trysetmagiccoat,
    atkE0_trysetsnatch,
    atkE1_trygetintimidatetarget,
    atkE2_switchoutabilities,
    atkE3_jumpifhasnohp,
    atkE4_getsecretpowereffect,
    atkE5_pickup,
    atkE6_docastformchangeanimation,
    atkE7_trycastformdatachange,
    atkE8_settypebasedhalvers,
    atkE9_setweatherballtype,
    atkEA_tryrecycleitem,
    atkEB_settypetoterrain,
    atkEC_pursuitrelated,
    atkED_snatchsetbattlers,
    atkEE_removelightscreenreflect,
    atkEF_handleballthrow,
    atkF0_givecaughtmon,
    atkF1_trysetcaughtmondexflags,
    atkF2_displaydexinfo,
    atkF3_trygivecaughtmonnick,
    atkF4_subattackerhpbydmg,
    atkF5_removeattackerstatus1,
    atkF6_finishaction,
    atkF7_finishturn,
    atkF8_callasm,
    atkF9_sethalfword,
    atkFA_setword,
    atkFB_setspecialstatusbit,
    atkFC_clearspecialstatusbit,
    atkFD_jumpifabilitypresenttargetfield,
    atkFE_prefaintmoveendeffects,
    atkFF_callsecondarytable,
};

void (* const gBattleScriptingCommandsTable2[])(void) = 
{
    NULL,
    NULL,
    atkFF02_cureprimarystatus,
    atkFF03_jumpifpartnerattack,
    NULL,
    NULL,
    atkFF06_setterrain,
    atkFF07_jumpifhelditemeffect,
    atkFF08_counterclear,
    atkFF09_jumpifcounter,
    atkFF0A_setability,
    NULL,
    atkFF0C_jumpiftargetpartner,
    NULL,
    atkFF0E_setcounter,
    atkFF0F_jumpifgrounded,
    atkFF10_jumpifhelditem,
    atk98_updatestatusicon,
    atkFF12_jumpifhealthcomparestomax,
    atkFF13_setdamagetobankhealthfraction,
    atkFF14_jumpiftypepresent,
    atkFF15_jumpifstatcanbemodified,
    atkFF16_jumpifnoviablemonsleft,
    atkFF17_setsidestatus,
    atkFF18_clearsidestatus,
    atkFF19_formchange,
    atkFF1A_jumpifabilitypresentattackerfield,
    atkFF1B_tryactivateswitchinability,
    atkFF1C_handletrainerslidemsg,
    atkFF1D_trytrainerslidefirstdownmsg,
    atkFF1E_trainerslideout,
    atkFF1F_flowershieldlooper,
    atkFF20_jumpifprotectedbycraftyshield,
    atkFF21_tryspectralthiefsteal,
    atkFF22_jumpifspecies,
    atkFF23_faintpokemonaftermove,
    atkFF24_jumpifattackeralreadydiddamage,
    atkFF25_jumpifterrainandgrounded,
    atkFF26_attackstringnoprotean,
    atkFF27_tryactivateprotean,
    atkFF28_jumpifweight,
    [0x29] = atkFF29_trysetsleep,
    atkFF2A_trysetparalysis,
    atkFF2B_trysetburn,
    atkFF2C_trysetpoison,
    atkFF2D_addindicatorforattackerswitchineffects,
    atkFF2E_setmoveeffect2,
    NULL, //atkFF2F_setmaxmoveeffect,
    atkFF30_jumpifdynamaxed,
    atkFF31_jumpifraidboss,
    atkFF32_recycleberry,
    atkFF33_SetEffectPrimaryScriptingBank,
    atkFF34_canconfuse,
    atkFF35_jumpifmaxchistrikecapped,
};

// The chance is 1/N for each stage.
static const u16 sCriticalHitChance[] = { 16, 8, 4, 3, 2 };

static const u8 *const sMoveEffectBS_Ptrs[] =
{
    [0] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_SLEEP] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_POISON] = BattleScript_MoveEffectPoison,
    [MOVE_EFFECT_BURN] = BattleScript_MoveEffectBurn,
    [MOVE_EFFECT_FREEZE] = BattleScript_MoveEffectFreeze,
    [MOVE_EFFECT_PARALYSIS] = BattleScript_MoveEffectParalysis,
    [MOVE_EFFECT_TOXIC] = BattleScript_MoveEffectToxic,
    [MOVE_EFFECT_CONFUSION] = BattleScript_MoveEffectConfusion,
    [MOVE_EFFECT_FLINCH] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_TRI_ATTACK] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_UPROAR] = BattleScript_MoveEffectUproar,
    [MOVE_EFFECT_PAYDAY] = BattleScript_MoveEffectPayDay,
    [MOVE_EFFECT_CHARGING] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_WRAP] = BattleScript_MoveEffectWrap,
    [MOVE_EFFECT_RECOIL_25] = BattleScript_MoveEffectRecoil,
    [MOVE_EFFECT_ATK_PLUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_DEF_PLUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_SPD_PLUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_SP_ATK_PLUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_SP_DEF_PLUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_ACC_PLUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_EVS_PLUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_ATK_MINUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_DEF_MINUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_SPD_MINUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_SP_ATK_MINUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_SP_DEF_MINUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_ACC_MINUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_EVS_MINUS_1] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_RECHARGE] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_RAGE] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_STEAL_ITEM] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_PREVENT_ESCAPE] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_NIGHTMARE] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_ALL_STATS_UP] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_RAPIDSPIN] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_REMOVE_PARALYSIS] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_ATK_DEF_DOWN] = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_RECOIL_33] = BattleScript_MoveEffectRecoil,
};

// not used
static const struct WindowTemplate sUnusedWinTemplate =
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 3,
    .width = 7,
    .height = 15,
    .paletteNum = 31,
    .baseBlock = 0x3F,
};

// not used
static const u16 sUnknownBattleboxPal[] = INCBIN_U16("graphics/battle_interface/unk_battlebox.gbapal");
static const u32 sUnknownBattleboxGfx[] = INCBIN_U32("graphics/battle_interface/unk_battlebox.4bpp.lz");

// not used
static const u8 sRubyLevelUpStatBoxStats[] =
{
    MON_DATA_MAX_HP, MON_DATA_SPATK, MON_DATA_ATK,
    MON_DATA_SPDEF, MON_DATA_DEF, MON_DATA_SPEED
};

static const struct OamData sOamData_MonIconOnLvlUpBox =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

#define MON_ICON_LVLUP_BOX_TAG 0xD75A

static const struct SpriteTemplate sSpriteTemplate_MonIconOnLvlUpBox =
{
    .tileTag = MON_ICON_LVLUP_BOX_TAG,
    .paletteTag = MON_ICON_LVLUP_BOX_TAG,
    .oam = &sOamData_MonIconOnLvlUpBox,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_MonIconOnLvlUpBox
};

static const u16 sProtectSuccessRates[] =
{ 
    USHRT_MAX,
    USHRT_MAX / 2, 
    USHRT_MAX / 4, 
    USHRT_MAX / 8
};

#define MIMIC_FORBIDDEN_END             0xFFFE
#define METRONOME_FORBIDDEN_END         0xFFFF
#define ASSIST_FORBIDDEN_END            0xFFFF

static const u16 sMovesForbiddenToCopy[] =
{
    MOVE_METRONOME,
    MOVE_STRUGGLE,
    MOVE_SKETCH,
    MOVE_MIMIC,
    MIMIC_FORBIDDEN_END,
    MOVE_COUNTER,
    MOVE_MIRROR_COAT,
    MOVE_PROTECT,
    MOVE_DETECT,
    MOVE_ENDURE,
    MOVE_DESTINY_BOND,
    MOVE_SLEEP_TALK,
    MOVE_THIEF,
    MOVE_FOLLOW_ME,
    MOVE_SNATCH,
    MOVE_HELPING_HAND,
    MOVE_COVET,
    MOVE_TRICK,
    MOVE_FOCUS_PUNCH,
    METRONOME_FORBIDDEN_END
};

static const u8 sFlailHpScaleToPowerTable[] =
{
    1, 200,
    4, 150,
    9, 100,
    16, 80,
    32, 40,
    48, 20
};

static const u16 sWeightToDamageTable[] =
{
    100, 20,
    250, 40,
    500, 60,
    1000, 80,
    2000, 100,
    0xFFFF, 0xFFFF
};

static const u8 sTerrainToType[] =
{
    TYPE_GRASS,  // tall grass
    TYPE_GRASS,  // long grass
    TYPE_GROUND, // sand
    TYPE_WATER,  // underwater
    TYPE_WATER,  // water
    TYPE_WATER,  // pond water
    TYPE_ROCK,   // rock
    TYPE_ROCK,   // cave
    TYPE_NORMAL, // building
    TYPE_NORMAL, // plain
};

static const u8 sBallCatchBonuses[] =
{
    20, 15, 10, 15 // Ultra, Great, Poke, Safari
};

const u8 gReceiverBannedAbilities[] = 
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
    ABILITY_DISGUISE,
    //ABILITY_RKSSYSTEM,
    ABILITY_BATTLEBOND,
    ABILITY_POWERCONSTRUCT,
    //ABILITY_WONDERGUARD,
    ABILITY_NEUTRALIZINGGAS,
    ABILITY_ICEFACE,
    //ABILITY_HUNGERSWITCH,
    //ABILITY_GULPMISSILE,
    ABILITY_TABLES_TERMIN,
};

const u8 gRolePlayBannedAbilities[] = 
{
  ABILITY_TRACE,
  ABILITY_WONDER_GUARD,
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
  ABILITY_DISGUISE,
  //ABILITY_RKSSYSTEM,
  ABILITY_BATTLEBOND,
  ABILITY_POWERCONSTRUCT,
  ABILITY_ICEFACE,
  //ABILITY_HUNGERSWITCH,
  //ABILITY_GULPMISSILE,
  ABILITY_TABLES_TERMIN, 
};

const u8 gRolePlayAttackerBannedAbilities[] = 
{
  ABILITY_MULTITYPE,
  ABILITY_ZENMODE,
  ABILITY_STANCECHANGE,
  ABILITY_SCHOOLING,
  ABILITY_COMATOSE,
  ABILITY_SHIELDSDOWN,
  ABILITY_DISGUISE,
  //ABILITY_RKSSYSTEM,
  ABILITY_BATTLEBOND,
  ABILITY_POWERCONSTRUCT,
  ABILITY_ICEFACE,
  //ABILITY_GULPMISSILE,
  ABILITY_TABLES_TERMIN, 
};

const u8 gSkillSwapBannedAbilities[] = 
{
  ABILITY_WONDER_GUARD,
  ABILITY_MULTITYPE,
  ABILITY_ILLUSION,
  ABILITY_STANCECHANGE,
  ABILITY_SCHOOLING,
  ABILITY_COMATOSE,
  ABILITY_SHIELDSDOWN,
  ABILITY_DISGUISE,
  //ABILITY_RKSSYSTEM,
  ABILITY_BATTLEBOND,
  ABILITY_POWERCONSTRUCT,
  ABILITY_NEUTRALIZINGGAS,
  ABILITY_ICEFACE,
  //ABILITY_HUNGERSWITCH,
  //ABILITY_GULPMISSILE,
  ABILITY_TABLES_TERMIN, 
};

const u8 gWorrySeedBannedAbilities[] = 
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
  ABILITY_TRUANT,
  ABILITY_ICEFACE,
  //ABILITY_GULPMISSILE,
  ABILITY_TABLES_TERMIN, 
};

const u8 gGastroAcidBannedAbilities[] = 
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
  ABILITY_ICEFACE,
  //ABILITY_GULPMISSILE,
  ABILITY_TABLES_TERMIN, 
};

const u8 gEntrainmentBannedAbilitiesAttacker[] = 
{
  ABILITY_TRACE,
  ABILITY_FORECAST,
  ABILITY_FLOWERGIFT,
  ABILITY_ZENMODE,
  ABILITY_ILLUSION,
  ABILITY_IMPOSTER,
  ABILITY_POWEROFALCHEMY,
  ABILITY_RECEIVER,
  ABILITY_DISGUISE,
  ABILITY_POWERCONSTRUCT,
  ABILITY_NEUTRALIZINGGAS,
  ABILITY_ICEFACE,
  //ABILITY_HUNGERSWITCH,
  //ABILITY_GULPMISSILE,
  ABILITY_TABLES_TERMIN, 
};

const u8 gSimpleBeamBannedAbilities[] = 
{
  ABILITY_TRUANT,
  ABILITY_MULTITYPE,
  ABILITY_STANCECHANGE,
  ABILITY_SCHOOLING,
  ABILITY_COMATOSE,
  ABILITY_SHIELDSDOWN,
  ABILITY_DISGUISE,
  //ABILITY_RKSSYSTEM,
  ABILITY_BATTLEBOND,
  ABILITY_ICEFACE,
  //ABILITY_GULPMISSILE,
  ABILITY_TABLES_TERMIN, 
};

const u8 gEntrainmentBannedAbilitiesTarget[] = 
{
  ABILITY_TRUANT,
  ABILITY_MULTITYPE,
  ABILITY_STANCECHANGE,
  ABILITY_SCHOOLING,
  ABILITY_COMATOSE,
  ABILITY_SHIELDSDOWN,
  ABILITY_DISGUISE,
  //ABILITY_RKSSYSTEM,
  ABILITY_BATTLEBOND,
  ABILITY_ICEFACE,
  //ABILITY_GULPMISSILE,
  ABILITY_TABLES_TERMIN, 
};

const u16 gBannedBattleEatBerries[] =
{
	ITEM_POMEG_BERRY,
	ITEM_KELPSY_BERRY,
	ITEM_QUALOT_BERRY,
	ITEM_HONDEW_BERRY,
	ITEM_GREPA_BERRY,
	ITEM_TAMATO_BERRY,
	ITEM_TABLES_TERMIN
};

const u16 gUltraBeastList[] =
{
	SPECIES_NIHILEGO,
	SPECIES_BUZZWOLE,
	SPECIES_PHEROMOSA,
	SPECIES_XURKITREE,
	SPECIES_CELESTEELA,
	SPECIES_KARTANA,
	SPECIES_GUZZLORD,
	SPECIES_POIPOLE,
	SPECIES_NAGANADEL,
	SPECIES_STAKATAKA,
	SPECIES_BLACEPHALON,
	SPECIES_TABLES_TERMIN
};

static const u8 sMoveEffectsThatIgnoreSubstitute[] =
{
	MOVE_EFFECT_PAYDAY,
	MOVE_EFFECT_ION_DELUGE,
	MOVE_EFFECT_BREAK_SCREENS,
	MOVE_EFFECT_REMOVE_TERRAIN,
	MOVE_EFFECT_EAT_BERRY,
	0xFF,
};

static const u8 sShieldDustIgnoredEffects[] =
{
	MOVE_EFFECT_SLEEP,
	MOVE_EFFECT_POISON,
	MOVE_EFFECT_BURN,
	MOVE_EFFECT_FREEZE,
	MOVE_EFFECT_PARALYSIS,
	MOVE_EFFECT_TOXIC,
	MOVE_EFFECT_CONFUSION,
	MOVE_EFFECT_FLINCH,
	MOVE_EFFECT_TRI_ATTACK,
	MOVE_EFFECT_ATK_MINUS_1,
	MOVE_EFFECT_DEF_MINUS_1,
	MOVE_EFFECT_SPD_MINUS_1,
	MOVE_EFFECT_SP_ATK_MINUS_1,
	MOVE_EFFECT_SP_DEF_MINUS_1,
	MOVE_EFFECT_ACC_MINUS_1,
	MOVE_EFFECT_EVS_MINUS_1,
	//MOVE_EFFECT_THROAT_CHOP,
	MOVE_EFFECT_PREVENT_ESCAPE,
	0xFF,
};

static const u32 sStatusFlagsForMoveEffects[] =
{
	[MOVE_EFFECT_SLEEP] = STATUS1_SLEEP,
	[MOVE_EFFECT_POISON] = STATUS1_POISON,
	[MOVE_EFFECT_BURN] = STATUS1_BURN,
	[MOVE_EFFECT_FREEZE] = STATUS1_FREEZE,
	[MOVE_EFFECT_PARALYSIS] = STATUS1_PARALYSIS,
	[MOVE_EFFECT_TOXIC] = STATUS1_TOXIC_POISON,
	[MOVE_EFFECT_CONFUSION] = STATUS2_CONFUSION,
	[MOVE_EFFECT_FLINCH] = STATUS2_FLINCHED,
	[MOVE_EFFECT_UPROAR] = STATUS2_UPROAR,
	[MOVE_EFFECT_CHARGING] = STATUS2_MULTIPLETURNS,
	[MOVE_EFFECT_WRAP] = STATUS2_WRAPPED,
	[MOVE_EFFECT_RECHARGE] = STATUS2_RECHARGE,
	[MOVE_EFFECT_PREVENT_ESCAPE] = STATUS2_ESCAPE_PREVENTION,
	[MOVE_EFFECT_NIGHTMARE] = STATUS2_NIGHTMARE,
	[MOVE_EFFECT_THRASH] = STATUS2_LOCK_CONFUSE,
};


const struct TerrainTableStruct gTerrainTable[] =
{
	[ELECTRIC_TERRAIN - 1] =
		{
			.camouflageType = TYPE_ELECTRIC,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_THUNDERSHOCK,
			.naturePowerMove = MOVE_THUNDERBOLT,
		},

	[GRASSY_TERRAIN - 1] =
		{
			.camouflageType = TYPE_GRASS,
			.secretPowerEffect = MOVE_EFFECT_SLEEP,
			.secretPowerAnim = MOVE_VINEWHIP,
			.naturePowerMove = MOVE_ENERGYBALL,
		},

	[MISTY_TERRAIN - 1] =
		{
			.camouflageType = TYPE_FAIRY,
			.secretPowerEffect = MOVE_EFFECT_SP_ATK_MINUS_1,
			.secretPowerAnim = MOVE_FAIRYWIND,
			.naturePowerMove = MOVE_MOONBLAST,
		},

	[PSYCHIC_TERRAIN - 1] =
		{
			.camouflageType = TYPE_PSYCHIC,
			.secretPowerEffect = MOVE_EFFECT_SPD_MINUS_1,
			.secretPowerAnim = MOVE_CONFUSION,
			.naturePowerMove = MOVE_PSYCHIC,
		},

	[BATTLE_TERRAIN_GRASS + 4] =
		{
			.camouflageType = TYPE_GRASS,
			.secretPowerEffect = MOVE_EFFECT_SLEEP,
			.secretPowerAnim = MOVE_VINEWHIP,
			.naturePowerMove = MOVE_ENERGYBALL,
			.burmyForm = SPECIES_BURMY,
		},

	[BATTLE_TERRAIN_LONG_GRASS + 4] =
		{
			.camouflageType = TYPE_GRASS,
			.secretPowerEffect = MOVE_EFFECT_SLEEP,
			.secretPowerAnim = MOVE_VINEWHIP,
			.naturePowerMove = MOVE_ENERGYBALL,
			.burmyForm = SPECIES_BURMY,
		},

	[BATTLE_TERRAIN_SAND + 4] =
		{
			.camouflageType = TYPE_GROUND,
			.secretPowerEffect = MOVE_EFFECT_ACC_MINUS_1,
			.secretPowerAnim = MOVE_MUDSLAP,
			.naturePowerMove = MOVE_EARTHPOWER,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_UNDERWATER + 4] =
		{
			.camouflageType = TYPE_WATER,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_WATERPULSE,
			.naturePowerMove = MOVE_HYDROPUMP,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_WATER + 4] =
		{
			.camouflageType = TYPE_WATER,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_WATERPULSE,
			.naturePowerMove = MOVE_HYDROPUMP,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_POND + 4] =
		{
			.camouflageType = TYPE_WATER,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_WATERPULSE,
			.naturePowerMove = MOVE_HYDROPUMP,
			.burmyForm = SPECIES_NONE,
		},

	[BATTLE_TERRAIN_MOUNTAIN + 4] =
		{
			.camouflageType = TYPE_GROUND,
			.secretPowerEffect = MOVE_EFFECT_ACC_MINUS_1,
			.secretPowerAnim = MOVE_MUDSLAP,
			.naturePowerMove = MOVE_EARTHPOWER,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_CAVE + 4] =
		{
			.camouflageType = TYPE_ROCK,
			.secretPowerEffect = MOVE_EFFECT_FLINCH,
			.secretPowerAnim = MOVE_ROCKTHROW,
			.naturePowerMove = MOVE_POWERGEM,
			.burmyForm = SPECIES_BURMY_SANDY,
		},

	[BATTLE_TERRAIN_BUILDING + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_PLAIN + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_SLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY,
		},

	[BATTLE_TERRAIN_LINK + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_GYM + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_LEADER + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_INDOOR_2 + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_INDOOR_1 + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_LORELEI + 4] =
		{
			.camouflageType = TYPE_ICE,
			.secretPowerEffect = MOVE_EFFECT_FREEZE,
			.secretPowerAnim = MOVE_ICESHARD,
			.naturePowerMove = MOVE_ICEBEAM,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_BRUNO + 4] =
		{
			.camouflageType = TYPE_FIGHTING,
			.secretPowerEffect = MOVE_EFFECT_FLINCH,
			.secretPowerAnim = MOVE_KARATECHOP,
			.naturePowerMove = MOVE_FOCUSBLAST,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_AGATHA + 4] =
		{
			.camouflageType = TYPE_GHOST,
			.secretPowerEffect = MOVE_EFFECT_EVS_MINUS_1,
			.secretPowerAnim = MOVE_SHADOWSNEAK,
			.naturePowerMove = MOVE_SHADOWBALL,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_LANCE + 4] =
		{
			.camouflageType = TYPE_DRAGON,
			.secretPowerEffect = MOVE_EFFECT_ATK_MINUS_1,
			.secretPowerAnim = MOVE_DRAGONRAGE,
			.naturePowerMove = MOVE_DRAGONPULSE,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	[BATTLE_TERRAIN_CHAMPION + 4] =
		{
			.camouflageType = TYPE_NORMAL,
			.secretPowerEffect = MOVE_EFFECT_PARALYSIS,
			.secretPowerAnim = MOVE_BODYSLAM,
			.naturePowerMove = MOVE_TRIATTACK,
			.burmyForm = SPECIES_BURMY_TRASH,
		},

	//ADD NEW ENTRIES HERE
};

const u8 gText_CriticalHitTwoFoes[] = _("");
const u8 gText_CriticalHitTarget[] = _("");
const u8 gText_SuperEffectiveTwoFoes[] = _("");
const u8 gText_NotVeryEffectiveTwoFoes[] = _("");
const u8 gText_NotVeryEffectiveTarget[] = _("");
const u8 gText_DoesntAffectTwoFoes[] = _("");
const u8 CraftyShieldSetString[] = _("");
const u8 MatBlockSetString[] = _("");
const u8 QuickGuardSetString[] = _("");
const u8 WideGuardSetString[] = _("");
const u8 gText_ScreenRaisedStat[] = _("");
extern const u8 RemovedEntryHazardsTargetSideString[];
const u8 gText_SuperEffectiveTarget[] = _("");

extern const u8 RemovedEntryHazardsString[];
extern const u8 SpikesLayString[];
extern const u8 StealthRockLayString[];
extern const u8 ToxicSpikesLayString[];
extern const u8 StickyWebLayString[];
extern const u8 gText_SteelsurgeLay[];

const u8 SeaOfFireString [] = _("");
const u8 SwampString [] = _("");
const u8 RainbowString [] = _("");
const u8 PowerTrickString[] = _("");
const u8 PowerSwapString[] = _("");
const u8 GuardSwapString[] = _("");
const u8 SpeedSwapString[] = _("");
const u8 HeartSwapString[] = _("");
const u8 PowerSplitString[] = _("");
const u8 GuardSplitString[] = _("");
const u8 gText_SetAuroraVeil[] = _("");
const u8 GravitySetString[] = _("");
const u8 TrickRoomSetString[] = _("");
const u8 TrickRoomEndString[] = _("");
const u8 WonderRoomEndString[] = _("");
const u8 WonderRoomSetString[] = _("");
const u8 MagicRoomEndString[] = _("");
const u8 MagicRoomSetString[] = _("");
const u8 GravityEndString[] = _("");
const u8 IonDelugeShowerString[]= _("A deluge of ions showers the\nbattlefield!");
const u8 NoMoreAirborneFromGravityString[] = _("");
const u8 ThirdTypeAddedString[] = _("");
const u8 FairyLockString[] = _("");
const u8 HappyHourString[] = _("");
const u8 CelebrateString[] = _("恭喜{B_PLAYER_NAME}");
const u8 HoldHandsString[] = _("");
const u8 TailwindSetString[] = _("");
const u8 LuckyChantSetString[] = _("");
const u8 WorrySeedString[] = _("");
const u8 AbilitySuppressedString[] = _("");
const u8 EntrainmentString[] = _("");
const u8 SimpleBeamString[] = _("");
const u8 Bank0AWasBroughtDownString[] = _("");
const u8 TargetTransformedIntoType[] = _("");
const u8 gText_TargetAlreadyAsleep[] = _("{B_ATK_NAME_WITH_PREFIX} is fast\nasleep.");
const u8 gText_TargetAlreadyHasStatusCondition[] = _("");
const u8 gText_TeamProtectedBySafeguard[] = _("");
extern const u8 gText_TargetWrappedInElectricTerrain[];
const u8 gText_AlreadyDrowsy[] = _("");
extern const u8 gText_TargetWrappedInMistyTerrain[];
const u8 gText_TargetAlreadyPoisoned[] = _("");
const u8 gText_CantFallAsleepDuringUproar[] = _("");
const u8 gText_SleepClausePrevents[] = _("");
const u8 gText_TargetAlreadyConfused[] = _("");
const u8 gText_TargetAlreadyParalyzed[] = _("");
const u8 gText_TargetAlreadyBurned[] = _("");
const u8 RoastedBerryString[] = _("");
static const u8 TargetStatsResetString[] = _("{B_DEF_NAME_WITH_PREFIX}'s stat changes\nwere removed!");
static const u8 gText_TargetWasInfested[] = _("{B_DEF_NAME_WITH_PREFIX} has been\nafflicted with an Infestation!");
static const u8 gText_TargetWasCaughtInSnapTrap[] = _("{B_DEF_NAME_WITH_PREFIX} got trapped\nby a snap trap!");
static const u8 ScreensShatteredString[] = _("The wall shattered!");
//need todo
const u8 ElectricTerrainSetString[] = _("An electric current runs across\nthe battlefield!");
const u8 GrassyTerrainSetString[] = _("Grass grew to cover the\nbattlefield!");
const u8 MistyTerrainSetString[] = _("Mist swirls around the\nbattlefield!");
const u8 PsychicTerrainSetString[] = _("The battlefield got weird!");
const u8 TerrainEndString[]= _("The terrain returned to normal!");
const u8 FreedFromSkyDropString[]= _("The terrain returned to normal!");
const u8 gText_RaidBattleKO1[] = _("The storm raging above you is\ngrowing stronger!");
const u8 gText_RaidBattleKO2[] = _("The storm is growing even\nstronger!");
const u8 gText_RaidBattleKO3[] = _("The storm is getting too strong to\nwithstand!");
const u8 gText_RaidBattleKO4[] = _("{B_PLAYER_NAME}被轰出了战场!");

const u8* const sEntryHazardsStrings[] =
{
	SpikesLayString,
	StealthRockLayString,
	ToxicSpikesLayString,
	StickyWebLayString,
	gText_SteelsurgeLay,
};

#include "data/item_tables.h"

#define PU_NUM_COMMON_ITEMS     18
#define PU_NUM_RARE_ITEMS       11

static const u16 sPickupCommonItems[PU_NUM_COMMON_ITEMS] =
{
	ITEM_POTION,
	ITEM_ANTIDOTE,
	ITEM_SUPER_POTION,
	ITEM_GREAT_BALL,
	ITEM_REPEL,
	ITEM_ESCAPE_ROPE,
	ITEM_FULL_HEAL,
	ITEM_HYPER_POTION,
	ITEM_ULTRA_BALL,
	ITEM_REVIVE,
	ITEM_RARE_CANDY,
	ITEM_SUN_STONE,
	ITEM_MOON_STONE,
	ITEM_HEART_SCALE,
	ITEM_FULL_RESTORE,
	ITEM_MAX_REVIVE,
	ITEM_PP_UP,
	ITEM_MAX_ELIXIR,
};

static const u16 sPickupRareItems[PU_NUM_RARE_ITEMS] =
{
	ITEM_HYPER_POTION,
	ITEM_NUGGET,
	ITEM_KINGS_ROCK,
	ITEM_FULL_RESTORE,
	ITEM_ETHER,
	//ITEM_IRON_BALL,
	//ITEM_DESTINY_KNOT,
	ITEM_ELIXIR,
	//ITEM_DESTINY_KNOT,
	ITEM_LEFTOVERS,
	//ITEM_DESTINY_KNOT,
};

const u32 pickup_common_item_ceilings[PU_COMMON_PER_ROW] =
{
	19661, 26214, 32768, 39322, 45875, 52429, 58982, 61604, 64225
};

const u32 pickup_rare_item_ceilings[PU_RARE_PER_ROW] =
{
	64881, 65536
};

const u16 gTelekinesisBanList[] =
{
	SPECIES_DIGLETT,
	SPECIES_DUGTRIO,
	SPECIES_DIGLETT_A,
	SPECIES_DUGTRIO_A,
	SPECIES_SANDYGAST,
	SPECIES_PALOSSAND,
	SPECIES_GENGAR_MEGA,
	SPECIES_TABLES_TERMIN
};


static void atk40_jumpifaffectedbyprotect(void)
{
    if (ProtectAffects(gCurrentMove, gBattlerAttacker, gBattlerTarget, FALSE))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		JumpIfMoveFailed(5, 0);
		gBattleCommunication[6] = 1;
	}
	else
		gBattlescriptCurrInstr += 5;
}


static void atk01_accuracycheck(void)
{
    bool8 recalculatedDragonDarts = FALSE;
	u16 move = T2_READ_16(gBattlescriptCurrInstr + 5);

ACCURACY_CHECK_START:
    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
	{
		if (!BtlCtrl_OakOldMan_TestState2Flag(1) && SPLIT(move) != SPLIT_STATUS)
		{
			if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
			{
				JumpIfMoveFailed(7, move);
				return;
			}
		}

		if (!BtlCtrl_OakOldMan_TestState2Flag(2) && SPLIT(move) == SPLIT_STATUS)
		{
			if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
			{
				JumpIfMoveFailed(7, move);
				return;
			}
		}
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
	{
		JumpIfMoveFailed(7, move);
		return;
	}

    if (move == NO_ACC_CALC || move == NO_ACC_CALC_CHECK_LOCK_ON)
    {
        if (gStatuses3[gBattlerTarget] & STATUS3_ALWAYS_HITS
		&& move == NO_ACC_CALC_CHECK_LOCK_ON
		&& gDisableStructs[gBattlerTarget].battlerWithSureHit == gBattlerAttacker)
		{
			gBattlescriptCurrInstr += 7;
		}
		else if (gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE
			  && ABILITY(gBattlerAttacker) != ABILITY_NOGUARD
			  && ABILITY(gBattlerTarget) != ABILITY_NOGUARD)
		{
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		}
		else if (!JumpIfMoveAffectedByProtect(move, gBattlerAttacker, gBattlerTarget, TRUE))
		{
			gBattlescriptCurrInstr += 7;
		}
    }
    else
    {
        if (move == MOVE_NONE)
            move = gCurrentMove;

        if (gBattleStruct->ParentalBondOn == 1
		||  (gBattleStruct->MultiHitOn && (gBattleMoves[move].effect != EFFECT_TRIPLE_KICK
						 || ABILITY(gBattlerAttacker) == ABILITY_SKILLLINK)))
		{
			//No acc checks for second hit of Parental Bond or multi hit moves
			gBattlescriptCurrInstr += 7;
		}
        else
		{
			u8 atkItemEffect = ITEM_EFFECT(gBattlerAttacker);
			bool8 calcSpreadMove = IS_DOUBLE_BATTLE && gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY) && SPLIT(move) != SPLIT_STATUS;
			bool8 clearMicleBerryBits = FALSE;
            u32 bankDef;

			for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
			{
				if (!calcSpreadMove) //Single target
					bankDef = gBattlerTarget;
				else if (gBattleStruct->calculatedSpreadMoveAccuracy)
				{
					if (gBattleStruct->ResultFlags[gBattlerTarget] & MOVE_RESULT_MISSED)
						gMoveResultFlags = gBattleStruct->ResultFlags[gBattlerTarget];
					else
						gMoveResultFlags = 0;
					break; //Already calculated accuracy miss
				}
				else if (!BATTLER_ALIVE(bankDef) || bankDef == gBattlerAttacker
				|| (bankDef == PARTNER(gBattlerAttacker) && !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_FOES_AND_ALLY))
				|| (gBattleStruct->noResultString[bankDef] && gBattleStruct->noResultString[bankDef] != 2))
					continue; //Don't bother with this target

				if (!JumpIfMoveAffectedByProtect(move, gBattlerAttacker, bankDef, FALSE) //Don't jump yet, jump later
				&& !AccuracyCalcHelper(move, bankDef))
				{
					u32 calc = AccuracyCalc(move, gBattlerAttacker, bankDef);
					clearMicleBerryBits = TRUE;
					
					gPotentialItemEffectBattler = bankDef;

					// final calculation
					if (Random32() % 101 > calc)
					{
						gBattleStruct->ResultFlags[bankDef] = MOVE_RESULT_MISSED; //Overwrite old value which can include effectiveness
						gBattleStruct->missStringId[bankDef] = gBattleCommunication[6] = 2; //STRINGID_PKMNAVOIDEDATTACK

						if (atkItemEffect == HOLD_EFFECT_BLUNDER_POLICY)
							gBattleStruct->activateBlunderPolicy = TRUE;

						if (gCurrentMove == MOVE_DRAGONDARTS
						&& !recalculatedDragonDarts //So don't jump back and forth between targets
						&& CanTargetPartner(bankDef)
						&& !TargetFullyImmuneToCurrMove(PARTNER(bankDef)))
						{
							//Smart target to partner if miss
							bankDef = PARTNER(bankDef);
							recalculatedDragonDarts = TRUE;
							gBattleStruct->ResultFlags[bankDef] &= ~MOVE_RESULT_MISSED;
							goto ACCURACY_CHECK_START;
						}
					}
				}

				if (!calcSpreadMove)
					break; //Only one target
			}

			if (gBattleStruct->ResultFlags[gBattlerTarget] & MOVE_RESULT_MISSED)
				gMoveResultFlags = MOVE_RESULT_MISSED; //We only care about the miss, not effectiveness
			else if (calcSpreadMove)
				gMoveResultFlags = 0;

			gBattleStruct->calculatedSpreadMoveAccuracy = TRUE;
			if (clearMicleBerryBits) //Micle Berry was used in some calculation
				gBattleStruct->MicleBerryBits &= ~gBitTable[gBattlerAttacker]; //Clear Micle Berry bit

			JumpIfMoveFailed(7, move);
		}
    }
}

bool8 TryActivateGemBattlescript(void)
{
	if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_GEM
	&&  ITEM_QUALITY(gBattlerAttacker) == gBattleStruct->dynamicMoveType
	&&  SPLIT(gCurrentMove) != SPLIT_STATUS
	&& !(gMoveResultFlags & (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED))
	&& !(NewTypeCalc(gCurrentMove, gBattlerAttacker, gBattlerTarget, GetBankPartyData(gBattlerAttacker), 0) & (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED))
	&& gBattleMoves[gCurrentMove].effect != EFFECT_PLEDGE
	&& AttacksThisTurn(gBattlerAttacker, gCurrentMove) == 2)
	{
		gLastUsedItem = ITEM(gBattlerAttacker);
		gBattleStruct->GemHelper = TRUE;
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_Gems;
		return TRUE;
	}

	return FALSE;
}

void TryUseGemFutureSight(void)
{
	if (TryActivateGemBattlescript())
		gBattlescriptCurrInstr -= 5;
}

static void atk02_attackstring(void)
{
    u8 moveType = gBattleStruct->dynamicMoveType;

    if (gBattleControllerExecFlags) return;

    if (gBattlescriptCurrInstr == BattleScript_ButItFailedAttackstring)
		gMoveResultFlags |= MOVE_RESULT_FAILED;
    
    if (!(gHitMarker & (HITMARKER_NO_ATTACKSTRING | HITMARKER_ATTACKSTRING_PRINTED)))
	{
		PrepareStringBattle(STRINGID_USEDMOVE, gBattlerAttacker);

		gBattlescriptCurrInstr++;
		gBattleCommunication[MSG_DISPLAY] = 0;

		if (!gBattleStruct->DancerInProgress && gCurrentMove != MOVE_COPYCAT)
		{
			gBattleStruct->LastUsedMove = gCurrentMove;
			gBattleStruct->LastUsedTypes[gBattlerAttacker] = moveType;

			if (IsAnyMaxMove(gCurrentMove))
				gBattleStruct->LastUsedMove = gChosenMove;

			if (!CheckTableForMove(gCurrentMove, gMovesThatCallOtherMoves))
			{
				u8 chargingBonus = 20 * gBattleStruct->metronomeItemBonus[gBattlerAttacker];
				if (gLastPrintedMoves[gBattlerAttacker] == gCurrentMove)
					gBattleStruct->MetronomeCounter[gBattlerAttacker] = MathMin(100, gBattleStruct->MetronomeCounter[gBattlerAttacker] + 20 + chargingBonus);
				else
					gBattleStruct->MetronomeCounter[gBattlerAttacker] = 0 + chargingBonus;
			}
		}


    if (gCurrentMove != MOVE_STRUGGLE && !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE))
		{
			TryActivateGemBattlescript();

			if (ABILITY(gBattlerAttacker) == ABILITY_PROTEAN
			&& !(gMoveResultFlags & MOVE_RESULT_FAILED)
			&& !CheckTableForMove(gCurrentMove, gMovesThatCallOtherMoves))
			{
				if (gBattleMons[gBattlerAttacker].type1 != moveType
				||  gBattleMons[gBattlerAttacker].type2 != moveType
				|| (gBattleMons[gBattlerAttacker].type3 != moveType && gBattleMons[gBattlerAttacker].type3 != TYPE_BLANK))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AbilityChangedType;
				}
			}
		}

		gHitMarker |= HITMARKER_ATTACKSTRING_PRINTED;
	}
	else
	{
		gBattlescriptCurrInstr++;
		gBattleCommunication[MSG_DISPLAY] = 0;
	}
}

void BufferAttackerItem(void)
{
	gLastUsedItem = ITEM(gBattlerAttacker);
}

static void atk03_ppreduce(void)
{
	s32 ppToDeduct = 1;

	if (gBattleControllerExecFlags) return;

	if (!gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure) {
		switch (gBattleMoves[gCurrentMove].target) {
			case MOVE_TARGET_FOES_AND_ALLY:
				ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_ON_FIELD, gBattlerAttacker, ABILITY_PRESSURE, 0, 0);
				break;

			case MOVE_TARGET_BOTH:
			case MOVE_TARGET_OPPONENTS_FIELD:
				ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIDE, gBattlerAttacker, ABILITY_PRESSURE, 0, 0);
				break;

			default:
				if (gBattlerAttacker != gBattlerTarget && ABILITY(gBattlerTarget) == ABILITY_PRESSURE)
					ppToDeduct++;
				break;
		}
	}

	if (!(gHitMarker & (HITMARKER_NO_PPDEDUCT | HITMARKER_NO_ATTACKSTRING))
	&& gBattleStruct->ParentalBondOn != 1
	&& !gBattleStruct->DancerInProgress
	&& gBattleMons[gBattlerAttacker].pp[gCurrMovePos]) {
		gProtectStructs[gBattlerAttacker].notFirstStrike = 1;

		if (gBattleMons[gBattlerAttacker].pp[gCurrMovePos] > ppToDeduct)
			gBattleMons[gBattlerAttacker].pp[gCurrMovePos] -= ppToDeduct;
		else
			gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = 0;

		if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED)
			&& !((gDisableStructs[gBattlerAttacker].mimickedMoves) & gBitTable[gCurrMovePos]))
		{
			gActiveBattler = gBattlerAttacker;
			BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + gCurrMovePos, 0, 1, &gBattleMons[gBattlerAttacker].pp[gCurrMovePos]);
			MarkBattlerForControllerExec(gBattlerAttacker);
		}
	}

	gHitMarker &= ~(HITMARKER_NO_PPDEDUCT);
	gBattlescriptCurrInstr++;
}


static bool8 TryActivateWeakenessBerry(u8 bank, u8 resultFlags)
{
	if (ITEM_EFFECT(bank) == HOLD_EFFECT_WEAKNESS_BERRY && !AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, bank, ABILITY_UNNERVE, 0, 0))
	{
		if ((resultFlags & MOVE_RESULT_SUPER_EFFECTIVE && ITEM_QUALITY(bank) == gBattleStruct->dynamicMoveType && !DoesBankNegateDamage(bank, gCurrentMove))
		||  (ITEM_QUALITY(bank) == TYPE_NORMAL && gBattleStruct->dynamicMoveType == TYPE_NORMAL)) //Chilan Berry
		{
			gLastUsedItem = ITEM(bank);
			gBattleScripting.battler = bank;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_WeaknessBerryActivate - 5;
			return TRUE;
		}
	}

	return FALSE;
}

void TryActivateWeakenessBerryFutureSight(void)
{
	TryActivateWeakenessBerry(gBattlerTarget, gMoveResultFlags);
}

static bool8 IsSingleTargetOfDoublesSpreadMove(void)
{
	if (gBattleMoves[gCurrentMove].target & MOVE_TARGET_FOES_AND_ALLY)
		return gBattleStruct->allSpreadTargets <= 1;
	else if (gBattleMoves[gCurrentMove].target & MOVE_TARGET_BOTH)
		return gBattleStruct->foeSpreadTargets <= 1;

	return TRUE;
}

static bool8 IsDoubleSpreadMove(void)
{
	return IS_DOUBLE_BATTLE
		&& !(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG | HITMARKER_UNABLE_TO_USE_MOVE))
		&& gBattleMoves[gCurrentMove].target & (MOVE_TARGET_FOES_AND_ALLY | MOVE_TARGET_BOTH)
		&& !IsSingleTargetOfDoublesSpreadMove();
}

static bool8 DoesBankNegateDamage(u8 bank, u16 move)
{
	u16 species = SPECIES(bank);
	ability_t ability = ABILITY(bank);

	return FALSE
#ifdef SPECIES_MIMIKYU
		   || (ability == ABILITY_DISGUISE && species == SPECIES_MIMIKYU && !IS_TRANSFORMED(bank))
#endif
#ifdef SPECIES_EISCUE
		   || (ability == ABILITY_ICEFACE && species == SPECIES_EISCUE && SPLIT(move) == SPLIT_PHYSICAL && !IS_TRANSFORMED(bank))
#endif
		;
}

static u8 UpdateEffectivenessResultFlagsForDoubleSpreadMoves(u8 resultFlags)
{
	u32 i, j;

	//Only play the "best" sound
	for (i = 0; i < 3; ++i)
	{
		for (j = 0; j < gBattlersCount; ++j)
		{
			if (!(gBattleStruct->ResultFlags[j] & (MOVE_RESULT_MISSED | MOVE_RESULT_NO_EFFECT)) && !gBattleStruct->noResultString[j])
			{
				switch (i) {
					case 0:
						if (gBattleStruct->ResultFlags[j] & MOVE_RESULT_SUPER_EFFECTIVE && !DoesBankNegateDamage(j, gCurrentMove))
							return gBattleStruct->ResultFlags[j];
						break;
					case 1:
						if (gBattleStruct->ResultFlags[j] & MOVE_RESULT_NOT_VERY_EFFECTIVE && !DoesBankNegateDamage(j, gCurrentMove))
							return gBattleStruct->ResultFlags[j];
						break;
					case 2:
						if (DoesBankNegateDamage(j, gCurrentMove))
							return 0; //Normal effectiveness
						return gBattleStruct->ResultFlags[j];
				}
			}
		}
	}

	return resultFlags;
}

static const u16 sCriticalHitChances[] =
#ifdef CRIT_CHANCE_GEN_6
	{16, 8, 2, 1, 1};
#elif (defined CRIT_CHANCE_GEN_2_TO_5)
	{16, 8, 4, 3, 2};
#else
	{24, 8, 2, 1, 1};
#endif

static void atk04_critcalc(void)
{
    u16 critChance;
	bool8 confirmedCrit;
    u32 bankDef;
	ability_t atkAbility = ABILITY(gBattlerAttacker);
	u8 atkEffect = ITEM_EFFECT(gBattlerAttacker);
	bool8 calcSpreadMove = IS_DOUBLE_BATTLE && gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY);
    ability_t defAbility;

	gPotentialItemEffectBattler = gBattlerAttacker;

	for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
	{
		confirmedCrit = FALSE;
		critChance = 0;

		if (!calcSpreadMove) //Single target
			bankDef = gBattlerTarget;
		else if (gBattleStruct->calculatedSpreadMoveData)
			break; //Already calculated crit chance
		else if (!BATTLER_ALIVE(bankDef) || bankDef == gBattlerAttacker
		|| (bankDef == PARTNER(gBattlerAttacker) && !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_FOES_AND_ALLY))
		|| gBattleStruct->ResultFlags[bankDef] & MOVE_RESULT_NO_EFFECT
		|| gBattleStruct->noResultString[bankDef])
			continue; //Don't bother with this target

		defAbility = ABILITY(bankDef);

		if (defAbility == ABILITY_BATTLE_ARMOR
		||  defAbility == ABILITY_SHELL_ARMOR
		||  CantScoreACrit(gBattlerAttacker, NULL)
		||  gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_FIRST_BATTLE | BATTLE_TYPE_POKEDUDE)
		||  gBattleStruct->LuckyChantTimers[SIDE(bankDef)])
		{
			confirmedCrit = FALSE;
		}
		else if ((atkAbility == ABILITY_MERCILESS && (gBattleMons[bankDef].status1 & STATUS1_PSN_ANY))
		|| IsLaserFocused(gBattlerAttacker)
		|| CheckTableForMove(gCurrentMove, gAlwaysCriticalMoves))
		{
			confirmedCrit = TRUE;
		}
		else 
		{
			critChance  = 2 * ((gBattleMons[gBattlerAttacker].status2 & STATUS2_FOCUS_ENERGY) != 0)
						+ gBattleStruct->chiStrikeCritBoosts[gBattlerAttacker]
						+ (CheckTableForMove(gCurrentMove, gHighCriticalChanceMoves))
						+ (atkEffect == HOLD_EFFECT_SCOPE_LENS)
						+ (atkAbility == ABILITY_SUPERLUCK)
						#ifdef SPECIES_CHANSEY
						+ 2 * (atkEffect == HOLD_EFFECT_LUCKY_PUNCH && gBattleMons[gBattlerAttacker].species == SPECIES_CHANSEY)
						#endif
						#ifdef SPECIES_FARFETCHD
						+ 2 * (atkEffect == HOLD_EFFECT_STICK && gBattleMons[gBattlerAttacker].species == SPECIES_FARFETCHD)
						#endif
						+ 2 * (gCurrentMove == MOVE_10000000_VOLT_THUNDERBOLT);

			if (critChance > 4)
				critChance = 4;

			if (!(Random() % sCriticalHitChances[critChance]))
				confirmedCrit = TRUE;
		}

		gCritMultiplier = BASE_CRIT_MULTIPLIER;

		//These damages will be divded by 100 so really 2x and 1.5x
		if (confirmedCrit)
			gCritMultiplier = CRIT_MULTIPLIER;

		gBattleStruct->criticalMultiplier[bankDef] = gCritMultiplier;

		if (!calcSpreadMove)
			break; //Only 1 target
	}

	gCritMultiplier = gBattleStruct->criticalMultiplier[gBattlerTarget];
	++gBattlescriptCurrInstr;
}

static void atk05_damagecalc(void)
{
    struct DamageCalc data = {0};
    
	gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBattlerAttacker, gCurrentMove);

	if (gBattleStruct->calculatedSpreadMoveData && gMultiHitCounter == 0)
	{
		//Just use the calculated values below
	}
	else if (IS_DOUBLE_BATTLE && gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY))
	{
        u32 bankDef;
		data.bankAtk = gBattlerAttacker;
		data.move = gCurrentMove;
		PopulateDamageCalcStructWithBaseAttackerData(&data);

		//All multi target foes are calculated now b/c stats can change after first kill (eg. Moxie) - also for synchronized HP bar reduction
		for ( bankDef = 0; bankDef < gBattlersCount; ++bankDef)
		{
			if (!BATTLER_ALIVE(bankDef) || bankDef == gBattlerAttacker
			|| (bankDef == PARTNER(gBattlerAttacker) && !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_FOES_AND_ALLY))
			|| gBattleStruct->ResultFlags[bankDef] & MOVE_RESULT_NO_EFFECT
			|| gBattleStruct->noResultString[bankDef])
					continue; //Don't bother with this target

			data.bankDef = bankDef;
			PopulateDamageCalcStructWithBaseDefenderData(&data);
			gCritMultiplier = gBattleStruct->criticalMultiplier[bankDef];
			gBattleStruct->DamageTaken[bankDef] = (CalculateBaseDamage(&data) * gCritMultiplier) / BASE_CRIT_MULTIPLIER;
		}
	}
	else //Single Battle or single target move
	{
		data.bankAtk = gBattlerAttacker;
		data.bankDef = gBattlerTarget;
		data.move = gCurrentMove;
		gCritMultiplier = gBattleStruct->criticalMultiplier[gBattlerTarget];
		gBattleMoveDamage = (CalculateBaseDamage(&data) * gCritMultiplier) / BASE_CRIT_MULTIPLIER;
		gBattleStruct->DamageTaken[gBattlerTarget] = gBattleMoveDamage;
	}

	gBattleMoveDamage = gBattleStruct->DamageTaken[gBattlerTarget];
	gCritMultiplier = gBattleStruct->criticalMultiplier[gBattlerTarget];
	++gBattlescriptCurrInstr;
}

void AI_CalcDmg(u8 attacker, u8 defender)
{
    u16 sideStatus = gSideStatuses[GET_BATTLER_SIDE(defender)];

    /*gBattleMoveDamage = CalculateBaseDamage(&gBattleMons[attacker],
                                            &gBattleMons[defender],
                                            gCurrentMove,
                                            sideStatus,
                                            gDynamicBasePower,
                                            gBattleStruct->dynamicMoveType,
                                            attacker,
                                            defender);*///need todo
    gDynamicBasePower = 0;
    gBattleMoveDamage = gBattleMoveDamage * gCritMultiplier * gBattleScripting.dmgMultiplier;
    if (gStatuses3[attacker] & STATUS3_CHARGED_UP && gBattleMoves[gCurrentMove].type == TYPE_ELECTRIC)
        gBattleMoveDamage *= 2;
    if (gProtectStructs[attacker].helpingHand)
        gBattleMoveDamage = gBattleMoveDamage * 15 / 10;
}

static void atk06_typecalc(void)
{
    u8 moveType = gBattleStruct->dynamicMoveType & 0x3F;
	ability_t atkAbility = ABILITY(gBattlerAttacker);
	u8 atkType1 = gBattleMons[gBattlerAttacker].type1;
	u8 atkType2 = gBattleMons[gBattlerAttacker].type2;
	u8 atkType3 = gBattleMons[gBattlerAttacker].type3;
	bool8 calcSpreadMove = IS_DOUBLE_BATTLE && gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY);
    u32 bankDef;
    ability_t defAbility;
	u8 defEffect;

	if (gCurrentMove != MOVE_STRUGGLE)
	{
		for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
		{
			if (!calcSpreadMove) //Single target move
				bankDef = gBattlerTarget;
			else if (gBattleStruct->calculatedSpreadMoveData)
				break; //Already calculated type adjustment
			else if (!BATTLER_ALIVE(bankDef) || bankDef == gBattlerAttacker
			|| (bankDef == PARTNER(gBattlerAttacker) && !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_FOES_AND_ALLY))
			|| gBattleStruct->noResultString[bankDef])
				continue;

			defAbility = ABILITY(bankDef);
			defEffect = ITEM_EFFECT(bankDef);
			gBattleMoveDamage = gBattleStruct->DamageTaken[bankDef];
			gBattleStruct->ResultFlags[bankDef] &= ~(MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE | MOVE_RESULT_DOESNT_AFFECT_FOE); //Reset for now so damage can be modulated properly

			//Check Stab
			if (atkType1 == moveType || atkType2 == moveType || atkType3 == moveType)
			{
				if (atkAbility == ABILITY_ADAPTABILITY)
					gBattleMoveDamage *= 2;
				else
					gBattleMoveDamage = (gBattleMoveDamage * 15) / 10;
			}

			//Check Special Ground Immunities
			if (moveType == TYPE_GROUND && !CheckGrounding(bankDef) && gCurrentMove != MOVE_THOUSANDARROWS)
			{
				if (defAbility == ABILITY_LEVITATE)
				{
					gLastUsedAbility = defAbility;
					gBattleStruct->ResultFlags[bankDef] |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
					gLastLandedMoves[bankDef] = 0;
					gLastHitByType[bankDef] = 0;
					gBattleStruct->missStringId[bankDef] = 3;
					RecordAbilityBattle(bankDef, gLastUsedAbility);
				}
				else if (defEffect == HOLD_EFFECT_AIR_BALLOON)
				{
					gBattleStruct->ResultFlags[bankDef] |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
					gLastLandedMoves[bankDef] = 0;
					gLastHitByType[bankDef] = 0;
					RecordItemEffectBattle(bankDef, defEffect);
				}
				else if (gStatuses3[bankDef] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS))
				{
					gBattleStruct->ResultFlags[bankDef] |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
					gLastLandedMoves[bankDef] = 0;
					gLastHitByType[bankDef] = 0;
				}
				else
					goto RE_ENTER_TYPE_CHECK;	//You're a flying type
			}

			//Check Powder Moves
			else if (CheckTableForMove(gCurrentMove, gPowderMoves))
			{
				if (defAbility == ABILITY_OVERCOAT)
				{
					gLastUsedAbility = defAbility;
					gBattleStruct->ResultFlags[bankDef] |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
					gLastLandedMoves[bankDef] = 0;
					gLastHitByType[bankDef] = 0xFF;
					gBattleStruct->missStringId[bankDef] = 3;
					RecordAbilityBattle(bankDef, gLastUsedAbility);
				}
				else if (defEffect == HOLD_EFFECT_SAFETY_GOGGLES)
				{
					gBattleStruct->ResultFlags[bankDef] |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
					gLastLandedMoves[bankDef] = 0;
					gLastHitByType[bankDef] = 0xFF;
					RecordItemEffectBattle(bankDef, defEffect);
				}
				else if (IsOfType(bankDef, TYPE_GRASS))
				{
					gBattleStruct->ResultFlags[bankDef] |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
					gLastLandedMoves[bankDef] = 0;
					gLastHitByType[bankDef] = 0xFF;
				}
				else
					goto RE_ENTER_TYPE_CHECK;
			}
			else if (gCurrentMove == MOVE_SKYDROP && IsOfType(bankDef, TYPE_FLYING))
			{
				gBattleStruct->ResultFlags[bankDef] |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
				gLastLandedMoves[bankDef] = 0;
				gLastHitByType[bankDef] = 0xFF;
			}
			else
			{
			RE_ENTER_TYPE_CHECK:
				TypeDamageModification(atkAbility, bankDef, gCurrentMove, moveType, &gBattleStruct->ResultFlags[bankDef]);
			}

			if (defAbility == ABILITY_WONDER_GUARD
			 //&& AttacksThisTurn(gBattlerAttacker, gCurrentMove) == 2
			 && (!(gBattleStruct->ResultFlags[bankDef] & MOVE_RESULT_SUPER_EFFECTIVE)
				|| ((gBattleStruct->ResultFlags[bankDef] & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
			 && SPLIT(gCurrentMove) != SPLIT_STATUS)
			 {
				gLastUsedAbility = defAbility;
				gBattleStruct->ResultFlags[bankDef] |= MOVE_RESULT_MISSED;
				gLastLandedMoves[bankDef] = 0;
				gLastHitByType[bankDef] = 0;
				gBattleStruct->missStringId[bankDef] = 3;
				RecordAbilityBattle(bankDef, gLastUsedAbility);
			}

			if (gBattleStruct->ResultFlags[bankDef] & MOVE_RESULT_DOESNT_AFFECT_FOE)
				gProtectStructs[gBattlerAttacker].targetNotAffected = 1;

			gBattleStruct->DamageTaken[bankDef] = gBattleMoveDamage;

			if (!calcSpreadMove)
				break; //Only 1 target
		}

		gBattleMoveDamage = gBattleStruct->DamageTaken[gBattlerTarget];
		gMoveResultFlags = gBattleStruct->ResultFlags[gBattlerTarget];
	}

	++gBattlescriptCurrInstr;
}

u8 TypeCalc(u16 move, u8 attacker, u8 defender)
{
    u8 flags = 0;

    return flags;
}

u8 AI_TypeCalc(u16 move, u16 targetSpecies, ability_t targetAbility)
{
    s32 i = 0;
    u8 flags = 0;
    return flags;
}

static inline void ApplyRandomDmgMultiplier(void)
{
    u16 rando = 100 - (Random() & 15);

	if (gBattleMoveDamage)
		gBattleMoveDamage = MathMax(1, (gBattleMoveDamage * rando) / 100);
}

static void atk07_adjustnormaldamage(void)
{
    ApplyRandomDmgMultiplier();
	AdjustDamage(TRUE);
}

// The same as 0x7 except it doesn't check for false swipe move effect.
static void atk08_adjustnormaldamage2(void)
{
    ApplyRandomDmgMultiplier();
	AdjustDamage(FALSE);
}

static void atk09_attackanimation(void)
{
    u8 bankDef;
    u8 resultFlags;

    if (gBattleControllerExecFlags) return;

	if (IsDoubleSpreadMove())
	{
		for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
		{
			if (!BATTLER_ALIVE(bankDef) || bankDef == gBattlerAttacker
			|| (bankDef == PARTNER(gBattlerAttacker) && !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_FOES_AND_ALLY))
			|| (gBattleStruct->noResultString[bankDef] && gBattleStruct->noResultString[bankDef] != 2))
				continue; //Don't bother with this target

			if (TryActivateWeakenessBerry(bankDef, gBattleStruct->ResultFlags[bankDef]))
			{
				gBattlescriptCurrInstr += 5; //Counteract the callasm decrement
				return;
			}
		}
	}
	else //Single Target Move
	{
		if (TryActivateWeakenessBerry(gBattlerTarget, gMoveResultFlags))
		{
			gBattlescriptCurrInstr += 5; //Counteract the callasm decrement
			return;
		}
	}
	
	resultFlags = gMoveResultFlags;
	if (IsDoubleSpreadMove())
		resultFlags = UpdateEffectivenessResultFlagsForDoubleSpreadMoves(resultFlags);

	if (((gHitMarker & HITMARKER_NO_ANIMATIONS) && (gCurrentMove != MOVE_TRANSFORM && gCurrentMove != MOVE_SUBSTITUTE))
	|| gBattleStruct->tempIgnoreAnimations)
	{
		if (!(resultFlags & MOVE_RESULT_NO_EFFECT))
			gBattleStruct->attackAnimationPlayed = TRUE;

		if (gBattleStruct->tempIgnoreAnimations)
		{
			gBattlescriptCurrInstr += 1;
			gBattleStruct->tempIgnoreAnimations = FALSE;
		}
		else
		{
			BattleScriptPush(gBattlescriptCurrInstr + 1);
			gBattlescriptCurrInstr = BattleScript_Pausex20;
		}

		gBattleScripting.animTurn++;
		gBattleScripting.animTargetsHit++;
	}
	else
	{
		if (gBattleStruct->ParentalBondOn == 1)
		{
			gBattlescriptCurrInstr++;
			return;
		}
		else if (gBattleScripting.animTargetsHit > 0
			&&  (gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY | MOVE_TARGET_DEPENDS)
			  || gCurrentMove == MOVE_DEFOG
			  || gCurrentMove == MOVE_SHELLSMASH
			  || gCurrentMove == MOVE_HOWL))
		{
			gBattlescriptCurrInstr++;
			return;
		}

		if (!(resultFlags & MOVE_RESULT_NO_EFFECT))
		{
			u8 multihit;

			gActiveBattler = gBattlerAttacker;

			if (gBattleMons[gBattlerTarget].status2 & STATUS2_SUBSTITUTE)
				multihit = gMultiHitCounter;
			else if (gMultiHitCounter != 0 && gMultiHitCounter != 1)
			{
				if (gBattleMons[gBattlerTarget].hp <= gBattleMoveDamage)
					multihit = 1;
				else
					multihit = gMultiHitCounter;
			}
			else
				multihit = gMultiHitCounter;

			gBattleStruct->attackAnimationPlayed = TRUE;
			BtlController_EmitMoveAnimation(0, gCurrentMove, gBattleScripting.animTurn, gBattleMovePower, gBattleMoveDamage, gBattleMons[gBattlerAttacker].friendship, &gDisableStructs[gBattlerAttacker], multihit);
			gBattleScripting.animTurn += 1;
			gBattleScripting.animTargetsHit += 1;
			MarkBattlerForControllerExec(gBattlerAttacker);
			gBattlescriptCurrInstr++;
		}
		else if (!IsDoubleSpreadMove() || !gBattleStruct->calculatedSpreadMoveData)
		{
			BattleScriptPush(gBattlescriptCurrInstr + 1);
			gBattlescriptCurrInstr = BattleScript_Pausex20;
		}
		else
			gBattlescriptCurrInstr++;
	}
	
	gBattleStruct->calculatedSpreadMoveData = TRUE;
}

static void atk0A_waitanimation(void)
{
    if (!gBattleControllerExecFlags)
        ++gBattlescriptCurrInstr;
}

static void DoublesHPBarReduction(void)
{
	if (!gBattleStruct->doneDoublesSpreadHit //Spread moves in doubles are only done once
	&& !(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG)))
	{
		s16 healthValue;
		s32 currDmg, maxPossibleDmgValue;
        u32 i;
		maxPossibleDmgValue = 0xFFFF; //Ensures that huge damage values don't change sign

		for (i = 0; i < gBattlersCount; ++i)
		{
			if (!(gBattleStruct->ResultFlags[i] & MOVE_RESULT_NO_EFFECT) && gBattleStruct->DamageTaken[i] != 0 && !gBattleStruct->noResultString[i]
			&& !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, i)
			&& !DoesBankNegateDamage(i, gCurrentMove))
			{
				gActiveBattler = i;
				currDmg = gBattleStruct->DamageTaken[i];
				
				if (currDmg <= maxPossibleDmgValue)
					healthValue = currDmg;
				else
					healthValue = maxPossibleDmgValue;

				BtlController_EmitHealthBarUpdate(0, healthValue);
				MarkBattlerForControllerExec(gActiveBattler);

				if (SIDE(gActiveBattler) == B_SIDE_PLAYER && currDmg > 0)
					gBattleResults.playerMonWasDamaged = TRUE;
			}
		}
		
		gBattleStruct->doneDoublesSpreadHit = TRUE;
	}
}

static void atk0B_healthbarupdate(void)
{
    ability_t ability;

	if (gBattleControllerExecFlags) return;

	if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) || (gHitMarker & HITMARKER_NON_ATTACK_DMG))
	{
		gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
		ability = ABILITY(gActiveBattler);

		if (IS_BEHIND_SUBSTITUTE(gActiveBattler)
		&& gDisableStructs[gActiveBattler].substituteHP != 0
		&& !(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG))
		&& !gBattleStruct->bypassSubstitute)
		{
			PrepareStringBattle(STRINGID_SUBSTITUTEDAMAGED, gActiveBattler);
			if (IsDoubleSpreadMove())
				DoublesHPBarReduction();
		}
		#ifdef SPECIES_MIMIKYU
		else if (ability == ABILITY_DISGUISE
		&& (!(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG)) || gBattleStruct->breakDisguiseSpecialDmg)
		&& SPECIES(gActiveBattler) == SPECIES_MIMIKYU
		&& !IS_TRANSFORMED(gActiveBattler))
		{
			gBattleScripting.battler = gBattlerTarget;
			BattleScriptPush(gBattlescriptCurrInstr + 2);
			gBattlescriptCurrInstr = BattleScript_DisguiseTookDamage;
			if (IsDoubleSpreadMove())
				DoublesHPBarReduction();
			return;
		}
		#endif
		#ifdef SPECIES_EISCUE
		else if (ability == ABILITY_ICEFACE
		&& SPECIES(gActiveBattler) == SPECIES_EISCUE
		&& (!(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG)) || gBattleStruct->breakDisguiseSpecialDmg)
		&& SPLIT(gCurrentMove) == SPLIT_PHYSICAL //Only physical moves are stopped by the ice face
		&& !IS_TRANSFORMED(gActiveBattler))
		{
			gBattleScripting.battler = gBattlerTarget;
			BattleScriptPush(gBattlescriptCurrInstr + 2);
			gBattlescriptCurrInstr = BattleScript_IceFaceTookDamage;
			if (IsDoubleSpreadMove())
				DoublesHPBarReduction();
			return;
		}
		#endif
		else
		{
			if (!IsDoubleSpreadMove())
			{
				s32 healthValue, maxPossibleDmgValue;
				maxPossibleDmgValue = 30000; //Ensures that huge damage values don't change sign
			
				if (gBattleMoveDamage <= maxPossibleDmgValue)
					healthValue = gBattleMoveDamage;
				else
					healthValue = maxPossibleDmgValue;

				BtlController_EmitHealthBarUpdate(0, healthValue);
				MarkBattlerForControllerExec(gActiveBattler);

				if (SIDE(gActiveBattler) == B_SIDE_PLAYER && gBattleMoveDamage > 0)
					gBattleResults.playerMonWasDamaged = TRUE;
			}
			else
			{
				DOUBLES_HEALTH_REDUCE:
				DoublesHPBarReduction();
			}
		}
	}
	else if (IsDoubleSpreadMove())
		goto DOUBLES_HEALTH_REDUCE;

	gBattlescriptCurrInstr += 2;
}

static void atk0C_datahpupdate(void)
{
    if (gBattleControllerExecFlags) return;

	if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) || (gHitMarker & HITMARKER_NON_ATTACK_DMG))
	{
		gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

		//If Substitute Up
		if (gBattleMons[gActiveBattler].status2 & STATUS2_SUBSTITUTE
		&& gDisableStructs[gActiveBattler].substituteHP
		&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE)
		&& !gBattleStruct->bypassSubstitute)
		{
			if (gDisableStructs[gActiveBattler].substituteHP >= gBattleMoveDamage)
			{
				if (gSpecialStatuses[gActiveBattler].dmg == 0)
					gSpecialStatuses[gActiveBattler].dmg = gBattleMoveDamage;
				gDisableStructs[gActiveBattler].substituteHP -= gBattleMoveDamage;
				gHpDealt = gBattleMoveDamage;
			}
			else
			{
				if (gSpecialStatuses[gActiveBattler].dmg == 0)
					gSpecialStatuses[gActiveBattler].dmg = gDisableStructs[gActiveBattler].substituteHP;
				gHpDealt = gDisableStructs[gActiveBattler].substituteHP;
				gDisableStructs[gActiveBattler].substituteHP = 0;
			}

			gBattleStruct->AttackerDidDamageAtLeastOnce = TRUE;
			//Check Substitute Fading
			if (gDisableStructs[gActiveBattler].substituteHP == 0)
			{
				gBattlescriptCurrInstr += 2;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SubstituteFade;
				return;
			}
		}
		#ifdef SPECIES_MIMIKYU
		else if (ABILITY(gActiveBattler) == ABILITY_DISGUISE //Disguise Protected
		&& SPECIES(gActiveBattler) == SPECIES_MIMIKYU
		&& (!(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG)) || gBattleStruct->breakDisguiseSpecialDmg)
		&& !IS_TRANSFORMED(gActiveBattler))
		{
			gBattleMoveDamage = GetBaseCurrentHP(gActiveBattler) / 8;

			if (gSpecialStatuses[gActiveBattler].dmg == 0)
				gSpecialStatuses[gActiveBattler].dmg = 1;
			gHpDealt = gBattleMoveDamage;
			gBattleStruct->AttackerDidDamageAtLeastOnce = TRUE;
			gMoveResultFlags = 0;

			if (CalcMoveSplit(gActiveBattler, gCurrentMove) == SPLIT_PHYSICAL)
			{
				gProtectStructs[gActiveBattler].physicalDmg = gHpDealt;
				gSpecialStatuses[gActiveBattler].physicalDmg = gHpDealt;
				gProtectStructs[gActiveBattler].physicalBattlerId = gBattlerAttacker;
				gSpecialStatuses[gActiveBattler].physicalBattlerId = gBattlerAttacker;
			}
			else if (CalcMoveSplit(gActiveBattler, gCurrentMove) == SPLIT_SPECIAL)
			{
				gProtectStructs[gActiveBattler].specialDmg = gHpDealt;
				gSpecialStatuses[gActiveBattler].specialDmg = gHpDealt;
				gProtectStructs[gActiveBattler].specialBattlerId = gBattlerAttacker;
				gSpecialStatuses[gActiveBattler].specialBattlerId = gBattlerAttacker;
			}

			gBattleScripting.battler = gActiveBattler;
			DoFormChange(gActiveBattler, SPECIES_MIMIKYU_BUSTED, TRUE, FALSE, FALSE);
			gBattlescriptCurrInstr += 2;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_DisguiseTransform;
			return;
		}
		#endif
		#ifdef SPECIES_EISCUE
		else if (ABILITY(gActiveBattler) == ABILITY_ICEFACE //Disguise Protected
		&& SPECIES(gActiveBattler) == SPECIES_EISCUE
		&& SPLIT(gCurrentMove) == SPLIT_PHYSICAL //Only physical attacks break the ice
		&& (!(gHitMarker & (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_NON_ATTACK_DMG)) || gBattleStruct->breakDisguiseSpecialDmg)
		&& !IS_TRANSFORMED(gActiveBattler))
		{
			if (gSpecialStatuses[gActiveBattler].dmg == 0)
				gSpecialStatuses[gActiveBattler].dmg = 1;
			gHpDealt = 1;
			gBattleStruct->AttackerDidDamageAtLeastOnce = TRUE;
			gMoveResultFlags = 0;

			gProtectStructs[gActiveBattler].physicalDmg = gHpDealt;
			gSpecialStatuses[gActiveBattler].physicalDmg = gHpDealt;
			gProtectStructs[gActiveBattler].physicalBattlerId = gBattlerAttacker;
			gSpecialStatuses[gActiveBattler].physicalBattlerId = gBattlerAttacker;

			gBattleScripting.battler = gActiveBattler;
			DoFormChange(gActiveBattler, SPECIES_EISCUE_NOICE, TRUE, TRUE, FALSE);
			gBattlescriptCurrInstr += 2;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_IceFaceTransform;
			return;
		}
		#endif
		else //No Substitute
		{
			if (gBattleMoveDamage < 0) //HP goes up
			{
				gBattleMons[gActiveBattler].hp -= gBattleMoveDamage;
				if (gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP)
					gBattleMons[gActiveBattler].hp = gBattleMons[gActiveBattler].maxHP;

				gHitMarker &= ~(HITMARKER_IGNORE_SUBSTITUTE);
			}
			else //HP goes down
			{
                u32 hpDealt;
				if (gHitMarker & HITMARKER_x20)
					gHitMarker &= ~(HITMARKER_x20);

				else
				{
					gTakenDmg[gActiveBattler] += gBattleMoveDamage;
					if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
						gTakenDmgByBattler[gActiveBattler] = gBattlerAttacker;
					else
						gTakenDmgByBattler[gActiveBattler] = gBattlerTarget;
				}


				if (gBattleMons[gActiveBattler].hp > gBattleMoveDamage)
				{
					gBattleMons[gActiveBattler].hp -= gBattleMoveDamage;
					gHpDealt = gBattleMoveDamage;
				}
				else
				{
					gHpDealt = gBattleMons[gActiveBattler].hp;
					gBattleMons[gActiveBattler].hp = 0;
				}

				hpDealt = gHpDealt;
				if (IsRaidBattle() && gActiveBattler == BANK_RAID_BOSS && gBattleStruct->dynamaxData.raidShieldsUp)
					hpDealt = MathMax(1, hpDealt); //Because damage can get heavily reduced to 0
				if (!gSpecialStatuses[gActiveBattler].dmg && !(gHitMarker & HITMARKER_NON_ATTACK_DMG))
					gSpecialStatuses[gActiveBattler].dmg = hpDealt;

				if (CalcMoveSplit(gActiveBattler, gCurrentMove) == SPLIT_PHYSICAL
				&& !(gHitMarker & HITMARKER_NON_ATTACK_DMG))
				{
					gProtectStructs[gActiveBattler].physicalDmg = hpDealt;
					gSpecialStatuses[gActiveBattler].physicalDmg = hpDealt;

					if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
					{
						gProtectStructs[gActiveBattler].physicalBattlerId = gBattlerAttacker;
						gSpecialStatuses[gActiveBattler].physicalBattlerId = gBattlerAttacker;
						gBattleStruct->AttackerDidDamageAtLeastOnce = TRUE;
					}
					else
					{
						gProtectStructs[gActiveBattler].physicalBattlerId = gBattlerTarget;
						gSpecialStatuses[gActiveBattler].physicalBattlerId = gBattlerTarget;
					}
				}

				else if (CalcMoveSplit(gActiveBattler, gCurrentMove) == SPLIT_SPECIAL
				&& !(gHitMarker & HITMARKER_NON_ATTACK_DMG))
				{
					gProtectStructs[gActiveBattler].specialDmg = hpDealt;
					gSpecialStatuses[gActiveBattler].specialDmg = hpDealt;

					if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
					{
						gProtectStructs[gActiveBattler].specialBattlerId = gBattlerAttacker;
						gSpecialStatuses[gActiveBattler].specialBattlerId = gBattlerAttacker;
						gBattleStruct->AttackerDidDamageAtLeastOnce = TRUE;
					}
					else
					{
						gProtectStructs[gActiveBattler].specialBattlerId = gBattlerTarget;
						gSpecialStatuses[gActiveBattler].specialBattlerId = gBattlerTarget;
					}
				}

				if (gStatuses3[gActiveBattler] & STATUS3_ILLUSION
				&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE)
				&& gBattleMons[gActiveBattler].hp == 0)
				{
					gBattleScripting.battler = gActiveBattler;
					gStatuses3[gActiveBattler] &= ~(STATUS3_ILLUSION);
					BattleScriptPush(gBattlescriptCurrInstr + 2);
					gBattlescriptCurrInstr = BattleScript_IllusionBrokenFaint - 2;
				}

				gHitMarker &= ~(HITMARKER_IGNORE_SUBSTITUTE);
			}

			gHitMarker &= ~(HITMARKER_NON_ATTACK_DMG);
			BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
			MarkBattlerForControllerExec(gActiveBattler);
		}
	}
	else
	{
		gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
		if (gSpecialStatuses[gActiveBattler].dmg == 0)
			gSpecialStatuses[gActiveBattler].dmg = 0xFFFF;
	}
	gBattlescriptCurrInstr += 2;
}

static void atk0D_critmessage(void)
{
    u16 stringId = 0;

    if (gBattleControllerExecFlags)
		return;

    gBattleCommunication[MSG_DISPLAY] = 0;
	if (gCritMultiplier > BASE_CRIT_MULTIPLIER)
	{
		if (IsDoubleSpreadMove())
		{
			if (!(gBattleStruct->ResultFlags[gBattlerTarget] & MOVE_RESULT_NO_EFFECT) && !gBattleStruct->noResultString[gBattlerTarget])
			{
				stringId = STRINGID_CUSTOMSTRING;
				if (gBattlerTarget == FOE(gBattlerAttacker)
				&& !(gBattleStruct->ResultFlags[PARTNER(gBattlerTarget)] & MOVE_RESULT_NO_EFFECT)
				&& !gBattleStruct->noResultString[PARTNER(gBattlerTarget)]
				&& gBattleStruct->criticalMultiplier[PARTNER(gBattlerTarget)] > BASE_CRIT_MULTIPLIER)
					gBattleStringLoader = gText_CriticalHitTwoFoes;
				else if (gBattlerTarget == PARTNER(FOE(gBattlerAttacker))
				&& !(gBattleStruct->ResultFlags[FOE(gBattlerAttacker)] & MOVE_RESULT_NO_EFFECT)
				&& !gBattleStruct->noResultString[FOE(gBattlerAttacker)]
				&& gBattleStruct->criticalMultiplier[FOE(gBattlerAttacker)] > BASE_CRIT_MULTIPLIER)
				{
					//Was handled or will be handled as a double string
					stringId = 0;
				}
				else
					gBattleStringLoader = gText_CriticalHitTarget;
			}
		}
		else if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
			stringId = STRINGID_CRITICALHIT;
	
		if (stringId != 0)
		{
			PrepareStringBattle(stringId, gBattlerAttacker);
			gBattleCommunication[MSG_DISPLAY] = 1;
		}
	}

	gBattlescriptCurrInstr++;
}

static void atk0E_effectivenesssound(void)
{
    u8 resultFlags = gMoveResultFlags;

    if (gBattleControllerExecFlags)
		return;

	if (IsDoubleSpreadMove())
	{
		if (gBattleStruct->doneDoublesSpreadHit
		|| !gBattleStruct->calculatedSpreadMoveData //The attack animation didn't play yet - only play sound after animation
		|| SPLIT(gCurrentMove) == SPLIT_STATUS) //To handle Dark Void missing basically
			goto END;
		resultFlags = UpdateEffectivenessResultFlagsForDoubleSpreadMoves(resultFlags);
	}
	else if (!(resultFlags & MOVE_RESULT_NO_EFFECT) && DoesBankNegateDamage(gBattlerTarget, gCurrentMove)) //Mimikyu disguised
		resultFlags = 0;

	gActiveBattler = gBattlerTarget;
	if (!(resultFlags & MOVE_RESULT_MISSED))
	{
		switch (resultFlags & (u8)(~(MOVE_RESULT_MISSED))) {
			case MOVE_RESULT_SUPER_EFFECTIVE:
				BtlController_EmitPlaySE(0, SE_SUPER_EFFECTIVE);
				MarkBattlerForControllerExec(gActiveBattler);
				break;
			case MOVE_RESULT_NOT_VERY_EFFECTIVE:
				BtlController_EmitPlaySE(0, SE_NOT_EFFECTIVE);
				MarkBattlerForControllerExec(gActiveBattler);
				break;
			case MOVE_RESULT_DOESNT_AFFECT_FOE:
			case MOVE_RESULT_FAILED:
				//No sound
				break;
			case MOVE_RESULT_FOE_ENDURED:
			case MOVE_RESULT_ONE_HIT_KO:
			case MOVE_RESULT_FOE_HUNG_ON:
			default:
				if (resultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
				{
					BtlController_EmitPlaySE(0, SE_SUPER_EFFECTIVE);
					MarkBattlerForControllerExec(gActiveBattler);
				}
				else if (resultFlags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
				{
					BtlController_EmitPlaySE(0, SE_NOT_EFFECTIVE);
					MarkBattlerForControllerExec(gActiveBattler);
				}
				else if (!(resultFlags & (MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED)))
				{
					BtlController_EmitPlaySE(0, SE_EFFECTIVE);
					MarkBattlerForControllerExec(gActiveBattler);
				}
				break;
			}
	}

END:
	gBattlescriptCurrInstr++;
}

static bool8 ShouldPrintTwoFoesMessage(u8 moveResult)
{
	return gBattlerTarget == FOE(gBattlerAttacker)
		&& gBattleStruct->ResultFlags[PARTNER(gBattlerTarget)] & moveResult
		&& !gBattleStruct->noResultString[PARTNER(gBattlerTarget)];
}

static bool8 ShouldRelyOnTwoFoesMessage(u8 moveResult)
{
	return gBattlerTarget == PARTNER(FOE(gBattlerAttacker))
		&& gBattleStruct->ResultFlags[FOE(gBattlerAttacker)] & moveResult
		&& !(gBattleStruct->ResultFlags[FOE(gBattlerAttacker)] & MOVE_RESULT_MISSED && gBattleStruct->missStringId[FOE(gBattlerAttacker)] > 2) //Partner was missed
		&& !gBattleStruct->noResultString[FOE(gBattlerAttacker)];
}

static void atk0F_resultmessage(void)
{
    u32 stringId = 0;

	if (gBattleControllerExecFlags) return;

	if (gMoveResultFlags & MOVE_RESULT_MISSED && (!(gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE) || gBattleStruct->missStringId[gBattlerTarget] > 2))
	{
		gBattleStruct->MetronomeCounter[gBattlerAttacker] = 0;

		if (gBattleStruct->missStringId[gBattlerTarget] == 3 || gBattleStruct->missStringId[gBattlerTarget] == 4) //Levitate + Wonder Guard
		{
			BattleScriptPush(gBattlescriptCurrInstr + 1);
			gBattleScripting.battler = gBattlerTarget;
			gBattlescriptCurrInstr = BattleScript_AvoidedMoveWithAbility;
			gBattleCommunication[MSG_DISPLAY] = 0;
			return;
		}
		else
		{
			stringId = gMissStringIds[gBattleStruct->missStringId[gBattlerTarget]];
			gBattleCommunication[MSG_DISPLAY] = 1;
		}
	}
	else
	{
		gBattleCommunication[MSG_DISPLAY] = 1;
		switch (gMoveResultFlags & (u8)(~(MOVE_RESULT_MISSED))) {
		case MOVE_RESULT_SUPER_EFFECTIVE:
			if (IsDoubleSpreadMove())
			{
				stringId = STRINGID_CUSTOMSTRING;
				if (ShouldPrintTwoFoesMessage(MOVE_RESULT_SUPER_EFFECTIVE))
					gBattleStringLoader = gText_SuperEffectiveTwoFoes;
				else if (ShouldRelyOnTwoFoesMessage(MOVE_RESULT_SUPER_EFFECTIVE))
				{
					//Was handled or will be handled as a double string
					stringId = 0;
				}
				else
					gBattleStringLoader = gText_SuperEffectiveTarget;
			}
			else
				stringId = STRINGID_SUPEREFFECTIVE;
			break;

		case MOVE_RESULT_NOT_VERY_EFFECTIVE:
			if (IsDoubleSpreadMove())
			{
				stringId = STRINGID_CUSTOMSTRING;
				if (ShouldPrintTwoFoesMessage(MOVE_RESULT_NOT_VERY_EFFECTIVE))
					gBattleStringLoader = gText_NotVeryEffectiveTwoFoes;
				else if (ShouldRelyOnTwoFoesMessage(MOVE_RESULT_NOT_VERY_EFFECTIVE))
				{
					//Was handled or will be handled as a double string
					stringId = 0;
				}
				else
					gBattleStringLoader = gText_NotVeryEffectiveTarget;
			}
			else
				stringId = STRINGID_NOTVERYEFFECTIVE;
			break;

		case MOVE_RESULT_ONE_HIT_KO:
			stringId = STRINGID_ONEHITKO;
			break;

		case MOVE_RESULT_FOE_ENDURED:
			if (gBattleStruct->EnduranceHelper[gBattlerTarget] == ENDURE_STURDY)
			{
				gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED);
				gBattleStruct->EnduranceHelper[gBattlerTarget] = 0;
				gProtectStructs[gBattlerTarget].enduredSturdy = 0;
				gBattleScripting.battler = gBattlerTarget;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_EnduredSturdy;
				return;
			}
			else
				stringId = STRINGID_PKMNENDUREDHIT;
			break;

		case MOVE_RESULT_FAILED:
			gBattleStruct->MetronomeCounter[gBattlerAttacker] = 0;
			stringId = STRINGID_BUTITFAILED;
			break;

		case MOVE_RESULT_DOESNT_AFFECT_FOE:
			if (IsDoubleSpreadMove())
			{
				stringId = STRINGID_CUSTOMSTRING;
				if (ShouldPrintTwoFoesMessage(MOVE_RESULT_DOESNT_AFFECT_FOE))
				{
					gBattleStruct->MetronomeCounter[gBattlerAttacker] = 0;
					gBattleStringLoader = gText_DoesntAffectTwoFoes;
				}
				else if (ShouldRelyOnTwoFoesMessage(MOVE_RESULT_DOESNT_AFFECT_FOE))
				{
					//Was handled or will be handled as a double string
					stringId = 0;
				}
				else
					stringId = STRINGID_ITDOESNTAFFECT;
			}
			else
			{
				gBattleStruct->MetronomeCounter[gBattlerAttacker] = 0;
				stringId = STRINGID_ITDOESNTAFFECT;
			}
			break;

		case MOVE_RESULT_FOE_HUNG_ON:
			gLastUsedItem = ITEM(gBattlerTarget);
			gPotentialItemEffectBattler = gBattlerTarget;
			gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
			BattleScriptPushCursor();
			if (gBattleStruct->EnduranceHelper[gBattlerTarget] == ENDURE_FOCUS_SASH)
			{
				gBattleStruct->EnduranceHelper[gBattlerTarget] = 0;
				gBattlescriptCurrInstr = BattleScript_HangedOnFocusSash;
			}
			else
				gBattlescriptCurrInstr = BattleScript_HangedOnMsg;
			return;

		default:
			if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
			{
				if (IsDoubleSpreadMove())
				{
					stringId = STRINGID_CUSTOMSTRING;
					if (ShouldPrintTwoFoesMessage(MOVE_RESULT_DOESNT_AFFECT_FOE))
					{
						gBattleStruct->MetronomeCounter[gBattlerAttacker] = 0;
						gBattleStringLoader = gText_DoesntAffectTwoFoes;
					}
					else if (ShouldRelyOnTwoFoesMessage(MOVE_RESULT_DOESNT_AFFECT_FOE))
					{
						//Was handled or will be handled as a double string
						stringId = 0;
					}
					else
						stringId = STRINGID_ITDOESNTAFFECT;
				}
				else
				{
					gBattleStruct->MetronomeCounter[gBattlerAttacker] = 0;
					stringId = STRINGID_ITDOESNTAFFECT;
				}
			}
			else if (gMoveResultFlags & MOVE_RESULT_ONE_HIT_KO)
			{
				gMoveResultFlags &= ~(MOVE_RESULT_ONE_HIT_KO);
				gMoveResultFlags &= ~(MOVE_RESULT_SUPER_EFFECTIVE);
				gMoveResultFlags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE);
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_OneHitKOMsg;
				return;
			}
			else if (gMoveResultFlags & MOVE_RESULT_FOE_ENDURED)
			{
				gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
				BattleScriptPushCursor();
				if (gBattleStruct->EnduranceHelper[gBattlerTarget] == ENDURE_STURDY)
				{
					gBattleStruct->EnduranceHelper[gBattlerTarget] = 0;
					gProtectStructs[gBattlerTarget].enduredSturdy = 0;
					gBattleScripting.battler = gBattlerTarget;
					gBattlescriptCurrInstr = BattleScript_EnduredSturdy;
				}
				else
					gBattlescriptCurrInstr = BattleScript_EnduredMsg;
				return;
			}
			else if (gMoveResultFlags & MOVE_RESULT_FOE_HUNG_ON)
			{
				gLastUsedItem = gBattleMons[gBattlerTarget].item;
				gPotentialItemEffectBattler = gBattlerTarget;
				gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
				BattleScriptPushCursor();
				if (gBattleStruct->EnduranceHelper[gBattlerTarget] == ENDURE_FOCUS_SASH)
				{
					gBattleStruct->EnduranceHelper[gBattlerTarget] = 0;
					gBattlescriptCurrInstr = BattleScript_HangedOnFocusSash;
				}
				else
					gBattlescriptCurrInstr = BattleScript_HangedOnMsg;

				gSpecialStatuses[gBattlerTarget].focusBanded = FALSE;
				return;
			}
			else if (gMoveResultFlags & MOVE_RESULT_FAILED)
			{
				gBattleStruct->MetronomeCounter[gBattlerAttacker] = 0;
				stringId = STRINGID_BUTITFAILED;
			}
			else
			{
				gBattleCommunication[MSG_DISPLAY] = 0;
			}
		}

		gBattleStruct->EnduranceHelper[gBattlerTarget] = 0; //Clear these here for multi-hit moves that didn't KO target
		gSpecialStatuses[gBattlerTarget].focusBanded = FALSE;
		gProtectStructs[gBattlerTarget].enduredSturdy = FALSE;
	}

	if (stringId != 0)
		PrepareStringBattle(stringId, gBattlerAttacker);
	else
		gBattleCommunication[MSG_DISPLAY] = 0;

	gBattlescriptCurrInstr++;

	if ((gBattleStruct->zMoveData.active || IsAnyMaxMove(gCurrentMove))
	&& !IsDynamaxed(gBattlerTarget) && ProtectsAgainstZMoves(gCurrentMove, gBattlerAttacker, gBattlerTarget)
	&& !(gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE))
	{
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_CouldntFullyProtect;
	}
}

static void atk10_printstring(void)
{
    if (!gBattleControllerExecFlags)
    {
        u16 var = T2_READ_16(gBattlescriptCurrInstr + 1);

        PrepareStringBattle(var, gBattlerAttacker);
        gBattlescriptCurrInstr += 3;
        gBattleCommunication[MSG_DISPLAY] = 1;
    }
}

static void atk11_printselectionstring(void)
{
    gActiveBattler = gBattlerAttacker;
    BtlController_EmitPrintSelectionString(0, T2_READ_16(gBattlescriptCurrInstr + 1));
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 3;
    gBattleCommunication[MSG_DISPLAY] = 1;
}

static void atk12_waitmessage(void)
{
    if (!gBattleControllerExecFlags)
    {
        if (!gBattleCommunication[MSG_DISPLAY])
		{
			gBattlescriptCurrInstr += 3;
		}
		else
		{
			u16 toWait = T2_READ_16(gBattlescriptCurrInstr + 1);
			if (++gPauseCounterBattle >= toWait
			#ifdef FLAG_FAST_BATTLE_MESSAGES
			|| FlagGet(FLAG_FAST_BATTLE_MESSAGES)
			#endif
			|| gMain.newKeys & A_BUTTON)
			{
				gPauseCounterBattle = 0;
				gBattlescriptCurrInstr += 3;
				gBattleCommunication[MSG_DISPLAY] = 0;
			}
		}
    }
}

static void atk13_printfromtable(void)
{
    u16 stringId;
    u16* ptr;

	if (gBattleControllerExecFlags) return;

	ptr = (u16*)T1_READ_PTR(gBattlescriptCurrInstr + 1);
	ptr += gBattleCommunication[MULTISTRING_CHOOSER];
	stringId = *(u16*)ptr;
	PrepareStringBattle(stringId, gBattlerAttacker);
	gBattlescriptCurrInstr += 5;
	gBattleCommunication[MSG_DISPLAY] = 1;

	if (stringId == STRINGID_PKMNSSTATCHANGED4)
	{	
		if (gBattleStruct->intimidateActive || SIDE(gBattlerTarget) != SIDE(gBattlerAttacker))
			DefiantActivation(); //Stat Fell From Enemy
	}
}

static void atk14_printselectionstringfromtable(void)
{
    if (!gBattleControllerExecFlags)
    {
        const u16 *ptr = (const u16 *) T1_READ_PTR(gBattlescriptCurrInstr + 1);

        ptr += gBattleCommunication[MULTISTRING_CHOOSER];
        gActiveBattler = gBattlerAttacker;
        BtlController_EmitPrintSelectionString(0, *ptr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 5;
        gBattleCommunication[MSG_DISPLAY] = 1;
    }
}

u8 GetBattlerTurnOrderNum(u8 battlerId)
{
    s32 i;
    
    for (i = 0; i < gBattlersCount && gBattlerByTurnOrder[i] != battlerId; ++i);
    return i;
}

void SetMoveEffect(bool8 primary, u8 certain)
{
    bool8 statusChanged = FALSE;
	u8 affectsUser = 0; // 0x40 otherwise
	u8 flags = 0;
	bool8 mirrorArmorReflected = ABILITY(gBattlerTarget) == ABILITY_MIRRORARMOR;

	if (gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_AFFECTS_USER)
	{
		gEffectBattler = gBattlerAttacker; // battlerId that effects get applied on
		gBattleCommunication[MOVE_EFFECT_BYTE] &= ~(MOVE_EFFECT_AFFECTS_USER);
		affectsUser = MOVE_EFFECT_AFFECTS_USER;
		gBattleScripting.battler = gBattlerAttacker;
	}
	else
	{
		gEffectBattler = gBattlerTarget;
		gBattleScripting.battler = gBattlerAttacker;
	}

	if (ABILITY(gEffectBattler) == ABILITY_SHIELDDUST
	&& !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) //Used by contact abilities and synchronize
	&& !primary
	&& CheckTableForAbility(gBattleCommunication[MOVE_EFFECT_BYTE], sShieldDustIgnoredEffects))
	{
		++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

	if (BankSideHasSafeguard(gEffectBattler)
	&& !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
	&& ABILITY(gBattlerAttacker) != ABILITY_INFILTRATOR
	&& gEffectBattler != gBattlerAttacker
	&& !primary
	&& certain != MOVE_EFFECT_CERTAIN
	&& gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_CONFUSION)
	{
		++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

	if (IsOfType(gEffectBattler, TYPE_GRASS)
	&& (ABILITY(gEffectBattler) == ABILITY_FLOWERVEIL || (IS_DOUBLE_BATTLE && ABILITY(PARTNER(gEffectBattler)) == ABILITY_FLOWERVEIL))
	&& gEffectBattler != gBattlerAttacker
	&& (gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_CONFUSION
	 || (gBattleCommunication[MOVE_EFFECT_BYTE] >= MOVE_EFFECT_ATK_MINUS_1 && gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_EVS_MINUS_1)
	 || (gBattleCommunication[MOVE_EFFECT_BYTE] >= MOVE_EFFECT_ATK_MINUS_2 && gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_EVS_MINUS_2)))
	{
		++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

	if (CheckTableForAbility(gBattleCommunication[MOVE_EFFECT_BYTE], sMoveEffectsThatIgnoreSubstitute))
		goto SKIP_SUBSTITUTE_CHECK;

	if (!BATTLER_ALIVE(gEffectBattler))
	{
		++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

	if (MoveBlockedBySubstitute(gCurrentMove, gBattleScripting.battler, gEffectBattler)
	&& affectsUser != MOVE_EFFECT_AFFECTS_USER
	&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
	{
		++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

	if (gBattleStruct->dynamaxData.raidShieldsUp
	&& affectsUser != MOVE_EFFECT_AFFECTS_USER
	&& gEffectBattler == BANK_RAID_BOSS
	&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
	{
		++gBattlescriptCurrInstr;
		goto CLEAR_MOVE_EFFECT_BYTE;
	}

	SKIP_SUBSTITUTE_CHECK:
	if (gBattleCommunication[MOVE_EFFECT_BYTE] <= 6) // status change
	{
		switch (sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]]) {
			case STATUS1_SLEEP:
				// check active uproar
				if (CanBePutToSleep(gEffectBattler, FALSE)) //Flower Veil & Safeguard checked earlier
				{
					CancelMultiTurnMoves(gEffectBattler);
					statusChanged = TRUE;
				}
			break;

			case STATUS1_POISON:
			case STATUS1_TOXIC_POISON:
				if (CanBePoisoned(gEffectBattler, gBattlerAttacker, FALSE)) //Flower Veil & Safeguard checked earlier
					statusChanged = TRUE;
				break;

			case STATUS1_BURN:
				if (CanBeBurned(gEffectBattler, FALSE)) //Flower Veil & Safeguard checked earlier
					statusChanged = TRUE;
				break;

			case STATUS1_FREEZE:
				if (CanBeFrozen(gEffectBattler, FALSE)) //Flower Veil & Safeguard checked earlier
				{
					CancelMultiTurnMoves(gEffectBattler);
					statusChanged = TRUE;
				}
				break;

			case STATUS1_PARALYSIS:
				if (CanBeParalyzed(gEffectBattler, FALSE)) //Flower Veil & Safeguard checked earlier
					statusChanged = TRUE;
				break;
		}

		if (statusChanged)
		{
			BattleScriptPush(gBattlescriptCurrInstr + 1);

			if (gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_TOXIC && IsDynamaxed(gEffectBattler))
				gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_POISON; //Toxic becomes regular poison on a Dynamaxed opponent

			if (gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_SLEEP)
				gBattleMons[gEffectBattler].status1 |= ((Random() % 3) + 2);
			else
				gBattleMons[gEffectBattler].status1 |= sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]];

			gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];

			gActiveBattler = gEffectBattler;
			BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gEffectBattler].status1);
			MarkBattlerForControllerExec(gActiveBattler);

			/*if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = 1;
				gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);
			}
			else
			{*/
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			//}

			if (gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_POISON
			|| gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_TOXIC
			|| gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_PARALYSIS
			|| gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_BURN
			|| gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_TOXIC)
			{
				if (gPoisonedBy != 0x2 && gPoisonedBy != 0x3) //Not Toxic Spikes or Toxic Orb / Flame Orb
				{
					gBattleStruct->synchronizeMoveEffect = gBattleCommunication[MOVE_EFFECT_BYTE];
					gBattleStruct->synchronizeTarget[gEffectBattler] = gBattlerAttacker + 1;
				}
			}
			return;
		}
		else
		{
			gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
			gBattlescriptCurrInstr++;
			return;
		}
		return;
	}
	else
	{
		if (gBattleCommunication[MOVE_EFFECT_BYTE] < NELEMS(sStatusFlagsForMoveEffects) //Error prevention
		&& gBattleMons[gEffectBattler].status2 & sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]])
		{
			gBattlescriptCurrInstr++;
		}
		else
		{
			switch (gBattleCommunication[MOVE_EFFECT_BYTE])
			{
			case MOVE_EFFECT_CONFUSION:
				if (CanBeConfused(gEffectBattler, FALSE)) //Safeguard checked earlier
				{
					gBattleMons[gEffectBattler].status2 |= ((Random() % 4)) + 2;

					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
				}
				else
					gBattlescriptCurrInstr++;
				break;

			case MOVE_EFFECT_FLINCH:
				if (ABILITY(gEffectBattler) == ABILITY_INNERFOCUS || IsDynamaxed(gEffectBattler))
					gBattlescriptCurrInstr++;
				else
				{
					if (gCurrentTurnActionNumber < GetBattlerTurnOrderNum(gEffectBattler))
						gBattleMons[gEffectBattler].status2 |= sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]];
					gBattlescriptCurrInstr++;
				}
				break;

			case MOVE_EFFECT_UPROAR:
				if (!(gBattleMons[gEffectBattler].status2 & STATUS2_UPROAR))
				{
					gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
					gLockedMoves[gEffectBattler] = gCurrentMove;
					gBattleMons[gEffectBattler].status2 |= ((Random() & 3) + 2) << 4;

					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
				}
				else
					gBattlescriptCurrInstr++;

				break;

			case MOVE_EFFECT_PAYDAY:
				if (!gBattleStruct->secondaryEffectApplied)
				{
					gBattleStruct->secondaryEffectApplied = TRUE;
					if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
					{
						if (IsAnyMaxMove(gCurrentMove) && gBattleMoves[gCurrentMove].z_move_effect == MAX_EFFECT_CONFUSE_FOES_PAY_DAY)
							gBattleStruct->maxGoldrushMoney += (100 * ++gBattleStruct->maxGoldrushUses * gBattleMons[gBattlerAttacker].level);
						else
							gBattleStruct->PayDayByPartyIndices[gBattlerPartyIndexes[gBattlerAttacker]]++;
					}
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
				}
				break;

			case MOVE_EFFECT_TRI_ATTACK:
				if (gBattleMons[gEffectBattler].status1)
				{
					gBattlescriptCurrInstr++;
				}
				else
				{
					gBattleCommunication[MOVE_EFFECT_BYTE] = (Random() % 3) + 3;
					SetMoveEffect(FALSE, 0);
				}
				break;

			case MOVE_EFFECT_CHARGING:
				gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
				gLockedMoves[gEffectBattler] = gCurrentMove;
				gProtectStructs[gEffectBattler].chargingTurn = 1;
				gBattleStruct->metronomeItemBonus[gEffectBattler] = 2;
				gBattlescriptCurrInstr++;
				break;

			case MOVE_EFFECT_WRAP:
				if (gBattleMons[gEffectBattler].status2 & STATUS2_WRAPPED || gBattleStruct->secondaryEffectApplied)
				{
					gBattlescriptCurrInstr++;
				}
				else
				{
					u16 trappingMove = gCurrentMove;
					gBattleStruct->secondaryEffectApplied = TRUE;

					if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_GRIP_CLAW)
						gBattleMons[gEffectBattler].status2 |= (7 << 0xD);
					else
						gBattleMons[gEffectBattler].status2 |= ((Random() & 1) + 4) << 0xD;

					//Turn G-Max Moves into their corresponding trapping moves
					switch (trappingMove) {
						case MOVE_G_MAX_CENTIFERNO_P:
						case MOVE_G_MAX_CENTIFERNO_S:
							trappingMove = MOVE_FIRESPIN;
							break;
						case MOVE_G_MAX_SANDBLAST_P:
						case MOVE_G_MAX_SANDBLAST_S:
							trappingMove = MOVE_SANDTOMB;
							break;
					}

					gBattleStruct->wrappedMove[gEffectBattler] = trappingMove;
					gBattleStruct->wrappedBy[gEffectBattler] = gBattlerAttacker;

					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];

					for (gBattleCommunication[MULTISTRING_CHOOSER] = 0; ; gBattleCommunication[MULTISTRING_CHOOSER]++)
					{
						if (gBattleCommunication[MULTISTRING_CHOOSER] > 6)
							break;
						if (gTrappingMoves[gBattleCommunication[MULTISTRING_CHOOSER]] == trappingMove)
							break;
					}

					switch (gCurrentMove) {
						case MOVE_INFESTATION:
							gBattleStringLoader = gText_TargetWasInfested;
							break;
						case MOVE_SNAPTRAP:
							gBattleStringLoader = gText_TargetWasCaughtInSnapTrap;
							break;
						case MOVE_G_MAX_CENTIFERNO_P:
						case MOVE_G_MAX_CENTIFERNO_S:
						case MOVE_G_MAX_SANDBLAST_P:
						case MOVE_G_MAX_SANDBLAST_S:
							gBattleStruct->sandblastCentiferno[gEffectBattler] = 1; //This condition doesn't get removed when the attacker leaves the field
							if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_GRIP_CLAW)
								gBattleStruct->sandblastCentiferno[gEffectBattler] |= 2;
							break;
					}
				}
				break;

			case MOVE_EFFECT_ATK_PLUS_1:
			case MOVE_EFFECT_DEF_PLUS_1:
			case MOVE_EFFECT_SPD_PLUS_1:
			case MOVE_EFFECT_SP_ATK_PLUS_1:
			case MOVE_EFFECT_SP_DEF_PLUS_1:
			case MOVE_EFFECT_ACC_PLUS_1:
			case MOVE_EFFECT_EVS_PLUS_1:
				if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(1),
									gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_PLUS_1 + 1,
									affectsUser | certain, 0))
				{
					gBattlescriptCurrInstr++;
				}
				else
				{
					gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
					gBattleScripting.animArg2 = 0;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_StatUp;
				}
				break;

			case MOVE_EFFECT_ATK_MINUS_1:
			case MOVE_EFFECT_DEF_MINUS_1:
			case MOVE_EFFECT_SPD_MINUS_1:
			case MOVE_EFFECT_SP_ATK_MINUS_1:
			case MOVE_EFFECT_SP_DEF_MINUS_1:
			case MOVE_EFFECT_ACC_MINUS_1:
			case MOVE_EFFECT_EVS_MINUS_1:
				flags = affectsUser | certain;
				if (mirrorArmorReflected && !affectsUser)
				{
					gBattleScripting.statChanger = DECREASE_1 | (gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_1 + 1);
					flags |= STAT_CHANGE_BS_PTR;
				}

				if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(1) | STAT_BUFF_NEGATIVE,
									gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_1 + 1,
									flags, gBattlescriptCurrInstr + 1))
				{
					if (!mirrorArmorReflected)
						gBattlescriptCurrInstr++;
				}
				else
				{
					gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
					gBattleScripting.animArg2 = 0;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_StatDown;
				}
				break;

			case MOVE_EFFECT_ATK_PLUS_2:
			case MOVE_EFFECT_DEF_PLUS_2:
			case MOVE_EFFECT_SPD_PLUS_2:
			case MOVE_EFFECT_SP_ATK_PLUS_2:
			case MOVE_EFFECT_SP_DEF_PLUS_2:
			case MOVE_EFFECT_ACC_PLUS_2:
			case MOVE_EFFECT_EVS_PLUS_2:
				if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(2),
									gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_PLUS_2 + 1,
									affectsUser | certain, 0))
				{
					gBattlescriptCurrInstr++;
				}
				else
				{
					gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
					gBattleScripting.animArg2 = 0;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_StatUp;
				}
				break;

			case MOVE_EFFECT_ATK_MINUS_2:
			case MOVE_EFFECT_DEF_MINUS_2:
			case MOVE_EFFECT_SPD_MINUS_2:
			case MOVE_EFFECT_SP_ATK_MINUS_2:
			case MOVE_EFFECT_SP_DEF_MINUS_2:
			case MOVE_EFFECT_ACC_MINUS_2:
			case MOVE_EFFECT_EVS_MINUS_2:
				flags = affectsUser | certain;
				if (mirrorArmorReflected && !affectsUser)
				{
					gBattleScripting.statChanger = DECREASE_2 | (gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_2 + 1);
					flags |= STAT_CHANGE_BS_PTR;
				}

				if (ChangeStatBuffs(SET_STAT_BUFF_VALUE(2) | STAT_BUFF_NEGATIVE,
									gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_2 + 1,
									flags, gBattlescriptCurrInstr + 1))
				{
					if (!mirrorArmorReflected)
						gBattlescriptCurrInstr++;
				}
				else
				{
					gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
					gBattleScripting.animArg2 = 0;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_StatDown;
				}
				break;

			case MOVE_EFFECT_RECHARGE:
				gBattleMons[gEffectBattler].status2 |= STATUS2_RECHARGE;
				gDisableStructs[gEffectBattler].rechargeTimer = 2;
				gLockedMoves[gEffectBattler] = gCurrentMove;
				gBattlescriptCurrInstr++;
				break;

			case MOVE_EFFECT_RAGE:
				gBattleMons[gBattlerAttacker].status2 |= STATUS2_RAGE;
				gBattlescriptCurrInstr++;
				break;

			case MOVE_EFFECT_PREVENT_ESCAPE:
				if (!IsOfType(gEffectBattler, TYPE_GHOST) && !(gBattleMons[gEffectBattler].status2 & STATUS2_ESCAPE_PREVENTION))
				{
					gBattleMons[gEffectBattler].status2 |= STATUS2_ESCAPE_PREVENTION;
					gDisableStructs[gEffectBattler].battlerPreventingEscape = gBattlerAttacker;

					if (gCurrentMove == MOVE_OCTOLOCK)
						gBattleStruct->trappedByOctolock |= gBitTable[gEffectBattler];
					else if (gCurrentMove == MOVE_NORETREAT)
						gBattleStruct->trappedByNoRetreat |= gBitTable[gEffectBattler];
					else
						gBattleStruct->trappedByOctolock &= ~(gBitTable[gEffectBattler]);
					gBattlescriptCurrInstr++;
				}
				break;

			case MOVE_EFFECT_NIGHTMARE:
				if (gBattleMons[gEffectBattler].status1 & STATUS1_SLEEP)
				{
					gBattleMons[gEffectBattler].status2 |= STATUS2_NIGHTMARE;
					gBattlescriptCurrInstr++;
				}
				break;

			case MOVE_EFFECT_ALL_STATS_UP:
				if (gCurrentMove != MOVE_CLANGOROUS_SOULBLAZE || !gBattleStruct->secondaryEffectApplied) //Hits two targets but only gets effect once
				{
					gBattleStruct->secondaryEffectApplied = TRUE;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_AllStatsUp;
				}
				break;

			case MOVE_EFFECT_RAPIDSPIN:
				BattleScriptPush(gBattlescriptCurrInstr + 1);
				gBattlescriptCurrInstr = BattleScript_RapidSpinAway;
				break;

			case MOVE_EFFECT_ATK_DEF_DOWN: // SuperPower
				BattleScriptPush(gBattlescriptCurrInstr + 1);
				gBattlescriptCurrInstr = BattleScript_AtkDefDown;
				break;

			case MOVE_EFFECT_THRASH:
				if (gBattleStruct->DancerInProgress
				|| gBattleMons[gEffectBattler].status2 & STATUS2_LOCK_CONFUSE)
				{
					gBattlescriptCurrInstr++;
				}
				else
				{
					gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
					gLockedMoves[gEffectBattler] = gCurrentMove;
					gBattleMons[gEffectBattler].status2 |= (((Random() & 1) + 2) << 0xA);
				}
				break;

			case MOVE_EFFECT_SP_ATK_TWO_DOWN: // Overheat
				BattleScriptPush(gBattlescriptCurrInstr + 1);
				gBattlescriptCurrInstr = BattleScript_SAtkDown2;
				break;

			case MOVE_EFFECT_BURN_BERRY:
				if ((IsBerry(ITEM(gEffectBattler)) || ITEM_EFFECT(gEffectBattler) == HOLD_EFFECT_GEM)
				&&  ABILITY(gEffectBattler) != ABILITY_STICKYHOLD)
				{
					gBattleStruct->IncinerateCounters[gEffectBattler] = TRUE;

					gLastUsedItem = gBattleMons[gEffectBattler].item;
					gBattleMons[gEffectBattler].item = 0;
					HandleUnburdenBoost(gEffectBattler); //Give target Unburden boost

					gActiveBattler = gEffectBattler;
					BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
					MarkBattlerForControllerExec(gActiveBattler);

					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattleStringLoader = RoastedBerryString;
					gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				}
				else
					gBattlescriptCurrInstr++;

				break;

			case MOVE_EFFECT_EAT_BERRY: //For Stuff Cheeks
				if (IsBerry(ITEM(gEffectBattler))
				&& !CheckTableForItem(ITEM(gEffectBattler), gBannedBattleEatBerries)
				&& BATTLER_ALIVE(gEffectBattler)
				&& gEffectBattler == gBattlerAttacker) //Don't do Pluck here
				{
					gLastUsedItem = ITEM(gEffectBattler);
					if (!ItemBattleEffects(ItemEffects_EndTurn, gBattlerAttacker, TRUE, TRUE))
					{
						//The Berry didn't activate an effect
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_EatEffectBankBerry;
					}
				}
				break;

			case MOVE_EFFECT_RESET_STAT_CHANGES:
            {
                int i;
				for (i = 0; i < BATTLE_STATS_NO-1; ++i)
					gBattleMons[gEffectBattler].statStages[i] = 6;

				BattleScriptPush(gBattlescriptCurrInstr + 1);
				gBattleStringLoader = TargetStatsResetString;
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
            }
				break;

			case MOVE_EFFECT_SUPPRESS_ABILITY:
				if (!gBattleStruct->secondaryEffectApplied
				&& !IsAbilitySuppressed(gEffectBattler)
				&&  GetBattlerTurnOrderNum(gEffectBattler) < gCurrentTurnActionNumber) //Target moved before attacker
				{
                    ability_t* defAbilityLoc;
					gBattleStruct->secondaryEffectApplied = TRUE;
					defAbilityLoc = GetAbilityLocation(gBattlerTarget);

					gStatuses3[gEffectBattler] |= STATUS3_ABILITY_SUPPRESS;
					gBattleStruct->SuppressedAbilities[gEffectBattler] = *defAbilityLoc;
					*defAbilityLoc = 0;

					gBattleStruct->backupAbility = gBattleStruct->SuppressedAbilities[gEffectBattler];

					gBattleScripting.battler = gEffectBattler;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_AbilityWasSuppressed;
				}
				else
					gBattlescriptCurrInstr++;

				break;

			case MOVE_EFFECT_BREAK_SCREENS:
            {
				u8 side = SIDE(gBattlerTarget);
				if (gSideTimers[side].reflectTimer
				||  gSideTimers[side].lightscreenTimer
				||  gSideStatuses[side] & SIDE_STATUS_REFLECT
				||  gSideStatuses[side] & SIDE_STATUS_LIGHTSCREEN
				||  gBattleStruct->AuroraVeilTimers[side])
				{
					RemoveScreensFromSide(side);
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattleStringLoader = ScreensShatteredString;
					gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				}
				else
					gBattlescriptCurrInstr++;
            }
				break;

			case MOVE_EFFECT_REMOVE_TERRAIN:
				if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN)
				{
					gBattlescriptCurrInstr++;
					break; //Can't be removed
				}

				if (gTerrainType)
				{
					gTerrainType = 0;
					gBattleStruct->TerrainTimer = 0;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattleStringLoader = TerrainEndString;
					gBattlescriptCurrInstr = BattleScript_PrintCustomString;

					gActiveBattler = 0;
					EmitDataTransfer(0, &gTerrainType, 1, &gTerrainType);
					MarkBattlerForControllerExec(gActiveBattler);
				}
				else
					gBattlescriptCurrInstr++;
				break;

			default:
				gBattlescriptCurrInstr++;
			}
		}
	}

CLEAR_MOVE_EFFECT_BYTE:
	switch (gBattleCommunication[MOVE_EFFECT_BYTE]) { //Handled later
		case MOVE_EFFECT_STEAL_ITEM:
		case MOVE_EFFECT_KNOCK_OFF:
		case MOVE_EFFECT_REMOVE_PARALYSIS:
		case MOVE_EFFECT_BRING_DOWN:
		case MOVE_EFFECT_ION_DELUGE:
			gBattleStruct->backupMoveEffect = gBattleCommunication[MOVE_EFFECT_BYTE];
	}

	gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
}

static void atk15_seteffectwithchance(void)
{
    u32 PercentChance;

    if (ABILITY(gBattlerAttacker) == ABILITY_SERENEGRACE || BankSideHasRainbow(gBattlerAttacker))
	{
		PercentChance = gBattleMoves[gCurrentMove].secondaryEffectChance * 2;
	}
	else
	{
		PercentChance = gBattleMoves[gCurrentMove].secondaryEffectChance;
	}

	if (!SheerForceCheck() || (gBattleCommunication[MOVE_EFFECT_BYTE] & 0x3F) == MOVE_EFFECT_RAPIDSPIN)
	{
		if ((gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_CERTAIN) && MOVE_HAD_EFFECT)
		{
			gBattleCommunication[MOVE_EFFECT_BYTE] &= 0x7F;
			SetMoveEffect(FALSE, MOVE_EFFECT_CERTAIN);
		}
		else if (Random() % 100 <= PercentChance && gBattleCommunication[MOVE_EFFECT_BYTE] != 0 && MOVE_HAD_EFFECT)
		{
			SetMoveEffect(FALSE, 0);
		}
		else
			gBattlescriptCurrInstr++;
	}
	else
		gBattlescriptCurrInstr++;

	gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
	gBattleScripting.multihitMoveEffect = 0;
}

static void atk16_seteffectprimary(void)
{
    SetMoveEffect(TRUE, 0);
}

static void atk17_seteffectsecondary(void)
{
    SetMoveEffect(FALSE, 0);
}

static void atk18_clearstatusfromeffect(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

    if (gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_TOXIC)
        gBattleMons[gActiveBattler].status1 &= (~sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]]);
    else
        gBattleMons[gActiveBattler].status2 &= (~sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]]);
    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
    gBattlescriptCurrInstr += 2;
    gBattleScripting.multihitMoveEffect = 0;
}

static void atk19_tryfaintmon(void)
{
    const u8* BS_ptr;

	if (gBattlescriptCurrInstr[2] != 0)
	{
		gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
		if (gHitMarker & HITMARKER_FAINTED(gActiveBattler) && gBattleStruct->endTurnDone) //To prevent things like Whirlwind from activating this
		{
			BS_ptr = T1_READ_PTR(gBattlescriptCurrInstr + 3);

			BattleScriptPop();
			gBattlescriptCurrInstr = BS_ptr;
			gSideStatuses[SIDE(gActiveBattler)] &= ~(SIDE_STATUS_SPIKES_DAMAGED);
		}
		else
		{
			gBattlescriptCurrInstr += 7;
		}
	}
	else
	{
		u8 bank;

		if (gBattlescriptCurrInstr[1] == BS_GET_ATTACKER)
		{
			gActiveBattler = gBattlerAttacker;
			bank = gBattlerAttacker;

			if (IsCatchableRaidBattle() && bank == BANK_RAID_BOSS)
				BS_ptr = BattleScript_FaintRaidAttacker;
			else
				BS_ptr = BattleScript_FaintAttacker;
		}
		else if (gBattlescriptCurrInstr[1] == BS_GET_SCRIPTING_BANK)
		{
			gActiveBattler = gBattleScripting.battler;
			bank = gBattleScripting.battler;

			if (IsCatchableRaidBattle() && bank == BANK_RAID_BOSS)
				BS_ptr = BattleScript_FaintRaidScriptingBank;
			else
				BS_ptr = BattleScript_FaintScriptingBank;
		}
		else
		{
			gActiveBattler = gBattlerTarget;
			bank = gBattlerTarget;

			if (IsCatchableRaidBattle() && bank == BANK_RAID_BOSS)
				BS_ptr = BattleScript_FaintRaidTarget;
			else
				BS_ptr = BattleScript_FaintTarget;
		}

		if (TryDoBenjaminButterfree(7))
			return;

		if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler])
		&& gBattleMons[gActiveBattler].hp == 0)
		{
			gBattleStruct->lastFainted = gActiveBattler;
			gHitMarker |= HITMARKER_FAINTED(gActiveBattler);
			BattleScriptPush(gBattlescriptCurrInstr + 7);
			gBattlescriptCurrInstr = BS_ptr;
			if (SIDE(gActiveBattler) == B_SIDE_PLAYER
			&& (!IsTagBattle() || GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_LEFT)) //Is player's mon
			{
				gHitMarker |= HITMARKER_PLAYER_FAINTED;
				if (gBattleResults.playerFaintCounter < 0xFF)
					gBattleResults.playerFaintCounter++;
				AdjustFriendshipOnBattleFaint(gActiveBattler);
			}
			else
			{
				if (gBattleResults.opponentFaintCounter < 0xFF)
					gBattleResults.opponentFaintCounter++;
				gBattleResults.lastOpponentSpecies = GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_SPECIES, NULL);
			}

			gBattleStruct->RetaliateCounters[SIDE(gActiveBattler)] = 2;

			if ((gHitMarker & HITMARKER_DESTINYBOND)
			&& gBattleMons[gBattlerAttacker].hp != 0
			&& !IsDynamaxed(gBattlerAttacker)
			&& gBattlescriptCurrInstr[1] != BS_GET_SCRIPTING_BANK)
			{
                const u8* backupScript = gBattlescriptCurrInstr;
				gHitMarker &= ~(HITMARKER_DESTINYBOND);
				gBattlescriptCurrInstr = BattleScript_DestinyBondTakesLife;
				BattleScriptPushCursor();
				gBattleMoveDamage = gBattleMons[bank].hp;
				gBattlescriptCurrInstr = backupScript;
			}

			if ((gStatuses3[gBattlerTarget] & STATUS3_GRUDGE)
			 && !(gHitMarker & HITMARKER_GRUDGE)
			 && SIDE(gBattlerAttacker) != SIDE(gBattlerTarget)
			 && gBattleMons[gBattlerAttacker].hp != 0
			 && gCurrentMove != MOVE_STRUGGLE
			 && gBattlescriptCurrInstr[1] != BS_GET_SCRIPTING_BANK)
			{
				u8 moveIndex = gBattleStruct->chosenMovePositions[gBattlerAttacker];

				gBattleMons[gBattlerAttacker].pp[moveIndex] = 0;
				BattleScriptPush(gBattlescriptCurrInstr);
				gBattlescriptCurrInstr = BattleScript_GrudgeTakesPp;
				gActiveBattler = gBattlerAttacker;
				BtlController_EmitSetMonData(0, moveIndex + REQUEST_PPMOVE1_BATTLE, 0, 1, &gBattleMons[gActiveBattler].pp[moveIndex]);
				MarkBattlerForControllerExec(gActiveBattler);

				PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleMons[gBattlerAttacker].moves[moveIndex])
			}
		}
		else
		{
			gBattlescriptCurrInstr += 7;
		}
	}
}

static void atk1A_dofaintanimation(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        BtlController_EmitFaintAnimation(0);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 2;
    }
}

bool8 TryDoBenjaminButterfree(u8 scriptOffset)
{
	if (IsBenjaminButterfreeBattle() && !IS_TRANSFORMED(gActiveBattler) && !BATTLER_ALIVE(gActiveBattler))
	{
		u16 devolutionSpecies = GetMonDevolution(GetBankPartyData(gActiveBattler));
		if (devolutionSpecies != SPECIES_NONE)
		{
			PREPARE_SPECIES_BUFFER(gBattleTextBuff1, devolutionSpecies)
			DoFormChange(gActiveBattler, devolutionSpecies, TRUE, TRUE, TRUE);
			gEffectBattler = gActiveBattler;
			BattleScriptPush(gBattlescriptCurrInstr + scriptOffset);
			gBattlescriptCurrInstr = BattleScript_BenjaminButterfreeDevolution;
			return TRUE;
		}
	}

	return FALSE;
}

enum FaintEffectsStates
{
	Faint_DynamaxHP,
	Faint_ClearEffects,
	Faint_SoulHeart,
	Faint_ReceiverActivate,
	Faint_SwitchInAbilities,
	Faint_PrimalWeather,
	Faint_RaidBattle,
	Faint_LastPokemonMusic,
	Faint_FormsRevert,
	Faint_FormsStats,
	Faint_FormsHP,
};

static void atk1B_cleareffectsonfaint(void)
{
    u8 partner;
    struct Pokemon* mon;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	partner = PARTNER(gActiveBattler);
	mon = GetBankPartyData(gActiveBattler);

	if (!gBattleControllerExecFlags) {
		switch (gBattleStruct->faintEffectsState) {
			case Faint_DynamaxHP: //Update max HP if was dynamaxed
				if (IsDynamaxed(gActiveBattler))
				{
					gBattleMons[gActiveBattler].maxHP = GetBaseMaxHP(gActiveBattler);
					BtlController_EmitSetMonData(0, REQUEST_MAX_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].maxHP);
					MarkBattlerForControllerExec(gActiveBattler);
					gBattleStruct->dynamaxData.timer[gActiveBattler] = 0;
					return;
				}
				++gBattleStruct->faintEffectsState;
			case Faint_ClearEffects:
				gBattleMons[gActiveBattler].status1 = 0;
				BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 0x4, &gBattleMons[gActiveBattler].status1);
				MarkBattlerForControllerExec(gActiveBattler);

				{
                    int i;
					u32 backupStatus2[gBattlersCount];
					for (i = 0; i < gBattlersCount; ++i)
						backupStatus2[i] = gBattleMons[i].status2;
					FaintClearSetData();
					TryReactivateCentifernoSandblast(backupStatus2);
				}

				ClearSwitchBytes(gActiveBattler);
				ClearSwitchBits(gActiveBattler);

				gBattleMons[gActiveBattler].type3 = TYPE_BLANK;
				*gSeedHelper = 0;
				++gBattleStruct->faintEffectsState;
				return;

			case Faint_SoulHeart:
				for (; *gSeedHelper < gBattlersCount; ++*gSeedHelper)
				{
					u8 bank = *gSeedHelper;
					if (ABILITY(bank) == ABILITY_SOULHEART
					&&  bank != gActiveBattler
					&&  gBattleMons[bank].hp != 0
					&&  STAT_CAN_RISE(bank, STAT_SPATK)
					&&  ViableMonCountFromBank(FOE(bank)) > 0) //Foe still has mons
					{
						++gBattleMons[bank].statStages[STAT_SPATK - 1];
						gBattleScripting.battler = gEffectBattler = bank;

						PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_SPATK);
						PREPARE_STAT_ROSE(gBattleTextBuff2);
						gBattleScripting.animArg1 = 0xE + STAT_STAGE_SPATK;
						gBattleScripting.animArg2 = 0;

						gLastUsedAbility = ABILITY_SOULHEART;

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_SoulHeart;
						++*gSeedHelper;
						return;
					}
				}
				++gBattleStruct->faintEffectsState;

			case Faint_ReceiverActivate:
            {
				ability_t partnerAbility = ABILITY(partner);
                gBattleStruct->ReceiverActivated = FALSE;

				if (IS_DOUBLE_BATTLE
				&& (partnerAbility == ABILITY_RECEIVER || partnerAbility == ABILITY_POWEROFALCHEMY)
				&& !CheckTableForAbility(CopyAbility(gActiveBattler), gReceiverBannedAbilities))
				{
					gLastUsedAbility = partnerAbility;
					*GetAbilityLocation(partner) = CopyAbility(gActiveBattler);
					gEffectBattler = gActiveBattler;
					gBattleScripting.battler = partner;
					gBattleStruct->ReceiverActivated = TRUE;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_Receiver;

					gAbilityPopUpHelper = gLastUsedAbility;
					EmitDataTransfer(0, &gAbilityPopUpHelper, 1, &gAbilityPopUpHelper);
					MarkBattlerForControllerExec(gActiveBattler);

					++gBattleStruct->faintEffectsState;
					return;
				}
				++gBattleStruct->faintEffectsState;
            }

			case Faint_SwitchInAbilities: //Now null b/c handled in BS
				++gBattleStruct->faintEffectsState;

			case Faint_PrimalWeather:	;
				if (HandleSpecialSwitchOutAbilities(gActiveBattler, ABILITY(gActiveBattler)))
					return;

				++gBattleStruct->faintEffectsState;

			case Faint_RaidBattle:
				++gBattleStruct->faintEffectsState;
			#ifdef FLAG_RAID_BATTLE
				if (IsRaidBattle() && SIDE(gActiveBattler) == B_SIDE_PLAYER)
				{
					u8 raidBank = BANK_RAID_BOSS;
					const u8* stormString = NULL;
					++gBattleStruct->dynamaxData.stormLevel;

					if (gCurrentMove != MOVE_STRUGGLE && Random() % 100 < GetRaidBattleRepeatedAttackChance(raidBank))
						gBattleStruct->dynamaxData.attackAgain = TRUE; //Set indicator to attack again after CMD49 finishes

					if ((Random() & 1)) //50 % Chance
					{
						u8 statId, increase;
						increase = GetRaidBattleKOStatIncreaseAmount(raidBank);

						if (increase > 0)
						{
                            u8 backup;
							if ((Random() & 1) == 0)
								statId = STAT_STAGE_ATK;
							else
								statId = STAT_STAGE_SPATK;

							if (ABILITY(raidBank) == ABILITY_CONTRARY)
								increase *= -1; //So it still gets the boost
							gBattleScripting.statChanger = statId | SET_STAT_BUFF_VALUE(increase);

							backup = gBattlerAttacker;
							gBattlerAttacker = raidBank;
							if (!ChangeStatBuffs(SET_STAT_BUFF_VALUE(increase), statId, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, NULL))
							{
								gBattleScripting.battler = raidBank;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_RaidBattleStatIncrease;
							}
							gBattlerAttacker = backup;
						}
					}

					#ifdef FLAG_RAID_BATTLE_NO_FORCE_END
					if (FlagGet(FLAG_RAID_BATTLE_NO_FORCE_END))
						return; //Don't force end this battle
					#endif

					BattleScriptPushCursor();

					switch (gBattleStruct->dynamaxData.stormLevel) {
						case 1:
							stormString = gText_RaidBattleKO1;
							gBattlescriptCurrInstr = BattleScript_RaidBattleStorm;
							break;
						case 2:
							stormString = gText_RaidBattleKO2;
							gBattlescriptCurrInstr = BattleScript_RaidBattleStorm;
							break;
						case 3:
							stormString = gText_RaidBattleKO3;
							gBattlescriptCurrInstr = BattleScript_RaidBattleStorm;
							break;
						case 4:
							stormString = gText_RaidBattleKO4;
							gBattleScripting.battler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
							gBattlescriptCurrInstr = BattleScript_LoseRaidBattle;
							break;
					}

					gBattleStringLoader = stormString;
					return;
				}
			#endif
				//Fallthrough
			case Faint_LastPokemonMusic:
				#ifdef BGM_BATTLE_GYM_LEADER_LAST_POKEMON
				if ((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE)) == (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE) //Double Gym battle
				&& !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_TOWER))
				&& gTrainers[gTrainerBattleOpponent_A].trainerClass == CLASS_LEADER
				&& ViableMonCount(gEnemyParty) <= 1)
				{
					PlayBGM(BGM_BATTLE_GYM_LEADER_LAST_POKEMON);
				}
				#endif
				++gBattleStruct->faintEffectsState;
				//Fallthrough

			case Faint_FormsRevert:
				if (TryFormRevert(mon))
				{
					BtlController_EmitSetMonData(0, REQUEST_SPECIES_BATTLE, 0, 2, &mon->box.species);
					MarkBattlerForControllerExec(gActiveBattler);
					mon->box.backupSpecies = SPECIES_NONE;
					++gBattleStruct->faintEffectsState;
					return;
				}
				break; //No form change means skip the next two states

			case Faint_FormsStats:
				CalculateMonStats(mon);
				BtlController_EmitSetRawMonData(0, offsetof(struct Pokemon, attack), 2 /*Atk*/ + 2 /*Def*/ + 2 /*Spd*/ + 2 */*Sp Atk*/ + 2 /*Sp Def*/, &mon->attack); //Reload all stats
				MarkBattlerForControllerExec(gActiveBattler);
				++gBattleStruct->faintEffectsState;
				return;

			case Faint_FormsHP: 
            {
				u16 oldHP, newHP;
				oldHP = mon->hp;

				#if (defined SPECIES_ZYGARDE && defined SPECIES_ZYGARDE_10)
				if (mon->box.species == SPECIES_ZYGARDE || mon->box.species == SPECIES_ZYGARDE_10)
				{
					newHP = MathMin(mon->maxHP, oldHP);
					BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &newHP);
					MarkBattlerForControllerExec(gActiveBattler);
					++gBattleStruct->faintEffectsState;
					return;
				}
				#endif
            }
		}

		gBattleStruct->faintEffectsState = 0;
		gBattlescriptCurrInstr += 2;
	}
}

static void atk1C_jumpifstatus(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u32 flags = T2_READ_32(gBattlescriptCurrInstr + 2);
	void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 6);

	if (gBattlescriptCurrInstr[1] == BS_GET_TARGET && !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bank))
		flags &= ~(STATUS2_SUBSTITUTE);

	if (gBattleMons[bank].status2 & flags && BATTLER_ALIVE(bank))
		gBattlescriptCurrInstr = jump_loc;
	else
		gBattlescriptCurrInstr += 10;
}

static void atk1D_jumpifstatus2(void)
{
    u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u32 flags = T2_READ_32(gBattlescriptCurrInstr + 2);
	void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 6);

	if (gBattlescriptCurrInstr[1] == BS_GET_TARGET && !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bank))
		flags &= ~(STATUS2_SUBSTITUTE);

	if (gBattleMons[bank].status2 & flags && BATTLER_ALIVE(bank))
		gBattlescriptCurrInstr = jump_loc;
	else
		gBattlescriptCurrInstr += 10;
}

static void atk1E_jumpifability(void)
{
    u8 battlerId;
	ability_t ability = gBattlescriptCurrInstr[2];
	const u8* jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 3);

	if (gBattlescriptCurrInstr[1] == BS_GET_ATTACKER_SIDE)
	{
		battlerId = AbilityBattleEffects(ABILITYEFFECT_CHECK_BATTLER_SIDE, gBattlerAttacker, ability, 0, 0);
		if (battlerId)
		{
			gLastUsedAbility = ability;
			gBattlescriptCurrInstr = jumpPtr;
			RecordAbilityBattle(battlerId - 1, gLastUsedAbility);
			gBattleScripting.battlerWithAbility = battlerId - 1;
		}
		else
			gBattlescriptCurrInstr += 7;
	}
	else if (gBattlescriptCurrInstr[1] == BS_GET_NOT_ATTACKER_SIDE)
	{
		battlerId = AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, gBattlerAttacker, ability, 0, 0);
		if (battlerId)
		{
			gLastUsedAbility = ability;
			gBattlescriptCurrInstr = jumpPtr;
			RecordAbilityBattle(battlerId - 1, gLastUsedAbility);
			gBattleScripting.battlerWithAbility = battlerId - 1;
		}
		else
			gBattlescriptCurrInstr += 7;
	}
	else
	{
		battlerId = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
		if (ABILITY(battlerId) == ability)
		{
			gLastUsedAbility = ability;
			gBattlescriptCurrInstr = jumpPtr;
			RecordAbilityBattle(battlerId, gLastUsedAbility);
			gBattleScripting.battlerWithAbility = battlerId;
		}
		else
			gBattlescriptCurrInstr += 7;
	}
}

static void atk1F_jumpifsideaffecting(void)
{
    u8 side;
	u16 flags = T2_READ_16(gBattlescriptCurrInstr + 2);
	void* jumpLoc = T2_READ_PTR(gBattlescriptCurrInstr + 4);

	if (gBattlescriptCurrInstr[1] == BS_GET_ATTACKER)
	{
		side = SIDE(gBattlerAttacker);
		if (BankSideHasSafeguard(gBattlerAttacker))
			flags |= SIDE_STATUS_SAFEGUARD;
		if (BankSideHasMist(gBattlerAttacker))
			flags |= SIDE_STATUS_MIST;
	}
	else
	{
		side = SIDE(gBattlerTarget);
		if (BankSideHasSafeguard(gBattlerTarget))
			flags |= SIDE_STATUS_SAFEGUARD;
		if (BankSideHasMist(gBattlerTarget))
			flags |= SIDE_STATUS_MIST;
		if (ABILITY(gBattlerTarget) == ABILITY_INFILTRATOR)
			flags &= ~(SIDE_STATUS_SAFEGUARD | SIDE_STATUS_MIST);
	}

	if (gSideStatuses[side] & flags)
		gBattlescriptCurrInstr = jumpLoc;
	else
		gBattlescriptCurrInstr += 8;
}

static void atk20_jumpifstat(void)
{
	u8 ret = 0;
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8 value = gBattleMons[bank].statStages[gBattlescriptCurrInstr[3] - 1];
	s8 comparison = gBattlescriptCurrInstr[4];

	if (ABILITY(bank) == ABILITY_CONTRARY)
	{
		comparison -= 12;
		comparison *= - 1;
	}

	switch (T2_READ_8(gBattlescriptCurrInstr + 2)) {
		case CMP_EQUAL:
			if (value == comparison)
				ret++;
			break;
		case CMP_NOT_EQUAL:
			if (value != comparison)
				ret++;
			break;
		case CMP_GREATER_THAN:
			if (value > comparison)
				ret++;
			break;
		case CMP_LESS_THAN:
			if (value < comparison)
				ret++;
			break;
		case CMP_COMMON_BITS:
			if (value & comparison)
				ret++;
			break;
		case CMP_NO_COMMON_BITS:
			if (!(value & comparison))
				ret++;
			break;
	}

	if (ret)
		gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 5);
	else
		gBattlescriptCurrInstr += 9;
}

static void atk21_jumpifstatus3condition(void)
{
    u32 flags;
    const u8 *jumpPtr;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    flags = T2_READ_32(gBattlescriptCurrInstr + 2);
    jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 7);
    if (gBattlescriptCurrInstr[6])
    {
        if (gStatuses3[gActiveBattler] & flags)
            gBattlescriptCurrInstr += 11;
        else
            gBattlescriptCurrInstr = jumpPtr;
    }
    else
    {
        if (gStatuses3[gActiveBattler] & flags)
            gBattlescriptCurrInstr = jumpPtr;
        else
            gBattlescriptCurrInstr += 11;
    }
}

static void atk22_jumpiftype(void)
{
    u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8 type = gBattlescriptCurrInstr[2];
	void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 3);

	if (IsOfType(bank, type))
		gBattlescriptCurrInstr = jump_loc;
	else
		gBattlescriptCurrInstr += 7;
}

static void atk23_getexp(void)
{
    u16 item;
    s32 i; // also used as stringId
    u8 holdEffect;
    s32 sentIn;
    s32 viaExpShare = 0;
    u16 *exp = &gBattleStruct->expValue;

    gBattlerFainted = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    sentIn = gSentPokesToOpponent[(gBattlerFainted & 2) >> 1];
    
    switch (gBattleScripting.atk23_state)
    {
    case 0: // check if should receive exp at all
        if (GetBattlerSide(gBattlerFainted) != B_SIDE_OPPONENT
         || (gBattleTypeFlags &
            (BATTLE_TYPE_LINK
            | BATTLE_TYPE_TRAINER_TOWER
            | BATTLE_TYPE_BATTLE_TOWER
            | BATTLE_TYPE_SAFARI
            | BATTLE_TYPE_EREADER_TRAINER)))
        {
            gBattleScripting.atk23_state = 6; // goto last case
        }
        else
        {
            ++gBattleScripting.atk23_state;
            gBattleStruct->givenExpMons |= gBitTable[gBattlerPartyIndexes[gBattlerFainted]];
        }
        break;
    case 1: // calculate experience points to redistribute
        {
            u16 calculatedExp;
            s32 viaSentIn;

            for (viaSentIn = 0, i = 0; i < PARTY_SIZE; ++i)
            {
                if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) != SPECIES_NONE && GetMonData(&gPlayerParty[i], MON_DATA_HP) != 0)
                {
                    if (gBitTable[i] & sentIn)
                        ++viaSentIn;
                    item = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM);
                    if (item == ITEM_ENIGMA_BERRY)
                        holdEffect = gSaveBlock1Ptr->enigmaBerry.holdEffect;
                    else
                        holdEffect = ItemId_GetHoldEffect(item);
                    if (holdEffect == HOLD_EFFECT_EXP_SHARE)
                        ++viaExpShare;
                }
            }
            calculatedExp = gBaseStats[gBattleMons[gBattlerFainted].species].expYield * gBattleMons[gBattlerFainted].level / 7;
            if (viaExpShare) // at least one mon is getting exp via exp share
            {
                *exp = SAFE_DIV(calculatedExp / 2, viaSentIn);
                if (*exp == 0)
                    *exp = 1;
                gExpShareExp = calculatedExp / 2 / viaExpShare;
                if (gExpShareExp == 0)
                    gExpShareExp = 1;
            }
            else
            {
                *exp = SAFE_DIV(calculatedExp, viaSentIn);
                if (*exp == 0)
                    *exp = 1;
                gExpShareExp = 0;
            }
            ++gBattleScripting.atk23_state;
            gBattleStruct->expGetterMonId = 0;
            gBattleStruct->sentInPokes = sentIn;
        }
        // fall through
    case 2: // set exp value to the poke in expgetter_id and print message
        if (!gBattleControllerExecFlags)
        {
            item = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HELD_ITEM);
            if (item == ITEM_ENIGMA_BERRY)
                holdEffect = gSaveBlock1Ptr->enigmaBerry.holdEffect;
            else
                holdEffect = ItemId_GetHoldEffect(item);
            if (holdEffect != HOLD_EFFECT_EXP_SHARE && !(gBattleStruct->sentInPokes & 1))
            {
                *(&gBattleStruct->sentInPokes) >>= 1;
                gBattleScripting.atk23_state = 5;
                gBattleMoveDamage = 0; // used for exp
            }
            else if (GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL) == MAX_LEVEL)
            {
                *(&gBattleStruct->sentInPokes) >>= 1;
                gBattleScripting.atk23_state = 5;
                gBattleMoveDamage = 0; // used for exp
            }
            else
            {
                // music change in wild battle after fainting a poke
                if (!(gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_POKEDUDE))
                 && gBattleMons[0].hp
                 && !gBattleStruct->wildVictorySong)
                {
                    BattleStopLowHpSound();
                    PlayBGM(MUS_VICTORY_WILD);
                    ++gBattleStruct->wildVictorySong;
                }
                if (GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP))
                {
                    if (gBattleStruct->sentInPokes & 1)
                        gBattleMoveDamage = *exp;
                    else
                        gBattleMoveDamage = 0;
                    if (holdEffect == HOLD_EFFECT_EXP_SHARE)
                        gBattleMoveDamage += gExpShareExp;
                    if (holdEffect == HOLD_EFFECT_LUCKY_EGG)
                        gBattleMoveDamage = (gBattleMoveDamage * 150) / 100;
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        gBattleMoveDamage = (gBattleMoveDamage * 150) / 100;
                    if (IsTradedMon(&gPlayerParty[gBattleStruct->expGetterMonId])
                     && !(gBattleTypeFlags & BATTLE_TYPE_POKEDUDE))
                    {
                        gBattleMoveDamage = (gBattleMoveDamage * 150) / 100;
                        i = STRINGID_ABOOSTED;
                    }
                    else
                    {
                        i = STRINGID_EMPTYSTRING4;
                    }
                    // get exp getter battlerId
                    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                    {
                        if (!(gBattlerPartyIndexes[2] != gBattleStruct->expGetterMonId) && !(gAbsentBattlerFlags & gBitTable[2]))
                            gBattleStruct->expGetterBattlerId = 2;
                        else
                        {
                            if (!(gAbsentBattlerFlags & gBitTable[0]))
                                gBattleStruct->expGetterBattlerId = 0;
                            else
                                gBattleStruct->expGetterBattlerId = 2;
                        }
                    }
                    else
                    {
                        gBattleStruct->expGetterBattlerId = 0;
                    }
                    PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gBattleStruct->expGetterBattlerId, gBattleStruct->expGetterMonId);
                    // buffer 'gained' or 'gained a boosted'
                    PREPARE_STRING_BUFFER(gBattleTextBuff2, i);
                    PREPARE_WORD_NUMBER_BUFFER(gBattleTextBuff3, 5, gBattleMoveDamage);
                    PrepareStringBattle(STRINGID_PKMNGAINEDEXP, gBattleStruct->expGetterBattlerId);
                    MonGainEVs(&gPlayerParty[gBattleStruct->expGetterMonId], gBattleMons[gBattlerFainted].species);
                }
                gBattleStruct->sentInPokes >>= 1;
                ++gBattleScripting.atk23_state;
            }
        }
        break;
    case 3: // Set stats and give exp
        if (!gBattleControllerExecFlags)
        {
            gBattleBufferB[gBattleStruct->expGetterBattlerId][0] = 0;
            if (GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP) && GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL) != MAX_LEVEL)
            {
                gBattleResources->beforeLvlUp->stats[STAT_HP]    = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MAX_HP);
                gBattleResources->beforeLvlUp->stats[STAT_ATK]   = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_ATK);
                gBattleResources->beforeLvlUp->stats[STAT_DEF]   = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_DEF);
                gBattleResources->beforeLvlUp->stats[STAT_SPEED] = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPEED);
                gBattleResources->beforeLvlUp->stats[STAT_SPATK] = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPATK);
                gBattleResources->beforeLvlUp->stats[STAT_SPDEF] = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPDEF);
                gActiveBattler = gBattleStruct->expGetterBattlerId;
                BtlController_EmitExpUpdate(0, gBattleStruct->expGetterMonId, gBattleMoveDamage);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            ++gBattleScripting.atk23_state;
        }
        break;
    case 4: // lvl up if necessary
        if (!gBattleControllerExecFlags)
        {
            gActiveBattler = gBattleStruct->expGetterBattlerId;
            if (gBattleBufferB[gActiveBattler][0] == CONTROLLER_TWORETURNVALUES && gBattleBufferB[gActiveBattler][1] == RET_VALUE_LEVELED_UP)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && gBattlerPartyIndexes[gActiveBattler] == gBattleStruct->expGetterMonId)
                    HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], gActiveBattler);
                PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gActiveBattler, gBattleStruct->expGetterMonId);
                PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff2, 3, GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL));
                BattleScriptPushCursor();
                gLeveledUpInBattle |= gBitTable[gBattleStruct->expGetterMonId];
                gBattlescriptCurrInstr = BattleScript_LevelUp;
                gBattleMoveDamage = (gBattleBufferB[gActiveBattler][2] | (gBattleBufferB[gActiveBattler][3] << 8));
                AdjustFriendship(&gPlayerParty[gBattleStruct->expGetterMonId], FRIENDSHIP_EVENT_GROW_LEVEL);
                // update battle mon structure after level up
                if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId && gBattleMons[0].hp)
                {
                    gBattleMons[0].level = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL);
                    gBattleMons[0].hp = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP);
                    gBattleMons[0].maxHP = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MAX_HP);
                    gBattleMons[0].attack = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_ATK);
                    gBattleMons[0].defense = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_DEF);
                    // Why is this duplicated?
                    gBattleMons[0].speed = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPEED);
                    gBattleMons[0].speed = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPEED);
                    gBattleMons[0].spAttack = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPATK);
                    gBattleMons[0].spDefense = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPDEF);
                }
                // What is else if?
                if (gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId && gBattleMons[2].hp && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
                {
                    gBattleMons[2].level = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL);
                    gBattleMons[2].hp = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP);
                    gBattleMons[2].maxHP = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MAX_HP);
                    gBattleMons[2].attack = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_ATK);
                    gBattleMons[2].defense = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_DEF);
                    // Duplicated again, but this time there's no Sp Defense
                    gBattleMons[2].speed = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPEED);
                    gBattleMons[2].speed = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPEED);
                    gBattleMons[2].spAttack = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPATK);
                }
                gBattleScripting.atk23_state = 5;
            }
            else
            {
                gBattleMoveDamage = 0;
                gBattleScripting.atk23_state = 5;
            }
        }
        break;
    case 5: // looper increment
        if (gBattleMoveDamage) // there is exp to give, goto case 3 that gives exp
        {
            gBattleScripting.atk23_state = 3;
        }
        else
        {
            ++gBattleStruct->expGetterMonId;
            if (gBattleStruct->expGetterMonId <= 5)
                gBattleScripting.atk23_state = 2; // loop again
            else
                gBattleScripting.atk23_state = 6; // we're done
        }
        break;
    case 6: // increment instruction
        if (!gBattleControllerExecFlags)
        {
            // not sure why gf clears the item and ability here
            gBattleMons[gBattlerFainted].item = ITEM_NONE;
            gBattleMons[gBattlerFainted].ability = ABILITY_NONE;
            gBattlescriptCurrInstr += 2;
        }
        break;
    }
}

static void atk24_ifwildbattleend(void)
{
    u16 HP_count = 0;
    s32 i;

    if (!gBattleControllerExecFlags)
    {
        for (i = 0; i < PARTY_SIZE; ++i)
            if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
                HP_count += GetMonData(&gPlayerParty[i], MON_DATA_HP);
        if (HP_count == 0)
            gBattleOutcome |= B_OUTCOME_LOST;
        for (HP_count = 0, i = 0; i < PARTY_SIZE; ++i)
            if (GetMonData(&gEnemyParty[i], MON_DATA_SPECIES) && !GetMonData(&gEnemyParty[i], MON_DATA_IS_EGG))
                HP_count += GetMonData(&gEnemyParty[i], MON_DATA_HP);
        if (HP_count == 0)
            gBattleOutcome |= B_OUTCOME_WON;
        if (gBattleOutcome == 0 && (gBattleTypeFlags & BATTLE_TYPE_LINK))
        {
            s32 foundPlayer;
            s32 foundOpponent;

            for (foundPlayer = 0, i = 0; i < gBattlersCount; i += 2)
            {
                u32 *ptr = &gHitMarker;
                u32 hitMarkerUnk = 0x10000000;
                
                ++i;
                --i;
                if ((hitMarkerUnk << i) & *ptr && !gSpecialStatuses[i].flag40)
                    ++foundPlayer;
            }
            for (foundOpponent = 0, i = 1; i < gBattlersCount; i += 2)
            {
                u32 *ptr = &gHitMarker;
                u32 hitMarkerUnk = 0x10000000;
                
                {
                    u8 match;

                    ++match;
                    --match;
                }
                if ((hitMarkerUnk << i) & *ptr && !gSpecialStatuses[i].flag40)
                    ++foundOpponent;
            }
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                if (foundOpponent + foundPlayer > 1)
                    gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
                else
                    gBattlescriptCurrInstr += 5;
            }
            else
            {
                if (foundOpponent != 0 && foundPlayer != 0)
                    gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
                else
                    gBattlescriptCurrInstr += 5;
            }
        }
        else
        {
            gBattlescriptCurrInstr += 5;
        }
    }
}

static void atk25_movevaluescleanup(void)
{
    MoveValuesCleanUp();
    gBattlescriptCurrInstr += 1;
}

static void atk26_setmultihit(void)
{
    gMultiHitCounter = gBattlescriptCurrInstr[1];
    gBattlescriptCurrInstr += 2;
}

void MoveValuesCleanUp(void)
{
	gMoveResultFlags = 0;
	gBattleScripting.dmgMultiplier = 1;
	gCritMultiplier = BASE_CRIT_MULTIPLIER;
	gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
	gBattleStruct->backupMoveEffect = 0;
	gBattleCommunication[6] = 0;
	gHitMarker &= ~(HITMARKER_DESTINYBOND);
	gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
}

static void atk27_decrementmultihit(void)
{
    if (gMultiHitCounter == 0 || --gMultiHitCounter == 0)
		gBattlescriptCurrInstr += 5;
	else
		gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atk28_goto(void)
{
    gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atk29_jumpifbyte(void)
{
    u8 caseID = gBattlescriptCurrInstr[1];
    const u8 *memByte = T2_READ_PTR(gBattlescriptCurrInstr + 2);
    u8 value = gBattlescriptCurrInstr[6];
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 7);

    gBattlescriptCurrInstr += 11;

    switch (caseID)
    {
    case CMP_EQUAL:
        if (*memByte == value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_NOT_EQUAL:
        if (*memByte != value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_GREATER_THAN:
        if (*memByte > value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_LESS_THAN:
        if (*memByte < value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_COMMON_BITS:
        if (*memByte & value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_NO_COMMON_BITS:
        if (!(*memByte & value))
            gBattlescriptCurrInstr = jumpPtr;
        break;
    }
}

static void atk2A_jumpifhalfword(void)
{
    u8 caseID = gBattlescriptCurrInstr[1];
    const u16 *memHword = T2_READ_PTR(gBattlescriptCurrInstr + 2);
    u16 value = T2_READ_16(gBattlescriptCurrInstr + 6);
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 8);

    gBattlescriptCurrInstr += 12;
    switch (caseID)
    {
    case CMP_EQUAL:
        if (*memHword == value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_NOT_EQUAL:
        if (*memHword != value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_GREATER_THAN:
        if (*memHword > value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_LESS_THAN:
        if (*memHword < value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_COMMON_BITS:
        if (*memHword & value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_NO_COMMON_BITS:
        if (!(*memHword & value))
            gBattlescriptCurrInstr = jumpPtr;
        break;
    }
}

static void atk2B_jumpifword(void)
{
    u8 caseID = gBattlescriptCurrInstr[1];
    const u32 *memWord = T2_READ_PTR(gBattlescriptCurrInstr + 2);
    u32 value = T1_READ_32(gBattlescriptCurrInstr + 6);
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 10);

    gBattlescriptCurrInstr += 14;
    switch (caseID)
    {
    case CMP_EQUAL:
        if (*memWord == value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_NOT_EQUAL:
        if (*memWord != value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_GREATER_THAN:
        if (*memWord > value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_LESS_THAN:
        if (*memWord < value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_COMMON_BITS:
        if (*memWord & value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_NO_COMMON_BITS:
        if (!(*memWord & value))
            gBattlescriptCurrInstr = jumpPtr;
        break;
    }
}

static void atk2C_jumpifarrayequal(void)
{
    const u8 *mem1 = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    const u8 *mem2 = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    u32 size = gBattlescriptCurrInstr[9];
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 10);
    u8 i;

    for (i = 0; i < size; ++i)
    {
        if (*mem1 != *mem2)
        {
            gBattlescriptCurrInstr += 14;
            break;
        }
        ++mem1, ++mem2;
    }
    if (i == size)
        gBattlescriptCurrInstr = jumpPtr;
}

static void atk2D_jumpifarraynotequal(void)
{
    u8 equalBytes = 0;
    const u8 *mem1 = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    const u8 *mem2 = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    u32 size = gBattlescriptCurrInstr[9];
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 10);
    u8 i;

    for (i = 0; i < size; ++i)
    {
        if (*mem1 == *mem2)
            ++equalBytes;
        ++mem1, ++mem2;
    }
    if (equalBytes != size)
        gBattlescriptCurrInstr = jumpPtr;
    else
        gBattlescriptCurrInstr += 14;
}

static void atk2E_setbyte(void)
{
    u8 *memByte = T2_READ_PTR(gBattlescriptCurrInstr + 1);

    *memByte = gBattlescriptCurrInstr[5];
    gBattlescriptCurrInstr += 6;
}

static void atk2F_addbyte(void)
{
    u8 *memByte = T2_READ_PTR(gBattlescriptCurrInstr + 1);

    *memByte += gBattlescriptCurrInstr[5];
    gBattlescriptCurrInstr += 6;
}

static void atk30_subbyte(void)
{
    u8 *memByte = T2_READ_PTR(gBattlescriptCurrInstr + 1);

    *memByte -= gBattlescriptCurrInstr[5];
    gBattlescriptCurrInstr += 6;
}

static void atk31_copyarray(void)
{
    u8 *dest = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    const u8 *src = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    s32 size = gBattlescriptCurrInstr[9];
    s32 i;

    for (i = 0; i < size; ++i)
    {
        dest[i] = src[i];
    }
    gBattlescriptCurrInstr += 10;
}

static void atk32_copyarraywithindex(void)
{
    u8 *dest = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    const u8 *src = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    const u8 *index = T2_READ_PTR(gBattlescriptCurrInstr + 9);
    s32 size = gBattlescriptCurrInstr[13];
    s32 i;

    for (i = 0; i < size; ++i)
    {
        dest[i] = src[i + *index];
    }
    gBattlescriptCurrInstr += 14;
}

static void atk33_orbyte(void)
{
    u8 *memByte = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    *memByte |= gBattlescriptCurrInstr[5];
    gBattlescriptCurrInstr += 6;
}

static void atk34_orhalfword(void)
{
    u16 *memHword = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u16 val = T2_READ_16(gBattlescriptCurrInstr + 5);

    *memHword |= val;
    gBattlescriptCurrInstr += 7;
}

static void atk35_orword(void)
{
    u32 *memWord = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u32 val = T2_READ_32(gBattlescriptCurrInstr + 5);

    *memWord |= val;
    gBattlescriptCurrInstr += 9;
}

static void atk36_bicbyte(void)
{
    u8 *memByte = T2_READ_PTR(gBattlescriptCurrInstr + 1);

    *memByte &= ~(gBattlescriptCurrInstr[5]);
    gBattlescriptCurrInstr += 6;
}

static void atk37_bichalfword(void)
{
    u16 *memHword = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u16 val = T2_READ_16(gBattlescriptCurrInstr + 5);

    *memHword &= ~val;
    gBattlescriptCurrInstr += 7;
}

static void atk38_bicword(void)
{
    u32 *memWord = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u32 val = T2_READ_32(gBattlescriptCurrInstr + 5);

    *memWord &= ~val;
    gBattlescriptCurrInstr += 9;
}

static void atk39_pause(void)
{
    if (!gBattleControllerExecFlags)
    {
        u16 value = T2_READ_16(gBattlescriptCurrInstr + 1);

        if (++gPauseCounterBattle >= value)
        {
            gPauseCounterBattle = 0;
            gBattlescriptCurrInstr += 3;
        }
    }
}

static void atk3A_waitstate(void)
{
    if (!gBattleControllerExecFlags)
        ++gBattlescriptCurrInstr;
}

static void atk3B_healthbar_update(void)
{
    if (gBattlescriptCurrInstr[1] == BS_TARGET)
        gActiveBattler = gBattlerTarget;
    else
        gActiveBattler = gBattlerAttacker;
    BtlController_EmitHealthBarUpdate(0, gBattleMoveDamage);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk3C_return(void)
{
    BattleScriptPop();
}

static void atk3D_end(void)
{
    gMoveResultFlags = 0;
    gActiveBattler = 0;
    gCurrentActionFuncId = B_ACTION_TRY_FINISH;
}

static void atk3E_end2(void)
{
    gActiveBattler = 0;
    gCurrentActionFuncId = B_ACTION_TRY_FINISH;
}

static void atk3F_end3(void) // pops the main function stack
{
    BattleScriptPop();
    if (gBattleResources->battleCallbackStack->size != 0)
        gBattleResources->battleCallbackStack->size--;
    gBattleMainFunc = gBattleResources->battleCallbackStack->function[gBattleResources->battleCallbackStack->size];
}

static void atk41_call(void)
{
    BattleScriptPush(gBattlescriptCurrInstr + 5);
    gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atk42_jumpiftype2(void)
{
    u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

	if (IsOfType(bank, gBattlescriptCurrInstr[2]))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
	else
		gBattlescriptCurrInstr += 7;
}

static void atk43_jumpifabilitypresent(void)
{
    if (AbilityBattleEffects(ABILITYEFFECT_CHECK_ON_FIELD, 0, gBattlescriptCurrInstr[1], 0, 0))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    else
        gBattlescriptCurrInstr += 6;
}

static void atk44_endselectionscript(void)
{
    *(gBattlerAttacker + gBattleStruct->selectionScriptFinished) = TRUE;
}

static void TryContraryChangeStatAnim(u8 bank, u16* argumentPtr)
{
	if (ABILITY(bank) == ABILITY_CONTRARY)
	{
		u8 value = 0;
		switch (GET_STAT_BUFF_VALUE_WITH_SIGN(gBattleScripting.statChanger)) {
			case SET_STAT_BUFF_VALUE(1): // +1
				value = STAT_ANIM_MINUS1;
				break;
			case SET_STAT_BUFF_VALUE(2): // +2
				value = STAT_ANIM_MINUS2;
				break;
			case SET_STAT_BUFF_VALUE(3): // +3
				value = STAT_ANIM_MINUS2;
				break;
			case SET_STAT_BUFF_VALUE(4): // +4
				value = STAT_ANIM_MINUS2;
				break;
			case SET_STAT_BUFF_VALUE(5): // +5
				value = STAT_ANIM_MINUS2;
				break;
			case SET_STAT_BUFF_VALUE(6): // +6
				value = STAT_ANIM_MINUS2;
				break;
			case SET_STAT_BUFF_VALUE(1) | STAT_BUFF_NEGATIVE: // -1
				value = STAT_ANIM_PLUS2;
				break;
			case SET_STAT_BUFF_VALUE(2) | STAT_BUFF_NEGATIVE: // -2
				value = STAT_ANIM_PLUS2;
				break;
			case SET_STAT_BUFF_VALUE(3) | STAT_BUFF_NEGATIVE: // -3
				value = STAT_ANIM_PLUS2;
				break;
			case SET_STAT_BUFF_VALUE(4) | STAT_BUFF_NEGATIVE: // -1
				value = STAT_ANIM_PLUS2;
				break;
			case SET_STAT_BUFF_VALUE(5) | STAT_BUFF_NEGATIVE: // -2
				value = STAT_ANIM_PLUS2;
				break;
			case SET_STAT_BUFF_VALUE(6) | STAT_BUFF_NEGATIVE: // -3
				value = STAT_ANIM_PLUS2;
				break;
		}
		*argumentPtr = GET_STAT_BUFF_ID(gBattleScripting.statChanger) + value - 1;
	}
}

static void atk45_playanimation(void)
{
	u16* argumentPtr;

	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	argumentPtr = T2_READ_PTR(gBattlescriptCurrInstr + 3);

	if (gBattlescriptCurrInstr[2] == B_ANIM_STATS_CHANGE)
		TryContraryChangeStatAnim(gActiveBattler, argumentPtr);

	if (gBattlescriptCurrInstr[2] == B_ANIM_STATS_CHANGE
	&&  gActiveBattler == gBattleStruct->skipBankStatAnim) //The Pokemon has no sprite on the screen, so don't play the stat anim
	{
		gBattleStruct->skipBankStatAnim = 0xFF;
		gBattlescriptCurrInstr += 7;
	}
	else if (gBattlescriptCurrInstr[2] == B_ANIM_CASTFORM_CHANGE
	||	gBattlescriptCurrInstr[2] == B_ANIM_STATS_CHANGE
	||  gBattlescriptCurrInstr[2] == B_ANIM_CALL_BACK_POKEMON
	||  gBattlescriptCurrInstr[2] == B_ANIM_CALL_BACK_POKEMON_2
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_SNATCH_MOVE
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_SUBSTITUTE_FADE
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_TRANSFORM
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_WISHIWASHI_FISH
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_ZYGARDE_CELL_SWIRL
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_BLUE_PRIMAL_REVERSION
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_RED_PRIMAL_REVERSION
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_TRANSFORM
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_ZMOVE_ACTIVATE
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_MEGA_EVOLUTION
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_ULTRA_BURST
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_ELECTRIC_SURGE
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_GRASSY_SURGE
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_MISTY_SURGE
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_PSYCHIC_SURGE
	||  gBattlescriptCurrInstr[2] == B_ANIM_LOAD_DEFAULT_BG
	||  gBattlescriptCurrInstr[2] == B_ANIM_LOAD_ABILITY_POP_UP
	||  gBattlescriptCurrInstr[2] == B_ANIM_DESTROY_ABILITY_POP_UP
	||  gBattlescriptCurrInstr[2] == B_ANIM_DYNAMAX_START
	||  gBattlescriptCurrInstr[2] == B_ANIM_POWDER_EXPLOSION)
	{
		BtlController_EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
		MarkBattlerForControllerExec(gActiveBattler);
		gBattlescriptCurrInstr += 7;
	}
	else if (gBattlescriptCurrInstr[2] == B_ANIM_TRANSFORM_MOVE)
	{
		BtlController_EmitMoveAnimation(0, MOVE_TRANSFORM, 0, 1, 1, 0xFF, &gDisableStructs[gActiveBattler], 0);
		MarkBattlerForControllerExec(gActiveBattler);
		gBattlescriptCurrInstr += 7;
	}
	else if (gHitMarker & HITMARKER_NO_ANIMATIONS)
	{
		BattleScriptPush(gBattlescriptCurrInstr + 7);
		gBattlescriptCurrInstr = BattleScript_Pausex20;
	}
	else if (gBattleStruct->tempIgnoreAnimations)
	{
		gBattlescriptCurrInstr += 7;
		gBattleStruct->tempIgnoreAnimations = FALSE;
	}
	else if (gBattlescriptCurrInstr[2] == B_ANIM_RAIN_CONTINUES
		  || gBattlescriptCurrInstr[2] == B_ANIM_SUN_CONTINUES
		  || gBattlescriptCurrInstr[2] == B_ANIM_SANDSTORM_CONTINUES
		  || gBattlescriptCurrInstr[2] == B_ANIM_HAIL_CONTINUES
		  || gBattlescriptCurrInstr[2] == B_ANIM_STRONG_WINDS_CONTINUE
		  || gBattlescriptCurrInstr[2] == B_ANIM_FOG_CONTINUES
		  || gBattlescriptCurrInstr[2] == B_ANIM_RAID_BATTLE_STORM)
	{
		BtlController_EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
		MarkBattlerForControllerExec(gActiveBattler);
		gBattlescriptCurrInstr += 7;
	}
	else if (gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
	{
		gBattlescriptCurrInstr += 7;
	}
	else
	{
		BtlController_EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
		MarkBattlerForControllerExec(gActiveBattler);
		gBattlescriptCurrInstr += 7;
	}
}

void atk46_playanimation2(void) // animation Id is stored in the first pointer
{
	u16* argumentPtr;
	const u8* animationIdPtr;

	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	animationIdPtr = T2_READ_PTR(gBattlescriptCurrInstr + 2);
	argumentPtr = T2_READ_PTR(gBattlescriptCurrInstr + 6);

	if (*animationIdPtr == B_ANIM_STATS_CHANGE)
		TryContraryChangeStatAnim(gActiveBattler, argumentPtr);

	if (*animationIdPtr == B_ANIM_CASTFORM_CHANGE
	||	*animationIdPtr == B_ANIM_STATS_CHANGE
	||  *animationIdPtr == B_ANIM_CALL_BACK_POKEMON
	||  *animationIdPtr == B_ANIM_CALL_BACK_POKEMON_2
	|| 	*animationIdPtr == B_ANIM_SNATCH_MOVE
	|| 	*animationIdPtr == B_ANIM_SUBSTITUTE_FADE
	|| 	*animationIdPtr == B_ANIM_TRANSFORM
	|| 	*animationIdPtr == B_ANIM_WISHIWASHI_FISH
	|| 	*animationIdPtr == B_ANIM_ZYGARDE_CELL_SWIRL
	|| 	*animationIdPtr == B_ANIM_BLUE_PRIMAL_REVERSION
	|| 	*animationIdPtr == B_ANIM_RED_PRIMAL_REVERSION
	|| 	*animationIdPtr == B_ANIM_TRANSFORM
	|| 	*animationIdPtr == B_ANIM_ZMOVE_ACTIVATE
	|| 	*animationIdPtr == B_ANIM_MEGA_EVOLUTION
	|| 	*animationIdPtr == B_ANIM_ULTRA_BURST
	|| 	*animationIdPtr == B_ANIM_ELECTRIC_SURGE
	|| 	*animationIdPtr == B_ANIM_GRASSY_SURGE
	|| 	*animationIdPtr == B_ANIM_MISTY_SURGE
	|| 	*animationIdPtr == B_ANIM_PSYCHIC_SURGE
	||  *animationIdPtr == B_ANIM_LOAD_DEFAULT_BG
	||  *animationIdPtr == B_ANIM_LOAD_ABILITY_POP_UP
	||  *animationIdPtr == B_ANIM_DESTROY_ABILITY_POP_UP
	||  *animationIdPtr == B_ANIM_DYNAMAX_START
	||  *animationIdPtr == B_ANIM_POWDER_EXPLOSION)
	{
		BtlController_EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
		MarkBattlerForControllerExec(gActiveBattler);
		gBattlescriptCurrInstr += 10;
	}
	else if (*animationIdPtr == B_ANIM_TRANSFORM_MOVE)
	{
		BtlController_EmitMoveAnimation(0, MOVE_TRANSFORM, 0, 1, 1, 0xFF, &gDisableStructs[gActiveBattler], 0);
		MarkBattlerForControllerExec(gActiveBattler);
		gBattlescriptCurrInstr += 10;
	}
	else if (gHitMarker & HITMARKER_NO_ANIMATIONS
	|| gBattleStruct->tempIgnoreAnimations)
	{
		gBattleStruct->tempIgnoreAnimations = FALSE;
		gBattlescriptCurrInstr += 10;
	}
	else if (*animationIdPtr == B_ANIM_RAIN_CONTINUES
			 || *animationIdPtr == B_ANIM_SUN_CONTINUES
			 || *animationIdPtr == B_ANIM_SANDSTORM_CONTINUES
			 || *animationIdPtr == B_ANIM_HAIL_CONTINUES
			 || *animationIdPtr == B_ANIM_STRONG_WINDS_CONTINUE
			 || *animationIdPtr == B_ANIM_FOG_CONTINUES)
	{
		BtlController_EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
		MarkBattlerForControllerExec(gActiveBattler);
		gBattlescriptCurrInstr += 10;
	}
	else if (gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
	{
		gBattlescriptCurrInstr += 10;
	}
	else
	{
		BtlController_EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
		MarkBattlerForControllerExec(gActiveBattler);
		gBattlescriptCurrInstr += 10;
	}
}

static void atk47_setgraphicalstatchangevalues(void)
{
    u8 value = 0;

    switch (GET_STAT_BUFF_VALUE2(gBattleScripting.statChanger))
    {
    case SET_STAT_BUFF_VALUE(1): // +1
        value = STAT_ANIM_PLUS1;
        break;
    case SET_STAT_BUFF_VALUE(2): // +2
        value = STAT_ANIM_PLUS2;
        break;
    case SET_STAT_BUFF_VALUE(1) | STAT_BUFF_NEGATIVE: // -1
        value = STAT_ANIM_MINUS1;
        break;
    case SET_STAT_BUFF_VALUE(2) | STAT_BUFF_NEGATIVE: // -2
        value = STAT_ANIM_MINUS2;
        break;
    }
    gBattleScripting.animArg1 = GET_STAT_BUFF_ID(gBattleScripting.statChanger) + value - 1;
    gBattleScripting.animArg2 = 0;
    ++gBattlescriptCurrInstr;
}

static void atk48_playstatchangeanimation(void)
{
	u32 currStat = 0;
	u16 statAnimId = 0;
	s32 changeableStatsCount = 0;
	u8 statsToCheck = 0;
	u8 flags = gBattlescriptCurrInstr[3];

	gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1));
	statsToCheck = T2_READ_8(gBattlescriptCurrInstr + 2);

	if (ABILITY(gActiveBattler) == ABILITY_SIMPLE)
		flags |= ATK48_STAT_BY_TWO;

	if (flags & ATK48_STAT_NEGATIVE) // goes down
	{
        s16 startingStatAnimId;
		ability_t ability;
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY)
			goto STAT_ANIM_UP_;

	STAT_ANIM_DOWN_:
		ability = ABILITY(gActiveBattler);

		if (flags & ATK48_STAT_BY_TWO)
			startingStatAnimId = STAT_ANIM_MINUS2 - 1;
		else
			startingStatAnimId = STAT_ANIM_MINUS1 - 1;

		while (statsToCheck != 0) {

			if (statsToCheck & 1)
			{
				if (flags & ATK48_DONT_CHECK_LOWER)
				{
					if (gBattleMons[gActiveBattler].statStages[currStat - 1] > 0)
					{
						statAnimId = startingStatAnimId + currStat;
						changeableStatsCount++;
					}
				}
				else if (!BankSideHasMist(gActiveBattler)
						&& ability != ABILITY_CLEAR_BODY
						&& ability != ABILITY_WHITE_SMOKE
						//&& ability != ABILITY_FULLMETALBODY
						&& !(ability == ABILITY_KEEN_EYE && currStat == STAT_STAGE_ACC)
						&& !(ability == ABILITY_HYPER_CUTTER && currStat == STAT_STAGE_ATK)
						&& !(ability == ABILITY_BIGPECKS && currStat == STAT_STAGE_DEF)
						&& !(ability == ABILITY_INNER_FOCUS && gBattleStruct->intimidateActive)
						&& !(ability == ABILITY_OWN_TEMPO && gBattleStruct->intimidateActive)
						&& !(ability == ABILITY_OBLIVIOUS && gBattleStruct->intimidateActive)
						&& !(ability == ABILITY_SCRAPPY && gBattleStruct->intimidateActive)
						&& !(ability == ABILITY_MIRRORARMOR && gBattlerAttacker != gBattlerTarget && gActiveBattler == gBattlerTarget))
				{
					if (STAT_STAGE(gActiveBattler, currStat) > STAT_STAGE_MIN)
					{
						statAnimId = startingStatAnimId + currStat;
						changeableStatsCount++;
					}
				}
			}
			statsToCheck >>= 1, ++currStat;
		}

		if (changeableStatsCount > 1) // more than one stat, so the color is gray
		{
			if (flags & ATK48_STAT_BY_TWO)
				statAnimId = STAT_ANIM_MULTIPLE_MINUS2;
			else
				statAnimId = STAT_ANIM_MULTIPLE_MINUS1;
		}
	}

	else // goes up
	{
        s16 startingStatAnimId;
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY)
			goto STAT_ANIM_DOWN_;

	STAT_ANIM_UP_:
		if (flags & ATK48_STAT_BY_TWO)
			startingStatAnimId = STAT_ANIM_PLUS2 - 1;
		else
			startingStatAnimId = STAT_ANIM_PLUS1 - 1;

		while (statsToCheck != 0)
		{
			if (statsToCheck & 1 && gBattleMons[gActiveBattler].statStages[currStat - 1] < 12)
			{
				statAnimId = startingStatAnimId + currStat;
				changeableStatsCount++;
			}
			statsToCheck >>= 1, ++currStat;
		}

		if (changeableStatsCount > 1) // more than one stat, so the color is gray
		{
			if (flags & ATK48_STAT_BY_TWO)
				statAnimId = STAT_ANIM_MULTIPLE_PLUS2;
			else
				statAnimId = STAT_ANIM_MULTIPLE_PLUS1;
		}
	}

	if (gBattleStruct->SpectralThiefActive)
	{
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY)
		{
			statAnimId = STAT_ANIM_MULTIPLE_MINUS1; //Invert stat changes
			changeableStatsCount = 0xFF;
		}
		else
		{
			statAnimId = STAT_ANIM_MULTIPLE_PLUS1;
			changeableStatsCount = 0xFF;
		}
	}

	if ((T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_ONLY_MULTIPLE && changeableStatsCount < 2)
	|| (T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_ONLY_TRIPLE && changeableStatsCount < 3))
	{
		gBattlescriptCurrInstr += 4;
	}
	else if (changeableStatsCount != 0 && !gBattleScripting.statAnimPlayed)
	{
		BtlController_EmitBattleAnimation(0, B_ANIM_STATS_CHANGE, statAnimId);
		MarkBattlerForControllerExec(gActiveBattler);
		if ((T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_ONLY_MULTIPLE && changeableStatsCount > 1)
		||  (T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_ONLY_TRIPLE && changeableStatsCount > 2))
			gBattleScripting.statAnimPlayed = TRUE;
		gBattlescriptCurrInstr += 4;
	}
	else
	{
		gBattlescriptCurrInstr += 4;
	}
}

enum
{
	ATK49_SET_UP,
	ATK49_ATTACKER_ABILITIES,
	ATK49_ADVERSE_PROTECTION,
	ATK49_RAGE,
	ATK49_SYNCHRONIZE_TARGET,
	ATK49_BEAK_BLAST_BURN,
	ATK49_SYNCHRONIZE_ATTACKER,
	ATK49_MOVE_END_ABILITIES,
	ATK49_SYNCHRONIZE_ATTACKER_2,
	ATK49_ITEM_EFFECTS_CONTACT_TARGET,

//Above are all done in prefaintmoveendeffects
//They are done here if they didn't activate before.

	ATK49_UNDO_SKY_DROP,
	ATK49_UNDO_SKY_DROP_2,
	ATK49_ATTACKER_INVISIBLE,
	ATK49_TARGET_INVISIBLE,
	ATK49_ATTACKER_VISIBLE,
	ATK49_TARGET_VISIBLE,
	ATK49_CHOICE_MOVE,
	ATK49_PLUCK,
	ATK49_STICKYHOLD,
	ATK49_ITEM_EFFECTS_END_TURN_TARGET,
	ATK49_ITEM_EFFECTS_KINGS_ROCK,
	ATK49_ITEM_EFFECTS_END_TURN_ATTACKER,
	ATK49_STATUS_IMMUNITY_ABILITIES,
	ATK49_UPDATE_LAST_MOVES,
	ATK49_MIRROR_MOVE,
	ATK49_MULTI_HIT_MOVES,
	ATK49_RAID_SHIELD_BREAK,
	ATK49_DEFROST,
	ATK49_SECOND_MOVE_EFFECT,
	ATK49_MAGICIAN_MOXIE_BATTLEBOND,
	ATK49_FATIGUE,
	ATK49_ITEM_EFFECTS_END_TURN_ATTACKER_2,
	ATK49_NEXT_TARGET,
	ATK49_MOVE_RECOIL,
	ATK49_ITEM_EFFECTS_END_TURN_ATTACKER_3,
	ATK49_EJECT_BUTTON,
	ATK49_RED_CARD,
	ATK49_EJECT_PACK,
	ATK49_SHELL_BELL_LIFE_ORB_RECOIL,
	ATK49_SWITCH_OUT_ABILITIES,
	ATK49_RESTORE_ABILITIES,
	ATK49_PICKPOCKET,
	ATK49_RAID_MON_PREP_MORE_ATTACKS,
	ATK49_SUBSTITUTE,
	ATK49_END_ZMOVES,
	ATK49_DANCER,
	ATK49_RAID_SHIELD,
	ATK49_COUNT,
};

static bool8 CanDoMoveEndSwitchout(u8 arg)
{
	return arg != ARG_IN_PURSUIT
		&& arg != ARG_DRAGON_TAIL
		&& arg != ARG_PARTING_SHOT
		&& (SPLIT(gCurrentMove) != SPLIT_STATUS || gBattleMoves[gCurrentMove].effect != EFFECT_ROAR); //Dragon Tail & Circle Throw are handled in line above
}

void SortBanksBySpeed(u8 banks[], bool8 slowToFast)
{
	int i, j, key, keyBank;
	u16 speeds[4] = {0};
	for (i = 0; i < gBattlersCount; ++i)
		speeds[i] = SpeedCalc(banks[i]);

	for (i = 1; i < gBattlersCount; ++i)
	{
		keyBank = banks[i];
		key = speeds[i];
		j = i - 1;

		if (slowToFast)
		{
			while (j >= 0 && speeds[j] > key)
			{
				banks[j + 1] = banks[j];
				speeds[j + 1] = speeds[j];
				j = j - 1;
			}
		}
		else
		{
			while (j >= 0 && speeds[j] < key)
			{
				banks[j + 1] = banks[j];
				speeds[j + 1] = speeds[j];
				j = j - 1;
			}
		}

		banks[j + 1] = keyBank;
		speeds[j + 1] = key;
	}
}
/*
// atk49, moveend cases
#define ATK49_RAGE                              0
#define ATK49_DEFROST                           1
#define ATK49_SYNCHRONIZE_TARGET                2
#define ATK49_MOVE_END_ABILITIES                3
#define ATK49_STATUS_IMMUNITY_ABILITIES         4
#define ATK49_SYNCHRONIZE_ATTACKER              5
#define ATK49_CHOICE_MOVE                       6
#define ATK49_CHANGED_ITEMS                     7
#define ATK49_ATTACKER_INVISIBLE                8
#define ATK49_ATTACKER_VISIBLE                  9
#define ATK49_TARGET_VISIBLE                    10
#define ATK49_ITEM_EFFECTS_ALL                  11
#define ATK49_KINGSROCK_SHELLBELL               12
#define ATK49_SUBSTITUTE                        13
#define ATK49_UPDATE_LAST_MOVES                 14
#define ATK49_MIRROR_MOVE                       15
#define ATK49_NEXT_TARGET                       16
#define ATK49_COUNT                             17

static void atk49_moveend(void)
{
    s32 i;
    bool32 effect = FALSE;
    u8 moveType = 0;
    u8 holdEffectAtk = 0;
    u16 *choicedMoveAtk = NULL;
    u8 arg1, arg2;
    u16 originallyUsedMove;

    if (gChosenMove == 0xFFFF)
        originallyUsedMove = MOVE_NONE;
    else
        originallyUsedMove = gChosenMove;
    arg1 = gBattlescriptCurrInstr[1];
    arg2 = gBattlescriptCurrInstr[2];
    if (gBattleMons[gBattlerAttacker].item == ITEM_ENIGMA_BERRY)
        holdEffectAtk = gEnigmaBerries[gBattlerAttacker].holdEffect;
    else
        holdEffectAtk = ItemId_GetHoldEffect(gBattleMons[gBattlerAttacker].item);
    choicedMoveAtk = &gBattleStruct->choicedMove[gBattlerAttacker];
    GET_MOVE_TYPE(gCurrentMove, moveType);
    do
    {
        switch (gBattleScripting.atk49_state)
        {
        case ATK49_RAGE: // rage check
            if (gBattleMons[gBattlerTarget].status2 & STATUS2_RAGE
             && gBattleMons[gBattlerTarget].hp != 0
             && gBattlerAttacker != gBattlerTarget
             && GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget)
             && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
             && TARGET_TURN_DAMAGED
             && gBattleMoves[gCurrentMove].power
             && gBattleMons[gBattlerTarget].statStages[STAT_ATK] <= 0xB)
            {
                ++gBattleMons[gBattlerTarget].statStages[STAT_ATK];
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_RageIsBuilding;
                effect = TRUE;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_DEFROST: // defrosting check
            if (gBattleMons[gBattlerTarget].status1 & STATUS1_FREEZE
             && gBattleMons[gBattlerTarget].hp != 0
             && gBattlerAttacker != gBattlerTarget
             && gSpecialStatuses[gBattlerTarget].specialDmg
             && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
             && moveType == TYPE_FIRE)
            {
                gBattleMons[gBattlerTarget].status1 &= ~(STATUS1_FREEZE);
                gActiveBattler = gBattlerTarget;
                BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gBattlerTarget].status1);
                MarkBattlerForControllerExec(gActiveBattler);
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_DefrostedViaFireMove;
                effect = TRUE;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_SYNCHRONIZE_TARGET: // target synchronize
            if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBattlerTarget, 0, 0, 0))
                effect = TRUE;
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_MOVE_END_ABILITIES: // Such as abilities activating on contact(Poison Spore, Rough Skin, etc.).
            if (AbilityBattleEffects(ABILITYEFFECT_MOVE_END, gBattlerTarget, 0, 0, 0))
                effect = TRUE;
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_STATUS_IMMUNITY_ABILITIES: // status immunities
            if (AbilityBattleEffects(ABILITYEFFECT_IMMUNITY, 0, 0, 0, 0))
                effect = TRUE; // it loops through all battlers, so we increment after its done with all battlers
            else
                ++gBattleScripting.atk49_state;
            break;
        case ATK49_SYNCHRONIZE_ATTACKER: // attacker synchronize
            if (AbilityBattleEffects(ABILITYEFFECT_ATK_SYNCHRONIZE, gBattlerAttacker, 0, 0, 0))
                effect = TRUE;
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_CHOICE_MOVE: // update choice band move
            if (gHitMarker & HITMARKER_OBEYS
             && holdEffectAtk == HOLD_EFFECT_CHOICE_BAND
             && gChosenMove != MOVE_STRUGGLE 
             && (*choicedMoveAtk == 0 || *choicedMoveAtk == 0xFFFF))
            {
                if (gChosenMove == MOVE_BATON_PASS && !(gMoveResultFlags & MOVE_RESULT_FAILED))
                {
                    ++gBattleScripting.atk49_state;
                    break;
                }
                *choicedMoveAtk = gChosenMove;
            }
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
                if (gBattleMons[gBattlerAttacker].moves[i] == *choicedMoveAtk)
                    break;
            }
            if (i == MAX_MON_MOVES)
                *choicedMoveAtk = 0;
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_CHANGED_ITEMS: // changed held items
            for (i = 0; i < gBattlersCount; ++i)
            {
                u16 *changedItem = &gBattleStruct->changedItems[i];
                
                if (*changedItem != 0)
                {
                    gBattleMons[i].item = *changedItem;
                    *changedItem = 0;
                }
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_ITEM_EFFECTS_ALL: // item effects for all battlers
            if (ItemBattleEffects(ITEMEFFECT_MOVE_END, 0, FALSE, 0))
                effect = TRUE;
            else
                ++gBattleScripting.atk49_state;
            break;
        case ATK49_KINGSROCK_SHELLBELL: // king's rock and shell bell
            if (ItemBattleEffects(ITEMEFFECT_KINGSROCK_SHELLBELL, 0, FALSE, 0))
                effect = TRUE;
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_ATTACKER_INVISIBLE: // make attacker sprite invisible
            if (gStatuses3[gBattlerAttacker] & (STATUS3_SEMI_INVULNERABLE)
             && gHitMarker & HITMARKER_NO_ANIMATIONS)
            {
                gActiveBattler = gBattlerAttacker;
                BtlController_EmitSpriteInvisibility(0, TRUE);
                MarkBattlerForControllerExec(gActiveBattler);
                ++gBattleScripting.atk49_state;
                return;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_ATTACKER_VISIBLE: // make attacker sprite visible
            if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT
             || !(gStatuses3[gBattlerAttacker] & (STATUS3_SEMI_INVULNERABLE))
             || WasUnableToUseMove(gBattlerAttacker))
            {
                gActiveBattler = gBattlerAttacker;
                BtlController_EmitSpriteInvisibility(0, FALSE);
                MarkBattlerForControllerExec(gActiveBattler);
                gStatuses3[gBattlerAttacker] &= ~(STATUS3_SEMI_INVULNERABLE);
                gSpecialStatuses[gBattlerAttacker].restoredBattlerSprite = 1;
                ++gBattleScripting.atk49_state;
                return;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_TARGET_VISIBLE: // make target sprite visible
            if (!gSpecialStatuses[gBattlerTarget].restoredBattlerSprite
             && gBattlerTarget < gBattlersCount
             && !(gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE))
            {
                gActiveBattler = gBattlerTarget;
                BtlController_EmitSpriteInvisibility(0, FALSE);
                MarkBattlerForControllerExec(gActiveBattler);
                gStatuses3[gBattlerTarget] &= ~(STATUS3_SEMI_INVULNERABLE);
                ++gBattleScripting.atk49_state;
                return;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_SUBSTITUTE: // update substitute
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (gDisableStructs[i].substituteHP == 0)
                    gBattleMons[i].status2 &= ~(STATUS2_SUBSTITUTE);
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_UPDATE_LAST_MOVES:
            if (gHitMarker & HITMARKER_SWAP_ATTACKER_TARGET)
            {
					for(;;i++)
						i--;
                gActiveBattler = gBattlerAttacker;
                gBattlerAttacker = gBattlerTarget;
                gBattlerTarget = gActiveBattler;
                gHitMarker &= ~(HITMARKER_SWAP_ATTACKER_TARGET);
				
            }
            if (gHitMarker & HITMARKER_ATTACKSTRING_PRINTED)
            {
                gLastPrintedMoves[gBattlerAttacker] = gChosenMove;
            }
            if (!(gAbsentBattlerFlags & gBitTable[gBattlerAttacker])
             && !(gBattleStruct->absentBattlerFlags & gBitTable[gBattlerAttacker])
             && gBattleMoves[originallyUsedMove].effect != EFFECT_BATON_PASS)
            {
                if (gHitMarker & HITMARKER_OBEYS)
                {
                    gLastMoves[gBattlerAttacker] = gChosenMove;
                    gLastResultingMoves[gBattlerAttacker] = gCurrentMove;
                }
                else
                {
                    gLastMoves[gBattlerAttacker] = 0xFFFF;
                    gLastResultingMoves[gBattlerAttacker] = 0xFFFF;
                }

                if (!(gHitMarker & HITMARKER_FAINTED(gBattlerTarget)))
                    gLastHitBy[gBattlerTarget] = gBattlerAttacker;

                if (gHitMarker & HITMARKER_OBEYS && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
                {
                    if (gChosenMove == 0xFFFF)
                    {
                        gLastLandedMoves[gBattlerTarget] = gChosenMove;
                    }
                    else
                    {
                        gLastLandedMoves[gBattlerTarget] = gCurrentMove;
                        GET_MOVE_TYPE(gCurrentMove, gLastHitByType[gBattlerTarget]);
                    }
                }
                else
                {
                    gLastLandedMoves[gBattlerTarget] = 0xFFFF;
                }
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_MIRROR_MOVE: // mirror move
            if (!(gAbsentBattlerFlags & gBitTable[gBattlerAttacker])
             && !(gBattleStruct->absentBattlerFlags & gBitTable[gBattlerAttacker])
             && gBattleMoves[originallyUsedMove].flags & FLAG_MIRROR_MOVE_AFFECTED
             && gHitMarker & HITMARKER_OBEYS
             && gBattlerAttacker != gBattlerTarget
             && !(gHitMarker & HITMARKER_FAINTED(gBattlerTarget))
             && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
            {
                u8 target, attacker;

                *(gBattleStruct->lastTakenMove + gBattlerTarget * 2 + 0) = gChosenMove;
                *(gBattleStruct->lastTakenMove + gBattlerTarget * 2 + 1) = gChosenMove >> 8;
                target = gBattlerTarget;
                attacker = gBattlerAttacker;
                gBattleStruct->lastTakenMoveFrom[target][gBattlerAttacker] = gChosenMove;
                target = gBattlerTarget;
                attacker = gBattlerAttacker;
                gBattleStruct->lastTakenMoveFrom[target][gBattlerAttacker] = gChosenMove;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_NEXT_TARGET: // For moves hitting two opposing Pokemon.
            if (!(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
             && gBattleTypeFlags & BATTLE_TYPE_DOUBLE
             && !gProtectStructs[gBattlerAttacker].chargingTurn
             && gBattleMoves[gCurrentMove].target == MOVE_TARGET_BOTH
             && !(gHitMarker & HITMARKER_NO_ATTACKSTRING))
            {
                u8 battlerId = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gBattlerTarget)));
                if (gBattleMons[battlerId].hp != 0)
                {

                    gBattlerTarget = battlerId;
                    gHitMarker |= HITMARKER_NO_ATTACKSTRING;
                    gBattleScripting.atk49_state = 0;
                    MoveValuesCleanUp();
                    BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
                    gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
                    return;
                }
                else
                {
                    gHitMarker |= HITMARKER_NO_ATTACKSTRING;
                }
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_COUNT:
            break;
        }
        if (arg1 == 1 && effect == FALSE)
            gBattleScripting.atk49_state = ATK49_COUNT;
        if (arg1 == 2 && arg2 == gBattleScripting.atk49_state)
            gBattleScripting.atk49_state = ATK49_COUNT;
    }
    while (gBattleScripting.atk49_state != ATK49_COUNT && effect == FALSE);
    if (gBattleScripting.atk49_state == ATK49_COUNT && effect == FALSE)
        gBattlescriptCurrInstr += 3;
}*/


static void atk49_moveend(void)
{
    int i;
	u32 effect = FALSE;
	u8 moveType = 0;
	u8 holdEffectAtk = 0;
	u16* choicedMoveAtk = NULL;
	u8 arg1, arg2;
	u16 originallyUsedMove;
	u8 bank;
	u8 battlerId;

	u8 bankDef = gBattlerTarget;

	if (gChosenMove == 0xFFFF)
		originallyUsedMove = 0;
	else
		originallyUsedMove = gChosenMove;

	arg1 = gBattlescriptCurrInstr[1];
	arg2 = gBattlescriptCurrInstr[2];

	holdEffectAtk = ITEM_EFFECT(gBattlerAttacker);

	if (gBattleStruct->MoveBounceInProgress)
		choicedMoveAtk = &gBattleStruct->choicedMove[gBattlerTarget];
	else
		choicedMoveAtk = &gBattleStruct->choicedMove[gBattlerAttacker];

	moveType = gBattleStruct->dynamicMoveType;

	if (gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
	{ 	//Cancel the Z-Moves
		gBattleStruct->zMoveData.active = FALSE;
		gBattleStruct->zMoveData.toBeUsed[gBattlerAttacker] = 0;
		gBattleStruct->dynamaxData.active = FALSE;
	}

	if (arg1 == ARG_ONLY_EMERGENCY_EXIT)
		gBattleScripting.atk49_state = ATK49_SWITCH_OUT_ABILITIES;

	if (gBattleScripting.atk49_state == ATK49_SET_UP //If these have already
	&&  gBattleStruct->preFaintEffectsState == FAINT_COUNT) //been done
	{
		gBattleStruct->originalAttackerBackup = gBattlerAttacker;
		gBattleScripting.atk49_state = ATK49_UNDO_SKY_DROP;
	}

	do
	{
		switch (gBattleScripting.atk49_state)
		{
		case ATK49_SET_UP: //For Emergency Exit to use later
			gBattleStruct->originalAttackerBackup = gBattlerAttacker;

			if (gBattleStruct->MultiHitOn)
				gBattleStruct->turnDamageTaken[gBattlerTarget] += gHpDealt; //Total up damage taken
			else
				gBattleStruct->turnDamageTaken[gBattlerTarget] = gHpDealt;

			gBattleStruct->totalDamageGiven += gHpDealt;
			gBattleStruct->ResultFlags[gBattlerTarget] = gMoveResultFlags;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ATTACKER_ABILITIES:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&& TOOK_DAMAGE(gBattlerTarget)
			&& MOVE_HAD_EFFECT
			&& BATTLER_ALIVE(gBattlerTarget)
			&& !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, gBattlerTarget))
			{
				switch (ABILITY(gBattlerAttacker)) {
					case ABILITY_STENCH: //Check for Stench is taken care of in King's Rock check
						if ((Random() % 100) < 10
						&& gCurrentTurnActionNumber < GetBattlerTurnOrderNum(gBattlerTarget)) //Attacker moved before target
						{
							gBattleMons[gBattlerTarget].status2 |= STATUS2_FLINCHED;
						}
						break;

					case ABILITY_POISONTOUCH:
						if (CheckContact(gCurrentMove, gBattlerAttacker)
						&& ABILITY(gBattlerTarget) != ABILITY_SHIELDDUST
						&& CanBePoisoned(gBattlerTarget, gBattlerAttacker, TRUE)
						&& (Random() % 100) < 30)
						{
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_PoisonTouch;
							effect = TRUE;
						}
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ADVERSE_PROTECTION:
			if (gProtectStructs[gBattlerTarget].kingsshield_damage)
			{
				gProtectStructs[gBattlerTarget].kingsshield_damage = 0;

				if (BATTLER_ALIVE(gBattlerAttacker) && STAT_CAN_FALL(gBattlerAttacker, STAT_ATK))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_KingsShield;
					effect = TRUE;
					break;
				}
			}

			if (gProtectStructs[gBattlerTarget].spikyshield_damage)
			{
				gProtectStructs[gBattlerTarget].spikyshield_damage = 0;

				if (BATTLER_ALIVE(gBattlerAttacker) && ABILITY(gBattlerAttacker) != ABILITY_MAGICGUARD)
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBattlerAttacker) / 8);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_SpikyShield;
					effect = TRUE;
					break;
				}
			}

			if (gProtectStructs[gBattlerTarget].banefulbunker_damage)
			{
				gProtectStructs[gBattlerTarget].banefulbunker_damage = 0;

				if (BATTLER_ALIVE(gBattlerAttacker) && CanBePoisoned(gBattlerAttacker, gBattlerTarget, TRUE)) //Target poisons Attacker
				{
					gBattleMons[gBattlerAttacker].status1 = STATUS1_POISON;
					gEffectBattler = gActiveBattler = gBattlerAttacker;
					BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gBattlerAttacker].status1);
					MarkBattlerForControllerExec(gActiveBattler);

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_BanefulBunker;
					effect = TRUE;
					break;
				}
			}

			if (gProtectStructs[gBattlerTarget].obstructDamage)
			{
				gProtectStructs[gBattlerTarget].obstructDamage = FALSE;

				if (BATTLER_ALIVE(gBattlerAttacker) && STAT_CAN_FALL(gBattlerAttacker, STAT_DEF))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_ObstructStatDecrement;
					effect = TRUE;
					break;
				}
			}

			gBattleScripting.atk49_state++;
			break;

		case ATK49_RAGE: // rage check
			if (gBattleMons[gBattlerTarget].status2 & STATUS2_RAGE
			&& BATTLER_ALIVE(gBattlerTarget)
			&& gBattlerAttacker != gBattlerTarget
			&& SIDE(gBattlerAttacker) != SIDE(gBattlerTarget)
			&& MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBattlerTarget)
			&& SPLIT(gCurrentMove) != SPLIT_STATUS
			&& STAT_CAN_RISE(gBattlerTarget, STAT_ATK))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_RageIsBuilding;
				effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_SYNCHRONIZE_TARGET: // target synchronize
			if (gCurrentMove != MOVE_PSYCHOSHIFT || !MOVE_HAD_EFFECT) //The lazy way of taking care of Psycho Shift Status Transfer->Synchronize->Heal Status
			{
				if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBattlerTarget, 0, 0, 0))
					effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_BEAK_BLAST_BURN:
			if (CheckContact(gCurrentMove, gBattlerAttacker)
			&& MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBattlerTarget)
			&& gBattleStruct->BeakBlastByte & gBitTable[gBattlerTarget]
			&& CanBeBurned(gBattlerAttacker, TRUE))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_BeakBlastBurn;
				effect = TRUE;
			}

			gBattleScripting.atk49_state++;
			break;

		case ATK49_SYNCHRONIZE_ATTACKER: // attacker synchronize
		case ATK49_SYNCHRONIZE_ATTACKER_2:
			if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBattlerAttacker, 0, 0, 0))
				effect = TRUE;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_MOVE_END_ABILITIES: // Such as abilities activating on contact(Poison Spore, Rough Skin, etc.).
			if (AbilityBattleEffects(ABILITYEFFECT_MOVE_END, gBattlerTarget, 0, 0, 0))
				effect = TRUE;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ITEM_EFFECTS_CONTACT_TARGET:
			if (gBattleMoves[gCurrentMove].effect != EFFECT_EAT_BERRY //Skip Berries for Pluck to eat
			|| gCurrentMove == MOVE_INCINERATE
			|| ITEM_POCKET(gBattlerTarget) != POCKET_BERRY_POUCH
			|| ITEM_EFFECT(gBattlerTarget) == HOLD_EFFECT_JABOCA_ROWAP_BERRY) //Only berries that activate before pluck
			{
				if (gBattleMoves[gCurrentMove].effect != EFFECT_KNOCK_OFF
				|| (ITEM_EFFECT(gBattlerTarget) != HOLD_EFFECT_KEE_BERRY && ITEM_EFFECT(gBattlerTarget) != HOLD_EFFECT_MARANGA_BERRY)) //Only contact items that don't activate first
				{
					if (ItemBattleEffects(ItemEffects_ContactTarget, gBattlerTarget, TRUE, FALSE))
						effect = TRUE;
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_UNDO_SKY_DROP:
		{
			u8 bankToFree;

			for (i = 0; i < gBattlersCount; ++i)
			{
				if (gBattleMons[i].hp == 0)
				{
					if (gStatuses3[gBattleStruct->skyDropAttackersTarget[gBattlerTarget]] & STATUS3_SKY_DROP_TARGET
					&&  gBattleStruct->skyDropTargetsAttacker[gBattleStruct->skyDropAttackersTarget[gBattlerTarget]] == gBattlerTarget)
					{
						bankToFree = gBattleStruct->skyDropAttackersTarget[i];
						gBattleStruct->skyDropAttackersTarget[i] = 0;
						gBattleStruct->skyDropTargetsAttacker[bankToFree] = 0;

						//A message is only printed when the target is freed.
						gBattleScripting.battler = bankToFree;
						gBattleStringLoader = FreedFromSkyDropString;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_PrintCustomString;
						effect = TRUE;

						gActiveBattler = bankToFree;
						BtlController_EmitSpriteInvisibility(0, FALSE);
						MarkBattlerForControllerExec(gActiveBattler);
					}
					else
						continue;

					gStatuses3[i] &= ~(STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET | STATUS3_ON_AIR);
					gStatuses3[bankToFree] &= ~(STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET | STATUS3_ON_AIR);
					break;
				}
			}

			gBattleScripting.atk49_state++;
		}
			break;

		case ATK49_UNDO_SKY_DROP_2: //The attacker can no longer attack while in the air due to paralysis etc.
			if (gCurrentMove == MOVE_SKYDROP
			&& gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE
			&& gStatuses3[gBattlerAttacker] & STATUS3_SKY_DROP_ATTACKER)
			{
				gStatuses3[gBattlerAttacker] &= ~STATUS3_SKY_DROP_ATTACKER;
				gStatuses3[gBattlerTarget] &= ~STATUS3_SKY_DROP_TARGET;
				gBattleScripting.battler = gBattlerTarget;
				gBattleStringLoader = FreedFromSkyDropString;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ATTACKER_INVISIBLE: // make attacker sprite invisible
			if (gStatuses3[gBattlerAttacker] & (STATUS3_SEMI_INVULNERABLE)
			&&  gHitMarker & HITMARKER_NO_ANIMATIONS)
			{
				gActiveBattler = gBattlerAttacker;
				 BtlController_EmitSpriteInvisibility(0, TRUE);
				MarkBattlerForControllerExec(gActiveBattler);
				gBattleScripting.atk49_state++;
				return;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_TARGET_INVISIBLE: // make target sprite invisible
			if (gStatuses3[gBattlerTarget] & (STATUS3_SEMI_INVULNERABLE)
			&&  gHitMarker & HITMARKER_NO_ANIMATIONS)
			{
				gActiveBattler = gBattlerTarget;
				 BtlController_EmitSpriteInvisibility(0, TRUE);
				MarkBattlerForControllerExec(gActiveBattler);
				gBattleScripting.atk49_state++;
				return;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ATTACKER_VISIBLE: // make attacker sprite visible
			if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT
			|| !(gStatuses3[gBattlerAttacker] & (STATUS3_SEMI_INVULNERABLE))
			|| WasUnableToUseMove(gBattlerAttacker))
			{
				gActiveBattler = gBattlerAttacker;
				BtlController_EmitSpriteInvisibility(0, FALSE);
				MarkBattlerForControllerExec(gActiveBattler);
				gStatuses3[gBattlerAttacker] &= ~(STATUS3_SEMI_INVULNERABLE);
				gSpecialStatuses[gBattlerAttacker].restoredBattlerSprite = 1;
				gBattleScripting.atk49_state++;
				return;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_TARGET_VISIBLE: // make target sprite visible
			if (!gSpecialStatuses[gBattlerTarget].restoredBattlerSprite
			&& gBattlerTarget < gBattlersCount
			&& !(gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE))
			{
				gActiveBattler = gBattlerTarget;
				BtlController_EmitSpriteInvisibility(0, FALSE);
				MarkBattlerForControllerExec(gActiveBattler);
				gStatuses3[gBattlerTarget] &= ~(STATUS3_SEMI_INVULNERABLE);
				gBattleScripting.atk49_state++;
				return;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_CHOICE_MOVE: // update choice band move
			if (arg1 != ARG_IN_FUTURE_ATTACK && !IsDynamaxed(gBattlerAttacker))
			{
				if (gChosenMove != MOVE_STRUGGLE)
				{
					if (gHitMarker & HITMARKER_OBEYS)
					{
						if (holdEffectAtk == HOLD_EFFECT_CHOICE_BAND || ABILITY(gBattlerAttacker) == ABILITY_GORILLATACTICS)
						{
							if (*choicedMoveAtk == 0 || *choicedMoveAtk == 0xFFFF)
							{
								if (gChosenMove == MOVE_BATONPASS && !(gMoveResultFlags & MOVE_RESULT_FAILED))
								{
									gBattleScripting.atk49_state++;
									break;
								}

								*choicedMoveAtk = gChosenMove;
							}
						}
						else //This should remove the choice lock glitch
							*choicedMoveAtk = 0;
					}

					if (!MoveInMoveset(*choicedMoveAtk, gBattlerAttacker))
						*choicedMoveAtk = 0;
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_PLUCK:
			if (MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBattlerTarget)
			&& gBattleMoves[gCurrentMove].effect == EFFECT_EAT_BERRY
			&& gCurrentMove != MOVE_INCINERATE
			&& IsBerry(ITEM(gBattlerTarget))
			&& !CheckTableForItem(ITEM(gBattlerTarget), gBannedBattleEatBerries)
			&& ABILITY(gBattlerTarget) != ABILITY_STICKYHOLD
			&& (!BATTLER_ALIVE(gBattlerTarget) || !(ITEM_EFFECT(gBattlerTarget) == HOLD_EFFECT_JABOCA_ROWAP_BERRY && ITEM_QUALITY(gBattlerTarget) == CalcMoveSplit(gBattlerAttacker, gCurrentMove))))
			{
				gBattleStruct->BelchCounters |= gBitTable[gBattlerPartyIndexes[gBattlerAttacker]];

				gLastUsedItem = gBattleMons[gBattlerTarget].item;
				gBattleMons[gBattlerTarget].item = 0;
				HandleUnburdenBoost(gBattlerTarget); //Give target Unburden boost

				gActiveBattler = gBattlerTarget;
				BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
				MarkBattlerForControllerExec(gActiveBattler);

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PluckEat;

				effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_STICKYHOLD:
			if (MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBattlerTarget)
			&& (gBattleMoves[gCurrentMove].effect == EFFECT_KNOCK_OFF || gBattleMoves[gCurrentMove].effect == EFFECT_THIEF)
			&& CanKnockOffItem(gBattlerTarget)
			&& ABILITY(gBattlerTarget) == ABILITY_STICKYHOLD
			&& BATTLER_ALIVE(gBattlerTarget))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_StickyHoldActivatesRet;
				effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ITEM_EFFECTS_END_TURN_TARGET:
			if ((ABILITY(gBattlerTarget) == ABILITY_STICKYHOLD && BATTLER_ALIVE(gBattlerTarget))
			||  !BATTLER_ALIVE(gBattlerAttacker)
			||  !CanKnockOffItem(gBattlerTarget)
			||  gBattleMoves[gCurrentMove].effect != EFFECT_KNOCK_OFF) //Don't activate items that should be knocked off
			{
				if (ItemBattleEffects(ItemEffects_EndTurn, gBattlerTarget, TRUE, FALSE))
					effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ITEM_EFFECTS_KINGS_ROCK:
			if (arg1 != ARG_IN_FUTURE_ATTACK && ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_FLINCH) //King's Rock Only
			{
				if (ItemBattleEffects(ItemEffects_ContactAttacker, gBattlerAttacker, TRUE, FALSE))
					effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_ITEM_EFFECTS_END_TURN_ATTACKER:
		case ATK49_ITEM_EFFECTS_END_TURN_ATTACKER_2:
		case ATK49_ITEM_EFFECTS_END_TURN_ATTACKER_3:
			if (ItemBattleEffects(ItemEffects_EndTurn, gBattlerAttacker, TRUE, FALSE))
				effect = TRUE;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_STATUS_IMMUNITY_ABILITIES: // status immunities
			if (AbilityBattleEffects(ABILITYEFFECT_IMMUNITY, 0, 0, 0, 0))
				effect = TRUE; // it loops through all battlers, so we increment after its done with all battlers
			else
				gBattleScripting.atk49_state++;
			break;

		case ATK49_UPDATE_LAST_MOVES:
			if (gBattleStruct->DancerInProgress
			|| arg1 == ARG_IN_FUTURE_ATTACK)
			{
				gBattleScripting.atk49_state++;
				break;
			}

			if (gHitMarker & HITMARKER_SWAP_ATTACKER_TARGET && arg1 != ARG_IN_PURSUIT)
			{
				bankDef = gBattlerAttacker;
				gBattlerAttacker = gBattlerTarget;
				gBattlerTarget = bankDef;
				gHitMarker &= ~(HITMARKER_SWAP_ATTACKER_TARGET);
			}

			if (gHitMarker & HITMARKER_ATTACKSTRING_PRINTED)
			{
				gBattleStruct->lastTargeted[gBattlerAttacker] = gBattlerTarget;
				if (IsZMove(gCurrentMove) || IsAnyMaxMove(gCurrentMove))
					gLastPrintedMoves[gBattlerAttacker] = gCurrentMove;
				else
					gLastPrintedMoves[gBattlerAttacker] = gChosenMove;

				if (!IsZMove(gCurrentMove) && !IsDynamaxed(gBattlerAttacker))
					gBattleStruct->usedMoveIndices[gBattlerAttacker] |= gBitTable[gCurrMovePos];
			}

			if (!(gAbsentBattlerFlags & gBitTable[gBattlerAttacker])
			&& !(gBattleStruct->absentBattlerFlags & gBitTable[gBattlerAttacker])
			&& gBattleMoves[originallyUsedMove].effect != EFFECT_BATON_PASS)
			{
				if (gHitMarker & HITMARKER_OBEYS)
				{
					gLastMoves[gBattlerAttacker] = gChosenMove;
					gLastResultingMoves[gBattlerAttacker] = gCurrentMove;
				}
				else
				{
					gLastMoves[gBattlerAttacker] = 0xFFFF;
					gLastResultingMoves[gBattlerAttacker] = 0xFFFF;
				}

				if (!(gHitMarker & HITMARKER_FAINTED(bankDef)))
					gLastHitBy[bankDef] = gBattlerAttacker;

				if (gHitMarker & HITMARKER_OBEYS && MOVE_HAD_EFFECT)
				{
					if (gChosenMove == 0xFFFF)
					{
						gLastLandedMoves[bankDef] = gChosenMove;
					}
					else
					{
						if (gBattleStruct->zMoveData.active)
							gLastLandedMoves[bankDef] = gChosenMove;

						else
							gLastLandedMoves[bankDef] = gCurrentMove;

						gLastHitByType[bankDef] = moveType;
					}
				}
				else
				{
					gLastLandedMoves[bankDef] = 0xFFFF;
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_MIRROR_MOVE:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&& !(gAbsentBattlerFlags & gBitTable[gBattlerAttacker])
			&& !(gBattleStruct->absentBattlerFlags & gBitTable[gBattlerAttacker])
			&& gBattleMoves[originallyUsedMove].flags & FLAG_MIRROR_MOVE_AFFECTED
			&& gHitMarker & HITMARKER_OBEYS
			&& gBattlerAttacker != bankDef
			&& !(gHitMarker & HITMARKER_FAINTED(bankDef))
			&& MOVE_HAD_EFFECT)
			{
				gBattleStruct->lastTakenMove[bankDef] = gChosenMove;
				gBattleStruct->lastTakenMoveFrom[bankDef][gBattlerAttacker] = gChosenMove;
			}

			if (gBattleStruct->zMoveData.active)
			{
				gBattleStruct->lastTakenMove[bankDef] = MOVE_NONE;
				gBattleStruct->lastTakenMoveFrom[bankDef][gBattlerAttacker] = MOVE_NONE;
			}

			gBattleScripting.atk49_state++;
			gSeedHelper[0] = 0; //Reset Seed Helper for Soul Heart
			break;

		case ATK49_MULTI_HIT_MOVES:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&&  arg1 != ARG_IN_PURSUIT
			&& !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
			&& !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
			&& gMultiHitCounter)
			{
				++gBattleScripting.multihitString[4];
				if (--gMultiHitCounter == 0)
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MultiHitPrintStrings;
					effect = 1;
				}
				else
				{
					if (gCurrentMove == MOVE_DRAGONDARTS
					&&  gBattleStruct->moveTarget[gBattlerAttacker] == gBattlerTarget //Haven't already changed targets
					&&  CanTargetPartner(gBattlerTarget)
					&&  !TargetFullyImmuneToCurrMove(PARTNER(gBattlerTarget)))
					{
						gBattlerTarget = PARTNER(gBattlerTarget); //Target the partner is doubles for second hit.
						PressurePPLose(gBattlerTarget, gBattlerAttacker, gChosenMove);
					}

					if (BATTLER_ALIVE(gBattlerAttacker)
					&& BATTLER_ALIVE(gBattlerTarget)
					&& (gChosenMove == MOVE_SLEEPTALK || !(gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP))
					&& !(gBattleMons[gBattlerAttacker].status1 & STATUS1_FREEZE))
					{
						if (gBattleStruct->ParentalBondOn)
							--gBattleStruct->ParentalBondOn;

						gHitMarker |= (HITMARKER_NO_PPDEDUCT | HITMARKER_NO_ATTACKSTRING);
						gBattleScripting.animTargetsHit = 0;
						gBattleStruct->preFaintEffectsState = 0;
						gBattleScripting.atk49_state = 0;
						gBattleStruct->MultiHitOn = TRUE; //Used to help second accuracy calcs
						MoveValuesCleanUp();
						BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
						gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
						return;
					}
					else
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MultiHitPrintStrings;
						effect = 1;
					}
				}
			}

			gBattleScripting.atk49_state++;
			gMultiHitCounter = 0;
			gBattleStruct->ParentalBondOn = 0;
			gBattleStruct->MultiHitOn = FALSE;
			break;

		case ATK49_RAID_SHIELD_BREAK: //Here b/c multi-hit moves only break one shield
		if (IsRaidBattle()
			&& GetBattlerPosition(gBattlerTarget) == B_POSITION_OPPONENT_LEFT
			&& MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBattlerTarget)
			&& gBattleStruct->dynamaxData.raidShieldsUp)
			{
				DestroyRaidShieldSprite();
				if (IsAnyMaxMove(gCurrentMove)
				|| IsZMove(gCurrentMove)
				|| gBattleMoves[gCurrentMove].effect == EFFECT_OHKO
				|| gBattleMoves[gCurrentMove].effect == EFFECT_BRICK_BREAK) //Unofficial addition
					DestroyRaidShieldSprite();
				if (IsZMove(gCurrentMove))
					DestroyRaidShieldSprite(); //Z-Moves destroy 3 shields

				if (gBattleStruct->dynamaxData.shieldsDestroyed >= gBattleStruct->dynamaxData.shieldCount)
				{
					gBattleStruct->dynamaxData.raidShieldsUp = FALSE;
					gBattleScripting.battler = gBattlerTarget;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_BrokenRaidBarrier;

					//Do some residual damage after the shattering
					gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 6;
					if (gBattleStruct->dynamaxData.shieldsDestroyed > gBattleStruct->dynamaxData.shieldCount)
						gBattleMoveDamage = (gBattleMoveDamage * 15) / 10;
					gBattleStruct->dynamaxData.turnStartHP = gBattleMons[gBattlerTarget].hp - gBattleMoveDamage; //No reactivating barrier yet
					effect = TRUE;
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_DEFROST: // defrosting check
			if (gBattleMons[bankDef].status1 & STATUS1_FREEZE
			&&  gBattleMons[bankDef].hp
			&&  gBattlerAttacker != bankDef
			&&  MOVE_HAD_EFFECT
			&&  TOOK_DAMAGE(bankDef)
			&& (moveType == TYPE_FIRE || CheckTableForMove(gCurrentMove, gMovesCanUnfreezeTarget)))
			{
				gBattleMons[bankDef].status1 &= ~(STATUS1_FREEZE);
				gActiveBattler = bankDef;
				BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bankDef].status1);
				MarkBattlerForControllerExec(gActiveBattler);
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_DefrostedViaFireMove;
				effect = TRUE;
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_SECOND_MOVE_EFFECT:
			gBattleCommunication[MOVE_EFFECT_BYTE] = gBattleStruct->backupMoveEffect;
			gBattleStruct->backupMoveEffect = 0;
			effect = SetMoveEffect2();
			gBattleScripting.atk49_state++;
			break;

		case ATK49_MAGICIAN_MOXIE_BATTLEBOND:
			switch (ABILITY(gBattlerAttacker)) {
				case ABILITY_MAGICIAN:
					if (ITEM(gBattlerAttacker) == ITEM_NONE
					&& ITEM(bankDef) != ITEM_NONE
					&& BATTLER_ALIVE(gBattlerAttacker)
					&& !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bankDef)
					&& TOOK_DAMAGE(bankDef)
					&& MOVE_HAD_EFFECT
					&& CanTransferItem(SPECIES(bankDef), ITEM(bankDef))
					&& CanTransferItem(SPECIES(gBattlerAttacker), ITEM(bankDef))
					&& (ABILITY(bankDef) != ABILITY_STICKYHOLD || !BATTLER_ALIVE(bankDef)))
					{
						gBattleScripting.battler = gBattlerAttacker;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Magician;
						effect = 1;
					}
					break;

				case ABILITY_MOXIE:
					if ((arg1 != ARG_IN_FUTURE_ATTACK || gWishFutureKnock.futureSightPartyIndex[bankDef] == gBattlerPartyIndexes[gBattlerAttacker])
					&& gBattleMons[bankDef].hp == 0
					&& BATTLER_ALIVE(gBattlerAttacker)
					&& TOOK_DAMAGE(bankDef)
					&& MOVE_HAD_EFFECT
					&& STAT_CAN_RISE(gBattlerAttacker, STAT_STAGE_ATK)
					&& ViableMonCountFromBank(FOE(gBattlerAttacker)) > 0) //Use FOE so as to not get boost when KOing partner last after enemy has no mons left
					{
						PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_ATK);

						gEffectBattler = gBattlerAttacker;
						gBattleScripting.battler = gBattlerAttacker;
						gBattleScripting.statChanger = INCREASE_1 | STAT_STAGE_ATK;
						gBattleScripting.animArg1 = 0xE + STAT_STAGE_ATK;
						gBattleScripting.animArg2 = 0;

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_Moxie;
						effect = 1;
					}
					break;

				case ABILITY_BEASTBOOST: ;
					if ((arg1 != ARG_IN_FUTURE_ATTACK || gWishFutureKnock.futureSightPartyIndex[bankDef] == gBattlerPartyIndexes[gBattlerAttacker])
					&& gBattleMons[bankDef].hp == 0
					&& BATTLER_ALIVE(gBattlerAttacker)
					&& TOOK_DAMAGE(bankDef)
					&& MOVE_HAD_EFFECT
					&& ViableMonCountFromBank(FOE(gBattlerAttacker)) > 0) //Use FOE so as to not get boost when KOing partner last after enemy has no mons left
					{
						u16 temp;
						u16 max;
						u16 stats[STAT_STAGE_SPDEF]; //Create new array to avoid modifying original stats
						int i;
						stats[STAT_STAGE_ATK-1] = gBattleMons[gBattlerAttacker].attack;
						stats[STAT_STAGE_DEF-1] = gBattleMons[gBattlerAttacker].defense;
						stats[STAT_STAGE_SPATK-2] = gBattleMons[gBattlerAttacker].spAttack;
						stats[STAT_STAGE_SPDEF-2] = gBattleMons[gBattlerAttacker].spDefense;
						stats[STAT_STAGE_SPEED+1] = gBattleMons[gBattlerAttacker].speed;

						if (IsWonderRoomActive()) {
							temp = stats[STAT_STAGE_DEF-1];
							stats[STAT_STAGE_DEF-1] = stats[STAT_STAGE_SPDEF-2]; //-2 b/c shifted left due to speed
							stats[STAT_STAGE_SPDEF-2] = temp;
						}

						max = 0;
						for (i = 1; i < STAT_STAGE_SPDEF; ++i) {
							if (stats[i] > stats[max])
								max = i;
						}

						//Get the proper stat stage value
						switch(max) {
							case 0: //Attack
							case 1: //Defense
								max += 1;
								break;
							case 2: //Special Attack
							case 3: //Special Defense
								max += 2;
								break;
							case 4:
								max = STAT_STAGE_SPEED;
						}

						if (STAT_CAN_RISE(gBattlerAttacker, max))
						{
							PREPARE_STAT_BUFFER(gBattleTextBuff1, max);

							gEffectBattler = gBattlerAttacker;
							gBattleScripting.battler = gBattlerAttacker;
							gBattleScripting.statChanger = INCREASE_1 | max;
							gBattleScripting.animArg1 = 0xE + max;
							gBattleScripting.animArg2 = 0;
							gLastUsedAbility = ABILITY_BEASTBOOST;
							RecordAbilityBattle(gBattlerAttacker, gLastUsedAbility);

							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_Moxie;
							effect = 1;
						}
					}
					break;

				#if (defined SPECIES_GRENINJA && defined SPECIES_ASHGRENINJA)
				case ABILITY_BATTLEBOND:
					if ((arg1 != ARG_IN_FUTURE_ATTACK || gWishFutureKnock.futureSightPartyIndex[bankDef] == gBattlerPartyIndexes[gBattlerAttacker])
					&& SPECIES(gBattlerAttacker) == SPECIES_GRENINJA
					&& gBattleMons[bankDef].hp == 0
					&& BATTLER_ALIVE(gBattlerAttacker)
					&& TOOK_DAMAGE(bankDef)
					&& MOVE_HAD_EFFECT
					&& ViableMonCountFromBank(FOE(gBattlerAttacker)) > 0 //Use FOE so as to not get boost when KOing partner last after enemy has no mons left
					&& !IS_TRANSFORMED(gBattlerAttacker))
					{
						DoFormChange(gBattlerAttacker, SPECIES_ASHGRENINJA, TRUE, TRUE, FALSE);

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_AbilityTransformed;
						effect = 1;
					}
				#endif
			}
			*gSeedHelper = 0; //For Soul-Heart Loop
			gBattleScripting.atk49_state++;
			break;

		case ATK49_FATIGUE:
			if (gBattleMons[gBattlerAttacker].status2 & STATUS2_LOCK_CONFUSE
			&& BATTLER_ALIVE(gBattlerAttacker)
			&& !gBattleStruct->DancerInProgress)
			{
				gBattleMons[gBattlerAttacker].status2 -= 0x400;

				if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_LOCK_CONFUSE)
				&&   (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))
				{
					gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);

					if (!IsConfused(gBattlerAttacker)
					&& ABILITY(gBattlerAttacker) != ABILITY_OWNTEMPO
					&& !(CheckGrounding(gBattlerAttacker) && gTerrainType == MISTY_TERRAIN))
					{
						gBattleMons[gBattlerAttacker].status2 |= ((Random() % 4)) + 2;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_ThrashConfuses;
						effect = 1;
					}
				}
				else if (!MOVE_HAD_EFFECT || (gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE))
					CancelMultiTurnMoves(gBattlerAttacker);
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_NEXT_TARGET: // For moves hitting two opposing Pokemon or whole field.
			gHitMarker |= HITMARKER_NO_ATTACKSTRING;
			if (gBattleStruct->MoveBounceInProgress)
				++gBattleStruct->MoveBounceTargetCount;
			else
				++gBattleStruct->OriginalAttackerTargetCount;

			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (!(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
				&& !gProtectStructs[gBattlerAttacker].chargingTurn)
				{
					if (gBattleMoves[gCurrentMove].target == MOVE_TARGET_BOTH)
					{
						if ((gBattleStruct->OriginalAttackerTargetCount < 2 && !gBattleStruct->MoveBounceInProgress)
						|| (gBattleStruct->MoveBounceTargetCount < 2 && gBattleStruct->MoveBounceInProgress))
						{ //Get Next Target
							u8 battlerId = PARTNER(bankDef);
							if (gBattleMons[battlerId].hp && battlerId != gBattlerAttacker)
							{
								gBattlerTarget = battlerId;
								gBattleStruct->preFaintEffectsState = 0;
								gBattleScripting.atk49_state = 0;
								MoveValuesCleanUp();

								if (gBattleMoves[gCurrentMove].effect == EFFECT_SUPERPOWER //Only lower user stats once for Clanging Scales (and similar custom moves)
								&&  gBattleStruct->AttackerDidDamageAtLeastOnce)
									gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[0];
								else
									gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];

								if (SPLIT(gCurrentMove) == SPLIT_STATUS)
								{
									BattleScriptPushCursor();
									gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
								}
								return;
							}
							else if (gBattleStruct->MoveBounceInProgress)
								goto RESTORE_BOUNCE_ATTACKER;
						}
						else if (gBattleStruct->MoveBounceInProgress)
						{
						RESTORE_BOUNCE_ATTACKER:
							++gBattleStruct->OriginalAttackerTargetCount;
							gBattleStruct->MoveBounceInProgress = 2; //Bounce just finished
							gBattleStruct->MoveBounceTargetCount = 0;
							battlerId = PARTNER(gBattlerAttacker);
							if (BATTLER_ALIVE(battlerId) && gBattleStruct->OriginalAttackerTargetCount < 2)
							{
								gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber]; //Restore original attacker
								gBattlerTarget = battlerId; //Attack Bouncer's partner
								gBattleScripting.animTargetsHit = 0;
								gBattleStruct->preFaintEffectsState = 0;
								gBattleScripting.atk49_state = 0;
								MoveValuesCleanUp();
								BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
								gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
								return;
							}
						}
					}
					else if (gBattleMoves[gCurrentMove].target == MOVE_TARGET_FOES_AND_ALLY && !CheckTableForMove(gCurrentMove, gSpecialWholeFieldMoves))
					{
						if (gBattleStruct->OriginalAttackerTargetCount < 3)
						{ //Get Next Target
							u8 battlerId = GetNextMultiTarget();
							while ((battlerId = GetNextMultiTarget()) != 0xFF)
							{
								gBattlerTarget = battlerId;
								if (BATTLER_ALIVE(battlerId) && battlerId != gBattlerAttacker)
								{
									gBattleStruct->preFaintEffectsState = 0;
									gBattleScripting.atk49_state = 0;
									MoveValuesCleanUp();
									if (gBattleMoves[gCurrentMove].effect == EFFECT_EXPLOSION)
										BattleScriptPush(gBattleScriptsForMoveEffects[0]); //Fixes issues with Explosion not changing targets
									else
										BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
									gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
									return;
								}
								++gBattleStruct->OriginalAttackerTargetCount;
							}
						}
					}
				}
			}

			if (gBattleStruct->MoveBounceInProgress)
				gBattleStruct->MoveBounceInProgress = 2; //Bounce just finished

			gBattleStruct->MoveBounceTargetCount = 0;
			gBattleStruct->OriginalAttackerTargetCount = 0;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_MOVE_RECOIL:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&&  !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE))
			{
				if ((gCurrentMove == MOVE_MINDBLOWN || gCurrentMove == MOVE_STEELBEAM)
				&& ABILITY(gBattlerAttacker) != ABILITY_MAGICGUARD
				&& BATTLER_ALIVE(gBattlerAttacker))
				{
					gBattleMoveDamage = MathMax(1, gBattleMons[gBattlerAttacker].maxHP / 2);

					if (((gBattleMons[gBattlerAttacker].maxHP % 2) != 0)
					&& gBattleMons[gBattlerAttacker].maxHP >= 3)
						++gBattleMoveDamage; //Round up Damage

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MindBlownDamage;

					effect = 1;
				}
				else if (gCurrentMove == MOVE_STRUGGLE
				&& BATTLER_ALIVE(gBattlerAttacker))
				{
					gBattleMoveDamage = MathMax(1, gBattleMons[gBattlerAttacker].maxHP / 4);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MoveEffectRecoil;
					effect = 1;
				}
				else if (gBattleMoves[gCurrentMove].effect == EFFECT_EXPLOSION
				&& !ABILITY_PRESENT(ABILITY_DAMP))
				{
					gBattleMoveDamage = 0;
					BattleScriptPushCursor();

					if (gBattleStruct->AttackerDidDamageAtLeastOnce)
						gBattlescriptCurrInstr = BattleScript_FaintAttackerForExplosion;
					else
						gBattlescriptCurrInstr = BattleScript_ExplosionAnim; //The attacker still needs to explode

					effect = 1;
				}
				else if (ABILITY(gBattlerAttacker) != ABILITY_MAGICGUARD
				&& ABILITY(gBattlerAttacker) != ABILITY_ROCKHEAD
				&& gBattleStruct->AttackerDidDamageAtLeastOnce
				&& BATTLER_ALIVE(gBattlerAttacker))
				{
					if (CheckTableForMove(gCurrentMove, gPercent25RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, gBattleStruct->totalDamageGiven / 4);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MoveEffectRecoil;
						effect = 1;
					}
					else if (CheckTableForMove(gCurrentMove, gPercent33RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, gBattleStruct->totalDamageGiven / 3);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MoveEffectRecoil;
						effect = 1;
					}
					else if (CheckTableForMove(gCurrentMove, gPercent50RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, gBattleStruct->totalDamageGiven / 2);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MoveEffectRecoil;
						effect = 1;
					}
					else if (CheckTableForMove(gCurrentMove, gPercent66RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, (gBattleStruct->totalDamageGiven * 2) / 3);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MoveEffectRecoil;
						effect = 1;
					}
					else if (CheckTableForMove(gCurrentMove, gPercent75RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, (gBattleStruct->totalDamageGiven * 3) / 4);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MoveEffectRecoil;
						effect = 1;
					}
					else if (CheckTableForMove(gCurrentMove, gPercent100RecoilMoves))
					{
						gBattleMoveDamage = MathMax(1, gBattleStruct->totalDamageGiven);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MoveEffectRecoil;
						effect = 1;
					}
				}
				
				if (effect)
					gBattleStruct->selfInflictedDamage += gBattleMoveDamage; //For Emergency Exit
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_EJECT_BUTTON:
			gBattlerAttacker = gBattleStruct->originalAttackerBackup;
			if (CanDoMoveEndSwitchout(arg1))
			{
				u8 banks[4] = {0, 1, 2, 3};
				SortBanksBySpeed(banks, FALSE);

				for (i = 0; i < gBattlersCount; ++i)
				{
					if (banks[i] != gBattlerAttacker
					&&  gBattleMons[banks[i]].hp
					&&  !SheerForceCheck()
					&&  ITEM_EFFECT(banks[i]) == HOLD_EFFECT_EJECT_BUTTON
					&&  !(gBattleStruct->ResultFlags[banks[i]] & MOVE_RESULT_NO_EFFECT)
					&&  gBattleStruct->turnDamageTaken[banks[i]] != 0
					&&  !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, gBattlerTarget)
					&&  ((gBattleTypeFlags & BATTLE_TYPE_TRAINER) || SIDE(i) == B_SIDE_PLAYER)) //Wilds can't activate
					{
						if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
							gBattlescriptCurrInstr = BattleScript_MoveEnd; //Cancel switchout for U-Turn & Volt Switch

						gBattleStruct->NoSymbiosisByte = TRUE;
						gActiveBattler = gBankSwitching = gBattleScripting.battler = banks[i];
						gLastUsedItem = ITEM(banks[i]);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_EjectButton;
						effect = 1;
						break; //Only the fastest Eject Button activates so end loop
					}
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_RED_CARD:
			gBattlerAttacker = gBattleStruct->originalAttackerBackup;
			if (CanDoMoveEndSwitchout(arg1)
			&& BATTLER_ALIVE(gBattlerAttacker))
			{
				u8 banks[4] = {0, 1, 2, 3};
				SortBanksBySpeed(banks, FALSE);

				for (i = 0; i < gBattlersCount; ++i)
				{
					if (banks[i] != gBattlerAttacker
					&&  BATTLER_ALIVE(banks[i])
					&&  !SheerForceCheck()
					//&&  !IsDynamaxed(banks[i]) //Can't force out a Dynamaxed mon - handled in BS
					&&  ITEM_EFFECT(banks[i]) == HOLD_EFFECT_RED_CARD
					&&  !(gBattleStruct->ResultFlags[banks[i]] & MOVE_RESULT_NO_EFFECT)
					&&  gBattleStruct->turnDamageTaken[banks[i]] != 0
					&&  !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, banks[i])
					&&  ((gBattleTypeFlags & BATTLE_TYPE_TRAINER) || SIDE(gBattlerAttacker) == B_SIDE_PLAYER)) //Wild attackers can't activate
					{
						gBattleStruct->NoSymbiosisByte = TRUE;
						gForceSwitchHelper = Force_Switch_Red_Card;
						gBattlescriptCurrInstr = BattleScript_Atk49; //Cancels U-Turn, Volt Switch, and Natural Gift
						gActiveBattler = gBankSwitching = gBattleScripting.battler = gBattleStruct->originalTargetBackup = banks[i];
						gLastUsedItem = ITEM(banks[i]);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_RedCard;
						effect = 1;
						break; //Only the fastest Red Card activates so end loop
					}
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_EJECT_PACK:
			gBattlerAttacker = gBattleStruct->originalAttackerBackup;
			if (CanDoMoveEndSwitchout(arg1))
			{
				for (i = 0; i < gBattlersCount; ++i)
				{
					if (BATTLER_ALIVE(i)
					&&  ITEM_EFFECT(i) == HOLD_EFFECT_EJECT_PACK
					&&  gBattleStruct->statFellThisTurn[i])
					{
						gBattleStruct->statFellThisTurn[i] = FALSE;
						gBattleStruct->NoSymbiosisByte = TRUE;
						gActiveBattler = gBankSwitching = gBattleScripting.battler = i;
						gLastUsedItem = ITEM(i);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_EjectPackCMD49;
						effect = 1;
						return;
					}
				}
			}
			gBattleScripting.atk49_state++;
			gBattleStruct->switchOutBankLooper = 0;
			break;

		case ATK49_SWITCH_OUT_ABILITIES:
			gBattlerAttacker = gBattleStruct->originalAttackerBackup;
			if (CanDoMoveEndSwitchout(arg1) && !SheerForceCheck())
			{
				for (; gBattleStruct->switchOutBankLooper < gBattlersCount; ++gBattleStruct->switchOutBankLooper)
				{
					u8 bank = gBattlerByTurnOrder[gBattleStruct->switchOutBankLooper];

					if (bank != gBattlerAttacker)
					{
						if ((ABILITY(bank) == ABILITY_EMERGENCYEXIT)
						&&  !(gBattleStruct->ResultFlags[bank] & MOVE_RESULT_NO_EFFECT)
						&&  !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bank)
						&&  !(gStatuses3[bank] & (STATUS3_SKY_DROP_ANY))
						&&  BATTLER_ALIVE(bank)
						&&  gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2
						&&  gBattleMons[bank].hp + gBattleStruct->turnDamageTaken[bank] > gBattleMons[bank].maxHP / 2) //Fell this turn
						{
							if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
								gBattlescriptCurrInstr = BattleScript_Atk49; //Cancel switchout for U-Turn & Volt Switch

							gActiveBattler = gBattleScripting.battler = gBankSwitching = bank;
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_EmergencyExit;
							effect = 1;
							break;
						}
					}
					else //Self inflicted damage
					{
						if ((ABILITY(bank) == ABILITY_EMERGENCYEXIT)
						&&  !(gStatuses3[bank] & (STATUS3_SKY_DROP_ANY))
						&&  BATTLER_ALIVE(bank)
						&&  (gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2 || gBattleStruct->lessThanHalfHPBeforeShellBell) //Ignore Shell Bell Recovery
						&&  gBattleMons[bank].hp + gBattleStruct->selfInflictedDamage > gBattleMons[bank].maxHP / 2) //Fell this turn
						{
							if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
								gBattlescriptCurrInstr = BattleScript_Atk49; //Cancel switchout for U-Turn & Volt Switch

							gActiveBattler = gBattleScripting.battler = gBankSwitching = bank;
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_EmergencyExit;
							effect = 1;
							break;
						}
					}
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_SHELL_BELL_LIFE_ORB_RECOIL:
		{
			u8 itemEffect;
			gBattlerAttacker = gBattleStruct->originalAttackerBackup; //Restore Original Attacker
			itemEffect = ITEM_EFFECT(gBattlerAttacker);

			if (arg1 != ARG_IN_FUTURE_ATTACK
			&& !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
			&& (itemEffect == HOLD_EFFECT_LIFE_ORB
			 || itemEffect == HOLD_EFFECT_SHELL_BELL
			 || itemEffect == HOLD_EFFECT_THROAT_SPRAY
			 || itemEffect == HOLD_EFFECT_BLUNDER_POLICY))
			{
				if (ItemBattleEffects(ItemEffects_ContactAttacker, gBattlerAttacker, TRUE, FALSE))
					effect = TRUE;
			}
			gBattleScripting.atk49_state++;
		}
			break;

		case ATK49_RESTORE_ABILITIES:
			for (i = 0; i < gBattlersCount; ++i)
			{
				if (gBattleStruct->DisabledMoldBreakerAbilities[i])
				{
					gBattleMons[i].ability = gBattleStruct->DisabledMoldBreakerAbilities[i];
					gBattleStruct->DisabledMoldBreakerAbilities[i] = 0;
				}
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_PICKPOCKET:
		{
			u8 banks[4] = {0, 1, 2, 3};
			SortBanksBySpeed(banks, FALSE);

			for (i = 0; i < gBattlersCount; ++i)
			{
				u8 bank = banks[i];

				if (bank != gBattlerAttacker
				&&  !SheerForceCheck()
				&&  ABILITY(bank) == ABILITY_PICKPOCKET
				&&  !(gBattleStruct->ResultFlags[bank] & MOVE_RESULT_NO_EFFECT)
				&&  TOOK_DAMAGE(bank)
				&&  !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bank)
				&&  gBattleMons[bank].hp != 0
				&&  CheckContact(gCurrentMove, gBattlerAttacker)
				&&  ITEM(gBattlerAttacker) != ITEM_NONE
				&&  ITEM(bank) == ITEM_NONE
				&& (ABILITY(gBattlerAttacker) != ABILITY_STICKYHOLD || !BATTLER_ALIVE(gBattlerAttacker)))
				{
					gBattleStruct->NoSymbiosisByte = TRUE;
					gLastUsedItem = ITEM(gBattlerAttacker);
					gBattlerTarget = gActiveBattler = gBattleScripting.battler = bank;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_Pickpocket;
					effect = 1;
					break; //Only fastest Pickpocket activates so exit loop.
				}
			}
			gBattleScripting.atk49_state++;
		}
			break;

		case ATK49_RAID_MON_PREP_MORE_ATTACKS:
			if (IsRaidBattle()
			&& BATTLER_ALIVE(gBattlerAttacker)
			&& gBattlerAttacker == BANK_RAID_BOSS
			&& (SPLIT(gCurrentMove) == SPLIT_STATUS || !IsAnyMaxMove(gCurrentMove)) //Status move or Raid Boss didn't use Max Move
			&& Random() % 100 < GetRaidBattleRepeatedAttackChance(gBattlerAttacker))
				gBattleStruct->dynamaxData.attackAgain = TRUE;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_SUBSTITUTE: // update substitute
			for (i = 0; i < gBattlersCount; i++)
			{
				if (gDisableStructs[i].substituteHP == 0)
					gBattleMons[i].status2 &= ~(STATUS2_SUBSTITUTE);
			}
			gBattleScripting.atk49_state++;
			break;

		case ATK49_END_ZMOVES:
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
			&&  gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY))
			{
				int i;

				for (i = 0; i < gBattlersCount; ++i)
				{
					if (!(gBattleStruct->ResultFlags[i] & (MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED))) //If at least one of the targets was affected
						break;
				}
				if (i == gBattlersCount) //No targets were affected
					gBattleStruct->StompingTantrumTimers[gBattlerAttacker] = 2;
			}
			else if (gMoveResultFlags & (MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED))
				gBattleStruct->StompingTantrumTimers[gBattlerAttacker] = 2;

			for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
			{
				gBattleStruct->turnDamageTaken[i] = 0;
				gBattleStruct->DamageTaken[i] = 0;
				gBattleStruct->criticalMultiplier[i] = 0;
				gBattleStruct->ResultFlags[i] = 0;
				gBattleStruct->missStringId[i] = 0;
				gBattleStruct->noResultString[i] = 0;
				gBattleStruct->statFellThisTurn[i] = 0;
			}

			gBattleStruct->totalDamageGiven = 0;
			gBattleStruct->selfInflictedDamage = 0;
			gBattleStruct->lessThanHalfHPBeforeShellBell = FALSE;
			//Clear spread move things
			gBattleStruct->doneDoublesSpreadHit = FALSE;
			gBattleStruct->calculatedSpreadMoveData = FALSE;
			gBattleStruct->calculatedSpreadMoveAccuracy = FALSE;

			switch (gCurrentMove) {
				case MOVE_FUSIONFLARE:
					gBattleStruct->fusionFlareUsedPrior = TRUE;
					gBattleStruct->fusionBoltUsedPrior = FALSE;
					break;
				case MOVE_FUSIONBOLT:
					gBattleStruct->fusionFlareUsedPrior = FALSE;
					gBattleStruct->fusionBoltUsedPrior = TRUE;
					break;
				default:
					gBattleStruct->fusionFlareUsedPrior = FALSE;
					gBattleStruct->fusionBoltUsedPrior = FALSE;
					break;
			}

			gBattleStruct->zMoveData.active = FALSE;
			gBattleStruct->zMoveData.effectApplied = FALSE;
			gBattleStruct->zMoveData.toBeUsed[gBattlerAttacker] = 0;
			gBattleStruct->dynamaxData.active = FALSE;
			gBattleStruct->secondaryEffectApplied = FALSE;
			gBattleStruct->InstructInProgress = FALSE;
			gBattleStruct->bypassSubstitute = FALSE;
			gBattleStruct->preFaintEffectsState = 0;
			gBattleStruct->MeFirstByte = FALSE;
			gBattleStruct->breakDisguiseSpecialDmg = FALSE;
			gBattleScripting.atk49_state++;
			break;

		case ATK49_DANCER:
			if (gBattleStruct->DancerBankCount == gBattlersCount)
			{
				gBattleStruct->DancerInProgress = FALSE;
				gBattleStruct->DancerByte = 0;
				gBattleStruct->DancerBankCount = 0;
				gBattleStruct->DancerTurnOrder[0] = 0; //Set the whole array to 0
				gBattleStruct->DancerTurnOrder[1] = 0;
				gBattleStruct->DancerTurnOrder[2] = 0;
				gBattleStruct->DancerTurnOrder[3] = 0;
				gBattleScripting.atk49_state++;
				gBattleStruct->AttackerDidDamageAtLeastOnce = FALSE;
				gBattleStruct->attackAnimationPlayed = FALSE;
				gBattleStruct->moveWasBouncedThisTurn = FALSE;
				break;
			}

			if (!gBattleStruct->DancerInProgress
			&& arg1 != ARG_IN_PURSUIT
			&& ABILITY_PRESENT(ABILITY_DANCER)
			&& gBattleStruct->attackAnimationPlayed
			&& !gBattleStruct->moveWasBouncedThisTurn
			&& CheckTableForMove(gCurrentMove, gDanceMoves))
			{
				gBattleStruct->DancerInProgress = TRUE;
				gBattleStruct->CurrentTurnAttacker = gBattlerAttacker;
				gBattleStruct->CurrentTurnTarget = gBattlerTarget;
				gBattleStruct->DancerBankCount = 0;

				for (i = 0; i < gBattlersCount; ++i)
					gBattleStruct->DancerTurnOrder[i] = i;

				SortBanksBySpeed(gBattleStruct->DancerTurnOrder, TRUE);
			}
			else if (!gBattleStruct->DancerInProgress)
			{
				gBattleScripting.atk49_state++;
				gBattleStruct->AttackerDidDamageAtLeastOnce = FALSE;
				gBattleStruct->attackAnimationPlayed = FALSE;
				gBattleStruct->moveWasBouncedThisTurn = FALSE;
				break;
			}

			bank = gBattleStruct->DancerTurnOrder[gBattleStruct->DancerBankCount];

			if (ABILITY(bank) == ABILITY_DANCER
			&& !(gAbsentBattlerFlags & gBitTable[bank])
			&& bank != gBattleStruct->CurrentTurnAttacker)
			{
				gBattlerAttacker = bank;

				if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
				&&  gBattleStruct->CurrentTurnAttacker == PARTNER(bank)
				&&  gBattleMoves[gCurrentMove].target == MOVE_TARGET_SELECTED
				&&  gBattleMons[gBattleStruct->CurrentTurnTarget].hp)
					gBattlerTarget = gBattleStruct->CurrentTurnTarget; //Target the same as partner's target
				else
					gBattlerTarget = GetMoveTarget(gCurrentMove, FALSE);

				gBattleStruct->preFaintEffectsState = 0;
				gBattleScripting.atk49_state = 0;
				gBattleStruct->atkCancellerTracker = 0;
				gBattleStruct->AttackerDidDamageAtLeastOnce = FALSE;
				gCalledMove = gCurrentMove;
				gBattleScripting.battler = gBattlerAttacker;
				gBattlescriptCurrInstr = BattleScript_DancerActivated;
				gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING | HITMARKER_ATTACKSTRING_PRINTED);
				++gBattleStruct->DancerBankCount;
				return;
			}

			++gBattleStruct->DancerBankCount;
			break;

		case ATK49_RAID_SHIELD:
			if (IsRaidBattle() && ShouldCreateRaidShields(BANK_RAID_BOSS))
			{
				gBattleStruct->dynamaxData.raidShieldsUp = TRUE;
				gBattleStruct->dynamaxData.shieldsDestroyed = 0;
				gBattleScripting.battler = BANK_RAID_BOSS;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_RaidShields;
				effect = 1;
			}

			gBattleScripting.atk49_state++;
			break;

		case ATK49_COUNT:
			break;
		}

		//Should I remove these lines?
		if (arg1 == 1 && effect == FALSE)
			gBattleScripting.atk49_state = ATK49_COUNT;
		else if (arg1 == 2 && arg2 == gBattleScripting.atk49_state)
			gBattleScripting.atk49_state = ATK49_COUNT;
		else if (arg1 == ARG_ONLY_EMERGENCY_EXIT)
			gBattleScripting.atk49_state = ATK49_COUNT;

	} while (gBattleScripting.atk49_state != ATK49_COUNT && effect == FALSE);

	if (gBattleScripting.atk49_state == ATK49_COUNT && effect == FALSE)
		gBattlescriptCurrInstr += 3;
}

void atk4A_typecalc2(void)
{
	u8 moveType = gBattleStruct->dynamicMoveType & 0x3F;
	ability_t atkAbility = ABILITY(gBattlerAttacker);
	ability_t defAbility = ABILITY(gBattlerTarget);
	u8 defEffect = ITEM_EFFECT(gBattlerTarget);
	s32 backupDamage;

	//Check Special Ground Immunities
	if (moveType == TYPE_GROUND && !CheckGrounding(gBattlerTarget) && gCurrentMove != MOVE_THOUSANDARROWS)
	{
		if (defAbility == ABILITY_LEVITATE)
		{
			gLastUsedAbility = atkAbility;
			gMoveResultFlags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBattlerTarget] = 0;
			gBattleStruct->missStringId[gBattlerTarget] = 3;
			RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
		}
		else if (defEffect == HOLD_EFFECT_AIR_BALLOON)
		{
			gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBattlerTarget] = 0;
			RecordItemEffectBattle(gBattlerTarget, defEffect);
		}
		else if (gStatuses3[gBattlerTarget] & (STATUS3_LEVITATING | STATUS3_TELEKINESIS))
		{
			gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBattlerTarget] = 0;
		}
		else
			goto RE_ENTER_TYPE_CHECK_2;	//You're a flying type
	}
	else if (CheckTableForMove(gCurrentMove, gPowderMoves))
	{
		if (defAbility == ABILITY_OVERCOAT)
		{
			gLastUsedAbility = defAbility;
			gMoveResultFlags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBattlerTarget] = 0;
			gBattleStruct->missStringId[gBattlerTarget] = 3;
			RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
		}
		else if (defEffect == HOLD_EFFECT_SAFETY_GOGGLES)
		{
			gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBattlerTarget] = 0;
			RecordItemEffectBattle(gBattlerTarget, defEffect);
		}
		else if (IsOfType(gBattlerTarget, TYPE_GRASS))
		{
			gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
			gLastLandedMoves[gBattlerTarget] = 0;
		}
		else
			goto RE_ENTER_TYPE_CHECK_2;
	}
	else if (gCurrentMove == MOVE_SKYDROP && IsOfType(gBattlerTarget, TYPE_FLYING))
	{
		gMoveResultFlags |= (MOVE_RESULT_DOESNT_AFFECT_FOE);
		gLastLandedMoves[gBattlerTarget] = 0;
	}
	else
	{
	RE_ENTER_TYPE_CHECK_2:	;
		backupDamage = gBattleMoveDamage;
		TypeDamageModification(atkAbility, gBattlerTarget, gCurrentMove, moveType, &gMoveResultFlags);
		gBattleMoveDamage = backupDamage;
	}

	if (defAbility == ABILITY_WONDERGUARD
	//&& AttacksThisTurn(gBattlerAttacker, gCurrentMove) == 2
	&& (!(gMoveResultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
		|| ((gMoveResultFlags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[gCurrentMove].power)
	{
		gLastUsedAbility = defAbility;
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gLastLandedMoves[gBattlerTarget] = 0;
		gBattleStruct->missStringId[gBattlerTarget] = 3;
		RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
	}

	if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
		gProtectStructs[gBattlerAttacker].targetNotAffected = 1;

	++gBattlescriptCurrInstr;
}

static void atk4B_returnatktoball(void)
{
    gActiveBattler = gBattlerAttacker;
    if (!(gHitMarker & HITMARKER_FAINTED(gActiveBattler)))
    {
        BtlController_EmitReturnMonToBall(0, 0);
        MarkBattlerForControllerExec(gActiveBattler);
    }
    ++gBattlescriptCurrInstr;
}

static void atk4C_getswitchedmondata(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        gBattlerPartyIndexes[gActiveBattler] = *(gBattleStruct->monToSwitchIntoId + gActiveBattler);
        BtlController_EmitGetMonData(0, REQUEST_ALL_BATTLE, gBitTable[gBattlerPartyIndexes[gActiveBattler]]);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 2;
    }
}

static void atk4D_switchindataupdate(void)
{
	struct BattlePokemon oldData;
	u32 i;
	u8* monData;

	if (gBattleControllerExecFlags) return;

	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	oldData = gBattleMons[gActiveBattler];
	monData = (u8*)(&gBattleMons[gActiveBattler]);

	for (i = 0; i < sizeof(struct BattlePokemon); ++i)
		monData[i] = gBattleBufferB[gActiveBattler][4 + i];

	if (gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) //The Pokemon takes on the types of its first two moves
	{
		UpdateTypesForCamomons(gActiveBattler);
	}
	else
	{
		gBattleMons[gActiveBattler].type1 = gBaseStats[gBattleMons[gActiveBattler].species].type1;
		gBattleMons[gActiveBattler].type2 = gBaseStats[gBattleMons[gActiveBattler].species].type2;
	}

	gBattleMons[gActiveBattler].ability = GetMonAbility(GetBankPartyData(gActiveBattler));

	CONSUMED_ITEMS(gActiveBattler) = 0;
	gBattleStruct->StakeoutCounters[gActiveBattler] = 1;

	ClearSwitchBytes(gActiveBattler);
	ClearSwitchBits(gActiveBattler);

	//gBattleStruct->LastUsedMoves[gActiveBattler] = 0;
	//gBattleStruct->LastUsedTypes[gActiveBattler] = TYPE_BLANK;

	//Former Knock Off Check was here

	if (gCurrentMove == MOVE_BATONPASS)
	{
		for (i = 0; i < BATTLE_STATS_NO-1; ++i)
			gBattleMons[gActiveBattler].statStages[i] = oldData.statStages[i];

		gBattleMons[gActiveBattler].status2 = oldData.status2;

		//Gastro Acid Passing
		if (IsAbilitySuppressed(gActiveBattler))
		{
			gBattleStruct->SuppressedAbilities[gActiveBattler] = gBattleMons[gActiveBattler].ability;
			gBattleMons[gActiveBattler].ability = 0;
		}

		//Power Trick Passing
		if (gStatuses3[gActiveBattler] & STATUS3_POWER_TRICK)
		{
			u8 attack = gBattleMons[gActiveBattler].attack;
			u8 defense = gBattleMons[gActiveBattler].defense;
			gBattleMons[gActiveBattler].attack = defense;
			gBattleMons[gActiveBattler].defense = attack;
		}
	}
	else
		ClearBehindSubstituteBit(gActiveBattler);

	if (!(gStatuses3[gActiveBattler] & STATUS3_LEVITATING))
		gBattleStruct->MagnetRiseTimers[gActiveBattler] = 0;

	{
		u32 backupStatus2[gBattlersCount];
		for (i = 0; i < gBattlersCount; ++i)
			backupStatus2[i] = gBattleMons[i].status2;
		SwitchInClearSetData();
		TryReactivateCentifernoSandblast(backupStatus2);
	}

	if (ABILITY(gActiveBattler) == ABILITY_ILLUSION)
	{
		gStatuses3[gActiveBattler] |= STATUS3_ILLUSION;

		if (GetIllusionPartyData(gActiveBattler) != GetBankPartyData(gActiveBattler))
		{
			EmitDataTransfer(0, &gStatuses3[gActiveBattler], 4, &gStatuses3[gActiveBattler]);
			MarkBattlerForControllerExec(gActiveBattler);
		}
		else
			gStatuses3[gActiveBattler] &= ~STATUS3_ILLUSION;
	}

	gBattleScripting.battler = gActiveBattler;

	PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gActiveBattler, GetIllusionPartyNumber(gActiveBattler));

	gBattleMons[gActiveBattler].type3 = TYPE_BLANK;

	gBattlescriptCurrInstr += 2;
}

static void atk4E_switchinanim(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT
         && !(gBattleTypeFlags & 
              (BATTLE_TYPE_LINK
            | BATTLE_TYPE_LEGENDARY
            | BATTLE_TYPE_OLD_MAN_TUTORIAL
            | BATTLE_TYPE_POKEDUDE
            | BATTLE_TYPE_EREADER_TRAINER
            | BATTLE_TYPE_GHOST)))
            HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), FLAG_SET_SEEN, gBattleMons[gActiveBattler].personality);
        gAbsentBattlerFlags &= ~(gBitTable[gActiveBattler]);
        BtlController_EmitSwitchInAnim(0, gBattlerPartyIndexes[gActiveBattler], gBattlescriptCurrInstr[2]);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 3;
    }
}

static void atk4F_jumpifcantswitch(void)
{
	int i;
	u8 firstMonId, lastMonId;
	u8 battlerIn1, battlerIn2;
	u8 foe1, foe2;
	struct Pokemon* party;

	gActiveBattler = GetBattlerForBattleScript(T2_READ_8(gBattlescriptCurrInstr + 1) & ~(ATK4F_DONT_CHECK_STATUSES));
	LoadBattlersAndFoes(&battlerIn1, &battlerIn2, &foe1, &foe2);
	party = LoadPartyRange(gActiveBattler, &firstMonId, &lastMonId);

	if (!(T2_READ_8(gBattlescriptCurrInstr + 1) & ATK4F_DONT_CHECK_STATUSES)
	&& !IsOfType(gActiveBattler, TYPE_GHOST)
	&& ITEM_EFFECT(gActiveBattler) != HOLD_EFFECT_SHED_SHELL
	&& ((gBattleMons[gActiveBattler].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION)) || (gStatuses3[gActiveBattler] & STATUS3_ROOTED) || IsFairyLockActive()))
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	}
	else
	{
		for (i = firstMonId; i < lastMonId; ++i)
		{
			if (GetMonData(&party[i], MON_DATA_HP, 0) != 0
			&& GetMonData(&party[i], MON_DATA_SPECIES, 0) != SPECIES_NONE
			&& !GetMonData(&party[i], MON_DATA_IS_EGG, 0)
			&& i != gBattlerPartyIndexes[battlerIn1]
			&& i != gBattlerPartyIndexes[battlerIn2])
				break;
		}

		if (i == lastMonId)
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
		else
			gBattlescriptCurrInstr += 6;
	}
}

static void sub_8024398(u8 arg0)
{
    *(gBattleStruct->battlerPartyIndexes + gActiveBattler) = gBattlerPartyIndexes[gActiveBattler];
    BtlController_EmitChoosePokemon(0, PARTY_ACTION_SEND_OUT, arg0, 0, gBattleStruct->battlerPartyOrders[gActiveBattler]);
    MarkBattlerForControllerExec(gActiveBattler);
}

static void atk50_openpartyscreen(void)
{
    u32 flags;
    u8 hitmarkerFaintBits;
    u8 battlerId;
    const u8 *jumpPtr;

    battlerId = 0;
    flags = 0;
    jumpPtr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    if (gBattlescriptCurrInstr[1] == 5)
    {
        if ((gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI)) != BATTLE_TYPE_DOUBLE)
        {
            for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
            {
                if (gHitMarker & HITMARKER_FAINTED(gActiveBattler))
                {
                    if (HasNoMonsToSwitch(gActiveBattler))
                    {
                        gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                        gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                        BtlController_EmitLinkStandbyMsg(0, 2);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                    else if (!gSpecialStatuses[gActiveBattler].flag40)
                    {
                        sub_8024398(6);
                        gSpecialStatuses[gActiveBattler].flag40 = 1;
                    }
                }
                else
                {
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        {
            u8 flag40_0, flag40_1, flag40_2, flag40_3;

            hitmarkerFaintBits = gHitMarker >> 0x1C;
            if (gBitTable[0] & hitmarkerFaintBits)
            {
                gActiveBattler = 0;
                if (HasNoMonsToSwitch(0))
                {
                    gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                    gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                    BtlController_EmitCmd42(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
                else if (!gSpecialStatuses[gActiveBattler].flag40)
                {
                    sub_8024398(gBattleStruct->monToSwitchIntoId[2]);
                    gSpecialStatuses[gActiveBattler].flag40 = 1;
                }
                else
                {
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                    flags |= 1;
                }
            }
            if (gBitTable[2] & hitmarkerFaintBits && !(gBitTable[0] & hitmarkerFaintBits))
            {
                gActiveBattler = 2;
                if (HasNoMonsToSwitch(2))
                {
                    gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                    gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                    BtlController_EmitCmd42(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
                else if (!gSpecialStatuses[gActiveBattler].flag40)
                {
                    sub_8024398(gBattleStruct->monToSwitchIntoId[0]);
                    gSpecialStatuses[gActiveBattler].flag40 = 1;
                }
                else if (!(flags & 1))
                {
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
            if (gBitTable[1] & hitmarkerFaintBits)
            {
                gActiveBattler = 1;
                if (HasNoMonsToSwitch(1))
                {
                    gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                    gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                    BtlController_EmitCmd42(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
                else if (!gSpecialStatuses[gActiveBattler].flag40)
                {
                    sub_8024398(gBattleStruct->monToSwitchIntoId[3]);
                    gSpecialStatuses[gActiveBattler].flag40 = 1;
                }
                else
                {
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                    flags |= 2;
                }
            }
            if (gBitTable[3] & hitmarkerFaintBits && !(gBitTable[1] & hitmarkerFaintBits))
            {
                gActiveBattler = 3;
                if (HasNoMonsToSwitch(3))
                {
                    gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                    gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                    BtlController_EmitCmd42(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
                else if (!gSpecialStatuses[gActiveBattler].flag40)
                {
                    sub_8024398(gBattleStruct->monToSwitchIntoId[1]);
                    gSpecialStatuses[gActiveBattler].flag40 = 1;
                }
                else if (!(flags & 2))
                {
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
            flag40_0 = gSpecialStatuses[0].flag40;
            if (!flag40_0)
            {
                flag40_2 = gSpecialStatuses[2].flag40;
                if (!flag40_2 && hitmarkerFaintBits != 0)
                {
                    if (gAbsentBattlerFlags & gBitTable[0])
                        gActiveBattler = 2;
                    else
                        gActiveBattler = 0;

                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }

            }
            flag40_1 = gSpecialStatuses[1].flag40;
            if (!flag40_1)
            {
                flag40_3 = gSpecialStatuses[3].flag40;
                if (!flag40_3 && hitmarkerFaintBits != 0)
                {
                    if (gAbsentBattlerFlags & gBitTable[1])
                        gActiveBattler = 3;
                    else
                        gActiveBattler = 1;
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
        }
        gBattlescriptCurrInstr += 6;
    }
    else if (gBattlescriptCurrInstr[1] == 6)
    {
        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                hitmarkerFaintBits = gHitMarker >> 0x1C;
                if (gBitTable[2] & hitmarkerFaintBits && gBitTable[0] & hitmarkerFaintBits)
                {
                    gActiveBattler = 2;
                    if (HasNoMonsToSwitch(2))
                    {
                        gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                        gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                        BtlController_EmitCmd42(0);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                    else if (!gSpecialStatuses[gActiveBattler].flag40)
                    {
                        sub_8024398(gBattleStruct->monToSwitchIntoId[0]);
                        gSpecialStatuses[gActiveBattler].flag40 = 1;
                    }
                }
                if (gBitTable[3] & hitmarkerFaintBits && hitmarkerFaintBits & gBitTable[1])
                {
                    gActiveBattler = 3;
                    if (HasNoMonsToSwitch(3))
                    {
                        gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                        gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                        BtlController_EmitCmd42(0);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                    else if (!gSpecialStatuses[gActiveBattler].flag40)
                    {
                        sub_8024398(gBattleStruct->monToSwitchIntoId[1]);
                        gSpecialStatuses[gActiveBattler].flag40 = 1;
                    }
                }
                gBattlescriptCurrInstr += 6;
            }
            else
            {
                gBattlescriptCurrInstr += 6;
            }
        }
        else
        {
            gBattlescriptCurrInstr += 6;
        }

        hitmarkerFaintBits = gHitMarker >> 0x1C;

        gBattlerFainted = 0;
        while (1)
        {
            if (gBitTable[gBattlerFainted] & hitmarkerFaintBits)
                break;
            if (gBattlerFainted >= gBattlersCount)
                break;
            ++gBattlerFainted;
        }

        if (gBattlerFainted == gBattlersCount)
            gBattlescriptCurrInstr = jumpPtr;
    }
    else
    {
        if (gBattlescriptCurrInstr[1] & OPEN_PARTY_ALLOW_CANCEL)
            hitmarkerFaintBits = PARTY_ACTION_CHOOSE_MON; // Used here as the caseId for the EmitChoose function.
        else
            hitmarkerFaintBits = PARTY_ACTION_SEND_OUT;
        battlerId = GetBattlerForBattleScript(gBattlescriptCurrInstr[1] & ~(OPEN_PARTY_ALLOW_CANCEL));
        if (gSpecialStatuses[battlerId].flag40)
        {
            gBattlescriptCurrInstr += 6;
        }
        else if (HasNoMonsToSwitch(battlerId))
        {
            gActiveBattler = battlerId;
            gAbsentBattlerFlags |= gBitTable[gActiveBattler];
            gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
            gBattlescriptCurrInstr = jumpPtr;
        }
        else
        {
            gActiveBattler = battlerId;
            *(gBattleStruct->battlerPartyIndexes + gActiveBattler) = gBattlerPartyIndexes[gActiveBattler];
            BtlController_EmitChoosePokemon(0, hitmarkerFaintBits, *(gBattleStruct->monToSwitchIntoId + (gActiveBattler ^ 2)), 0, gBattleStruct->battlerPartyOrders[gActiveBattler]);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattlescriptCurrInstr += 6;
            if (GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_LEFT && gBattleResults.playerSwitchesCounter < 0xFF)
                ++gBattleResults.playerSwitchesCounter;

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
                {
                    if (gActiveBattler != battlerId)
                    {
                        BtlController_EmitLinkStandbyMsg(0, 2);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                }
            }
            else
            {
                gActiveBattler = GetBattlerAtPosition(GetBattlerPosition(battlerId) ^ BIT_SIDE);
                if (gAbsentBattlerFlags & gBitTable[gActiveBattler])
                    gActiveBattler ^= BIT_FLANK;
                BtlController_EmitLinkStandbyMsg(0, 2);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
    }
}

static void atk51_switchhandleorder(void)
{
	int i;
	if (gBattleControllerExecFlags) return;

	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

	switch (gBattlescriptCurrInstr[2]) {
		case 0:
			for (i = 0; i < gBattlersCount; ++i)
			{
				if (gBattleBufferB[i][0] == CONTROLLER_CHOSENMONRETURNVALUE)
					gBattleStruct->monToSwitchIntoId[i] = gBattleBufferB[i][1];
			}
			break;

		case 1:
			if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
				UpdatePartyOwnerOnSwitch_NonMulti(gActiveBattler);
			break;

		case 2:
			gBattleCommunication[0] = gBattleBufferB[gActiveBattler][1];
			gBattleStruct->monToSwitchIntoId[gActiveBattler] = gBattleBufferB[gActiveBattler][1];

			if (gBattleTypeFlags & BATTLE_TYPE_LINK && gBattleTypeFlags & BATTLE_TYPE_MULTI)
			{
				gBattleStruct->battlerPartyOrders[gActiveBattler][0] &= 0xF;
				gBattleStruct->battlerPartyOrders[gActiveBattler][0] |= (gBattleBufferB[gActiveBattler][2] & 0xF0);
				gBattleStruct->battlerPartyOrders[gActiveBattler][1] = gBattleBufferB[gActiveBattler][3];

				gBattleStruct->battlerPartyOrders[gActiveBattler ^ BIT_FLANK][0] &= (0xF0);
				gBattleStruct->battlerPartyOrders[gActiveBattler ^ BIT_FLANK][0] |= (gBattleBufferB[gActiveBattler][2] & 0xF0) >> 4;
				gBattleStruct->battlerPartyOrders[gActiveBattler ^ BIT_FLANK][2] = gBattleBufferB[gActiveBattler][3];
			}
			else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
				SwitchPartyOrderInGameMulti(gActiveBattler, gBattleStruct->monToSwitchIntoId[gActiveBattler]);

			else
				UpdatePartyOwnerOnSwitch_NonMulti(gActiveBattler);

			PREPARE_SPECIES_BUFFER(gBattleTextBuff1, gBattleMons[gBattlerAttacker].species)
			PREPARE_MON_NICK_BUFFER(gBattleTextBuff2, gActiveBattler, gBattleBufferB[gActiveBattler][1])
			break;
		}
	gBattlescriptCurrInstr += 3;
}


static void atk53_trainerslidein(void)
{
    if (!gBattlescriptCurrInstr[1])
        gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
    else
        gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
    BtlController_EmitTrainerSlide(0);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk54_playse(void)
{
    gActiveBattler = gBattlerAttacker;
    BtlController_EmitPlaySE(0, T2_READ_16(gBattlescriptCurrInstr + 1));
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 3;
}

static void atk55_fanfare(void)
{
    gActiveBattler = gBattlerAttacker;
    BtlController_EmitPlayFanfare(0, T2_READ_16(gBattlescriptCurrInstr + 1));
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 3;
}

static void atk56_playfaintcry(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    BtlController_EmitFaintingCry(0);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk57(void)
{
    gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
    BtlController_EmitCmd55(0, gBattleOutcome);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 1;
}

static void atk58_returntoball(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    BtlController_EmitReturnMonToBall(0, 1);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk59_handlelearnnewmove(void)
{
    const u8 *jumpPtr1 = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    const u8 *jumpPtr2 = T1_READ_PTR(gBattlescriptCurrInstr + 5);
    u16 ret = MonTryLearningNewMove(&gPlayerParty[gBattleStruct->expGetterMonId], gBattlescriptCurrInstr[9]);
    
    while (ret == 0xFFFE)
        ret = MonTryLearningNewMove(&gPlayerParty[gBattleStruct->expGetterMonId], 0);
    if (ret == 0)
    {
        gBattlescriptCurrInstr = jumpPtr2;
    }
    else if (ret == 0xFFFF)
    {
        gBattlescriptCurrInstr += 10;
    }
    else
    {
        gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        if (gBattlerPartyIndexes[gActiveBattler] == gBattleStruct->expGetterMonId
         && !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
        {
            GiveMoveToBattleMon(&gBattleMons[gActiveBattler], ret);
        }
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        {
            gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
            if (gBattlerPartyIndexes[gActiveBattler] == gBattleStruct->expGetterMonId
             && !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
            {
                GiveMoveToBattleMon(&gBattleMons[gActiveBattler], ret);
            }
        }
        gBattlescriptCurrInstr = jumpPtr1;
    }
}

static void atk5A_yesnoboxlearnmove(void)
{
    gActiveBattler = 0;

    switch (gBattleScripting.learnMoveState)
    {
    case 0:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
        BattlePutTextOnWindow(gText_BattleYesNoChoice, 0xE);
        ++gBattleScripting.learnMoveState;
        gBattleCommunication[CURSOR_POSITION] = 0;
        BattleCreateYesNoCursorAt();
        break;
    case 1:
        if (JOY_NEW(DPAD_UP) && gBattleCommunication[CURSOR_POSITION] != 0)
        {
            PlaySE(SE_SELECT);
            BattleDestroyYesNoCursorAt();
            gBattleCommunication[CURSOR_POSITION] = 0;
            BattleCreateYesNoCursorAt();
        }
        if (JOY_NEW(DPAD_DOWN) && gBattleCommunication[CURSOR_POSITION] == 0)
        {
            PlaySE(SE_SELECT);
            BattleDestroyYesNoCursorAt();
            gBattleCommunication[CURSOR_POSITION] = 1;
            BattleCreateYesNoCursorAt();
        }
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            if (gBattleCommunication[1] == 0)
            {
                HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
                BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
                ++gBattleScripting.learnMoveState;
            }
            else
            {
                gBattleScripting.learnMoveState = 4;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            gBattleScripting.learnMoveState = 4;
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            FreeAllWindowBuffers();
            ShowSelectMovePokemonSummaryScreen(gPlayerParty, gBattleStruct->expGetterMonId, gPlayerPartyCount - 1, ReshowBattleScreenAfterMenu, gMoveToLearn);
            ++gBattleScripting.learnMoveState;
        }
        break;
    case 3:
        if (!gPaletteFade.active && gMain.callback2 == BattleMainCB2)
        {
            u8 movePosition = GetMoveSlotToReplace();

            if (movePosition == 4)
            {
                gBattleScripting.learnMoveState = 4;
            }
            else
            {
                u16 moveId = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MOVE1 + movePosition);
                
                if (IsHMMove2(moveId))
                {
                    PrepareStringBattle(STRINGID_HMMOVESCANTBEFORGOTTEN, gActiveBattler);
                    gBattleScripting.learnMoveState = 5;
                }
                else
                {
                    gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
                    PREPARE_MOVE_BUFFER(gBattleTextBuff2, moveId)
                    RemoveMonPPBonus(&gPlayerParty[gBattleStruct->expGetterMonId], movePosition);
                    SetMonMoveSlot(&gPlayerParty[gBattleStruct->expGetterMonId], gMoveToLearn, movePosition);
                    if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId
                     && !(gBattleMons[0].status2 & STATUS2_TRANSFORMED)
                     && !(gDisableStructs[0].mimickedMoves & gBitTable[movePosition]))
                    {
                        RemoveBattleMonPPBonus(&gBattleMons[0], movePosition);
                        SetBattleMonMoveSlot(&gBattleMons[0], gMoveToLearn, movePosition);
                    }
                    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
                     && gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId
                     && !(gBattleMons[2].status2 & STATUS2_TRANSFORMED)
                     && !(gDisableStructs[2].mimickedMoves & gBitTable[movePosition]))
                    {
                        RemoveBattleMonPPBonus(&gBattleMons[2], movePosition);
                        SetBattleMonMoveSlot(&gBattleMons[2], gMoveToLearn, movePosition);
                    }
                }
            }
        }
        break;
    case 4:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, WINDOW_CLEAR);
        gBattlescriptCurrInstr += 5;
        break;
    case 5:
        if (!gBattleControllerExecFlags)
        {
            gBattleScripting.learnMoveState = 2;
        }
        break;
    }
}

static void atk5B_yesnoboxstoplearningmove(void)
{
    switch (gBattleScripting.learnMoveState)
    {
    case 0:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
        BattlePutTextOnWindow(gText_BattleYesNoChoice, 0xE);
        ++gBattleScripting.learnMoveState;
        gBattleCommunication[CURSOR_POSITION] = 0;
        BattleCreateYesNoCursorAt();
        break;
    case 1:
        if (JOY_NEW(DPAD_UP) && gBattleCommunication[CURSOR_POSITION] != 0)
        {
            PlaySE(SE_SELECT);
            BattleDestroyYesNoCursorAt();
            gBattleCommunication[CURSOR_POSITION] = 0;
            BattleCreateYesNoCursorAt();
        }
        if (JOY_NEW(DPAD_DOWN) && gBattleCommunication[CURSOR_POSITION] == 0)
        {
            PlaySE(SE_SELECT);
            BattleDestroyYesNoCursorAt();
            gBattleCommunication[CURSOR_POSITION] = 1;
            BattleCreateYesNoCursorAt();
        }
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);

            if (gBattleCommunication[1] != 0)
                gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
            else
                gBattlescriptCurrInstr += 5;
            HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
            HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
        }
        break;
    }
}

static void atk5C_hitanimation(void)
{
    u32 i;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

	if (!IsDoubleSpreadMove())
	{
		if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
		{
		}
		else if (!(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE || gBattleStruct->bypassSubstitute) || !IS_BEHIND_SUBSTITUTE(gActiveBattler) || gDisableStructs[gActiveBattler].substituteHP == 0)
		{
			//Do the hit animation on the actual Pokemon sprite, not the Substitute
			BtlController_EmitHitAnimation(0);
			MarkBattlerForControllerExec(gActiveBattler);
		}
	}
	else if (!gBattleStruct->doneDoublesSpreadHit) //Spread move in doubles
	{
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (!(gBattleStruct->ResultFlags[i] & MOVE_RESULT_NO_EFFECT) && !gBattleStruct->noResultString[i])
			{
				gActiveBattler = i;

				if (!(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE || gBattleStruct->bypassSubstitute) || !IS_BEHIND_SUBSTITUTE(gActiveBattler) || gDisableStructs[gActiveBattler].substituteHP == 0)
				{
					BtlController_EmitHitAnimation(0);
					MarkBattlerForControllerExec(gActiveBattler);
				}
			}
		}
	}

	gBattlescriptCurrInstr += 2;
}

static void atk5D_getmoneyreward(void)
{
    u32 i = 0;
    u32 moneyReward;
    u8 lastMonLevel = 0;

    const struct TrainerMonItemCustomMoves *party4; //This needs to be out here

    if (gBattleOutcome == B_OUTCOME_WON)
    {
        if (gTrainerBattleOpponent_A == TRAINER_SECRET_BASE)
        {
            moneyReward = gBattleResources->secretBase->party.levels[0] * 20 * gBattleStruct->moneyMultiplier;
        }
        else
        {
            switch (gTrainers[gTrainerBattleOpponent_A].partyFlags)
            {
            case 0:
                {
                    const struct TrainerMonNoItemDefaultMoves *party1 = gTrainers[gTrainerBattleOpponent_A].party.NoItemDefaultMoves;
                    
                    lastMonLevel = party1[gTrainers[gTrainerBattleOpponent_A].partySize - 1].lvl;
                }
                break;
            case F_TRAINER_PARTY_CUSTOM_MOVESET:
                {
                    const struct TrainerMonNoItemCustomMoves *party2 = gTrainers[gTrainerBattleOpponent_A].party.NoItemCustomMoves;
                    
                    lastMonLevel = party2[gTrainers[gTrainerBattleOpponent_A].partySize - 1].lvl;
                }
                break;
            case F_TRAINER_PARTY_HELD_ITEM:
                {
                    const struct TrainerMonItemDefaultMoves *party3 = gTrainers[gTrainerBattleOpponent_A].party.ItemDefaultMoves;
                    
                    lastMonLevel = party3[gTrainers[gTrainerBattleOpponent_A].partySize - 1].lvl;
                }
                break;
            case (F_TRAINER_PARTY_CUSTOM_MOVESET | F_TRAINER_PARTY_HELD_ITEM):
                {
                    party4 = gTrainers[gTrainerBattleOpponent_A].party.ItemCustomMoves;
                    
                    lastMonLevel = party4[gTrainers[gTrainerBattleOpponent_A].partySize - 1].lvl;
                }
                break;
            }
            for (; gTrainerMoneyTable[i].classId != 0xFF; i++)
            {
                if (gTrainerMoneyTable[i].classId == gTrainers[gTrainerBattleOpponent_A].trainerClass)
                    break;
            }
            party4 = gTrainers[gTrainerBattleOpponent_A].party.ItemCustomMoves; // Needed to Match. Has no effect.
            moneyReward = 4 * lastMonLevel * gBattleStruct->moneyMultiplier * (gBattleTypeFlags & BATTLE_TYPE_DOUBLE ? 2 : 1) * gTrainerMoneyTable[i].value;
        }
        AddMoney(&gSaveBlock1Ptr->money, moneyReward);
    }
    else
    {
        moneyReward = ComputeWhiteOutMoneyLoss();
    }
    PREPARE_WORD_NUMBER_BUFFER(gBattleTextBuff1, 5, moneyReward);
    if (moneyReward)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atk5E(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

    switch (gBattleCommunication[0])
    {
    case 0:
        BtlController_EmitGetMonData(0, REQUEST_ALL_BATTLE, 0);
        MarkBattlerForControllerExec(gActiveBattler);
        ++gBattleCommunication[0];
        break;
    case 1:
         if (!gBattleControllerExecFlags)
         {
            s32 i;
            struct BattlePokemon *bufferPoke = (struct BattlePokemon *) &gBattleBufferB[gActiveBattler][4];
            
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
                gBattleMons[gActiveBattler].moves[i] = bufferPoke->moves[i];
                gBattleMons[gActiveBattler].pp[i] = bufferPoke->pp[i];
            }
            gBattlescriptCurrInstr += 2;
         }
         break;
    }
}

static void atk5F_swapattackerwithtarget(void)
{
    gActiveBattler = gBattlerAttacker;
    gBattlerAttacker = gBattlerTarget;
    gBattlerTarget = gActiveBattler;
    if (gHitMarker & HITMARKER_SWAP_ATTACKER_TARGET)
        gHitMarker &= ~(HITMARKER_SWAP_ATTACKER_TARGET);
    else
        gHitMarker |= HITMARKER_SWAP_ATTACKER_TARGET;
    ++gBattlescriptCurrInstr;
}

static void atk60_incrementgamestat(void)
{
    if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
        IncrementGameStat(gBattlescriptCurrInstr[1]);
    gBattlescriptCurrInstr += 2;
}

static void atk61_drawpartystatussummary(void)
{
	int i;
	struct Pokemon* party;
	struct HpAndStatus hpStatus[6];
	if (gBattleControllerExecFlags)
		return;

	if (!gBattleStruct->handlingFaintSwitching)
		RestoreOriginalAttackerAndTarget(); //I'm not sure if this function is even necessary anymore, but I'd rather not remove it and cause bugs
	gBattleStruct->skipBankStatAnim = gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

	if (HandleSpecialSwitchOutAbilities(gActiveBattler, ABILITY(gActiveBattler)))
		return;

	gBattleStruct->skipBankStatAnim = 0xFF; //No longer needed
	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

	if (SIDE(gActiveBattler) == 0)
		party = gPlayerParty;
	else
		party = gEnemyParty;

	for (i = 0; i < 6; ++i)
	{
		if (party[i].box.species == SPECIES_NONE || party[i].box.species == SPECIES_EGG)
		{
			hpStatus[i].hp = 0xFFFF;
			hpStatus[i].status = 0;
		}
		else
		{
			hpStatus[i].hp = party[i].hp;
			hpStatus[i].status = party[i].status;
		}
	}

	BtlController_EmitDrawPartyStatusSummary(0, hpStatus, 1);
	MarkBattlerForControllerExec(gActiveBattler);
	gBattlescriptCurrInstr += 2;
}

static void atk62_hidepartystatussummary(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    BtlController_EmitHidePartyStatusSummary(0);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void UpdateMoveStartValuesForCalledMove(void)
{
	gBattleStruct->atkCancellerTracker = CANCELLER_GRAVITY_2;
	gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBattlerAttacker, gCurrentMove);
	gBattleStruct->doneDoublesSpreadHit = FALSE;
	gBattleStruct->calculatedSpreadMoveData = FALSE;
	gBattleStruct->calculatedSpreadMoveAccuracy = FALSE;
	gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);

	if (gBattleMoves[gCurrentMove].target & MOVE_TARGET_USER)
		gBattlerTarget = gBattlerAttacker;
}

static void TryUpdateCalledMoveWithZMove(void)
{
	if (gBattleStruct->zMoveData.active && SPLIT(gCurrentMove) != SPLIT_STATUS)
	{
		gBattleStruct->ai.zMoveHelper = gCurrentMove;
		gCurrentMove = GetTypeBasedZMove(gCurrentMove, gBattlerAttacker);
	}
}

static void atk63_jumptocalledmove(void)
{
	if (gBattlescriptCurrInstr[1])
		gCurrentMove = gCalledMove;
	else
		gChosenMove = gCurrentMove = gCalledMove;

	TryUpdateCalledMoveWithZMove();
	if (gBattlescriptCurrInstr[1] != 0xFF)
		UpdateMoveStartValuesForCalledMove();

	gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);

	gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
}

static void atk64_statusanimation(void)
{
	if (gBattleControllerExecFlags) return;

	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

	if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS)) {
			BtlController_EmitStatusAnimation(0, 0, gBattleMons[gActiveBattler].status1);
			MarkBattlerForControllerExec(gActiveBattler);
	}
	gBattlescriptCurrInstr += 2;
}

static void atk65_status2animation(void)
{
    u32 wantedToAnimate;

    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        wantedToAnimate = T1_READ_32(gBattlescriptCurrInstr + 2);
        if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
         && gDisableStructs[gActiveBattler].substituteHP == 0
         && !(gHitMarker & HITMARKER_NO_ANIMATIONS))
        {
            BtlController_EmitStatusAnimation(0, TRUE, gBattleMons[gActiveBattler].status2 & wantedToAnimate);
            MarkBattlerForControllerExec(gActiveBattler);
        }
        gBattlescriptCurrInstr += 6;
    }
}

static void atk66_chosenstatusanimation(void)
{
    u32 status;

	if (gBattleControllerExecFlags) return;

	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	status = T1_READ_32(gBattlescriptCurrInstr + 3);

	if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS)) {
			BtlController_EmitStatusAnimation(0, gBattlescriptCurrInstr[2], status);
			MarkBattlerForControllerExec(gActiveBattler);
	}
	gBattlescriptCurrInstr += 7;
}

static void atk67_yesnobox(void)
{
    switch (gBattleCommunication[0])
    {
    case 0:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
        BattlePutTextOnWindow(gText_BattleYesNoChoice, 0xE);
        ++gBattleCommunication[0];
        gBattleCommunication[CURSOR_POSITION] = 0;
        BattleCreateYesNoCursorAt();
        break;
    case 1:
        if (JOY_NEW(DPAD_UP) && gBattleCommunication[CURSOR_POSITION] != 0)
        {
            PlaySE(SE_SELECT);
            BattleDestroyYesNoCursorAt();
            gBattleCommunication[CURSOR_POSITION] = 0;
            BattleCreateYesNoCursorAt();
        }
        if (JOY_NEW(DPAD_DOWN) && gBattleCommunication[CURSOR_POSITION] == 0)
        {
            PlaySE(SE_SELECT);
            BattleDestroyYesNoCursorAt();
            gBattleCommunication[CURSOR_POSITION] = 1;
            BattleCreateYesNoCursorAt();
        }
        if (JOY_NEW(B_BUTTON))
        {
            gBattleCommunication[CURSOR_POSITION] = 1;
            PlaySE(SE_SELECT);
            HandleBattleWindow(0x17, 8, 0x1D, 0xD, WINDOW_CLEAR);
            ++gBattlescriptCurrInstr;
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            HandleBattleWindow(0x17, 8, 0x1D, 0xD, WINDOW_CLEAR);
            ++gBattlescriptCurrInstr;
        }
        break;
    }
}

static void atk68_cancelallactions(void)
{
    s32 i;

    for (i = 0; i < gBattlersCount; ++i)
        gActionsByTurnOrder[i] = B_ACTION_CANCEL_PARTNER;
    ++gBattlescriptCurrInstr;
}

// The same as 0x7, except there's no random damage multiplier.
static void atk69_adjustsetdamage(void)
{
    AdjustDamage(TRUE);
}

static void atk6A_removeitem(void)
{
    u8 partner;
    u16 partnerItem;
    u8 bank = gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8 oldItemEffect = ITEM_EFFECT(bank);
	
	if (gBattleStruct->doingPluckItemEffect) //This item removal was triggered by using someone else's item
	{
		gBattleStruct->doingPluckItemEffect = FALSE;
		gBattlescriptCurrInstr += 2;
		return;
	}
	
	gLastUsedItem = gBattleMons[bank].item;

	if (gLastUsedItem != ITEM_NONE)
	{
		if (!gBattleStruct->IncinerateCounters[bank]) //Item can be restored
		{
			CONSUMED_ITEMS(bank) = gLastUsedItem;
			SAVED_CONSUMED_ITEMS(bank) = gLastUsedItem;
			AddBankToPickupStack(bank);
		}

		gBattleMons[bank].item = ITEM_NONE;
		HandleUnburdenBoost(bank);
	}

	gBattlescriptCurrInstr += 2;

	partner = PARTNER(bank);
	partnerItem = gBattleMons[partner].item;

	if (IS_DOUBLE_BATTLE
	&& gBattleStruct->NoSymbiosisByte == FALSE
	&& ABILITY(partner) == ABILITY_SYMBIOSIS
	&& (!gBattleStruct->IncinerateCounters[bank] || oldItemEffect == HOLD_EFFECT_AIR_BALLOON) //Air Balloons can't be restored by Recycle, but they trigger Symbiosis
	&& !(gWishFutureKnock.knockedOffMons[SIDE(bank)] & gBitTable[gBattlerPartyIndexes[bank]])
	&& partnerItem != ITEM_NONE
	&& CanTransferItem(SPECIES(bank), partnerItem)
	&& CanTransferItem(SPECIES(partner), partnerItem))
	{
		gBattleMons[bank].item = partnerItem;
		gBattleMons[partner].item = 0;
		HandleUnburdenBoost(bank); //Remove the Unburden boost it may have gained

		BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[bank].item);
		MarkBattlerForControllerExec(bank);

		BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[partner].item);
		MarkBattlerForControllerExec(partner);

		gEffectBattler = bank;
		gBattleScripting.battler = partner;

		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_Symbiosis;
	}
	else
	{
		BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[bank].item);
		MarkBattlerForControllerExec(bank);
	}

	gBattleStruct->NoSymbiosisByte = FALSE;
	gBattleStruct->IncinerateCounters[bank] = 0;
	gWishFutureKnock.knockedOffMons[SIDE(bank)] &= ~(gBitTable[gBattlerPartyIndexes[bank]]);
}

static void atk6B_atknameinbuff1(void)
{
    PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker])
    ++gBattlescriptCurrInstr;
}

static void atk6C_drawlvlupbox(void)
{
    if (gBattleScripting.atk6C_state == 0)
    {
        if (IsMonGettingExpSentOut())
            gBattleScripting.atk6C_state = 3;
        else
            gBattleScripting.atk6C_state = 1;
    }

    switch (gBattleScripting.atk6C_state)
    {
    case 1:
        gBattle_BG2_Y = 0x60;
        SetBgAttribute(2, BG_ATTR_PRIORITY, 0);
        ShowBg(2);
        sub_8026480();
        gBattleScripting.atk6C_state = 2;
        break;
    case 2:
        if (!sub_80264D0())
            gBattleScripting.atk6C_state = 3;
        break;
    case 3:
        gBattle_BG1_X = 0;
        gBattle_BG1_Y = 0x100;
        SetBgAttribute(0, BG_ATTR_PRIORITY, 1);
        SetBgAttribute(1, BG_ATTR_PRIORITY, 0);
        ShowBg(0);
        ShowBg(1);
        HandleBattleWindow(18, 7, 0x1D, 0x13, WINDOW_x80);
        gBattleScripting.atk6C_state = 4;
        break;
    case 4:
        DrawLevelUpWindow1();
        PutWindowTilemap(12);
        CopyWindowToVram(12, COPYWIN_BOTH);
        ++gBattleScripting.atk6C_state;
        break;
    case 5:
    case 7:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            gBattle_BG1_Y = 0;
            ++gBattleScripting.atk6C_state;
        }
        break;
    case 6:
        if (gMain.newKeys)
        {
            PlaySE(SE_SELECT);
            DrawLevelUpWindow2();
            CopyWindowToVram(12, COPYWIN_GFX);
            ++gBattleScripting.atk6C_state;
        }
        break;
    case 8:
        if (gMain.newKeys)
        {
            PlaySE(SE_SELECT);
            HandleBattleWindow(18, 7, 0x1D, 0x13, WINDOW_x80 | WINDOW_CLEAR);
            ++gBattleScripting.atk6C_state;
        }
        break;
    case 9:
        if (!sub_8026648())
        {
            ClearWindowTilemap(13);
            CopyWindowToVram(13, COPYWIN_MAP);
            ClearWindowTilemap(12);
            CopyWindowToVram(12, COPYWIN_MAP);
            SetBgAttribute(2, BG_ATTR_PRIORITY, 2);
            ShowBg(2);
            gBattleScripting.atk6C_state = 10;
        }
        break;
    case 10:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            SetBgAttribute(0, BG_ATTR_PRIORITY, 0);
            SetBgAttribute(1, BG_ATTR_PRIORITY, 1);
            ShowBg(0);
            ShowBg(1);
            ++gBattlescriptCurrInstr;
        }
        break;
    }
}

static void DrawLevelUpWindow1(void)
{
    u16 currStats[NUM_STATS];

    GetMonLevelUpWindowStats(&gPlayerParty[gBattleStruct->expGetterMonId], currStats);
    DrawLevelUpWindowPg1(12, gBattleResources->beforeLvlUp->stats, currStats, 0xE, 0xD, 0xF);
}

static void DrawLevelUpWindow2(void)
{
    u16 currStats[NUM_STATS];

    GetMonLevelUpWindowStats(&gPlayerParty[gBattleStruct->expGetterMonId], currStats);
    DrawLevelUpWindowPg2(12, currStats, 0xE, 0xD, 0xF);
}

static void sub_8026480(void)
{
    gBattle_BG2_Y = 0;
    gBattle_BG2_X = 0x1A0;
    LoadPalette(sUnknownBattleboxPal, 0x60, 0x20);
    CopyToWindowPixelBuffer(13, sUnknownBattleboxGfx, 0, 0);
    PutWindowTilemap(13);
    CopyWindowToVram(13, COPYWIN_BOTH);
    PutMonIconOnLvlUpBox();
}

static bool8 sub_80264D0(void)
{
    if (IsDma3ManagerBusyWithBgCopy())
        return TRUE;
    if (gBattle_BG2_X == 0x200)
        return FALSE;
    if (gBattle_BG2_X == 0x1A0)
        PutLevelAndGenderOnLvlUpBox();
    gBattle_BG2_X += 8;
    if (gBattle_BG2_X >= 0x200)
        gBattle_BG2_X = 0x200;
    return (gBattle_BG2_X != 0x200);
}

static void PutLevelAndGenderOnLvlUpBox(void)
{
    u16 monLevel;
    u8 monGender;
    struct TextPrinterTemplate printerTemplate;
    u8 *txtPtr;
    u8 *txtPtr2;

    monLevel = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL);
    monGender = GetMonGender(&gPlayerParty[gBattleStruct->expGetterMonId]);
    GetMonNickname(&gPlayerParty[gBattleStruct->expGetterMonId], gStringVar4);
    printerTemplate.currentChar = gStringVar4;
    printerTemplate.windowId = 13;
    printerTemplate.fontId = 0;
    printerTemplate.x = 32;
    printerTemplate.y = 0;
    printerTemplate.currentX = 32;
    printerTemplate.currentY = 0;
    printerTemplate.letterSpacing = 0;
    printerTemplate.lineSpacing = 0;
    printerTemplate.unk = 0;
    printerTemplate.fgColor = TEXT_COLOR_WHITE;
    printerTemplate.bgColor = TEXT_COLOR_TRANSPARENT;
    printerTemplate.shadowColor = TEXT_COLOR_DARK_GRAY;
    AddTextPrinter(&printerTemplate, 0xFF, NULL);
    txtPtr = gStringVar4;
    gStringVar4[0] = 0xF9;
    *++txtPtr = 5;
    *++txtPtr = 0;
    txtPtr2 = txtPtr + 1;
    txtPtr = ConvertIntToDecimalStringN(++txtPtr, monLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
    txtPtr = StringFill(txtPtr, 0, 5);
    txtPtr = txtPtr2 + 4;
    if (monGender != MON_GENDERLESS)
    {
        if (monGender == MON_MALE)
        {
            txtPtr = WriteColorChangeControlCode(txtPtr, 0, 0xC);
            txtPtr = WriteColorChangeControlCode(txtPtr, 1, 0xD);
            *(txtPtr++) = CHAR_MALE;
        }
        else
        {
            txtPtr = WriteColorChangeControlCode(txtPtr, 0, 0xE);
            txtPtr = WriteColorChangeControlCode(txtPtr, 1, 0xF);
            *(txtPtr++) = CHAR_FEMALE;
        }
        *(txtPtr++) = EOS;
    }
    printerTemplate.y = 10;
    printerTemplate.currentY = 10;
    AddTextPrinter(&printerTemplate, TEXT_SPEED_FF, NULL);
    CopyWindowToVram(13, COPYWIN_GFX);
}

static bool8 sub_8026648(void)
{
    if (gBattle_BG2_X == 0x1A0)
        return FALSE;
    if (gBattle_BG2_X - 16 < 0x1A0)
        gBattle_BG2_X = 0x1A0;
    else
        gBattle_BG2_X -= 16;
    return (gBattle_BG2_X != 0x1A0);
}

#define sDestroy                    data[0]
#define sSavedLvlUpBoxXPosition     data[1]

static void PutMonIconOnLvlUpBox(void)
{
    u8 spriteId;
    const u16 *iconPal;
    struct SpriteSheet iconSheet;
    struct SpritePalette iconPalSheet;
    u16 species = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPECIES);
    u32 personality = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_PERSONALITY);
    const u8 *iconPtr = GetMonIconPtr(species, personality, 1);

    iconSheet.data = iconPtr;
    iconSheet.size = 0x200;
    iconSheet.tag = MON_ICON_LVLUP_BOX_TAG;
    iconPal = GetValidMonIconPalettePtr(species);
    iconPalSheet.data = iconPal;
    iconPalSheet.tag = MON_ICON_LVLUP_BOX_TAG;
    LoadSpriteSheet(&iconSheet);
    LoadSpritePalette(&iconPalSheet);
    spriteId = CreateSprite(&sSpriteTemplate_MonIconOnLvlUpBox, 256, 10, 0);
    gSprites[spriteId].sDestroy = FALSE;
    gSprites[spriteId].sSavedLvlUpBoxXPosition = gBattle_BG2_X;
}

static void SpriteCB_MonIconOnLvlUpBox(struct Sprite* sprite)
{
    sprite->x2 = sprite->sSavedLvlUpBoxXPosition - gBattle_BG2_X;
    if (sprite->x2 != 0)
    {
        sprite->sDestroy = TRUE;
    }
    else if (sprite->sDestroy)
    {
        DestroySprite(sprite);
        FreeSpriteTilesByTag(MON_ICON_LVLUP_BOX_TAG);
        FreeSpritePaletteByTag(MON_ICON_LVLUP_BOX_TAG);
    }
}

bool32 IsMonGettingExpSentOut(void)
{
    if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId)
        return TRUE;
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId)
        return TRUE;
    return FALSE;
}

static void atk6D_resetsentmonsvalue(void)
{
    ResetSentPokesToOpponentValue();
    ++gBattlescriptCurrInstr;
}

static void atk6E_setatktoplayer0(void)
{
    gBattlerAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
    ++gBattlescriptCurrInstr;
}

static void atk6F_makevisible(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    BtlController_EmitSpriteInvisibility(0, FALSE);
    MarkBattlerForControllerExec(gActiveBattler);

    gBattlescriptCurrInstr += 2;
}

static void atk70_recordlastability(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
	gBattlescriptCurrInstr += 2;
}

void BufferMoveToLearnIntoBattleTextBuff2(void)
{
    PREPARE_MOVE_BUFFER(gBattleTextBuff2, gMoveToLearn);
}

static void atk71_buffermovetolearn(void)
{
    BufferMoveToLearnIntoBattleTextBuff2();
    ++gBattlescriptCurrInstr;
}

static void atk72_jumpifplayerran(void)
{
    if (TryRunFromBattle(gBattlerFainted))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
        gBattlescriptCurrInstr += 5;
}

static void atk73_hpthresholds(void)
{
    u8 opposingBattler;
    s32 result;

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        opposingBattler = gActiveBattler ^ BIT_SIDE;

        result = gBattleMons[opposingBattler].hp * 100 / gBattleMons[opposingBattler].maxHP;
        if (result == 0)
            result = 1;
        if (result > 69 || !gBattleMons[opposingBattler].hp)
            gBattleStruct->hpScale = 0;
        else if (result > 39)
            gBattleStruct->hpScale = 1;
        else if (result > 9)
            gBattleStruct->hpScale = 2;
        else
            gBattleStruct->hpScale = 3;
    }
    gBattlescriptCurrInstr += 2;
}

static void atk74_hpthresholds2(void)
{
    u8 opposingBattler;
    s32 result;
    u8 hpSwitchout;

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        opposingBattler = gActiveBattler ^ BIT_SIDE;
        hpSwitchout = *(gBattleStruct->hpOnSwitchout + GetBattlerSide(opposingBattler));
        result = (hpSwitchout - gBattleMons[opposingBattler].hp) * 100 / hpSwitchout;

        if (gBattleMons[opposingBattler].hp >= hpSwitchout)
            gBattleStruct->hpScale = 0;
        else if (result <= 29)
            gBattleStruct->hpScale = 1;
        else if (result <= 69)
            gBattleStruct->hpScale = 2;
        else
            gBattleStruct->hpScale = 3;
    }
    gBattlescriptCurrInstr += 2;
}

static void atk75_useitemonopponent(void)
{
    gBattlerInMenuId = gBattlerAttacker;
    PokemonUseItemEffects(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker]], gLastUsedItem, gBattlerPartyIndexes[gBattlerAttacker], 0, 1);
    ++gBattlescriptCurrInstr;
}

static void atk76_various(void)
{
    u8 side;
    s32 i;
    u32 monToCheck, status;
    u16 species;
    u8 abilityNum;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

    switch (gBattlescriptCurrInstr[2])
    {
    case VARIOUS_CANCEL_MULTI_TURN_MOVES:
        CancelMultiTurnMoves(gActiveBattler);
        break;
    case VARIOUS_SET_MAGIC_COAT_TARGET:
        gBattlerAttacker = gBattlerTarget;
        side = GetBattlerSide(gBattlerAttacker) ^ BIT_SIDE;
        if (gSideTimers[side].followmeTimer != 0 && gBattleMons[gSideTimers[side].followmeTarget].hp != 0)
            gBattlerTarget = gSideTimers[side].followmeTarget;
        else
            gBattlerTarget = gActiveBattler;
        break;
    case VARIOUS_IS_RUNNING_IMPOSSIBLE:
        gBattleCommunication[0] = IsRunningFromBattleImpossible();
        break;
    case VARIOUS_GET_MOVE_TARGET:
        gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
        break;
    case VARIOUS_CASE_4:
        if (gHitMarker & HITMARKER_FAINTED(gActiveBattler))
            gBattleCommunication[0] = 1;
        else
            gBattleCommunication[0] = 0;
        break;
    case VARIOUS_RESET_INTIMIDATE_TRACE_BITS:
        gSpecialStatuses[gActiveBattler].intimidatedMon = 0;
        gSpecialStatuses[gActiveBattler].traced = 0;
        break;
    case VARIOUS_UPDATE_CHOICE_MOVE_ON_LVL_UP:
        if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId || gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId)
        {
            u16 *choicedMove;

            if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId)
                gActiveBattler = 0;
            else
                gActiveBattler = 2;
            choicedMove = &gBattleStruct->choicedMove[gActiveBattler];
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
                if (gBattleMons[gActiveBattler].moves[i] == *choicedMove)
                    break;
            }
            if (i == MAX_MON_MOVES)
                *choicedMove = MOVE_NONE;
        }
        break;
    case VARIOUS_RESET_PLAYER_FAINTED_FLAG:
        if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLE))
         && gBattleTypeFlags & BATTLE_TYPE_TRAINER
         && gBattleMons[0].hp != 0
         && gBattleMons[1].hp != 0)
            gHitMarker &= ~(HITMARKER_PLAYER_FAINTED);
        break;
    case VARIOUS_CASE_8:
        i = 0; // redundant
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        gActiveBattler = 1;
        for (i = 0; gActiveBattler < MAX_BATTLERS_COUNT; gActiveBattler += 2)
        {
            if (gActiveBattler < gBattlersCount && gBattleMons[gActiveBattler].hp != 0)
                gBattleCommunication[MULTISTRING_CHOOSER] |= gBitTable[i];
            ++i;
        }
        break;
    case VARIOUS_RETURN_OPPONENT_MON1:
        gActiveBattler = 1;
        if (gBattleMons[gActiveBattler].hp != 0)
        {
            BtlController_EmitReturnMonToBall(0, 0);
            MarkBattlerForControllerExec(gActiveBattler);
        }
        break;
    case VARIOUS_RETURN_OPPONENT_MON2:
        if (gBattlersCount > 3)
        {
            gActiveBattler = 3;
            if (gBattleMons[gActiveBattler].hp != 0)
            {
                BtlController_EmitReturnMonToBall(0, 0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
        break;
    case VARIOUS_CHECK_POKEFLUTE:
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        monToCheck = 0;
        for (i = 0; i < gBattlersCount; ++i)
        {
            if (gBattleMons[i].ability != ABILITY_SOUNDPROOF)
            {
                gBattleMons[i].status1 &= ~STATUS1_SLEEP;
                gBattleMons[i].status2 &= ~STATUS2_NIGHTMARE;

            }
        }
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2);
            abilityNum = GetMonData(&gPlayerParty[i], MON_DATA_ABILITY_NUM);
            status = GetMonData(&gPlayerParty[i], MON_DATA_STATUS);
            if (species != SPECIES_NONE
             && species != SPECIES_EGG
             && status & AILMENT_FNT
             && GetMonAbility(&gPlayerParty[i]) != ABILITY_SOUNDPROOF)
                monToCheck |= (1 << i);
        }
        if (monToCheck)
        {
            gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
            status = 0;
            BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, monToCheck, 4, &status);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        }
        monToCheck = 0;
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            species = GetMonData(&gEnemyParty[i], MON_DATA_SPECIES2);
            abilityNum = GetMonData(&gEnemyParty[i], MON_DATA_ABILITY_NUM);
            status = GetMonData(&gEnemyParty[i], MON_DATA_STATUS);

            if (species != SPECIES_NONE
             && species != SPECIES_EGG
             && status & AILMENT_FNT
             && GetMonAbility(&gEnemyParty[i]) != ABILITY_SOUNDPROOF)
                monToCheck |= (1 << i);
        }
        if (monToCheck)
        {
            gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
            status = 0;
            BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, monToCheck, 4, &status);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattleCommunication[5] = 1;
        }
        break;
    case VARIOUS_WAIT_FANFARE:
        if (!IsFanfareTaskInactive())
            return;
        break;
    }
    gBattlescriptCurrInstr += 3;
}

static void atk77_setprotectlike(void)
{
    bool8 notLastTurn = TRUE;
	u16 rate = 0xFFFF;
	u16 divisor = 1;
	int i;

	switch (gLastResultingMoves[gBattlerAttacker]) {
		case MOVE_PROTECT:
		case MOVE_DETECT:
		case MOVE_ENDURE:
		case MOVE_KINGSSHIELD:
		case MOVE_SPIKYSHIELD:
		case MOVE_BANEFULBUNKER:
		case MOVE_CRAFTYSHIELD:
		case MOVE_MATBLOCK:
		case MOVE_QUICKGUARD:
		case MOVE_WIDEGUARD:
		case MOVE_MAX_GUARD:
			break;
		default:
			gDisableStructs[gBattlerAttacker].protectUses = 0;
	}

	if (gCurrentTurnActionNumber == (gBattlersCount - 1))
		notLastTurn = FALSE;

	if (gCurrentMove == MOVE_WIDEGUARD || gCurrentMove == MOVE_QUICKGUARD || gCurrentMove == MOVE_CRAFTYSHIELD || gCurrentMove == MOVE_MATBLOCK)
		divisor = 1; //These moves increase the fail rate, but can't fail themselves
	else
	{
		for (i = 0; i < gDisableStructs[gBattlerAttacker].protectUses && i <= 6; ++i)
			divisor *= 3;
	}

	if (rate / divisor >= Random() && notLastTurn)
	{
		u8 atkSide = SIDE(gBattlerAttacker);
		switch (gCurrentMove) {
			case MOVE_KINGSSHIELD:
				gProtectStructs[gBattlerAttacker].KingsShield = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				break;

			case MOVE_SPIKYSHIELD:
				gProtectStructs[gBattlerAttacker].SpikyShield = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				break;

			case MOVE_BANEFULBUNKER:
				gProtectStructs[gBattlerAttacker].BanefulBunker = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				break;

			case MOVE_OBSTRUCT:
				gProtectStructs[gBattlerAttacker].obstruct = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				break;

			case MOVE_CRAFTYSHIELD:
				gSideStatuses[atkSide] |= SIDE_STATUS_CRAFTY_SHIELD;
				gBattleCommunication[MULTISTRING_CHOOSER] = 3;
				gBattleStringLoader = CraftyShieldSetString;
				break;

			case MOVE_MATBLOCK:
				if (!gDisableStructs[gBattlerAttacker].isFirstTurn)
					goto PROTECT_FAILED;

				gSideStatuses[atkSide] |= SIDE_STATUS_MAT_BLOCK;
				gBattleCommunication[MULTISTRING_CHOOSER] = 3;
				gBattleStringLoader = MatBlockSetString;
				break;

			case MOVE_QUICKGUARD:
				gSideStatuses[atkSide] |= SIDE_STATUS_QUICK_GUARD;
				gBattleCommunication[MULTISTRING_CHOOSER] = 3;
				gBattleStringLoader = QuickGuardSetString;
				break;

			case MOVE_WIDEGUARD:
				gSideStatuses[atkSide] |= SIDE_STATUS_WIDE_GUARD;
				gBattleCommunication[MULTISTRING_CHOOSER] = 3;
				gBattleStringLoader = WideGuardSetString;
				break;

			case MOVE_ENDURE:
				gProtectStructs[gBattlerAttacker].endured = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 1;
				break;

			default:
				gProtectStructs[gBattlerAttacker].protected = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		}

		++gDisableStructs[gBattlerAttacker].protectUses;
	}
	else
	{
	PROTECT_FAILED:
		gDisableStructs[gBattlerAttacker].protectUses = 0;
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		gMoveResultFlags |= MOVE_RESULT_MISSED;
	}

	gBattlescriptCurrInstr++;
}

static void atk78_faintifabilitynotdamp(void)
{
    if (gBattleControllerExecFlags) return;

	gActiveBattler = gBattlerAttacker;
	gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
	BtlController_EmitHealthBarUpdate(0, INSTANT_HP_BAR_DROP);
	MarkBattlerForControllerExec(gActiveBattler);
	gBattlescriptCurrInstr++;
}

static void atk79_setatkhptozero(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = gBattlerAttacker;
        gBattleMons[gActiveBattler].hp = 0;
        BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
        MarkBattlerForControllerExec(gActiveBattler);
        ++gBattlescriptCurrInstr;
    }
}

static void atk7A_jumpifnexttargetvalid(void)
{
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (IS_DOUBLE_BATTLE) {

		if (gBattlerAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
		&& gBattlerTarget == GetBattlerAtPosition(B_POSITION_PLAYER_LEFT))
		{
			gBattlescriptCurrInstr += 5;
			return;
		}

		for (gBattlerTarget++; gBattlerTarget < gBattlersCount; gBattlerTarget++)
		{
			if (gBattlerTarget == gBattlerAttacker && !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_USER))
				continue;
			if (!(gAbsentBattlerFlags & gBitTable[gBattlerTarget]))
				break;
		}

		if (gBattlerTarget >= gBattlersCount)
			if (gBattlerAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
			&&  !(gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]))
			{
				gBattlerTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
				gBattlescriptCurrInstr = jump_loc;
			}
			else
				gBattlescriptCurrInstr += 5;
		else
			gBattlescriptCurrInstr = jump_loc;
	}
	else
		gBattlescriptCurrInstr += 5;
}

static void atk7B_tryhealhalfhealth(void)
{
    const u8* failPtr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (gBattlescriptCurrInstr[5] == BS_GET_ATTACKER)
		gBattlerTarget = gBattlerAttacker;

	if (BATTLER_MAX_HP(gBattlerTarget))
		gBattlescriptCurrInstr = failPtr;
	else
	{
		if (gCurrentMove == MOVE_LIFEDEW)
			gBattleMoveDamage = MathMax(1, (GetBaseMaxHP(gBattlerTarget) / 4)); //25 %
		else
			gBattleMoveDamage = MathMax(1, (GetBaseMaxHP(gBattlerTarget) / 2)); //50 %

		gBattleMoveDamage *= -1;
		gBattlescriptCurrInstr += 6;
	}
}

static void atk7C_trymirrormove(void)
{
    s32 validMovesCount;
	s32 i;
	u16 move;
	u16 movesArray[4] = {0};

	for (validMovesCount = 0, i = 0; i < gBattlersCount; i++)
	{
		if (i != gBattlerAttacker)
		{
			move = gBattleStruct->lastTakenMoveFrom[gBattlerAttacker][i];

			if (move != 0 && move != 0xFFFF)
				movesArray[validMovesCount++] = move;
		}
	}

	move = gBattleStruct->lastTakenMove[gBattlerAttacker];

	if (move != 0 && move != 0xFFFF)
	{
		gCurrentMove = move;
		gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
		TryUpdateCalledMoveWithZMove();
		UpdateMoveStartValuesForCalledMove();
		gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
	}
	else if (validMovesCount)
	{
		i = Random() % validMovesCount;
		gCurrentMove = movesArray[i];
		gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
		TryUpdateCalledMoveWithZMove();
		UpdateMoveStartValuesForCalledMove();
		gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
	}
	else
	{
		gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
		gBattlescriptCurrInstr++;
	}
}

bool8 SetRainyWeather(void)
{
	if (gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS))
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		return FALSE;
	}

	gBattleWeather = WEATHER_RAIN_TEMPORARY;
	gBattleCommunication[MULTISTRING_CHOOSER] = 0;

	if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_DAMP_ROCK)
		gWishFutureKnock.weatherDuration = 8;
	else
		gWishFutureKnock.weatherDuration = 5;

	gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
	return TRUE;
}

static void atk7D_setrain(void)
{
    if (!SetRainyWeather())
		gMoveResultFlags |= MOVE_RESULT_FAILED;

	gBattlescriptCurrInstr++;
}

static void atk7E_setreflect(void)
{
    if (gSideStatuses[SIDE(gBattlerAttacker)] & SIDE_STATUS_REFLECT) {
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}
	else {
		gSideStatuses[SIDE(gBattlerAttacker)] |= SIDE_STATUS_REFLECT;

		if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_LIGHT_CLAY)
			gSideTimers[SIDE(gBattlerAttacker)].reflectTimer = 8;
		else
			gSideTimers[SIDE(gBattlerAttacker)].reflectTimer = 5;

		PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_DEF);
		gBattleStringLoader = gText_ScreenRaisedStat;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}
	gBattlescriptCurrInstr++;
}

const u16 gReflectLightScreenSafeguardStringIds[] =
{
	STRINGID_BUTITFAILED, STRINGID_CUSTOMSTRING
};

static void atk7F_setseeded(void)
{
    if ((gMoveResultFlags & MOVE_RESULT_NO_EFFECT) || (gStatuses3[gBattlerTarget] & STATUS3_LEECHSEED)) {
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}

	else if (IsOfType(gBattlerTarget, TYPE_GRASS)) {
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
	}

	else {
		gStatuses3[gBattlerTarget] |= gBattlerAttacker;
		gStatuses3[gBattlerTarget] |= STATUS3_LEECHSEED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}

	gBattlescriptCurrInstr++;
}

static void atk80_manipulatedamage(void)
{
    switch (gBattlescriptCurrInstr[1])
    {
    case ATK80_DMG_CHANGE_SIGN:
        gBattleMoveDamage *= -1;
        break;
    case ATK80_DMG_HALF_BY_TWO_NOT_MORE_THAN_HALF_MAX_HP:
        gBattleMoveDamage /= 2;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        if ((gBattleMons[gBattlerTarget].maxHP / 2) < gBattleMoveDamage)
            gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 2;
        break;
    case ATK80_DMG_DOUBLED:
        gBattleMoveDamage *= 2;
        break;
    }
    gBattlescriptCurrInstr += 2;
}

static void atk81_trysetrest(void)
{
    bool8 fail = FALSE;
	gActiveBattler = gBattlerTarget = gBattlerAttacker;
	gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP * (-1);

	if (CheckGrounding(gActiveBattler) && (gTerrainType == MISTY_TERRAIN || gTerrainType == ELECTRIC_TERRAIN))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
		fail = TRUE;
	}
	else if (IsOfType(gActiveBattler, TYPE_GRASS) && ABILITY(gActiveBattler) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.battler = gActiveBattler;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		fail = TRUE;
	}
	else if (IsOfType(gActiveBattler, TYPE_GRASS) && IS_DOUBLE_BATTLE && ABILITY(PARTNER(gActiveBattler)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.battler = PARTNER(gActiveBattler);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		fail = TRUE;
	}
	else if (ABILITY(gActiveBattler) == ABILITY_SWEETVEIL)
	{
		gBattleScripting.battler = gActiveBattler;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		fail = TRUE;
	}
	else if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(gActiveBattler)) == ABILITY_SWEETVEIL)
	{
		gBattleScripting.battler = PARTNER(gActiveBattler);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		fail = TRUE;
	}
	else if (IsUproarBeingMade())
	{
		gBattleScripting.battler = gActiveBattler;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		gBattlescriptCurrInstr = BattleScript_RestCantSleep;
		fail = TRUE;
	}
	else if (BATTLER_MAX_HP(gActiveBattler))
	{
		gBattleScripting.battler = gActiveBattler;
		gBattlescriptCurrInstr = BattleScript_AlreadyAtFullHp;
		fail = TRUE;
	}

	if (!fail)
	{
		switch (ABILITY(gActiveBattler)) {
			case ABILITY_INSOMNIA:
			case ABILITY_VITAL_SPIRIT:
				gBattlescriptCurrInstr = BattleScript_TargetStayedAwakeUsingAbility;
				fail = TRUE;
				break;
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					fail = TRUE;
				}
				break;
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ButItFailed;
				fail = TRUE;
				break;
			#ifdef SPECIES_MINIOR_SHIELD
			case ABILITY_SHIELDSDOWN:
				if (SPECIES(gBattlerAttacker) == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ButItFailed;
					fail = TRUE;
				}
				break;
			#endif
		}
	}

	if (!fail)
	{
		if (gBattleMons[gActiveBattler].status1 & ((u8)(~STATUS1_SLEEP)))
			gBattleCommunication[MULTISTRING_CHOOSER] = 1;
		else
			gBattleCommunication[MULTISTRING_CHOOSER] = 0;

		gBattleMons[gBattlerTarget].status1 = 3;
		BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
		MarkBattlerForControllerExec(gActiveBattler);
		gBattlescriptCurrInstr += 5;
	}
}

static void atk82_jumpifnotfirstturn(void)
{
    u8* failJump = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (gDisableStructs[gBattlerAttacker].isFirstTurn
	&& !gBattleStruct->InstructInProgress)
		gBattlescriptCurrInstr += 5;
	else
		gBattlescriptCurrInstr = failJump;
}

static void atk83_nop(void)
{
    ++gBattlescriptCurrInstr;
}

bool8 UproarWakeUpCheck(u8 bank)
{
	u32 i;

	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gBattleMons[i].status2 & STATUS2_UPROAR)
		{
			gBattleScripting.battler = i;
			if (gBattlerTarget == 0xFF)
				gBattlerTarget = i;
			else if (gBattlerTarget == i)
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			else
				gBattleCommunication[MULTISTRING_CHOOSER] = 1;

			break;
		}
	}

	return i < gBattlersCount;
}

static void atk84_jumpifcantmakeasleep(void)
{
    u8 bankDef = gBattlerTarget;
	u8 defPartner = PARTNER(bankDef);
	ability_t defAbility = ABILITY(bankDef);
	ability_t defPartnerAbility = ABILITY(defPartner);

	u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (UproarWakeUpCheck(bankDef)
	#ifdef SPECIES_MINIOR_SHIELD
	|| SPECIES(bankDef) == SPECIES_MINIOR_SHIELD
	#endif
	)
		gBattlescriptCurrInstr = jump_loc;

	else if (defAbility == ABILITY_INSOMNIA || defAbility == ABILITY_VITAL_SPIRIT || defAbility == ABILITY_COMATOSE || defAbility == ABILITY_SWEETVEIL
	|| (defAbility == ABILITY_LEAFGUARD && WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
	|| (defAbility == ABILITY_FLOWERVEIL && IsOfType(bankDef, TYPE_GRASS) && gCurrentMove != MOVE_REST)) {
		gLastUsedAbility = defAbility;
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		gBattlescriptCurrInstr = jump_loc;
		RecordAbilityBattle(bankDef, gLastUsedAbility);
	}

	else if (defPartnerAbility == ABILITY_SWEETVEIL
	|| (defPartnerAbility == ABILITY_FLOWERVEIL && IsOfType(bankDef, TYPE_GRASS) && gCurrentMove != MOVE_REST)) {
		gLastUsedAbility = defPartnerAbility;
		gBattlescriptCurrInstr = jump_loc;
		RecordAbilityBattle(defPartner, gLastUsedAbility);
	}

	else if (CheckGrounding(bankDef) && (gTerrainType == ELECTRIC_TERRAIN || gTerrainType == MISTY_TERRAIN))
		gBattlescriptCurrInstr = jump_loc;

	else
		gBattlescriptCurrInstr += 5;
}

static void atk85_stockpile(void)
{
    if (gDisableStructs[gBattlerAttacker].stockpileCounter == 3)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        ++gDisableStructs[gBattlerAttacker].stockpileCounter;
        PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff1, 1, gDisableStructs[gBattlerAttacker].stockpileCounter)
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    ++gBattlescriptCurrInstr;
}

static void atk86_stockpiletobasedamage(void)
{
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (gDisableStructs[gBattlerAttacker].stockpileCounter == 0)
		gBattlescriptCurrInstr = jump_loc;
	else
	{
		gBattleScripting.animTurn = gDisableStructs[gBattlerAttacker].stockpileCounter;
		gBattlescriptCurrInstr += 5;
	}
}

static void atk87_stockpiletohpheal(void)
{
    u8* jumpPtr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (gDisableStructs[gBattlerAttacker].stockpileCounter == 0)
	{
		gBattlescriptCurrInstr = jumpPtr;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}
	else if (BATTLER_MAX_HP(gBattlerAttacker))
	{
		gDisableStructs[gBattlerAttacker].stockpileCounter = 0;
		gBattlescriptCurrInstr = jumpPtr;
		gBattlerTarget = gBattlerAttacker;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}
	else
	{
		gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBattlerAttacker) / (1 << (3 - gDisableStructs[gBattlerAttacker].stockpileCounter)));
		gBattleMoveDamage *= -1;

		gBattleScripting.animTurn = gDisableStructs[gBattlerAttacker].stockpileCounter;
		gBattlescriptCurrInstr += 5;
		gBattlerTarget = gBattlerAttacker;
	}
}

static void atk88_negativedamage(void)
{
    if (gCurrentMove == MOVE_OBLIVIONWING || MOVE_DRAININGKISS)
		gBattleMoveDamage = 75 * gHpDealt / 100;
	else
		gBattleMoveDamage = (gHpDealt / 2);

	if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_BIG_ROOT)
		gBattleMoveDamage = 130 * gHpDealt / 100;

	gBattleMoveDamage *= -1;

	if (gBattleMoveDamage == 0)
		gBattleMoveDamage = -1;

	gBattlescriptCurrInstr++;
}

static void atk89_statbuffchange(void)
{
	u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	if (ChangeStatBuffs(gBattleScripting.statChanger & 0xF0, gBattleScripting.statChanger & 0xF, T2_READ_8(gBattlescriptCurrInstr + 1), jump_loc) == 0)
		gBattlescriptCurrInstr += 6;
}

static void atk8A_normalisebuffs(void) // haze
{
    int i, j;

	for (i = 0; i < gBattlersCount; ++i)
	{
		for (j = 0; j < BATTLE_STATS_NO-1; ++j)
			gBattleMons[i].statStages[j] = 6;
	}

	++gBattlescriptCurrInstr;
}

static void atk8B_setbide(void)
{
    gBattleMons[gBattlerAttacker].status2 |= STATUS2_MULTIPLETURNS;
    gLockedMoves[gBattlerAttacker] = gCurrentMove;
    gTakenDmg[gBattlerAttacker] = 0;
    gBattleMons[gBattlerAttacker].status2 |= (STATUS2_BIDE - 0x100); // 2 turns
    ++gBattlescriptCurrInstr;
}

static void atk8C_confuseifrepeatingattackends(void)
{
    if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_LOCK_CONFUSE))
        gBattleCommunication[MOVE_EFFECT_BYTE] = (MOVE_EFFECT_THRASH | MOVE_EFFECT_AFFECTS_USER);
    ++gBattlescriptCurrInstr;
}

static void atk8D_setmultihitcounter(void)
{
    if (gBattlescriptCurrInstr[1])
		gMultiHitCounter = gBattlescriptCurrInstr[1];

	if (ABILITY(gBattlerAttacker) == ABILITY_SKILLLINK)
		gMultiHitCounter = 5;

	#ifdef SPECIES_ASHGRENINJA
	else if (ABILITY(gBattlerAttacker) == ABILITY_BATTLEBOND
	&& gCurrentMove == MOVE_WATERSHURIKEN
	&& gBattleMons[gBattlerAttacker].species == SPECIES_ASHGRENINJA)
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

	gBattlescriptCurrInstr += 2;
}

static void atk8E_initmultihitstring(void)
{
    PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0)
    ++gBattlescriptCurrInstr;
}

static void atk90_tryconversiontypechange(void) // randomly changes user's type to one of its moves' type
{
    u8* failScript = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (gCurrentMove != MOVE_REFLECTTYPE) //Conversion
	{
		u8 moveType = gBattleMoves[gBattleMons[gBattlerAttacker].moves[0]].type;

		if (IS_BLANK_TYPE(moveType))
		{
			gBattlescriptCurrInstr = failScript;
		}
		else if(
			moveType == gBattleMons[gBattlerAttacker].type1
		||  moveType == gBattleMons[gBattlerAttacker].type2
		||  moveType == gBattleMons[gBattlerAttacker].type3)
		{
			gBattlescriptCurrInstr = failScript;
		}
		else
		{
			gBattleMons[gBattlerAttacker].type1 = moveType;
			gBattleMons[gBattlerAttacker].type2 = moveType;
			gBattleMons[gBattlerAttacker].type3 = TYPE_BLANK;

			PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);

			gBattlescriptCurrInstr += 5;
		}
	}
	else //Reflect Type
	{
		u8 defType1 = gBattleMons[gBattlerTarget].type1;
		u8 defType2 = gBattleMons[gBattlerTarget].type2;
		u8 defType3 = gBattleMons[gBattlerTarget].type3;

		//If target has no type
		if((IS_BLANK_TYPE(defType1))
		&& (IS_BLANK_TYPE(defType2))
		&& (IS_BLANK_TYPE(defType3)))
		{
			gBattlescriptCurrInstr = failScript;
		}
		else
		{
			//If target has no main types, but has a third type
			if ((IS_BLANK_TYPE(defType1))
			&&  (IS_BLANK_TYPE(defType2))
			&& !(IS_BLANK_TYPE(defType3))) {
				defType1 = TYPE_NORMAL;
				defType2 = TYPE_NORMAL;
			}

			else { //Target Has Main Type
				if (IS_BLANK_TYPE(defType1))
					defType1 = defType2;
				else if (IS_BLANK_TYPE(defType2))
					defType2 = defType1;

				if (IS_BLANK_TYPE(defType3)) //Just in case it has a burned out Fire type
					defType3 = TYPE_BLANK;
			}

			gBattleMons[gBattlerAttacker].type1 = defType1;
			gBattleMons[gBattlerAttacker].type2 = defType2;
			gBattleMons[gBattlerAttacker].type3 = defType3;

			gBattlescriptCurrInstr += 5;
		}
	}
}

static void atk91_givepaydaymoney(void)
{
    int i;
	u32 money = 0;

	if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_TOWER)))
	{
		for (i = 0; i < PARTY_SIZE; ++i)
			money += (gPlayerParty[i].level * 5) * gBattleStruct->PayDayByPartyIndices[i];
		money *= gBattleStruct->moneyMultiplier;
		money += MathMin(gBattleStruct->maxGoldrushMoney * gBattleStruct->moneyMultiplier, 99999); //Gold Rush caps at $99 999

		if (money > 0)
		{
			AddMoney(&gSaveBlock1Ptr->money, money);
			PREPARE_WORD_NUMBER_BUFFER(gBattleTextBuff1, 10, money);
			BattleScriptPush(gBattlescriptCurrInstr + 1);
			gBattlescriptCurrInstr = BattleScript_PrintPayDayMoneyString;
			return;
		}
	}
	gBattlescriptCurrInstr++;
}

static void atk92_setlightscreen(void)
{
    if (gSideStatuses[SIDE(gBattlerAttacker)] & SIDE_STATUS_LIGHTSCREEN)
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}
	else
	{
		gSideStatuses[SIDE(gBattlerAttacker)] |= SIDE_STATUS_LIGHTSCREEN;

		if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_LIGHT_CLAY)
			gSideTimers[SIDE(gBattlerAttacker)].lightscreenTimer = 8;
		else
			gSideTimers[SIDE(gBattlerAttacker)].lightscreenTimer = 5;

		PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_SPDEF);
		gBattleStringLoader = gText_ScreenRaisedStat;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}
	gBattlescriptCurrInstr++;
}

static void atk93_tryKO(void)
{
    u8 bankAtk = gBattlerAttacker;
	u8 bankDef = gBattlerTarget;

	ability_t atkAbility = ABILITY(bankAtk);
	ability_t defAbility = ABILITY(bankDef);
	u8 defEffect = ITEM_EFFECT(bankDef);
	u8 defQuality = ITEM_QUALITY(bankDef);
	u8 mystery = itemid_get_x19(ITEM(bankDef));

	gPotentialItemEffectBattler = bankDef;

	if (defAbility == ABILITY_STURDY)
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleScripting.battler = gBattlerTarget;
		gBattlescriptCurrInstr = BattleScript_SturdyPreventsOHKO;
	}
	else
	{
		u16 chance = FALSE;
		u8 baseAcc;
		#ifdef NO_SHEER_COLD_NERF
			baseAcc = gBattleMoves[gCurrentMove].accuracy;
		#else
			if (gCurrentMove == MOVE_SHEER_COLD && !IsOfType(bankAtk, TYPE_ICE))
				baseAcc = 20;
			else
				baseAcc = gBattleMoves[gCurrentMove].accuracy;
		#endif

		if (gBattleMons[bankAtk].level >= gBattleMons[bankDef].level)
		{
			if (IsRaidBattle() && gBattlerTarget == BANK_RAID_BOSS)
			{
				if (!gBattleStruct->dynamaxData.raidShieldsUp)
					chance = FALSE; //Never works on regular raid mon
				else
					chance = TRUE; //Always breaks 2 raid shields
			}
			else if (IsDynamaxed(gBattlerTarget))
			{
				chance = FALSE; //Doesn't work on Dynamaxed foe
			}
			#ifndef NO_SHEER_COLD_NERF
			else if (gCurrentMove == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
			{
				chance = FALSE;
			}
			#endif
			else if (((gStatuses3[bankDef] & STATUS3_ALWAYS_HITS && gDisableStructs[bankDef].battlerWithSureHit == bankAtk)
			|| atkAbility == ABILITY_NOGUARD || defAbility == ABILITY_NOGUARD))
			{
				chance = TRUE;
			}
			else
			{
				chance = baseAcc + (gBattleMons[bankAtk].level - gBattleMons[bankDef].level);
				if ((Random() % 100) + 1 < chance)
					chance = TRUE;
				else
					chance = FALSE;
			}
		}
		else
			chance = FALSE;

		if (chance)
		{
			if (IsRaidBattle() && gBattlerTarget == BANK_RAID_BOSS && gBattleStruct->dynamaxData.raidShieldsUp)
			{
				//Just break shields
			}
			else if (defEffect == HOLD_EFFECT_FOCUS_BAND && !mystery && Random() % 100 < defQuality)
			{
				RecordItemEffectBattle(bankDef, defEffect);
				gSpecialStatuses[bankDef].focusBanded = 1;
			}
			//Focus Sash
			else if (IsBankHoldingFocusSash(bankDef) && BATTLER_MAX_HP(bankDef))
			{
				RecordItemEffectBattle(bankDef, defEffect);
				gSpecialStatuses[bankDef].focusBanded = 1;
				gBattleStruct->EnduranceHelper[gBattlerTarget] = ENDURE_FOCUS_SASH;
			}

			if (IsRaidBattle() && gBattlerTarget == BANK_RAID_BOSS && gBattleStruct->dynamaxData.raidShieldsUp)
			{
				//Just break shields
			}
			else if (gProtectStructs[bankDef].endured)
			{
				gBattleMoveDamage = gBattleMons[bankDef].hp - 1;
				gMoveResultFlags |= MOVE_RESULT_FOE_ENDURED;
			}
			else if (gSpecialStatuses[bankDef].focusBanded)
			{
				gBattleMoveDamage = gBattleMons[bankDef].hp - 1;
				gMoveResultFlags |= MOVE_RESULT_FOE_HUNG_ON;
				gLastUsedItem = gBattleMons[bankDef].item;
			}
			else
			{
				gBattleMoveDamage = gBattleMons[bankDef].hp;
				gMoveResultFlags |= MOVE_RESULT_ONE_HIT_KO;
			}

			gBattlescriptCurrInstr += 5;
		}
		else
		{
			gMoveResultFlags |= MOVE_RESULT_MISSED;
			#ifdef NO_SHEER_COLD_NERF
				if (gBattleMons[bankAtk].level >= gBattleMons[bankDef].level && !IsDynamaxed(bankDef))
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			#else
				if (gBattleMons[bankAtk].level < gBattleMons[bankDef].level
				|| IsDynamaxed(bankDef)
				|| (gCurrentMove == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE)))
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			#endif

			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		}
	}
}

static void atk94_damagetohalftargethp(void) // super fang
{
    switch (gCurrentMove) {
		case MOVE_GUARDIAN_OF_ALOLA:
			gBattleMoveDamage = (GetBaseCurrentHP(gBattlerTarget) * 75) / 100;
			if (ProtectsAgainstZMoves(gCurrentMove, gBattlerAttacker, gBattlerTarget))
				gBattleMoveDamage = (gBattleMoveDamage * 25) / 100;
			break;

		default:
			gBattleMoveDamage = GetBaseCurrentHP(gBattlerTarget) / 2;
	}

	if (gBattleMoveDamage == 0)
		gBattleMoveDamage = 1;
	gBattlescriptCurrInstr++;
}

bool8 SetSandstormWeather(void)
{
	if (gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS))
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		return FALSE;
	}

	gBattleWeather = WEATHER_SANDSTORM_TEMPORARY;
	gBattleCommunication[MULTISTRING_CHOOSER] = 3;

	if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_SMOOTH_ROCK)
		gWishFutureKnock.weatherDuration = 8;
	else
		gWishFutureKnock.weatherDuration = 5;

	gBattleScripting.animArg1 = B_ANIM_SANDSTORM_CONTINUES;
	return TRUE;
}

static void atk95_setsandstorm(void)
{
    if (!SetSandstormWeather())
		gMoveResultFlags |= MOVE_RESULT_FAILED;

	gBattlescriptCurrInstr++;
}

static bool8 TakesGeneralWeatherDamage(u8 bank)
{
	ability_t ability = ABILITY(bank);
	u8 effect = ITEM_EFFECT(bank);

	return WEATHER_HAS_EFFECT
		&& 	ability != ABILITY_MAGICGUARD
		&&	ability != ABILITY_OVERCOAT
		&&	effect  != HOLD_EFFECT_SAFETY_GOGGLES
		&& !(gStatuses3[bank] & (STATUS3_UNDERGROUND | STATUS3_UNDERWATER)); //For some strange reason, Pokemon using Shadow/Phantom Force still take weather damage
}

bool8 SandstormHurts(u8 bank)
{
	ability_t ability = ABILITY(bank);

	if (TakesGeneralWeatherDamage(bank))
	{
		if (!IsOfType(bank, TYPE_ROCK) && !IsOfType(bank, TYPE_GROUND) && !IsOfType(bank, TYPE_STEEL)
		&& ability != ABILITY_SAND_VEIL && ability != ABILITY_SANDRUSH && ability != ABILITY_SANDFORCE)
			return TRUE;
	}

	return FALSE;
}

bool8 TakesDamageFromSandstorm(u8 bank)
{
	if (WEATHER_HAS_EFFECT
	&&  gBattleWeather & WEATHER_SANDSTORM_ANY)
	{
		return SandstormHurts(bank);
	}

	return FALSE;
}

bool8 HailHurts(u8 bank)
{
	ability_t ability = ABILITY(bank);

	if (TakesGeneralWeatherDamage(bank))
	{
		if (!IsOfType(bank, TYPE_ICE) && ability != ABILITY_ICEBODY && ability != ABILITY_SNOWCLOAK && ability != ABILITY_SLUSHRUSH)
			return TRUE;
	}

	return FALSE;
}

bool8 TakesDamageFromHail(u8 bank)
{
	if (WEATHER_HAS_EFFECT
	&&  gBattleWeather & WEATHER_HAIL_ANY)
	{
		return HailHurts(bank);
	}

	return FALSE;
}

u32 GetSandstormDamage(u8 bank)
{
	u32 damage = 0;

	if (TakesDamageFromSandstorm(bank))
		damage = MathMax(1, GetBaseMaxHP(bank) / 16);

	return damage;
}

u32 GetHailDamage(u8 bank)
{
	u32 damage = 0;

	if (TakesDamageFromHail(bank))
		damage = MathMax(1, GetBaseMaxHP(bank) / 16);

	return damage;
}

static void atk96_weatherdamage(void)
{
    u8 bank = gBattlerAttacker;
	gBattleMoveDamage = 0;

	if (gAbsentBattlerFlags & gBitTable[bank])
	{
		gBattleMoveDamage = 0;
	}
	#ifndef NO_GHOST_BATTLES //Ghosts can't take damage from Sand Stream or Snow Warning
	else if ((gBattleTypeFlags & (BATTLE_TYPE_WILD_SCRIPTED | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST
	&&  SIDE(bank) == B_SIDE_OPPONENT)
	{
		gBattleMoveDamage = 0;
	}
	#endif
	else
	{
		if (gBattleWeather & WEATHER_SANDSTORM_ANY)
		{
			gBattleMoveDamage = GetSandstormDamage(bank);
		}
		else if (gBattleWeather & WEATHER_HAIL_ANY)
		{
			gBattleMoveDamage = GetHailDamage(bank);
		}
	}

	gBattleStruct->turnDamageTaken[bank] = gBattleMoveDamage; //For Emergency Exit
	gBattlescriptCurrInstr++;
}

static void atk97_tryinfatuating(void)
{
    u8 bankAtk, bankDef;

	if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
	{
		bankAtk = gBattlerAttacker;
		bankDef = gBattlerTarget;
	}
	else
	{
		bankAtk = gBattlerTarget;
		bankDef = gBattlerAttacker;
	}

	if (ABILITY(bankDef) == ABILITY_OBLIVIOUS)
	{
		gBattlescriptCurrInstr = BattleScript_ObliviousPrevents;
	}
	else if (!CanBeInfatuated(bankDef, bankAtk))
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	}
	else
	{
		gBattleMons[bankDef].status2 |= STATUS2_INFATUATED_WITH(bankAtk);
		gBattlescriptCurrInstr += 6;
	}
}

static void atk98_updatestatusicon(void)
{
    if (!gBattleControllerExecFlags)
    {
        if (gBattlescriptCurrInstr[1] == BS_PLAYER2)
        {
            for (gActiveBattler = gBattleControllerExecFlags; gActiveBattler < gBattlersCount; ++gActiveBattler)
            {
                if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
                {
                    BtlController_EmitStatusIconUpdate(0, gBattleMons[gActiveBattler].status1, gBattleMons[gActiveBattler].status2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
            gBattlescriptCurrInstr += 2;
        }
        else if (gBattlescriptCurrInstr[1] == BS_ATTACKER_WITH_PARTNER)
        {
            gActiveBattler = gBattlerAttacker;
            if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
            {
                BtlController_EmitStatusIconUpdate(0, gBattleMons[gActiveBattler].status1, gBattleMons[gActiveBattler].status2);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
            {
                gActiveBattler = GetBattlerAtPosition(GetBattlerPosition(gBattlerAttacker) ^ BIT_FLANK);
                if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
                {
                    BtlController_EmitStatusIconUpdate(0, gBattleMons[gActiveBattler].status1, gBattleMons[gActiveBattler].status2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
            gBattlescriptCurrInstr += 2;
        }
        else
        {

            gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
            BtlController_EmitStatusIconUpdate(0, gBattleMons[gActiveBattler].status1, gBattleMons[gActiveBattler].status2);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattlescriptCurrInstr += 2;
        }
    }
}

void atk99_setmist(void)
{
	if (BankSideHasMist(gBattlerAttacker))
	{
		gMoveResultFlags |= MOVE_RESULT_FAILED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}
	else
	{
		gSideStatuses[SIDE(gBattlerAttacker)] |= SIDE_STATUS_MIST;
		gSideTimers[SIDE(gBattlerAttacker)].mistTimer = 5;
		gSideTimers[SIDE(gBattlerAttacker)].mistBattlerId = gBattlerAttacker;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}
	gBattlescriptCurrInstr++;
}

static void atk9A_setfocusenergy(void)
{
    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_FOCUS_ENERGY)
    {
        gMoveResultFlags |= MOVE_RESULT_FAILED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gBattleMons[gBattlerAttacker].status2 |= STATUS2_FOCUS_ENERGY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    ++gBattlescriptCurrInstr;
}

static void atk9B_transformdataexecution(void)
{
	gChosenMove = 0xFFFF;
	gBattlescriptCurrInstr++;
	if (ProtectedByMaxGuard(gBattlerTarget, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_FAILED;
		gBattleCommunication[6] = 1; //Protected
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
	}
	else if (gBattleMons[gBattlerTarget].status2 & STATUS2_TRANSFORMED
	|| gStatuses3[gBattlerTarget] & (STATUS3_SEMI_INVULNERABLE | STATUS3_ILLUSION)
	|| gSideStatuses[SIDE(gBattlerTarget)] & SIDE_STATUS_CRAFTY_SHIELD
	|| (IsDynamaxed(gBattlerAttacker) && IsBannedDynamaxSpecies(SPECIES(gBattlerTarget)))
	|| (IsRaidBattle() && gBattlerTarget == BANK_RAID_BOSS && gBattleStruct->dynamaxData.raidShieldsUp))
	{
		gMoveResultFlags |= MOVE_RESULT_FAILED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}
	else
	{
		TransformPokemon(gBattlerAttacker, gBattlerTarget);
		gActiveBattler = gBattlerAttacker;
		BtlController_EmitResetActionMoveSelection(0, RESET_MOVE_SELECTION);
		MarkBattlerForControllerExec(gActiveBattler);
	}
}

void TransformPokemon(u8 bankAtk, u8 bankDef)
{
	u32 i;
	u8 *battleMonAttacker, *battleMonTarget;
	u16 species = SPECIES_NONE;

	gBattleMons[bankAtk].status2 |= STATUS2_TRANSFORMED;
	gDisableStructs[bankAtk].disabledMove = 0;
	gDisableStructs[bankAtk].disableTimer = 0;
	gDisableStructs[bankAtk].transformedMonPersonality = gBattleMons[bankDef].personality;
	gDisableStructs[bankAtk].mimickedMoves = 0;

	if (IsGigantamaxed(bankDef))
		species = GetGigantamaxBaseForm(SPECIES(bankDef));

	if (species == SPECIES_NONE)
		species = SPECIES(bankDef);

	PREPARE_SPECIES_BUFFER(gBattleTextBuff1, species)

	battleMonAttacker = (u8*)(&gBattleMons[bankAtk]);
	battleMonTarget = (u8*)(&gBattleMons[bankDef]);

	for (i = 0; i < offsetof(struct BattlePokemon, pp); i++)
	{
		battleMonAttacker[i] = battleMonTarget[i];
	}

	if (IS_BLANK_TYPE(gBattleMons[bankDef].type1)
	&&  IS_BLANK_TYPE(gBattleMons[bankDef].type2))
	{
		gBattleMons[bankAtk].type1 = TYPE_NORMAL;
		gBattleMons[bankAtk].type2 = TYPE_NORMAL;
	}

	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (gBattleMoves[gBattleMons[bankAtk].moves[i]].pp < 5)
			gBattleMons[bankAtk].pp[i] = gBattleMoves[gBattleMons[bankAtk].moves[i]].pp;
		else
			gBattleMons[bankAtk].pp[i] = 5;
	}
	
	gBattleStruct->chiStrikeCritBoosts[bankAtk] = gBattleStruct->chiStrikeCritBoosts[bankDef];

	gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	gBattleSpritesDataPtr->battlerData[bankAtk].transformSpecies = species;
}

static void atk9C_setsubstitute(void)
{
    u32 hp = gBattleMons[gBattlerAttacker].maxHP / 4;

    if (gBattleMons[gBattlerAttacker].maxHP / 4 == 0)
        hp = 1;
    if (gBattleMons[gBattlerAttacker].hp <= hp)
    {
        gBattleMoveDamage = 0;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 4; // one bit value will only work for pokemon which max hp can go to 1020(which is more than possible in games)
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMons[gBattlerAttacker].status2 |= STATUS2_SUBSTITUTE;
        gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_WRAPPED);
        gDisableStructs[gBattlerAttacker].substituteHP = gBattleMoveDamage;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        gHitMarker |= HITMARKER_IGNORE_SUBSTITUTE;
    }
    ++gBattlescriptCurrInstr;
}

static bool8 IsMoveUncopyableByMimic(u16 move)
{
    s32 i;

    for (i = 0; sMovesForbiddenToCopy[i] != MIMIC_FORBIDDEN_END && sMovesForbiddenToCopy[i] != move; ++i);
    return (sMovesForbiddenToCopy[i] != MIMIC_FORBIDDEN_END);
}

static void atk9D_mimicattackcopy(void)
{
	gChosenMove = 0xFFFF;

	if (gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED
	|| gLastMoves[gBattlerTarget] == 0
	|| gLastMoves[gBattlerTarget] == 0xFFFF
	|| CheckTableForMove(gLastMoves[gBattlerTarget], gMimicBannedMoves)
	|| IsZMove(gLastMoves[gBattlerTarget])
	|| IsAnyMaxMove(gLastMoves[gBattlerTarget]))
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		int i;

		for (i = 0; i < MAX_MON_MOVES; i++)
		{
			if (gBattleMons[gBattlerAttacker].moves[i] == gLastMoves[gBattlerTarget])
				break;
		}

		if (i == MAX_MON_MOVES)
		{
			gBattleMons[gBattlerAttacker].moves[gCurrMovePos] = gLastMoves[gBattlerTarget];
			gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = gBattleMoves[gLastMoves[gBattlerTarget]].pp;

			PREPARE_MOVE_BUFFER(gBattleTextBuff1, gLastMoves[gBattlerTarget])

			gDisableStructs[gBattlerAttacker].mimickedMoves |= gBitTable[gCurrMovePos];
			gBattlescriptCurrInstr += 5;
		}
		else
		{
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		}
	}
}

static void atk9E_metronome(void)
{
    do
	{
		gCurrentMove = (Random() % LAST_MOVE_INDEX) + 1;
	} while (IsZMove(gCurrentMove) || IsAnyMaxMove(gCurrentMove)
		|| CheckTableForMove(gCurrentMove, gMetronomeBannedMoves));

	TryUpdateCalledMoveWithZMove();
	UpdateMoveStartValuesForCalledMove();
	gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
	gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
}

static void atk9F_dmgtolevel(void)
{
    gBattleMoveDamage = gBattleMons[gBattlerAttacker].level;
    ++gBattlescriptCurrInstr;
}

static void atkA0_psywavedamageeffect(void)
{
    gBattleMoveDamage = GetPsywaveDamage(Random() % 101);
	++gBattlescriptCurrInstr;
}

s32 GetPsywaveDamage(u8 randDamage)
{
	return MathMax(1, (gBattleMons[gBattlerAttacker].level * (randDamage + 50)) / 100);
}

static void atkA1_counterdamagecalculator(void)
{
    u8 atkSide = SIDE(gBattlerAttacker);
	u8 defSide = SIDE(gProtectStructs[gBattlerAttacker].physicalBattlerId);

	if (gProtectStructs[gBattlerAttacker].physicalDmg && atkSide != defSide && gBattleMons[gProtectStructs[gBattlerAttacker].physicalBattlerId].hp)
	{
		gBattleMoveDamage = gProtectStructs[gBattlerAttacker].physicalDmg * 2;

		if (IsMoveRedirectedByFollowMe(gCurrentMove, gBattlerAttacker, defSide))
			gBattlerTarget = gSideTimers[defSide].followmeTarget;
		else
			gBattlerTarget = gProtectStructs[gBattlerAttacker].physicalBattlerId;
		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

// a copy of atkA1 with the physical -> special field changes
static void atkA2_mirrorcoatdamagecalculator(void)
{
    u8 atkSide = SIDE(gBattlerAttacker);
	u8 defSide = SIDE(gProtectStructs[gBattlerAttacker].specialBattlerId);

	if (gProtectStructs[gBattlerAttacker].specialDmg && atkSide != defSide && gBattleMons[gProtectStructs[gBattlerAttacker].specialBattlerId].hp)
	{
		gBattleMoveDamage = gProtectStructs[gBattlerAttacker].specialDmg * 2;

		if (IsMoveRedirectedByFollowMe(gCurrentMove, gBattlerAttacker, defSide))
			gBattlerTarget = gSideTimers[defSide].followmeTarget;
		else
			gBattlerTarget = gProtectStructs[gBattlerAttacker].specialBattlerId;
		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

static void atkA3_disablelastusedattack(void)
{
    int i;
	u8 bankDef;

	if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
		bankDef = gBattlerTarget;
	else
		bankDef = gBattlerAttacker; //For Cursed Body

	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (gBattleMons[bankDef].moves[i] == gLastMoves[bankDef])
			break;
	}

	if (gDisableStructs[bankDef].disabledMove == 0
	&& i != MAX_MON_MOVES
	&& gBattleMons[bankDef].pp[i] != 0
	&& !IsAnyMaxMove(gLastMoves[bankDef])
	&& !IsZMove(gLastMoves[bankDef])
	&& !AbilityBattleEffects(ABILITYEFFECT_CHECK_BATTLER_SIDE, bankDef, ABILITY_AROMAVEIL, 0, 0))
	{
		PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleMons[bankDef].moves[i])

		gDisableStructs[bankDef].disabledMove = gBattleMons[bankDef].moves[i];
		gDisableStructs[bankDef].disableTimer = 4;
		gDisableStructs[bankDef].disableTimerStartValue = gDisableStructs[bankDef].disableTimer; // used to save the random amount of turns?
		gBattlescriptCurrInstr += 6;
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	}
}

static void atkA4_trysetencore(void)
{
    int i;

	if (gLastMoves[gBattlerTarget] == MOVE_STRUGGLE
	||  gLastMoves[gBattlerTarget] == MOVE_ENCORE
	||  gBattleStruct->playedShellTrapMessage & gBitTable[gBattlerTarget]
	||  CheckTableForMove(gLastMoves[gBattlerTarget], gMovesThatCallOtherMoves)
	||  IsZMove(gLastMoves[gBattlerTarget])
	||  IsAnyMaxMove(gLastMoves[gBattlerTarget])
	||  IsDynamaxed(gBattlerTarget))
	{
		i = MAX_MON_MOVES;
	}
	else
		i = FindMovePositionInMoveset(gLastMoves[gBattlerTarget], gBattlerTarget);

	if (gDisableStructs[gBattlerTarget].encoredMove == 0
	&& i < MAX_MON_MOVES
	&& gBattleMons[gBattlerTarget].pp[i] != 0)
	{
		gDisableStructs[gBattlerTarget].encoredMove = gBattleMons[gBattlerTarget].moves[i];
		gDisableStructs[gBattlerTarget].encoredMovePos = i;
		gDisableStructs[gBattlerTarget].encoreTimer = 3;
		gDisableStructs[gBattlerTarget].encoreTimerStartValue = gDisableStructs[gBattlerTarget].encoreTimer;
		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

static void atkA5_painsplitdmgcalc(void)
{
    if (!MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, gBattlerTarget))
	{
		s32 hpDiff = (GetBaseCurrentHP(gBattlerAttacker) + GetBaseCurrentHP(gBattlerTarget)) / 2;
		s32 painSplitHp = gBattleMoveDamage = GetBaseCurrentHP(gBattlerTarget) - hpDiff;
		u8* storeLoc = (void*)(&gBattleScripting.painSplitHp);

		storeLoc[0] = (painSplitHp);
		storeLoc[1] = (painSplitHp & 0x0000FF00) >> 8;
		storeLoc[2] = (painSplitHp & 0x00FF0000) >> 16;
		storeLoc[3] = (painSplitHp & 0xFF000000) >> 24;

		gBattleMoveDamage = GetBaseCurrentHP(gBattlerAttacker) - hpDiff;
		gSpecialStatuses[gBattlerTarget].dmg = 0xFFFF;

		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

static void atkA6_settypetorandomresistance(void) // conversion 2
{
    u8 moveType, defType;
	u32 i, rands;

	u8 bankAtk = gBattlerAttacker;
	u8 bankDef = gBattlerTarget;
	bool8 isInverseBattle = IsInverseBattle();

	if (gLastMoves[bankDef] == MOVE_NONE || gLastMoves[bankDef] == 0xFFFF)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		return;
	}

	moveType = gBattleStruct->LastUsedTypes[bankDef];

	if (moveType == TYPE_MYSTERY || moveType == TYPE_BLANK || moveType == TYPE_ROOSTLESS)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	for (rands = 0; rands < 1000; ++rands)
	{
		defType = Random() % NUMBER_OF_MON_TYPES;

		if (IsOfType(bankAtk, defType))
			continue;

		if (!isInverseBattle)
		{
			if (gTypeEffectiveness_2[moveType][defType] == TYPE_MUL_NO_EFFECT
			||  gTypeEffectiveness_2[moveType][defType] == TYPE_MUL_NOT_EFFECTIVE)
			{
				SET_BATTLER_TYPE(bankAtk, defType);
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, defType);
				gBattlescriptCurrInstr += 5;
				return;
			}
		}
		else //Inverse Battle
		{
			if (gTypeEffectiveness_2[moveType][defType] == TYPE_MUL_SUPER_EFFECTIVE)
			{
				SET_BATTLER_TYPE(bankAtk, defType);
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, defType);
				gBattlescriptCurrInstr += 5;
				return;
			}
		}
	}

	//Couldn't find a type randomly so check all types
	for (i = 0; i < NUMBER_OF_MON_TYPES; ++i)
	{
		for (defType = 0; defType < NUMBER_OF_MON_TYPES; ++defType)
		{
			if (IsOfType(bankAtk, defType))
				continue;

			if (!isInverseBattle)
			{
				if (gTypeEffectiveness_2[moveType][defType] == TYPE_MUL_NO_EFFECT
				||  gTypeEffectiveness_2[moveType][defType] == TYPE_MUL_NOT_EFFECTIVE)
				{
					SET_BATTLER_TYPE(bankAtk, defType);
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, defType);
					gBattlescriptCurrInstr += 5;
					return;
				}
			}
			else //Inverse Battle
			{
				if (gTypeEffectiveness_2[moveType][defType] == TYPE_MUL_SUPER_EFFECTIVE)
				{
					SET_BATTLER_TYPE(bankAtk, defType);
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, defType);
					gBattlescriptCurrInstr += 5;
					return;
				}
			}
		}
	}

	gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1); //Basically against Normal-type moves in an Inverse Battle
}

static void atkA7_setalwayshitflag(void)
{
    gStatuses3[gBattlerTarget] &= ~(STATUS3_ALWAYS_HITS);
    gStatuses3[gBattlerTarget] |= 0x10;
    gDisableStructs[gBattlerTarget].battlerWithSureHit = gBattlerAttacker;
    ++gBattlescriptCurrInstr;
}

static void atkA8_copymovepermanently(void) // sketch
{
    gChosenMove = 0xFFFF;

	if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED)
	&& !IsZMove(gLastPrintedMoves[gBattlerTarget])
	&& !IsAnyMaxMove(gLastPrintedMoves[gBattlerTarget])
	&& gLastPrintedMoves[gBattlerTarget] != MOVE_STRUGGLE
	&& gLastPrintedMoves[gBattlerTarget] != MOVE_CHATTER
	&& gLastPrintedMoves[gBattlerTarget] != MOVE_SKETCH
	&& gLastPrintedMoves[gBattlerTarget] != 0
	&& gLastPrintedMoves[gBattlerTarget] != 0xFFFF)
	{
		int i;

		for (i = 0; i < MAX_MON_MOVES; i++)
		{
			if (gBattleMons[gBattlerAttacker].moves[i] == MOVE_SKETCH)
				continue;
			if (gBattleMons[gBattlerAttacker].moves[i] == gLastPrintedMoves[gBattlerTarget])
				break;
		}

		if (i != MAX_MON_MOVES)
		{
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		}
		else // sketch worked
		{
			struct MovePpInfo movePpData;

			gBattleMons[gBattlerAttacker].moves[gCurrMovePos] = gLastPrintedMoves[gBattlerTarget];
			gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = gBattleMoves[gLastPrintedMoves[gBattlerTarget]].pp;
			gActiveBattler = gBattlerAttacker;

			for (i = 0; i < MAX_MON_MOVES; i++)
			{
				movePpData.moves[i] = gBattleMons[gBattlerAttacker].moves[i];
				movePpData.pp[i] = gBattleMons[gBattlerAttacker].pp[i];
			}
			movePpData.ppBonuses = gBattleMons[gBattlerAttacker].ppBonuses;

			BtlController_EmitSetMonData(0, REQUEST_MOVES_PP_BATTLE, 0, sizeof(struct MovePpInfo), &movePpData);
			MarkBattlerForControllerExec(gActiveBattler);

			PREPARE_MOVE_BUFFER(gBattleTextBuff1, gLastPrintedMoves[gBattlerTarget])

			gBattlescriptCurrInstr += 5;
		}
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

bool8 IsTwoTurnsMove(u16 move)
{
    if (gBattleMoves[move].effect == EFFECT_SKULL_BASH
     || gBattleMoves[move].effect == EFFECT_RAZOR_WIND
     || gBattleMoves[move].effect == EFFECT_SKY_ATTACK
     || gBattleMoves[move].effect == EFFECT_SOLARBEAM
     || gBattleMoves[move].effect == EFFECT_SEMI_INVULNERABLE
     || gBattleMoves[move].effect == EFFECT_BIDE)
        return TRUE;
    else
        return FALSE;
}

static bool8 IsInvalidForSleepTalkOrAssist(u16 move)
{
    if (move == MOVE_NONE || move == MOVE_SLEEP_TALK || move == MOVE_ASSIST
     || move == MOVE_MIRROR_MOVE || move == MOVE_METRONOME)
        return TRUE;
    else
        return FALSE;
}

u8 AttacksThisTurn(u8 bank, u16 move) // Note: returns 1 if it's a charging turn, otherwise 2
{
	u8 moveEffect = gBattleMoves[move].effect;

	// first argument is unused
	if (ITEM_EFFECT(bank) == HOLD_EFFECT_POWER_HERB)
		return 2;

	if (moveEffect == EFFECT_SOLARBEAM && (gBattleWeather & WEATHER_SUN_ANY) && WEATHER_HAS_EFFECT)
		return 2;

	if (moveEffect == EFFECT_SKULL_BASH
	||  moveEffect == EFFECT_RAZOR_WIND
	||  moveEffect == EFFECT_SKY_ATTACK
	||  moveEffect == EFFECT_SOLARBEAM
	||  moveEffect == EFFECT_SEMI_INVULNERABLE
	||  moveEffect == EFFECT_BIDE
	||	move == MOVE_GEOMANCY
	||  move == MOVE_SKYDROP)
	{
		if (gBattleMons[bank].status2 & STATUS2_MULTIPLETURNS)
			return 2;
		else
			return 1; //About to initiate attack.
	}

	return 2;
}

static void atkA9_trychoosesleeptalkmove(void)
{
    u8 unusable_moves = 0;
	int i;

	for (i = 0; i < 4; ++i) {
		if (CheckTableForMove(gBattleMons[gBattlerAttacker].moves[i], gSleepTalkBannedMoves))
			unusable_moves |= gBitTable[i];
	}

	unusable_moves = CheckMoveLimitations(gBattlerAttacker, unusable_moves, 0xFF ^ MOVE_LIMITATION_PP ^ MOVE_LIMITATION_DISABLED ^ MOVE_LIMITATION_CHOICE);
	if (unusable_moves == 0xF) //all 4 moves cannot be chosen
		gBattlescriptCurrInstr += 5;

	else { //at least one move can be chosen
		u32 random_pos;
		do {
			random_pos = Random() & 3;
		} while ((gBitTable[random_pos] & unusable_moves));

		gCalledMove = gBattleMons[gBattlerAttacker].moves[random_pos];
		gCurrMovePos = random_pos;
		gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
		gBattlerTarget = GetMoveTarget(gCalledMove, 0);
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		gHitMarker |= HITMARKER_NO_PPDEDUCT;
	}
}

static void atkAA_setdestinybond(void)
{
    if (gBattleStruct->DestinyBondCounters[gBattlerAttacker])
		gBattlescriptCurrInstr = BattleScript_ButItFailed + 2;

	else {
		gBattleStruct->DestinyBondCounters[gBattlerAttacker] = 2;
		gBattleMons[gBattlerAttacker].status2 |= STATUS2_DESTINY_BOND;
		gBattlescriptCurrInstr++;
	}
}

void TrySetDestinyBondToHappen(void)
{
    u8 sideAttacker = GetBattlerSide(gBattlerAttacker);
    u8 sideTarget = GetBattlerSide(gBattlerTarget);

    if (gBattleMons[gBattlerTarget].status2 & STATUS2_DESTINY_BOND
     && sideAttacker != sideTarget
     && !(gHitMarker & HITMARKER_GRUDGE))
        gHitMarker |= HITMARKER_DESTINYBOND;
}

static void atkAB_trysetdestinybondtohappen(void)
{
    TrySetDestinyBondToHappen();
    ++gBattlescriptCurrInstr;
}

static void atkAC_remaininghptopower(void)
{
    s32 i;
    s32 hpFraction = GetScaledHPFraction(gBattleMons[gBattlerAttacker].hp, gBattleMons[gBattlerAttacker].maxHP, 48);

    for (i = 0; i < (s32)sizeof(sFlailHpScaleToPowerTable); i += 2)
    {
        if (hpFraction <= sFlailHpScaleToPowerTable[i])
            break;
    }
    gDynamicBasePower = sFlailHpScaleToPowerTable[i + 1];
    ++gBattlescriptCurrInstr;
}

static void atkAD_tryspiteppreduce(void)
{
    if (TrySpitePPReduce(gBattlerTarget, 4))
		gBattlescriptCurrInstr += 5;
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

bool8 TrySpitePPReduce(u8 bank, u8 lostPP)
{
	u8 movePos = FindMovePositionInMoveset(gLastMoves[bank], bank);

	if (gLastMoves[bank] != MOVE_NONE && gLastMoves[bank] != 0xFFFF
	&& movePos < MAX_MON_MOVES && gBattleMons[bank].pp[movePos] > 0
	&& (!BATTLER_SEMI_INVULNERABLE(bank) || CanHitSemiInvulnerableTarget(gBattlerAttacker, bank, MOVE_SPITE)))
	{
		if (gBattleMons[bank].pp[movePos] < lostPP)
			lostPP = gBattleMons[bank].pp[movePos];

		PREPARE_MOVE_BUFFER(gBattleTextBuff1, gLastMoves[bank]);
		ConvertIntToDecimalStringN(gBattleTextBuff2, lostPP, STR_CONV_MODE_LEFT_ALIGN, 1);
		PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff2, 1, lostPP)

		gBattleMons[bank].pp[movePos] -= lostPP;
		gActiveBattler = bank;
		if (!(gDisableStructs[gActiveBattler].mimickedMoves & gBitTable[movePos]) && !IS_TRANSFORMED(gActiveBattler))
		{
			BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + movePos, 0, 1, &gBattleMons[gActiveBattler].pp[movePos]);
			MarkBattlerForControllerExec(gActiveBattler);
		}

		if (gBattleMons[bank].pp[movePos] == 0)
			CancelMultiTurnMoves(bank);

		return TRUE;
	}

	return FALSE;
}

static void atkAE_healpartystatus(void)
{
    u32 zero = 0;
	u8 toHeal = 0;

	if (CheckSoundMove(gCurrentMove))
	{
		int i;
		struct Pokemon *party;

		gBattleCommunication[MULTISTRING_CHOOSER] = 0;

		if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
			party = gPlayerParty;
		else
			party = gEnemyParty;

		if (ABILITY(gBattlerAttacker) != ABILITY_SOUNDPROOF)
		{
			gBattleMons[gBattlerAttacker].status1 = 0;
			gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_NIGHTMARE);
		}
		else
		{
			RecordAbilityBattle(gBattlerAttacker, ABILITY(gBattlerAttacker));
			gBattleCommunication[MULTISTRING_CHOOSER] |= 1;
		}

		gActiveBattler = gBattleScripting.battler = GetBattlerAtPosition(GetBattlerPosition(gBattlerAttacker) ^ BIT_FLANK);

		if (IS_DOUBLE_BATTLE
		&& !(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
		{
			if (ABILITY(gActiveBattler) != ABILITY_SOUNDPROOF)
			{
				gBattleMons[gActiveBattler].status1 = 0;
				gBattleMons[gActiveBattler].status2 &= ~(STATUS2_NIGHTMARE);
			}
			else
			{
				RecordAbilityBattle(gActiveBattler, ABILITY(gActiveBattler));
				gBattleCommunication[MULTISTRING_CHOOSER] |= 2;
			}
		}

		for (i = 0; i < PARTY_SIZE; ++i)
		{
			u16 species = GetMonData(&party[i], MON_DATA_SPECIES2, NULL);

			if (species != SPECIES_NONE && species != SPECIES_EGG)
			{
				ability_t ability;

				if (i == gBattlerPartyIndexes[gBattlerAttacker])
					ability = ABILITY(gBattlerAttacker);
				else if (IS_DOUBLE_BATTLE
				&& i == gBattlerPartyIndexes[gActiveBattler]
				&& !(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
					ability = ABILITY(gActiveBattler);
				else
					ability = GetMonAbility(&party[i]);

				if (ability != ABILITY_SOUNDPROOF)
					toHeal |= (1 << i);
			}
		}
	}
	else // Aromatherapy
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 4;
		toHeal = 0x3F;

		gBattleMons[gBattlerAttacker].status1 = 0;
		gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_NIGHTMARE);

		gActiveBattler = GetBattlerAtPosition(GetBattlerPosition(gBattlerAttacker) ^ BIT_FLANK);
		if (IS_DOUBLE_BATTLE
		&& !(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
		{
			gBattleMons[gActiveBattler].status1 = 0;
			gBattleMons[gActiveBattler].status2 &= ~(STATUS2_NIGHTMARE);
		}
	}

	if (toHeal)
	{
		gActiveBattler = gBattlerAttacker;
		BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, toHeal, 4, &zero);
		MarkBattlerForControllerExec(gActiveBattler);
	}

	gBattlescriptCurrInstr++;
}

static void atkAF_cursetarget(void)
{
    if (gBattleMons[gBattlerTarget].status2 & STATUS2_CURSED
	|| (BATTLER_SEMI_INVULNERABLE(gBattlerTarget) && !CanHitSemiInvulnerableTarget(gBattlerAttacker, gBattlerTarget, gCurrentMove)))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
	{
		gBattleMons[gBattlerTarget].status2 |= STATUS2_CURSED;
		gBattleMoveDamage = MathMax(1, gBattleMons[gBattlerAttacker].maxHP / 2);
		gBattlescriptCurrInstr += 5;
	}
}

static void atkB0_trysetspikes(void)
{
    u8 atkSide = SIDE(gBattlerAttacker);
	u8 defSide = atkSide ^ BIT_SIDE;
	u8 stringcase = 0xFF;

	switch (gCurrentMove) {
		case MOVE_STEALTHROCK:
		case MOVE_G_MAX_STONESURGE_P:
		case MOVE_G_MAX_STONESURGE_S:
			if (gSideTimers[defSide].srAmount)
			{
				gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else
			{
				gSideStatuses[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].srAmount = 1;
				gBattlescriptCurrInstr += 5;
				stringcase = 1;
			}
			break;

		case MOVE_TOXICSPIKES:
			if (gSideTimers[defSide].tspikesAmount >= 2)
			{
				gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else
			{
				gSideStatuses[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].tspikesAmount++;
				gBattlescriptCurrInstr += 5;
				stringcase = 2;
			}
			break;

		case MOVE_STICKYWEB:
			if (gSideTimers[defSide].stickyWeb)
			{
				gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else
			{
				gSideStatuses[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].stickyWeb = 1;
				gBattlescriptCurrInstr += 5;
				stringcase = 3;
			}
			break;

		case MOVE_G_MAX_STEELSURGE_P:
		case MOVE_G_MAX_STEELSURGE_S:
			if (gSideTimers[defSide].steelsurge)
			{
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else
			{
				gSideStatuses[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].steelsurge = 1;
				gBattlescriptCurrInstr += 5;
				stringcase = 4;
			}
			break;

		default:
			if (gSideTimers[defSide].spikesAmount >= 3)
			{
				gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else {
				gSideStatuses[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].spikesAmount++;
				gBattlescriptCurrInstr += 5;
				stringcase = 0;
			}
	}

	if (stringcase != 0xFF)
		gBattleStringLoader = (u8*) sEntryHazardsStrings[stringcase];
}

static void atkB1_setforesight(void)
{
    gBattleMons[gBattlerTarget].status2 |= STATUS2_FORESIGHT;
    ++gBattlescriptCurrInstr;
}

static void atkB2_trysetperishsong(void)
{
    s32 i;
	s32 notAffectedCount = 0;

	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gStatuses3[i] & STATUS3_PERISH_SONG || ABILITY(i) == ABILITY_SOUNDPROOF)
		{
			notAffectedCount++;
		}
		else
		{
			gStatuses3[i] |= STATUS3_PERISH_SONG;
			gDisableStructs[i].perishSongTimer = 3;
			gDisableStructs[i].perishSongTimerStartValue = 3;
		}
	}

	PressurePPLoseOnUsingPerishSong(gBattlerAttacker);

	if (notAffectedCount == gBattlersCount)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
		gBattlescriptCurrInstr += 5;
}

static void atkB3_rolloutdamagecalculation(void)
{
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
	{
		CancelMultiTurnMoves(gBattlerAttacker);
		gBattlescriptCurrInstr = BattleScript_MoveMissedPause;
	}
	else
	{
		if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)) // first hit
		{
			gDisableStructs[gBattlerAttacker].rolloutTimer = 5;
			gDisableStructs[gBattlerAttacker].rolloutTimerStartValue = 5;
			gBattleMons[gBattlerAttacker].status2 |= STATUS2_MULTIPLETURNS;
			gLockedMoves[gBattlerAttacker] = gCurrentMove;
		}
		if (--gDisableStructs[gBattlerAttacker].rolloutTimer == 0) // last hit
		{
			gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);
		}

		gBattlescriptCurrInstr++;
	}
}

static void atkB4_jumpifconfusedandstatmaxed(void)
{
    if (IsConfused(gBattlerTarget) && !STAT_CAN_RISE(gBattlerTarget, gBattlescriptCurrInstr[1]))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	else
		gBattlescriptCurrInstr += 6;
}

static void atkB5_furycuttercalc(void)
{
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT
	|| gLastPrintedMoves[gBattlerAttacker] != gChosenMove)
	{
		gDisableStructs[gBattlerAttacker].furyCutterCounter = 0;
	}
	else
	{
		if (gDisableStructs[gBattlerAttacker].furyCutterCounter < 4)
			gDisableStructs[gBattlerAttacker].furyCutterCounter++;
	}

	gBattlescriptCurrInstr++;
}

static void atkB6_happinesstodamagecalculation(void)
{
    if (gBattleMoves[gCurrentMove].effect == EFFECT_RETURN)
        gDynamicBasePower = 10 * (gBattleMons[gBattlerAttacker].friendship) / 25;
    else // EFFECT_FRUSTRATION
        gDynamicBasePower = 10 * (255 - gBattleMons[gBattlerAttacker].friendship) / 25;
    ++gBattlescriptCurrInstr;
}

static void atkB7_presentdamagecalculation(void)
{
    int rand = Random() & 0xFF;

	if (rand < 102)
		gDynamicBasePower = 40;
	else if (rand < 178)
		gDynamicBasePower = 80;
	else if (rand < 204)
		gDynamicBasePower = 120;
	else
	{
		gBattleMoveDamage = MathMax(GetBaseMaxHP(gBattlerTarget) / 4, 1);
		gBattleMoveDamage *= -1;
	}

	if (rand < 204)
		gBattlescriptCurrInstr = BattleScript_HitFromCritCalc;
	else if (IsHealBlocked(gBattlerTarget))
		gBattlescriptCurrInstr = BattleScript_NoHealTargetAfterHealBlock;
	else if (BATTLER_MAX_HP(gBattlerTarget))
		gBattlescriptCurrInstr = BattleScript_AlreadyAtFullHp;
	else
	{
		gBattleStruct->ParentalBondOn = FALSE;
		gMultiHitCounter = 0;
		gMoveResultFlags &= ~(MOVE_RESULT_DOESNT_AFFECT_FOE);
		gBattlescriptCurrInstr = BattleScript_PresentHealTarget;
	}
}

static void atkB8_setsafeguard(void)
{
    if (BankSideHasSafeguard(gBattlerAttacker))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}
	else
	{
		gSideStatuses[SIDE(gBattlerAttacker)] |= SIDE_STATUS_SAFEGUARD;
		gSideTimers[SIDE(gBattlerAttacker)].safeguardTimer = 5;
		gSideTimers[SIDE(gBattlerAttacker)].safeguardBattlerId = gBattlerAttacker;
		gBattleCommunication[MULTISTRING_CHOOSER] = 5;
	}

	gBattlescriptCurrInstr++;
}

static void atkB9_magnitudedamagecalculation(void)
{
    int magnitude = (Random() % 100);

    if (gDynamicBasePower) //Power has already been calculated
	{
		gBattlescriptCurrInstr = BattleScript_HitFromCritCalc;
		return;
	}

	if (magnitude < 5)
	{
		gDynamicBasePower = 10;
		magnitude = 4;
	}
	else if (magnitude < 15)
	{
		gDynamicBasePower = 30;
		magnitude = 5;
	}
	else if (magnitude < 35)
	{
		gDynamicBasePower = 50;
		magnitude = 6;
	}
	else if (magnitude < 65)
	{
		gDynamicBasePower = 70;
		magnitude = 7;
	}
	else if (magnitude < 85)
	{
		gDynamicBasePower = 90;
		magnitude = 8;
	}
	else if (magnitude < 95)
	{
		gDynamicBasePower = 110;
		magnitude = 9;
	}
	else
	{
		gDynamicBasePower = 150;
		magnitude = 10;
	}

	PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff1, 2, magnitude)
	gBattlescriptCurrInstr++;
}

static void atkBA_jumpifnopursuitswitchdmg(void)
{
    int i;
    u8 toDoAction = 0;
	switch (gMultiHitCounter) {
		case 1:
			if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
				gBattlerTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
			else
				gBattlerTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
			break;

		default:
			if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
				gBattlerTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
			else
				gBattlerTarget = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
	}

	
	for (i = 0; i < gBattlersCount; i++)
	{
		if (gBattlerByTurnOrder[i] == gBattlerTarget)
		{
			toDoAction = gActionsByTurnOrder[i];
			break;
		}
	}

	if (gChosenActionByBattler[gBattlerTarget] == B_ACTION_USE_MOVE
	&& toDoAction == B_ACTION_USE_MOVE
	&& gChosenMoveByBattler[gBattlerTarget] == MOVE_PURSUIT
	//&& gBattlerAttacker == gBattleStruct->moveTarget[gBattlerTarget] //Used pre Gen 4
	&& !(gBattleMons[gBattlerTarget].status1 & (STATUS1_SLEEP | STATUS1_FREEZE))
	&& BATTLER_ALIVE(gBattlerAttacker)
	&& !gDisableStructs[gBattlerTarget].truantCounter)
	{
		for (i = 0; i < gBattlersCount; i++)
		{
			if (gBattlerByTurnOrder[i] == gBattlerTarget)
				gActionsByTurnOrder[i] = B_ACTION_TRY_FINISH;
		}

		gCurrentMove = MOVE_PURSUIT;
		gBattlescriptCurrInstr += 5;
		gBattleScripting.animTurn = 1;
		gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED | HITMARKER_NO_ATTACKSTRING);
	}
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

bool8 SetSunnyWeather(void)
{
	if (gBattleWeather & (WEATHER_SUN_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS))
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		return FALSE;
	}

	gBattleWeather = WEATHER_SUN_TEMPORARY;
	gBattleCommunication[MULTISTRING_CHOOSER] = 4;

	if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_HEAT_ROCK)
		gWishFutureKnock.weatherDuration = 8;
	else
		gWishFutureKnock.weatherDuration = 5;

	gBattleScripting.animArg1 = B_ANIM_SUN_CONTINUES;
	return TRUE;
}

static void atkBB_setsunny(void)
{
	if (!SetSunnyWeather())
		gMoveResultFlags |= MOVE_RESULT_FAILED;

	gBattlescriptCurrInstr++;
}

static void atkBC_maxattackhalvehp(void) // belly drum
{
    u32 halfHp = gBattleMons[gBattlerAttacker].maxHP / 2;

	if (gBattleMons[gBattlerAttacker].maxHP / 2 == 0)
	{
		halfHp = 1;
	}

	gBattleScripting.statChanger = INCREASE_2 | STAT_STAGE_ATK;
	gBattleScripting.animArg1 = 0xE + STAT_STAGE_ATK;
	gBattleScripting.animArg2 = 0;

	if (ABILITY(gBattlerAttacker) == ABILITY_CONTRARY)
	{
		if (STAT_STAGE(gBattlerAttacker, STAT_STAGE_ATK) > STAT_STAGE_MIN
		&& gBattleMons[gBattlerAttacker].hp > halfHp)
		{
			gBattleMons[gBattlerAttacker].statStages[STAT_STAGE_ATK - 1] = STAT_STAGE_MIN;
			gBattleMoveDamage = MathMax(1, gBattleMons[gBattlerAttacker].maxHP / 2);
			gBattlescriptCurrInstr += 5;
		}
		else
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else if (STAT_STAGE(gBattlerAttacker, STAT_STAGE_ATK) < STAT_STAGE_MAX
	&& gBattleMons[gBattlerAttacker].hp > halfHp)
	{
		gBattleMons[gBattlerAttacker].statStages[STAT_STAGE_ATK - 1] = STAT_STAGE_MAX;
		gBattleMoveDamage = MathMax(1, gBattleMons[gBattlerAttacker].maxHP / 2);
		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

static void atkBD_copyfoestats(void) // psych up
{
    s32 i;

	if (gSideStatuses[SIDE(gBattlerTarget)] & SIDE_STATUS_CRAFTY_SHIELD
	|| ProtectedByMaxGuard(gBattlerTarget, gCurrentMove))
	{
		gBattleCommunication[6] = 1; //Protected
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		for (i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
			STAT_STAGE(gBattlerAttacker, i) = STAT_STAGE(gBattlerTarget, i);

		//Copy critical hit ratio
		gBattleMons[gBattlerAttacker].status2 &= ~STATUS2_FOCUS_ENERGY; //Remove old boost if had
		gBattleMons[gBattlerAttacker].status2 |= (gBattleMons[gBattlerTarget].status2 & STATUS2_FOCUS_ENERGY); //Give boost if opponent has
		gBattleStruct->chiStrikeCritBoosts[gBattlerAttacker] = gBattleStruct->chiStrikeCritBoosts[gBattlerTarget];
		gBattlescriptCurrInstr += 5;
	}
}

static void atkBE_rapidspinfree(void)
{
    u8 bankAtk = gBattlerAttacker;
	u8 sideAtk = SIDE(bankAtk);
	u8 sideDef = SIDE(gBattlerTarget);

	if (gCurrentMove == MOVE_RAPIDSPIN)
	{
		if (gBattleMons[bankAtk].status2 & STATUS2_WRAPPED)
		{
			gBattleMons[bankAtk].status2 &= ~(STATUS2_WRAPPED);
			gBattleStruct->sandblastCentiferno[bankAtk] = 0;
			gBattlerTarget = gBattleStruct->wrappedBy[bankAtk];

			PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleStruct->wrappedMove[bankAtk]);
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_WrapFree;
		}
		else if (gStatuses3[bankAtk] & STATUS3_LEECHSEED)
		{
			gStatuses3[bankAtk] &= ~(STATUS3_LEECHSEED);
			gStatuses3[bankAtk] &= ~(STATUS3_LEECHSEED_BATTLER);
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_LeechSeedFree;
		}
		else if (gSideStatuses[sideAtk] & SIDE_STATUS_SPIKES)
		{
			gSideStatuses[sideAtk] &= ~(SIDE_STATUS_SPIKES);
			gSideTimers[sideAtk].spikesAmount = 0;
			gSideTimers[sideAtk].tspikesAmount = 0;
			gSideTimers[sideAtk].srAmount = 0;
			gSideTimers[sideAtk].stickyWeb = 0;
			gSideTimers[sideAtk].steelsurge = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_SpikesFree;
		}
		else
		{
			if (gCurrentMove == MOVE_RAPID_SPIN && ABILITY(bankAtk) != ABILITY_SHEERFORCE) //From Gen 8
			{
				gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_SPD_PLUS_1 | MOVE_EFFECT_AFFECTS_USER;
				SetMoveEffect(TRUE, TRUE); //Automatically increments gBattlescriptCurrInstr
			}
			else
				gBattlescriptCurrInstr++;
		}
	}
	else //Defog + G-Max Windrage
	{
		if (gSideStatuses[sideAtk] & SIDE_STATUS_SPIKES)
		{
			gSideStatuses[sideAtk] &= ~(SIDE_STATUS_SPIKES);
			gSideTimers[sideAtk].spikesAmount = 0;
			gSideTimers[sideAtk].tspikesAmount = 0;
			gSideTimers[sideAtk].srAmount = 0;
			gSideTimers[sideAtk].stickyWeb = 0;
			gSideTimers[sideAtk].steelsurge = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			gBattleStringLoader = RemovedEntryHazardsString;
		}
		else if (gSideStatuses[sideDef] & SIDE_STATUS_SPIKES)
		{
			gSideStatuses[sideDef] &= ~(SIDE_STATUS_SPIKES);
			gSideTimers[sideDef].spikesAmount = 0;
			gSideTimers[sideDef].tspikesAmount = 0;
			gSideTimers[sideDef].srAmount = 0;
			gSideTimers[sideDef].stickyWeb = 0;
			gSideTimers[sideDef].steelsurge = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			gBattleStringLoader = RemovedEntryHazardsTargetSideString;
		}
		else if (gSideStatuses[sideDef] & (SIDE_STATUS_REFLECT))
		{
			gSideTimers[sideDef].reflectTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_REFLECT, SIDE_STATUS_REFLECT, sideDef);
		}
		else if (gSideStatuses[sideDef] & (SIDE_STATUS_LIGHTSCREEN))
		{
			gSideTimers[sideDef].lightscreenTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_LIGHT_SCREEN, SIDE_STATUS_LIGHTSCREEN, sideDef);
		}
		else if (gSideStatuses[sideDef] & SIDE_STATUS_SAFEGUARD)
		{
			gSideTimers[sideDef].safeguardTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_SAFEGUARD, SIDE_STATUS_SAFEGUARD, sideDef);
		}
		else if (gSideStatuses[sideDef] & SIDE_STATUS_MIST)
		{
			gSideTimers[sideDef].mistTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_MIST, SIDE_STATUS_MIST, sideDef);
		}
		else if (gBattleStruct->AuroraVeilTimers[sideDef])
		{
			gBattleStruct->AuroraVeilTimers[sideDef] = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_AURORAVEIL, 0, sideDef);
		}
		else if (gTerrainType != 0 //Since Gen 8
		&& !(gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN)) //Terrain is permanent
		{
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_SetTerrain; //Removes the Terrain
		}
		else
		{
			gBattleStruct->fogBlownAwayByDefog = TRUE; //No reactivating fog
			gBattlescriptCurrInstr++;
		}
	}
}

static void atkBF_setdefensecurlbit(void)
{
    gBattleMons[gBattlerAttacker].status2 |= STATUS2_DEFENSE_CURL;
    ++gBattlescriptCurrInstr;
}

static void atkC0_recoverbasedonsunlight(void)
{
    gBattlerTarget = gBattlerAttacker;

	if (!BATTLER_MAX_HP(gBattlerAttacker))
	{
		if (gBattleWeather == 0 || !WEATHER_HAS_EFFECT)
		{
			NO_WEATHER_EFFECT:
			gBattleMoveDamage = GetBaseMaxHP(gBattlerAttacker) / 2;
		}
		else if (gCurrentMove == MOVE_SHOREUP)
		{
			if (gBattleWeather & WEATHER_SANDSTORM_ANY)
				gBattleMoveDamage = (2 * GetBaseMaxHP(gBattlerAttacker)) / 3;
		}
		else if (gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(gBattlerAttacker) != HOLD_EFFECT_UTILITY_UMBRELLA)
		{
			gBattleMoveDamage = (2 * GetBaseMaxHP(gBattlerAttacker)) / 3;
		}
		else //Not Sunny Weather
		{
			if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_UTILITY_UMBRELLA)
				goto NO_WEATHER_EFFECT;

			gBattleMoveDamage = GetBaseMaxHP(gBattlerAttacker) / 4;
		}

		gBattleMoveDamage = MathMax(1, gBattleMoveDamage);
		gBattleMoveDamage *= -1;
		gBattlescriptCurrInstr += 5;
	}
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkC1_hiddenpowercalc(void)
{
    s32 powerBits, typeBits;

    powerBits = ((gBattleMons[gBattlerAttacker].hpIV & 2) >> 1)
              | ((gBattleMons[gBattlerAttacker].attackIV & 2) << 0)
              | ((gBattleMons[gBattlerAttacker].defenseIV & 2) << 1)
              | ((gBattleMons[gBattlerAttacker].speedIV & 2) << 2)
              | ((gBattleMons[gBattlerAttacker].spAttackIV & 2) << 3)
              | ((gBattleMons[gBattlerAttacker].spDefenseIV & 2) << 4);
    typeBits  = ((gBattleMons[gBattlerAttacker].hpIV & 1) << 0)
              | ((gBattleMons[gBattlerAttacker].attackIV & 1) << 1)
              | ((gBattleMons[gBattlerAttacker].defenseIV & 1) << 2)
              | ((gBattleMons[gBattlerAttacker].speedIV & 1) << 3)
              | ((gBattleMons[gBattlerAttacker].spAttackIV & 1) << 4)
              | ((gBattleMons[gBattlerAttacker].spDefenseIV & 1) << 5);
    gDynamicBasePower = (40 * powerBits) / 63 + 30;
    gBattleStruct->dynamicMoveType = (15 * typeBits) / 63 + 1;
    if (gBattleStruct->dynamicMoveType >= TYPE_MYSTERY)
        ++gBattleStruct->dynamicMoveType;
    gBattleStruct->dynamicMoveType |= 0xC0;
    ++gBattlescriptCurrInstr;
}

static void atkC2_selectfirstvalidtarget(void)
{
    for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount; ++gBattlerTarget) {
		if (gBattlerTarget == gBattlerAttacker) continue;

		if (!(gAbsentBattlerFlags & gBitTable[gBattlerTarget])) break;
	}

	//Stops certain animations from acting like they only target the partner
	if (IS_DOUBLE_BATTLE
	&&  gBattlerAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
	&&  gBattlerTarget == GetBattlerAtPosition(B_POSITION_PLAYER_LEFT))
	{
		if (IsBattlerAlive(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)))
			gBattlerTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		else if (IsBattlerAlive(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)))
			gBattlerTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
	}

	gBattlescriptCurrInstr++;
}

static void atkC3_trysetfutureattack(void)
{
    if (gWishFutureKnock.futureSightCounter[gBattlerTarget] != 0)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	else
	{
		gWishFutureKnock.futureSightMove[gBattlerTarget] = gCurrentMove;
		gWishFutureKnock.futureSightAttacker[gBattlerTarget] = gBattlerAttacker;
		gWishFutureKnock.futureSightPartyIndex[gBattlerTarget] = gBattlerPartyIndexes[gBattlerAttacker];
		gWishFutureKnock.futureSightCounter[gBattlerTarget] = 3;

		if (gCurrentMove == MOVE_DOOMDESIRE)
			gBattleCommunication[MULTISTRING_CHOOSER] = 1;
		else
			gBattleCommunication[MULTISTRING_CHOOSER] = 0;

		gBattlescriptCurrInstr += 5;
	}
}

static void atkC4_trydobeatup(void)
{
    struct Pokemon *party;

    if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
        party = gPlayerParty;
    else
        party = gEnemyParty;
    if (gBattleMons[gBattlerTarget].hp == 0)
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        u8 beforeLoop = gBattleCommunication[0];

        for (;gBattleCommunication[0] < 6; ++gBattleCommunication[0])
        {
            if (GetMonData(&party[gBattleCommunication[0]], MON_DATA_HP)
             && GetMonData(&party[gBattleCommunication[0]], MON_DATA_SPECIES2)
             && GetMonData(&party[gBattleCommunication[0]], MON_DATA_SPECIES2) != SPECIES_EGG
             && !GetMonData(&party[gBattleCommunication[0]], MON_DATA_STATUS))
                break;
        }
        if (gBattleCommunication[0] < 6)
        {
            PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gBattlerAttacker, gBattleCommunication[0])
            gBattlescriptCurrInstr += 9;
            gBattleMoveDamage = gBaseStats[GetMonData(&party[gBattleCommunication[0]], MON_DATA_SPECIES)].baseAttack;
            gBattleMoveDamage *= gBattleMoves[gCurrentMove].power;
            gBattleMoveDamage *= (GetMonData(&party[gBattleCommunication[0]], MON_DATA_LEVEL) * 2 / 5 + 2);
            gBattleMoveDamage /= gBaseStats[gBattleMons[gBattlerTarget].species].baseDefense;
            gBattleMoveDamage = (gBattleMoveDamage / 50) + 2;
            if (gProtectStructs[gBattlerAttacker].helpingHand)
                gBattleMoveDamage = gBattleMoveDamage * 15 / 10;
            ++gBattleCommunication[0];
        }
        else if (beforeLoop != 0)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        else
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 5);
    }
}

static void atkC5_setsemiinvulnerablebit(void)
{
    switch (gCurrentMove) {
		case MOVE_FLY:
		case MOVE_BOUNCE:
			gStatuses3[gBattlerAttacker] |= STATUS3_ON_AIR;
			break;

		case MOVE_DIG:
			gStatuses3[gBattlerAttacker] |= STATUS3_UNDERGROUND;
			break;

		case MOVE_DIVE:
			gStatuses3[gBattlerAttacker] |= STATUS3_UNDERWATER;
			break;

		case MOVE_SHADOWFORCE:
		case MOVE_PHANTOMFORCE:
			gStatuses3[gBattlerAttacker] |= STATUS3_DISAPPEARED;
			break;

		case MOVE_SKYDROP:
			gStatuses3[gBattlerAttacker] |= (STATUS3_SKY_DROP_ATTACKER | STATUS3_ON_AIR);
			gStatuses3[gBattlerTarget] |= (STATUS3_SKY_DROP_TARGET | STATUS3_ON_AIR);

			gBattleStruct->skyDropAttackersTarget[gBattlerAttacker] = gBattlerTarget;
			gBattleStruct->skyDropTargetsAttacker[gBattlerTarget] = gBattlerAttacker;

			if (gSideTimers[SIDE(gBattlerTarget)].followmeTarget == gBattlerTarget) //Removes Follow Me's effect
			{
				gSideTimers[SIDE(gBattlerTarget)].followmeTimer = 0;
				gBattleStruct->ragePowdered &= ~gBitTable[SIDE(gBattlerTarget)];
			}
	}
	gBattlescriptCurrInstr++;
}

static void atkC6_clearsemiinvulnerablebit(void)
{
    switch (gCurrentMove) {
		case MOVE_FLY:
		case MOVE_BOUNCE:
			gStatuses3[gBattlerAttacker] &= ~STATUS3_ON_AIR;
			break;
		case MOVE_DIG:
			gStatuses3[gBattlerAttacker] &= ~STATUS3_UNDERGROUND;
			break;
		case MOVE_DIVE:
			gStatuses3[gBattlerAttacker] &= ~STATUS3_UNDERWATER;
			break;
		case MOVE_SHADOWFORCE:
		case MOVE_PHANTOMFORCE:
			gStatuses3[gBattlerAttacker] &= ~STATUS3_DISAPPEARED;
			break;
		case MOVE_SKYDROP:
			gStatuses3[gBattlerAttacker] &= ~(STATUS3_SKY_DROP_ATTACKER | STATUS3_ON_AIR);
			gStatuses3[gBattlerTarget] &= ~(STATUS3_SKY_DROP_TARGET | STATUS3_ON_AIR);
			gBattleStruct->skyDropAttackersTarget[gBattlerAttacker] = 0;
			gBattleStruct->skyDropTargetsAttacker[gBattlerTarget] = 0;
			break;
	}
	gBattlescriptCurrInstr++;
}

static void atkC7_setminimize(void)
{
    if (gHitMarker & HITMARKER_OBEYS)
        gStatuses3[gBattlerAttacker] |= STATUS3_MINIMIZED;
    ++gBattlescriptCurrInstr;
}

bool8 SetHailWeather(void)
{
	if (gBattleWeather & (WEATHER_HAIL_ANY | WEATHER_PRIMAL_ANY | WEATHER_CIRCUS))
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		return FALSE;
	}

	gBattleWeather = WEATHER_HAIL_TEMPORARY;
	gBattleCommunication[MULTISTRING_CHOOSER] = 5;

	if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_ICY_ROCK)
		gWishFutureKnock.weatherDuration = 8;
	else
		gWishFutureKnock.weatherDuration = 5;

	gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
	return TRUE;
}

static void atkC8_sethail(void)
{
	if (!SetHailWeather())
		gMoveResultFlags |= MOVE_RESULT_FAILED;

	gBattlescriptCurrInstr++;
}

static void atkC9_jumpifattackandspecialattackcannotfall(void) // memento
{
    if (!STAT_CAN_FALL(gBattlerTarget, STAT_ATK)
	&& !STAT_CAN_FALL(gBattlerTarget, STAT_SPATK)
	&& gBattleCommunication[6] != 1)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		gActiveBattler = gBattlerAttacker;
		gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
		BtlController_EmitHealthBarUpdate(0, INSTANT_HP_BAR_DROP);
		MarkBattlerForControllerExec(gActiveBattler);
		gBattlescriptCurrInstr += 5;
	}
}

static void atkCA_setforcedtarget(void) // follow me
{
    switch (gCurrentMove) {
		case MOVE_SPOTLIGHT:
			gSideTimers[SIDE(gBattlerTarget)].followmeTimer = 1;
			gSideTimers[SIDE(gBattlerTarget)].followmeTarget = gBattlerTarget;
			gBattleStruct->ragePowdered &= ~gBitTable[SIDE(gBattlerTarget)];
			break;
		case MOVE_RAGEPOWDER:
			gSideTimers[SIDE(gBattlerAttacker)].followmeTimer = 1;
			gSideTimers[SIDE(gBattlerAttacker)].followmeTarget = gBattlerAttacker;
			gBattleStruct->ragePowdered |= gBitTable[SIDE(gBattlerAttacker)];
			break;
		default:
			gSideTimers[SIDE(gBattlerAttacker)].followmeTimer = 1;
			gSideTimers[SIDE(gBattlerAttacker)].followmeTarget = gBattlerAttacker;
			gBattleStruct->ragePowdered &= ~gBitTable[SIDE(gBattlerAttacker)];
	}

	gBattlescriptCurrInstr++;
}

static void atkCB_setcharge(void)
{
    gStatuses3[gBattlerAttacker] |= STATUS3_CHARGED_UP;
    gDisableStructs[gBattlerAttacker].chargeTimer = 2;
    gDisableStructs[gBattlerAttacker].chargeTimerStartValue = 2;
    ++gBattlescriptCurrInstr;
}

static void atkCC_callterrainattack(void) // nature power
{
    gCurrentMove = GetNaturePowerMove();

	gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
	TryUpdateCalledMoveWithZMove();
	UpdateMoveStartValuesForCalledMove();
	BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
	gBattlescriptCurrInstr++;
}

bool8 IsTerrainMoveIndoors(void)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_EREADER_TRAINER))
		return TRUE;

	return FALSE;
}

u16 GetNaturePowerMove(void)
{
	u16 move;

	switch (gTerrainType) {
		case ELECTRIC_TERRAIN:
			move = gTerrainTable[0].naturePowerMove;
			break;

		case GRASSY_TERRAIN:
			move = gTerrainTable[1].naturePowerMove;
			break;

		case MISTY_TERRAIN:
			move = gTerrainTable[2].naturePowerMove;
			break;

		case PSYCHIC_TERRAIN:
			move = gTerrainTable[3].naturePowerMove;
			break;

		default:
			if (IsTerrainMoveIndoors())
				move = gTerrainTable[BATTLE_TERRAIN_BUILDING + 4].naturePowerMove;
			else
				move = gTerrainTable[gBattleTerrain + 4].naturePowerMove;
	}

	return move;
}

static void atkCD_cureifburnedparalysedorpoisoned(void) // refresh
{
    if (gBattleMons[gBattlerAttacker].status1 & (STATUS1_POISON | STATUS1_BURN | STATUS1_PARALYSIS | STATUS1_TOXIC_POISON))
	{
		gBattleMons[gBattlerAttacker].status1 = 0;
		gBattlescriptCurrInstr += 5;
		gActiveBattler = gBattlerAttacker;
		BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
		MarkBattlerForControllerExec(gActiveBattler);
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

static void atkCE_settorment(void)
{
    if (!CanBeTormented(gBattlerTarget) || !BATTLER_ALIVE(gBattlerTarget))
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		gBattleMons[gBattlerTarget].status2 |= STATUS2_TORMENT;
		gBattlescriptCurrInstr += 5;
	}
}

static void atkCF_jumpifnodamage(void)
{
    if (gProtectStructs[gBattlerAttacker].physicalDmg
	||  gProtectStructs[gBattlerAttacker].specialDmg
	||  !(gBattleStruct->playedFocusPunchMessage & gBitTable[gBattlerAttacker])) //Forced to use Focus Punch by Encore
		gBattlescriptCurrInstr += 5;
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkD0_settaunt(void)
{
    if (!IsTaunted(gBattlerTarget))
	{
		gDisableStructs[gBattlerTarget].tauntTimer = 4;
		gDisableStructs[gBattlerTarget].tauntTimer2 = 4;
		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

static void atkD1_trysethelpinghand(void)
{
    gBattlerTarget = GetBattlerAtPosition(GetBattlerPosition(gBattlerAttacker) ^ BIT_FLANK);
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
     && !(gAbsentBattlerFlags & gBitTable[gBattlerTarget])
     && !gProtectStructs[gBattlerAttacker].helpingHand
     && !gProtectStructs[gBattlerTarget].helpingHand)
    {
        gProtectStructs[gBattlerTarget].helpingHand = 1;
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkD2_tryswapitems(void) // trick
{
    // Wild Pokemon can't swap items with player
	if ((SIDE(gBattlerAttacker) == B_SIDE_OPPONENT && !(gBattleTypeFlags & (BATTLE_TYPE_TRAINER)))
	|| IsRaidBattle())
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		// can't swap if two pokemon don't have an item
		// or if either of them is a forbidden item
		if ((gBattleMons[gBattlerAttacker].item == 0 && gBattleMons[gBattlerTarget].item == 0)
		|| !CanTransferItem(SPECIES(gBattlerAttacker), gBattleMons[gBattlerAttacker].item)
		|| !CanTransferItem(SPECIES(gBattlerAttacker), gBattleMons[gBattlerTarget].item)
		|| !CanTransferItem(SPECIES(gBattlerTarget), gBattleMons[gBattlerTarget].item)
		|| !CanTransferItem(SPECIES(gBattlerTarget), gBattleMons[gBattlerAttacker].item))
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

		// check if ability prevents swapping
		else if (ABILITY(gBattlerTarget) == ABILITY_STICKY_HOLD)  {
			gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
			gLastUsedAbility = ABILITY(gBattlerTarget);
			RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
		}

		//Took a while, but all checks passed and items can be safely swapped
		else
		{
			u16 oldItemAtk, *newItemAtk;

			newItemAtk = &gBattleStruct->changedItems[gBattlerAttacker];
			oldItemAtk = gBattleMons[gBattlerAttacker].item;
			*newItemAtk = gBattleMons[gBattlerTarget].item;

			gBattleMons[gBattlerAttacker].item = *newItemAtk;
			gBattleMons[gBattlerTarget].item = oldItemAtk;
			HandleUnburdenBoost(gBattlerAttacker); //Give or take away Unburden boost
			HandleUnburdenBoost(gBattlerTarget); //Give or take away Unburden boost

			gActiveBattler = gBattlerAttacker;
			BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gBattlerAttacker].item);
			MarkBattlerForControllerExec(gBattlerAttacker);

			gActiveBattler = gBattlerTarget;
			BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gBattlerTarget].item);
			MarkBattlerForControllerExec(gBattlerTarget);

			gBattleStruct->choicedMove[gBattlerTarget] = 0;
			gBattleStruct->choicedMove[gBattlerTarget] = 0;

			gBattleStruct->choicedMove[gBattlerAttacker] = 0;
			gBattleStruct->choicedMove[gBattlerAttacker] = 0;

			gBattlescriptCurrInstr += 5;

			PREPARE_ITEM_BUFFER(gBattleTextBuff1, *newItemAtk);
			PREPARE_ITEM_BUFFER(gBattleTextBuff2, oldItemAtk);

			if (oldItemAtk != 0 && *newItemAtk != 0)
				gBattleCommunication[MULTISTRING_CHOOSER] = 2; // attacker's item -> <- target's item
			else if (oldItemAtk == 0 && *newItemAtk != 0)
				gBattleCommunication[MULTISTRING_CHOOSER] = 0; // nothing -> <- target's item
			else
				gBattleCommunication[MULTISTRING_CHOOSER] = 1; // attacker's item -> <- nothing
		}
	}
}

static void atkD3_trycopyability(void) // role play
{
	ability_t* atkAbilityLoc, *defAbilityLoc;
	ability_t atkAbility, defAbility;

    if (gBattleControllerExecFlags) return;
	//Get correct location of ability
	atkAbilityLoc = GetAbilityLocation(gBattlerAttacker);
	defAbilityLoc = GetAbilityLocation(gBattlerTarget);

	atkAbility = *atkAbilityLoc;
	defAbility = *defAbilityLoc;

	if (atkAbility == defAbility
	||  defAbility == ABILITY_NONE
	||  CheckTableForAbility(atkAbility, gRolePlayAttackerBannedAbilities)
	||  CheckTableForAbility(defAbility, gRolePlayBannedAbilities))
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		gBattleStruct->backupAbility = atkAbility;
		*atkAbilityLoc = defAbility;
		gLastUsedAbility = atkAbility; //To display what changed
		TransferAbilityPopUp(gBattlerAttacker, gLastUsedAbility);
		ResetVarsForAbilityChange(gBattlerAttacker);
		gBattlescriptCurrInstr += 5;
	}
}

static void atkD4_trywish(void)
{
    u8 firstId, lastId;
	struct Pokemon* party;

	switch (gBattlescriptCurrInstr[1])
	{
		case 0: // use wish
			if (gWishFutureKnock.wishCounter[gBattlerAttacker] == 0)
			{
				gWishFutureKnock.wishCounter[gBattlerAttacker] = 2;
				gWishFutureKnock.wishMonId[gBattlerAttacker] = gBattlerPartyIndexes[gBattlerAttacker];
				gBattlescriptCurrInstr += 6;
			}
			else
			{
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
			}
			break;
		case 1: // heal effect
			PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gBattlerTarget, gWishFutureKnock.wishMonId[gBattlerTarget])
			if (!BATTLER_MAX_HP(gBattlerTarget))
			{
				party = LoadPartyRange(gBattlerTarget, &firstId, &lastId);

				gBattleMoveDamage = MathMax(1, GetMonData(&party[gWishFutureKnock.wishMonId[gBattlerTarget]], MON_DATA_MAX_HP, NULL) / 2);
				gBattleMoveDamage *= -1;
				gBattlescriptCurrInstr += 6;
			}
			else
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

			break;
	}
}

static void atkD5_trysetroots(void) // ingrain
{
    if (gStatuses3[gBattlerAttacker] & STATUS3_ROOTED)
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        gStatuses3[gBattlerAttacker] |= STATUS3_ROOTED;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkD6_doubledamagedealtifdamaged(void)
{
    if ((gProtectStructs[gBattlerAttacker].physicalDmg != 0
        && gProtectStructs[gBattlerAttacker].physicalBattlerId == gBattlerTarget)
     || (gProtectStructs[gBattlerAttacker].specialDmg != 0
        && gProtectStructs[gBattlerAttacker].specialBattlerId == gBattlerTarget))
    {
        gBattleScripting.dmgMultiplier = 2;
    }
    ++gBattlescriptCurrInstr;
}

void ActuallySetYawn(void)
{
	gStatuses3[gBattlerTarget] |= 0x1000;
}

static void atkD7_setyawn(void)
{
    u8 bank = gBattlerTarget;
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	bool8 fail = FALSE;

	if (BATTLER_SEMI_INVULNERABLE(bank) && !CanHitSemiInvulnerableTarget(gBattlerAttacker, bank, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_SLEEP)
	{
		gBattleStringLoader = gText_TargetAlreadyAsleep;
		fail = TRUE;
	}
	else if (gBattleMons[bank].status1 != STATUS1_NONE)
	{
		gBattleStringLoader = gText_TargetAlreadyHasStatusCondition; //String not in official games; officially "But it failed!"
		fail = TRUE;
	}
	else if (ABILITY(gBattlerAttacker) != ABILITY_INFILTRATOR && BankSideHasSafeguard(bank))
	{
		gBattleStringLoader = gText_TeamProtectedBySafeguard;
		fail = TRUE;
	}
	/*else if (CheckGrounding(bank) && gTerrainType == MISTY_TERRAIN) //Misty Terrain doesn't cause yawn to fail
	{
		gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}*/
	else if (CheckGrounding(bank) && gTerrainType == ELECTRIC_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInElectricTerrain;
		fail = TRUE;
	}
	else if (IsOfType(bank, TYPE_GRASS) && ABILITY(bank) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.battler = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	else if (IsOfType(bank, TYPE_GRASS) && IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.battler = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	else if (ABILITY(bank) == ABILITY_SWEETVEIL)
	{
		gBattleScripting.battler = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		return;
	}
	else if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_SWEETVEIL)
	{
		gBattleScripting.battler = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		return;
	}
	/*else if (IsUproarBeingMade())
	{
		gBattleScripting.battler = bank;
		gBattleStringLoader = gText_CantFallAsleepDuringUproar;
		fail = TRUE;
	}*/

	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_INSOMNIA:
			case ABILITY_VITAL_SPIRIT:
				gBattlescriptCurrInstr = BattleScript_TargetStayedAwakeUsingAbility;
				return;
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
				break;
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
				return;
			case ABILITY_SHIELDSDOWN:
				if (SPECIES(bank) == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
		}

		if (!fail && gStatuses3[gBattlerTarget] & STATUS3_YAWN)
		{
			gBattleStringLoader = gText_AlreadyDrowsy;
			fail = TRUE;
		}
	}

	if (fail)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = ptr;
	}
	else
		gBattlescriptCurrInstr += 5;
}

static void atkD8_setdamagetohealthdifference(void)
{
    gBattleMoveDamage = GetBaseCurrentHP(gBattlerTarget) - GetBaseCurrentHP(gBattlerAttacker);

	if (gBattleMoveDamage <= 0)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
		gBattlescriptCurrInstr += 5;
}

static void atkD9_scaledamagebyhealthratio(void)
{
    if (gDynamicBasePower == 0)
    {
        u8 power = gBattleMoves[gCurrentMove].power;
        
        gDynamicBasePower = gBattleMons[gBattlerAttacker].hp * power / gBattleMons[gBattlerAttacker].maxHP;
        if (gDynamicBasePower == 0)
            gDynamicBasePower = 1;
    }
    ++gBattlescriptCurrInstr;
}

static void atkDA_tryswapabilities(void) // skill swap
{
    ability_t* atkAbilityLoc, *defAbilityLoc;
	ability_t atkAbility, defAbility;

	//Get correct location of ability
	atkAbilityLoc = GetAbilityLocation(gBattlerAttacker);
	defAbilityLoc = GetAbilityLocation(gBattlerTarget);

	atkAbility = *atkAbilityLoc;
	defAbility = *defAbilityLoc;

	if (atkAbility == ABILITY_NONE || defAbility == ABILITY_NONE
	|| IsDynamaxed(gBattlerAttacker) || IsDynamaxed(gBattlerTarget)
	|| CheckTableForAbility(atkAbility, gSkillSwapBannedAbilities) || CheckTableForAbility(defAbility, gSkillSwapBannedAbilities)
	|| gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		*atkAbilityLoc = defAbility;
		*defAbilityLoc = atkAbility;

		ResetVarsForAbilityChange(gBattlerAttacker);
		ResetVarsForAbilityChange(gBattlerTarget);

		if (*atkAbilityLoc == ABILITY_TRUANT)
			gDisableStructs[gBattlerAttacker].truantCounter = 0; //Reset counter

		if (*defAbilityLoc == ABILITY_TRUANT)
			gDisableStructs[gBattlerTarget].truantCounter = 0; //Reset counter

		gBattlescriptCurrInstr += 5;
	}
}

static void atkDB_tryimprison(void)
{
    if ((gStatuses3[gBattlerAttacker] & STATUS3_IMPRISONED_OTHERS))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	else {
		PressurePPLoseOnUsingImprison(gBattlerAttacker);
		gStatuses3[gBattlerAttacker] |= STATUS3_IMPRISONED_OTHERS;
		gBattlescriptCurrInstr += 5;
	}
}

static void atkDC_trysetgrudge(void)
{
    if (gStatuses3[gBattlerAttacker] & STATUS3_GRUDGE)
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        gStatuses3[gBattlerAttacker] |= STATUS3_GRUDGE;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkDD_weightdamagecalculation(void)
{
    s32 i;

    for (i = 0; sWeightToDamageTable[i] != 0xFFFF; i += 2)
    {
        if (sWeightToDamageTable[i] > GetPokedexHeightWeight(SpeciesToNationalPokedexNum(gBattleMons[gBattlerTarget].species), 1))
            break;
    }
    if (sWeightToDamageTable[i] != 0xFFFF)
        gDynamicBasePower = sWeightToDamageTable[i + 1];
    else
        gDynamicBasePower = 120;
    ++gBattlescriptCurrInstr;
}

static void atkDE_assistattackselect(void)
{
    s32 chooseableMovesNo = 0;
	struct Pokemon* party;
	s32 monId, moveId;
	u16* movesArray = gBattleStruct->assistPossibleMoves;
	u8 firstMonId, lastMonId;

	party = LoadPartyRange(gBattlerAttacker, &firstMonId, &lastMonId);

	for (monId = firstMonId; monId < lastMonId; ++monId)
	{
		if (monId == gBattlerPartyIndexes[gBattlerAttacker]
		|| party[monId].box.species == SPECIES_NONE
		|| GetMonData(&party[monId], MON_DATA_IS_EGG, 0))
			continue;

		for (moveId = 0; moveId < MAX_MON_MOVES; moveId++)
		{
			u16 move = party[monId].box.moves[moveId];

			if (CheckTableForMove(move, gAssistBannedMoves))
				continue;

			if (move == MOVE_NONE)
				continue;

			movesArray[chooseableMovesNo] = move;
			chooseableMovesNo++;
		}
	}
	if (chooseableMovesNo)
	{
		gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
		gCalledMove = movesArray[((Random() & 0xFF) * chooseableMovesNo) >> 8];
		gBattlerTarget = GetMoveTarget(gCalledMove, 0);
		gBattlescriptCurrInstr += 5;
	}
	else
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
}

static void atkDF_trysetmagiccoat(void)
{
    gBattlerTarget = gBattlerAttacker;
    gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
    if (gCurrentTurnActionNumber == gBattlersCount - 1) // moves last turn
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        gProtectStructs[gBattlerAttacker].bounceMove = 1;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkE0_trysetsnatch(void) // snatch
{
    gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
    if (gCurrentTurnActionNumber == gBattlersCount - 1) // moves last turn
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        gProtectStructs[gBattlerAttacker].stealMove = 1;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkE1_trygetintimidatetarget(void)
{
    u8 side;

	gBattleScripting.battler = gBattleStruct->intimidateBattler;
	side = SIDE(gBattleScripting.battler);

	PREPARE_ABILITY_BUFFER(gBattleTextBuff1, ABILITY(gBattleScripting.battler))

	for (; gBattlerTarget < gBattlersCount; ++gBattlerTarget)
	{
		if (SIDE(gBattlerTarget) == side)
			continue;
		if (!(gAbsentBattlerFlags & gBitTable[gBattlerTarget]) && BATTLER_ALIVE(gBattlerTarget))
			break;
	}

	if (gBattlerTarget >= gBattlersCount)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
		gBattlescriptCurrInstr += 5;
}

static void atkE2_switchoutabilities(void)
{
	if (gBattleControllerExecFlags)
		return;

	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

	switch (gBattleStruct->switchOutAbilitiesState) {
		case 0: //Update current HP if dynamaxed
			if (IsDynamaxed(gActiveBattler))
			{
				gBattleMons[gActiveBattler].hp = GetBaseCurrentHP(gActiveBattler);
				BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
				MarkBattlerForControllerExec(gActiveBattler);
			}
			++gBattleStruct->switchOutAbilitiesState;
			break;

		case 1: //Update max HP if dynamaxed
			if (IsDynamaxed(gActiveBattler))
			{
				gBattleMons[gActiveBattler].maxHP = GetBaseMaxHP(gActiveBattler);
				BtlController_EmitSetMonData(0, REQUEST_MAX_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].maxHP);
				MarkBattlerForControllerExec(gActiveBattler);
				gBattleStruct->dynamaxData.timer[gActiveBattler] = 0;
			}
			++gBattleStruct->switchOutAbilitiesState;
			break;

		case 2: //Switch out abilities
			switch (ABILITY(gActiveBattler)) {
				case ABILITY_NATURAL_CURE:
					gBattleMons[gActiveBattler].status1 = 0;
					BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, gBitTable[gBattleStruct->battlerPartyIndexes[gActiveBattler]], 4, &gBattleMons[gActiveBattler].status1);
					MarkBattlerForControllerExec(gActiveBattler);
					break;
				case ABILITY_REGENERATOR:
					gBattleMons[gActiveBattler].hp = MathMin(gBattleMons[gActiveBattler].maxHP, gBattleMons[gActiveBattler].hp + (gBattleMons[gActiveBattler].maxHP / 3));
					BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, gBitTable[gBattleStruct->battlerPartyIndexes[gActiveBattler]], 4, &gBattleMons[gActiveBattler].hp);
					MarkBattlerForControllerExec(gActiveBattler);
			}
			++gBattleStruct->switchOutAbilitiesState;
			break;

		case 3: //Form reversion
			SwitchOutFormsRevert(gActiveBattler);
			++gBattleStruct->switchOutAbilitiesState;
			break;

		case 4: //Gigantamax reversion
			TryRevertBankGigantamax(gActiveBattler);
			++gBattleStruct->switchOutAbilitiesState;
			break;

		case 5: //Done
			gBattleStruct->switchOutAbilitiesState = 0; //Reset for next time
			gBattlescriptCurrInstr += 2;
	}
}

static void atkE3_jumpifhasnohp(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

    if (gBattleMons[gActiveBattler].hp == 0)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    else
        gBattlescriptCurrInstr += 6;
}

static void atkE4_getsecretpowereffect(void)
{
    switch (gTerrainType) {
		case ELECTRIC_TERRAIN:
			gBattleCommunication[MOVE_EFFECT_BYTE] = gTerrainTable[0].secretPowerEffect;
			break;

		case GRASSY_TERRAIN:
			gBattleCommunication[MOVE_EFFECT_BYTE] = gTerrainTable[1].secretPowerEffect;
			break;

		case MISTY_TERRAIN:
			gBattleCommunication[MOVE_EFFECT_BYTE] = gTerrainTable[2].secretPowerEffect;
			break;

		case PSYCHIC_TERRAIN:
			gBattleCommunication[MOVE_EFFECT_BYTE] = gTerrainTable[3].secretPowerEffect;
			break;

		default:
			if (IsTerrainMoveIndoors())
				gBattleCommunication[MOVE_EFFECT_BYTE] = gTerrainTable[BATTLE_TERRAIN_BUILDING + 4].secretPowerEffect;
			else
				gBattleCommunication[MOVE_EFFECT_BYTE] = gTerrainTable[gBattleTerrain + 4].secretPowerEffect;
	}
	gBattlescriptCurrInstr++;
}

static void atkE5_pickup(void)
{
    u16 item = 0;
	u8 chance = 0;
    u8 level;
    int i;

	for (i = 0; i < 6; ++i)
	{
		if (gPlayerParty[i].box.species == SPECIES_NONE) break;
		if (gPlayerParty[i].box.heldItem != ITEM_NONE) continue;
		if (GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, 0)) continue;

		level = gPlayerParty[i].level;

		switch (GetMonAbility(&gPlayerParty[i])) {
			case ABILITY_PICKUP:
				chance = 10; // ~10% chance of pickup to activate
				item = ChoosePickupItem(level);
				break;

			case ABILITY_HONEYGATHER:
				chance = 5 + 5 * ((level - 1) / 10);
				item = ITEM_HONEY;
		}

		if (Random() % 100 < chance)
			SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &item);
	}

	++gBattlescriptCurrInstr;
}

static u16 ChoosePickupItem(u8 level)
{
	u32 row_num = (level - 1) / 10;
    u16 k = Random();
    u32 i;
	const u16* const common_row = sPickupCommonItems + row_num;
	const u16* const rare_row = sPickupRareItems + row_num;

	for (i = 0; i < PU_COMMON_PER_ROW; ++i)
		if (k < pickup_common_item_ceilings[i])
			return common_row[i];

	for (i = 0; i < PU_RARE_PER_ROW; ++i)
		if (k < pickup_rare_item_ceilings[i])
			return rare_row[i];

	// will never be reached
	return ITEM_NONE;
}

static void atkE6_docastformchangeanimation(void)
{
    gActiveBattler = gBattleScripting.battler;
    if (gBattleMons[gActiveBattler].status2 & STATUS2_SUBSTITUTE)
        *(&gBattleStruct->formToChangeInto) |= 0x80;
    BtlController_EmitBattleAnimation(0, B_ANIM_CASTFORM_CHANGE, gBattleStruct->formToChangeInto);
    MarkBattlerForControllerExec(gActiveBattler);
    ++gBattlescriptCurrInstr;
}

static void atkE7_trycastformdatachange(void)
{
	u8 form;
	u8 bank = gBattleScripting.battler;

    if (gBattleControllerExecFlags) return;

	gBattlescriptCurrInstr++;

	switch (SPECIES(bank)) { //Not ability b/c you can lose ability
		#ifdef SPECIES_CASTFORM
		case SPECIES_CASTFORM:
			form = CastformDataTypeChange(bank);
			if (form)
			{
				BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
				gBattleStruct->formToChangeInto = form - 1;
			}
			return;
		#endif

		#if (defined SPECIES_CHERRIM && defined SPECIES_CHERRIM_SUN)
		case SPECIES_CHERRIM:
			if (ABILITY(bank) == ABILITY_FLOWERGIFT && !IS_TRANSFORMED(bank)
			&& WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY
			&& ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA)
			{
				DoFormChange(bank, SPECIES_CHERRIM_SUN, FALSE, FALSE, FALSE);
				BattleScriptPushCursorAndCallback(BattleScript_FlowerGift);
			}
			break;

		case SPECIES_CHERRIM_SUN:
			if (ABILITY(bank) != ABILITY_FLOWERGIFT
			|| !WEATHER_HAS_EFFECT || !(gBattleWeather & WEATHER_SUN_ANY)
			|| ITEM_EFFECT(bank) == HOLD_EFFECT_UTILITY_UMBRELLA)
			{
				DoFormChange(bank, SPECIES_CHERRIM, FALSE, FALSE, FALSE);
				BattleScriptPushCursorAndCallback(BattleScript_FlowerGift);
			}
		#endif
	}
}

u8 CastformDataTypeChange(u8 bank)
{
	u8 formChange = CASTFORM_NO_CHANGE;
	#ifdef SPECIES_CASTFORM
	u8 itemEffect = ITEM_EFFECT(bank);
    ability_t ability = ABILITY(bank);
	bool8 weatherHasEffect = WEATHER_HAS_EFFECT;

	if (SPECIES(bank) != SPECIES_CASTFORM || !BATTLER_ALIVE(bank))
		return formChange;

	if (gBattleMonForms[bank] != CASTFORM_NORMAL
	&& (!weatherHasEffect
	 || ability != ABILITY_FORECAST
	 || !(gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_SUN_ANY | WEATHER_HAIL_ANY))))
	{
		SET_BATTLER_TYPE(bank, TYPE_NORMAL);
		formChange = CASTFORM_TO_NORMAL;
	}
	else if (weatherHasEffect && ability == ABILITY_FORECAST)
	{
		if (gBattleWeather & WEATHER_SUN_ANY && itemEffect != HOLD_EFFECT_UTILITY_UMBRELLA)
		{
			if (gBattleMonForms[bank] != CASTFORM_SUN)
			{
				SET_BATTLER_TYPE(bank, TYPE_FIRE);
				formChange = CASTFORM_TO_FIRE;
			}
		}
		else if (gBattleWeather & WEATHER_RAIN_ANY && itemEffect != HOLD_EFFECT_UTILITY_UMBRELLA)
		{
			if (gBattleMonForms[bank] != CASTFORM_RAIN)
			{
				SET_BATTLER_TYPE(bank, TYPE_WATER);
				formChange = CASTFORM_TO_WATER;
			}
		}
		else if (gBattleWeather & WEATHER_HAIL_ANY)
		{
			if (gBattleMonForms[bank] != CASTFORM_HAIL)
			{
				SET_BATTLER_TYPE(bank, TYPE_ICE);
				formChange = CASTFORM_TO_ICE;
			}
		}
		else if (gBattleMonForms[bank] != CASTFORM_NORMAL)
		{
			SET_BATTLER_TYPE(bank, TYPE_NORMAL);
			formChange = CASTFORM_TO_NORMAL;
		}
	}
	#endif

	return formChange;
}

static void atkE8_settypebasedhalvers(void) // water and mud sport
{
    bool8 worked = FALSE;

	if (gBattleMoves[gCurrentMove].effect == EFFECT_MUD_SPORT)
	{
		if (!IsMudSportActive())
		{
			gBattleStruct->MudSportTimer = 5;
			gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			worked = TRUE;
		}
	}
	else //Water Sport
	{
		if (!IsWaterSportActive())
		{
			gBattleStruct->WaterSportTimer = 5;
			gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			worked = TRUE;
		}
	}

	if (worked)
		gBattlescriptCurrInstr += 5;
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkE9_setweatherballtype(void)
{
    if (WEATHER_HAS_EFFECT)
    {
        if (gBattleWeather & WEATHER_ANY)
            gBattleScripting.dmgMultiplier = 2;
        if (gBattleWeather & WEATHER_RAIN_ANY)
            *(&gBattleStruct->dynamicMoveType) = TYPE_WATER | 0x80;
        else if (gBattleWeather & WEATHER_SANDSTORM_ANY)
            *(&gBattleStruct->dynamicMoveType) = TYPE_ROCK | 0x80;
        else if (gBattleWeather & WEATHER_SUN_ANY)
            *(&gBattleStruct->dynamicMoveType) = TYPE_FIRE | 0x80;
        else if (gBattleWeather & WEATHER_HAIL_ANY)
            *(&gBattleStruct->dynamicMoveType) = TYPE_ICE | 0x80;
        else
            *(&gBattleStruct->dynamicMoveType) = TYPE_NORMAL | 0x80;
    }
    ++gBattlescriptCurrInstr;
}

static void atkEA_tryrecycleitem(void)
{
    if (SAVED_CONSUMED_ITEMS(gBattlerAttacker) != ITEM_NONE && ITEM(gBattlerAttacker) == ITEM_NONE)
	{
		RecycleItem(gBattlerAttacker);
		gBattlescriptCurrInstr += 5;
	}
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void RecycleItem(u8 bank)
{
	gActiveBattler = bank;
	gLastUsedItem = SAVED_CONSUMED_ITEMS(gActiveBattler);

	SAVED_CONSUMED_ITEMS(gActiveBattler) = ITEM_NONE;
	CONSUMED_ITEMS(gActiveBattler) = ITEM_NONE;	//Remove the temporary item storage
	RemoveBankFromPickupStack(gActiveBattler);

	gBattleMons[gActiveBattler].item = gLastUsedItem;
	HandleUnburdenBoost(gActiveBattler); //Remove the Unburden Boost

	BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
	MarkBattlerForControllerExec(gActiveBattler);
}

static void atkEB_settypetoterrain(void)
{
    u8 type = GetCamouflageType();

	if (!IsOfType(gBattlerAttacker, type))
	{
		gBattleMons[gBattlerAttacker].type1 = type;
		gBattleMons[gBattlerAttacker].type2 = type;
		gBattleMons[gBattlerAttacker].type3 = TYPE_BLANK;
		PREPARE_TYPE_BUFFER(gBattleTextBuff1, type);
		gBattlescriptCurrInstr += 5;
	}
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkEC_pursuitrelated(void)
{
    gActiveBattler = GetBattlerAtPosition(GetBattlerPosition(gBattlerAttacker) ^ BIT_FLANK);

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
     && !(gAbsentBattlerFlags & gBitTable[gActiveBattler])
     && gChosenActionByBattler[gActiveBattler] == 0
     && gChosenMoveByBattler[gActiveBattler] == MOVE_PURSUIT)
    {
        gActionsByTurnOrder[gActiveBattler] = 11;
        gCurrentMove = MOVE_PURSUIT;
        gBattlescriptCurrInstr += 5;
        gBattleScripting.animTurn = 1;
        gBattleScripting.field_20 = gBattlerAttacker;
        gBattlerAttacker = gActiveBattler;
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkED_snatchsetbattlers(void)
{
    gEffectBattler = gBattlerAttacker;
    if (gBattlerAttacker == gBattlerTarget)
        gBattlerAttacker = gBattlerTarget = gBattleScripting.battler;
    else
        gBattlerTarget = gBattleScripting.battler;
    gBattleScripting.battler = gEffectBattler;
    ++gBattlescriptCurrInstr;
}

u8 GetCamouflageType(void)
{
	u8 type;

	switch (gTerrainType) {
		case ELECTRIC_TERRAIN:
			type = gTerrainTable[0].camouflageType;
			break;

		case GRASSY_TERRAIN:
			type = gTerrainTable[1].camouflageType;
			break;

		case MISTY_TERRAIN:
			type = gTerrainTable[2].camouflageType;
			break;

		case PSYCHIC_TERRAIN:
			type = gTerrainTable[3].camouflageType;
			break;

		default:
			if (IsTerrainMoveIndoors())
				type = gTerrainTable[BATTLE_TERRAIN_BUILDING + 4].camouflageType;
			else
				type = gTerrainTable[gBattleTerrain + 4].camouflageType;
	}

	return type;
}

static void atkEE_removelightscreenreflect(void) // brick break
{
    u8 side = SIDE(gBattlerTarget);
	if (gSideTimers[side].reflectTimer
	||  gSideTimers[side].lightscreenTimer
	||  gSideStatuses[side] & SIDE_STATUS_REFLECT
	||  gSideStatuses[side] & SIDE_STATUS_LIGHTSCREEN
	||  gBattleStruct->AuroraVeilTimers[side])
	{
		//Now handled as a move effect
		/*gSideStatuses[side] &= ~(SIDE_STATUS_REFLECT);
		gSideStatuses[side] &= ~(SIDE_STATUS_LIGHTSCREEN);
		gSideTimers[side].reflectTimer = 0;
		gSideTimers[side].lightscreenTimer = 0;
		gBattleStruct->AuroraVeilTimers[side] = 0;*/
		gBattleScripting.animTurn = 1;
		gBattleScripting.animTargetsHit = 1;
	}
	else {
		gBattleScripting.animTurn = 0;
		gBattleScripting.animTargetsHit = 0;
	}
	gBattlescriptCurrInstr++;
}

#ifdef CRITICAL_CAPTURE
struct CriticalCaptureOdds
{
	u16 numCaught; //Less than or equal to this number
	u16 oddsMultiplier; //x10
};

static const struct CriticalCaptureOdds sCriticalCaptureSpeciesCounts[] =
{
	{30,      0}, //x0.0
	{150,     5}, //x0.5
	{300,    10}, //x1.0
	{450,    15}, //x1.5
	{600,    20}, //x2.0
	{0xFFFF, 25}, //x2.5 default
};
#endif

static bool8 CriticalCapture(u32 odds)
{
	#ifndef CRITICAL_CAPTURE
		gBattleStruct->criticalCapture = FALSE;
		return FALSE;
	#else
	u16 pokesCaught = GetNationalPokedexCount(FLAG_GET_CAUGHT);

	for (u32 i = 0; i < NELEMS(sCriticalCaptureSpeciesCounts); ++i)
	{
		if (pokesCaught <= sCriticalCaptureSpeciesCounts[i].numCaught)
		{
			u8 multiplier = sCriticalCaptureSpeciesCounts[i].oddsMultiplier;
			#ifdef ITEM_CATCHING_CHARM
			if (CheckBagHasItem(ITEM_CATCHING_CHARM, 1) > 0)
				multiplier += 5; //Temp value until the real one becomes known
			#endif
			odds = (odds * multiplier) / 10;
			break;
		}
	}

	odds /= 6;
	gBattleStruct->criticalCaptureSuccess = FALSE;
	if (Random() % 0xFF < odds)
		return gBattleStruct->criticalCapture = TRUE;
	else
		return gBattleStruct->criticalCapture = FALSE;
	#endif
}

bool8 IsCriticalCapture(void)
{
	return gBattleStruct->criticalCapture;
}

bool8 IsCriticalCaptureSuccess(void)
{
	return gBattleStruct->criticalCaptureSuccess;
}

u8 GetCatchingBattler(void)
{
	u8 battler;

	if (IS_DOUBLE_BATTLE && GetBankPartyData(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT))->hp > 0)
		battler = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
	else
		battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);

	if (battler >= gBattlersCount || SIDE(battler) == B_SIDE_PLAYER)
		battler = B_POSITION_OPPONENT_LEFT;

	return battler;
}

static void atkEF_handleballthrow(void)
{
    u8 ballMultiplier = 0;
    u16 atkSpecies;
	u16 defSpecies;
	u8 atkLevel;
	u8 defLevel;
	u8 ballType;

    if (gBattleControllerExecFlags) return;

    gActiveBattler = gBattlerAttacker;
	gBattlerTarget = GetCatchingBattler();

    atkSpecies = gBattleMons[gBattlerAttacker].species;
	defSpecies = gBattleMons[gBattlerTarget].species;
	atkLevel = gBattleMons[gBattlerAttacker].level;
	defLevel = gBattleMons[gBattlerTarget].level;

	ballType = ItemId_GetType(gLastUsedItem);

    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		#ifdef FLAG_CATCH_TRAINERS_POKEMON
			if (FlagGet(FLAG_CATCH_TRAINERS_POKEMON))
			{
				BtlController_EmitBallThrowAnim(0, 4);
				MarkBattlerForControllerExec(gActiveBattler);
				gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;
				if (ballType != BALL_TYPE_PARK_BALL)
					SetMonData(GetBankPartyData(gBattlerTarget), MON_DATA_POKEBALL, &ballType);
				if (CalculatePlayerPartyCount() == 6)
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			}
			else
			{
				BtlController_EmitBallThrowAnim(0, 5);
				MarkBattlerForControllerExec(gActiveBattler);
				gBattlescriptCurrInstr = BattleScript_TrainerBallBlock;
			}
		#else
			BtlController_EmitBallThrowAnim(0, BALL_TRAINER_BLOCK);
			MarkBattlerForControllerExec(gActiveBattler);
			gBattlescriptCurrInstr = BattleScript_TrainerBallBlock;
		#endif
	}
    else if (gBattleTypeFlags & BATTLE_TYPE_GHOST
	#ifdef FLAG_NO_CATCHING
	|| FlagGet(FLAG_NO_CATCHING)
	#endif
	#ifdef FLAG_NO_CATCHING_AND_RUNNING
	|| FlagGet(FLAG_NO_CATCHING_AND_RUNNING)
	#endif
	)
	{
		BtlController_EmitBallThrowAnim(0, BALL_GHOST_DODGE);
		MarkBattlerForControllerExec(gActiveBattler);
		gBattlescriptCurrInstr = BattleScript_GhostBallDodge;
	}
    else if (gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_POKEDUDE))
	{
		BtlController_EmitBallThrowAnim(0, BALL_3_SHAKES_SUCCESS);
		MarkBattlerForControllerExec(gActiveBattler);
		gBattlescriptCurrInstr = BattleScript_OldMan_Pokedude_CaughtMessage;
	}
    else
	{
		u32 odds;
		u8 catchRate;

		if (ballType == BALL_TYPE_SAFARI_BALL)
			catchRate = (gBattleStruct->safariCatchFactor * 1275 / 100);
		else
			catchRate = gBaseStats[GetBankPartyData(gBattlerTarget)->box.species].catchRate; //Uses party data b/c Transform update Gen 5+

		if (ballType >= BALL_TYPE_NET_BALL)
		{
			switch (ballType) {
				case BALL_TYPE_NET_BALL:
					if (IsOfType(gBattlerTarget, TYPE_WATER) || IsOfType(gBattlerTarget, TYPE_BUG))
						ballMultiplier = 35;
					else
						ballMultiplier = 10;
					break;

				case BALL_TYPE_DIVE_BALL:
					if (GetCurrentMapType() == MAP_TYPE_UNDERWATER
					|| gFishingByte || TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
						ballMultiplier = 35;
					else
						ballMultiplier = 10;
					break;

				case BALL_TYPE_NEST_BALL:
					if (gBattleMons[gBattlerTarget].level <= 29)
						ballMultiplier = MathMax(10, 41 - defLevel);
					else
						ballMultiplier = 10;
					break;

				case BALL_TYPE_REPEAT_BALL:
					if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(defSpecies), FLAG_GET_CAUGHT))
						ballMultiplier = 35;
					else
						ballMultiplier = 10;
					break;

				case BALL_TYPE_TIMER_BALL:
					ballMultiplier = MathMax(40, 10 + gBattleResults.battleTurnCounter * 3);
					break;

				case BALL_TYPE_LUXURY_BALL:
				case BALL_TYPE_PREMIER_BALL:
				case BALL_TYPE_CHERISH_BALL:
				case BALL_TYPE_FRIEND_BALL:
				case BALL_TYPE_HEAL_BALL:
					ballMultiplier = 10;
					break;

				case BALL_TYPE_LEVEL_BALL:
					if (atkLevel <= defLevel)
						ballMultiplier = 10;
					else if (atkLevel > defLevel && atkLevel < defLevel * 2)
						ballMultiplier = 20;
					else if (atkLevel > defLevel * 2 && atkLevel < defLevel * 4)
						ballMultiplier = 40;
					else
						ballMultiplier = 80;
					break;

				case BALL_TYPE_LURE_BALL:
					if (gFishingByte)
						ballMultiplier = 50;
					else
						ballMultiplier = 10;
					break;

				case BALL_TYPE_MOON_BALL: ;
					switch (defSpecies) {
						default:
							ballMultiplier = 10;
							break;
						#ifdef SPECIES_NIDORAN_F
						case SPECIES_NIDORAN_F:
						#endif
						#ifdef SPECIES_NIDORINA
						case SPECIES_NIDORINA:
						#endif
						#ifdef SPECIES_NIDOQUEEN
						case SPECIES_NIDOQUEEN:
						#endif
						#ifdef SPECIES_NIDORAN_M
						case SPECIES_NIDORAN_M:
						#endif
						#ifdef SPECIES_NIDORINO
						case SPECIES_NIDORINO:
						#endif
						#ifdef SPECIES_NIDOKING
						case SPECIES_NIDOKING:
						#endif
						#ifdef SPECIES_CLEFFA
						case SPECIES_CLEFFA:
						#endif
						#ifdef SPECIES_CLEFAIRY
						case SPECIES_CLEFAIRY:
						#endif
						#ifdef SPECIES_CLEFABLE
						case SPECIES_CLEFABLE:
						#endif
						#ifdef SPECIES_IGGLYBUFF
						case SPECIES_IGGLYBUFF:
						#endif
						#ifdef SPECIES_JIGGLYPUFF
						case SPECIES_JIGGLYPUFF:
						#endif
						#ifdef SPECIES_WIGGLYTUFF
						case SPECIES_WIGGLYTUFF:
						#endif
						#ifdef SPECIES_SKITTY
						case SPECIES_SKITTY:
						#endif
						#ifdef SPECIES_DELCATTY
						case SPECIES_DELCATTY:
						#endif
						#ifdef SPECIES_MUNNA
						case SPECIES_MUNNA:
						#endif
						#ifdef SPECIES_MUSHARNA
						case SPECIES_MUSHARNA:
						#endif
							ballMultiplier = 40;
							break;
					}

					break;

				case BALL_TYPE_LOVE_BALL:
					if (SpeciesToNationalPokedexNum(atkSpecies) == SpeciesToNationalPokedexNum(defSpecies))
					{
						u8 atkGender = GetGenderFromSpeciesAndPersonality(atkSpecies, gBattleMons[gBattlerAttacker].personality);
						u8 defGender = GetGenderFromSpeciesAndPersonality(defSpecies, gBattleMons[gBattlerTarget].personality);

						if (atkGender != 0xFF && defGender != 0xFF && atkGender != defGender)
							ballMultiplier = 80;
						else
							ballMultiplier = 10;
					}
					else
						ballMultiplier = 10;
					break;

				//Heavy Ball modifies the catch rate itself, not the multiplier
				case BALL_TYPE_HEAVY_BALL:
                {
					
					//Apparently abilities or Float Stone have no affect here
					u16 defWeight = GetPokedexHeightWeight(SpeciesToNationalPokedexNum(defSpecies), 1);
					u8 oldCatchRate;
                    ballMultiplier = 10;
					if (defWeight < 1000)
					{
						oldCatchRate = catchRate;
						catchRate -= 20;
						if (catchRate > oldCatchRate)
							catchRate = 1; //New in USUM
					}
					else if (defWeight >= 2000 && defWeight < 3000)
						catchRate += 20;
					else if (defWeight > 3000)
						catchRate += 30;
                }
					break;

				case BALL_TYPE_FAST_BALL:
					if (gBaseStats[defSpecies].baseSpeed >= 100)
						ballMultiplier = 40;
					break;

				case BALL_TYPE_SPORT_BALL:
					ballMultiplier = 15;
					break;

				case BALL_TYPE_DUSK_BALL:
					if (GetCurrentMapType() == MAP_TYPE_UNDERGROUND)
						ballMultiplier = DUSK_BALL_MULTIPLIER;

					#ifdef DNS_ENABLED
					else if (IsNightTime())
						ballMultiplier = DUSK_BALL_MULTIPLIER;
					#endif
					break;

				case BALL_TYPE_QUICK_BALL:
					if (gBattleResults.battleTurnCounter == 0)
						ballMultiplier = 50;
					else
						ballMultiplier = 10;
					break;

				case BALL_TYPE_DREAM_BALL:
					if (gBattleMons[gBattlerTarget].status1 & STATUS1_SLEEP)
						ballMultiplier = 30;
					break;

				case BALL_TYPE_BEAST_BALL:
					if (CheckTableForSpecies(defSpecies, gUltraBeastList))
						ballMultiplier = 50;
					else
						ballMultiplier = 1;
					break;
			}
		}
		else
			ballMultiplier = sBallCatchBonuses[ballType - BALL_TYPE_ULTRA_BALL];

		if (CheckTableForSpecies(defSpecies, gUltraBeastList) && ballType != BALL_TYPE_BEAST_BALL)
			ballMultiplier = 1; //All balls except for Beast Ball have a hard time catching Ultra Beasts

		odds = (((catchRate * ballMultiplier) / 10) * (gBattleMons[gBattlerTarget].maxHP * 3 - gBattleMons[gBattlerTarget].hp * 2)) / (3 * gBattleMons[gBattlerTarget].maxHP);

		#ifndef NO_HARDER_WILD_DOUBLES
		if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && IS_DOUBLE_BATTLE)
		{
			u16 PokesCaught = GetNationalPokedexCount(FLAG_GET_CAUGHT);
			if (PokesCaught < 30)
				odds = (odds * 30) / 100;
			else if (PokesCaught <= 150)
				odds = (odds * 50) / 100;
			else if (PokesCaught <= 300)
				odds = (odds * 70) / 100;
			else if (PokesCaught <= 450)
				odds = (odds * 80) / 100;
			else if (PokesCaught <= 600)
				odds = (odds * 80) / 100;
		}
		#endif

		if (gBattleMons[gBattlerTarget].status1 & (STATUS1_SLEEP | STATUS1_FREEZE))
			odds = (odds * 25) / 10;
		if (gBattleMons[gBattlerTarget].status1 & (STATUS1_PSN_ANY | STATUS1_BURN | STATUS1_PARALYSIS))
			odds = (odds * 15) / 10;

		if (IsRaidBattle()) //Dynamax Raid Pokemon can be caught easier
			odds *= 4;

		if (ballType != BALL_TYPE_SAFARI_BALL)
		{
			if (ballType == BALL_TYPE_MASTER_BALL)
				gBattleResults.usedMasterBall = 1;

			//This was used for the TV shows in Ruby, but seems kind of pointless in FR.
			//Commenting it out also prevents errors from using poke balls with large indices.
			//else if (gBattleResults.usedBalls[ballType - BALL_TYPE_ULTRA_BALL] < 0xFF)
			//		gBattleResults.usedBalls[ballType - BALL_TYPE_ULTRA_BALL]++;
		}

		if (odds >= 0xFF) //Pokemon is Caught
		{
			BtlController_EmitBallThrowAnim(0, 4);
			MarkBattlerForControllerExec(gActiveBattler);
			gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;

			if (ballType != BALL_TYPE_PARK_BALL || IsRaidBattle())
				SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]], MON_DATA_POKEBALL, &ballType);

			if (CalculatePlayerPartyCount() == 6)
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			else
				gBattleCommunication[MULTISTRING_CHOOSER] = 1;
		}
		else //Pokemon may be caught, calculate shakes
		{
			u8 shakes, maxShakes;

			if (CriticalCapture(odds))
			{
				maxShakes = 2;  //Critical capture doesn't gauarantee capture
			}
			else
			{
				maxShakes = 4;
			}

			if (ballType == BALL_TYPE_MASTER_BALL
			||	ballType == BALL_TYPE_PARK_BALL)
				shakes = maxShakes;
			else
			{
				odds = (0xFFFF0 / Sqrt(Sqrt((0xFF0000 / odds))));
				for (shakes = 0; shakes < maxShakes && Random() < odds; ++shakes) ;
			}

			BtlController_EmitBallThrowAnim(0, shakes);
			MarkBattlerForControllerExec(gActiveBattler);

			if (!gBattleStruct->firstFailedPokeBallStored)
			{
				gBattleStruct->firstFailedPokeBallStored = TRUE; //Only once per battle
				gBattleStruct->failedThrownPokeBall = gLastUsedItem;
			}

			if (shakes >= maxShakes)
			{
				if (gBattleStruct->criticalCapture)
					gBattleStruct->criticalCaptureSuccess = TRUE;

				gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;
				if (ballType != BALL_TYPE_PARK_BALL || IsRaidBattle())
					SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]], MON_DATA_POKEBALL, &ballType);

				if (CalculatePlayerPartyCount() == 6)
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			}
			else if (IsRaidBattle())
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = shakes;
				gBattlescriptCurrInstr = BattleScript_RaidMonEscapeBall;
			}
			else //Rip
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = shakes;
				gBattlescriptCurrInstr = BattleScript_ShakeBallThrow;
			}
		}
	}
}

struct Pokemon* LoadTargetPartyData(void)
{
	return GetBankPartyData(gBattlerTarget);
}

static void atkF0_givecaughtmon(void)
{
    struct Pokemon* mon = LoadTargetPartyData();

	if (GiveMonToPlayer(mon) != MON_GIVEN_TO_PARTY)
	{
		if (!ShouldShowBoxWasFullMessage())
		{
			gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			StringCopy(gStringVar1, GetBoxNamePtr(VarGet(VAR_PC_BOX_TO_SEND_MON)));
			GetMonData(mon, MON_DATA_NICKNAME, gStringVar2);
		}
		else
		{
			StringCopy(gStringVar1, GetBoxNamePtr(VarGet(VAR_PC_BOX_TO_SEND_MON))); //Box the mon was sent to
			GetMonData(mon, MON_DATA_NICKNAME, gStringVar2);
			StringCopy(gStringVar3, GetBoxNamePtr(GetPCBoxToSendMon())); //Box the mon was going to be sent to
			gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		}

		if (FlagGet(FLAG_SYS_NOT_SOMEONES_PC))
			++gBattleCommunication[MULTISTRING_CHOOSER];
	}

	if (!IsTimeInVarInFuture(VAR_SWARM_DAILY_EVENT))
		IncrementGameStat(GAME_STAT_CAUGHT_TODAY);
	else
		SetGameStat(GAME_STAT_CAUGHT_TODAY, 0);
	gBattleResults.caughtMonSpecies = SPECIES(gBattlerTarget);
	GetMonData(mon, MON_DATA_NICKNAME, gBattleResults.caughtMonNick);
	++gBattlescriptCurrInstr;
}

static void atkF1_trysetcaughtmondexflags(void)
{
    struct Pokemon* mon = LoadTargetPartyData();
    u16 species;
    u32 personality;
	TryRevertMega(mon); //Megas aren't set in the habitat table

	species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);

	if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
	else
	{
		HandleSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_CAUGHT, personality);
		gBattlescriptCurrInstr += 5;
	}
}

static void atkF2_displaydexinfo(void)
{
    u16 species = GetMonData(&gEnemyParty[0], MON_DATA_SPECIES, NULL);

    switch (gBattleCommunication[0])
    {
    case 0:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_WHITE);
        ++gBattleCommunication[0];
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            FreeAllWindowBuffers();
            gBattleCommunication[TASK_ID] = DexScreen_RegisterMonToPokedex(species);
            ++gBattleCommunication[0];
        }
        break;
    case 2:
        if (!gPaletteFade.active
            && gMain.callback2 == BattleMainCB2
            && !gTasks[gBattleCommunication[TASK_ID]].isActive)
        {
            CpuFill32(0, (void *)VRAM, VRAM_SIZE);
            SetVBlankCallback(VBlankCB_Battle);
            ++gBattleCommunication[0];
        }
        break;
    case 3:
        InitBattleBgsVideo();
        LoadBattleTextboxAndBackground();
        gBattle_BG3_X = 0x100;
        ++gBattleCommunication[0];
        break;
    case 4:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            CreateMonPicSprite_HandleDeoxys(species,
                                            gBattleMons[B_POSITION_OPPONENT_LEFT].otId,
                                            gBattleMons[B_POSITION_OPPONENT_LEFT].personality,
                                            TRUE,
                                            120,
                                            64,
                                            0,
                                            0xFFFF);
            CpuFill32(0, gPlttBufferFaded, BG_PLTT_SIZE);
            BeginNormalPaletteFade(0x1FFFF, 0, 0x10, 0, RGB_BLACK);
            ShowBg(0);
            ShowBg(3);
            ++gBattleCommunication[0];
        }
        break;
    case 5:
        if (!gPaletteFade.active)
            ++gBattlescriptCurrInstr;
        break;
    }
}

void HandleBattleWindow(u8 xStart, u8 yStart, u8 xEnd, u8 yEnd, u8 flags)
{
    s32 destY, destX;
    u16 var = 0;

    for (destY = yStart; destY <= yEnd; ++destY)
    {
        for (destX = xStart; destX <= xEnd; ++destX)
        {
            if (destY == yStart)
            {
                if (destX == xStart)
                    var = 0x1022;
                else if (destX == xEnd)
                    var = 0x1024;
                else
                    var = 0x1023;
            }
            else if (destY == yEnd)
            {
                if (destX == xStart)
                    var = 0x1028;
                else if (destX == xEnd)
                    var = 0x102A;
                else
                    var = 0x1029;
            }
            else
            {
                if (destX == xStart)
                    var = 0x1025;
                else if (destX == xEnd)
                    var = 0x1027;
                else
                    var = 0x1026;
            }
            if (flags & WINDOW_CLEAR)
                var = 0;
            if (flags & WINDOW_x80)
                CopyToBgTilemapBufferRect_ChangePalette(1, &var, destX, destY, 1, 1, 0x11);
            else
                CopyToBgTilemapBufferRect_ChangePalette(0, &var, destX, destY, 1, 1, 0x11);
        }
    }
    CopyBgTilemapBufferToVram(1);
}

void BattleCreateYesNoCursorAt(void)
{
    u16 src[2];

    src[0] = 1;
    src[1] = 2;
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 0x18, 9 + (2 * gBattleCommunication[1]), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

void BattleDestroyYesNoCursorAt(void)
{
    u16 src[2];

    src[0] = 32;
    src[1] = 32;
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 0x18, 9 + (2 * gBattleCommunication[1]), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

static void atkF3_trygivecaughtmonnick(void)
{
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
        BattlePutTextOnWindow(gText_BattleYesNoChoice, 0xE);
        ++gBattleCommunication[MULTIUSE_STATE];
        gBattleCommunication[CURSOR_POSITION] = 0;
        BattleCreateYesNoCursorAt();
        break;
    case 1:
        if (JOY_NEW(DPAD_UP) && gBattleCommunication[CURSOR_POSITION] != 0)
        {
            PlaySE(SE_SELECT);
            BattleDestroyYesNoCursorAt();
            gBattleCommunication[CURSOR_POSITION] = 0;
            BattleCreateYesNoCursorAt();
        }
        if (JOY_NEW(DPAD_DOWN) && gBattleCommunication[CURSOR_POSITION] == 0)
        {
            PlaySE(SE_SELECT);
            BattleDestroyYesNoCursorAt();
            gBattleCommunication[CURSOR_POSITION] = 1;
            BattleCreateYesNoCursorAt();
        }
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            if (gBattleCommunication[CURSOR_POSITION] == 0)
            {
                ++gBattleCommunication[MULTIUSE_STATE];
                BeginFastPaletteFade(3);
            }
            else
            {
                gBattleCommunication[MULTIUSE_STATE] = 4;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            gBattleCommunication[MULTIUSE_STATE] = 4;
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            GetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ BIT_SIDE]], MON_DATA_NICKNAME, gBattleStruct->caughtMonNick);
            FreeAllWindowBuffers();
            DoNamingScreen(NAMING_SCREEN_CAUGHT_MON, gBattleStruct->caughtMonNick,
                           GetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ BIT_SIDE]], MON_DATA_SPECIES),
                           GetMonGender(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ BIT_SIDE]]),
                           GetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ BIT_SIDE]], MON_DATA_PERSONALITY, NULL),
                           BattleMainCB2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 3:
        if (gMain.callback2 == BattleMainCB2 && !gPaletteFade.active)
        {
            SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ BIT_SIDE]], MON_DATA_NICKNAME, gBattleStruct->caughtMonNick);
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
        break;
    case 4:
        if (CalculatePlayerPartyCount() == PARTY_SIZE)
            gBattlescriptCurrInstr += 5;
        else
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        break;
    }
}

static void atkF4_subattackerhpbydmg(void)
{
    gBattleMons[gBattlerAttacker].hp -= gBattleMoveDamage;
    ++gBattlescriptCurrInstr;
}

static void atkF5_removeattackerstatus1(void)
{
    gBattleMons[gBattlerAttacker].status1 = 0;
    ++gBattlescriptCurrInstr;
}

static void atkF6_finishaction(void)
{
    gCurrentActionFuncId = B_ACTION_FINISHED;
}

static void atkF7_finishturn(void)
{
    gCurrentActionFuncId = B_ACTION_FINISHED;
    gCurrentTurnActionNumber = gBattlersCount;
}

static void atkF8_callasm(void)
{
	u32 ptr = ((u32) T1_READ_PTR(gBattlescriptCurrInstr + 1)) | 1;

	void (*func)(void) = (void (*)(void)) ptr; //The cast of doom
	func();

	gBattlescriptCurrInstr += 5; //Ideally this should be before the function is called, but too late now lol
}

//sethalfword RAM_OFFSET HWORD
static void atkF9_sethalfword(void)
{
	u16* offset = (u16*) T1_READ_PTR(gBattlescriptCurrInstr + 1);
	u16 hword = T1_READ_16(gBattlescriptCurrInstr + 5);

	if ((u32) offset % 2 == 0) //Only store is hword aligned
		*offset = hword;

	gBattlescriptCurrInstr += 7;
}

//setword RAM_OFFSET WORD
static void atkFA_setword(void)
{
	u32* offset = (u32*) T1_READ_PTR(gBattlescriptCurrInstr + 1);
	u32 word = T1_READ_32(gBattlescriptCurrInstr + 5);

	if ((u32) offset % 4 == 0) //Only store is word aligned
		*offset = word;

	gBattlescriptCurrInstr += 9;
}

//setspecialstatusbit BANK
static void atkFB_setspecialstatusbit(void)
{
    u32 status3;
	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	status3 = T1_READ_32(gBattlescriptCurrInstr + 2);

	gStatuses3[gActiveBattler] |= status3;

	gBattlescriptCurrInstr += 6;
}

static void atkFC_clearspecialstatusbit(void)
{
    u32 status3;

	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	status3 = T1_READ_32(gBattlescriptCurrInstr + 2);

	gStatuses3[gActiveBattler] &= ~(status3);

	gBattlescriptCurrInstr += 6;
}

//jumpifabilitypresenttargetfield ABILITY ROM_OFFSET
static void atkFD_jumpifabilitypresenttargetfield(void)
{
	ability_t ability = gBattlescriptCurrInstr[1];
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (ABILITY(gBattlerTarget) == ability)
		gBattleScripting.battler = gBattlerTarget;
	else if (ABILITY(PARTNER(gBattlerTarget)) == ability)
		gBattleScripting.battler = PARTNER(gBattlerTarget);
	else
	{
		gBattlescriptCurrInstr += 6;
		return;
	}

	gLastUsedAbility = ability;
	gBattlescriptCurrInstr = ptr;
}

//prefaintmoveendeffects ARG NULL_BYTE
static void atkFE_prefaintmoveendeffects(void)
{
	u32 effect = FALSE;
	u8 arg1 = gBattlescriptCurrInstr[1];

    if (gBattleControllerExecFlags) return;

	do
	{
		switch (gBattleStruct->preFaintEffectsState)
		{
		case FAINT_SET_UP: //For Emergency Exit to use later
			if (gBattleStruct->MultiHitOn)
				gBattleStruct->turnDamageTaken[gBattlerTarget] += gHpDealt; //Total up damage taken
			else
				gBattleStruct->turnDamageTaken[gBattlerTarget] = gHpDealt;

			gBattleStruct->totalDamageGiven += gHpDealt;
			gBattleStruct->ResultFlags[gBattlerTarget] = gMoveResultFlags;
			gBattleStruct->preFaintEffectsState++;
			break;

		case FAINT_ATTACKER_ABILITIES:
			if (arg1 != ARG_IN_FUTURE_ATTACK
			&& TOOK_DAMAGE(gBattlerTarget)
			&& MOVE_HAD_EFFECT
			&& gBattleMons[gBattlerTarget].hp
			&& !MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, gBattlerTarget))
			{
				switch (ABILITY(gBattlerAttacker)) {
					case ABILITY_STENCH: //Check for Stench is taken care of in King's Rock check
						if (Random() % 100 < 10
						&& gCurrentTurnActionNumber < GetBattlerTurnOrderNum(gBattlerTarget)) //Attacker moved before target
						{
							gBattleMons[gBattlerTarget].status2 |= STATUS2_FLINCHED;
						}
						break;

					case ABILITY_POISONTOUCH:
						if (CheckContact(gCurrentMove, gBattlerAttacker)
						&& ABILITY(gBattlerTarget) != ABILITY_SHIELD_DUST
						&& CanBePoisoned(gBattlerTarget, gBattlerAttacker, TRUE)
						&& Random() % 100 < 30)
						{
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_PoisonTouch;
							effect = TRUE;
						}
				}
			}
			gBattleStruct->preFaintEffectsState++;
			break;

		case FAINT_ADVERSE_PROTECTION:
			if (gProtectStructs[gBattlerTarget].kingsshield_damage)
			{
				gProtectStructs[gBattlerTarget].kingsshield_damage = 0;

				if (BATTLER_ALIVE(gBattlerAttacker) && STAT_CAN_FALL(gBattlerAttacker, STAT_ATK))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_KingsShield;
					effect = TRUE;
					break;
				}
			}

			if (gProtectStructs[gBattlerTarget].spikyshield_damage)
			{
				gProtectStructs[gBattlerTarget].spikyshield_damage = 0;

				if (BATTLER_ALIVE(gBattlerAttacker) && ABILITY(gBattlerAttacker) != ABILITY_MAGICGUARD)
				{
					gBattleMoveDamage = MathMax(1, GetBaseMaxHP(gBattlerAttacker) / 8);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_SpikyShield;
					effect = TRUE;
					break;
				}
			}
	
			if (gProtectStructs[gBattlerTarget].banefulbunker_damage)
			{
				gProtectStructs[gBattlerTarget].banefulbunker_damage = 0;

				if (BATTLER_ALIVE(gBattlerAttacker) && CanBePoisoned(gBattlerAttacker, gBattlerTarget, TRUE)) //Target poisons Attacker
				{
					gBattleMons[gBattlerAttacker].status1 = STATUS1_POISON;
					gEffectBattler = gActiveBattler = gBattlerAttacker;
					BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gBattlerAttacker].status1);
					MarkBattlerForControllerExec(gActiveBattler);

					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_BanefulBunker;
					effect = TRUE;
					break;
				}
			}

			if (gProtectStructs[gBattlerTarget].obstructDamage)
			{
				gProtectStructs[gBattlerTarget].obstructDamage = FALSE;

				if (BATTLER_ALIVE(gBattlerAttacker) && STAT_CAN_FALL(gBattlerAttacker, STAT_DEF))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_ObstructStatDecrement;
					effect = TRUE;
					break;
				}
			}
	
			gBattleStruct->preFaintEffectsState++;
			break;

		case FAINT_RAGE: // rage check
			if (gBattleMons[gBattlerTarget].status2 & STATUS2_RAGE
			&& gBattleMons[gBattlerTarget].hp
			&& gBattlerAttacker != gBattlerTarget
			&& SIDE(gBattlerAttacker) != SIDE(gBattlerTarget)
			&& MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBattlerTarget)
			&& SPLIT(gCurrentMove) != SPLIT_STATUS
			&& STAT_CAN_RISE(gBattlerTarget, STAT_ATK))
			{
				gBattleMons[gBattlerTarget].statStages[STAT_ATK - 1]++;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_RageIsBuilding;
				effect = TRUE;
			}
			gBattleStruct->preFaintEffectsState++;
			break;

		case FAINT_SYNCHRONIZE_TARGET: // target synchronize
			if (gCurrentMove != MOVE_PSYCHOSHIFT || !MOVE_HAD_EFFECT) //The lazy way of taking care of Psycho Shift Status Transfer->Synchronize->Heal Status
			{
				if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBattlerTarget, 0, 0, 0))
					effect = TRUE;
			}
			gBattleStruct->preFaintEffectsState++;
			break;

		case FAINT_BEAK_BLAST_BURN:
			if (CheckContact(gCurrentMove, gBattlerAttacker)
			&& MOVE_HAD_EFFECT
			&& TOOK_DAMAGE(gBattlerTarget)
			&& gBattleStruct->BeakBlastByte & gBitTable[gBattlerTarget]
			&& CanBeBurned(gBattlerAttacker, TRUE))
			{
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_BeakBlastBurn;
				effect = TRUE;
			}

			gBattleStruct->preFaintEffectsState++;
			break;

		case FAINT_SYNCHRONIZE_ATTACKER: // attacker synchronize
		case FAINT_SYNCHRONIZE_ATTACKER_2:
			if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBattlerAttacker, 0, 0, 0))
				effect = TRUE;
			gBattleStruct->preFaintEffectsState++;
			break;

		case FAINT_MOVE_END_ABILITIES: // Such as abilities activating on contact(Poison Spore, Rough Skin, etc.).
			if (AbilityBattleEffects(ABILITYEFFECT_MOVE_END, gBattlerTarget, 0, 0, 0))
				effect = TRUE;
			gBattleStruct->preFaintEffectsState++;
			break;

		case FAINT_ITEM_EFFECTS_CONTACT_TARGET:
			if (gBattleMoves[gCurrentMove].effect != EFFECT_EAT_BERRY //Skip Berries for Pluck to eat
			|| gCurrentMove == MOVE_INCINERATE
			|| ITEM_POCKET(gBattlerTarget) != POCKET_BERRY_POUCH
			|| ITEM_EFFECT(gBattlerTarget) == HOLD_EFFECT_JABOCA_ROWAP_BERRY) //Only berries that activate before pluck
			{
				if (gBattleMoves[gCurrentMove].effect != EFFECT_KNOCK_OFF
				|| (ITEM_EFFECT(gBattlerTarget) != HOLD_EFFECT_KEE_BERRY && ITEM_EFFECT(gBattlerTarget) != HOLD_EFFECT_MARANGA_BERRY)) //Only contact items that don't activate first
				{
					if (ItemBattleEffects(ItemEffects_ContactTarget, gBattlerTarget, TRUE, FALSE))
						effect = TRUE;
				}
			}
			gBattleStruct->preFaintEffectsState++;
			break;

		case FAINT_COUNT:
			break;
		}
	} while (gBattleStruct->preFaintEffectsState != FAINT_COUNT && effect == FALSE);

	if (gBattleStruct->preFaintEffectsState == FAINT_COUNT && effect == FALSE)
		gBattlescriptCurrInstr += 3;
}

static void atkFF_callsecondarytable(void)
{
    u8 command;

	gBattlescriptCurrInstr += 1;
	command = gBattlescriptCurrInstr[0];

	gBattleScriptingCommandsTable2[command](); //The cast of doom
}

//////////////////////////////////////////////////////////////////////////

void UpdatePrimalAbility(void)
{
	*GetAbilityLocation(gBattlerAttacker) = GetMonAbility(GetBankPartyData(gBattlerAttacker));
}

void TryActivateTargetEndTurnItemEffect(void)
{
	if (ItemBattleEffects(ItemEffects_EndTurn, gBattlerTarget, TRUE, FALSE))
		gBattlescriptCurrInstr -= 5;
}

void TransferLastUsedItem(void)
{
	if (gBattleControllerExecFlags)
		gBattlescriptCurrInstr -= 5;
	else
	{
		gActiveBattler = 0;
		EmitDataTransfer(0, &gLastUsedItem, 2, &gLastUsedItem);
		MarkBattlerForControllerExec(gActiveBattler);
	}
}

void ClearDoingPluckItemEffect(void)
{
	gBattleStruct->doingPluckItemEffect = FALSE;
}

void PluckBerryEat(void)
{
	gBattlescriptCurrInstr += 5;
    
	if (ItemBattleEffects(ItemEffects_EndTurn, gBattlerAttacker, TRUE, TRUE))
		gBattleStruct->doingPluckItemEffect = TRUE;
	else if (ItemBattleEffects(ItemEffects_ContactTarget, gBattlerAttacker, TRUE, TRUE))
		gBattleStruct->doingPluckItemEffect = TRUE;

	gBattlescriptCurrInstr -= 5;
}

void TransferTerrainData(void)
{
	if (gBattleControllerExecFlags)
		gBattlescriptCurrInstr -= 5;
	else
	{
		gActiveBattler = 0;
		EmitDataTransfer(0, &gTerrainType, 1, &gTerrainType);
		MarkBattlerForControllerExec(gActiveBattler);
	}
}

void SeedRoomServiceLooper(void)
{
	for (; *gSeedHelper < gBattlersCount; ++*gSeedHelper)
	{
		u8 bank = gBattlerByTurnOrder[*gSeedHelper];

		if (ITEM_EFFECT(bank) == HOLD_EFFECT_SEEDS || ITEM_EFFECT(bank) == HOLD_EFFECT_ROOM_SERVICE)
		{
			if (ItemBattleEffects(ItemEffects_SwitchIn, bank, TRUE , FALSE))
			{
				gBattlescriptCurrInstr -= 5;
				return;
			}
		}
	}
}

const u8* TryActivateMimicryForBank(u8 bank)
{
	u8 monType;

	switch (gTerrainType) {
		case ELECTRIC_TERRAIN:
			monType = TYPE_ELECTRIC;
			break;
		case GRASSY_TERRAIN:
			monType = TYPE_GRASS;
			break;
		case MISTY_TERRAIN:
			monType = TYPE_FAIRY;
			break;
		case PSYCHIC_TERRAIN:
			monType = TYPE_PSYCHIC;
			break;
		default:
			monType = 0xFF;
	}

	if (ABILITY(bank) == ABILITY_MIMICRY)
	{
		struct Pokemon* mon = GetBankPartyData(bank);

		if (monType == 0xFF)
		{
			u8 type1 = GetMonType(mon, 0);
			u8 type2 = GetMonType(mon, 1);

			if (gBattleMons[bank].type1 != type1 || gBattleMons[bank].type2 != type2
			||  !IS_BLANK_TYPE(gBattleMons[bank].type3))
			{
				gBattleMons[bank].type1 = type1;
				gBattleMons[bank].type2 = type2;
				gBattleMons[bank].type3 = TYPE_BLANK;

				gBattleScripting.battler = bank;
				return BattleScript_MimicryReturnedToNormal;
			}
		}
		else
		{
			if (gBattleMons[bank].type1 != monType || gBattleMons[bank].type2 != monType
			|| !IS_BLANK_TYPE(gBattleMons[bank].type3))
			{
				gBattleScripting.battler = bank;
				SET_BATTLER_TYPE(bank, monType);
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, monType);
				return BattleScript_MimicryTransformed;
			}
		}
	}

	return NULL;
}

void TryActivateMimicry(void)
{
	u8 i;
    const u8* script;

	for (i = 0; i < gBattlersCount; ++i)
	{
		script = TryActivateMimicryForBank(i);
		if (script != NULL)
		{
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = script - 5;
			break;
		}
	}
}

void UndoAbilityEffectsForNeutralizingGas(void)
{
    int i;

	gBattleStringLoader = NULL;

	if (gBattleWeather & WEATHER_RAIN_PRIMAL)
		gBattleStringLoader = PrimalRainEndString;
	else if (gBattleWeather & WEATHER_SUN_PRIMAL)
		gBattleStringLoader = PrimalSunEndString;
	else if (gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL)
		gBattleStringLoader = PrimalAirCurrentEndString;

	if (gBattleStringLoader != NULL)
	{
		gBattleWeather = 0;
		gWishFutureKnock.weatherDuration = 0;
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_PrimalWeatherEnd - 5;
		return;
	}

	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gStatuses3[i] & STATUS3_ILLUSION)
		{
			gBattleScripting.battler = i;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_TryRemoveIllusion - 5;
			break;
		}
	}
}

void HarvestActivateBerry(void)
{
	gBattlescriptCurrInstr += 5;
	if (ItemBattleEffects(ItemEffects_EndTurn, gBattleScripting.battler, TRUE, FALSE))
		gBattlescriptCurrInstr -= 5;
}

void MakeScriptingBankInvisible(void)
{
	if (gBattleControllerExecFlags) return;

	gActiveBattler = gBattleScripting.battler;
	BtlController_EmitSpriteInvisibility(0, TRUE);
	MarkBattlerForControllerExec(gActiveBattler);
}

void MakeSwitchingBankInvisible(void)
{
	if (gBattleControllerExecFlags) return;

	gActiveBattler = gBankSwitching;
	BtlController_EmitSpriteInvisibility(0, TRUE);
	MarkBattlerForControllerExec(gActiveBattler);
}

void SetLaserFocusTimer(void)
{
	if (!IsLaserFocused(gBattlerAttacker))
		gBattleStruct->LaserFocusTimers[gBattlerAttacker] = 2;
}

void SetHealBlockTimer(void)
{
	if (!IsHealBlocked(gBattlerTarget))
		gBattleStruct->HealBlockTimers[gBattlerTarget] = 5;
}

void SetThroatChopTimer(void)
{
	if (!CantUseSoundMoves(gBattlerTarget))
		gBattleStruct->ThroatChopTimers[gBattlerTarget] = 2;
}

void SetNoMoreMovingThisTurnSwitchingBank(void)
{
	gBattleStruct->NoMoreMovingThisTurn |= gBitTable[gBankSwitching];
}

void TransferIllusionBroken(void)
{
	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	gActiveBattler = gBattleScripting.battler;
	EmitDataTransfer(0, &gStatuses3[gActiveBattler], 4, &gStatuses3[gActiveBattler]);
	MarkBattlerForControllerExec(gActiveBattler);
}

void ActivatePerishBody(void)
{
	bool8 activated = FALSE;

	//Perish Body only works if the attacker doesn't already have a perish count
	if (!(gStatuses3[gBattlerAttacker] & STATUS3_PERISH_SONG))
	{
		activated = TRUE;
		gStatuses3[gBattlerAttacker] |= STATUS3_PERISH_SONG;
		gDisableStructs[gBattlerAttacker].perishSongTimer = 3;
		gDisableStructs[gBattlerAttacker].perishSongTimerStartValue = 3;

		if (!(gStatuses3[gBattlerTarget] & STATUS3_PERISH_SONG))
		{
			gStatuses3[gBattlerTarget] |= STATUS3_PERISH_SONG;
			gDisableStructs[gBattlerTarget].perishSongTimer = 3;
			gDisableStructs[gBattlerTarget].perishSongTimerStartValue = 3;
		}
	}

	if (!activated)
		gBattlescriptCurrInstr = BattleScript_PerishBodyReturn - 5;
}

void CycleScriptingBankHealthBetween0And1(void)
{
	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	gActiveBattler = gBattleScripting.battler;

	if (!gBattleMons[gActiveBattler].hp)
		gBattleMons[gActiveBattler].hp = 1;
	else
		gBattleMons[gActiveBattler].hp = 0;

	BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
	MarkBattlerForControllerExec(gActiveBattler);
}

void RestoreBanksFromSynchronize(void)
{
	gBattlerAttacker = gBattleStruct->backupSynchronizeBanks[0];
	gBattlerTarget = gBattleStruct->backupSynchronizeBanks[1];
};

void HealTargetFunc(void)
{
	u16 maxHP = gBattleMons[gBattlerTarget].maxHP;

	if (BATTLER_MAX_HP(gBattlerTarget)) {
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
		return;
	}

	gBattleMoveDamage = 0;

	switch (gCurrentMove) {
		case MOVE_HEALPULSE:
			if (ABILITY(gBattlerTarget) == ABILITY_MEGALAUNCHER)
				gBattleMoveDamage = (maxHP * 75 / 100);
			else
				gBattleMoveDamage = maxHP / 2;
			break;

		case MOVE_FLORALHEALING:
			if (gTerrainType == GRASSY_TERRAIN)
				gBattleMoveDamage = (maxHP * 2 / 3);
			else
				gBattleMoveDamage = maxHP / 2;
			break;

		case MOVE_POLLENPUFF:
			gBattleMoveDamage = maxHP / 2;
			break;
	}

	gBattleMoveDamage = MathMax(gBattleMoveDamage, 1);

	gBattleMoveDamage *= -1;
}

void SetBattleScriptingBankForPartnerAbilityNoStatLoss(void)
{
	gBattleScripting.battler = PARTNER(gBattleScripting.battler);
}

void ChooseTargetForMirrorArmorStickyWeb(void)
{
	u8 foe = FOE(gBattlerTarget);
	gBattlerAttacker = gBattlerTarget;
	PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_SPEED); //Just in case speed isn't lowered

	if (!IS_DOUBLE_BATTLE)
	{
		if (BATTLER_ALIVE(foe)
		&& !IS_BEHIND_SUBSTITUTE(foe))
			gBattlerAttacker = foe;
	}
	else //Pick target that's alive and not behind substitute
	{
		if (BATTLER_ALIVE(foe)
		&& !IS_BEHIND_SUBSTITUTE(foe))
			gBattlerAttacker = foe;
		else if (BATTLER_ALIVE(PARTNER(foe))
		&& !IS_BEHIND_SUBSTITUTE(PARTNER(foe)))
			gBattlerAttacker = PARTNER(foe);
	}
}

void FinishTurn(void)
{
	gCurrentActionFuncId = B_ACTION_FINISHED;
	gCurrentTurnActionNumber = gBattlersCount; //Taken from Pokeem, not a mistake
}

//For Benjamin Butterfree
void RestoreEffectBankHPStatsAndRemoveBackupSpecies(void)
{
	u32 i, j;

    //Update Moveset
	struct Pokemon* mon = GetBankPartyData(gEffectBattler);
	bool8 canLearnMove[MAX_MON_MOVES] = {FALSE};
	u16 moves[MAX_LEARNABLE_MOVES + EGG_MOVES_ARRAY_COUNT + EGG_MOVES_ARRAY_COUNT + NUM_TMS + NUM_HMS + (LAST_TOTAL_TUTOR_NUM + 1)] = {MOVE_NONE};
	u16 numMoves = BuildLearnableMoveset(mon, moves);

    u16 newMoves[MAX_MON_MOVES] = {0};
	u16 newPP[MAX_MON_MOVES] = {0};
    u8 counter = 0;
    u8 originalMovePos;

	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		u16 move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
		if (move == MOVE_NONE)
			continue;

		for (j = 0; j < numMoves; ++j)
		{
			if (moves[j] == move)
				break;
		}

		if (j < numMoves)
			canLearnMove[i] = TRUE;
	}

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		if (canLearnMove[i])
		{
			newMoves[counter] = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
			newPP[counter++] = GetMonData(mon, MON_DATA_PP1 + i, NULL);
		}
	}

	for (i = 0; i < MAX_MON_MOVES; ++i)
	{
		SetMonData(mon, MON_DATA_MOVE1 + i, &newMoves[i]); //Don't care about Emit as this isn't link compatible anyways
		SetMonData(mon, MON_DATA_PP1 + i, &newPP[i]);
		gBattleMons[gEffectBattler].moves[i] = newMoves[i];
		gBattleMons[gEffectBattler].pp[i] = newPP[i];
	}

	//Check if chosen move is still in moveset
	originalMovePos = FindMovePositionInMoveset(gChosenMoveByBattler[gEffectBattler], gEffectBattler);
	if (gChosenMoveByBattler[gEffectBattler] != MOVE_NONE && originalMovePos < MAX_MON_MOVES)
	{
		gBattleStruct->chosenMovePositions[gEffectBattler] = originalMovePos;
		gMoveSelectionCursor[gEffectBattler] = originalMovePos;
	}
	else
	{
		gBattleStruct->devolveForgotMove |= gBitTable[gEffectBattler]; //Can't use move anymore
		gMoveSelectionCursor[gEffectBattler] = 0; //Reset selection so can't select null move
	}

	gActiveBattler = gEffectBattler;
	GetBankPartyData(gActiveBattler)->box.backupSpecies = SPECIES_NONE; //There's no going back from a Benjamin Butterfree

	//Reset all stats
	for (i = STAT_STAGE_ATK; i < BATTLE_STATS_NO; ++i)
	{
		STAT_STAGE(gActiveBattler, i) = 6;
	}

	gBattleMons[gActiveBattler].status2 &= ~(STATUS2_CONFUSION);
	gStatuses3[gActiveBattler] &= ~(STATUS3_INTIMIDATE_POKES);
	gBattleMons[gActiveBattler].hp = gBattleMons[gActiveBattler].maxHP;
	BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
	MarkBattlerForControllerExec(gActiveBattler);
}

void ClearAttackerDidDamageOnce(void)
{
	gBattleStruct->AttackerDidDamageAtLeastOnce = 0;
}

void SetTargetPartner(void)
{
	gBattlerTarget = PARTNER(gBattlerAttacker);
}

void SetTargetFoePartner(void)
{
	gBattlerTarget = PARTNER(gBattlerTarget);
}

void SetAttackerPartner(void)
{
	gBattlerAttacker = PARTNER(gBattlerAttacker);
}

bool8 CheckCraftyShield(u8 bank)
{
	if (gSideStatuses[SIDE(bank)] & SIDE_STATUS_CRAFTY_SHIELD)
		return TRUE;

	return FALSE;
}

extern const u8 BS_MOVE_FAINT[];

void LiftProtectTarget(void)
{
	if (!LiftProtect(gBattlerTarget))
		gBattlescriptCurrInstr = BS_MOVE_FAINT - 0x5;
}

void IncreaseNimbleCounter(void)
{
	if (gBattleStruct->NimbleCounters[gBattlerAttacker] != 0xFF)
		gBattleStruct->NimbleCounters[gBattlerAttacker] += 1;
}

void ModifyGrowthInSun(void)
{
	if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(gBattlerAttacker) != HOLD_EFFECT_UTILITY_UMBRELLA)
		gBattleScripting.statChanger += INCREASE_1;
}

void AcupressureFunc(void)
{
    u8 stat;
	if (gBattlerAttacker != gBattlerTarget)
	{
		if (MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, gBattlerTarget) || CheckCraftyShield(gBattlerTarget))
		{
			gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			return;
		}
	}

	if (StatsMaxed(gBattlerTarget) && ABILITY(gBattlerTarget) != ABILITY_CONTRARY)
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
		return;
	}
	else if (StatsMinned(gBattlerTarget) && ABILITY(gBattlerTarget) == ABILITY_CONTRARY)
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
		return;
	}

	do
	{
		stat = (Random() % (BATTLE_STATS_NO - 1)) + 1;
	} while (gBattleMons[gActiveBattler].statStages[stat - 1] == 12);

	SET_STATCHANGER(stat, 2, FALSE);
}


void SetStatSwapSplit(void)
{
	u8 bankAtk = gBattlerAttacker;
	u8 bankDef = gBattlerTarget;
	u32 temp, i;

	switch (gCurrentMove) {
		case MOVE_POWERTRICK:
			temp = gBattleMons[bankAtk].attack;
			gBattleMons[bankAtk].attack = gBattleMons[bankAtk].defense;
			gBattleMons[bankAtk].defense = temp;

			gStatuses3[bankAtk] ^= STATUS3_POWER_TRICK;

			gBattleStringLoader = PowerTrickString;
			break;

		case MOVE_POWERSWAP:
        {
			u8 atkAtkBuff = gBattleMons[bankAtk].statStages[STAT_STAGE_ATK-1];
			u8 atkSpAtkBuff = gBattleMons[bankAtk].statStages[STAT_STAGE_SPATK-1];
			gBattleMons[bankAtk].statStages[STAT_STAGE_ATK-1] = gBattleMons[bankDef].statStages[STAT_STAGE_ATK-1];
			gBattleMons[bankAtk].statStages[STAT_STAGE_SPATK-1] = gBattleMons[bankDef].statStages[STAT_STAGE_SPATK-1];
			gBattleMons[bankDef].statStages[STAT_STAGE_ATK-1] = atkAtkBuff;
			gBattleMons[bankDef].statStages[STAT_STAGE_SPATK-1] = atkSpAtkBuff;

			gBattleStringLoader = PowerSwapString;
        }
			break;

		case MOVE_GUARDSWAP:
        {
			u8 atkDefBuff = gBattleMons[bankAtk].statStages[STAT_STAGE_DEF-1];
			u8 atkSpDefBuff = gBattleMons[bankAtk].statStages[STAT_STAGE_SPDEF-1];
			gBattleMons[bankAtk].statStages[STAT_STAGE_DEF-1] = gBattleMons[bankDef].statStages[STAT_STAGE_DEF-1];
			gBattleMons[bankAtk].statStages[STAT_STAGE_SPDEF-1] = gBattleMons[bankDef].statStages[STAT_STAGE_SPDEF-1];
			gBattleMons[bankDef].statStages[STAT_STAGE_DEF-1] = atkDefBuff;
			gBattleMons[bankDef].statStages[STAT_STAGE_SPDEF-1] = atkSpDefBuff;

			gBattleStringLoader = GuardSwapString;
        }
			break;

		case MOVE_SPEEDSWAP:
			temp = gBattleMons[bankAtk].speed;
			gBattleMons[bankAtk].speed = gBattleMons[bankDef].speed;
			gBattleMons[bankDef].speed = temp;

			gBattleStringLoader = SpeedSwapString;
			break;

		case MOVE_HEARTSWAP:
			for (i = 0; i < BATTLE_STATS_NO-1; ++i)
			{
				temp = gBattleMons[bankAtk].statStages[i];
				gBattleMons[bankAtk].statStages[i] = gBattleMons[bankDef].statStages[i];
				gBattleMons[bankDef].statStages[i] = temp;
			}

			gBattleStringLoader = HeartSwapString;
			break;

		case MOVE_POWERSPLIT:
        {
			u16 newAtk = (gBattleMons[bankAtk].attack + gBattleMons[bankDef].attack) / 2;
			u16 newSpAtk = (gBattleMons[bankAtk].spAttack + gBattleMons[bankDef].spAttack) / 2;

			gBattleMons[bankAtk].attack = MathMax(1, newAtk);
			gBattleMons[bankAtk].spAttack = MathMax(1, newSpAtk);
			gBattleMons[bankDef].attack = MathMax(1, newAtk);
			gBattleMons[bankDef].spAttack = MathMax(1, newSpAtk);

			gBattleStringLoader = PowerSplitString;
        }
			break;

		case MOVE_GUARDSPLIT:
        {
			u16 newDef = (gBattleMons[bankAtk].defense + gBattleMons[bankDef].defense) / 2;
			u16 newSpDef = (gBattleMons[bankAtk].spDefense + gBattleMons[bankDef].spDefense) / 2;

			gBattleMons[bankAtk].defense = MathMax(1, newDef);
			gBattleMons[bankAtk].spDefense = MathMax(1, newSpDef);
			gBattleMons[bankDef].defense = MathMax(1, newDef);
			gBattleMons[bankDef].spDefense = MathMax(1, newSpDef);

			gBattleStringLoader = GuardSplitString;
        }
	}
}

void ResetTargetStats(void)
{
    int i;
	for (i = 0; i < BATTLE_STATS_NO-1; ++i)
		gBattleMons[gBattlerTarget].statStages[i] = 6;
}

void ToggleSpectralThiefByte(void)
{
	gBattleStruct->SpectralThiefActive ^= TRUE;
}

void CheeckPouchFunc(void)
{
	u8 bank = gBattleScripting.battler;
	if (ABILITY(bank) == ABILITY_CHEEKPOUCH && !gBattleStruct->HealBlockTimers[bank]) { //Berry check should already be done
		gBattleMoveDamage = MathMax(1, (gBattleMons[bank].maxHP / 3));
		gBattleMoveDamage *= -1;
		gFormCounter = TRUE;
	}
	else
		gFormCounter = FALSE;
}

void MoldBreakerRemoveAbilitiesOnForceSwitchIn(void)
{
	u8 bank;
	if (gForceSwitchHelper == 2)
		bank = gBattleScripting.battler;
	else
		bank = gBattlerAttacker;

	if (ABILITY(bank) == ABILITY_MOLDBREAKER
	||  ABILITY(bank) == ABILITY_TURBOBLAZE
	||  ABILITY(bank) == ABILITY_TERAVOLT)
	{
		if (gMoldBreakerIgnoredAbilities[ABILITY(gBankSwitching)])
		{
			gBattleStruct->DisabledMoldBreakerAbilities[gBankSwitching] = gBattleMons[gBankSwitching].ability;
			gBattleMons[gBankSwitching].ability = 0;
		}
	}
}


void MoldBreakerRestoreAbilitiesOnForceSwitchIn(void)
{
	if (gBattleStruct->DisabledMoldBreakerAbilities[gBankSwitching])
	{
		gBattleMons[gBankSwitching].ability = gBattleStruct->DisabledMoldBreakerAbilities[gBankSwitching];
		gBattleStruct->DisabledMoldBreakerAbilities[gBankSwitching] = 0;
	}
}

void TrainerSlideOut(void)
{
	gActiveBattler = B_POSITION_OPPONENT_LEFT;
	BtlController_EmitTrainerSlideBack(0);
	MarkBattlerForControllerExec(gActiveBattler);
}

void SetAuroraVeil(void)
{
	if (!gBattleStruct->AuroraVeilTimers[SIDE(gBattlerAttacker)]
	&& gBattleWeather & WEATHER_HAIL_ANY
	&& WEATHER_HAS_EFFECT)
	{
		if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_LIGHT_CLAY)
			gBattleStruct->AuroraVeilTimers[SIDE(gBattlerAttacker)] = 8;
		else
			gBattleStruct->AuroraVeilTimers[SIDE(gBattlerAttacker)] = 5;

		gBattleStringLoader = gText_SetAuroraVeil;
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	}
	else
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	}
}

void MetalBurstDamageCalculator(void)
{
	u8 atkSide = SIDE(gBattlerAttacker);
	u8 defSide = SIDE(gProtectStructs[gBattlerAttacker].physicalBattlerId);
	u8 defSide2 = SIDE(gProtectStructs[gBattlerAttacker].specialBattlerId);

    if (GetBattlerTurnOrderNum(gBattlerTarget) > gCurrentTurnActionNumber)
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;

	if (gProtectStructs[gBattlerAttacker].physicalDmg && atkSide != defSide && gBattleMons[gProtectStructs[gBattlerAttacker].physicalBattlerId].hp)
	{
		gBattleMoveDamage = (gProtectStructs[gBattlerAttacker].physicalDmg * 15) / 10;

		if (IsMoveRedirectedByFollowMe(gCurrentMove, gBattlerAttacker, defSide))
			gBattlerTarget = gSideTimers[defSide].followmeTarget;
		else
			gBattlerTarget = gProtectStructs[gBattlerAttacker].physicalBattlerId;
	}
	else if (gProtectStructs[gBattlerAttacker].specialDmg && atkSide != defSide2 && gBattleMons[gProtectStructs[gBattlerAttacker].specialBattlerId].hp)
	{
		gBattleMoveDamage = (gProtectStructs[gBattlerAttacker].specialDmg * 15) / 10;

		if (IsMoveRedirectedByFollowMe(gCurrentMove, gBattlerAttacker, defSide2))
			gBattlerTarget = gSideTimers[defSide2].followmeTarget;
		else
			gBattlerTarget = gProtectStructs[gBattlerAttacker].specialBattlerId;
	}
	else
	{
		gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
	}
}


void TryActivatePartnerSapSipper(void)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&& !(gAbsentBattlerFlags & gBitTable[PARTNER(gBattlerAttacker)])
	&& ABILITY(PARTNER(gBattlerAttacker)) == ABILITY_SAPSIPPER
	&& gBattleStruct->dynamicMoveType == TYPE_GRASS
	&& STAT_CAN_RISE(PARTNER(gBattlerAttacker), STAT_STAGE_ATK))
	{
		gBattleScripting.battler = gBattlerTarget = PARTNER(gBattlerAttacker);
		gBattleScripting.statChanger = INCREASE_1 | STAT_STAGE_ATK;
		gBattleScripting.animArg1 = 0xE + STAT_STAGE_ATK;
		gBattleScripting.animArg2 = 0;
		gLastUsedAbility = ABILITY(gBattlerTarget);
		BattleScriptPush(gBattlescriptCurrInstr + 5);
		gBattlescriptCurrInstr = BattleScript_SapSipperAromatherapy - 5;
	}
}

void RoundBSFunction(void)
{
	int i;
	u8 j = 0;
	u8 k = 0;
	u8 index = 0;
	u8 rounders[3] = {0xFF, 0xFF, 0xFF};
	u8 nonRounders[3] = {0xFF, 0xFF, 0xFF};

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
		for (i = 0; i < gBattlersCount; ++i) {
			if (gBattlerByTurnOrder[i] == gBattlerAttacker) {
				index = i + 1; //Index after attacker; index to start swapping data
				break;
			}
		}

		for (i = index; i < gBattlersCount; ++i) {
			if (gChosenMoveByBattler[gBattlerByTurnOrder[i]] == MOVE_ROUND)
				rounders[j++] = gBattlerByTurnOrder[i];
			else
				nonRounders[k++] = gBattlerByTurnOrder[i];
		}

		for (i = 0; rounders[i] != 0xFF && i < 3; ++i)
		{
			gBattlerByTurnOrder[index++] = rounders[i];
			gBattleStruct->quashed |= gBitTable[rounders[i]]; //So their turn order can't be changed
		}

		for (i = 0; nonRounders[i] != 0xFF && i < 3; ++i)
			gBattlerByTurnOrder[index++] = nonRounders[i];

	}
}

void SetRoundUsed(void)
{
	gBattleStruct->roundUsed = TRUE;
}

void EchoedVoiceFunc(void)
{
	switch (gBattleStruct->EchoedVoiceCounter) {
		case 0:
			gBattleStruct->EchoedVoiceCounter = 2;
			gBattleStruct->EchoedVoiceDamageScale = 0;
			break;
		case 2:
			break;
		default:
			gBattleStruct->EchoedVoiceCounter = 2;
			gBattleStruct->EchoedVoiceDamageScale = MathMin(10, gBattleStruct->EchoedVoiceDamageScale + 1);
	}
}

void DefogHelperFunc(void)
{
	if (gBattleStruct->AuroraVeilTimers[SIDE(gBattlerTarget)]
	|| gTerrainType != 0
	|| gSideStatuses[SIDE(gBattlerAttacker)] & SIDE_STATUS_SPIKES
	|| gSideStatuses[SIDE(gBattlerTarget)] & (SIDE_STATUS_SPIKES
										  | SIDE_STATUS_REFLECT
										  | SIDE_STATUS_LIGHTSCREEN
										  | SIDE_STATUS_SAFEGUARD
										  | SIDE_STATUS_MIST))
	{
		gBattlescriptCurrInstr = BattleScript_DefogAdditionalEffects - 5;
	}
}

void ClearBeakBlastBit(void)
{
	if (!(gBattleStruct->BeakBlastByte & gBitTable[gBattlerAttacker])) //Forced by Encore
		gBattlescriptCurrInstr = BattleScript_NoTargetMoveFailed - 5;
	else
		gBattleStruct->BeakBlastByte &= ~(gBitTable[gBattlerAttacker]);
}

void BestowItem(void)
{
	if (ITEM(gBattlerTarget) == 0
	&& ITEM(gBattlerAttacker) != 0
	&& CanTransferItem(SPECIES(gBattlerTarget), ITEM(gBattlerAttacker))
	&& CanTransferItem(SPECIES(gBattlerAttacker), ITEM(gBattlerAttacker)))
	{
		gLastUsedItem = gBattleMons[gBattlerTarget].item = ITEM(gBattlerAttacker);
		gBattleMons[gBattlerAttacker].item = 0;
		HandleUnburdenBoost(gBattlerAttacker); //Give attacker Unburden boost
		HandleUnburdenBoost(gBattlerTarget); //Remove target Unburden boost

		gActiveBattler = gBattlerAttacker;
		BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
		MarkBattlerForControllerExec(gActiveBattler);

		gActiveBattler = gBattlerTarget;
		BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
		MarkBattlerForControllerExec(gActiveBattler);
	}
	else
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
}

void BelchFunction(void)
{
	if (IsRaidBattle() && gBattlerAttacker == BANK_RAID_BOSS)
		return; //Raid bosses can always use Belch

	if (!(gBattleStruct->BelchCounters & gBitTable[gBattlerPartyIndexes[gBattlerAttacker]]))
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5 - 2;
}

s32 CalcStrengthSapHealAmount(u8 bankAtk, u8 bankDef)
{
	u16 attack = gBattleMons[bankDef].attack;
	APPLY_QUICK_STAT_MOD(attack, STAT_STAGE(bankDef, STAT_STAGE_ATK));
	attack = MathMax(1, attack);

	if (ITEM_EFFECT(bankAtk) == HOLD_EFFECT_BIG_ROOT)
		attack = (13 * attack) / 10;

	return attack * -1;
}

void StrengthSapFunc(void)
{
	gBattleMoveDamage = CalcStrengthSapHealAmount(gBattlerAttacker, gBattlerTarget);
}

void PlayAttackAnimationForExplosion(void)
{
    u8 multihit;
	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	if ((gHitMarker & HITMARKER_NO_ANIMATIONS))
		return;

	gActiveBattler = gBattlerAttacker;
	multihit = gMultiHitCounter;

	BtlController_EmitMoveAnimation(0, gCurrentMove, gBattleScripting.animTurn, gBattleMovePower, gBattleMoveDamage, gBattleMons[gBattlerAttacker].friendship, &gDisableStructs[gBattlerAttacker], multihit);
	MarkBattlerForControllerExec(gBattlerAttacker);
}

void CopycatFunc(void)
{
	if (gBattleStruct->LastUsedMove == 0
	|| gBattleStruct->LastUsedMove == 0xFFFF
	|| IsZMove(gBattleStruct->LastUsedMove)
	|| IsAnyMaxMove(gBattleStruct->LastUsedMove)
	|| CheckTableForMove(gBattleStruct->LastUsedMove, gCopycatBannedMoves))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 1 - 5;	//From PP Reduce
	}
	else
	{
		gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
		gCalledMove = gBattleStruct->LastUsedMove;
	}
}

void SetRoostFunc(void)
{
	if (gBattleMons[gBattlerAttacker].type1 == TYPE_FLYING
	&&  gBattleMons[gBattlerAttacker].type2 == TYPE_FLYING)
	{
		gBattleStruct->RoostCounter |= gBitTable[gBattlerAttacker];
		gBattleMons[gBattlerAttacker].type1 = TYPE_NORMAL;
		gBattleMons[gBattlerAttacker].type2 = TYPE_NORMAL;
	}
	else
	{
		if (gBattleMons[gBattlerAttacker].type1 == TYPE_FLYING)
			gBattleMons[gBattlerAttacker].type1 = TYPE_ROOSTLESS;

		if (gBattleMons[gBattlerAttacker].type2 == TYPE_FLYING)
			gBattleMons[gBattlerAttacker].type2 = TYPE_ROOSTLESS;
	}

	if (gBattleMons[gBattlerAttacker].type3 == TYPE_FLYING)
		gBattleMons[gBattlerAttacker].type3 = TYPE_ROOSTLESS;
}

void CaptivateFunc(void)
{
	struct Pokemon *monAttacker, *monTarget;
	u16 speciesAtk, speciesDef;
	u32 personalityAtk, personalityDef;

	if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
		monAttacker = &gPlayerParty[gBattlerPartyIndexes[gBattlerAttacker]];
	else
		monAttacker = &gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker]];

	if (SIDE(gBattlerTarget) == B_SIDE_PLAYER)
		monTarget = &gPlayerParty[gBattlerPartyIndexes[gBattlerTarget]];
	else
		monTarget = &gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]];

	speciesAtk = GetMonData(monAttacker, MON_DATA_SPECIES, 0);
	personalityAtk = GetMonData(monAttacker, MON_DATA_PERSONALITY, 0);

	speciesDef = GetMonData(monTarget, MON_DATA_SPECIES, 0);
	personalityDef = GetMonData(monTarget, MON_DATA_PERSONALITY, 0);

	if (GetGenderFromSpeciesAndPersonality(speciesAtk, personalityAtk) == GetGenderFromSpeciesAndPersonality(speciesDef, personalityDef)
	|| GetGenderFromSpeciesAndPersonality(speciesAtk, personalityAtk) == MON_GENDERLESS
	|| GetGenderFromSpeciesAndPersonality(speciesDef, personalityDef) == MON_GENDERLESS)
	{
		gBattlescriptCurrInstr = BattleScript_NotAffected - 5;
	}
}

void MeFirstFunc(void)
{
	u16 move = gChosenMoveByBattler[gBattlerTarget];

	if (SPLIT(move) == SPLIT_STATUS
	|| GetBattlerTurnOrderNum(gBattlerTarget) < gCurrentTurnActionNumber
	|| CheckTableForMove(move, gMeFirstBannedMoves)
	|| CheckTableForMove(move, gMovesThatCallOtherMoves))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5 - 1; //Start from PP Reduce
	}
	else
	{
		gBattleStruct->MeFirstByte = TRUE;
		gCalledMove = move;
	}

}

enum PledgeEffects
{
	Pledge_Swamp = 2,
	Pledge_SeaOfFire,
	Pledge_Rainbow,
};

void LoadPledgeScript(void)
{
	int i;
	int k = 0;

	u16 partnerMove = gChosenMoveByBattler[PARTNER(gBattlerAttacker)];

	if (gBattleStruct->PledgeHelper)
	{
		if ((gCurrentMove == MOVE_GRASSPLEDGE && partnerMove == MOVE_WATERPLEDGE)
		||  (partnerMove == MOVE_GRASSPLEDGE && gCurrentMove == MOVE_WATERPLEDGE))
		{
			gCurrentMove = MOVE_GRASSPLEDGE;
			gBattleStruct->PledgeHelper = Pledge_Swamp;
		}
		else if ((gCurrentMove == MOVE_GRASSPLEDGE && partnerMove == MOVE_FIREPLEDGE)
			  || (partnerMove == MOVE_GRASSPLEDGE && gCurrentMove == MOVE_FIREPLEDGE))
		{
			gCurrentMove = MOVE_FIREPLEDGE;
			gBattleStruct->PledgeHelper = Pledge_SeaOfFire;
		}
		else if ((gCurrentMove == MOVE_FIREPLEDGE && partnerMove == MOVE_WATERPLEDGE)
			  || (partnerMove == MOVE_FIREPLEDGE && gCurrentMove == MOVE_WATERPLEDGE))
		{
			gCurrentMove = MOVE_WATERPLEDGE;
			gBattleStruct->PledgeHelper = Pledge_Rainbow;
		}

		gBattlescriptCurrInstr = BattleScript_PledgeCombined - 5;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&& gBattleMons[PARTNER(gBattlerAttacker)].hp
	&& partnerMove != gCurrentMove
	&& gBattleMoves[partnerMove].effect == EFFECT_PLEDGE)
	{
		u8 index = 0;
		u8 newTurnOrder[] = {0xFF, 0xFF};

		for (i = 0; i < gBattlersCount; ++i)
		{
			if (gBattlerByTurnOrder[i] == gBattlerAttacker)
			{
				index = i + 1; //Index after attacker; index to start swapping data
				break;
			}
		}

		for (i = index; i < gBattlersCount; ++i) {
			if (gBattlerByTurnOrder[i] != PARTNER(gBattlerAttacker))
				newTurnOrder[k++] = gBattlerByTurnOrder[i];
		}

		gBattlerByTurnOrder[index++] = PARTNER(gBattlerAttacker);

		if (newTurnOrder[0] != 0xFF)
			gBattlerByTurnOrder[index++] = newTurnOrder[0];

		if (newTurnOrder[1] != 0xFF)
			gBattlerByTurnOrder[index] = newTurnOrder[1];

		gBattleStruct->PledgeHelper = TRUE;
		gBattleScripting.battler = PARTNER(gBattlerAttacker);
		gBattlescriptCurrInstr = BattleScript_PledgeReady - 5;
	}
}

void SetPledgeEffect(void)
{
	switch (gBattleStruct->PledgeHelper) {
		case Pledge_Swamp:
			if (!BankSideHasSwamp(gBattlerTarget))
			{
				gBattleStruct->SwampTimers[SIDE(gBattlerTarget)] = 5;
				gBattleStringLoader = SwampString;
				//gBattleScripting.animArg1 = B_ANIM_SWAMP;
			}
			else
				gBattlescriptCurrInstr = BS_MOVE_FAINT - 5;
			break;

		case Pledge_SeaOfFire:
			if (!BankSideHasSeaOfFire(gBattlerTarget))
			{
				gBattleStruct->SeaOfFireTimers[SIDE(gBattlerTarget)] = 5;
				gBattleStringLoader = SeaOfFireString;
				gBattleScripting.animArg1 = B_ANIM_SEA_OF_FIRE;
			}
			else
				gBattlescriptCurrInstr = BS_MOVE_FAINT - 5;
			break;

		case Pledge_Rainbow:
			if (!BankSideHasRainbow(gBattlerTarget))
			{
				gBattleStruct->RainbowTimers[SIDE(gBattlerTarget)] = 5;
				gBattleStringLoader = RainbowString;
				//BattleScripting->animArg1 = B_ANIM_RAINBOW;
			}
			else
				gBattlescriptCurrInstr = BS_MOVE_FAINT - 5;
			break;

		default:
			gBattlescriptCurrInstr = BS_MOVE_FAINT - 5;
	}

	gBattleStruct->PledgeHelper = 0;
}

void DoFieldEffect(void)
{
	if (IsAnyMaxMove(gCurrentMove)
	&& gBattleMoves[gCurrentMove].z_move_effect == MAX_EFFECT_GRAVITY)
	{
		if (!IsGravityActive())
		{
            int i;
			for (i = 0; i < gBattlersCount; ++i)
			{
				if (!CheckGrounding(i))
					gBattleStruct->targetsToBringDown |= gBitTable[i];
			}

			gBattleStruct->GravityTimer = 5;
			gBattleStringLoader = GravitySetString;
		}
		return;
	}

	switch (gCurrentMove) {
		case MOVE_TRICKROOM:
			if (gBattleStruct->TrickRoomTimer > 0)
			{
				gBattleStruct->TrickRoomTimer = 0;
				gBattleStringLoader = TrickRoomEndString;
			}
			else if (!IsTrickRoomActive())
			{
				gBattleStruct->TrickRoomTimer = 5;
				gBattleStringLoader = TrickRoomSetString;
			}
			else
				 gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			break;

		case MOVE_WONDERROOM:
			if (gBattleStruct->WonderRoomTimer > 0)
			{
				gBattleStruct->WonderRoomTimer = 0;
				gBattleStringLoader = WonderRoomEndString;
			}
			else if (!IsWonderRoomActive())
			{
				gBattleStruct->WonderRoomTimer = 5;
				gBattleStringLoader = WonderRoomSetString;
			}
			else
				 gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			break;

		case MOVE_MAGICROOM:
			if (gBattleStruct->MagicRoomTimer)
			{
				gBattleStruct->MagicRoomTimer = 0;
				gBattleStringLoader = MagicRoomEndString;
			}
			else if (!IsMagicRoomActive())
			{
				gBattleStruct->MagicRoomTimer = 5;
				gBattleStringLoader = MagicRoomSetString;
			}
			else
				 gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			break;

		case MOVE_GRAVITY:
			if (gBattleStruct->GravityTimer > 0)
			{
				gBattleStruct->GravityTimer = 0;
				gBattleStringLoader = GravityEndString;
			}
			else if (!IsGravityActive())
			{
                int i;
				for (i = 0; i < gBattlersCount; ++i)
				{
					if (!CheckGrounding(i))
						gBattleStruct->targetsToBringDown |= gBitTable[i];
				}

				gBattleStruct->GravityTimer = 5;
				gBattleStringLoader = GravitySetString;
			}
			else
				 gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			break;

		case MOVE_IONDELUGE:
			if (!IsIonDelugeActive())
			{
				gBattleStruct->IonDelugeTimer = 1;
			}

			//Doesn't fail even if already Ion Deluge
			gBattleStringLoader = IonDelugeShowerString;
			break;
	}
}

//Continues until no marked mons are left in air
void BringDownMons(void)
{
    int i;
	for (i = 0; i < gBattlersCount; ++i)
	{
		if (gBattleStruct->targetsToBringDown & gBitTable[i])
		{
			if (gStatuses3[i] & (STATUS3_ON_AIR | STATUS3_SKY_DROP_ATTACKER))
			{
				gBattleStruct->NoMoreMovingThisTurn |= gBitTable[i];
				CancelMultiTurnMoves(i);
			}

			gBattleStruct->targetsToBringDown ^= gBitTable[i];
			gBattleStruct->TelekinesisTimers[i] = 0;
			gBattleStruct->MagnetRiseTimers[i] = 0;
			gStatuses3[i] &= ~(STATUS3_ON_AIR
							 | STATUS3_SKY_DROP_ATTACKER
							 | STATUS3_SKY_DROP_TARGET
							 | STATUS3_LEVITATING
							 | STATUS3_TELEKINESIS);

			gActiveBattler = i;
			BtlController_EmitSpriteInvisibility(0, FALSE);
			MarkBattlerForControllerExec(gActiveBattler);

			gBattleScripting.battler = i;
			if (gCurrentMove == MOVE_GRAVITY)
				gBattleStringLoader = NoMoreAirborneFromGravityString;
			else
				gBattleStringLoader = Bank0AWasBroughtDownString;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_PrintCustomString - 5;
			return;
		}
	}
}

u8 GetFlingPower(u16 item, u16 species, ability_t ability, u8 bank, bool8 partyCheck)
{
	u8 power = 0;
	u8 embargoTimer = (partyCheck) ? 0 : gBattleStruct->EmbargoTimers[bank];

	if (CanFling(item, species, ability, bank, embargoTimer))
	{
		power = gFlingTable[item].power;
		if (power == 0)
			power = 30; //Default power is 30
	}

	return power;
}

void TryFling(void)
{
	if (!CanFling(ITEM(gBattlerAttacker), SPECIES(gBattlerAttacker), ABILITY(gBattlerAttacker),
				  gBattlerAttacker, gBattleStruct->EmbargoTimers[gBattlerAttacker]))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
	}
	else
	{
		gLastUsedItem = ITEM(gBattlerAttacker);
		gBattleCommunication[MOVE_EFFECT_BYTE] = gFlingTable[gLastUsedItem].effect;
	}
}

void CheckTelekinesisFail(void)
{
	u16 species = gBattleMons[gBattlerTarget].species;

	if (gStatuses3[gBattlerTarget] & (STATUS3_TELEKINESIS | STATUS3_ROOTED | STATUS3_SMACKED_DOWN)
	||  IsGravityActive()
	||  ITEM_EFFECT(gBattlerTarget) == HOLD_EFFECT_IRON_BALL
	||  CheckTableForSpecies(species, gTelekinesisBanList))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5 - 2;
	}
}

void ChangeTargetTypeFunc(void)
{
	switch (gCurrentMove) {
		case MOVE_SOAK:
			if (ABILITY(gBattlerTarget) == ABILITY_MULTITYPE
			||  ABILITY(gBattlerTarget) == ABILITY_RKS_SYSTEM
			|| (gBattleMons[gBattlerTarget].type1 == TYPE_WATER &&
				gBattleMons[gBattlerTarget].type2 == TYPE_WATER &&
				gBattleMons[gBattlerTarget].type3 == TYPE_BLANK))
			{
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			}
			else
			{
				SET_BATTLER_TYPE(gBattlerTarget, TYPE_WATER);
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_WATER);
				gBattleStringLoader = TargetTransformedIntoType;
			}
			break;

		case MOVE_MAGICPOWDER:
			if (ABILITY(gBattlerTarget) == ABILITY_MULTITYPE
			||  ABILITY(gBattlerTarget) == ABILITY_RKS_SYSTEM
			|| (gBattleMons[gBattlerTarget].type1 == TYPE_PSYCHIC &&
				gBattleMons[gBattlerTarget].type2 == TYPE_PSYCHIC &&
				gBattleMons[gBattlerTarget].type3 == TYPE_BLANK))
			{
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			}
			else
			{
				SET_BATTLER_TYPE(gBattlerTarget, TYPE_PSYCHIC);
				PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_PSYCHIC);
				gBattleStringLoader = TargetTransformedIntoType;
			}
			break;

		case MOVE_TRICKORTREAT:
			if (IsOfType(gBattlerTarget, TYPE_GHOST))
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				if (gBattleMons[gBattlerTarget].type1 == gBattleMons[gBattlerTarget].type2)
					gBattleMons[gBattlerTarget].type2 = TYPE_GHOST;
				else
					gBattleMons[gBattlerTarget].type3 = TYPE_GHOST;

				PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_GHOST);
				gBattleStringLoader = ThirdTypeAddedString;
			}
			break;

		case MOVE_FORESTSCURSE:
			if (IsOfType(gBattlerTarget, TYPE_GRASS))
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				if (gBattleMons[gBattlerTarget].type1 == gBattleMons[gBattlerTarget].type2)
					gBattleMons[gBattlerTarget].type2 = TYPE_GRASS;
				else
					gBattleMons[gBattlerTarget].type3 = TYPE_GRASS;

				PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_GRASS);
				gBattleStringLoader = ThirdTypeAddedString;
			}
			break;
	}
}

void TopsyTurvyFunc(void)
{
	int i;

	for (i = 0; i < BATTLE_STATS_NO-1; ++i)
	{
		if (gBattleMons[gBattlerTarget].statStages[i] != 6)
			break;
	}

	if (i == BATTLE_STATS_NO-1)
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
	else
	{
		for (i = 0; i < BATTLE_STATS_NO-1; ++i)
			gBattleMons[gBattlerTarget].statStages[i] = ((gBattleMons[gBattlerTarget].statStages[i] - 6) * -1) + 6;
	}
}

void DoFairyLockHappyHourFunc(void)
{
	switch (gCurrentMove) {
		case MOVE_FAIRYLOCK:
			if (IsFairyLockActive())
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				gBattleStruct->FairyLockTimer = 2;
				gBattleStringLoader = FairyLockString;
			}
			break;

		case MOVE_HAPPYHOUR:
			if (!gBattleStruct->HappyHourByte)
			{
				gBattleStruct->HappyHourByte = TRUE;
				gBattleStruct->moneyMultiplier *= 2;
			}
			gBattleStringLoader = HappyHourString;
			break;

		case MOVE_CELEBRATE:
			gBattleStringLoader = CelebrateString;
			break;

		case MOVE_HOLDHANDS:
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
			&&  gBattleMons[PARTNER(gBattlerAttacker)].hp)
			{
				gBattleStringLoader = HoldHandsString;
			}
			else
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;

			break;
	}
}

bool8 TargetMovesRightAfterAttacker(void)
{
    int i;
	for (i = 1; i < gBattlersCount; ++i)
	{
		if (gBattlerByTurnOrder[i] == gBattlerTarget
		&&  gBattlerByTurnOrder[i - 1] == gBattlerAttacker)
			return TRUE;
	}
	return FALSE;
}

void AfterYouFunc(void)
{
	int i;

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&&  GetBattlerTurnOrderNum(gBattlerTarget) > gCurrentTurnActionNumber //Attacker moved before target
	&&  !TargetMovesRightAfterAttacker())
	{
		u8 index = 0;
		u8 k = 0;
		u8 newTurnOrder[] = {0xFF, 0xFF};

		for (i = 0; i < gBattlersCount; ++i)
		{
			if (gBattlerByTurnOrder[i] == gBattlerAttacker)
			{
				index = i + 1; //Index after attacker; index to start swapping data
				break;
			}
		}

		for (i = index; i < gBattlersCount; ++i)
		{
			if (gBattlerByTurnOrder[i] != gBattlerTarget)
				newTurnOrder[k++] = gBattlerByTurnOrder[i];
		}

		gBattlerByTurnOrder[index++] = gBattlerTarget;

		if (newTurnOrder[0] != 0xFF)
			gBattlerByTurnOrder[index++] = newTurnOrder[0];

		if (newTurnOrder[1] != 0xFF)
			gBattlerByTurnOrder[index] = newTurnOrder[1];
	}
	else
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
}

void QuashFunc(void)
{
	int i;

	u8 numPokemonOnField = CountAliveMonsInBattle(BATTLE_ALIVE_ATK_SIDE, gBattlerAttacker, gBattlerTarget)
						 + CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE, gBattlerAttacker, gBattlerTarget);

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&&  GetBattlerTurnOrderNum(gBattlerTarget) > gCurrentTurnActionNumber) //Attacker moved before target
	{
		int k = 0;
		u8 newTurnOrder[] = {0xFF, 0xFF, 0xFF, 0xFF};

		for (i = 0; i < numPokemonOnField; ++i)
		{
			if (gBattlerByTurnOrder[i] != gBattlerTarget)
				newTurnOrder[k++] = gBattlerByTurnOrder[i];
		}

		newTurnOrder[k] = gBattlerTarget;

		for (i = 0; i < numPokemonOnField; ++i)
		{
			if (newTurnOrder[i] != 0xFF)
				gBattlerByTurnOrder[i] = newTurnOrder[i];
		}

		gBattleStruct->quashed |= gBitTable[gBattlerTarget];
	}
	else
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
}

void TryExecuteInstruct(void)
{
	u16 move = gLastPrintedMoves[gBattlerTarget];

	if (CheckTableForMove(move, gInstructBannedMoves)
	||  CheckTableForMove(move, gMovesThatRequireRecharging)
	||  CheckTableForMove(move, gMovesThatCallOtherMoves)
	|| IsZMove(move)
	|| IsAnyMaxMove(move)
	|| IsDynamaxed(gBattlerTarget)
	|| (gLockedMoves[gBattlerTarget] != 0 && gLockedMoves[gBattlerTarget] != 0xFFFF)
	|| gBattleMons[gBattlerTarget].status2 & STATUS2_MULTIPLETURNS
	|| FindMovePositionInMoveset(move, gBattlerTarget) == 4 //No longer knows the move
	|| gBattleMons[gBattlerTarget].pp[FindMovePositionInMoveset(move, gBattlerTarget)] == 0
	|| (GetBattlerTurnOrderNum(gBattlerTarget) > gCurrentTurnActionNumber && (gChosenMoveByBattler[gBattlerTarget] == MOVE_FOCUSPUNCH
													 || gChosenMoveByBattler[gBattlerTarget] == MOVE_BEAKBLAST
													 || gChosenMoveByBattler[gBattlerTarget] == MOVE_SHELLTRAP)))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
		return;
	}
}

void InitiateInstruct(void)
{
	gCurrentMove = gLastPrintedMoves[gBattlerTarget];
	gCurrMovePos = FindMovePositionInMoveset(gCurrentMove, gBattlerTarget);
	gChosenMovePos = FindMovePositionInMoveset(gCurrentMove, gBattlerTarget);

	gBattlerAttacker = gBattlerTarget;

	if (gAbsentBattlerFlags & gBitTable[gBattleStruct->lastTargeted[gBattlerAttacker]])
		gBattlerTarget = GetMoveTarget(gCurrentMove, FALSE);
	else
		gBattlerTarget = gBattleStruct->lastTargeted[gBattlerAttacker];

	gLastMoves[gBattlerAttacker] = gChosenMove;
	gLastResultingMoves[gBattlerAttacker] = gCurrentMove;
	gLastPrintedMoves[gBattlerAttacker] = gCurrentMove;

	gBattleScripting.animTargetsHit = 0;
	gBattleStruct->atkCancellerTracker = 0;
	MoveValuesCleanUp();
	gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
	BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
	gBattlescriptCurrInstr = BattleScript_FlushMessageBox - 5;
	gBattleStruct->zMoveData.active = FALSE;
	gBattleStruct->InstructInProgress = TRUE;
}

void TrySetMagnetRise(void)
{
	if (IsGravityActive()
	|| gBattleStruct->MagnetRiseTimers[gBattlerAttacker] != 0
	|| gStatuses3[gBattlerAttacker] & (STATUS3_LEVITATING | STATUS3_SMACKED_DOWN | STATUS3_ROOTED)
	|| ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_IRON_BALL)
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
	else
	{
		gBattleStruct->MagnetRiseTimers[gBattlerAttacker] = 5;
		gStatuses3[gBattlerAttacker] |= STATUS3_LEVITATING;
	}
}

void TailwindLuckyChantFunc(void)
{
	switch (gCurrentMove) {
		case MOVE_TAILWIND:
			if (gBattleStruct->TailwindTimers[SIDE(gBattlerAttacker)])
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				gBattleStruct->TailwindTimers[SIDE(gBattlerAttacker)] = 4;
				gBattleStringLoader = TailwindSetString;
			}
			break;
		case MOVE_LUCKYCHANT:
			if (gBattleStruct->LuckyChantTimers[SIDE(gBattlerAttacker)])
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				gBattleStruct->LuckyChantTimers[SIDE(gBattlerAttacker)] = 4;
				gBattleStringLoader = LuckyChantSetString;
			}
			break;
	}
}

void FlameBurstFunc(void)
{
	u8 par = PARTNER(gBattlerTarget);

	if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	||  ABILITY(par) == ABILITY_MAGICGUARD
	||  gBattleMons[par].hp == 0)
	{
		gBattlescriptCurrInstr = BattleScript_MoveEnd - 5;
	}
	else
	{
		gBattleScripting.battler = par;
		gBattleMoveDamage = MathMax(1, gBattleMons[par].maxHP / 16);
	}
}

void ModifyPostStockpileBoostDecrement(void)
{
	switch (gDisableStructs[gBattlerAttacker].stockpileCounter) {
		case 1:
			gBattleScripting.statChanger |= DECREASE_1;
			break;
		case 2:
			gBattleScripting.statChanger |= DECREASE_2;
			break;
		case 3:
			gBattleScripting.statChanger |= DECREASE_3;
			break;
	}
}

void RemoveStockpileBoosts(void)
{
	gDisableStructs[gBattlerAttacker].stockpileCounter = 0;
}

void SetHealingWishLunarDanceFunc(void)
{
	switch (gCurrentMove) {
		case MOVE_HEALINGWISH:
			gBattleStruct->HealingWishLoc |= gBitTable[GetBattlerPosition(gBattlerAttacker)];
			break;
		case MOVE_LUNARDANCE:
			gBattleStruct->HealingWishLoc |= (gBitTable[GetBattlerPosition(gBattlerAttacker)] << 4);
	}
}

void FinalGambitDamageCalc(void)
{
	gBattleMoveDamage = gBattleMons[gBattlerAttacker].hp;
}

void AbilityChangeBSFunc(void)
{
    ability_t* atkAbilityLoc, *defAbilityLoc;
	ability_t atkAbility, defAbility;

	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	//Get correct location of ability
	atkAbilityLoc = GetAbilityLocation(gBattlerAttacker);
	defAbilityLoc = GetAbilityLocation(gBattlerTarget);

	atkAbility = *atkAbilityLoc;
	defAbility = *defAbilityLoc;

	gBattleStruct->backupAbility = *defAbilityLoc;

	switch (gCurrentMove) {
		case MOVE_WORRYSEED:
			if (CheckTableForAbility(defAbility, gWorrySeedBannedAbilities))
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				*defAbilityLoc = ABILITY_INSOMNIA;
				gLastUsedAbility = defAbility; //Original ability
				ResetVarsForAbilityChange(gBattlerTarget);
				gBattleStringLoader = WorrySeedString;
			}
			break;

		case MOVE_GASTROACID:
			if (CheckTableForAbility(defAbility, gGastroAcidBannedAbilities)
			|| gStatuses3[gBattlerTarget] & STATUS3_ABILITY_SUPPRESS)
			{
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			}
			else
			{
				gStatuses3[gBattlerTarget] |= STATUS3_ABILITY_SUPPRESS;
				gBattleStruct->SuppressedAbilities[gBattlerTarget] = defAbility;
				*defAbilityLoc = ABILITY_NONE;
				gBattleStruct->SlowStartTimers[gBattlerTarget] = 0;
				gDisableStructs[gBattlerTarget].truantCounter = 0;
				gBattleScripting.battler = gBattlerTarget;
				gBattleStringLoader = AbilitySuppressedString;
				return; //No transfer needed
			}
			break;

		case MOVE_ENTRAINMENT:
			if (atkAbility == ABILITY_NONE
			||  IsDynamaxed(gBattlerTarget)
			||  CheckTableForAbility(atkAbility, gEntrainmentBannedAbilitiesAttacker)
			||  CheckTableForAbility(defAbility, gEntrainmentBannedAbilitiesTarget))
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			else
			{
				*defAbilityLoc = atkAbility;
				gLastUsedAbility = defAbility; //Original ability
				ResetVarsForAbilityChange(gBattlerTarget);
				gBattleStringLoader = EntrainmentString;

				if (gLastUsedAbility == ABILITY_TRUANT)
					gDisableStructs[gBattlerTarget].truantCounter = 0; //Reset counter
			}
			break;

		case MOVE_SIMPLEBEAM:
			if (CheckTableForAbility(defAbility, gSimpleBeamBannedAbilities))
			{
				gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
			}
			else
			{
				*defAbilityLoc = ABILITY_SIMPLE;
				gLastUsedAbility = defAbility; //Original ability
				ResetVarsForAbilityChange(gBattlerTarget);
				gBattleStringLoader = SimpleBeamString;
			}
			break;
	}

	if (gBattlescriptCurrInstr != BattleScript_ButItFailed - 5)
		TransferAbilityPopUp(gBattlerTarget, gLastUsedAbility);
}

void LoadStatustoPsychoShiftTransfer(void)
{
	u32 status = gBattleMons[gBattlerAttacker].status1;

	if (status & STATUS1_SLEEP && CanBePutToSleep(gBattlerTarget, TRUE))
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_SLEEP;
	}
	else if (status & STATUS1_TOXIC_POISON && CanBePoisoned(gBattlerTarget, gBattlerAttacker, TRUE))
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_TOXIC;
	}
	else if (status & STATUS1_POISON && CanBePoisoned(gBattlerTarget, gBattlerAttacker, TRUE))
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_POISON;
	}
	else if (status & STATUS1_BURN && CanBeBurned(gBattlerTarget, TRUE))
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_BURN;
	}
	else if (status & STATUS1_PARALYSIS && CanBeParalyzed(gBattlerTarget, TRUE))
	{
		gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
	}
	else
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
}

void BurnUpFunc(void)
{
	if (gBattleMons[gBattlerAttacker].type1 == TYPE_FIRE)
		gBattleMons[gBattlerAttacker].type1 = TYPE_MYSTERY;

	if (gBattleMons[gBattlerAttacker].type2 == TYPE_FIRE)
		gBattleMons[gBattlerAttacker].type2 = TYPE_MYSTERY;

	if (gBattleMons[gBattlerAttacker].type3 == TYPE_FIRE)
		gBattleMons[gBattlerAttacker].type3 = TYPE_BLANK;
}

bool8 CanUseLastResort(u8 bank)
{
	u32 i;
	bool8 knowsLastResort = FALSE;

	for (i = 0; i < MAX_MON_MOVES && gBattleMons[bank].moves[i] != MOVE_NONE; ++i)
	{
		u16 move = gBattleMons[bank].moves[i];

		if (move == MOVE_LASTRESORT)
			knowsLastResort = TRUE; //Last Resort can't be called from other moves

		else if (!(gBattleStruct->usedMoveIndices[bank] & gBitTable[i]))
			return FALSE;
	}

	if (i == 1 //Attacker only knows Last Resort
	|| !knowsLastResort)
		return FALSE;

	return TRUE;
}

void LastResortFunc(void)
{
	if (!CanUseLastResort(gBattlerAttacker))
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 2 - 5;
}


void SynchronoiseFunc(void)
{
	if (ITEM_EFFECT(gBattlerTarget) == HOLD_EFFECT_RING_TARGET
	|| (!IS_BLANK_TYPE(gBattleMons[gBattlerAttacker].type1) && IsOfType(gBattlerTarget, gBattleMons[gBattlerAttacker].type1))
	|| (!IS_BLANK_TYPE(gBattleMons[gBattlerAttacker].type2) && IsOfType(gBattlerTarget, gBattleMons[gBattlerAttacker].type2))
	|| (!IS_BLANK_TYPE(gBattleMons[gBattlerAttacker].type3) && IsOfType(gBattlerTarget, gBattleMons[gBattlerAttacker].type3)))
	{
		return;
	}

	gBattlescriptCurrInstr = BattleScript_NotAffected - 5;
}

static void TransferDontRemoveTransformSpecies(bool8 val)
{
	gActiveBattler = gBattleScripting.battler;
	gDontRemoveTransformSpecies = val;
	EmitDataTransfer(0, &gDontRemoveTransformSpecies, 1, &gDontRemoveTransformSpecies);
	MarkBattlerForControllerExec(gActiveBattler);
}

void SetAndTransferDontRemoveTransformSpecies(void)
{
	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	TransferDontRemoveTransformSpecies(TRUE);
}

void ClearAndTransferDontRemoveTransformSpecies(void)
{
	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	TransferDontRemoveTransformSpecies(FALSE);
}

void BackupScriptingBankMoveSelectionCursor(void)
{
	gBattleStruct->dynamaxData.backupMoveSelectionCursorPos = gMoveSelectionCursor[gBattleScripting.battler];
}

void RestoreScriptingBankMoveSelectionCursor(void)
{
	gMoveSelectionCursor[gBattleScripting.battler] = gBattleStruct->dynamaxData.backupMoveSelectionCursorPos;
}


void SetTeleportBit(void)
{
	gBattleStruct->TeleportBit = TRUE;
}

void TryLaunchShellTrap(void)
{
	u8 atkSide = SIDE(gBattlerAttacker);
	u8 defSide = SIDE(gProtectStructs[gBattlerAttacker].physicalBattlerId);

	if (!gProtectStructs[gBattlerAttacker].physicalDmg || atkSide == defSide)
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5 - 2;
}

void TrySuckerPunch(void)
{
	if ((SPLIT(gChosenMoveByBattler[gBattlerTarget]) != SPLIT_STATUS && GetBattlerTurnOrderNum(gBattlerTarget) > gCurrentTurnActionNumber) //Attacker moved before target
	|| gChosenMoveByBattler[gBattlerTarget] == MOVE_MEFIRST)
		return;

	gBattlescriptCurrInstr = BattleScript_ButItFailed - 5 - 2;
}

void DoProteanTypeChange(void)
{
	u8 moveType = gBattleStruct->dynamicMoveType;

	gBattleScripting.battler = gBattlerAttacker;
	SET_BATTLER_TYPE(gBattlerAttacker, moveType);
	PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);
}

void TryManipulateDamageForLeechSeedBigRoot(void)
{
	if (ITEM_EFFECT(gBattlerTarget) == HOLD_EFFECT_BIG_ROOT)
		gBattleMoveDamage = (gBattleMoveDamage * 130) / 100;
}

#define gText_BattleYesNoChoice (u8*) 0x83FE791
void DisplayForfeitYesNoBox(void)
{
	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
	BattlePutTextOnWindow(gText_BattleYesNoChoice, 0xE);
	gBattleCommunication[CURSOR_POSITION] = 0;
	BattleCreateYesNoCursorAt();
}

void HandleForfeitYesNoBox(void)
{
	if (gMain.newKeys & DPAD_UP && gBattleCommunication[CURSOR_POSITION] != 0)
	{
		PlaySE(SE_SELECT);
		BattleDestroyYesNoCursorAt();
		gBattleCommunication[CURSOR_POSITION] = 0;
		BattleCreateYesNoCursorAt();
	}
	if (gMain.newKeys & DPAD_DOWN && gBattleCommunication[CURSOR_POSITION] == 0)
	{
		PlaySE(SE_SELECT);
		BattleDestroyYesNoCursorAt();
		gBattleCommunication[CURSOR_POSITION] = 1;
		BattleCreateYesNoCursorAt();
	}
	if (gMain.newKeys & A_BUTTON)
	{
		PlaySE(SE_SELECT);

		if (gBattleCommunication[1] == 0)
			gBattleMainFunc = HandleEndTurn_RanFromBattle;

		gBattleCommunication[CURSOR_POSITION] = 0; //So the game doesn't crash when selecting a move
		HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
		return;
	}
	else if (gMain.newKeys & B_BUTTON)
	{
		PlaySE(SE_SELECT);
		gBattleCommunication[CURSOR_POSITION] = 0; //So the game doesn't crash when selecting a move
		HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
		return;
	}

	gBattlescriptCurrInstr -= 5;
}

void TrySetAlternateFlingEffect(void)
{
	u8 effect = ItemId_GetHoldEffect(ITEM(gBattlerAttacker));
	gBattlescriptCurrInstr += 5;

	if (effect == HOLD_EFFECT_CURE_ATTRACT || effect == HOLD_EFFECT_RESTORE_STATS
	|| IsBerry(ITEM(gBattlerAttacker)))
	{
		if (ItemBattleEffects(ItemEffects_EndTurn, gBattlerTarget, TRUE, TRUE))
			gBattleStruct->doingPluckItemEffect = TRUE;
		else if (ItemBattleEffects(ItemEffects_ContactTarget, gBattlerTarget, TRUE, TRUE))
			gBattleStruct->doingPluckItemEffect = TRUE;
	}

	gBattlescriptCurrInstr -= 5;
}

void TryToStopNewMonFromSwitchingInAfterSRHurt(void)
{
	if (gBattleStruct->endTurnDone)
	{
		gBattleStruct->restartEndTurnSwitching = TRUE;
		gBattlescriptCurrInstr = BattleScript_EntryHazardsHurtReturn - 5; //Continue the switch in effects
	}

	gBattleStruct->switchInEffectsState = 0;
}

void ClearSwitchInEffectsState(void)
{
	if (!gBattleStruct->endTurnDone)
	{
		gBattlescriptCurrInstr = BattleScript_HandleFaintedMonDoublesSwitchInEffects - 5;
		gBattleStruct->switchInEffectsState = 0;
	}
};

void RestoreOriginalAttackerAndTarget(void)
{
	gBattlerAttacker = gBattleStruct->originalAttackerBackup;
	gBattlerTarget = gBattleStruct->originalTargetBackup;
}

void TryRemovePrimalWeatherOnPivot(void)
{
	RestoreOriginalAttackerAndTarget();
	gBattleStruct->skipBankStatAnim = gBattlerAttacker; //Helps with Neutralizing Gas and Intimidate
	if (HandleSpecialSwitchOutAbilities(gBattlerAttacker, ABILITY(gBattlerAttacker)))
		gBattlescriptCurrInstr -= 5;
	else
		gBattleStruct->skipBankStatAnim = 0xFF; //No longer needed
}

void TryRemovePrimalWeatherSwitchingBank(void)
{
	RestoreOriginalAttackerAndTarget();
	gBattleStruct->skipBankStatAnim = gBankSwitching; //Helps with Neutralizing Gas and Intimidate
	if (HandleSpecialSwitchOutAbilities(gBankSwitching, ABILITY(gBankSwitching)))
		gBattlescriptCurrInstr -= 5;
	else
		gBattleStruct->skipBankStatAnim = 0xFF; //No longer needed
}

void TryRemovePrimalWeatherAfterAbilityChange(void)
{
	RestoreOriginalAttackerAndTarget();
	if (HandleSpecialSwitchOutAbilities(gBattlerTarget, gBattleStruct->backupAbility))
		gBattlescriptCurrInstr -= 5;
}

void TryLoadSecondFriskTargetDoubles(void)
{
	u8 partner = PARTNER(gBattlerTarget);

	if (IsDoubleBattle() && BATTLER_ALIVE(partner) && ITEM(partner))
	{
		gLastUsedItem = ITEM(partner);
		gBattlerTarget = partner;
		return;
	}

	gBattlescriptCurrInstr = BattleScript_FriskEnd - 5;
}

void BackupSwitchingBank(void)
{
	gBattleStruct->originalAttackerBackup = gBattlerAttacker;
	gBattleStruct->originalTargetBackup = gBattlerTarget;
}

void RestoreAllOriginalMoveData(void)
{
	gCurrentMove = gChosenMove;

	gBattlerAttacker = gBattleStruct->originalAttackerBackup;
	gBattlerTarget = gBattleStruct->originalTargetBackup;
}


void SetBatonPassSwitchingBit(void)
{
	gBattleStruct->batonPassing = TRUE;
}

void ClearBatonPassSwitchingBit(void)
{
	gBattleStruct->batonPassing = FALSE;
}

void ClearTargetStatFellThisTurn(void)
{
	gBattleStruct->statFellThisTurn[gBattlerTarget] = FALSE;
}

void ReturnOpponentMon2(void)
{
	if (gBattleControllerExecFlags)
	{
		gBattlescriptCurrInstr -= 5;
		return;
	}

	gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);

	if (IS_DOUBLE_BATTLE
	&& BATTLER_ALIVE(gActiveBattler))
	{
		BtlController_EmitReturnMonToBall(0, 0);
		MarkBattlerForControllerExec(gActiveBattler);
	}
}

void BackupScriptingBank(void)
{
	gBattleStruct->SentInBackup = gBattleScripting.battler;
}

void FailIfAttackerIsntHoldingEdibleBerry(void)
{
	u16 item = ITEM(gBattlerAttacker);

	if (!IsBerry(item) || CheckTableForItem(item, gBannedBattleEatBerries)
	|| AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, gBattlerAttacker, ABILITY_UNNERVE, 0, 0))
		gBattlescriptCurrInstr = BattleScript_ButItFailedAttackstring - 5;
}

void SetTempIgnoreAnimations(void)
{
	gBattleStruct->tempIgnoreAnimations = TRUE;
}

void ClearTempIgnoreAnimations(void)
{
	gBattleStruct->tempIgnoreAnimations = FALSE;
}

void FailIfTrappedByNoRetreat(void)
{
	if (gBattleStruct->trappedByNoRetreat & gBitTable[gBattlerAttacker])
		gBattlescriptCurrInstr = BattleScript_ButItFailedAttackstring - 5;
}

void ClearPlayerRechargeMultipleTurns(void)
{
	gBattleMons[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)].status2 &= ~(STATUS2_RECHARGE | STATUS2_MULTIPLETURNS);
	gBattleMons[GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)].status2 &= ~(STATUS2_RECHARGE | STATUS2_MULTIPLETURNS);
}

void SetScriptingBankToItsPartner(void)
{
	gBattleScripting.battler = PARTNER(gBattleScripting.battler);
}

void TryFailLifeDew(void)
{
	if (!IS_DOUBLE_BATTLE || !BATTLER_ALIVE(PARTNER(gBattlerAttacker)))
		gBattlescriptCurrInstr = RecoverBS - 5;
	else if (BATTLER_MAX_HP(gBattlerAttacker) && BATTLER_MAX_HP(PARTNER(gBattlerAttacker)))
		gBattlescriptCurrInstr = BattleScript_LifeDewFail - 5;
}


void FailClangorousSoulIfLowHP(void)
{
	if (gBattleMons[gBattlerAttacker].hp <= gBattleMons[gBattlerAttacker].maxHP / 3)
		gBattlescriptCurrInstr = BattleScript_ButItFailed - 5;
}

void LoadMoodyStatToLower(void)
{
	gBattleScripting.animArg1 = STAT_ANIM_MINUS1 + gBattleCommunication[MOVE_EFFECT_BYTE] - 1;
	gBattleScripting.statChanger = gBattleCommunication[MOVE_EFFECT_BYTE] | DECREASE_1; //Stored here for safety
}

void ClearCalculatedSpreadMoveData(void)
{
	gBattleStruct->calculatedSpreadMoveData = FALSE;
}

void ClearScriptingBankDisguisedAs(void)
{
	gBattleStruct->disguisedAs[gBattleScripting.battler] = SPECIES_NONE;
}

void GotoMoveEndIfMoveDidntDamageAtLeastOnce(void)
{
	if (!gBattleStruct->AttackerDidDamageAtLeastOnce)
		gBattlescriptCurrInstr = BattleScript_MoveEnd - 5;
}

void SetSwitchingBankToPlayer0(void)
{
	gBankSwitching = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
}

void SetAttackerAndSwitchingBankToOpponent0(void)
{
	gBattlerAttacker = gBankSwitching = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
}

void SetSwitchingBankSwitchingCooldownTo2(void)
{
	gBattleStruct->ai.sideSwitchedThisRound |= gBitTable[SIDE(gBankSwitching)];
	if (!(gBattleStruct->ai.sideSwitchedThisRound & gBitTable[SIDE(FOE(gBankSwitching))])) //There was no change on the other side of the field
		gBattleStruct->ai.switchingCooldown[gBankSwitching] = 2;
}

void FaintedBankNameInBuff1(void)
{
    PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gBattlerFainted, gBattlerPartyIndexes[gBattlerFainted]);
}

void SetCorrectTeleportBattleScript(void)
{
	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		gBattlescriptCurrInstr = BattleScript_TeleportSwitch - 5;
	}
	else //Wild Battle
	{
		if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
			gBattlescriptCurrInstr = BattleScript_TeleportSwitch - 5;
		else
			gBattlescriptCurrInstr = BattleScript_TeleportFlee - 5;
	}
}

void RemoveSwitchInForFaintedBank(void)
{
	gBattleStruct->handleSetSwitchIns &= ~(gBitTable[gBattlerFainted]);
}

void AdjustDamage(bool8 checkFalseSwipe)
{
	s32 damage = gBattleMoveDamage;
	u8 resultFlags = gMoveResultFlags;
	bool8 calcSpreadMove = checkFalseSwipe && IS_DOUBLE_BATTLE && gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY);
    u32 bankDef;
    
	gPotentialItemEffectBattler = gBattlerTarget;

	for (bankDef = 0; bankDef < gBattlersCount; ++bankDef)
	{
        u16 item;
        u8 itemEffect, itemQuality;
        ability_t defAbility;

		if (!calcSpreadMove) //Single target move
			bankDef = gBattlerTarget;
		else if (gBattleStruct->calculatedSpreadMoveData)
			break; //Already calculated adjusted damage
		else if (!BATTLER_ALIVE(bankDef) || bankDef == gBattlerAttacker
		|| (bankDef == PARTNER(gBattlerAttacker) && !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_FOES_AND_ALLY))
		|| gBattleStruct->noResultString[bankDef])
			continue;

		if (calcSpreadMove)
		{
			damage = gBattleStruct->DamageTaken[bankDef];
			resultFlags = gBattleStruct->ResultFlags[bankDef];
		}

		item = ITEM(bankDef);
		itemEffect = ITEM_EFFECT(bankDef);
		itemQuality = ITEM_QUALITY(bankDef);
		defAbility = ABILITY(bankDef);

		if ((gBattleStruct->zMoveData.active || IsAnyMaxMove(gCurrentMove))
		&& !IsDynamaxed(bankDef)
		&& ProtectsAgainstZMoves(gCurrentMove, gBattlerAttacker, bankDef))
			damage = (damage  * 25) / 100;

		if (MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bankDef))
			goto END;

		if (IsRaidBattle() && bankDef == BANK_RAID_BOSS)
		{
			if (gBattleStruct->dynamaxData.raidShieldsUp) //Shields heavily reduce damage
			{
				u16 divisor = gBattleMons[bankDef].hp / 8;
				damage /= divisor; //1 hp of damage for every 8th of hp that would have been done

				if (damage >= gBattleMons[bankDef].hp)
					damage = gBattleMons[bankDef].hp - 1; //Can't KO while shields are up
				goto END;
			}
			else
			{
				u16 cutOff = GetNextRaidShieldHP(bankDef);
				if (cutOff > 0 && gBattleMons[bankDef].hp - damage < cutOff)
				{
					damage = gBattleMons[bankDef].hp - cutOff; //Limit damage before Raid shields go up
					goto END;
				}
			}
		}

		if (BATTLER_MAX_HP(bankDef) && defAbility == ABILITY_STURDY)
		{
			RecordAbilityBattle(bankDef, defAbility);
			gProtectStructs[bankDef].enduredSturdy = TRUE;
			gBattleStruct->EnduranceHelper[bankDef] = ENDURE_STURDY;
		}
		else if (BATTLER_MAX_HP(bankDef) && IsBankHoldingFocusSash(bankDef))
		{
			RecordItemEffectBattle(bankDef, itemEffect);
			gSpecialStatuses[bankDef].focusBanded = 1;
			gBattleStruct->EnduranceHelper[bankDef] = ENDURE_FOCUS_SASH;
		}
		else if (itemEffect == HOLD_EFFECT_FOCUS_BAND && Random() % 100 < itemQuality && !IsBankHoldingFocusSash(bankDef))
		{
			RecordItemEffectBattle(bankDef, itemEffect);
			gSpecialStatuses[bankDef].focusBanded = TRUE;
		}

		if (checkFalseSwipe)
		{
			if (gBattleMoves[gCurrentMove].effect != EFFECT_FALSE_SWIPE
			&& !gProtectStructs[bankDef].endured
			&& !gProtectStructs[bankDef].enduredSturdy
			&& !gSpecialStatuses[bankDef].focusBanded)
				goto END;
		}
		else
		{
			if (!gProtectStructs[bankDef].endured
			&& !gProtectStructs[bankDef].enduredSturdy
			&& !gSpecialStatuses[bankDef].focusBanded)
				goto END;
		}

		if (gBattleMons[bankDef].hp > damage)
			goto END;

		damage = gBattleMons[bankDef].hp - 1;

		if (gProtectStructs[bankDef].endured || gProtectStructs[bankDef].enduredSturdy)
		{
			resultFlags |= MOVE_RESULT_FOE_ENDURED;
			goto END;
		}

		if (gSpecialStatuses[bankDef].focusBanded)
		{
			resultFlags |= MOVE_RESULT_FOE_HUNG_ON;
			gLastUsedItem = item;
		}

		END:		
		if (!calcSpreadMove)
			break; //Only 1 target
		else
		{
			gBattleStruct->DamageTaken[bankDef] = damage;
			gBattleStruct->ResultFlags[bankDef] = resultFlags;
		}
	}

	if (calcSpreadMove)
	{
		gBattleMoveDamage = gBattleStruct->DamageTaken[gBattlerTarget];
		gMoveResultFlags = gBattleStruct->ResultFlags[gBattlerTarget];
	}
	else
	{
		gBattleMoveDamage = damage;
		gMoveResultFlags = resultFlags;
	}

	++gBattlescriptCurrInstr;
}

//jumpifspecies BANK SPECIES ROM_OFFSET
void atkFF22_jumpifspecies(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u16 species = T1_READ_16(gBattlescriptCurrInstr + 2);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 4);

	if (gBattleMons[bank].species == species)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 8;
}

//cureprimarystatus BANK FAIL_POINTER
void atkFF02_cureprimarystatus(void)
{
    u8 bank;
    u8* ptr;

	if (gBattleControllerExecFlags) return;

	bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (gBattleMons[bank].status1 == 0)
	{
		if (ptr != NULL)
		{
			gBattlescriptCurrInstr = ptr;
			return;
		}
	}
	else
	{
		ClearBankStatus(bank);
		gBattleScripting.battler = bank;
	}

	gBattlescriptCurrInstr += 6;
}

//jumpifpartnerattack BANK MOVE ROM_OFFSET
void atkFF03_jumpifpartnerattack(void)
{
    u8 bank;
    u16 move;
    u8* ptr;

	if (IS_SINGLE_BATTLE)
		gBattlescriptCurrInstr += 8;

	bank = PARTNER(GetBattlerForBattleScript(gBattlescriptCurrInstr[1]));
	move = T1_READ_16(gBattlescriptCurrInstr + 2);
	ptr = T1_READ_PTR(gBattlescriptCurrInstr + 4);

	if (gBattleMons[bank].hp && gChosenMoveByBattler[bank] == move)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 8;

}

//setterrain ROM_ADDRESS
void atkFF06_setterrain(void)
{
	u8 type = 0xFF;
	u8 duration = 5;
	if (ITEM_EFFECT(gBattlerAttacker) == HOLD_EFFECT_TERRAIN_EXTENDER)
		duration = 8;

	if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN)
		type = 0xFF; //Can't be changed
	else
	{
		if (IsAnyMaxMove(gCurrentMove))
		{
			switch (gBattleMoves[gCurrentMove].z_move_effect) {
				case MAX_EFFECT_ELECTRIC_TERRAIN:
					goto SET_ELECTRIC_TERRAIN;
				case MAX_EFFECT_GRASSY_TERRAIN:
					goto SET_GRASSY_TERRAIN;
				case MAX_EFFECT_MISTY_TERRAIN:
					goto SET_MISTY_TERRAIN;
				case MAX_EFFECT_PSYCHIC_TERRAIN:
					goto SET_PSYCHIC_TERRAIN;
				case MAX_EFFECT_DEFOG:
					goto REMOVE_TERRAIN;
			}
		}

		switch (gCurrentMove) {
			case MOVE_ELECTRICTERRAIN:
			SET_ELECTRIC_TERRAIN:
				type = ELECTRIC_TERRAIN;
				gBattleScripting.animArg1 = B_ANIM_ELECTRIC_SURGE;
				gBattleStringLoader = ElectricTerrainSetString;
				break;
			case MOVE_GRASSYTERRAIN:
			SET_GRASSY_TERRAIN:
				type = GRASSY_TERRAIN;
				gBattleScripting.animArg1 = B_ANIM_GRASSY_SURGE;
				gBattleStringLoader = GrassyTerrainSetString;
				break;
			case MOVE_MISTYTERRAIN:
			SET_MISTY_TERRAIN:
				type = MISTY_TERRAIN;
				gBattleScripting.animArg1 = B_ANIM_MISTY_SURGE;
				gBattleStringLoader = MistyTerrainSetString;
				break;
			case MOVE_PSYCHICTERRAIN:
			case MOVE_GENESIS_SUPERNOVA:
			SET_PSYCHIC_TERRAIN:
				type = PSYCHIC_TERRAIN;
				gBattleScripting.animArg1 = B_ANIM_PSYCHIC_SURGE;
				gBattleStringLoader = PsychicTerrainSetString;
				break;
			case MOVE_SPLINTERED_STORMSHARDS:
			case MOVE_DEFOG:
			case MOVE_STEELROLLER:
			REMOVE_TERRAIN:
				type = 0;
				gBattleStruct->terrainForcefullyRemoved = TRUE;
				gBattleScripting.animArg1 = B_ANIM_LOAD_DEFAULT_BG;
				gBattleStringLoader = TerrainEndString;
				break;
		}
	}

	if (gTerrainType == type || type == 0xFF)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
	{
		gTerrainType = type;

		if (type == 0) //No Terrain means no timer
			gBattleStruct->TerrainTimer = 0;
		else
			gBattleStruct->TerrainTimer = duration;
		gBattlescriptCurrInstr += 5;
	}
}

//jumpifhelditemeffect BANK ITEM_EFFECT ROM_OFFSET
void atkFF07_jumpifhelditemeffect(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8 effect = T1_READ_8(gBattlescriptCurrInstr + 2);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 3);

	if (ITEM_EFFECT(bank) == effect)
	{
		gLastUsedItem = ITEM(bank);
		gBattlescriptCurrInstr = ptr;
	}
	else
		gBattlescriptCurrInstr += 7;
}

enum Counters
{
	Counters_Telekinesis,		//0
	Counters_MagnetRise,		//1
	Counters_HealBlock,			//2
	Counters_LaserFocus,		//3
	Counters_ThroatChop,		//4
	Counters_Embargo,			//5
	Counters_Electrify,			//6
	Counters_SlowStart,			//7
	Counters_Stakeout,			//8
	Counters_StompingTantrum,	//9
	Counters_Nimble,			//10
	Counters_DestinyBond,		//11
	Counters_Metronome,			//12
	Counters_Incinerate,		//13
	Counters_Powder,			//14
	Counters_BeakBlast,			//15
	Counters_AuroraVeil, 		//16
	Counters_TarShot,			//17
};

//counterclear BANK COUNTER_ID FAIL_OFFSET
void atkFF08_counterclear(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8 id = T1_READ_8(gBattlescriptCurrInstr + 2);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
	bool8 failed = FALSE;

	switch(id) {
		case Counters_Telekinesis:
			if (gBattleStruct->TelekinesisTimers[bank])
				gBattleStruct->TelekinesisTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_MagnetRise:
			if (gBattleStruct->MagnetRiseTimers[bank])
				gBattleStruct->MagnetRiseTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_HealBlock:
			if (gBattleStruct->HealBlockTimers[bank])
				gBattleStruct->HealBlockTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_LaserFocus:
			if (gBattleStruct->LaserFocusTimers[bank])
				gBattleStruct->LaserFocusTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_ThroatChop:
			if (gBattleStruct->ThroatChopTimers[bank])
				gBattleStruct->ThroatChopTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Embargo:
			if (gBattleStruct->EmbargoTimers[bank])
				gBattleStruct->EmbargoTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Electrify:
			if (gBattleStruct->ElectrifyTimers[bank])
				gBattleStruct->ElectrifyTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_SlowStart:
			if (gBattleStruct->SlowStartTimers[bank])
				gBattleStruct->SlowStartTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Stakeout:
			if (gBattleStruct->StakeoutCounters[bank])
				gBattleStruct->StakeoutCounters[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_StompingTantrum:
			if (gBattleStruct->StompingTantrumTimers[bank])
				gBattleStruct->StompingTantrumTimers[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Nimble:
			if (gBattleStruct->NimbleCounters[bank])
				gBattleStruct->NimbleCounters[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_DestinyBond:
			if (gBattleStruct->DestinyBondCounters[bank])
				gBattleStruct->DestinyBondCounters[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Metronome:
			if (gBattleStruct->MetronomeCounter[bank])
				gBattleStruct->MetronomeCounter[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Incinerate:
			if (gBattleStruct->IncinerateCounters[bank])
				gBattleStruct->IncinerateCounters[bank] = 0;
			else
				failed = TRUE;
			break;
		case Counters_Powder:
			if (gBattleStruct->PowderByte & gBitTable[bank])
				gBattleStruct->PowderByte &= ~(gBitTable[bank]);
			else
				failed = TRUE;
			break;
		case Counters_BeakBlast:
			if (gBattleStruct->BeakBlastByte & gBitTable[bank])
				gBattleStruct->BeakBlastByte &= ~(gBitTable[bank]);
			else
				failed = TRUE;
			break;
		case Counters_AuroraVeil:
			if (gBattleStruct->AuroraVeilTimers[SIDE(bank)])
				gBattleStruct->AuroraVeilTimers[SIDE(bank)] = 0;
			else
				failed = TRUE;
			break;
		case Counters_TarShot:
			if (gBattleStruct->tarShotBits & gBitTable[bank])
				gBattleStruct->tarShotBits &= ~(gBitTable[bank]);
			else
				failed = TRUE;
			break;
	}

	if (failed)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 7;
}

//jumpifcounter BANK COUNTER_ID PREDICATE BYTE_TO_COMPARE ROM_OFFSET
void atkFF09_jumpifcounter(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8 id = T1_READ_8(gBattlescriptCurrInstr + 2);
	u8 predicate = T1_READ_8(gBattlescriptCurrInstr + 3);
	u8 value = T1_READ_8(gBattlescriptCurrInstr + 4);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 5);
	u8 counter;
	bool8 ret = FALSE;

	switch(id) {
		case Counters_Telekinesis:
			counter = gBattleStruct->TelekinesisTimers[bank];
			break;
		case Counters_MagnetRise:
			counter = gBattleStruct->MagnetRiseTimers[bank];
			break;
		case Counters_HealBlock:
			counter = IsHealBlocked(bank);
			break;
		case Counters_LaserFocus:
			counter = gBattleStruct->LaserFocusTimers[bank];
			break;
		case Counters_ThroatChop:
			counter = CantUseSoundMoves(bank);
			break;
		case Counters_Embargo:
			counter = gBattleStruct->EmbargoTimers[bank];
			break;
		case Counters_Electrify:
			counter = gBattleStruct->ElectrifyTimers[bank];
			break;
		case Counters_SlowStart:
			counter = gBattleStruct->SlowStartTimers[bank];
			break;
		case Counters_Stakeout:
			counter = gBattleStruct->StakeoutCounters[bank];
			break;
		case Counters_StompingTantrum:
			counter = gBattleStruct->StompingTantrumTimers[bank];
			break;
		case Counters_Nimble:
			counter = gBattleStruct->NimbleCounters[bank];
			break;
		case Counters_DestinyBond:
			counter = gBattleStruct->DestinyBondCounters[bank];
			break;
		case Counters_Metronome:
			counter = gBattleStruct->MetronomeCounter[bank];
			break;
		case Counters_Incinerate:
			counter = gBattleStruct->IncinerateCounters[bank];
			break;
		case Counters_Powder:
			counter = gBattleStruct->PowderByte & gBitTable[bank];
			break;
		case Counters_BeakBlast:
			counter = gBattleStruct->BeakBlastByte & gBitTable[bank];
			break;
		case Counters_AuroraVeil:
			counter = gBattleStruct->AuroraVeilTimers[SIDE(bank)];
			break;
		case Counters_TarShot:
			counter = gBattleStruct->tarShotBits & gBitTable[bank];
			break;
		default:
			counter = 0; //Shouldn't happen...
	}

	switch(predicate) {
		case CMP_EQUAL:
			if (value == counter)
				ret++;
			break;
		case CMP_NOT_EQUAL:
			if (value != counter)
				ret++;
			break;
		case CMP_GREATER_THAN:
			if (value > counter)
				ret++;
			break;
		case CMP_LESS_THAN:
			if (value < counter)
				ret++;
			break;
		case CMP_COMMON_BITS:
			if (value & counter)
				ret++;
			break;
		case CMP_NO_COMMON_BITS:
			if (!(value & counter))
				ret++;
			break;
	}

	if (ret)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 9;
}

//setability BANK ABILITY
void atkFF0A_setability(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	ability_t ability = gBattlescriptCurrInstr[2];
	ability_t* abilityLoc = GetAbilityLocation(bank);

	*abilityLoc = ability;

	gBattlescriptCurrInstr += 3;
}

//jumpiftargetpartner ROM_OFFSET
void atkFF0C_jumpiftargetpartner(void)
{
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (gBattlerTarget == PARTNER(gBattlerAttacker))
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 5;
}

//setcounter BANK COUNTER_ID AMOUNT
void atkFF0E_setcounter(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8 id = gBattlescriptCurrInstr[2];
	u8 amount = gBattlescriptCurrInstr[3];

	switch(id) {
		case Counters_Telekinesis:
			gBattleStruct->TelekinesisTimers[bank] = amount;
			break;
		case Counters_MagnetRise:
			gBattleStruct->MagnetRiseTimers[bank] = amount;
			break;
		case Counters_HealBlock:
			gBattleStruct->HealBlockTimers[bank] = amount;
			break;
		case Counters_LaserFocus:
			gBattleStruct->LaserFocusTimers[bank] = amount;
			break;
		case Counters_ThroatChop:
			gBattleStruct->ThroatChopTimers[bank] = amount;
			break;
		case Counters_Embargo:
			gBattleStruct->EmbargoTimers[bank] = amount;
			break;
		case Counters_Electrify:
			gBattleStruct->ElectrifyTimers[bank] = amount;
			break;
		case Counters_SlowStart:
			gBattleStruct->SlowStartTimers[bank] = amount;
			break;
		case Counters_Stakeout:
			gBattleStruct->StakeoutCounters[bank] = amount;
			break;
		case Counters_StompingTantrum:
			gBattleStruct->StompingTantrumTimers[bank] = amount;
			break;
		case Counters_Nimble:
			gBattleStruct->NimbleCounters[bank] = amount;
			break;
		case Counters_DestinyBond:
			gBattleStruct->DestinyBondCounters[bank] = amount;
			break;
		case Counters_Metronome:
			gBattleStruct->MetronomeCounter[bank] = amount;
			break;
		case Counters_Incinerate:
			gBattleStruct->IncinerateCounters[bank] = amount;
			break;
		case Counters_Powder:
			gBattleStruct->PowderByte |= gBitTable[bank];
			break;
		case Counters_BeakBlast:
			gBattleStruct->BeakBlastByte |= gBitTable[bank];
			break;
		case Counters_TarShot:
			gBattleStruct->tarShotBits |= gBitTable[bank];
			break;
	}

	gBattlescriptCurrInstr += 4;
}

//jumpifgrounded BANK ROM_OFFSET
void atkFF0F_jumpifgrounded(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (CheckGrounding(bank))
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;
}

//jumpifhelditem BANK ITEM ROM_OFFSET
void atkFF10_jumpifhelditem(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u16 item = T1_READ_16(gBattlescriptCurrInstr + 2);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 4);

	if (ITEM(bank) == item)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 8;
}

//reloadhealthbar BANK
//void atkFF11_reloadhealthbar(void)

//jumpifhealthcomparestomax BANK PREDICATE ROM_OFFSET
void atkFF12_jumpifhealthcomparestomax(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8 predicate = gBattlescriptCurrInstr[2];
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
	u16 value = gBattleMons[bank].hp;
	u16 comparison = gBattleMons[bank].maxHP;
	bool8 ret = FALSE;

	switch (predicate) {
		case CMP_EQUAL:
			if (value == comparison)
				ret++;
			break;
		case CMP_NOT_EQUAL:
			if (value != comparison)
				ret++;
			break;
	}

	if (ret)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 7;
}

//atkFF13_setdamagetobankhealthfraction BANK FRACTION USE_ACTUAL_MAX_HP
void atkFF13_setdamagetobankhealthfraction(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	s8 fraction = gBattlescriptCurrInstr[2];
	bool8 useActualMaxHP = gBattlescriptCurrInstr[3];

	if (useActualMaxHP)
		gBattleMoveDamage = gBattleMons[bank].maxHP / fraction;
	else
		gBattleMoveDamage = GetBaseMaxHP(bank) / fraction;

	gBattlescriptCurrInstr += 4;
}

//jumpiftypepresent TYPE ROM_OFFSET
void atkFF14_jumpiftypepresent(void)
{
	u8 type = gBattlescriptCurrInstr[1];
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    int i;

	for (i = 0; i < 4; ++i)
	{
		if (IsOfType(i, type))
		{
			gBattlescriptCurrInstr = ptr;
			return;
		}
	}

	gBattlescriptCurrInstr += 6;
}

//jumpifstatcanbelowered BANK STAT ROM_ADDRESS
void atkFF15_jumpifstatcanbemodified(void)
{
	u32 currStat = 0;
    ability_t ability;

	gFormCounter = 0;

	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	currStat = T2_READ_8(gBattlescriptCurrInstr + 2);
	ability = ABILITY(gActiveBattler);

	if (T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_STAT_NEGATIVE) // goes down
	{
		if (ability == ABILITY_CONTRARY)
			goto STAT_ANIM_UP_;

	STAT_ANIM_DOWN_:
		if (gBattleMons[gActiveBattler].statStages[currStat - 1] == 0)
			gFormCounter = 1;

		else if (BankSideHasMist(gActiveBattler) && (gBattlescriptCurrInstr[1] != BS_GET_TARGET || ABILITY(gBattlerAttacker) != ABILITY_INFILTRATOR))
			gFormCounter = 2;

		else if (ability == ABILITY_CLEAR_BODY
		|| ability == ABILITY_WHITE_SMOKE
		//|| ability == ABILITY_FULLMETALBODY
		|| (ability == ABILITY_FLOWERVEIL && IsOfType(gActiveBattler, TYPE_GRASS)))
		{
			gBattleScripting.battler = gActiveBattler;
			gFormCounter = 3;
		}
		else if (ABILITY(PARTNER(gActiveBattler)) == ABILITY_FLOWERVEIL
		&& IsOfType(gActiveBattler, TYPE_GRASS))
		{
			gBattleScripting.battler = PARTNER(gActiveBattler);
			gFormCounter = 3;
		}
		else if ((ability == ABILITY_KEEN_EYE && currStat == STAT_STAGE_ACC)
		|| (ability == ABILITY_HYPER_CUTTER && currStat == STAT_STAGE_ATK)
		|| (ability == ABILITY_BIGPECKS && currStat == STAT_STAGE_DEF))
			gFormCounter = 4;

		PREPARE_STAT_BUFFER(gBattleTextBuff1, currStat)

		if (gFormCounter)
			gBattlescriptCurrInstr += 8;
		else
			gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 4);
	}

	else // goes up
	{
		if (ability == ABILITY_CONTRARY)
			goto STAT_ANIM_DOWN_;

	STAT_ANIM_UP_:	;
		if (gBattleMons[gActiveBattler].statStages[currStat - 1] >= 12)
		{
			gFormCounter = 5;
			gBattlescriptCurrInstr += 8;
		}
		else
			gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 4);
	}
}

//jumpifnoviablemonsleft BANK ROM_ADDRESS
void atkFF16_jumpifnoviablemonsleft(void)
{
	if (ViableMonCountFromBank(GetBattlerForBattleScript(gBattlescriptCurrInstr[1])) == 0)
		gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 2);
	else
		gBattlescriptCurrInstr += 6;
}

//setsidestatus BANK STATUS
void atkFF17_setsidestatus(void)
{
	u8 side = SIDE(GetBattlerForBattleScript(gBattlescriptCurrInstr[1]));
	u16 status = T1_READ_16(gBattlescriptCurrInstr + 2);

	gSideStatuses[side] |= status;

	gBattlescriptCurrInstr += 3;
}

//clearsidestatus BANK STATUS
void atkFF18_clearsidestatus(void)
{
	u8 side = SIDE(GetBattlerForBattleScript(gBattlescriptCurrInstr[1]));
	u16 status = T1_READ_16(gBattlescriptCurrInstr + 2);

	gSideStatuses[side] &= ~status;

	gBattlescriptCurrInstr += 3;
}

//formchange BANK ORIGIN_SPECIES TARGET_SPECIES RELOAD_TYPE RELOAD_STATS RELOAD_ABILITY ROM_ADDRESS
void atkFF19_formchange(void)
{
    u8 bank;
    u16 originalSpecies, targetSpecies;
    bool8 reloadType, reloadStats, reloadAbility;

	if (gBattleControllerExecFlags) return;

	bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	originalSpecies = T1_READ_16(gBattlescriptCurrInstr + 2);
	targetSpecies = T1_READ_16(gBattlescriptCurrInstr + 4);
	reloadType = T2_READ_8(gBattlescriptCurrInstr + 6);
	reloadStats = T2_READ_8(gBattlescriptCurrInstr + 7);
	reloadAbility = T2_READ_8(gBattlescriptCurrInstr + 8);


	if (gBattleMons[bank].species != originalSpecies || gBattleMons[bank].hp == 0)
		gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 9);
	else
	{
		DoFormChange(bank, targetSpecies, reloadType, reloadStats, reloadAbility);
		gBattlescriptCurrInstr += 13;
	}
}

//jumpifabilitypresentattackerfield ABILITY ROM_OFFSET
void atkFF1A_jumpifabilitypresentattackerfield(void)
{
	ability_t ability = gBattlescriptCurrInstr[1];
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (AbilityBattleEffects(ABILITYEFFECT_CHECK_BATTLER_SIDE, gBattlerAttacker, ability, 0, 0))
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;
}

//tryactivateswitchinability
void atkFF1B_tryactivateswitchinability(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	gBattlescriptCurrInstr += 2;
	AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, bank, 0, 0, 0);
}

//atkFF1C - atkFF1E: Trainer Sliding

const u16 gFlowerShieldStringIds[] =
{
	STRINGID_PKMNAVOIDEDATTACK,
	STRINGID_PKMNPROTECTEDITSELF,
	STRINGID_ITDOESNTAFFECT,
	STRINGID_STATSWONTINCREASE2,
};

//flowershieldlooper PLUS_MINUS SUCCESS_ADDRESS FAIL_ADDRESS
void atkFF1F_flowershieldlooper(void)
{
	bool8 plusMinus = gBattlescriptCurrInstr[1];
	u8 battlerCount = (plusMinus) ? gBattlersCount / 2 : gBattlersCount;

	for (; gBattleCommunication[0] < battlerCount; ++gBattleCommunication[0])
	{
		u8 bank = gBattlerByTurnOrder[gBattleCommunication[0]];

		if (plusMinus)
		{
			if (gBattleCommunication[0] == 0)
				bank = gBattlerAttacker;
			else
				bank = PARTNER(gBattlerAttacker);
		}

		if (BATTLER_ALIVE(bank))
		{
			gBattlerTarget = bank;
			++gBattleCommunication[0];

			if (IsMaxGuardUp(bank))
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = 1; //Protected itself
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 6);
				gBattleStruct->StompingTantrumTimers[gBattlerAttacker] = 2;
			}
			else if (MoveBlockedBySubstitute(gCurrentMove, gBattlerAttacker, bank))
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = 2; //No effect
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 6);
				gBattleStruct->StompingTantrumTimers[gBattlerAttacker] = 2;
			}
			else if (BATTLER_SEMI_INVULNERABLE(bank) && ABILITY(gBattlerAttacker) != ABILITY_NOGUARD && ABILITY(bank) != ABILITY_NOGUARD)
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = 0; //Avoided attack
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 6);
			}
			else if ((!plusMinus && IsOfType(bank, TYPE_GRASS))
			|| (plusMinus && (ABILITY(bank) == ABILITY_PLUS || ABILITY(bank) == ABILITY_MINUS)))
			{
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
				gMoveResultFlags = 0;
			}
			else
			{
				gBattleCommunication[MULTISTRING_CHOOSER] = 2; //No effect
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 6);
				gBattleStruct->StompingTantrumTimers[gBattlerAttacker] = 2;
			}

			return;
		}
	}

	gBattlerTarget = gBattlerAttacker;
	gBattlescriptCurrInstr += 10;
}

//jumpifprotectedbycraftyshield BANK ROM_ADDRESS
void atkFF20_jumpifprotectedbycraftyshield(void)
{
	if (CheckCraftyShield(GetBattlerForBattleScript(gBattlescriptCurrInstr[1])))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	else
		gBattlescriptCurrInstr += 5;
}

//tryspectralthiefsteal SUCCESS_ADDRESS
void atkFF21_tryspectralthiefsteal(void)
{
    int i;
	s8 increment = 1;
	bool8 success = FALSE;
	ability_t atkAbility = ABILITY(gBattlerAttacker);

	for (i = 0; i < BATTLE_STATS_NO-1; ++i)
	{
		switch (atkAbility) {
			case ABILITY_SIMPLE:
				increment = 2;
				break;

			case ABILITY_CONTRARY:
				increment = -1;
		}

		if (atkAbility == ABILITY_CONTRARY)
		{
			while (gBattleMons[gBattlerTarget].statStages[i] > 6 && gBattleMons[gBattlerAttacker].statStages[i] > 0)
			{
				success = TRUE;
				gBattleMons[gBattlerTarget].statStages[i] -= 1;
				gBattleMons[gBattlerAttacker].statStages[i] += increment;
				
				gBattleStruct->statFellThisTurn[gBattlerAttacker] = TRUE;
				gBattleStruct->statFellThisRound[gBattlerAttacker] = TRUE;
			}
		}
		else
		{
			while (gBattleMons[gBattlerTarget].statStages[i] > 6 && gBattleMons[gBattlerAttacker].statStages[i] < 12)
			{
				success = TRUE;
				gBattleMons[gBattlerTarget].statStages[i] -= 1;
				gBattleMons[gBattlerAttacker].statStages[i] += increment;

				if (gBattleMons[gBattlerAttacker].statStages[i] > STAT_STAGE_MAX)
					gBattleMons[gBattlerAttacker].statStages[i] = STAT_STAGE_MAX;
				
				gBattleStruct->statRoseThisRound[gBattlerAttacker] = TRUE;
			}
		}
	}

	if (success)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
		gBattlescriptCurrInstr += 5;
}

// faintpokemonaftermove NULL_BYTE NULL_WORD
void atkFF23_faintpokemonaftermove(void)
{
	gActiveBattler = gBattlerTarget;

	if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler])
	&& gBattleMons[gActiveBattler].hp == 0)
	{
		if (TryDoBenjaminButterfree(3))
			return;

		gBattleStruct->lastFainted = gActiveBattler;
		gHitMarker |= HITMARKER_FAINTED(gActiveBattler);
		BattleScriptPush(gBattlescriptCurrInstr + 3);

		if (IsCatchableRaidBattle() && gActiveBattler == BANK_RAID_BOSS)
			gBattlescriptCurrInstr = BattleScript_FaintRaidTarget;
		else
			gBattlescriptCurrInstr = BattleScript_FaintTarget;

		if (SIDE(gActiveBattler) == B_SIDE_PLAYER
		&& (!IsTagBattle() || GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_LEFT)) //Is player's mon
		{
			gHitMarker |= HITMARKER_PLAYER_FAINTED;
			if (gBattleResults.playerFaintCounter < 0xFF)
				gBattleResults.playerFaintCounter++;
			AdjustFriendshipOnBattleFaint(gActiveBattler);
		}
		else
		{
			if (gBattleResults.opponentFaintCounter < 0xFF)
				gBattleResults.opponentFaintCounter++;
			gBattleResults.lastOpponentSpecies = GetBankPartyData(gActiveBattler)->box.species;
		}

		gBattleStruct->RetaliateCounters[SIDE(gActiveBattler)] = 2;

		if ((gHitMarker & HITMARKER_DESTINYBOND)
		&& !IsDynamaxed(gBattlerAttacker)
		&& gBattleMons[gBattlerAttacker].hp != 0)
		{
            const u8* backupScript = gBattlescriptCurrInstr;
			gHitMarker &= ~(HITMARKER_DESTINYBOND);
			
			gBattlescriptCurrInstr = BattleScript_DestinyBondTakesLife;
			BattleScriptPushCursor();
			gBattleMoveDamage = gBattleMons[gBattlerAttacker].hp;
			gBattlescriptCurrInstr = backupScript;
		}

		if ((gStatuses3[gBattlerTarget] & STATUS3_GRUDGE)
		&& !(gHitMarker & HITMARKER_GRUDGE)
		&& SIDE(gBattlerAttacker) != SIDE(gBattlerTarget)
		&& gBattleMons[gBattlerAttacker].hp != 0
		&& gCurrentMove != MOVE_STRUGGLE)
		{
			u8 moveIndex = gBattleStruct->chosenMovePositions[gBattlerAttacker];

			gBattleMons[gBattlerAttacker].pp[moveIndex] = 0;
			BattleScriptPush(gBattlescriptCurrInstr);
			gBattlescriptCurrInstr = BattleScript_GrudgeTakesPp;
			gActiveBattler = gBattlerAttacker;
			BtlController_EmitSetMonData(0, moveIndex + REQUEST_PPMOVE1_BATTLE, 0, 1, &gBattleMons[gActiveBattler].pp[moveIndex]);
			MarkBattlerForControllerExec(gActiveBattler);

			PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleMons[gBattlerAttacker].moves[moveIndex])
		}
	}
	else
	{
		gBattlescriptCurrInstr += 3;
	}
}

//jumpifattackeralreadydiddamage ROM_ADDRESS
void atkFF24_jumpifattackeralreadydiddamage(void)
{
	if (gBattleStruct->AttackerDidDamageAtLeastOnce)
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
		gBattlescriptCurrInstr += 5;
}

//jumpifterrainandgrounded TERRAIN_ID BANK ROM_ADDRESS
void atkFF25_jumpifterrainandgrounded(void)
{
	if (gTerrainType == gBattlescriptCurrInstr[1]
	&&  CheckGrounding(GetBattlerForBattleScript(gBattlescriptCurrInstr[2])))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
	else
		gBattlescriptCurrInstr += 7;
}

//attackstringnoprotean
void atkFF26_attackstringnoprotean(void)
{
    u8 moveType = gBattleStruct->dynamicMoveType;
	if (gBattleControllerExecFlags) return;

	if (!(gHitMarker & (HITMARKER_NO_ATTACKSTRING | HITMARKER_ATTACKSTRING_PRINTED)))
	{
		PrepareStringBattle(STRINGID_USEDMOVE, gBattlerAttacker);

		gBattlescriptCurrInstr++;
		gBattleCommunication[MSG_DISPLAY] = 0;

		if (!gBattleStruct->DancerInProgress && gCurrentMove != MOVE_COPYCAT)
		{
			gBattleStruct->LastUsedMove = gCurrentMove;
			gBattleStruct->LastUsedTypes[gBattlerAttacker] = moveType;

			if (IsAnyMaxMove(gCurrentMove))
				gBattleStruct->LastUsedMove = gChosenMove;

			if (!CheckTableForMove(gCurrentMove, gMovesThatCallOtherMoves))
			{
				u8 chargingBonus = 20 * gBattleStruct->metronomeItemBonus[gBattlerAttacker];
				if (gLastPrintedMoves[gBattlerAttacker] == gCurrentMove)
					gBattleStruct->MetronomeCounter[gBattlerAttacker] = MathMin(100, gBattleStruct->MetronomeCounter[gBattlerAttacker] + 20 + chargingBonus);
				else
					gBattleStruct->MetronomeCounter[gBattlerAttacker] = 0 + chargingBonus;
			}
		}

		gHitMarker |= HITMARKER_ATTACKSTRING_PRINTED;
	}
	else
	{
		gBattlescriptCurrInstr++;
		gBattleCommunication[MSG_DISPLAY] = 0;
	}
}

//tryactivateprotean
void atkFF27_tryactivateprotean(void)
{
	u8 moveType = gBattleStruct->dynamicMoveType;

	if (ABILITY(gBattlerAttacker) == ABILITY_PROTEAN
	&& !(gMoveResultFlags & MOVE_RESULT_FAILED)
	&& gCurrentMove != MOVE_STRUGGLE
	&& !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
	&& !(CheckTableForMove(gCurrentMove, gMovesThatCallOtherMoves)))
	{
		if (gBattleMons[gBattlerAttacker].type1 != moveType
		||  gBattleMons[gBattlerAttacker].type2 != moveType
		|| (gBattleMons[gBattlerAttacker].type3 != moveType && gBattleMons[gBattlerAttacker].type3 != TYPE_BLANK))
		{
			BattleScriptPush(gBattlescriptCurrInstr + 1);
			gBattlescriptCurrInstr = BattleScript_AbilityChangedType;
			return;
		}
	}

	gBattlescriptCurrInstr++;
}

//jumpifweight BANK PREDICATE WEIGHT ROM_OFFSET
void atkFF28_jumpifweight(void)
{
	bool8 ret = FALSE;
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8 predicate = gBattlescriptCurrInstr[2];
	u16 value = T1_READ_16(gBattlescriptCurrInstr + 3);
	const u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 5);

	u32 weight = GetActualSpeciesWeight(SPECIES(bank), ABILITY(bank), ITEM_EFFECT(bank), bank, TRUE);

	switch (predicate) {
		case CMP_EQUAL:
			if (value == weight)
				ret = TRUE;
			break;
		case CMP_NOT_EQUAL:
			if (value != weight)
				ret = TRUE;
			break;
		case CMP_GREATER_THAN:
			if (weight > value)
				ret = TRUE;
			break;
		case CMP_LESS_THAN:
			if (weight < value)
				ret = TRUE;
			break;
		case CMP_COMMON_BITS:
			if (value & weight)
				ret = TRUE;
			break;
		case CMP_NO_COMMON_BITS:
			if (!(value & weight))
				ret = TRUE;
			break;
	}

	if (ret)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 9;
}

/*
Doesn't Affect (Spore, Minior Shield etc.)
Attack Misses
Already Asleep
Already Has Status (Failed)
Safeguard?
Misty Terrain / Electric Terrain
Flower Veil
Sweet Veil
Ability Protects
*/

//trysetsleep BANK FAIL_ADDRESS
void atkFF29_trysetsleep(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	bool8 fail = FALSE;

	if (BATTLER_SEMI_INVULNERABLE(bank) && !CanHitSemiInvulnerableTarget(gBattlerAttacker, bank, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, bank, 0, 0, gCurrentMove))
	{
		return;
	}
	else if (CheckTableForMove(gCurrentMove, gPowderMoves) && NewTypeCalc(gCurrentMove, gBattlerAttacker, bank, NULL, FALSE) & MOVE_RESULT_DOESNT_AFFECT_FOE)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_SLEEP)
	{
		gBattleStringLoader = gText_TargetAlreadyAsleep;
		fail = TRUE;
	}
	else if (gBattleMons[bank].status1 != STATUS1_NONE)
	{
		gBattleStringLoader = gText_TargetAlreadyHasStatusCondition; //String not in official games; officially "But it failed!"
		fail = TRUE;
	}
	else if (ABILITY(gBattlerAttacker) != ABILITY_INFILTRATOR && BankSideHasSafeguard(bank))
	{
		gBattleStringLoader = gText_TeamProtectedBySafeguard;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && gTerrainType == MISTY_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && gTerrainType == ELECTRIC_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInElectricTerrain;
		fail = TRUE;
	}
	else if (IsOfType(bank, TYPE_GRASS) && ABILITY(bank) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.battler = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	else if (IsOfType(bank, TYPE_GRASS) && IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.battler = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	else if (ABILITY(bank) == ABILITY_SWEETVEIL)
	{
		gBattleScripting.battler = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		return;
	}
	else if (IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_SWEETVEIL)
	{
		gBattleScripting.battler = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		return;
	}
	else if (IsUproarBeingMade())
	{
		gBattleScripting.battler = bank;
		gBattleStringLoader = gText_CantFallAsleepDuringUproar;
		fail = TRUE;
	}

	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_INSOMNIA:
			case ABILITY_VITAL_SPIRIT:
				gBattlescriptCurrInstr = BattleScript_TargetStayedAwakeUsingAbility;
				return;
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
				break;
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
				return;
			case ABILITY_SHIELDSDOWN:
				if (SPECIES(bank) == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
		}

		if (!fail && DoesSleepClausePrevent(bank))
		{
			gBattleStringLoader = gText_SleepClausePrevents;
			fail = TRUE;
		}
	}

	if (fail)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = ptr;
	}
	else
		gBattlescriptCurrInstr += 6;
}

//trysetparalysis BANK FAIL_ADDRESS
void atkFF2A_trysetparalysis(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	bool8 fail = FALSE;

	if (BATTLER_SEMI_INVULNERABLE(bank) && !CanHitSemiInvulnerableTarget(gBattlerAttacker, bank, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, bank, 0, 0, gCurrentMove))
	{
		return;
	}
	else if ((CheckTableForMove(gCurrentMove, gPowderMoves) || gCurrentMove == MOVE_THUNDERWAVE)
	&& NewTypeCalc(gCurrentMove, gBattlerAttacker, bank, NULL, FALSE) & MOVE_RESULT_DOESNT_AFFECT_FOE)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (IsOfType(bank, TYPE_ELECTRIC))
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
	{
		gBattleStringLoader = gText_TargetAlreadyParalyzed;
		fail = TRUE;
	}
	else if (gBattleMons[bank].status1 != STATUS1_NONE)
	{
		gBattleStringLoader = gText_TargetAlreadyHasStatusCondition; //String not in official games; officially "But it failed!"
		fail = TRUE;
	}
	else if (ABILITY(gBattlerAttacker) != ABILITY_INFILTRATOR && BankSideHasSafeguard(bank))
	{
		gBattleStringLoader = gText_TeamProtectedBySafeguard;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && gTerrainType == MISTY_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}
	else if (IsOfType(bank, TYPE_GRASS) && ABILITY(bank) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.battler = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	else if (IsOfType(bank, TYPE_GRASS) && IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.battler = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}

	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
				break;
			case ABILITY_LIMBER:
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
				return;
			case ABILITY_SHIELDSDOWN:
				if (SPECIES(bank) == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
		}
	}

	if (fail)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = ptr;
	}
	else
		gBattlescriptCurrInstr += 6;
}

//trysetburn BANK FAIL_ADDRESS
void atkFF2B_trysetburn(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	bool8 fail = FALSE;

	if (BATTLER_SEMI_INVULNERABLE(bank) && !CanHitSemiInvulnerableTarget(gBattlerAttacker, bank, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, bank, 0, 0, gCurrentMove))
	{
		return;
	}
	if (IsOfType(bank, TYPE_FIRE))
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_BURN)
	{
		gBattleStringLoader = gText_TargetAlreadyBurned;
		fail = TRUE;
	}
	else if (gBattleMons[bank].status1 != STATUS1_NONE)
	{
		gBattleStringLoader = gText_TargetAlreadyHasStatusCondition; //String not in official games; officially "But it failed!"
		fail = TRUE;
	}
	else if (ABILITY(gBattlerAttacker) != ABILITY_INFILTRATOR && BankSideHasSafeguard(bank))
	{
		gBattleStringLoader = gText_TeamProtectedBySafeguard;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && gTerrainType == MISTY_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}
	else if (IsOfType(bank, TYPE_GRASS) && ABILITY(bank) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.battler = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	else if (IsOfType(bank, TYPE_GRASS) && IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.battler = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}

	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
				break;
			case ABILITY_WATER_VEIL:
			case ABILITY_WATERBUBBLE:
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
				return;
			case ABILITY_SHIELDSDOWN:
				if (SPECIES(bank) == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
				break;
		}
	}

	if (fail)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = ptr;
	}
	else
		gBattlescriptCurrInstr += 6;
}

//trysetpoison BANK FAIL_ADDRESS
void atkFF2C_trysetpoison(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	bool8 fail = FALSE;

	if (BATTLER_SEMI_INVULNERABLE(bank) && !CanHitSemiInvulnerableTarget(gBattlerAttacker, bank, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, bank, 0, 0, gCurrentMove))
	{
		return;
	}
	else if (ABILITY(gBattlerAttacker) != ABILITY_CORROSION
	&& (IsOfType(bank, TYPE_POISON) || IsOfType(bank, TYPE_STEEL)))
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_PSN_ANY)
	{
		gBattleStringLoader = gText_TargetAlreadyPoisoned;
		fail = TRUE;
	}
	else if (gBattleMons[bank].status1 != STATUS1_NONE)
	{
		gBattleStringLoader = gText_TargetAlreadyHasStatusCondition; //String not in official games; officially "But it failed!"
		fail = TRUE;
	}
	else if (ABILITY(gBattlerAttacker) != ABILITY_INFILTRATOR && BankSideHasSafeguard(bank))
	{
		gBattleStringLoader = gText_TeamProtectedBySafeguard;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && gTerrainType == MISTY_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}
	else if (IsOfType(bank, TYPE_GRASS) && ABILITY(bank) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.battler = bank;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	//Put Pastel Veil here
	else if (IsOfType(bank, TYPE_GRASS) && IS_DOUBLE_BATTLE && ABILITY(PARTNER(bank)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting.battler = PARTNER(bank);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		return;
	}
	//Put Pastel Veil here

	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY && ITEM_EFFECT(bank) != HOLD_EFFECT_UTILITY_UMBRELLA)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
				break;
			case ABILITY_IMMUNITY:
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
				return;
			case ABILITY_SHIELDSDOWN:
				if (SPECIES(bank) == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					return;
				}
		}
	}

	if (fail)
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = ptr;
	}
	else
		gBattlescriptCurrInstr += 6;
}

//addindicatorforattackerswitchineffects
void atkFF2D_addindicatorforattackerswitchineffects(void) //Used for when the game asks you if you want to switch to counter what the foe is sending in
{
	gBattleStruct->doSwitchInEffects |= gBitTable[gBattlerAttacker];
	gBattlescriptCurrInstr += 1;
}

//setmoveeffect2
void atkFF2E_setmoveeffect2(void)
{
	gBattlescriptCurrInstr += 1;
	SetMoveEffect2();
}

//setmaxmoveeffect
//void atkFF2F_setmaxmoveeffect

//jumpifdynamaxed BANK ROM_OFFSET
void atkFF30_jumpifdynamaxed(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	const u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (IsDynamaxed(bank))
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;
}

//jumpifraidboss BANK ROM_OFFSET
void atkFF31_jumpifraidboss(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	const u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (IsRaidBattle() && bank == BANK_RAID_BOSS)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;
}

//recycleberry BANKish FAIL_OFFSET
void atkFF32_recycleberry(void)
{
	u8 bank = (gBattlescriptCurrInstr[1] == BS_GET_ATTACKER) ? gBattlerAttacker : PARTNER(gBattlerAttacker);
	u16 item = SAVED_CONSUMED_ITEMS(bank);

	if (item != ITEM_NONE && IsBerry(item) && ITEM(bank) == ITEM_NONE)
	{
		RecycleItem(bank);
		gBattlescriptCurrInstr += 6;
	}
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
}

//seteffectprimaryscriptingbank
void atkFF33_SetEffectPrimaryScriptingBank(void)
{
	u8 backupBank = gBattlerTarget;
	gBattlerTarget = gBattleScripting.battler;
	SetMoveEffect(TRUE, (gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_CERTAIN) != 0);
	gBattlerTarget = backupBank;
}

//canconfuse BANK FAIL_ADDRESS
void atkFF34_canconfuse(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	bool8 fail = FALSE;

	if (BATTLER_SEMI_INVULNERABLE(bank) && !CanHitSemiInvulnerableTarget(gBattlerAttacker, bank, gCurrentMove))
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		gBattlescriptCurrInstr = BattleScript_PauseResultMessage;
		return;
	}
	else if (IsConfused(bank))
	{
		gBattleStringLoader = gText_TargetAlreadyConfused;
		fail = TRUE;
	}
	else if (ABILITY(gBattlerAttacker) != ABILITY_INFILTRATOR && BankSideHasSafeguard(bank))
	{
		gBattleStringLoader = gText_TeamProtectedBySafeguard;
		fail = TRUE;
	}
	else if (CheckGrounding(bank) && gTerrainType == MISTY_TERRAIN)
	{
		gBattleStringLoader = gText_TargetWrappedInMistyTerrain;
		fail = TRUE;
	}

	if (!fail)
	{
		switch (ABILITY(bank)) {
			case ABILITY_OWN_TEMPO:
				gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
				return;
		}

		gBattlescriptCurrInstr += 6;
	}
	else //Fail
	{
		gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gBattlescriptCurrInstr = ptr;
	}
}

//jumpifmaxchistrikecapped BANK FAIL_ADDRESS
void atkFF35_jumpifmaxchistrikecapped(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	u8* ptr = T1_READ_PTR(gBattlescriptCurrInstr + 2);

	if (gBattleStruct->chiStrikeCritBoosts[bank] >= 3)
		gBattlescriptCurrInstr = ptr;
	else
		gBattlescriptCurrInstr += 6;
}

////////////////////////////////////////////////////////////////////////////////////////

#define INCREMENT_RESET_RETURN				  \
{											   \
	gBattlescriptCurrInstr++;				   \
	gBattleCommunication[MOVE_EFFECT_BYTE] = 0; \
	return;									 \
}

#define RESET_RETURN							\
{											   \
	gBattleCommunication[MOVE_EFFECT_BYTE] = 0; \
	return effect;							  \
}

bool8 SetMoveEffect2(void)
{
	bool8 affectsUser = FALSE;
	bool8 effect = FALSE;

	if (gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_AFFECTS_USER)
	{
		gEffectBattler = gBattlerAttacker; // battlerId that effects get applied on
		gBattleCommunication[MOVE_EFFECT_BYTE] &= ~(MOVE_EFFECT_AFFECTS_USER);
		affectsUser = TRUE;
		gBattleScripting.battler = gBattlerAttacker;
	}
	else
	{
		gEffectBattler = gBattlerTarget;
		gBattleScripting.battler = gBattlerAttacker;
	}

	if (CheckTableForAbility(gBattleCommunication[MOVE_EFFECT_BYTE], sMoveEffectsThatIgnoreSubstitute))
		goto SKIP_SUBSTITUTE_CHECK;

	if (gBattleMons[gEffectBattler].hp == 0
	&& gBattleCommunication[MOVE_EFFECT_BYTE] != MOVE_EFFECT_STEAL_ITEM)
		RESET_RETURN

	if (MoveBlockedBySubstitute(gCurrentMove, gBattleScripting.battler, gEffectBattler)
	&& !affectsUser
	&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
		RESET_RETURN

	if (gBattleStruct->dynamaxData.raidShieldsUp
	&& !affectsUser
	&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
		RESET_RETURN

	SKIP_SUBSTITUTE_CHECK:
	switch (gBattleCommunication[MOVE_EFFECT_BYTE]) {
		case MOVE_EFFECT_STEAL_ITEM:
			if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && SIDE(gBattlerAttacker) == B_SIDE_OPPONENT) //Wild mons can steal items
			{
				break;
			}
			else if (ITEM(gEffectBattler) == 0
			||  ITEM(gBattlerAttacker) != 0
			||  !CanTransferItem(SPECIES(gEffectBattler), ITEM(gEffectBattler))
			||  !CanTransferItem(SPECIES(gBattlerAttacker), ITEM(gEffectBattler))
			||	!BATTLER_ALIVE(gBattlerAttacker))
			{
				break;
			}
			else if (ABILITY(gEffectBattler) == ABILITY_STICKY_HOLD && gBattleMons[gEffectBattler].hp != 0)
			{
				//Handled in CMD49
				/*
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_StickyHoldActivatesRet;

				gLastUsedAbility = ABILITY(gEffectBattler);
				RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
				effect = TRUE;
				*/
				break;
			}
			else
			{
				gLastUsedItem = gBattleMons[gEffectBattler].item;
				gBattleMons[gEffectBattler].item = 0;
				gBattleMons[gBattlerAttacker].item = gLastUsedItem;
				HandleUnburdenBoost(gEffectBattler); //Give target Unburden boost
				HandleUnburdenBoost(gBattlerAttacker); //Remove attacker's Unburden boost

				gActiveBattler = gBattlerAttacker;
				BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gLastUsedItem);
				MarkBattlerForControllerExec(gActiveBattler);

				gActiveBattler = gEffectBattler;
				BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
				MarkBattlerForControllerExec(gActiveBattler);

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_ItemSteal;

				gBattleStruct->choicedMove[gEffectBattler] = 0;
				effect = TRUE;
			}
			break;

		case MOVE_EFFECT_KNOCK_OFF:
			if (!CanKnockOffItem(gEffectBattler)
			||	!BATTLER_ALIVE(gBattlerAttacker))
			{
				break;
			}
			else if (ABILITY(gEffectBattler) == ABILITY_STICKY_HOLD && gBattleMons[gEffectBattler].hp)
			{
				//Handled in CMD49
				/*
				gLastUsedAbility = ABILITY_STICKYHOLD;
				gBattlescriptCurrInstr = BattleScript_StickyHoldActivatesRet;
				gBattleScripting.battler = gEffectBattler;
				effect = TRUE;
				*/
				break;
			}
			else
			{
				gLastUsedItem = gBattleMons[gEffectBattler].item;
				gBattleMons[gEffectBattler].item = 0;
				HandleUnburdenBoost(gEffectBattler); //Give target Unburden boost

				gActiveBattler = gEffectBattler;
				BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
				MarkBattlerForControllerExec(gActiveBattler);

				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_KnockedOff;

				gBattleStruct->choicedMove[gEffectBattler] = 0;
				effect = TRUE;
			}
			break;

		case MOVE_EFFECT_REMOVE_PARALYSIS: // Smelling salts
			switch (gCurrentMove) {
				case MOVE_SMELLINGSALTS:
					if (gBattleMons[gEffectBattler].status1 & STATUS1_PARALYSIS)
					{
						gBattleMons[gEffectBattler].status1 &= ~(STATUS1_PARALYSIS);

						gActiveBattler = gEffectBattler;
						BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
						MarkBattlerForControllerExec(gActiveBattler);

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_TargetPRLZHeal;
						effect = TRUE;
					}
					break;

				case MOVE_WAKEUPSLAP:
					if (gBattleMons[gEffectBattler].status1 & STATUS1_SLEEP)
					{
						gBattleMons[gEffectBattler].status1 &= ~(STATUS1_SLEEP);

						gActiveBattler = gEffectBattler;
						BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
						MarkBattlerForControllerExec(gActiveBattler);

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_TargetSleepHeal;
						effect = TRUE;
					}
					break;

				case MOVE_SPARKLINGARIA:
					if (gBattleMons[gEffectBattler].status1 & STATUS1_BURN)
					{
						gBattleMons[gEffectBattler].status1 &= ~(STATUS1_BURN);

						gActiveBattler = gEffectBattler;
						BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
						MarkBattlerForControllerExec(gActiveBattler);

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_TargetBurnHeal;
						effect = TRUE;
					}
					break;
			}
			break;

		case MOVE_EFFECT_BRING_DOWN:
			if (gStatuses3[gEffectBattler] & STATUS3_ON_AIR)
				goto SMACK_TGT_DOWN;

			if (gStatuses3[gEffectBattler] & (STATUS3_SKY_DROP_ATTACKER | STATUS3_SKY_DROP_TARGET | STATUS3_ROOTED | STATUS3_SMACKED_DOWN)
			||  ITEM_EFFECT(gEffectBattler) == HOLD_EFFECT_IRON_BALL
			||  IsGravityActive())
			{
				break;
			}

			if (IsOfType(gEffectBattler, TYPE_FLYING)
			||  ABILITY(gEffectBattler) == ABILITY_LEVITATE
			||  (gStatuses3[gEffectBattler] & (STATUS3_ON_AIR | STATUS3_LEVITATING | STATUS3_TELEKINESIS))
			||  (ITEM_EFFECT(gEffectBattler) == HOLD_EFFECT_AIR_BALLOON))
			{
			SMACK_TGT_DOWN:
				gStatuses3[gEffectBattler] |= STATUS3_SMACKED_DOWN;
				gBattleStruct->targetsToBringDown |= gBitTable[gEffectBattler];
				BringDownMons();
				gBattlescriptCurrInstr = BattleScript_PrintCustomString;
				effect = TRUE;
			}
			break;

		case MOVE_EFFECT_ION_DELUGE:
			if (!IsIonDelugeActive())
				gBattleStruct->IonDelugeTimer = 1;
			BattleScriptPushCursor();
			gBattleStringLoader = IonDelugeShowerString;
			gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			effect = TRUE;
			break;
	}

	gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
	return effect;
}