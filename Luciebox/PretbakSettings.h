
#define SOFTWARE_VERSION 308
// started at v2.00: 200
// v300 : pcb v3 burst implemented. change in button sequence...
// v301 : sending out to Belgium 2021-03-19
// v302 : development since 2021-03-19. Numerous small bugfixes and additional functionality.
// v303 : settings debug selector dial. Make sure values are not too erratic. small bugfixes.
// v304 : quizmaster scoring bugfix and optimization / baby dreamtime mode change behaviour + shift button keeps it in dreamtime mode (all button mash apps should have the shift button included). / total time timer is now set to a timer of the chrono app.
// v305 : add battery voltage indicator (note the voltagedrop because of a Schottky diode to differentiate between real battery voltage and Vcc) / buzzer rework (note scheme, buzzer buffer) / sequencer rework
// v306 : song composer / change apps sequence. baby app has no shift.
// v307 : introduce V4 prototype. make app selector dial optional. redo apps (see manual)
// v308 : V5: bugfixes

// #define ENABLE_SERIAL  //for debugging. if used, pin 0 and 1 cannot be used for other purposes than tx and rx
//#define ENABLE_TILT_SWITCHES
//#define ENABLE_ANALOG_PIN_DEBUG  // for debugging at startup (always available in apps anyways.)

// #define V2_PCB_CORONA
//#define V3_PCB_BURST
// #define V4_PCB_MINI --> abandoned. extremely similar layout of V5 (which is just a debugged V4)
#define V5_PCB_MINI

// #define ENABLE_SELECT_APPS_WITH_SELECTOR  // up to version 3 a big selector dial is used to change apps.
#define ENABLE_SOFT_POWER_OFF

#define ENABLE_APPS
#define ENABLE_BATTERY_STATUS

#ifdef ENABLE_SERIAL
//#define DEBUG_ENCODER_DIAL
//#define DEBUG_ANALOG_IN
//#define DEBUG_MERCURY
//#define DEBUG_POTENTIO
//#define DEBUG_BUTTONS
//#define DEBUG_SELECTOR_KNOB
//#define DEBUG_MINIMULTITIMER
//#define DEBUG_SEQUENCER
//#define SUPERDEBUG
#endif

#define DISPLAY_IS_COMMON_ANODE true //check led displays both displays should be of same type   //also set in SevSeg5Digits.h : MODEISCOMMONANODE

#define PIN_DUMMY 66
#define PIN_DUMMY_2 22 // randomly chosen. I've had it set to 67, and at some point, multiple segments were lit up. This STILL is C hey, it's gonna chug on forever!

#define PIN_DISPLAY_DIGIT_0 11
#define PIN_DISPLAY_DIGIT_1 10
#define PIN_DISPLAY_DIGIT_2 9
#define PIN_DISPLAY_DIGIT_3 5
#define PIN_DISPLAY_DIGIT_BUTTON_LIGHTS 6

#define PIN_DISPLAY_SEGMENT_A 7
#define PIN_DISPLAY_SEGMENT_B A3 // was 2
#define PIN_DISPLAY_SEGMENT_C 4
#define PIN_DISPLAY_SEGMENT_D 8
#define PIN_DISPLAY_SEGMENT_E 12
#define PIN_DISPLAY_SEGMENT_F 13

#define PIN_BUZZER_FAKE 68

#define PIN_BUZZER A5

#ifdef ENABLE_SERIAL
#define PIN_DISPLAY_SEGMENT_DP PIN_DUMMY_2
#define PIN_DISPLAY_SEGMENT_G PIN_DUMMY_2 // was 3

#else
#define PIN_DISPLAY_SEGMENT_DP 0
#define PIN_DISPLAY_SEGMENT_G 1 // was 3

#endif

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
#define PIN_SELECTOR_DIAL A0
#define SELECTOR_DIAL_POSITIONS 12 //there are only 12 actual positions on the knob, but the resistor bridge has 13 positions on the final version (gnd is never provided, to avoid short circuits at the switch between ground and vcc)

#else
#define PIN_SELECTOR_BUTTON A0

#endif

