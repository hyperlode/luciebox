#include "PotentioSelector.h"
#include "ButtonsDacR2r.h"
#include "BinaryInput.h"
#include "Button.h"

// Lode Ameije 2019-05
// Pretbak is a busy box for my newborn niece
// The hardware is a box with
// input: buttons, momentary and switches, a potentiometer, a selector dial, some mercury switches.
// output: lights on buttons, 7seg display (4 digits), buzzer

#define PIN_SELECTOR_DIAL A0
#define PIN_BUTTONS_1 A1
#define PIN_BUTTONS_2 A2
#define PIN_POTENTIO A3

#define PIN_BUTTON_DOUBLE_LATCH 2
#define PIN_BUTTON_BIG_LATCH 3

#define BINARY_INPUTS_COUNT 8
#define BUTTONS_1_COUNT 4
#define BUTTONS_1_TO_BINARY_INPUT_OFFSET 4

#define BUT_BIG 0
#define TILT 1
#define BUT_L 2
#define BUT_R 3

#define BUTTON_LATCHING_BIG_RED 0
#define TILT_SWITCH_VERTICAL 1
#define BUTTON_LATCHING_SMALL_RED_LEFT 2
#define BUTTON_LATCHING_SMALL_RED_RIGHT 3
#define BUTTON_LATCHING_YELLOW 4
#define BUTTON_MOMENTARY_RED 5
#define BUTTON_MOMENTARY_GREEN 6
#define BUTTON_MOMENTARY_BLUE 7

BinaryInput binaryInputs[BINARY_INPUTS_COUNT];

PotentioSelector selectorDial;
ButtonsDacR2r buttons_1;
Button buttons[4];

void refresh(){
  selectorDial.refresh();
  buttons_1.refresh();
  
  for(uint8_t i=0;i<4;i++){
    buttons[i].twoButtonsOnePinHandler();
    buttons[i].refresh();
    binaryInputs[i].setValue(buttons[i].getState());
  }

  if (buttons_1.getValueChangedEdge()) {
    Serial.println("analog in buttons:");
//    Serial.println(buttons_1.getButtonsValueAnalog());
    //Serial.println(buttons_1.getButtonsValue());
    for (uint8_t i=0; i< BUTTONS_1_COUNT; i++){
      binaryInputs[BUTTONS_1_TO_BINARY_INPUT_OFFSET + i].setValue(buttons_1.getButtonValueByIndex(i));
      Serial.println("------");
      Serial.println(i);
//      Serial.println(binaryInputs[i].getValue());
//      Serial.println(binaryInputs[i].getValueChanged());
//      Serial.println(binaryInputs[i].getEdgeUp());
//      Serial.println(binaryInputs[i].getEdgeDown());
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
//    Serial.println(selectorDial.getSelectorValue());
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


void setup() {
  // put your setup code here, to run once:
  selectorDial.setPin(PIN_SELECTOR_DIAL);
  buttons_1.setPin(PIN_BUTTONS_1,BUTTONS_1_COUNT);
  
  buttons[BUT_L].setPin(PIN_BUTTON_DOUBLE_LATCH);
  buttons[BUT_R].setPin(PIN_BUTTON_DOUBLE_LATCH);

  buttons[BUT_BIG].setPin(PIN_BUTTON_BIG_LATCH);
  buttons[TILT].setPin(PIN_BUTTON_BIG_LATCH);

  buttons[BUT_L].setTwoButtonsOnePinButtonConnectedToGnd(true);
  buttons[BUT_R].setTwoButtonsOnePinButtonConnectedToGnd(false);
  
  buttons[BUT_BIG].setTwoButtonsOnePinButtonConnectedToGnd(false);
  buttons[TILT].setTwoButtonsOnePinButtonConnectedToGnd(true);


  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  refresh();
}
