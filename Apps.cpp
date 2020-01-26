#include "Apps.h"
//#include "Arduino.h"
#include "PretbakSettings.h"

Apps::Apps(){
};

void Apps::setPeripherals( BinaryInput binaryInputs[], Potentio* potentio, DisplayManagement* ledDisp, Buzzer* buzzer, bool silentMode){
  this->buzzer = buzzer;
  this->binaryInputs = binaryInputs;
  this->potentio = potentio;
  this->ledDisp = ledDisp;
  this->silentMode = silentMode;
}

void Apps::setBuffers(  char*  textBuf, char*  scrollBuf){
   this->textBuf = textBuf;
   this->scrollBuf = scrollBuf;
   this->lights = lights;  
}

void Apps::test(){
  
  //(*this->buzzer).programBuzzerRoll(45);
  (*this->ledDisp).showNumber(666);
//  if (*this->binaryInputs)[BUTTON_MOMENTARY_0].getValue()){
  if (binaryInputs[BUTTON_MOMENTARY_0].getValue()){
    buzzer->programBuzzerRoll(45);
  }
}

void Apps::appSelector(bool init, uint8_t selector){

	if (init){
		// title mode (title screen will be displayed before real app starts)
		this->app_init_mode = true;
		
		// this->TIMER_INIT_APP.setInitTimeMillis(-2000);
		// this->TIMER_INIT_APP.start();
		ledDisp->SetFourDigits(0x0ff00f0f); 	
	}
	
	if (this->app_init_mode){
		
		if (this->init_app(init, selector)){ 
			// do init routine, if finished,end of init.
			this->app_init_mode = false;
			init = true;
		}

	}
	
	// not as else statement, to have the init properly transferred after app beginning screen.
	if (!this->app_init_mode){
		
		switch (selector) {
		case 0:
		  this->modeCountingLettersAndChars(init);
		  break;
		  
		case 1:
		  //sound fun with notes
		  this->modeSoundNotes();
		  break;
		  
		case 2:
		  //sound fun with frequencies.
      #ifdef SIMON_APP
		  this->modeSimon(init);
      #endif
		  break;
		  
		case 3:
		  //this->modeScroll(init);
      this->modeDiceRoll(init);

		  break;
		  
		case 4:
		  this->modeGeiger(init);
		  break;
		  
		case 5:
		  this->modeSoundSong(init);
		  break;
		  
		case 6:
		  this->draw(init);
		  break;
		  
		case 7:
		  this->modeSimpleButtonsAndLights();    
		  break;
		  
		case 8:
		  this->modeSequencer(init);
		  break;
		  
		case 9:
		  this->modeReactionGame(init);
		  break;
		  
		case 10:
		  this->tiltSwitchTest(init);
		  break;
		  
		case 11:
		  this->miniMultiTimer(init);
		  break;

		case 12:
      //this is the debug mode
      this->modeButtonDebug(init);
      break;
                  
		default:
		  break;
		}
	}
}


void Apps:: setDefaultMode(){
  //button lights
  ledDisp->SetLedArray(0b00000000);  // no lights

  //display
  //ledDisp->SetFourDigits(0xC0C0C0C0); //default dispaly 4x minus and decimal point.
  //ledDisp->SetFourDigits(0x80808080); //default dispaly 4x minus
  ledDisp->setBlankDisplay();
  //ledDisp->SetFourDigits(0xC0C0C0C0); 
  ledDisp->setBrightness(0,false);

  //buzzer
  buzzer->setSpeedRatio(1);
  buzzer->buzzerOff();
  buzzer->setTranspose(0);

}

bool Apps::init_app(bool init, uint8_t selector){
	// shows splash screen
	
	if(init){
		// init of the init_app...
		this->displayAllSegments = 0;
    for (uint8_t i=0;i<4;i++){
      this->displayAllSegments |= (uint32_t)pgm_read_byte_near(app_splash_screens + selector*4 + (i)) << (8*i); //* 4 --> 4 bytes per dword
    }
		
    // initialize list
    for (uint8_t i = 0; i < bytes_list_bufsize; i++) {
     	this->FADE_IN_RANDOM_LIST[i] = i;
    }

    // // shuffle in place
    this->shuffle(this->FADE_IN_RANDOM_LIST, bytes_list_bufsize);

		//this->fadeInList(step, bytes_list_bufsize, this->displayAllSegments, this->FADE_IN_RANDOM_LIST);
		// counter = 0;
		// this->TIMER_INIT_APP.setInitTimeMillis(-5); 

		counter = 27;
		this->TIMER_INIT_APP.setInitTimeMillis(-20); 

		this->TIMER_INIT_APP.start();
	}
	
	// advance one frame
	if(!this->TIMER_INIT_APP.getTimeIsNegative()){
		this->TIMER_INIT_APP.start();
		counter++;
	}

	if (counter < 32){
		// ledDisp->SetFourDigits(displaySequence[31-counter]); // use fade in as fade out to set text.
		ledDisp->SetFourDigits(0xFFFFFFFF); // use fade in as fade out to set text.

	}else if (counter < 50){
		//ledDisp->SetFourDigits(this->displayAllSegments);
		ledDisp->showNumber(selector);
		
	}else if (counter == 50){
		//this->fadeInList(displaySequence, 32, ~this->displayAllSegments);
		
	}else if (counter < 82 ){
		//ledDisp->SetFourDigits(~displaySequence[counter-51]);
    ledDisp->SetFourDigits(~ this->fadeInList(counter-51, bytes_list_bufsize, this->displayAllSegments, this->FADE_IN_RANDOM_LIST)); 
		
	}else {
		this->setDefaultMode();
		return true;
	}	  
	return false;
}

void Apps::modeButtonDebug(bool init){
  // integrated debug mode (intended to be able to be activated in the final product in order to debug).
  // will show in sequence all analog input values.
  if (init){
    generalTimer.setInitTimeMillis(0);
    generalTimer.start();
    counter = 0;
  }
  
  if(!generalTimer.getTimeIsNegative()){

    counter++;
    if (counter > 9){
       counter = 0; 
    }
    
    textBuf[1]=' ';
    textBuf[2]=' ';
    textBuf[3]='A'; 
   
    switch (counter){
      case 0:{
        textBuf[4]='0'; // analog A0
        ledDisp->displayHandler(textBuf);
        generalTimer.setInitTimeMillis((long)-500);
        break; 
      }
      case 1:{
        ledDisp->showNumber( (int16_t) analogRead(PIN_SELECTOR_DIAL));
        generalTimer.setInitTimeMillis((long)-1000);
        break; 
      }
      case 2:{
        textBuf[4]='1'; // analog A1
        ledDisp->displayHandler(textBuf);
        generalTimer.setInitTimeMillis((long)-500);
        break; 
      }
      case 3:{
        ledDisp->showNumber( (int16_t) analogRead(PIN_BUTTONS_1));
        generalTimer.setInitTimeMillis((long)-1000);
        break; 
      }
      case 4:{
        textBuf[4]='2'; // analog A2
        ledDisp->displayHandler(textBuf);
        generalTimer.setInitTimeMillis((long)-500);
        break; 
      }
      case 5:{
        ledDisp->showNumber( (int16_t) analogRead(PIN_BUTTONS_2));
        generalTimer.setInitTimeMillis((long)-1000);
        break; 
      }
      case 6:{
        textBuf[4]='3';// analog A3
        ledDisp->displayHandler(textBuf);
        generalTimer.setInitTimeMillis((long)-500);
        break; 
      }
      case 7:{
        ledDisp->showNumber( (int16_t) analogRead(PIN_POTENTIO));
        generalTimer.setInitTimeMillis((long)-1000);
        break; 
      }
      case 8:{
        textBuf[4]='4';// analog A4
        ledDisp->displayHandler(textBuf);
        generalTimer.setInitTimeMillis((long)-500);
        break; 
      }
      case 9:{
        ledDisp->showNumber( (int16_t) analogRead(PIN_MERCURY_SWITCHES));
        generalTimer.setInitTimeMillis((long)-1000);
        break; 
      }
    }
    
    generalTimer.start();
  }  
}

