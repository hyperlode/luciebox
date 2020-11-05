#include "Apps.h"
//#include "Arduino.h"
#include "PretbakSettings.h"

Apps::Apps(){};

void Apps::setPeripherals(BinaryInput binaryInputs[], RotaryEncoderDial *encoder_dial, DisplayManagement *ledDisp, LedMultiplexer5x8* allLights, Buzzer *buzzer)
{
	this->buzzer = buzzer;
	this->binaryInputs = binaryInputs;
	this->encoder_dial = encoder_dial;
	this->ledDisp = ledDisp;
	this->allLights = allLights;

	textHandle = ledDisp->getDisplayTextBufHandle();
	decimalDotsHandle = ledDisp->getDecimalPointsHandle();
	lightsHandle = ledDisp->getLedArrayHandle();
	// displaySegmentsHandle = ledDisp->getBinaryHandle();
}

void Apps::appSelector(bool init, uint8_t selector)
{

	updateEveryAppCycleBefore();

	if (init)
	{
		// title mode (title screen will be displayed before real app starts)
		this->app_init_mode = true;

#ifdef ENABLE_SERIAL
		Serial.println("select knob:");
		Serial.println(selector);
#endif
	}

	if (this->app_init_mode)
	{
		if (this->init_app(init, selector))
		{
			// do init routine (showing splash screen), if finished,end of init. Then continue to init of the chosen application
			this->app_init_mode = false;
			init = true;
		}
	}

	// not as else statement, to have the init properly transferred after app beginning screen.
	if (!this->app_init_mode)
	{

		uint8_t appSelector = selector * 2 +
							  ((binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_BIG_RED)) > 0);

		bool initOnBigLatchInitToo = init || binaryInputs[BUTTON_LATCHING_BIG_RED].getValueChanged();

		if (init ||
			(initOnBigLatchInitToo && (appSelector != APP_SELECTOR_MULTITIMER_PLAYING)) // it's a bit messy, but for now, the multitimer app is the only one without a double app per selector position
		)
		{
#ifdef ENABLE_SERIAL
			Serial.println("app select:");
			Serial.println(appSelector);
#endif
			// set to init state before a new app starts
			this->setDefaultMode();
		}

#ifdef FUNCTION_POINTER_APP_SELECTION
		// problem: takes more memory than switch-case. AND init and initOnBigLatchInitToo not good. The solution would be to have all the apps without advanced init bundled together, and from certain selector value onwards and up, use "init"

		//https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_74/rzarg/cplr034.htm
		/////function pointer: working, but with weird audio beeps intermixed. What's going on there?!
		//// this->appPointer = &Apps::modeSimon;
		//// (this->*appPointer)(initOnBigLatchInitToo);

		//modeCountingLettersAndChars(initOnBigLatchInitToo);

		fptr allAppsIndexed[24] = {
			&Apps::modeCountingLettersAndChars,
			&Apps::modeSimon,
			&Apps::modeSoundNotes,
			&Apps::modeComposeSong,
			&Apps::stopwatch,
			&Apps::pomodoroTimer,
			&Apps::modeRandomWorld,
			&Apps::modeRandomWorld,
			&Apps::modeGeiger,
			&Apps::modeHackTime,
			&Apps::modeSoundSong,
			&Apps::movieAnimationMode,
			&Apps::draw,
			&Apps::drawGame,
			&Apps::modeSimpleButtonsAndLights,
			&Apps::modeSimpleButtonsAndLights,
			&Apps::modeMetronome,
			&Apps::modeSequencer,
			&Apps::modeReactionGame,
			&Apps::modeReactionGame,
			&Apps::tiltSwitchTest,
			&Apps::tiltSwitchTest,
			&Apps::miniMultiTimer,
			&Apps::miniMultiTimer};

		(this->*(allAppsIndexed[appSelector]))(initOnBigLatchInitToo);

#else

		switch (appSelector)
		{

		case APP_SELECTOR_LETTERS_AND_CHARS:
			this->modeCountingLettersAndChars(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_SIMON:

#ifdef ENABLE_SIMON_APP
			this->modeSimon(initOnBigLatchInitToo);
#endif
			break;

		case APP_SELECTOR_SOUND_NOTES:
			this->modeSoundNotes(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_SOUND_COMPOSER:
			this->modeComposeSong(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_STOPWATCH:
			stopwatch(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_POMODORO:
			pomodoroTimer(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_RANDOMWORLD:
			this->modeRandomWorld(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_QUIZ:
			this->quiz(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_GEIGER:
			this->modeGeiger(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_HACK_TIME:
			this->modeHackTime(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_SOUND_SONG:
			this->modeSoundSong(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_DRAW:
			this->draw(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_DRAW_GAME:
			this->drawGame(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_MOVIE_MODE:
			this->movieAnimationMode(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_SETTING:
		case APP_SELECTOR_SETTING_TOO:
			this->modeSimpleButtonsAndLights(init);
			break;

		case APP_SELECTOR_SOUND_METRONOME:
			this->modeMetronome(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_SOUND_SEQUENCER:
			this->modeSequencer(initOnBigLatchInitToo);
			break;

		case APP_SELECTOR_REACTION_GAME:
		case APP_SELECTOR_GUITAR_HERO:
#ifdef ENABLE_REACTION_APP
			this->modeReactionGame(initOnBigLatchInitToo);
#endif
			break;

#ifdef ENABLE_TILT_SWITCHES
		case APP_SELECTOR_TILT:
		case APP_SELECTOR_TILT_ADVANCED:
			this->tiltSwitchTest(initOnBigLatchInitToo);
			break;
#endif

		case APP_SELECTOR_MULTITIMER_SETTING:
		case APP_SELECTOR_MULTITIMER_PLAYING:
#ifdef ENABLE_MULTITIMER
			this->miniMultiTimer(init);
#endif
			break;

			// #ifdef ENABLE_ANALOG_PIN_DEBUG
			// 	case 12:
			//   //this is the debug mode
			//   this->modeButtonDebug(init);
			//   break;
			// #endif

		default:
			break;
		}
#endif
	}
}

void Apps::updateEveryAppCycleBefore()
{
	binaryInputsEdgeUp = 0;
	binaryInputsEdgeDown = 0;
	binaryInputsValue = 0;

	// load quick access bytes.
	for (uint8_t i = 0; i < 8; i++)
	{
		binaryInputsEdgeUp |= binaryInputs[buttons_indexed[i]].getEdgeUp() << i;
		binaryInputsEdgeDown |= binaryInputs[buttons_indexed[i]].getEdgeDown() << i;
		binaryInputsValue |= binaryInputs[buttons_indexed[i]].getValue() << i;
	}

	// it's important to realize the display is reset before every cycle. Do not forget to update it all the time.
	setBlankDisplay();
}

// void Apps::updateEveryAppCycleAfter()
// {
// }

void Apps::setDefaultMode()
{
	allLights->setBrightness(0, false);

	//buzzer
	buzzer->setSpeedRatio(2);
	buzzerOff(); // stop all sounds that were playing in an app.
	buzzer->setTranspose(0);

	//encoder
	encoder_dial->setRange(1023,true); // allow overflow. (this makes it different from the limited potentio.)
	encoder_dial->setValue(0);
}

bool Apps::init_app(bool init, uint8_t selector)
{
	// shows animated introduction splash screen

	if (init)
	{
		// init of the init_app..
		this->displayAllSegments = 0;

		flashPictureToDisplayAllSegments(app_splash_screens + selector * 4);

		// initialize list
		randomSequence(FADE_IN_RANDOM_LIST, 32);

		counter = 27; 
		this->TIMER_INIT_APP.setInitTimeMillis(-20);
		this->TIMER_INIT_APP.start();

		randomSeed(millis());
	}

	// advance one frame
	if (!this->TIMER_INIT_APP.getTimeIsNegative())
	{
		this->TIMER_INIT_APP.start();
		counter++;
	}

	if (counter < 32)
	{
		ledDisp->setBinaryToDisplay(0xFFFFFFFF); // use fade in as fade out to set text.
	}
	else if (counter < 50)
	{
		ledDisp->setBinaryToDisplay(this->displayAllSegments);
	}
	else if (counter < 82)
	{
		ledDisp->setBinaryToDisplay(~this->fadeInList(counter - 51, 32, ~this->displayAllSegments, this->FADE_IN_RANDOM_LIST));
	}
	else
	{
		return true;
	}
	return false;
}

void Apps::quiz(bool init)
{

	// quiz app
	uint8_t lights = 0;

	if (init)
	{
		quizState = quizInit;
	}

	switch (quizState)
	{
	case quizInit:
	{
		for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
		{
			QUIZ_SCORE[i] = 0;
		}
		quizState = quizWaitForQuizMaster;
		lights |= 1 << LIGHT_LATCHING_SMALL_LEFT;
	}
	break;
	case quizWaitForQuizMaster:
	{
		lights |= 1 << LIGHT_LATCHING_EXTRA;

		if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeUp() || binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue())
		// if (binaryInputsEdgeUp && (1 << BUTTON_INDEXED_LATCHING_EXTRA) ||
		// 	binaryInputsEdgeUp && (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))

		{
			quizState = quizWaitRandomTime;
			QUIZ_RANDOM_WAIT_TIME.setInitTimeMillis(random(-3000, -500));
			QUIZ_RANDOM_WAIT_TIME.start();
		}
	}
	break;
	case quizWaitRandomTime:
	{
		lights |= 1 << LIGHT_LATCHING_SMALL_RIGHT;
		if (!QUIZ_RANDOM_WAIT_TIME.getTimeIsNegative())
		{
			quizState = quizWaitPlayerPress;
		}

		// check here, any player pressing his button = score to zero.
		for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
		{
			if (binaryInputsEdgeUp & 1 << i)
			{
				QUIZ_SCORE[i] = 0;
			}
		}
	}
	break;
	case quizWaitPlayerPress:
	{
		lights = 0xFF;
		for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
		{
			if (binaryInputsEdgeUp & 1 << i)
			{
				// add to score.
				QUIZ_SCORE[i]++;
				// go to next state

				quizState = quizPlayerPressed;
			}
		}
	}
	break;
	case quizPlayerPressed:
	{
		lights |= 1 << LIGHT_LATCHING_EXTRA;
		if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeDown() || binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue())
		{
			quizState = quizWaitForQuizMaster;
		}
	}
	break;
	}

	// as long as switched on, scores reset (i.e. needs to be set to zero to play. So, this makes it the big reset button.)
	if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue())
	{
		quizState = quizInit;
	}

	int16_t scoreToDisplay = 0;
	int16_t multiplier = 1000;

	for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
	{
		scoreToDisplay += multiplier * QUIZ_SCORE[i];
		multiplier /= 10;
	}
	ledDisp->setNumberToDisplayAsDecimal(scoreToDisplay);

	// lights |= 1 << lights_indexed[i];
	setLedArray(lights);
}

void Apps::pomodoroTimer(bool init)
{
	uint8_t display_mode = POMODORO_DISPLAY_TIMER;
	lights = 0;

	if (init)
	{
		POMODORO_MAIN_CLOCK_TIME_INDEX = POMODORO_INIT_DEFAULT_TIME_INDEX;
		POMODORO_PAUSE_TIME_INDEX = POMODORO_PAUSE_DEFAULT_TIME_INDEX;
		POMODORO_STATS_WORKING_GOOD = 0;
		POMODORO_STATS_WORKING_BAD = 0;
		POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX = POMODORO_PROBABILITY_BEEP_INTERVAL_DEFAULT_TIME_INDEX;
	}
	
	POMODORO_AUTO_RESTART_ENABLED = binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT);
	POMODORO_ENABLE_INTERVAL_BEEP = binaryInputsValue & (1<< BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT);

	bool in_menu = !(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA) );

	if (binaryInputsEdgeUp & (1<<BUTTON_LATCHING_EXTRA)){
		POMODORO_TIMER.start();
	}
	
	// in main menu or timing? (run main menu at least once at init. Even when start button started) to initialize variables depending on settings latching buttons
	if (!in_menu)
	{
		// pomodoro timer running

		if (!POMODORO_TIMER.getTimeIsNegative())
		{
			POMODORO_IN_BREAK = !POMODORO_IN_BREAK;
			if (POMODORO_IN_BREAK)
			{
				// finished main pomodoro
				loadBuzzerTrack(SONG_DRYER_HAPPY);

				if (POMODORO_AUTO_RESTART_ENABLED)
				{
					POMODORO_TIMER.setInitCountDownTimeSecs(indexToTimeSeconds(POMODORO_PAUSE_TIME_INDEX));
					POMODORO_TIMER.start();
				}else{
					POMODORO_TIMER.reset();
					POMODORO_IN_BREAK = false;
				}
			}
			else
			{
				// coming out of break. Not executed at starting Pomodoro by switch.

				loadBuzzerTrack(SONG_ATTACK);
				POMODORO_TIMER.setInitCountDownTimeSecs(indexToTimeSeconds(POMODORO_MAIN_CLOCK_TIME_INDEX));
				POMODORO_TIMER.start();
			}
		}

		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
		{
			POMODORO_STATS_WORKING_BAD++;
		}
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
		{
			POMODORO_STATS_WORKING_GOOD++;
		}

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)) ||
			(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)))
		{
			display_mode = POMODORO_DISPLAY_SHOW_BAD;
		}

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)) ||
			(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		{
			display_mode = POMODORO_DISPLAY_SHOW_GOOD;
		}

		// ticking sound / random checkup-beep
		uint8_t tick_duration = encoder_dial->getValueLimited(160,false) / 2 ;  // was 40, but set to 80 for less sensitivity. do again divide by four to limit options.
		bool tick_twice_a_second = tick_duration > 40;
		tick_duration = tick_duration % 40; 

		if (POMODORO_TIMER.getEdgeSinceLastCallFirstGivenHundredsPartOfSecond(500, true, tick_twice_a_second))
		{
			if (POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX > 0 
			&& POMODORO_ENABLE_INTERVAL_BEEP
			)
			{
				if (random(0, indexToTimeSeconds(POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX) * 2) <= 1 - (tick_twice_a_second))
				{
					// random has 0 included. as we have to take into account the double ticking,
					// calculate the probability in half a seconds. i.e.  one every minute: (0,120)
					addNoteToBuzzer(C5_1);
				};
			}

			if (tick_duration > 0)
			{
				// no sound when zero
				if (buzzer->getBuzzerRollEmpty()) // if alarm sounds, no ticking!
				{
					buzzer->playTone(500, tick_duration); // works well
				}
			}
		}
	}
	else
	{
		// in main menu
		POMODORO_TIMER.reset();
		POMODORO_TIMER.setInitCountDownTimeSecs(indexToTimeSeconds(POMODORO_MAIN_CLOCK_TIME_INDEX));

		// set timer up for change
		int16_t* active_seconds_modifier = &POMODORO_MAIN_CLOCK_TIME_INDEX; // normal time setting is default option
		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
		{
			active_seconds_modifier = &POMODORO_PAUSE_TIME_INDEX;
			display_mode = POMODORO_DISPLAY_PAUSE_INIT_SECS;

		}else if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
		{
			active_seconds_modifier = &POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX;
			display_mode = POMODORO_DISPLAY_BEEP_PROBABILITY;
		}

		// actually change the set up timer
		if (encoder_dial->getDelta()){
			nextStep(active_seconds_modifier,encoder_dial->getDelta()>0,0,90,false);
		}

		// show performance results
		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)))
		{
			display_mode = POMODORO_DISPLAY_SHOW_BAD;
		}
		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		{
			display_mode = POMODORO_DISPLAY_SHOW_GOOD;
		}
	}

	// display
	switch (display_mode)
	{

	case POMODORO_DISPLAY_TIMER:
	{
		POMODORO_TIMER.getTimeString(textBuf);
		if (POMODORO_IN_BREAK && millis() % 1000 > 650){
			setStandardTextToTextBuf(TEXT_PAUS);
		}
	}
	break;

	case POMODORO_DISPLAY_PAUSE_INIT_SECS:
	{
		timeSecondsToClockString(textBuf, indexToTimeSeconds(POMODORO_PAUSE_TIME_INDEX));
		if (millis() % 1000 > 650)
		{
			setStandardTextToTextBuf(TEXT_PAUS);
		}
	}
	break;

	case POMODORO_DISPLAY_BEEP_PROBABILITY:
	{
		timeSecondsToClockString(textBuf, indexToTimeSeconds(POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX));
		if (millis() % 1000 > 650)
		{
			// rnd beep time....
			setStandardTextToTextBuf(TEXT_RANDOM_BEEP);
		}
	}
	break;

	case POMODORO_DISPLAY_SHOW_GOOD:
	{
		if (millis() % 1000 > 650)
		{
			setStandardTextToTextBuf(TEXT_YES);
		}
		else
		{
			numberToBufAsDecimal(POMODORO_STATS_WORKING_GOOD);
		}
	}
	break;

	case POMODORO_DISPLAY_SHOW_BAD:
	{
		if (millis() % 1000 > 650)
		{
			setStandardTextToTextBuf(TEXT_NO);
		}
		else
		{
			numberToBufAsDecimal(POMODORO_STATS_WORKING_BAD);
		}
	}
	break;

	default:
		break;
	}

	if (in_menu)
	{
		if (millis() % 500 < 250)
		{
			lights |= 1 << lights_indexed[LIGHT_INDEXED_LATCHING_EXTRA];
		}
	}

	setDecimalPoint(POMODORO_TIMER.getSecondsBlinker(),1);

	// leds
	if (POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX > 0)
	{
		lights |= 1 << LIGHT_LATCHING_SMALL_LEFT;
	}
	if (POMODORO_ENABLE_INTERVAL_BEEP)
	{
		lights |= 1 << LIGHT_LATCHING_SMALL_RIGHT;
	}
	if (POMODORO_AUTO_RESTART_ENABLED)
	{
		lights |= 1 << LIGHT_LATCHING_SMALL_LEFT;
	}
	if (!in_menu)
	{
		lights |= 1 << LIGHT_LATCHING_EXTRA;
		lights |= 1 << LIGHT_MOMENTARY_2;
		lights |= 1 << LIGHT_MOMENTARY_3;
	}

	textBufToDisplay();
	setLedArray(lights);
}

