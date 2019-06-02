#include "Buzzer.h"
#include "Arduino.h"

Buzzer::Buzzer(){
      //constructor
      //init buzzerRoll
      
      for (uint8_t i = 0; i < BUZZER_ROLL_LENGTH; i++) {
        this->buzzerRoll[i] = BUZZER_ROLL_EMPTY_SLOT;
      }
      this->soundFinishedTimeMillis = 0;
      this->speedScale = 1;
}

void Buzzer::setPin(uint8_t pin){
  this->pin = pin;
  pinMode(this->pin, OUTPUT);
}

void Buzzer::loadBuzzerTrack(const uint8_t* seq) {
      //load a whole track in the buzzer roll
      //load all bytes from sequence in buffer.
      uint8_t i = 0;
      while (pgm_read_byte_near(seq + i) != BUZZER_ROLL_SONG_STOPVALUE and i < BUZZER_ROLL_LENGTH) {
        programBuzzerRoll(pgm_read_byte_near(seq + i)); //progmem = special command so arrays are read out of flash directly without being loaded in eeprom.
        i++;
      }
    }

void Buzzer::programBuzzerRoll(uint8_t sound) {
  //load one sound to next slot in roll
  
  //sound:
  //0 = empty slot
  //+ 1-62 = note (1 = 110Hz A , and going up 1/12 of an octave each number
  //+ 63, 126, 189, 252 = duration = 1/8, 1/4, 1/2, 1
  //program in next available slot AFTER playSlotCounter

  //A0sharp 1/2 = 191
  //1/8 stop = 63

  this->programSlotCounter = getNextBuzzerRollSlot( true);
  this->buzzerRoll[this->programSlotCounter] = sound;

  //http://members.efn.org/~qehn/global/building/cents.htm
  //F = {[(2)^1/12]^n} * 220 Hz //220Hz for A 440 , 880 .... for other octaves
}

uint8_t Buzzer::addRandomSoundToRoll(uint8_t lowest, uint8_t highest){
  if (getBuzzerRollEmpty()) {
    uint8_t r = random(lowest, highest); //223, 235
    programBuzzerRoll(r);
    //programBuzzerRoll(random(1,60));
    return r;
  }
  return 0;
}

void Buzzer::doBuzzerRoll() {
  //play all the sounds in the array.
  //array of 16 bytes
  //0 stands for free and programmable
  //one playSlotCounter

  if ( millis() > this->soundFinishedTimeMillis) {
    //erase previous slot
    this->buzzerRoll[this->playSlotCounter] = BUZZER_ROLL_EMPTY_SLOT;

    //move active slot
    this->playSlotCounter = getNextBuzzerRollSlot( false);

    if ( this->buzzerRoll[this->playSlotCounter] != BUZZER_ROLL_EMPTY_SLOT) {
      //uint8_t noteNumber = buzzerRoll[this->playSlotCounter]%63;
      //F = {[(2)^1/12]^n} * 220 Hz //220Hz for A 440 , 880 .... for other octaves
      float freq = 1;


      for (uint8_t i = 1; i < buzzerRoll[this->playSlotCounter] % 64; i++) {
        //same result as: (but 2K less memory!!)
        //freq = pow(1.059463,(buzzerRoll[this->playSlotCounter])-1 % 63); 
        freq *= 1.059463;
      }
      
      if (buzzerRoll[this->playSlotCounter] % 64 != 0) { //no sound, stop. (rust)
        freq *= BUZZER_ROLL_BASE_FREQUENCY; //frequency
      }
      tone(this->pin, (unsigned int)freq , (unsigned long)(this->speedScale * BUZZER_ROLL_EIGHTNOTE_DURATION_MILLIS * (B00000001 << buzzerRoll[this->playSlotCounter] / 63))); //duration, number is exponent of 2.

      //tone(PIN_BUZZER, (unsigned int)freq , 50); //duration, number is exponent of 2.
      //tone(PIN_BUZZER, 3000 , (unsigned long)(BUZZER_ROLL_EIGHTNOTE_DURATION_MILLIS * (B00000001 << buzzerRoll[this->playSlotCounter]/63))); //duration, number is exponent of 2.

      //Serial.println((unsigned long)(BUZZER_ROLL_EIGHTNOTE_DURATION_MILLIS * (B00000001 << buzzerRoll[this->playSlotCounter]/63)));
      //Serial.println((unsigned long)(BUZZER_ROLL_EIGHTNOTE_DURATION_MILLIS * (B00000001 << buzzerRoll[this->playSlotCounter]/63)));

      this->soundFinishedTimeMillis = millis() + (unsigned long)(this->speedScale * BUZZER_ROLL_EIGHTNOTE_DURATION_MILLIS * (B00000001 << buzzerRoll[this->playSlotCounter] / 63));
    }
  }
}

