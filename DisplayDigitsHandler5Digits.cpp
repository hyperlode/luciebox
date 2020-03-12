#include "DisplayDigitsHandler5Digits.h"

//changed from 4 to 5 digits 20140123 Lode

DisplayManagement::DisplayManagement()
{
	// this->text[5] = '\0';
	// brightness = (8888 - LED_DISP_BRIGHTNESS) / 1000;
	setBlankDisplay();

#ifdef ENABLE_SCROLL
	this->setScrollSpeed(SCROLL_SPEED_DELAY_MILLIS);
#endif
};

// DisplayManagement::~DisplayManagement()
// {
// #ifdef ENABLE_SCROLL
// 	delete[] scrollTextAddress;
// #endif
// };

// void DisplayManagement::startUp(bool dispHasCommonAnode, byte D0, byte D1, byte D2, byte D3, byte D4, byte LedArrayDigit, byte S1, byte S2, byte S3, byte S4, byte S5, byte S6, byte S7, byte S8)
// {
// 	//for extra led array.
// 	sevseg.Begin(dispHasCommonAnode, D0, D1, D2, D3, D4, LedArrayDigit, S1, S2, S3, S4, S5, S6, S7, S8);

// #ifdef ENABLE_SCROLL
// 	textStartPos = 0;
// #endif
// }

// void DisplayManagement::startUp(bool dispHasCommonAnode, byte D0, byte D1, byte D2, byte D3, byte D4, byte S1, byte S2, byte S3, byte S4, byte S5, byte S6, byte S7, byte S8)
// {
// 	sevseg.Begin(dispHasCommonAnode, D0, D1, D2, D3, D4, S1, S2, S3, S4, S5, S6, S7, S8);

// #ifdef ENABLE_SCROLL
// 	textStartPos = 0;
// #endif
// }

// void DisplayManagement::showNumberAsChars(int16_t number)
// {
// 	// chars as in the alphabet: A=1, B=2,...
// 	for (int i = 0; i < 4; i++)
// 	{
// 		text[4 - i] = 64 + number; //ascii 65 = a
// 	}
// 	this->setText(text);
// }

// void DisplayManagement::showNumber(int16_t number, bool noLeadingZeros){

void DisplayManagement::setText(char *inText) // updateDisplayChars
{
	//inText[5] = '\0';
	//writeStringToDisplay(inText);
	for (uint8_t i=0;i<4;i++){
		setCharToDisplay(inText[i],i);
	}
}

void DisplayManagement::setCharToDisplay(char character, uint8_t digit){
	this->text[digit] = character;
}

char* DisplayManagement::getTextHandle(){
	return this->text;
}

void DisplayManagement::clearText(){

	blanksToBuf(this->text);
}


void DisplayManagement::showNumber(int16_t number) //updateDisplayNumber
{
	numberToBuf(this->text, number);
	this->setText(text);
}

void DisplayManagement::SetFourDigits(uint32_t value) //updateDisplayAllBits
{
	// value has 32 bits, that's 4x 8 bits. so for four digits by 8 segements.  digit1: DP G F ..... A  , DIGIT 2 DP G ...., ...
	//sevseg.SetFourDigits(value);
	this->displayBinary = value;
}

void DisplayManagement::setDecimalPoints(byte decimalPoints){
	this->decimalPoints = decimalPoints;
}

void DisplayManagement::setDecimalPoint(boolean isOn, uint8_t digit) // updateDisplayDecimalPoint
{
	//sevseg.SetDecPointSingle(isOn, digit);
	if (isOn){
		this->decimalPoints |= 1<<digit;
	}else{
		this->decimalPoints &= ~(1<<digit);
	}

#ifdef ARDUINO_SIMULATION
	mtc->setPoint(isOn, digit);
#endif

}

byte* DisplayManagement::getDecimalPointsHandle()
{
	return &decimalPoints;
}



void DisplayManagement::SetLedArray(byte ledsAsBits) //  updateLights
{
	//sevseg.SetLedArray(ledsAsBits);
	this->lights = ledsAsBits;
};

byte* DisplayManagement::getLedArrayHandle(){
	return &this->lights;
}



