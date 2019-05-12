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
 
#define PIN_DISPLAY_DIGIT_0 6
#define PIN_DISPLAY_DIGIT_1 5
#define PIN_DISPLAY_DIGIT_2 9
#define PIN_DISPLAY_DIGIT_3 10
#define PIN_DISPLAY_DIGIT_4 11

#define PIN_DISPLAY_SEGMENT_A 7
#define PIN_DISPLAY_SEGMENT_B 2
#define PIN_DISPLAY_SEGMENT_C 4
#define PIN_DISPLAY_SEGMENT_D 8
#define PIN_DISPLAY_SEGMENT_E 12
#define PIN_DISPLAY_SEGMENT_F 13
#define PIN_DISPLAY_SEGMENT_G 3
#define PIN_DISPLAY_SEGMENT_DP A4

#define PIN_BUZZER 13 
  



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
#define BUTTON_LATCHING_YELLOW 3
#define BUTTON_MOMENTARY_RED 4
#define BUTTON_MOMENTARY_GREEN 5
#define BUTTON_MOMENTARY_BLUE 6

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

void refresh(){

  ledDisp.refresh();


  
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
//  Serial.println("papa help "); // display this PAPA - HELP - PAPA - ... on screen when errors.
 
  if (selectorDial.getValueChangedEdge()) {
    Serial.println(selectorDial.getSelectorValue());
    switch (selectorDial.getSelectorValue()) {
      case 0:
//        Serial.println("zeor");
        break;
      case 1:
        break;
      default:
        break;
    }
  }  
}

void mode_auto(){

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

  ledDisp.startUp(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_4, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);
  textBuf[1]='L';
  textBuf[2]='O';
  textBuf[3]='D';
  textBuf[4]='E';
  textBuf[5]='/0';
  ledDisp.setDecimalPoint(true, 1);
  ledDisp.setDecimalPoint(true, 2);
  ledDisp.setDecimalPoint(true, 3);
  ledDisp.setDecimalPoint(true, 4);
  ledDisp.displayHandler(textBuf);


//  ledDisp.setIsScrolling(true);
//  ledDisp.doSequence();
//  ledDisp.doScroll();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  refresh();
}
