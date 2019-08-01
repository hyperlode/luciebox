#include "Apps.h"
#include "Arduino.h"
#include "PretbakSettings.h"

Apps::Apps(){
	//initialize sequencer note only at apps startup.  
	for (uint8_t i=0;i<32;i++){
		this->sequencer_song[i] = C7_8;
	}
};

void Apps::setPeripherals( BinaryInput binaryInputs[], Potentio* potentio, DisplayManagement* ledDisp, Buzzer* buzzer){
  this->buzzer = buzzer;
  this->binaryInputs = binaryInputs;
  this->potentio = potentio;
  this->ledDisp = ledDisp;
}

void Apps::setBuffers(  char*  textBuf, char*  scrollBuf){
   this->textBuf = textBuf;
   this->scrollBuf = scrollBuf;
   this->lights = lights;  
}

void Apps::test(){
  
  //(*this->buzzer).programBuzzerRoll(45);
  (*this->ledDisp).showNumber(666);
//  if (*this->binaryInputs)[BUTTON_MOMENTARY_RED].getValue()){
  if (binaryInputs[BUTTON_MOMENTARY_RED].getValue()){
    buzzer->programBuzzerRoll(45);
    
    #ifdef DEBUG_POTENTIO
    Serial.println(potentio->getValue());
    #endif
    
  }
  
}

void Apps::modeScroll(bool init){
  
  // display scroll mode
  
  if (init){
    // display scroll mode
//    scrollBuf[0]='L';
//    scrollBuf[1]='U';
//    scrollBuf[2]='C';
//    scrollBuf[3]='I';
//    scrollBuf[4]='E';
//    scrollBuf[5]=' ';
//    scrollBuf[6]='B';
//    scrollBuf[7]='A';
//    scrollBuf[8]='B';
//    scrollBuf[9]='Y';
//

    this->scrollBuf[0]='H';
    scrollBuf[1]='A';
    scrollBuf[2]='P';
    scrollBuf[3]='P';
    scrollBuf[4]='Y';
    scrollBuf[5]=' ';
    scrollBuf[6]='B';
    scrollBuf[7]='D';
    scrollBuf[8]='A';
    scrollBuf[9]='Y';
    scrollBuf[10]=' ';
    scrollBuf[11]='B';
    scrollBuf[12]='R';
    scrollBuf[13]='A';
    scrollBuf[14]='M';
    scrollBuf[15]='Z';
    scrollBuf[16]='Y';
    //scrollBuf[10]='/0';
    ledDisp->dispHandlerWithScroll(scrollBuf, true, false);
      
  }
  
  if (!binaryInputs[BUTTON_MOMENTARY_BLUE].getValue()){
    ledDisp->doScroll();
  }

  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
    ledDisp->setScrollSpeed((long)potentio->getValueStable());
  }else{
    ledDisp->setBrightness((byte)(potentio->getValueMapped(0,50)),false);
  }
  
}
void Apps::modeSimpleButtonsAndLights(){
  
      // simple repetitive, predictive mode.
      // each button triggers its corresponding light. 
      // potentio sets display brightness
      // no buzzer
      // display lights up a segment for each button.
      bool updateScreen = false;

      //delete all content from screen.
      ledDisp->setBlankDisplay();      
      
      lights = 0b00000000; //reset before switch enquiry
      if (binaryInputs[BUTTON_MOMENTARY_RED].getValue()){
        lights|= 1<<LIGHT_RED;
        updateScreen = true;
      }
      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getValue()){
        lights|= 1<<LIGHT_BLUE;
        updateScreen = true;
      }
      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getValue()){
        lights|= 1<<LIGHT_GREEN;
        updateScreen = true;
      }
      

      if (updateScreen){
        textBuf[1]='8';
        textBuf[2]='8';
        textBuf[3]='8';
        textBuf[4]='8';        
      }else{
        //display
        textBuf[1]='-';
        textBuf[2]='-';
        textBuf[3]='-';
        textBuf[4]='-';
      }
      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
        lights|= 1<<LIGHT_LED_1;
      }else{
        textBuf[1]=' ';
      }
      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
        lights|= 1<<LIGHT_LED_2;
      }else{
        textBuf[2]=' ';
      }
      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
        lights|= 1<<LIGHT_LED_3;
        updateScreen = true;
      }else{
        textBuf[3]=' ';
      }
      if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){
        lights|= 1<<LIGHT_YELLOW;
        updateScreen = true;
      }else{
        textBuf[4]=' ';
      }
      
      ledDisp->displayHandler(textBuf);
      
      ledDisp->SetLedArray(lights);
      ledDisp->setBrightness((byte)(potentio->getValueMapped(0,50)),false);
}

