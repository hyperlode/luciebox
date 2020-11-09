#ifndef APPS_H
#define APPS_H

#define ENABLE_EEPROM
#define ENABLE_MULTITIMER
#define ENABLE_SIMON_APP
#define ENABLE_REACTION_APP
#define ENABLE_TILT_SWITCHES

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

#ifdef ENABLE_MULTITIMER
#include "MiniMultiTimer.h"
#endif

#include "DataPlayer.h"
#include "DisplayDigitsHandler5Digits.h"
#include "RotaryEncoderDial.h"
#include "PretbakSettings.h"

#define APP_SELECTOR_LETTERS_AND_CHARS 0
#define APP_SELECTOR_SIMON 1
#define APP_SELECTOR_SOUND_NOTES 2
#define APP_SELECTOR_SOUND_COMPOSER 3
#define APP_SELECTOR_STOPWATCH 4
#define APP_SELECTOR_POMODORO 5
#define APP_SELECTOR_RANDOMWORLD 6
#define APP_SELECTOR_QUIZ 7
#define APP_SELECTOR_GEIGER 8
#define APP_SELECTOR_HACK_TIME 9
#define APP_SELECTOR_SOUND_SONG 10
#define APP_SELECTOR_MOVIE_MODE 11
#define APP_SELECTOR_DRAW 12
#define APP_SELECTOR_DRAW_GAME 13
#define APP_SELECTOR_SETTING 14
#define APP_SELECTOR_SETTING_TOO 15
#define APP_SELECTOR_SOUND_METRONOME 16
#define APP_SELECTOR_SOUND_SEQUENCER 17
#define APP_SELECTOR_REACTION_GAME 18
#define APP_SELECTOR_GUITAR_HERO 19
#define APP_SELECTOR_TILT 20
#define APP_SELECTOR_DREAMTIME 21
#define APP_SELECTOR_MULTITIMER_SETTING 22
#define APP_SELECTOR_MULTITIMER_PLAYING 23

#define TILT_FORWARD 0
#define TILT_BACKWARD 1
#define TILT_LEFT 2
#define TILT_RIGHT 3

#define ANIMATION_STOP_CODE_PART_0 0x6D
#define ANIMATION_STOP_CODE_PART_1 0x70
#define ANIMATION_STOP_CODE_PART_2 0x5C
#define ANIMATION_STOP_CODE_PART_3 0xF3

#define RANDOMWORLD_ROLLONEDICE 0
#define RANDOMWORLD_ROLLFOURDICE 4
#define RANDOMWORLD_RANDOMLETTER 1
#define RANDOMWORLD_TAKERANDOMCARDFROMDECK 5
#define RANDOMWORLD_RANDOMNUMBER 2
#define RANDOMWORLD_TOMBOLA 6
#define RANDOMWORLD_HEADSORTAILS 3
#define RANDOMWORLD_YESORNO 7

#define SIMON_NO_ACTIVE_LIGHT 666
#define SIMON_DEAD_PLAYER 666
#define SIMON_MAX_PLAYERS 8
#define SIMON_NO_BUTTON_PRESSED 111

#define POMODORO_INIT_DEFAULT_TIME_INDEX 50 //1500s
#define POMODORO_PAUSE_DEFAULT_TIME_INDEX 25 //300s
#define POMODORO_PROBABILITY_BEEP_INTERVAL_DEFAULT_TIME_INDEX 16 //60s
#define POMODORO_DISPLAY_TIMER 0
#define POMODORO_DISPLAY_PAUSE_INIT_SECS 1
#define POMODORO_DISPLAY_SHOW_GOOD 2
#define POMODORO_DISPLAY_SHOW_BAD 3
#define POMODORO_DISPLAY_BEEP_PROBABILITY 4
#define POMODORO_NONSENSE_TIME 60001

#define HACKTIME_MEMORY_FLASH 0
#define HACKTIME_MEMORY_RAM 1
#define HACKTIME_MEMORY_EEPROM 2
#define HACKTIME_DISPLAY_CHARS 0
#define HACKTIME_DISPLAY_BYTES 1
#define HACKTIME_DISPLAY_DECIMAL 2
#define HACKTIME_DISPLAY_HEX 3

#define SOUND_NOTE_DISPLAY_NOTE 0 
#define SOUND_NOTE_MODE_MANUAL 0 
#define SOUND_NOTE_MODE_ARPEGGIO_UP 1
#define SOUND_NOTE_MODE_ARPEGGIO_DOWN 2
#define SOUND_NOTE_MODE_ARPEGGIO_SAWTOOTH 3
#define SOUND_NOTE_MODE_RANDOM 4
#define SOUND_NOTE_MODE_RANDOM_ERRATIC 5

