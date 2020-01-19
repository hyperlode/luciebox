#ifndef APPS_H 
#define APPS_H 
#include "Arduino.h"
#include "SuperTimer.h"
#include "Buzzer.h"
#include "PotentioSelector.h"
#include "ButtonsDacR2r.h"
#include "BinaryInput.h"
#include <avr/eeprom.h>



#define ENABLE_MULTITIMER
#ifdef ENABLE_MULTITIMER
#include "MiniMultiTimer.h"
#endif

#include "DataPlayer.h"

#include "SevSeg5Digits.h" //sevseb -->this should not be included here, but only in DisplayDigitsHandler.h, as it only gets used there (but ARDUINO needs this here!) DEBUG
#include "DisplayDigitsHandler5Digits.h"
#include "PretbakSettings.h"
#include "Potentio.h"

// #define TILT_FORWARD 	0x00000001
// #define TILT_BACKWARD 	0x00000010
// #define TILT_LEFT 		0x00000100
// #define TILT_RIGHT 		0x00001000
#define TILT_FORWARD 	0
#define TILT_BACKWARD 	1
#define TILT_LEFT 		2
#define TILT_RIGHT 		3

#define DO_NOTHING		0
#define TOGGLE_SEGMENT	1
#define MOVE_RIGHT		2
#define MOVE_LEFT		3
#define MOVE_DOWN		4
#define MOVE_UP	  	    5
#define ANIMATION_STOP_CODE 0x00

#define TIMER_METRONOME generalTimer
#define TIMER_REACTION_GAME_SPEED generalTimer
#define TIMER_REACTION_GAME_RESTART_DELAY generalTimer2

#define COUNTER_GEIGER counter3
#define GEIGER_INCREASE_CHANCE counter_long

#define TIMER_INIT_APP generalTimer


const uint8_t app_splash_screens [] PROGMEM = {
	//sorted by selector number
	
	// 0x38, 0x3F, 0x5E, 0x79, // LODE minimultitimer
	// 0x5E, 0x5E, 0x01, 0x5F, // music notes. duh!  (doesn't show nicely)
	// 0x00, 0x7C, 0x5E, 0x6F, // BdG
	// 0x6F, 0x79, 0x06, 0x6F, // GEIG geiger
	// 0x70, 0x06, 0x38, 0x70, // TILT
	0x61, 0x43, 0x58, 0x4C, // abstract two half screen circles.
	0xD8, 0xE9, 0xCB, 0xC4, // abstract drol of tank
	0x39, 0x09, 0x09, 0x0F, // abstractbig circle
	0x40, 0x39, 0x0F, 0x40, // abstract. circle with lines
	0x5E, 0x7C, 0x67, 0x73, // abstract.  bolletjes with verticallines
	0xB9, 0x40, 0x40, 0x0F, // abstract. [--]
	0x5D, 0x6B, 0x5D, 0x5D, // abstract   camion
	0x49, 0x49, 0x49, 0x49, // abstract horizontal lines.
	0x08, 0xCE, 0x78, 0x08, // abstract piramid
	0x40, 0x4F, 0x79, 0x40, // abstract art deco 
	0x36, 0x36, 0x36, 0x36, // abstract vertical lines
	0x00, 0x63, 0xDA, 0x63, // abstract face 0x63, 0xDA, 0x6C, 0x63
        0x5F, 0x79, 0x1C, 0x3D  // DBUG
		
};