void Apps::modeDiceRoll(bool init){
  if (init){
  
    counter = 1;
    generalTimer.setInitTimeMillis(-1000);
  	generalTimer.start();
  }

  if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){

    generalTimer.setInitTimeMillis((long)(1* potentio->getValueMapped(-1000, 0))); //divided by ten, this way, we can set the timer very accurately as displayed on screen when big red is pressed. *100ms
  
  }else{
    generalTimer.setInitTimeMillis(-1000);

  }

  if (binaryInputs[BUTTON_MOMENTARY_2].getValue() || binaryInputs[BUTTON_MOMENTARY_0].getValue()|| binaryInputs[BUTTON_MOMENTARY_1].getValue()){
    
    counter ++;
    if (counter > 6){  // about 600 times per second cycling through, so, this is as good as random, or is it? can you time it and predict your roll? I'll leave it in for you to find out!
      counter = 1;
    }

    if (!generalTimer.getTimeIsNegative()){

      if (binaryInputs[BUTTON_MOMENTARY_1].getValue()){   
        
        for (uint8_t  i=1;i<4;i++){
          textBuf[i]=textBuf[i+1];
        }
        textBuf[4] = 48 + counter; // convert digit to number char.
      }

      buzzer->cleanBuzzerRoll();
      buzzer->programBuzzerRoll(14);
      ledDisp->displayHandler(textBuf);
      generalTimer.start();
    }

    if (binaryInputs[BUTTON_MOMENTARY_2].getValue()){
      // show random nonsense coolness. 
      
      textBuf[1]='?';
      textBuf[2]='?';
      textBuf[3]='?'; 
      textBuf[4]='?'; 

    } else if (binaryInputs[BUTTON_MOMENTARY_1].getValue()){
      // sequence of numbers
      
    }else{
      // show dice eyes
      
      textBuf[4] = ' '; 
      for (uint8_t i=1;i<4;i++){
        // build up dice eyes over three digits 

        //set default for digit 1 2 and 3. Because most used (seg A and D) 
        textBuf[i] = '=';  

        //first and third digit
        if (counter == 1 ){
          textBuf[i] = ' '; // 
        }else if(counter <4){
          textBuf[i] = '^'; // assume first digit seg A
          if( i == 3){
            textBuf[i] = '_'; // seg D
          }
        }

        //second digit
        if (i == 2 && counter < 6){
          textBuf[i] = '-'; // assume odd
          if (counter%2 == 0 ){ // if even
            textBuf[i] = ' '; 
          }
        }
      }
    }
  } 
  
}

// void Apps::modeScroll(bool init){
  
//   // display scroll mode
  
//   if (init){
//     // display scroll mode
// //    scrollBuf[0]='L';
// //    scrollBuf[1]='U';
// //    scrollBuf[2]='C';
// //    scrollBuf[3]='I';
// //    scrollBuf[4]='E';
// //    scrollBuf[5]=' ';
// //    scrollBuf[6]='B';
// //    scrollBuf[7]='A';
// //    scrollBuf[8]='B';
// //    scrollBuf[9]='Y';
// //

//     this->scrollBuf[0]='H';
//     scrollBuf[1]='A';
//     scrollBuf[2]='P';
//     scrollBuf[3]='P';
//     scrollBuf[4]='Y';
//     scrollBuf[5]=' ';
//     scrollBuf[6]='B';
//     scrollBuf[7]='D';
//     scrollBuf[8]='A';
//     scrollBuf[9]='Y';
//     scrollBuf[10]=' ';
//     scrollBuf[11]='B';
//     scrollBuf[12]='R';
//     scrollBuf[13]='A';
//     scrollBuf[14]='M';
//     scrollBuf[15]='Z';
//     scrollBuf[16]='Y';
//     //scrollBuf[10]='/0';
//     ledDisp->dispHandlerWithScroll(scrollBuf, true, false);
      
	  
// 	this->fadeInList(displaySequence, 32, 0);
// 	counter = 0;
// 	counter2= false;
	
// 	generalTimer.setInitTimeMillis(-200);
// 	generalTimer.start();
//   }
//   if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
// 		  if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
// 			counter++;
			
// 			// Serial.println(displaySequence[counter], BIN);
// 		  }
// 		  if(!generalTimer.getTimeIsNegative()){
// 			  generalTimer.start();
// 			  counter++;
// 		  }
// 		  if (counter>31){
// 				counter = 0;
// 				this->fadeInList(displaySequence, 32, 0);
// 				counter2  = !counter2;
// 			}
			
// 		  if (potentio->getValueStableChangedEdge()){
// 			generalTimer.setInitTimeMillis((long)( potentio->getValueMapped(-1000, 0))); //divided by ten, this way, we can set the timer very accurately as displayed on screen when big red is pressed. *100ms
// 		  }
		  
// 		  displayAllSegments = displaySequence[counter];
// 		  if (counter2){
// 			  // negative ==> which makes it fade out.
// 			  displayAllSegments = ~displayAllSegments;
// 		  }
			  
// 		  ledDisp->SetFourDigits(displayAllSegments);

		 
// 	  }else{
// 		  if (!binaryInputs[BUTTON_MOMENTARY_2].getValue()){
// 			ledDisp->doScroll();
// 		  }

// 		  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
// 			ledDisp->setScrollSpeed((long)potentio->getValueStable());
// 		  }else{
// 			ledDisp->setBrightness((byte)(potentio->getValueMapped(0,50)),false);
// 		  }
// 	  }
  