void Apps::resetStopwatch(SuperTimer* pTimer){
		pTimer->setInitTimeMillis(0);
		pTimer->reset();
		pTimer->startPaused(true);		
}

void Apps::stopwatch(bool init)
{
	// classic stopwatch
	long time_millis = 0;
	uint8_t timeDisplayShift = 0;

	if (init)
	{
		STOPWATCH_LAP_MEMORY_1 = 0;
		STOPWATCH_LAP_MEMORY_2 = 0;

		resetStopwatch(&STOPWATCH_CHRONO_1);
		resetStopwatch(&STOPWATCH_CHRONO_2);
	}

	if (binaryInputsValue & (1<<BUTTON_INDEXED_LATCHING_EXTRA)){
		pSsuperTimer = &STOPWATCH_CHRONO_1;
		pLongValue = &STOPWATCH_LAP_MEMORY_1;
	}else{
		pSsuperTimer = &STOPWATCH_CHRONO_2;
		pLongValue = &STOPWATCH_LAP_MEMORY_2;
	}

	time_millis = pSsuperTimer->getTimeMillis();
	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
	{
		// save and show laptime at press
		*pLongValue = time_millis;
	}

	bool paused = pSsuperTimer->getIsPaused();
	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
	{
		// set chronometer to zero
		pSsuperTimer->startPaused(paused);
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
	{
		pSsuperTimer->paused(!paused);
	}

	if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)) ||
		(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
	{
		// show saved laptime at press
		time_millis = *pLongValue;
	}

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
	{
		pSsuperTimer->getTimeString(textBuf);
		setDecimalPoint(pSsuperTimer->getSecondsBlinker(), 1);
	}
	else
	{
		timeDisplayShift = 0;
		while (time_millis > 9999){
			timeDisplayShift++;
			time_millis /= 10;
		}

		textBuf[0] = ' ';
		textBuf[1] = ' ';

		intToDigitsString(textBuf, time_millis, true);

		setDecimalPoint(true, timeDisplayShift);

	}

	textBufToDisplay();

}

void Apps::modeRandomWorld(bool init)
{
	if (init)
	{
		RANDOMWORLD_CARD_FROM_DECK_INDEX = 0;
		randomWorldState = randomWorldIdle;
		RANDOMWORLD_RANDOM_TYPE = 0;
		randomModeTrigger(false);
		// decimalPoints = 0;

		RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW = 100;
	}

	// textbuf contains the actual random generated graphics. Set to display at start, because it might be overriden in this routine.
	textBufToDisplay();

	switch (randomWorldState)
	{

	case randomWorldIdle:
	{

		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
		{

			// set autoroll time.
#ifdef ENABLE_MULTITIMER
			uint16_t delay_seconds = this->multiTimer.getIndexedTime(encoder_dial->getValueLimited(90,false)); // 0 seconds to an hour
#else
			uint16_t delay_seconds = encoder_dial->getValueLimited(100,false) * 30;
#endif
			RANDOMWORLD_AUTODRAW_DELAY.setInitTimeMillis(-1000 * (long)delay_seconds);

			if (millis() % 1000 > 750)
			{
				setStandardTextToTextHANDLE(TEXT_AUTO);
			}
			else
			{
				ledDisp->setNumberToDisplayAsDecimal(delay_seconds);
			}
		}

		for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
		{
			if (binaryInputs[buttons_indexed[i]].getEdgeUp())
			{
				RANDOMWORLD_RANDOM_TYPE = i + 4 * ((binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT)) > 0);
				randomWorldState = randomWorldRolling;

				// set up animation
				RANDOMWORLD_ROLL_SPEED.setInitTimeMillis(-30 - (((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)) > 0) * 1970)); // special case for upper limit setting for random number.
				RANDOMWORLD_ROLL_SPEED.start();
			}
		}
	}
	break;

	case randomWorldRolling:
	{
		// check state
		bool roll_end = false;

		//if autoroll
		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
		{
			// autoroll no need for button
			roll_end = true;
		}
		else
		{
			// wait for button release
			
			// the following build up of the mask looks cumbersome, but, it's all done at precompiler time. , so it's just one byte of flash memory. 
			byte momentary_buttons_mask = 
				1<<BUTTON_INDEXED_MOMENTARY_0 | 
				1<<BUTTON_INDEXED_MOMENTARY_1 |
				1<<BUTTON_INDEXED_MOMENTARY_2 |
				1<<BUTTON_INDEXED_MOMENTARY_3;

			if ((binaryInputsEdgeDown & momentary_buttons_mask) > 0){
			 	roll_end = true;
			}
		}

		if (roll_end)
		{
			if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
			{
				randomWorldState = randomWorldRollingEnd;
			}
			else
			{
				randomWorldState = randomWorldShowResult;
				addNoteToBuzzer(D4_8);
			}
		}

		// display
		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
		{
			// animated
			if (!RANDOMWORLD_ROLL_SPEED.getTimeIsNegative())
			{
				addNoteToBuzzer(C7_8);
				randomModeTrigger(false);

				RANDOMWORLD_ROLL_SPEED.start();
			}
		}
		else
		{
			// during roll all lights on
			ledDisp->setNumberToDisplayAsDecimal(8888);
			
			// check for continued key press at random number generator to activate settings menu
			if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)) && !RANDOMWORLD_ROLL_SPEED.getTimeIsNegative())
			{
				// hack to set upper limit for random number

				RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW = encoder_dial->getValueLimited(100, false);

				RANDOMWORLD_CARD_FROM_DECK_INDEX = 0; // reset the tombola.

				if (millis() % 1000 < 200)
				{
					setStandardTextToTextHANDLE(TEXT_SET);
				}
				else
				{

					ledDisp->setNumberToDisplay(RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW, false);
				}
			}
		}
	}
	break;

	case randomWorldRollingEnd:
	{
		if (!RANDOMWORLD_ROLL_SPEED.getTimeIsNegative())
		{
			addNoteToBuzzer(C7_8);
			randomModeTrigger(false);

			// roll slower and slower until threshold reached.
			RANDOMWORLD_ROLL_SPEED.setInitTimeMillis(RANDOMWORLD_ROLL_SPEED.getInitTimeMillis() * 1.4);
			if (RANDOMWORLD_ROLL_SPEED.getInitTimeMillis() < -600)
			{
				randomWorldState = randomWorldShowResult;
			}

			RANDOMWORLD_ROLL_SPEED.start();
		}
	}
	break;

	case randomWorldShowResult:
	{

		randomModeTrigger(true);
		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
		{
			// auto roll delay.
			RANDOMWORLD_AUTODRAW_DELAY.start();
			randomWorldState = randomWorldAutoRollDelay;
		}
		else
		{
			randomWorldState = randomWorldIdle;
		}
	}
	break;

	case randomWorldAutoRollDelay:
	{
		if (!RANDOMWORLD_AUTODRAW_DELAY.getTimeIsNegative())
		{
			// set up animation
			RANDOMWORLD_ROLL_SPEED.setInitTimeMillis(-30);
			RANDOMWORLD_ROLL_SPEED.start();

			randomWorldState = randomWorldRolling;
		}
		if (!(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA)))
		{
			randomWorldState = randomWorldIdle;
		}
	}

	break;
	}
}

void Apps::randomModeTrigger(bool forReal)
{
	// set the textBuf.

	// forReal: if false, just for animations. Important for i.e. drawing a card from the deck. During animations, we're not really drawing a card from the deck.

	const int16_t randomUpperLimits[8] = {6, 26, RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW, 2, 6, 52, RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW, 2};

	RANDOMWORLD_RANDOM_NUMBER = random(0, randomUpperLimits[RANDOMWORLD_RANDOM_TYPE]);

	ledDisp->blanksToBuf(textBuf);

	switch (RANDOMWORLD_RANDOM_TYPE)
	{

	case RANDOMWORLD_ROLLONEDICE:
	{

		for (uint8_t i = 0; i < 4; i++)
		{
			textBuf[i] = pgm_read_byte_near(dice_eyes_display + RANDOMWORLD_RANDOM_NUMBER * 4 + (i)); //* 4 --> 4 bytes per dword
		}
	}
	break;

	case RANDOMWORLD_ROLLFOURDICE:
	{
		// throw four dice
		for (uint8_t i = 0; i < 4; i++)
		{
			textBuf[i] = random(49, 55); // char 1 to 6
		}
	}
	break;

	case RANDOMWORLD_RANDOMLETTER:
	{
		// show letter alphabeth, plus its position.
		displayLetterAndPositionInAlphabet(textBuf, RANDOMWORLD_RANDOM_NUMBER);
	}
	break;

	case RANDOMWORLD_TAKERANDOMCARDFROMDECK:
	{

		if (!forReal)
		{
			// don't draw a card if it's not for real. We will not even display a card, as that would be confusing. Just show blanks.
			break;
		}

		RANDOMWORLD_RANDOM_NUMBER = tombola(&RANDOMWORLD_CARD_FROM_DECK_INDEX, CARDS_DECK, 52);

		//show playing card
		if (RANDOMWORLD_RANDOM_NUMBER % 13 < 9)
		{
			textBuf[1] = RANDOMWORLD_RANDOM_NUMBER % 13 + 49;
		}
		else
		{
			textBuf[0] = 49;													   // 1
			textBuf[1] = (3 - (((RANDOMWORLD_RANDOM_NUMBER) % 13) + 1) % 10) + 48; // 9,10,11,13 to char 0 1 2 3
		}

		char test [4]  = {'H','D','S','C'};
		textBuf[3] = test[RANDOMWORLD_RANDOM_NUMBER / 13];
	}

	break;

	case RANDOMWORLD_TOMBOLA:
	{
		if (!forReal)
		{
			// dont draw if not for real
			break;
		}
		RANDOMWORLD_RANDOM_NUMBER = tombola(&RANDOMWORLD_CARD_FROM_DECK_INDEX, CARDS_DECK, RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW);
		
	}
	// NO BREAK HERE we just changed the random number to a part of a raffle draw.
	case RANDOMWORLD_RANDOMNUMBER:
	{
		// random number
		numberToBufAsDecimal(RANDOMWORLD_RANDOM_NUMBER + 1);
	}
	break;

	case RANDOMWORLD_HEADSORTAILS:
	//{
	// if (RANDOMWORLD_RANDOM_NUMBER)
	// {
	// 	textBuf[0] = 'H';
	// 	textBuf[1] = 'E';
	// 	textBuf[2] = 'A';
	// 	textBuf[3] = 'D';
	// }
	// else
	// {
	// 	textBuf[0] = 'T';
	// 	textBuf[1] = 'A';
	// 	textBuf[2] = 'I';
	// 	textBuf[3] = 'L';
	// }
	//setStandardTextToTextBuf(60 + RANDOMWORLD_RANDOM_NUMBER*4);
	// }
	// break;
	case RANDOMWORLD_YESORNO:
	{
		// if (RANDOMWORLD_RANDOM_NUMBER)
		// {
		// 	setStandardTextToTextBuf(TEXT_YES);
		// }
		// else
		// {
		// 	setStandardTextToTextBuf(TEXT_NO);
		// }
		setStandardTextToTextBuf(60 + RANDOMWORLD_RANDOM_NUMBER * 4 + (RANDOMWORLD_RANDOM_TYPE - 3) * 2);
	}
	break;
	}

}

void Apps::modeSimpleButtonsAndLights(bool init)
{
	lights = 0b00000000; //reset before switch enquiry
	const uint8_t analog_input_pins[4] = {PIN_SELECTOR_DIAL, PIN_BUTTONS_1, PIN_BUTTONS_2, PIN_MERCURY_SWITCHES};

	if (init)
	{
		SETTINGS_MODE_SELECTOR = 0;
	}

	// // back and forth motion required of the potentio to count up modes
	if (encoder_dial->getDelta() < 0 && SETTINGS_MODE_SELECTOR % 2 == 0)
	{
		SETTINGS_MODE_SELECTOR++;
	}
	else if (encoder_dial->getDelta() > 0 && SETTINGS_MODE_SELECTOR % 2 != 0)
	{
		SETTINGS_MODE_SELECTOR++;
	}

	setStandardTextToTextBuf(TEXT_SPACES);

	if (SETTINGS_MODE_SELECTOR < 6)
	{
		//normal mode

		// simple repetitive, predictive mode.
		// each button triggers its corresponding light.
		// potentio sets display brightness
		// no buzzer
		// display lights up a segment for each button.

		//delete all content from screen.
		// setBlankDisplay();

		for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
		{
			// every momentary button has a matching latching button assigned
			if (binaryInputs[buttons_indexed[i + 4]].getValue())
			{
				
				// momentary button pressed while its matching latching button is ON?

				lights |= 1 << lights_indexed[i + 4];

				if (binaryInputs[buttons_indexed[i]].getValue())
				{
					lights |= 1 << lights_indexed[i];
					textHandle[i] = '8';
				}
				else
				{
					textHandle[i] = ONLY_MIDDLE_SEGMENT_FAKE_ASCII;
				}
			}
			else if (binaryInputs[buttons_indexed[i]].getValue())
			{
				textHandle[i] = '0';
			}
			else
			{
				textHandle[i] = ' ';
			}
		}

		allLights->setBrightness((byte)(50 - encoder_dial->getValueLimited(50,false)), false);
	}
	else if (SETTINGS_MODE_SELECTOR < 8)
	{
		lights |= 1 << LIGHT_MOMENTARY_0;
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
		{
#ifdef ENABLE_EEPROM

			eeprom_update_byte(
				(uint8_t *)EEPROM_SOUND_OFF_BY_DEFAULT,
				!eeprom_read_byte((uint8_t *)EEPROM_SOUND_OFF_BY_DEFAULT));
#endif

			if (buzzer->getPin() == PIN_BUZZER)
			{
				buzzer->setPin(PIN_BUZZER_FAKE);
			}
			else
			{
				buzzer->setPin(PIN_BUZZER);
			}
		}

		if (millis() % 1000 < 500)
		{
			setStandardTextToTextBuf(TEXT_BEEP);
		}
		else
		{

			uint8_t text = TEXT_YES;
			if (buzzer->getPin() == PIN_BUZZER)
			{

				text = TEXT_NO;
			}
			setStandardTextToTextHANDLE(text);
		}
	}
	else if (SETTINGS_MODE_SELECTOR < 16)
	{

		// menu title
		textBuf[2] = 'A';
		uint8_t index = (SETTINGS_MODE_SELECTOR - 8) / 2;
		textBuf[3] = 48 + index;

		// Value
		ledDisp->setNumberToDisplayAsDecimal((int16_t)analogRead(analog_input_pins[index]));
	}

	else if (SETTINGS_MODE_SELECTOR < 18)
	{
		lights |= 1 << LIGHT_MOMENTARY_0;
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
		{

#ifdef ENABLE_EEPROM
			for (uint16_t i = 0; i < 100; i = i + 2)
			{
				eeprom_update_word(
					(uint16_t *)i,
					//i,
					0);
			}
#endif
			loadBuzzerTrack(SONG_DRYER_HAPPY);
		}
		else
		{
			setStandardTextToTextHANDLE(TEXT_RESET);
		}
	}
	else
	{
		ledDisp->setNumberToDisplayAsDecimal(SETTINGS_MODE_SELECTOR);
	}

	if (SETTINGS_MODE_SELECTOR >= 6)
	{
		// show values one seconds, menu items half a second
		// in real settings mode
		if (millis() % 1000 < 500)
		{
			textBufToDisplay();
		}
	}

	setLedArray(lights);
	//setLedArray(0);
}
void Apps::displayLetterAndPositionInAlphabet(char *textBuf, int16_t letterValueAlphabet)
{
	if (letterValueAlphabet > 8)
	{
		textBuf[0] = (letterValueAlphabet + 1) / 10 + 48;
	}
	textBuf[1] = (letterValueAlphabet + 1) % 10 + 48;
	textBuf[3] = letterValueAlphabet + 65; // show letters alphabet.
}