// VARIABLE REUSE

#define TIMER_METRONOME generalTimer
#define TIMER_INIT_APP generalTimer
#define TIMER_REACTION_GAME_RESTART_DELAY generalTimer
#define SETTINGS_MODE_DISPLAY_VALUES_BLINK generalTimer
#define RANDOMWORLD_ROLL_SPEED generalTimer
#define COMPOSER_STEP_TIMER generalTimer
#define SIMON_STEP_TIMER generalTimer
#define STOPWATCH_CHRONO_1 generalTimer
#define POMODORO_TIMER generalTimer
#define DRAW_GAME_DISPLAY_TIMER generalTimer
#define HACKTIME_MOVE_TIMER generalTimer
#define COUNTING_LETTERS_AND_CHARS_TIMER generalTimer
#define SOUND_NOTE_AUTO_TIMER generalTimer
#define QUIZ_RANDOM_WAIT_TIME generalTimer
#define SEQUENCER_SPEED generalTimer
#define TILT_TIMER generalTimer
#define MOVIE_MODE_FRAME_INTERVAL_TIMER generalTimer
#define TIMER_DREAMTIME generalTimer

#define TIMER_REACTION_GAME_SPEED generalTimer2
#define TIMER_REACTION_END_OF_GAME_DELAY generalTimer2
#define SAVE_LOAD_MENU_BLINK_TIMER generalTimer2
#define SEQUENCER_EEPROM_MODE_BLINK generalTimer2
#define RANDOMWORLD_AUTODRAW_DELAY generalTimer2
#define SIMON_BLINK_TIMER generalTimer2
#define STOPWATCH_CHRONO_2 generalTimer2

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

#define GEIGER_TONE_FREQUENCY_HEIGHEST general_int16_t_2
#define REACTION_GAME_TIMER_STEP general_int16_t_2
#define DRAW_ACTIVE_DRAWING_INDEX general_int16_t_2
#define SIMON_INDEX general_int16_t_2
#define DEBUGMODE_ACTIVATED general_int16_t_2
#define RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW general_int16_t_2
#define POMODORO_MAIN_CLOCK_TIME_INDEX general_int16_t_2
#define HACKTIME_DISPLAY_MODE general_int16_t_2
#define SOUND_NOTES_SCALE_INDEX general_int16_t_2
#define METRONOME_TICKER_2_POSITION general_int16_t_2
#define TILT_CYCLE_COUNTER general_int16_t_2
#define MODE_DREAMTIME_STEP_MEMORY general_int16_t_2
#define MOVIE_MODE_MOVIE_FRAME_INDEX_END general_int16_t_2

#define DRAW_CURSOR_INDEX general_long_1
#define GEIGER_PROBABILITY_THRESHOLD general_long_1
#define REACTION_GAME_STEP_TIME_MILLIS general_long_1
#define SETTINGS_MODE_SELECTOR general_long_1
#define RANDOMWORLD_ANIMATION_DELAY general_long_1
#define SIMON_RANDOM_PLAYER_SEQUENCE general_long_1
#define STOPWATCH_LAP_MEMORY_1 general_long_1
#define POMODORO_STATS_WORKING_BAD general_long_1
#define SOUND_NOTES_SCALE_ROOT general_long_1

#define REACTION_GAME_TARGET general_uint16_t_1
#define GEIGER_INCREASE_CHANCE general_uint16_t_1
#define SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET general_uint16_t_1
#define SIMON_ACTIVE_LIGHT general_uint16_t_1
#define SOUND_MODE_SCALE_RANGE_LENGTH general_uint16_t_1
#define MOVIE_MODE_MOVIE_FRAME_INDEX_START general_uint16_t_1

#define REACTION_GAME_LEVEL general_uint8_t_1
#define SEQUENCER_TEMP_NOTE general_uint8_t_1
#define RANDOMWORLD_RANDOM_TYPE general_uint8_t_1
#define COMPOSER_SONG_LENGTH general_uint8_t_1
#define SIMON_PLAYERS_ALIVE_COUNT general_uint8_t_1
#define DRAW_CURSOR_DIGIT general_uint8_t_1
#define SOUND_NOTES_PROGRESSION_MODE general_uint8_t_1
#define MODE_DREAMTIME_NOTE_OFFSET general_uint8_t_1

