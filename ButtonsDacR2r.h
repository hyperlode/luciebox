
#ifndef ButtonsDacR2r_h 
#define ButtonsDacR2r_h 
#include "Arduino.h" 

class ButtonsDacR2r{
  public:
  
    //out of use, margin depending on number of buttons used now.#define VALUE_MARGIN_FOR_SELECTOR 30 //8    //fixed value margin for adc TODO should be a relative margin (depending on button  i.e. 1 for 64 , 2 for 128, 4 for 256,...)
    #define ADC_POWERS_OF_TWO 10  //1024 = 2^10 (adc resolution in bits)
    #define DEBOUNCE_MILLIS 30  //30 is realistic
    ButtonsDacR2r();
    void setPin( byte pin, byte buttonsCount, uint16_t* thresholds);
    uint8_t getButtonsValue();
    void refresh();
    void setSoftwareDebouncing(uint8_t cycleBeforeConsideredStable);
    bool getValueChangedEdge();
    bool getButtonValueByIndex(uint8_t index);
    uint8_t getButtonsValueRaw();
    //int getButtonsValueAnalog();
    void setDebounceMillis(uint8_t value);
    
  private:  

    unsigned long debounceMillis;
    byte buttonsCount;
    byte analogPin;
    unsigned long buttonEdgeMillis;
    uint8_t buttonsValue; //before debouncing
    uint8_t buttonsPreviousValue;
    
    uint16_t* thresholds; 
    
    uint8_t buttonsPreviousStableValue; //after debouncing
    uint8_t buttonsCurrentStableValue;
};
#endif