void Apps::modeCountingLettersAndChars(bool init){
        //counting mode: numbers and letters.
      bool updateScreen = false;

      if (init){
        updateScreen = true;
		generalTimer.setInitTimeMillis(-1000);
      }
      
      numberElseAlphabethMode = !binaryInputs[BUTTON_LATCHING_YELLOW].getValue();
	  
  
      if (binaryInputs[BUTTON_LATCHING_YELLOW].getValueChanged()){
        updateScreen = true;
		if (!numberElseAlphabethMode){
			buzzer->buzzerOff();
			buzzer->setSpeedRatio(4);
			buzzer->loadBuzzerTrack(alphabeth_song);
		}else{
			buzzer->buzzerOff();
		}
      }
    
      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
        counter++;
        updateScreen = true;
      }
       
      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
        counter--;
        updateScreen = true;
      } 
      if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
        if (numberElseAlphabethMode){
          counter = 0;
        }else{
          counter = 1;
        }
        updateScreen = true;
      }
      
	  // auto count
      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getEdgeUp()){
        generalTimer.start();
      }  
      
	  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getEdgeDown()){
        generalTimer.pause();        
      }  
	  
      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue() && !generalTimer.getTimeIsNegative()){
        if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
          counter++;  
        }else{
          counter--;
        }
        generalTimer.start();
        updateScreen = true;
      }
      
      //potentio behaviour
      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
		if (potentio->getValueStableChangedEdge()){
			
			generalTimer.setInitTimeMillis((long)(100 * potentio->getValueMapped(-100, 0))); //divided by ten, this way, we can set the timer very accurately as displayed on screen when big red is pressed. *100ms
			generalTimer.start();
			ledDisp->showNumber( (int16_t)100 * (100 - potentio->getValueMapped(0,100)));
			
		}
	  }else if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
        if (numberElseAlphabethMode){
          counter = (int16_t)(potentio->getValueMapped(0,100));
        }else{
          counter = (int16_t)(potentio->getValueMapped(1,26)); //1024 to 26 letters.
        }
        updateScreen = true;
      }
      
      //only do the characters of the alphabet in lettermode.
      if (!numberElseAlphabethMode){
        if (counter > 26){
            counter = 1;
        }
        if (counter < 1){
          counter = 26;
        }
      }else{  
        //no negative numbers yet for little lucie
        if (counter < 0){
          counter = 0;
        }
      }

      if (updateScreen){
			// when potentio setting init time, it overrules the updateScreen and displays its value. updateScreen erases potentio value display..
			if (numberElseAlphabethMode){
			  ledDisp->showNumber(counter);
			}else{
			  ledDisp->showNumberAsChars(counter);
			}
      }
}

void Apps::modeSoundSong(bool init){
  if (init){
    buzzer->loadBuzzerTrack(song_happy_dryer);
    buzzer->setSpeedRatio((float)2);
  }

  if (potentio->getValueStableChangedEdge()){
	if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
		buzzer->setTranspose((int8_t)(potentio->getValueMapped(-12,12)));
	}else{
		buzzer->setSpeedRatio((float)(potentio->getValue()) / 256 );
	}
  }
  
  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
    // advanced mode scales
    if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){
      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
        buzzer->loadBuzzerTrack(scale_major);
      }
      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
        //buzzer->loadBuzzerTrack(song_unhappy_dryer);
      }
      if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
        //buzzer->loadBuzzerTrack(song_attack );
      }
    }else{
      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
        buzzer->loadBuzzerTrack(scale_major_reversed);
      }
      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
        //buzzer->loadBuzzerTrack(song_unhappy_dryer);
      }
      if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
        //buzzer->loadBuzzerTrack(song_retreat );
      }
    }
  }else{
    // simple mode: songs!
    if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){
      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
        buzzer->loadBuzzerTrack(song_unhappy_dryer);
      }
      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
        buzzer->loadBuzzerTrack(kindeke_douwen);
      }
      if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
        buzzer->loadBuzzerTrack(song_retreat );
      }
    }else{
      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
        buzzer->loadBuzzerTrack(song_happy_dryer);
      }
      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
        buzzer->loadBuzzerTrack(song_lang_zal_ze_leven);
      }
      if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
        buzzer->loadBuzzerTrack(song_attack );
      }
    }
  }
}