#define SIMON_PLAYERS_COUNT general_uint8_t_2
#define POMODORO_AUTO_RESTART_ENABLED general_uint8_t_2
#define DRAW_SHOW_MODE general_uint8_t_2
#define HACKTIME_SOUND general_uint8_t_2
#define RANDOMWORLD_CARD_FROM_DECK_INDEX general_uint8_t_2
#define REACTION_GAME_HEX_ACTIVE_DIGIT general_uint8_t_2

#define DRAW_CURSOR_SEGMENT general_uint8_t_3
#define SIMON_PLAYER_PLAYING_INDEX general_uint8_t_3
#define POMODORO_RANDOM_BEEP_FOR_PERFORMANCE_TRACKING_ENABLED general_uint8_t_3
#define REACTION_GAME_HEX_VALUE_TO_FIND general_uint8_t_3

#define SOUND_NOTE_SETTING_TEXT_TO_DISPLAY general_uint16_t_2
#define COUNTER_GEIGER general_uint16_t_2


#define SOUND_NOTES_NOTE_INDEX general_int16_t_3
#define GEIGER_TONE_LENGTH general_int16_t_3
#define METRONOME_TICKER_1_POSITION general_int16_t_3
#define HACKTIME_MEMORY_SELECT general_int16_t_3  // was 5
#define POMODORO_PAUSE_TIME_INDEX general_int16_t_3
#define MOVIE_MODE_SOUNDTRACK_INDEX general_int16_t_3

#define STOPWATCH_LAP_MEMORY_2 general_long_2
#define POMODORO_STATS_WORKING_GOOD general_long_2

// #define APP_SELECTOR_INIT_MODE general_boolean
#define REACTION_HEX_GUESSED_CORRECTLY general_boolean
#define NUMBERS_AND_LETTERS_COUNT_UP_ELSE_DOWN general_boolean
#define SIMON_CUSTOM_BUILD_UP general_boolean
#define POMODORO_IN_BREAK general_boolean
#define HACKTIME_ADDRESS_COUNTER_EDGE general_boolean
#define SOUND_NOTE_AUTO_UP_ELSE_DOWN general_boolean
#define STOPWATCH_PAUSED_1 general_boolean
#define MOVIE_MODE_RESTART_SOUNDTRACK_AT_MOVIE_START general_boolean
#define MODE_DREAMTIME_FADE_IN_ELSE_FADE_OUT general_boolean
#define METRONOME_ENABLE_FLASH_AT_BEEP general_boolean

#define STOPWATCH_PAUSED_2 general_boolean2
#define HACKTIME_VALUE_TO_SOUND general_boolean2
#define NUMBERS_AND_LETTERS_NUMBER_ELSE_LETTER_MODE general_boolean2
#define REACTION_GUITAR_HERO_MODE general_boolean2
#define SIMON_END_OF_GAME general_boolean2
#define MOVIE_MODE_SHOW_NEGATIVE general_boolean2
#define SOUND_NOTE_PLAY_NOTE general_boolean2
#define POMODORO_ENABLE_INTERVAL_BEEP general_boolean2

#define EXTRA_OPTION_REACTION_SOUND_MODE_GUITAR_HEX_HERO general_boolean3
#define MOVIE_MODE_AUTO_BACKWARDS general_boolean3

#define OPTION_REACTION_COUNTDOWN_MODE_HERO_ADD_PAUSE_MODE general_boolean4

#define SIMON_LIST bytes_list
#define SEQUENCER_SONG bytes_list
#define FADE_IN_RANDOM_LIST bytes_list
#define CARDS_DECK bytes_list
#define COMPOSER_SONG bytes_list
#define REACTION_GAME_TEMP_SELECTED_NOTES bytes_list
#define MOVIE_MODE_STOPS bytes_list
#define MODE_SOUND_SONG_BUFFER bytes_list
#define MODE_DREAMTIME_RANDOM_LIST bytes_list

#define REACTION_GAME_SELECTED_SOUNDS array_8_bytes
#define REACTION_GAME_HEX_MEMORY array_8_bytes
#define SIMON_PLAYERS array_8_bytes
#define QUIZ_SCORE array_8_bytes

