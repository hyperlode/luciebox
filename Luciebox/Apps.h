#ifndef APPS_H
#define APPS_H

#define ENABLE_EEPROM
// // // #define ENABLE_MULTITIMER_STANDALONE_DEPRECATED // DO NOT USE app as a class (most elegant, but takes most memory)
#define ENABLE_MULTITIMER_INTEGRATED
#define ENABLE_SIMON_APP
#define ENABLE_REACTION_APP
#define ENABLE_POMODORO
#define POMODORO_ENABLE_HOURGLASS

#ifdef ENABLE_TILT_SWITCHES
#define ENABLE_TILT_APP
#endif

#define ENABLE_QUIZ_MASTER
#define ENABLE_TALLY_KEEPER

#include "Arduino.h"
#include "SuperTimer.h"
#include "Buzzer.h"
#include "PotentioSelector.h"
#include "ButtonsDacR2r.h"
#include "BinaryInput.h"
#include "PretbakSettings.h"
#include "LedMultiplexer5x8.h"
#include "DisplayDigitsHandler5Digits.h"

#ifdef ENABLE_EEPROM
    #include <avr/eeprom.h>
#endif

#ifdef ENABLE_MULTITIMER_STANDALONE_DEPRECATED
#include "MiniMultiTimer.h"
#endif

#include "DataPlayer.h"
#include "DisplayDigitsHandler5Digits.h"
#include "RotaryEncoderDial.h"
#include "PretbakSettings.h"

#define APP_SELECTOR_SETTING 0
#define APP_SELECTOR_SETTING_TOO 1
//#define APP_SELECTOR_TOTAL_TIME 2
#define APP_SELECTOR_STOPWATCH 2
#define APP_SELECTOR_MULTITIMER 3

#define APP_SELECTOR_LETTERS_AND_CHARS 4
#define APP_SELECTOR_POMODORO 5

#define APP_SELECTOR_RANDOMWORLD 6
#define APP_SELECTOR_TALLY_KEEPER 7
#define APP_SELECTOR_SIMON 8
#define APP_SELECTOR_QUIZ_MASTER 9
#define APP_SELECTOR_REACTION_GAME 10
#define APP_SELECTOR_GUITAR_HERO 11
#define APP_SELECTOR_DRAW 12
#define APP_SELECTOR_DRAW_GAME 13
#define APP_SELECTOR_SOUND_SONG 14
#define APP_SELECTOR_MOVIE_MODE 15
#define APP_SELECTOR_SOUND_METRONOME 16
#define APP_SELECTOR_SOUND_SEQUENCER 17
#define APP_SELECTOR_SOUND_NOTES 18
#define APP_SELECTOR_SOUND_COMPOSER 19
#define APP_SELECTOR_GEIGER 20
#define APP_SELECTOR_HACK_TIME 21
#define APP_SELECTOR_DREAMTIME 22
#ifdef ENABLE_TILT_APP
#define APP_SELECTOR_TILT 23
#else
#define APP_SELECTOR_DREAMTIME_TOO 23
#endif

#define INACTIVITY_TIME_BEEP_INDEX 61  // this is just an index, needs to be converted to seconds with the lookup table

#define DRAW_GAME_RANDOM 0
#define DRAW_GAME_WORD 1
#define DRAW_GAME_NUMBER 2
#define DRAW_GAME_CLOCK 3

#define TILT_FORWARD 0
#define TILT_BACKWARD 1
#define TILT_LEFT 2
#define TILT_RIGHT 3

#define ANIMATION_STOP_CODE_PART_0 0x6D
#define ANIMATION_STOP_CODE_PART_1 0x70
#define ANIMATION_STOP_CODE_PART_2 0x5C
#define ANIMATION_STOP_CODE_PART_3 0xF3

#define RANDOMWORLD_ROLLONEDICE 0
#define RANDOMWORLD_RANDOMLETTER 1
#define RANDOMWORLD_RANDOMNUMBER 2
#define RANDOMWORLD_HEADSORTAILS 3
#define RANDOMWORLD_ROLLFOURDICE 4
#define RANDOMWORLD_TAKERANDOMCARDFROMDECK 5
#define RANDOMWORLD_TOMBOLA 6
#define RANDOMWORLD_YESORNO 7

#define SIMON_NO_ACTIVE_LIGHT 666
#define SIMON_DEAD_PLAYER 666
#define SIMON_MAX_PLAYERS 8
#define SIMON_NO_BUTTON_PRESSED 111

#define POMODORO_INIT_DEFAULT_TIME_INDEX 50                      //1500s
#define POMODORO_PAUSE_DEFAULT_TIME_INDEX 25                     //300s
#define POMODORO_PROBABILITY_BEEP_INTERVAL_DEFAULT_TIME_INDEX 16 //60s
#define POMODORO_DISPLAY_TIMER 0
#define POMODORO_DISPLAY_PAUSE_INIT_SECS 1
#define POMODORO_DISPLAY_SHOW_GOOD 2
#define POMODORO_DISPLAY_SHOW_BAD 3
#define POMODORO_DISPLAY_BEEP_PROBABILITY 4
#define POMODORO_DISPLAY_TIMER_HOURGLASS 5
#define POMODORO_DISPLAY_SHOW_TALLY 6

#define POMODORO_NONSENSE_TIME 60001

#define HACKTIME_MEMORY_FLASH 0
#define HACKTIME_MEMORY_RAM 1
#define HACKTIME_MEMORY_EEPROM 2
#define HACKTIME_DISPLAY_DECIMAL 1
#define HACKTIME_DISPLAY_HEX 2
#define HACKTIME_DISPLAY_BYTES 0
#define HACKTIME_DISPLAY_CHARS 3

#define SOUND_NOTE_DISPLAY_NOTE 0

#define SOUND_NOTE_MODE_MANUAL 0
#define SOUND_NOTE_MODE_ARPEGGIO_UP 1
#define SOUND_NOTE_MODE_ARPEGGIO_DOWN 2
#define SOUND_NOTE_MODE_ARPEGGIO_SAWTOOTH 3
#define SOUND_NOTE_MODE_RANDOM 4
#define SOUND_NOTE_MODE_RANDOM_ERRATIC 5

// VARIABLE REUSE

#define TIMER_METRONOME general_timer
#define TIMER_INIT_APP general_timer
#define SETTINGS_MODE_DISPLAY_VALUES_BLINK general_timer
#define RANDOMWORLD_ROLL_SPEED general_timer
#define COMPOSER_STEP_TIMER general_timer
#define SIMON_STEP_TIMER general_timer
#define STOPWATCH_CHRONO_1 general_timer
// #define POMODORO_MAIN_TIMER general_timer
#define POMODORO_TIMER general_timer
#define DRAW_GAME_DISPLAY_TIMER general_timer
#define HACKTIME_MOVE_TIMER general_timer
#define COUNTING_LETTERS_AND_CHARS_TIMER general_timer
#define SOUND_NOTE_AUTO_TIMER general_timer
#define QUIZ_RANDOM_WAIT_TIME general_timer
#define SEQUENCER_SPEED general_timer
#define TILT_TIMER general_timer
#define MOVIE_MODE_FRAME_INTERVAL_TIMER general_timer
#define TIMER_DREAMTIME general_timer
#define TIMER_REACTION_GAME_SPEED general_timer

