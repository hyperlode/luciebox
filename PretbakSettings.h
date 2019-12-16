#define DISPLAY_IS_COMMON_ANODE true  //check led displays both displays should be of same type   //also set in SevSeg5Digits.h : MODEISCOMMONANODE

 

#ifdef PROTOTYPE //I made a mistake in pcb v1.0.0 by swapping the digit pins. So, now I make it a feature. The prototype hardware has to be changed if we want this to work.
	#define PIN_DUMMY 66
	#define PIN_DISPLAY_DIGIT_0 PIN_DUMMY  //invalid
	#define PIN_DISPLAY_DIGIT_1 5
	#define PIN_DISPLAY_DIGIT_2 9
	#define PIN_DISPLAY_DIGIT_3 10
	#define PIN_DISPLAY_DIGIT_4 11
	#define PIN_DISPLAY_DIGIT_BUTTON_LIGHTS 6
#else
	#define PIN_DUMMY 66
	#define PIN_DISPLAY_DIGIT_0 PIN_DUMMY  //invalid
	#define PIN_DISPLAY_DIGIT_1 11
	#define PIN_DISPLAY_DIGIT_2 10
	#define PIN_DISPLAY_DIGIT_3 9
	#define PIN_DISPLAY_DIGIT_4 5
	#define PIN_DISPLAY_DIGIT_BUTTON_LIGHTS 6
#endif

#define PIN_DISPLAY_SEGMENT_A 7
#define PIN_DISPLAY_SEGMENT_B 2
#define PIN_DISPLAY_SEGMENT_C 4
#define PIN_DISPLAY_SEGMENT_D 8
#define PIN_DISPLAY_SEGMENT_E 12
#define PIN_DISPLAY_SEGMENT_F 13
#define PIN_DISPLAY_SEGMENT_G 3
#define PIN_DISPLAY_SEGMENT_DP 0 

#define PIN_BUZZER A5
  
#define LIGHT_RED 1
#define LIGHT_GREEN 6
#define LIGHT_BLUE 2
#define LIGHT_YELLOW 0
#define LIGHT_LED_1 3
#define LIGHT_LED_2 4
#define LIGHT_LED_3 5

#define PIN_SELECTOR_DIAL A0
#define PIN_BUTTONS_1 A1
#define PIN_BUTTONS_2 A2
#define PIN_MERCURY_SWITCHES A4
#define PIN_POTENTIO A3


#define BINARY_INPUTS_COUNT 11 

#define BUTTONS_1_COUNT 4
#define BUTTONS_1_TO_BINARY_INPUT_OFFSET 3

#define BUTTONS_2_COUNT 3
#define BUTTONS_2_TO_BINARY_INPUT_OFFSET 0

#define MERCURY_SWITCHES_COUNT 4
#define MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET 7

#define BUTTON_LATCHING_BIG_RED 2
#define BUTTON_LATCHING_SMALL_RED_LEFT 1
#define BUTTON_LATCHING_SMALL_RED_RIGHT 0
#define BUTTON_LATCHING_YELLOW 6
#define BUTTON_MOMENTARY_RED 5
#define BUTTON_MOMENTARY_GREEN 4
#define BUTTON_MOMENTARY_BLUE 3

#define SWITCH_TILT_FORWARD 10
#define SWITCH_TILT_BACKWARD 8
#define SWITCH_TILT_LEFT 7
#define SWITCH_TILT_RIGHT 9

#define POTENTIO_SENSITIVITY 5 //value change before value update.