const char dice_eyes_display[] PROGMEM = {
    SPACE_FAKE_ASCII, ONLY_MIDDLE_SEGMENT_FAKE_ASCII,SPACE_FAKE_ASCII,SPACE_FAKE_ASCII,
    ONLY_TOP_SEGMENT_FAKE_ASCII,SPACE_FAKE_ASCII,ONLY_BOTTOM_SEGMENT_FAKE_ASCII,SPACE_FAKE_ASCII,
    ONLY_TOP_SEGMENT_FAKE_ASCII,ONLY_MIDDLE_SEGMENT_FAKE_ASCII,ONLY_BOTTOM_SEGMENT_FAKE_ASCII,SPACE_FAKE_ASCII,
    ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII,SPACE_FAKE_ASCII,ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII,SPACE_FAKE_ASCII,
    ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII,ONLY_MIDDLE_SEGMENT_FAKE_ASCII,ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII,SPACE_FAKE_ASCII,
    ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII,ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII,ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII,SPACE_FAKE_ASCII
};

// const char dice_eyes_display[] PROGMEM = {
//     NO_SEGMENTS, ONLY_MIDDLE_SEGMENT,NO_SEGMENTS,NO_SEGMENTS,
//     ONLY_TOP_SEGMENT,NO_SEGMENTS,ONLY_BOTTOM_SEGMENT,NO_SEGMENTS,
//     ONLY_TOP_SEGMENT,ONLY_MIDDLE_SEGMENT,ONLY_BOTTOM_SEGMENT,NO_SEGMENTS,
//     ONLY_TOP_AND_BOTTOM_SEGMENT,NO_SEGMENTS,ONLY_TOP_AND_BOTTOM_SEGMENT,NO_SEGMENTS,
//     ONLY_TOP_AND_BOTTOM_SEGMENT,ONLY_MIDDLE_SEGMENT,ONLY_TOP_AND_BOTTOM_SEGMENT,NO_SEGMENTS,
//     ONLY_TOP_AND_BOTTOM_SEGMENT,ONLY_TOP_AND_BOTTOM_SEGMENT,ONLY_TOP_AND_BOTTOM_SEGMENT,NO_SEGMENTS
// };

#define SCALES_COUNT 5-1

#define LEN_MAJ 7-1
#define LEN_MIN 8-1
#define LEN_PENT_MAJ 5-1
#define LEN_BLUES_MAJ 6 -1
#define LEN_HARM 12-1

#define MAJ 0
#define MIN 7
#define PENT 15 
#define BLUES_MAJ 20
#define HARM 26

const uint8_t scale_start_indeces [] PROGMEM = {MAJ, MIN ,PENT, BLUES_MAJ, HARM};

const uint8_t scale_lengths [] PROGMEM = {LEN_MAJ, LEN_MIN, LEN_PENT_MAJ, LEN_BLUES_MAJ, LEN_HARM};
 
const uint8_t scales [] PROGMEM = {
    2,2,1,2,2,2,1,  
    2,1,2,2,1,2,2,1,
    3,2,2,3,2,
    3,2,1,1,3,2,
    1,1,1,1,1,1,1,1,1,1,1,1
};

const uint8_t app_splash_screens[] PROGMEM = {
    //sorted by selector number

    // 0x61, 0x43, 0x58, 0x4C, // abstract two half screen circles.
    0xc4, 0x88, 0x50, 0x00, // student koala    
    //0xD8, 0xE9, 0xCB, 0xC4, // abstract turd or tank
    0x5e, 0x01, 0x5f, 0x00, // music notes
    0x00, 0xd0, 0xab, 0x44, // stopwatch
    // 0x39, 0x09, 0x09, 0x0F, // abstractbig circle
    //0x40, 0x39, 0x0F, 0x40, // abstract. circle with lines
    0x5C, 0x63, 0x5c, 0x63, // dice
    0x00, 0x02, 0x6b, 0x20, // radioactive sign
    // 0x5E, 0x7C, 0x67, 0x73, // abstract.  bolletjes with verticallines
    //0xB9, 0x40, 0x40, 0x0F, // abstract. [--]
   
    0x7F, 0x39, 0x0f, 0x7f, // media player. film roll
    // 0x5e, 0x01, 0xdf, 0x5e, // music notes full
    // 0x32, 0x26, 0x34, 0x16, // vertical lines short and long
    //0x5D, 0x6B, 0x5D, 0x5D, // abstract   camion
    //0x49, 0x49, 0x49, 0x49, // abstract horizontal lines.
    0x40, 0x49, 0x49, 0x4F, //pen
    0x0F, 0x40, 0x40, 0x39, //wrench
    // 0x08, 0xCE, 0x78, 0x08, // abstract piramid
    0x00, 0x07, 0x5c, 0x31, // sequencer / metronome: drum kit
    // 0x40, 0x4F, 0x79, 0x40, // abstract art deco
    // 0x60, 0x3a, 0x2e, 0x42, // body builder. gamer.
    0x3d, 0x41, 0x43, 0x02, // gun 
    // 0x36, 0x36, 0x36, 0x36, // abstract vertical lines
    0x61, 0xc5, 0x51, 0x43, // balancing beam .(tilt)
    // 0x36, 0x36, 0x36, 0x36, // abstract vertical lines
    0x00, 0x63, 0xDA, 0x63, // abstract face 0x63, 0xDA, 0x6C, 0x63
    0x5F, 0x79, 0x1C, 0x3D  // DBUG
};

