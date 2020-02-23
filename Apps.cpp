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
	}
	 
	if (this->app_init_mode){
		 
		if (this->init_app(init, selector)){ 
			// do init routine (showing splash screen), if finished,end of init. Then continue to init of the chosen application
			this->app_init_mode = false;
			init = true;
		}
	}
	 
	// not as else statement, to have the init properly transferred after app beginning screen.
	if (!this->app_init_mode){
		 
		bool advancedMode = binaryInputs[BUTTON_LATCHING_BIG_RED].getValue();
		bool advancedInit = init || binaryInputs[BUTTON_LATCHING_BIG_RED].getValueChanged();
		

		switch (selector) {
		case 0:{
			if (advancedMode){
				#ifdef SIMON_APP
				this->modeSimon(advancedInit);
				#endif

			}else{
				this->modeCountingLettersAndChars(advancedInit);
			}
		  }
		  break;
		   
		case 1:{
		  //sound fun with notes
		  	if (advancedMode){
				this->modeComposeSong(advancedInit);
			}else{
		  		this->modeSoundNotes(advancedInit);
			}
		  }
		  break;
		   
		case 2:
		  if (advancedMode){
			pomodoroTimer(advancedInit);
		  }else{
		  	stopwatch(advancedInit);
		  }

		  break;

		case 3:
		  //this->modeScroll(init);
	      this->modeDiceRoll(advancedInit);
		  break;
		   
		case 4:
		  this->modeGeiger(advancedInit);
		  break;
		   
		case 5:
		  this->modeSoundSong(advancedInit);
		  break;
		   
		case 6:
		  this->draw(advancedInit);
		  break;
		   
		case 7:
	 	  this->modeSimpleButtonsAndLights(advancedInit);	
		  break;
		   
		case 8:
		  if (advancedMode){
		  	this->modeSequencer(advancedInit);

		  }else{
			this->modeMetronome(advancedInit);
		  }
		  break;
		   
		case 9:
		  #ifdef ENABLE_REACTION_APP
		  this->modeReactionGame(advancedInit);
	 	  #endif
		  break;
		   
		case 10:
		  this->tiltSwitchTest(advancedInit);
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
  decimalPoints = 0;  // set all decimal points off. segment 4 = bit 3, ....   00043210 (segment number)
  //ledDisp->SetFourDigits(0xC0C0C0C0); 
  ledDisp->setBrightness(0,false);
  
  //buzzer
  buzzer->setSpeedRatio(2);
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

void Apps::pomodoroTimer(bool init){

	uint8_t display_mode = POMODORO_DISPLAY_TIMER;
	lights = 0;

	if (init){
		POMODORO_INIT_TIME_SECONDS = POMODORO_INIT_DEFAULT_SECS;
		POMODORO_PAUSE_TIME_SECONDS = POMODORO_PAUSE_DEFAULT_SECS;

		// most variables set at main menu init.
		POMODORO_SHOW_MENU_EDGE = false; // at init make sure to run through main menu once to set all varialbes that are always set at main menu entering.

		POMODORO_STATS_WORKING_GOOD = 0;
		POMODORO_STATS_WORKING_BAD = 0;
		POMODORO_PROBABILITY_BEEP_EVERY_SECONDS = 0; // zero means disabled.

	}
	boolean showMenu = true;
	if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue() && 
		!init && 
		(POMODORO_AUTO_RESTART_ENABLED || (!POMODORO_IN_BREAK) ) // if not auto restart, we don't even do a break
	   )
	{
		showMenu = false;
	}

	if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeUp()){
		POMODORO_IN_BREAK = false;
	} 

	// in main menu or timing? (run main menu at least once at init. Even when start button started) to initialize variables depending on settings latching buttons
	if (!showMenu){
		// pomodoro timer running

		if (POMODORO_SHOW_MENU_EDGE){
			POMODORO_TIMER.start();
		}

		if (!POMODORO_TIMER.getTimeIsNegative()){
			POMODORO_IN_BREAK = !POMODORO_IN_BREAK;
			if (POMODORO_IN_BREAK){
				POMODORO_TIMER.setInitCountDownTimeSecs(POMODORO_PAUSE_TIME_SECONDS);
				buzzer->loadBuzzerTrack(song_happy_dryer);
				POMODORO_TIMER.start();
				
			}else{
				// coming out of break. Not executed at starting Pomodoro by switch.
				buzzer->loadBuzzerTrack(song_attack);
				POMODORO_TIMER.setInitCountDownTimeSecs(POMODORO_INIT_TIME_SECONDS);
				if (POMODORO_AUTO_RESTART_ENABLED){
					POMODORO_TIMER.start();
				}else{
					POMODORO_SHOW_MENU_EDGE = true;
				}
			}
		}

		if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){
			POMODORO_STATS_WORKING_BAD ++;
		}
		if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
			POMODORO_STATS_WORKING_GOOD ++;
		}

		if (binaryInputs[BUTTON_MOMENTARY_1].getValue() ||
			binaryInputs[BUTTON_MOMENTARY_3].getValue()
			){
			display_mode = POMODORO_DISPLAY_SHOW_BAD;
		}

		if (binaryInputs[BUTTON_MOMENTARY_0].getValue() ||
			binaryInputs[BUTTON_MOMENTARY_2].getValue()
			){
			display_mode = POMODORO_DISPLAY_SHOW_GOOD;
		}

		


	}else{
		// in main menu

		if (!POMODORO_SHOW_MENU_EDGE){
			POMODORO_TIMER.reset();
			POMODORO_TIMER.setInitCountDownTimeSecs( POMODORO_INIT_TIME_SECONDS);
		}

		uint16_t tmpSeconds = this->multiTimer.getIndexedTime(potentio->getValueMapped(0,91));
		if (binaryInputs[BUTTON_MOMENTARY_1].getValue()){
			POMODORO_INIT_TIME_SECONDS = tmpSeconds;
			POMODORO_TIMER.setInitCountDownTimeSecs( POMODORO_INIT_TIME_SECONDS);
		}

		if (binaryInputs[BUTTON_MOMENTARY_0].getValue()){
			POMODORO_PAUSE_TIME_SECONDS = tmpSeconds;
			display_mode = POMODORO_DISPLAY_PAUSE_INIT_SECS;
		}

		if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
			POMODORO_PROBABILITY_BEEP_EVERY_SECONDS = tmpSeconds;
			display_mode = POMODORO_DISPLAY_BEEP_PROBABILITY;
		}

		if (binaryInputs[BUTTON_MOMENTARY_2].getValue()){
			display_mode = POMODORO_DISPLAY_SHOW_GOOD;
		}
		
		if (binaryInputs[BUTTON_MOMENTARY_3].getValue()){
			display_mode = POMODORO_DISPLAY_SHOW_BAD;
		}
		
		

	}
	
	POMODORO_AUTO_RESTART_ENABLED = binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue();

	// POMODORO_SHOW_MENU_EDGE is our edge detector
	POMODORO_SHOW_MENU_EDGE = showMenu;
	
	// ticking sound
	long tick_duration = potentio->getValueMapped(0,40);
	bool tick_twice_a_second = tick_duration>20;
	if(POMODORO_TIMER.getEdgeSinceLastCallFirstGivenHundredsPartOfSecond(500, true, tick_twice_a_second)){	
		
		if (POMODORO_PROBABILITY_BEEP_EVERY_SECONDS > 0){
	
			// on average every two minutes play beep
			// if ( random(0,21) > 19 + (tick_twice_a_second) ){
			if ( random(0,POMODORO_PROBABILITY_BEEP_EVERY_SECONDS*2) <= 1 - (tick_twice_a_second) ){
				// random has 0 included. as we have to take into account the double ticking, 
				// calculate the probability in half a seconds. i.e.  one every minute: (0,120)

				buzzer->programBuzzerRoll(C5_1);
			};
		}

		if (tick_duration > 0){
			// no sound when zero

			// twenty settings, for each mode (two ticks or one tick per second)
			if (tick_duration > 20){
				tick_duration -= 20;
			}
			if (buzzer->getBuzzerRollEmpty()){ 
				// if alarm sounds, no ticking!
				buzzer->playTone(8000,tick_duration); // works well
			}
		}
	}
	// display
	switch(display_mode){
		case POMODORO_DISPLAY_TIMER:
			POMODORO_TIMER.getTimeString(textBuf+1);
			break;
			
		case POMODORO_DISPLAY_PAUSE_INIT_SECS:{
			timeSecondsToClockString(textBuf+1, POMODORO_PAUSE_TIME_SECONDS);
			if (millis()%1000 > 650  ){
				textBuf[1]='P';
				textBuf[2]='A';
				textBuf[3]='U';
				textBuf[4]='S';
			}
		}
		break;
		case POMODORO_DISPLAY_BEEP_PROBABILITY:{
			timeSecondsToClockString(textBuf+1, POMODORO_PROBABILITY_BEEP_EVERY_SECONDS);
			if (millis()%1000 > 650  ){
				// rnd beep time....
				textBuf[1]='R';
				textBuf[2]='N';
				textBuf[3]='D';
				textBuf[4]='B';
			}
		}
		break;
		case POMODORO_DISPLAY_SHOW_GOOD:{
			if (millis()%1000 > 650  ){
				textBuf[1]=' ';
				textBuf[2]='Y';
				textBuf[3]='E';
				textBuf[4]='S';
			}else{
				ledDisp->numberToBuf(textBuf, POMODORO_STATS_WORKING_GOOD);
			}
		}
		break;

		case POMODORO_DISPLAY_SHOW_BAD:{
			if (millis()%1000 > 650 ){
				textBuf[1]=' ';
				textBuf[2]='N';
				textBuf[3]='O';
				textBuf[4]=' ';
			}else{
				ledDisp->numberToBuf(textBuf, POMODORO_STATS_WORKING_BAD);
			}
		}
		break;

		default:
			break;
	}

	decimalPoints = 0;
	if (showMenu){
		decimalPoints = 1 << 2;

		if (millis()%500 < 250){
			lights|= 1<<LIGHT_LATCHING_EXTRA;
		}

	}else if (POMODORO_TIMER.getInFirstGivenHundredsPartOfSecond(500)){
		decimalPoints = 1 << 2;
		lights|= 1<<LIGHT_LATCHING_EXTRA;

	}else if (POMODORO_IN_BREAK){
		textBuf[1]='P';
		textBuf[2]='A';
		textBuf[3]='U';
		textBuf[4]='S';
		
	}