void Buzzer::setSpeedRatio(float speedMultiplier){
  // argument is a long: 1 is normal speed, 0.5 is half, 2 is double, ....
  this->speedScale = speedMultiplier;
}

uint8_t Buzzer::getNextBuzzerRollSlot ( bool getNextEmptySlot) {
  //returns value of next slot
  //if argument true, will search for next free slot. If no free slots, returns current slot.
  uint8_t slot = this->playSlotCounter;
  do {
    slot >= BUZZER_ROLL_LENGTH - 1 ? slot = 0: slot ++;    
  } while ( getNextEmptySlot
            && buzzerRoll[slot] != BUZZER_ROLL_EMPTY_SLOT
            && this->playSlotCounter != slot 
          );
  return slot;
}

void Buzzer::cleanBuzzerRoll(){
  //all slots empty
  for(uint8_t i=0; i<BUZZER_ROLL_LENGTH; i++){
    buzzerRoll[i] = BUZZER_ROLL_EMPTY_SLOT;
  }
}

void Buzzer::buzzerOff(){
  //erase contents of buzzerRoll and switch off.
  this->cleanBuzzerRoll();
  noTone(this->pin);
}

void Buzzer::buzzerOn(uint16_t freq){
  tone(this->pin, freq);
}

uint8_t Buzzer::getBuzzerRollFull() {
  //check if there are free slots.
  return (getNextBuzzerRollSlot(true) == this->playSlotCounter);
}
uint8_t Buzzer::getBuzzerRollEmpty() {
  int sum = 0;
  for (uint8_t i = 0; i < BUZZER_ROLL_LENGTH ; i++) {
    sum += buzzerRoll[i];
  }
  return sum == BUZZER_ROLL_LENGTH * BUZZER_ROLL_EMPTY_SLOT;
}

/// ---------------------------
/// ---------------------------
/// ---------------------------
/// ---------------------------
/// ---------------------------
/// ---------------------------


//Buzzer::playSound(uint8_t frequncey, uint8_t 
//
//Buzzer::getSoundBehaviourreeer(){
//
//  //0 = off
//      //1=alarm
//      //2=notifications only (no time beeping in game.)
//      //3=all
//      //4=fancy song mode.
//
//      #ifdef BUZZER_ENABLED
//      if (game.getSound() == 4 ) {
//        game.loadBuzzerTrack(song_happy_dryer);
//      } else if (game.getSound() > 0) {
//        game.loadBuzzerTrack(song_simple_beep);
//      }
//#endif
//
//      if (getSound() > 1 ) {
//        loadBuzzerTrack(song_simple_beep);
//      }
//
//
//      if (getSound() > 2 ) {
//        programBuzzerRoll(213);
//        programBuzzerRoll(213);
//        programBuzzerRoll(63);
//      }
//
//        if (getSound() == 4) {
//          //fancy
//          // loadBuzzerTrack(song_gameStart_attack);
//          loadBuzzerTrack(song_simple_beep);
//        } else if (getSound() > 1) {
//          //normal
//          loadBuzzerTrack(song_simple_beep);
//        }
//if (getSound() > 2 ) {
//        programBuzzerRoll(205);
//        programBuzzerRoll(205);
//        programBuzzerRoll(205);
//        programBuzzerRoll(63);
//
//      }
//
//
//}
//
//      //sound management
//      doBuzzerRoll();
//
//      if ( (getSound() == 1 && ( getStatePauseEndOfPlayer() || getStatePauseEndOfGame() || getStatePauseNextPlayer() ))
//           || (getNumberOfPlayers() == 1 && getMenuEasyMode() && getStatePauseEndOfGame() && getSound() > 0) //if one player in easy mode: default = alarm
//         ) {
//        //beep alarm! at all pauses if not manual...
//
//        //tone(PIN_BUZZER, BUZZER_ALARM_FREQUENCY , BUZZER_ALARM_DURATION_MILLIS);
//        /////int randomPlayer = random(0,this->numberOfPlayers);
//        //tone(PIN_BUZZER, random(1000,5000) , 15);
//        //tone(PIN_BUZZER, random(5000,8000) , 30);
//        //tone(PIN_BUZZER, random(1000,8000) , 100);
//        if (getBuzzerRollEmpty()) {
//          programBuzzerRoll(random(223, 235));
//          //programBuzzerRoll(random(1,60));
//        }
//      }
//

