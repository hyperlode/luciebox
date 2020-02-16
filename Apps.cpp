#include "Apps.h"
//#include "Arduino.h"
#include "PretbakSettings.h"
 
Apps::Apps(){
};
 
void Apps::setPeripherals( BinaryInput binaryInputs[], Potentio* potentio, DisplayManagement* ledDisp, Buzzer* buzzer){
  this->buzzer = buzzer;
  this->binaryInputs = binaryInputs;
  this->potentio = potentio;
  this->ledDisp = ledDisp;
}
 
void Apps::appSelector(bool init, uint8_t selector){
 
	if (init){
		// title mode (title screen will be displayed before real app starts)
		this->app_init_mode = true;
	//Serial.println("initapps");
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
	  this->modeSimpleButtonsAndLights(init);	
		  break;
		   
		case 8:
		  this->modeSequencer(init);
		  break;
		   
		case 9:
	  #ifdef ENABLE_REACTION_APP
		  this->modeReactionGame(init);
	  #endif
		  break;
		   
		case 10:
		  this->tiltSwitchTest(init);
		  break;
		   
		case 11:
	  #ifdef ENABLE_MULTITIMER
		  this->miniMultiTimer(init);
	  #endif
		  break;
 
	#ifdef ENABLE_ANALOG_PIN_DEBUG
		case 12:
	  //this is the debug mode
	  this->modeButtonDebug(init);
	  break;
	#endif
		 
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
  buzzer->buzzerOff(); // stop all sounds that were playing in an app. 
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
	for (uint8_t i = 0; i < 32; i++) {
		this->FADE_IN_RANDOM_LIST[i] = i;
	}
 
	// // shuffle in place
	this->shuffle(this->FADE_IN_RANDOM_LIST, 32);
 
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
		ledDisp->SetFourDigits(this->displayAllSegments);
		// ledDisp->showNumber(selector);
		 
	}else if (counter == 50){
		//this->fadeInList(displaySequence, 32, ~this->displayAllSegments);
		 
	}else if (counter < 82 ){
		//ledDisp->SetFourDigits(~displaySequence[counter-51]);
	ledDisp->SetFourDigits(~ this->fadeInList(counter-51, 32, ~this->displayAllSegments, this->FADE_IN_RANDOM_LIST)); 
		 
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
	counter = -1;
  }
   
  if(!generalTimer.getTimeIsNegative()){
	// Serial.println("dbug displ");
	counter++;
	if (counter > 9){
	   counter = 0; 
	}
	 
	// Serial.println(counter);
	textBuf[1]=' ';
	textBuf[2]=' ';
	textBuf[3]='A'; 
	
	switch (counter){
	  // case 0:{
	  //   // textBuf[4]='0'; // analog A0
	  //   // ledDisp->displayHandler(textBuf);
	  //   // generalTimer.setInitTimeMillis((long)-500);
	  //   break; 
	  // }
	  case 1:{
		ledDisp->showNumber( (int16_t) analogRead(PIN_SELECTOR_DIAL));
		// generalTimer.setInitTimeMillis((long)-1000);
		break; 
	  }
	  // case 2:{
	  //   // textBuf[4]='1'; // analog A1
	  //   // ledDisp->displayHandler(textBuf);
	  //   // generalTimer.setInitTimeMillis((long)-500);
	  //   break; 
	  // }
	  case 3:{
		ledDisp->showNumber( (int16_t) analogRead(PIN_BUTTONS_1));
		// generalTimer.setInitTimeMillis((long)-1000);
		break; 
	  }
	  // case 4:{
	  //   // textBuf[4]='2'; // analog A2
	  //   // ledDisp->displayHandler(textBuf);
	  //   // generalTimer.setInitTimeMillis((long)-500);
	  //   break; 
	  // }
	  case 5:{
		ledDisp->showNumber( (int16_t) analogRead(PIN_BUTTONS_2));
		// generalTimer.setInitTimeMillis((long)-1000);
		break; 
	  }
	  // case 6:{
	  //   textBuf[4]='3';// analog A3
	  //   ledDisp->displayHandler(textBuf);
	  //   // generalTimer.setInitTimeMillis((long)-500);
	  //   break; 
	  // }
	  case 7:{
		ledDisp->showNumber( (int16_t) analogRead(PIN_POTENTIO));
		// generalTimer.setInitTimeMillis((long)-1000);
		break; 
	  }
	  // case 8:{
	  //   textBuf[4]='4';// analog A4
	  //   ledDisp->displayHandler(textBuf);
	  //   // generalTimer.setInitTimeMillis((long)-500);
	  //   break; 
	  // }
	  case 9:{
		ledDisp->showNumber( (int16_t) analogRead(PIN_MERCURY_SWITCHES));
		// generalTimer.setInitTimeMillis((long)-1000);
		break; 
	  }
	}
 
	// show menu title (compressed)
	if (counter%2 == 0){
	  // show analog pin
	  textBuf[4] = counter/2 + 48; // char 0 + analog pin . 
	  ledDisp->displayHandler(textBuf);
	}
 
	// show values one seconds, menu items half a second
	generalTimer.setInitTimeMillis((long) (-500 - (counter%2)*500)); 
	 
	generalTimer.start();
  }  
}
 
