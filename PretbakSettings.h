#define DISPLAY_IS_COMMON_ANODE true  //check led displays both displays should be of same type   //also set in SevSeg5Digits.h : MODEISCOMMONANODE

#define PIN_DUMMY 66
#define PIN_DUMMY_2 67



#ifdef PROTOTYPE //I made a mistake in pcb v1.0.0 by swapping the digit pins. So, now I make it a feature. The prototype hardware has to be changed if we want this to work.
    
    #define PIN_DISPLAY_DIGIT_0 PIN_DUMMY  //invalid
    #define PIN_DISPLAY_DIGIT_1 5
    #define PIN_DISPLAY_DIGIT_2 9
    #define PIN_DISPLAY_DIGIT_3 10
    #define PIN_DISPLAY_DIGIT_4 11
    #define PIN_DISPLAY_DIGIT_BUTTON_LIGHTS 6
    #define SELECTOR_DIAL_POSITIONS 12  

#else
    
    #define PIN_DISPLAY_DIGIT_0 PIN_DUMMY  //invalid
    #define PIN_DISPLAY_DIGIT_1 11
    #define PIN_DISPLAY_DIGIT_2 10
    #define PIN_DISPLAY_DIGIT_3 9
    #define PIN_DISPLAY_DIGIT_4 5
    #define PIN_DISPLAY_DIGIT_BUTTON_LIGHTS 6
    #define SELECTOR_DIAL_POSITIONS 12  //there are only 12 actual positions on the knob, but the resistor bridge has 13 positions on the final version (gnd is never provided, to avoid short circuits at the switch between ground and vcc)

#endif

#define PIN_DISPLAY_SEGMENT_A 7
#define PIN_DISPLAY_SEGMENT_B 2
#define PIN_DISPLAY_SEGMENT_C 4
#define PIN_DISPLAY_SEGMENT_D 8
#define PIN_DISPLAY_SEGMENT_E 12
#define PIN_DISPLAY_SEGMENT_F 13
#define PIN_DISPLAY_SEGMENT_G 3

#ifndef ENABLE_SERIAL
  #define PIN_DISPLAY_SEGMENT_DP 0 
#else
  #define PIN_DISPLAY_SEGMENT_DP PIN_DUMMY_2
#endif 

#define PIN_BUZZER A5
  
#define LIGHT_RED 	    1
#define LIGHT_GREEN     6
#define LIGHT_BLUE 	    2
#define LIGHT_YELLOW    0
#define LIGHT_LED_1     3
#define LIGHT_LED_2     4
#define LIGHT_LED_3     5

#define PIN_SELECTOR_DIAL A0
#define PIN_BUTTONS_1 A1
#define PIN_BUTTONS_2 A2
#define PIN_POTENTIO A3
#define PIN_MERCURY_SWITCHES A4


#ifdef PROTOTYPE
  #define BINARY_INPUTS_COUNT 11 
#else
  #define BINARY_INPUTS_COUNT 12 
#endif

#define BUTTONS_1_TO_BINARY_INPUT_OFFSET 3

#ifdef PROTOTYPE
  #define BUTTONS_1_COUNT 4
  #define BUTTONS_1_VALUES {512,256,128,64}
#else
    
  #define BUTTONS_1_COUNT 5
  //#define BUTTONS_1_VALUES {446,223,109,53}
  #define BUTTONS_1_VALUES {509,253,129,63,32}
#endif

#define BUTTONS_2_COUNT 3
#define BUTTONS_2_TO_BINARY_INPUT_OFFSET 0

#ifdef PROTOTYPE
  #define BUTTONS_2_VALUES {512,256,128}
#else
//  #define BUTTONS_2_VALUES {452,219,107}
  #define BUTTONS_2_VALUES {525,263,117}
#endif

#define MERCURY_SWITCHES_COUNT 4

#ifdef PROTOTYPE
  #define MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET 7
  #define MERCURY_SWITCHES_VALUES {512, 256, 128, 64}
#else
  #define MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET 8
  #define MERCURY_SWITCHES_VALUES {512, 256, 128, 64}
#endif

// binary input indeces per button
#define BUTTON_LATCHING_BIG_RED 2
#define BUTTON_LATCHING_SMALL_RED_LEFT 1
#define BUTTON_LATCHING_SMALL_RED_RIGHT 0
#define BUTTON_LATCHING_YELLOW 6
#define BUTTON_MOMENTARY_RED 5
#define BUTTON_MOMENTARY_GREEN 4
#define BUTTON_MOMENTARY_BLUE 3


#ifdef PROTOTYPE
  
    #define SWITCH_TILT_FORWARD 10
    #define SWITCH_TILT_BACKWARD 8
    #define SWITCH_TILT_LEFT 7
    #define SWITCH_TILT_RIGHT 9
#else
    #define BUTTON_MOMENTARY_EXTRA_YELLOW 7

    #define SWITCH_TILT_FORWARD 11
    #define SWITCH_TILT_BACKWARD 9
    #define SWITCH_TILT_LEFT 8
    #define SWITCH_TILT_RIGHT 10
#endif

#define POTENTIO_SENSITIVITY 5 //value change before value update.