void DisplayManagement::convert_text4Bytes_to_32bits(char* text, uint32_t* binary){
	*binary = 0;
	for (uint8_t i=0;i<4;i++){
		byte digitBinary;
		charToScreen(text[i], &digitBinary);
		*binary |= (uint32_t) digitBinary << (8*i);		
	}
}


void DisplayManagement::convert_4bytesArray_32bits(char* characters, uint32_t* displayAllSegments, boolean toArray){
	
	if (toArray){
		for(uint8_t i=0;i<4;i++){
			characters[i] = (uint8_t)((*displayAllSegments >> (i * 8)) & 0xFF) ;
		}
	}else{
		*displayAllSegments = 0;

		for(uint8_t i=0;i<4;i++){
			*displayAllSegments |= (uint32_t)(characters[i]) << (8*i); 
		}
	}
}

void DisplayManagement::setBlankDisplay()
{
	//this->SetFourDigits(0x00000000); //reset display, includes the decimal points.
	this->displayBinary = 0;
	clearText();
	this->decimalPoints = 0;
	this->lights = 0;
}

// void DisplayManagement::bufToScreenBits(char *textBuf, uint32_t *screenBits)
// {
// 	*screenBits = 0;
// 	for (uint8_t i = 0; i < 4; i++)
// 	{
// 		*screenBits |= (uint32_t)(textBuf[i]) << (8 * i);
// 	}
// }

void DisplayManagement::minutesToMinutesHoursString(char* textBuf, uint16_t minutes){

	textBuf[0] = 48 + minutes/600;
	minutes %= 600;
	textBuf[1] = 48 + minutes/60;
	minutes %= 60;
	textBuf[2] = 48 + minutes/10; 
	textBuf[3] = 48 + minutes%10; 
}

void DisplayManagement::blanksToBuf(char* textBuf){
	textBuf[0]=SPACE_FAKE_ASCII;
	textBuf[1]=SPACE_FAKE_ASCII;
	textBuf[2]=SPACE_FAKE_ASCII;
	textBuf[3]=SPACE_FAKE_ASCII;
}

void DisplayManagement::numberToBuf(char *textBuf, int16_t number)
{
	// negative numbers made absolute!
	// textbuf four positions.

	int16_t c;
	number = abs(number);
	c = number;

	blanksToBuf(textBuf);

	for (uint8_t i = 0; i < 4; i++)
	{

		textBuf[3 - i] = 48 + c % 10; //ascii 48 = 0
		//textBuf[3 - i] = 48 ;
		c /= 10;
		if (c==0){
			break;
		}
	}

}

// void DisplayManagement::SetSingleDigit(uint8_t value, int digit){
//   //arbitrary segments lights per digit. according to bits set in value (bit 0=A , bit6 is seg G, bit 7 = decimal point).
//   sevseg.SetSingleDigit(value, digit);
// }

// void DisplayManagement::displaySetTextAndDecimalPoints(char *inText, uint8_t *decimalPoints)
// {
// 	setText(inText);
// 	// always think of 5 digits.
// 	for (uint8_t i = 0; i < 5; i++)
// 	{
// 		setDecimalPoint(*decimalPoints & (0x01 << i), i);
// 	}
// }

//Create Array
/*******************************************************************************************/
//From the numbers found, says which LEDs need to be turned on

void DisplayManagement::charsToScreen(char* text, byte* digits)
{
	// expect 4 chars.
	for (byte index = 0; index < 4; index++)
	{
		charToScreen(text[index], &digits[index]);
	}
}

void DisplayManagement::charToScreen(char character, byte* digit)
{

	if (character == ' ')
	{
		character = SPACE_FAKE_ASCII;
	}

	if (character == '?')
	{
		byte i = random(0, 7);

		// toggles one segment of the digit ?!
		setBit(digit, !getBit(digit, i), i); //random on or off...
	}
	else if (character < 91)
	{
		*digit = pgm_read_byte_near(selected_ascii_to_7seg_digit + character - 48);
	}
	else
	{
		// illegal state
	}
}

// void DisplayManagement::getActiveSegmentAddress(byte **carrier)
// {
// 	*carrier = this->activeSegment;
// }

void DisplayManagement::setMultiplexerData(byte* multiplexerDigits){
	this->multiplexerData = multiplexerDigits;
}