void Apps::modeDiceRoll(bool init){
	if (init){
		// generalTimer.setInitTimeMillis(-1000);
		// generalTimer.start();
		DICEROLL_CARD_FROM_DECK_INDEX = 0;
	}

	DICEROLL_SECONDARY_OPTION = binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue();
	//   if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){

	// 	generalTimer.setInitTimeMillis((long)(1* potentio->getValueMapped(-1000, 0))); //divided by ten, this way, we can set the timer very accurately as displayed on screen when big red is pressed. *100ms

	//   }else{
	// 	generalTimer.setInitTimeMillis(-1000);

	//   }

	// if (binaryInputs[BUTTON_MOMENTARY_2].getValue() || binaryInputs[BUTTON_MOMENTARY_0].getValue()|| binaryInputs[BUTTON_MOMENTARY_1].getValue()){
		
	// DICEROLL_RANDOM_NUMBER ++;
	// if (DICEROLL_RANDOM_NUMBER > 6){  // NO THIS IS TERRIBLE! about 600 times per second cycling through, so, this is as good as random, or is it? can you time it and predict your roll? I'll leave it in for you to find out!
	//   DICEROLL_RANDOM_NUMBER = 1;
	// }
	
	// if (!generalTimer.getTimeIsNegative()){

	//   if (binaryInputs[BUTTON_MOMENTARY_1].getValue()){   
			
	// 	for (uint8_t  i=1;i<4;i++){
	// 	  textBuf[i]=textBuf[i+1];
	// 	}
	// 	textBuf[4] = 48 + DICEROLL_RANDOM_NUMBER; // convert digit to number char.
	//   }

	//   buzzer->cleanBuzzerRoll();
	//   buzzer->programBuzzerRoll(14);
	//   ledDisp->displayHandler(textBuf);
	//   generalTimer.start();
	// }

	// }
	
	textBuf[1] = ' ';
	textBuf[2] = ' ';
	textBuf[3] = ' ';
	textBuf[4] = ' ';

	for (uint8_t i=0;i<4;i++){
		
		if (binaryInputs[buttons_momentary_indexed[i]].getValue()){
			ledDisp->showNumber(8888);
		}
	}

	if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeDown()){
		// dice 

		if (DICEROLL_SECONDARY_OPTION){
			// throw four dice
			DICEROLL_RANDOM_NUMBER = random (1, 7);
			for (uint8_t  i=1;i<5;i++){
				textBuf[i] = random (49, 55);  // char 1 to 6
			}

		}else{
			DICEROLL_RANDOM_NUMBER = random (1, 7);
			// show dice eyes
			textBuf[4] = ' '; 
			for (uint8_t i=1;i<4;i++){
				// build up dice eyes over three digits 

				//set default for digit 1 2 and 3. Because most used (seg A and D) 
				textBuf[i] = '=';  

				//first and third digit
				if (DICEROLL_RANDOM_NUMBER == 1){
					textBuf[i] = ' '; // 
				}else if(DICEROLL_RANDOM_NUMBER < 4){
					textBuf[i] = '^'; // assume first digit seg A
					if( i == 3){
					textBuf[i] = '_'; // seg D
					}
				}

				//second digit
				if (i == 2 && DICEROLL_RANDOM_NUMBER < 6){
					textBuf[i] = '-'; // assume odd
					if (DICEROLL_RANDOM_NUMBER%2 == 0 ){ // if even
					textBuf[i] = ' '; 
					}
				}
			}
		}
		ledDisp->displayHandler(textBuf);
	}
	
	if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeDown()){
		// cards 

		if (DICEROLL_SECONDARY_OPTION){
			// take card off deck
			if (DICEROLL_CARD_FROM_DECK_INDEX == 0){
				// pick card from stack. --> reshuffle if all gone.
				//shuffle(SIMON_LIST, bytes_list_bufsize);
				for (int i = 0; i < 52; i++) {
					CARDS_DECK[i] = i;
				}
				shuffle(CARDS_DECK, 52);
			}
			DICEROLL_RANDOM_NUMBER = CARDS_DECK[DICEROLL_CARD_FROM_DECK_INDEX];
			DICEROLL_CARD_FROM_DECK_INDEX++;
			if (DICEROLL_CARD_FROM_DECK_INDEX == 52){
				DICEROLL_CARD_FROM_DECK_INDEX = 0;
			}
		}else{
			// random card
			DICEROLL_RANDOM_NUMBER = random (0, 52); // 52 cards
		}
		
		//show playing card
		if (DICEROLL_RANDOM_NUMBER%13 < 9){
			textBuf[2] = DICEROLL_RANDOM_NUMBER%13 + 49;
		}else{
			textBuf[1] = 49;  // 1
			textBuf[2] = (3 - (((DICEROLL_RANDOM_NUMBER)%13) + 1 )%10) + 48;  // 9,10,11,13 to char 0 1 2 3
			// Serial.println(DICEROLL_RANDOM_NUMBER);
		}

		switch (DICEROLL_RANDOM_NUMBER/13){
		case 0:
		textBuf[4]='H';
		break;
		case 1:
		textBuf[4]='D';
		break;
		case 2:
		textBuf[4]='S';
		break;
		case 3:
		textBuf[4]='C';
		break;
		}
		ledDisp->displayHandler(textBuf);
	}
	
	if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeDown()){
		if (DICEROLL_SECONDARY_OPTION){
			// show random nonsense coolness. 
			textBuf[1]='?';
			textBuf[2]='?';
			textBuf[3]='?'; 
			textBuf[4]='?'; 
			ledDisp->displayHandler(textBuf);
			
		}else{
			// random number
			ledDisp->showNumber( random(0,10000));
		}
	}
	
	if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeDown()){
		if (DICEROLL_SECONDARY_OPTION){
			if (random(0,2)){
				textBuf[2]='Y';
				textBuf[3]='E'; 
				textBuf[4]='S'; 
			}else{
				textBuf[2]='N';
				textBuf[3]='O'; 
			}
		}else{
			if (random(0,2)){
				textBuf[1]='H';
				textBuf[2]='E';
				textBuf[3]='A'; 
				textBuf[4]='D'; 
			}else{
				textBuf[1]='T';
				textBuf[2]='A';
				textBuf[3]='I'; 
				textBuf[4]='L'; 
			}
		}
		ledDisp->displayHandler(textBuf);
		
	}


} 
   

 
// void Apps::modeScroll(bool init){
   
//   // display scroll mode
   
//   if (init){
//	 // display scroll mode
// //	scrollBuf[0]='L';
// //	scrollBuf[1]='U';
// //	scrollBuf[2]='C';
// //	scrollBuf[3]='I';
// //	scrollBuf[4]='E';
// //	scrollBuf[5]=' ';
// //	scrollBuf[6]='B';
// //	scrollBuf[7]='A';
// //	scrollBuf[8]='B';
// //	scrollBuf[9]='Y';
// //
 
