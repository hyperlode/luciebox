#include "PotentioSelector.h"
#include "ButtonsDacR2r.h"
#include "BinaryInput.h"

#include "SevSeg5Digits.h" //sevseb -->this should not be included here, but only in DisplayDigitsHandler.h, as it only gets used there (but ARDUINO needs this here!) DEBUG
#include "DisplayDigitsHandler5Digits.h"


// Lode Ameije 2019-05
// Pretbak is a busy box for my newborn niece
// The hardware is a box with
// input: buttons, momentary and switches, a potentiometer, a selector dial, some mercury switches.
// output: lights on buttons, 7seg display (4 digits), buzzer

#define DISPLAY_IS_COMMON_ANODE true  //check led displays both displays should be of same type   //also set in SevSeg5Digits.h : MODEISCOMMONANODE
 
#define PIN_DISPLAY_DIGIT_0 0
#define PIN_DISPLAY_DIGIT_1 5
#define PIN_DISPLAY_DIGIT_2 9
#define PIN_DISPLAY_DIGIT_3 10
#define PIN_DISPLAY_DIGIT_4 11
#define PIN_DISPLAY_DIGIT_BUTTON_LIGHTS 6

#define PIN_DISPLAY_SEGMENT_A 7
#define PIN_DISPLAY_SEGMENT_B 2
#define PIN_DISPLAY_SEGMENT_C 4
#define PIN_DISPLAY_SEGMENT_D 8
#define PIN_DISPLAY_SEGMENT_E 12
#define PIN_DISPLAY_SEGMENT_F 13
#define PIN_DISPLAY_SEGMENT_G 3
#define PIN_DISPLAY_SEGMENT_DP A4

#define PIN_BUZZER 13 
  
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
#define PIN_POTENTIO A3

#define BINARY_INPUTS_COUNT 8
#define BUTTONS_1_COUNT 4
#define BUTTONS_1_TO_BINARY_INPUT_OFFSET 3

#define BUTTONS_2_COUNT 3
#define BUTTONS_2_TO_BINARY_INPUT_OFFSET 0

#define BUTTON_LATCHING_BIG_RED 2
#define BUTTON_LATCHING_SMALL_RED_LEFT 1
#define BUTTON_LATCHING_SMALL_RED_RIGHT 0
#define BUTTON_LATCHING_YELLOW 6
#define BUTTON_MOMENTARY_RED 5
#define BUTTON_MOMENTARY_GREEN 4
#define BUTTON_MOMENTARY_BLUE 3

#define POTENTIO_SENSITIVITY 5 //value change before value update.

// INPUT
BinaryInput binaryInputs[BINARY_INPUTS_COUNT];
PotentioSelector selectorDial;
ButtonsDacR2r buttons_1;
ButtonsDacR2r buttons_2; // buttons with normally closed. this is a problem for the R-2R ladder. instead, I used a pull down resistor to ground at the switch. so: ON = 5V, OFF = GND over 1Kohm. 10K, 20K R2Rladder.  will only work for limited number of buttons.
int16_t potentio_value;
int16_t potentio_value_stable;

// OUTPUT
DisplayManagement ledDisp;
char  textBuf [6];
char  scrollBuf [60];
uint8_t lights;

void refresh(){

  //output process
  
  ledDisp.doScroll();
  ledDisp.refresh();
  //input process  
  input_process();
  
  //Serial.println("papa help "); // display this PAPA - HELP - PAPA - ... on screen when errors.

  //modes functionality
  
  //mode change
  if (selectorDial.getValueChangedEdge()) {
    selector_value_changed();
  }

  mode_refresh();

    
}