void Apps::modeCountingLettersAndChars(bool init)
{
	//counting mode: numbers and letters. 

	NUMBERS_AND_LETTERS_NUMBER_ELSE_LETTER_MODE = !(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT));

	if (init)
	{
		counter = 0;
		COUNTING_LETTERS_AND_CHARS_TIMER.setInitTimeMillis(-1000);
		NUMBERS_AND_LETTERS_COUNT_UP_ELSE_DOWN = true;
	}

	// when potentio setting init time, it overrules the updateScreen and displays its value. updateScreen erases potentio value display..

	// setBlankDisplay();
	if (NUMBERS_AND_LETTERS_NUMBER_ELSE_LETTER_MODE)
	{
		ledDisp->setNumberToDisplayAsDecimal(counter);
	}
	else
	{
		displayLetterAndPositionInAlphabet(textHandle, counter);
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
	{
		NUMBERS_AND_LETTERS_COUNT_UP_ELSE_DOWN = !NUMBERS_AND_LETTERS_COUNT_UP_ELSE_DOWN;
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
	{
		counter = 0;
	}
	
	modifyValueUpDownWithMomentary2And3(&counter, 1);

 	// auto count
	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_LATCHING_EXTRA) )
	{
		if (!NUMBERS_AND_LETTERS_NUMBER_ELSE_LETTER_MODE){
			buzzer->setSpeedRatio(4);
			loadBuzzerTrack(SONG_ALPHABET);
		}
		COUNTING_LETTERS_AND_CHARS_TIMER.start();
	}

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
	{
		// auto mode next item
		if (!COUNTING_LETTERS_AND_CHARS_TIMER.getTimeIsNegative())
		{
			counter += -1 + (2 * NUMBERS_AND_LETTERS_COUNT_UP_ELSE_DOWN);
			COUNTING_LETTERS_AND_CHARS_TIMER.start();
		}

		// set timer speed
		dialOnEdgeChangeInitTimerPercentage(&COUNTING_LETTERS_AND_CHARS_TIMER);
	}
	else // if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
	{
		// show number right away depending on potentio value
		//counter = (int16_t)(encoder_dial->getValueLimited(25 + NUMBERS_AND_LETTERS_NUMBER_ELSE_LETTER_MODE * 75, false)); //1024 to 26 letters.
		counter += encoder_dial->getDelta();
	}

	//only do the characters of the alphabet in lettermode.
	checkBoundaries(&counter, 0, 25 + (NUMBERS_AND_LETTERS_NUMBER_ELSE_LETTER_MODE * 76), true);
}

void Apps::modeSoundSong(bool init)
{
	if (init)
	{
		loadBuzzerTrack(SONG_DRYER_HAPPY);
	}

	// setBlankDisplay();

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
	{
		buzzer->changeTranspose(encoder_dial->getDelta()); 
	}
	else
	{
		buzzerChangeSpeedRatioWithEncoderDial();
	}

	uint8_t shift = (4 * ((binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT)) > 0));

	for (uint8_t index = 0; index < MOMENTARY_BUTTONS_COUNT; index++)
	{

		if (binaryInputs[buttons_indexed[index]].getEdgeUp())
		{
			if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT))
			{
				uint8_t song[32];
#ifdef ENABLE_EEPROM
				saveLoadFromEepromSlot(song, index + shift, EEPROM_SEQUENCER_SONG_LENGTH, EEPROM_SEQUENCER_SONGS_START_ADDRESS, true);
#endif
				for (uint8_t i = 0; i < 32; i++)
				{
					addNoteToBuzzer(song[i]);
				}
			}
			else
			{
				loadBuzzerTrack(index + shift);
			}
		}
	}
	buzzer->lastPlayedNoteToDisplay(textHandle, decimalDotsHandle);
}

void Apps::modeComposeSong(bool init)
{

	bool defaultDisplay = true;
	int16_t step = 0;

	if (init)
	{
		for (uint8_t i = 0; i < bytes_list_bufsize; i++)
		{
			COMPOSER_SONG[i] = BUZZER_ROLL_SONG_STOPVALUE;
		}
		COMPOSER_SONG_LENGTH = 1;
		COMPOSER_SONG[0] = rest_1; //default note
		COMPOSER_STEP = 0;
		COMPOSER_STEP_TIMER.setInitTimeMillis(-200);
	}

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT))
	{
#ifdef ENABLE_EEPROM
		bool loaded;
		loaded = saveLoadMenu(COMPOSER_SONG, 4, EEPROM_COMPOSER_SONG_LENGTH, EEPROM_COMPOSER_SONGS_START_ADDRESS);

		if (loaded)
		{
			COMPOSER_STEP = 0;
			// search for the last note in the composed song.
			// assume last note of longest possible song is always BUZZER_ROLL_SONG_STOPVALUE
			for (uint8_t i = bytes_list_bufsize - 2; i > 0; i--)
			{
				COMPOSER_SONG_LENGTH = i + 1;
				if (COMPOSER_SONG[i] != BUZZER_ROLL_SONG_STOPVALUE)
				{

					break;
				}
			}
		}
#endif
	}
	else
	{

		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
		{
			// display song notes by their position (index) in song (enable insert delete position)

			if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
			{
				//delete current position

				// move all notes one position down.
				for (uint8_t i = COMPOSER_STEP; i < bytes_list_bufsize - 1; i++)
				{
					COMPOSER_SONG[i] = COMPOSER_SONG[i + 1];
				}
				// deleted space should be a song stop note.
				COMPOSER_SONG[COMPOSER_SONG_LENGTH - 1] = BUZZER_ROLL_SONG_STOPVALUE;

				//adjust length
				COMPOSER_SONG_LENGTH--;
			}

			if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
			{
				//insert after current index (and move to it)
				if (COMPOSER_SONG_LENGTH >= bytes_list_bufsize - 2)
				{
					//max length reached
				}
				else
				{
					// remember, last note of longest song possible MUST be BUZZER_ROLL_SONG_STOPVALUE, don't copy a note to it.
					for (uint8_t i = bytes_list_bufsize - 3; i > COMPOSER_STEP; i--)
					{
						COMPOSER_SONG[i + 1] = COMPOSER_SONG[i];
					}
					COMPOSER_SONG[COMPOSER_STEP + 1] = rest_1;
					COMPOSER_SONG_LENGTH++;
					step = 1; // move to new position
				}
			}
			ledDisp->setNumberToDisplayAsDecimal(COMPOSER_STEP);
		}
		else
		{
			// display song by note (enable programming and listening to notes)
			if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
			{
				// just listen to note on index in song
				buzzerOffAndAddNote(COMPOSER_SONG[COMPOSER_STEP]);
			}

			if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
			{
				// just play notes selected with dial
				if (encoder_dial->getDelta())
				{
					uint8_t note = (uint8_t)encoder_dial->getValueLimited(255, false);
					buzzerOffAndAddNote(note);
					noteToDisplay(note);
				}
				defaultDisplay = false;
			}

			// program note in song
			if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
			{
				COMPOSER_SONG[COMPOSER_STEP] = (uint8_t)encoder_dial->getValueLimited(255, true);
				buzzerOffAndAddNote(COMPOSER_SONG[COMPOSER_STEP]);

				// if note added to end, expand song length and add default note
				if (COMPOSER_STEP == COMPOSER_SONG_LENGTH - 1)
				{
					COMPOSER_SONG_LENGTH++;
					COMPOSER_SONG[COMPOSER_SONG_LENGTH - 1] = rest_1; //default note
				}
			}

			if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
			{
				if (encoder_dial->getDelta())
				{
					COMPOSER_SONG[COMPOSER_STEP] = (uint8_t)encoder_dial->getValueLimited(255, true);
					buzzerOffAndAddNote(COMPOSER_SONG[COMPOSER_STEP]);
				}
			}
		}

		modifyValueUpDownWithMomentary2And3(&step, 1);

		// autoplay
		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
		{

			if (!COMPOSER_STEP_TIMER.getTimeIsNegative())
			{
				step = 1;
				COMPOSER_STEP_TIMER.start();
			}
		}

		//default potentio behaviour
		byte momentary_buttons_mask = 1 << BUTTON_INDEXED_MOMENTARY_0 | 1 << BUTTON_INDEXED_MOMENTARY_1 | 1 << BUTTON_INDEXED_MOMENTARY_2 | 1 << BUTTON_INDEXED_MOMENTARY_3;
		if ((binaryInputsValue & momentary_buttons_mask) == 0) // no buttons pressed
		{
			if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
			{
				// change speed if default behaviour of potentio.
				COMPOSER_STEP_TIMER.setInitTimeMillis(COMPOSER_STEP_TIMER.getInitTimeMillis() + encoder_dial->getDelta() *10); //step +1 or -1
			}
			else
			{
				step = encoder_dial->getDelta(); ////step +1 or -1
			}
		}

		if (step != 0)
		{
			nextStepRotate(&COMPOSER_STEP, (step + 1) / 2, 0, COMPOSER_SONG_LENGTH - 1);
			addNoteToBuzzer(COMPOSER_SONG[COMPOSER_STEP]);
		}

		//sometimes overrule screen if potentio looking for a note.
		if (defaultDisplay)
		{
			if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
			{
				ledDisp->setNumberToDisplayAsDecimal(COMPOSER_STEP);
			}
			else
			{
				noteToDisplay( COMPOSER_SONG[COMPOSER_STEP]);
			}
		}
	}
}

void Apps::modeSoundNotesInitScale()
{
	SOUND_NOTES_NOTE_INDEX = SOUND_NOTES_SCALE_ROOT;
	SOUND_NOTES_NOTE_ON_SCALE_INDEX = 0;
	SOUND_NOTE_PLAY_NOTE = true;
}

void Apps::modeSoundNotes(bool init)
{
	bool update_note = false;

	if (init)
	{
		SOUND_NOTES_PROGRESSION_MODE = SOUND_NOTE_MODE_MANUAL;
		SOUND_NOTE_AUTO_TIMER.setInitTimeMillis(-500);
		SOUND_NOTE_AUTO_TIMER.start();
		SOUND_NOTES_SCALE_ROOT = C4_4;
		modeSoundNotesInitScale();

		SOUND_NOTES_SCALE_INDEX = 0;

		SOUND_NOTE_SETTING_TEXT_TO_DISPLAY = SOUND_NOTE_DISPLAY_NOTE;
	}

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
	{
		// auto play
		if (!SOUND_NOTE_AUTO_TIMER.getTimeIsNegative())
		{
			SOUND_NOTE_AUTO_TIMER.start();
			update_note = true;
		}

		dialOnEdgeChangeInitTimerPercentage(&SOUND_NOTE_AUTO_TIMER);
	}
	else
	{
		// change note with potentio
		if (encoder_dial->getDelta())
		{
			SOUND_NOTE_AUTO_UP_ELSE_DOWN = encoder_dial->getDelta()>0;
			update_note = true;
		}
	}

	// change note length,available in all modes.
	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
	{
		buzzer->changeNoteToNextLength(&SOUND_NOTES_NOTE_INDEX);
		SOUND_NOTE_PLAY_NOTE = true;
	}

	// advanced vs manual controls
	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT))
	{
		// advanced

		//  change scale
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
		{
			nextStepRotate(&SOUND_NOTES_SCALE_INDEX, 1, 0, SCALES_COUNT);
			SOUND_NOTE_SETTING_TEXT_TO_DISPLAY = 76 + SOUND_NOTES_SCALE_INDEX * 4;
			modeSoundNotesInitScale();
		}

		// change key
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3)) // set key when changing scale

		{
			// first keypress, back to root.
			// second keypress, change root.
			if (SOUND_NOTES_NOTE_INDEX == SOUND_NOTES_SCALE_ROOT)
			{
				SOUND_NOTES_SCALE_ROOT++;
				if (SOUND_NOTES_SCALE_ROOT > B5_4)
				{
					SOUND_NOTES_SCALE_ROOT = SOUND_NOTES_SCALE_ROOT;
				}
			}
			modeSoundNotesInitScale();
		}

		// change progression mode
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
		{
			SOUND_NOTES_PROGRESSION_MODE++;
			if (SOUND_NOTES_PROGRESSION_MODE > 5)
			{
				SOUND_NOTES_PROGRESSION_MODE = SOUND_NOTE_MODE_MANUAL;
			}
			SOUND_NOTE_SETTING_TEXT_TO_DISPLAY = 96 + 4 * SOUND_NOTES_PROGRESSION_MODE;
		}
	}
	else
	{
		// easy

		// just play the active note
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
		{
			SOUND_NOTE_PLAY_NOTE = true;
		}

		// change note up / downwith button press
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2) ||
		   (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		{
			SOUND_NOTE_AUTO_UP_ELSE_DOWN = (binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3));
			update_note = true;
		}
	}

	if (update_note)
	{
		uint8_t note_jumps = 1;
		//number of notes to skip
		if (SOUND_NOTES_PROGRESSION_MODE == SOUND_NOTE_MODE_RANDOM)
		{
			note_jumps = random(0, 100);
		}

		if (SOUND_NOTES_PROGRESSION_MODE == SOUND_NOTE_MODE_ARPEGGIO_SAWTOOTH)
		{
			if (random(0, 5) == 0)
			{
				SOUND_NOTE_AUTO_UP_ELSE_DOWN = !SOUND_NOTE_AUTO_UP_ELSE_DOWN;
			}
		}

		if (SOUND_NOTES_PROGRESSION_MODE == SOUND_NOTE_MODE_ARPEGGIO_UP)
		{
			SOUND_NOTE_AUTO_UP_ELSE_DOWN = true;
		}

		if (SOUND_NOTES_PROGRESSION_MODE == SOUND_NOTE_MODE_ARPEGGIO_DOWN)
		{
			SOUND_NOTE_AUTO_UP_ELSE_DOWN = false;
		}

		if (SOUND_NOTES_PROGRESSION_MODE == SOUND_NOTE_MODE_RANDOM_ERRATIC)
		{
			SOUND_NOTE_AUTO_UP_ELSE_DOWN = random(0, 2);
			note_jumps = random(0, 3);
		}

		// every jump is one step on the scale.
		for (uint8_t note_jump = 0; note_jump < note_jumps; note_jump++)
		{
			// which scale are we on
			uint8_t scale_start_index = pgm_read_byte_near(scale_start_indeces + SOUND_NOTES_SCALE_INDEX);

			uint8_t distance_to_next_note_on_scale = pgm_read_byte_near(scales + scale_start_index + SOUND_NOTES_NOTE_ON_SCALE_INDEX);

			// next index on scale
			nextStepRotate(&SOUND_NOTES_NOTE_ON_SCALE_INDEX,
						   SOUND_NOTE_AUTO_UP_ELSE_DOWN,
						   0,
						   (int16_t)pgm_read_byte_near(scale_lengths + SOUND_NOTES_SCALE_INDEX));

			if (!SOUND_NOTE_AUTO_UP_ELSE_DOWN)
			{
				distance_to_next_note_on_scale = pgm_read_byte_near(scales + scale_start_index + SOUND_NOTES_NOTE_ON_SCALE_INDEX);
			}

			// take number of steps on the chromatic scale to go to desired note
			for (uint8_t i = 0; i < distance_to_next_note_on_scale; i++)
			{
				buzzer->nextNote(
					&SOUND_NOTES_NOTE_INDEX,
					SOUND_NOTE_AUTO_UP_ELSE_DOWN,
					true);
			}
		}

		SOUND_NOTE_SETTING_TEXT_TO_DISPLAY = SOUND_NOTE_DISPLAY_NOTE;
		SOUND_NOTE_PLAY_NOTE = true;
	}

	if (SOUND_NOTE_PLAY_NOTE)
	{
		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
		{
			buzzerOff();
		}
		addNoteToBuzzer(SOUND_NOTES_NOTE_INDEX);
		SOUND_NOTE_PLAY_NOTE = false;
	}

	// index to actual note on the scale
	// USE A TIMER to end display of settings. hmmm not sure if it will be that much better...
	if (SOUND_NOTE_SETTING_TEXT_TO_DISPLAY == SOUND_NOTE_DISPLAY_NOTE)
	{
		noteToDisplay( SOUND_NOTES_NOTE_INDEX);
	}
	else
	{
		setStandardTextToTextHANDLE(SOUND_NOTE_SETTING_TEXT_TO_DISPLAY);
	}
}