// decimalPoints = 1 << 2;
// 	//lights|= 1<<LIGHT_LATCHING_EXTRA;
// 	if (showMenu){
		
// 	}else if (POMODORO_TIMER.getInFirstGivenHundredsPartOfSecond(500)){
// 		//decimalPoints = 1 << 2;

// 	}else if  (POMODORO_IN_BREAK){
// 		textBuf[1]='P';
// 		textBuf[2]='A';
// 		textBuf[3]='U';
// 		textBuf[4]='S';
// 		decimalPoints = 0;
// 	}else{
// 		decimalPoints = 0;
// 	}



	// leds
	
	if (POMODORO_PROBABILITY_BEEP_EVERY_SECONDS > 0){
		lights|= 1<<LIGHT_LATCHING_SMALL_LEFT;
	}
	if (POMODORO_AUTO_RESTART_ENABLED){
		lights|= 1<<LIGHT_LATCHING_SMALL_RIGHT;
	}
	if (!showMenu){
		lights|= 1<<LIGHT_LATCHING_EXTRA;
	}
	
	ledDisp->displaySetTextAndDecimalPoints(textBuf, &decimalPoints);
	ledDisp->SetLedArray(lights);
}

void Apps::stopwatch(bool init){
	// classic stopwatch

	if (init){
		STOPWATCH_LAP_MEMORY= 0;
		
		STOPWATCH_CHRONO.setInitTimeMillis(0);
		STOPWATCH_CHRONO.reset();
	}

	long time_millis = 0;
	if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeDown()){
		STOPWATCH_CHRONO.pause();
	}

	if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeUp()){
		STOPWATCH_CHRONO.startPaused(false);
	}

	if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
		// save and show laptime at press
		STOPWATCH_LAP_MEMORY =  STOPWATCH_CHRONO.getTimeMillis(); 
	}

	if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
		// reset
		// STOPWATCH_CHRONO.reset();
		STOPWATCH_CHRONO.startPaused(STOPWATCH_CHRONO.getIsPaused());
	}

	if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){
		STOPWATCH_CHRONO.paused(!STOPWATCH_CHRONO.getIsPaused());
	}

	time_millis = STOPWATCH_CHRONO.getTimeMillis();

	if (binaryInputs[BUTTON_MOMENTARY_0].getValue() || 
		binaryInputs[BUTTON_MOMENTARY_1].getValue() ){
		// show saved laptime at press
		time_millis = STOPWATCH_LAP_MEMORY;
	}

	uint8_t timeDisplayShift = 0;
	
	if (time_millis < 10000){
		timeDisplayShift = 1;
	}else if (time_millis < 100000){
		time_millis/=10;
		timeDisplayShift = 2;
	}else if (time_millis < 1000000){
		time_millis/=100;
		timeDisplayShift = 3;
	}else{
		time_millis/=1000;
		timeDisplayShift = 4;
	}

	textBuf[1] = ' ';
	textBuf[2] = ' ';

	intToDigitsString(textBuf+1, time_millis, true);
	decimalPoints = 0x1<<timeDisplayShift;

	if(binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
		//timeSecondsToClockString(textBuf, (unsigned int)(time_millis/1000));
		STOPWATCH_CHRONO.getTimeString(textBuf+1);
	}else{
		//
		ledDisp->displaySetTextAndDecimalPoints(textBuf, &decimalPoints);
	}
	
	
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
	counter++;
	if (counter > 9){
	   counter = 0; 
	}
	 
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
		// DICEROLL_ROLL_SPEED.setInitTimeMillis(-100);
		// DICEROLL_ROLL_SPEED.start();

		DICEROLL_CARD_FROM_DECK_INDEX = 0;
		diceRollState = dicerollIdle;
		DICEROLL_RANDOM_TYPE = 0;
		randomModeDisplay(false);
		decimalPoints = 0;
		// DICEROLL_ANIMATION_DELAY = 14;
	}

	if (potentio->getValueStableChangedEdge()){
		if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
			// set auto draw time seconds
			int16_t delay_seconds =  potentio->getValueMapped(1, 300);
			ledDisp->showNumber(delay_seconds);
			DICEROLL_AUTODRAW_DELAY.setInitTimeMillis(-1000 * delay_seconds );

		}else if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
			//set animation speed time
			// DICEROLL_ANIMATION_DELAY =  potentio->getValueMapped(0, 50);
			// ledDisp->showNumber(DICEROLL_ANIMATION_DELAY);
			
		}else{
			// display random
			randomModeDisplay(false);
			buzzer->programBuzzerRoll( C7_8);
		}
	}

	switch(diceRollState){
		
		case dicerollIdle:{
			for (uint8_t i=0;i<MOMENTARY_BUTTONS_COUNT;i++){
				if ( binaryInputs[buttons_momentary_indexed[i]].getEdgeUp()){
					DICEROLL_RANDOM_TYPE = i + 10 * binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue();
					diceRollState = dicerollRolling;
					
					// set up animation
					DICEROLL_ROLL_SPEED.setInitTimeMillis(-30);
					DICEROLL_ROLL_SPEED.start();
				}
			}
		}
		break;
			
		case dicerollRolling:{
			// during roll all lights on

			if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){

				if (!DICEROLL_ROLL_SPEED.getTimeIsNegative()){
					buzzer->programBuzzerRoll( C7_8);
					randomModeDisplay(false);

					DICEROLL_ROLL_SPEED.start();
				}
			
			}else{
				ledDisp->showNumber(8888);
			}

			bool roll_end = false;
			if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
				// autoroll no need for button
				roll_end = true;

			}else{
				// wait for button release
				for (uint8_t i=0;i<MOMENTARY_BUTTONS_COUNT;i++){
					
					if (binaryInputs[buttons_momentary_indexed[i]].getEdgeDown()){
						roll_end = true;
					}
				}
			}

			if (roll_end){
				if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
					diceRollState = dicerollRollingEnd; 
				}else{
					diceRollState = dicerollShowResult;
					buzzer->programBuzzerRoll( D4_8);
				}
			}
		}
		break;

		case dicerollRollingEnd:{
			
			if (!DICEROLL_ROLL_SPEED.getTimeIsNegative()){
				buzzer->programBuzzerRoll( C7_8);
				randomModeDisplay(false);

				// roll slower and slower until threshold reached.
				// DICEROLL_ROLL_SPEED.setInitTimeMillis(DICEROLL_ROLL_SPEED.getInitTimeMillis() * ((float)DICEROLL_ANIMATION_DELAY)/10 ); //1.5 //1.4
				DICEROLL_ROLL_SPEED.setInitTimeMillis(DICEROLL_ROLL_SPEED.getInitTimeMillis() * 1.4 ); //1.5 //1.4
				if (DICEROLL_ROLL_SPEED.getInitTimeMillis() < -600){  //-800 //-600
				// if (DICEROLL_ROLL_SPEED.getInitTimeMillis() < -100*DICEROLL_ANIMATION_DELAY){  //-800 //-600
					diceRollState = dicerollShowResult;
				}
			
				DICEROLL_ROLL_SPEED.start();
			}
		}
		break;
			
		case dicerollShowResult:{
			
			randomModeDisplay(true);
			if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
				// auto roll delay.
				DICEROLL_AUTODRAW_DELAY.start();	
				diceRollState = dicerollAutoRollDelay;

			}else{
				diceRollState = dicerollIdle;
			}
			
		}
		break;

		case dicerollAutoRollDelay:{
			if (!DICEROLL_AUTODRAW_DELAY.getTimeIsNegative()){
				// set up animation
				DICEROLL_ROLL_SPEED.setInitTimeMillis(-30);
				DICEROLL_ROLL_SPEED.start();
				
				diceRollState = dicerollRolling;
			}
			if (!binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
				diceRollState = dicerollIdle;
			}
		}

		break;
	}
} 
   
