#include "DisplayDigitsHandler5Digits.h"

//changed from 4 to 5 digits 20140123 Lode

DisplayManagement::DisplayManagement (){
	this->text[5] = '\0';
	// brightness = (LED_DISP_BRIGHTNESS - 888)/1000;
	brightness = (8888 - LED_DISP_BRIGHTNESS )/1000;
	this->setScrollSpeed(SCROLL_SPEED_DELAY_MILLIS);
	//scrollNextMove.setInitTimeMillis();
  
};

DisplayManagement::~DisplayManagement(){
	delete [] scrollTextAddress;
};

void DisplayManagement::startUp(bool dispHasCommonAnode ,byte D0, byte D1, byte D2, byte D3, byte D4, byte LedArrayDigit, byte S1, byte S2, byte S3, byte S4, byte S5, byte S6, byte S7, byte S8){
	//for extra led array.
	sevseg.Begin(dispHasCommonAnode, D0,  D1,  D2,  D3, D4,LedArrayDigit,  S1,  S2,  S3,  S4,  S5,  S6,  S7,  S8); 
	textStartPos = 0;
}

void DisplayManagement::startUp(bool dispHasCommonAnode ,byte D0, byte D1, byte D2, byte D3, byte D4, byte S1, byte S2, byte S3, byte S4, byte S5, byte S6, byte S7, byte S8){
	sevseg.Begin(dispHasCommonAnode, D0,  D1,  D2,  D3, D4,  S1,  S2,  S3,  S4,  S5,  S6,  S7,  S8); 
	textStartPos = 0;
}

//void DisplayManagement::showSecondsBlinker(bool secondsBlink, bool menuItemValueQuickView){
//	sevseg.SetDecPoints(0, 0,  secondsBlink,  0, menuItemValueQuickView );
//}

void DisplayManagement::showNumberAsChars(int16_t number){
  // chars as in the alphabet: A=1, B=2,...
  for (int i=0;i<4;i++){
      text[4-i] = 64 + number; //ascii 65 = a
  }
  this->displayHandler(text);
}

void DisplayManagement::showNumber(int16_t number){
  //negative numbers not yet working.
  int16_t c;
  c = number;
  for (int i=0;i<4;i++){
    text[4-i] = 48 + c%10; //ascii 48 = 0
    c/=10;
  }
  this->displayHandler(text);
}

void DisplayManagement::setDecimalPoint(boolean isOn, int digit){
	sevseg.SetDecPointSingle(isOn,digit);
#ifdef ARDUINO_SIMULATION
	mtc->setPoint(isOn, digit);
#endif
}

void DisplayManagement::SetSingleDigit(uint8_t value, int digit){
  //arbitrary segments lights per digit. according to bits set in value (bit 0=A , bit6 is seg G, bit 7 = decimal point).
  sevseg.SetSingleDigit(value, digit);
}

void DisplayManagement::setBlankDisplay(){
  this->SetFourDigits(0x00000000); //reset display, includes the decimal points.
}
void DisplayManagement::SetFourDigits(uint32_t value){
  // value has 32 bits, that's 4x 8 bits. so for four digits by 8 segements.  digit1: DP G F ..... A  , DIGIT 2 DP G ...., ...
  sevseg.SetFourDigits(value);
}

void DisplayManagement::SetLedArray(byte ledsAsBits){
	sevseg.SetLedArray(ledsAsBits);
};

void DisplayManagement::displayHandler(char* inText){
	inText[5] = '\0';
	writeStringToDisplay(inText);
}

void DisplayManagement::displayHandlerSequence(char* movie){
	//take each time the 4 next chars and displays them at once (time in between = scrollNextMove timer)
	this->scrollTextAddress = movie ;
	scrollNextMove.start();
	this->textStartPos = 0;
	get5DigitsFromString(this->scrollTextAddress, text, this->textStartPos);
	writeStringToDisplay(text);
	this->isScrolling = true;
}