// }

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
      if (binaryInputs[BUTTON_MOMENTARY_0].getValue()){
        lights|= 1<<LIGHT_RED;
        updateScreen = true;
      }
      if (binaryInputs[BUTTON_MOMENTARY_2].getValue()){
        lights|= 1<<LIGHT_BLUE;
        updateScreen = true;
      }
      if (binaryInputs[BUTTON_MOMENTARY_1].getValue()){
        lights|= 1<<LIGHT_GREEN;
        updateScreen = true;
      }
      
      #if MOMENTARY_BUTTONS_COUNT == 4
      if (binaryInputs[BUTTON_MOMENTARY_3].getValue()){
        lights|= 1<<LIGHT_YELLOW_EXTRA;
        updateScreen = true;
      }
      #endif

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
      if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
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
      
      const bool numberElseAlphabethMode = !binaryInputs[BUTTON_LATCHING_EXTRA].getValue();
	  
  
      if (binaryInputs[BUTTON_LATCHING_EXTRA].getValueChanged()){
        updateScreen = true;
        if (!numberElseAlphabethMode){
          buzzer->buzzerOff();
          buzzer->setSpeedRatio(4);
          buzzer->loadBuzzerTrack(alphabeth_song);
        }else{
          buzzer->buzzerOff();
        }
      }
    
      if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
        counter++;
        updateScreen = true;
      }
       
      if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
        counter--;
        updateScreen = true;
      } 
      if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
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
    if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
      if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
        buzzer->loadBuzzerTrack(scale_major);
      }
      if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
        //buzzer->loadBuzzerTrack(song_unhappy_dryer);
      }
      if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
        //buzzer->loadBuzzerTrack(song_attack );
      }
    }else{
      if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
        buzzer->loadBuzzerTrack(scale_major_reversed);
      }
      if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
        //buzzer->loadBuzzerTrack(song_unhappy_dryer);
      }
      if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
        //buzzer->loadBuzzerTrack(song_retreat );
      }
    }
  }else{
    // simple mode: songs!
    if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
      if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
        buzzer->loadBuzzerTrack(song_unhappy_dryer);
		//Serial.println("unhappy");
      }
      if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
        buzzer->loadBuzzerTrack(kindeke_douwen);
		//Serial.println("dow");
      }
      if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
        buzzer->loadBuzzerTrack(song_retreat );
		//Serial.println("rettr");
      }
    }else{
      if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
        buzzer->loadBuzzerTrack(song_happy_dryer);
      }
      if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
        buzzer->loadBuzzerTrack(song_lang_zal_ze_leven);
      }
      if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
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
        // SOUND_FUN_NOTE_INDEX = potentio->getValueMapped(0,255);
      // ledDisp->showNumber(SOUND_FUN_NOTE_INDEX);
      // buzzer->programBuzzerRoll(SOUND_FUN_NOTE_INDEX);
      // }
    // }else  
    if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
      
      if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
        buzzer->buzzerOff();
        ledDisp->showNumber(buzzer->addRandomSoundToRoll(223, 235));
        //0 -> 63 short
      }
      if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
        buzzer->buzzerOff();
        ledDisp->showNumber(buzzer->addRandomSoundToRoll(160, 223));
      }
      if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
        buzzer->buzzerOff();
        ledDisp->showNumber(buzzer->addRandomSoundToRoll(97, 160));
      }  
    }else{ 

      // simple mode.
      if (potentio->getValueStableChangedEdge()){
        //buzzer->programBuzzerRoll(potentio->getValueStable() /4);;
        SOUND_FUN_NOTE_INDEX = potentio->getValueMapped(0,255);
        if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){  
          buzzer->buzzerOff();
      } 
        buzzer->programBuzzerRoll(SOUND_FUN_NOTE_INDEX);
      }
        
      if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
        buzzer->programBuzzerRoll(SOUND_FUN_NOTE_INDEX);
        SOUND_FUN_NOTE_INDEX--;
      }
      if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
        buzzer->programBuzzerRoll(SOUND_FUN_NOTE_INDEX);
      }
      
      if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
        buzzer->programBuzzerRoll(SOUND_FUN_NOTE_INDEX);
        SOUND_FUN_NOTE_INDEX++;
      }
      ledDisp->showNumber(SOUND_FUN_NOTE_INDEX);
    }
  }
}

void Apps::draw(bool init){
	
	if(binaryInputs[BUTTON_LATCHING_BIG_RED].getEdgeUp()){
		this->movieAnimationMode(true);
		
	}else if (binaryInputs[BUTTON_LATCHING_BIG_RED].getEdgeDown()){
		this->modeSingleSegmentManipulation(false);
		
	}else if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
		this->movieAnimationMode(init);
		
	}else{
		this->modeSingleSegmentManipulation(init);
	}
}

void Apps::movieAnimationMode(bool init){
	//bool nextStep = 0;
	 //reset saved led disp state.
	if (init){
		counter = 4; // display is four characters. Four bytes.So, it should advance four bytes every step (default). But, it could give fun effects to change that number and see what happens... 
		this->dataPlayer.loadAllData(disp_4digits_animations);
		this->dataPlayer.loadDataSet(1);
		this->dataPlayer.setAutoSteps(4);
	}

	if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
		// manual mode
		if (potentio->getValueStableChangedEdge()){
			if (potentio->getLastStableValueChangedUp()){
				this->dataPlayer.setSetIndexDirection(1);
				this->dataPlayer.moveIndexSteps(counter);
			}else{
				this->dataPlayer.setSetIndexDirection(0);
				this->dataPlayer.moveIndexSteps(counter);
			}
		}
		
		if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){	
			 this->dataPlayer.setSetIndexDirection(1);
			 this->dataPlayer.moveIndexSteps(counter);
		}
		
		if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){	
			counter++;
			if (counter>4){
				counter = 1;
			}
			this->dataPlayer.setAutoSteps(counter);
		}
		
		if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){	
			this->dataPlayer.setSetIndexDirection(0);
			 this->dataPlayer.moveIndexSteps(counter);
		}	
		
	}else{
		// auto mode.
		this->dataPlayer.update();
		if (potentio->getValueStableChangedEdge()){
			dataPlayer.setAutoStepSpeed(potentio->getValueMapped(-1024,0));
		}

		if(binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
			this->dataPlayer.setSetIndexDirection(2);
		}
		  
		  // if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
			// if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){	
			 // animation_step = (uint16_t)(counter/4) - 1; // last step
			// }
			// if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){	
				 // animation_step = 0; // first step
			// }
		  // }
		  
		  if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp() || binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
		
			if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
				this->dataPlayer.nextDataSet(false);
			}
			if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
				this->dataPlayer.nextDataSet(true);
			}
		}
	}
	
	// get the display data.
	displayAllSegments = this->dataPlayer.getActive32bit();
	
	//invert all data in picture 
	if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
	  // negative .
	  displayAllSegments = ~displayAllSegments ;
	}
	
	// set to display 
	ledDisp->SetFourDigits(displayAllSegments);
}