//	 this->scrollBuf[0]='H';
//	 scrollBuf[1]='A';
//	 scrollBuf[2]='P';
//	 scrollBuf[3]='P';
//	 scrollBuf[4]='Y';
//	 scrollBuf[5]=' ';
//	 scrollBuf[6]='B';
//	 scrollBuf[7]='D';
//	 scrollBuf[8]='A';
//	 scrollBuf[9]='Y';
//	 scrollBuf[10]=' ';
//	 scrollBuf[11]='B';
//	 scrollBuf[12]='R';
//	 scrollBuf[13]='A';
//	 scrollBuf[14]='M';
//	 scrollBuf[15]='Z';
//	 scrollBuf[16]='Y';
//	 //scrollBuf[10]='/0';
//	 ledDisp->dispHandlerWithScroll(scrollBuf, true, false);
	   
	   
//  this->fadeInList(displaySequence, 32, 0);
//  counter = 0;
//  counter2= false;
	 
//  generalTimer.setInitTimeMillis(-200);
//  generalTimer.start();
//   }
//   if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
//		if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
//		  counter++;
			 
//		  // Serial.println(displaySequence[counter], BIN);
//		}
//		if(!generalTimer.getTimeIsNegative()){
//			generalTimer.start();
//			counter++;
//		}
//		if (counter>31){
//			  counter = 0;
//			  this->fadeInList(displaySequence, 32, 0);
//			  counter2  = !counter2;
//		  }
			 
//		if (potentio->getValueStableChangedEdge()){
//		  generalTimer.setInitTimeMillis((long)( potentio->getValueMapped(-1000, 0))); //divided by ten, this way, we can set the timer very accurately as displayed on screen when big red is pressed. *100ms
//		}
		   
//		displayAllSegments = displaySequence[counter];
//		if (counter2){
//			// negative ==> which makes it fade out.
//			displayAllSegments = ~displayAllSegments;
//		}
			   
//		ledDisp->SetFourDigits(displayAllSegments);
 
		  
//	}else{
//		if (!binaryInputs[BUTTON_MOMENTARY_2].getValue()){
//		  ledDisp->doScroll();
//		}
 
//		if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
//		  ledDisp->setScrollSpeed((long)potentio->getValueStable());
//		}else{
//		  ledDisp->setBrightness((byte)(potentio->getValueMapped(0,50)),false);
//		}
//	}
   
// }
 