void DisplayManagement::refresh()
{

	// set it all up.

	for (uint8_t i = 0;i<5;i++){

		// start with a clean sheet.
		multiplexerData[i] = 0;

		if (i < 4){
			// led display digits			
			charToScreen(text[i], &multiplexerData[i]);
			multiplexerData[i] |=  (uint8_t)((displayBinary >> (i * 8)) & 0xFF) ;
			multiplexerData[i] |=  getBit(&this->decimalPoints, i) << 7;

		}else{
			// button lights
			multiplexerData[i] = lights;
		}
	}
}

#ifdef ENABLE_SCROLL

void DisplayManagement::writeStringToDisplay(char *shortText)
{
	sevseg.NewText(shortText);
}
void DisplayManagement::get5DigitsFromString(char *in, char *out, int startPos)
{ //, int spacesBetweenRepeat
	//startPos can be a negative number (for scrolling in at the start), all negative numbers are "spaces". i.e. -2  => [' ', ' ', pos0char, pos1char]
	bool endOfString = false;
	//int spacesAtEnd = 0;

	//check for end of string befor startpos in  incoming text. If so, then all spaces...
	for (int i = 0; i < startPos - 1; i++)
	{
		if (i > 0 && in[i] == '\0')
		{
			endOfString = true;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (startPos + i < 0)
		{
			out[i] = ' ';
		}
		else if (in[startPos + i] != '\0')
		{
			out[i] = in[startPos + i];
		}
		else
		{
			endOfString = true;
		}
		if (endOfString)
		{
			out[i] = ' ';
		}
	}
}

void DisplayManagement::displayHandlerSequence(char *movie)
{
	//take each time the 4 next chars and displays them at once (time in between = scrollNextMove timer)
	this->scrollTextAddress = movie;
	scrollNextMove.start();
	this->textStartPos = 0;
	get5DigitsFromString(this->scrollTextAddress, text, this->textStartPos);
	writeStringToDisplay(text);
	this->isScrolling = true;
}

void DisplayManagement::doSequence()
{
	if (this->isScrolling && scrollNextMove.getTimeIsNegative())
	{

		this->textStartPos += 5;

		//check for end of string in "next sequence"
		bool endOfString = false;
		for (int i = this->textStartPos; i < this->textStartPos + 5; i++)
		{
			if (this->scrollTextAddress[i] == '\0')
			{
				endOfString = true;
			}
		}

		if (endOfString)
		{
			this->textStartPos = 0;
		}

		//update display
		get5DigitsFromString(this->scrollTextAddress, text, this->textStartPos);
		writeStringToDisplay(text);
		//scrollNextMove.reset(); //start includes reset.
		scrollNextMove.start();
	}
}

void DisplayManagement::dispHandlerWithScroll(char *intext, bool comeScrollIn, bool scrollOnceElseInfinit)
{

	this->scrollTextAddress = intext;
	this->scrollOnceElseInfinit = scrollOnceElseInfinit;
	this->comeScrollIn = comeScrollIn;

	if (this->comeScrollIn)
	{
		this->textStartPos = -5;
	}
	else
	{
		this->textStartPos = 0;
	}

	//scrollNextMove.reset();
	scrollNextMove.start();

	get5DigitsFromString(intext, text, this->textStartPos);
	writeStringToDisplay(text);
	this->isScrolling = true;
}

void DisplayManagement::doScroll()
{ //char * intext
	//activate next move
	if (this->isScrolling && !scrollNextMove.getTimeIsNegative())
	{
		this->textStartPos++;
		//check for end of scrollstring
		if (this->textStartPos > 0 && this->scrollTextAddress[textStartPos - 1] == '\0')
		{
			if (this->scrollOnceElseInfinit)
			{
				this->isScrolling = false;
			}

			if (this->comeScrollIn)
			{
				this->textStartPos = -5;
			}
			else
			{
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

//check scroll status
bool DisplayManagement::getIsScrolling()
{
	return this->isScrolling;
}

//set scrollstatus
void DisplayManagement::setIsScrolling(bool enableScroll)
{
	this->isScrolling = enableScroll;
}

void DisplayManagement::setScrollSpeed(long value)
{
	scrollNextMove.setInitTimeMillis(value * -1);
	//scrollNextMove.start();
}

#endif