#define PIN_ROTARY_ENCODER_DIAL_CHANNEL_A 2
#define PIN_ROTARY_ENCODER_DIAL_CHANNEL_B 3

#ifdef ENABLE_SOFT_POWER_OFF
#define PIN_POWER_ON_HOLD A4
#else
#define PIN_MERCURY_SWITCHES A4
#endif

#ifdef ENABLE_TILT_SWITCHES

#define SWITCH_TILT_FORWARD 11
#define SWITCH_TILT_BACKWARD 9
#define SWITCH_TILT_LEFT 8
#define SWITCH_TILT_RIGHT 10

#define BINARY_INPUTS_COUNT 12
#else
#define BINARY_INPUTS_COUNT 8

#endif

#define BIG_BUTTONS_COUNT 4

#if (defined V5_PCB_MINI)
#define PIN_BUTTONS_BIG A2
#define PIN_BUTTONS_SMALL A1
#define LIGHT_BUTTON_SMALL_0 0  //old: LIGHT_BUTTON_SMALL_0 0  // old: big latch
#define LIGHT_BUTTON_SMALL_1 1  //old: LIGHT_BUTTON_SMALL_1 1  // old: small latch left
#define LIGHT_BUTTON_SMALL_2 2  //old: LIGHT_BUTTON_SMALL_2 2  // old: small latch right
#define LIGHT_BUTTON_SMALL_3 3  //old: LIGHT_BUTTON_SMALL_3 3  // old: extra latch
#define LIGHT_BUTTON_BIG_0 7  //old: LIGHT_BUTTON_BIG_0 7 // old: m omentary 0
#define LIGHT_BUTTON_BIG_1 6  //old: LIGHT_BUTTON_BIG_1 6 // old: m omentary 1
#define LIGHT_BUTTON_BIG_2 5  //old: LIGHT_BUTTON_BIG_2 5 // old: m omentary 2
#define LIGHT_BUTTON_BIG_3 4  //old: LIGHT_BUTTON_BIG_3 4 // old: m omentary 3

#define BUTTONS_BIG_TO_BINARY_INPUT_OFFSET 4
#define BUTTONS_BIG_COUNT 4
#define BUTTONS_BIG_VALUES \
    {                            \
        512, 256, 128, 64        \
    }

#define BUTTONS_SMALL_COUNT 4
#define BUTTONS_SMALL_TO_BINARY_INPUT_OFFSET 0
#define BUTTONS_SMALL_VALUES \
    {                           \
        512, 256, 128, 64       \
    }

#define MERCURY_SWITCHES_COUNT 4
#define MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET 8
#define MERCURY_SWITCHES_VALUES \
    {                           \
        450, 262, 128, 64       \
    }

#elif (defined V3_PCB_BURST)
#define PIN_BUTTONS_BIG A2
#define PIN_BUTTONS_SMALL A1
#define LIGHT_BUTTON_BIG_0 5 // old: m omentary 0
#define LIGHT_BUTTON_BIG_1 2 // old: m omentary 1
#define LIGHT_BUTTON_BIG_2 6 // old: m omentary 2
#define LIGHT_BUTTON_BIG_3 1 // old: m omentary 3
#define LIGHT_BUTTON_SMALL_3 7  // old: extra latch
#define LIGHT_BUTTON_SMALL_1 4  // old: small latch left
#define LIGHT_BUTTON_SMALL_2 3  // old: small latch right
#define LIGHT_BUTTON_SMALL_0 0  // old: big latch

#define BUTTONS_BIG_TO_BINARY_INPUT_OFFSET 4
#define BUTTONS_BIG_COUNT 4
#define BUTTONS_BIG_VALUES \
    {                            \
        512, 256, 128, 64        \
    }

#define BUTTONS_SMALL_COUNT 4
#define BUTTONS_SMALL_TO_BINARY_INPUT_OFFSET 0
#define BUTTONS_SMALL_VALUES \
    {                           \
        512, 256, 128, 64       \
    }

#define MERCURY_SWITCHES_COUNT 4
#define MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET 8
#define MERCURY_SWITCHES_VALUES \
    {                           \
        450, 262, 128, 64       \
    }

