#ifndef RotaryEncoderDial_h
#define RotaryEncoderDial_h
#include "Arduino.h"

// ****** IMPLEMENTATION EXAMPLE ******
// in .ino file: 
//
// RotaryEncoderDial encoder_dial;

// // hack to get interrupts nicely going without too much magic.
// // https://forum.arduino.cc/index.php?topic=41713.0
// void isr0(){
//     encoder_dial.interruptChannelA();
// }
// void isr1(){
//     encoder_dial.interruptChannelB();
// }

// void setup()
// {
//     // trick to use the easy arduino libary instead of complicated native functions
//     attachInterrupt(0, isr0, CHANGE);
//     attachInterrupt(1, isr1, CHANGE);
// }
// void loop()
// {
// 	   encoder_dial.refresh();
// }
//**********************************

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
	void setUpperLimit(int16_t maxValue, boolean minToMax);
	void refresh();

	int16_t getValueMapped(int16_t minValue, int16_t maxValue);
    
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
	int16_t maxValue;
	boolean minToMax;

	int16_t value;
	int value_memory;   // 
	int16_t value_changed;

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