#define MAX_FRAMES_MOVIES_FLASH 70
#define MOVIE_INDEX_EMPTY 255
#define ANIMATE_CIRCLE_OFFSET 4

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
    0x30, 0x00, 0x00, 0x00, //last byte is byte 52.

    ANIMATION_STOP_CODE_PART_0, ANIMATION_STOP_CODE_PART_1,ANIMATION_STOP_CODE_PART_2, ANIMATION_STOP_CODE_PART_3,
    0x00, 0x00, 0x00, 0x00, // vertical swoop
    0x01, 0x01, 0x01, 0x01,
    0x22, 0x22, 0x22, 0x22,
    0x40, 0x40, 0x40, 0x40,
    0x14, 0x14, 0x14, 0x14,
    0x08, 0x08, 0x08, 0x08, // byte 77,
    ANIMATION_STOP_CODE_PART_0, ANIMATION_STOP_CODE_PART_1,ANIMATION_STOP_CODE_PART_2, ANIMATION_STOP_CODE_PART_3,

    0x00, 0x00, 0x00, 0x00,
    0x21, 0x00, 0x00, 0x00, // sweep in
    0x71, 0x00, 0x00, 0x00,
    0x7B, 0x00, 0x00, 0x00,
    0xFF, 0x00, 0x00, 0x00,
    0xFF, 0x21, 0x00, 0x00,
    0xFF, 0x71, 0x00, 0x00,
    0xFF, 0x7B, 0x00, 0x00,
    0xFF, 0xFF, 0x00, 0x00,
    0xFF, 0xFF, 0x21, 0x00,
    0xFF, 0xFF, 0x71, 0x00,
    0xFF, 0xFF, 0x7B, 0x00,
    0xFF, 0xFF, 0xFF, 0x00,
    0xFF, 0xFF, 0xFF, 0x21,
    0xFF, 0xFF, 0xFF, 0x71,
    0xFF, 0xFF, 0xFF, 0x7B,

    0xFF, 0xFF, 0xFF, 0xFF, // horizonal sweep.sweep out
    0xDE, 0xFF, 0xFF, 0xFF,
    0x8E, 0xFF, 0xFF, 0xFF,
    0x84, 0xFF, 0xFF, 0xFF,
    0x00, 0xFF, 0xFF, 0xFF,
    0x00, 0xDE, 0xFF, 0xFF,
    0x00, 0x8E, 0xFF, 0xFF,
    0x00, 0x84, 0xFF, 0xFF,
    0x00, 0x00, 0xFF, 0xFF,
    0x00, 0x00, 0xDE, 0xFF,
    0x00, 0x00, 0x8E, 0xFF,
    0x00, 0x00, 0x84, 0xFF,
    0x00, 0x00, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0xDE,
    0x00, 0x00, 0x00, 0x8E,
    0x00, 0x00, 0x00, 0x84,
    ANIMATION_STOP_CODE_PART_0, ANIMATION_STOP_CODE_PART_1,ANIMATION_STOP_CODE_PART_2, ANIMATION_STOP_CODE_PART_3,
};

#define SONG_DRYER_HAPPY 0
#define SONG_LANG_ZAL_ZE_LEVEN 1
#define SONG_ATTACK 2
#define SONG_EMPTY 3
#define SONG_DRYER_UNHAPPY 4
#define SONG_KINDEKE_DOUWEN 5
#define SONG_RETREAT 6
#define SONG_ALPHABET 7

#define LEN_SONG_DRYER_HAPPY 18
#define LEN_LANG_ZAL_ZE_LEVEN 84
#define LEN_SONG_ATTACK 14
#define LEN_SONG_EMPTY 0
#define LEN_SONG_DRYER_UNHAPPY 13
#define LEN_SONG_KINDEKE_DOUWEN 49
#define LEN_SONG_RETREAT 14
#define LEN_SONG_ALPHABET 100

