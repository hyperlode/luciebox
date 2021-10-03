
#define SOFTWARE_VERSION 305
// started at v2.00: 200
// v300 : pcb v3 burst implemented. change in button sequence...
// v301 : sending out to Belgium 2021-03-19
// v302 : development since 2021-03-19. Numerous small bugfixes and additional functionality.
// v303 : settings debug selector dial. Make sure values are not too erratic. small bugfixes.
// v304 : quizmaster scoring bugfix and optimization / baby dreamtime mode change behaviour + shift button keeps it in dreamtime mode (all button mash apps should have the shift button included). / total time timer is now set to a timer of the chrono app.
// v305 : add battery voltage indicator / buzzer rework (note scheme, buzzer buffer)

// #define ENABLE_SERIAL  //for debugging. if used, pin 0 and 1 cannot be used for other purposes than tx and rx
//#define ENABLE_TILT_SWITCHES
//#define ENABLE_ANALOG_PIN_DEBUG  // for debugging at startup (always available in apps anyways.)


// #define V2_PCB_CORONA
#define V3_PCB_BURST

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
#define PIN_DUMMY_2 22  // randomly chosen. I've had it set to 67, and at some point, multiple segments were lit up. This STILL is C hey, it's gonna chug on forever!

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

#define PIN_SELECTOR_DIAL A0

#define PIN_ROTARY_ENCODER_DIAL_CHANNEL_A 2
#define PIN_ROTARY_ENCODER_DIAL_CHANNEL_B 3

#define PIN_MERCURY_SWITCHES A4

#define SELECTOR_DIAL_POSITIONS 12 //there are only 12 actual positions on the knob, but the resistor bridge has 13 positions on the final version (gnd is never provided, to avoid short circuits at the switch between ground and vcc)


#ifdef ENABLE_TILT_SWITCHES
    
    #define SWITCH_TILT_FORWARD 11
    #define SWITCH_TILT_BACKWARD 9
    #define SWITCH_TILT_LEFT 8
    #define SWITCH_TILT_RIGHT 10

    #define BINARY_INPUTS_COUNT 12
#else
    #define BINARY_INPUTS_COUNT 8

#endif

#define MOMENTARY_BUTTONS_COUNT 4

#if defined V3_PCB_BURST
    #define PIN_BUTTONS_MOMENTARY A2
    #define PIN_BUTTONS_LATCHING A1
    #define LIGHT_MOMENTARY_0 5          // old: momentary 0
    #define LIGHT_MOMENTARY_1 2          // old: momentary 1
    #define LIGHT_MOMENTARY_2 6          // old: momentary 2
    #define LIGHT_MOMENTARY_3 1          // old: momentary 3
    #define LIGHT_LATCHING_3 7           // old: extra latch
    #define LIGHT_LATCHING_1 4           // old: small latch left
    #define LIGHT_LATCHING_2 3           // old: small latch right
    #define LIGHT_LATCHING_0 0           // old: big latch

    #define BUTTONS_MOMENTARY_TO_BINARY_INPUT_OFFSET 4
    #define BUTTONS_MOMENTARY_COUNT 4
    #define BUTTONS_MOMENTARY_VALUES      \
        {                         \
            512, 256, 128, 64 \
        }

    #define BUTTONS_LATCHING_COUNT 4
    #define BUTTONS_LATCHING_TO_BINARY_INPUT_OFFSET 0  
    #define BUTTONS_LATCHING_VALUES \
        {                    \
            512, 256, 128, 64    \
        }

    #define MERCURY_SWITCHES_COUNT 4
    #define MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET 8
    #define MERCURY_SWITCHES_VALUES \
        {                           \
            450, 262, 128, 64       \
        }

#elif (defined V2_PCB_CORONA)
    #define PIN_BUTTONS_MOMENTARY A1
    #define PIN_BUTTONS_LATCHING A2
    #define LIGHT_MOMENTARY_0 1          // old: momentary 0
    #define LIGHT_MOMENTARY_1 6          // old: momentary 1
    #define LIGHT_MOMENTARY_2 2          // old: momentary 2
    #define LIGHT_MOMENTARY_3 5          // old: momentary 3
    #define LIGHT_LATCHING_3 0           // old: extra latch
    #define LIGHT_LATCHING_1 3           // old: small latch left
    #define LIGHT_LATCHING_2 4           // old: small latch right
    #define LIGHT_LATCHING_0 7           // old: big latch
    #define BUTTONS_MOMENTARY_TO_BINARY_INPUT_OFFSET 4
    #define BUTTONS_MOMENTARY_COUNT 4
    #define BUTTONS_MOMENTARY_VALUES      \
        {                         \
            512, 256, 128, 64 \
        }

    #define BUTTONS_LATCHING_COUNT 4
    #define BUTTONS_LATCHING_TO_BINARY_INPUT_OFFSET 0  
    #define BUTTONS_LATCHING_VALUES \
        {                    \
            512, 256, 128, 64    \
        }

    #define MERCURY_SWITCHES_COUNT 4
    #define MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET 8
    #define MERCURY_SWITCHES_VALUES \
        {                           \
            450, 262, 128, 64       \
        }

