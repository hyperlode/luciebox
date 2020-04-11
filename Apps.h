#ifndef APPS_H
#define APPS_H




#define ENABLE_EEPROM
#define ENABLE_MULTITIMER
#define SIMON_APP
#define ENABLE_REACTION_APP


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
// #include <EEPROM.h>
#endif

#ifdef ENABLE_MULTITIMER
#include "MiniMultiTimer.h"
#endif

#include "DataPlayer.h"

//#include "SevSeg5Digits.h" //sevseb -->this should not be included here, but only in DisplayDigitsHandler.h, as it only gets used there (but ARDUINO needs this here!) DEBUG
#include "DisplayDigitsHandler5Digits.h"
#include "PretbakSettings.h"
#include "Potentio.h"

#define APP_SELECTOR_LETTERS_AND_CHARS 0
#define APP_SELECTOR_SIMON 1
#define APP_SELECTOR_SOUND_NOTES 2
#define APP_SELECTOR_SOUND_COMPOSER 3
#define APP_SELECTOR_STOPWATCH 4
#define APP_SELECTOR_POMODORO 5
#define APP_SELECTOR_RANDOMWORLD 6
#define APP_SELECTOR_SLOTS 7
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
#define APP_SELECTOR_TILT_ADVANCED 21
#define APP_SELECTOR_MULTITIMER_SETTING 22
#define APP_SELECTOR_MULTITIMER_PLAYING 23



// #define TILT_FORWARD 	0x00000001
// #define TILT_BACKWARD 	0x00000010
// #define TILT_LEFT 		0x00000100
// #define TILT_RIGHT 		0x00001000
#define TILT_FORWARD 0
#define TILT_BACKWARD 1
#define TILT_LEFT 2
#define TILT_RIGHT 3

// #define DO_NOTHING		0
// #define TOGGLE_SEGMENT	1
// #define MOVE_RIGHT		2
// #define MOVE_LEFT		3
// #define MOVE_DOWN		4
// #define MOVE_UP	  	    5
#define ANIMATION_STOP_CODE 0x00
// enum RandomWorldMode:uint8_t{
//   rollOneDice,
//   rollFourDice,
//   takeRandomCard,
//   takeRandomCardFromDeck,
//   randomNumber,
//   randomLetter,
//   headsOrTails,
//   yesOrNo,
// };
// RandomWorldMode randomWorldMode;




#define RANDOMWORLD_ROLLONEDICE 0
#define RANDOMWORLD_ROLLFOURDICE 4
// #define RANDOMWORLD_TAKERANDOMCARD 1
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
#define POMODORO_INIT_DEFAULT_SECS 1500
#define POMODORO_PAUSE_DEFAULT_SECS 600
#define POMODORO_DISPLAY_TIMER 0
#define POMODORO_DISPLAY_PAUSE_INIT_SECS 1
#define POMODORO_DISPLAY_SHOW_GOOD 2
#define POMODORO_DISPLAY_SHOW_BAD 3
#define POMODORO_DISPLAY_BEEP_PROBABILITY 4
#define POMODORO_NONSENSE_TIME 60000

#define HACKTIME_MEMORY_FLASH 0
#define HACKTIME_MEMORY_RAM 1
#define HACKTIME_MEMORY_EEPROM 2

#define HACKTIME_DISPLAY_ADDRESS 0
#define HACKTIME_DISPLAY_CHARS 1
#define HACKTIME_DISPLAY_BYTES 2
#define HACKTIME_DISPLAY_DECIMAL 3
#define HACKTIME_DISPLAY_HEX 4

#define SOUND_NOTE_MODE_ARPEGGIO_UP 0
#define SOUND_NOTE_MODE_ARPEGGIO_DOWN 1
#define SOUND_NOTE_MODE_ARPEGGIO_SAWTOOTH 2
#define SOUND_NOTE_MODE_RANDOM 3
#define SOUND_NOTE_MODE_RANDOM_ERRATIC 4
#define SOUND_NOTE_MODE_MANUAL 99

// VARIABLE REUSE

