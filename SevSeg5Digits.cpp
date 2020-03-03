/*
Written by Dean Reading, 2012
 Version 2.1; Includes a bug fix as pointed out by a community member, Ryan.
 Version 2.0; Now works for any digital pin arrangement, common anode and common cathode displays.
 
 Direct any questions or suggestions to deanreading@hotmail.com
 
 This library allows an arduino to easily display numbers in decimal format on
 a 4 digit 7-segment display without a separate 7-segment display controller.
 
 Hardware:
 4 digit 7 segment displays use 12 digital pins.
 
 There are:
 4 common pins; 1 for each digit.  These will be cathodes (negative pins) for
 common cathode displays, or anodes (positive pins) for common anode displays.
 I refer to these as digit pins.
 8 pins for the individual segments (seven segments plus the decimal point).
 I refer to these as segment pins.
 
 Connect the four digit pins with four limiting resistors in series to any digital or analog pins.
 Connect the eight cathodes to any digital or analog pins.
 
 I have a cheap one from China, and the pins of the display are in the following order:
 Top Row
 1,a,f,2,3,b
 Bottom Row
 e,d,dp,c,g,4
 Where the digit pins are 1-4 and the segment pins are a-g + dp
 
 Software:
 Call SevSeg5Digits.Begin in setup.  
 The first argument (boolean) tells whether the display is common cathode (0) or common
 anode (1).
 The next four arguments (bytes) tell the library which arduino pins are connected to
 the digit pins of the seven segment display.  Put them in order from left to right.
 The next eight arguments (bytes) tell the library which arduino pins are connected to
 the segment pins of the seven segment display.  Put them in order a to g then the dp.
 
 In summary, Begin(type, digit pins 1-4, segment pins a-g,dp)
 
 The calling program must run the PrintOutput() function repeatedly to get
 the number displayed.
 To set the number displayed, use the NewNum function.  Any number between
 -999 and 9999 can be displayed. Out of range numbers show up as '----'.
 To move the decimal place one digit to the left, use '1' as the second
 argument in NewNum. For example, if you wanted to display '3.141' you would
 call NewNum(3141,3);
 
 */

#include "SevSeg5Digits.h"


SevSeg5Digits::SevSeg5Digits()
{
	//Initial values
	this->segActive = 0;
	this->extraLedArray=false;
}

bool SevSeg5Digits::getMode(){
	return getBit(&this->boolContainer, MODEISCOMMONANODE);
}
//Begin
/*******************************************************************************************/
//Set pin modes and turns all displays off

void SevSeg5Digits::Begin(boolean mode_isCommonAnode, byte D0, byte D1, byte D2, byte D3, byte D4, byte LedArrayDigit, byte S1, byte S2, byte S3, byte S4, byte S5, byte S6, byte S7, byte S8){
	//for initialization with extra led array
	DigitPins[5]=LedArrayDigit;
	Begin(mode_isCommonAnode, D0, D1, D2, D3, D4, S1, S2, S3, S4, S5, S6, S7, S8);
	this->extraLedArray=true;

}
void SevSeg5Digits::Begin(boolean mode_isCommonAnode, byte D0, byte D1, byte D2, byte D3, byte D4, byte S1, byte S2, byte S3, byte S4, byte S5, byte S6, byte S7, byte S8){
	//for pure 5 7seg digits.

	//Assign input values to variables
	//Mode sets what the digit pins must be set at for it to be turned on.	0 for common cathode, 1 for common anode
	//this->mode_isCommonAnode=mode_isCommonAnode;
	setBit(&this->boolContainer, mode_isCommonAnode, MODEISCOMMONANODE);
	
	// if (mode_in==1){
	// DigitOn=true;
	// DigitOff=false;
	// SegOn=false;
	// SegOff=true;
	// }else{
	// DigitOn=false;
	// DigitOff=true;
	// SegOn=true;
	// SegOff=false;
	// }

	DigitPins[0]=D0;
	DigitPins[1]=D1;
	DigitPins[2]=D2;
	DigitPins[3]=D3;
	DigitPins[4]=D4;
	SegmentPins[0]=S1;
	SegmentPins[1]=S2;
	SegmentPins[2]=S3;
	SegmentPins[3]=S4;
	SegmentPins[4]=S5;
	SegmentPins[5]=S6;
	SegmentPins[6]=S7;
	SegmentPins[7]=S8;

	//Set Pin Modes as outputs
	for (byte digit=0;digit<6;digit++) {
		pinMode(DigitPins[digit], OUTPUT);
	}

	for (byte seg=0;seg<8;seg++) {
		pinMode(SegmentPins[seg], OUTPUT);
	}

	//Turn Everything Off
	//Set all digit pins off.	Low for common anode, high for common cathode
	for (byte digit=0;digit<6;digit++) {
		digitalWrite(DigitPins[digit], DIGITOFF);
	}
	
	//Set all segment pins off.	High for common anode, low for common cathode
	for (byte seg=0;seg<8;seg++) {
		digitalWrite(SegmentPins[seg], SEGMENTOFF);
	}
}