#define TIMER_REACTION_END_OF_GAME_DELAY general_timer_2
#define SEQUENCER_EEPROM_MODE_BLINK general_timer_2
#define RANDOMWORLD_AUTODRAW_DELAY general_timer_2
#define SIMON_BLINK_TIMER general_timer_2
#define STOPWATCH_CHRONO_2 general_timer_2
#define POMODORO_PAUSE_TIMER general_timer_2

#define SETTINGS_MODE_SELECTOR general_int16_t_1
#define LETTERS_AND_CHARS_COUNTER general_int16_t_1
#define INIT_SPLASH_ANIMATION_STEP general_int16_t_1
#define SOUND_FUN_NOTE_INDEX general_int16_t_1
#define REACTION_GAME_SCORE general_int16_t_1
#define GEIGER_TONE_FREQUENY_LOWEST general_int16_t_1
#define DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY general_int16_t_1
#define SIMON_LENGTH general_int16_t_1
#define RANDOMWORLD_RANDOM_NUMBER general_int16_t_1
#define SEQUENCER_STEP_COUNTER general_int16_t_1
#define COMPOSER_STEP general_int16_t_1
#define HACKTIME_ADDRESS general_int16_t_1
#define SOUND_NOTES_NOTE_ON_SCALE_INDEX general_int16_t_1
#define QUIZ_ROUNDS_INIT general_int16_t_1
#define METRONOME_TICKER_3_POSITION general_int16_t_1
#define TILT_EXPECTED_SWITCH_INDEX general_int16_t_1
#define POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX general_int16_t_1
#define MOVIE_MODE_FLASH_FRAME_INDEX general_int16_t_1
#define MODE_DREAMTIME_STEP general_int16_t_1
#define TALLY_KEEPER_0 general_int16_t_1
#define MULTITIMER_TIMERS_COUNT general_int16_t_1

#define GEIGER_TONE_FREQUENCY_HEIGHEST general_int16_t_2
#define INIT_SPLASH_LIGHTS_STEP general_int16_t_2
#define REACTION_GAME_TIMER_STEP general_int16_t_2
#define DRAW_ACTIVE_DRAWING_INDEX general_int16_t_2
#define SIMON_INDEX general_int16_t_2
#define RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW general_int16_t_2
#define POMODORO_MAIN_CLOCK_TIME_INDEX general_int16_t_2
#define HACKTIME_DISPLAY_MODE general_int16_t_2
#define SOUND_NOTES_SCALE_INDEX general_int16_t_2
#define METRONOME_TICKER_2_POSITION general_int16_t_2
#define TILT_CYCLE_COUNTER general_int16_t_2
#define MODE_DREAMTIME_STEP_MEMORY general_int16_t_2
#define MOVIE_MODE_MOVIE_FRAME_INDEX_END general_int16_t_2
#define TALLY_KEEPER_1 general_int16_t_2
#define QUIZ_ANALOG_INPUT_SAMPLE_INDEX general_int16_t_2
#define MULTITIMER_FISCHER_TIME_INDEX general_int16_t_2
#define COMPOSER_STAGED_NOTE general_int16_t_2

#define SOUND_NOTES_NOTE_INDEX general_int16_t_3
#define GEIGER_TONE_LENGTH general_int16_t_3
#define METRONOME_TICKER_1_POSITION general_int16_t_3
#define HACKTIME_MEMORY_SELECT general_int16_t_3 // was 5
#define POMODORO_PAUSE_TIME_INDEX general_int16_t_3
#define MOVIE_MODE_SOUNDTRACK_INDEX general_int16_t_3
#define TALLY_KEEPER_2 general_int16_t_3
#define DRAW_CURSOR_ACTIVE_SEGMENT_IN_ACTIVE_DIGIT general_int16_t_3
#define MULTITIMER_DIAL_TIME_INDEX general_int16_t_3

#define DRAW_CURSOR_ACTIVE_DIGIT general_int16_t_4
#define TALLY_KEEPER_3 general_int16_t_4
#define POMODORO_SOUND general_int16_t_4
#define SETTINGS_MODE_ANALOG_VALUE  general_int16_t_4

#define REACTION_GAME_TARGET general_uint16_t_1
#define GEIGER_INCREASE_CHANCE general_uint16_t_1
#define SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET general_uint16_t_1
#define SIMON_ACTIVE_LIGHT general_uint16_t_1
#define SOUND_MODE_SCALE_RANGE_LENGTH general_uint16_t_1
#define MOVIE_MODE_MOVIE_FRAME_INDEX_START general_uint16_t_1
#define POMODORO_STATS_WORKING_BAD general_uint16_t_1

#define SOUND_NOTE_SETTING_TEXT_TO_DISPLAY general_uint16_t_2
#define COUNTER_GEIGER general_uint16_t_2
#define POMODORO_STATS_WORKING_GOOD general_uint16_t_2

#define POMODORO_VISUAL_TIMER_PROGRESS general_uint8_t_1
#define REACTION_GAME_LEVEL general_uint8_t_1
#define HACK_TIME_ACTIVE_VALUE general_uint8_t_1
#define SEQUENCER_TEMP_NOTE general_uint8_t_1
#define RANDOMWORLD_RANDOM_TYPE general_uint8_t_1
#define COMPOSER_SONG_LENGTH general_uint8_t_1
#define SIMON_PLAYERS_ALIVE_COUNT general_uint8_t_1
#define DRAW_CURSOR_DIGIT general_uint8_t_1
#define SOUND_NOTES_PROGRESSION_MODE general_uint8_t_1
#define MODE_DREAMTIME_NOTE_OFFSET general_uint8_t_1
#define TALLY_KEEPER_DISPLAYED_COUNTER general_uint8_t_1
#define INIT_APP_LIGHTS_COUNTER general_uint8_t_1
#define MODE_SETTINGS_DECIMAL_POINT_COUNTER general_uint8_t_1
#define MODE_MULTITIMER_SET_COUNTER_COUNT_SENSITIVITY general_uint8_t_1
#define QUIZ_FIRST_ANALOG_MOMENTARY_BUTTON_NON_ZERO_VALUE general_uint8_t_1

