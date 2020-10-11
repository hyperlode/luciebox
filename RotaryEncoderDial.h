#ifndef RotaryEncoderDial_h
#define RotaryEncoderDial_h
#include "Arduino.h"

class RotaryEncoderDial
{
public:

#define POTENTIO_SENSITIVITY 5
	RotaryEncoderDial();

	void setPins(byte pinChannelA, byte pinChannelB);
	
	bool getValueChanged();
    int16_t getValue();
	void setValue(int16_t value);

    void setIncrement(uint8_t increment);
	void setSensitivity(uint8_t sensitivity);
	void refresh();
    
    void interruptChannelA();
    void interruptChannelB();
    void initialize();
    void checkState();
    void setNewState();

private:
	byte pinChannelA;
	byte pinChannelB;

	uint8_t increment;
	uint8_t sensitivity; //rotary step per count.
	uint8_t sensitivity_counter ;

	int16_t value;
	int value_memory;   // 
	boolean value_changed;

	volatile int encoderPos;  //raw dial counter

	// interrupt service routine vars
	volatile boolean A_set = false;
	volatile boolean B_set = false;
	volatile boolean A_waitfor = false;
	volatile boolean B_waitfor = false;
	volatile boolean A_changedir_waitfor = false;
	volatile boolean B_changedir_waitfor = false;
	volatile boolean ccwElseCw = false;  // rotation direction: counter-clockwise if true. clockwise if false.
};
#endif
