
#ifndef PotentioSelector_h 
#define PotentioSelector_h 
#include "Arduino.h" 

#include "Utilitieslode.h"


class PotentioSelector {
	public:
	
		#define VALUE_MARGIN_FOR_SELECTOR 50    //in order to avoid floats, we "add a value to the read value of the analog input. dision truncates the float, so instead of 1.002, it's better to have 1.502  to get the value of one.
		#define CYCLES_BEFORE_CONSIDERED_STABLE 3
		//#define NUMBER_OF_KNOB_POSITIONS 12  //provide an equal resistance between each position
		PotentioSelector();
		void initialize( byte pin, uint8_t selector_positions_count);
		uint8_t getSelectorValue();
		void refresh();
		bool getValueChangedEdge();
		
	private:	
		uint8_t getSelectorValueRaw();
		
		byte analogPin;
		uint8_t cyclesValueIsStable;
		uint8_t selectorPreviousValue;
		uint8_t selectorPreviousStableValue;
		uint8_t selectorCurrentStableValue;

                float analog_range_per_value;
};
#endif
