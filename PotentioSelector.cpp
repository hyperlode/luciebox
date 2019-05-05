#include "PotentioSelector.h"
#include "Arduino.h"

//empty constructor
PotentioSelector::PotentioSelector(){
}
void PotentioSelector::setPin(byte pin){
	this->analogPin = pin;
}

uint8_t PotentioSelector::getSelectorValueRaw(){
	
	return (uint8_t)(((analogRead(this->analogPin) + VALUE_MARGIN_FOR_SELECTOR) * (NUMBER_OF_KNOB_POSITIONS-1)) / 1024);
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
	}
	this->selectorPreviousValue = getSelectorValueRaw();
}