//Refresh Display	for 1 + 4 digit display with max brightness.
/*******************************************************************************************/
//Cycles through each segment and turns on the correct digits for each.
//Leaves everything off
byte* SevSeg5Digits::PrintOutputSeg(byte brightness){
	//brightness: 0 = max, the higher, the more delay.
	//brightness = 49;
	//turn active segmentOff , as active segment can be more than there are available segments, check the number!
	if (segActive<8){ //if we don't do this, after a certain amount of time, about more than 50, the device errors.
		//turn segment line off
		digitalWrite(SegmentPins[segActive],SEGMENTOFF);
		pinMode(SegmentPins[segActive], INPUT); 

		//turn digits off
		for (byte digit=0;digit<6;digit++){
			digitalWrite(DigitPins[digit],DIGITOFF);
			pinMode(DigitPins[digit], INPUT); 
		}
	}

	segActive++;
	if (segActive>7){
		//this is the brightness control, going into virtual segments, and doing nothing...
		if ( segActive > 7 + brightness	){
			segActive=0;
		}
	}

	if (segActive<8){
		//Turn the relevant segment on
    	pinMode(SegmentPins[segActive], OUTPUT);
		digitalWrite(SegmentPins[segActive],SEGMENTON);

		//For each digit, turn relevant digits on
		for (byte digit=0;digit<6;digit++){
			if (this->extraLedArray && digit == 5){
				
				//led array digit
				// if (this->ledArrayValues & ( 1 << segActive ) != 0){ //check if segment bit is not zero.
				if (this->ledArrayValues & ( 1 << segActive ) ){ //check if segment bit is not zero.
					pinMode(DigitPins[digit], OUTPUT);
					digitalWrite(DigitPins[digit],DIGITON);
				}
			}else{
				if ( getBit(&lights[digit], segActive)) {
         			pinMode(DigitPins[digit], OUTPUT);
					digitalWrite(DigitPins[digit],DIGITON);
				}
			}
		}
	}
	return &segActive; //return the address of the integer that contains the active Segment.
}

//New Number
/*******************************************************************************************/

void SevSeg5Digits::SetSingleDigit(int8_t value, int digit){
  //value =  bits. Segment bit0=segmentA, bit6=segmentG, bit7 = decimal point. 
  this->lights[digit] = value;
}

void SevSeg5Digits::SetFourDigits(uint32_t value){
  for (uint8_t i = 0;i<4;i++){
    this->SetSingleDigit(value>>(8*i),i+1);
  }
}

void SevSeg5Digits::SetDecPointSingle(boolean pointIsOn, int digit){
	setBit(&this->lights[digit],pointIsOn,7);
	//CreateArray();
	
}
void SevSeg5Digits::SetLedArray(byte ledsAsBits){
	this->ledArrayValues = ledsAsBits;
}
void SevSeg5Digits::NewText(char* text)
{
	this-> text = text;
	CreateArray();
#ifdef ARDUINO_SIMULATION
	mtc->setText(text);
	mtc->setLights(this->lights);
#endif
}

//Create Array
/*******************************************************************************************/
//From the numbers found, says which LEDs need to be turned on
void SevSeg5Digits::CreateArray() {

	for (byte digit=0;digit<5;digit++) {
  	 
		//temporary save the decimal point of the segment. 
		setBit(&this->boolContainer, getBit(&this->lights[digit],7), TEMPDECPOINTMEMORY);
		
		if (text[digit] == ' '){
			text[digit] = SPACE_FAKE_ASCII;
		}

		if (text[digit] == '?'){
			byte i = random(0,7);
			setBit(&this->lights[digit],  !getBit(&this->lights[digit],i), i); //random on or off...
			
		}else if (text[digit] < 91){
			lights[digit] = pgm_read_byte_near(selected_ascii_to_7seg_digit  + text[digit] - 48);
		}else{

			lights[digit] = B01011100;
		}
		
	//Set the decimal place lights
	setBit(&this->lights[digit], getBit(&this->boolContainer,TEMPDECPOINTMEMORY), 7);
	}
}




















































































