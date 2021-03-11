
#define SOFTWARE_VERSION 300 
// started at v2.00: 200
// v300 : pcb v3 burst implemented. change in button sequence...

// #define DBUG_REFACTOR_DISP
//#define ENABLE_TILT_SWITCHES
#define ENABLE_SERIAL  //for debugging. if used, pin 0 and 1 cannot be used for other purposes than tx and rx
//#define ENABLE_ANALOG_PIN_DEBUG  // for debugging at startup (always available in apps anyways.)
// #define DBUG_REFACTOR_DISP

// #define V2_PCB_CORONA
#define V3_PCB_BURST

#define ENABLE_APPS 

#ifdef ENABLE_SERIAL
#define DEBUG_ENCODER_DIAL
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
#define PIN_DUMMY_2 67

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

#define LIGHT_MOMENTARY_0 1          // old: momentary 0
#define LIGHT_MOMENTARY_1 6          // old: momentary 1
#define LIGHT_MOMENTARY_2 2          // old: momentary 2
#define LIGHT_MOMENTARY_3 5          // old: momentary 3
#define LIGHT_LATCHING_3 0           // old: extra latch
#define LIGHT_LATCHING_1 3           // old: small latch left
#define LIGHT_LATCHING_2 4           // old: small latch right
#define LIGHT_LATCHING_0 7           // old: big latch




#define PIN_SELECTOR_DIAL A0
#define PIN_BUTTONS_1 A1
#define PIN_BUTTONS_2 A2
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


#if (defined V2_PCB_CORONA || defined V3_PCB_BURST)
    #define BUTTONS_1_TO_BINARY_INPUT_OFFSET 4
    #define BUTTONS_1_COUNT 4
    #define BUTTONS_1_VALUES      \
        {                         \
            512, 256, 128, 64 \
        }

    #define BUTTONS_2_COUNT 4
    #define BUTTONS_2_TO_BINARY_INPUT_OFFSET 0  
    #define BUTTONS_2_VALUES \
        {                    \
            512, 256, 128, 64    \
        }

    #define MERCURY_SWITCHES_COUNT 4
    #define MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET 8
    #define MERCURY_SWITCHES_VALUES \
        {                           \
            450, 262, 128, 64       \
        }

#else  // v1 pcb
    #define BUTTONS_1_TO_BINARY_INPUT_OFFSET 3
    #define BUTTONS_1_COUNT 5
    #define BUTTONS_1_VALUES      \
        {                         \
            500, 257, 128, 62, 30 \
        }

    #define BUTTONS_2_COUNT 3
    #define BUTTONS_2_TO_BINARY_INPUT_OFFSET 0
    #define BUTTONS_2_VALUES \
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
#define EEPROM_REACTION_GAME_OFFSET                     0


// wrong... see code. built up differently...
#define EEPROM_REACTION_GAME_WHACK_A_MOLE               EEPROM_REACTION_GAME_OFFSET + 0 // 10 levels, each 2 bytes.
#define EEPROM_REACTION_GAME_WHACK_A_MOLE_TIMED         EEPROM_REACTION_GAME_OFFSET + 18 // 10 levels, each 2 bytes.
#define EEPROM_REACTION_GAME_WHACK_A_BIRD               EEPROM_REACTION_GAME_OFFSET + 34 // 10 levels, each 2 bytes.
#define EEPROM_REACTION_GAME_WHACK_A_BIRD_TIMED         EEPROM_REACTION_GAME_OFFSET + 54 // 10 levels, each 2 bytes.
#define EEPROM_REACTION_GAME_GUITAR_HERO                EEPROM_REACTION_GAME_OFFSET + 72 // 10 levels, each 2 bytes.
#define EEPROM_REACTION_GAME_TOTAL_LENGTH               96  // in game 8 games types * 6 levels * 2 bytes

#define EEPROM_SETTINGS_OFFSET                          EEPROM_REACTION_GAME_OFFSET + EEPROM_REACTION_GAME_TOTAL_LENGTH
#define EEPROM_SOUND_OFF_BY_DEFAULT                     EEPROM_SETTINGS_OFFSET + 0
#define EEPROM_SETTINGS_TOTAL_LENGTH                    2


#define EEPROM_SEQUENCER_SONGS_START_ADDRESS            EEPROM_SETTINGS_OFFSET + EEPROM_SETTINGS_TOTAL_LENGTH
#define EEPROM_SEQUENCER_SONGS_COUNT                    5
#define EEPROM_SEQUENCER_SONG_LENGTH                    32
#define EEPROM_SEQUENCER_TOTAL_LENGTH                   EEPROM_SEQUENCER_SONGS_COUNT * EEPROM_SEQUENCER_SONG_LENGTH //5*32 = 160

#define EEPROM_COMPOSER_SONGS_START_ADDRESS             EEPROM_SEQUENCER_SONGS_START_ADDRESS + EEPROM_SEQUENCER_TOTAL_LENGTH
#define EEPROM_COMPOSER_SONG_LENGTH                     100
#define EEPROM_COMPOSER_SONG_COUNT                      4  // let's make four songs available.
#define EEPROM_COMPOSER_TOTAL_LENGTH                    EEPROM_COMPOSER_SONG_LENGTH * EEPROM_COMPOSER_SONG_COUNT //4*100 = 400

#define EEPROM_PICTURES_START_ADDRESS                   EEPROM_COMPOSER_SONGS_START_ADDRESS + EEPROM_COMPOSER_TOTAL_LENGTH
#define EEPROM_NUMBER_OF_DRAWINGS                       90 // 
#define EEPROM_PICTURE_LENGTH                           4
#define EEPROM_PICTURES_TOTAL_LENGTH                    EEPROM_NUMBER_OF_DRAWINGS * EEPROM_PICTURE_LENGTH  // 90 * 4 = 360


#endif