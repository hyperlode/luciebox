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
			// this->quiz(initOnBigLatchInitToo);
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

		case APP_SELECTOR_TILT:
		case APP_SELECTOR_TILT_ADVANCED:
			this->tiltSwitchTest(initOnBigLatchInitToo);
			break;

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

// void Apps::updateEveryAppCycleAfter()
// {

// }

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
}
// void Apps::updateDisplay()
// {
// 	// every cycle display update.

// }

void Apps::setDefaultMode()
{
	//button lights
	setLedArray(0b00000000); // no lights

	//display
	setBlankDisplay();
	decimalPoints = 0; // set all decimal points off. segment 4 = bit 3, ....   00043210 (segment number)
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
	// shows splash screen

	if (init)
	{
		setBlankDisplay();
		// init of the init_app..
		this->displayAllSegments = 0;

		ledDisp->progmemToDisplayBuffer(&displayAllSegments, app_splash_screens + selector * 4);

		// // initialize list
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

// int8_t Apps::momentaryButtonEdge(){

// }

void Apps::pomodoroTimer(bool init)
{
	uint8_t display_mode = POMODORO_DISPLAY_TIMER;
	lights = 0;

	if (init)
	{
		POMODORO_INIT_TIME_SECONDS = POMODORO_INIT_DEFAULT_SECS;
		POMODORO_PAUSE_TIME_SECONDS = POMODORO_PAUSE_DEFAULT_SECS;

		// most variables set at main menu init.
		POMODORO_SHOW_MENU_EDGE = false; // at init make sure to run through main menu once to set all varialbes that are always set at main menu entering.

		POMODORO_STATS_WORKING_GOOD = 0;
		POMODORO_STATS_WORKING_BAD = 0;
		POMODORO_PROBABILITY_BEEP_EVERY_SECONDS = 0; // zero means disabled.

		setBlankDisplay();
	}

	boolean showMenu = true;
	if ((binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA)) &&
		!init &&
		(POMODORO_AUTO_RESTART_ENABLED || (!POMODORO_IN_BREAK)) // if not auto restart, we don't even do a break
	)
	{
		showMenu = false;
	}

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
	{
		POMODORO_IN_BREAK = false;
	}

	// in main menu or timing? (run main menu at least once at init. Even when start button started) to initialize variables depending on settings latching buttons
	if (!showMenu)
	{
		// pomodoro timer running

		if (POMODORO_SHOW_MENU_EDGE)
		{
			POMODORO_TIMER.start();
		}

		if (!POMODORO_TIMER.getTimeIsNegative())
		{
			POMODORO_IN_BREAK = !POMODORO_IN_BREAK;
			if (POMODORO_IN_BREAK)
			{
				POMODORO_TIMER.setInitCountDownTimeSecs(POMODORO_PAUSE_TIME_SECONDS);
				loadBuzzerTrack(SONG_DRYER_HAPPY);
				POMODORO_TIMER.start();
			}
			else
			{
				// coming out of break. Not executed at starting Pomodoro by switch.
				loadBuzzerTrack(SONG_ATTACK);
				POMODORO_TIMER.setInitCountDownTimeSecs(POMODORO_INIT_TIME_SECONDS);
				if (POMODORO_AUTO_RESTART_ENABLED)
				{
					POMODORO_TIMER.start();
				}
				else
				{
					POMODORO_SHOW_MENU_EDGE = true;
				}
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
	}
	else
	{
		// in main menu
// #ifndef ENABLE_MULTITIMER
// 		uint16_t tmpSeconds = encoder_dial->getValueMapped(0, 1024);
// #endif

		if (!POMODORO_SHOW_MENU_EDGE)
		{
			POMODORO_TIMER.reset();
			POMODORO_TIMER.setInitCountDownTimeSecs(POMODORO_INIT_TIME_SECONDS);
		}

		uint16_t tmpSeconds = POMODORO_NONSENSE_TIME;
		// encoder_dial->setRange(90,false);

		if (encoder_dial->getValueChanged())
		{

#ifdef ENABLE_MULTITIMER
			// uint16_t tmpSeconds = this->multiTimer.getIndexedTime(encoder_dial->getValueMapped(0, 90));
			
			uint16_t tmpSeconds = this->multiTimer.getIndexedTime(encoder_dial->getValueLimited(90,false));
#else
			uint16_t tmpSeconds = encoder_dial->getValueLimited(60,false) * 30;
#endif
			if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
			{
				POMODORO_INIT_TIME_SECONDS = tmpSeconds;
				POMODORO_TIMER.setInitCountDownTimeSecs(POMODORO_INIT_TIME_SECONDS);
			}
		}

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
		{
			if (tmpSeconds != POMODORO_NONSENSE_TIME)
			{
				POMODORO_PAUSE_TIME_SECONDS = tmpSeconds;
			}
			display_mode = POMODORO_DISPLAY_PAUSE_INIT_SECS;
		}

		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT))
		{
			if (tmpSeconds != POMODORO_NONSENSE_TIME)
			{
				POMODORO_PROBABILITY_BEEP_EVERY_SECONDS = tmpSeconds;
			}
			display_mode = POMODORO_DISPLAY_BEEP_PROBABILITY;
		}

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		{
			display_mode = POMODORO_DISPLAY_SHOW_GOOD;
		}

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)))
		{
			display_mode = POMODORO_DISPLAY_SHOW_BAD;
		}
	}

	POMODORO_AUTO_RESTART_ENABLED = binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT);

	// POMODORO_SHOW_MENU_EDGE is our edge detector
	POMODORO_SHOW_MENU_EDGE = showMenu;

	// ticking sound
	// long tick_duration = encoder_dial->getValueMapped(0, 40);
	encoder_dial->setRange(40,true);
	int16_t tick_duration = encoder_dial->getValue();
	bool tick_twice_a_second = tick_duration > 20;
	if (POMODORO_TIMER.getEdgeSinceLastCallFirstGivenHundredsPartOfSecond(500, true, tick_twice_a_second))
	{
		if (POMODORO_PROBABILITY_BEEP_EVERY_SECONDS > 0)
		{
			// on average every two minutes play beep
			// if ( random(0,21) > 19 + (tick_twice_a_second) ){
			if (random(0, POMODORO_PROBABILITY_BEEP_EVERY_SECONDS * 2) <= 1 - (tick_twice_a_second))
			{
				// random has 0 included. as we have to take into account the double ticking,
				// calculate the probability in half a seconds. i.e.  one every minute: (0,120)

				addNoteToBuzzer(C5_1);
			};
		}

		if (tick_duration > 0)
		{
			// no sound when zero

			// twenty settings, for each mode (two ticks or one tick per second)
			if (tick_duration > 20)
			{
				tick_duration -= 20;
			}
			if (buzzer->getBuzzerRollEmpty())
			{
				// if alarm sounds, no ticking!
				buzzer->playTone(8000, tick_duration); // works well
			}
		}
	}
	// display
	switch (display_mode)
	{
	case POMODORO_DISPLAY_TIMER:
		POMODORO_TIMER.getTimeString(textBuf);
		break;

	case POMODORO_DISPLAY_PAUSE_INIT_SECS:
	{
		timeSecondsToClockString(textBuf, POMODORO_PAUSE_TIME_SECONDS);
		if (millis() % 1000 > 650)
		{
			setStandardTextToTextBuf(TEXT_PAUS);
		}
	}
	break;
	case POMODORO_DISPLAY_BEEP_PROBABILITY:
	{
		timeSecondsToClockString(textBuf, POMODORO_PROBABILITY_BEEP_EVERY_SECONDS);
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

	ledDisp->setDecimalPointToDisplay(false, 1);
	// decimalPoints = 0;
	if (showMenu)
	{
		// decimalPoints = 1 << 2;
		ledDisp->setDecimalPointToDisplay(true, 1);

		if (millis() % 500 < 250)
		{
			lights |= 1 << LIGHT_LATCHING_EXTRA;
		}
	}
	else if (POMODORO_TIMER.getInFirstGivenHundredsPartOfSecond(500))
	{
		ledDisp->setDecimalPointToDisplay(true, 1);
		lights |= 1 << LIGHT_LATCHING_EXTRA;
	}
	else if (POMODORO_IN_BREAK)
	{
		setStandardTextToTextBuf(TEXT_PAUS);
	}

	// decimalPoints = 1 << 2;
	// 	//lights|= 1<<LIGHT_LATCHING_EXTRA;
	// 	if (showMenu){

	// 	}else if (POMODORO_TIMER.getInFirstGivenHundredsPartOfSecond(500)){
	// 		//decimalPoints = 1 << 2;

	// 	}else if  (POMODORO_IN_BREAK){
	// 		textBuf[0]='P';
	// 		textBuf[1]='A';
	// 		textBuf[2]='U';
	// 		textBuf[3]='S';
	// 		decimalPoints = 0;
	// 	}else{
	// 		decimalPoints = 0;
	// 	}

	// leds

	if (POMODORO_PROBABILITY_BEEP_EVERY_SECONDS > 0)
	{
		lights |= 1 << LIGHT_LATCHING_SMALL_LEFT;
	}
	if (POMODORO_AUTO_RESTART_ENABLED)
	{
		lights |= 1 << LIGHT_LATCHING_SMALL_RIGHT;
	}
	if (!showMenu)
	{
		lights |= 1 << LIGHT_LATCHING_EXTRA;
		lights |= 1 << LIGHT_MOMENTARY_2;
		lights |= 1 << LIGHT_MOMENTARY_3;
	}

	textBufToDisplay();
	setLedArray(lights);
}

void Apps::stopwatch(bool init)
{
	// classic stopwatch
	long time_millis = 0;
	uint8_t timeDisplayShift = 0;

	if (init)
	{
		STOPWATCH_LAP_MEMORY = 0;

		STOPWATCH_CHRONO.setInitTimeMillis(0);
		STOPWATCH_CHRONO.reset();
		STOPWATCH_CHRONO.startPaused(true);
	}

	// if (binaryInputsEdgedown& (1<<BUTTON_INDEXED_LATCHING_EXTRA))
	// {
	// 	STOPWATCH_CHRONO.pause();
	// }

	// if (binaryInputsValue & (1<<BUTTON_INDEXED_LATCHING_EXTRA))
	// {
	// 	STOPWATCH_CHRONO.startPaused(false);
	// }

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
	{
		// save and show laptime at press
		STOPWATCH_LAP_MEMORY = STOPWATCH_CHRONO.getTimeMillis();
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
	{
		// reset
		// STOPWATCH_CHRONO.reset();
		STOPWATCH_CHRONO.startPaused(STOPWATCH_CHRONO.getIsPaused());
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
	{
		STOPWATCH_CHRONO.paused(!STOPWATCH_CHRONO.getIsPaused());
	}

	time_millis = STOPWATCH_CHRONO.getTimeMillis();

	if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)) ||
		(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
	{
		// show saved laptime at press
		time_millis = STOPWATCH_LAP_MEMORY;
	}

	if (time_millis < 10000)
	{
		timeDisplayShift = 0;
	}
	else if (time_millis < 100000)
	{
		time_millis /= 10;
		timeDisplayShift = 1;
	}
	else if (time_millis < 1000000)
	{
		time_millis /= 100;
		timeDisplayShift = 2;
	}
	else
	{
		time_millis /= 1000;
		timeDisplayShift = 3;
	}

	textBuf[0] = ' ';
	textBuf[1] = ' ';

	intToDigitsString(textBuf, time_millis, true);
	decimalPoints = 0x1 << timeDisplayShift;

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
	{
		STOPWATCH_CHRONO.getTimeString(textBuf);
	}
	else
	{
		textBufToDisplay();
		ledDisp->setDecimalPointsToDisplay(decimalPoints);
	}
}

void Apps::modeRandomWorld(bool init)
{
	if (init)
	{
		RANDOMWORLD_CARD_FROM_DECK_INDEX = 0;
		randomWorldState = randomWorldIdle;
		RANDOMWORLD_RANDOM_TYPE = 0;
		randomModeDisplay(false);
		decimalPoints = 0;

		RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW = 100;
	}

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
				randomModeDisplay(false);

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
			randomModeDisplay(false);

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

		randomModeDisplay(true);
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

void Apps::randomModeDisplay(bool forReal)
{
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

	textBufToDisplay();
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
	if (encoder_dial->getValueChanged() < 0 && SETTINGS_MODE_SELECTOR % 2 == 0)
	{
		SETTINGS_MODE_SELECTOR++;
	}
	else if (encoder_dial->getValueChanged() > 0 && SETTINGS_MODE_SELECTOR % 2 != 0)
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
		setBlankDisplay();

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

	setBlankDisplay();
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
		counter += encoder_dial->getValueChanged();
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

	setBlankDisplay();

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
	{
		buzzer->changeTranspose(encoder_dial->getValueChanged()); 
	}
	else
	{
		buzzer->changeSpeedRatio(encoder_dial->getValueChanged());
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
			// display song by index (enable insert delete position)

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
				buzzerOff();
				addNoteToBuzzer(COMPOSER_SONG[COMPOSER_STEP]);
			}

			if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
			{
				// just play notes selected with dial
				if (encoder_dial->getValueChanged())
				{
					buzzerOff();
					uint8_t note = (uint8_t)encoder_dial->getValueLimited(255, true);
					addNoteToBuzzer(note);
					buzzer->noteToDisplay(textBuf, &decimalPoints, note);
					textBufToDisplay();
					ledDisp->setDecimalPointsToDisplay(decimalPoints);
				}
				defaultDisplay = false;
			}

			// program note in song
			if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
			{
				uint8_t note = (uint8_t)encoder_dial->getValueLimited(255, true);
				COMPOSER_SONG[COMPOSER_STEP] = note;
				buzzerOff();
				addNoteToBuzzer(COMPOSER_SONG[COMPOSER_STEP]);

				// if note added to end, expand song length and add default note
				if (COMPOSER_STEP == COMPOSER_SONG_LENGTH - 1)
				{
					COMPOSER_SONG_LENGTH++;
					COMPOSER_SONG[COMPOSER_SONG_LENGTH - 1] = rest_1; //default note
				}
			}

			if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
			{
				if (encoder_dial->getValueChanged())
				{
					uint8_t note = (uint8_t)encoder_dial->getValueLimited(255, true);
					COMPOSER_SONG[COMPOSER_STEP] = note;
					buzzerOff();
					addNoteToBuzzer(COMPOSER_SONG[COMPOSER_STEP]);
				}
			}
		}
#if MOMENTARY_BUTTONS_COUNT == 4
		modifyValueUpDownWithMomentary2And3(&step, 1);
#else
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
		{
			step = 1;
		}
#endif

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
		// if (!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)) &&
		// 	!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)) &&
		// 	!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)) &&
		// 	!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3))) 

		{
			// int8_t tmp = 2 * potentio->getLastStableValueChangedUp() - 1;

			if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
			{
				// change speed if default behaviour of potentio.
				// COMPOSER_STEP_TIMER.setInitTimeMillis(COMPOSER_STEP_TIMER.getInitTimeMillis() + tmp * 10); //step +1 or -1
				COMPOSER_STEP_TIMER.setInitTimeMillis(COMPOSER_STEP_TIMER.getInitTimeMillis() + encoder_dial->getValueChanged() *10); //step +1 or -1
			}
			else
			{
				step = encoder_dial->getValueChanged(); ////step +1 or -1
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
				buzzer->noteToDisplay(textBuf, &decimalPoints, COMPOSER_SONG[COMPOSER_STEP]);
				textBufToDisplay();
				ledDisp->setDecimalPointsToDisplay(decimalPoints);
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
	int16_t delta;
	//buzzer with buzzer roll (notes).

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

		// delta = (uint16_t)(SOUND_NOTE_AUTO_TIMER.getInitTimeMillis() / -8);

		// checkBoundaries(&delta, 2, 254, false);

		dialOnEdgeChangeInitTimerPercentage(&SOUND_NOTE_AUTO_TIMER);
	}
	else
	{
		// change note with potentio
		if (encoder_dial->getValueChanged())
		{
			SOUND_NOTE_AUTO_UP_ELSE_DOWN = encoder_dial->getValueChanged()>0;
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
		buzzer->noteToDisplay(textHandle, decimalDotsHandle, SOUND_NOTES_NOTE_INDEX);
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
		counter = 4; // display is four characters. Four bytes.So, it should advance four bytes every frame (default). But, it could give fun effects to change that number and see what happens...
		this->dataPlayer.loadAllData(disp_4digits_animations);
		this->dataPlayer.loadDataSet(1);
		this->dataPlayer.setAutoSteps(4);
		MOVIE_MODE_SHOW_NEGATIVE = false;
	}

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
	{
		// manual mode
		if (encoder_dial->getValueChanged())
		{
			this->dataPlayer.setSetIndexDirection(encoder_dial->getValueChanged()>0);
			this->dataPlayer.moveIndexSteps(counter); // every frame is four bytes. advance four to move one frame.
		}

		// one step forward
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
		{
			this->dataPlayer.setSetIndexDirection(1);
			this->dataPlayer.moveIndexSteps(counter);
		}

		// one step backward
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
		{
			this->dataPlayer.setSetIndexDirection(0);
			this->dataPlayer.moveIndexSteps(counter);
		}
	}
	else
	{
		// auto mode.

		if (encoder_dial->getValueChanged())
		{
			dataPlayer.setAutoStepSpeed(encoder_dial->getValueLimited(1023, false) - 1023);
		}

		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
		{
			this->dataPlayer.setSetIndexDirection(2);
		}

		if (!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		{
			this->dataPlayer.update(); // this to pause the movie while holding.
		}
	}

	// next movie
	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
	{
		this->dataPlayer.nextDataSet(true);
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
	{
		MOVIE_MODE_SHOW_NEGATIVE = !MOVIE_MODE_SHOW_NEGATIVE;
	}

	// get the display data.
	displayAllSegments = this->dataPlayer.getActive32bit();

	// invert all data in picture
	if (MOVIE_MODE_SHOW_NEGATIVE)
	{
		// negative .
		displayAllSegments = ~displayAllSegments;
	}

	// set to display
	ledDisp->setBinaryToDisplay(displayAllSegments);
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

		DRAW_SHOW_MODE >= 3 ? DRAW_SHOW_MODE = 0 : DRAW_SHOW_MODE++;

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
			//full
			*display_buffer = 0xFFFFFFFF;
			break;
		case 3:
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
	if (encoder_dial->getValueChanged())
	{
		DRAW_CURSOR_INDEX = encoder_dial->getValueLimited(35,true);
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
	// shows a picture. After it disappears, you have to drawn it exactly as it was.

	uint32_t cursorBlinker = 0;

	if (init)
	{
		drawGameState = drawGameWaitForStart;
		//numberToBufAsDecimal(4444);
	}

	switch (drawGameState)
	{

	case drawGameWaitForStart:
	{
		setBlankDisplay();

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
		// displayAllSegments = 0xFF00FF00;

		if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
		{
			drawGameState = drawGameDraw;
			displayAllSegmentsBuffer = displayAllSegments;
			displayAllSegments = 0;
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

			//DRAW_GAME_DISPLAY_TIMER.setInitTimeMillis(-3000);
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
		// if (!DRAW_GAME_DISPLAY_TIMER.getTimeIsNegative()){
		if (binaryInputs[BUTTON_LATCHING_EXTRA].getValueChanged())
		{
			drawGameState = drawGameWaitForStart;
		}

		if (DRAW_GAME_DISPLAY_TIMER.getEdgeSinceLastCallFirstGivenHundredsPartOfSecond(500, true, true))
		{
			//if (millis() % 500 > 250){
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
		HACKTIME_ADDRESS = 0;
		HACKTIME_DISPLAY_MODE = HACKTIME_DISPLAY_ADDRESS;
		HACKTIME_MOVE_TIMER.setInitTimeMillis(-500);
		HACKTIME_MOVE_TIMER.start();
	}

	// write to mem if possible
	if ((binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT)) && HACKTIME_MEMORY_SELECT != HACKTIME_MEMORY_FLASH)
	{ //
		// change value in address location (left char on display)
		// will not work for flash memory

		HACKTIME_DISPLAY_MODE = HACKTIME_DISPLAY_HEX;
		// value to change is the one that is in the primary position (arry8bytes 0)

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
		{
			// change value
			array_8_bytes[0] = encoder_dial->getValueLimited(255,false);
		}

		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
		{
			// store value.
			switch (HACKTIME_MEMORY_SELECT)
			{
			// case HACKTIME_MEMORY_FLASH:

			// break;
			case HACKTIME_MEMORY_RAM:
				*((uint8_t *)HACKTIME_ADDRESS) = array_8_bytes[0];

				break;
			case HACKTIME_MEMORY_EEPROM:
				eeprom_write_byte((uint8_t *)HACKTIME_ADDRESS, array_8_bytes[0]);
				break;
			}
		}
	}
	else
	{
		// change address values

		// which memory are we investigating?
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
		{
			HACKTIME_MEMORY_SELECT++;
			if (HACKTIME_MEMORY_SELECT > 2)
			{
				HACKTIME_MEMORY_SELECT = 0;
			}
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

			// address_changed = potentio->increaseSubtractAtChange(
			// 	&HACKTIME_ADDRESS,
			// 	1 + 99 * ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)) > 0) +
			// 		999 * ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)) > 0) // speed up memory scroll by pressing buttons.
			// );
			if (encoder_dial->getValueChanged()){
				address_changed = true;
				HACKTIME_ADDRESS += encoder_dial->getValueChanged() ;  // todo change rate depending on buttons pressed: BUTTON_INDEXED_MOMENTARY_2 -> *100,  BUTTON_INDEXED_MOMENTARY_3 -> *1000,
			}
			
		}

		// display mode change (how to represent the memory value?)
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
		{
			nextStepRotate(&HACKTIME_DISPLAY_MODE, 1, 0, 4);
		}

		// button address change.

		// if (binaryInputsEdgeUp & (1<<BUTTON_INDEXED_MOMENTARY_2)){

		// 	HACKTIME_ADDRESS --;
		// 	address_changed = true;
		// }

		// if (binaryInputsEdgeUp & (1<<BUTTON_INDEXED_MOMENTARY_3)){
		// 	// no limit checks. This is hacktime!
		// 	HACKTIME_ADDRESS ++;
		// 	address_changed = true;
		// }
		address_changed = modifyValueUpDownWithMomentary2And3(&HACKTIME_ADDRESS, 1);

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
	if (address_changed && (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT)))
	{ //
		addNoteToBuzzer(array_8_bytes[0]);
	}

	// compressed display mode (to save memory)
	if (HACKTIME_DISPLAY_MODE == HACKTIME_DISPLAY_CHARS)
	{
		// do nothing
	}
	else
	{
		setBlankDisplay();

		if (HACKTIME_DISPLAY_MODE == HACKTIME_DISPLAY_BYTES)
		{
			displayAllSegments = 0;
			for (uint8_t i = 0; i < 4; i++)
			{
				displayAllSegments |= ((uint32_t)(array_8_bytes[i])) << (8 * i);
			}
			ledDisp->setBinaryToDisplay(displayAllSegments);
		}
		else if (HACKTIME_DISPLAY_MODE == HACKTIME_DISPLAY_ADDRESS)
		{
			if (millis() % 1000 > 200)
			{
				ledDisp->setNumberToDisplay(HACKTIME_ADDRESS, true); // show address as hex, to fit all addresses on 4 chars display
			}
			else
			{
				textHandle[0] = drive_letter[HACKTIME_MEMORY_SELECT];
			}
		}
		else
		{
			ledDisp->setNumberToDisplay(array_8_bytes[0], HACKTIME_DISPLAY_MODE == HACKTIME_DISPLAY_HEX);
		}
	}

	// switch(HACKTIME_DISPLAY_MODE){
	// 	case HACKTIME_DISPLAY_ADDRESS:{
	// 		if (millis() % 1000 > 200){

	// 			ledDisp->setNumberToDisplay(HACKTIME_ADDRESS, true);
	// 		}else{
	// 			// ledDisp->setNumberToDisplayAsDecimal(HACKTIME_ADDRESS);
	// 			setBlankDisplay();
	// 			textHandle[0] = drive_letter[HACKTIME_MEMORY_SELECT];
	// 		}
	// 	}
	// 	break;
	// 	case HACKTIME_DISPLAY_CHARS:{
	// 		for(uint8_t i=0;i<4;i++){
	// 			textHandle[i] = array_8_bytes[i];
	// 		}
	// 	}
	// 	break;
	// 	case HACKTIME_DISPLAY_BYTES:{
	// 		displayAllSegments = 0;
	// 		for(uint8_t i=0;i<4;i++){
	// 			displayAllSegments |= (array_8_bytes[i]) << (8*i);
	// 		}
	// 		ledDisp->setBinaryToDisplay(displayAllSegments);
	// 	}
	// 	break;
	// 	case HACKTIME_DISPLAY_DECIMAL:{
	// 		ledDisp->setNumberToDisplayAsDecimal(array_8_bytes[0]);
	// 	}
	// 	break;
	// 	case HACKTIME_DISPLAY_HEX:{
	// 		ledDisp->setNumberToDisplay(array_8_bytes[0],true);
	// 	}
	// 	break;
	// }
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
	if (encoder_dial->getValueChanged()){
		long original = (aTimer->getInitTimeMillis());
		long result = long((float)original* ( 1 - (float)(encoder_dial->getValueChanged()) * 0.01));
		// Serial.println(original);
		// Serial.println(result);
		// if value to small to make an absolute difference, force it! (make sure to stay negative)
		if (original == result){
			result -= encoder_dial->getValueChanged() * encoder_dial->getValueChanged();
		}

		aTimer->setInitTimeMillis(
			result	 
			);
	}
}

void Apps::draw(bool init)
{

	uint32_t cursorBlinker = 0;

	if (init)
	{
		//reset saved led disp state.
		// no memory wasting with setting to zero, instead, we'll load the first picture right away

		DRAW_ACTIVE_DRAWING_INDEX = 0;
		DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY = 1; // make different than active drawing index to force loading of first drawing.
		
	}

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
		// if (potentio->getValueChanged())
		// {
		// 	DRAW_ACTIVE_DRAWING_INDEX += 1 - (2 * potentio->getLastStableValueChangedUp());
		// }
		// potentio->increaseSubtractAtChange(&DRAW_ACTIVE_DRAWING_INDEX, 1);

		DRAW_ACTIVE_DRAWING_INDEX += encoder_dial->getValueChanged();
		
		if (!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
		{ // shift function for saving drawings to eeprom.

			//check for global display change. we're not really changing the drawing, just seeing how it would look negative, and stuf..
			this->displayChangeGlobal(&displayAllSegments, false);

			//scroll through drawings.
			modifyValueUpDownWithMomentary2And3(&DRAW_ACTIVE_DRAWING_INDEX, 1);
		}
	}
#ifdef ENABLE_EEPROM
	checkBoundaries(&DRAW_ACTIVE_DRAWING_INDEX, 0, EEPROM_NUMBER_OF_DRAWINGS - 1, true);

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
	{

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
		{
			// special function in save image to eeprom mode.
			if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
			{
				// insert

				// //insert after current index (and move to it)
				if (DRAW_ACTIVE_DRAWING_INDEX >= EEPROM_NUMBER_OF_DRAWINGS - 1)
				{
					// one picture before the last one is the last position where you can still insert a drawing.
				}
				else
				{

					// work with eeprom addresses, not with picture indexes.
					for (int16_t i = EEPROM_PICTURES_START_ADDRESS + (EEPROM_PICTURES_LENGTH - 1);
						 i >= EEPROM_PICTURES_START_ADDRESS + DRAW_ACTIVE_DRAWING_INDEX * 4;
						 i--)
					{
						// move all pictures one up.
						uint8_t tmp = eeprom_read_byte((uint8_t *)(i));
						eeprom_write_byte((uint8_t *)(i + 4), tmp);
					}
					DRAW_ACTIVE_DRAWING_INDEX++; // move to "new picture."
				}
			}

			if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
			{
				// delete slot. (and shift all drawings.)
				for (int16_t i = EEPROM_PICTURES_START_ADDRESS + DRAW_ACTIVE_DRAWING_INDEX * 4;
					 i < EEPROM_PICTURES_START_ADDRESS + (EEPROM_PICTURES_LENGTH - 3);
					 i++)
				{
					uint8_t tmp = eeprom_read_byte((uint8_t *)(i + 4));
					eeprom_write_byte((uint8_t *)(i), tmp);
				}
			}
		}
		else
		{
			if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
			{
				// save active drawing on display to eeprom.
				for (uint8_t i = 0; i < 4; i++)
				{
					eeprom_write_byte((uint8_t *)(EEPROM_PICTURES_START_ADDRESS + DRAW_ACTIVE_DRAWING_INDEX * 4 + i), (uint8_t)((displayAllSegments >> (i * 8)) & 0xFF));
				}
			}
		}

	}
	else if (DRAW_ACTIVE_DRAWING_INDEX != DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY)
	{
		// load drawing from memory at request.
		// load drawing
		displayAllSegments = 0;
		for (uint8_t i = 0; i < 4; i++)
		{
			displayAllSegments |= (uint32_t)(eeprom_read_byte((uint8_t *)(EEPROM_PICTURES_START_ADDRESS + DRAW_ACTIVE_DRAWING_INDEX * 4 + i))) << (i * 8);
		}
		this->displayChangeGlobal(&displayAllSegments, true);
	}
#endif

	DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY = DRAW_ACTIVE_DRAWING_INDEX;

	setBlankDisplay();
	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT))
	{
		// always show index of active drawing if activated.
		ledDisp->setNumberToDisplayAsDecimal(DRAW_ACTIVE_DRAWING_INDEX + 1); // in the real world, most of the people start counting from 1. Welcome to an eternal discussion Lucie!
	}
	else
	{
		// set display
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
	if (encoder_dial->getValueChanged())
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

	//textBufToDisplay();
	setLedArray(lights);
	ledDisp->setDecimalPointToDisplay(LIGHT_SECONDS_BLINKER & settingsLights, 1);
}
#endif

void Apps::tiltSwitchTest(bool init)
{
	// four tilt switches are positioned as such that they are "ON" in rest position.
	uint32_t screen = 0;
	if (init)
	{
		setStandardTextToTextBuf(TEXT_TILT);
		counter = 0;
		counter2 = 0; // counts progress in movement.
		buzzer->setSpeedRatio(2.0);

		this->dataPlayer.loadAllData(disp_4digits_animations);

		this->dataPlayer.setAutoSteps(4);
		this->dataPlayer.setAutoStep(true);
		this->dataPlayer.setAutoStepSpeed(-30);
	}

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_BIG_RED))
	{
		// movie for each gesture

		if (binaryInputs[SWITCH_TILT_FORWARD].getEdgeDown())
		{
			this->dataPlayer.loadDataSet(1);
			this->dataPlayer.setSetIndexDirection(1);
		}
		if (binaryInputs[SWITCH_TILT_BACKWARD].getEdgeDown())
		{
			this->dataPlayer.loadDataSet(1);
			this->dataPlayer.setSetIndexDirection(0);
		}
		if (binaryInputs[SWITCH_TILT_LEFT].getEdgeDown())
		{
			this->dataPlayer.loadDataSet(0);
			this->dataPlayer.setSetIndexDirection(1);
		}
		if (binaryInputs[SWITCH_TILT_RIGHT].getEdgeDown())
		{
			this->dataPlayer.loadDataSet(0);
			this->dataPlayer.setSetIndexDirection(0);
		}
		this->dataPlayer.update();
		screen = this->dataPlayer.getActive32bit();

		ledDisp->setBinaryToDisplay(screen);
	}
	else
	{
		if (binaryInputs[SWITCH_TILT_FORWARD].getEdgeDown())
		{
			addNoteToBuzzer(1); //not beep but "puck"
			counter2 |= 0x01 << TILT_FORWARD;
		}

		if (binaryInputs[SWITCH_TILT_BACKWARD].getEdgeDown())
		{
			addNoteToBuzzer(1); //not beep but "puck"
			counter2 |= 0x01 << TILT_BACKWARD;
		}

		if (binaryInputs[SWITCH_TILT_LEFT].getEdgeDown())
		{
			addNoteToBuzzer(1); //not beep but "puck"
			counter2 |= 0x01 << TILT_LEFT;
		}

		if (binaryInputs[SWITCH_TILT_RIGHT].getEdgeDown())
		{
			addNoteToBuzzer(1); //not beep but "puck"
			counter2 |= 0x01 << TILT_RIGHT;
		}

		if (counter2 > 0 || counter > 0)
		{

			for (uint8_t i = 0; i <= counter; i++)
			{

				if (1 << TILT_FORWARD & counter2 || i < counter)
				{
					screen |= (uint32_t)pgm_read_byte_near(tilt_forward + i) << (8 * i); //* 4 --> 4 bytes per dword
				}
				if (1 << TILT_BACKWARD & counter2 || i < counter)
				{
					screen |= (uint32_t)pgm_read_byte_near(tilt_backward + i) << (8 * i); //* 4 --> 4 bytes per dword
				}
				if (1 << TILT_LEFT & counter2 || i < counter)
				{
					screen |= (uint32_t)pgm_read_byte_near(tilt_left + i) << (8 * i); //* 4 --> 4 bytes per dword
				}
				if (1 << TILT_RIGHT & counter2 || i < counter)
				{
					screen |= (uint32_t)pgm_read_byte_near(tilt_right + i) << (8 * i); //* 4 --> 4 bytes per dword
				}
			}
			ledDisp->setBinaryToDisplay(screen);
		}
		else
		{
			textBufToDisplay();
		}
	}

	// keep track of progress
	if (counter2 == 0x0F)
	{ //if a digit is complete
		counter++;
		if (counter == 4)
		{
			loadBuzzerTrack(SONG_DRYER_HAPPY);
			counter = 0;
		}
		counter2 = 0;
	}
}

void Apps::modeGeiger(bool init)
{

	if (init)
	{
		//textBuf[3]=' ';
		COUNTER_GEIGER = 0;
		GEIGER_TONE_FREQUENY_LOWEST = 2000;
		GEIGER_TONE_FREQUENCY_HEIGHEST = 4000;
		GEIGER_TONE_LENGTH = 10;
	}

	//play tick.
	//wait random time.
	//X = - log(1 - Y)/ K   with Y a random value ( 0<Y<1) and K a constant ?
	long r = random(0, 1024) * random(0, 1024);
	//long r = random(0, 1024);
	//r = r*r;

	setBlankDisplay();

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT))
	{
		// note mode

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
		{
			//lower
			// if (encoder_dial->getValueChanged())
			// {
			// 	GEIGER_TONE_FREQUENY_LOWEST = encoder_dial->getValueMapped(0, 5000);
			// }
			
			GEIGER_TONE_FREQUENY_LOWEST = encoder_dial->getValueLimited(500,false) * 10;
			// checkBoundaries(&GEIGER_TONE_FREQUENY_LOWEST, 5000, 0, false);

			ledDisp->setNumberToDisplayAsDecimal(GEIGER_TONE_FREQUENY_LOWEST);
		}
		else if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
		{
			//upper
			// if (encoder_dial->getValueChanged())
			// {
			// 	GEIGER_TONE_FREQUENCY_HEIGHEST = encoder_dial->getValueMapped(0, 5000);
			// }
			GEIGER_TONE_FREQUENCY_HEIGHEST = encoder_dial->getValueLimited(500,false) * 10;
			// checkBoundaries(&GEIGER_TONE_FREQUENCY_HEIGHEST, 5000, 0, false);
			ledDisp->setNumberToDisplayAsDecimal(GEIGER_TONE_FREQUENCY_HEIGHEST);
		}
		else if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)))
		{
			//length
			// if (encoder_dial->getValueChanged())
			// {
			// 	GEIGER_TONE_LENGTH = encoder_dial->getValueMapped(0, 256);
			// }
			// GEIGER_TONE_LENGTH += encoder_dial->getValueChanged();
			GEIGER_TONE_LENGTH = encoder_dial->getValueLimited(255,false);
			//checkBoundaries(&GEIGER_TONE_LENGTH, 256, 0, false);
			
			ledDisp->setNumberToDisplayAsDecimal(GEIGER_TONE_LENGTH);
		}
		else if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		{
			if (encoder_dial->getValueChanged())
			{
				buzzer->playTone(
					encoder_dial->getValueLimited(500,false),
					(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA)) ? 0 : GEIGER_TONE_LENGTH);
			}
		}
		else
		{

			if (r > GEIGER_PROBABILITY_THRESHOLD)
			{ // 1024*1024
				long tmp = random(GEIGER_TONE_FREQUENY_LOWEST, GEIGER_TONE_FREQUENCY_HEIGHEST + 1);
				buzzer->playTone(
					tmp,
					(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA)) ? 0 : GEIGER_TONE_LENGTH);

				ledDisp->setNumberToDisplayAsDecimal(tmp);
				COUNTER_GEIGER++;
			}

			if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
			{
				ledDisp->setNumberToDisplayAsDecimal(COUNTER_GEIGER);
			}

			GEIGER_PROBABILITY_THRESHOLD = encoder_dial->getValueLimited(1024, false) * 1024;
		}
	}
	else
	{
		// simple Geiger mode
		// todo: idea: if tilted forward, dramatically increase chance, tilted backward, decrease. This way, we can truly simulate a geiger counte.r

		// If you press the button and approach an object, the object appears super radio active! hi-la-ri-ous!
		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		{
			// binaryInputs[SWITCH_TILT_FORWARD].getValue() ||
			// r *= 2; //
			GEIGER_INCREASE_CHANCE += 1000;
		}
		else
		{
			if (GEIGER_INCREASE_CHANCE > 0)
			{
				GEIGER_INCREASE_CHANCE -= 1500;
			}
		}

		r += GEIGER_INCREASE_CHANCE;

		// textBuf[0] = ' ';
		if (r > encoder_dial->getValueLimited(1024, false) * 1024)
		{
			//	addNoteToBuzzer(1); //not beep but "puck"
			buzzer->playTone((unsigned int)50, 10);
			setStandardTextToTextBuf(TEXT_RANDOM_SEGMENTS);
			textBufToDisplay();
		}
	}
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
	setBlankDisplay();

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

			buzzer->noteToDisplay(textHandle, decimalDotsHandle, SEQUENCER_TEMP_NOTE);
			showNote = true;

			// bonus effect: TRANSPOSE!

			//potentio->increaseSubtractAtChange((int16_t *)&(SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET), 1);
			SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET += encoder_dial->getValueChanged();
		}

		// if ((this->binaryInputsEdgeDown & (1<<BUTTON_INDEXED_MOMENTARY_0)))
		// {
		// 	ledDisp->setBinaryToDisplay(displayAllSegments);
		// }

		// just listen to the potentio note
		// SEQUENCER_TEMP_NOTE = encoder_dial->getValueMapped(0, 255);
		SEQUENCER_TEMP_NOTE += encoder_dial->getValueChanged();

		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
		{
			addNoteToBuzzer(SEQUENCER_TEMP_NOTE);
		}

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
		{
			buzzer->buzzerOff();
			// if button continuously pressed, rotate potentio to hear notes.
			if (encoder_dial->getValueChanged())
			{
				addNoteToBuzzer(SEQUENCER_TEMP_NOTE);
			}
			buzzer->noteToDisplay(textHandle, decimalDotsHandle, SEQUENCER_TEMP_NOTE);
			showNote = true;
		}

		// program note to song
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
		{

			uint8_t note = encoder_dial->getValueLimited(255, false);

			addNoteToBuzzer(note);

			if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT))
			{
				//copy to all measures
				for (uint8_t i = 0; i < 4; i++)
				{
					this->SEQUENCER_SONG[(SEQUENCER_STEP_COUNTER % 8) + 8 * i] = note;
				}
			}
			else
			{
				this->SEQUENCER_SONG[SEQUENCER_STEP_COUNTER] = note;
			}
		}

		// song progression
		if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
		{
			step = 1;
		}

		if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
		{
			// if (potentio->getValueChanged())
			// {
			// 	step = 2 * potentio->getLastStableValueChangedUp() - 1; //step +1 or -1
			// }
			//}
			//potentio->increaseSubtractAtChange((int16_t *)&(step), 1);
			step += encoder_dial->getValueChanged();

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

		// if music note needs to be shown
		// if (showNote)
		// {
		// 	//ledDisp->displaySetTextAndDecimalPoints(textBuf, &decimalPoints);
		// 	textBufToDisplay();
		// 	ledDisp->setDecimalPointsToDisplay(decimalPoints);
		// }

		// handle step change
		if (step != 0 || init)
		{
			SEQUENCER_STEP_COUNTER += step;

			if (SEQUENCER_STEP_COUNTER < 0)
			{
				SEQUENCER_STEP_COUNTER = 31;
			}

			if (SEQUENCER_STEP_COUNTER > 31)
			{
				SEQUENCER_STEP_COUNTER = 0;
			}

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
			ledDisp->setBinaryToDisplay(displayAllSegments);
		}
	}
}