void Apps::modeSingleSegmentManipulation(bool init){
  uint8_t moveDir;
  moveDir = DO_NOTHING;
  if (init){

    // animation_step = 0;
	
    //bottom left is origin
    DRAW_X_POS = 0;
    DRAW_Y_POS = 0;
    DRAW_ACTIVE_SEGMENT = 0; // segment active
    
    //reset saved led disp state.
    DRAW_DISP_STATE[0]=0;
    DRAW_DISP_STATE[1]=0;
    DRAW_DISP_STATE[2]=0;
    DRAW_DISP_STATE[3]=0;
    
    //generalTimer.setInitTimeMillis(3000);
    //generalTimer.start();
  }
  
  if (potentio->getValueStableChangedEdge()){
		  uint16_t val;
		  val = (uint16_t)potentio->getValueMapped(0,102); // 1024/10 causes no skips of segments. 
		  val = val % 48;   // 48 positions for 3x4 matrix * 4 digits
		  DRAW_ACTIVE_SEGMENT = val/12; // get digit
		  val = val%12; 
		  DRAW_X_POS = val/4; // get xpos
		  DRAW_Y_POS = val%4; // get ypos
  }
  
  if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
	  if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
		  moveDir = MOVE_RIGHT;
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
		  moveDir = TOGGLE_SEGMENT;
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){	
		  moveDir = MOVE_UP;
	  }
  }else{
	  if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
		  moveDir = MOVE_RIGHT;
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
		  moveDir = MOVE_LEFT;
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){	
		  moveDir = MOVE_UP;
	  }
  }
  
  // set x and y coords
  // 3 (x) * 4(y) matrix. (y is 4 positions to "remember" if low or high part when passing through segment G)
  switch(moveDir){
	
	case MOVE_RIGHT:{
	  //move right
	  DRAW_X_POS++;
	  if (DRAW_X_POS > 2){
		  DRAW_ACTIVE_SEGMENT++; // next digit
		  if (DRAW_ACTIVE_SEGMENT> 3){ 
			  DRAW_ACTIVE_SEGMENT = 0;
		  }
		  DRAW_X_POS = 0;
	  }
	}
	break;
	
	case MOVE_DOWN:
	{
	  //move down
	  switch (DRAW_Y_POS){
		case 0:
			DRAW_Y_POS = 3;
			break;
		case 1:
			DRAW_Y_POS = 0;
			break;
		case 2:
			DRAW_Y_POS = 0;
			break;
		case 3:
			DRAW_Y_POS = (DRAW_X_POS == 1)?2:0;
			break;
	  }	
	}
	break;

	case MOVE_UP:
	{	
	  //move up
	   switch (DRAW_Y_POS){
		case 0:
			DRAW_Y_POS = (DRAW_X_POS == 1)?1:3;
			break;
		case 1:
			DRAW_Y_POS = 3;
			break;
		case 2:
			DRAW_Y_POS = 3;
			break;
		case 3:
			DRAW_Y_POS = 0;
			break;
	  }
    }
    break;
    
    case MOVE_LEFT:
    { 
	  //move left
	  if (DRAW_X_POS == 0){
		DRAW_ACTIVE_SEGMENT = (DRAW_ACTIVE_SEGMENT == 0)?3:(DRAW_ACTIVE_SEGMENT-1); // previous digit
		DRAW_X_POS = 2;
	  }else{
		DRAW_X_POS--;
	  }
    }
	break;
	 
	default:
      break;
  }

  uint8_t seg;

  switch (10*DRAW_Y_POS + DRAW_X_POS){
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
  
  // toggle active segment.
  if (moveDir == TOGGLE_SEGMENT){
	    // toggle segment with xor old^toggleval=new  
		//	toggleval==0: 0^0=0, 1^0=1 , 
		//  toggleval==1: 1^1=0, 0^1=1
		DRAW_DISP_STATE[DRAW_ACTIVE_SEGMENT]  ^= seg; 
  }
  
  //invert all data in picture 
  if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValueChanged()){
    //i can typ whatever i want mlem mlem mlem
	  // negative .
	  for (uint8_t i=0;i<4;i++){
		  DRAW_DISP_STATE[i] = ~DRAW_DISP_STATE[i] ;
	  }  
  }
  
  
  // set display
  for (uint8_t i=0;i<4;i++){
	  ledDisp->SetSingleDigit(DRAW_DISP_STATE[i],i+1);  
  }

  //show active segment on display
  // if (generalTimer.getInFirstGivenHundredsPartOfSecond(500) || !binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
  if (millis()%250 > 125 || !binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
	ledDisp->SetSingleDigit(seg^DRAW_DISP_STATE[DRAW_ACTIVE_SEGMENT],DRAW_ACTIVE_SEGMENT+1); // XOR the seg with the segment saved value, so it shows negatively.  
  }
  // else{
	  // //only show the saved state, no blinking of active segment. 
	  // ledDisp->SetSingleDigit(DRAW_DISP_STATE[DRAW_ACTIVE_SEGMENT],DRAW_ACTIVE_SEGMENT+1);
  // }
  
// ledDisp->displayHandler(textBuf);  
// ledDisp->SetSingleDigit(B11111111, 1);  
// ledDisp->SetSingleDigit((uint8_t) (potentio->getValueStable()/4), 3);  
  
  // if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
    // ledDisp->SetSingleDigit(pgm_read_byte_near(disp_digit_animate + animation_step),1);  
    // //ledDisp->SetSingleDigit(pgm_read_byte_near(disp_digit_animate_double + animation_step),2);  
    // //ledDisp->SetSingleDigit(pgm_read_byte_near(disp_digit_animate_inverted + animation_step),3);  
    // ledDisp->SetSingleDigit(pgm_read_byte_near(disp_digit_animate + animation_step),4);  
     // (animation_step>=5)?animation_step=0:animation_step++;
  // }
}