#define SONGS_FLASH_COUNT 8

const uint8_t song_lengths [] PROGMEM = {
    LEN_SONG_DRYER_HAPPY,
    LEN_LANG_ZAL_ZE_LEVEN,
    LEN_SONG_ATTACK,
    LEN_SONG_EMPTY,
    LEN_SONG_DRYER_UNHAPPY,
    LEN_SONG_KINDEKE_DOUWEN,
    LEN_SONG_RETREAT,
    LEN_SONG_ALPHABET,
};
 
// one big library. length of song as byte at start. lenght of song INCLUDES the length byte. There is no need then to keep a separate array with start indeces. 
const uint8_t songs [] PROGMEM = {
    //happy dryer
    A6_2, rest_4, rest_2,
    Cs7_2, rest_4, rest_2,
    E7_4, rest_8, rest_4,
    Cs7_4, rest_8, rest_4,
    E7_4, rest_8, rest_4,
    A7_1, A7_1,
    rest_2 ,
    
    // lang zal ze leven
    C7_4, rest_4, rest_2,
    C7_4, rest_2, C7_8, rest_8,
    C7_4, rest_4, rest_2,
    G6_2, G6_4, rest_2,

    E7_4, rest_4, rest_2,
    E7_4, rest_2, E7_8, rest_8,
    E7_4, rest_4, rest_2,
    C7_2, C7_4, rest_2,

    G7_4, rest_4, rest_2,
    G7_4, rest_2, G7_8, rest_8,

    A7_8, rest_8, rest_4,
    G7_8, rest_8, rest_4,

    F7_8, rest_8, rest_4,
    E7_8, rest_8, rest_4,

    D7_2, D7_4, rest_4,
    D7_2, D7_4, rest_4,
    D7_2, D7_4, rest_4,
    rest_4,
    G7_8, rest_8, rest_4,
    F7_8, rest_8, rest_4,

    E7_1, E7_2, rest_2,
    F7_1, F7_2, rest_2,

    G7_1, G7_2, rest_2,
    A7_2, A7_4, rest_4,
    F7_2, F7_4, rest_4,

    E7_1, E7_2, rest_2,
    D7_1, D7_2, rest_2,
    C7_1, C7_1,
    
    // aaanvallueeeeee!
     Gs6_2, rest_4, Gs6_2, rest_4, Gs6_2, rest_4, Cs7_2, rest_2, rest_2, Gs6_2, rest_4, Cs7_1, Cs7_1, Cs7_1, 
    
    // unhappy dryer
    A6_1, rest_2, Cs7_1, rest_2, E7_2, rest_4, Cs7_2, rest_4, B6_2, rest_4, A6_1, rest_2, rest_2, 
    
    // kindeke douwen
    B7_4, rest_4, rest_2,       // wie
    B7_1,                       //zal
    rest_2, A7_4, rest_4,       //er
    G6_4, rest_4, rest_2,       //ons
    B7_1,                       //kind-
    B7_4, rest_4, A7_4, rest_4, //-de
    G6_4, rest_4, rest_2,       //-ke
    A7_1,                       //dou
    A7_4, rest_4, rest_2,
    C7_4, rest_4, rest_2,
    C7_1, //wen
    rest_1,
    C7_4, rest_4, rest_2, // dat
    B7_1,                 // schaars
    B7_4, rest_4, B7_4, rest_4,
    B7_4, rest_4, rest_2,

    E7_4, rest_4, rest_2, //
    E7_1,
    E7_4, rest_4, E7_4, rest_4,
    A7_4, rest_4, rest_2,
    
    // retreat song
    Gs6_2, rest_4, Gs6_2, rest_4, Gs6_2, rest_4, Gs6_2, rest_2, rest_2, Gs6_2, rest_4, Cs6_1, Cs6_1, Cs6_1,
    
    // alphabet song
    C7_4, rest_4, rest_2, 
    C7_4, rest_4, rest_2, 
    G7_4, rest_4, rest_2, 
    G7_4, rest_4, rest_2, 
    A7_4, rest_4, rest_2, 
    A7_4, rest_4, rest_2, 
    G7_1, rest_1,
    F7_4, rest_4, rest_2, F7_4, rest_4, rest_2, E7_4, rest_4, rest_2, E7_4, rest_4, rest_2, D7_4, rest_4, D7_4, rest_4, D7_4, rest_4, D7_4, rest_4, C7_1, rest_1,
    G7_4, rest_4, rest_2, G7_4, rest_4, rest_2, F7_4, rest_4, rest_2, F7_4, rest_4, rest_2, E7_4, rest_4, rest_2, E7_4, rest_4, rest_2, D7_1, rest_1, C7_4, rest_4, rest_2,
    C7_4, rest_4, rest_2, G7_4, rest_4, rest_2, G7_4, rest_4, rest_2, A7_1, rest_1, G7_4, rest_4, rest_2, rest_1,
    F7_4, rest_4, rest_2, F7_4, rest_4, rest_2, E7_4, rest_4, rest_2, E7_4, rest_4, rest_2, D7_4, rest_4, rest_2, G7_4, rest_4, rest_2, C7_1, C7_1,
};

