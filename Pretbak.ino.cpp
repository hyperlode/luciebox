# 1 "C:\\Users\\LODE~1.AME\\AppData\\Local\\Temp\\tmp7op9otmh"
#include <Arduino.h>
# 1 "C:/Users/lode.ameije/Documents/PlatformIO/Projects/200215-122529-uno/src/Pretbak.ino"

#include "PotentioSelector.h"
#include "ButtonsDacR2r.h"
#include "BinaryInput.h"
#include "Buzzer.h"
#include "Apps.h"
#include "Potentio.h"
#include "DisplayDigitsHandler5Digits.h"
#include "PretbakSettings.h"




#ifdef ENABLE_SERIAL
# 25 "C:/Users/lode.ameije/Documents/PlatformIO/Projects/200215-122529-uno/src/Pretbak.ino"
#endif







uint16_t setValues_1 [BUTTONS_1_COUNT] = BUTTONS_1_VALUES;
uint16_t setValues_2 [BUTTONS_2_COUNT] = BUTTONS_2_VALUES;
uint16_t setValues_mercury [MERCURY_SWITCHES_COUNT] = MERCURY_SWITCHES_VALUES;


Apps pretbak_apps;


BinaryInput binaryInputs[BINARY_INPUTS_COUNT];
PotentioSelector selectorDial;
ButtonsDacR2r buttons_1;
ButtonsDacR2r buttons_2;
ButtonsDacR2r mercurySwitches;
Potentio potentio;


DisplayManagement ledDisp;
Buzzer buzzer;





#ifdef ENABLE_ANALOG_PIN_DEBUG

bool debugMode;
#endif
void refresh();
void input_process();
void setup();
void loop();
#line 61 "C:/Users/lode.ameije/Documents/PlatformIO/Projects/200215-122529-uno/src/Pretbak.ino"
void refresh(){


  input_process();

  #ifdef DEBUG_BUTTONS
    for(uint8_t i=0;i<BINARY_INPUTS_COUNT;i++){

      if (binaryInputs[i].getEdgeUp()){

        Serial.println("{{{-------PRESS:-------:");
        Serial.println(i);
        Serial.println( (analogRead(PIN_BUTTONS_1)));
        Serial.println( (analogRead(PIN_BUTTONS_2)));



      }

      if (binaryInputs[i].getEdgeDown()){
        Serial.println("-----RELEASE:");
        Serial.println(i);



      }
    }

  #endif




  #ifdef ENABLE_ANALOG_PIN_DEBUG
  if (!debugMode){
  #endif



    if (selectorDial.getValueChangedEdge()) {

      pretbak_apps.setDefaultMode();

      #ifdef DEBUG_SELECTOR_KNOB

      Serial.println("selector:");
      Serial.println(selectorDial.getSelectorValue());
      Serial.println(analogRead(PIN_SELECTOR_DIAL));
      #endif

    }




    bool init = selectorDial.getValueChangedEdge();
    #ifdef PROTOTYPE
      pretbak_apps.appSelector(init, selectorDial.getSelectorValue());
    #else
      pretbak_apps.appSelector(init, selectorDial.getSelectorValue() - 1);
    #endif

    buzzer.doBuzzerRoll();

  #ifdef ENABLE_ANALOG_PIN_DEBUG
  }else{
    pretbak_apps.appSelector(false, 12);
  }
  #endif


  ledDisp.refresh();

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

void setup() {

  #ifdef ENABLE_SERIAL
  Serial.begin(9600);
  #endif



  selectorDial.initialize(PIN_SELECTOR_DIAL, SELECTOR_DIAL_POSITIONS);

  buttons_1.setPin(PIN_BUTTONS_1,BUTTONS_1_COUNT, setValues_1);

  buttons_2.setPin(PIN_BUTTONS_2,BUTTONS_2_COUNT, setValues_2);

  mercurySwitches.setPin(PIN_MERCURY_SWITCHES, MERCURY_SWITCHES_COUNT, setValues_mercury);
  mercurySwitches.setDebounceMillis(30);

  potentio.setPin(PIN_POTENTIO);

  ledDisp.startUp(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_4, PIN_DISPLAY_DIGIT_BUTTON_LIGHTS, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);
# 206 "C:/Users/lode.ameije/Documents/PlatformIO/Projects/200215-122529-uno/src/Pretbak.ino"
  #ifdef ENABLE_EEPROM
  if (eeprom_read_byte((uint8_t*)EEPROM_SOUND_OFF_BY_DEFAULT)){
    buzzer.setPin(PIN_BUZZER_FAKE);
  }else{
    buzzer.setPin(PIN_BUZZER);
  }
  #else
  buzzer.setPin(PIN_BUZZER);
  #endif


  pretbak_apps.setPeripherals(binaryInputs, &potentio, &ledDisp, &buzzer);

  pretbak_apps.setDefaultMode();

  #ifdef ENABLE_ANALOG_PIN_DEBUG

  debugMode = false;
  if (analogRead(PIN_BUTTONS_1) > 10 && analogRead(PIN_BUTTONS_1) < 200){
    debugMode = true;
    pretbak_apps.appSelector(true, 12);
  }
  #endif

}

void loop() {

  refresh();
}