#elif (defined V2_PCB_CORONA)
#define PIN_BUTTONS_BIG A1
#define PIN_BUTTONS_SMALL A2
#define LIGHT_BUTTON_BIG_0 1 // old: m omentary 0
#define LIGHT_BUTTON_BIG_1 6 // old: m omentary 1
#define LIGHT_BUTTON_BIG_2 2 // old: m omentary 2
#define LIGHT_BUTTON_BIG_3 5 // old: m omentary 3
#define LIGHT_BUTTON_SMALL_3 0  // old: extra latch
#define LIGHT_BUTTON_SMALL_1 3  // old: small latch left
#define LIGHT_BUTTON_SMALL_2 4  // old: small latch right
#define LIGHT_BUTTON_SMALL_0 7  // old: big latch
#define BUTTONS_BIG_TO_BINARY_INPUT_OFFSET 4
#define BUTTONS_BIG_COUNT 4
#define BUTTONS_BIG_VALUES \
    {                            \
        512, 256, 128, 64        \
    }

#define BUTTONS_SMALL_COUNT 4
#define BUTTONS_SMALL_TO_BINARY_INPUT_OFFSET 0
#define BUTTONS_SMALL_VALUES \
    {                           \
        512, 256, 128, 64       \
    }

#define MERCURY_SWITCHES_COUNT 4
#define MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET 8
#define MERCURY_SWITCHES_VALUES \
    {                           \
        450, 262, 128, 64       \
    }

#else // v1 clouds pcb
#define PIN_BUTTONS_BIG A1
#define PIN_BUTTONS_SMALL A2
#define LIGHT_BUTTON_BIG_0 1 // old: m omentary 0
#define LIGHT_BUTTON_BIG_1 6 // old: m omentary 1
#define LIGHT_BUTTON_BIG_2 2 // old: m omentary 2
#define LIGHT_BUTTON_BIG_3 5 // old: m omentary 3
#define LIGHT_BUTTON_SMALL_3 0  // old: extra latch
#define LIGHT_BUTTON_SMALL_1 3  // old: small latch left
#define LIGHT_BUTTON_SMALL_2 4  // old: small latch right
#define LIGHT_BUTTON_SMALL_0 7  // old: big latch
#define BUTTONS_BIG_TO_BINARY_INPUT_OFFSET 3
#define BUTTONS_BIG_COUNT 5
#define BUTTONS_BIG_VALUES \
    {                            \
        500, 257, 128, 62, 30    \
    }

#define BUTTONS_SMALL_COUNT 3
#define BUTTONS_SMALL_TO_BINARY_INPUT_OFFSET 0
#define BUTTONS_SMALL_VALUES \
    {                           \
        513, 254, 125           \
    }

#define MERCURY_SWITCHES_COUNT 4
#define MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET 8
#define MERCURY_SWITCHES_VALUES \
    {                           \
        450, 262, 128, 64       \
    }
#endif

// postion of buttons in the indexed array.
// don't change indeces, for the sake of compression code, the sequence is fixed
#define BUTTON_INDEXED_BIG_0 0
#define BUTTON_INDEXED_BIG_1 1
#define BUTTON_INDEXED_BIG_2 2
#define BUTTON_INDEXED_BIG_3 3
#define BUTTON_INDEXED_SMALL_0 4 // old: big latch
#define BUTTON_INDEXED_SMALL_1 5 // old: small latch left
#define BUTTON_INDEXED_SMALL_2 6 // old: small latch right
#define BUTTON_INDEXED_SMALL_3 7 // old: extra latch

#ifdef V2_PCB_CORONA
#define BUTTON_SMALL_0 0
#define BUTTON_SMALL_1 1
#define BUTTON_SMALL_2 2
#define BUTTON_SMALL_3 3

#define BUTTON_BIG_0 4 // most left
#define BUTTON_BIG_1 5 // second most left
#define BUTTON_BIG_2 6 // seconds most right
#define BUTTON_BIG_3 7 // most right

#elif defined V3_PCB_BURST
// binary input indeces per button
#define BUTTON_SMALL_0 0
#define BUTTON_SMALL_1 1
#define BUTTON_SMALL_2 2
#define BUTTON_SMALL_3 3