void DisplayManagement::doSequence(){
	if (this->isScrolling && scrollNextMove.getTimeIsNegative()){
    
		this->textStartPos += 5;
		
		//check for end of string in "next sequence"
		bool endOfString = false;
		for (int i = this->textStartPos; i < this->textStartPos + 5 ;i++){
			if (this->scrollTextAddress[i] == '\0'){  
				 endOfString = true;
			}
		}

		if (endOfString){
			this->textStartPos = 0;
		}

		//update display
		get5DigitsFromString(this->scrollTextAddress, text, this->textStartPos);
		writeStringToDisplay(text);
		//scrollNextMove.reset(); //start includes reset.
		scrollNextMove.start();
	}

}

void DisplayManagement::dispHandlerWithScroll(char* intext, bool comeScrollIn, bool scrollOnceElseInfinit){
	
	this->scrollTextAddress = intext;
	this->scrollOnceElseInfinit = scrollOnceElseInfinit;
	this->comeScrollIn = comeScrollIn;
	
	if (this->comeScrollIn){
		this->textStartPos = -5;
	}else{
		this->textStartPos = 0;
	}

	//scrollNextMove.reset();
	scrollNextMove.start();
	
	get5DigitsFromString(intext, text, this->textStartPos);
	writeStringToDisplay(text);
	this->isScrolling = true;
}

void DisplayManagement::doScroll(){ //char * intext
	//activate next move
  if (this->isScrolling && !scrollNextMove.getTimeIsNegative()){
		this->textStartPos++;
		//check for end of scrollstring
		if (this->textStartPos>0 && this->scrollTextAddress[textStartPos-1] == '\0'){
      if (this->scrollOnceElseInfinit){
				this->isScrolling = false;
      }

			if (this->comeScrollIn){
				this->textStartPos = -5;
			}else{
				this->textStartPos = 0;
			}
		}

		//dispUpdate
		get5DigitsFromString(this->scrollTextAddress, text, this->textStartPos);
		writeStringToDisplay(text);
		
		//scrollNextMove.reset();
		scrollNextMove.start();
	}
}

void DisplayManagement::get5DigitsFromString(char* in, char* out, int startPos){ //, int spacesBetweenRepeat
	//startPos can be a negative number (for scrolling in at the start), all negative numbers are "spaces". i.e. -2  => [' ', ' ', pos0char, pos1char]
	bool endOfString = false;
	int spacesAtEnd = 0;
	
	//check for end of string befor startpos in  incoming text. If so, then all spaces...
	for (int i = 0; i<startPos-1;i++){
		if (i > 0 && in[i] == '\0'){  
			endOfString = true;
		}
	}

	for (int i = 0; i<5; i++){
		if (startPos + i<0){
			out[i] = ' ';
		}else if (in[startPos + i] != '\0'){
			out[i] = in[startPos + i];
		}else{
			endOfString = true;
		}
		if (endOfString){
			out[i] = ' ';
		}
	}
}

void DisplayManagement::writeStringToDisplay(char* shortText){
  sevseg.NewText(shortText);  
}

//check scroll status
bool DisplayManagement::getIsScrolling(){
	return this->isScrolling;
}

//set scrollstatus 
void DisplayManagement::setIsScrolling(bool enableScroll){
	this->isScrolling = enableScroll;
}

void DisplayManagement::setBrightness(byte value, bool exponential){ //smaller number is brighter
	if (exponential){
	  this->brightness = value*value;
	}else{
    this->brightness = value;
	}
}

void DisplayManagement::setScrollSpeed(long value){
	scrollNextMove.setInitTimeMillis(value * -1);
  //scrollNextMove.start();
}

void DisplayManagement::getActiveSegmentAddress(byte** carrier){
	*carrier = this->activeSegment;
}

void DisplayManagement::refresh(){
	
	this->activeSegment = sevseg.PrintOutputSeg(this->brightness);

}