void Apps::miniMultiTimer(bool init){
#ifdef ENABLE_MULTITIMER
  // every player: init time, time left, alive? 
  // game: pause, player alive? ,fischertimer active?/time, random starter

  if (init){
	  this->multiTimer.setBuzzer(this->buzzer);
	  this->multiTimer.init();
  }  
  
  // TIMER BUTTONS
  #if MOMENTARY_BUTTONS_COUNT == 4
  
  if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){
	  this->multiTimer.playerButtonPressEdgeUp(3);
  }
  
  if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeDown()){
	  this->multiTimer.playerButtonPressEdgeDown(3);
  }
  
  #endif

  if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
	  this->multiTimer.playerButtonPressEdgeUp(2);
  }
  if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
	  this->multiTimer.playerButtonPressEdgeUp(1);
  }
  if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
	  this->multiTimer.playerButtonPressEdgeUp(0);
    //Serial.println("eijejfjeetttt");
  }
  
    if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeDown()){
	  this->multiTimer.playerButtonPressEdgeDown(2);
  }
  if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeDown()){
	  this->multiTimer.playerButtonPressEdgeDown(1);
  }
  if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeDown()){
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
  this->multiTimer.setStatePause(binaryInputs[BUTTON_LATCHING_EXTRA].getValue()); // do not only work on edge here, as latching switch can  be in any state.
  
  // # set number of timers SWITCH	
  this->multiTimer.setStateTimersCount(binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()); // do not only work on edge here, as latching switch can  be in any state.
  
  // set fischer timer SWITCH	
  this->multiTimer.setStateFischerTimer(binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()); // do not only work on edge here, as latching switch can  be in any state.
  
  // THE DIAL
  if (potentio->getValueStableChangedEdge()){
	  // number of timers

    #ifdef PROTOTYPE
    this->multiTimer.setTimersCount((uint8_t)potentio->getValueMapped(1,3));
    #else
	  this->multiTimer.setTimersCount((uint8_t)potentio->getValueMapped(1,MAX_TIMERS_COUNT));
    #endif
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

  ledDisp->displayHandler(textBuf);
  ledDisp->SetLedArray(lights); 
  ledDisp->setDecimalPoint(LIGHT_SECONDS_BLINKER & settingsLights ,2);
    
#endif
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
	  
	  this->dataPlayer.loadAllData(disp_4digits_animations);
	 
	  this->dataPlayer.setAutoSteps(4);
	  this->dataPlayer.setAutoStep(true);
	  this->dataPlayer.setAutoStepSpeed(-30);
  }
  
  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
	  // movie for each gesture
	  
     if (binaryInputs[SWITCH_TILT_FORWARD].getEdgeDown()){
		 this->dataPlayer.loadDataSet(1);
		 this->dataPlayer.setSetIndexDirection(1);
	 }
	 if (binaryInputs[SWITCH_TILT_BACKWARD].getEdgeDown()){
		 this->dataPlayer.loadDataSet(1);
		 this->dataPlayer.setSetIndexDirection(0);
	 }
	 if (binaryInputs[SWITCH_TILT_LEFT].getEdgeDown()){
		 this->dataPlayer.loadDataSet(0);
		 this->dataPlayer.setSetIndexDirection(1);
	 }
	 if (binaryInputs[SWITCH_TILT_RIGHT].getEdgeDown()){
		 this->dataPlayer.loadDataSet(0);
		 this->dataPlayer.setSetIndexDirection(0);
	 }
	 this->dataPlayer.update();
	 screen = this->dataPlayer.getActive32bit();

	 ledDisp->SetFourDigits(screen);  
  }else{
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

		if(counter2>0 || counter > 0){  
	  
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
  }
  
  // keep track of progress
  if (counter2 == 0x0F){ //if a digit is complete
	counter++;
	//Serial.println(counter);
	if (counter == 4){
		buzzer->loadBuzzerTrack(song_happy_dryer);
		counter = 0;
	}
	counter2 = 0;
  }
  
}

void Apps::modeGeiger(bool init){

  if (init){
    //textBuf[4]=' ';
    COUNTER_GEIGER = 0;
	  GEIGER_TONE_FREQUENY_LOWEST = 2000;
	  GEIGER_TONE_FREQUENCY_HEIGHEST = 4000;
	  GEIGER_TONE_LENGTH = 10;
  }
    
  //play tick. 
  //wait random time.
  //X = - log(1 - Y)/ K   with Y a random value ( 0<Y<1) and K a constant ?
  long r = random(0, 1024)*random(0, 1024); 
  //long r = random(0, 1024);
  //r = r*r;

  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){

    if (potentio->getValueStableChangedEdge()){
      //Serial.println(potentio->getValueStable());
      if (binaryInputs[BUTTON_MOMENTARY_0].getValue()){
        //lower
        GEIGER_TONE_FREQUENY_LOWEST = potentio->getValueMapped(0,5000);
        if (GEIGER_TONE_FREQUENY_LOWEST >= GEIGER_TONE_FREQUENCY_HEIGHEST){
          GEIGER_TONE_FREQUENY_LOWEST = GEIGER_TONE_FREQUENCY_HEIGHEST;
        }

      }else if (binaryInputs[BUTTON_MOMENTARY_1].getValue()){
        //upper
        GEIGER_TONE_FREQUENCY_HEIGHEST = potentio->getValueMapped(0,5000);
        if (GEIGER_TONE_FREQUENCY_HEIGHEST <= GEIGER_TONE_FREQUENY_LOWEST){
          GEIGER_TONE_FREQUENCY_HEIGHEST = GEIGER_TONE_FREQUENY_LOWEST;
        }

      }else if (binaryInputs[BUTTON_MOMENTARY_2].getValue()){
        //length
        GEIGER_TONE_LENGTH = potentio->getValueMapped(0,500);

      }else{
        GEIGER_PROBABILITY_THRESHOLD = potentio->getValueMapped(0,1048576);

      }		
    }

    if (r > GEIGER_PROBABILITY_THRESHOLD){ // 1024*1024
      

      if (!this->silentMode){
        if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
          // when tone playing, play it until next tone
          //tone(PIN_BUZZER, random(GEIGER_TONE_FREQUENY_LOWEST, GEIGER_TONE_FREQUENCY_HEIGHEST));
        }else{
          // limited time length tone
          //tone(PIN_BUZZER, random(GEIGER_TONE_FREQUENY_LOWEST, GEIGER_TONE_FREQUENCY_HEIGHEST), GEIGER_TONE_LENGTH);
        }
      }

      COUNTER_GEIGER++;
      ledDisp->showNumber(COUNTER_GEIGER);
      
    }
    
  }else{
    // simple Geiger mode
    // todo: idea: if tilted forward, dramatically increase chance, tilted backward, decrease. This way, we can truly simulate a geiger counte.r
    
    // If you press the button and approach an object, the object appears super radio active! hi-la-ri-ous!
    if ( binaryInputs[BUTTON_MOMENTARY_1].getValue()){
      // binaryInputs[SWITCH_TILT_FORWARD].getValue() ||
      // r *= 2; //
      GEIGER_INCREASE_CHANCE+=1000;
    }else{
      if (GEIGER_INCREASE_CHANCE > 0){
        GEIGER_INCREASE_CHANCE-=1500;
      }
    }

    r+=GEIGER_INCREASE_CHANCE;
	
    if (r > potentio->getValueMapped(0,1048576)){
    //    buzzer->programBuzzerRoll(1); //not beep but "puck"

      if (this->silentMode){
        tone(PIN_BUZZER, (unsigned int)50, 10);
      }
    
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
	 int8_t step = 0;
	 if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){ 
		 
     if (init){
			counter = 0;   // step counter
			counter2 = 0;  // measure counter
			generalTimer.setInitTimeMillis((long)potentio->getValueStable() * -1);
			generalTimer.start();
			
			
      //resets song.
			for (uint8_t i=0;i<32;i++){
			  this->SEQUENCER_SONG[i] = C7_8;
			}

		}
		
    // just listen to the potentio note
		if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
		    buzzer->programBuzzerRoll(potentio->getValueMapped(0,255));
		}
		
		if (binaryInputs[BUTTON_MOMENTARY_0].getValue()){
			// if button continuously pressed, rotate potentio to hear notes.
			if (potentio->getValueStableChangedEdge()){
				buzzer->programBuzzerRoll(potentio->getValueMapped(0,255));
			}
		}

    // program note to song
		if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
			
			uint8_t note = potentio->getValueMapped(0,255);
			
			buzzer->programBuzzerRoll(note);

			if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){ 
				//copy to all measures
				for (uint8_t i=0;i<4;i++){
					this->SEQUENCER_SONG[(counter%8) + 8*i] = note;
				}
			}else{
				this->SEQUENCER_SONG[counter] = note;
			}
		}
		
		// song progression
		if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
		  step = 1;
		}
	
    if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){
		  step = -1;
		}

		if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
			if (!binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){ 
				if (potentio->getValueStableChangedEdge()){
					generalTimer.setInitTimeMillis(potentio->getValueMapped(-1024,0));
				}
			}

			if (!generalTimer.getTimeIsNegative()){
				step = 1;
				generalTimer.start();
			}
		}


		if (step != 0){
			counter+=step;

      if (counter < 0){
        counter = 31;
      }

			if (counter > 31){
				counter = 0;
			}
			
      buzzer->programBuzzerRoll(this->SEQUENCER_SONG[counter]);
			
			// sequencer shows every step in 32 notes bar. 8steps (circle) times 4 measures (bar on bottom)
			uint32_t screen = 0;
			if (counter % 8 < 4){
				screen |=  (uint32_t)0x1 << (8* (counter % 8)) ; 
			}else{
				screen |=  (uint32_t)0x1 << ((8*( 3 - ( counter % 8 -4 ))) + 6); 
			}
			
			screen |=  (uint32_t)0x1 << ((8*(counter / 8))+3) ;  // bar at bottom.
						
			//#ifdef DEBUG_SEQUENCER
			//Serial.println(counter);
			//Serial.println(counter, "BIN");
			//#endif 
			
			ledDisp->SetFourDigits(screen);
		}
		
		
	 }else{
		 this->modeMetronome(init);
	 }
  
}

