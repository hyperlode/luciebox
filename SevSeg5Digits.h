//Written by Dean Reading, 2012.  deanreading@hotmail.com
//See .cpp file for info

#ifndef SevSeg5Digits_h
#define SevSeg5Digits_h

//#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#include "Utilitieslode.h"
// #include "uservariables.h"
//#else
//#include "WProgram.h"
//#endif

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

const byte selected_ascii_to_7seg_digit [] PROGMEM = {
  B00111111 , //'0' 
  B00000110, //'1'
  B01011011 , //'2'
  B01001111, //'3'
  B01100110, //'4'
  B01101101, //'5'
  B01111101, //'6'
  B00000111, //'7'
  B01111111, //'8'
  B01101111, //'9'
  ONLY_TOP_SEGMENT, //non ascii
  ONLY_MIDDLE_SEGMENT, //non ascii
  ONLY_BOTTOM_SEGMENT, //non ascii
  ONLY_TOP_AND_BOTTOM_SEGMENT, //non ascii
  NO_SEGMENTS, //non ascii
  B00000000, //non ascii
  B00000000, //non ascii
  B01110111, //'A'
  B01111100, //'B'
  B00111001, //'C'
  B01011110, //'D'
  B01111001, //'E'
  B01110001, //'F'
  B00111101, //'G'
  B01110100, //'H'
  B00000100, //'I'
  B00001100, //'J'
  B01110101, //'K'
  B00111000, //'L'
  B01010101, //'M'
  B01010100, //'N'
  B01011100, //'O'
  B01110011, //'P'
  B01100111, //'Q'
  B01010000, //'R'
  B01101101, //'S'
  B01111000, //'T'
  B00011100, //'U'
  B00011110, //'V'
  B00011101, //'W'
  B00110110, //'X'
  B01101110, //'Y'
  B00011011 //'Z'

};

class SevSeg5Digits
{

public:
  SevSeg5Digits();

  //Public Functions
//  void PrintOutput();
//  void PrintOutput1And4();
  byte* PrintOutputSeg(byte brightness);
  void NewText(char* text);
  void Begin(boolean mode_in,byte C1, byte C2, byte C3, byte C4, byte C5, byte UC1, byte UC2, byte UC3, byte UC4, byte UC5, byte UC6, byte UC7, byte UC8);
  void Begin(boolean mode_in,byte C1, byte C2, byte C3, byte C4, byte C5, byte LedArrayDigit, byte UC1, byte UC2, byte UC3, byte UC4, byte UC5, byte UC6, byte UC7, byte UC8);
  
  void SetSingleDigit(int8_t value, int digit);
  void SetFourDigits(uint32_t value);
  void SetDecPointSingle(boolean decDig, int digit);
  void SetLedArray(byte ledsAsBits);
  
  bool getMode();
  //Public Variables
  byte segActive;
private:
  //Private Functions
  void CreateArray();
  
  //Private Variables
 // boolean mode,DigitOn,DigitOff,SegOn,SegOff;

  uint32_t test;
  byte DigitPins[6];
  byte SegmentPins[8];
  // boolean lights[5][8];
  uint8_t lights[5];
  uint8_t ledArrayValues;
  bool extraLedArray;
  //byte chars[5];
  char* text;
  //boolean decPoints[5];

  uint8_t boolContainer;
  #define TEMPDECPOINTMEMORY 0
  #define MODEISCOMMONANODE 1  //should be defined in uservariables
 
  #define DIGITON getMode()
  #define DIGITOFF !getMode()
  #define SEGMENTON !getMode()
  #define SEGMENTOFF getMode()
  
//  byte digitActive;
  

};

#endif









































