void Apps::movieAnimationMode(bool init)
{
	//reset saved led disp state.
	if (init)
	{
		MOVIE_MODE_SHOW_NEGATIVE = false;
		MOVIE_MODE_FRAME_INTERVAL_TIMER.setInitTimeMillis(-500);
		MOVIE_MODE_FRAME_INTERVAL_TIMER.start();
		MOVIE_MODE_FLASH_FRAME_INDEX = 0;
		loadNextMovie();
	}

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
	{
		// manual mode
		MOVIE_MODE_FLASH_FRAME_INDEX += encoder_dial->getDelta();

		// one step forward
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
		{
			MOVIE_MODE_FLASH_FRAME_INDEX++;
		}

		// one step backward
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
		{
			MOVIE_MODE_FLASH_FRAME_INDEX--;
		}
	}
	else
	{
		// auto mode.

		dialOnEdgeChangeInitTimerPercentage(&MOVIE_MODE_FRAME_INTERVAL_TIMER);

		if (!MOVIE_MODE_FRAME_INTERVAL_TIMER.getTimeIsNegative())
		{
			MOVIE_MODE_FLASH_FRAME_INDEX += (1 - 2*MOVIE_MODE_AUTO_BACKWARDS);
			MOVIE_MODE_FRAME_INTERVAL_TIMER.start();
		}

		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
		{
			// this->dataPlayer.setSetIndexDirection(2);
			// MOVIE_MODE_AUTO_BACKWARDS = !MOVIE_MODE_AUTO_BACKWARDS;
			MOVIE_MODE_AUTO_BACKWARDS = true;
		}

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		{
			// this->dataPlayer.update(); // this to pause the movie while holding.
			MOVIE_MODE_AUTO_BACKWARDS = false;
		}
	}

	// next movie
	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
	{
		loadNextMovie();
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
	{
		MOVIE_MODE_SHOW_NEGATIVE = !MOVIE_MODE_SHOW_NEGATIVE;
	}

	// check limits of movie 
	if (MOVIE_MODE_FLASH_FRAME_INDEX >  MOVIE_MODE_MOVIE_FRAME_INDEX_END){ 
		MOVIE_MODE_FLASH_FRAME_INDEX = MOVIE_MODE_MOVIE_FRAME_INDEX_START;

	}else if (MOVIE_MODE_FLASH_FRAME_INDEX <  MOVIE_MODE_MOVIE_FRAME_INDEX_START){
		MOVIE_MODE_FLASH_FRAME_INDEX =  MOVIE_MODE_MOVIE_FRAME_INDEX_END;
	}

	loadScreenFromMemory(MOVIE_MODE_FLASH_FRAME_INDEX);

	// invert all data in picture
	if (MOVIE_MODE_SHOW_NEGATIVE)
	{
		// negative .
		displayAllSegments = ~displayAllSegments;
	}

	// set to display
	displayAllSegmentsToScreen();
}

bool Apps::loadScreenFromMemory(int16_t frame_index)
{
	this->displayAllSegments = 0;

	if (frame_index < MAX_FRAMES_MOVIES_FLASH){
		//flash
		flashPictureToDisplayAllSegments(disp_4digits_animations + frame_index*4);
	}else{
		//eeprom
		eepromPictureToDisplayAllSegments(EEPROM_PICTURES_START_ADDRESS, frame_index - MAX_FRAMES_MOVIES_FLASH);
	}

	// check for end of movie
	uint32_t stop_screen = (uint32_t)ANIMATION_STOP_CODE_PART_0 <<0 |  (uint32_t)ANIMATION_STOP_CODE_PART_1 <<8 |  (uint32_t)ANIMATION_STOP_CODE_PART_2 <<16 |  (uint32_t)ANIMATION_STOP_CODE_PART_3 <<24;

	if(displayAllSegments == stop_screen){
		return false;
	}else{
		return true;
	}
}



void Apps::loadNextMovie(){

	bool startFrameSet = false;

	// set new stop frame index
	while (loadScreenFromMemory(MOVIE_MODE_FLASH_FRAME_INDEX) || !startFrameSet){

		// check if over limit (last movie)
		if (MOVIE_MODE_FLASH_FRAME_INDEX >= (MAX_FRAMES_MOVIES_FLASH + EEPROM_NUMBER_OF_DRAWINGS)){
			startFrameSet = true;
			MOVIE_MODE_MOVIE_FRAME_INDEX_START = 1;
			MOVIE_MODE_FLASH_FRAME_INDEX = 1;
		}

		// presume for next iteration to not come anymore.
		if (!startFrameSet){
			MOVIE_MODE_MOVIE_FRAME_INDEX_START = MOVIE_MODE_FLASH_FRAME_INDEX + 1;
		}else{
			MOVIE_MODE_MOVIE_FRAME_INDEX_END = MOVIE_MODE_FLASH_FRAME_INDEX ;
		}
		
		if (!loadScreenFromMemory(MOVIE_MODE_FLASH_FRAME_INDEX)){
			startFrameSet = true;
		}
		
		MOVIE_MODE_FLASH_FRAME_INDEX++;
	}
}

void Apps::displayChangeGlobal(uint32_t *display_buffer, bool saveStateToBuffer)
{
	// global picture operations
	if (saveStateToBuffer)
	{
		DRAW_SHOW_MODE = 3; // prepare for next button press to save buffer and show inversion.
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
	{
		DRAW_SHOW_MODE >= 4 ? DRAW_SHOW_MODE = 0 : DRAW_SHOW_MODE++;

		switch (DRAW_SHOW_MODE)
		{
		case 0:
			//invert
			displayAllSegmentsBuffer = *display_buffer;
			*display_buffer = ~*display_buffer;
			break;

		case 1:
			//blank
			*display_buffer = 0;
			break;
			
		case 2:
			//special stop frame (for end of animations)
			*display_buffer = (uint32_t)ANIMATION_STOP_CODE_PART_0 <<0 |  (uint32_t)ANIMATION_STOP_CODE_PART_1 <<8 |  (uint32_t)ANIMATION_STOP_CODE_PART_2 <<16 |  (uint32_t)ANIMATION_STOP_CODE_PART_3 <<24;
			break;
			
		case 3:
			//full
			*display_buffer = 0xFFFFFFFF;
			break;
			
		case 4:
			//restore
			*display_buffer = displayAllSegmentsBuffer;
		}
	}
}

uint32_t Apps::modeSingleSegmentManipulation(uint32_t *display_buffer)
{
	// return blinking segment.
	// no need to initialize DRAW_CURSOR_INDEX, it get's within boundaries in one cycle. And we don't care about starting position.

	uint8_t segmentMoveIndexed[9] = {0x20, 0x10, 0x00, 0x01, 0x40, 0x08, 0x02, 0x04, 0x80}; // 0x00 for empty . It's good to have spots where the cursor is invisible. In order not to pollute the display if you want to really see your drawing.

	// scroll through segments
	if (encoder_dial->getDelta())
	{
		DRAW_CURSOR_INDEX = encoder_dial->getValueLimited(71,true) / 2; // 36 positions, but, make dial less sensitive
	}

	// check for global display change
	this->displayChangeGlobal(&displayAllSegments, false);

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
	{
		*display_buffer ^= (uint32_t)(segmentMoveIndexed[DRAW_CURSOR_INDEX % 9]) << (DRAW_CURSOR_INDEX / 9) * 8;
		this->displayChangeGlobal(display_buffer, true);
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
	{
		// move inside digit
		DRAW_CURSOR_INDEX++;
		if ((DRAW_CURSOR_INDEX) % 9 == 0)
		{
			DRAW_CURSOR_INDEX -= 9;
		}
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
	{
		// move digit
		DRAW_CURSOR_INDEX += 9;
	}

	// set limits on cursor position
	if (DRAW_CURSOR_INDEX < 0)
	{
		DRAW_CURSOR_INDEX = 31;
	}

	if (DRAW_CURSOR_INDEX > 35)
	{
		DRAW_CURSOR_INDEX = 0;
	}

	//add blinking cursor. (depending on time, we set the active segment)
	if (millis() % 250 > 125)
	{
		return (uint32_t)(segmentMoveIndexed[DRAW_CURSOR_INDEX % 9]) << (DRAW_CURSOR_INDEX / 9) * 8;
	}
	else
	{
		return 0;
	}
}

void Apps::drawGame(bool init)
{
	// shows a picture. After it disappears, you have to draw it exactly as it was.

	uint32_t cursorBlinker = 0;

	if (init)
	{
		drawGameState = drawGameWaitForStart;
	}

	switch (drawGameState)
	{

	case drawGameWaitForStart:
	{
		// setBlankDisplay();

		drawGameState = drawGameShowPicture;

		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT))
		{

			if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
			{
				// learn how to read the clock

				ledDisp->minutesToMinutesHoursString(textBuf, (uint16_t)random(0, 1440));
				ledDisp->convert_text4Bytes_to_32bits(textBuf, &displayAllSegments);

				// add hour:minute divider.
				displayAllSegments |= 1UL << 15;
			}
			else
			{
				// random number
				long r = random(0, 10000);
				numberToBufAsDecimal((int16_t)r);
				ledDisp->convert_text4Bytes_to_32bits(textBuf, &displayAllSegments);
			}
		}
		else if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
		{
			// random text
			for (uint8_t i = 0; i < 4; i++)
			{
				long r = random(0, 25);
				textBuf[i] = (char)r + 65;
			}
			ledDisp->convert_text4Bytes_to_32bits(textBuf, &displayAllSegments);
		}
		else
		{
			// random segments
			displayAllSegments = 0UL;
			for (uint8_t i = 0; i < 32; i++)
			{
				displayAllSegments |= random(0, 2) << i;
			}
		}

		break;
	}

	case drawGameShowPicture:
	{
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
		{
			drawGameState = drawGameDraw;
			displayAllSegmentsBuffer = displayAllSegments;
			displayAllSegments = 0;
		}
		byte option_latching_buttons_mask = 1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT | 1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT ;
		if (( (binaryInputsEdgeDown | binaryInputsEdgeUp) & option_latching_buttons_mask) != 0) // a button edge detecred
		{
			drawGameState = drawGameWaitForStart;
		}
		break;
	}

	case drawGameDraw:
	{
		cursorBlinker = modeSingleSegmentManipulation(&displayAllSegments);
		// bug: you can't use this because of same buffer reuse. displayChangeGlobal(&displayAllSegments, false);

		if (binaryInputs[BUTTON_LATCHING_EXTRA].getValueChanged())
		{
			drawGameState = drawGameEvaluate;
			DRAW_GAME_DISPLAY_TIMER.start();

			if (displayAllSegments == displayAllSegmentsBuffer)
			{
				loadBuzzerTrack(SONG_DRYER_HAPPY);
			}
			else
			{
				loadBuzzerTrack(SONG_DRYER_UNHAPPY);
			}
		}
		break;
	}

	case drawGameEvaluate:
	{
		// wait for user input to continue.
		byte momentary_buttons_mask = 1 << BUTTON_INDEXED_MOMENTARY_0 | 1 << BUTTON_INDEXED_MOMENTARY_1 | 1 << BUTTON_INDEXED_MOMENTARY_2 | 1 << BUTTON_INDEXED_MOMENTARY_3;
		if ((binaryInputsEdgeUp & momentary_buttons_mask) != 0) // a button pressed
		{
			drawGameState = drawGameWaitForStart;
		}

		// show difference result with original drawing
		if (DRAW_GAME_DISPLAY_TIMER.getEdgeSinceLastCallFirstGivenHundredsPartOfSecond(500, true, true))
		{
			uint32_t displayAllSegments_swap_buffer;
			displayAllSegments_swap_buffer = displayAllSegments;
			displayAllSegments = displayAllSegmentsBuffer;
			displayAllSegmentsBuffer = displayAllSegments_swap_buffer;
		}
		break;
	}

	default:
		break;
	}
	ledDisp->setBinaryToDisplay(displayAllSegments ^ cursorBlinker);
}

void Apps::modeHackTime(bool init)
{
	// run through all the addresses to see the raw values!

	bool address_changed = false;
	const char drive_letter[3] = {'F', 'R', 'E'};

	if (init)
	{
		// HACKTIME_ADDRESS = 0;
		// HACKTIME_DISPLAY_MODE = HACKTIME_DISPLAY_ADDRESS;
		HACKTIME_MOVE_TIMER.setInitTimeMillis(-500);
		HACKTIME_MOVE_TIMER.start();
	}

	
	// setBlankDisplay();

	// write to mem if possible
	if (!init && (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT)) 
		)
	{ 

		// change value in address location (left char on display)
		// will not work for flash memory
		array_8_bytes[0] += encoder_dial->getDelta();

		if (encoder_dial->getDelta()){

			// store value.
			switch (HACKTIME_MEMORY_SELECT)
			{
			// case HACKTIME_MEMORY_FLASH:
				//it's not possible to write to flash
			// break;

			case HACKTIME_MEMORY_RAM:
				*((uint8_t *)HACKTIME_ADDRESS) = array_8_bytes[0];
				addNoteToBuzzer(C5_8);
				break;

			case HACKTIME_MEMORY_EEPROM:
				eeprom_write_byte((uint8_t *)HACKTIME_ADDRESS, array_8_bytes[0]);
				addNoteToBuzzer(C5_8);
				break;
			}
		}
	}
	else
	{
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
		{
			if ((binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))){
				// which memory are we investigating?
				nextStepRotate(&HACKTIME_MEMORY_SELECT, 1, 0, 2);
			}else{
				// display mode change (how to represent the memory value?)
				nextStepRotate(&HACKTIME_DISPLAY_MODE, 1, 0, 3);
			}
		}
		
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1)){
			// sound mode.
			HACKTIME_VALUE_TO_SOUND = !HACKTIME_VALUE_TO_SOUND;
		}

		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
		{
			// auto scroll mode
			if (!HACKTIME_MOVE_TIMER.getTimeIsNegative())
			{
				HACKTIME_ADDRESS++;
				HACKTIME_MOVE_TIMER.start();
				address_changed = true;
			}

			// set speed.
			dialOnEdgeChangeInitTimerPercentage(&HACKTIME_MOVE_TIMER);
		}
		else
		{
			// manual scroll
			if (encoder_dial->getDelta()){
				address_changed = true;
				HACKTIME_ADDRESS += encoder_dial->getDelta() * ( 
						1 
						+ 15* ((binaryInputsValue & 1<<BUTTON_INDEXED_MOMENTARY_3)>0)
						+ 255* ((binaryInputsValue & 1<<BUTTON_INDEXED_MOMENTARY_2)>0)  
						//+ 4095 *((binaryInputsValue & 1<<BUTTON_INDEXED_MOMENTARY_1)>0) // saving memory here. but, it's not really needed. with 32000 address locations, going time 255 is fast enough. (takes about 125 steps)
				 	)
				  	;  
			}else{

				// button address change.
				address_changed = address_changed || modifyValueUpDownWithMomentary2And3(&HACKTIME_ADDRESS, 1);
			}
			
		}


		// ok ok, let's do one little check.
		if (HACKTIME_ADDRESS <= 0)
		{
			HACKTIME_ADDRESS = 0;
		}
		
		
		// get value from memory address and memory type
		for (uint8_t i = 0; i < 4; i++)
		{
			switch (HACKTIME_MEMORY_SELECT)
			{
			case HACKTIME_MEMORY_FLASH:
				textHandle[i] = pgm_read_byte(HACKTIME_ADDRESS + i);
				break;

			case HACKTIME_MEMORY_RAM:
				textHandle[i] = *(((uint8_t *)HACKTIME_ADDRESS) + i);
				break;

			case HACKTIME_MEMORY_EEPROM:
				textHandle[i] = eeprom_read_byte((uint8_t *)HACKTIME_ADDRESS + i);
				break;
			}
			array_8_bytes[i] = textHandle[i];
		}
	}

	// convert memory to sounds... Be prepared for a post-modernistic masterpiece.
	if (address_changed && HACKTIME_VALUE_TO_SOUND)
	{ 
		buzzerOffAndAddNote(array_8_bytes[0]);
	}

	// display 
	
	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT)){
		// display address location

		if (millis() % 1000 > 200)
		{
			ledDisp->setNumberToDisplay(HACKTIME_ADDRESS, true); // show address as hex, to fit all addresses on 4 chars display
		}
		else
		{
			textHandle[0] = drive_letter[HACKTIME_MEMORY_SELECT];
			// ledDisp->setBlankDisplay();
			textHandle[1] = SPACE_FAKE_ASCII;
			textHandle[2] = SPACE_FAKE_ASCII;
			textHandle[3] = SPACE_FAKE_ASCII;
		}

	}else{
		// compressed display mode (to save memory)
		if (HACKTIME_DISPLAY_MODE == HACKTIME_DISPLAY_CHARS)
		{
			// // do nothing
			// for(uint8_t i=0;i<4;i++){
			// 	textHandle[i] = array_8_bytes[i];
			// }
		}
		else
		{
			if (HACKTIME_DISPLAY_MODE == HACKTIME_DISPLAY_BYTES)
			{
				displayAllSegments = 0;
				for (uint8_t i = 0; i < 4; i++)
				{
					displayAllSegments |= ((uint32_t)(array_8_bytes[i])) << (8 * i);
				}
				displayAllSegmentsToScreen();
			}
			
			else
			{
				ledDisp->setNumberToDisplay(array_8_bytes[0], HACKTIME_DISPLAY_MODE == HACKTIME_DISPLAY_HEX);
			}
		}
	}
}