void Apps::modeMetronome(bool init){
  // todo: with extra timer, create slight timing offset in second ticker, for fun effects (zwevingen)!
  bool update = 0;
  // bool direction1 = true;
  // bool direction2 = true;
  bool nextStep = false;

  
  if (init){
    METRONOME_TICKER_1_POSITION = 0;
    METRONOME_TICKER_2_POSITION = 0;
    METRONOME_TICKER_3_POSITION = 0;
    TIMER_METRONOME.setInitTimeMillis((long)potentio->getValueStable() * -1);
    TIMER_METRONOME.start();
    update = true;
  }
  
  //ticker one direction inverse
  // if(binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
  //   direction1 = false;
  // }  
  
  // if(binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
  //   direction2 = false;
  // }
  

  if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){

    if (potentio->getValueStableChangedEdge()){
      TIMER_METRONOME.setInitTimeMillis(potentio->getValueMapped(-1024,0));
    }
    
    if (!TIMER_METRONOME.getTimeIsNegative()){
      
      TIMER_METRONOME.start();
      update = true;
      nextStep = true;
    }
  }
  
  if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
    //ticker 1,2 and 3 back together
    METRONOME_TICKER_2_POSITION = METRONOME_TICKER_1_POSITION;
    METRONOME_TICKER_3_POSITION = METRONOME_TICKER_1_POSITION;
    update = true;
  }

  if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
  
    //set ticker 2 one step extra forward .
    METRONOME_TICKER_2_POSITION = this->nextStepRotate(
      METRONOME_TICKER_2_POSITION, 
      //binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue(), 
      true,
      0, 
      11);
    update = true;
  }
  

  if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
    //set ticker 3 one step extra forward .
    METRONOME_TICKER_3_POSITION = this->nextStepRotate(
      METRONOME_TICKER_3_POSITION, 
      true,
      0, 
      11);
    update = true;
  }

  if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){
    update = true;
    nextStep = true;
  }

  if (update){
    uint32_t screen = 0;
    for (uint8_t i=0;i<4;i++){
      screen |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + METRONOME_TICKER_1_POSITION*4 + (i)) << (8*i); //* 4 --> 4 bytes per dword
      screen |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + METRONOME_TICKER_2_POSITION*4 + (i)) << (8*i); 
      screen |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + METRONOME_TICKER_3_POSITION*4 + (i)) << (8*i); 
    }
    ledDisp->SetFourDigits(screen);

    if (nextStep){
      METRONOME_TICKER_1_POSITION = this->nextStepRotate(
        METRONOME_TICKER_1_POSITION, 
        binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue(), 
        //true,
        0, 
        11);

      METRONOME_TICKER_2_POSITION = this->nextStepRotate(
        METRONOME_TICKER_2_POSITION, 
        binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue(), 
        // true,
        0, 
        11);
      METRONOME_TICKER_3_POSITION = this->nextStepRotate(
        METRONOME_TICKER_3_POSITION, 
        true, 
        0, 
        11);
    }  

    if (METRONOME_TICKER_1_POSITION == 0){
      buzzer->programBuzzerRoll(C7_8);
    }

    if (METRONOME_TICKER_2_POSITION == 0){
      buzzer->programBuzzerRoll(C6_4);
    }

    if ( (METRONOME_TICKER_3_POSITION == 0) && 
      (METRONOME_TICKER_3_POSITION != METRONOME_TICKER_1_POSITION)
      ){
      buzzer->programBuzzerRoll(C5_4);
    }
  }  
}

#ifdef SIMON_APP

