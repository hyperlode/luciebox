#ifndef APPS_H 
#define APPS_H 


#define ENABLE_EEPROM
#define ENABLE_MULTITIMER
#define SIMON_APP
#define ENABLE_REACTION_APP

#include "Arduino.h"
#include "SuperTimer.h"
#include "Buzzer.h" 
#include "PotentioSelector.h"
#include "ButtonsDacR2r.h"
#include "BinaryInput.h"
#ifdef ENABLE_EEPROM
   #include <avr/eeprom.h>
  // #include <EEPROM.h>
#endif

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
    // enum DiceRollMode:uint8_t{
    //   rollOneDice,
    //   rollFourDice,
    //   takeRandomCard,
    //   takeRandomCardFromDeck,
    //   randomNumber,
    //   randomLetter,
    //   headsOrTails,
    //   yesOrNo,
    // };
    // DiceRollMode diceRollMode;


 #define DICEROLL_ROLLONEDICE 0
 #define DICEROLL_ROLLFOURDICE 10
 #define DICEROLL_TAKERANDOMCARD 1
 #define DICEROLL_TAKERANDOMCARDFROMDECK 11
 #define DICEROLL_RANDOMNUMBER 2
 #define DICEROLL_RANDOMLETTER 12
 #define DICEROLL_HEADSORTAILS 3
 #define DICEROLL_YESORNO 13


// VARIABLE REUSE

#define TIMER_METRONOME generalTimer
#define TIMER_INIT_APP generalTimer
#define TIMER_REACTION_GAME_SPEED generalTimer
#define TIMER_REACTION_GAME_RESTART_DELAY generalTimer
#define SETTINGS_MODE_DISPLAY_VALUES_BLINK generalTimer
#define DICEROLL_ROLL_SPEED generalTimer

#define SEQUENCER_EEPROM_MODE_BLINK generalTimer2
#define DICEROLL_AUTODRAW_DELAY generalTimer2

#define SOUND_FUN_NOTE_INDEX counter
#define REACTION_GAME_SCORE counter
#define GEIGER_TONE_FREQUENY_LOWEST counter
#define DRAW_ACTIVE_SEGMENT counter
#define SIMON_LENGTH counter
#define METRONOME_TICKER_1_POSITION counter
#define DICEROLL_RANDOM_NUMBER counter
#define SEQUENCER_STEP_COUNTER counter

#define GEIGER_TONE_FREQUENCY_HEIGHEST counter2
#define REACTION_GAME_TIMER_STEP counter2
#define DRAW_X_POS counter2
#define SIMON_INDEX counter2
#define METRONOME_TICKER_2_POSITION counter2
#define DEBUGMODE_ACTIVATED counter2
#define DICEROLL_CARD_FROM_DECK_INDEX counter2


#define DRAW_Y_POS counter3
#define GEIGER_INCREASE_CHANCE counter3
#define GEIGER_PROBABILITY_THRESHOLD counter3
#define REACTION_GAME_STEP_TIME_MILLIS counter3
#define METRONOME_TICKER_3_POSITION counter3
#define SETTINGS_MODE_SELECTOR counter3
#define DICEROLL_ANIMATION_DELAY counter3


#define REACTION_GAME_TARGET counter4
#define COUNTER_GEIGER counter4
#define SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET counter4


#define REACTION_GAME_LEVEL counter5
#define GEIGER_TONE_LENGTH counter5
#define SEQUENCER_TEMP_NOTE counter5
#define DICEROLL_RANDOM_TYPE counter5

// #define REACTION_GAME_COUNTDOWN counter6

#define REACTION_COUNTDOWN_MODE general_boolean  

#define REACTION_GUITAR_HERO_MODE general_boolean2



#define SIMON_LIST bytes_list
#define SEQUENCER_SONG bytes_list
#define FADE_IN_RANDOM_LIST bytes_list
#define CARDS_DECK bytes_list

#define DRAW_DISP_STATE array_4_bytes
#define REACTION_GAME_SELECTED_SOUNDS array_4_bytes



const uint8_t app_splash_screens [] PROGMEM = {
	//sorted by selector number
  
	0x61, 0x43, 0x58, 0x4C, // abstract two half screen circles.
	0xD8, 0xE9, 0xCB, 0xC4, // abstract drol or tank
	0x39, 0x09, 0x09, 0x0F, // abstractbig circle
	0x40, 0x39, 0x0F, 0x40, // abstract. circle with lines
	0x5E, 0x7C, 0x67, 0x73, // abstract.  bolletjes with verticallines
	//0xB9, 0x40, 0x40, 0x0F, // abstract. [--]
  0x32, 0x26, 0x34, 0x16,  // vertical lines short and long 
	//0x5D, 0x6B, 0x5D, 0x5D, // abstract   camion
	0x49, 0x49, 0x49, 0x49, // abstract horizontal lines.
  0x0F, 0x40, 0x40, 0x39, //wrench
	0x08, 0xCE, 0x78, 0x08, // abstract piramid
	0x40, 0x4F, 0x79, 0x40, // abstract art deco 
	0x36, 0x36, 0x36, 0x36, // abstract vertical lines
	0x00, 0x63, 0xDA, 0x63, // abstract face 0x63, 0xDA, 0x6C, 0x63
  0x5F, 0x79, 0x1C, 0x3D  // DBUG
		
};