//
//
////manage total time chrono
//#ifdef BUZZER_ENABLED
//      //do total time minute beeps (timer ON reminder) when not in menu mode and when not in run state.
//      if (getSound() > 1 && (!getStateRunning() || getStateRunning() && getStatePauseManual()) && !getStateEnded() && getHasScope()) {
//        //(!getStateRunning() || getStateRunning() && getStatePauseManual()) : only let beep when in manual pause (no pausetimer active) during running of game
//        bool inFirstHalfOfMinute = (millis() - this->millisForTotalGameTime ) % 60000 < 30000; //beeps every minute
//        //bool inFirstHalfOfMinute = (millis() - millisForTotalGameTime )%1000 <500; //beeps every second
//        if (!totalGameTimeBeepEdgeInFirstHalfOfMinuteDetection && inFirstHalfOfMinute && millis() > 1000 ) {
//          //millis() > 1000 : dont beep at startup , this is managed by the startup sound.
//          loadBuzzerTrack(song_simple_beep);
//          //loadBuzzerTrack(song_unpause);
//        }
//        totalGameTimeBeepEdgeInFirstHalfOfMinuteDetection = inFirstHalfOfMinute;
//
//        ////////////////////////
//        // beep every minute during manual pause
//        // manual pause without any timer showing, but we still start the pauseTimer to trigger the beeping every minute of pause (this beeping is good to have a sound, otherwise the timer will be silent forever (until the batteries drain) during pause and not unpaused.
//        ////////////////////////
//
//
//      }
//#endif
//
//
//beep every minute
//#ifdef BUZZER_ENABLED
//          //sounds in game beep each minute
//          if ( getSound() > 1 && !pauseTimer.getInFirstGivenHundredsPartOfSecond(PAUSE_BLINKING_HUNDREDS) && pauseTimer.getTimeSeconds() % 60 == 0 ) {
//            loadBuzzerTrack(song_simple_beep);
//          }
//#endif
//
//countdown
//      //pausetimer as countdown timer
//#ifdef BUZZER_ENABLED
//          //countdown pause beeps when nearing zero (and pause will be over.)
//          if ( getSound() > 1 && pauseTimer.getTimeSecondsCountDownTimer() <= PAUSETIMER_NUMBEROFBEEPS_AT_COUNTDOWNEND && pauseTimer.getInitTimeMillis() < 0) {
//            //playSoundIfAllowed();
//            loadBuzzerTrack(song_simple_beep);
//          }
//#endif
//
//
//
//start stop game
//#ifdef BUZZER_ENABLED
//        if (game.getSound() > 1 && game.getMenuEasyMode() &&   (millis() - startGameTriggerTimer) < SUPEREASYMENU_STARTGAME_TRIGGER_TIME_MILLIS) {
//
//          //generate a beep every so much time.
//          //(millis() - startGameTriggerTimer)%40 == 0 &&
//          if ((millis() - beepTriggerTimer) > 30) {
//            beepTriggerTimer = millis();
//
//            if (game.getStateRunning()) {
//              //'game will stop' sound
//              if (millis() - startGameTriggerTimer > 200) {
//                //sounds starts after this time
//                game.programBuzzerRoll( (( SUPEREASYMENU_STARTGAME_TRIGGER_TIME_MILLIS - (millis() - startGameTriggerTimer)) / 60 ) + 20);
//              }
//            } else {
//              //game will start sound
//              game.programBuzzerRoll( ((millis() - startGameTriggerTimer) / 60 ) + 20);
//            }
//          }
//        }
//#endif
//