void Apps::modeMetronome(bool init)
{
	// todo: with extra timer, create slight timing offset in second ticker, for fun effects (zwevingen)!
	bool update = false;

	if (init)
	{
		// initializing is silly!
		// METRONOME_TICKER_1_POSITION = 0;
		// METRONOME_TICKER_2_POSITION = 0;
		// METRONOME_TICKER_3_POSITION = 0;

		TIMER_METRONOME.setInitTimeMillis(-500);
		TIMER_METRONOME.start();
	}

	if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA))
	{
		dialOnEdgeChangeInitTimerPercentage(&TIMER_METRONOME);

		if (!TIMER_METRONOME.getTimeIsNegative())
		{
			TIMER_METRONOME.start();
			update = true;
		}
	}else{
		update = encoder_dial->getValueChanged() !=0;
	}

	if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
	{
		//ticker 1,2 and 3 back together (at position of ticker 1)
		METRONOME_TICKER_2_POSITION = METRONOME_TICKER_1_POSITION;
		METRONOME_TICKER_3_POSITION = METRONOME_TICKER_1_POSITION;
	}

	displayAllSegments = 0;
	bool forceNextStep = update || binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3);
	
	modeMetronomeTickerUpdate(&METRONOME_TICKER_2_POSITION, BUTTON_MOMENTARY_1, !(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_SMALL_RED_LEFT)), C6_4, forceNextStep);
	modeMetronomeTickerUpdate(&METRONOME_TICKER_3_POSITION, BUTTON_MOMENTARY_2, true, C5_4, forceNextStep);
	modeMetronomeTickerUpdate(&METRONOME_TICKER_1_POSITION, BUTTON_MOMENTARY_3, true, C7_8, update);
	
	ledDisp->setBlankDisplay();
	if (binaryInputsValue & (1<< BUTTON_INDEXED_LATCHING_SMALL_RED_RIGHT)){
		// bpm --> full 12 step circles per minute.   timing is per step. so: 60bpm == 1 circle / second = timer: 1000/12 = 83.333ms/step
		ledDisp->setNumberToDisplayAsDecimal(  (int16_t) (1.4388* (float)TIMER_METRONOME.getInitTimeMillis() ) + 180);

	}else{
		ledDisp->setBinaryToDisplay(displayAllSegments);
	}

}

