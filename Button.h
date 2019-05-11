//Written by Lode Ameije 2013 
#ifndef Button_h 
#define Button_h 
#include "Arduino.h" 

#include "Utilitieslode.h"
#define NO_EXTRA_INPUTPIN_SET 33
#define PIN_FAKE 33

class Button {
	public:
		byte pin1;
		
		
		#define BITLOCATION_STATE 0
		#define BITLOCATION_PREVIOUSSTATE 1
		#define BITLOCATION_BUTTONISPARTOFTWOBUTTONSPERPINSYSTEM 2
		#define BITLOCATION_TWOBUTTONSONEPINBUTTONCONNECTEDTOGND 3
		
		
		uint8_t boolContainer;
	
		Button();
		bool getStateEdgeUp();
		bool getStateEdgeDown();
		bool getState();
		bool getPreviousState();
		
		//void oneButtonHandler();
		void twoButtonsOnePinHandler();
		////void setButtonPartOfTwoButtonsPerPinSystem(bool isPartOfTwoButtonsPerPin);
		void refresh();
		void setTwoButtonsOnePinButtonConnectedToGnd(bool connectedToGnd);
		
		
		void setPin( byte pin1  );
		
	private:
		

};

#endif



































































































































































































