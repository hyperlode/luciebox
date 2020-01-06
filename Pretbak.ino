#include "PotentioSelector.h"
#include "ButtonsDacR2r.h"
#include "BinaryInput.h"
#include "Buzzer.h"
#include "Apps.h"
#include "Potentio.h"
#include "DisplayDigitsHandler5Digits.h"
#include "PretbakSettings.h"
#include <EEPROM.h>

#define ENABLE_SERIAL  //for debugging. if used, pin 0 and 1 cannot be used for other purposes than tx and rx

#ifdef ENABLE_SERIAL
  //#define DEBUG_ANALOG_IN 
  // #define DEBUG_MERCURY
  //#define DEBUG_POTENTIO
  //#define DEBUG_BUTTONS
  //#define DEBUG_SELECTOR_KNOB
  //#define DEBUG_MINIMULTITIMER
  //#define DEBUG_SEQUENCER
  
  //#define SUPERDEBUG
  
#endif 

// Lode Ameije 2019-05
// Pretbak is a busy box for my newly born niece
// The hardware is a box with:
// input: momentary buttons, latching buttons, a potentiometer, a selector dial, four mercury switches.
// output: lights on buttons (one light per button), 7seg display (4 digits), buzzer

uint16_t setValues_1 [BUTTONS_1_COUNT] = BUTTONS_1_VALUES;
uint16_t setValues_2 [BUTTONS_2_COUNT] = BUTTONS_2_VALUES;
uint16_t setValues_mercury [MERCURY_SWITCHES_COUNT] = MERCURY_SWITCHES_VALUES;

//pretbak
Apps pretbak_apps;

// INPUT
BinaryInput binaryInputs[BINARY_INPUTS_COUNT];
PotentioSelector selectorDial;
ButtonsDacR2r buttons_1;
ButtonsDacR2r buttons_2; // buttons without normally closed. this is a problem for the R-2R ladder. instead, I used a pull down resistor to ground at the switch. so: ON = 5V, OFF = GND over 1Kohm. 10K, 20K R2Rladder.  will only work for limited number of buttons.
ButtonsDacR2r mercurySwitches;  // mercury switches go on or off depending on the position. Works with R-2R ladder. No NC in mercury switch, so, pulldown resistor (0.1R)to ground. R=10K 
Potentio potentio;

// OUTPUT
DisplayManagement ledDisp;
Buzzer buzzer;

//output
char  textBuf [6];
char  scrollBuf [40];

void refresh(){
  //input process  
  input_process();
  
  #ifdef SUPERDEBUG
  uint16_t setValues_1 [BUTTONS_1_COUNT] = BUTTONS_1_VALUES;
  
  uint16_t* tmp  = setValues_1;
  
  if (  potentio.getValueStableChangedEdge()){
    Serial.println("rawval:");
    Serial.println(analogRead(PIN_BUTTONS_2));
  }
  #endif
  
  #ifdef DEBUG_BUTTONS
    for(uint8_t i=0;i<BINARY_INPUTS_COUNT;i++){
      
      if (binaryInputs[i].getEdgeUp()){
        
        Serial.println("{{{-------PRESS:-------:");
        Serial.println(i);
     //   Serial.println( (analogRead(PIN_BUTTONS_1)));
      //  Serial.println( (analogRead(PIN_BUTTONS_2)));
       // Serial.println(buttons_1.getButtonsValueRaw(),BIN);
    //    Serial.println(buttons_2.getButtonsValueRaw(),BIN);
  
      //  Serial.println("-------PRESS:-------}}}");  
      }
      if (binaryInputs[i].getEdgeDown()){
        Serial.println("-----RELEASE:");
       // Serial.println(i);
        //Serial.println( (analogRead(PIN_BUTTONS_1)));
       
        //Serial.println(buttons_1.getButtonsValueRaw(),BIN);
        //Serial.println("-----------");
  //      Serial.println("Button released");
  //      Serial.println("analog in mercury switches edge down:");
        
      }
      
    }
      
  
  #endif
  
  //Serial.println("papa help "); // display this PAPA - HELP - PAPA - ... on screen when errors.

  //modes functionality
    
  //mode change
  if (selectorDial.getValueChangedEdge()) {
    //default mode (go to default state at each change)
    
#ifndef SUPERDEBUG
    pretbak_apps.setDefaultMode();
#endif
    
#ifdef DEBUG_SELECTOR_KNOB
    Serial.println("selector:");
    Serial.println(selectorDial.getSelectorValue());
    Serial.println(analogRead(PIN_SELECTOR_DIAL));
#endif
    
  }
  #ifndef SUPERDEBUG
  
  mode_refresh();
  #endif

  //output process
  ledDisp.refresh();
  #ifndef SUPERDEBUG
  buzzer.doBuzzerRoll();
  #endif
  
  for(uint8_t i=0;i<BINARY_INPUTS_COUNT;i++){
    binaryInputs[i].refresh();
  }
}