#define SIMON_PLAYERS_COUNT general_uint8_t_2
#define POMODORO_AUTO_RESTART_ENABLED general_uint8_t_2
#define DRAW_SHOW_MODE general_uint8_t_2
#define QUIZ_MOST_RECENT_ROUND_WINNER_INDEX general_uint8_t_2
#define HACKTIME_SOUND general_uint8_t_2
#define RANDOMWORLD_INDEX_FROM_RANDOM_IN_A_BAG general_uint8_t_2
#define REACTION_GAME_HEX_ACTIVE_DIGIT general_uint8_t_2
#define DRAW_ACTIVE_SEGMENT general_uint8_t_2
#define MODE_SETTINGS_CYCLE_TIMING_INDEX general_uint8_t_2
#define SEQUENCER_CHOSEN_NOTE general_uint8_t_2

#define TALLY_KEEPER_TEST general_uint8_t_3
#define DRAW_CURSOR_SEGMENT general_uint8_t_3
#define SIMON_PLAYER_PLAYING_INDEX general_uint8_t_3
#define POMODORO_RANDOM_BEEP_FOR_PERFORMANCE_TRACKING_ENABLED general_uint8_t_3
#define QUIZ_SCORE_MEMORY general_uint8_t_3
#define REACTION_WHACK_A_BIRD_SHOW_NOTES general_uint8_t_3

#define DRAW_GAME_PICTURE_TYPE general_uint8_t_4
#define REACTION_GAME_DECIMAL_POINTS general_uint8_t_4
#define POMODORO_TALLY_TYPE general_uint8_t_4

#define DRAW_CURSOR_INDEX general_long_1
#define GEIGER_PROBABILITY_THRESHOLD general_long_1
#define REACTION_GAME_STEP_TIME_MILLIS general_long_1
#define RANDOMWORLD_ANIMATION_DELAY general_long_1
#define SIMON_ALL_PLAYERS_PLAY_IN_EACH_LEVEL general_long_1
#define STOPWATCH_LAP_MEMORY_1 general_long_1
#define SOUND_NOTES_SCALE_ROOT general_long_1
#define TALLY_KEEPER_DELTA general_long_1
#define QUIZ_MAX_RANDOM_WAIT_TIME general_long_1
#define ZEN_APP_NEXT_ALARM general_long_1
#define SETTINGS_APP_BATTERY_VOLTAGE general_long_1
#define MODE_SETTINGS_CYCLE_TIMING_MILLIS general_long_1

#define STOPWATCH_LAP_MEMORY_2 general_long_2
#define TALLY_KEEPER_DELTA_SIGNED general_long_2
#define MODE_SETTINGS_CYCLE_TIMING_MILLIS_2 general_long_2

#define REACTION_HEX_GUESSED_CORRECTLY general_boolean
#define NUMBERS_AND_LETTERS_COUNT_DOWN_ELSE_UP general_boolean
#define SIMON_CUSTOM_BUILD_UP general_boolean
#define POMODORO_IN_BREAK general_boolean
#define HACKTIME_ADDRESS_COUNTER_EDGE general_boolean
#define SOUND_NOTE_AUTO_UP_ELSE_DOWN general_boolean
#define STOPWATCH_PAUSED_1 general_boolean
#define MOVIE_MODE_RESTART_SOUNDTRACK_AT_MOVIE_START general_boolean
#define MODE_DREAMTIME_FADE_IN_ELSE_FADE_OUT general_boolean
#define METRONOME_ENABLE_FLASH_AT_BEEP general_boolean

#define STOPWATCH_PAUSED_2 general_boolean2
// #define HACKTIME_VALUE_TO_SOUND general_boolean2
#define NUMBERS_AND_LETTERS_NUMBER_ELSE_LETTER_MODE general_boolean2
#define REACTION_GUITAR_APP_SELECT_HERO_ELSE_WHACKING_APP general_boolean2
#define SIMON_END_OF_GAME general_boolean2
#define MOVIE_MODE_SHOW_NEGATIVE general_boolean2
#define SOUND_NOTE_PLAY_NOTE general_boolean2
#define POMODORO_ENABLE_HOURGLASS_VISUALS general_boolean2

#define REACTION_OPTION_WHACKABIRD_OR_HEXHERO general_boolean3
#define MOVIE_MODE_AUTO_BACKWARDS general_boolean3
#define SOUND_NOTE_MUTE general_boolean3
#define POMODORO_FIRST_TICKING_CYCLING_DONE general_boolean3

#define REACTION_OPTION_WHACKENDURANCE_OR_HEROPAUSE_OR_HEXCOMPLEMENT general_boolean4

#define SIMON_LIST bytes_list
#define SEQUENCER_SONG bytes_list
#define FADE_IN_RANDOM_LIST bytes_list
#define CARDS_DECK bytes_list
#define COMPOSER_SONG bytes_list
#define MOVIE_MODE_STOPS bytes_list
#define MODE_SOUND_SONG_BUFFER bytes_list
#define MODE_DREAMTIME_RANDOM_LIST bytes_list
#define REACTION_GAME_TEMP_SELECTED_NOTES bytes_list
#define REACTION_GAME_HEX_COMPLEMENT_WANTED bytes_list
#define QUIZ_ANALOG_VALUES_CHECK bytes_list

#define REACTION_GAME_SELECTED_NOTES array_8_bytes
#define REACTION_GAME_HEX_MEMORY array_8_bytes
#define SIMON_PLAYERS array_8_bytes
#define QUIZ_SCORE array_8_bytes
#define MULTITIMER_INIT_TIME_INDECES array_8_bytes

#ifdef ENABLE_MULTITIMER_INTEGRATED
    const uint16_t timeDialDiscreteSeconds[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 15, 20, 25, 30, 45, 60, 75, 90, 120,
        150, 180, 210, 240, 270, 300, 330, 360, 390, 420,
        450, 480, 510, 540, 570, 600, 660, 720, 780, 840,
        900, 960, 1020, 1080, 1140, 1200, 1260, 1320, 1380, 1440,
        1500, 1560, 1620, 1680, 1740, 1800, 2100, 2400, 2700, 3000,
        3300, 3600, 3900, 4200, 4500, 4800, 5100, 5400, 6000, 6600,
        7200, 7800, 8400, 9000, 9600, 10200, 10800, 12600, 14400, 16200,
        18000, 19800, 21600, 23400, 25200, 27000, 28800, 30600, 32400, 34200,
        36000};

    #define MULTITIMER_MAX_TIMERS_COUNT 4
    #define MULTITIMER_LIGHT_PAUSE 0x01
    #define MULTITIMER_LIGHT_PLAYING 0x02
    #define MULTITIMER_LIGHT_FISCHER 0x04
    #define MULTITIMER_LIGHT_SET_TIMERS_COUNT 0x08
    #define MULTITIMER_LIGHT_SECONDS_BLINKER 0b00010000

#endif