void Apps::modeSoundNotes(){
  //buzzer with buzzer roll (notes).
      
      if (!binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
		  
		// if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){  
		  // if (potentio->getValueStableChangedEdge()){
			 
			 // buzzer->buzzerOff();
		     // allNotesIndex = potentio->getValueMapped(0,255);
			 // ledDisp->showNumber(allNotesIndex);
			 // buzzer->programBuzzerRoll(allNotesIndex);
		  // }
		// }else  
		if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
			
			if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
			  buzzer->buzzerOff();
			  ledDisp->showNumber(buzzer->addRandomSoundToRoll(223, 235));
			  //0 -> 63 short
			}
			if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
			  buzzer->buzzerOff();
			  ledDisp->showNumber(buzzer->addRandomSoundToRoll(160, 223));
			}
			if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
			  buzzer->buzzerOff();
			  ledDisp->showNumber(buzzer->addRandomSoundToRoll(97, 160));
			}  
		}else{ 
			// simple mode.
			if (potentio->getValueStableChangedEdge()){
			  //buzzer->programBuzzerRoll(potentio->getValueStable() /4);;
			   allNotesIndex = potentio->getValueMapped(0,255);
			  if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){  
			    buzzer->buzzerOff();
			} 
			  buzzer->programBuzzerRoll(allNotesIndex);
			}
			  
			if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
			  buzzer->programBuzzerRoll(allNotesIndex);
			  allNotesIndex--;
			}
			if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
			  buzzer->programBuzzerRoll(allNotesIndex);
			}
			
			if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
			  buzzer->programBuzzerRoll(allNotesIndex);
			  allNotesIndex++;
			}
			ledDisp->showNumber(allNotesIndex);
		}
	  }
}

void Apps::modeSingleSegmentManipulation(bool init){

  if (init){

    // animation_step = 0;
	
	//bottom left is origin
	game_x_pos = 0;
    game_y_pos = 0;
	counter = 0; // segment active
	// counter2 = 0; 
	
  }
  
  if (potentio->getValueStableChangedEdge()){
    //uint8_t tmp = (uint8_t) (potentio->getValueStable()/4);
    //ledDisp->SetSingleDigit(tmp, 2);  
    
  }
  
  
  if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
	  //move right
	  game_x_pos++;
	  if (game_x_pos > 2){
		  counter++;
		  if (counter> 3){
			  counter = 0;
		  }
		  game_x_pos = 0;
	  }
  }
  
  if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){ 
	  //move left
	  if (game_x_pos == 0){
		counter = (counter == 0)?3:(counter-1);
		game_x_pos = 2;
	  }else{
		game_x_pos--;
	  }
  }
  
  if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){	
	  //move up
	   switch (game_y_pos){
		case 0:
			game_y_pos = (game_x_pos == 1)?1:3;
			break;
		case 1:
			game_y_pos = 3;
			break;
		case 2:
			game_y_pos = 3;
			break;
		case 3:
			game_y_pos = 0;
			break;
	  }
  }
  
  if (binaryInputs[BUTTON_LATCHING_YELLOW].getEdgeUp()){
	  //move down
	  switch (game_y_pos){
		case 0:
			game_y_pos = 3;
			break;
		case 1:
			game_y_pos = 0;
			break;
		case 2:
			game_y_pos = 0;
			break;
		case 3:
			
			game_y_pos = (game_x_pos == 1)?2:0;
			break;
	  }	
  }

  uint8_t seg;
  switch (10*game_y_pos + game_x_pos){
	  case 0:
	  case 10:
		seg = 0b00010000;  //E
	    break;
	  case 1:
		seg = 0b00001000;  //D
		break;
	  case 32:
	  case 22:
	    seg = 0b00000010;  //B
		break;
	  case 20:
	  case 30:
		seg = 0b00100000;  //F
		break;
	  case 12:
	  case 2:
		seg = 0b00000100;  //C
		break;
	  case 11:
	  case 21:
		seg = 0b01000000;  //G
		break;
	  
	  case 31:
		seg = 0b00000001;  //A
		break;
	  default:
		seg = 0b01010101;
		break;
  }
  
  for (uint8_t i=0;i<4;i++){
	  ledDisp->SetSingleDigit(0,i+1);  
  }
  ledDisp->SetSingleDigit(seg,counter+1);  
  
