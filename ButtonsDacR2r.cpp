#include "ButtonsDacR2r.h"
#include "Arduino.h"

//R-2R ladder --> DAC.  make sure the buttons have Normal Open and Normal Closed positions. Because, they should provide GND at OFF and VCC at ON.

//empty constructor
ButtonsDacR2r::ButtonsDacR2r(){
  this->debounceMillis = DEBOUNCE_MILLIS;
}

void ButtonsDacR2r::setDebounceMillis(uint8_t value){
  this->debounceMillis = (unsigned long)value;  
}

void ButtonsDacR2r::setPin(byte pin, byte buttonsCount, uint16_t* thresholds){
  this->analogPin = pin;
  this->buttonsCount = buttonsCount;
  this->thresholds = thresholds;
}

//int ButtonsDacR2r::getButtonsValueAnalog(){
//  return analogRead(this->analogPin);
//}

uint8_t ButtonsDacR2r::getButtonsValueRaw(){
  int raw = (analogRead(this->analogPin));
  uint8_t allButtonsState = 0b00000000; //every bit is a button.
  
  
  
  for(uint8_t i=0; i<this->buttonsCount; i++){
    //uint16_t checkValue = ( 0x0001 << ADC_POWERS_OF_TWO -1 - i); 
    uint16_t checkValue = this->thresholds[i];
    if (raw - (VALUE_MARGIN_FOR_SELECTOR/2) > checkValue ){   //for 10bit (max1024) adc, first check for higher than 512-margin, then 256-margin,...
      allButtonsState |= 0b00000001 << this->buttonsCount - 1 - i; 
      raw -= checkValue;  
      if (raw<0){
        raw=0;
      }
    }
    
    
    //uint16_t checkValue = ( 0x0001 << ADC_POWERS_OF_TWO -1 - i); 
    //if (raw > (checkValue - VALUE_MARGIN_FOR_SELECTOR )){   //for 10bit (max1024) adc, first check for higher than 512-margin, then 256-margin,...
    //  allButtonsState |= 0b00000001 << this->buttonsCount - 1 - i; 
    //  raw -= checkValue;  
    //  if (raw<0){
    //    raw=0;
    //  }
    //}
  }
  return allButtonsState;
}

bool ButtonsDacR2r::getButtonValueByIndex(uint8_t index){
  return (0b00000001 << index) & (this->getButtonsValue());
}

uint8_t ButtonsDacR2r::getButtonsValue(){
  //returns the STABLE value
  return this->buttonsCurrentStableValue ;
}

bool ButtonsDacR2r::getValueChangedEdge(){
  //will only trigger if a change in STABLE value is detected.
  return this->buttonsPreviousStableValue != this->buttonsCurrentStableValue ;
}

void ButtonsDacR2r::refresh(){
  //call this to update the value of the analog input. call multiple times to check for stable value.
  this->buttonsPreviousStableValue = this->buttonsCurrentStableValue;
  uint8_t buttonsValue = getButtonsValueRaw();
  
  // software debouncing: wait some time to make sure value is stable.
  // reset wait time when value changes.
  
  if ( buttonsValue != this->buttonsPreviousValue){
	  this->buttonEdgeMillis = millis();
  }
  
  if (millis() - this->buttonEdgeMillis > (unsigned long) this->debounceMillis){
    this->buttonsCurrentStableValue = buttonsValue ;
  }
  
  this->buttonsPreviousValue = buttonsValue;
}