void Apps::randomModeDisplay(bool forReal){
	// forReal: if false, just for animations. Important for i.e. drawing a card from the deck. During animations, we're not really drawing a card from the deck.
	textBuf[1] = ' ';
	textBuf[2] = ' ';
	textBuf[3] = ' ';
	textBuf[4] = ' ';

	switch (DICEROLL_RANDOM_TYPE){

		case DICEROLL_ROLLONEDICE: {
			DICEROLL_RANDOM_NUMBER = random (1, 7);
			// show dice eyes
			//textBuf[4] = ' '; 
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
		break;

		case DICEROLL_ROLLFOURDICE: {
			// throw four dice
			DICEROLL_RANDOM_NUMBER = random (1, 7);
			for (uint8_t  i=1;i<5;i++){
				textBuf[i] = random (49, 55);  // char 1 to 6
			}
		}
		break;

		case DICEROLL_TAKERANDOMCARDFROMDECK: {

			if (!forReal){
				// don't draw a card if it's not for real. We will not even display a card, as that would be confusing. Just show blanks.
				break;
			}
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
		
		}
		// NO BREAK, fallthrough to show card!!!!
		
		case DICEROLL_TAKERANDOMCARD: {

			if (DICEROLL_RANDOM_TYPE != DICEROLL_TAKERANDOMCARDFROMDECK){ // fall through from random card.
				// random card
				DICEROLL_RANDOM_NUMBER = random (0, 52); // 52 cards
			}

			//show playing card
			if (DICEROLL_RANDOM_NUMBER%13 < 9){
				textBuf[2] = DICEROLL_RANDOM_NUMBER%13 + 49;
			}else{
				textBuf[1] = 49;  // 1
				textBuf[2] = (3 - (((DICEROLL_RANDOM_NUMBER)%13) + 1 )%10) + 48;  // 9,10,11,13 to char 0 1 2 3
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

		}
		
		break;

		case DICEROLL_RANDOMNUMBER: {
			// random number
		
			ledDisp->numberToBuf(textBuf, random(0,10000));
		}
		break;
		case DICEROLL_RANDOMLETTER: {
			// show letter alphabeth, plus its position.
			DICEROLL_RANDOM_NUMBER = random(0,26);
			if (DICEROLL_RANDOM_NUMBER > 8){
				textBuf[1] = (DICEROLL_RANDOM_NUMBER + 1) / 10 + 48;
			}
			textBuf[2] = (DICEROLL_RANDOM_NUMBER +1) % 10 + 48;
			textBuf[4] = DICEROLL_RANDOM_NUMBER + 65; // show letters alphabet.
			ledDisp->displayHandler(textBuf);
		}
		break;
		case DICEROLL_HEADSORTAILS: {
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
		break;
		case DICEROLL_YESORNO: {
			if (random(0,2)){
				textBuf[2]='Y';
				textBuf[3]='E'; 
				textBuf[4]='S'; 
			}else{
				textBuf[2]='N';
				textBuf[3]='O'; 
			}
		}
		break;
		default:{
			textBuf[2]='-';
		}
	}

	ledDisp->displayHandler(textBuf);




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
		lights|= 1<<LIGHT_MOMENTARY_0;
		updateScreen = true;
		SETTINGS_MODE_SELECTOR = 0;
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_2].getValue()){
		lights|= 1<<LIGHT_MOMENTARY_2;
		updateScreen = true;
		SETTINGS_MODE_SELECTOR = 0;
	  }
	  if (binaryInputs[BUTTON_MOMENTARY_1].getValue()){
		lights|= 1<<LIGHT_MOMENTARY_1;
		updateScreen = true;
		SETTINGS_MODE_SELECTOR = 0;
	  }
	   
	  #if MOMENTARY_BUTTONS_COUNT == 4
	  if (binaryInputs[BUTTON_MOMENTARY_3].getValue()){
		lights|= 1<<LIGHT_MOMENTARY_3;
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
		lights|= 1<<LIGHT_LATCHING_SMALL_LEFT;
	  }else{
		textBuf[1]=' ';
	  }
	  if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
		lights|= 1<<LIGHT_LATCHING_SMALL_RIGHT;
	  }else{
		textBuf[2]=' ';
	  }
	  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
		lights|= 1<<LIGHT_LATCHING_BIG;
		updateScreen = true;
	  }else{
		textBuf[3]=' ';
	  }
	  if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
		lights|= 1<<LIGHT_LATCHING_EXTRA;
		updateScreen = true;
	  }else{
		textBuf[4]=' ';
	  }
 
	  ledDisp->displayHandler(textBuf);
	   
	  
	  ledDisp->setBrightness((byte)(50 - potentio->getValueMapped(0,50)),false);
 
	}else if (SETTINGS_MODE_SELECTOR < 8){
	  lights|= 1<<LIGHT_MOMENTARY_0;
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
	  lights|= 1<<LIGHT_MOMENTARY_0;
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
		NUMBERS_AND_LETTERS_COUNT_UP_ELSE_DOWN= true;
	  }
	   
	  const bool numberElseAlphabethMode = !binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue();

	  if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
	   	 NUMBERS_AND_LETTERS_COUNT_UP_ELSE_DOWN = !NUMBERS_AND_LETTERS_COUNT_UP_ELSE_DOWN; 
	   }
   
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
	 
	  if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){
		counter++;
		updateScreen = true;
	  }
		
	  if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
		counter--;
		updateScreen = true;
	  } 
	  if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
		if (numberElseAlphabethMode){
		  counter = 0;
		}else{
		  counter = 1;
		}
		updateScreen = true;
	  }
	   
	  // auto count
	  if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeUp()){
		generalTimer.start();
	  }  
	   
	  if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeDown()){
		generalTimer.pause();		
	  }  
	   
	  
	  if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
		// auto mode

		if (!generalTimer.getTimeIsNegative()){
			if (NUMBERS_AND_LETTERS_COUNT_UP_ELSE_DOWN){
			counter++;  
			}else{
			counter--;
			}
			generalTimer.start();
			updateScreen = true;
		}

		//potentio behaviour
		if (potentio->getValueStableChangedEdge()){
			 
			generalTimer.setInitTimeMillis((long)(100 * potentio->getValueMapped(-100, 0))); //divided by ten, this way, we can set the timer very accurately as displayed on screen when big red is pressed. *100ms
			//generalTimer.start();
			ledDisp->showNumber( (int16_t)100 * (100 - potentio->getValueMapped(0,100)));
			 
		}
	  }else if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
		// show number right away depending on potention value
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
		  counter = 100;
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

  buzzer->lastPlayedNoteToDisplay(textBuf, &decimalPoints);
	 
  ledDisp->displaySetTextAndDecimalPoints(textBuf, &decimalPoints);
}

