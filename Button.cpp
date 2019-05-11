#include "Button.h"
#include "Arduino.h"

//empty constructor
Button::Button(){
	//check if multiple buttons per Pin!!!
	// this->twoButtonsOnePinButtonConnectedToGnd = true;
	//setBit(&this->boolContainer, true, BITLOCATION_TWOBUTTONSONEPINBUTTONCONNECTEDTOGND); //default button connected to gnd for on.
	setTwoButtonsOnePinButtonConnectedToGnd(true); //default button connected to gnd for on.
	//setButtonPartOfTwoButtonsPerPinSystem(true);//default button is part of a two button per pin system.
	
};


//void twoButtonsHandler(){
// http://www.arduino.cc/en/Tutorial/TwoSwitchesOnePin

/*
 * Read_Two_Switches_On_One_Pin
 * Read two pushbutton switches or one center-off toggle switch with one Arduino pin
 * Paul Badger 2008 
 * From an idea in EDN (Electronic Design News)
 *
 * Exploits the pullup resistors available on each I/O and analog pin
 * The idea is that the 200K resistor to ground will cause the input pin to report LOW when the 
 * (20K) pullup resistor is turned off, but when the pullup resistor is turned on, 
 * it will overwhelm the 200K resistor and the pin will report HIGH.
 *
 * Schematic Diagram    ( can't belive I drew this funky ascii schematic )     
 *
 *
 *                             +5 V
 *                                |
 *                                \
 *                                /   
 *                                \    10K
 *                                /
 *                                \
 *                                |
 *                               /    switch 1  or 1/2 of center-off toggle or slide switch
 *                              /       
 *                                |
 *            digital pin ________+_____________/\/\/\____________   ground              
 *                                |               
 *                                |               200K to 1M  (not critical)
 *                               /   
 *                              /        switch 2 or 1/2 of center-off toggle or slide switch
 *                                |
 *                                |
 *                              _____   
 *                               ___     ground
 *                                _
 *
 */


// #define swPin 2                 // pin for input  - note: no semicolon after #define
// int stateA, stateB;             // variables to store pin states
// int sw1, sw2;                   // variables to represent switch states 

// void setup()                   
// {
   // Serial.begin(9600);
// }

// void loop()            
// {
   // digitalWrite(swPin, LOW);                   // make sure the puillup resistors are off
   // stateA = digitalRead(swPin);
   // digitalWrite(swPin, HIGH);                  // turn on the puillup resistors
   // stateB = digitalRead(swPin);

   // if ( stateA == 1 && stateB == 1 ){          // both states HIGH - switch 1 must be pushed
      // sw1 = 1;
      // sw2 = 0;
   // }
   // else if ( stateA == 0 && stateB == 0 ){     // both states LOW - switch 2 must be pushed
      // sw1 = 0;
      // sw2 = 1;
   // }
   // else{                                       // stateA HIGH and stateB LOW 
      // sw1 = 0;                                 // no switches pushed - or center-off toggle in middle position
      // sw2 = 0;
   // }  

   // Serial.print(sw1);
   // Serial.print("    ");     // pad some spaces to format print output
   // Serial.println(sw2);

   // delay(100);  
//}

//};

void Button::setPin( byte pin1){
	this->pin1 = pin1;
	//Serial.println(pin1);
	pinMode(pin1, INPUT);
	digitalWrite(pin1, HIGH);  // set pullup on pin
	
	
};

void Button::setTwoButtonsOnePinButtonConnectedToGnd(bool connectedToGnd){
	//we are talking about pin1 here! this has nothing to do with pin2 (pin2 comes from an external source...not involved in the multiplexing)
	// this->twoButtonsOnePinButtonConnectedToGnd = connectedToGnd;
	setBit(&this->boolContainer, connectedToGnd, BITLOCATION_TWOBUTTONSONEPINBUTTONCONNECTEDTOGND);
}


