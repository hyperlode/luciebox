#include "Apps.h"
#include "Arduino.h"
#include "PretbakSettings.h"

Apps::Apps(){
//  
};

void Apps::setPeripherals( BinaryInput binaryInputs[], DisplayManagement* ledDisp, Buzzer* buzzer){
  this->buzzer = buzzer;
  this->binaryInputs = binaryInputs;
  this->ledDisp = ledDisp;
}

void Apps::test(){
  Serial.println("okkekekeke");
  //(*this->buzzer).programBuzzerRoll(45);
  (*this->ledDisp).showNumber(666);
//  if (*this->binaryInputs)[BUTTON_MOMENTARY_RED].getValue()){
  if (binaryInputs[BUTTON_MOMENTARY_RED].getValue()){
    buzzer->programBuzzerRoll(45);
  }
  
}

//void Apps::modeScroll(bool init){
//  
//  // display scroll mode
//  
//  if (init){
//    // display scroll mode
////    scrollBuf[0]='L';
////    scrollBuf[1]='U';
////    scrollBuf[2]='C';
////    scrollBuf[3]='I';
////    scrollBuf[4]='E';
////    scrollBuf[5]=' ';
////    scrollBuf[6]='B';
////    scrollBuf[7]='A';
////    scrollBuf[8]='B';
////    scrollBuf[9]='Y';
////
//
//    scrollBuf[0]='H';
//    scrollBuf[1]='A';
//    scrollBuf[2]='P';
//    scrollBuf[3]='P';
//    scrollBuf[4]='Y';
//    scrollBuf[5]=' ';
//    scrollBuf[6]='B';
//    scrollBuf[7]='D';
//    scrollBuf[8]='A';
//    scrollBuf[9]='Y';
//    scrollBuf[10]=' ';
//    scrollBuf[11]='B';
//    scrollBuf[12]='R';
//    scrollBuf[13]='A';
//    scrollBuf[14]='M';
//    scrollBuf[15]='Z';
//    scrollBuf[16]='Y';
//    //scrollBuf[10]='/0';
//    ledDisp.dispHandlerWithScroll(scrollBuf, true, false);
//      
//  }
//  
//  if (!binaryInputs[BUTTON_MOMENTARY_BLUE].getValue()){
//    ledDisp.doScroll();
//  }
//
//  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
//    ledDisp.setScrollSpeed((long)potentio_value_stable);
//  }else{
//    ledDisp.setBrightness((byte)(potentio_value_stable/20),false);
//  }
//  
//}
//void Apps::modeSimpleButtonsAndLights(){
//  
//      // simple repetitive, predictive mode.
//      // each button triggers its corresponding light. 
//      // potentio sets display brightness
//      // no buzzer
//      // display lights up a segment for each button.
//      bool aButtonIsPressed = false;
//
//      //delete all content from screen.
//      ledDisp.setBlankDisplay();      
//      
//      lights = 0b00000000; //reset before switch enquiry
//      if (binaryInputs[BUTTON_MOMENTARY_RED].getValue()){
//        lights|= 1<<LIGHT_RED;
//        aButtonIsPressed = true;
//      }
//      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getValue()){
//        lights|= 1<<LIGHT_BLUE;
//        aButtonIsPressed = true;
//      }
//      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getValue()){
//        lights|= 1<<LIGHT_GREEN;
//        aButtonIsPressed = true;
//      }
//      
//
//      if (aButtonIsPressed){
//        textBuf[1]='8';
//        textBuf[2]='8';
//        textBuf[3]='8';
//        textBuf[4]='8';        
//      }else{
//        //display
//        textBuf[1]='-';
//        textBuf[2]='-';
//        textBuf[3]='-';
//        textBuf[4]='-';
//      }
//      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
//        lights|= 1<<LIGHT_LED_1;
//      }else{
//        textBuf[1]=' ';
//      }
//      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
//        lights|= 1<<LIGHT_LED_2;
//      }else{
//        textBuf[2]=' ';
//      }
//      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
//        lights|= 1<<LIGHT_LED_3;
//        aButtonIsPressed = true;
//      }else{
//        textBuf[3]=' ';
//      }
//      if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){
//        lights|= 1<<LIGHT_YELLOW;
//        aButtonIsPressed = true;
//      }else{
//        textBuf[4]=' ';
//      }
//      
//      ledDisp.displayHandler(textBuf);
//      
//      ledDisp.SetLedArray(lights);
//      ledDisp.setBrightness((byte)(potentio_value_stable/20),false);
//}
//
//void Apps::modeCountingLettersAndChars(bool init){
//        //counting mode: numbers and letters.
//      bool aButtonIsPressed = false;
//
//      if (init){
//        aButtonIsPressed = true;
//      }
//      
//      numberElseAlphabethMode = binaryInputs[BUTTON_LATCHING_YELLOW].getValue();
//  
//      if (binaryInputs[BUTTON_LATCHING_YELLOW].getValueChanged()){
//        aButtonIsPressed = true;
//      }
//    
//      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
//        counter++;
//        aButtonIsPressed = true;
//      }
//       
//      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
//        counter--;
//        aButtonIsPressed = true;
//      } 
//      if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
//        if (numberElseAlphabethMode){
//          counter = 0;
//        }else{
//          counter = 1;
//        }
//        aButtonIsPressed = true;
//      }
//      
//      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getEdgeUp()){
//        
//        generalTimer.setInitTimeMillis(-1000);
//        generalTimer.start();
////        Serial.println(generalTimer.getTimeMillis());
////        Serial.println(generalTimer.getTimeIsNegative());
//      }  
//      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getEdgeDown()){
//        generalTimer.pause();        
//        //generalTimer.setInitTimeMillis(-10000);
//        //generalTimer.start();
////        Serial.println(generalTimer.getTimeMillis());
////        Serial.println(generalTimer.getTimeIsNegative());
//      }  
////      
//      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue() && !generalTimer.getTimeIsNegative()){
//        if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
//          counter++;  
//        }else{
//          counter--;
//        }
//        
//        generalTimer.start();
//        aButtonIsPressed = true;
//      }
//      
//      //potentio behaviour
//      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
//        if (numberElseAlphabethMode){
//          counter = (int16_t)potentio_value/10;
//        }else{
//          counter = (int16_t)potentio_value/39; //1024 to 26 letters.
//        }
//        aButtonIsPressed = true;
//      }
//      else if(binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
//        // if autocounter is on.
//        
//        generalTimer.setInitTimeMillis(-100 * (long)(potentio_value_stable/10)); //divided by ten, this way, we can set the timer very accurately as displayed on screen when big red is pressed. *100ms
//        if (potentio_value_stable_changed){
//          generalTimer.start();
//        }
//      }
//      
//      //only do the characters of the alphabet in lettermode.
//      if (!numberElseAlphabethMode){
//        if (counter > 26){
//            counter = 1;
//        }
//        if (counter < 1){
//          counter = 26;
//        }
//      }else{  
//        //no negative numbers yet for little lucie
//        if (counter < 0){
//          counter = 0;
//        }
//      }
//
//      if (aButtonIsPressed){
//        //Serial.println(counter);
//        if (numberElseAlphabethMode){
//          ledDisp.showNumber(counter);
//        }else{
//          ledDisp.showNumberAsChars(counter);
//        }
//      }
//}
//
//void Apps::modeSoundSong(bool init){
//  if (init){
//    buzzer.loadBuzzerTrack(song_happy_dryer);
//    buzzer.setSpeedRatio((float)2);
////    buzzer.loadBuzzerTrack(song_unhappy_dryer);
//  }
//
//  if (potentio_value_stable_changed){
//    buzzer.setSpeedRatio((float)(potentio_value_stable) / 256 );
//  }
//  
//  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
//    
//    if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){
//     if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
//        buzzer.loadBuzzerTrack(song_unhappy_dryer);
//      }
//      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
//        //buzzer.loadBuzzerTrack(song_unhappy_dryer);
//      }
//      if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
//        buzzer.loadBuzzerTrack(song_retreat );
//      }
//    }else{
//      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
//        buzzer.loadBuzzerTrack(song_happy_dryer);
//      }
//      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
//        buzzer.loadBuzzerTrack(song_lang_zal_ze_leven);
//      }
//      if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
//        buzzer.loadBuzzerTrack(song_attack );
//      }
//    }
//  }else{
//    //scales
//     if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){
//      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
//        buzzer.loadBuzzerTrack(scale_major);
//      }
//      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
//        //buzzer.loadBuzzerTrack(song_unhappy_dryer);
//      }
//      if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
//        //buzzer.loadBuzzerTrack(song_attack );
//      }
//    }else{
//      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
//        buzzer.loadBuzzerTrack(scale_major_reversed);
//      }
//      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
//        //buzzer.loadBuzzerTrack(song_unhappy_dryer);
//      }
//      if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
//        //buzzer.loadBuzzerTrack(song_retreat );
//      }
//    }
//  
//  }
//
//
//}
//
//void Apps::modeSoundNotes(){
//  //buzzer with buzzer roll (notes).
//      
//      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
//        if (potentio_value_stable_changed){
//          //buzzer.programBuzzerRoll(potentio_value_stable /4);;
//           allNotesIndex = potentio_value /4;
//           buzzer.programBuzzerRoll(allNotesIndex);
//        }
//          
//        if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
//          buzzer.programBuzzerRoll(allNotesIndex);
//          allNotesIndex--;
//        }
//        if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
//          buzzer.programBuzzerRoll(allNotesIndex);
//        }
//        
//        if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
//          buzzer.programBuzzerRoll(allNotesIndex);
//          allNotesIndex++;
//        }
//        ledDisp.showNumber(allNotesIndex);
//      }else{
//        if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
//          ledDisp.showNumber(buzzer.addRandomSoundToRoll(223, 235));
//          //0 -> 63 short
//        }
//        if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
//          ledDisp.showNumber(buzzer.addRandomSoundToRoll(160, 223));
//        }
//        if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
//          ledDisp.showNumber(buzzer.addRandomSoundToRoll(97, 160));
//        }  
//      }
//}
//
//void Apps::modeSingleSegmentManipulation(bool init){
//
//  if (init){
//
//    animation_step = 0;
//  }
//  if (potentio_value_stable_changed){
//    //uint8_t tmp = (uint8_t) (potentio_value_stable/4);
//    //ledDisp.SetSingleDigit(tmp, 2);  
//    
//  }
//  
//  //ledDisp.displayHandler(textBuf);  
////  ledDisp.SetSingleDigit(B11111111, 1);  
////  ledDisp.SetSingleDigit((uint8_t) (potentio_value_stable/4), 3);  
//  
//  if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
//    ledDisp.SetSingleDigit(pgm_read_byte_near(disp_digit_animate + animation_step),1);  
//    //ledDisp.SetSingleDigit(pgm_read_byte_near(disp_digit_animate_double + animation_step),2);  
//    //ledDisp.SetSingleDigit(pgm_read_byte_near(disp_digit_animate_inverted + animation_step),3);  
//    ledDisp.SetSingleDigit(pgm_read_byte_near(disp_digit_animate + animation_step),4);  
//
//     (animation_step>=5)?animation_step=0:animation_step++;
////     Serial.println(animation_step);
////     Serial.println(pgm_read_byte_near(disp_digit_animate + animation_step));
//  }
//  
//  
//}
//
//void Apps::modeGeiger(bool init){
//
//  if (init){
//      textBuf[4]=' ';
//      geiger_counter = 0;
//  }
//  
//    
//  //play tick. 
//  //wait random time.
//  //X = - log(1 - Y)/ K   with Y a random value ( 0<Y<1) and K a constant ?
//  long r = random(0, 1024)*random(0, 1024); 
//  //long r = random(0, 1024);
//  r = r*r;
//
//  if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){
//      
//    if (r > (long)potentio_value*1024){
//      tone(PIN_BUZZER, (unsigned int)50, 10);
//    
//      geiger_counter++;
//      ledDisp.SetSingleDigit(geiger_counter, 1);  
//      ledDisp.SetSingleDigit(pgm_read_byte_near(disp_digit_animate + animation_step),4);  
//  
//      (animation_step>=5)?animation_step=0:animation_step++;
//      
//    }
//    
//  }else{
//    if (r > (long)potentio_value*1024){
//  //    buzzer.programBuzzerRoll(1); //not beep but "puck"
//      tone(PIN_BUZZER, (unsigned int)50, 10);
//      textBuf[1]='?';
//      textBuf[2]='?';
//      textBuf[3]='?';
//      textBuf[4]='?';
//      
//    }else{
//      textBuf[1]=' ';
//      textBuf[2]=' ';
//      textBuf[3]=' ';
//      textBuf[4]=' ';
//    }
//    ledDisp.displayHandler(textBuf);  
//  }
//}
//
//void Apps::fullScreenMovie(bool init){
//  bool nextStep = 0;
//  if (init){
//    counter = 0;
//    counter2 = 0;
//    game_x_pos = 0;
//    game_y_pos = 0;
//    animation_speed.setInitTimeMillis((long)potentio_value_stable * -1);
//    animation_speed.start();
//    nextStep = true;
//  }
//  
//  bool direction1 = true;
//  if(binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
//    direction1 = false;
//  }  
//  
//  bool direction2 = true;
//  if(binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
//    direction2 = false;
//  }
//  
// // if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){ 
//    //movie mode
//    if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){
//
//      if (potentio_value_stable_changed){
//        animation_speed.setInitTimeMillis((long)potentio_value_stable * -1);
////        animation_speed.start(); //during turning it pauses because of the continuous restarting.
//      }
//      
//      if (!animation_speed.getTimeIsNegative()){
//        nextStep = true;
//        animation_speed.start();
//      }
//    }
//    
//    if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
//      nextStep = true;
//    }
//    
//    if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
//      counter = nextStepRotate(counter, !direction1, 0, 11);
//      counter2 = counter;
//      nextStep = true;
//    }
//
//    if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
//      counter2 = nextStepRotate(counter2, !direction2, 0, 11);
//      nextStep = true;
//    }
//    
//    if (nextStep){
//      uint32_t screen = 0;
//      for (uint8_t i=0;i<4;i++){
//        screen |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + counter*4 + (i)) << (8*i); 
//        screen |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + counter2*4 + (i)) << (8*i); 
//      }
//      ledDisp.SetFourDigits(screen);
//      
//      counter = nextStepRotate(counter, direction1, 0, 11);
//      counter2 = nextStepRotate(counter2, direction2, 0, 11);
//    }
//    
////  }else{
////    //interactive mode
////    // game: x,y 0,0 = bottom left 
////    if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
////      
////    }
////    
////
////  }
//  
//}
//
//int16_t nextStepRotate(int16_t counter, bool countUpElseDown, int16_t minValue, int16_t maxValue){
//
//     if (countUpElseDown){
//        counter++;
//     }else{
//        counter--;
//     }
//
//     if (counter > maxValue){
//        counter = minValue;
//     }
//     if (counter < minValue){
//        counter = maxValue;
//     }
//     return counter;
//}
//
//void Apps::gameButtonInteraction(bool init){
//  //yellow button active at start: yellow button is also a guess button
//  // big red active: timed game
//  // small red right active: time progressively shorter as game advances
//  // small red left active: play by sound.
//  
//  bool getNewNumber = false;
//  bool isDead = false;
//  
//  if (init){
//    counter = 0; // holds score
//    //randomSeed(123456);
//    getNewNumber = true;
//    generalTimer.setInitTimeMillis(0);
//
//    initTime = (long)potentio_value_stable * -1; // only set the default inittime at selecting the game. If multiple games are played, init time stays the same.
//    animation_speed.setInitTimeMillis(initTime);
//    
//    counter2 = 0;
//    screenPersistenceOfVision = 0;
//    reactionGameYellowButtonIsIncluded = binaryInputs[BUTTON_LATCHING_YELLOW].getValue();
//    
//    for (uint8_t i=0;i<4;i++){
//      selectedSounds[i] = (uint8_t)random(100, 114);
//    }
//  }
//
//  if (!(binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue() && binaryInputs[BUTTON_LATCHING_BIG_RED].getValue() )){
//     //always show unless in soundmode->competition
//    ledDisp.setDecimalPoint(true, reactionGameTarget+1);
//  }
//
//  if (!animation_speed.getTimeIsNegative()){
//    // game timing animation update.
//    for (uint8_t i=0;i<4;i++){
//      screenPersistenceOfVision |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + counter2*4 + (i)) << (8*i); 
//    }
//    ledDisp.SetFourDigits(screenPersistenceOfVision);
//     
//    counter2 = nextStepRotate(counter2, true, 0, 12);
//    
//    animation_speed.reset();
//    if (counter2 == 12){
//      counter2 = 0;
//      screenPersistenceOfVision=0;    
//      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue() ){
//        // timed out.
//        isDead = true;
//      }else{
//        // time out not enabled.
//        animation_speed.start();
//      }
//    }else{
//      animation_speed.start();
//    }   
//  }
//  
//  if (!generalTimer.getTimeIsNegative()){
//    //end of display high score, next number
//    counter = 0;
//    getNewNumber = true;
//    generalTimer.reset();
//    animation_speed.setInitTimeMillis(initTime);
//       
//  }else if(generalTimer.getIsStarted()){
//     //do nothing.  wait for display high score is finished.
//     if (generalTimer.getInFirstGivenHundredsPartOfSecond(500)){
//        ledDisp.setBlankDisplay(); //make high score blink
//     }else{
//        ledDisp.showNumber(counter ); //score display. Leave at beginning, to display high score blinking.
//     }
//  }else if (binaryInputs[buttons_indexed[reactionGameTarget]].getEdgeUp() ||
//      (binaryInputs[BUTTON_LATCHING_YELLOW].getValueChanged()&& reactionGameTarget == 0))
//  {
//      //right button
//      counter++;
//      getNewNumber = true;
//      if (!binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
//         //play by sounds
//        buzzer.programBuzzerRoll(C7_8);
//      }
//      
//  }else if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()  ||
//      binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()  ||
//      binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp() ||
//      binaryInputs[BUTTON_LATCHING_YELLOW].getValueChanged())
//  {
//    //wrong button
//      isDead = true;
//  }
//  
//  // ledDisp.SetSingleDigit(&counter,3);
//  // ledDisp.SetSingleDigit(*(&game_random+counter),3);
//  if (isDead){
//    if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
//      generalTimer.setInitTimeMillis(-2000);
//      generalTimer.start();
//      //play by sounds
//      for (uint8_t i=reactionGameYellowButtonIsIncluded?0:1;i<4;i++){
//        buzzer.programBuzzerRoll(selectedSounds[i]+128);
//        buzzer.programBuzzerRoll(rest_1);
//      }
//       
//    }else{
//      generalTimer.setInitTimeMillis(-2000);
//      generalTimer.start();
//      buzzer.programBuzzerRoll(F4_1);  
//      buzzer.programBuzzerRoll(F4_1);  
//      buzzer.programBuzzerRoll(F4_1);  
//      buzzer.programBuzzerRoll(F4_1);  
//    }
//  }
//  
//  if (getNewNumber){
//    ledDisp.setBlankDisplay();
//    lights = 0b00000000; //reset before switch enquiry
//    
//    reactionGameTarget = (uint8_t)random(reactionGameYellowButtonIsIncluded?0:1, 4);
//    
//    screenPersistenceOfVision = 0; //reset animation graphics screen
//    counter2= 0; //reset animation step
//
//    if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
//       //play by sounds
//       buzzer.programBuzzerRoll(selectedSounds[reactionGameTarget]);
//    }else{
//      lights |= 1<<lights_indexed[reactionGameTarget];
//      
//    }
//    ledDisp.SetLedArray(lights); 
//    if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
//      animation_speed.setInitTimeMillis(animation_speed.getInitTimeMillis()*0.99);
//    }
//        
//    animation_speed.start();    
//  }
//}