bool Apps::modifyValueUpDownWithMomentary2And3(int16_t *value, uint8_t amount)
{

	int16_t value_memory = *value;

	//if (binaryInputsEdgeUp & (1<<BUTTON_INDEXED_MOMENTARY_2))
	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
	{
		*value -= amount;
	}

	// if (binaryInputsEdgeUp & (1<<BUTTON_INDEXED_MOMENTARY_3))
	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
	{
		*value += amount;
	}
	return value_memory != *value;
}

void Apps::dialOnEdgeChangeInitTimerPercentage(SuperTimer *aTimer)
{
	// fixed to one percent for now.
	// only works for countdown times (negative init value)!
	if (encoder_dial->getDelta()){
		long original = (aTimer->getInitTimeMillis());
		long result = long((float)original* ( 1 - (float)(encoder_dial->getDelta()) * 0.01));
		// if value to small to make an absolute difference, force it! (make sure to stay negative)
		if (original == result){
			result -= encoder_dial->getDelta() * encoder_dial->getDelta();
		}

		aTimer->setInitTimeMillis(
			result	 
			);
	}
}

void Apps::draw(bool init)
{

	// scroll through drawings from eeprom memories
	// change drawings with a fancy painting app
	// save drawings too eeprom
	// manage drawings library by deleting or inserting slots. This is good when making animations. 

	uint32_t cursorBlinker = 0;

	if (init)
	{
		DRAW_ACTIVE_DRAWING_INDEX = 0;
		DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY = 1; // make different than active drawing index to force loading of first drawing.
	}

	// VIEW / MODIFY drawing
	if ((binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA)) &&
		!(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT)))
	{
		//   // modify drawing on display in draw mode.
		//   // if in save to eeprom mode, always only scroll through drawings.

		cursorBlinker = this->modeSingleSegmentManipulation(&displayAllSegments);
	}
	else
	{
		// scroll through drawings
		DRAW_ACTIVE_DRAWING_INDEX += encoder_dial->getDelta();
		
		if (!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
		{ // shift function for saving drawings to eeprom.

			//check for global display change. we're not really changing the drawing, just seeing how it would look negative, and stuf..
			this->displayChangeGlobal(&displayAllSegments, false);

			//scroll through drawings.
			modifyValueUpDownWithMomentary2And3(&DRAW_ACTIVE_DRAWING_INDEX, 1);
		}
	}

	// SAVE / LOAD drawings from memory
#ifdef ENABLE_EEPROM
	checkBoundaries(&DRAW_ACTIVE_DRAWING_INDEX, 0, EEPROM_NUMBER_OF_DRAWINGS - 1, true);

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT))
	{
		// eeprom save mode

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
		{
			// SHIFT button to insert or delete drawing slots from eeprom
			if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
			{
				//insert slot after current index (and move to it)
				if (DRAW_ACTIVE_DRAWING_INDEX >= EEPROM_NUMBER_OF_DRAWINGS - 1)
				{
					// one picture before the last one is the last position where you can still insert a drawing.
				}
				else
				{
					// work with eeprom addresses, not with picture indexes.
					for (int16_t i = EEPROM_PICTURES_START_ADDRESS + (EEPROM_PICTURES_LENGTH - 1);
						 i >= EEPROM_PICTURES_START_ADDRESS + (DRAW_ACTIVE_DRAWING_INDEX-1) * 4;
						 i--)
					{
						// move all pictures one up.
						uint8_t tmp = eeprom_read_byte((uint8_t *)(i));
						eeprom_write_byte((uint8_t *)(i + 4), tmp);
					}
				}
			}

			if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
			{
				// delete slot. (and shift all drawings.)
				for (int16_t i = EEPROM_PICTURES_START_ADDRESS + (DRAW_ACTIVE_DRAWING_INDEX+1) * 4;
					 i < EEPROM_PICTURES_START_ADDRESS + (EEPROM_PICTURES_LENGTH - 3);
					 i++)
				{
					uint8_t tmp = eeprom_read_byte((uint8_t *)(i + 4));
					eeprom_write_byte((uint8_t *)(i), tmp);
				}
				// DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY ++; // hack to make it refersh the drawing.
			}
		}
		else
		{
			if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
			{
				// save active drawing on display to eeprom.
				for (uint8_t i = 0; i < 4; i++)
				{
					eeprom_write_byte(
						(uint8_t *)(EEPROM_PICTURES_START_ADDRESS + DRAW_ACTIVE_DRAWING_INDEX * 4 + i),
						(uint8_t)((displayAllSegments >> (i * 8)) & 0xFF)
						);
				}
			}
		}
	}
	
	if (DRAW_ACTIVE_DRAWING_INDEX != DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY)
	{
		// load drawing from memory only if index changed
		displayAllSegments = 0;
		// for (uint8_t i = 0; i < 4; i++)
		// {
			// displayAllSegments |= (uint32_t)(eeprom_read_byte((uint8_t *)(EEPROM_PICTURES_START_ADDRESS + DRAW_ACTIVE_DRAWING_INDEX * 4 + i))) << (i * 8);
			eepromPictureToDisplayAllSegments(EEPROM_PICTURES_START_ADDRESS, DRAW_ACTIVE_DRAWING_INDEX);
		// }

		this->displayChangeGlobal(&displayAllSegments, true);
	}
#endif

	DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY = DRAW_ACTIVE_DRAWING_INDEX;

	// OUTPUT to display
	// setBlankDisplay();
	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
	{
		// number: always show index of active drawing if activated.
		ledDisp->setNumberToDisplayAsDecimal(DRAW_ACTIVE_DRAWING_INDEX ); // do +1 or not?  in the real world, most of the people start counting from 1. Welcome to an eternal discussion Lucie!
	}
	else
	{
		// drawing: set display
		ledDisp->setBinaryToDisplay(displayAllSegments ^ cursorBlinker);
	}
}

#ifdef ENABLE_MULTITIMER
void Apps::miniMultiTimer(bool init)
{
	// every player: init time, time left, alive?
	// game: pause, player alive? ,fischertimer active?/time, random starter

	if (init)
	{
		this->multiTimer.setBuzzer(this->buzzer);
		this->multiTimer.init();
	}

	// TIMER BUTTONS

	for (uint8_t i = 0; i < MAX_TIMERS_COUNT; i++)
	{
		if (binaryInputs[buttons_indexed[i]].getEdgeUp())
		{
			this->multiTimer.playerButtonPressEdgeUp(i);
		}
		if (binaryInputs[buttons_indexed[i]].getEdgeDown())
		{
			this->multiTimer.playerButtonPressEdgeDown(i);
		}
	}

	// START STOP Button
	if (binaryInputs[BUTTON_LATCHING_BIG_RED].getEdgeUp())
	{
		this->multiTimer.start();
	}
	if (binaryInputs[BUTTON_LATCHING_BIG_RED].getEdgeDown())
	{
		this->multiTimer.init();
	}

	// PAUSE Switch
	this->multiTimer.setStatePause(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA)); // do not only work on edge here, as latching switch can  be in any state.

	// # set number of timers SWITCH
	this->multiTimer.setStateTimersCount(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT)); // do not only work on edge here, as latching switch can  be in any state.

	// set fischer timer SWITCH
	this->multiTimer.setStateFischerTimer(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT)); // do not only work on edge here, as latching switch can  be in any state.

	// THE DIAL
	if (encoder_dial->getDelta())
	{
		// number of timers
		int16_t encoder_mapped = encoder_dial->getValueLimited(90, false);

		this->multiTimer.setTimersCount((uint8_t) ((float)encoder_mapped / 25) + 1); 
		// convert value to predefined amount of seconds.
		uint16_t seconds = this->multiTimer.getIndexedTime(encoder_mapped); // 0 seconds to an hour

		// pass through to multitimer app, it has to decide about validity.
		bool individualTimerSet = false;
		for (uint8_t i = 0; i < MAX_TIMERS_COUNT; i++)
		{
			if (binaryInputs[buttons_indexed[i]].getValue())
			{
				this->multiTimer.setTimerInitCountTimeSecs(i, seconds);
				individualTimerSet = true;
			}
		}

		if (!individualTimerSet)
		{
			this->multiTimer.setAllInitCountDownTimeSecs(seconds);
		}

		this->multiTimer.setFischerTimer(seconds);
	}

	// UPDATE CYCLIC
	this->multiTimer.refresh();

	uint8_t buttonLights;

	uint8_t settingsLights;

	this->multiTimer.getDisplay(textHandle, &buttonLights, &settingsLights);

	uint8_t lights = 0b00000000;
	// timer buttons lights to real lights
	for (uint8_t i = 0; i < 4; i++)
	{
		if (1 << i & buttonLights)
		{
			lights |= 1 << lights_indexed[i];
		}
	}

	// settings light to real lights
	(LIGHT_PAUSE & settingsLights) ? lights |= 1 << LIGHT_LATCHING_EXTRA : false;
	(LIGHT_PLAYING & settingsLights) ? lights |= 1 << LIGHT_LATCHING_BIG : false;
	(LIGHT_FISCHER & settingsLights) ? lights |= 1 << LIGHT_LATCHING_SMALL_RIGHT : false;
	(LIGHT_SET_TIMERS_COUNT & settingsLights) ? lights |= 1 << LIGHT_LATCHING_SMALL_LEFT : false;

	setLedArray(lights);
	setDecimalPoint(LIGHT_SECONDS_BLINKER & settingsLights, 1);
}
#endif

#ifdef ENABLE_TILT_SWITCHES
void Apps::tiltSwitchTest(bool init)
{
	// four tilt switches are positioned as such that they are "ON" in rest position.
	const uint8_t segments_to_fill [] = {70,12,73,45}; // one segment has four sides. we want seg 0 to be on for forward, 1 and 2 for right, 3 for backward, and 4 and 5 for left. Black magic was needed in this limited memory environment.
	if (init)
	{
		setStandardTextToTextBuf(TEXT_TILT);

		TILT_EXPECTED_SWITCH_INDEX = 0;
		TILT_CYCLE_COUNTER = 0;
		displayAllSegments = 0;
	}

	// time the amount of cycles.

	if (binaryInputs[mercury_switches_indexed[TILT_EXPECTED_SWITCH_INDEX]].getEdgeUp()){

		displayAllSegments ^= 1<< (segments_to_fill[TILT_EXPECTED_SWITCH_INDEX]/10);
		displayAllSegments ^= 1<< (segments_to_fill[TILT_EXPECTED_SWITCH_INDEX]%10);
		displayAllSegments &= ~(1UL<<7); 
		
		// if (binaryInputsValue & (1<<BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT)){
		// 	TILT_EXPECTED_SWITCH_INDEX+=4;

		// }else 
		if (binaryInputsValue & (1<<BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT)){
			TILT_EXPECTED_SWITCH_INDEX+=2;

		}else{
			TILT_EXPECTED_SWITCH_INDEX++;
		}
		
		addNoteToBuzzer(1); //not beep but "puck"
		if (TILT_EXPECTED_SWITCH_INDEX >= 4){
			TILT_CYCLE_COUNTER ++;
			TILT_EXPECTED_SWITCH_INDEX = 0;
			addNoteToBuzzer(rest_2); // would you believe it, it sounds like a chicken. this is the kedeeeeet  in the tok tok tok kedeeet (the ke is also a tok sound)
			addNoteToBuzzer(C7_1); 
		}
	}

	if (binaryInputsEdgeUp & (1<<BUTTON_INDEXED_MOMENTARY_0)){
		TILT_CYCLE_COUNTER = 0;
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_LATCHING_EXTRA)){
		// start timer
		TILT_TIMER.start();
		TILT_CYCLE_COUNTER = 0;
	}

	// display 
	if (binaryInputsValue & (1<<BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT)){
		setDecimalPoint(TILT_TIMER.getSecondsBlinker(),1);
		TILT_TIMER.getTimeString(textBuf);
		textBufToDisplay();
		
	}else{
		ledDisp->setNumberToDisplayAsDecimal(TILT_CYCLE_COUNTER);
		ledDisp->setBinaryToDisplay(displayAllSegments);
	}

	// normal vs timed mode.
	if (binaryInputsValue & 1 << BUTTON_LATCHING_EXTRA){
		
		if (!TILT_TIMER.getTimeIsNegative()){
			loadBuzzerTrack(SONG_DRYER_UNHAPPY);
			TILT_TIMER.reset();
		}
	}else{

#ifdef ENABLE_MULTITIMER
		uint16_t tmpSeconds = this->multiTimer.getIndexedTime(encoder_dial->getValueLimited(90,false));
#else
		uint16_t tmpSeconds = encoder_dial->getValueLimited(60,false) * 30;
#endif
		TILT_TIMER.setInitCountDownTimeSecs(tmpSeconds);
		TILT_TIMER.reset();
	}
}
#endif