// ledDisp->displayHandler(textBuf);  
// ledDisp->SetSingleDigit(B11111111, 1);  
// ledDisp->SetSingleDigit((uint8_t) (potentio->getValueStable()/4), 3);  
  
  // if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
    // ledDisp->SetSingleDigit(pgm_read_byte_near(disp_digit_animate + animation_step),1);  
    // //ledDisp->SetSingleDigit(pgm_read_byte_near(disp_digit_animate_double + animation_step),2);  
    // //ledDisp->SetSingleDigit(pgm_read_byte_near(disp_digit_animate_inverted + animation_step),3);  
    // ledDisp->SetSingleDigit(pgm_read_byte_near(disp_digit_animate + animation_step),4);  
     // (animation_step>=5)?animation_step=0:animation_step++;
  // }
  
}

void Apps::miniMultiTimer(bool init){

  // every player: init time, time left, alive? 
  // game: pause, player alive? ,fischertimer active?/time, random starter

  if (init){
	 
	  this->multiTimer.setBuzzer(this->buzzer);
	  this->multiTimer.init();
  }  
  
  // TIMER BUTTONS
  if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
	  this->multiTimer.playerButtonPressEdgeUp(2);
  }
  if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
	  this->multiTimer.playerButtonPressEdgeUp(1);
  }
  if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
	  this->multiTimer.playerButtonPressEdgeUp(0);
  }
  
  if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeDown()){
	  this->multiTimer.playerButtonPressEdgeDown(2);
  }
  if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeDown()){
	  this->multiTimer.playerButtonPressEdgeDown(1);
  }
  if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeDown()){
	  this->multiTimer.playerButtonPressEdgeDown(0);
  }

  // START STOP Button
  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getEdgeUp()){
	  this->multiTimer.start();
  }
  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getEdgeDown()){
	  this->multiTimer.init();
  }
  
  // PAUSE Switch
  this->multiTimer.setStatePause(binaryInputs[BUTTON_LATCHING_YELLOW].getValue()); // do not only work on edge here, as latching switch can  be in any state.
  
  // # set number of timers SWITCH	
  this->multiTimer.setStateTimersCount(binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()); // do not only work on edge here, as latching switch can  be in any state.
  
  // set fischer timer SWITCH	
  this->multiTimer.setStateFischerTimer(binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()); // do not only work on edge here, as latching switch can  be in any state.
  
  // THE DIAL
  if (potentio->getValueStableChangedEdge()){
	  // number of timers
	  this->multiTimer.setTimersCount((uint8_t)potentio->getValueMapped(1,3));

	  // convert value to predefined amount of seconds.
	  uint16_t seconds =  this->multiTimer.getIndexedTime(potentio->getValueMapped(0,91)); // 0 seconds to an hour
      
      // pass through to multitimer app, it has to decide about validity.
	  this->multiTimer.setAllInitCountDownTimeSecs(seconds);
      this->multiTimer.setFischerTimer(seconds);
  }
  
  // UPDATE CYCLIC
  this->multiTimer.refresh();
  
  uint8_t buttonLights;
  
  uint8_t settingsLights;
  this->multiTimer.getDisplay(textBuf, &buttonLights, &settingsLights);
  
  
  uint8_t lights=0b00000000;
  // timer buttons lights to real lights
  for(uint8_t i=0;i<4;i++){
	if (1<<i & buttonLights){
			lights |= 1<<lights_indexed[i+1];    
	}
  }
  
  // settings light to real lights
  (LIGHT_PAUSE & settingsLights)? lights|= 1<<LIGHT_YELLOW:false;
  (LIGHT_PLAYING & settingsLights)? lights|= 1<<LIGHT_LED_3:false;
  (LIGHT_FISCHER & settingsLights)? lights|= 1<<LIGHT_LED_2:false;
  (LIGHT_SET_TIMERS_COUNT & settingsLights)? lights|= 1<<LIGHT_LED_1:false;
  
  ledDisp->SetLedArray(lights); 
  ledDisp->displayHandler(textBuf);
}

