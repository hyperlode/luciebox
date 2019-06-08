#include "Potentio.h"
#include "Arduino.h"

//empty constructor
Potentio::Potentio(){
}
void Potentio::setPin(byte pin){
	this->analogPin = pin;
}

uint8_t Potentio::getValue(){
	return (int16_t)analogRead(this->analogPin);
}

uint8_t Potentio::getValueStable(){
	// returns the STABLE value
	return this->potentio_value_stable ;
}

bool Potentio::getValueStableChangedEdge(){
	//will only trigger if a change in STABLE value is detected.
	return this->potentio_value_stable_changed;
}

void Potentio::refresh(){
	//call this to update the value of the analog input. call multiple times to check for stable value.
	
	potentio_value = this->getValue();
	

	if (potentio_value > potentio_value_stable + POTENTIO_SENSITIVITY || potentio_value < potentio_value_stable - POTENTIO_SENSITIVITY  ){
		this->potentio_value_stable_changed = true;  //simple edge detection
		this->potentio_value_stable = potentio_value;
		Serial.println(potentio_value_stable);
	}else{
		this->potentio_value_stable_changed = false;  //simple edge detection
	}
	
	
	// this->selectorPreviousStableValue = this->selectorCurrentStableValue;
	// if (getSelectorValueRaw() == this->selectorPreviousValue){
		// if (this->cyclesValueIsStable < CYCLES_BEFORE_CONSIDERED_STABLE){
			// // take care for overflows.
			// this->cyclesValueIsStable++;
		// }
	// }else{
		// this->cyclesValueIsStable = 0;
	// }
	// if (this->cyclesValueIsStable >=CYCLES_BEFORE_CONSIDERED_STABLE ){
		// this->selectorCurrentStableValue = getSelectorValueRaw();
	// }
	// this->selectorPreviousValue = getSelectorValueRaw();
}