void Apps::modeGeiger(bool init)
{

	if (init)
	{
		//textBuf[3]=' ';
		COUNTER_GEIGER = 0;
		GEIGER_TONE_FREQUENY_LOWEST = 2000;
		GEIGER_TONE_FREQUENCY_HEIGHEST = 4000;
		GEIGER_TONE_LENGTH = 10;
		GEIGER_PROBABILITY_THRESHOLD = 950000;
		GEIGER_INCREASE_CHANCE = 0;
	}

	//play tick.
	//wait random time.
	//X = - log(1 - Y)/ K   with Y a random value ( 0<Y<1) and K a constant ?
	long r = random(0, 1024) * random(0, 1024);
	//long r = random(0, 1024);
	//r = r*r;
	// setBlankDisplay();

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT))
	{
		// note mode
		int8_t delta = encoder_dial->getDelta();

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
		{
			//lower
			GEIGER_TONE_FREQUENY_LOWEST +=  delta * 10;
			checkBoundaries(&GEIGER_TONE_FREQUENY_LOWEST, 0, 5000, false);
			ledDisp->setNumberToDisplayAsDecimal(GEIGER_TONE_FREQUENY_LOWEST);
		}
		else if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
		{
			//upper
			GEIGER_TONE_FREQUENCY_HEIGHEST += delta * 10;
			checkBoundaries(&GEIGER_TONE_FREQUENCY_HEIGHEST, 0, 5000, false);
			ledDisp->setNumberToDisplayAsDecimal(GEIGER_TONE_FREQUENCY_HEIGHEST);
		}
		else if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)))
		{
			//length
			GEIGER_TONE_LENGTH += delta;
			checkBoundaries(&GEIGER_TONE_LENGTH, 1, 255, false);
			ledDisp->setNumberToDisplayAsDecimal(GEIGER_TONE_LENGTH);
			
		}
		else if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		{
			if (delta)
			{
				this->geigerToneHelper();
			}
			
		}else{
			GEIGER_PROBABILITY_THRESHOLD -= encoder_dial->getDelta() * 10 *1024;
		}

		if (r > GEIGER_PROBABILITY_THRESHOLD)
		{ 
			// 1024*1024
			this->geigerToneHelper();
		}
	}
	else
	{
		// simple Geiger mode

		// If you press the button and approach an object, the object appears super radio active! hi-la-ri-ous!
		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		{
			GEIGER_INCREASE_CHANCE += 1;
		}
		else
		{
			if (GEIGER_INCREASE_CHANCE > 0)
			{
				GEIGER_INCREASE_CHANCE -= 1;
			}
		}
		r += (long)(GEIGER_INCREASE_CHANCE)*1000;

		GEIGER_PROBABILITY_THRESHOLD -= encoder_dial->getDelta() * 10 *1024;
		if (r > GEIGER_PROBABILITY_THRESHOLD)
		{
			buzzer->playTone((unsigned int)50, 10);
			setStandardTextToTextBuf(TEXT_RANDOM_SEGMENTS);
			textBufToDisplay();
			COUNTER_GEIGER++;
		}
	}

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
	{
		ledDisp->setNumberToDisplayAsDecimal(COUNTER_GEIGER);
	}
}

void Apps::geigerToneHelper(){
	unsigned int random_frequency_within_limits = random(GEIGER_TONE_FREQUENY_LOWEST, GEIGER_TONE_FREQUENCY_HEIGHEST + 1);
	
	buzzer->playTone(
		random_frequency_within_limits,
		(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA)) ? 0 : GEIGER_TONE_LENGTH);

	ledDisp->setNumberToDisplayAsDecimal(random_frequency_within_limits);
	COUNTER_GEIGER++;
}

void Apps::modeSequencer(bool init)
{
	int8_t step = 0;

	bool showNote = false;

	if (init)
	{
		SEQUENCER_STEP_COUNTER = 0;
		SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET = 0;

		SEQUENCER_SPEED.setInitTimeMillis(-1000);
		SEQUENCER_SPEED.start();

		SEQUENCER_EEPROM_MODE_BLINK.setInitTimeMillis(-1000);
		SEQUENCER_EEPROM_MODE_BLINK.start();

		// //resets song.
		// for (uint8_t i = 0; i < 32; i++)
		// {
		// 	this->SEQUENCER_SONG[i] = C7_8;
		// }
	}

	// erase screen at start.
	// setBlankDisplay();

	if (this->binaryInputsEdgeDown & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT))
	{
		init = true; // make sure we display the sequencer when returning from save/load mode
	}

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT))
	{
#ifdef ENABLE_EEPROM
		this->saveLoadMenu(this->SEQUENCER_SONG, 9, EEPROM_SEQUENCER_SONG_LENGTH, EEPROM_SEQUENCER_SONGS_START_ADDRESS);
#endif
	}
	else
	{
		// manipulate the sequencer

		// visualize programmed note
		SEQUENCER_TEMP_NOTE = SEQUENCER_SONG[SEQUENCER_STEP_COUNTER];

		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
		{
			addNoteToBuzzer(SEQUENCER_TEMP_NOTE);
		}

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
		{
			// if button continuously pressed, show notes.

			noteToDisplay( SEQUENCER_TEMP_NOTE);
			showNote = true;

			// bonus effect: TRANSPOSE!
			SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET += encoder_dial->getDelta();
		}

		// just listen to the potentio note
		SEQUENCER_TEMP_NOTE = encoder_dial->getValueLimited(255,true);

		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
		{
			buzzerOffAndAddNote(SEQUENCER_TEMP_NOTE);
		}

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
		{
			// if button continuously pressed, listen to notes as they are chosen
			buzzerOffAndAddNoteAtEncoderDialChange(SEQUENCER_TEMP_NOTE);
			noteToDisplay( SEQUENCER_TEMP_NOTE);
			showNote = true;
		}

		// program note to song
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
		{
			buzzerOffAndAddNote(SEQUENCER_TEMP_NOTE);

			if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
			{
				//copy to all measures
				for (uint8_t i = 0; i < 4; i++)
				{
					this->SEQUENCER_SONG[(SEQUENCER_STEP_COUNTER % 8) + 8 * i] = SEQUENCER_TEMP_NOTE;
				}
			}
			else
			{
				this->SEQUENCER_SONG[SEQUENCER_STEP_COUNTER] = SEQUENCER_TEMP_NOTE;
			}
		}

		// song progression
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
		{
			step = 1;
		}

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		{
			step += encoder_dial->getDelta();
		}

		if (this->binaryInputsEdgeDown & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
		{
			// reset transpose when stop autoplay.
			SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET = 0;
		}

		// autoplay
		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
		{
			byte momentary_buttons_mask = 1 << BUTTON_INDEXED_MOMENTARY_0 | 1 << BUTTON_INDEXED_MOMENTARY_1 | 1 << BUTTON_INDEXED_MOMENTARY_2 | 1 << BUTTON_INDEXED_MOMENTARY_3;
			if ((binaryInputsValue & momentary_buttons_mask) == 0) // no button pressed

			// change speed is default behaviour of dial
			{
				dialOnEdgeChangeInitTimerPercentage(&SEQUENCER_SPEED);
			}

			if (!SEQUENCER_SPEED.getTimeIsNegative())
			{
				step = 1;
				SEQUENCER_SPEED.start();
			}
		}

		// handle step change
		if (step != 0 || init)
		{
			nextStepRotate(&SEQUENCER_STEP_COUNTER,true,0,32);

			addNoteToBuzzer(
				this->SEQUENCER_SONG[SEQUENCER_STEP_COUNTER] +
				SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET * ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)) > 0));

			// sequencer shows every step in 32 notes bar. 8steps (circle) times 4 measures (bar on bottom)
			displayAllSegments = 0;
			if (SEQUENCER_STEP_COUNTER % 8 < 4)
			{
				displayAllSegments |= (uint32_t)0x1 << (8 * (SEQUENCER_STEP_COUNTER % 8));
			}
			else
			{
				displayAllSegments |= (uint32_t)0x1 << ((8 * (3 - (SEQUENCER_STEP_COUNTER % 8 - 4))) + 6);
			}

			displayAllSegments |= (uint32_t)0x1 << ((8 * (SEQUENCER_STEP_COUNTER / 8)) + 3); // bar at bottom.
		}

		if (!showNote)
		{
			displayAllSegmentsToScreen();
		}
	}
}

void Apps::modeMetronome(bool init)
{
	// todo: LFO:  with extra timer, create slight timing offset in second ticker, for fun effects (zwevingen) --> I think nonkel Lode was too optimistic about the available memory at the beginning. We went for a metronome here, not for trippy hippy
	bool update = false;

	if (init)
	{
		TIMER_METRONOME.setInitTimeMillis(-166);
		TIMER_METRONOME.start();
		METRONOME_TICKER_1_POSITION = 0;
		METRONOME_TICKER_2_POSITION = 0;
		METRONOME_TICKER_3_POSITION = 0;
	}

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
	{
		dialOnEdgeChangeInitTimerPercentage(&TIMER_METRONOME);
		// auto counting (metronome)
		if (!TIMER_METRONOME.getTimeIsNegative())
		{
			TIMER_METRONOME.start();
			update = true;
		}

	}else if (binaryInputsValue & (1<< BUTTON_INDEXED_MOMENTARY_3)){
		// manual mode
		update = encoder_dial->getDelta() != 0;

	}else{
		buzzerChangeSpeedRatioWithEncoderDial();
		buzzerOffAndAddNoteAtEncoderDialChange(C6_4);
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
	{
		// ticker 1,2 and 3 back together (at position of ticker 1)
		METRONOME_TICKER_2_POSITION = METRONOME_TICKER_1_POSITION;
		METRONOME_TICKER_3_POSITION = METRONOME_TICKER_1_POSITION;
	}

	displayAllSegments = 0;
	bool forceNextStep = update || binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3);

	modeMetronomeTickerUpdate(&METRONOME_TICKER_3_POSITION, BUTTON_INDEXED_MOMENTARY_1, !(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT)), C6_4, forceNextStep);
	modeMetronomeTickerUpdate(&METRONOME_TICKER_2_POSITION, BUTTON_INDEXED_MOMENTARY_2, true, C5_4, forceNextStep);
	modeMetronomeTickerUpdate(&METRONOME_TICKER_1_POSITION, BUTTON_INDEXED_MOMENTARY_3, true, C7_8, update);
	
	// display
	if (binaryInputsValue & (1<< BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT)){
		// bpm --> full 12 step circles per minute.   timing is per step. so: 60bpm == 1 circle / second = timer: 1000/12 = 83.333ms/step
		ledDisp->setNumberToDisplayAsDecimal(  (int16_t) ( 1 / (12* (float)TIMER_METRONOME.getInitTimeMillis()/60000 ))); // millis/step to fullcirclesp/minute (bpm)

	}else{
		displayAllSegmentsToScreen();
	}
}

void Apps::modeMetronomeTickerUpdate(int16_t *ticker_counter, uint8_t momentary_id, bool direction, uint8_t sound_at_zero_pass, boolean force_step)
{
	// every ticker gets updated.

	// check for next step
	if (binaryInputsEdgeUp & 1<<momentary_id || force_step)
	{
		this->nextStepRotate(
			ticker_counter,
			direction,
			0,
			11);

		if (*ticker_counter == 0)
		{
			addNoteToBuzzer(sound_at_zero_pass);
		}
	}
	flashPictureToDisplayAllSegments( disp_4digits_animations + ANIMATE_CIRCLE_OFFSET + *ticker_counter * 4);
}

