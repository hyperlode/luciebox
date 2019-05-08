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

uint8_t ButtonsDacR2r::getButtonsValueRaw(){
  
  int raw = (analogRead(this->analogPin));
//  long (analogRead(this->analogPin));
  uint8_t allButtonsState = 0b00000000; //every bit is a button.
  for(uint8_t i=0; i<this->buttonsCount; i++){
    if (raw > ( ( 0x01 << ADC_POWERS_OF_TWO -1 - i) - VALUE_MARGIN_FOR_SELECTOR)){
      allButtonsState |= 0b00000001 << this->buttonsCount - 1 - i; 
      raw -= ( 0x01 << ADC_POWERS_OF_TWO -1 - i);
    }
  }
  return allButtonsState;
}

uint8_t ButtonsDacR2r::getButtonsValue(){
//  // returns the STABLE value
  return this->buttonsCurrentStableValue ;

  //if (buttonsAnalog.getValueChangedEdge()) {
    
    //set buttons array depending on the buttons value. 
    //set all buttons
    //check value --> msb on? yes /no
    //divide by two. ON? yes /no
    //divide by two ...

    // check and update the edges 
//    for(int i=0;i<7;i++){
//      if (buttons[i] != buttonsPreviousState[i]){
//        buttonsEdgeDown[i]=false;          
//        buttonsEdgeUp[i]=false;          
//        if (buttons[i]){
//          buttonsEdgeUp[i]=true;          
//        }else{
//          buttonsEdgeUp[i]=true;          
//        }
//      }
//      buttonsPreviousState[i]=buttons[i];
//    }
//  }
//  
}

bool ButtonsDacR2r::getValueChangedEdge(){
  //will only trigger if a change in STABLE value is detected.
  return this->buttonsPreviousStableValue != this->buttonsCurrentStableValue ;
}

void ButtonsDacR2r::refresh(){
  //call this to update the value of the analog input. call multiple times to check for stable value.
  this->buttonsPreviousStableValue = this->buttonsCurrentStableValue;
  
  if (getButtonsValueRaw() == this->buttonsPreviousValue){
    if (this->cyclesValueIsStable < CYCLES_BEFORE_CONSIDERED_STABLE){
      // take care for overflows.
      this->cyclesValueIsStable++;
    }
  }else{
    this->cyclesValueIsStable = 0;
}
  if (this->cyclesValueIsStable >=CYCLES_BEFORE_CONSIDERED_STABLE ){
    this->buttonsCurrentStableValue = getButtonsValueRaw();
    

    
  }
  this->buttonsPreviousValue = getButtonsValueRaw();
}