void Apps::modeMetronomeTickerUpdate(uint8_t *ticker_counter, uint8_t momentary_id, bool direction, uint8_t sound_at_zero_pass, boolean force_step)
{
	// every ticker gets updated.

	// check for next step
	if (binaryInputs[momentary_id].getEdgeUp() || force_step)
	{

		int16_t ttmp = (int16_t)*ticker_counter;
		this->nextStepRotate(
			&ttmp,
			direction,
			0,
			11);

		*ticker_counter = (uint8_t)ttmp;

		if (*ticker_counter == 0)
		{
			addNoteToBuzzer(sound_at_zero_pass);
		}
	}

	// // update screen, every cycle.
	// for (uint8_t i = 0; i < 4; i++)
	// {
	// 	displayAllSegments |= (uint32_t)pgm_read_byte_near(disp_4digits_animate_circle + *ticker_counter * 4 + (i)) << (8 * i); //* 4 --> 4 bytes per dword
	// }

	ledDisp->progmemToDisplayBuffer(&displayAllSegments, disp_4digits_animate_circle + *ticker_counter * 4);
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

	if (!(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_EXTRA)) || init)
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

	//   // check if a momentary button was pressed, and create byte with status: 0000 is no button pressed.  0001, 0010, 0100, 1000
	//   uint8_t buttonsChanged = 0;
	//   for (int k = 0; k <  MOMENTARY_BUTTONS_COUNT; ++k) {
	// 	buttonsChanged |= (binaryInputs[buttons_indexed[k]].getEdgeUp() << k);
	//   }
	uint8_t pressed_momentary_button = SIMON_NO_BUTTON_PRESSED;
	for (int k = 0; k < MOMENTARY_BUTTONS_COUNT; ++k)
	{
		if (binaryInputs[buttons_indexed[k]].getEdgeUp())
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
		if (encoder_dial->getValueChanged())
		{
			SIMON_PLAYERS_COUNT = encoder_dial->getValueMapped(SIMON_MAX_PLAYERS - 1, false) + 1; // start counting from player 1 to display
		}

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

bool Apps::nextStepRotate(int16_t *counter, bool countUpElseDown, int16_t minValue, int16_t maxValue)
{
	int16_t original_value;
	*counter += -1 + (2 * countUpElseDown);

	checkBoundaries(counter, minValue, maxValue, true);
	return original_value != *counter;
	// return false;
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

	setBlankDisplay();

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
		if (encoder_dial->getValueChanged())
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
		// Serial.println(REACTION_GAME_HEX_ACTIVE_DIGIT);
		
		// //attempt to optimization, but with a bug, and too tired. So, give it a shot! 
		// REACTION_GAME_HEX_VALUE_TO_FIND = (byte)textBuf[REACTION_GAME_HEX_ACTIVE_DIGIT];
		// //Serial.println(REACTION_GAME_HEX_VALUE_TO_FIND);

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
		ledDisp->setBinaryToDisplay(displayAllSegments);
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
			ledDisp->progmemToDisplayBuffer(&displayAllSegments, disp_4digits_animate_circle + step * 4);
		}
		ledDisp->setBinaryToDisplay(displayAllSegments);

		if (!EXTRA_OPTION_REACTION_SOUND_MODE_GUITAR_HEX_HERO || REACTION_GAME_LEVEL == 0)
		{
			// in the easy level of sound mode, we show the lights.

			// show decimal point for digit corresponding with button
			ledDisp->setDecimalPointToDisplay(true, REACTION_GAME_TARGET);
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

	// load/save songs
	//blink alternatively song number and "load" or "save"

	encoder_dial->setRange(slotCount, false); //todo: minimum should be "1"
	encoder_dial->setSensitivity(10);
	uint8_t slot_number = encoder_dial->getValue();
	
	// uint8_t slot_number = encoder_dial->getValueMapped(1, slotCount);
	if (SAVE_LOAD_MENU_BLINK_TIMER.getInFirstGivenHundredsPartOfSecond(500))
	{
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

void Apps::textBufToDisplay(){
	ledDisp->setTextBufToDisplay(textBuf);
}

void Apps::addNoteToBuzzer(uint8_t note){
	buzzer->programBuzzerRoll(note);
}

void Apps::addNoteToBuzzerRepeated(uint8_t note, uint8_t repeater){
	for (uint8_t i=0;i<repeater;i++){
        this->addNoteToBuzzer(note);
    }
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