void Apps::tiltSwitchTest(bool init){
  // four tilt switches are positioned as such that they are "ON" in rest position. 
  uint32_t screen = 0;
  if (init){
	  textBuf[1]='T'; 
	  textBuf[2]='I'; 
	  textBuf[3]='L'; 
	  textBuf[4]='T'; 
	  counter = 0;
	  counter2 = 0; // counts progress in movement.
	  buzzer->setSpeedRatio(2.0);
  }
  
  if (binaryInputs[SWITCH_TILT_FORWARD].getEdgeDown()){
    buzzer->programBuzzerRoll(1); //not beep but "puck"
	counter2|=0x01<<TILT_FORWARD;
  }
  
  if (binaryInputs[SWITCH_TILT_BACKWARD].getEdgeDown()){
    buzzer->programBuzzerRoll(1); //not beep but "puck"
	counter2|=0x01<<TILT_BACKWARD;
  }

  if (binaryInputs[SWITCH_TILT_LEFT].getEdgeDown()){
    buzzer->programBuzzerRoll(1); //not beep but "puck"
	counter2|=0x01<<TILT_LEFT;
  }
  
  if (binaryInputs[SWITCH_TILT_RIGHT].getEdgeDown()){
    buzzer->programBuzzerRoll(1); //not beep but "puck"
	counter2|=0x01<<TILT_RIGHT;
  }

  if (counter2>0 || counter > 0){
	  for (uint8_t i=0;i<=counter;i++){
		  
		  if (1<<TILT_FORWARD & counter2 || i<counter){
			screen |= (uint32_t)pgm_read_byte_near(tilt_forward + i)<< (8*i); //* 4 --> 4 bytes per dword
		  }
		  if (1<<TILT_BACKWARD & counter2 || i<counter){
			screen |= (uint32_t)pgm_read_byte_near(tilt_backward + i)<< (8*i); //* 4 --> 4 bytes per dword
		  }
		  if (1<<TILT_LEFT & counter2 || i<counter){
			screen |= (uint32_t)pgm_read_byte_near(tilt_left + i)<< (8*i); //* 4 --> 4 bytes per dword
		  }
		  if (1<<TILT_RIGHT & counter2 || i<counter){
			screen |= (uint32_t)pgm_read_byte_near(tilt_right + i)<< (8*i); //* 4 --> 4 bytes per dword
		  }
	  }	
	  
	  ledDisp->SetFourDigits(screen);  
  }else{
	  ledDisp->displayHandler(textBuf);
  }
  
  // keep track of progress
  if (counter2 == 0x0F){ //if a digit is complete
	counter++;
	Serial.println(counter);
	if (counter == 4){
		buzzer->loadBuzzerTrack(song_happy_dryer);
		counter = 0;
	}
	counter2 = 0;
  }
  
}