#define TIMER_METRONOME generalTimer
#define TIMER_INIT_APP generalTimer
#define TIMER_REACTION_GAME_SPEED generalTimer
#define TIMER_REACTION_GAME_RESTART_DELAY generalTimer
#define SETTINGS_MODE_DISPLAY_VALUES_BLINK generalTimer
#define RANDOMWORLD_ROLL_SPEED generalTimer
#define COMPOSER_STEP_TIMER generalTimer
#define SIMON_STEP_TIMER generalTimer
#define STOPWATCH_CHRONO generalTimer
#define POMODORO_TIMER generalTimer
#define DRAW_GAME_DISPLAY_TIMER generalTimer
#define HACKTIME_MOVE_TIMER generalTimer
#define COUNTING_LETTERS_AND_CHARS_TIMER generalTimer
#define SOUND_NOTE_AUTO_TIMER generalTimer

#define SAVE_LOAD_MENU_BLINK_TIMER generalTimer2
#define SEQUENCER_EEPROM_MODE_BLINK generalTimer2
#define RANDOMWORLD_AUTODRAW_DELAY generalTimer2
#define SIMON_BLINK_TIMER generalTimer2

#define SOUND_FUN_NOTE_INDEX counter
#define REACTION_GAME_SCORE counter
#define GEIGER_TONE_FREQUENY_LOWEST counter
#define DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY counter
#define SIMON_LENGTH counter
#define RANDOMWORLD_RANDOM_NUMBER counter
#define SEQUENCER_STEP_COUNTER counter
#define COMPOSER_STEP counter
#define POMODORO_STATS_WORKING_GOOD counter
#define HACKTIME_ADDRESS counter
#define SOUND_NOTES_NOTE_ON_SCALE_INDEX counter


#define GEIGER_TONE_FREQUENCY_HEIGHEST counter2
#define REACTION_GAME_TIMER_STEP counter2
#define DRAW_ACTIVE_DRAWING_INDEX counter2
#define SIMON_INDEX counter2
#define DEBUGMODE_ACTIVATED counter2
#define RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW counter2
#define POMODORO_INIT_TIME_SECONDS counter2
#define HACKTIME_DISPLAY_MODE counter2
#define SOUND_NOTES_SCALE_INDEX counter2

//#define DRAW_Y_POS counter3
#define DRAW_CURSOR_INDEX counter3
#define GEIGER_INCREASE_CHANCE counter3
#define GEIGER_PROBABILITY_THRESHOLD counter3
#define REACTION_GAME_STEP_TIME_MILLIS counter3
#define SETTINGS_MODE_SELECTOR counter3
#define RANDOMWORLD_ANIMATION_DELAY counter3
#define SIMON_RANDOM_PLAYER_SEQUENCE counter3
#define STOPWATCH_LAP_MEMORY counter3
#define POMODORO_STATS_WORKING_BAD counter3
#define SOUND_NOTES_SCALE_ROOT counter3

#define REACTION_GAME_TARGET counter4
#define COUNTER_GEIGER counter4
#define SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET counter4
#define SIMON_ACTIVE_LIGHT counter4
#define POMODORO_PAUSE_TIME_SECONDS counter4
#define SOUND_MODE_SCALE_RANGE_LENGTH counter4

#define REACTION_GAME_LEVEL counter5
#define GEIGER_TONE_LENGTH counter5
#define SEQUENCER_TEMP_NOTE counter5
#define RANDOMWORLD_RANDOM_TYPE counter5
#define COMPOSER_SONG_LENGTH counter5
#define SIMON_PLAYERS_ALIVE_COUNT counter5
#define HACKTIME_MEMORY_SELECT counter5
#define DRAW_CURSOR_POTENTIO_INDEX counter5
#define METRONOME_TICKER_1_POSITION counter5
#define SOUND_NOTES_AUTO_MODE counter5

#define SIMON_PLAYERS_COUNT counter6
#define POMODORO_AUTO_RESTART_ENABLED counter6
#define DRAW_SHOW_MODE counter6
#define METRONOME_TICKER_2_POSITION counter6
#define HACKTIME_SOUND counter6
#define RANDOMWORLD_CARD_FROM_DECK_INDEX counter6

#define SIMON_PLAYER_PLAYING_INDEX counter7
#define POMODORO_RANDOM_BEEP_FOR_PERFORMANCE_TRACKING_ENABLED counter7
#define METRONOME_TICKER_3_POSITION counter7

#define POMODORO_PROBABILITY_BEEP_EVERY_SECONDS counter8
#define SOUND_NOTE_SETTING_TO_DISPLAY counter8

#define SOUND_NOTES_NOTE_INDEX counter9