const uint8_t disp_4digits_animations [] PROGMEM = {
	0x35,  // first byte = length of animation in bytes  (here 53, including the length byte)
// const uint32_t disp_4digits_swoosh [] PROGMEM = {
    0x00,0x00,0x00,0x00, // horizontal right to left sweep.
    0x00,0x00,0x00,0x06,
    0x00,0x00,0x00,0x49,
    0x00,0x00,0x00,0x30,
    0x00,0x00,0x06,0x00,
    0x00,0x00,0x49,0x00,
    0x00,0x00,0x30,0x00,
	0x00,0x06,0x00,0x00,
    0x00,0x49,0x00,0x00,
    0x00,0x30,0x00,0x00,
	0x06,0x00,0x00,0x00,
    0x49,0x00,0x00,0x00,
    0x30,0x00,0x00,0x00, //last byte is byte 52.
	0x19,  //length of next animation (25 including the length)  byte53
	0x00,0x00,0x00,0x00, // vertical swoop     
	0x01,0x01,0x01,0x01,
	0x22,0x22,0x22,0x22,
	0x40,0x40,0x40,0x40,
	0x14,0x14,0x14,0x14,
	0x08,0x08,0x08,0x08,   // byte 77,
	0x81, // ( length of following animation, including the length byte) 
	
	0x00,0x00,0x00,0x00,
	
	0x21,0x00,0x00,0x00, // sweep in
	0x71,0x00,0x00,0x00, 
	0x7B,0x00,0x00,0x00, 
	0xFF,0x00,0x00,0x00, 
	0xFF,0x21,0x00,0x00, 
	0xFF,0x71,0x00,0x00, 
	0xFF,0x7B,0x00,0x00, 
	0xFF,0xFF,0x00,0x00, 
	0xFF,0xFF,0x21,0x00, 
	0xFF,0xFF,0x71,0x00, 
	0xFF,0xFF,0x7B,0x00, 
	0xFF,0xFF,0xFF,0x00,
	0xFF,0xFF,0xFF,0x21, 
	0xFF,0xFF,0xFF,0x71, 
	0xFF,0xFF,0xFF,0x7B, 
	
	0xFF,0xFF,0xFF,0xFF,  // horizonal sweep.sweep out
	0xDE,0xFF,0xFF,0xFF, 
	0x8E,0xFF,0xFF,0xFF, 
	0x84,0xFF,0xFF,0xFF, 
	0x00,0xFF,0xFF,0xFF, 
	0x00,0xDE,0xFF,0xFF, 
	0x00,0x8E,0xFF,0xFF, 
	0x00,0x84,0xFF,0xFF, 
	0x00,0x00,0xFF,0xFF, 
	0x00,0x00,0xDE,0xFF, 
	0x00,0x00,0x8E,0xFF, 
	0x00,0x00,0x84,0xFF, 
	0x00,0x00,0x00,0xFF, 
	0x00,0x00,0x00,0xDE, 
	0x00,0x00,0x00,0x8E, 
	0x00,0x00,0x00,0x84, 
		
	ANIMATION_STOP_CODE   //length 0 byte is stop byte.
};	

#define ALL_DATA_SIZE 1
//PGM_P const allData[ALL_DATA_SIZE] PROGMEM = {disp_4digits_animations};

const uint8_t song_lang_zal_ze_leven [] PROGMEM = {
  C7_4,rest_4,rest_2,
  C7_4,rest_2, C7_8,rest_8,
  C7_4,rest_4,rest_2,
  G6_2,G6_4,rest_2, 

  E7_4,rest_4,rest_2,
  E7_4,rest_2, E7_8,rest_8,
  E7_4,rest_4,rest_2,
  C7_2,C7_4,rest_2, 

  G7_4,rest_4,rest_2,
  G7_4,rest_2, G7_8,rest_8,
  
  A7_8,rest_8,rest_4,
  G7_8,rest_8,rest_4,
  
  F7_8,rest_8,rest_4,
  E7_8,rest_8,rest_4,
  
  D7_2,D7_4,rest_4,
  D7_2,D7_4,rest_4,
  D7_2,D7_4,rest_4,
  rest_4,
  G7_8,rest_8,rest_4,
  F7_8,rest_8,rest_4,

  E7_1,E7_2,rest_2,
  F7_1,F7_2,rest_2,

  G7_1,G7_2,rest_2,
  A7_2,A7_4,rest_4,
  F7_2,F7_4,rest_4,

  E7_1,E7_2,rest_2,
  D7_1,D7_2,rest_2,
  C7_1,C7_1,BUZZER_ROLL_SONG_STOPVALUE
  };
const uint8_t song_happy_dryer [] PROGMEM = {
  A6_2,rest_4,rest_2,
  Cs7_2,rest_4,rest_2,
  E7_4,rest_8,rest_4,
  Cs7_4,rest_8,rest_4,
  E7_4,rest_8,rest_4,
  A7_1,A7_1,
  rest_2,BUZZER_ROLL_SONG_STOPVALUE};