void Apps::modeGeiger(bool init){

  if (init){
      textBuf[4]=' ';
      this->geiger_counter = 0;
	  this->frequency_lower = 2000;
	  this->frequency_upper = 4000;
	  this->tone_length_millis = 10;
  }
    
  //play tick. 
  //wait random time.
  //X = - log(1 - Y)/ K   with Y a random value ( 0<Y<1) and K a constant ?
  long r = random(0, 1024)*random(0, 1024); 
  //long r = random(0, 1024);
  //r = r*r;

  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){

	if (potentio->getValueStableChangedEdge()){
		Serial.println(potentio->getValueStable());
		if (binaryInputs[BUTTON_MOMENTARY_RED].getValue()){
			this->frequency_lower = potentio->getValueMapped(0,5000);
			if (this->frequency_lower >= this->frequency_upper){
				this->frequency_lower = this->frequency_upper;
			}
			Serial.println("lower");
		}else if (binaryInputs[BUTTON_MOMENTARY_GREEN].getValue()){
			
			this->frequency_upper = potentio->getValueMapped(0,5000);
			if (this->frequency_upper <= this->frequency_lower){
				this->frequency_upper = this->frequency_lower;
			} 
			Serial.println("upper");
		}else if (binaryInputs[BUTTON_MOMENTARY_BLUE].getValue()){
		    this->tone_length_millis = potentio->getValueMapped(0,500);
			Serial.println("length");
		}else{
			this->geiger_trigger_chance = potentio->getValueMapped(0,1048576);
		}
		
	}

    if (r > this->geiger_trigger_chance){ // 1024*1024
		
	  if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){
		 // when tone playing, play it until next tone
		 tone(PIN_BUZZER, random(this->frequency_lower,this->frequency_upper));
	  }else{
		// limited time length tone
		tone(PIN_BUZZER, random(this->frequency_lower,this->frequency_upper), this->tone_length_millis);
	  }
    
      geiger_counter++;
      ledDisp->SetSingleDigit(geiger_counter, 1);  
      ledDisp->SetSingleDigit(pgm_read_byte_near(disp_digit_animate + animation_step),4);  
      (animation_step>=5)?animation_step=0:animation_step++;      
    }
    
  }else{
	// simple Geiger mode
	// todo: idea: if tilted forward, dramatically increase chance, tilted backward, decrease. This way, we can truly simulate a geiger counte.r
    if (r > potentio->getValueMapped(0,1048576)){
  //    buzzer->programBuzzerRoll(1); //not beep but "puck"
 	  tone(PIN_BUZZER, (unsigned int)50, 10);
    
      textBuf[1]='?';
      textBuf[2]='?';
      textBuf[3]='?';
      textBuf[4]='?';
      
    }else{
      textBuf[1]=' ';
      textBuf[2]=' ';
      textBuf[3]=' ';
      textBuf[4]=' ';
    }
    ledDisp->displayHandler(textBuf);  
  }
}

void Apps::modeSequencer(bool init){
	bool nextStep = 0;
	 if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){ 
		 if (init){
			counter = 0;   // step counter
			counter2 = 0;  // measure counter
			animation_speed.setInitTimeMillis((long)potentio->getValueStable() * -1);
			animation_speed.start();
			nextStep = true;
			
			// for (uint8_t i=0;i<32;i++){
				// this->sequencer_song[i] = C7_8;
			// }
		}
		
		if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
		  nextStep = true;
		}
	
		if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){
			if (!binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){ 
				if (potentio->getValueStableChangedEdge()){
					animation_speed.setInitTimeMillis(potentio->getValueMapped(-1024,0));
				}
			}

			if (!animation_speed.getTimeIsNegative()){
				nextStep = true;
				animation_speed.start();
			}
		}
	
		if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
			
			uint8_t note = potentio->getValueMapped(0,255);
			
			buzzer->programBuzzerRoll(note);

			if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){ 
				//copy to all measures
				for (uint8_t i=0;i<4;i++){
					this->sequencer_song[(counter%8) + 8*i] = note;
				}
			}else{
				this->sequencer_song[counter] = note;
			}
		}
		
		if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
			// just listen to the potentio note
		    buzzer->programBuzzerRoll(potentio->getValueMapped(0,255));
		}
		
		if (binaryInputs[BUTTON_MOMENTARY_RED].getValue()){
			// if red button continuously pressed, rotate potentio to hear all notes.
			if (potentio->getValueStableChangedEdge()){
				buzzer->programBuzzerRoll(potentio->getValueMapped(0,255));
			}
		}
		
		if (nextStep){
			counter++;
			if (counter > 31){
				counter = 0;
			}
			buzzer->programBuzzerRoll(this->sequencer_song[counter]);
			
			// sequencer shows every step in 32 notes bar. 8steps (circle) times 4 measures (bar on bottom)
			uint32_t screen = 0;
			if (counter % 8 < 4){
				screen |=  (uint32_t)0x1 << (8* (counter % 8)) ; 
			}else{
				screen |=  (uint32_t)0x1 << ((8*( 3 - ( counter % 8 -4 ))) + 6); 
			}
			
			screen |=  (uint32_t)0x1 << ((8*(counter / 8))+3) ;  // bar at bottom.
						
			//#ifdef DEBUG_SEQUENCER
			Serial.println(counter);
			Serial.println(counter, "BIN");
			//#endif 
			
			ledDisp->SetFourDigits(screen);
		}
		
		
	 }else{
		 this->modeMetronome(init);
	 }
  
}