const uint8_t lights_indexed[] = {LIGHT_MOMENTARY_0, LIGHT_MOMENTARY_1, LIGHT_MOMENTARY_2, LIGHT_MOMENTARY_3, 
                                    LIGHT_LATCHING_BIG,  LIGHT_LATCHING_SMALL_LEFT, LIGHT_LATCHING_SMALL_RIGHT, LIGHT_LATCHING_EXTRA};

const uint8_t buttons_indexed[] = {BUTTON_MOMENTARY_0, BUTTON_MOMENTARY_1, BUTTON_MOMENTARY_2, BUTTON_MOMENTARY_3, 
                                    BUTTON_LATCHING_BIG_RED, BUTTON_LATCHING_SMALL_RED_LEFT, BUTTON_LATCHING_SMALL_RED_RIGHT, BUTTON_LATCHING_EXTRA};

const uint8_t mercury_switches_indexed[] = {SWITCH_TILT_FORWARD,SWITCH_TILT_LEFT,SWITCH_TILT_BACKWARD,SWITCH_TILT_RIGHT};

class Apps
{

public:
    Apps();

    BinaryInput *binaryInputs;
    DisplayManagement *ledDisp;
    Buzzer *buzzer;
    RotaryEncoderDial *encoder_dial;
    LedMultiplexer5x8* allLights;
    PotentioSelector* selectorDial;

    void setPeripherals(BinaryInput *binaryInput, RotaryEncoderDial *encoder_dial, DisplayManagement *ledDisp, LedMultiplexer5x8* allLights, Buzzer *buzzer, PotentioSelector* selectorDial);

    void setDefaultMode();
    void appSelector();

    bool init_app(bool init, uint8_t selector);
    void modeSimpleButtonsAndLights(bool init);
    void modeCountingLettersAndChars(bool init);
    void modeSoundSong(bool init);
    void modeComposeSong(bool init);
    void modeSoundNotesInitScale();
    void modeSoundNotes(bool init);
    void draw(bool init);
    void drawGame(bool init);
    void modeMovie(bool init);
    void pomodoroTimer(bool init);
    void stopwatch(bool init);
    void modeGeiger(bool init);
    void modeSequencer(bool init);
    void modeMetronome(bool init);
    void modeSimon(bool init);
    void modeReactionGame(bool init);
    void tiltSwitchTest(bool init);
    void modeDreamtime(bool init);
    void modeButtonDebug(bool init);
    void miniMultiTimer(bool init);
    void modeRandomWorld(bool init);
    void modeHackTime(bool init);
    void quiz(bool init);

    void modeMetronomeTickerUpdate(int16_t* ticker_counter, uint8_t momentary_id, bool direction, uint8_t sound_at_zero_pass, boolean force_step);
    void dialOnEdgeChangeInitTimerPercentage(SuperTimer* aTimer);
    bool modifyValueUpDownWithMomentary2And3(int16_t* value, uint8_t amount);
    uint32_t modeSingleSegmentManipulation(uint32_t *display_buffer);
    void displayChangeGlobal(uint32_t *display_buffer, bool saveStateToBuffer);
    void displayLetterAndPositionInAlphabet(char* textBuf, int16_t letterValueAlphabet);
    uint16_t _animationGetStartByte(uint8_t number);
    uint32_t fadeInList(uint8_t step, uint8_t length, uint32_t startScreen, uint8_t *shuffledSequence);
    uint8_t tombola(uint8_t* indexVariable, uint8_t *sequenceList, uint8_t length);
    void randomSequence(uint8_t *sequenceList, uint8_t length);
    void shuffle(uint8_t *list, uint8_t length);
    bool saveLoadMenu(uint8_t *data, uint8_t slotCount, uint8_t eepromSlotLength, uint16_t eepromStartAddress);
    void saveLoadFromEepromSlot(uint8_t *data, uint8_t slotIndex, uint8_t eepromSlotLength, uint16_t eepromStartAddress, boolean loadElseSave);
    void updateEveryAppCycleBefore();
    void geigerToneHelper();
private:

#ifdef ENABLE_MULTITIMER
    MiniMultiTimer multiTimer;
#endif
    void nextStepRotate(int16_t* counter, bool countUpElseDown, int16_t minValue, int16_t maxValue);
    void nextStep(int16_t* counter, bool countUpElseDown, int16_t minValue, int16_t maxValue, bool overflowToOtherSide);
    bool checkBoundaries(int16_t* counter, int16_t maxValue, int16_t minValue, bool rotate);
    void randomModeTrigger(bool forReal);