#ifdef ENABLE_SIMON_APP
void Apps::modeSimon(bool init)
{
	uint8_t lights = 0b00000000;

	if (init)
	{
		SIMON_BLINK_TIMER.setInitTimeMillis(-250);
		SIMON_STEP_TIMER.setInitTimeMillis(-500);

		SIMON_ACTIVE_LIGHT = SIMON_NO_ACTIVE_LIGHT;
		SIMON_PLAYERS_COUNT = 1;
		// SIMON_LEVEL = 1; // levels don't look that appealing. I choose one speed and you'll have to stick with it.
	}

	if (!(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA)) )
	{
		// at any time, if play button off: no more playing!
		simonState = simonWaitForNewGame;
	}

	//   if (SIMON_PLAYERS_COUNT > 1){
	// 	  // if more than one player, light always on.
	// 	  lights |= 1 << LIGHT_LATCHING_SMALL_RIGHT;
	//   }
	if (SIMON_RANDOM_PLAYER_SEQUENCE)
	{
		lights |= 1 << LIGHT_LATCHING_SMALL_RIGHT;
	}

	if (SIMON_CUSTOM_BUILD_UP)
	{
		// if custom sequence, light always on.
		lights |= 1 << LIGHT_LATCHING_SMALL_LEFT;
	}

	// check if a momentary button was pressed, and create byte with status: 0000 is no button pressed.  0001, 0010, 0100, 1000
	uint8_t pressed_momentary_button = SIMON_NO_BUTTON_PRESSED;
	for (int k = 0; k < MOMENTARY_BUTTONS_COUNT; ++k)
	{
		if (binaryInputsEdgeUp & 1<<k)
		{
			pressed_momentary_button = k;
		}
	}

	switch (simonState)
	{
	case simonWaitForNewGame:
	{
		setStandardTextToTextBuf(TEXT_SPACES);
		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
		{
			simonState = simonNewGame;
		}

		// play button light blinking invitingly.
		if (millis() % 250 > 125)
		{
			lights |= 1 << LIGHT_LATCHING_EXTRA;
		}

		// number of players.
		SIMON_PLAYERS_COUNT= (encoder_dial->getValueLimited((SIMON_MAX_PLAYERS - 1) * 4, false)/4 + 1); // start counting from player 0 to display

		numberToBufAsDecimal(SIMON_PLAYERS_COUNT);
		//textBuf[0] = ' ';
		textBuf[1] = 'P';
		//textBuf[2] = ' ';

		// Instead of computer, user choses the next light in simon sequence.
		SIMON_CUSTOM_BUILD_UP = binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT);

		// random player sequence during game if multiplayer
		SIMON_RANDOM_PLAYER_SEQUENCE = binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT);

		break;
	}

	case simonNewGame:
	{
		loadBuzzerTrack(SONG_DRYER_HAPPY);
		SIMON_END_OF_GAME = false;
		SIMON_PLAYERS_ALIVE_COUNT = SIMON_PLAYERS_COUNT;

		// set all players in array. don't bother about real player count.
		for (uint8_t i = 0; i < SIMON_MAX_PLAYERS; i++)
		{
			SIMON_PLAYERS[i] = i;
		}

		// generate new light sequence
		for (int k = 0; k < bytes_list_bufsize; ++k)
		{
			SIMON_LIST[k] = k % MOMENTARY_BUTTONS_COUNT;
		}
		shuffle(SIMON_LIST, bytes_list_bufsize);

		// set starting parameters
		SIMON_LENGTH = 0;

		simonState = simonNewLevel;
		break;
	}

	case simonNewLevel:
	{
		++SIMON_LENGTH;

		// shuffle alive players.
		if (SIMON_RANDOM_PLAYER_SEQUENCE)
		{
			shuffle(SIMON_PLAYERS, SIMON_PLAYERS_ALIVE_COUNT);
		}

		// set first player
		SIMON_PLAYER_PLAYING_INDEX = 0; // this is just the index.

		numberToBufAsDecimal(SIMON_LENGTH);
		//intToDigitsString(textBuf+1, SIMON_LENGTH, false);

		// let 'maximum length breach' be a happy crash. I can't afford the bytes!
		//   if (SIMON_LENGTH >= bytes_list_bufsize) {
		// 	  // reached maximum length
		// 	  loadBuzzerTrack(SONG_DRYER_HAPPY);
		// 	  simonState = simonWaitForNewGame;
		// 	  break;
		//   }

		if (SIMON_CUSTOM_BUILD_UP)
		{
			// for custom sequence games, there is nothing to be replayed.
			simonState = simonStartUserRepeats;
		}
		else
		{
			simonState = simonStartPlaySequence;
		}
		break;
	}

	case simonStartPlaySequence:
	{
		SIMON_INDEX = -1; // negative index allows for lead-in time
		SIMON_STEP_TIMER.start();
		simonState = simonPlaySequence;
	}

	case simonPlaySequence:
	{
		if (SIMON_END_OF_GAME)
		{

			if (millis() % 1000 > 650)
			{
				numberToBufAsDecimal(SIMON_LENGTH);
				textBuf[0] = SIMON_PLAYERS[SIMON_PLAYER_PLAYING_INDEX] + 49;
				textBuf[1] = 'P';
			}
			else
			{
				setStandardTextToTextBuf(TEXT_END);
			}
		}
		else
		{
			textBuf[1] = 'S';
		}

		if (SIMON_ACTIVE_LIGHT != SIMON_NO_ACTIVE_LIGHT)
		{
			lights |= 1 << lights_indexed[SIMON_ACTIVE_LIGHT];
			if (!SIMON_BLINK_TIMER.getTimeIsNegative())
			{
				SIMON_ACTIVE_LIGHT = SIMON_NO_ACTIVE_LIGHT;
			}
		}

		if (SIMON_INDEX >= SIMON_LENGTH && !SIMON_BLINK_TIMER.getTimeIsNegative())
		{
			// after last step display, immediatly continue
			if (SIMON_END_OF_GAME)
			{

				simonState = simonWaitForNewGame;
			}
			else
			{
				// time for the user to prove himself.
				simonState = simonStartUserRepeats;
			}
			break;
		}

		if (SIMON_STEP_TIMER.getTimeIsNegative())
		{
			// still in step.
			break;
		}

		SIMON_STEP_TIMER.start();
		if (SIMON_INDEX < 0)
		{
			++SIMON_INDEX; // do-nothing lead in time
			break;
		}

		// show one light from the sequence
		SIMON_ACTIVE_LIGHT = SIMON_LIST[SIMON_INDEX];

		SIMON_BLINK_TIMER.start();

		addNoteToBuzzer(A3_8);
		++SIMON_INDEX;
		break;
	}

	case simonStartUserRepeats:
	{
		SIMON_INDEX = 0;
		simonState = simonUserRepeats;
	}
	break;

	case simonUserRepeats:
	{

		for (int k = 0; k < MOMENTARY_BUTTONS_COUNT; ++k)
		{

			lights |= binaryInputs[buttons_indexed[k]].getValue() << lights_indexed[k];
		}

		textBuf[0] = SIMON_PLAYERS[SIMON_PLAYER_PLAYING_INDEX] + 49;
		textBuf[1] = 'P';
		if (pressed_momentary_button == SIMON_NO_BUTTON_PRESSED)
		{
			break;
		}

		if (SIMON_CUSTOM_BUILD_UP && (SIMON_INDEX + 1) == SIMON_LENGTH)
		{
			// last light of sequence is not yet defined.
			// user can choose any button.
			SIMON_LIST[SIMON_INDEX] = pressed_momentary_button;
		}

		const int expected = SIMON_LIST[SIMON_INDEX];

		if (pressed_momentary_button != expected)
		{
			// player made mistake, player dies
			this->addNoteToBuzzerRepeated(C4_1, 4);
			simonState = simonPlayerDead;
			break;
		}
		// player pressed correct button
		addNoteToBuzzer(A3_8);

		++SIMON_INDEX;

		if (SIMON_INDEX >= SIMON_LENGTH)
		{
			// sequence done!
			addNoteToBuzzer(E5_4);
			addNoteToBuzzer(rest_1);
			addNoteToBuzzer(B6_1);
			simonState = simonNextPlayer;
			break;
		}
		break;
	}

	case simonNextPlayer:
	{
		// check next alive player (assume there is always a player alive.)

		SIMON_PLAYER_PLAYING_INDEX++;
		if (SIMON_PLAYER_PLAYING_INDEX >= SIMON_PLAYERS_ALIVE_COUNT)
		{
			// completely new level
			simonState = simonNewLevel;
		}
		else
		{
			// repeat existing level for next player
			simonState = simonStartUserRepeats;
		}
	}
	break;

	case simonPlayerDead:
	{
		SIMON_PLAYERS_ALIVE_COUNT--;
		if (SIMON_PLAYERS_ALIVE_COUNT == 0)
		{
			//everybody dead
			simonState = simonStartPlaySequence;
			SIMON_END_OF_GAME = true;
			break;
		}

		if (SIMON_PLAYER_PLAYING_INDEX >= SIMON_PLAYERS_ALIVE_COUNT)
		{
			// if last player in the sequence was playing. not much needs to be done.
			simonState = simonNewLevel;
			break;
		}

		// replace the current (dead) player by the last player in the list that needs to play.
		// remember: we already decrease alive_count by 1
		// no need to go to next player, as the new current player didnt play yet
		SIMON_PLAYERS[SIMON_PLAYER_PLAYING_INDEX] = SIMON_PLAYERS[SIMON_PLAYERS_ALIVE_COUNT];
		simonState = simonStartUserRepeats;
	}
	break;
	}

	setLedArray(lights);
	textBufToDisplay();
}
#endif  //ENABLE_SIMON_APP

void Apps::modeReactionGame(bool init)
{
#ifdef ENABLE_REACTION_APP
	//yellow button active at start: yellow button is also a guess button
	// big red active: timed game
	// small red right active: time progressively shorter as game advances
	// small red left active: play by sound.

	if (init)
	{
		reactionGameState = reactionWaitForStart;
		displayAllSegments = 0x0;
		TIMER_REACTION_GAME_RESTART_DELAY.setInitTimeMillis(0);  // 1 better than 0?  blinking does not look nice otherwise.
		TIMER_REACTION_GAME_RESTART_DELAY.start();

		// //play by sound, only initiate pattern at start of app. They way, players can get used to it. To change pattern, leave and come back to app.
		// never twice the same sound. Only first notes of array will be used.
		for (uint8_t i = 0; i < 12; i++)
		{
			REACTION_GAME_TEMP_SELECTED_NOTES[i] =  234 + i ;//234, 245
		}
		this->shuffle(REACTION_GAME_TEMP_SELECTED_NOTES, 12);
		
	}

	// setBlankDisplay();

	// at any time, leave game when depressing play button.
	if (this->binaryInputsEdgeDown & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
	{
		reactionGameState = reactionWaitForStart;
	}

	switch (reactionGameState)
	{

	case reactionWaitForStart:
	{
		// change level
		REACTION_GAME_LEVEL = (encoder_dial->getValueLimited(64,false) / 16); // only set the default inittime at selecting the game. If multiple games are played, init time stays the same.
		if (encoder_dial->getDelta())
		{
			TIMER_REACTION_GAME_RESTART_DELAY.start();
		}

		// check options
		REACTION_GUITAR_HERO_MODE = (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_BIG_RED)) > 0;
		OPTION_REACTION_COUNTDOWN_MODE_HERO_ADD_PAUSE_MODE = (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT)) > 0;
		EXTRA_OPTION_REACTION_SOUND_MODE_GUITAR_HEX_HERO = (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT)) > 0;

		// display level and high score
#ifdef ENABLE_EEPROM
		if (TIMER_REACTION_GAME_RESTART_DELAY.getInFirstGivenHundredsPartOfSecond(500))
		{
			intToDigitsString(textBuf, REACTION_GAME_LEVEL + 1, false); // utilities lode
			textBuf[0] = 'L';
			textBufToDisplay();
		}
		else
		{
			ledDisp->setNumberToDisplayAsDecimal(
				eeprom_read_word(
					(uint16_t *)EEPROM_REACTION_GAME_OFFSET + 
								REACTION_GUITAR_HERO_MODE * 48 +
								EXTRA_OPTION_REACTION_SOUND_MODE_GUITAR_HEX_HERO * 24 +
								OPTION_REACTION_COUNTDOWN_MODE_HERO_ADD_PAUSE_MODE * 12 +
								REACTION_GAME_LEVEL
								 ));

		}
#else
		intToDigitsString(textBuf, REACTION_GAME_LEVEL + 1, false); // utilities lode
		textBuf[0] = 'L';
		textBufToDisplay();
#endif

		// play game button pressed
		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
		{
			if (!REACTION_GUITAR_HERO_MODE && EXTRA_OPTION_REACTION_SOUND_MODE_GUITAR_HEX_HERO)
			{
				// sound mode let them all play so the player gets a feel for them.
				for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
				{
					addNoteToBuzzer(REACTION_GAME_TEMP_SELECTED_NOTES[i]);
					this->addNoteToBuzzerRepeated(rest_1, 2);
				}
				addNoteToBuzzerRepeated(rest_1, 4);
			}
			reactionGameState = reactionNewGame;
		}
		break;
	}

	case reactionNewGame:
	{
		REACTION_GAME_SCORE = 0;

		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_BIG_RED))
		{
			REACTION_GAME_STEP_TIME_MILLIS = (5 - REACTION_GAME_LEVEL) * -200;
			displayAllSegments = 0;

			if (EXTRA_OPTION_REACTION_SOUND_MODE_GUITAR_HEX_HERO)
			{
				// hex geek mode
				for (uint8_t i = 0;i<8;i++){
					REACTION_GAME_HEX_MEMORY[i] = 0;
				}
				setStandardTextToTextBuf(TEXT_SPACES);
				reactionGameState = reactionHexNextStep;
			}
			else
			{
				// guitar hero mode
				reactionGameState = reactionGuitarHeroNewTurn;
			}
		}
		else
		{

			if (buzzer->getBuzzerRollEmpty()){ // in sound mode, wait till demo is done
				reactionGameState = reactionNewTurn;

				if (OPTION_REACTION_COUNTDOWN_MODE_HERO_ADD_PAUSE_MODE)
				{
					// if enabled, we go for "as many points in a limited time. --> this to make it more exciting for adults (can be boring after a while if you just have to press the right button in time)
					REACTION_GAME_STEP_TIME_MILLIS = (1UL << (REACTION_GAME_LEVEL + 1)) * -4000; // step speed depending on level
				}
				else
				{
					REACTION_GAME_STEP_TIME_MILLIS = (1UL << (5 - REACTION_GAME_LEVEL)) * -35; // step speed depending on level
				}
			}
		}

		TIMER_REACTION_GAME_SPEED.setInitTimeMillis(REACTION_GAME_STEP_TIME_MILLIS);
		TIMER_REACTION_GAME_SPEED.start();
		break;
	}

	case reactionHexNextStep:
	{
		// shift textBuf to right
		for (uint8_t i = 3; i > 0; i--)
		{
			textBuf[i] = textBuf[i - 1];
			REACTION_GAME_HEX_MEMORY[i] = REACTION_GAME_HEX_MEMORY[i - 1];
		}

		// add new hex char on left
		textBuf[0] = random(49, 64);
		REACTION_GAME_HEX_MEMORY[0] = textBuf[0] - 48;

		if (textBuf[0] > 57){
			textBuf[0] += 7;
		}

		// prepare next move
		reactionGameState = reactionHexPlaying;
		TIMER_REACTION_GAME_SPEED.start();
		REACTION_HEX_GUESSED_CORRECTLY = false;
		break;
	}

	case reactionHexWaitForButtonsRelease:
	{
		// all buttons need to be release before we can check for a new press.
		// if (!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)) &&
		// 	!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)) &&
		// 	!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)) &&
		// 	!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		// {
		byte momentary_buttons_mask = 1 << BUTTON_INDEXED_MOMENTARY_0 | 1 << BUTTON_INDEXED_MOMENTARY_1 | 1 << BUTTON_INDEXED_MOMENTARY_2 | 1 << BUTTON_INDEXED_MOMENTARY_3;
		if ((binaryInputsValue & momentary_buttons_mask) == 0) // no buttons pressed
		{
			reactionGameState = reactionHexPlaying;
		}


		textBufToDisplay();
		break;
	}

	case reactionHexPlaying:
	{
		REACTION_GAME_HEX_ACTIVE_DIGIT = 3;
		while(
			REACTION_GAME_HEX_ACTIVE_DIGIT > 0 
			&& 
			(textBuf[REACTION_GAME_HEX_ACTIVE_DIGIT] == ' '
				|| textBuf[REACTION_GAME_HEX_ACTIVE_DIGIT] == SPACE_FAKE_ASCII)
			){
			REACTION_GAME_HEX_ACTIVE_DIGIT--;
		}
		
		// //attempt to optimization, but with a bug, and too tired. So, give it a shot! 
		// REACTION_GAME_HEX_VALUE_TO_FIND = (byte)textBuf[REACTION_GAME_HEX_ACTIVE_DIGIT];

		// if (REACTION_GAME_HEX_VALUE_TO_FIND == ' ' || REACTION_GAME_HEX_VALUE_TO_FIND == SPACE_FAKE_ASCII){
		// 	REACTION_GAME_HEX_VALUE_TO_FIND = 0;
			
		// }else{
		// 	if (REACTION_GAME_HEX_VALUE_TO_FIND > 57){
		// 		REACTION_GAME_HEX_VALUE_TO_FIND -=7;
		// 	}
		// 	REACTION_GAME_HEX_VALUE_TO_FIND -= 48;
		// }
		
		// #else

		REACTION_GAME_HEX_VALUE_TO_FIND = REACTION_GAME_HEX_MEMORY[REACTION_GAME_HEX_ACTIVE_DIGIT];
		
		// check for all buttons pressed in binary pattern or wrong button press
		uint8_t build_up_value = 0;
		for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
		{
			if (binaryInputs[buttons_indexed[i]].getValue())
			{
				if  (! ( REACTION_GAME_HEX_VALUE_TO_FIND & (1<<(3-i)) )){
					reactionGameState = reactionJustDied;
				}else{
					build_up_value |= (1<<(3-i));
				}
			}
		}

		if (! (reactionGameState == reactionJustDied)){

			// check of button press pattern is the sought pattern
			if ( build_up_value == (0x0F & REACTION_GAME_HEX_VALUE_TO_FIND )){
				reactionGameState = reactionHexWaitForButtonsRelease;
				REACTION_HEX_GUESSED_CORRECTLY = true;
				textBuf[REACTION_GAME_HEX_ACTIVE_DIGIT] = ' ';
				REACTION_GAME_SCORE++;
			}

			// end of move 
			if (!TIMER_REACTION_GAME_SPEED.getTimeIsNegative())
			{
				// check if correct combination was pressed at end of move
				reactionGameState = reactionHexNextStep;
				if (!REACTION_HEX_GUESSED_CORRECTLY && REACTION_GAME_HEX_ACTIVE_DIGIT==3){
					reactionGameState = reactionJustDied;
				}
			}
		}

		// update
		textBufToDisplay();
		break;
	}

	case reactionGuitarHeroNewTurn:
	{
		// like in guitar hero, the horizontal segments in the screen fall down and button combo's have to be pressed to "catch" the falling segments.

		// three rows of four horizontal segments in 4 digits 7 segment display.
		// choose top row random. any combination of 4 (including zero) ==> 16 combinations.


		if (random(0,10) || !OPTION_REACTION_COUNTDOWN_MODE_HERO_ADD_PAUSE_MODE){
			
			uint8_t new_segment;
			uint32_t tmp_segments;
			tmp_segments = 0;
			addNoteToBuzzer(1);

			// treat every segment separatly
			for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
			{
				new_segment = 0;
				// get relevant segment

				//mid seg to bottom seg
				new_segment |= ((displayAllSegments >> (8 * i)) & 0b01000000) >> 3; // G segment, move to D segment  0G00D000

				//mid seg to DP seg
				new_segment |= ((displayAllSegments >> (8 * i)) & 0b01000000) << 1; // G segment, move to DP segment  DP.G.0.0.0.0.0.0

				//top seg to mid seg
				new_segment |= ((displayAllSegments >> (8 * i)) & 0b00000001) << 6; // A segment, ON or OFF?, move to G segment

				// random top
				// if (random(0, 2)){  // 0 or 1  // move to segment A)
				//   new_segment |= 1;
				// }else{
				//   new_segment &= ~(0b00000001);
				// }
				// new_segment |= random(0, 2);

				if (random(0, 100) > 50)
				{
					new_segment |= 0b00000001;
				}

				tmp_segments |= ((uint32_t)new_segment) << (8 * i);
			}

			displayAllSegments = tmp_segments;
		}
		
		TIMER_REACTION_GAME_SPEED.start();
		reactionGameState = reactionGuitarHeroPlaying;
		break;
	}

	case reactionGuitarHeroPlaying:
	{

		for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
		{

			if (binaryInputs[buttons_indexed[i]].getEdgeUp())
			{
				//if DP of this button was on, switch it off. else, die!
				//check if DP of digit "i" is set

				if (displayAllSegments & (0x80UL << 8 * i))
				{
					// DP is on, set to zero.
					// buzzer->addRandomSoundToRoll(190, 198);
					addNoteToBuzzer(103 + i);
					displayAllSegments &= ~(0x80UL << 8 * i);
					REACTION_GAME_SCORE++;
				}
				else
				{
					//DP is off --> button should not have been pressed --> die!
					reactionGameState = reactionJustDied;
				}
			}
		}

		if (!TIMER_REACTION_GAME_SPEED.getTimeIsNegative())
		{
			//   // if not all needed buttons pressed, player is dead.
			//   // check dps!

			if ((displayAllSegments & 0x80808080UL) != 0)
			{
				// no success... (not all DP's cleared.)
				reactionGameState = reactionJustDied;
			}
			else
			{
				
				// success!
				reactionGameState = reactionGuitarHeroNewTurn;
				//REACTION_GAME_SCORE++; // let's not update the score here, because the first two rows also "count" which is silly, let's go for "point per correct button press."
			}
		}
		displayAllSegmentsToScreen();
		break;
	}

	case reactionWaitBeforeNewTurn:
	{
		if (buzzer->getBuzzerRollEmpty()){
			reactionGameState = reactionNewTurn;
		}
		break;
	}

	case reactionNewTurn:
	{
		if (!OPTION_REACTION_COUNTDOWN_MODE_HERO_ADD_PAUSE_MODE)
		{
			REACTION_GAME_TIMER_STEP = 0;	   //reset animation step
			TIMER_REACTION_GAME_SPEED.start(); // only restart if 12 steps time per turn
		}
		displayAllSegments = 0; //reset animation graphics screen

		REACTION_GAME_TARGET = random(0, MOMENTARY_BUTTONS_COUNT);

		if (EXTRA_OPTION_REACTION_SOUND_MODE_GUITAR_HEX_HERO)
		{
			addNoteToBuzzer(REACTION_GAME_TEMP_SELECTED_NOTES[REACTION_GAME_TARGET]);
		}

		reactionGameState = reactionPlaying;
		break;
	}

	case reactionPlaying:
	{
		if (OPTION_REACTION_COUNTDOWN_MODE_HERO_ADD_PAUSE_MODE)
		{
			// if timer out, always dead.
			if (!TIMER_REACTION_GAME_SPEED.getTimeIsNegative())
			{
				reactionGameState = reactionJustDied;
			}
			// show number of segments:
			REACTION_GAME_TIMER_STEP = 10 - (uint16_t)((float)TIMER_REACTION_GAME_SPEED.getTimeMillis() / TIMER_REACTION_GAME_SPEED.getInitTimeMillis() * 11);
		}
		else
		{
			// animation next step

			if (!TIMER_REACTION_GAME_SPEED.getTimeIsNegative())
			{
				// game timing animation update.

				this->nextStepRotate(&REACTION_GAME_TIMER_STEP, true, 0, 12);

				// check game status 'dead'
				if (REACTION_GAME_TIMER_STEP == 12)
				{
					REACTION_GAME_TIMER_STEP = 0;
					displayAllSegments = 0;

					reactionGameState = reactionJustDied;
				}
				else
				{
					TIMER_REACTION_GAME_SPEED.start();
				}
			}
		}

		// set graphics
		for (uint8_t step = 0; step <= REACTION_GAME_TIMER_STEP; step++)
		{
			flashPictureToDisplayAllSegments(disp_4digits_animations + ANIMATE_CIRCLE_OFFSET + step * 4);
		}
		displayAllSegmentsToScreen();

		if (!EXTRA_OPTION_REACTION_SOUND_MODE_GUITAR_HEX_HERO || REACTION_GAME_LEVEL == 0)
		{
			// in the easy level of sound mode, we show the lights.

			// show decimal point for digit corresponding with button
			setDecimalPoint(true, REACTION_GAME_TARGET);
			// set appropriate led per button
			*lightsHandle = 1 << lights_indexed[REACTION_GAME_TARGET];
		}

		// check player pressed a button.
		for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
		{
			// button press
			if (binaryInputs[buttons_indexed[i]].getEdgeUp())
			{
				if (i == REACTION_GAME_TARGET)
				{
					//right button
					REACTION_GAME_SCORE++;

					if (EXTRA_OPTION_REACTION_SOUND_MODE_GUITAR_HEX_HERO)
					{
						// a small pause must be implemented after the button press before the new turn as off not to confuse the player
						addNoteToBuzzerRepeated(rest_1, 4);
						reactionGameState = reactionWaitBeforeNewTurn;
					}else{
						reactionGameState = reactionNewTurn;
					}
				}
				else
				{
					//wrong button
					reactionGameState = reactionJustDied;

					// zero points if you fail during the timed challenge
					if (OPTION_REACTION_COUNTDOWN_MODE_HERO_ADD_PAUSE_MODE){
						REACTION_GAME_SCORE=0;
					}
				}
			}
		}

		break;
	}

	case reactionJustDied:
	{

#ifdef ENABLE_EEPROM

		// play death song
		addNoteToBuzzerRepeated(F4_1, 3);

		//start high score end timer
		if (REACTION_GAME_SCORE > (int16_t)
									  eeprom_read_word(
									(uint16_t *)EEPROM_REACTION_GAME_OFFSET + 
								REACTION_GUITAR_HERO_MODE * 48 +
								EXTRA_OPTION_REACTION_SOUND_MODE_GUITAR_HEX_HERO * 24 +
								OPTION_REACTION_COUNTDOWN_MODE_HERO_ADD_PAUSE_MODE * 12 +
								REACTION_GAME_LEVEL
								 ))
		{

			eeprom_update_word(
			
				(uint16_t *)EEPROM_REACTION_GAME_OFFSET + 
								REACTION_GUITAR_HERO_MODE * 48 +
								EXTRA_OPTION_REACTION_SOUND_MODE_GUITAR_HEX_HERO * 24 +
								OPTION_REACTION_COUNTDOWN_MODE_HERO_ADD_PAUSE_MODE * 12 +
								REACTION_GAME_LEVEL
								,REACTION_GAME_SCORE
								);

			loadBuzzerTrack(SONG_ATTACK);
		}
#endif

		// prepare next game delay.

		TIMER_REACTION_END_OF_GAME_DELAY.setInitTimeMillis(-2000);
		TIMER_REACTION_END_OF_GAME_DELAY.start();

		reactionGameState = reactionFinished;

		break;
	}

	case reactionFinished:
	{
		if (!TIMER_REACTION_END_OF_GAME_DELAY.getTimeIsNegative())
		{
			//end of display high score, next game
			reactionGameState = reactionNewGame;
		}
		else
		{
			//do nothing.  wait for display high score is finished.
			if (TIMER_REACTION_END_OF_GAME_DELAY.getInFirstGivenHundredsPartOfSecond(500))
			{
			}
			else
			{
				ledDisp->setNumberToDisplayAsDecimal(REACTION_GAME_SCORE); //score display. Leave at beginning, to display high score blinking.
			}
		}
		break;
	}
	}
