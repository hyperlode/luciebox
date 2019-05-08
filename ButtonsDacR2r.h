
#ifndef ButtonsDacR2r_h 
#define ButtonsDacR2r_h 
#include "Arduino.h" 

#include "Utilitieslode.h"


class ButtonsDacR2r{
  public:
  
    #define VALUE_MARGIN_FOR_SELECTOR 50    //in order to avoid floats, we "add a value to the read value of the analog input. dision truncates the float, so instead of 1.002, it's better to have 1.502  to get the value of one.
    #define CYCLES_BEFORE_CONSIDERED_STABLE 3
    #define NUMBER_OF_KNOB_POSITIONS 8  //provide an equal resistance between each position
    #define ADC_POWERS_OF_TWO 10  //1024 = 2^10
    ButtonsDacR2r();
    void setPin( byte pin, byte buttonsCount);
    uint8_t getButtonsValue();
    void refresh();
    bool getValueChangedEdge();
    
  private:  
   
    uint8_t getButtonsValueRaw();

    uint8_t buttonsValue;
    uint8_t buttonsEdgeUp;
    uint8_t buttonsEdgeDown;
    byte buttonsCount;
    byte analogPin;
    uint8_t cyclesValueIsStable;
    uint8_t buttonsPreviousValue;
    uint8_t buttonsPreviousStableValue;
    uint8_t buttonsCurrentStableValue;
};
#endif
