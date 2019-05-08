#include "PotentioSelector.h"
#include "ButtonsDacR2r.h"

// Lode Ameije 2019-05
// Pretbak is a busy box for my newborn niece
// The hardware is a box with
// input: buttons, momentary and switches, a potentiometer, a selector dial, some mercury switches.
// output: lights on buttons, 7seg display (4 digits), buzzer

#define PIN_SELECTOR_DIAL A0
#define PIN_BUTTONS_1 A1
#define PIN_BUTTONS_2 A2
#define PIN_POTENTIO A3

#define BUTTON_LATCHING_BIG_RED 0
#define BUTTON_LATCHING_SMALL_RED_LEFT 1
#define BUTTON_LATCHING_SMALL_RED_LEFT 2
#define BUTTON_LATCHING_YELLOW 3
#define BUTTON_MOMENTARY_RED 4
#define BUTTON_MOMENTARY_GREEN 5
#define BUTTON_MOMENTARY_BLUE 6

//bool buttons[7];
//bool buttonsEdgeUp[7];
//bool buttonsEdgeDown[7];
//bool buttonsPreviousState[7];

PotentioSelector selectorDial;
//PotentioSelector buttonsAnalog;
ButtonsDacR2r buttons_1;

void refresh(){
  selectorDial.refresh();
  buttons_1.refresh();
//  Serial.println("papa help "); // display this PAPA - HELP - PAPA - ... on screen when errors.
    
//  buttonsAnalog.refresh();  

  if (buttons_1.getValueChangedEdge()) {
    Serial.println(buttons_1.getButtonsValue());
  }
  
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
  buttons_1.setPin(PIN_BUTTONS_1,4);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  refresh();
}