#endif

	*lightsHandle |= EXTRA_OPTION_REACTION_SOUND_MODE_GUITAR_HEX_HERO << LIGHT_LATCHING_SMALL_LEFT;
	*lightsHandle |= OPTION_REACTION_COUNTDOWN_MODE_HERO_ADD_PAUSE_MODE << LIGHT_LATCHING_SMALL_RIGHT;
	*lightsHandle |= REACTION_GUITAR_HERO_MODE << LIGHT_LATCHING_BIG;
}


bool Apps::nextStep(int16_t *counter, bool countUpElseDown, int16_t minValue, int16_t maxValue, bool overflowToOtherSide){
	int16_t original_value;
	*counter += -1 + (2 * countUpElseDown);

	checkBoundaries(counter, minValue, maxValue, overflowToOtherSide);
	return original_value != *counter;
}

bool Apps::nextStepRotate(int16_t *counter, bool countUpElseDown, int16_t minValue, int16_t maxValue)
{
	return nextStep(counter, countUpElseDown, minValue, maxValue, true);
}

void Apps::checkBoundaries(int16_t *counter, int16_t minValue, int16_t maxValue, bool rotate)
{
	if (*counter > maxValue)
	{
		if (rotate)
		{
			*counter = minValue;
		}
		else
		{
			*counter = maxValue;
		}
	}
	else if (*counter < minValue)
	{
		if (rotate)
		{
			*counter = maxValue;
		}
		else
		{
			*counter = minValue;
		}
	}
}

uint32_t Apps::fadeInList(uint8_t step, uint8_t length, uint32_t startScreen, uint8_t *shuffledSequence)
{
	uint32_t fullScreen = startScreen;

	for (uint8_t i = 0; i < step; i++)
	{
		fullScreen |= 1UL << shuffledSequence[i]; // 1UL because if just 1 it's a 16 bit constant. (yep yep Lucie, nonkel Lode lost a couple of hours solving this!)
	}
	return fullScreen;
}

uint8_t Apps::tombola(uint8_t *indexVariable, uint8_t *sequenceList, uint8_t length)
{
	// will populate a list of certain length with unique numbers in random order. At every call, will take a next number until all numbers are depleted. It will then repopulate the list with a new random sequence
	if (*indexVariable == 0)
	{
		loadBuzzerTrack(SONG_DRYER_HAPPY);
		randomSequence(sequenceList, length);
		*indexVariable = length;
	}

	// draw card off deck
	return sequenceList[--(*indexVariable)]; //
}

void Apps::randomSequence(uint8_t *sequenceList, uint8_t length)
{
	// initialize list
	for (uint8_t i = 0; i < length; i++)
	{
		sequenceList[i] = i;
	}

	// // shuffle in place
	this->shuffle(sequenceList, length);
}

void Apps::shuffle(uint8_t *listToShuffle, uint8_t length)
{
	//shuffle the array:
	//http://www.geeksforgeeks.org/shuffle-a-given-array/
	for (int i = 0; i < length; i++)
	{
		uint8_t randomIndex = random(i, length);

		// swap contents of current index with content further index.
		uint8_t tmp = listToShuffle[i];
		listToShuffle[i] = listToShuffle[randomIndex];
		listToShuffle[randomIndex] = tmp;
	}
}

bool Apps::saveLoadMenu(uint8_t *data, uint8_t slotCount, uint8_t eepromSlotLength, uint16_t eepromStartAddress)
{
	// we will need to make this a separate function, it's almost identical to sequencer load save.
	// pointer to the data
	//-number of save slots
	//-length of data to save.  (=eeprom data length)
	// eeprom start address

	//kind of an init
	if (!SAVE_LOAD_MENU_BLINK_TIMER.getIsStarted())
	{
		SAVE_LOAD_MENU_BLINK_TIMER.setInitTimeMillis(-1000);
		SAVE_LOAD_MENU_BLINK_TIMER.start();
	}

	// load/save data

	uint8_t slot_number = encoder_dial->getValueLimited(slotCount*16, false) / 16;
	
	if (SAVE_LOAD_MENU_BLINK_TIMER.getInFirstGivenHundredsPartOfSecond(500))
	{
		//blink alternatively song number and "load" or "save"
		ledDisp->setNumberToDisplayAsDecimal(slot_number);
	}
	else
	{
		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
		{
			setStandardTextToTextBuf(TEXT_SAVE);
		}
		else
		{
			setStandardTextToTextBuf(TEXT_LOAD);
		}

		textBufToDisplay();
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
	{
		bool loadElseSave = !(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA));

		saveLoadFromEepromSlot(data, slot_number - 1, eepromSlotLength, eepromStartAddress, loadElseSave);

		return loadElseSave;
	}

	if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
	{
		SAVE_LOAD_MENU_BLINK_TIMER.start();
	}
	return false;
}

void Apps::saveLoadFromEepromSlot(uint8_t *data, uint8_t slotIndex, uint8_t eepromSlotLength, uint16_t eepromStartAddress, boolean loadElseSave)
{
	for (uint8_t i = 0; i < eepromSlotLength; i++)
	{
		uint8_t *eeprom_address = (uint8_t *)(eepromStartAddress +
											  slotIndex * eepromSlotLength + i);
		if (loadElseSave)
		{
			//load
			data[i] = eeprom_read_byte(eeprom_address);
		}
		else
		{
			//save
			eeprom_write_byte(eeprom_address, data[i]);
		}
	}
}

void Apps::displayAllSegmentsToScreen(){
	ledDisp->setBinaryToDisplay(this->displayAllSegments);
}

void Apps::flashPictureToDisplayAllSegments(const uint8_t* progmemAddress){
	for (uint8_t i = 0; i < 4; i++)
	{
		this->displayAllSegments |= (uint32_t)pgm_read_byte_near(progmemAddress + (i)) << (8 * i); //* 4 --> 4 bytes per dword
	}
}

void Apps::eepromPictureToDisplayAllSegments(int16_t offset, int16_t pictureIndex){
	for (uint8_t i = 0; i < 4; i++){
		this->displayAllSegments |= (uint32_t)(eeprom_read_byte((uint8_t *)(offset + pictureIndex * 4 + i))) << (i * 8);
	}
}

void Apps::textBufToDisplay(){
	ledDisp->setTextBufToDisplay(textBuf);
}

void Apps::buzzerOffAndAddNoteAtEncoderDialChange(uint8_t note){
	if (encoder_dial->getDelta()){
		buzzerOffAndAddNote(note);
	}
}

void Apps::buzzerOffAndAddNote(uint8_t note){
	buzzerOff();
	addNoteToBuzzer(note);
}

void Apps::addNoteToBuzzer(uint8_t note){
	buzzer->programBuzzerRoll(note);
}

void Apps::addNoteToBuzzerRepeated(uint8_t note, uint8_t repeater){
	for (uint8_t i=0;i<repeater;i++){
        this->addNoteToBuzzer(note);
    }
}
void Apps::noteToDisplay(uint8_t note){
	buzzer->noteToDisplay(textHandle, decimalDotsHandle,note);
}

void Apps::setDecimalPoint(bool onElseOff, uint8_t digit){
	ledDisp->setDecimalPointToDisplay(onElseOff,digit);
}

void Apps::decimalPointTimingOn(){
	setDecimalPoint(true,1);
}

void Apps::setBlankDisplay(){
	ledDisp->setBlankDisplay();
}
void Apps::setLedArray(byte lights){
	ledDisp->setLedArray(lights);
}

void Apps::buzzerOff(){
	buzzer->buzzerOff(); // stop all sounds that were playing in an app.
}

void Apps::buzzerChangeSpeedRatioWithEncoderDial(){
	buzzer->changeSpeedRatio(encoder_dial->getDelta());
}

unsigned int Apps::indexToTimeSeconds(int16_t index){
#ifdef ENABLE_MULTITIMER
	return this->multiTimer.getIndexedTime(index);
#else
	return index * index; // untested
#endif
}
void Apps::setStandardTextToTextBuf(uint8_t textPosition){
	ledDisp->setStandardTextToTextBuf(textBuf, textPosition);
}

void Apps::setStandardTextToTextHANDLE(uint8_t textPosition){
	ledDisp->setStandardTextToTextBuf(textHandle, textPosition);
}

// does not decrease footprint!
// bool  Apps::isNoMomentaryButtonOn(){
// 	return !(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)) &&
// 			!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)) &&
// 			!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)) &&
// 			!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3))
// }

void Apps::numberToBufAsDecimal(int16_t number){
	ledDisp->numberToBufAsDecimal(textBuf, number);
}

void Apps::loadBuzzerTrack(uint8_t songIndex){
	buzzer->loadBuzzerTrack(songs, songIndex);
}