void Apps::modeSimpleButtonsAndLights(bool init){
  lights = 0b00000000; //reset before switch enquiry
 
  if (init){
	SETTINGS_MODE_SELECTOR = 0;
 
	if(  analogRead(PIN_BUTTONS_1) == 0 &&
		  analogRead(PIN_BUTTONS_2) == 0 &&
		  analogRead(PIN_POTENTIO) == 0){
		  DEBUGMODE_ACTIVATED = 666;
		  // SETTINGS_MODE_DISPLAY_VALUES_BLINK.setInitCountDownTimeSecs(1);
		  SETTINGS_MODE_DISPLAY_VALUES_BLINK.setInitCountDownTimeMillis(-1000);
		  SETTINGS_MODE_DISPLAY_VALUES_BLINK.start();
	}  
  }
 
  // back and forth motion required of the potentio to count up modes
  if (potentio->getValue() < 5 && SETTINGS_MODE_SELECTOR % 2 ==0){
	SETTINGS_MODE_SELECTOR++;
  }else if (potentio->getValue() >1000 && SETTINGS_MODE_SELECTOR % 2 !=0){
	SETTINGS_MODE_SELECTOR++;
  }
 
	textBuf[1]=' ';
	textBuf[2]=' ';
	textBuf[3]=' '; 
	textBuf[4]=' '; 
	
  if (SETTINGS_MODE_SELECTOR < 6){
	 
	  //normal mode
 
	  // simple repetitive, predictive mode.
	  // each button triggers its corresponding light. 
	  // potentio sets display brightness
	  // no buzzer
	  // display lights up a segment for each button.
 
	  bool updateScreen = false;
 
	  //delete all content from screen.
	  ledDisp->setBlankDisplay();	  
	   
	   
	  if (binaryInputs[BUTTON_MOMENTARY_0].getValue()){
		lights|= 1<<LIGHT_RED;
		updateScreen = true;
		SETTINGS_MODE_SELECTOR = 0;
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_2].getValue()){
		lights|= 1<<LIGHT_BLUE;
		updateScreen = true;
		SETTINGS_MODE_SELECTOR = 0;
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_1].getValue()){
		lights|= 1<<LIGHT_GREEN;
		updateScreen = true;
		SETTINGS_MODE_SELECTOR = 0;
	  }
	   
	  #if MOMENTARY_BUTTONS_COUNT == 4
	  if (binaryInputs[BUTTON_MOMENTARY_3].getValue()){
		lights|= 1<<LIGHT_YELLOW_EXTRA;
		updateScreen = true;
		SETTINGS_MODE_SELECTOR = 0;
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
	   
	  
	  ledDisp->setBrightness((byte)(50 - potentio->getValueMapped(0,50)),false);
 
	}else if (SETTINGS_MODE_SELECTOR < 8){
	  lights|= 1<<LIGHT_RED;
	  if(binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
	   #ifdef ENABLE_EEPROM
		 
		eeprom_update_byte(
		  (uint8_t*)EEPROM_SOUND_OFF_BY_DEFAULT,
		  //i,
		  !eeprom_read_byte((uint8_t*)EEPROM_SOUND_OFF_BY_DEFAULT)
		);
		#endif
	   
		if (buzzer->getPin() == PIN_BUZZER){
		  buzzer->setPin(PIN_BUZZER_FAKE);
		}else{
		  buzzer->setPin(PIN_BUZZER);
		}
	  }
 
	  if (SETTINGS_MODE_DISPLAY_VALUES_BLINK.getInFirstGivenHundredsPartOfSecond(500)){
		textBuf[1]='B'; 
		textBuf[2]='E'; 
		textBuf[3]='E'; 
		textBuf[4]='P'; 
 
	  }else{
		textBuf[2]='O'; // On Off o char
		 
		if (buzzer->getPin() == PIN_BUZZER){
		  //ON 
		  //textBuf[1]=' '; 
		  textBuf[3]='N'; 
		  // textBuf[4]=' ';
		}else{
		  // OFF
 
		  // textBuf[1]=' '; 
		  // textBuf[2]='O'; 
		  textBuf[3]='F'; 
		  textBuf[4]='F';
		} 
		ledDisp->displayHandler(textBuf);
	  }
	   
	}else if (SETTINGS_MODE_SELECTOR < 10){
	  textBuf[3]='A'; 
	  textBuf[4]='0'; 
	  ledDisp->showNumber( (int16_t) analogRead(PIN_SELECTOR_DIAL)); 
	 
	}else if (SETTINGS_MODE_SELECTOR < 12){
	  textBuf[3]='A'; 
	  textBuf[4]='1'; 
	  ledDisp->showNumber( (int16_t) analogRead(PIN_BUTTONS_1)); 
	 
	}else if (SETTINGS_MODE_SELECTOR < 14){
	  textBuf[3]='A'; 
	  textBuf[4]='2'; 
	  ledDisp->showNumber( (int16_t) analogRead(PIN_BUTTONS_2));
	 
	}else if (SETTINGS_MODE_SELECTOR < 16){
	  textBuf[3]='A'; 
	  textBuf[4]='3'; 
	  ledDisp->showNumber( (int16_t) analogRead(PIN_POTENTIO));
 
	}else if (SETTINGS_MODE_SELECTOR < 18){
	  textBuf[3]='A'; 
	  textBuf[4]='4'; 
	  ledDisp->showNumber( (int16_t) analogRead(PIN_MERCURY_SWITCHES));
	 
	}else if (SETTINGS_MODE_SELECTOR < 20){
	  if (SETTINGS_MODE_DISPLAY_VALUES_BLINK.getInFirstGivenHundredsPartOfSecond(300)){
		textBuf[1]='E'; 
		textBuf[2]='E'; 
		textBuf[3]='P'; 
		textBuf[4]='R'; 
 
	  }else{
		textBuf[1]='R'; 
		textBuf[2]='S'; 
		textBuf[3]='E'; 
		textBuf[4]='T'; 
		ledDisp->displayHandler(textBuf);
	  }
	}else if (SETTINGS_MODE_SELECTOR < 22){
	  lights|= 1<<LIGHT_RED;
	 if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
		 
		#ifdef ENABLE_EEPROM
		for(uint16_t i = 0;i<100;i=i+2){
		  eeprom_update_word(
			(uint16_t*)i,
			//i,
			0
		  );
		}
		#endif
		textBuf[1]='D'; 
		textBuf[2]='O'; 
		textBuf[3]='N'; 
		textBuf[4]='E'; 
 
 
	  }else{
		textBuf[1]='D'; 
		textBuf[2]='O'; 
		textBuf[3]='I'; 
		textBuf[4]='T'; 
		ledDisp->displayHandler(textBuf);
 
	  }
	}else if (SETTINGS_MODE_SELECTOR < 24){
	 
	  ledDisp->showNumber(SETTINGS_MODE_SELECTOR);
 
 
 
	}else{
	  ledDisp->showNumber(SETTINGS_MODE_SELECTOR);
	}
 
 
	if (SETTINGS_MODE_SELECTOR >= 6){
	  // in real settings mode
 
	  // show menu title (compressed)
	  // if (counter%2 == 0){
	  //   // show analog pin
	  //   textBuf[4] = counter/2 + 48; // char 0 + analog pin . 
	  //   ledDisp->displayHandler(textBuf);
	  // }
 
	   
	  if (SETTINGS_MODE_DISPLAY_VALUES_BLINK.getTimeIsNegative()){
		SETTINGS_MODE_DISPLAY_VALUES_BLINK.start();
	  }
	   
	  if (SETTINGS_MODE_DISPLAY_VALUES_BLINK.getInFirstGivenHundredsPartOfSecond(500)){
		  ledDisp->displayHandler(textBuf);
	  }
 
	  // show values one seconds, menu items half a second
	  //generalTimer.setInitTimeMillis((long) (-500 - (counter%2)*500)); 
	  
 
	}
	ledDisp->SetLedArray(lights);
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
	  if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
		//buzzer->loadBuzzerTrack(song_attack );
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
		//buzzer->loadBuzzerTrack(song_unhappy_dryer);
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
		buzzer->loadBuzzerTrack(scale_major);
	  }
#ifdef BUTTON_MOMENTARY_3
	  if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){
		buzzer->loadBuzzerTrack(scale_pentatonic);
	  }
#endif
 
	}else{
	  if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
		//buzzer->loadBuzzerTrack(song_retreat );
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
		//buzzer->loadBuzzerTrack(song_unhappy_dryer);
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
		buzzer->loadBuzzerTrack(scale_major_reversed);
	  }
#ifdef BUTTON_MOMENTARY_3
	  if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){
		// buzzer->loadBuzzerTrack(scale_pentatonic_reversed);
	  }