const uint8_t song_unhappy_dryer[] PROGMEM = {A6_1,rest_2,Cs7_1,rest_2,E7_2,rest_4,Cs7_2,rest_4,B6_2,rest_4,A6_1,rest_2,rest_2,BUZZER_ROLL_SONG_STOPVALUE};
const uint8_t song_attack [] PROGMEM = {Gs6_2,rest_4,Gs6_2,rest_4,Gs6_2,rest_4, Cs7_2,rest_2,rest_2,Gs6_2,rest_4,Cs7_1,Cs7_1,Cs7_1 ,BUZZER_ROLL_SONG_STOPVALUE}; // aaanvallueeeeee!
const uint8_t kindeke_douwen [] PROGMEM = {
	
	// Gs6_2,rest_4,Gs6_2,rest_4,Gs6_2,rest_4, Cs7_2,rest_2,rest_2,Gs6_2,rest_4,Cs7_1,Cs7_1,Cs7_1 
	B7_4,rest_4,rest_2, // wie
	B7_1,  //zal
	rest_2,A7_4,rest_4, //er
	G6_4,rest_4,rest_2, //ons
	B7_1, //kind-
	B7_4,rest_4,A7_4,rest_4, //-de
	G6_4,rest_4,rest_2, //-ke
	A7_1, //dou
	A7_4, rest_4,rest_2 ,
	C7_4, rest_4,rest_2,
    C7_1, //wen
	rest_1,
	C7_4,rest_4,rest_2, // dat 
	B7_1, // schaars
	B7_4,rest_4,B7_4, rest_4,
	B7_4, rest_4, rest_2, 
	
	
	E7_4,rest_4,rest_2, // 
	E7_1,
	E7_4,rest_4,E7_4, rest_4,
	A7_4, rest_4, rest_2, 
	
	// E7_4,rest_4,rest_2, // dat schaars...
	// E7_1,
	// rest_2,E7_4, rest_4,
	// A7_2,rest_2,
	
	// E7_4,rest_4,rest_2, // dat schaars...
	// E7_1,
	// rest_2,E7_4, rest_4,
	// A7_2,rest_2,
	
	
	
	BUZZER_ROLL_SONG_STOPVALUE}; // kleine kleine moederke alleen

const uint8_t alphabeth_song [] PROGMEM = {
	
	C7_4,rest_4,rest_2, C7_4,rest_4,rest_2, G7_4,rest_4,rest_2, G7_4,rest_4,rest_2, A7_4,rest_4,rest_2, A7_4,rest_4,rest_2,	G7_1,rest_1,
	F7_4,rest_4,rest_2, F7_4,rest_4,rest_2,	E7_4,rest_4,rest_2, E7_4,rest_4,rest_2, D7_4,rest_4, D7_4,rest_4, D7_4,rest_4, D7_4,rest_4, C7_1,rest_1,
	G7_4,rest_4,rest_2, G7_4,rest_4,rest_2, F7_4,rest_4,rest_2, F7_4,rest_4,rest_2, E7_4,rest_4,rest_2, E7_4,rest_4,rest_2, D7_1,rest_1, C7_4,rest_4,rest_2,
	C7_4,rest_4,rest_2, G7_4,rest_4,rest_2, G7_4,rest_4,rest_2, A7_1, rest_1, G7_4,rest_4,rest_2, rest_1,
	F7_4,rest_4,rest_2, F7_4,rest_4,rest_2,	E7_4,rest_4,rest_2, E7_4,rest_4,rest_2, D7_4,rest_4,rest_2, G7_4,rest_4,rest_2, C7_1,C7_1, 	
	BUZZER_ROLL_SONG_STOPVALUE
};

const uint8_t song_retreat [] PROGMEM = {Gs6_2,rest_4,Gs6_2,rest_4,Gs6_2,rest_4,Gs6_2,rest_2,rest_2,Gs6_2,rest_4,Cs6_1,Cs6_1,Cs6_1 ,BUZZER_ROLL_SONG_STOPVALUE};//  retreat!
const uint8_t scale_major [] PROGMEM = {C7_2,rest_4,D7_2,rest_4,E7_2,rest_4,F7_2,rest_4,G7_2,rest_4,A7_2,rest_4,B7_2,rest_4,C8_2,rest_4,BUZZER_ROLL_SONG_STOPVALUE};
const uint8_t scale_major_reversed [] PROGMEM = {C8_2,rest_4,B7_2,rest_4,A7_2,rest_4,G7_2,rest_4,F7_2,rest_4,E7_2,rest_4,D7_2,rest_4,C7_2,rest_4,BUZZER_ROLL_SONG_STOPVALUE};


const uint8_t lights_indexed [] = {LIGHT_YELLOW, LIGHT_RED, LIGHT_GREEN, LIGHT_BLUE, LIGHT_LED_1, LIGHT_LED_2, LIGHT_LED_3};
const uint8_t buttons_indexed [] = {BUTTON_LATCHING_YELLOW, BUTTON_MOMENTARY_RED, BUTTON_MOMENTARY_GREEN, BUTTON_MOMENTARY_BLUE};
  

