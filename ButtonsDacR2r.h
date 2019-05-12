
#ifndef ButtonsDacR2r_h 
#define ButtonsDacR2r_h 
#include "Arduino.h" 

class ButtonsDacR2r{
  public:
  
    #define VALUE_MARGIN_FOR_SELECTOR 15 //8    //fixed value margin for adc TODO should be a relative margin (depending on button  i.e. 1 for 64 , 2 for 128, 4 for 256,...)
    #define CYCLES_BEFORE_CONSIDERED_STABLE 5 //software debouncing
    #define NUMBER_OF_KNOB_POSITIONS 8  //provide an equal resistance between each position
    #define ADC_POWERS_OF_TWO 10  //1024 = 2^10 (adc resolution in bits)
    ButtonsDacR2r();
    void setPin( byte pin, byte buttonsCount);
    uint8_t getButtonsValue();
    void refresh();
    bool getValueChangedEdge();
    bool getButtonValueByIndex(uint8_t index);
    uint8_t getButtonsValueRaw();
    int getButtonsValueAnalog();
  private:  
    byte buttonsCount;
    byte analogPin;
    long cyclesValueIsStable;
    
    uint8_t buttonsValue; //before debouncing
    uint8_t buttonsPreviousValue;
    
    uint8_t buttonsPreviousStableValue; //after debouncing
    uint8_t buttonsCurrentStableValue;
};
#endif