// const uint8_t disp_dice [] PROGMEM = {
//   0x00,0x00,0x00,0x00,


// }

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

const uint8_t scale_pentatonic [] PROGMEM = {C6_2, D6_2, F6_2, G6_2, A7_2,BUZZER_ROLL_SONG_STOPVALUE};

const uint8_t lights_indexed [] = {LIGHT_YELLOW, LIGHT_RED, LIGHT_GREEN, LIGHT_BLUE, LIGHT_LED_1, LIGHT_LED_2, LIGHT_LED_3};

#if MOMENTARY_BUTTONS_COUNT == 3
  const uint8_t buttons_momentary_indexed [] = { BUTTON_MOMENTARY_0, BUTTON_MOMENTARY_1, BUTTON_MOMENTARY_2};
#else
  const uint8_t buttons_momentary_indexed [] = {BUTTON_MOMENTARY_0, BUTTON_MOMENTARY_1, BUTTON_MOMENTARY_2, BUTTON_MOMENTARY_3};

#endif

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

class Apps{

  public:

    Apps();

    BinaryInput* binaryInputs;
    DisplayManagement* ledDisp;
    Buzzer* buzzer;
    Potentio* potentio;
    
    void setPeripherals( BinaryInput* binaryInput, Potentio* potentio, DisplayManagement* ledDisp, Buzzer* buzzer);
    // void setBuffers(char* textBuf, char*  scrollBuf);
    //void setBuffers(char* textBuf);
    // void test();
    
    void setDefaultMode();
    void appSelector(bool init, uint8_t selector);
    bool init_app(bool init, uint8_t selector);
    //void modeScroll(bool init);
    void modeSimpleButtonsAndLights(bool init);
    void modeCountingLettersAndChars(bool init);
    void modeSoundSong(bool init);
    void modeSoundNotes(bool init);
    void draw(bool init);
    void movieAnimationMode(bool init);
    uint16_t _animationGetStartByte(uint8_t number);
    void modeSingleSegmentManipulation(bool init);
    void modeGeiger(bool init);
    void modeSequencer(bool init);
    void modeMetronome(bool init);
    void modeSimon(bool init);
    void modeReactionGame(bool init);
    void tiltSwitchTest(bool init);
    void modeButtonDebug(bool init);
    void miniMultiTimer(bool init);
    void modeDiceRoll(bool init);
    
    uint32_t fadeInList(uint8_t step, uint8_t length, uint32_t startScreen, uint8_t* shuffledSequence);    
    // void fadeInList(uint32_t* movie, uint8_t length, uint32_t startScreen); //old school used too much ram for the movie buffer.
    void shuffle(uint8_t* list, uint8_t length);

  private:
    DataPlayer dataPlayer;
    
    #ifdef ENABLE_MULTITIMER
    MiniMultiTimer multiTimer;
    #endif

    int16_t nextStepRotate(int16_t counter, bool countUpElseDown, int16_t minValue, int16_t maxValue);
    void randomModeDisplay(bool forReal);
    //void _eepromWriteByteIfChanged(uint8_t* address , uint8_t value);
    
    SuperTimer generalTimer;
    SuperTimer generalTimer2;
    uint32_t displayAllSegments;

    //reused variables per app
    bool general_boolean;
    bool general_boolean2;
    int16_t counter;
    int16_t counter2;
    long counter3;
    uint16_t counter4;
    uint8_t counter5;

    enum {
      bytes_list_bufsize = 52,
    };
    uint8_t bytes_list[bytes_list_bufsize];
    
    uint8_t array_4_bytes [4];

    #ifdef ENABLE_REACTION_APP
    // reaction
    enum ReactionGameState:uint8_t {
    // enum ReactionGameState{
        reactionWaitForStart,
        reactionNewGame,
        reactionNewTurn,
        reactionPlaying,
        reactionJustDied,
        reactionFinished,
        reactionMultiNewTurn,
        reactionMultiPlaying,

        
    };
    ReactionGameState reactionGameState;
    #endif

    enum DiceRollState:uint8_t {
      dicerollIdle,
      dicerollShowResult,
      dicerollRolling,
      dicerollRollingEnd,
      dicerollAutoRollDelay,
    };
    DiceRollState diceRollState;



    // enum SettingsState:uint8_t{
    //   settingNormal,
    //   settingSound,
    //   settingButtons,
    //   settingEepromReset
    // };
    #ifdef SIMON_APP
    // simon
    enum SimonState:uint8_t {
    // enum SimonState{
        simonWaitForNewGame,
        simonNewGame,
        simonNewLevel,
        simonPlaySequence,
        simonUserRepeats,
    };
    SimonState simonState;

    #endif

    char textBuf[6];
    uint8_t decimalPoints;  // segment 4 = bit 3, ....   00043210 (segment number)
    uint8_t lights;
    
    bool app_init_mode;
   
};

#endif 