void Apps::modeMetronome(bool init){
  bool nextStep = 0;
  if (init){
    counter = 0;
    counter2 = 0;
    game_x_pos = 0;
    game_y_pos = 0;
    animation_speed.setInitTimeMillis((long)potentio->getValueStable() * -1);
    animation_speed.start();
    nextStep = true;
  }
  

  
  
  bool direction1 = true;
  if(binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
    direction1 = false;
  }  
  
  bool direction2 = true;
  if(binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
    direction2 = false;
  }
  
 // if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){ 
    if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){

      if (potentio->getValueStableChangedEdge()){
        animation_speed.setInitTimeMillis(potentio->getValueMapped(-1024,0));
//        animation_speed.start(); //during turning it pauses because of the continuous restarting.
      }
      
      if (!animation_speed.getTimeIsNegative()){
        nextStep = true;
        animation_speed.start();
      }
    }
    
    if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
      nextStep = true;
    }
    
    if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
      counter = this->nextStepRotate(counter, !direction1, 0, 11);
      counter2 = counter;
      nextStep = true;
    }

    if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
      counter2 = this->nextStepRotate(counter2, !direction2, 0, 11);
      nextStep = true;
    }
    
    if (nextStep){
      uint32_t screen = 0;
      for (uint8_t i=0;i<4;i++){
        screen |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + counter*4 + (i)) << (8*i); //* 4 --> 4 bytes per dword
        screen |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + counter2*4 + (i)) << (8*i); 
      }
      ledDisp->SetFourDigits(screen);
      
      counter = this->nextStepRotate(counter, direction1, 0, 11);
      counter2 = this->nextStepRotate(counter2, direction2, 0, 11);
	  
	  if (counter == 0){
		  buzzer->programBuzzerRoll(C7_8);
	  }
	  if (counter2 == 0){
		  buzzer->programBuzzerRoll(C6_4);
	  }
    }
    
//  }else{
//    //interactive mode
//    // game: x,y 0,0 = bottom left 
//    if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
//      
//    }
//    
//
//  }
  
}

int16_t Apps::nextStepRotate(int16_t counter, bool countUpElseDown, int16_t minValue, int16_t maxValue){

     if (countUpElseDown){
        counter++;
     }else{
        counter--;
     }

     if (counter > maxValue){
        counter = minValue;
     }
     if (counter < minValue){
        counter = maxValue;
     }
     return counter;
}