    void progmemToBuffer(const uint8_t *offset, uint8_t length);
    uint8_t progmemToBufferUntil(const uint8_t *offset, uint8_t stopConditionValue);
    
    unsigned int indexToTimeSeconds(int16_t index);

    void textBufToDisplay();
    void decimalPointTimingOn();
    void setDecimalPoint(bool onElseOff, uint8_t digit);
    void displayAllSegmentsToScreen();
    void addNoteToBuzzer(uint8_t note);
    void addNoteToBuzzerRepeated(uint8_t note, uint8_t repeater);
    void buzzerOff();
    void buzzerOffAndAddNote(uint8_t note);
    void buzzerChangeSpeedRatioWithEncoderDial();
    void buzzerOffAndAddNoteAtEncoderDialChange(uint8_t note);
    void noteToDisplay(uint8_t note);
    void loadBuzzerTrack(uint8_t songIndex);
    // void loadBuzzerTrackFromEeprom(uint8_t songIndex);
    void setBlankDisplay();
    void setLedArray(byte lights);
    void setStandardTextToTextBuf(uint8_t textPosition);
    void setStandardTextToTextHANDLE(uint8_t textPosition);
    void numberToBufAsDecimal(int16_t number);
    void dialSetCheckDisplay(int16_t* pVariable, uint8_t multiplier, int16_t maxValue);
    
    void loadNextMovie();
    bool loadScreenFromMemory(int16_t address);

    void eepromPictureToDisplayAllSegments(int16_t offset, int16_t pictureIndex);
    void flashPictureToDisplayAllSegments(const uint8_t* progmemAddress);

    void resetStopwatch(SuperTimer* pTimer);
    
    // bool isNoMomentaryButtonOn(); // doesnt decrease memory footprint. I wonder why.
    //void _eepromWriteByteIfChanged(uint8_t* address , uint8_t value);

    SuperTimer generalTimer;
    SuperTimer generalTimer2;
    SuperTimer* pSsuperTimer;
    uint32_t displayAllSegments;
    uint32_t displayAllSegmentsBuffer;

    //reused variables per app
    bool general_boolean;
    bool general_boolean2;
    bool general_boolean3;
    bool general_boolean4;
    int16_t general_int16_t_1;
    int16_t general_int16_t_2;
    long general_long_1;
    uint16_t general_uint16_t_1;
    uint8_t general_uint8_t_1;
    uint8_t general_uint8_t_2;
    uint8_t general_uint8_t_3;
    uint16_t general_uint16_t_2;
    int16_t general_int16_t_3;
    long general_long_2;
    long* pLongValue;

    byte binaryInputsValue;
    byte binaryInputsEdgeUp;
    byte binaryInputsEdgeDown;

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
        reactionHexPlaying

    };
    ReactionGameState reactionGameState;
#endif

    enum QuizState : uint8_t
    {
        quizInit,
        quizWaitForQuizMaster,
        quizWaitRandomTime,
        quizWaitPlayerPress,
        quizPlayerPressed
    };
    QuizState quizState;

    enum RandomWorldState : uint8_t
    {
        randomWorldIdle,
        randomWorldShowResult,
        randomWorldRolling,
        randomWorldRollingEnd,
        randomWorldAutoRollDelay,
    };
    RandomWorldState randomWorldState;

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

    char textBuf[4];
    char* textHandle;  // contains the text for the display. (4 chars)
    uint8_t decimalPoints; // segment 4 = bit 3, ....   00043210 (segment number)
    byte* decimalDotsHandle; // segment 4 = bit 3, ....   00043210 (segment number)
    byte* lightsHandle;
    uint8_t lights;

    bool init_app_playing;
};

#endif