#endif
	}
  }else{
	// simple mode: songs!
	if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
	  if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
		buzzer->loadBuzzerTrack(song_retreat );
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
		buzzer->loadBuzzerTrack(kindeke_douwen);
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
		buzzer->loadBuzzerTrack(song_unhappy_dryer);
	  }
	}else{
	  if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
		buzzer->loadBuzzerTrack(song_attack );
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
		buzzer->loadBuzzerTrack(song_lang_zal_ze_leven);
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
		buzzer->loadBuzzerTrack(song_happy_dryer);
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
		//  toggleval==0: 0^0=0, 1^0=1 , 
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
 
#ifdef ENABLE_MULTITIMER
void Apps::miniMultiTimer(bool init){
  // every player: init time, time left, alive? 
  // game: pause, player alive? ,fischertimer active?/time, random starter
 
  if (init){
	  this->multiTimer.setBuzzer(this->buzzer);
	  this->multiTimer.init();
  }  
   
  // TIMER BUTTONS
   
  for (uint8_t i = 0; i<MAX_TIMERS_COUNT; i++){
	if (binaryInputs[buttons_momentary_indexed[i]].getEdgeUp()){
	  this->multiTimer.playerButtonPressEdgeUp(i);
	}
	if (binaryInputs[buttons_momentary_indexed[i]].getEdgeDown()){
	  this->multiTimer.playerButtonPressEdgeDown(i);
	}
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
	bool individualTimerSet = false;
	for (uint8_t i = 0; i<MAX_TIMERS_COUNT; i++){
	  if (binaryInputs[buttons_momentary_indexed[i]].getValue()){
		this->multiTimer.setTimerInitCountTimeSecs(i, seconds);
		individualTimerSet = true;
	  }
	}
	if (!individualTimerSet){
	  this->multiTimer.setAllInitCountDownTimeSecs(seconds);
	}
 
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
	 
}
#endif
 
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
 
	if (binaryInputs[BUTTON_MOMENTARY_0].getValue()){
	  //lower
	  if (potentio->getValueStableChangedEdge()){
		GEIGER_TONE_FREQUENY_LOWEST = potentio->getValueMapped(0,5000);
	  }
	  ledDisp->showNumber(GEIGER_TONE_FREQUENY_LOWEST);
 
	}else if (binaryInputs[BUTTON_MOMENTARY_1].getValue()){
	  //upper
	  if (potentio->getValueStableChangedEdge()){
		GEIGER_TONE_FREQUENCY_HEIGHEST = potentio->getValueMapped(0,5000);
	  }
	  ledDisp->showNumber(GEIGER_TONE_FREQUENCY_HEIGHEST);
 
	}else if (binaryInputs[BUTTON_MOMENTARY_2].getValue()){
	  //length
	  if (potentio->getValueStableChangedEdge()){
		GEIGER_TONE_LENGTH = potentio->getValueMapped(0,256);
	  }
	  ledDisp->showNumber(GEIGER_TONE_LENGTH);
 
#ifdef BUTTON_MOMENTARY_3
	}else if(binaryInputs[BUTTON_MOMENTARY_3].getValue()){
	  if (potentio->getValueStableChangedEdge()){
		buzzer->playTone(
			potentio->getValueMapped(0,500), 
			binaryInputs[BUTTON_LATCHING_EXTRA].getValue()?0:GEIGER_TONE_LENGTH
		  );
	  }
#endif
	}else{
 
	  if (r > GEIGER_PROBABILITY_THRESHOLD){ // 1024*1024
		long tmp  =  random(GEIGER_TONE_FREQUENY_LOWEST, GEIGER_TONE_FREQUENCY_HEIGHEST+1);
		buzzer->playTone(
		  tmp , 
		  binaryInputs[BUTTON_LATCHING_EXTRA].getValue()?0:GEIGER_TONE_LENGTH
		  );
 
		ledDisp->showNumber(tmp);
		COUNTER_GEIGER++;
	  }
 
	  if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
		ledDisp->showNumber(COUNTER_GEIGER);
	  }
	   
	  GEIGER_PROBABILITY_THRESHOLD = potentio->getValueMapped(0,1048576);
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
 
	  textBuf[1]=' '; 
	if (r > potentio->getValueMapped(0,1048576)){
	  //	buzzer->programBuzzerRoll(1); //not beep but "puck"
	  buzzer->playTone((unsigned int)50, 10);
	  textBuf[1]='?';
	 
	}
 
	//all digits same value
	for(uint8_t i=2;i<5;i++){
	  textBuf[i]=textBuf[1];
	}
	 
	ledDisp->displayHandler(textBuf);  
  }
}
 
void Apps::modeSequencer(bool init){
	int8_t step = 0;
 
  if (!binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){ 
	// metrone function is the "easy mode here"
	this->modeMetronome(init);
	return;
  }
	 
  if (init){
	SEQUENCER_STEP_COUNTER = 0; 
	SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET = 0;
	generalTimer.setInitTimeMillis((long)potentio->getValueStable() * -1);
	generalTimer.start();
 
	SEQUENCER_EEPROM_MODE_BLINK.setInitTimeMillis(-1000);
	SEQUENCER_EEPROM_MODE_BLINK.start();
	 
	//resets song.
	for (uint8_t i=0;i<32;i++){
	  this->SEQUENCER_SONG[i] = C7_8;
	}
  }
   
  if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getEdgeDown()){
	init = true; // make sure we display the sequencer when returning from save/load mode
  }
 
  if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){ 
	// load/save songs to the sequencer
	//blink alternatively song number and "load" or "save"
	uint8_t song_number  = potentio->getValueMapped(1,9);
	if (TIMER_REACTION_GAME_RESTART_DELAY.getInFirstGivenHundredsPartOfSecond(500)){
	  ledDisp->showNumber(song_number);
 
	}else{
	   
	  if(binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
		textBuf[1] = 'S';
		textBuf[2] = 'A';
		textBuf[3] = 'V';
		textBuf[4] = 'E';
	   
	  }else{
		textBuf[1] = 'L';
		textBuf[2] = 'O';
		textBuf[3] = 'A';
		textBuf[4] = 'D';
 
	  }
 
	  ledDisp->displayHandler(textBuf);
	}
 
	if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
	  for (uint8_t i=0;i<32;i++){
		uint8_t* eeprom_address = (uint8_t*)
			  (EEPROM_SEQUENCER_SONGS_START_ADDRESS +
			   (song_number - 1) * EEPROM_SEQUENCER_SONG_LENGTH +
			   i			   
			  );
 
		if(binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
		  //save
		  eeprom_write_byte(eeprom_address, this->SEQUENCER_SONG[i]);
 
		}else{
		  //load
		  this->SEQUENCER_SONG[i] = eeprom_read_byte(eeprom_address);
		}
	  }
	}
 
	if (binaryInputs[BUTTON_MOMENTARY_0].getValue()){
	  TIMER_REACTION_GAME_RESTART_DELAY.start();
	}
	 
 
  }else{
	// manipulate the sequencer
 
 
	// visualize programmed note
 
	SEQUENCER_TEMP_NOTE = SEQUENCER_SONG[SEQUENCER_STEP_COUNTER];
 
	if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
	  buzzer->programBuzzerRoll(SEQUENCER_TEMP_NOTE);
	  ledDisp->showNumber(SEQUENCER_TEMP_NOTE);
	}
 
	if (binaryInputs[BUTTON_MOMENTARY_0].getValue()){
	  // if button continuously pressed, show notes.
	  ledDisp->showNumber(SEQUENCER_TEMP_NOTE);
 
	  // bonus effect: TRANSPOSE!
	  if (potentio->getValueStableChangedEdge()){
		SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET += 2 * potentio->getLastStableValueChangedUp() - 1; //step +1 or -1
	  }
	} 
	 
	if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeDown()){
	  ledDisp->SetFourDigits(displayAllSegments);
	}
 
	// just listen to the potentio note
	SEQUENCER_TEMP_NOTE = potentio->getValueMapped(0,255);  
 
	if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
	  buzzer->programBuzzerRoll(SEQUENCER_TEMP_NOTE);
	  ledDisp->showNumber(SEQUENCER_TEMP_NOTE);
	}
	 
	if (binaryInputs[BUTTON_MOMENTARY_1].getValue()){
	  // if button continuously pressed, rotate potentio to hear notes.
	  if (potentio->getValueStableChangedEdge()){
		 
		buzzer->programBuzzerRoll(SEQUENCER_TEMP_NOTE);
		ledDisp->showNumber(SEQUENCER_TEMP_NOTE);
	  }
	} 
	 
	if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeDown()){
	  ledDisp->SetFourDigits(displayAllSegments);
	}
 
	// program note to song
	if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
	   
	  uint8_t note = potentio->getValueMapped(0,255);
	   
	  buzzer->programBuzzerRoll(note);
 
	  if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){ 
		//copy to all measures
		for (uint8_t i=0;i<4;i++){
		  this->SEQUENCER_SONG[(SEQUENCER_STEP_COUNTER%8) + 8*i] = note;
		}
	  }else{
		this->SEQUENCER_SONG[SEQUENCER_STEP_COUNTER] = note;
	  }
	}
	 
