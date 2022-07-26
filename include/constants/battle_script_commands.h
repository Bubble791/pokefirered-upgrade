#ifndef GUARD_CONSTANTS_BATTLE_SCRIPT_COMMANDS_H
#define GUARD_CONSTANTS_BATTLE_SCRIPT_COMMANDS_H

// Battle Scripting and BattleCommunication addresses
#define sPAINSPLIT_HP gBattleScripting
#define sBIDE_DMG gBattleScripting + 4
#define sMULTIHIT_STRING gBattleScripting + 8
#define sDMG_MULTIPLIER gBattleScripting + 0xE
#define sTWOTURN_STRINGID gBattleScripting + 0xF
#define sB_ANIM_ARG1 gBattleScripting + 0x10
#define sB_ANIM_ARG2 gBattleScripting + 0x11
#define sTRIPLE_KICK_POWER gBattleScripting + 0x12
#define sMOVEEND_STATE gBattleScripting + 0x14
#define sBATTLER_WITH_ABILITY gBattleScripting + 0x15
#define sMULTIHIT_EFFECT gBattleScripting + 0x16
#define sBATTLER gBattleScripting + 0x17
#define sB_ANIM_TURN gBattleScripting + 0x18
#define sB_ANIM_TARGETS_HIT gBattleScripting + 0x19
#define sSTATCHANGER gBattleScripting + 0x1A
#define sSTAT_ANIM_PLAYED gBattleScripting + 0x1B
#define sGIVEEXP_STATE gBattleScripting + 0x1C
#define sBATTLE_STYLE gBattleScripting + 0x1D
#define sLVLBOX_STATE gBattleScripting + 0x1E
#define sLEARNMOVE_STATE gBattleScripting + 0x1F
#define sFIELD_20 gBattleScripting + 0x20
#define sRESHOW_MAIN_STATE gBattleScripting + 0x21
#define sRESHOW_HELPER_STATE gBattleScripting + 0x22
#define sFIELD_23 gBattleScripting + 0x23
#define sWINDOWS_TYPE gBattleScripting + 0x24
#define sMULTIPLAYER_ID gBattleScripting + 0x25
#define sSPECIAL_TRAINER_BATTLE_TYPE gBattleScripting + 0x26

#define cEFFECT_CHOOSER gBattleCommunication + 3
#define cMULTISTRING_CHOOSER gBattleCommunication + 5

#define POISONED_BY gPoisonedBy
#define BATTLE_STRING_LOADER gBattleStringLoader
#define HIT_MARKER gHitMarker
#define SEED_HELPER gSeedHelper
#define NUM_POKEMON gBattlersCount
#define TARGET_BANK gBattlerTarget
#define FORM_COUNTER gFormCounter
#define BACKUP_HWORD gBackupHWord
// Battle Script defines for getting the wanted battler
#define BS_TARGET                   0
#define BS_ATTACKER                 1
#define BS_EFFECT_BATTLER           2
#define BS_FAINTED                  3
#define BS_ATTACKER_WITH_PARTNER    4 // for atk98_updatestatusicon
#define BS_UNKNOWN_5                5 // for openpartyscreen
#define BS_UNKNOWN_6                6 // for openpartyscreen
#define BS_BATTLER_0                7
#define BS_ATTACKER_SIDE            8 // for atk1E_jumpifability
#define BS_NOT_ATTACKER_SIDE        9 // for atk1E_jumpifability
#define BS_SCRIPTING                10
#define BS_PLAYER1                  11
#define BS_OPPONENT1                12
#define BS_PLAYER2                  13 // for atk98_updatestatusicon
#define BS_OPPONENT2                14
#define BS_SWITCHING                15

// used for openpartyscreen
#define OPEN_PARTY_ALLOW_CANCEL     0x80

// atk 01 accuracy calc
#define NO_ACC_CALC 0xFFFE
#define NO_ACC_CALC_CHECK_LOCK_ON 0xFFFF
#define ACC_CURR_MOVE 0

// compare operands
#define CMP_EQUAL               0x0
#define CMP_NOT_EQUAL           0x1
#define CMP_GREATER_THAN        0x2
#define CMP_LESS_THAN           0x3
#define CMP_COMMON_BITS         0x4
#define CMP_NO_COMMON_BITS      0x5

// TODO: documentation
// atk76 various
#define VARIOUS_CANCEL_MULTI_TURN_MOVES         0
#define VARIOUS_SET_MAGIC_COAT_TARGET           1
#define VARIOUS_IS_RUNNING_IMPOSSIBLE           2
#define VARIOUS_GET_MOVE_TARGET                 3
#define VARIOUS_CASE_4                          4
#define VARIOUS_RESET_INTIMIDATE_TRACE_BITS     5
#define VARIOUS_UPDATE_CHOICE_MOVE_ON_LVL_UP    6
#define VARIOUS_RESET_PLAYER_FAINTED_FLAG                          7
#define VARIOUS_CASE_8                          8
#define VARIOUS_RETURN_OPPONENT_MON1            9
#define VARIOUS_RETURN_OPPONENT_MON2            10
#define VARIOUS_CHECK_POKEFLUTE                 11
#define VARIOUS_WAIT_FANFARE                    12

// atk80 dmg manipulation
#define ATK80_DMG_CHANGE_SIGN                               0
#define ATK80_DMG_HALF_BY_TWO_NOT_MORE_THAN_HALF_MAX_HP     1
#define ATK80_DMG_DOUBLED                                   2