const char dice_eyes_display[] PROGMEM = {
    SPACE_FAKE_ASCII, ONLY_MIDDLE_SEGMENT_FAKE_ASCII, SPACE_FAKE_ASCII, SPACE_FAKE_ASCII,
    ONLY_TOP_SEGMENT_FAKE_ASCII, SPACE_FAKE_ASCII, ONLY_BOTTOM_SEGMENT_FAKE_ASCII, SPACE_FAKE_ASCII,
    ONLY_TOP_SEGMENT_FAKE_ASCII, ONLY_MIDDLE_SEGMENT_FAKE_ASCII, ONLY_BOTTOM_SEGMENT_FAKE_ASCII, SPACE_FAKE_ASCII,
    ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII, SPACE_FAKE_ASCII, ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII, SPACE_FAKE_ASCII,
    ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII, ONLY_MIDDLE_SEGMENT_FAKE_ASCII, ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII, SPACE_FAKE_ASCII,
    ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII, ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII, ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII, SPACE_FAKE_ASCII};

#define SCALES_COUNT 5 - 1

#define LEN_MAJ 7 - 1
#define LEN_MIN 8 - 1
#define LEN_PENT_MAJ 5 - 1
#define LEN_BLUES_MAJ 6 - 1
#define LEN_HARM 12 - 1

#define MAJ 0
#define MIN 7
#define PENT 15
#define BLUES_MAJ 20
#define HARM 26

const uint8_t scale_start_indeces[] PROGMEM = {MAJ, MIN, PENT, BLUES_MAJ, HARM};

const uint8_t scale_lengths[] PROGMEM = {LEN_MAJ, LEN_MIN, LEN_PENT_MAJ, LEN_BLUES_MAJ, LEN_HARM};

