#ifndef GUARD_CONSTANTS_GLOBAL_CONFIG_H
#define GUARD_CONSTANTS_GLOBAL_CONFIG_H

/*===== Time Options =====*/
/* End hours for each of the timelapses */
#define MIDNIGHT_END_HOUR   7       //00 - 07
#define DAWN_END_HOUR       8       //07 - 08
#define DAY_END_HOUR        16      //08 - 19
#define SUNSET_END_HOUR     19      //19 - 20
#define NIGHTFALL_END_HOUR  20      //20 - 21
#define NIGHT_END_HOUR      0       //21 - 00

#define MAX_NUM_ROAMERS 1

#define DNS_ENABLED
#define OVERWRITE_RIVAL
#define EVO_HOLD_ITEM_REMOVAL //Comment this out if you want leveling up/hold item evolution (eg. sneasel) to remove the item (like normal)
#define CHANGE_MAP_WEATHER_BATTLE_END

#define DEBUG_DYNAMAX //测试
#define DEBUG_MEGA

#define LAST_TOTAL_TUTOR_NUM 24 //Should be equal to (NUM_MOVE_TUTORS - 1) + 9. Must be set to an actual integer or the compilation will not work.
#define VAR_SECOND_OPPONENT 0x5010 //Set this to the var used to store the Trainer Id of the second opponent in Multi Battles (can be any free var)
#define VAR_PARTNER 0x5011 //Set this to the var used to store the Trainer Id of your partner in Multi Battles (can be any free var)
#define VAR_PARTNER_BACKSPRITE 0x5012 //Set this to the var used to store the backsprite number of your partner in Multi Battles (can be any free var)

#define BASE_CRIT_MULTIPLIER 10

/*===== General Vars =====*/
#define VAR_TERRAIN 0x5000 //Set to a terrain type for a battle to begin with the given terrain
#define VAR_TOTEM 0x5001 //to var + 3 (0x5004)
#define VAR_BACKSPRITE_SWITCH 0x5006 //This var can be set to a number to change the Player's backsprite
//#define VAR_BATTLE_BG 0x5007 //Set this var to a custom background id
#define VAR_SWARM_INDEX 0x5008 //Set by the engine
#define VAR_SWARM_DAILY_EVENT 0x5009 //Set by the engine. Used to tell the game if a swarm has already been generated for the day. (Uses + 1 var also)
#define VAR_DEFAULT_WALKING_SCRIPT 0x500B  //Walking scripts from JPAN's engine. His engine used 0x407E.
#define VAR_DEXNAV 0x500C //Set by the engine indicating which Pokemon has been registered to find in the OW
#define VAR_STATUS_INDUCER 0x500D //Causes wild Pokemon to be spawned with the given status condition. Status + 0x100 clears var after battle.

/*===== General Flags =====*/
#define FLAG_NO_CATCHING 0x902 //The player cannot catch Pokemon in battles when this flag is set
#define FLAG_NO_RUNNING 0x903 //The player cannot run in battles when this flag is set
#define FLAG_NO_CATCHING_AND_RUNNING 0x904 //The player cannot run or catch Pokemon in battles when this flag is set

/*===== Battle Tower Options =====*/
#define FLAG_BATTLE_FACILITY 0x930

#define DUSK_BALL_MULTIPLIER 30 //Change this line to 35 to make the catch rate for the Dusk Ball 3.5x like before Gen 7
#define STANDARD_IV 10 //Change this to be the number of IVs Pokemon owned by random trainers should have (all stats will have this number)
#define SWARM_CHANCE 50 //Change this to the percentage that swarming Pokemon will appear if they can be found on the current route.
#define WILD_DOUBLE_RANDOM_CHANCE 50 //Change this to the percentage that a wild double battle will be initiated if the player is in special grass.
#define CREATE_WITH_X_PERFECT_IVS 3 //Change this to the number of set 31 IVs you'd like Pokemon defined in gSetPerfectXIvList to have.
#define CREATE_ROAMER_WITH_X_PERFECT_IVS 3 //Change this to the number of set 31 IVs you'd like roaming pokemon to have.
#define EGG_HATCH_LEVEL 1 //The level eggs hatch at
#define AI_TRY_TO_KILL_RATE 50 //In battles against a trainer with AI flags of 1, the AI will try to use a move to knock out the opponents XX percent of the time

/*===== Badge Obedience Options =====*/
#define BASE_OBEDIENCE_LEVEL	10
#define BADGE_1_OBEDIENCE_LEVEL	10
#define BADGE_2_OBEDIENCE_LEVEL 30
#define BADGE_3_OBEDIENCE_LEVEL 30
#define BADGE_4_OBEDIENCE_LEVEL 50
#define BADGE_5_OBEDIENCE_LEVEL 50
#define BADGE_6_OBEDIENCE_LEVEL 70
#define BADGE_7_OBEDIENCE_LEVEL 70

#define TIME_MORNING_START 4		//4:00 AM / 4:00
#define TIME_DAY_START 8			//8:00 AM / 8:00
#define TIME_EVENING_START 17		//5:00 PM / 17:00
#define TIME_NIGHT_START 20			//8:00 PM / 20:00

/*===== Other Battle Options =====*/
#define MEGA_EVOLUTION_FEATURE //Comment this line to remove Mega Evolutions
#define TIME_ENABLED
#define DNS_IN_BATTLE
//#define DYNAMAX_FEATURE //Comment this line to remove Dynamax and Raid Battles
#define DISPLAY_EFFECTIVENESS_ON_MENU
#define NO_GHOST_BATTLES //Uncomment this line to disable the Ghost battle feature from Pokemon Tower in Lavender town
#define HAIL_IN_BATTLE //Uncommenting this line enables the Hail weather effect in battle when the OW weather is set to WEATHER_STEADY_SNOW (0x7)
#define FOG_IN_BATTLE //Uncommenting this line enables the Fog weather effect in battle. Don't uncomment this line without uncommenting one of the lines below!
#define FOG_IN_BATTLE_1 //Uncommenting this line enables the Fog weather effect when the OW weather is set to WEATHER_FOG_1 (0x6)
#define FOG_IN_BATTLE_2 //Uncommenting this line enables the Fog weather effect when the OW weather is set to WEATHER_FOG_2 (0x9)
#define FOG_IN_BATTLE_3 //Uncommenting this line enables the Fog weather effect when the OW weather is set to WEATHER_FOG_3 (0xA)

#define ALLOW_RUNNING_IN_DOOR
#define NO_EGG_COUNT_IN_PC //Don't Count Eggs Healing PKMN Centre
#define ESTEND_DIRECT_SOUND //Extend Direct Sound Tracks
#define REMOVE_EVOLUTION_LIMIT //Remove GF Evolution Limiters 151 or NationPokeDex
#define REMOVE_NATION_DEX_NUM //Display Foreign Pokemon's ID even without National Dex
//#define REMOVE_QUEST_LOG
#define ALWAYS_NATIONAL_DEX //直接开启全国图鉴模式
#define REMOVE_FOOT_PRINT //去除脚印显示


/*===== 非官方的一些设置 =====*/
#define KEENEYE_INFILTRATOR_DONT_LOSS_ACC_ON_FOG    //鸽眼在雾天下战斗里不降低命中

#endif //GUARD_CONSTANTS_GLOBAL_CONFIG_H