void Apps::modeComposeSong(bool init){

	bool defaultDisplay = true;
	int8_t step = 0;

	if (init){
    	for (uint8_t i=0;i<bytes_list_bufsize;i++){
			COMPOSER_SONG[i] = BUZZER_ROLL_SONG_STOPVALUE;
		}
		COMPOSER_SONG_LENGTH = 1;
		COMPOSER_SONG[0] = rest_1; //default note
		COMPOSER_STEP = 0;
		COMPOSER_STEP_TIMER.setInitTimeMillis(-200);
	}

	if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){ 
		bool loaded;
		loaded = saveLoadMenu(COMPOSER_SONG, 4, EEPROM_COMPOSER_SONG_LENGTH, EEPROM_COMPOSER_SONGS_START_ADDRESS);
		
		if (loaded){
			COMPOSER_STEP = 0;
			// search for the last note in the composed song. 
			// assume last note of longest possible song is always BUZZER_ROLL_SONG_STOPVALUE
			for(uint8_t i=bytes_list_bufsize-2; i>0; i--){
				COMPOSER_SONG_LENGTH = i+1;
				if (COMPOSER_SONG[i] != BUZZER_ROLL_SONG_STOPVALUE){
					
					break;
				} 
			}
		}
	}else{

		if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
			// display song by index (enable insert delete position)
			
			if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
				//delete current position

				// move all notes one position down.
				for (uint8_t i=COMPOSER_STEP; i<bytes_list_bufsize-1; i++){
					COMPOSER_SONG[i] = COMPOSER_SONG[i+1]; 
				}
				// deleted space should be a song stop note.
				COMPOSER_SONG[COMPOSER_SONG_LENGTH-1] = BUZZER_ROLL_SONG_STOPVALUE;
				
				//adjust length
				COMPOSER_SONG_LENGTH--;
			}

			if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
				//insert after current index (and move to it)
				if (COMPOSER_SONG_LENGTH  >= bytes_list_bufsize-2){
					//max length reached 
					
				}else{
					
					// remember, last note of longest song possible MUST be BUZZER_ROLL_SONG_STOPVALUE, don't copy a note to it.
					for (uint8_t i=bytes_list_bufsize-3; i>COMPOSER_STEP; i--){
						COMPOSER_SONG[i+1] = COMPOSER_SONG[i]; 
					}
					COMPOSER_SONG[COMPOSER_STEP+1] = rest_1;
					COMPOSER_SONG_LENGTH++;
					step = 1; // move to new position
				}
			}

			ledDisp->showNumber(COMPOSER_STEP);

		}else{

			// display song by note (enable programming and listening to notes)
			if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
				// just listen to note on index in song
				buzzer->buzzerOff();
				buzzer->programBuzzerRoll(COMPOSER_SONG[COMPOSER_STEP]);
			}

			if (binaryInputs[BUTTON_MOMENTARY_0].getValue()){
				// just play notes selected with potentio
				if (potentio->getValueStableChangedEdge()){			
					buzzer->buzzerOff();
					buzzer->programBuzzerRoll(potentio->getValueMapped(0,254));
					
					buzzer->noteToDisplay(textBuf, &decimalPoints,potentio->getValueMapped(0,254));
					ledDisp->displaySetTextAndDecimalPoints(textBuf, &decimalPoints);
				}
				defaultDisplay = false;
			}

			// program note in song
			if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
				
				COMPOSER_SONG[COMPOSER_STEP] = potentio->getValueMapped(0,254);
				buzzer->buzzerOff();
				buzzer->programBuzzerRoll(COMPOSER_SONG[COMPOSER_STEP]);
			
				// if note added to end, expand song length and add default note 
				if (COMPOSER_STEP == COMPOSER_SONG_LENGTH - 1  ){
					COMPOSER_SONG_LENGTH++;
					COMPOSER_SONG[COMPOSER_SONG_LENGTH - 1] = rest_1;  //default note
				}
			}

			if (binaryInputs[BUTTON_MOMENTARY_1].getValue()){
				if(potentio->getValueStableChangedEdge()){
					COMPOSER_SONG[COMPOSER_STEP] = potentio->getValueMapped(0,255);
					buzzer->buzzerOff();
					buzzer->programBuzzerRoll(COMPOSER_SONG[COMPOSER_STEP]);
				}
			}
		}
	#ifdef BUTTON_MOMENTARY_3
		if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
			step = -1;
		}
		if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){
			step = 1;
		}
	#else
		if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp()){
			step = 1;
		}
	#endif

		// autoplay
		if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
			

			if (!COMPOSER_STEP_TIMER.getTimeIsNegative()){
				step = 1;
				COMPOSER_STEP_TIMER.start();
			}
		}

		//default potentio behaviour
		if (!binaryInputs[BUTTON_MOMENTARY_0].getValue() && 
			!binaryInputs[BUTTON_MOMENTARY_1].getValue() &&
			!binaryInputs[BUTTON_MOMENTARY_2].getValue() &&
	#ifdef BUTTON_MOMENTARY_3
			!binaryInputs[BUTTON_MOMENTARY_3].getValue() &&
	#endif
			potentio->getValueStableChangedEdge()){
			int8_t tmp = 2 * potentio->getLastStableValueChangedUp() - 1;
			if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
				// change speed if default behaviour of potentio.
				COMPOSER_STEP_TIMER.setInitTimeMillis(COMPOSER_STEP_TIMER.getInitTimeMillis() + tmp * 10) ; //step +1 or -1
			}else{
				step = tmp;
			}
		}

		if (step != 0){
			COMPOSER_STEP = nextStepRotate(COMPOSER_STEP, (step + 1)/2, 0, COMPOSER_SONG_LENGTH-1 );
			buzzer->programBuzzerRoll(COMPOSER_SONG[COMPOSER_STEP]);
		}

		//sometimes overrule screen if potentio looking for a note.
		if (defaultDisplay){
			if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
				ledDisp->showNumber(COMPOSER_STEP);
			}else{
				buzzer->noteToDisplay(textBuf, &decimalPoints, COMPOSER_SONG[COMPOSER_STEP]);
				ledDisp->displaySetTextAndDecimalPoints(textBuf, &decimalPoints);
			}
		}
	}
}