const uint8_t scales[] PROGMEM = {
    2, 2, 1, 2, 2, 2, 1,
    2, 1, 2, 2, 1, 2, 2, 1,
    3, 2, 2, 3, 2,
    3, 2, 1, 1, 3, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

// const uint8_t letter_frequency_table_english_dutch_ish[] PROGMEM = {
//     80, //a
//     20, //b
//     15,  //c
//     50, //d
//     140, //e
//     15, //f
//     25, //g
//     50, //h
//     65, //i
//     6, //j
//     10, //k
//     35, //l
//     25, //m
//     85, //n
//     66, //o
//     18, //p
//     1, //q
//     60, //r
//     40, //s
//     80, //t
//     23, //u
//     16, //v
//     19, //w
//     1, //x
//     2, //y
//     8 //z
// };
// scrabble dutch 
const uint8_t letter_frequency_table_english_dutch_ish[] PROGMEM = {
    6, //a
    2, //b
    2,  //c
    5, //d
    18, //e
    2, //f
    3, //g
    2, //h
    4, //i
    2, //j
    3, //k
    3, //l
    3, //m
    10, //n
    6, //o
    2, //p
    1, //q
    5, //r
    5, //s
    5, //t
    3, //u
    2, //v
    2, //w
    1, //x
    1, //y
    2//z
};
// const uint8_t letter_frequency_table_english_dutch_ish[] PROGMEM = {
//     1, //a
//     0, //b
//     0,  //c
//     0, //d
//     0, //e
//     0, //f
//     5, //g
//     0, //h
//     0, //i
//     0, //j
//     0, //k
//     0, //l
//     0, //m
//     0, //n
//     0, //o
//     0, //p
//     0, //q
//     0, //r
//     0, //s
//     0, //t
//     0, //u
//     0, //v
//     0, //w
//     0, //x
//     0, //y
//     1 //z
// };

const uint8_t app_splash_screens[] PROGMEM = {
    //sorted by selector number
    0x0F, 0x40, 0x40, 0x39, //wrench
    // 0x61, 0xc5, 0x51, 0x43, // balancing beam .(tilt)
    0x00, 0xd0, 0xab, 0x44, // stopwatch
    0xc4, 0x88, 0x50, 0x00, // student koala
	
    // 0x63, 0x44, 0x50, 0x63, // abstract battle ground (table with two people on it's side)
    0x5C, 0x63, 0x5c, 0x63, // dice
    0x00, 0x63, 0xDA, 0x63, // abstract face 0x63, 0xDA, 0x6C, 0x63
    0x3d, 0x41, 0x43, 0x02, // gun
    0x40, 0x49, 0x49, 0x4F, //pencil
    0x7F, 0x39, 0x0f, 0x7f, // media player. film roll
    0x00, 0x07, 0x5c, 0x31, // sequencer / metronome: drum kit
    0x5e, 0x01, 0x5f, 0x00, // music notes
    0x00, 0x02, 0x6b, 0x20, // radioactive sign    
	0x63, 0x58, 0x4C, 0x63, // beast face
    // 0x61, 0x43, 0x58, 0x4C, // abstract two half screen circles.
    //0xD8, 0xE9, 0xCB, 0xC4, // abstract turd or tank
    // 0x39, 0x09, 0x09, 0x0F, // abstractbig circle
    //0x40, 0x39, 0x0F, 0x40, // abstract. circle with lines
    // 0x5E, 0x7C, 0x67, 0x73, // abstract.  bolletjes with verticallines
    //0xB9, 0x40, 0x40, 0x0F, // abstract. [--]

    // 0x5e, 0x01, 0xdf, 0x5e, // music notes full
    // 0x32, 0x26, 0x34, 0x16, // vertical lines short and long
    //0x5D, 0x6B, 0x5D, 0x5D, // abstract   camion
    //0x49, 0x49, 0x49, 0x49, // abstract horizontal lines.
    // 0x08, 0xCE, 0x78, 0x08, // abstract piramid
    // 0x40, 0x4F, 0x79, 0x40, // abstract art deco
    // 0x60, 0x3a, 0x2e, 0x42, // body builder. gamer.
    // 0x36, 0x36, 0x36, 0x36, // abstract vertical lines
    // 0x36, 0x36, 0x36, 0x36, // abstract vertical lines

    //0x5F, 0x79, 0x1C, 0x3D // DBUG
};

const uint8_t guitar_hero_level_speeds[] PROGMEM = {101, 75, 50, 40, 30, 20};                 // time in ms per step
const uint8_t whack_a_mole_level_step_speeds[] PROGMEM = {250, 41, 21, 8, 4, 3};              // a x10 multiplier is applied. Time in ms per step, twelve steps for total time. 12x10xTIME_MS = total time for a round
const uint8_t whack_a_mole_countdown_level_step_speeds[] PROGMEM = {200, 100, 50, 33, 16, 8}; //{2min, 1min, 30s, 20s, 10s, 5s} // will have a *50 multiplier per step. and *12 for total countdowntime

#define MOVIE_INDEX_EMPTY 255
#define ANIMATE_CIRCLE_OFFSET 4
#define ANIMATION_INDEX_SEQUENCER_TOP_CIRCLE 56
#define ANIMATION_INDEX_SEQUENCER_BOTTOM_BAR 92
#define SWEEP_OFFSET 112
#define FADE_IN_OFFSET 196

#define MAX_FRAMES_MOVIES_FLASH 84 // important, set the number of total frames (including stop frames)
const uint8_t disp_4digits_animations[] PROGMEM = {
    ANIMATION_STOP_CODE_PART_0, ANIMATION_STOP_CODE_PART_1,ANIMATION_STOP_CODE_PART_2, ANIMATION_STOP_CODE_PART_3,
    0x01, 0x00, 0x00, 0x00,  // animate circle (KEEP IT FIRST, some apps depend on it.)
    0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x08,
    0x00, 0x00, 0x08, 0x00,
    0x00, 0x08, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x00,
    ANIMATION_STOP_CODE_PART_0, ANIMATION_STOP_CODE_PART_1,ANIMATION_STOP_CODE_PART_2, ANIMATION_STOP_CODE_PART_3,
    0x01, 0x00, 0x00, 0x00,  // top circle (for sequencer)
    0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x40, 
    0x00, 0x00, 0x40, 0x00,
    0x00, 0x40, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00,
    ANIMATION_STOP_CODE_PART_0, ANIMATION_STOP_CODE_PART_1,ANIMATION_STOP_CODE_PART_2, ANIMATION_STOP_CODE_PART_3,
    0x08, 0x00, 0x00, 0x00, // bottom bar (for sequencer)
    0x00, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x08,
    ANIMATION_STOP_CODE_PART_0, ANIMATION_STOP_CODE_PART_1,ANIMATION_STOP_CODE_PART_2, ANIMATION_STOP_CODE_PART_3,
    0x00, 0x00, 0x00, 0x00, // horizontal right to left sweep.
    0x00, 0x00, 0x00, 0x06,
    0x00, 0x00, 0x00, 0x49,
    0x00, 0x00, 0x00, 0x30,
    0x00, 0x00, 0x06, 0x00,
    0x00, 0x00, 0x49, 0x00,
    0x00, 0x00, 0x30, 0x00,
    0x00, 0x06, 0x00, 0x00,
    0x00, 0x49, 0x00, 0x00,
    0x00, 0x30, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00,
    0x49, 0x00, 0x00, 0x00,
    0x30, 0x00, 0x00, 0x00, 
    ANIMATION_STOP_CODE_PART_0, ANIMATION_STOP_CODE_PART_1,ANIMATION_STOP_CODE_PART_2, ANIMATION_STOP_CODE_PART_3,
    0x00, 0x00, 0x00, 0x00, // vertical swoop
    0x01, 0x01, 0x01, 0x01,
    0x22, 0x22, 0x22, 0x22,
    0x40, 0x40, 0x40, 0x40,
    0x14, 0x14, 0x14, 0x14,
    0x08, 0x08, 0x08, 0x08, 
    ANIMATION_STOP_CODE_PART_0, ANIMATION_STOP_CODE_PART_1,ANIMATION_STOP_CODE_PART_2, ANIMATION_STOP_CODE_PART_3,
    0x00, 0x00, 0x00, 0x00, // sweep in
    0x21, 0x00, 0x00, 0x00,
    0x71, 0x00, 0x00, 0x00,
    0x7B, 0x00, 0x00, 0x00,
    0x7F, 0x00, 0x00, 0x00,
    0x7F, 0x21, 0x00, 0x00,
    0x7F, 0x71, 0x00, 0x00,
    0x7F, 0x7B, 0x00, 0x00,
    0x7F, 0x7F, 0x00, 0x00,
    0x7F, 0x7F, 0x21, 0x00,
    0x7F, 0x7F, 0x71, 0x00,
    0x7F, 0x7F, 0x7B, 0x00,
    0x7F, 0x7F, 0x7F, 0x00,
    0x7F, 0x7F, 0x7F, 0x21,
    0x7F, 0x7F, 0x7F, 0x71,
    0x7F, 0x7F, 0x7F, 0x7B,
    0x7F, 0x7F, 0x7F, 0x7F, // horizonal sweep.sweep out
    0x5E, 0x7F, 0x7F, 0x7F,
    0x0E, 0x7F, 0x7F, 0x7F,
    0x04, 0x7F, 0x7F, 0x7F,
    0x00, 0x7F, 0x7F, 0x7F,
    0x00, 0x5E, 0x7F, 0x7F,
    0x00, 0x0E, 0x7F, 0x7F,
    0x00, 0x04, 0x7F, 0x7F,
    0x00, 0x00, 0x7F, 0x7F,
    0x00, 0x00, 0x5E, 0x7F,
    0x00, 0x00, 0x0E, 0x7F,
    0x00, 0x00, 0x04, 0x7F,
    0x00, 0x00, 0x00, 0x7F,
    0x00, 0x00, 0x00, 0x5E,
    0x00, 0x00, 0x00, 0x0E,
    0x00, 0x00, 0x00, 0x04,
    ANIMATION_STOP_CODE_PART_0, ANIMATION_STOP_CODE_PART_1,ANIMATION_STOP_CODE_PART_2, ANIMATION_STOP_CODE_PART_3,
};

#define SONG_DRYER_HAPPY 0
#define SONG_LANG_ZAL_ZE_LEVEN 1
#define SONG_ATTACK 2
#define SONG_STARGOOSE 3
#define SONG_DRYER_UNHAPPY 4
#define SONG_KINDEKE_DOUWEN 5
#define SONG_RETREAT 6
#define SONG_ALPHABET 7

#define LEN_SONG_DRYER_HAPPY 12
#define LEN_LANG_ZAL_ZE_LEVEN 73
#define LEN_SONG_ATTACK 13
#define LEN_SONG_STARGOOSE 51
#define LEN_SONG_DRYER_UNHAPPY 11
#define LEN_SONG_KINDEKE_DOUWEN 39
#define LEN_SONG_RETREAT 14
#define LEN_SONG_ALPHABET 77

// #define INDEX_SONG_DRYER_HAPPY 0
// #define INDEX_LANG_ZAL_ZE_LEVEN     LEN_SONG_DRYER_HAPPY
// #define INDEX_SONG_ATTACK           INDEX_LANG_ZAL_ZE_LEVEN + LEN_LANG_ZAL_ZE_LEVEN
// #define INDEX_SONG_STARGOOSE        INDEX_SONG_ATTACK + LEN_SONG_ATTACK
// #define INDEX_SONG_DRYER_UNHAPPY    INDEX_SONG_STARGOOSE + LEN_SONG_STARGOOSE
// #define INDEX_SONG_KINDEKE_DOUWEN   INDEX_SONG_DRYER_UNHAPPY + LEN_SONG_DRYER_UNHAPPY
// #define INDEX_SONG_RETREAT          INDEX_SONG_KINDEKE_DOUWEN + LEN_SONG_KINDEKE_DOUWEN
// #define INDEX_SONG_ALPHABET         INDEX_SONG_RETREAT + LEN_SONG_RETREAT

#define SONGS_FLASH_COUNT 8

// const uint8_t song_start_indeces [] PROGMEM = {
//     INDEX_SONG_DRYER_HAPPY,
//     INDEX_LANG_ZAL_ZE_LEVEN,
//     INDEX_SONG_ATTACK,
//     INDEX_SONG_STARGOOSE,
//     INDEX_SONG_DRYER_UNHAPPY,
//     INDEX_SONG_KINDEKE_DOUWEN,
//     INDEX_SONG_RETREAT,
//     INDEX_SONG_ALPHABET    
// };

const uint8_t song_lengths [] PROGMEM = {
    LEN_SONG_DRYER_HAPPY,
    LEN_LANG_ZAL_ZE_LEVEN,
    LEN_SONG_ATTACK,
    LEN_SONG_STARGOOSE,
    LEN_SONG_DRYER_UNHAPPY,
    LEN_SONG_KINDEKE_DOUWEN,
    LEN_SONG_RETREAT,
    LEN_SONG_ALPHABET
};
 
// one big library. song lengths in separate array. This is the easiest option.
const uint8_t songs [] PROGMEM = {
    //happy dryer
    A6_2, REST_6_8,
    Cs7_2, REST_6_8,
    E7_4, REST_3_8,
    Cs7_4, REST_3_8,
    E7_4, REST_3_8,
    A7_1, A7_1,
 
    
    // lang zal ze leven
    C7_4, REST_6_8,
    C7_4, REST_4_8, C7_8, REST_1_8,
    C7_4, REST_6_8,
    G6_2, G6_4, REST_4_8,

    E7_4, REST_6_8,
    E7_4, REST_4_8, E7_8, REST_1_8,
    E7_4, REST_6_8,
    C7_2, C7_4, REST_4_8,

    G7_4, REST_6_8,
    G7_4, REST_4_8, G7_8, REST_1_8,

    A7_8, REST_3_8,
    G7_8, REST_3_8,

    F7_8, REST_3_8,
    E7_8, REST_3_8,

    D7_2, D7_4, REST_2_8,
    D7_2, D7_4, REST_2_8,
    D7_2, D7_4, REST_2_8,
    REST_2_8,
    G7_8, REST_3_8,
    F7_8, REST_3_8,

    E7_1, E7_2, REST_4_8,
    F7_1, F7_2, REST_4_8,

    G7_1, G7_2, REST_4_8,
    A7_2, A7_4, REST_2_8,
    F7_2, F7_4, REST_2_8,

    E7_1, E7_2, REST_4_8,
    D7_1, D7_2, REST_4_8,
    C7_1, C7_1,
    
    // aaanvallueeeeee!
    Gs6_2, REST_2_8, Gs6_2, REST_2_8, Gs6_2, REST_2_8, Cs7_2, REST_8_8, Gs6_2, REST_2_8, Cs7_1, Cs7_1, Cs7_1, 
    
    // star goose
    C6_2, REST_4_8,
    E6_2, E6_4, REST_2_8,
    C6_2, REST_4_8,
    E6_2, E6_4, REST_2_8,
    C6_2, REST_4_8,
    E6_2, E6_4,  REST_2_8,
    C6_2, REST_4_8,
    E6_2, E6_4, REST_2_8,
    
    C6_2, REST_4_8,
    A6_1, REST_4_8,
    G6_4, REST_6_8,
    G6_1, REST_2_8,

    C6_2, REST_4_8,
    E6_2, E6_4, REST_2_8,
    C6_2, REST_4_8,
    E6_2, E6_4, REST_2_8,

    C6_2, REST_4_8,
    F6_2, F6_4, REST_2_8,
    E6_2, REST_4_8,
    C6_2, C6_4, REST_4_8,
    D6_4,REST_2_8,
    C6_1,
    
    // unhappy dryer
    A6_1, REST_4_8, Cs7_1, REST_4_8, E7_2, REST_2_8, Cs7_2, REST_2_8, B6_2, REST_2_8, A6_1,
    
    // kindeke douwen
    B7_4, REST_6_8,       // wie
    B7_1,                       //zal
    REST_4_8, A7_4, REST_2_8,       //er
    G6_4, REST_6_8,       //ons
    B7_1,                       //kind-
    B7_4, REST_2_8, A7_4, REST_2_8, //-de
    G6_4, REST_6_8,       //-ke
    A7_1,                       //dou
    A7_4, REST_6_8,
    C7_4, REST_6_8,
    C7_1, //wen
    REST_8_8,
    C7_4, REST_6_8, // dat
    B7_1,                 // schaars
    B7_4, REST_2_8, B7_4, REST_2_8,
    B7_4, REST_6_8,

    E7_4, REST_6_8, //
    E7_1,
    E7_4, REST_2_8, E7_4, REST_2_8,
    A7_4,
    
    // retreat song
    Gs6_2, REST_2_8, Gs6_2, REST_2_8, Gs6_2, REST_2_8, Gs6_2, REST_4_8, REST_4_8, Gs6_2, REST_2_8, Cs6_1, Cs6_1, Cs6_1,
    
    // alphabet song
    C7_2, REST_12_8, 
    C7_2, REST_12_8, 
    G7_2, REST_12_8, 
    G7_2, REST_12_8, 
    A7_2, REST_12_8, 
    A7_2, REST_12_8, 
    G7_1, G7_1, REST_15_8,
    F7_2, REST_12_8, F7_2, REST_12_8, E7_2, REST_12_8, E7_2, REST_12_8, D7_2, REST_4_8, D7_2, REST_4_8, D7_2, REST_4_8, D7_2, REST_4_8, C7_1, C7_1, REST_15_8,
    G7_2, REST_12_8, G7_2, REST_12_8, F7_2, REST_12_8, F7_2, REST_12_8, E7_2, REST_12_8, E7_2, REST_12_8, D7_1, D7_1, REST_15_8, C7_2, REST_12_8,
    C7_2, REST_12_8, G7_2, REST_12_8, G7_2, REST_12_8, A7_1, A7_1, REST_15_8, G7_2, REST_13_8, REST_15_8,
    F7_2, REST_12_8, F7_2, REST_12_8, E7_2, REST_12_8, E7_2, REST_12_8, D7_2, REST_12_8, G7_2, REST_12_8, C7_1, C7_1
};


const uint8_t lights_indexed[] = {LIGHT_MOMENTARY_0, LIGHT_MOMENTARY_1, LIGHT_MOMENTARY_2, LIGHT_MOMENTARY_3,
                                  LIGHT_LATCHING_0, LIGHT_LATCHING_1, LIGHT_LATCHING_2, LIGHT_LATCHING_3};

#ifdef V2_PCB_CORONA
const uint8_t lights_indexed_as_installed[] = {LIGHT_LATCHING_0, LIGHT_LATCHING_1, LIGHT_LATCHING_2, LIGHT_MOMENTARY_0,
                                               LIGHT_MOMENTARY_1, LIGHT_MOMENTARY_2, LIGHT_MOMENTARY_3, LIGHT_LATCHING_3};

#elif defined V3_PCB_BURST
const uint8_t lights_indexed_as_installed[] = {LIGHT_LATCHING_0, LIGHT_LATCHING_1, LIGHT_LATCHING_2, LIGHT_LATCHING_3, LIGHT_MOMENTARY_3,
                                               LIGHT_MOMENTARY_2, LIGHT_MOMENTARY_1, LIGHT_MOMENTARY_0};

#endif

const uint8_t buttons_indexed[] = {BUTTON_MOMENTARY_0, BUTTON_MOMENTARY_1, BUTTON_MOMENTARY_2, BUTTON_MOMENTARY_3,
                                   BUTTON_LATCHING_0, BUTTON_LATCHING_1, BUTTON_LATCHING_2, BUTTON_LATCHING_3};

#ifdef ENABLE_TILT_SWITCHES
const uint8_t mercury_switches_indexed[] = {SWITCH_TILT_FORWARD, SWITCH_TILT_LEFT, SWITCH_TILT_BACKWARD, SWITCH_TILT_RIGHT};
#endif

class Apps
{

public:
    Apps();

   

    void setPeripherals(
        BinaryInput *binaryInput,
        RotaryEncoderDial *encoder_dial,
        DisplayManagement *ledDisp,
        LedMultiplexer5x8 *allLights,
        Buzzer *buzzer,
        PotentioSelector *selectorDial);
    void eraseEepromRangeLimited(uint8_t setting);

    void initializeAppDataToDefault();
    void appSelector();

private:

    // all applitcations
    bool init_app(bool init, uint8_t selector);
    void modeSettings();
#ifdef ENABLE_TALLY_KEEPER
    void modeTallyKeeper();
#endif
#ifdef ENABLE_QUIZ_MASTER
    void quiz();
#endif
    void modeTotalTime();
    void modeCountingLettersAndChars();
    void modeSoundSong();
    void modeComposeSong();
    void modeSoundNotesInitScale();
    void modeSoundNotes();
    void draw();
    void drawGame();
    void modeMovie();
#ifdef ENABLE_POMODORO
    void pomodoroTimer();
#endif

    void stopwatch();
    void modeGeiger();
    void modeSequencer();
    void modeMetronome();
    void modeSimon();
    void modeReactionGame();
    void tiltSwitchTest();
    void modeDreamtime();
    void modeButtonDebug();
#ifdef ENABLE_MULTITIMER_STANDALONE_DEPRECATED
    void miniMultiTimer();
#endif
    void modeRandomWorld();
    void modeHackTime();
#ifdef ENABLE_MULTITIMER_INTEGRATED
    void multitimer_integrated();
#endif

    // application helper methods
    // void pomodoroScoreValueManipulator(uint16_t* score, uint8_t buttonIndexIncrease, uint8_t buttonView);
    void modeMetronomeTickerUpdate(int16_t *ticker_counter, uint8_t momentary_id, bool direction, uint8_t sound_at_zero_pass, boolean force_step);
#ifdef ENABLE_MULTITIMER_INTEGRATED
    void multitimer_setDefaults();
    void multitimer_setTimerInitCountTimeByTimeIndex(uint8_t timer, uint8_t index);
    uint8_t multitimer_getTimerInitTimeIndex(uint8_t timer);
    void multitimer_getDisplay();
    bool multitimer_getTimerFinished(uint8_t timerIndex);
    bool multitimer_checkAllTimersFinished();
    void multitimer_playerButtonPressEdgeUp(uint8_t index);
    // void multitimer_playerButtonPressEdgeDown(uint8_t index);
    // void multitimer_setTimersCount(int8_t delta);
    // void multitimer_setStateTimersCount(bool set);
    // void multitimer_setStatePause(bool set);
    void multitimer_refresh();
    void multitimer_buzzerRefresh(bool alarm);
    void multitimer_next(bool activePlayerDied);
    void multitimer_init();
    void multitimer_start();
    void multitimer_pause();
    void multitimer_continu();
    void multitimer_reset();
#endif

    void updateEveryAppCycleBefore();
    void updateEveryAppCycleAfter();
    uint32_t modeSingleSegmentManipulation(uint32_t *display_buffer);
    void displayChangeGlobal(uint32_t *display_buffer);
    void randomModeTrigger(bool forReal);
    void geigerToneHelper();
    void loadNextMovie();

    uint16_t* reactionGameLevelToEepromAddress();
    void resetStopwatch(SuperTimer *pTimer);
    unsigned int indexToTimeSeconds(int16_t index);
    bool saveLoadMenu(uint8_t *data, uint8_t slotCount, uint8_t eepromSlotLength, uint16_t eepromStartAddress);
    void eepromPictureToDisplayAllSegments(int16_t pictureIndex);
    void flashPictureToDisplayAllSegments(const uint8_t *progmemAddress);
    bool loadMovieFrame(int16_t address);
    void displayLetterAndPositionInAlphabet(char *textBuf, int16_t letterValueAlphabet);
    bool modifyValueUpDownWithMomentary2And3(int16_t *value);
    uint32_t fadeInList(uint8_t step, uint32_t startScreen, uint8_t *shuffledSequence);

    // general methods

    void dialOnEdgeChangeInitTimerPercentage(SuperTimer *aTimer);
    void encoderDialRefreshTimeIndex(int16_t *indexHolder);
    uint16_t dialGetIndexedtime();
    void dialMultiplyValueAndDisplay(int16_t *pVariable, uint8_t multiplier, int16_t maxValue);
    
    uint8_t tombola(uint8_t *indexVariable, uint8_t *sequenceList, uint8_t length);
    void randomSequence(uint8_t *sequenceList, uint8_t length);
    void shuffle(uint8_t *list, uint8_t length);
    void fill8BytesArrayWithZero();
    uint8_t weightedRandomLetter();
    
    void nextStepRotate(int16_t *counter, bool countUpElseDown, int16_t minValue, int16_t maxValue);
    void nextStep(int16_t *counter, bool countUpElseDown, int16_t minValue, int16_t maxValue, bool wrapAround);
    bool stepChange(int16_t *counter, int8_t increment, int16_t minValue, int16_t maxValue, bool wrapAround);
    bool checkBoundaries(int16_t *counter, int16_t maxValue, int16_t minValue, bool rotate);
    
    void latching_3_blink();
    bool millis_second_period();
    bool millis_half_second_period();
    bool millis_quarter_second_period();
    bool millis_blink_250_750ms();
    void set_blink_offset();

    void saveLoadFromEepromSlot(uint8_t *data, uint8_t slotIndex, uint8_t eepromSlotLength, uint16_t eepromStartAddress, boolean loadElseSave);
    void eepromCopyValues(uint16_t fromAddress, uint16_t toAddress);

    void progmemToBuffer(const uint8_t *offset, uint8_t length);
    uint8_t progmemToBufferUntil(const uint8_t *offset, uint8_t stopConditionValue);

    void setButtonLight(uint8_t light_index, bool onElseOff);
    void textBufToDisplay();
    void textBufToDisplayAllSegments();
    void displayAllSegmentsToScreen();
    void setBlankDisplay();
    // void setLedArray();
    void numberToBufAsDecimal(int16_t number);
    void noteToDisplay(uint8_t note);
    void setStandardTextToTextBuf(uint8_t textPosition);
    void setStandardTextToTextHANDLE(uint8_t textPosition);

    //void decimalPointClockPositionOn();
    void displayTimerSecondsBlinker(SuperTimer* timerToBlink);
    void setDecimalPoint(bool onElseOff, uint8_t digit);
    bool getCountDownTimerHasElapsed(SuperTimer* timerToBlink);
    void initiateCountDowntimerWith500Millis(SuperTimer* ptimer);

    void buzzerPlayApproval();
    void buzzerPlayDisappointment();
    void addNoteToBuzzer(uint8_t note);
    void addNoteToBuzzerRepeated(uint8_t note, uint8_t repeater);
    void buzzerSilentClearBuffer();
    void buzzerSilentClearBufferAndAddNote(uint8_t note);
    void buzzerChangeSpeedRatioWithEncoderDial();
    void buzzerSilentClearBufferAndAddNoteAtEncoderDialChange(uint8_t note);
    void loadBuzzerTrack(uint8_t songIndex);
    void playSongHappyDryer();
	long readVcc();
	

    //reused variables per app
    SuperTimer general_timer;
    SuperTimer general_timer_2;

    bool general_boolean;
    bool general_boolean2;
    bool general_boolean3;
    bool general_boolean4;

    int16_t general_int16_t_1;
    int16_t general_int16_t_2;
    int16_t general_int16_t_3;
    int16_t general_int16_t_4;

    uint16_t general_uint16_t_1;
    uint16_t general_uint16_t_2;

    uint8_t general_uint8_t_1;
    uint8_t general_uint8_t_2;
    uint8_t general_uint8_t_3;
    uint8_t general_uint8_t_4;

    long general_long_1;
    long general_long_2;

    // specific 
    SuperTimer inactivity_timer;
    SuperTimer always_on_timer;
    
    BinaryInput *binaryInputs;
    DisplayManagement *ledDisp;
    Buzzer *buzzer;
    RotaryEncoderDial *encoder_dial;
    LedMultiplexer5x8 *allLights;
    PotentioSelector *selectorDial;
#ifdef ENABLE_MULTITIMER_STANDALONE_DEPRECATED
    MiniMultiTimer multiTimer;
#endif
	long batteryVoltage;
    long* pLongValue;
    long blink_offset;
    SuperTimer* pSsuperTimer;
    uint32_t displayAllSegments;
    uint32_t displayAllSegmentsBuffer;
    byte binaryInputsValue;
    byte binaryInputsEdgeUp;
    byte binaryInputsEdgeDown;
    char textBuf[4];
    // char textBuf2[4];
    char *textHandle;        // contains the text for the display. (4 chars)
    uint8_t decimalPoints;   // segment 4 = bit 3, ....   00043210 (segment number)
    byte *decimalDotsHandle; // segment 4 = bit 3, ....   00043210 (segment number)
    uint8_t lights;
    bool splash_screen_playing; // actual flash screen app
    bool app_init_edge;         // one cycle
    uint8_t selected_app;
#ifdef ENABLE_MULTITIMER_INTEGRATED
    SuperTimer multitimer_timers[MULTITIMER_MAX_TIMERS_COUNT];
    uint8_t multitimer_activeTimer;
    uint8_t multitimer_timerDisplayed;
    bool multitimer_randomStarter;
    bool multitimer_fisherTimer;
    enum multitimer_state : uint8_t
    {
        // enum state{
        initialized = 0,
        playing,
        finished,
        paused,
        setTimers,
        setFischer
    };
    multitimer_state multitimer_state;
#endif

    enum
    {
        bytes_list_bufsize = 100,
    };
    uint8_t bytes_list[bytes_list_bufsize];

    uint8_t array_8_bytes[8];

    enum DrawGameState : uint8_t
    {
        drawGameWaitForStart,
        drawGameShowPicture,
        drawGameDraw,
        drawGameEvaluate,
    };
    DrawGameState drawGameState;

#ifdef ENABLE_REACTION_APP
    // reaction
    enum ReactionGameState : uint8_t
    {
        reactionWaitForStart,
        reactionNewGame,
        reactionNewTurn,
        reactionWaitBeforeNewTurn,
        reactionPlaying,
        reactionJustDied,
        reactionFinished,
        reactionGuitarHeroNewTurn,
        reactionGuitarHeroPlaying,
        reactionHexNextStep,
        reactionHexWaitForButtonsRelease,
        reactionHexPlaying,
        reactionSoundInit

    };
    ReactionGameState reactionGameState;
#endif

    enum QuizState : uint8_t
    {
        // quizInit,
        quizWaitForQuizMaster,
        quizWaitRandomTime,
        quizWaitPlayerPress,
        quizDefineRoundWinner,
        quizWaitSomeTimeForNextRound,
        quizRoundAfterMath
    };
    QuizState quizState;

    enum RandomWorldState : uint8_t
    {
        randomWorldIdle,
        randomWorldShowResult,
        randomWorldRolling,
        randomWorldInstantRoll,
        randomWorldRollingEnd,
        randomWorldAutoRollDelay,
    };
    RandomWorldState randomWorldState;

    // enum PomodoroState: uint8_t 
    // {
    //     pomoSetMainTime,
    //     pomoSetPauseTime,
    //     pomoSetRandomBeepInterval,
    //     pomoStartMain,
    //     pomoMainTicking,
    //     pomoStartPause,
    //     pomoPauseTicking,
    //     pomoEndOfCycle
    // };
    // PomodoroState pomodoroState;

#ifdef ENABLE_SIMON_APP
    // simon
    enum SimonState : uint8_t
    {
        simonWaitForNewGame,
        simonNewGame,
        simonNewLevel,
        simonStartPlaySequence,
        simonPlaySequence,
        simonStartUserRepeats,
        simonUserRepeats,
        simonNextPlayer,
        simonPlayerDead,
    };
    SimonState simonState;

#endif


};

#endif