#define BUTTON_BIG_0 4 // most left
#define BUTTON_BIG_1 5 // second most left
#define BUTTON_BIG_2 6 // seconds most right
#define BUTTON_BIG_3 7 // most right
#elif defined V5_PCB_MINI

// binary input indeces per button
#define BUTTON_SMALL_0 0  // #define B UTTON_L ATCHING_0 0
#define BUTTON_SMALL_1 1  // #define B UTTON_L ATCHING_1 1
#define BUTTON_SMALL_2 2  // #define B UTTON_L ATCHING_2 2
#define BUTTON_SMALL_3 3  // #define B UTTON_L ATCHING_3 3
#define BUTTON_BIG_0 4 // old: BUTTON_MO MENTARY_0 4 // most left
#define BUTTON_BIG_1 5 // old: BUTTON_MO MENTARY_1 5 // second most left
#define BUTTON_BIG_2 6 // old: BUTTON_MO MENTARY_2 6 // seconds most right
#define BUTTON_BIG_3 7 /// old: BUTTON_M OMENTARY_3 7 // most right



#else
// binary input indeces per button
#define BUTTON_SMALL_2 0
#define BUTTON_SMALL_1 1
#define BUTTON_SMALL_0 2

#define BUTTON_SMALL_3 7 // L atching yellow

#define BUTTON_BIG_0 6 // most left (Red)
#define BUTTON_BIG_1 5 // Green (second most left)
#define BUTTON_BIG_2 4 // Blue (seconds most right)
#define BUTTON_BIG_3 3 // most right (Yellow)

#endif

// #define POTENTIO_SENSITIVITY 5 //value change before value update.

#ifdef ENABLE_EEPROM

#define EEPROM_TOTAL_RESET 0 // not an address
#define EEPROM_USER_RESET 1 // not an address
#define EEPROM_APP_SETTINGS_RESET 2 // not an address

#define EEPROM_SETTINGS_OFFSET 0
#define EEPROM_SOUND_DISABLED EEPROM_SETTINGS_OFFSET + 0                    // 1 byte
#define EEPROM_LUCIEBOX_POWER_CYCLE_COUNTER EEPROM_SETTINGS_OFFSET + 1     // 2 bytes
#define EEPROM_LUCIEBOX_AUTO_POWER_OFF_DISABLED EEPROM_SETTINGS_OFFSET + 3 // 1 byte

#ifndef ENABLE_SELECT_APPS_WITH_SELECTOR
#define EEPROM_LUCIEBOX_ACTIVE_APP_AT_SHUTDOWN EEPROM_SETTINGS_OFFSET + 4 // 1 byte
#endif

#define EEPROM_SETTINGS_TOTAL_LENGTH 6 // 6 bytes


// start user range (from here to end)


#define EEPROM_SEQUENCER_SONGS_START_ADDRESS EEPROM_SETTINGS_OFFSET + EEPROM_SETTINGS_TOTAL_LENGTH

#define EEPROM_FIRST_ADDRESS_OF_USER_RANGE EEPROM_SEQUENCER_SONGS_START_ADDRESS

// #define EEPROM_SEQUENCER_SONGS_START_ADDRESS EEPROM_POMODORO_START_ADDRESS + EEPROM_POMODORO_TOTAL_LENGTH
#define EEPROM_SEQUENCER_SONGS_COUNT 4
#define EEPROM_SEQUENCER_SONG_LENGTH 32
#define EEPROM_SEQUENCER_TOTAL_LENGTH EEPROM_SEQUENCER_SONGS_COUNT *EEPROM_SEQUENCER_SONG_LENGTH //4*32 = 128

#define EEPROM_COMPOSER_SONGS_START_ADDRESS EEPROM_SEQUENCER_SONGS_START_ADDRESS + EEPROM_SEQUENCER_TOTAL_LENGTH
#define EEPROM_COMPOSER_SONG_LENGTH 100
#define EEPROM_COMPOSER_SONG_COUNT 4                                                         // let's make four songs available.
#define EEPROM_COMPOSER_TOTAL_LENGTH EEPROM_COMPOSER_SONG_LENGTH *EEPROM_COMPOSER_SONG_COUNT //4*100 = 400