#ifdef BUTTON_MOMENTARY_3
	// song progression
	if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){
	  step = 1;
	}
 
	if (binaryInputs[BUTTON_MOMENTARY_3].getValue()){
	  if (potentio->getValueStableChangedEdge()){
		step = 2 * potentio->getLastStableValueChangedUp() - 1; //step +1 or -1
	  }
	}
#endif
 
	// autoplay
	if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
	  // change speed is default behaviour of potentio.
		if (!binaryInputs[BUTTON_MOMENTARY_0].getValue() && 
			!binaryInputs[BUTTON_MOMENTARY_1].getValue() &&
			!binaryInputs[BUTTON_MOMENTARY_2].getValue() &&
#ifdef BUTTON_MOMENTARY_3
			!binaryInputs[BUTTON_MOMENTARY_3].getValue() &&
#endif
			potentio->getValueStableChangedEdge()){
 
			//  generalTimer.setInitTimeMillis(potentio->getValueMapped(-1024,0));
			int8_t tmp = 2 * potentio->getLastStableValueChangedUp() - 1;
			generalTimer.setInitTimeMillis(generalTimer.getInitTimeMillis() + tmp * 10) ; //step +1 or -1
	  }
 
	  if (!generalTimer.getTimeIsNegative()){
		step = 1;
		generalTimer.start();
	  }
	}
	 
	// handle step change
	if (step != 0 || init){
	  SEQUENCER_STEP_COUNTER+=step;
 
	  if (SEQUENCER_STEP_COUNTER < 0){
		SEQUENCER_STEP_COUNTER = 31;
	  }
 
	  if (SEQUENCER_STEP_COUNTER > 31){
		SEQUENCER_STEP_COUNTER = 0;
	  }
	   
	  buzzer->programBuzzerRoll(
		this->SEQUENCER_SONG[SEQUENCER_STEP_COUNTER] + 
		SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET * binaryInputs[BUTTON_MOMENTARY_0].getValue()
		);
	   
	  // sequencer shows every step in 32 notes bar. 8steps (circle) times 4 measures (bar on bottom)
	  displayAllSegments = 0;
	  if (SEQUENCER_STEP_COUNTER % 8 < 4){
		displayAllSegments |=  (uint32_t)0x1 << (8* (SEQUENCER_STEP_COUNTER % 8)) ; 
	  }else{
		displayAllSegments |=  (uint32_t)0x1 << ((8*( 3 - ( SEQUENCER_STEP_COUNTER % 8 -4 ))) + 6); 
	  }
	   
	  displayAllSegments |=  (uint32_t)0x1 << ((8*(SEQUENCER_STEP_COUNTER / 8))+3) ;  // bar at bottom.
	  ledDisp->SetFourDigits(displayAllSegments);
 
	}
  }
}
 
void Apps::modeMetronome(bool init){
  // todo: with extra timer, create slight timing offset in second ticker, for fun effects (zwevingen)!
  bool update = false;
  bool nextStep = false;
   
  if (init){
	METRONOME_TICKER_1_POSITION = 0;
	METRONOME_TICKER_2_POSITION = 0;
	METRONOME_TICKER_3_POSITION = 0;
	TIMER_METRONOME.setInitTimeMillis((long)potentio->getValueStable() * -1);
	TIMER_METRONOME.start();
	update = true;
  }
   
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
	//ticker 1,2 and 3 back together (at position of ticker 1)
	METRONOME_TICKER_2_POSITION = METRONOME_TICKER_1_POSITION;
	METRONOME_TICKER_3_POSITION = METRONOME_TICKER_1_POSITION;
	update = true;
  }
 
  if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
   
	//set ticker 2 one step extra forward .
	METRONOME_TICKER_2_POSITION = this->nextStepRotate(
	  METRONOME_TICKER_2_POSITION, 
	  !binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue(), 
	  // true,
	  0, 
	  11);
	update = true;
  }
   
 
  if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
	//set ticker 3 one step extra forward .
	METRONOME_TICKER_3_POSITION = this->nextStepRotate(
	  METRONOME_TICKER_3_POSITION, 
	  !binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue(), 
	  // true,
	  0, 
	  11);
	update = true;
  }
 
#ifdef BUTTON_MOMENTARY_3
  if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){
	update = true;
	nextStep = true;
  }