#else  // v1 clouds pcb
    #define PIN_BUTTONS_MOMENTARY A1
    #define PIN_BUTTONS_LATCHING A2
    #define LIGHT_MOMENTARY_0 1          // old: momentary 0
    #define LIGHT_MOMENTARY_1 6          // old: momentary 1
    #define LIGHT_MOMENTARY_2 2          // old: momentary 2
    #define LIGHT_MOMENTARY_3 5          // old: momentary 3
    #define LIGHT_LATCHING_3 0           // old: extra latch
    #define LIGHT_LATCHING_1 3           // old: small latch left
    #define LIGHT_LATCHING_2 4           // old: small latch right
    #define LIGHT_LATCHING_0 7           // old: big latch
    #define BUTTONS_MOMENTARY_TO_BINARY_INPUT_OFFSET 3
    #define BUTTONS_MOMENTARY_COUNT 5
    #define BUTTONS_MOMENTARY_VALUES      \
        {                         \
            500, 257, 128, 62, 30 \
        }

    #define BUTTONS_LATCHING_COUNT 3
    #define BUTTONS_LATCHING_TO_BINARY_INPUT_OFFSET 0
    #define BUTTONS_LATCHING_VALUES \
        {                    \
            513, 254, 125    \
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
#define BUTTON_INDEXED_MOMENTARY_0 0  
#define BUTTON_INDEXED_MOMENTARY_1 1
#define BUTTON_INDEXED_MOMENTARY_2 2
#define BUTTON_INDEXED_MOMENTARY_3 3
#define BUTTON_INDEXED_LATCHING_0 4  // old: big latch
#define BUTTON_INDEXED_LATCHING_1 5  // old: small latch left
#define BUTTON_INDEXED_LATCHING_2 6  // old: small latch right
#define BUTTON_INDEXED_LATCHING_3 7  // old: extra latch

#ifdef V2_PCB_CORONA
    #define BUTTON_LATCHING_0 0
    #define BUTTON_LATCHING_1 1
    #define BUTTON_LATCHING_2 2
    #define BUTTON_LATCHING_3 3 

    #define BUTTON_MOMENTARY_0 4 // most left
    #define BUTTON_MOMENTARY_1 5 // second most left
    #define BUTTON_MOMENTARY_2 6 // seconds most right
    #define BUTTON_MOMENTARY_3 7 // most right

#elif defined V3_PCB_BURST
    // binary input indeces per button
    #define BUTTON_LATCHING_0 0
    #define BUTTON_LATCHING_1 1
    #define BUTTON_LATCHING_2 2
    #define BUTTON_LATCHING_3 3 

    #define BUTTON_MOMENTARY_0 4 // most left
    #define BUTTON_MOMENTARY_1 5 // second most left
    #define BUTTON_MOMENTARY_2 6 // seconds most right
    #define BUTTON_MOMENTARY_3 7 // most right

#else
    // binary input indeces per button
    #define BUTTON_LATCHING_2 0
    #define BUTTON_LATCHING_1 1
    #define BUTTON_LATCHING_0 2

    #define BUTTON_LATCHING_3 7 // Latching yellow

    #define BUTTON_MOMENTARY_0 6 // most left (Red)
    #define BUTTON_MOMENTARY_1 5 // Green (second most left)
    #define BUTTON_MOMENTARY_2 4 // Blue (seconds most right)
    #define BUTTON_MOMENTARY_3 3 // most right (Yellow)

#endif

#define POTENTIO_SENSITIVITY 5 //value change before value update.

#ifdef ENABLE_EEPROM

#define EEPROM_TOTAL_RESET 0
#define EEPROM_USER_RESET 1
#define EEPROM_HIGH_SCORES_RESET 2

#define EEPROM_SETTINGS_OFFSET                          0
#define EEPROM_SOUND_DISABLED                           EEPROM_SETTINGS_OFFSET + 0
#define EEPROM_LUCIEBOX_POWER_CYCLE_COUNTER             EEPROM_SETTINGS_OFFSET + 1  // 2bytes
// #define EEPROM_LUCIEBOX_HOURS                        EEPROM_SETTINGS_OFFSET + 3
#define EEPROM_SETTINGS_TOTAL_LENGTH                    6  // 8 bytes
#define EEPROM_FIRST_ADDRESS_OF_USER_RANGE              EEPROM_SETTINGS_OFFSET + EEPROM_SETTINGS_TOTAL_LENGTH

#define EEPROM_RANDOM_WORLD_START_ADDRESS               EEPROM_FIRST_ADDRESS_OF_USER_RANGE
#define EEPROM_RANDOM_WORLD_UPPER_BOUNDARY_NUMBER_DRAW  EEPROM_RANDOM_WORLD_START_ADDRESS + 0
#define EEPROM_RANDOM_WORLD_TOTAL_LENGTH                2

#define EEPROM_REACTION_GAME_START_ADDRESS              EEPROM_RANDOM_WORLD_START_ADDRESS + EEPROM_RANDOM_WORLD_TOTAL_LENGTH
#define EEPROM_REACTION_GAME_WHACK_A_MOLE               // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_WHACK_A_MOLE_TIMED         // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_WHACK_A_BIRD               // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_WHACK_A_BIRD_TIMED         // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_GUITAR_HERO                // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_GUITAR_HERO_WITH_PAUSES    // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_HEX_HERO                   // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_HEX_HERO_WITH_COMPLEMENTS  // 6 levels, each 2 bytes = 12bytes total
#define EEPROM_REACTION_GAME_TOTAL_LENGTH               96  // in game 8 games types * 6 levels * 2 bytes


#define EEPROM_MULTITIMER_START_ADDRESS                 EEPROM_REACTION_GAME_START_ADDRESS + EEPROM_REACTION_GAME_TOTAL_LENGTH
#define EEPROM_MULTITIMER_TIMERS_COUNT          EEPROM_MULTITIMER_START_ADDRESS + 0   // 1 byte
#define EEPROM_MULTITIMER_TIMERS_INIT_TIME_START_INDEX  EEPROM_MULTITIMER_START_ADDRESS + 1   // 4* 1 bytes
#define EEPROM_MULTITIMER_FISHER_TIME_INDEX             EEPROM_MULTITIMER_START_ADDRESS + 5   // 1 bytes
#define EEPROM_MULTITIMER_TOTAL_LENGTH                  6  // 6 bytes timers count, timers init time index, fischer time index


#define EEPROM_POMODORO_START_ADDRESS                   EEPROM_MULTITIMER_START_ADDRESS + EEPROM_MULTITIMER_TOTAL_LENGTH
#define EEPROM_POMODORO_INIT_TIME_INDEX                 EEPROM_POMODORO_START_ADDRESS + 0
#define EEPROM_POMODORO_PAUSE_TIME_INDEX                EEPROM_POMODORO_START_ADDRESS + 1
#define EEPROM_POMODORO_RND_BEEP_TIME_INDEX             EEPROM_POMODORO_START_ADDRESS + 2
#define EEPROM_POMODORO_TOTAL_LENGTH                    3

#define EEPROM_LAST_ADDRESS_OF_EEPROM_SOFT_ERASE       EEPROM_POMODORO_START_ADDRESS + EEPROM_POMODORO_TOTAL_LENGTH - 1

#define EEPROM_SEQUENCER_SONGS_START_ADDRESS            EEPROM_POMODORO_START_ADDRESS + EEPROM_POMODORO_TOTAL_LENGTH
#define EEPROM_SEQUENCER_SONGS_COUNT                    4
#define EEPROM_SEQUENCER_SONG_LENGTH                    32
#define EEPROM_SEQUENCER_TOTAL_LENGTH                   EEPROM_SEQUENCER_SONGS_COUNT * EEPROM_SEQUENCER_SONG_LENGTH //4*32 = 128

#define EEPROM_COMPOSER_SONGS_START_ADDRESS             EEPROM_SEQUENCER_SONGS_START_ADDRESS + EEPROM_SEQUENCER_TOTAL_LENGTH
#define EEPROM_COMPOSER_SONG_LENGTH                     100
#define EEPROM_COMPOSER_SONG_COUNT                      4  // let's make four songs available.
#define EEPROM_COMPOSER_TOTAL_LENGTH                    EEPROM_COMPOSER_SONG_LENGTH * EEPROM_COMPOSER_SONG_COUNT //4*100 = 400

#define EEPROM_PICTURES_START_ADDRESS                   EEPROM_COMPOSER_SONGS_START_ADDRESS + EEPROM_COMPOSER_TOTAL_LENGTH
#define EEPROM_NUMBER_OF_DRAWINGS                       95  
#define EEPROM_PICTURE_LENGTH                           4
#define EEPROM_PICTURES_TOTAL_LENGTH                    EEPROM_NUMBER_OF_DRAWINGS * EEPROM_PICTURE_LENGTH  // 95 * 4 = 380

#define EEPROM_LAST_ADDRESS                            1023

#endif