#define EEPROM_PICTURES_START_ADDRESS EEPROM_COMPOSER_SONGS_START_ADDRESS + EEPROM_COMPOSER_TOTAL_LENGTH
#define EEPROM_NUMBER_OF_DRAWINGS 93
#define EEPROM_PICTURE_LENGTH 4
#define EEPROM_PICTURES_TOTAL_LENGTH EEPROM_NUMBER_OF_DRAWINGS *EEPROM_PICTURE_LENGTH // 93 * 4 = 372


// start soft reset range (From here to end)
#define EEPROM_TALLY_KEEPER_START_ADDRESS EEPROM_PICTURES_START_ADDRESS + EEPROM_PICTURES_TOTAL_LENGTH
#define EEPROM_FIRST_ADDRESS_APP_SETTINGS_RESET EEPROM_TALLY_KEEPER_START_ADDRESS
#define EEPROM_TALLY_KEEPER_SCORES EEPROM_TALLY_KEEPER_START_ADDRESS + 0 // 4 scores * 2 bytes
#define EEPROM_TALLY_KEEPER_TOTAL_LENGTH 8

#define EEPROM_RANDOM_WORLD_START_ADDRESS EEPROM_TALLY_KEEPER_START_ADDRESS + EEPROM_TALLY_KEEPER_TOTAL_LENGTH
#define EEPROM_RANDOM_WORLD_UPPER_BOUNDARY_NUMBER_DRAW EEPROM_RANDOM_WORLD_START_ADDRESS + 0
#define EEPROM_RANDOM_WORLD_TOTAL_LENGTH 2

#define EEPROM_REACTION_GAME_START_ADDRESS EEPROM_RANDOM_WORLD_START_ADDRESS + EEPROM_RANDOM_WORLD_TOTAL_LENGTH
#define EEPROM_REACTION_GAME_WHACK_A_MOLE              // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_WHACK_A_MOLE_TIMED        // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_WHACK_A_BIRD              // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_WHACK_A_BIRD_TIMED        // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_GUITAR_HERO               // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_GUITAR_HERO_WITH_PAUSES   // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_HEX_HERO                  // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_HEX_HERO_WITH_COMPLEMENTS // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_TOTAL_LENGTH 96           // in game 8 games types * 6 levels * 2 bytes

#define EEPROM_MULTITIMER_START_ADDRESS EEPROM_REACTION_GAME_START_ADDRESS + EEPROM_REACTION_GAME_TOTAL_LENGTH
#define EEPROM_MULTITIMER_TIMERS_COUNT EEPROM_MULTITIMER_START_ADDRESS + 0                 // 1 byte
#define EEPROM_MULTITIMER_TIMERS_INIT_TIME_START_INDEX EEPROM_MULTITIMER_START_ADDRESS + 1 // 4* 1 bytes
#define EEPROM_MULTITIMER_FISHER_TIME_INDEX EEPROM_MULTITIMER_START_ADDRESS + 5            // 1 bytes
#define EEPROM_MULTITIMER_SURVIVE_AT_TIMEOUT EEPROM_MULTITIMER_START_ADDRESS + 6            // 1 bytes
#define EEPROM_MULTITIMER_TOTAL_LENGTH 7                                                   // 7 bytes timers count, timers init time index, fischer time index

#define EEPROM_POMODORO_START_ADDRESS EEPROM_MULTITIMER_START_ADDRESS + EEPROM_MULTITIMER_TOTAL_LENGTH
#define EEPROM_POMODORO_INIT_TIME_INDEX EEPROM_POMODORO_START_ADDRESS + 0
#define EEPROM_POMODORO_PAUSE_TIME_INDEX EEPROM_POMODORO_START_ADDRESS + 1
#define EEPROM_POMODORO_RND_BEEP_TIME_INDEX EEPROM_POMODORO_START_ADDRESS + 2
#define EEPROM_POMODORO_TOTAL_LENGTH 3



#define EEPROM_LAST_ADDRESS 1023

#endif