void Apps::modeSimon(bool init)
{
  const int numButtons = 4;
  const int buttons[numButtons] = { BUTTON_LATCHING_EXTRA, BUTTON_MOMENTARY_0, BUTTON_MOMENTARY_1, BUTTON_MOMENTARY_2 };
  const byte lights[numButtons] = { 1 << LIGHT_YELLOW, 1 << LIGHT_RED, 1 << LIGHT_GREEN, 1 << LIGHT_BLUE };
  const uint8_t sounds[numButtons] = { C4_1, F4_1, A4_1, C5_1};

  const bool hasSound = binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue();
  const bool hasLight = binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue() || !hasSound;

  if (init) {
    simonState = simonWaitForNewGame;
  }

  if (init || potentio->getValueStableChangedEdge()) {
    generalTimer.setInitTimeMillis(potentio->getValueMapped(-1000,-100));
  }

  uint8_t buttonsChanged = 0;
  for (int k = 0; k < numButtons; ++k) {
    const bool changed = (buttons[k] == BUTTON_LATCHING_EXTRA)
      ? binaryInputs[buttons[k]].getValueChanged()
      : binaryInputs[buttons[k]].getEdgeUp();
    if (changed) {
      buttonsChanged |= (1 << k);
    }
  }

  switch (simonState) {
    case simonWaitForNewGame: {
      // all lights on
      byte allLights = 0;
      for (int k = 0; k < numButtons; ++k) {
        allLights |= lights[k];
      }
      ledDisp->SetLedArray(allLights);
      if (!buttonsChanged) {
        break;
      }
      simonState = simonNewGame;
      break;
    }

    case simonNewGame: {
      ledDisp->SetLedArray(0);
      // generate new sequence
      randomSeed(millis());
      for (int k = 0; k < bytes_list_bufsize; ++k) {
        SIMON_LIST[k] = k % numButtons;
      }
      shuffle(SIMON_LIST, bytes_list_bufsize);
      SIMON_LENGTH = 0;
      simonState = simonNewLevel;
      break;
    }

    case simonNewLevel: {
      ledDisp->showNumber(SIMON_LENGTH);
      ++SIMON_LENGTH;
      if (SIMON_LENGTH >= bytes_list_bufsize) {
          // reached maximum length
          if (hasSound) buzzer->loadBuzzerTrack(song_attack);
          simonState = simonWaitForNewGame;
          break;
      }
      SIMON_INDEX = -1; // negative index allows for lead-in time
      simonState = simonPlaySequence;
      generalTimer.start();
      break;
    }

    case simonPlaySequence: {
      if (generalTimer.getTimeIsNegative()) {
        break;
      }
      generalTimer.start();
      if (SIMON_INDEX < 0) {
        ++SIMON_INDEX; // do-nothing lead in time
        break;
      }
      if (SIMON_INDEX >= SIMON_LENGTH) {
        // sequence finished, give control to user
        ledDisp->SetLedArray(0);
        SIMON_INDEX = 0;
        simonState = simonUserRepeats;
        break;
      }
      // show one button from the sequence
      const uint8_t button = SIMON_LIST[SIMON_INDEX];
      if (hasLight) ledDisp->SetLedArray(lights[button]);
      if (hasSound) buzzer->programBuzzerRoll(sounds[button]); 
      ++SIMON_INDEX;
      break;
    }

    case simonUserRepeats: {
      if (!buttonsChanged) {
        break;
      }
      const int expected = SIMON_LIST[SIMON_INDEX];
      if (buttonsChanged != (1 << expected)) {
        // player made mistake, start new game
        if (hasSound) buzzer->loadBuzzerTrack(scale_major_reversed);
        simonState = simonWaitForNewGame;
        break;
      }
      // player pressed correct button
      if (hasSound) buzzer->programBuzzerRoll(sounds[expected]);
      ++SIMON_INDEX;
      if (SIMON_INDEX >= SIMON_LENGTH) {
        // sequence fully replaced, add one more note
        simonState = simonNewLevel;
        break;
      }
      break;
    }
  }
}
#endif 

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

void Apps::modeReactionGame(bool init){
  //yellow button active at start: yellow button is also a guess button
  // big red active: timed game
  // small red right active: time progressively shorter as game advances
  // small red left active: play by sound.
  
  if (init){
    randomSeed(millis());
    reactionGameState = reactionWaitForStart;
    displayAllSegments = 0x0;
  }

  // at any time, leave game when depressing play button.
  if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeDown() ){
    reactionGameState = reactionWaitForStart;
  }
 
  switch (reactionGameState) {

    case reactionWaitForStart: {

      ledDisp->SetFourDigits(displayAllSegments);

      if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeUp() ){
        reactionGameState = reactionNewGame;

        //play by sound
        for (uint8_t i=0;i<MOMENTARY_BUTTONS_COUNT;i++){
          REACTION_GAME_SELECTED_SOUNDS[i] = (uint8_t)random(100, 114);
        }

        //if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue() ){
        REACTION_GAME_STEP_TIME_MILLIS = (potentio->getValueMapped(-1024,0)); // only set the default inittime at selecting the game. If multiple games are played, init time stays the same.
        //}
      } 

      break;
    }

    case reactionNewGame: {
      
      TIMER_REACTION_GAME_SPEED.setInitTimeMillis(REACTION_GAME_STEP_TIME_MILLIS);
      REACTION_GAME_SCORE = 0;

      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){

        reactionGameState = reactionMultiNewTurn;
        displayAllSegments = 0;
        
      }else{
        reactionGameState = reactionNewTurn;
      }
     break;

    }

    case reactionMultiNewTurn: {
      // like in guitar hero, the horizontal segments in the screen fall down and button combo's have to be pressed to "catch" the falling segments.
      
      // three rows of four horizontal segments in 4 digits 7 segment display. 
      // choose top row random. any combination of 4 (including zero) ==> 16 combinations.

      ledDisp->setBlankDisplay(); 
      lights = 0b00000000;
     
      uint8_t new_segment;
      uint32_t tmp_segments;
      
      tmp_segments = 0;
      
      // treat every segment separatly
      for (uint8_t i=0;i<MOMENTARY_BUTTONS_COUNT;i++){
        new_segment = 0;
        // get relevant segment
        

        //mid seg to bottom seg
        new_segment |= ((displayAllSegments >> (8*i)) & 0b01000000) >> 3;// G segment, move to D segment  0G00D000
        
        //mid seg to DP seg
        new_segment |= ((displayAllSegments >> (8*i)) & 0b01000000) << 1;// G segment, move to DP segment  DP.G.0.0.0.0.0.0

        //top seg to mid seg
        new_segment |= ((displayAllSegments >> (8*i)) & 0b00000001)  << 6;// A segment, ON or OFF?, move to G segment

        // random top
        // if (random(0, 2)){  // 0 or 1  // move to segment A)
        //   new_segment |= 1;
        // }else{
        //   new_segment &= ~(0b00000001);
        // }
        // new_segment |= random(0, 2);
        
        if (random(0, 100) > 50){
          new_segment |= 0b00000001;
        }

        tmp_segments |= ((uint32_t)new_segment) << (8*i);
        
      }

      displayAllSegments = tmp_segments;

      //ledDisp->SetLedArray(lights); 
      ledDisp->SetFourDigits(displayAllSegments);
      TIMER_REACTION_GAME_SPEED.start();

      reactionGameState = reactionMultiPlaying;
      break; 
    }

    case reactionMultiPlaying: {

      for (uint8_t i=0;i<MOMENTARY_BUTTONS_COUNT;i++){

        if (binaryInputs[buttons_momentary_indexed[i]].getEdgeUp()){
          //if DP of this button was on, switch it off. else, die!
          //check if DP of digit "i" is set
          
          if (displayAllSegments & (0x80UL << 8*i)){
            // DP is on, set to zero.
            

            //buzzer->programBuzzerRoll(rest_1);
            buzzer->addRandomSoundToRoll(190,198);
            displayAllSegments &= ~(0x80UL << 8*i);
            REACTION_GAME_SCORE++; 
            //buzzer->programBuzzerRoll(rest_1);
            buzzer->programBuzzerRoll(C5_1);
            
          }else{
             //DP is off --> button should not have been pressed --> die!
            reactionGameState = reactionJustDied;
          }
        }
      }

      if (!TIMER_REACTION_GAME_SPEED.getTimeIsNegative()){
      //   // if not all needed buttons pressed, player is dead.
      //   // check dps!
        
        if ((displayAllSegments & 0x80808080UL) != 0 ){
          // no success... (not all DP's cleared.)
          reactionGameState = reactionJustDied;
          
        }else{
          // success!
          reactionGameState = reactionMultiNewTurn;
          //REACTION_GAME_SCORE++; // let's not update the score here, because the first two rows also "count" which is silly, let's go for "point per correct button press."
        }
      }
      ledDisp->SetFourDigits(displayAllSegments);
      break;
    }

    case reactionNewTurn:{
     
      ledDisp->setBlankDisplay(); // yes, this is needed.
      lights = 0b00000000; //reset before switch enquiry
      
      REACTION_GAME_TARGET = random(0, MOMENTARY_BUTTONS_COUNT);
      
      displayAllSegments = 0; //reset animation graphics screen
      REACTION_GAME_TIMER_STEP= 0; //reset animation step

      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
        //play by sounds
        buzzer->programBuzzerRoll(REACTION_GAME_SELECTED_SOUNDS[REACTION_GAME_TARGET]);
      }else{
        lights |= 1<<lights_indexed[REACTION_GAME_TARGET];     
      }

      ledDisp->SetLedArray(lights); 

      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
        TIMER_REACTION_GAME_SPEED.setInitTimeMillis(TIMER_REACTION_GAME_SPEED.getInitTimeMillis()*0.99);
      }        

      TIMER_REACTION_GAME_SPEED.start();    
    
      reactionGameState = reactionPlaying;
      break;
    }
    
    case reactionPlaying: {
      
      // animation next step
      if (!TIMER_REACTION_GAME_SPEED.getTimeIsNegative()){
        // game timing animation update.
        for (uint8_t i=0;i<MOMENTARY_BUTTONS_COUNT;i++){
          displayAllSegments |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + REACTION_GAME_TIMER_STEP*4 + (i)) << (8*i); 
        }
        ledDisp->SetFourDigits(displayAllSegments);
        
        REACTION_GAME_TIMER_STEP = this->nextStepRotate(REACTION_GAME_TIMER_STEP, true, 0, 12);
        
        TIMER_REACTION_GAME_SPEED.reset();
      
        // check game status 'dead'
        if (REACTION_GAME_TIMER_STEP == 12){
          REACTION_GAME_TIMER_STEP = 0;
          displayAllSegments=0;    
          
          reactionGameState = reactionJustDied;
         
        }else{
          TIMER_REACTION_GAME_SPEED.start();
        }   
      }

      // set decimal point as "button lights" helper, in bright daylight button lights might not be visible.
      if ( !binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue() ){

        //always show unless in soundmode->competition
        ledDisp->setDecimalPoint(true, REACTION_GAME_TARGET+1);
      }

      // check player input
      if ( binaryInputs[buttons_momentary_indexed[REACTION_GAME_TARGET]].getEdgeUp() ) {
          //right button
          REACTION_GAME_SCORE++;
          reactionGameState = reactionNewTurn;
          if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
            //play by sounds
            //buzzer->programBuzzerRoll(C7_8);
            buzzer->programBuzzerRoll(rest_1);
            //buzzer->programBuzzerRoll(rest_1);
          }
            
        }else{
          
          for (uint8_t i=0;i<MOMENTARY_BUTTONS_COUNT;i++){
            if (binaryInputs[buttons_momentary_indexed[i]].getEdgeUp()){
              //wrong button
               reactionGameState = reactionJustDied;
            }
          }
        }

     break;
    }
    
    case reactionJustDied:{
        
        //start high score end timer
        TIMER_REACTION_GAME_RESTART_DELAY.setInitTimeMillis(-2000);
        TIMER_REACTION_GAME_RESTART_DELAY.start();

        if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
          
          //play by sounds, let them all play.
          for (uint8_t i=0;i<MOMENTARY_BUTTONS_COUNT;i++){
            buzzer->programBuzzerRoll(REACTION_GAME_SELECTED_SOUNDS[i]+128);
            buzzer->programBuzzerRoll(rest_1);
          }
          
        }else{
          
          // play death song
          buzzer->programBuzzerRoll(F4_1);  
          buzzer->programBuzzerRoll(F4_1);  
          buzzer->programBuzzerRoll(F4_1);  
          buzzer->programBuzzerRoll(F4_1);  
        }

        reactionGameState = reactionFinished;

      break;
    }
    
    case reactionFinished: {
      if (!TIMER_REACTION_GAME_RESTART_DELAY.getTimeIsNegative()){
        //end of display high score, next game
        reactionGameState = reactionNewGame;
          
      }else {

        //do nothing.  wait for display high score is finished.
        if (TIMER_REACTION_GAME_RESTART_DELAY.getInFirstGivenHundredsPartOfSecond(500)){
            ledDisp->setBlankDisplay(); //make high score blink
            
        }else{
            ledDisp->showNumber(REACTION_GAME_SCORE ); //score display. Leave at beginning, to display high score blinking.
        }
        
      }
      break;
    }
  }
}