#define REACTION_COUNTDOWN_MODE general_boolean
#define NUMBERS_AND_LETTERS_COUNT_UP_ELSE_DOWN general_boolean
#define SIMON_CUSTOM_BUILD_UP general_boolean
#define POMODORO_IN_BREAK general_boolean
#define HACKTIME_ADDRESS_COUNTER_EDGE general_boolean
#define SOUND_NOTE_AUTO_UP_ELSE_DOWN general_boolean

#define REACTION_GUITAR_HERO_MODE general_boolean2
#define SIMON_END_OF_GAME general_boolean2
#define POMODORO_SHOW_MENU_EDGE general_boolean2
#define MOVIE_MODE_SHOW_NEGATIVE general_boolean2

#define SIMON_LIST bytes_list
#define SEQUENCER_SONG bytes_list
#define FADE_IN_RANDOM_LIST bytes_list
#define CARDS_DECK bytes_list
#define COMPOSER_SONG bytes_list

#define REACTION_GAME_SELECTED_SOUNDS array_8_bytes
#define SIMON_PLAYERS array_8_bytes



// #define ONLY_TOP_SEGMENT_FAKE_ASCII 58
// #define ONLY_MIDDLE_SEGMENT_FAKE_ASCII 59
// #define ONLY_BOTTOM_SEGMENT_FAKE_ASCII 60
// #define ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII 61
// #define SPACE_FAKE_ASCII 62

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
    2,1,2,2,1,2,2,1 ,
    3,2,2,3,2,
    3,2,1,1,3,2,
    1,1,1,1,1,1,1,1,1,1,1,1
};

