#include "PotentioSelector.h"
#include "Arduino.h"


//for selector switch where 

//empty constructor
PotentioSelector::PotentioSelector(){
          this->debounceMillis = DEBOUNCE_MILLIS;
}

void PotentioSelector::initialize(byte pin, uint8_t selector_positions_count){
	this->analogPin = pin;
        this->analog_range_per_value =  (float)1024 / selector_positions_count;
}

uint8_t PotentioSelector::getSelectorValueRaw(){
	
	//return (uint8_t)(((analogRead(this->analogPin) + VALUE_MARGIN_FOR_SELECTOR) * (NUMBER_OF_KNOB_POSITIONS-1)) / 1024);
//        return (uint8_t)((( analogRead(this->analogPin) - VALUE_MARGIN_FOR_SELECTOR/2 ) / (1024 / NUMBER_OF_KNOB_POSITIONS+1))-1);
        return (uint8_t)(   (float)(analogRead(this->analogPin) + this->analog_range_per_value/2 ) /  this->analog_range_per_value); //this->analog_range_per_value/2 = get value in middle of the range.
        
}

uint8_t PotentioSelector::getSelectorValue(){
	// returns the STABLE value
	return this->selectorCurrentStableValue ;
}

bool PotentioSelector::getValueChangedEdge(){
	//will only trigger if a change in STABLE value is detected.
	return this->selectorPreviousStableValue != this->selectorCurrentStableValue ;
}

void PotentioSelector::refresh(){
	//call this to update the value of the analog input. call multiple times to check for stable value.
	
        this->selectorPreviousStableValue = this->selectorCurrentStableValue;
        
        if ( getSelectorValueRaw() != this->selectorPreviousValue){
      	  this->buttonEdgeMillis = millis();
          this->debounceMillis = DEBOUNCE_MILLIS;
        }
                
        if (millis() - this->buttonEdgeMillis > this->debounceMillis){

          this->selectorCurrentStableValue = getSelectorValueRaw();
          this->debounceMillis = 1000000; // set high, so does not trigger all the time once stable. (for debugging)
          //Serial.println("delete me raw value stable: ");
          //Serial.println(analogRead(this->analogPin));
        }
        
        this->selectorPreviousValue = getSelectorValueRaw();
        
/*
	this->selectorPreviousStableValue = this->selectorCurrentStableValue;
	if (getSelectorValueRaw() == this->selectorPreviousValue){
		if (this->cyclesValueIsStable < CYCLES_BEFORE_CONSIDERED_STABLE){
			// take care for overflows.
			this->cyclesValueIsStable++;
		}
	}else{
		this->cyclesValueIsStable = 0;
	}
	if (this->cyclesValueIsStable >=CYCLES_BEFORE_CONSIDERED_STABLE ){
		this->selectorCurrentStableValue = getSelectorValueRaw();
                Serial.println(analogRead(this->analogPin));
	}
	this->selectorPreviousValue = getSelectorValueRaw();
*/
}