void input_process(){
  selectorDial.refresh();
  buttons_1.refresh();
  buttons_2.refresh();
  potentio_refresh();

  
  if (buttons_2.getValueChangedEdge()) {
    //Serial.println("analog 2in buttons:");
    //Serial.println(buttons_2.getButtonsValueAnalog());
    //Serial.println(buttons_2.getButtonsValue());
    for (uint8_t i=0; i< BUTTONS_2_COUNT; i++){
      binaryInputs[BUTTONS_2_TO_BINARY_INPUT_OFFSET + i].setValue(buttons_2.getButtonValueByIndex(i));
    }
  }

  if (buttons_1.getValueChangedEdge()) {
    //Serial.println("analog in buttons:");
    //Serial.println(buttons_1.getButtonsValueAnalog());
    //Serial.println(buttons_1.getButtonsValue());
    for (uint8_t i=0; i< BUTTONS_1_COUNT; i++){
      binaryInputs[BUTTONS_1_TO_BINARY_INPUT_OFFSET + i].setValue(buttons_1.getButtonValueByIndex(i));
      //Serial.println("------");
      //Serial.println(i);
    }
  }
 
  for(uint8_t i=0;i<BINARY_INPUTS_COUNT;i++){
    if (binaryInputs[i].getEdgeUp()){
      Serial.println(i);
      Serial.println("PRESSED");
    }
    if (binaryInputs[i].getEdgeDown()){
      Serial.println(i);
      Serial.println("Button released");
    }
    binaryInputs[i].refresh();
  }
}

void selector_value_changed(){
  // as the selector dial is taken for main mode selection, init the mode here when selected
  Serial.println(selectorDial.getSelectorValue());
}

void mode_refresh(){
  
  //rotary 12 positions selector knob is taken as base for mode selecion. so there are 12 states. 
  
  switch (selectorDial.getSelectorValue()) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      // simple repetitive, predictive mode.
      // each button triggers its corresponding light. 
      // potentio sets display brightness
      // no buzzer
      // display lights up a segment for each button.
    
      lights = 0b00000000;
      if (binaryInputs[BUTTON_MOMENTARY_RED].getValue()){
        lights|= 1<<LIGHT_RED;
      }
      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getValue()){
        lights|= 1<<LIGHT_BLUE;
      }
      if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){
        lights|= 1<<LIGHT_YELLOW;
      }
      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getValue()){
        lights|= 1<<LIGHT_GREEN;
      }
      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
        lights|= 1<<LIGHT_LED_1;
      }
      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
        lights|= 1<<LIGHT_LED_2;
      }
      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
        lights|= 1<<LIGHT_LED_3;
      }
      
      ledDisp.SetLedArray(lights);
    
      break;
    case 8:
      break;
    case 9:
      break;
    case 10:
      break;
    case 11:
      break;
    
    default:
      break;
  }
  
}

void potentio_refresh(){
  potentio_value = (int16_t)analogRead(PIN_POTENTIO);
  if (potentio_value > potentio_value_stable + POTENTIO_SENSITIVITY || potentio_value < potentio_value_stable - POTENTIO_SENSITIVITY  ){
    potentio_value_stable = potentio_value;
    Serial.println(potentio_value_stable);
  }
}

void setup() {
  // put your setup code here, to run once:
  selectorDial.setPin(PIN_SELECTOR_DIAL);
  buttons_1.setPin(PIN_BUTTONS_1,BUTTONS_1_COUNT);
  buttons_2.setPin(PIN_BUTTONS_2,BUTTONS_2_COUNT);

  ledDisp.startUp(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_4, PIN_DISPLAY_DIGIT_BUTTON_LIGHTS, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);
  //ledDisp.startUp(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_4, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);
  textBuf[1]='L';
  textBuf[2]='O';
  textBuf[3]='D';
  textBuf[4]='E';
  textBuf[5]='/0';
  scrollBuf[0]='L';
  scrollBuf[1]='U';
  scrollBuf[2]='C';
  scrollBuf[3]='I';
  scrollBuf[4]='E';
  scrollBuf[5]=' ';
  scrollBuf[6]='B';
  scrollBuf[7]='A';
  scrollBuf[8]='B';
  scrollBuf[9]='Y';
  scrollBuf[10]='/0';
  ledDisp.setDecimalPoint(true, 1);
  ledDisp.setDecimalPoint(true, 2);
  ledDisp.setDecimalPoint(true, 3);
  ledDisp.setDecimalPoint(true, 4);
  
//  ledDisp.displayHandler(textBuf);

  
  //uint8_t buttonLights = 0b01000111; 
  lights = 0b00000000;
  
//  uint8_t buttonLights = 0b00000010; // 
  //uint8_t buttonLights = 0b00111000; // 3 extra leds
  
  ledDisp.SetLedArray(lights);


  //ledDisp.displayHandlerSequence(textBuf);
  ledDisp.dispHandlerWithScroll(scrollBuf, false, true);
//  ledDisp.setIsScrolling(true);
//  ledDisp.doSequence();
//  ledDisp.doScroll();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  refresh();
}