const uint8_t app_splash_screens[] PROGMEM = {
    //sorted by selector number

    // 0x61, 0x43, 0x58, 0x4C, // abstract two half screen circles.
    0xc4, 0x88, 0x50, 0x00, // student koala    
    //0xD8, 0xE9, 0xCB, 0xC4, // abstract drol or tank
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

const uint8_t disp_4digits_animations[] PROGMEM = {
    0x35,                   // first byte = length of animation in bytes  (here 53, including the length byte)
                            // const uint32_t disp_4digits_swoosh [] PROGMEM = {
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
    0x19,                   //length of next animation (25 including the length)  byte53
    0x00, 0x00, 0x00, 0x00, // vertical swoop
    0x01, 0x01, 0x01, 0x01,
    0x22, 0x22, 0x22, 0x22,
    0x40, 0x40, 0x40, 0x40,
    0x14, 0x14, 0x14, 0x14,
    0x08, 0x08, 0x08, 0x08, // byte 77,
    0x81,                   // ( length of following animation, including the length byte)

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

    ANIMATION_STOP_CODE //length 0 byte is stop byte.
};

#define ALL_DATA_SIZE 1
//PGM_P const allData[ALL_DATA_SIZE] PROGMEM = {disp_4digits_animations};

#define SONG_DRYER_HAPPY 0
#define SONG_LANG_ZAL_ZE_LEVEN 1
#define SONG_ATTACK 2

#define SONG_DRYER_UNHAPPY 4
#define SONG_KINDEKE_DOUWEN 5
#define SONG_RETREAT 6
#define SONG_ALPHABET 7

// one big library. length of song as byte at start. lenght of song INCLUDES the length byte. There is no need then to keep a separate array with start indeces. 
const uint8_t songs [] PROGMEM = {
    //happy dryer
    19, A6_2, rest_4, rest_2,
    Cs7_2, rest_4, rest_2,
    E7_4, rest_8, rest_4,
    Cs7_4, rest_8, rest_4,
    E7_4, rest_8, rest_4,
    A7_1, A7_1,
    rest_2 ,
    // lang zal ze leven
    85, C7_4, rest_4, rest_2,
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
    15, Gs6_2, rest_4, Gs6_2, rest_4, Gs6_2, rest_4, Cs7_2, rest_2, rest_2, Gs6_2, rest_4, Cs7_1, Cs7_1, Cs7_1, 
    // empty slot test
    1,

 // unhappy dryer
    14, A6_1, rest_2, Cs7_1, rest_2, E7_2, rest_4, Cs7_2, rest_4, B6_2, rest_4, A6_1, rest_2, rest_2, 
    // kindeke douwen
    50, B7_4, rest_4, rest_2,       // wie
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
    15, Gs6_2, rest_4, Gs6_2, rest_4, Gs6_2, rest_4, Gs6_2, rest_2, rest_2, Gs6_2, rest_4, Cs6_1, Cs6_1, Cs6_1,

    // alphabet song
    21, C7_4, rest_4, rest_2, 
    C7_4, rest_4, rest_2, 
    G7_4, rest_4, rest_2, 
    G7_4, rest_4, rest_2, 
    A7_4, rest_4, rest_2, 
    A7_4, rest_4, rest_2, 
    G7_1, rest_1,
    // F7_4, rest_4, rest_2, F7_4, rest_4, rest_2, E7_4, rest_4, rest_2, E7_4, rest_4, rest_2, D7_4, rest_4, D7_4, rest_4, D7_4, rest_4, D7_4, rest_4, C7_1, rest_1,
    // G7_4, rest_4, rest_2, G7_4, rest_4, rest_2, F7_4, rest_4, rest_2, F7_4, rest_4, rest_2, E7_4, rest_4, rest_2, E7_4, rest_4, rest_2, D7_1, rest_1, C7_4, rest_4, rest_2,
    // C7_4, rest_4, rest_2, G7_4, rest_4, rest_2, G7_4, rest_4, rest_2, A7_1, rest_1, G7_4, rest_4, rest_2, rest_1,
    // F7_4, rest_4, rest_2, F7_4, rest_4, rest_2, E7_4, rest_4, rest_2, E7_4, rest_4, rest_2, D7_4, rest_4, rest_2, G7_4, rest_4, rest_2, C7_1, C7_1,
   
};

// const uint8_t scale_major[] PROGMEM = {C7_2, rest_4, D7_2, rest_4, E7_2, rest_4, F7_2, rest_4, G7_2, rest_4, A7_2, rest_4, B7_2, rest_4, C8_2, rest_4, BUZZER_ROLL_SONG_STOPVALUE};
// const uint8_t scale_major_reversed[] PROGMEM = {C8_2, rest_4, B7_2, rest_4, A7_2, rest_4, G7_2, rest_4, F7_2, rest_4, E7_2, rest_4, D7_2, rest_4, C7_2, rest_4, BUZZER_ROLL_SONG_STOPVALUE};

// const uint8_t scale_pentatonic[] PROGMEM = {C6_2, D6_2, F6_2, G6_2, A7_2, BUZZER_ROLL_SONG_STOPVALUE};

#if MOMENTARY_BUTTONS_COUNT == 3
const uint8_t buttons_indexed[] = {BUTTON_MOMENTARY_0, BUTTON_MOMENTARY_1, BUTTON_MOMENTARY_2};
const uint8_t lights_indexed[] = {LIGHT_LATCHING_EXTRA, LIGHT_MOMENTARY_0, LIGHT_MOMENTARY_1, LIGHT_MOMENTARY_2,
                             LIGHT_LATCHING_SMALL_LEFT, LIGHT_LATCHING_SMALL_RIGHT, LIGHT_LATCHING_BIG};
#else
const uint8_t lights_indexed[] = {LIGHT_MOMENTARY_0, LIGHT_MOMENTARY_1, LIGHT_MOMENTARY_2, LIGHT_MOMENTARY_3, 
                                    LIGHT_LATCHING_BIG,  LIGHT_LATCHING_SMALL_LEFT, LIGHT_LATCHING_SMALL_RIGHT, LIGHT_LATCHING_EXTRA};

const uint8_t buttons_indexed[] = {BUTTON_MOMENTARY_0, BUTTON_MOMENTARY_1, BUTTON_MOMENTARY_2, BUTTON_MOMENTARY_3, 
                                    BUTTON_LATCHING_BIG_RED, BUTTON_LATCHING_SMALL_RED_LEFT, BUTTON_LATCHING_SMALL_RED_RIGHT, BUTTON_LATCHING_EXTRA};

#endif

const uint8_t tilt_forward[] PROGMEM = {
    0x08, 0x08, 0x08, 0x08};
const uint8_t tilt_backward[] PROGMEM = {
    0x01, 0x01, 0x01, 0x01};
const uint8_t tilt_left[] PROGMEM = {
    0x30, 0x30, 0x30, 0x30};
const uint8_t tilt_right[] PROGMEM = {
    0x06, 0x06, 0x06, 0x06};

const uint8_t disp_4digits_animate[] PROGMEM = {
    0x00, 0xFF, 0xF0, 0x0F,
    0xFF, 0x00, 0x00, 0x01,
    0x01, 0x02, 0x04, 0x08};

const uint8_t disp_4digits_animate_circle[] PROGMEM = {
    0x01, 0x00, 0x00, 0x00,
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
    0x20, 0x00, 0x00, 0x00};

class Apps
{

public:
    Apps();

    BinaryInput *binaryInputs;
    DisplayManagement *ledDisp;
    Buzzer *buzzer;
    Potentio *potentio;
    LedMultiplexer5x8* allLights;

    void setPeripherals(BinaryInput *binaryInput, Potentio *potentio, DisplayManagement *ledDisp, LedMultiplexer5x8* allLights, Buzzer *buzzer);
    // void setBuffers(char* textBuf, char*  scrollBuf);
    //void setBuffers(char* textBuf);
    // void test();

    void setDefaultMode();
    void appSelector(bool init, uint8_t selector);
    bool init_app(bool init, uint8_t selector);
    void modeSimpleButtonsAndLights(bool init);
    void modeCountingLettersAndChars(bool init);
    void modeSoundSong(bool init);
    void modeComposeSong(bool init);
    void modeSoundNotes(bool init);
    void draw(bool init);
    void drawGame(bool init);
    void movieAnimationMode(bool init);
    void pomodoroTimer(bool init);
    void stopwatch(bool init);
    void modeGeiger(bool init);
    void modeSequencer(bool init);
    void modeMetronome(bool init);
    void modeMetronomeTickerUpdate(uint8_t* ticker_counter, uint8_t momentary_id, bool direction, uint8_t sound_at_zero_pass, boolean force_step);
    void modeSimon(bool init);
    void modeReactionGame(bool init);
    void tiltSwitchTest(bool init);
    void modeButtonDebug(bool init);
    void miniMultiTimer(bool init);
    void modeRandomWorld(bool init);
    void modeHackTime(bool init);

    void listenToPotentioToIncrementTimerInit(SuperTimer* aTimer, int16_t increment_millis);
    bool listenToMomentary2and3ModifyValue(int16_t* value, uint8_t amount);
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

private:

#ifdef FUNCTION_POINTER_APP_SELECTION
    typedef void (Apps::*fptr)(bool init);
    fptr appPointer;
#endif

    DataPlayer dataPlayer;

#ifdef ENABLE_MULTITIMER
    MiniMultiTimer multiTimer;
#endif

    bool nextStepRotate(int16_t* counter, bool countUpElseDown, int16_t minValue, int16_t maxValue);
    void checkBoundaries(int16_t* counter, int16_t maxValue, int16_t minValue);
    void randomModeDisplay(bool forReal);
    //void _eepromWriteByteIfChanged(uint8_t* address , uint8_t value);

    SuperTimer generalTimer;
    SuperTimer generalTimer2;
    uint32_t displayAllSegments;
    uint32_t displayAllSegmentsBuffer;

    //reused variables per app
    bool general_boolean;
    bool general_boolean2;
    int16_t counter;
    int16_t counter2;
    long counter3;
    uint16_t counter4;
    uint8_t counter5;
    uint8_t counter6;
    uint8_t counter7;

    uint16_t counter8;
    int16_t counter9;

    
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
        // enum ReactionGameState{
        reactionWaitForStart,
        reactionNewGame,
        reactionNewTurn,
        reactionPlaying,
        reactionJustDied,
        reactionFinished,
        reactionMultiNewTurn,
        reactionMultiPlaying,

    };
    ReactionGameState reactionGameState;
#endif

    enum RandomWorldState : uint8_t
    {
        randomWorldIdle,
        randomWorldShowResult,
        randomWorldRolling,
        randomWorldRollingEnd,
        randomWorldAutoRollDelay,
    };
    RandomWorldState randomWorldState;

// enum SettingsState:uint8_t{
//   settingNormal,
//   settingSound,
//   settingButtons,
//   settingEepromReset
// };
#ifdef SIMON_APP
    // simon
    enum SimonState : uint8_t
    {
        // enum SimonState{
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
    //uint32_t* displaySegmentsHandle;
    byte* lightsHandle;
    uint8_t lights;
    
    bool app_init_mode;
};

#endif
