//Written by Lode Ameije 2013
#ifndef DisplayDigitsHandler_h
#define DisplayDigitsHandler_h

// #if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
// #else
// #include "WProgram.h"
// #endif

//#include "SevSeg5Digits.h"
//#include "Countdowntimer.h"
#include "SuperTimer.h"

// #define ENABLE_SCROLL
#ifdef ENABLE_SCROLL

#define SCROLL_SPEED_DELAY_MILLIS 250
#endif

#define LED_DISP_BRIGHTNESS 8888


#define ONLY_TOP_SEGMENT_FAKE_ASCII 58
#define ONLY_MIDDLE_SEGMENT_FAKE_ASCII 59
#define ONLY_BOTTOM_SEGMENT_FAKE_ASCII 60
#define ONLY_TOP_AND_BOTTOM_SEGMENT_FAKE_ASCII 61
#define SPACE_FAKE_ASCII 62

#define ONLY_TOP_SEGMENT B00000001
#define ONLY_MIDDLE_SEGMENT B01000000
#define ONLY_BOTTOM_SEGMENT B00001000
#define ONLY_TOP_AND_BOTTOM_SEGMENT B00001001
#define NO_SEGMENTS B00000000


const byte selected_ascii_to_7seg_digit[] PROGMEM = {
    B00111111,                   //'0'
    B00000110,                   //'1'
    B01011011,                   //'2'
    B01001111,                   //'3'
    B01100110,                   //'4'
    B01101101,                   //'5'
    B01111101,                   //'6'
    B00000111,                   //'7'
    B01111111,                   //'8'
    B01101111,                   //'9'
    ONLY_TOP_SEGMENT,            //non ascii
    ONLY_MIDDLE_SEGMENT,         //non ascii
    ONLY_BOTTOM_SEGMENT,         //non ascii
    ONLY_TOP_AND_BOTTOM_SEGMENT, //non ascii
    NO_SEGMENTS,                 //non ascii
    B00000000,                   //non ascii
    B00000000,                   //non ascii
    B01110111,                   //'A'
    B01111100,                   //'B'
    B00111001,                   //'C'
    B01011110,                   //'D'
    B01111001,                   //'E'
    B01110001,                   //'F'
    B00111101,                   //'G'
    B01110100,                   //'H'
    B00000100,                   //'I'
    B00001100,                   //'J'
    B01110101,                   //'K'
    B00111000,                   //'L'
    B01010101,                   //'M'
    B01010100,                   //'N'
    B01011100,                   //'O'
    B01110011,                   //'P'
    B01100111,                   //'Q'
    B01010000,                   //'R'
    B01101101,                   //'S'
    B01111000,                   //'T'
    B00011100,                   //'U'
    B00011110,                   //'V'
    B00011101,                   //'W'
    B00110110,                   //'X'
    B01101110,                   //'Y'
    B00011011                    //'Z'

};

class DisplayManagement
{
public:
    DisplayManagement();
    // ~DisplayManagement();

    // void startUp(bool dispHasCommonAnode, byte D0, byte D1, byte D2, byte D3, byte D4, byte LedArrayDigit, byte S1, byte S2, byte S3, byte S4, byte S5, byte S6, byte S7, byte S8);
    // void startUp(bool dispHasCommonAnode, byte D0, byte D1, byte D2, byte D3, byte D4, byte S1, byte S2, byte S3, byte S4, byte S5, byte S6, byte S7, byte S8);

    // void showNumberAsChars(int16_t number); //deprecated
    // void bufToScreenBits(char *textBuf, uint32_t *screenBits);
    // void SetSingleDigit(uint8_t value, int digit); //deprecated
    
    void setText(char *inText); // updateDisplayChars
    void setCharToDisplay(char character, uint8_t digit);
    char* getTextHandle();
    void clearText();

    void showNumber(int16_t number); //updateDisplayNumber
    
    void SetFourDigits(uint32_t value); //  updateDisplayAllBits
    
    void setDecimalPoints(byte decimalPoints);
    void setDecimalPoint(boolean isOn, uint8_t digit); // updateDisplayDecimalPoint
    byte* getDecimalPointsHandle();

    void SetLedArray(byte ledsAsBits);  //  updateLights
    uint8_t* getLedArrayHandle();
    
    void setBlankDisplay(); //eraseAll

    void refresh();

    void numberToBuf(char *textBuf, int16_t number);
    void blanksToBuf(char* textBuf);

    void convert_4bytesArray_32bits(char* characters, uint32_t* displayAllSegments, boolean toArray);

    void charsToScreen (char* text, byte* digits);
    void charToScreen(char character, byte* digit);

    void setMultiplexerData(byte* multiplexerDigits);

    //void displaySetTextAndDecimalPoints(char *inText, uint8_t *decimalPoints);  //deprecated

    // void setBrightness(byte value, bool exponential); // should not be done here!

    // void getActiveSegmentAddress(byte **carrier);


#ifdef ENABLE_SCROLL

    void writeStringToDisplay(char *shortText);
    void get5DigitsFromString(char *in, char *out, int startPos);
    void displayHandlerSequence(char *movie);
    void dispHandlerWithScroll(char *intext, bool comeScrollIn, bool scrollOnceElseInfinit);
    void doSequence();
    void doScroll();

    //check scroll status
    bool getIsScrolling();

    //set scrollstatus
    void setIsScrolling(bool enableScroll);
    void setScrollSpeed(long value);
#endif

private:
    //SevSeg5Digits sevseg;
    // byte brightness;
    
    //update
    char text[4];
    byte lights;
    byte decimalPoints;
    uint32_t displayBinary;

    byte* multiplexerData; // pointer to array containing the actual lights data.

    //byte *activeSegment;

#ifdef ENABLE_SCROLL
    int textStartPos; //can be negative
    bool comeScrollIn;
    bool scrollOnceElseInfinit;
    bool isScrolling;
    SuperTimer scrollNextMove;
    char *scrollTextAddress;
#endif
};
#endif