void Apps::modeSoundNotes(bool init){
  //buzzer with buzzer roll (notes).
	

	if (init){
		decimalPoints = 0xFF;
	}
		
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
		buzzer->noteToDisplay(textBuf, &decimalPoints, SOUND_FUN_NOTE_INDEX);
		ledDisp->displaySetTextAndDecimalPoints(textBuf, &decimalPoints);
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
		lights |= 1<<lights_indexed[i];	
	}
  }
   
  // settings light to real lights
  (LIGHT_PAUSE & settingsLights)? lights|= 1<<LIGHT_LATCHING_EXTRA:false;
  (LIGHT_PLAYING & settingsLights)? lights|= 1<<LIGHT_LATCHING_BIG:false;
  (LIGHT_FISCHER & settingsLights)? lights|= 1<<LIGHT_LATCHING_SMALL_RIGHT:false;
  (LIGHT_SET_TIMERS_COUNT & settingsLights)? lights|= 1<<LIGHT_LATCHING_SMALL_LEFT:false;
 
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
  bool showNote = false;
	 
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
	  this->saveLoadMenu(this->SEQUENCER_SONG, 9, EEPROM_SEQUENCER_SONG_LENGTH, EEPROM_SEQUENCER_SONGS_START_ADDRESS);

  }else{
	// manipulate the sequencer
 
 
	// visualize programmed note
 
	SEQUENCER_TEMP_NOTE = SEQUENCER_SONG[SEQUENCER_STEP_COUNTER];
 
	if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
	  buzzer->programBuzzerRoll(SEQUENCER_TEMP_NOTE);
	  buzzer->noteToDisplay(textBuf, &decimalPoints, SEQUENCER_TEMP_NOTE);
	  showNote = true;
	}
 
	if (binaryInputs[BUTTON_MOMENTARY_0].getValue()){
	  // if button continuously pressed, show notes.
      buzzer->noteToDisplay(textBuf, &decimalPoints, SEQUENCER_TEMP_NOTE);	
	  showNote = true;
 
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
	//   ledDisp->showNumber(SEQUENCER_TEMP_NOTE);
		buzzer->noteToDisplay(textBuf, &decimalPoints, SEQUENCER_TEMP_NOTE);
		showNote = true;
	}
	 
	if (binaryInputs[BUTTON_MOMENTARY_1].getValue()){
	  // if button continuously pressed, rotate potentio to hear notes.
	  if (potentio->getValueStableChangedEdge()){
		 
		buzzer->programBuzzerRoll(SEQUENCER_TEMP_NOTE);
		// ledDisp->showNumber(SEQUENCER_TEMP_NOTE);
		buzzer->noteToDisplay(textBuf, &decimalPoints, SEQUENCER_TEMP_NOTE);
		showNote = true;
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
	
	if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeDown()){
		// reset transpose when stop autoplay.
		SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET = 0;
	}

	// autoplay
	if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
	  // change speed if default behaviour of potentio.
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
	
	// if music note needs to be shown
	if (showNote){
	  ledDisp->displaySetTextAndDecimalPoints(textBuf, &decimalPoints);
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

#ifdef PROTOTYPE
void Apps::modeSimon(bool init)
{
  const int numButtons = 4;
  const int buttons[numButtons] = { BUTTON_LATCHING_EXTRA, BUTTON_MOMENTARY_0, BUTTON_MOMENTARY_1, BUTTON_MOMENTARY_2 };
  const byte lights[numButtons] = { 1 << LIGHT_LATCHING_EXTRA, 1 << LIGHT_MOMENTARY_0, 1 << LIGHT_MOMENTARY_1, 1 << LIGHT_MOMENTARY_2 };
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
#else

void Apps::modeSimon(bool init)
{
  uint8_t lights = 0b00000000;

  if (init) {
	SIMON_BLINK_TIMER.setInitTimeMillis(-250);
	SIMON_STEP_TIMER.setInitTimeMillis(-500);

	SIMON_ACTIVE_LIGHT = SIMON_NO_ACTIVE_LIGHT;	
	SIMON_PLAYERS_COUNT = 1;
	// SIMON_LEVEL = 1; // levels don't look that appealing. I choose one speed and you'll have to stick with it.
  }
 
  if (!binaryInputs[BUTTON_LATCHING_EXTRA].getValue() || init){
	// at any time, if play button off: no more playing!
	simonState = simonWaitForNewGame;
  }

//   if (SIMON_PLAYERS_COUNT > 1){
// 	  // if more than one player, light always on.
// 	  lights |= 1 << LIGHT_LATCHING_SMALL_RIGHT;
//   }
  if (SIMON_RANDOM_PLAYER_SEQUENCE){
	  lights |= 1 << LIGHT_LATCHING_SMALL_RIGHT;
  }
  
  if (SIMON_CUSTOM_BUILD_UP){
	// if custom sequence, light always on.	
 	lights |= 1<<LIGHT_LATCHING_SMALL_LEFT;
  } 
  
//   // check if a momentary button was pressed, and create byte with status: 0000 is no button pressed.  0001, 0010, 0100, 1000
//   uint8_t buttonsChanged = 0;
//   for (int k = 0; k <  MOMENTARY_BUTTONS_COUNT; ++k) {
// 	buttonsChanged |= (binaryInputs[buttons_momentary_indexed[k]].getEdgeUp() << k);
//   }
  uint8_t pressed_momentary_button = SIMON_NO_BUTTON_PRESSED;
  for (int k = 0; k <  MOMENTARY_BUTTONS_COUNT; ++k) {
	if (binaryInputs[buttons_momentary_indexed[k]].getEdgeUp()){
		pressed_momentary_button = k;		
	}
  }
  	
  switch (simonState) {
	case simonWaitForNewGame: {
		textBuf[1]=' ';
		textBuf[2]=' ';
		textBuf[3]=' '; 
		textBuf[4]=' '; 
		
		if (binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
			simonState = simonNewGame;
		}

		// play button light blinking invitingly.
		if (millis()%250 > 125){
			lights|= 1<<LIGHT_LATCHING_EXTRA;
		}
		
		// number of players.
		if( potentio->getValueStableChangedEdge()) {
			SIMON_PLAYERS_COUNT = potentio->getValueMapped(1,SIMON_MAX_PLAYERS);
		}
		ledDisp->numberToBuf(textBuf, SIMON_PLAYERS_COUNT);
		textBuf[1] = ' ';	
		textBuf[2] = 'P';	
		textBuf[3] = ' ';	
		

		// Instead of computer, user choses the next light in simon sequence.
		SIMON_CUSTOM_BUILD_UP = binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue();

		// random player sequence during game if multiplayer
		SIMON_RANDOM_PLAYER_SEQUENCE = binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue();
		
	  break;
	}
 
	case simonNewGame: {
	  SIMON_END_OF_GAME = false;
	  SIMON_PLAYERS_ALIVE_COUNT = SIMON_PLAYERS_COUNT;
	  
	  // set all players in array. don't bother about real player count.
	  for (uint8_t i=0;i<SIMON_MAX_PLAYERS;i++){
		SIMON_PLAYERS[i]=i;
	  }

	  // generate new light sequence
	  randomSeed(millis());
	  for (int k = 0; k < bytes_list_bufsize; ++k) {
		SIMON_LIST[k] = k %  MOMENTARY_BUTTONS_COUNT;
	  }
	  shuffle(SIMON_LIST, bytes_list_bufsize);

	  // set starting parameters
	  SIMON_LENGTH = 0;

	  simonState = simonNewLevel;
	  break;
	}
 
	case simonNewLevel: {
	  ++SIMON_LENGTH;

	  // shuffle alive players.
	  if (SIMON_RANDOM_PLAYER_SEQUENCE){
	  	shuffle(SIMON_PLAYERS, SIMON_PLAYERS_ALIVE_COUNT);
	  }

	  // set first player
	  SIMON_PLAYER_PLAYING_INDEX = 0;  // this is just the index.

	  ledDisp->numberToBuf(textBuf, SIMON_LENGTH);
	  //intToDigitsString(textBuf+1, SIMON_LENGTH, false);

	// let maximum length breach be a happy crash. I can't afford the bytes!
	//   if (SIMON_LENGTH >= bytes_list_bufsize) {
	// 	  // reached maximum length
	// 	  buzzer->loadBuzzerTrack(song_attack);
	// 	  simonState = simonWaitForNewGame;
	// 	  break;
	//   }
	 
      if (SIMON_CUSTOM_BUILD_UP){
		// for custom sequence games, there is nothing to be replayed.
		simonState = simonStartUserRepeats;
	  }else{
	  	simonState = simonStartPlaySequence;
	  }
	  break;
	}
 
    case simonStartPlaySequence:{
		SIMON_INDEX = -1; // negative index allows for lead-in time
		SIMON_STEP_TIMER.start();
	    simonState = simonPlaySequence;
	}

	case simonPlaySequence: {
	  if (SIMON_END_OF_GAME){

		if (millis()%1000 > 650){
			ledDisp->numberToBuf(textBuf, SIMON_LENGTH);
			textBuf[1] = SIMON_PLAYERS[SIMON_PLAYER_PLAYING_INDEX] + 49;
			textBuf[2] = 'P';
		}else{
			textBuf[1] = ' ';
			textBuf[2] = 'E';
			textBuf[3] = 'N';
			textBuf[4] = 'D';
		}
	  }else{
	  	textBuf[2] = 'S';		
	  }
	  
	  if (SIMON_ACTIVE_LIGHT != SIMON_NO_ACTIVE_LIGHT){
	  	lights |= 1<<lights_indexed[SIMON_ACTIVE_LIGHT];
		if(!SIMON_BLINK_TIMER.getTimeIsNegative()){
			SIMON_ACTIVE_LIGHT = SIMON_NO_ACTIVE_LIGHT;
		}
	  }

 	  if (SIMON_INDEX >= SIMON_LENGTH && !SIMON_BLINK_TIMER.getTimeIsNegative()) {
		// after last step display, immediatly continue
		if (SIMON_END_OF_GAME){

	 		simonState = simonWaitForNewGame;
	 	}else{
		  // time for the user to prove himself.
			simonState = simonStartUserRepeats;
        }	
		break;
	  }
	  
	  if (SIMON_STEP_TIMER.getTimeIsNegative()){
		// still in step. 
		break;
	  }
	  
	  SIMON_STEP_TIMER.start();
	  if (SIMON_INDEX < 0) {
		++SIMON_INDEX; // do-nothing lead in time
		break;
	  }

	  // show one light from the sequence
	  SIMON_ACTIVE_LIGHT = SIMON_LIST[SIMON_INDEX];
	 
	  SIMON_BLINK_TIMER.start();
	 
	  buzzer->programBuzzerRoll(A3_8);
	  ++SIMON_INDEX;
	  break;
	}
	
	case simonStartUserRepeats:{
		SIMON_INDEX = 0;
		simonState = simonUserRepeats;
	}
	break;

	case simonUserRepeats: {

	  for (int k = 0; k <  MOMENTARY_BUTTONS_COUNT; ++k) {
	
			lights |= binaryInputs[buttons_momentary_indexed[k]].getValue()<<lights_indexed[k];
		
	  
	  }

      textBuf[1] = SIMON_PLAYERS[SIMON_PLAYER_PLAYING_INDEX] + 49;
      textBuf[2] = 'P';
	  if (pressed_momentary_button == SIMON_NO_BUTTON_PRESSED) {
		break;
	  }

	  if (SIMON_CUSTOM_BUILD_UP && (SIMON_INDEX + 1) == SIMON_LENGTH){
		// last light of sequence is not yet defined.
		// user can choose any button.
		SIMON_LIST[SIMON_INDEX] = pressed_momentary_button;
		
	  }
	  
	  const int expected = SIMON_LIST[SIMON_INDEX];

	  if (pressed_momentary_button != expected) {
		// player made mistake, player dies
		buzzer->programBuzzerRoll(C4_1); 
		buzzer->programBuzzerRoll(C4_1); 
		buzzer->programBuzzerRoll(C4_1); 
		buzzer->programBuzzerRoll(C4_1); 
		simonState = simonPlayerDead;
		break;
	  }
	  // player pressed correct button
	  buzzer->programBuzzerRoll(A3_8);
	  
	  ++SIMON_INDEX;

	  if (SIMON_INDEX >= SIMON_LENGTH) {
		// sequence done!
		buzzer->programBuzzerRoll(E5_4); 
		buzzer->programBuzzerRoll(rest_1); 
		buzzer->programBuzzerRoll(B6_1); 
		simonState = simonNextPlayer;
		break;
	  }
	  break;
	}

	case simonNextPlayer:{
	  // check next alive player (assume there is always a player alive.)
	
	   SIMON_PLAYER_PLAYING_INDEX++;
	   if (SIMON_PLAYER_PLAYING_INDEX >= SIMON_PLAYERS_ALIVE_COUNT){
		 // completely new level
		 simonState = simonNewLevel;
	   }else{
		 // repeat existing level for next player
		 simonState = simonStartUserRepeats;
	   }
	}
	break;

	case simonPlayerDead:{
		SIMON_PLAYERS_ALIVE_COUNT--;
		if (SIMON_PLAYERS_ALIVE_COUNT == 0){
			//everybody dead
			simonState = simonStartPlaySequence;
			SIMON_END_OF_GAME = true;
			break;
		}

		if (SIMON_PLAYER_PLAYING_INDEX >= SIMON_PLAYERS_ALIVE_COUNT){
			// if last player in the sequence was playing. not much needs to be done.
			simonState = simonNewLevel;
			break;
		}

		// replace the current (dead) player by the last player in the list that needs to play.
		// remember: we already decrease alive_count by 1
		// no need to go to next player, as the new current player didnt play yet
		SIMON_PLAYERS[SIMON_PLAYER_PLAYING_INDEX] = SIMON_PLAYERS[SIMON_PLAYERS_ALIVE_COUNT ];
		simonState = simonStartUserRepeats;
	}
	break;
  }

  ledDisp->SetLedArray(lights);
  ledDisp->displayHandler(textBuf);
}
#endif 
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

bool Apps::saveLoadMenu(uint8_t* data, uint8_t slotCount, uint8_t eepromSlotLength, uint16_t eepromStartAddress){
	// we will need to make this a separate function, it's almost identical to sequencer load save.
	//-number of save slots
	//-length of data to save.  (=eeprom data length)
	// eeprom start length
	// pointer to the data

	//kind of an init
	if (!SAVE_LOAD_MENU_BLINK_TIMER.getIsStarted()){
		SAVE_LOAD_MENU_BLINK_TIMER.setInitTimeMillis(-1000);
		SAVE_LOAD_MENU_BLINK_TIMER.start();
	}

	// load/save songs 
	//blink alternatively song number and "load" or "save"
	uint8_t slot_number  = potentio->getValueMapped(1,slotCount);
	if (SAVE_LOAD_MENU_BLINK_TIMER.getInFirstGivenHundredsPartOfSecond(500)){
		ledDisp->showNumber(slot_number);

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
		bool loaded = false;
		for (uint8_t i=0;i<eepromSlotLength;i++){
			uint8_t* eeprom_address = (uint8_t*)
				(eepromStartAddress +
				(slot_number - 1) * eepromSlotLength + i  
				);
	
			if(binaryInputs[BUTTON_LATCHING_EXTRA].getValue()){
				//save
				eeprom_write_byte(eeprom_address, data[i]);
	
			}else{
				//load
				data[i] = eeprom_read_byte(eeprom_address);
				loaded = true;
			}
		}

		return loaded;

	}

	if (binaryInputs[BUTTON_MOMENTARY_0].getValue()){
		SAVE_LOAD_MENU_BLINK_TIMER.start();
	}
	return false;
	
}