
#ifndef Potentio_h 
#define Potentio_h 
#include "Arduino.h" 

#include "Utilitieslode.h"


class Potentio {
	public:
	
		// #define VALUE_MARGIN_FOR_SELECTOR 50    //in order to avoid floats, we "add a value to the read value of the analog input. dision truncates the float, so instead of 1.002, it's better to have 1.502  to get the value of one.
		// #define CYCLES_BEFORE_CONSIDERED_STABLE 3
		// #define NUMBER_OF_KNOB_POSITIONS 12  //provide an equal resistance between each position
		#define POTENTIO_SENSITIVITY 5
		Potentio();

    long getValueMapped(long minimumValue, long maximumValue);
    
		void setPin( byte pin);
		uint16_t getValue();
		uint16_t getValueStable();
		void refresh();
			
		bool getValueStableChangedEdge();
		
	private:	
		byte analogPin;
		
		int16_t potentio_value;
		int16_t potentio_value_stable;
		bool potentio_value_stable_changed;
};
#endif