// atk4F a flag used for the jumpifcantswitch command
#define ATK4F_DONT_CHECK_STATUSES   0x80

// statchange defines
#define STAT_CHANGE_BS_PTR                  0x1
#define STAT_CHANGE_NOT_PROTECT_AFFECTED    0x20

// atk48
#define ATK48_STAT_NEGATIVE         0x1
#define ATK48_STAT_BY_TWO           0x2
#define ATK48_ONLY_MULTIPLE         0x4
#define ATK48_DONT_CHECK_LOWER      0x8
#define ATK48_ONLY_TRIPLE			0x10

#define BIT_HP                      0x1
#define BIT_ATK                     0x2
#define BIT_DEF                     0x4
#define BIT_SPEED                   0x8
#define BIT_SPATK                   0x10
#define BIT_SPDEF                   0x20
#define BIT_ACC                     0x40
#define BIT_EVASION                 0x80

#define EQUALS 0x0
#define NOTEQUALS 0x1
#define GREATERTHAN 0x2
#define LESSTHAN 0x3
#define ANDS 0x4
#define NOTANDS 0x5

#define DELAY_1SECOND 0x40
#define DELAY_HALFSECOND 0x20

#define STAT_TARGET 0x0
#define STAT_BS_PTR 0x1
#define STAT_NOT_PROTECT_AFFECTED 0x20
#define STAT_ATTACKER 0x40
#define STAT_CERTAIN 0x80

#define INCREASE_1 0x10
#define INCREASE_2 0x20
#define INCREASE_3 0x30
#define INCREASE_4 0x40
#define INCREASE_5 0x50
#define INCREASE_6 0x60

#define DECREASE_1 0x90
#define DECREASE_2 0xA0
#define DECREASE_3 0xB0
#define DECREASE_4 0xC0
#define DECREASE_5 0xD0
#define DECREASE_6 0xE0

#define STAT_MIN 0x0
#define STAT_MAX 0xC

#define STAT_ANIM_HP 0x1
#define STAT_ANIM_ATK 0x2
#define STAT_ANIM_DEF 0x4
#define STAT_ANIM_SPD 0x8
#define STAT_ANIM_SPATK 0x10
#define STAT_ANIM_SPDEF 0x20
#define STAT_ANIM_ACC 0x40
#define STAT_ANIM_EVSN 0x80

#define STAT_ANIM_UP 0x0
#define STAT_ANIM_DOWN 0x1
#define STAT_ANIM_BY_TWO 0x2
#define STAT_ANIM_ONLY_MULTIPLE 0x4
#define STAT_ANIM_IGNORE_ABILITIES 0x8
#define STAT_ANIM_ONLY_TRIPLE 0x10

//side affecting
#define SIDE_REFLECT 0x1
#define SIDE_LIGHTSCREEN 0x2
#define SIDE_CRAFTY_SHIELD 0x8
#define SIDE_SPIKES 0x10
#define SIDE_SAFEGUARD 0x20
#define SIDE_MAT_BLOCK 0x80
#define SIDE_MIST 0x100
#define SIDE_QUICK_GUARD 0x400
#define SIDE_WIDE_GUARD 0x800

#define OUTCOME_MISSED  0x1
#define OUTCOME_SUPER_EFFECTIVE  0x2
#define OUTCOME_NOT_VERY_EFFECTIVE  0x4
#define OUTCOME_ONE_HIT_KO 0x10
#define OUTCOME_FAILED 0x20
#define OUTCOME_ENDURED 0x40
#define OUTCOME_HUNG_ON_USING_ITEM 0x80

#define weather_rain 0x1
#define weather_downpour 0x2
#define weather_permament_rain 0x4
#define weather_sandstorm 0x8
#define weather_permament_sandstorm 0x10
#define weather_sun 0x20
#define weather_permament_sun 0x40
#define weather_hail 0x80
#define weather_permament_hail 0x100
#define weather_fog 0x200
#define weather_permament_fog 0x400
#define weather_harsh_sun 0x800
#define weather_heavy_rain 0x1000
#define weather_air_current 0x2000
#define weather_circus 0x4000

#define TELEKINESIS_TIMERS 0
#define MAGNET_RISE_TIMERS 1
#define HEAL_BLOCK_TIMERS 2
#define LASER_FOCUS_TIMERS 3
#define THROAT_CHOP_TIMERS 4
#define EMBARGO_TIMERS 5
#define ELECTRIFY_TIMERS 6
#define SLOW_START_TIMERS 7
#define STAKOUT_TIMERS 8
#define STOMPING_TANTRUM_TIMERS 9
#define NIMBLE_COUNTERS 10
#define DESTINY_BOND_TIMERS 11
#define METRONOME_COUNTERS 12
#define INCINERATE_COUNTERS 13
#define POWDER_TIMERS 14
#define BEAK_BLAST_TIMERS 15
#define AURORA_VEIL_TIMERS 16
#define TAR_SHOT_TIMERS 17

#define BATTLE_TRAINER 0x8

#define NO_TERRAIN 0
#define ELECTRIC_TERRAIN 1
#define GRASSY_TERRAIN 2
#define MISTY_TERRAIN 3
#define PSYCHIC_TERRAIN 4

#endif // GUARD_CONSTANTS_BATTLE_SCRIPT_COMMANDS_H
