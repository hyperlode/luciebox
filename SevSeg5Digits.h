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









































