const uint32_t tilt_forward [] PROGMEM = {
	0x08,0x08,0x08,0x08
};
const uint32_t tilt_backward [] PROGMEM = {
	0x01,0x01,0x01,0x01
};
const uint32_t tilt_left [] PROGMEM = {
	0x30,0x30,0x30,0x30
};
const uint32_t tilt_right [] PROGMEM = {
	0x06,0x06,0x06,0x06
};

const uint32_t disp_4digits_animate [] PROGMEM = {
    0x00,0xFF, 0xF0,0x0F,
    0xFF,0x00, 0x00,0x01,
    0x01,0x02, 0x04,0x08
    };
const uint32_t disp_4digits_animate_circle [] PROGMEM = {
    0x01,0x00,0x00,0x00,
    0x00,0x01,0x00,0x00,
    0x00,0x00,0x01,0x00,
    0x00,0x00,0x00,0x01,
    0x00,0x00,0x00,0x02,
    0x00,0x00,0x00,0x04,
    0x00,0x00,0x00,0x08,
    0x00,0x00,0x08,0x00,
    0x00,0x08,0x00,0x00,    
    0x08,0x00,0x00,0x00,    
    0x10,0x00,0x00,0x00,    
    0x20,0x00,0x00,0x00    
    };
	
	

	

//INPUT



class Apps{

  public:

  Apps();

  BinaryInput* binaryInputs;
  DisplayManagement* ledDisp;
  Buzzer* buzzer;
  Potentio* potentio;
  
  
  void setPeripherals( BinaryInput* binaryInput, Potentio* potentio, DisplayManagement* ledDisp, Buzzer* buzzer);
  void setBuffers(char* textBuf, char*  scrollBuf);
  void test();
  
  void setDefaultMode();
  void appSelector(bool init, uint8_t selector);
  bool init_app(bool init, uint8_t selector);
  void modeScroll(bool init);
  void modeSimpleButtonsAndLights();
  void modeCountingLettersAndChars(bool init);
  void modeSoundSong(bool init);
  void modeSoundNotes();
  void draw(bool init);
  void movieAnimationMode(bool init);
  uint16_t _animationGetStartByte(uint8_t number);
  void modeSingleSegmentManipulation(bool init);
  void modeGeiger(bool init);
  void modeSequencer(bool init);
  void modeMetronome(bool init);
  void modeSimon(bool init);
  void gameButtonInteraction(bool init);
  void tiltSwitchTest(bool init);
  void modeButtonDebug(bool init);
  void miniMultiTimer(bool init);
  
  
  void fadeInList(uint32_t* movie, uint8_t length, uint32_t startScreen);
  void shuffle(uint8_t* list, uint8_t length);
  
  private:
  DataPlayer dataPlayer;
  #ifdef ENABLE_MULTITIMER
  MiniMultiTimer multiTimer;
  #endif
  int16_t nextStepRotate(int16_t counter, bool countUpElseDown, int16_t minValue, int16_t maxValue);
  void _eepromWriteByteIfChanged(uint8_t* address , uint8_t value);
  
  SuperTimer generalTimer;
  SuperTimer generalTimer2;
  
  //global app variables
  enum {
    sequencer_bufsize = 32,
  };
  uint8_t sequencer_song[sequencer_bufsize];
  uint8_t* game_random;
  uint32_t screenPersistenceOfVision;
  //SuperTimer animation_speed;
  uint8_t allNotesIndex;
  int16_t counter;
  int16_t counter2;
  int16_t counter3;
  long counter_long;
  bool numberElseAlphabethMode;
  // int16_t animation_step;
  uint8_t game_x_pos;
  uint8_t game_y_pos;
  uint8_t reactionGameTarget;
  
  long frequency_lower;
  long frequency_upper;
  long tone_length_millis;
  long geiger_trigger_chance;
  
  long initTime;
  bool reactionGameYellowButtonIsIncluded;
  uint8_t selectedSounds[4];
  uint8_t dispState[4];
  
  uint32_t displaySequence[32];

  // simon
  enum SimonState {
      simonWaitForNewGame,
      simonNewGame,
      simonNewLevel,
      simonPlaySequence,
      simonUserRepeats,
  };
  SimonState simonState;
  uint8_t simonLength;
  int8_t simonIndex;

  char*  textBuf;
  char*  scrollBuf;
  uint8_t lights;
  
  bool app_init_mode;
  
};

#endif 