void Apps::gameButtonInteraction(bool init){
  //yellow button active at start: yellow button is also a guess button
  // big red active: timed game
  // small red right active: time progressively shorter as game advances
  // small red left active: play by sound.
  
  bool getNewNumber = false;
  bool isDead = false;
  
  if (init){
    counter = 0; // holds score
    //randomSeed(123456);
    getNewNumber = true;
    generalTimer.setInitTimeMillis(0);

    initTime = (potentio->getValueMapped(-1024,0)); // only set the default inittime at selecting the game. If multiple games are played, init time stays the same.
    animation_speed.setInitTimeMillis(initTime);
    
    counter2 = 0;
    screenPersistenceOfVision = 0;
    reactionGameYellowButtonIsIncluded = binaryInputs[BUTTON_LATCHING_YELLOW].getValue();
    
    for (uint8_t i=0;i<4;i++){
      selectedSounds[i] = (uint8_t)random(100, 114);
    }
  }

  if (!(binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue() && binaryInputs[BUTTON_LATCHING_BIG_RED].getValue() )){
     //always show unless in soundmode->competition
    ledDisp->setDecimalPoint(true, reactionGameTarget+1);
  }

  if (!animation_speed.getTimeIsNegative()){
    // game timing animation update.
    for (uint8_t i=0;i<4;i++){
      screenPersistenceOfVision |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + counter2*4 + (i)) << (8*i); 
    }
    ledDisp->SetFourDigits(screenPersistenceOfVision);
     
    counter2 = this->nextStepRotate(counter2, true, 0, 12);
    
    animation_speed.reset();
    if (counter2 == 12){
      counter2 = 0;
      screenPersistenceOfVision=0;    
      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue() ){
        // timed out.
        isDead = true;
      }else{
        // time out not enabled.
        animation_speed.start();
      }
    }else{
      animation_speed.start();
    }   
  }
  
  if (!generalTimer.getTimeIsNegative()){
    //end of display high score, next number
    counter = 0;
    getNewNumber = true;
    generalTimer.reset();
    animation_speed.setInitTimeMillis(initTime);
       
  }else if(generalTimer.getIsStarted()){
     //do nothing.  wait for display high score is finished.
     if (generalTimer.getInFirstGivenHundredsPartOfSecond(500)){
        ledDisp->setBlankDisplay(); //make high score blink
     }else{
        ledDisp->showNumber(counter ); //score display. Leave at beginning, to display high score blinking.
     }
  }else if (binaryInputs[buttons_indexed[reactionGameTarget]].getEdgeUp() ||
      (binaryInputs[BUTTON_LATCHING_YELLOW].getValueChanged()&& reactionGameTarget == 0))
  {
      //right button
      counter++;
      getNewNumber = true;
      if (!binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
         //play by sounds
        buzzer->programBuzzerRoll(C7_8);
      }
      
  }else if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()  ||
      binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()  ||
      binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp() ||
      binaryInputs[BUTTON_LATCHING_YELLOW].getValueChanged())
  {
    //wrong button
      isDead = true;
  }
  
  // ledDisp->SetSingleDigit(&counter,3);
  // ledDisp->SetSingleDigit(*(&game_random+counter),3);
  if (isDead){
    if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
      generalTimer.setInitTimeMillis(-2000);
      generalTimer.start();
      //play by sounds
      for (uint8_t i=reactionGameYellowButtonIsIncluded?0:1;i<4;i++){
        buzzer->programBuzzerRoll(selectedSounds[i]+128);
        buzzer->programBuzzerRoll(rest_1);
      }
       
    }else{
      generalTimer.setInitTimeMillis(-2000);
      generalTimer.start();
      buzzer->programBuzzerRoll(F4_1);  
      buzzer->programBuzzerRoll(F4_1);  
      buzzer->programBuzzerRoll(F4_1);  
      buzzer->programBuzzerRoll(F4_1);  
    }
  }
  
  if (getNewNumber){
    ledDisp->setBlankDisplay();
    lights = 0b00000000; //reset before switch enquiry
    
    reactionGameTarget = (uint8_t)random(reactionGameYellowButtonIsIncluded?0:1, 4);
    
    screenPersistenceOfVision = 0; //reset animation graphics screen
    counter2= 0; //reset animation step

    if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
       //play by sounds
       buzzer->programBuzzerRoll(selectedSounds[reactionGameTarget]);
    }else{
      lights |= 1<<lights_indexed[reactionGameTarget];     
    }
    ledDisp->SetLedArray(lights); 
    if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
      animation_speed.setInitTimeMillis(animation_speed.getInitTimeMillis()*0.99);
    }        
    animation_speed.start();    
  }
}
