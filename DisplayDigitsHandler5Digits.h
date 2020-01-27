//Written by Lode Ameije 2013 
#ifndef DisplayDigitsHandler_h 
#define DisplayDigitsHandler_h 
#if defined(ARDUINO) && ARDUINO >= 100 
#include "Arduino.h" 
#else 
#include "WProgram.h" 
#endif 
#include "SevSeg5Digits.h"		
//#include "Countdowntimer.h"		
#include "SuperTimer.h"

#define LED_DISP_BRIGHTNESS 8888
#define SCROLL_SPEED_DELAY_MILLIS 250

class DisplayManagement {
    public:
        DisplayManagement ();
        ~DisplayManagement();
        void startUp(bool dispHasCommonAnode , byte D0,byte D1, byte D2, byte D3, byte D4, byte LedArrayDigit, byte S1, byte S2, byte S3, byte S4, byte S5, byte S6, byte S7, byte S8);
        void startUp(bool dispHasCommonAnode , byte D0,byte D1, byte D2, byte D3, byte D4, byte S1, byte S2, byte S3, byte S4, byte S5, byte S6, byte S7, byte S8);
        
        void showNumberAsChars(int16_t number);
        void showNumber(int16_t number);
        void setBlankDisplay();
        void setDecimalPoint(boolean isOn, int digit);
        void SetSingleDigit(uint8_t value, int digit);
        void SetFourDigits(uint32_t value);
        void SetLedArray(byte ledsAsBits);

        void displayHandler(char* inText);
        void displayHandlerSequence(char* movie);
        void doSequence();
        void dispHandlerWithScroll(char* intext, bool comeScrollIn, bool scrollOnceElseInfinit);
        void doScroll();
        
        void get5DigitsFromString(char* in, char* out, int startPos);
        void writeStringToDisplay(char* shortText);

        //check scroll status
        bool getIsScrolling();

        //set scrollstatus 
        void setIsScrolling(bool enableScroll);
        
        void setBrightness(byte value, bool exponential);
        void setScrollSpeed(long value);
        void refresh();
        void getActiveSegmentAddress(byte** carrier);

    private:
        
        SevSeg5Digits sevseg;
        byte brightness;
        
        int textStartPos; //can be negative
        bool comeScrollIn;
        bool scrollOnceElseInfinit;
        bool isScrolling;
        SuperTimer scrollNextMove;
        byte* activeSegment;
        char text[6];
        char *scrollTextAddress;
        
};
#endif 