#endif
 
  if (update){
	 
 
	if (nextStep){
	  METRONOME_TICKER_1_POSITION = this->nextStepRotate(
		METRONOME_TICKER_1_POSITION, 
		true,
		0, 
		11);
 
	  METRONOME_TICKER_2_POSITION = this->nextStepRotate(
		METRONOME_TICKER_2_POSITION, 
		!binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue(), 
		// true,
		0, 
		11);
	  METRONOME_TICKER_3_POSITION = this->nextStepRotate(
		METRONOME_TICKER_3_POSITION, 
		!binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue(), 
		// true, 
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
 
	uint32_t screen = 0;
	for (uint8_t i=0;i<4;i++){
	  screen |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + METRONOME_TICKER_1_POSITION*4 + (i)) << (8*i); //* 4 --> 4 bytes per dword
	  screen |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + METRONOME_TICKER_2_POSITION*4 + (i)) << (8*i); 
	  screen |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + METRONOME_TICKER_3_POSITION*4 + (i)) << (8*i); 
	}
	ledDisp->SetFourDigits(screen);
 
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
  #ifdef ENABLE_REACTION_APP
  //yellow button active at start: yellow button is also a guess button
  // big red active: timed game
  // small red right active: time progressively shorter as game advances
  // small red left active: play by sound.
   
  if (init){
	randomSeed(millis());
	reactionGameState = reactionWaitForStart;
	displayAllSegments = 0x0;
	TIMER_REACTION_GAME_RESTART_DELAY.setInitTimeMillis(0);
		 
	TIMER_REACTION_GAME_RESTART_DELAY.start();
 
  }
 
  // at any time, leave game when depressing play button.
  if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeDown() ){
	reactionGameState = reactionWaitForStart;
  }
  
  switch (reactionGameState) {
 
	case reactionWaitForStart: {
 
	  // change level
	  REACTION_GAME_LEVEL = (potentio->getValueMapped(1,5)); // only set the default inittime at selecting the game. If multiple games are played, init time stays the same.
	  if (potentio->getValueStableChangedEdge()){
		TIMER_REACTION_GAME_RESTART_DELAY.start();
	  }
 
	  // special mode for the reaction game: limited total time during which as many points should be collected. 
	  REACTION_COUNTDOWN_MODE = binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue();
	  REACTION_GUITAR_HERO_MODE = binaryInputs[BUTTON_LATCHING_BIG_RED].getValue();
 
	  // guitar hero and countdown mode should never occure at the same time!
	  if (REACTION_GUITAR_HERO_MODE && REACTION_COUNTDOWN_MODE){
		// error!!!
		REACTION_COUNTDOWN_MODE = false; // no reaction count downmode for guitar hero.
	  }
 
	  // display level and high score
	  #ifdef ENABLE_EEPROM
	  if (TIMER_REACTION_GAME_RESTART_DELAY.getInFirstGivenHundredsPartOfSecond(500)){
		intToDigitsString(textBuf+1, REACTION_GAME_LEVEL, false);  // utilities lode
		textBuf[1] = 'L';
		ledDisp->displayHandler(textBuf);
	  }else{
		//ledDisp->setBlankDisplay(); //make high score blink
		 
 
		ledDisp->showNumber(
		  eeprom_read_word(
			  (uint16_t*)
			  (EEPROM_REACTION_GAME_START_ADDRESS +
			   REACTION_GAME_LEVEL * 2 +
			   EEPROM_REACTION_GAME_GUITAR_HERO_EXTRA_OFFSET * REACTION_GUITAR_HERO_MODE +
			   EEPROM_REACTION_GAME_COUNTDOWN_MODE_OFFSET * REACTION_COUNTDOWN_MODE
			  )
			)
		  );
	  }			
	  #else
	  intToDigitsString(textBuf+1, REACTION_GAME_LEVEL, false);  // utilities lode
	  textBuf[1] = 'L';
	  ledDisp->displayHandler(textBuf);
	  #endif
 
		 
	  // play game button pressed
	  if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeUp() ){
		reactionGameState = reactionNewGame;
 
		if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
		  //play by sound
		  for (uint8_t i=0;i<MOMENTARY_BUTTONS_COUNT;i++){
			 
			REACTION_GAME_SELECTED_SOUNDS[i] = (uint8_t)random(234, 245);
			 
			 
			for (uint8_t j=0;j<i;j++){
			  // never twice the same sound
			  if (REACTION_GAME_SELECTED_SOUNDS[j] == REACTION_GAME_SELECTED_SOUNDS[i]){
				i--;
			  }
			}
		  }
		}
	  } 
 
	  break;
	}
 
	case reactionNewGame: {
	   
	  REACTION_GAME_SCORE = 0;
 
	  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
 
		reactionGameState = reactionMultiNewTurn;
		displayAllSegments = 0;
 
		REACTION_GAME_STEP_TIME_MILLIS = (6-REACTION_GAME_LEVEL) * -200;
		 
	  }else{
		if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
		  // let them all play so the player gets a feel for them.
		  for (uint8_t i=0;i<MOMENTARY_BUTTONS_COUNT;i++){
			buzzer->programBuzzerRoll(REACTION_GAME_SELECTED_SOUNDS[i]);
			buzzer->programBuzzerRoll(rest_1);
		  }
 
		  buzzer->programBuzzerRoll(rest_1);
		  buzzer->programBuzzerRoll(rest_1);
		}  
 
		reactionGameState = reactionNewTurn;
 
		if (REACTION_COUNTDOWN_MODE){
		  // if enabled, we go for "as many points in a limited time. --> this to make it more exciting for adults (can be boring after a while if you just have to press the right button in time)
		  REACTION_GAME_STEP_TIME_MILLIS = (1UL << (REACTION_GAME_LEVEL )) * -4000; // step speed depending on level
 
		}else{
		  REACTION_GAME_STEP_TIME_MILLIS = (1UL << (6 - REACTION_GAME_LEVEL)) * -35; // step speed depending on level
		}
 
	  }
 
	  TIMER_REACTION_GAME_SPEED.setInitTimeMillis(REACTION_GAME_STEP_TIME_MILLIS);
	  TIMER_REACTION_GAME_SPEED.start(); 
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
		new_segment |= ((displayAllSegments >> (8*i)) & 0b00000001) << 6;// A segment, ON or OFF?, move to G segment
 
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
			buzzer->addRandomSoundToRoll(190,198);
			displayAllSegments &= ~(0x80UL << 8*i);
			REACTION_GAME_SCORE++; 
			//buzzer->programBuzzerRoll(rest_1);
			//buzzer->programBuzzerRoll(C5_1);
			 
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
	  
	  if (!REACTION_COUNTDOWN_MODE){
		
		REACTION_GAME_TIMER_STEP= 0; //reset animation step
		TIMER_REACTION_GAME_SPEED.start(); // only restart if 12 steps time per turn
	  }
	  ledDisp->setBlankDisplay(); // yes, this is needed.
	  displayAllSegments = 0; //reset animation graphics screen
	   
	  lights = 0b00000000; //reset before switch enquiry
	   
	  REACTION_GAME_TARGET = random(0, MOMENTARY_BUTTONS_COUNT);
	   
	   
	   
 
	  if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
		 
		//play by sounds
		for (uint8_t j=0; j<4;j++){
		  buzzer->programBuzzerRoll(rest_1);
 
		}
		buzzer->programBuzzerRoll(REACTION_GAME_SELECTED_SOUNDS[REACTION_GAME_TARGET]);
 
	  }else{
		lights |= 1<<lights_indexed[REACTION_GAME_TARGET];	 
	  }
 
	  ledDisp->SetLedArray(lights); 
 
			
 
		   
	 
	  reactionGameState = reactionPlaying;
	  break;
	}
 
 
 
 
	case reactionPlaying: {
	   
	   
	  if (REACTION_COUNTDOWN_MODE){
		// if timer out, always dead.
		if (!TIMER_REACTION_GAME_SPEED.getTimeIsNegative()){
		   reactionGameState = reactionJustDied;
		}
		// show number of segments:
		REACTION_GAME_TIMER_STEP = 10 - (uint16_t) ((float)TIMER_REACTION_GAME_SPEED.getTimeMillis() / TIMER_REACTION_GAME_SPEED.getInitTimeMillis() * 11);
		// REACTION_GAME_TIMER_STEP = 11;
		for (uint8_t step=0;step<=REACTION_GAME_TIMER_STEP ;step++){
		 for (uint8_t i=0;i<4;i++){
			displayAllSegments |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + step*4 + (i)) << (8*i); 
		  }
		}
		ledDisp->SetFourDigits(displayAllSegments);
 
	  }else{
		// animation next step
	   
		if (!TIMER_REACTION_GAME_SPEED.getTimeIsNegative()){
		  // game timing animation update.
		  for (uint8_t i=0;i<4;i++){
			displayAllSegments |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + REACTION_GAME_TIMER_STEP*4 + (i)) << (8*i); 
		  }
		  ledDisp->SetFourDigits(displayAllSegments);
		   
		  REACTION_GAME_TIMER_STEP = this->nextStepRotate(REACTION_GAME_TIMER_STEP, true, 0, 12);
		   
		  // check game status 'dead'
		  if (REACTION_GAME_TIMER_STEP == 12){
			REACTION_GAME_TIMER_STEP = 0;
			displayAllSegments=0;	
			 
			reactionGameState = reactionJustDied;
		   
		  }else{
			TIMER_REACTION_GAME_SPEED.start();
		  }   
		}
	  }
 
	  // set decimal point as "button lights" helper, in bright daylight button lights might not be visible.
 
	  if ( !binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue() ){
		//always show unless in soundmode
		ledDisp->setDecimalPoint(true, REACTION_GAME_TARGET+1);
	  }
 
	  // check player pressed a button. 
	  for (uint8_t i=0;i<MOMENTARY_BUTTONS_COUNT;i++){
 
		// button press
		if (binaryInputs[buttons_momentary_indexed[i]].getEdgeUp()){
		   
		  if (i==REACTION_GAME_TARGET){
			//right button
			REACTION_GAME_SCORE++;
			reactionGameState = reactionNewTurn;
 
		  }else{
			//wrong button
			reactionGameState = reactionJustDied;
		  }
		}
	  }
 
	  break;
	}
	 
	case reactionJustDied:{
	   
	  #ifdef ENABLE_EEPROM
 
	  //start high score end timer
	  if (REACTION_GAME_SCORE > (int16_t)
			eeprom_read_word(
				  (uint16_t*)
				  (EEPROM_REACTION_GAME_START_ADDRESS +
				  REACTION_GAME_LEVEL*2 +
				  EEPROM_REACTION_GAME_GUITAR_HERO_EXTRA_OFFSET * REACTION_GUITAR_HERO_MODE +
				  EEPROM_REACTION_GAME_COUNTDOWN_MODE_OFFSET * REACTION_COUNTDOWN_MODE
				  )
			)
	  )	
	  {
		eeprom_update_word(
		  (uint16_t*)
		  (EEPROM_REACTION_GAME_START_ADDRESS +
		  REACTION_GAME_LEVEL*2 +
		  EEPROM_REACTION_GAME_GUITAR_HERO_EXTRA_OFFSET * REACTION_GUITAR_HERO_MODE +
		  EEPROM_REACTION_GAME_COUNTDOWN_MODE_OFFSET * REACTION_COUNTDOWN_MODE
		  ),
		  REACTION_GAME_SCORE
		);
 
		buzzer->loadBuzzerTrack(song_attack );
	  }
	  #endif
 
	  TIMER_REACTION_GAME_RESTART_DELAY.setInitTimeMillis(-2000);
	  TIMER_REACTION_GAME_RESTART_DELAY.start();
		 
	  // play death song
	  buzzer->programBuzzerRoll(F4_1);  
	  buzzer->programBuzzerRoll(F4_1);  
	  buzzer->programBuzzerRoll(F4_1);  
 
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
			ledDisp->showNumber(REACTION_GAME_SCORE); //score display. Leave at beginning, to display high score blinking.
			// intToDigitsString(textBuf+1, REACTION_GAME_SCORE, false);  // utilities lode
			// ledDisp->displayHandler(textBuf);
		}
	  }
	  break;
	}
  }
  #endif
}
 
uint32_t Apps::fadeInList(uint8_t step, uint8_t length, uint32_t startScreen, uint8_t* shuffledSequence ){
	 
 
  // uint8_t sequence[32];
	// for (uint8_t i = 0; i < 32; i++) {
	//  sequence[i] = i;
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