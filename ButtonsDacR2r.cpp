#include "ButtonsDacR2r.h"
#include "Arduino.h"

//R-2R ladder --> DAC.  make sure the buttons have Normal Open and Normal Closed positions. Because, they should provide GND at OFF and VCC at ON.

//empty constructor
ButtonsDacR2r::ButtonsDacR2r(){
}
void ButtonsDacR2r::setPin(byte pin, byte buttonsCount){
  this->analogPin = pin;
  this->buttonsCount = buttonsCount;
}

//int ButtonsDacR2r::getButtonsValueAnalog(){
//  return analogRead(this->analogPin);
//}

uint8_t ButtonsDacR2r::getButtonsValueRaw(){
  int raw = (analogRead(this->analogPin));
  uint8_t allButtonsState = 0b00000000; //every bit is a button.
  for(uint8_t i=0; i<this->buttonsCount; i++){
    
    if (raw > ( ( 0x0001 << ADC_POWERS_OF_TWO -1 - i) - VALUE_MARGIN_FOR_SELECTOR )){   //for 10bit (max1024) adc, first check for higher than 512-margin, then 256-margin,...
      allButtonsState |= 0b00000001 << this->buttonsCount - 1 - i; 
      raw -= ( 0x0001 << ADC_POWERS_OF_TWO -1 - i);
      if (raw<0){
        raw=0;
      }
    }
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
  
  //software debouncing: wait a number of cycles to make sure value is stable.
  if (buttonsValue == this->buttonsPreviousValue){
    // take care for overflows.
    this->cyclesValueIsStable++;
  }else{
    this->cyclesValueIsStable = 0;
  }
  
  if (this->cyclesValueIsStable >=CYCLES_BEFORE_CONSIDERED_STABLE ){
    this->buttonsCurrentStableValue = buttonsValue ;
    this->cyclesValueIsStable = 0;
  }
  
  this->buttonsPreviousValue = buttonsValue;
}
