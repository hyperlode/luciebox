#include "PotentioSelector.h"
#include "ButtonsDacR2r.h"
#include "BinaryInput.h"
#include "Buzzer.h"
#include "Apps.h"
#include "Potentio.h"

#include "SevSeg5Digits.h" //sevseb -->this should not be included here, but only in DisplayDigitsHandler.h, as it only gets used there (but ARDUINO needs this here!) DEBUG
#include "DisplayDigitsHandler5Digits.h"

#include "PretbakSettings.h"


// Lode Ameije 2019-05
// Pretbak is a busy box for my newleborn niece
// The hardware is a box with
// input: buttons, momentary and switches, a potentiometer, a selector dial, some mercury switches.
// output: lights on buttons, 7seg display (4 digits), buzzer



//pretbak
Apps pretbak_apps;

// INPUT
BinaryInput binaryInputs[BINARY_INPUTS_COUNT];
PotentioSelector selectorDial;
ButtonsDacR2r buttons_1;
ButtonsDacR2r buttons_2; // buttons with normally closed. this is a problem for the R-2R ladder. instead, I used a pull down resistor to ground at the switch. so: ON = 5V, OFF = GND over 1Kohm. 10K, 20K R2Rladder.  will only work for limited number of buttons.
Potentio potentio;

// OUTPUT
DisplayManagement ledDisp;

Buzzer buzzer;

//output
char  textBuf [6];
char  scrollBuf [40];
uint8_t lights;

void refresh(){
  //input process  
  input_process();

  for(uint8_t i=0;i<BINARY_INPUTS_COUNT;i++){
    if (binaryInputs[i].getEdgeUp()){
      Serial.println(i);
      Serial.println("PRESSED");
    }
    if (binaryInputs[i].getEdgeDown()){
      Serial.println(i);
      Serial.println("Button released");
    }
  }
  //Serial.println("papa help "); // display this PAPA - HELP - PAPA - ... on screen when errors.

  //modes functionality
  
  //mode change
  if (selectorDial.getValueChangedEdge()) {
    //default mode (go to default state at each change)
    setDefaultMode();
    Serial.println(selectorDial.getSelectorValue());
  }

  mode_refresh();

  //output process
  ledDisp.refresh();
  buzzer.doBuzzerRoll();
    
  for(uint8_t i=0;i<BINARY_INPUTS_COUNT;i++){
    binaryInputs[i].refresh();
  }
}

void input_process(){
  selectorDial.refresh();
  buttons_1.refresh();
  buttons_2.refresh();
  potentio.refresh();

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
    }
  }
}

void setDefaultMode(){
  //button lights
  lights = 0b00000000; //no lights
  ledDisp.SetLedArray(lights);

  //display
  ledDisp.SetFourDigits(0xC0C0C0C0); //default dispaly 4x minus and decimal point.
  ledDisp.setBrightness(0,false);

  //buzzer
  buzzer.buzzerOff();
}


void mode_refresh(){
  
  //rotary 12 positions selector knob is taken as base for mode selecion. so there are 12 states. 

  //check if first iteration at new selector value.
  bool init = selectorDial.getValueChangedEdge();
//  if (init){
//    pretbak_apps.test();
//  }
  
  
  switch (selectorDial.getSelectorValue()) {
    case 0:
      pretbak_apps.modeCountingLettersAndChars(init);
      break;
      
    case 1:
      //sound fun with notes
      pretbak_apps.modeSoundNotes();
      break;
      
    case 2:
      //sound fun with frequencies.
      //long freqq = 0;
      //freqq = map((long)potentio_value, 0, 1023, 0 , 65535);
      //buzzer.buzzerOn((uint16_t) freqq);
      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
        if (potentio.getValueStableChangedEdge()){      
          buzzer.buzzerOn( (uint16_t) map((long)potentio.getValueStable(), 0, 1023, 0 , 65535));
        }
      }else{
        if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue() && binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
          if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
            buzzer.buzzerOn( (uint16_t) map((long)potentio.getValueStable(), 0, 1023, 0 , 65535));
          }
        }else if (true) {
          
        }
        
      }
      break;
      
    case 3:
      pretbak_apps.modeScroll(init);
      break;
      
    case 4:
      pretbak_apps.modeGeiger(init);
      break;
      
    case 5:
      pretbak_apps.modeSoundSong(init);
      break;
    case 6:
      pretbak_apps.modeSingleSegmentManipulation(init);
      break;
    case 7:
      pretbak_apps.modeSimpleButtonsAndLights();    
      break;
    case 8:
      pretbak_apps.fullScreenMovie(init);
      break;
    case 9:
      pretbak_apps.gameButtonInteraction(init);
      
      break;
    case 10:
      break;
    case 11:
      break;
    
    default:
      break;
  }
  
}

void setup() {
  // put your setup code here, to run once:
  selectorDial.setPin(PIN_SELECTOR_DIAL);
  buttons_1.setPin(PIN_BUTTONS_1,BUTTONS_1_COUNT);
  buttons_2.setPin(PIN_BUTTONS_2,BUTTONS_2_COUNT);

  buzzer.setPin(PIN_BUZZER);

  potentio.setPin(PIN_POTENTIO);
  
  ledDisp.startUp(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_4, PIN_DISPLAY_DIGIT_BUTTON_LIGHTS, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);
  //ledDisp.startUp(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_4, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);


  setDefaultMode();
  pretbak_apps.setPeripherals(binaryInputs, &potentio, &ledDisp, &buzzer);
  pretbak_apps.setBuffers(scrollBuf, textBuf);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  refresh();
}


