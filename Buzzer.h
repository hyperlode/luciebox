
#ifndef Buzzer_h
#define Buzzer_h
#include "Arduino.h" 



#define BUZZER_MINUTE_BEEP_FREQUENCY 4000 //beep at each minute with this frequency
#define BUZZER_MINUTEBEEP_DURATION_MILLIS 20
#define BUZZER_MINUTEBEEP_DURATION_MILLIS 20
#define BUZZER_ALARM_FREQUENCY 2000
#define BUZZER_ALARM_DURATION_MILLIS 1000000

#define BUZZER_NEXTPLAYER_BEEP_FREQUENCY 500
#define BUZZER_NEXTPLAYER_DURATION_MILLIS 20
#define BUZZER_NEXTPLAYER_MULTIPLEBEEP_INTERVAL_MILLIS 100

#define BUZZER_ROLLVALUE_BUTTONPRESS 
#define BUZZER_ROLLVALUE_PAUSE
#define BUZZER_ROLLVALUE_UNPAUSE
#define BUZZER_ROLLVALUE_STARTGAME
#define BUZZER_ROLLVALUE_ENDGAME
//#define BUZZER_ROLLVALUE_


#define BUZZER_ROLL_LENGTH 20
#define BUZZER_ROLL_EMPTY_SLOT 255
// #define BUZZER_ROLL_EIGHTNOTE_DURATION_MILLIS 63
#define BUZZER_ROLL_EIGHTNOTE_DURATION_MILLIS 15
#define BUZZER_ROLL_BASE_FREQUENCY 220
#define BUZZER_ROLL_SONG_STOPVALUE 255
//
//const uint8_t song_pause [] PROGMEM = {39,126,170,126,BUZZER_ROLL_SONG_STOPVALUE};
//// const uint8_t song_simple_beep [] PROGMEM = {51,63,BUZZER_ROLL_SONG_STOPVALUE};
//const uint8_t song_simple_beep [] PROGMEM = {51,126,BUZZER_ROLL_SONG_STOPVALUE};
//const uint8_t song_unpause [] PROGMEM = {44,126,165,126,BUZZER_ROLL_SONG_STOPVALUE};
//
//const uint8_t song_nextPlayer_Single [] PROGMEM = {49,63,BUZZER_ROLL_SONG_STOPVALUE};
//
//const uint8_t song_alarm [] PROGMEM = {49,63,49,63,49,63,49,63,49,63,BUZZER_ROLL_SONG_STOPVALUE};
//
//const uint8_t song_nextPlayerSequence [] PROGMEM = {228,126,105,126,228,126,228,126,102,126,49,126,230,126,41,126,227};


class Buzzer{
  public:
    Buzzer();

    void setPin(uint8_t pin);
    void loadBuzzerTrack(const uint8_t* seq);
    void programBuzzerRoll(uint8_t sound);
    void addRandomSoundToRoll();
    void doBuzzerRoll();
    uint8_t getNextBuzzerRollSlot ( bool getNextEmptySlot);
    uint8_t getBuzzerRollFull();
    uint8_t getBuzzerRollEmpty(); 

  private:
    uint8_t pin;
    uint8_t buzzerRoll[BUZZER_ROLL_LENGTH];
    uint8_t playSlotCounter;
    uint8_t programSlotCounter;
    long soundFinishedTimeMillis;
};
#endif

//
//#ifdef BUZZER_ENABLED
//    //beeps
//
//    //byte numberOfBeepsToBePlayed;
//    //unsigned long beepsWatchMillis;
//
//    
//#endif
//
//#ifdef BUZZER_ENABLED
//    byte soundBehaviour;
//#endif
//