void Apps::_eepromWriteByteIfChanged(uint8_t* address , uint8_t value) {
	//as the number of write operations to eeprom is limited, only write when different value.
	if (eeprom_read_byte(address) != value) {
		eeprom_write_byte(address, value);
	}
}

uint32_t Apps::fadeInList(uint8_t step, uint8_t length, uint32_t startScreen, uint8_t* shuffledSequence ){
	

  // uint8_t sequence[32];
	// for (uint8_t i = 0; i < 32; i++) {
	// 	sequence[i] = i;
	// }
	
	// // shuffle in place
	// this->shuffle(sequence, 32);
	
	// fade in effect, enable random segments.
	// uint32_t fullScreen = 0x00000000;
	uint32_t fullScreen = startScreen;
	
	for (uint8_t i=0; i<step;i++){
		fullScreen |= 1UL <<shuffledSequence[i];// 1UL because if just 1 it's a 16 bit constant. (yep yep Lucie, nonkel Lode lost a couple of hours solving this!)
		
	}
  return fullScreen;
}

// void Apps::fadeInList(uint32_t* movie, uint8_t length, uint32_t startScreen, ){
	
//   uint8_t sequence[32];
// 	for (uint8_t i = 0; i < 32; i++) {
// 		sequence[i] = i;
// 	}
	
// 	// shuffle in place
// 	this->shuffle(sequence, 32);
	
// 	// fade in effect, enable random segments.
// 	// uint32_t fullScreen = 0x00000000;
// 	uint32_t fullScreen = startScreen;
	
// 	for (uint8_t i=0; i<32;i++){
// 		fullScreen |= 1UL <<sequence[i];// 1UL because if just 1 it's a 16 bit constant. (yep yep Lucie, nonkel Lode lost a couple of hours solving this!)
// 		movie[i] = fullScreen;
// 	}
// }


void Apps::shuffle(uint8_t* listToShuffle, uint8_t length) {
	//shuffle the array:
	//http://www.geeksforgeeks.org/shuffle-a-given-array/
	for (int i = 0; i < length; i++) {
		uint8_t randomIndex = random( i, length);
		
		// swap contents of current index with content further index.
		uint8_t tmp = listToShuffle[i];
		listToShuffle[i] = listToShuffle[randomIndex];
		listToShuffle[randomIndex] = tmp;
	}
}