void Button::twoButtonsOnePinHandler(){
	
		digitalWrite(this->pin1, HIGH);                  // turn on the puillup resistors
		delay(2);
		bool stateB = digitalRead(this->pin1);
		delay(20);
    if (stateB){
      //Serial.println("BHIGH");
    }
    
    
		digitalWrite(this->pin1, LOW);                   // make sure the puillup resistors are off
		delay(1);
   
		bool stateA = digitalRead(this->pin1); 
		stateA = digitalRead(this->pin1);  //set here two times, we could also use a small delay.... if not, the reading is not good, I think it needs time to come down after the pullup has been switched off!

    if (stateA){
      //Serial.println("AHIGH");
    }
    
    
		///////pinMode(5, INPUT_PULLUP); // Enable internal pull-up resistor on pin 5
		
		//port manipulation    http://www.arduino.cc/en/Reference/PortManipulation
		
		// CLR(PORTB, 0) ;     SET(PORTB, 0);
		// http://www.billporter.info/2010/08/18/ready-set-oscillate-the-fastest-way-to-change-arduino-pins/
		
		// if ( stateA == 1 && stateB == 1 ){          // both states HIGH - switch1 connected to V+ must be pushed
			// if (this ->twoButtonsOnePinButtonConnectedToGnd ){
				// this->state = false;
			// }else{
				// this->state = true;
			// }
			// // sw1 = 1;
			// // sw2 = 0;
		// }else if ( stateA == 0 && stateB == 0 ){     // both states LOW - switch2 connected to GND must be pushed
			// if (this ->twoButtonsOnePinButtonConnectedToGnd ){
				// this->state = true;
			// }else{
				// this->state = false;
			// }
		
			// // sw1 = 0;
			// // sw2 = 1;
		// }else{                                       // stateA HIGH and stateB LOW 
			// this->state = false;
			// // sw1 = 0;                                 // no switches pushed - or center-off toggle in middle position
			// // sw2 = 0;
		// }  
		
		//short notation for above: (to test if above works)
		// this->state = (stateA == 1 && stateB == 1 && !this ->twoButtonsOnePinButtonConnectedToGnd) || (stateA == 0 && stateB == 0 && this ->twoButtonsOnePinButtonConnectedToGnd);
		setBit(&this->boolContainer, (stateA == 1 && stateB == 1 && !getBit(&this->boolContainer, BITLOCATION_TWOBUTTONSONEPINBUTTONCONNECTEDTOGND)) || (stateA == 0 && stateB == 0 && getBit(&this->boolContainer, BITLOCATION_TWOBUTTONSONEPINBUTTONCONNECTEDTOGND)), BITLOCATION_STATE) ;
		//Serial.println(
		//  (stateA == 1 && stateB == 1 && !getBit(&this->boolContainer, BITLOCATION_TWOBUTTONSONEPINBUTTONCONNECTEDTOGND))
		//  || (stateA == 0 && stateB == 0 && getBit(&this->boolContainer, BITLOCATION_TWOBUTTONSONEPINBUTTONCONNECTEDTOGND))
		//  );
		

}

bool Button::getStateEdgeUp(){
	//edge detection
	if (!getPreviousState() && getState()){
		return true;
		//this->timeLastPress = millis();
	}else{
		return false;
	}
}

bool Button::getStateEdgeDown(){
	if (getPreviousState() && !getState()){
		return true;
	}else{
		return false;
	}
}

bool Button::getState(){
	return getBit(&this->boolContainer, BITLOCATION_STATE);
}

bool Button::getPreviousState(){
	return getBit(&this->boolContainer, BITLOCATION_PREVIOUSSTATE);
}

void Button::refresh(){
	//store previous state
	// this->previousState = this->state;
	setBit(&this->boolContainer, getState(), BITLOCATION_PREVIOUSSTATE);
	
	// //edge detection
	// if (previousState == 0 and this->state == 1){
		// this->stateEdgeUp = 1 ;
		// //this->timeLastPress = millis();
	// }

	// if (this->previousState == 1 and this->state == 0){
		// this->stateEdgeDown = 1;

	// }
	// this->previousState = this->state;
	//update time elapsed since last edge up
	//this->timeSinceLastPressMillis = millis() - this->timeLastPress;
};

































































