void input_process(){
  selectorDial.refresh();
  buttons_1.refresh();
  buttons_2.refresh();
  mercurySwitches.refresh();
  potentio.refresh();

  if (buttons_2.getValueChangedEdge()) {
    for (uint8_t i=0; i< BUTTONS_2_COUNT; i++){
      binaryInputs[BUTTONS_2_TO_BINARY_INPUT_OFFSET + i].setValue(buttons_2.getButtonValueByIndex(i));
    }
  }

  if (buttons_1.getValueChangedEdge()) {
    //Serial.println("analog in buttons:");
    //Serial.println(buttons_1.getButtonsValueRaw());
    //Serial.println(buttons_1.getButtonsValue());
    for (uint8_t i=0; i< BUTTONS_1_COUNT; i++){
      binaryInputs[BUTTONS_1_TO_BINARY_INPUT_OFFSET + i].setValue(buttons_1.getButtonValueByIndex(i));
      
    }
  }

  if (mercurySwitches.getValueChangedEdge()) {
#ifdef DEBUG_MERCURY
      Serial.println("analog in mercury switches:");
      Serial.println(analogRead(A4));
      Serial.println(mercurySwitches.getButtonsValueRaw(), BIN);
      Serial.println(mercurySwitches.getButtonsValue());
#endif

    for (uint8_t i=0; i< MERCURY_SWITCHES_COUNT; i++){
#ifdef DEBUG_MERCURY
      Serial.println(MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET );
      Serial.println(i);
#endif
      binaryInputs[MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET + i].setValue(mercurySwitches.getButtonValueByIndex(i));
    }

  }
}

void mode_refresh(){
  
  //rotary 12 positions selector knob is taken as base for mode selecion. so there are 12 states. 

  //check if first iteration at new selector value.
  bool init = selectorDial.getValueChangedEdge();
#ifdef PROTOTYPE
  pretbak_apps.appSelector(init, selectorDial.getSelectorValue());
#else
  pretbak_apps.appSelector(init, selectorDial.getSelectorValue() - 1);  // -1 because 13 resistor values for 12 pos knob, gnd is never switchted.
#endif  
}

void setup() {
  
  #ifdef ENABLE_SERIAL
  Serial.begin(9600);
  #endif
  
  // put your setup code here, to run once
  
  selectorDial.initialize(PIN_SELECTOR_DIAL, SELECTOR_DIAL_POSITIONS); 
  
  buttons_1.setPin(PIN_BUTTONS_1,BUTTONS_1_COUNT, setValues_1);
  
  buttons_2.setPin(PIN_BUTTONS_2,BUTTONS_2_COUNT, setValues_2);
  
  mercurySwitches.setPin(PIN_MERCURY_SWITCHES, MERCURY_SWITCHES_COUNT, setValues_mercury);
  mercurySwitches.setDebounceMillis(30);

  buzzer.setPin(PIN_BUZZER);

  potentio.setPin(PIN_POTENTIO);
  
  ledDisp.startUp(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_4, PIN_DISPLAY_DIGIT_BUTTON_LIGHTS, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);
  //ledDisp.startUp(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_4, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);


  pretbak_apps.setPeripherals(binaryInputs, &potentio, &ledDisp, &buzzer);
  pretbak_apps.setBuffers(scrollBuf, textBuf);
  pretbak_apps.setDefaultMode();
}

void loop() {
  // put your main code here, to run repeatedly:
  refresh();
}


