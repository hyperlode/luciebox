#include "Apps.h"
#include "PretbakSettings.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
//		INIT AND ADMINISTRATION
// ------------------------------------------------------------------------------------------------------------------------------------------------------

Apps::Apps(){};

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
void Apps::setPeripherals(BinaryInput binaryInputs[], RotaryEncoderDial *encoder_dial, DisplayManagement *ledDisp, LedMultiplexer5x8 *allLights, Buzzer *buzzer, PotentioSelector *selectorDial)
{
    this->buzzer = buzzer;
    this->binaryInputs = binaryInputs;
    this->encoder_dial = encoder_dial;
    this->ledDisp = ledDisp;
    this->allLights = allLights;
    this->selectorDial = selectorDial;

    textHandle = ledDisp->getDisplayTextBufHandle();
    decimalDotsHandle = ledDisp->getDecimalPointsHandle();
    inactivity_timer.setInitCountDownTimeSecs(indexToTimeSeconds(INACTIVITY_TIME_BEEP_INDEX));
    resetInactivityTimer();
    always_on_timer.start();
}
#else
void Apps::setPeripherals(BinaryInput binaryInputs[], RotaryEncoderDial *encoder_dial, DisplayManagement *ledDisp, LedMultiplexer5x8 *allLights, Buzzer *buzzer)
{
    this->buzzer = buzzer;
    this->binaryInputs = binaryInputs;
    this->encoder_dial = encoder_dial;
    this->ledDisp = ledDisp;
    this->allLights = allLights;

    textHandle = ledDisp->getDisplayTextBufHandle();
    decimalDotsHandle = ledDisp->getDecimalPointsHandle();
    inactivity_timer.setInitCountDownTimeSecs(indexToTimeSeconds(INACTIVITY_TIME_BEEP_INDEX));
    resetInactivityTimer();
    always_on_timer.start();

    appState = appStateInit;
}

#endif

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
void Apps::appStateLoop()
{
    updateEveryAppCycleBefore();
    bool selector_changed = selectorDial->getValueChangedEdge();
    int selector_dial_value = selectorDial->getSelectorValue() - 1; // -1 because 13 resistor values for 12 pos knob, gnd is never switchted.

    bool shift_changed = (binaryInputsEdgeUp | binaryInputsEdgeDown) & (1 << BUTTON_INDEXED_LATCHING_0); // latching button acts as a "shift button" to have two apps per selector location

    // uint8_t selected_app = selector_dial_value * 2 + ((binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_0)) > 0);

    bool init_app_init = (selector_changed && (SETTINGS_MODE_SELECTOR != 10)) || (shift_changed && (selector_dial_value != 0)); // settings mode have no dual app available. So, don't init if shift button is toggled.  //&& (selector_dial_value != 11) and multitimer app

    this->app_init_edge = false;

    if (init_app_init)
    {
#ifdef ENABLE_SERIAL
        Serial.println("app select:");
        Serial.println(selected_app);
#endif

        buzzerSilentClearBuffer(); // The moment we switch apps, the sound goes off immediatly.

        // set to init state before a new app starts
        splash_screen_playing = true;
    }

    if (splash_screen_playing)
    {
        // do init routine (showing splash screen), if finished,end of init. Then continue to init of the chosen application
        splash_screen_playing = this->init_app(init_app_init, selector_dial_value);
        if (!splash_screen_playing)
        {
            this->initializeAppDataToDefault();
            this->app_init_edge = true;
            selected_app = selector_dial_value * 2 + ((binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_0)) > 0);
        }
    }

    // not as else statement, to have the init properly transferred after app beginning screen.
    if (!splash_screen_playing)
    {
        appSelector();
    }

    inactivityHandler();

    updateEveryAppCycleAfter();
}

#else

void Apps::appStateLoop()
{
    updateEveryAppCycleBefore();

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
    bool selector_changed = selectorDial->getValueChangedEdge();
    int selector_dial_value = selectorDial->getSelectorValue() - 1; // -1 because 13 resistor values for 12 pos knob, gnd is never switchted.

#endif
    switch (appState)
    {

    case appStateInit:
    {
        this->selected_app = 1; // start with one, which also works for the non selector button setting

#ifdef ENABLE_EEPROM
#ifndef ENABLE_SELECT_APPS_WITH_SELECTOR
        // memorize active app
        this->selected_app = eeprom_read_byte(
            (uint8_t *)EEPROM_LUCIEBOX_ACTIVE_APP_AT_SHUTDOWN);
#endif
#endif
        appState = appSplashInit;
        selected_app_memory = selected_app;
        break;
    }

    case appSplashInit:
    {
#ifdef ENABLE_SERIAL
        Serial.println("app select:");
        Serial.println(selected_app);
#endif

        buzzerSilentClearBuffer(); // The moment we switch apps, the sound goes off immediatly.

        this->init_app(true, selected_app - 1); // selected app -1 because we start from 1
        // set to init state before a new app starts
        appState = appSplash;
        break;
    }

    case appSelection:
    {
        
        // switch off. shut down
        if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0))) 
        {
                autoShutdown();
        }

        if (!(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_0)))
        {
            // switch off. shut down
            if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0))) 
            {
                    autoShutdown();
                
            }

            if (selected_app == selected_app_memory)
            {
                appState = appRunning;
            }
            else
            {
                appState = appSplashInit;
            }
            selected_app_memory = selected_app;
        }

        modifyValueUpDownWithMomentary2And3(&selected_app);
        selected_app += encoder_dial->getDelta();
        checkBoundaries(&selected_app, 1, 22, true);

        this->displayAllSegments = 0;
        flashPictureToDisplayAllSegments(app_splash_screens + (selected_app - 1) * 4);
        displayAllSegmentsToScreen();
        //ledDisp->setNumberToDisplayAsDecimal(selected_app);

#ifndef ENABLE_SELECT_APPS_WITH_SELECTOR
        // latching button lights off at app selection, whatever their toggle state is (but we do not want to lose the toggle state)
        for (uint8_t i = 5; i < 8; i++)
        {
            lights &= ~(1 << lights_indexed[i]);
        }
#endif

      

        break;
    }

    case appSplash:
    {

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
        // do init routine (showing splash screen), if finished,end of init. Then continue to init of the chosen application
        splash_screen_playing = this->init_app(false, selector_dial_value);
        if (!splash_screen_playing)
        {
            // this->app_init_edge = true;
            appState = appInit;
            selected_app = selector_dial_value * 2 + ((binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_0)) > 0);
        }
#else
        // do init routine (showing splash screen), if finished,end of init. Then continue to init of the chosen application
        splash_screen_playing = this->init_app(false, selected_app - 1); // selected app -1 because we start from 1
        if (!splash_screen_playing)
        {
            appState = appInit;
       
        }
        
        if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_0))
        {
            appState = appSelection;
        }
#endif

        break;
    }

    case appInit:
    {
        this->initializeAppDataToDefault();
        this->app_init_edge = true;
        appState = appRunning;
        break;
    }

    case appRunning:
    {
        // not as else statement, to have the init properly transferred after app beginning screen.
        appSelector();
        this->app_init_edge = false; // global variable (saves memory) true at first run (set in another state )

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR

        bool shift_changed = (binaryInputsEdgeUp | binaryInputsEdgeDown) & (1 << BUTTON_INDEXED_LATCHING_0); // latching button acts as a "shift button" to have two apps per selector location

        // uint8_t selected_app = selector_dial_value * 2 + ((binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_0)) > 0);

        bool init_app_init = (selector_changed && (SETTINGS_MODE_SELECTOR != 10)) || (shift_changed && (selector_dial_value != 0)); // settings mode have no dual app available. So, don't init if shift button is toggled OR if we're visualizing the selector switch analog value in the settings app

        if (init_app_init)
        {
            appState = appSplashInit;
        }

#else
        if (binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_0))
        {
            appState = appSelection;
        }

#endif

        break;
    }
    }
    inactivityHandler();
    updateEveryAppCycleAfter();
}

#endif

void Apps::inactivityHandler()
{

    // for some timing aps, we want this watchdog timer to be disabled.
    // It's easily accomplished by resetting the inactivity timer in those apps continuously.

    // for every button press, the watchdog timer is reset
    if (binaryInputsEdgeUp)
    {
        resetInactivityTimer();
    }

    // watchdog timer elapsed
    if (inactivity_timer.getCountDownTimerElapsedAndRestart())
    {
        autoShutdown();
    }
}

void Apps::autoShutdown()
{
    // will shutdown if available and play an "I'm still on" sound if not.

#ifdef ENABLE_SOFT_POWER_OFF
    // auto power off

    uint8_t enable_auto_power_off = true;
#ifdef ENABLE_EEPROM
    enable_auto_power_off = !eeprom_read_byte((uint8_t *)EEPROM_LUCIEBOX_AUTO_POWER_OFF_DISABLED);
#endif
    if (enable_auto_power_off)
    {
        // memorize active app
        eeprom_update_byte(
            (uint8_t *)EEPROM_LUCIEBOX_ACTIVE_APP_AT_SHUTDOWN,
            selected_app);

        digitalWrite(PIN_POWER_ON_HOLD, LOW);
        digitalWrite(PIN_SELECTOR_BUTTON, LOW); // pull down input to enable transistor base to go low

        //normally, the device is switched off at this point. IF however, it's powered through an external device bypassing the auto power off mode, it should keep on working.

        for (uint16_t i=0;i<26000;i++){
            //fadeInList(MODE_DREAMTIME_STEP, 0, MODE_DREAMTIME_RANDOM_LIST);
            resetInactivityTimer();

        }

        //delay(100);
        digitalWrite(PIN_POWER_ON_HOLD, HIGH);
        digitalWrite(PIN_SELECTOR_BUTTON, HIGH);
    }
    else
    {
        playSongHappyDryer();
    }

#else
    playSongHappyDryer();
#endif
}

void Apps::appSelector()
{
    //#ifdef FUNCTION_POINTER_APP_SELECTION // problem: takes more memory than switch-case. AND init and initOnBigLatchInitToo not good. The solution would be to have all the apps without advanced init bundled together, and from certain selector value onwards and up, use "init"

    switch (selected_app)
    {
    case APP_SELECTOR_LETTERS_AND_CHARS:

        this->modeCountingLettersAndChars();
        break;

    case APP_SELECTOR_SIMON:
#ifdef ENABLE_SIMON_APP
        this->modeSimon();
#endif
        break;

    case APP_SELECTOR_SOUND_NOTES:
        this->modeSoundNotes();
        break;

    case APP_SELECTOR_SOUND_COMPOSER:
        this->modeComposeSong();
        break;

    case APP_SELECTOR_STOPWATCH:
        stopwatch();
        break;

    case APP_SELECTOR_POMODORO:
#ifdef ENABLE_POMODORO
        pomodoroTimer();
#endif
        break;

    case APP_SELECTOR_RANDOMWORLD:
        this->modeRandomWorld();
        break;

    case APP_SELECTOR_TALLY_KEEPER:
#ifdef ENABLE_TALLY_KEEPER
        this->modeTallyKeeper();
#endif
        break;

    case APP_SELECTOR_GEIGER:
        this->modeGeiger();
        break;

    case APP_SELECTOR_HACK_TIME:
        this->modeHackTime();
        break;

    case APP_SELECTOR_SOUND_SONG:
        this->modeSoundSong();
        break;

    case APP_SELECTOR_DRAW:
        this->draw();
        break;

    case APP_SELECTOR_DRAW_GAME:
        this->drawGame();
        break;

    case APP_SELECTOR_MOVIE_MODE:
        this->modeMovie();
        break;

    case APP_SELECTOR_SETTING:
    case APP_SELECTOR_SETTING_TOO:
        this->modeSettings();
        break;

    case APP_SELECTOR_SOUND_METRONOME:
        this->modeMetronome();
        break;

    case APP_SELECTOR_SOUND_SEQUENCER:
        this->modeSequencer();
        break;

    case APP_SELECTOR_REACTION_GAME:
    case APP_SELECTOR_GUITAR_HERO:
#ifdef ENABLE_REACTION_APP
        this->modeReactionGame();
#endif
        break;

#ifdef ENABLE_TILT_APP
    case APP_SELECTOR_DREAMTIME:
        this->modeDreamtime();
        break;
    case APP_SELECTOR_TILT:
        this->tiltSwitchTest();
        break;
#else
    case APP_SELECTOR_DREAMTIME:
    case APP_SELECTOR_DREAMTIME_TOO:
        this->modeDreamtime();
        break;
#endif

#ifdef ENABLE_QUIZ_MASTER
    case APP_SELECTOR_QUIZ_MASTER:
        this->quiz();
        break;
#endif

    case APP_SELECTOR_MULTITIMER:
#ifdef ENABLE_MULTITIMER_STANDALONE_DEPRECATED
        this->miniMultiTimer();
#elif defined ENABLE_MULTITIMER_INTEGRATED
        this->multitimer_integrated();
#endif
        break;

    default:
        break;
    }
}

void Apps::resetInactivityTimer()
{
    inactivity_timer.start();
}

void Apps::updateEveryAppCycleBefore()
{
    binaryInputsEdgeUp = 0;
    binaryInputsEdgeDown = 0;
    binaryInputsValue = 0;
    binaryInputsToggleValue = 0;

    // it's important to realize the display is reset before every cycle. Do not forget to update it all the time.
    lights = 0x00;

    // load quick access bytes.
    for (uint8_t i = 0; i < 8; i++)
    {
        binaryInputsEdgeUp |= binaryInputs[buttons_indexed[i]].getEdgeUp() << i;
        binaryInputsEdgeDown |= binaryInputs[buttons_indexed[i]].getEdgeDown() << i;
        binaryInputsValue |= binaryInputs[buttons_indexed[i]].getValue() << i;
        binaryInputsToggleValue |= binaryInputs[buttons_indexed[i]].getToggleValue() << i;

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
        lights |= binaryInputs[buttons_indexed[i]].getValue() << lights_indexed[i];

#else
        // by default: button lights on if activated
        if (i < 5)
        {
            // 5 instead of 4 because BUTTON_INDEXED_LATCHING_0 is standard also only on if pressed.
            lights |= binaryInputs[buttons_indexed[i]].getValue() << lights_indexed[i];
        }
        else
        {
            lights |= binaryInputs[buttons_indexed[i]].getToggleValue() << lights_indexed[i];
        }
#endif
    }
    setBlankDisplay();
}

void Apps::updateEveryAppCycleAfter()
{
    // setLedArray();
    ledDisp->setLedArray(lights);
}

void Apps::initializeAppDataToDefault()
{
    // allLights->setBrightness(0, false); // disable because it annoys me:)

    initiateCountDowntimerWith500Millis(&general_timer);

    //buzzer  (buzzer off at init of splash screen)
    buzzer->setSpeedRatio(2);
    buzzer->setTranspose(0);

    //encoder
    encoder_dial->setRange(1023, true); // allow overflow. (this makes it different from the limited potentio.)
    encoder_dial->setValue(0);

    fill8BytesArrayWithZero();

    randomSeed(millis());

    encoder_dial->setSensitivity(4);

    // all shared variables to zero or false

    general_boolean = 0;
    general_boolean2 = 0;
    general_boolean3 = 0;
    general_boolean4 = 0;
    general_int16_t_1 = 0;
    general_int16_t_2 = 0;
    general_int16_t_3 = 0;
    general_int16_t_4 = 0;
    general_uint16_t_1 = 0;
    general_uint16_t_2 = 0;
    general_uint8_t_1 = 0;
    general_uint8_t_2 = 0;
    general_uint8_t_3 = 0;
    general_uint8_t_4 = 0;
    general_long_1 = 0;
    general_long_2 = 0;

    // simulated latching buttons reset. This is the great advantage of not having latching buttons. The app starts in a truly blank state when initialized.
    for (uint8_t i = 0; i < 8; i++)
    {
        binaryInputs[buttons_indexed[i]].setToggleValue(0);
    }
}

bool Apps::init_app(bool init, uint8_t selector)
{
    // shows animated introduction splash screen at each main app startup
    // returns true as long as it is busy

    if (init)
    {
        // init of the init_app..
        this->displayAllSegments = 0;
        flashPictureToDisplayAllSegments(app_splash_screens + selector * 4);

        // initialize list
        randomSequence(FADE_IN_RANDOM_LIST, 32);

        INIT_SPLASH_ANIMATION_STEP = 0;
        INIT_SPLASH_LIGHTS_STEP = 0;

        this->TIMER_INIT_APP.start(-20);
    }

    // advance one frame
    if (this->TIMER_INIT_APP.getCountDownTimerElapsedAndRestart())
    {
        INIT_SPLASH_LIGHTS_STEP++;
        INIT_SPLASH_ANIMATION_STEP++;
    }

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
    if (INIT_SPLASH_ANIMATION_STEP < 5)
    {
        lights = 0xff;
        // all lights on
        ledDisp->setBinaryToDisplay(0xFFFFFFFF); // use fade in as fade out to set text.
    }
    else
#endif
        if (INIT_SPLASH_ANIMATION_STEP < 23)
    {
        // show app splash screen
        ledDisp->setBinaryToDisplay(this->displayAllSegments);

        //ledDisp->setNumberToDisplayAsDecimal(SETTINGS_MODE_SELECTOR);
        //textBufToDisplay();

        lights = 0xff;
    }
    else if (INIT_SPLASH_ANIMATION_STEP < 55)
    {
        // fade out
        ledDisp->setBinaryToDisplay(~this->fadeInList(INIT_SPLASH_ANIMATION_STEP - 24, ~this->displayAllSegments, this->FADE_IN_RANDOM_LIST));
        INIT_APP_LIGHTS_COUNTER = ((32 - (INIT_SPLASH_ANIMATION_STEP - 22)) / 4);
        for (uint8_t i = 0; i <= INIT_APP_LIGHTS_COUNTER; i++)
        {
            lights |= 0x1 << lights_indexed_as_installed[i];
        }
    }
    else
    {
        return false;
    }

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
    // hold down momentary button to freeze app splash screen (easier to rotate knob to search for desired app)
    byte momentary_buttons_mask = 1 << BUTTON_INDEXED_MOMENTARY_0 | 1 << BUTTON_INDEXED_MOMENTARY_1 | 1 << BUTTON_INDEXED_MOMENTARY_2 | 1 << BUTTON_INDEXED_MOMENTARY_3;
    if ((binaryInputsValue & momentary_buttons_mask) > 0)
    {
        INIT_SPLASH_ANIMATION_STEP = 6;
        lights = 0x00;
        lights |= 0x1 << lights_indexed_as_installed[(INIT_SPLASH_LIGHTS_STEP % 32) / 4];
        // lights	|= 0x1 << lights_indexed_as_installed[((INIT_SPLASH_LIGHTS_STEP+16) %32)/4];
    }
#endif

    return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
//		ACTUAL APPS
// ------------------------------------------------------------------------------------------------------------------------------------------------------

void Apps::modeDreamtime()
{
    // baby activity for those who can not yet rotate the selector dial. (the shift button makes it stay in the same app).

    if (this->app_init_edge)
    {
        MODE_DREAMTIME_FADE_IN_ELSE_FADE_OUT = true;
        MODE_DREAMTIME_STEP = random(3, 31); // let's not start with an empty screen
    }

    modifyValueUpDownWithMomentary2And3(&MODE_DREAMTIME_STEP);

    if (!(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3)))
    {
        if (this->TIMER_DREAMTIME.getCountDownTimerElapsedAndRestart())
        {
            MODE_DREAMTIME_STEP++;
        }

        dialOnEdgeChangeInitTimerPercentage(&TIMER_DREAMTIME);
    }
    else
    {
        MODE_DREAMTIME_STEP += encoder_dial->getDelta();
    }

    if (!checkBoundaries(&MODE_DREAMTIME_STEP, 0, 31, true))
    {
        MODE_DREAMTIME_FADE_IN_ELSE_FADE_OUT = !MODE_DREAMTIME_FADE_IN_ELSE_FADE_OUT;
        randomSequence(MODE_DREAMTIME_RANDOM_LIST, 32);
    }

    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_LATCHING_1))
    {
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1))
        {
            loadBuzzerTrack(SONG_ALPHABET);
        }
        else
        {

            playSongHappyDryer();
        }
    }

    if (!(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2)))
    {
        if (MODE_DREAMTIME_STEP_MEMORY != MODE_DREAMTIME_STEP)
        {
            buzzerSilentClearBufferAndAddNote(MODE_DREAMTIME_RANDOM_LIST[MODE_DREAMTIME_STEP]); //60
        }
    }

    uint32_t display_binary = fadeInList(MODE_DREAMTIME_STEP, 0, MODE_DREAMTIME_RANDOM_LIST);

    MODE_DREAMTIME_STEP_MEMORY = MODE_DREAMTIME_STEP;

    if (!MODE_DREAMTIME_FADE_IN_ELSE_FADE_OUT)
    {
        display_binary = ~display_binary;
    }

    if (binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1))
    {
        display_binary = 0xFFFFFFFF;
        buzzerPlayApproval();
    }

    if (binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0))
    {
        display_binary = 0x795E3F38; // self glorification time.
    }

    ledDisp->setBinaryToDisplay(display_binary);
}

#ifdef ENABLE_POMODORO

void Apps::pomodoroTimer()
{
    uint8_t display_mode = POMODORO_DISPLAY_TIMER;

#ifdef POMODORO_ENABLE_HOURGLASS
    this->displayAllSegments = 0;
#endif

    if (encoder_dial->getDelta() != 0)
    {
        set_blink_offset();
    }

    if (this->app_init_edge)
    {
#ifdef ENABLE_EEPROM
        POMODORO_MAIN_CLOCK_TIME_INDEX = eeprom_read_byte((uint8_t *)EEPROM_POMODORO_INIT_TIME_INDEX);
        POMODORO_PAUSE_TIME_INDEX = eeprom_read_byte((uint8_t *)EEPROM_POMODORO_PAUSE_TIME_INDEX);
        POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX = eeprom_read_byte((uint8_t *)EEPROM_POMODORO_RND_BEEP_TIME_INDEX);
#else
        POMODORO_MAIN_CLOCK_TIME_INDEX = POMODORO_INIT_DEFAULT_TIME_INDEX;
        POMODORO_PAUSE_TIME_INDEX = POMODORO_PAUSE_DEFAULT_TIME_INDEX;
        POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX = POMODORO_PROBABILITY_BEEP_INTERVAL_DEFAULT_TIME_INDEX;
#endif
    }

    POMODORO_AUTO_RESTART_ENABLED = binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1);
    //#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
    POMODORO_TIMER_TICKING = binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3);

    //#endif
    // in main menu or timing? (run main menu at least once at init. Even when start button started) to initialize variables depending on settings latching buttons

    if (POMODORO_TIMER_TICKING && !app_init_edge)
    {
        // never auto power off when timer is on
        resetInactivityTimer();

        // pomodoro timer running

        if (!POMODORO_FIRST_TICKING_CYCLING_DONE)
        {
            POMODORO_FIRST_TICKING_CYCLING_DONE = true;
            POMODORO_IN_BREAK = false;
            // just started timing
            POMODORO_TIMER.start();
#ifdef ENABLE_EEPROM
            eeprom_update_byte((uint8_t *)EEPROM_POMODORO_INIT_TIME_INDEX, POMODORO_MAIN_CLOCK_TIME_INDEX);
            eeprom_update_byte((uint8_t *)EEPROM_POMODORO_PAUSE_TIME_INDEX, POMODORO_PAUSE_TIME_INDEX);
            eeprom_update_byte((uint8_t *)EEPROM_POMODORO_RND_BEEP_TIME_INDEX, POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX);
#endif
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
        // uint8_t tick_duration = encoder_dial->getValueLimited(160, false) / 2; // was 40, but set to 80 for less sensitivity. do again divide by four to limit options.
        // tick_duration += encoder
        stepChange(&POMODORO_SOUND, encoder_dial->getDelta(), 0, 80, false);
        bool tick_twice_a_second = POMODORO_SOUND > 40;
        // tick_duration = tick_duration % 40;

        if (POMODORO_TIMER.getEdgeSinceLastCallFirstGivenHundredsPartOfSecond(500, true, tick_twice_a_second))
        {
#ifdef POMODORO_ENABLE_HOURGLASS
            POMODORO_VISUAL_TIMER_PROGRESS =
                (uint8_t)((
                    (17 * POMODORO_TIMER.getTimeMillis()) / POMODORO_TIMER.getInitTimeMillis())

                );
#ifdef ENABLE_SERIAL
            Serial.println(POMODORO_VISUAL_TIMER_PROGRESS);
            Serial.println(this->displayAllSegments, BIN);
#endif
#endif

            if (POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX > 0)
            {
                if (random(0, indexToTimeSeconds(POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX) * 2) <= 1 - (tick_twice_a_second))
                {
                    // random has 0 included. as we have to take into account the double ticking,
                    // calculate the probability in half a seconds. i.e.  one every minute: (0,120)
                    // addNoteToBuzzer(C5_1);
                    // buzzerPlayApproval();
                    buzzerPlayDisappointment();
                };
            }

            // no sound when zero
            if (buzzer->getBuzzerNotesBufferEmpty()) // if end of clock signal sounds, no ticking! erase to optimize memory
            {
                buzzer->playTone(500, 1 + (unsigned long)POMODORO_SOUND % 40); // works well
            }
        }

        // timer
        if (getCountDownTimerHasElapsed(&POMODORO_TIMER))
        {
            POMODORO_IN_BREAK = !POMODORO_IN_BREAK;
            if (POMODORO_IN_BREAK)
            {
                // finished main pomodoro

                playSongHappyDryer();
                if (POMODORO_AUTO_RESTART_ENABLED)
                {
                    POMODORO_TIMER.setInitCountDownTimeSecs(indexToTimeSeconds(POMODORO_PAUSE_TIME_INDEX));
                    POMODORO_TIMER.start();
                }
                else
                {

#ifndef ENABLE_SELECT_APPS_WITH_SELECTOR
                    // with latching buttons, we need to unlatch the button physically. But, with momentary buttons, we can unlatch the button with software
                    binaryInputs[BUTTON_LATCHING_3].setToggleValue(0);

#endif
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

        // inviting lights to press the buttons
        lights |= 1 << LIGHT_MOMENTARY_2;
        lights |= 1 << LIGHT_MOMENTARY_3;
    }
    else
    {
        // in main menu
        POMODORO_FIRST_TICKING_CYCLING_DONE = false;
        POMODORO_TIMER.reset();
        POMODORO_TIMER.setInitCountDownTimeSecs(indexToTimeSeconds(POMODORO_MAIN_CLOCK_TIME_INDEX));

        // set timer up for change
        int16_t *active_seconds_modifier = &POMODORO_MAIN_CLOCK_TIME_INDEX; // normal time setting is default option
        if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
        {
            active_seconds_modifier = &POMODORO_PAUSE_TIME_INDEX;
            display_mode = POMODORO_DISPLAY_PAUSE_INIT_SECS;
        }
        else if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
        {
            active_seconds_modifier = &POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX;
            display_mode = POMODORO_DISPLAY_BEEP_PROBABILITY;
        }

        // look for value change with encoder dial
        encoderDialRefreshTimeIndex(active_seconds_modifier);

        // show performance results
        if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)))
        {
            display_mode = POMODORO_DISPLAY_SHOW_BAD;
        }
        if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
        {
            display_mode = POMODORO_DISPLAY_SHOW_GOOD;
        }

        // blinking latching button invites user to start
        if (millis_half_second_period())
        {
            lights |= 1 << LIGHT_LATCHING_3;
        }
    }
#ifdef POMODORO_ENABLE_HOURGLASS
    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
    {
        display_mode = POMODORO_DISPLAY_TIMER_HOURGLASS;
    }
#endif

    // display
    switch (display_mode)
    {
    case POMODORO_DISPLAY_TIMER_HOURGLASS:
    {
        setStandardTextToTextBuf(TEXT_SPACES);
#ifdef POMODORO_ENABLE_HOURGLASS
        // always set to buffer. later on it's decided if it's displayed or not.
        // for (uint8_t i=0;i<POMODORO_VISUAL_TIMER_PROGRESS;i++)
        // {
        // 	this->displayAllSegments |= 1UL << i;
        // }
        // this->displayAllSegments &= ~(1UL << 15); // keep seconds blinker spot clear
        // setBlankDisplay();
        flashPictureToDisplayAllSegments(disp_4digits_animations + FADE_IN_OFFSET + (POMODORO_VISUAL_TIMER_PROGRESS)*4);
#endif
    }
    break;

    case POMODORO_DISPLAY_TIMER:
    {
        POMODORO_TIMER.getTimeString(textHandle);
        if (POMODORO_IN_BREAK)
        {
            setStandardTextToTextBuf(TEXT_PAUS);
        }
        else
        {
            POMODORO_TIMER.getTimeString(textBuf);
        }
    }
    break;

    case POMODORO_DISPLAY_PAUSE_INIT_SECS:
    {
        timeSecondsToClockString(textHandle, indexToTimeSeconds(POMODORO_PAUSE_TIME_INDEX));
        setStandardTextToTextBuf(TEXT_PAUS);
    }
    break;

    case POMODORO_DISPLAY_BEEP_PROBABILITY:
    {
        timeSecondsToClockString(textHandle, indexToTimeSeconds(POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX));
        setStandardTextToTextBuf(TEXT_RANDOM_BEEP);
    }
    break;

    case POMODORO_DISPLAY_SHOW_GOOD:
    {
        setStandardTextToTextBuf(TEXT_YES);
        ledDisp->numberToBufAsDecimal(textHandle, POMODORO_STATS_WORKING_GOOD);
    }
    break;

    case POMODORO_DISPLAY_SHOW_BAD:
    {
        setStandardTextToTextBuf(TEXT_NO);
        ledDisp->numberToBufAsDecimal(textHandle, POMODORO_STATS_WORKING_BAD);
    }
    break;

    default:
        break;
    }

    displayTimerSecondsBlinker(&POMODORO_TIMER);

    // leds
    if (POMODORO_PROBABILITY_BEEP_INTERVAL_INDEX > 0)
    {
        lights |= 1 << LIGHT_MOMENTARY_1;
    }

#ifdef POMODORO_ENABLE_HOURGLASS
    // if (POMODORO_ENABLE_HOURGLASS_VISUALS){
    displayAllSegmentsToScreen();

    // }else{
    if (millis_blink_250_750ms())
    {
        textBufToDisplay();
    }
    // }

#else
    if (millis_blink_250_750ms())
    {
        textBufToDisplay();
    }
#endif
}
#endif

void Apps::encoderDialRefreshTimeIndex(int16_t *indexHolder)
{
    // actually change the set up timer

    stepChange(indexHolder, encoder_dial->getDelta(), 0, 90, false);
}

void Apps::resetStopwatch(SuperTimer *pTimer)
{
    pTimer->setInitTimeMillis(0);
    pTimer->reset();
    pTimer->startPaused(true);
}

void Apps::stopwatch()
{
    // classic stopwatch
    long time_millis = 0;
    uint8_t timeDisplayShift = 0;

    if (this->app_init_edge)
    {
        // STOPWATCH_LAP_MEMORY_2 = 0;

        resetStopwatch(&STOPWATCH_CHRONO_2);

        STOPWATCH_CHRONO_1.setInitTimeMillis(always_on_timer.getTimeMillis());
        STOPWATCH_CHRONO_1.start();

        //always_on_timer.getTimeString(textHandle);
        //displayTimerSecondsBlinker(&always_on_timer);
    }

    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
    {
        pSsuperTimer = &STOPWATCH_CHRONO_1;
        pLongValue = &STOPWATCH_LAP_MEMORY_1;
    }
    else
    {
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

    if (pSsuperTimer->getIsStarted() && !paused)
    {
        // never auto power off when timer is on
        resetInactivityTimer();
    }

    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
    {
        // set chronometer to zero
        pSsuperTimer->setInitTimeMillis(0);
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

    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
    {
        // normal clock with minutes and seconds
        pSsuperTimer->getTimeString(textBuf);
        // setDecimalPoint(pSsuperTimer->getSecondsBlinker(), 1);
        displayTimerSecondsBlinker(pSsuperTimer);
    }
    else
    {
        // stopwatch. Accuracy depending on total time
        timeDisplayShift = 0;
        while (time_millis > 9999)
        {
            timeDisplayShift++;
            time_millis /= 10;
        }

        textBuf[0] = SPACE_FAKE_ASCII;
        textBuf[1] = SPACE_FAKE_ASCII;

        intToDigitsString(textBuf, time_millis, true);

        setDecimalPoint(true, timeDisplayShift);
    }

    // latching button without a purpose
    lights &= ~(1 << LIGHT_LATCHING_1);

    textBufToDisplay();
}

void Apps::modeRandomWorld()
{
    if (this->app_init_edge)
    {
        randomWorldState = randomWorldIdle;
        randomModeTrigger(false);
#ifdef ENABLE_EEPROM
        RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW = eeprom_read_word((uint16_t *)EEPROM_RANDOM_WORLD_UPPER_BOUNDARY_NUMBER_DRAW);
#else
        RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW = 10;
#endif
    }

    // textbuf contains the actual random generated graphics. Set to display at start, because it might be overriden in this routine.
    textBufToDisplay();

    switch (randomWorldState)
    {
    case randomWorldIdle:
    {
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
        {
            uint16_t delay_seconds = dialGetIndexedtime();
            // set autoroll time.
            RANDOMWORLD_AUTODRAW_DELAY.setInitTimeMillis(-1000 * (long)delay_seconds);

            if (millis_blink_250_750ms())
            {
                setStandardTextToTextHANDLE(TEXT_AUTO);
            }
            else
            {
                ledDisp->setNumberToDisplayAsDecimal(delay_seconds);
            }
        }
        else
        {
            if (encoder_dial->getDelta())
            {
                randomWorldState = randomWorldRollingEnd;
            }
        }

        for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
        {
            if (binaryInputsEdgeUp & 1 << i)
            {
                RANDOMWORLD_RANDOM_TYPE = i + 4 * ((binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1)) > 0);
                randomWorldState = randomWorldRolling;

                // set up animation
                RANDOMWORLD_ROLL_SPEED.start(-30 - (((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)) > 0) * 1970)); // special case for upper limit setting for random number.
            }
        }
    }
    break;

    case randomWorldInstantRoll:
    {
    }
    break;
    case randomWorldRolling:
    {
        // check state
        bool roll_end = false;

        // //if autoroll
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
        {
            // autoroll no need for button
            roll_end = true;
        }
        else
        {
            // wait for button release

            // the following build up of the mask looks cumbersome, but, it's all done at precompiler time. , so it's just one byte of flash memory.
            byte momentary_buttons_mask =
                1 << BUTTON_INDEXED_MOMENTARY_0 |
                1 << BUTTON_INDEXED_MOMENTARY_1 |
                1 << BUTTON_INDEXED_MOMENTARY_2 |
                1 << BUTTON_INDEXED_MOMENTARY_3;

            if ((binaryInputsEdgeDown & momentary_buttons_mask) > 0)
            {
                roll_end = true;
            }
        }

        if (roll_end)
        {
            randomWorldState = randomWorldRollingEnd;
        }

        // display
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
        {
            // animated
            if (RANDOMWORLD_ROLL_SPEED.getCountDownTimerElapsedAndRestart())
            {
                // addNoteToBuzzer(C7_8);
                buzzerPlayApproval();
                randomModeTrigger(false);
            }
        }
        else
        {
            // during roll all lights on
            ledDisp->setNumberToDisplayAsDecimal(8888);

            // check for continued key press at random number generator to activate settings menu
            if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)) && (getCountDownTimerHasElapsed(&RANDOMWORLD_ROLL_SPEED)))
            {
                // hack to set upper limit for random number

                RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW = encoder_dial->getValueLimited(100, false);

                RANDOMWORLD_INDEX_FROM_RANDOM_IN_A_BAG = 0; // reset the tombola.

                if (encoder_dial->getDelta())
                {
                    // for a more pleasant experience (no blinking during knob turning)
                    set_blink_offset();
                }

                if (millis_blink_250_750ms())
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
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
        {
            if (RANDOMWORLD_ROLL_SPEED.getCountDownTimerElapsedAndRestart())
            {
                buzzerPlayApproval();
                randomModeTrigger(false);

                // roll slower and slower until threshold reached.
                RANDOMWORLD_ROLL_SPEED.setInitTimeMillis(RANDOMWORLD_ROLL_SPEED.getInitTimeMillis() * 1.4);
                if (RANDOMWORLD_ROLL_SPEED.getInitTimeMillis() < -600)
                {
                    randomWorldState = randomWorldShowResult;
                }
            }
        }
        else
        {
#ifdef ENABLE_EEPROM
            // save to eeprom
            eeprom_update_word((uint16_t *)EEPROM_RANDOM_WORLD_UPPER_BOUNDARY_NUMBER_DRAW, RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW);
#endif

            randomWorldState = randomWorldShowResult;
            buzzerPlayApproval();
        }
    }
    break;

    case randomWorldShowResult:
    {
        randomModeTrigger(true);
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
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
        if (getCountDownTimerHasElapsed(&RANDOMWORLD_AUTODRAW_DELAY))
        {
            // set up animation
            RANDOMWORLD_ROLL_SPEED.start(-30);

            randomWorldState = randomWorldRolling;
        }
        if (!(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3)))
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
        // displayLetterAndPositionInAlphabet(textBuf, RANDOMWORLD_RANDOM_NUMBER);

        displayLetterAndPositionInAlphabet(textBuf, (int16_t)weightedRandomLetter());
        // numberToBufAsDecimal((int16_t)weightedRandomLetter());
    }
    break;

    case RANDOMWORLD_TAKERANDOMCARDFROMDECK:
    {
        if (!forReal)
        {
            // don't draw a card if it's not for real. We will not even display a card, as that would be confusing. Just show blanks.
            break;
        }

        RANDOMWORLD_RANDOM_NUMBER = tombola(&RANDOMWORLD_INDEX_FROM_RANDOM_IN_A_BAG, CARDS_DECK, 52);

        //show playing card
        if (RANDOMWORLD_RANDOM_NUMBER % 13 < 9)
        {
            textBuf[1] = RANDOMWORLD_RANDOM_NUMBER % 13 + 49;
        }
        else
        {
            textBuf[0] = 49;                                                       // 1
            textBuf[1] = (3 - (((RANDOMWORLD_RANDOM_NUMBER) % 13) + 1) % 10) + 48; // 9,10,11,13 to char 0 1 2 3
        }

        char test[4] = {'H', 'D', 'S', 'C'};
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
        RANDOMWORLD_RANDOM_NUMBER = tombola(&RANDOMWORLD_INDEX_FROM_RANDOM_IN_A_BAG, CARDS_DECK, RANDOMWORLD_UPPER_BOUNDARY_NUMBER_DRAW);
    }
    // NO BREAK HERE we just changed the random number to a part of a raffle draw.
    case RANDOMWORLD_RANDOMNUMBER:
    {
        // random number
        numberToBufAsDecimal(RANDOMWORLD_RANDOM_NUMBER + 1);
    }
    break;

    case RANDOMWORLD_HEADSORTAILS:
    // NO BREAK HERE, we will overflow to yes no, but change the text. DAngereous I know. But, we need the extra bytes.
    case RANDOMWORLD_YESORNO:
    {
        setStandardTextToTextBuf(60 + RANDOMWORLD_RANDOM_NUMBER * 4 + (RANDOMWORLD_RANDOM_TYPE - 3) * 2);
    }
    break;
    }
}

void Apps::modeSettings()
{
#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
#ifdef ENABLE_SOFT_POWER_OFF
    const uint8_t analog_input_pins[4] = {PIN_SELECTOR_DIAL, PIN_BUTTONS_LATCHING, PIN_BUTTONS_MOMENTARY, PIN_POWER_ON_HOLD};
#else
    const uint8_t analog_input_pins[4] = {PIN_SELECTOR_DIAL, PIN_BUTTONS_LATCHING, PIN_BUTTONS_MOMENTARY, PIN_MERCURY_SWITCHES};

#endif
#else
    const uint8_t analog_input_pins[2] = {PIN_BUTTONS_LATCHING, PIN_BUTTONS_MOMENTARY};
#endif
    if (this->app_init_edge)
    {
    }

    // back and forth motion required of the potentio to count up modes
    if ((encoder_dial->getDelta() < 0 && SETTINGS_MODE_SELECTOR % 2 == 0))
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

        for (uint8_t i = 0; i < 4; i++)
        {

            // every momentary button has a matching latching button assigned
            if (binaryInputsToggleValue & (1 << (i + 4))) //latching
            {
                // momentary button pressed while its matching latching button is ON?

                if (binaryInputsValue & (1 << i))
                {
                    textHandle[i] = '8';
                }
                else
                {
                    textHandle[i] = ONLY_MIDDLE_SEGMENT_FAKE_ASCII;
                }
            }
            else if (binaryInputsValue & (1 << i))
            {
                textHandle[i] = '0';
            }
        }

        if (millis_quarter_second_period())
        { // BAD PRACTICE GIVES FUN RESULTS. Will go through this loop continuously when true (no edge detection). And then half a second static. I kind of like the effect
            MODE_SETTINGS_DECIMAL_POINT_COUNTER++;
            if (MODE_SETTINGS_DECIMAL_POINT_COUNTER > 15)
            {
                MODE_SETTINGS_DECIMAL_POINT_COUNTER = 0;
            }
        }
        ledDisp->setDecimalPointsToDisplay(MODE_SETTINGS_DECIMAL_POINT_COUNTER);
    }
    else if (SETTINGS_MODE_SELECTOR < 8)
    {
        // enable/disable sound

        lights |= 1 << LIGHT_MOMENTARY_0;
        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
        {
#ifdef ENABLE_EEPROM

            eeprom_update_byte(
                (uint8_t *)EEPROM_SOUND_DISABLED,
                !eeprom_read_byte((uint8_t *)EEPROM_SOUND_DISABLED));
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

        // menu title
        setStandardTextToTextBuf(TEXT_BEEP);

        // value text
        uint8_t text = TEXT_YES;
        if (buzzer->getPin() == PIN_BUZZER)
        {
            text = TEXT_NO;
        }
        setStandardTextToTextHANDLE(text);
    }
    else if (SETTINGS_MODE_SELECTOR < 10)
    {
#ifdef ENABLE_BATTERY_STATUS

        ledDisp->setNumberToDisplayAsDecimal((int16_t)batteryVoltage);
        // menu title
        setStandardTextToTextBuf(TEXT_BATTERY);

#endif
    }

#ifdef ENABLE_SOFT_POWER_OFF
    else if (SETTINGS_MODE_SELECTOR < 12)
    {
        // enable/disable auto power off

        lights |= 1 << LIGHT_MOMENTARY_0;

        uint8_t auto_power_off_enabled = true;
        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
        {
#ifdef ENABLE_EEPROM

            eeprom_update_byte(
                (uint8_t *)EEPROM_LUCIEBOX_AUTO_POWER_OFF_DISABLED,
                !eeprom_read_byte((uint8_t *)EEPROM_LUCIEBOX_AUTO_POWER_OFF_DISABLED));
        }

        auto_power_off_enabled = (eeprom_read_byte((uint8_t *)EEPROM_LUCIEBOX_AUTO_POWER_OFF_DISABLED) != 0);
#endif

        // menu title
        setStandardTextToTextBuf(TEXT_AUTO);

        // value text
        uint8_t text = TEXT_NO;
        if (auto_power_off_enabled)
        {
            text = TEXT_YES;
        }
        setStandardTextToTextHANDLE(text);
    }

#endif

    else if (SETTINGS_MODE_SELECTOR < 20)
    {

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
        // A0 handles the selector button analog input. It will not change apps when the selector knob is rotated in this menu.
        // menu title
        textBuf[2] = 'A';
        uint8_t index = (SETTINGS_MODE_SELECTOR - 10) / 2;
        ledDisp->digitValueToChar(&textBuf[3], index);

        // Value
        if (millis_blink_250_750ms())
        {
            // be stable when shown (only measure when menu text is shown)
            SETTINGS_MODE_ANALOG_VALUE = (int16_t)analogRead(analog_input_pins[index]);
        }
        ledDisp->setNumberToDisplayAsDecimal(SETTINGS_MODE_ANALOG_VALUE);
#else

        // menu title
        // todo: this does not represent the analog pin. just buttons latching and buttons momentary. Analog input days are almost over...
        textBuf[2] = 'B';
        uint8_t index = SETTINGS_MODE_SELECTOR % 2;
        ledDisp->digitValueToChar(&textBuf[3], index);

        // Value
        if (millis_blink_250_750ms())
        {
            // be stable when shown (only measure when menu text is shown)
            SETTINGS_MODE_ANALOG_VALUE = (int16_t)analogRead(analog_input_pins[index]);
        }
        ledDisp->setNumberToDisplayAsDecimal(SETTINGS_MODE_ANALOG_VALUE);
#endif
    }
    else if (SETTINGS_MODE_SELECTOR < 22)
    {
        // show luciebox firmware version number
        // menu title

        // value
        ledDisp->setNumberToDisplayAsDecimal(SOFTWARE_VERSION);
        textHandle[0] = 'F'; // v doesn't work. So, F from Firmware version it is.
    }
    else if (SETTINGS_MODE_SELECTOR < 24)
    {
// display amount of times the luciebox was used
// menu value
#ifdef ENABLE_EEPROM
        ledDisp->setNumberToDisplayAsDecimal(eeprom_read_word((uint16_t *)EEPROM_LUCIEBOX_POWER_CYCLE_COUNTER));
#else
        ledDisp->setNumberToDisplayAsDecimal(666);
#endif
        // menu title
        setStandardTextToTextBuf(TEXT_QUANTITY);
    }
    else if (SETTINGS_MODE_SELECTOR < 26)
    {
        lights |= 1 << LIGHT_MOMENTARY_0;
        if (binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0))
        {
#ifdef ENABLE_EEPROM
            //lights |= 1 << LIGHT_MOMENTARY_2; // will erase all user program memory. Purposly don't advertise
            lights |= 1 << LIGHT_MOMENTARY_3; // erases only high scores in games. Which is nice if you want to have a fresh start for a nice gamenight of "Lucieboxes and booz"
            if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
            {
                this->eraseEepromRangeLimited(EEPROM_USER_RESET);
            }
            if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
            {
                this->eraseEepromRangeLimited(EEPROM_HIGH_SCORES_RESET);
            }
#endif
        }
        else
        {
            setStandardTextToTextBuf(TEXT_RESET);
        }
    }
    else if (SETTINGS_MODE_SELECTOR < 28)
    {
        // measure cycle time.
        // 2021-10-02: between 800 and 1200 millis per cycle.
        // if (MODE_SETTINGS_CYCLE_TIMING_INDEX == 0){
        // MODE_SETTINGS_CYCLE_TIMING_INDEX ++;
        // MODE_SETTINGS_CYCLE_TIMING_MILLIS = micros();

        // }else if (MODE_SETTINGS_CYCLE_TIMING_INDEX == 1){
        // MODE_SETTINGS_CYCLE_TIMING_INDEX ++;
        // MODE_SETTINGS_CYCLE_TIMING_MILLIS_2 = micros();

        // }else{
        // MODE_SETTINGS_CYCLE_TIMING_INDEX = 0;
        // #ifdef ENABLE_SERIAL
        // Serial.println("---");
        // Serial.println(MODE_SETTINGS_CYCLE_TIMING_MILLIS);

        // Serial.println(MODE_SETTINGS_CYCLE_TIMING_MILLIS_2);
        // Serial.println(MODE_SETTINGS_CYCLE_TIMING_MILLIS_2 - MODE_SETTINGS_CYCLE_TIMING_MILLIS);
        // #endif
        // }
    }
    else
    {
        ledDisp->setNumberToDisplayAsDecimal(SETTINGS_MODE_SELECTOR);
    }

    if (SETTINGS_MODE_SELECTOR >= 6)
    {
        // originally: show values one seconds, menu items half a second
        // in luciebox settings mode
        if (millis_blink_250_750ms())
        {
            textBufToDisplay();

#ifdef ENABLE_BATTERY_STATUS
            // atmega328p
            ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
            //  ADMUX = bit (REFS0) | bit (REFS1);  // Internal 1.1V reference
            //delay(2); // Wait for Vref to settle
            // playSongHappyDryer();
            // playSongHappyDryer();

            loadBuzzerTrack(SONG_ALPHABET);
            buzzerSilentClearBuffer();
            //fill8BytesArrayWithZero();
            buzzerChangeSpeedRatioWithEncoderDial();
            //dialGetIndexedtime();

            //Serial.println(SETTINGS_APP_BATTERY_VOLTAGE);
            ADCSRA |= _BV(ADSC); // Start conversion
            while (bit_is_set(ADCSRA, ADSC))
                ; // measuring

            uint8_t low = ADCL;  // must read ADCL first - it then locks ADCH
            uint8_t high = ADCH; // unlocks both

            long result = (high << 8) | low;

            result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000

            batteryVoltage = result;

#endif
        }
    }
}

void Apps::eraseEepromRangeLimited(uint8_t setting)
{
#ifdef ENABLE_EEPROM
    uint16_t first = 0;
    uint16_t last = 0;
    if (setting == EEPROM_TOTAL_RESET)
    {
        first = 0;
        last = EEPROM_LAST_ADDRESS;
    }
    else if (setting == EEPROM_USER_RESET)
    {
        first = EEPROM_FIRST_ADDRESS_OF_USER_RANGE;
        last = EEPROM_LAST_ADDRESS;
    }
    else
    {
        // soft erase (only high scores and other app settings)
        first = EEPROM_FIRST_ADDRESS_OF_USER_RANGE;
        last = EEPROM_LAST_ADDRESS_OF_EEPROM_SOFT_ERASE;
    }

    for (uint16_t i = first; i <= last; i++)
    {
        uint8_t erase_value = 0;

        if (i >= EEPROM_SEQUENCER_SONGS_START_ADDRESS && i < EEPROM_PICTURES_START_ADDRESS)
        {
            erase_value = 0xff;
        }

        eeprom_update_byte(
            (uint8_t *)i,
            erase_value);
    }
    playSongHappyDryer();

#endif
}

void Apps::displayLetterAndPositionInAlphabet(char *textBuf, int16_t letterValueAlphabet)
{
    if (letterValueAlphabet > 8)
    {
        ledDisp->digitValueToChar(&textBuf[0], (letterValueAlphabet + 1) / 10);
    }
    ledDisp->digitValueToChar(&textBuf[1], (letterValueAlphabet + 1) % 10);

    textBuf[3] = letterValueAlphabet + 65; // show letters alphabet.
}

#ifdef ENABLE_TALLY_KEEPER

void Apps::modeTallyKeeper()
{
    int16_t *tally_counters[] = {&TALLY_KEEPER_0, &TALLY_KEEPER_1, &TALLY_KEEPER_2, &TALLY_KEEPER_3};
    int16_t display_value;

    if (this->app_init_edge)
    {
    }

    byte momentary_buttons_mask = 1 << BUTTON_INDEXED_MOMENTARY_0 | 1 << BUTTON_INDEXED_MOMENTARY_1 | 1 << BUTTON_INDEXED_MOMENTARY_2 | 1 << BUTTON_INDEXED_MOMENTARY_3;

    display_value = *tally_counters[TALLY_KEEPER_DISPLAYED_COUNTER];

    // Check for momentary keypress initiated
    for (uint8_t i = 0; i < 4; i++)
    {
        if (binaryInputsEdgeUp & (1 << i))
        {
            TALLY_KEEPER_DISPLAYED_COUNTER = i;
            if (!(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3)))
            {
                TALLY_KEEPER_DELTA = 1;
            }
            else
            {
                // buzzerSilentClearBufferAndAddNote(C5_2);
                buzzerPlayApproval();
            }
        }
    }

    TALLY_KEEPER_DELTA_SIGNED = TALLY_KEEPER_DELTA;
    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1))
    {
        TALLY_KEEPER_DELTA_SIGNED = -TALLY_KEEPER_DELTA;
    }

    if ((binaryInputsValue & momentary_buttons_mask) != 0) // a button is being held
    {
        if (TALLY_KEEPER_DELTA >= 1)
        { // little trick to do checking of limit AND latching_extra
            TALLY_KEEPER_DELTA += encoder_dial->getDelta();
        }

        display_value = *tally_counters[TALLY_KEEPER_DISPLAYED_COUNTER] + TALLY_KEEPER_DELTA_SIGNED;
        if (TALLY_KEEPER_DELTA > 1)
        {
            display_value = TALLY_KEEPER_DELTA;
        }
    }
    else if ((binaryInputsEdgeDown & momentary_buttons_mask) != 0) // a button unpressed
    {
        // --- Story time ---
        // (*tally_counters[i])++;
        //*tally_counters[i]++; // THIS WONT WORK LUCIE! DU-UH SAYS DADDY BRECHT YOU'RE PLUS PLUSSING GOD KNOWS WHAT!
        // *(*tally_counters+i) = (*(*tally_counters+i)) + 1;  // uncle lodie was having a pointer fight right here! In memoriam.

        for (uint8_t i = 0; i < 4; i++)
        {
            // affect all counters if requested.
            if (i == TALLY_KEEPER_DISPLAYED_COUNTER || (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2)))
            {
                int16_t v = (*tally_counters[i]) + TALLY_KEEPER_DELTA_SIGNED;
                if (v < 0)
                {
                    v = 0;
                }

                (*tally_counters[i]) = v;
            }
        }

        TALLY_KEEPER_DELTA = 0;
    }

    ledDisp->setNumberToDisplayAsDecimal(display_value);
    lights |= 1 << lights_indexed[TALLY_KEEPER_DISPLAYED_COUNTER];
}
#endif

#ifdef ENABLE_QUIZ_MASTER
void Apps::quiz()
{
    if (this->app_init_edge)
    {
        QUIZ_MAX_RANDOM_WAIT_TIME = -3000;
        quizState = quizWaitForQuizMaster;
    }

    switch (quizState)
    {
    case quizWaitForQuizMaster:
    {
        if (this->millis_half_second_period())
        {
            lights |= 1 << LIGHT_LATCHING_3;
        }
        else
        {
            lights &= ~(1 << LIGHT_LATCHING_3);
        }

        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1))
        {
            // QUIZ_MAX_RANDOM_WAIT_TIME = -1000 * dialGetIndexedtime();
            QUIZ_MAX_RANDOM_WAIT_TIME = -20000;
        }
        else
        {
            QUIZ_MAX_RANDOM_WAIT_TIME = -3000;
        }

        // bool quizmasterButtonChanged = (binaryInputsEdgeUp ) & (1 << BUTTON_INDEXED_LATCHING_3);
        // bool quizmasterButtonChanged = (binaryInputsEdgeUp | binaryInputsEdgeDown) & (1 << BUTTON_INDEXED_LATCHING_3);

        // if (quizmasterButtonChanged || ((binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_2) && binaryInputsValue &  (1 << BUTTON_INDEXED_LATCHING_3))))
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
        {
            quizState = quizWaitRandomTime;
            QUIZ_RANDOM_WAIT_TIME.start(random(QUIZ_MAX_RANDOM_WAIT_TIME, -500));
        }
    }
    break;

    case quizWaitRandomTime:
    {
        if (getCountDownTimerHasElapsed(&QUIZ_RANDOM_WAIT_TIME))
        {
            // //TEST SHOW RAW ANALOG VALUES
            // //empty analog input buffer
            // for(uint8_t i=0;i<100;i++){
            // 	QUIZ_ANALOG_VALUES_CHECK[i] = 0;
            // }
            // QUIZ_ANALOG_INPUT_SAMPLE_INDEX = 0;
            // //ENDTEST

            QUIZ_FIRST_ANALOG_MOMENTARY_BUTTON_NON_ZERO_VALUE = 0;
            quizState = quizWaitPlayerPress;
        }

        // check here, any player pressing his button = score to zero.
        for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
        {
            if (binaryInputsEdgeUp & 1 << i)
            {
                QUIZ_SCORE[i] = 0;
                // addNoteToBuzzer(C4_1);
                buzzerPlayDisappointment();
            }
        }
    }
    break;

    case quizWaitPlayerPress:
    {
        // //TEST SHOW RAW ANALOG VALUES
        // QUIZ_ANALOG_VALUES_CHECK[QUIZ_ANALOG_INPUT_SAMPLE_INDEX] = analogRead(PIN_BUTTONS_MOMENTARY)/4;
        // nextStepRotate(&QUIZ_ANALOG_INPUT_SAMPLE_INDEX,true,0,99);
        // //ENDTEST

        // 16,32,64,128 are the stored values of the individual raw momentary buttons (analog / 4)
        uint8_t momentaryAnalogRaw = analogRead(PIN_BUTTONS_MOMENTARY) / 4;
        if (momentaryAnalogRaw > 3 && QUIZ_FIRST_ANALOG_MOMENTARY_BUTTON_NON_ZERO_VALUE == 0)
        {
            QUIZ_FIRST_ANALOG_MOMENTARY_BUTTON_NON_ZERO_VALUE = momentaryAnalogRaw;
        }

        byte momentary_buttons_lights = 1 << LIGHT_MOMENTARY_0 | 1 << LIGHT_MOMENTARY_1 | 1 << LIGHT_MOMENTARY_2 | 1 << LIGHT_MOMENTARY_3;
        lights |= momentary_buttons_lights;

        // uint8_t compare_values [4];

        for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
        {
            if (binaryInputsEdgeUp & 1 << i)
            {
                QUIZ_ANALOG_VALUES_CHECK[i] = 0x10 << i; // button 0:16, 1:32, 2:64, 3:128

                // go to next state
                quizState = quizDefineRoundWinner;
            }
            else
            {
                QUIZ_ANALOG_VALUES_CHECK[i] = 255; // unobtainable value. maximum.
            }
        }

        if (!(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3)))
        {
            quizState = quizWaitForQuizMaster;
        }

        // //TEST SHOW RAW ANALOG VALUES
        // Serial.println("---");
        // Serial.println(QUIZ_ANALOG_INPUT_SAMPLE_INDEX);
        // //display recorded values
        // uint8_t first_non_zero_value = 0;
        // for(uint8_t i=0;i<100;i++){
        // 	Serial.println(QUIZ_ANALOG_VALUES_CHECK[QUIZ_ANALOG_INPUT_SAMPLE_INDEX]);
        // 	// if (first_non_zero_value == 0){
        // 	// 	first_non_zero_value = QUIZ_ANALOG_VALUES_CHECK[QUIZ_ANALOG_INPUT_SAMPLE_INDEX];
        // 	// }
        // 	nextStepRotate(&QUIZ_ANALOG_INPUT_SAMPLE_INDEX,true,0,99);
        // }
        // //ENDTEST
    }
    break;

    case quizDefineRoundWinner:
    {
        // Which theoretical button value is the closest to the first detected analog value? That's the winner. It looks pretty sound after some experimentation.

        for (uint8_t i = 0; i < 4; i++)
        {
            QUIZ_ANALOG_VALUES_CHECK[4 + i] = abs(QUIZ_FIRST_ANALOG_MOMENTARY_BUTTON_NON_ZERO_VALUE - QUIZ_ANALOG_VALUES_CHECK[i]);
        }

        QUIZ_MOST_RECENT_ROUND_WINNER_INDEX = 0;
        uint8_t smallestDiff = 255;

        for (uint8_t i = 0; i < 4; i++)
        {
            if (QUIZ_ANALOG_VALUES_CHECK[4 + i] < smallestDiff)
            {
                QUIZ_MOST_RECENT_ROUND_WINNER_INDEX = i;
                smallestDiff = QUIZ_ANALOG_VALUES_CHECK[4 + i];
            }
        }

        // //TEST SHOW RAW ANALOG VALUES
        // for(uint8_t i=0;i<8;i++){
        // 	Serial.println(QUIZ_ANALOG_VALUES_CHECK[i]);
        // }
        // Serial.println(QUIZ_FIRST_ANALOG_MOMENTARY_BUTTON_NON_ZERO_VALUE);
        // Serial.println(QUIZ_MOST_RECENT_ROUND_WINNER_INDEX);
        // //ENDTEST
        QUIZ_SCORE_MEMORY = QUIZ_SCORE[QUIZ_MOST_RECENT_ROUND_WINNER_INDEX];

        quizState = quizRoundAfterMath;
    }
    break;

    case quizRoundAfterMath:
    {
        // in case of normal quizmaster, this is the moment to assign scores with the dial
        // will keep on looping through this state to adjust score when in normal mode.
        // encoder_dial->setSensitivity(2);
        int16_t score = (int16_t)QUIZ_SCORE[QUIZ_MOST_RECENT_ROUND_WINNER_INDEX];
        stepChange(&score, encoder_dial->getDelta(), 0, 10, false);

        if (!(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2)))
        {
            // QUIZ_RANDOM_WAIT_TIME.start(-500);
            initiateCountDowntimerWith500Millis(&QUIZ_RANDOM_WAIT_TIME);
            score++;
            quizState = quizWaitSomeTimeForNextRound;
        }

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
        else if (!(binaryInputsValue & (1 << BUTTON_INDEXED_LATCHING_3)))
        {
            quizState = quizWaitForQuizMaster;
        }
#else
            else if (!(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3)))
            {
                quizState = quizWaitForQuizMaster;
            }
#endif

        QUIZ_SCORE[QUIZ_MOST_RECENT_ROUND_WINNER_INDEX] = (uint8_t)score;
        if (score > 9)
        {
            QUIZ_SCORE[QUIZ_MOST_RECENT_ROUND_WINNER_INDEX] = 0;
            playSongHappyDryer();
        }
    }
    break;

    case quizWaitSomeTimeForNextRound:
    {
        if (getCountDownTimerHasElapsed(&QUIZ_RANDOM_WAIT_TIME))
        {
            quizState = quizWaitForQuizMaster;
        }
    }
    break;
    }

    int16_t scoreToDisplay = 10000; // 10000 to make sure there will always be zeros for all digits
    int16_t multiplier = 1000;
    for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
    {
        scoreToDisplay += multiplier * (QUIZ_SCORE[i]);
        multiplier /= 10;
    }
    ledDisp->setNumberToDisplayAsDecimal(scoreToDisplay);

    ledDisp->setDecimalPointsToDisplay(B00001111);
}

#endif

void Apps::modeCountingLettersAndChars()
{
    //counting mode: numbers and letters.

    NUMBERS_AND_LETTERS_NUMBER_ELSE_LETTER_MODE = !(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1));

    if (this->app_init_edge)
    {
        // LETTERS_AND_CHARS_COUNTER = 0;
        COUNTING_LETTERS_AND_CHARS_TIMER.start(-1000);
        // NUMBERS_AND_LETTERS_COUNT_DOWN_ELSE_UP = true;
    }

    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
    {
        LETTERS_AND_CHARS_COUNTER = 0;
    }

    // auto count
    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_LATCHING_3))
    {
        if (!NUMBERS_AND_LETTERS_NUMBER_ELSE_LETTER_MODE)
        {
            loadBuzzerTrack(SONG_ALPHABET);
        }
    }

    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
    {
        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
        {
            NUMBERS_AND_LETTERS_COUNT_DOWN_ELSE_UP = false;
        }

        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
        {
            NUMBERS_AND_LETTERS_COUNT_DOWN_ELSE_UP = true;
        }

        // auto mode next item
        if (COUNTING_LETTERS_AND_CHARS_TIMER.getCountDownTimerElapsedAndRestart())
        {
            LETTERS_AND_CHARS_COUNTER += 1 - (2 * NUMBERS_AND_LETTERS_COUNT_DOWN_ELSE_UP);
        }

        // set timer speed
        dialOnEdgeChangeInitTimerPercentage(&COUNTING_LETTERS_AND_CHARS_TIMER);
    }
    else
    {
        // show number right away depending on potentio value
        //LETTERS_AND_CHARS_COUNTER = (int16_t)(encoder_dial->getValueLimited(25 + NUMBERS_AND_LETTERS_NUMBER_ELSE_LETTER_MODE * 75, false)); //1024 to 26 letters.
        LETTERS_AND_CHARS_COUNTER += encoder_dial->getDelta();
        modifyValueUpDownWithMomentary2And3(&LETTERS_AND_CHARS_COUNTER);
    }

    //only do the characters of the alphabet in lettermode.
    int16_t maxValue = 9999;
    if (!NUMBERS_AND_LETTERS_NUMBER_ELSE_LETTER_MODE)
    {
        maxValue = 25;
    }
    checkBoundaries(&LETTERS_AND_CHARS_COUNTER, 0, maxValue, true);

    if (NUMBERS_AND_LETTERS_NUMBER_ELSE_LETTER_MODE)
    {
        ledDisp->setNumberToDisplay(LETTERS_AND_CHARS_COUNTER, (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2)));
    }
    else
    {
        displayLetterAndPositionInAlphabet(textHandle, LETTERS_AND_CHARS_COUNTER);
    }
}

void Apps::modeSoundSong()
{
    if (this->app_init_edge)
    {
        playSongHappyDryer();
        // MODE_SOUND_SONG_INDEX = 0;
    }

    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
    {
        buzzer->changeTranspose(encoder_dial->getDelta());
    }
    else
    {
        buzzerChangeSpeedRatioWithEncoderDial();
    }

    uint8_t shift = (4 * ((binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1)) > 0));
    uint8_t shift_eeprom = (8 * ((binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2)) > 0));

    for (uint8_t index = 0; index < MOMENTARY_BUTTONS_COUNT; index++)
    {
        if (binaryInputsEdgeUp & (1 << index))
        {
            loadBuzzerTrack(index + shift + shift_eeprom);
        }
    }

    buzzer->lastPlayedNoteToDisplay(textHandle, decimalDotsHandle);
}

void Apps::modeComposeSong()
{
    bool displayStagedNote = false;
    int16_t step = 0;

    if (this->app_init_edge)
    {
        for (uint8_t i = 0; i < bytes_list_bufsize; i++)
        {
            COMPOSER_SONG[i] = BUZZER_NOTES_BUFFER_SONG_STOPVALUE;
        }
        COMPOSER_SONG_LENGTH = 1;
        COMPOSER_SONG[0] = SOUND_SONG_COMPOSER_DEFAULT_NOTE; //default note
        COMPOSER_STEP_TIMER.setInitTimeMillis(-200);
    }

    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1))
    {
        // eeprom load / save
#ifdef ENABLE_EEPROM
        bool loaded;
        loaded = saveLoadMenu(COMPOSER_SONG, 4, EEPROM_COMPOSER_SONG_LENGTH, EEPROM_COMPOSER_SONGS_START_ADDRESS);

        if (loaded)
        {
            COMPOSER_STEP = 0;
            // search for the last note in the composed song.
            // assume last note of longest possible song is always BUZZER_NOTES_BUFFER_SONG_STOPVALUE
            for (uint8_t i = bytes_list_bufsize - 2; i > 0; i--)
            {
                COMPOSER_SONG_LENGTH = i + 1;
                if (COMPOSER_SONG[i] != BUZZER_NOTES_BUFFER_SONG_STOPVALUE)
                {
                    break;
                }
            }
        }
#endif
    }
    else
    {
        // not in eeprom mode

        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
        {
            // display song notes indeces + enable insert and delete position

            if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
            {
                //delete current position
                if (COMPOSER_SONG_LENGTH <= 0)
                {
                    // empty songs stay empty.
                }
                else
                {
                    // move all notes one position down.
                    for (uint8_t i = COMPOSER_STEP; i < bytes_list_bufsize - 1; i++)
                    {
                        COMPOSER_SONG[i] = COMPOSER_SONG[i + 1];
                    }
                    // deleted space should be a song stop note.
                    COMPOSER_SONG[COMPOSER_SONG_LENGTH - 1] = BUZZER_NOTES_BUFFER_SONG_STOPVALUE;

                    //adjust length
                    COMPOSER_SONG_LENGTH--;
                }
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
                    // remember, last note of longest song possible MUST be BUZZER_NOTES_BUFFER_SONG_STOPVALUE, don't copy a note to it.
                    for (uint8_t i = bytes_list_bufsize - 3; i > COMPOSER_STEP; i--)
                    {
                        COMPOSER_SONG[i + 1] = COMPOSER_SONG[i];
                    }
                    COMPOSER_SONG[COMPOSER_STEP + 1] = SOUND_SONG_COMPOSER_DEFAULT_NOTE;
                    COMPOSER_SONG_LENGTH++;
                    step = 1; // move to new position
                }
            }
            ledDisp->setNumberToDisplayAsDecimal(COMPOSER_STEP);
        }
        else
        {
            // display song notes + enable programming and listening to notes

            // select a note
            if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
            {
                displayStagedNote = true;

                bool change = false;
                change = stepChange(&COMPOSER_STAGED_NOTE, encoder_dial->getDelta(), 0, 255, true);
                if (change)
                {
                    buzzerSilentClearBufferAndAddNote((uint8_t)COMPOSER_STAGED_NOTE);
                }

                // let the program button blink to invite it for being pressed.
                if (millis_half_second_period())
                {
                    lights |= 1 << LIGHT_MOMENTARY_0;
                }

                // program note in song when the combination of the two buttons is pressed.
                if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
                {
                    COMPOSER_SONG[COMPOSER_STEP] = (uint8_t)COMPOSER_STAGED_NOTE;
                    // buzzerSilentClearBufferAndAddNote(COMPOSER_SONG[COMPOSER_STEP]);

                    // if note added to end, expand song length and add default note
                    if (COMPOSER_STEP == COMPOSER_SONG_LENGTH - 1)
                    {
                        COMPOSER_SONG_LENGTH++;
                        //COMPOSER_SONG[COMPOSER_SONG_LENGTH - 1] = SOUND_SONG_COMPOSER_DEFAULT_NOTE; //default note
                        COMPOSER_SONG[COMPOSER_SONG_LENGTH - 1] = BUZZER_NOTES_BUFFER_SONG_STOPVALUE; //default note
                    }
                }
            }

            // display song notes + enable programming and listening to notes
            if ((binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1)))
            {
                buzzerSilentClearBufferAndAddNote((uint8_t)COMPOSER_STAGED_NOTE);
            }

            if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
            {
                // just listen to note on index in song
                // will also sound when just being programmed
                buzzerSilentClearBufferAndAddNote(COMPOSER_SONG[COMPOSER_STEP]);
            }
        }

        modifyValueUpDownWithMomentary2And3(&step);

        // autoplay
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
        {
            if (COMPOSER_STEP_TIMER.getCountDownTimerElapsedAndRestart())
            {
                step = 1;
            }
        }

        //default potentio behaviour
        byte momentary_buttons_mask = 1 << BUTTON_INDEXED_MOMENTARY_0 | 1 << BUTTON_INDEXED_MOMENTARY_1 | 1 << BUTTON_INDEXED_MOMENTARY_2 | 1 << BUTTON_INDEXED_MOMENTARY_3;
        if ((binaryInputsValue & momentary_buttons_mask) == 0) // no buttons pressed
        {
            if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
            {
                // change speed if default behaviour of potentio.
                // COMPOSER_STEP_TIMER.setInitTimeMillis(COMPOSER_STEP_TIMER.getInitTimeMillis() + encoder_dial->getDelta() * 10); //step +1 or -1
                dialOnEdgeChangeInitTimerPercentage(&COMPOSER_STEP_TIMER);
            }
            else
            {
                step = encoder_dial->getDelta(); ////step +1 or -1
            }
        }

        if (step != 0)
        {
            nextStepRotate(&COMPOSER_STEP, (step + 1) / 2, 0, COMPOSER_SONG_LENGTH - 1);
            // stepChange(&COMPOSER_STEP, step, 0, COMPOSER_SONG_LENGTH - 1, true);
            addNoteToBuzzer(COMPOSER_SONG[COMPOSER_STEP]);
        }

        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
        {
            ledDisp->setNumberToDisplayAsDecimal(COMPOSER_STEP);
        }
        else if (displayStagedNote)
        {
            noteToDisplay((uint8_t)COMPOSER_STAGED_NOTE);
        }
        else
        {
            noteToDisplay(COMPOSER_SONG[COMPOSER_STEP]);
        }
    }
}

void Apps::modeSoundNotesInitScale()
{
    SOUND_NOTES_NOTE_INDEX = SOUND_NOTES_SCALE_ROOT;
    SOUND_NOTES_NOTE_ON_SCALE_INDEX = 0;
    SOUND_NOTE_PLAY_NOTE = true;
}

void Apps::modeSoundNotes()
{
    bool update_note = false;
    SOUND_NOTE_MUTE = false;

    if (this->app_init_edge)
    {
        SOUND_NOTES_PROGRESSION_MODE = SOUND_NOTE_MODE_MANUAL;

        // initiateCountDowntimerWith500Millis(&SOUND_NOTE_AUTO_TIMER);
        SOUND_NOTES_SCALE_ROOT = C5_4;
        modeSoundNotesInitScale();

        SOUND_NOTE_SETTING_TEXT_TO_DISPLAY = SOUND_NOTE_DISPLAY_NOTE;
    }

    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
    {
        // auto play
        if (SOUND_NOTE_AUTO_TIMER.getCountDownTimerElapsedAndRestart())
        {
            update_note = true;
        }
        dialOnEdgeChangeInitTimerPercentage(&SOUND_NOTE_AUTO_TIMER);
    }
    else
    {
        int8_t encoder_delta = encoder_dial->getDelta();
        // change note with dial
        if (encoder_delta)
        {
            SOUND_NOTE_AUTO_UP_ELSE_DOWN = encoder_delta > 0;
            update_note = true;
        }
    }

    // advanced vs manual controls
    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1))
    {
        // advanced

        // change note length
        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
        {
            buzzer->changeNoteToNextLength(&SOUND_NOTES_NOTE_INDEX);
            SOUND_NOTE_PLAY_NOTE = true;
        }

        //  change scale
        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
        {
            nextStepRotate(&SOUND_NOTES_SCALE_INDEX, 1, 0, SCALES_COUNT);
            modeSoundNotesInitScale();
        }
        if (binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2))
        {
            SOUND_NOTE_SETTING_TEXT_TO_DISPLAY = 76 + SOUND_NOTES_SCALE_INDEX * 4;
        }

        // change key
        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3)) // set key when changing scale

        {
            // first keypress, back to root.
            // second keypress, change root.
            if (SOUND_NOTES_NOTE_INDEX == SOUND_NOTES_SCALE_ROOT)
            {
                SOUND_NOTES_SCALE_ROOT++;
                if (SOUND_NOTES_SCALE_ROOT > B6_4)
                {
                    SOUND_NOTES_SCALE_ROOT = C5_4;
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
        }

        if (binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1))
        {
            SOUND_NOTE_SETTING_TEXT_TO_DISPLAY = 96 + 4 * SOUND_NOTES_PROGRESSION_MODE;
        }
    }
    else
    {

        // mute button to silently change notes.
        // this allows the box to be used as a primitive piano.
        if (binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0))
        {
            SOUND_NOTE_MUTE = true;
        }

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

        // if (SOUND_NOTES_PROGRESSION_MODE != SOUND_NOTE_MODE_MANUAL){
        // 	SOUND_NOTE_AUTO_UP_ELSE_DOWN = SOUND_NOTES_PROGRESSION_MODE == SOUND_NOTE_MODE_ARPEGGIO_UP;

        // }

        switch (SOUND_NOTES_PROGRESSION_MODE)
        {
        case SOUND_NOTE_MODE_RANDOM:
        {
            note_jumps = random(0, 100);
            break;
        }
        case SOUND_NOTE_MODE_ARPEGGIO_SAWTOOTH:
        {

            if (random(0, 5) == 0)
            {
                SOUND_NOTE_AUTO_UP_ELSE_DOWN = !SOUND_NOTE_AUTO_UP_ELSE_DOWN;
            }
            break;
        }
        case SOUND_NOTE_MODE_ARPEGGIO_UP:
        {

            SOUND_NOTE_AUTO_UP_ELSE_DOWN = true;
            break;
        }
        case SOUND_NOTE_MODE_ARPEGGIO_DOWN:
        {

            SOUND_NOTE_AUTO_UP_ELSE_DOWN = false;
            break;
        }
        case SOUND_NOTE_MODE_RANDOM_ERRATIC:
        {
            // this is the more clear version, but takes 12bytes of memory more:
            // SOUND_NOTE_AUTO_UP_ELSE_DOWN = (bool)random(0, 2);
            // note_jumps = random(0, 3);

            // less memory intensive:
            uint8_t tmp = random(0, 6);
            SOUND_NOTE_AUTO_UP_ELSE_DOWN = tmp > 2;
            note_jumps = tmp / 2;

            break;
        }
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
                // buzzer->nextNote(
                // &SOUND_NOTES_NOTE_INDEX,
                // SOUND_NOTE_AUTO_UP_ELSE_DOWN,
                // true);
                int16_t limit = (SOUND_NOTES_NOTE_INDEX / NOTES_COUNT) * NOTES_COUNT;
                // nextStepRotate(&SOUND_NOTES_NOTE_INDEX,SOUND_NOTE_AUTO_UP_ELSE_DOWN, limit,limit+NOTES_COUNT);

                nextStepRotate(&SOUND_NOTES_NOTE_INDEX, SOUND_NOTE_AUTO_UP_ELSE_DOWN, limit, limit + NOTES_COUNT - 1);

                // //(SOUND_NOTES_NOTE_INDEX % NOTES_COUNT) * NOTES_COUNT, (SOUND_NOTES_NOTE_INDEX % NOTES_COUNT) + 1) * NOTES_COUNT);
            }
        }

        SOUND_NOTE_SETTING_TEXT_TO_DISPLAY = SOUND_NOTE_DISPLAY_NOTE;
        SOUND_NOTE_PLAY_NOTE = true;
    }

    if (SOUND_NOTE_PLAY_NOTE && !SOUND_NOTE_MUTE)
    {
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
        {
            buzzerSilentClearBuffer();
        }
        addNoteToBuzzer(SOUND_NOTES_NOTE_INDEX);
    }
    SOUND_NOTE_PLAY_NOTE = false;

    // index to actual note on the scale
    // USE A TIMER to end display of settings. hmmm not sure if it will be that much better...
    if (SOUND_NOTE_SETTING_TEXT_TO_DISPLAY == SOUND_NOTE_DISPLAY_NOTE)
    {
        noteToDisplay(SOUND_NOTES_NOTE_INDEX);
    }
    else
    {
        setStandardTextToTextHANDLE(SOUND_NOTE_SETTING_TEXT_TO_DISPLAY);
    }
}

void Apps::modeMovie()
{
    bool sound_on = false;
    bool reload_soundtrack = false;
    bool movie_restart = false;

    //reset saved led disp state.
    if (this->app_init_edge)
    {
        // MOVIE_MODE_FRAME_INTERVAL_TIMER.start(-500);
        // initiateCountDowntimerWith500Millis(&MOVIE_MODE_FRAME_INTERVAL_TIMER);
        loadNextMovie();
        MOVIE_MODE_RESTART_SOUNDTRACK_AT_MOVIE_START = true;
    }

    // sound ON/OFF

    sound_on = !(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2));

    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1))
    {
        // sound settings
        reload_soundtrack = modifyValueUpDownWithMomentary2And3(&MOVIE_MODE_SOUNDTRACK_INDEX);

        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
        {
            MOVIE_MODE_RESTART_SOUNDTRACK_AT_MOVIE_START = !MOVIE_MODE_RESTART_SOUNDTRACK_AT_MOVIE_START;
        }

        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
        {
            reload_soundtrack = true;
        }
    }
    else
    {
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
        {
            // manual mode
            MOVIE_MODE_FLASH_FRAME_INDEX += encoder_dial->getDelta();

            // step mode
            modifyValueUpDownWithMomentary2And3(&MOVIE_MODE_FLASH_FRAME_INDEX);
        }
        else
        {
            // auto mode.

            dialOnEdgeChangeInitTimerPercentage(&MOVIE_MODE_FRAME_INTERVAL_TIMER);

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
    }

    if (!(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3)))
    {
        if (MOVIE_MODE_FRAME_INTERVAL_TIMER.getCountDownTimerElapsedAndRestart())
        {
            MOVIE_MODE_FLASH_FRAME_INDEX += (1 - 2 * MOVIE_MODE_AUTO_BACKWARDS);
        }
    }

    // check limits of movie
    if (MOVIE_MODE_FLASH_FRAME_INDEX > MOVIE_MODE_MOVIE_FRAME_INDEX_END)
    {
        MOVIE_MODE_FLASH_FRAME_INDEX = MOVIE_MODE_MOVIE_FRAME_INDEX_START;
        movie_restart = true;
    }
    else if (MOVIE_MODE_FLASH_FRAME_INDEX < (int16_t)MOVIE_MODE_MOVIE_FRAME_INDEX_START)
    {
        MOVIE_MODE_FLASH_FRAME_INDEX = MOVIE_MODE_MOVIE_FRAME_INDEX_END;
        movie_restart = true;
    }

    if (MOVIE_MODE_RESTART_SOUNDTRACK_AT_MOVIE_START)
    {
        reload_soundtrack = reload_soundtrack || movie_restart;
    }
    else if (buzzer->getBuzzerNotesBufferEmpty())
    {
        reload_soundtrack = true;
    }

    // soundtrack
    if (reload_soundtrack && sound_on)
    {
        // buzzerSilentClearBuffer();
        loadBuzzerTrack(MOVIE_MODE_SOUNDTRACK_INDEX);
    }

    // graphics
    loadMovieFrame(MOVIE_MODE_FLASH_FRAME_INDEX);

    // invert all data in picture
    if (MOVIE_MODE_SHOW_NEGATIVE)
    {
        // negative .
        displayAllSegments = ~displayAllSegments;
    }

    // set to display
    displayAllSegmentsToScreen();
}

bool Apps::loadMovieFrame(int16_t frame_index)
{
    this->displayAllSegments = 0;

    if (frame_index < MAX_FRAMES_MOVIES_FLASH)
    {
        //flash
        flashPictureToDisplayAllSegments(disp_4digits_animations + frame_index * 4);
    }
    else
    {
        //eeprom
        eepromPictureToDisplayAllSegments(frame_index - MAX_FRAMES_MOVIES_FLASH);
    }

    // check for end of movie
    uint32_t stop_screen = (uint32_t)ANIMATION_STOP_CODE_PART_0 << 0 | (uint32_t)ANIMATION_STOP_CODE_PART_1 << 8 | (uint32_t)ANIMATION_STOP_CODE_PART_2 << 16 | (uint32_t)ANIMATION_STOP_CODE_PART_3 << 24;

    if (displayAllSegments == stop_screen)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Apps::loadNextMovie()
{
    bool foundStartOfMovie = false;

    // iterate until a stop frame is found check for stop frame
    while (loadMovieFrame(MOVIE_MODE_FLASH_FRAME_INDEX) || !foundStartOfMovie)
    {
        // check if over limit (last movie)
        if (MOVIE_MODE_FLASH_FRAME_INDEX >= (MAX_FRAMES_MOVIES_FLASH
#ifdef ENABLE_EEPROM
                                             + EEPROM_NUMBER_OF_DRAWINGS
#endif
                                             ))

        {
            foundStartOfMovie = true;
            MOVIE_MODE_MOVIE_FRAME_INDEX_START = 1;
            MOVIE_MODE_FLASH_FRAME_INDEX = 1;
        }

        // set variables presuming that this could be the last iteration of the while loop.
        if (!foundStartOfMovie)
        {
            MOVIE_MODE_MOVIE_FRAME_INDEX_START = MOVIE_MODE_FLASH_FRAME_INDEX + 1;
        }
        else
        {
            MOVIE_MODE_MOVIE_FRAME_INDEX_END = MOVIE_MODE_FLASH_FRAME_INDEX;
        }

        if (!loadMovieFrame(MOVIE_MODE_FLASH_FRAME_INDEX))
        {
            foundStartOfMovie = true;
        }

        MOVIE_MODE_FLASH_FRAME_INDEX++;
    }
}

void Apps::displayChangeGlobal(uint32_t *display_buffer)
{
    // global picture operations

    // WARNING, nonkel Lode took this out this function to save some measly bytes. But, just so you know, if you ever add stuff here, look for DRAW_SHOW_MODE everywhere in the code and change it accordingly.
    // if (saveStateToBuffer)
    // {
    // 	DRAW_SHOW_MODE = 4; // prepare for next button press to save buffer and show inversion.

    // }

    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
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
            *display_buffer = (uint32_t)ANIMATION_STOP_CODE_PART_0 << 0 | (uint32_t)ANIMATION_STOP_CODE_PART_1 << 8 | (uint32_t)ANIMATION_STOP_CODE_PART_2 << 16 | (uint32_t)ANIMATION_STOP_CODE_PART_3 << 24;
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
    // return blinking cursor segment.
    // no need to initialize DRAW_CURSOR_INDEX, it get's within boundaries in one cycle. And we don't care about starting position.

    uint8_t segmentMoveIndexed[9] = {0x20, 0x10, 0x00, 0x01, 0x40, 0x08, 0x02, 0x04, 0x80}; // 0x00 for empty . It's good to have spots where the cursor is invisible. In order not to pollute the display if you want to really see your drawing, or want to show it to your mother
    uint32_t cursor_position_on_display;

    // encoder_dial->setSensitivity(2);

    int16_t cursor_position = DRAW_CURSOR_ACTIVE_DIGIT * 9 + DRAW_CURSOR_ACTIVE_SEGMENT_IN_ACTIVE_DIGIT;

    stepChange(&cursor_position, encoder_dial->getDelta(), 0, 35, true);

    DRAW_CURSOR_ACTIVE_DIGIT = cursor_position / 9;
    DRAW_CURSOR_ACTIVE_SEGMENT_IN_ACTIVE_DIGIT = cursor_position % 9;

    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
    {
        // move segment inside digit
        nextStepRotate(&DRAW_CURSOR_ACTIVE_SEGMENT_IN_ACTIVE_DIGIT, true, 0, 8);
    }

    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
    {
        // move digit
        nextStepRotate(&DRAW_CURSOR_ACTIVE_DIGIT, true, 0, 3);
    }

    cursor_position_on_display = (uint32_t)(segmentMoveIndexed[DRAW_CURSOR_ACTIVE_SEGMENT_IN_ACTIVE_DIGIT]) << (DRAW_CURSOR_ACTIVE_DIGIT)*8;

    // check for global display change
    this->displayChangeGlobal(&displayAllSegments);

    // toggle individual segment
    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
    {
        // drawing or erasing the segment by xor'ing it with the drawing
        *display_buffer ^= cursor_position_on_display;
        DRAW_SHOW_MODE = 4; // prepare for next button press to save buffer and show inversion at next global change button press.
    }

    if (millis_quarter_second_period())
    {
        return cursor_position_on_display;
    }
    else
    {
        return 0;
    }
}

void Apps::drawGame()
{
    // shows a picture. After it disappears, you have to draw it exactly as it was.

    uint32_t cursorBlinker = 0;

    if (this->app_init_edge)
    {
        drawGameState = drawGameWaitForStart;
    }

#ifndef ENABLE_SELECT_APPS_WITH_SELECTOR
    lights &= ~(1 << LIGHT_LATCHING_3); // set light for sure off
    if (millis_half_second_period())
    {
        lights |= 1 << LIGHT_LATCHING_3;
    }
#endif

    switch (drawGameState)
    {
    case drawGameWaitForStart:
    {
        drawGameState = drawGameShowPicture;

        DRAW_GAME_PICTURE_TYPE =
            (((uint8_t)((binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1)) > 0)) << 1) + ((binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2)) > 0);

        if (DRAW_GAME_PICTURE_TYPE == DRAW_GAME_CLOCK)
        {
            // learn how to read the clock

            ledDisp->minutesToMinutesHoursString(textBuf, (uint16_t)random(0, 1440));
            textBufToDisplayAllSegments();

            // add hour:minute divider.
            displayAllSegments |= 1UL << 15;
        }
        else if (DRAW_GAME_PICTURE_TYPE == DRAW_GAME_NUMBER)
        {
            // random number
            long r = random(0, 10000);
            numberToBufAsDecimal((int16_t)r);
            textBufToDisplayAllSegments();
        }

        else if (DRAW_GAME_PICTURE_TYPE == DRAW_GAME_WORD)
        {
            // random text
            for (uint8_t i = 0; i < 4; i++)
            {
                long r = random(0, 25);
                textBuf[i] = (char)r + 65;
            }
            textBufToDisplayAllSegments();
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
        // start game button blinking

        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_LATCHING_3))
        {
            drawGameState = drawGameDraw;
            displayAllSegmentsBuffer = displayAllSegments;
            displayAllSegments = 0;
        }
        byte option_latching_buttons_mask = 1 << BUTTON_INDEXED_LATCHING_1 | 1 << BUTTON_INDEXED_LATCHING_2;
#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
        if (millis_half_second_period())
        {
            lights |= 1 << LIGHT_LATCHING_3;
        }
        if (((binaryInputsEdgeDown | binaryInputsEdgeUp) & option_latching_buttons_mask) != 0) // a button edge detected
#else
        if (((binaryInputsEdgeUp)&option_latching_buttons_mask) != 0) // a button edge detected
#endif
        {
            drawGameState = drawGameWaitForStart;
        }
        break;
    }

    case drawGameDraw:
    {
        cursorBlinker = modeSingleSegmentManipulation(&displayAllSegments);
        // bug: you can't use this because of same buffer reuse. displayChangeGlobal(&displayAllSegments, false);
#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
        if ((binaryInputsEdgeUp | binaryInputsEdgeDown) & (1 << BUTTON_INDEXED_LATCHING_3))
#else
        if ((binaryInputsEdgeUp) & (1 << BUTTON_INDEXED_LATCHING_3))
#endif
        {
            drawGameState = drawGameEvaluate;
            DRAW_GAME_DISPLAY_TIMER.start();

            if (displayAllSegments == displayAllSegmentsBuffer)
            {
                playSongHappyDryer();
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
#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
        // wait for user input to continue.
        byte momentary_buttons_mask = 1 << BUTTON_INDEXED_MOMENTARY_0 | 1 << BUTTON_INDEXED_MOMENTARY_1 | 1 << BUTTON_INDEXED_MOMENTARY_2 | 1 << BUTTON_INDEXED_MOMENTARY_3;
        if ((binaryInputsEdgeUp & momentary_buttons_mask) != 0) // a button pressed
        {
            drawGameState = drawGameWaitForStart;
        }
        if (millis_half_second_period())
        {
            lights |= 1 << LIGHT_MOMENTARY_0;
        }
#else
        if ((binaryInputsEdgeUp) & (1 << BUTTON_INDEXED_LATCHING_3))
        {
            drawGameState = drawGameWaitForStart;
        }

#endif

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

    setButtonLight(LIGHT_LATCHING_1, DRAW_GAME_PICTURE_TYPE / 2);
    setButtonLight(LIGHT_LATCHING_2, DRAW_GAME_PICTURE_TYPE % 2);

    ledDisp->setBinaryToDisplay(displayAllSegments ^ cursorBlinker);
}

void Apps::modeHackTime()
{
    // run through all the addresses to see the raw values!

    bool address_changed = false;
    const char drive_letter[3] = {'F', 'R', 'E'};
    int16_t memory_sizes[3] = {32255, 2047, 1023};

    if (this->app_init_edge)
    {
        // HACKTIME_MOVE_TIMER.start(-500);
        // initiateCountDowntimerWith500Millis(&HACKTIME_MOVE_TIMER);
    }

    if ((binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1)))
    {
        // set memory type
        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
        {
            nextStepRotate(&HACKTIME_MEMORY_SELECT, 1, 0, 2);
        }

        // display address location

        if (millis_blink_250_750ms() || binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0))
        {
            ledDisp->setBlankDisplay();
            textHandle[0] = drive_letter[HACKTIME_MEMORY_SELECT];
        }
        else
        {
            ledDisp->setNumberToDisplay(HACKTIME_ADDRESS, true); // show address as hex, to fit all addresses on 4 chars display
        }
    }
    else
    {

        // read and write
        if (binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1))
        {
            // change value in address location (left char on display)
            // will not work for flash memory

            array_8_bytes[0] += encoder_dial->getDelta();

            if (encoder_dial->getDelta() != 0)
            {
                // store value.
                // change ram
                if (HACKTIME_MEMORY_SELECT == HACKTIME_MEMORY_RAM)
                {
                    *((uint8_t *)HACKTIME_ADDRESS) = array_8_bytes[0];
                }

                if (HACKTIME_MEMORY_SELECT == HACKTIME_MEMORY_EEPROM)
                {
#ifdef ENABLE_EEPROM
                    eeprom_update_byte((uint8_t *)HACKTIME_ADDRESS, array_8_bytes[0]);
#endif
                }
            }
        }

        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
        {
            // display mode change (how to represent the memory value?)
            nextStepRotate(&HACKTIME_DISPLAY_MODE, 1, 0, 3);
        }

        if (HACKTIME_DISPLAY_MODE == HACKTIME_DISPLAY_CHARS)
        {
            // do nothing
            for (uint8_t i = 0; i < 4; i++)
            {
                textHandle[i] = array_8_bytes[i];
            }
        }
        else if (HACKTIME_DISPLAY_MODE == HACKTIME_DISPLAY_BYTES)
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

    // address changing
    address_changed = modifyValueUpDownWithMomentary2And3(&HACKTIME_ADDRESS);

    // read mode
    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
    {
        // auto scroll mode
        if (HACKTIME_MOVE_TIMER.getCountDownTimerElapsedAndRestart())
        {
            HACKTIME_ADDRESS++;
            address_changed = true;
        }

        // set speed.
        dialOnEdgeChangeInitTimerPercentage(&HACKTIME_MOVE_TIMER);
    }
    else if (binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1))
    {
    }
    else
    {
        // manual scroll
        if (encoder_dial->getDelta())
        {
            address_changed = true;
            HACKTIME_ADDRESS += encoder_dial->getDelta() *
                                (1 + 3 * ((binaryInputsValue & 1 << BUTTON_INDEXED_MOMENTARY_3) > 0) // advance four bytes. The display can show four bytes at once, so, we can advance 'one screen' per step
                                 + 255 * ((binaryInputsValue & 1 << BUTTON_INDEXED_MOMENTARY_2) > 0) // quick move
                                );
        }
    }

    // check memory boundary limits
    checkBoundaries(&HACKTIME_ADDRESS, 0, memory_sizes[HACKTIME_MEMORY_SELECT], true);

    // get value from memory address and memory type
    for (uint8_t i = 0; i < 4; i++)
    {
        switch (HACKTIME_MEMORY_SELECT)
        {
        case HACKTIME_MEMORY_FLASH:
            array_8_bytes[i] = pgm_read_byte(HACKTIME_ADDRESS + i);
            break;

        case HACKTIME_MEMORY_RAM:
            array_8_bytes[i] = *(((uint8_t *)HACKTIME_ADDRESS) + i);
            break;

        case HACKTIME_MEMORY_EEPROM:
#ifdef ENABLE_EEPROM
            array_8_bytes[i] = eeprom_read_byte((uint8_t *)HACKTIME_ADDRESS + i);
#else
            array_8_bytes[i] = 111;
#endif
            break;
        }
    }

    // convert memory to sounds... Be prepared for post-modernist masterpieces
    // if ( (address_changed ))
    if ((address_changed || (HACK_TIME_ACTIVE_VALUE != array_8_bytes[0])))
    {
        if (!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
        {
            if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
            {
                buzzerSilentClearBufferAndAddNote(array_8_bytes[0]);
            }
            set_blink_offset();
        }
    }

    //edge for value just changed
    HACK_TIME_ACTIVE_VALUE = array_8_bytes[0];
}

bool Apps::modifyValueUpDownWithMomentary2And3(int16_t *value)
{
    // int16_t value_memory = *value;
    int8_t amount = 0;
    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
    {
        // *value -= amount;
        amount = -1;
    }

    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
    {
        // *value += amount;
        amount = 1;
    }
    *value += amount;

    return amount != 0;
    // return value_memory != *value;
}

void Apps::dialOnEdgeChangeInitTimerPercentage(SuperTimer *aTimer)
{
    // will change the init time millis with five percent up or down if encoder dial movement detected

    // only works for countdown times (negative init value)!
    if (encoder_dial->getDelta())
    {
        long original = (aTimer->getInitTimeMillis());
        long result = long((float)original * (1 - (float)(encoder_dial->getDelta()) * 0.005));

        // if value too small to make an absolute difference, force it! (make sure to stay negative)
        // the problem is that it can get stuck on a low value. In that case, we can make it more negative again by forcing at least one milli.
        if (original == result)
        {
            // result -= encoder_dial->getDelta() * encoder_dial->getDelta();
            result -= 1;
        }

        aTimer->setInitTimeMillis(
            result);
    }
}

void Apps::eepromCopyValues(uint16_t fromAddress, uint16_t toAddress)
{
#ifdef ENABLE_EEPROM
    uint8_t tmp = eeprom_read_byte((uint8_t *)(fromAddress));
    eeprom_update_byte((uint8_t *)(toAddress), tmp);
#endif
}

void Apps::draw()
{
    // scroll through drawings from eeprom memories
    // change drawings with a fancy painting app
    // save drawings too eeprom
    // manage drawings library by deleting or inserting slots. This is good when making animations.

    uint32_t cursorBlinker = 0;

    if (this->app_init_edge)
    {
        DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY = 1; // make different than active drawing index to force loading of first drawing.
    }

    // SAVE / LOAD drawings from memory
#ifdef ENABLE_EEPROM
    checkBoundaries(&DRAW_ACTIVE_DRAWING_INDEX, 0, EEPROM_NUMBER_OF_DRAWINGS - 1, true);

    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1))
    {
        // eeprom save mode
        if (millis_half_second_period())
        {
            lights |= 1 << LIGHT_MOMENTARY_0;
            lights |= 1 << LIGHT_MOMENTARY_1;
        }

        // SHIFT button to insert or delete drawing slots from eeprom
        if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
        {
            int16_t lastPictureEepromAddress = EEPROM_PICTURES_START_ADDRESS + (EEPROM_PICTURES_TOTAL_LENGTH - 2);
            // DELETE
            if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
            {
                // delete slot. (and shift all drawings.)
                uint16_t currentIndexPictureEepromAddress = EEPROM_PICTURES_START_ADDRESS + (DRAW_ACTIVE_DRAWING_INDEX)*4;
                for (int16_t i = currentIndexPictureEepromAddress;
                     i <= lastPictureEepromAddress;
                     i++)
                {
                    eepromCopyValues(i + 4, i);
                }
                DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY++;
            }

            // INSERT
            if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
            {
                // copy drawing from current index to the next one and move to it (and move to it).
                // the drawing on the last address location is lost.

                // one picture before the last one is the last position where you can still insert a drawing.
                // we got this covered by the main for loop. index++ is jumping to zero... But, let's suck it up for the sake of 18 saved bytes.

                // work with eeprom addresses, not with picture indexes.
                // each picture is four bytes
                for (int16_t i = lastPictureEepromAddress;
                     i >= EEPROM_PICTURES_START_ADDRESS + (DRAW_ACTIVE_DRAWING_INDEX)*4;
                     i--)
                {
                    // move all pictures one up.
                    eepromCopyValues(i, i + 4);
                }
                DRAW_ACTIVE_DRAWING_INDEX++;
            }

            if (millis_half_second_period())
            {
                lights |= 1 << LIGHT_MOMENTARY_2;
                lights |= 1 << LIGHT_MOMENTARY_3;
            }
        }
        else

            if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
        {
            // save active drawing on display to eeprom.
            for (uint8_t i = 0; i < 4; i++)
            {
#ifdef ENABLE_EEPROM
                eeprom_update_byte(
                    (uint8_t *)(EEPROM_PICTURES_START_ADDRESS + DRAW_ACTIVE_DRAWING_INDEX * 4 + i),
                    (uint8_t)((displayAllSegments >> (i * 8)) & 0xFF));
#endif
            }
        }
    }
    else
#endif
        // VIEW / MODIFY drawing
        if ((binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3)) &&
            !(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2)))
    {
        // modify drawing on display in draw mode.
        // if in save to eeprom mode, always only scroll through drawings.

        cursorBlinker = this->modeSingleSegmentManipulation(&displayAllSegments);
    }
    else
    {
        // scroll through drawings
        DRAW_ACTIVE_DRAWING_INDEX += encoder_dial->getDelta();

        if (!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
        { // shift function for saving drawings to eeprom.

            //check for global display change. we're not really changing the drawing, just seeing how it would look negative, and stuf..
            this->displayChangeGlobal(&displayAllSegments);

            //scroll through drawings.
            modifyValueUpDownWithMomentary2And3(&DRAW_ACTIVE_DRAWING_INDEX);
        }
    }

#ifdef ENABLE_EEPROM
    if (DRAW_ACTIVE_DRAWING_INDEX != DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY)
    {
        // load drawing from memory only if index changed
        displayAllSegments = 0;
        eepromPictureToDisplayAllSegments(DRAW_ACTIVE_DRAWING_INDEX);

        DRAW_SHOW_MODE = 4; // prepare for next button press to save current buffer when showing inverted picture
    }
#endif

    DRAW_ACTIVE_DRAWING_INDEX_EDGE_MEMORY = DRAW_ACTIVE_DRAWING_INDEX;

    // OUTPUT to display
    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
    {
        // number: always show index of active drawing if activated.
        ledDisp->setNumberToDisplayAsDecimal(DRAW_ACTIVE_DRAWING_INDEX); // do +1 or not?  in the real world, most of the people start counting from 1. Welcome to an eternal discussion Lucie!
    }
    else
    {
        // drawing: set display
        ledDisp->setBinaryToDisplay(displayAllSegments ^ cursorBlinker);
    }
}

#ifdef ENABLE_TILT_APP
void Apps::tiltSwitchTest()
{
    // four tilt switches are positioned as such that they are "ON" in rest position.
    const uint8_t segments_to_fill[] = {70, 12, 73, 45}; // one segment has four sides. we want seg 0 to be on for forward, 1 and 2 for right, 3 for backward, and 4 and 5 for left. Black magic was needed in this limited memory environment.
    if (this->app_init_edge)
    {
        setStandardTextToTextBuf(TEXT_TILT);

        // TILT_EXPECTED_SWITCH_INDEX = 0;
        // TILT_CYCLE_COUNTER = 0;
        displayAllSegments = 0;
    }

    // time the amount of cycles.

    if (binaryInputs[mercury_switches_indexed[TILT_EXPECTED_SWITCH_INDEX]].getEdgeUp())
    {
        displayAllSegments ^= 1 << (segments_to_fill[TILT_EXPECTED_SWITCH_INDEX] / 10);
        displayAllSegments ^= 1 << (segments_to_fill[TILT_EXPECTED_SWITCH_INDEX] % 10);
        displayAllSegments &= ~(1UL << 7);

        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1))
        {
            TILT_EXPECTED_SWITCH_INDEX += 2;
        }
        else
        {
            TILT_EXPECTED_SWITCH_INDEX++;
        }

        addNoteToBuzzer(1); //not beep but "puck"
        if (TILT_EXPECTED_SWITCH_INDEX >= 4)
        {
            TILT_CYCLE_COUNTER++;
            TILT_EXPECTED_SWITCH_INDEX = 0;
            addNoteToBuzzer(REST_4_8); // would you believe it, it sounds like a chicken. this is the kedeeeeet  in the tok tok tok kedeeet (the ke is also a tok sound)
            addNoteToBuzzer(C7_1);
        }
    }

    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
    {
        TILT_CYCLE_COUNTER = 0;
    }

    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_LATCHING_3))
    {
        // start timer
        TILT_TIMER.start();
        TILT_CYCLE_COUNTER = 0;
    }

    // display
    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
    {
        setDecimalPoint(TILT_TIMER.getSecondsBlinker(), 1);
        TILT_TIMER.getTimeString(textBuf);
        textBufToDisplay();
    }
    else
    {
        ledDisp->setNumberToDisplayAsDecimal(TILT_CYCLE_COUNTER);
        ledDisp->setBinaryToDisplay(displayAllSegments);
    }

    // normal vs timed mode.
    if (binaryInputsToggleValue & 1 << BUTTON_INDEXED_LATCHING_3)
    {
        if (!TILT_TIMER.getTimeIsNegative())
        {
            loadBuzzerTrack(SONG_DRYER_UNHAPPY);
            TILT_TIMER.reset();
        }
    }
    else
    {
        TILT_TIMER.setInitCountDownTimeSecs(dialGetIndexedtime());
        TILT_TIMER.reset();
    }
}
#endif

void Apps::dialMultiplyValueAndDisplay(int16_t *pVariable, uint8_t multiplier, int16_t maxValue)
{
    // assume zero as min value
    *pVariable += encoder_dial->getDelta() * multiplier;
    checkBoundaries(pVariable, 0, maxValue, false);
    ledDisp->setNumberToDisplayAsDecimal(*pVariable);
}

void Apps::modeGeiger()
{
    if (this->app_init_edge)
    {
        GEIGER_TONE_FREQUENY_LOWEST = 2000;
        GEIGER_TONE_FREQUENCY_HEIGHEST = 4000;
        GEIGER_TONE_LENGTH = 10;
        GEIGER_PROBABILITY_THRESHOLD = 950000;
    }

    //play tick.
    //wait random time.
    long r = random(0, 1024) * random(0, 1024);

    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1))
    {
        // note mode
        if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
        {
            //lower
            dialMultiplyValueAndDisplay(&GEIGER_TONE_FREQUENY_LOWEST, 10, 5000);
        }
        else if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
        {
            //upper
            dialMultiplyValueAndDisplay(&GEIGER_TONE_FREQUENCY_HEIGHEST, 10, 5000);
        }
        else if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)))
        {
            //length
            dialMultiplyValueAndDisplay(&GEIGER_TONE_LENGTH, 1, 255);
        }
        else if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3)))
        {
            if (encoder_dial->getDelta())
            {
                this->geigerToneHelper();
            }
        }
        else
        {
            GEIGER_PROBABILITY_THRESHOLD -= encoder_dial->getDelta() * 10 * 1024;
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

        GEIGER_PROBABILITY_THRESHOLD -= encoder_dial->getDelta() * 10 * 1024;
        if (r > GEIGER_PROBABILITY_THRESHOLD)
        {
            buzzer->playTone((unsigned int)50, 10);
            setStandardTextToTextHANDLE(TEXT_RANDOM_SEGMENTS);
            // textBufToDisplay();
            COUNTER_GEIGER++;
        }
    }

    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
    {
        ledDisp->setNumberToDisplayAsDecimal(COUNTER_GEIGER);
    }
}

void Apps::geigerToneHelper()
{
    unsigned int random_frequency_within_limits = random(GEIGER_TONE_FREQUENY_LOWEST, GEIGER_TONE_FREQUENCY_HEIGHEST + 1);

    buzzer->playTone(
        random_frequency_within_limits,
        (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3)) ? 0 : GEIGER_TONE_LENGTH);

    ledDisp->setNumberToDisplayAsDecimal(random_frequency_within_limits);
    COUNTER_GEIGER++;
}

void Apps::modeSequencer()
{
    int8_t step = 0;

    bool showNote = false;

    if (this->app_init_edge)
    {
        SEQUENCER_SPEED.start(-125); // 4steps per beat, 120 bpm
        SEQUENCER_EEPROM_MODE_BLINK.start(-1000);

        // Create autogenerated song. Lucie, I invite you to play around with the parameters.
        uint8_t base_note = random(77, 87);
        for (uint8_t i = 0; i < 32; i++)
        {
            // beat sequence for all four bars equal
            if (i < 8)
            {
                for (uint8_t j = 0; j < 4; j++)
                {
                    // all notes blank to start
                    this->SEQUENCER_SONG[i + (8 * j)] = REST_1_8;

                    // some base notes as a beat
                    if (random(0, 3))
                    {
                        this->SEQUENCER_SONG[i + (8 * j)] = base_note;
                    }
                }
            }

            // extra notes to add some melody, it's ok to overwrite the basenote beat.
            if (!random(0, 3))
            {
                // base line
                this->SEQUENCER_SONG[i] = pgm_read_byte_near(scales + PENT + random(0, 6));
            }
            else
            {
                if (!random(0, 3))
                {
                    // high notes
                    this->SEQUENCER_SONG[i] = random(25, 63);
                }
            }
        }
    }

    if (this->binaryInputsEdgeDown & (1 << BUTTON_INDEXED_LATCHING_1))
    {
        this->app_init_edge = true; // make sure we display the sequencer when returning from save/load mode
    }

    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1))
    {
#ifdef ENABLE_EEPROM
        this->saveLoadMenu(this->SEQUENCER_SONG, EEPROM_SEQUENCER_SONGS_COUNT, EEPROM_SEQUENCER_SONG_LENGTH, EEPROM_SEQUENCER_SONGS_START_ADDRESS);
#endif
    }
    else
    {
        // note at active sequencer step
        SEQUENCER_ACTIVE_STEP_NOTE = SEQUENCER_SONG[SEQUENCER_STEP_COUNTER];

        // autoplay
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
        {
            dialOnEdgeChangeInitTimerPercentage(&SEQUENCER_SPEED);

            if (SEQUENCER_SPEED.getCountDownTimerElapsedAndRestart())
            {
                step = 1;
            }
        }
        else
        {
            // manipulate the sequencer

            if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_1))
            {
                buzzerSilentClearBufferAndAddNote(SEQUENCER_CHOSEN_NOTE);
            }

            if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)))
            {
                // just listen to the potentio note
                SEQUENCER_CHOSEN_NOTE += encoder_dial->getDelta();

                // if button continuously pressed, listen to notes as they are chosen
                buzzerSilentClearBufferAndAddNoteAtEncoderDialChange(SEQUENCER_CHOSEN_NOTE);
                noteToDisplay(SEQUENCER_CHOSEN_NOTE);
                showNote = true;

                // let the program button blink to invite it for being pressed.
                if (millis_half_second_period())
                {
                    lights |= 1 << LIGHT_MOMENTARY_0;
                }

                // program note to song
                if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
                {
                    buzzerSilentClearBufferAndAddNote(SEQUENCER_CHOSEN_NOTE);

                    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
                    {
                        //copy to all measures
                        for (uint8_t i = 0; i < 4; i++)
                        {
                            this->SEQUENCER_SONG[(SEQUENCER_STEP_COUNTER % 8) + 8 * i] = SEQUENCER_CHOSEN_NOTE;
                        }
                    }
                    else
                    {
                        this->SEQUENCER_SONG[SEQUENCER_STEP_COUNTER] = SEQUENCER_CHOSEN_NOTE;
                    }
                }
            }
            else if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
            {
                // play sequencer note at active index
                addNoteToBuzzer(SEQUENCER_ACTIVE_STEP_NOTE);
            }
            else
            {
                step += encoder_dial->getDelta();
            }

            if (this->binaryInputsEdgeDown & (1 << BUTTON_INDEXED_LATCHING_3))
            {
                // reset transpose when stop autoplay.
                SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET = 0;
            }
        }

        // song progression
        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
        {
            step = 1;
        }

        // song progression
        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_2))
        {
            step = -1;
        }

        // int16_t step_16 = (int16_t) step;
        // modifyValueUpDownWithMomentary2And3(&step_16);
        // step = (int8_t) step_16;

        if ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)))
        {
            // if button continuously pressed, show sequencer note at active index

            noteToDisplay(SEQUENCER_ACTIVE_STEP_NOTE);
            showNote = true;

            // bonus effect: TRANSPOSE!
            SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET += encoder_dial->getDelta();
        }

        // handle step change
        if (step != 0 || this->app_init_edge)
        {
            nextStepRotate(&SEQUENCER_STEP_COUNTER, step > 0, 0, 31);

            buzzerSilentClearBufferAndAddNote(
                this->SEQUENCER_SONG[SEQUENCER_STEP_COUNTER] +
                SEQUENCER_TEMPORARY_TRANSPOSE_OFFSET * ((binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)) > 0));

            displayAllSegments = 0;

            flashPictureToDisplayAllSegments(disp_4digits_animations + ANIMATION_INDEX_SEQUENCER_TOP_CIRCLE + (SEQUENCER_STEP_COUNTER % 8) * 4);
            flashPictureToDisplayAllSegments(disp_4digits_animations + ANIMATION_INDEX_SEQUENCER_BOTTOM_BAR + (SEQUENCER_STEP_COUNTER / 8) * 4);
        }

        if (!showNote)
        {
            displayAllSegmentsToScreen();
        }
    }
}

void Apps::modeMetronome()
{
    // todo: LFO:  with extra timer, create slight timing offset in second ticker, for fun effects (zwevingen) --> I think nonkel Lode was too optimistic about the available memory at the beginning. We went for a metronome here, not for trippy hippy
    bool update = false;

    if (this->app_init_edge)
    {
        TIMER_METRONOME.start(-83); // 60bpm as default
    }

    if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
    {
        // dialOnEdgeChangeInitTimerPercentage(&TIMER_METRONOME); // bpm change

        // auto counting (metronome)
        if (TIMER_METRONOME.getCountDownTimerElapsedAndRestart())
        {
            update = true;
        }

        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3))
        {
            METRONOME_ENABLE_FLASH_AT_BEEP = !METRONOME_ENABLE_FLASH_AT_BEEP;
        }
    }
    else if (binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3))
    {
        buzzerChangeSpeedRatioWithEncoderDial();
        buzzerSilentClearBufferAndAddNoteAtEncoderDialChange(C6_4);
    }
    else
    {
        // manual mode
        update = encoder_dial->getDelta() != 0;
    }

    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
    {
        // ticker 1,2 and 3 back together (at position of ticker 1)
        METRONOME_TICKER_2_POSITION = METRONOME_TICKER_1_POSITION;
        METRONOME_TICKER_3_POSITION = METRONOME_TICKER_1_POSITION;
    }

    displayAllSegments = 0;
    bool forceNextStep = update || binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_3);

    modeMetronomeTickerUpdate(&METRONOME_TICKER_3_POSITION, BUTTON_INDEXED_MOMENTARY_1, !(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1)), C6_4, forceNextStep);
    modeMetronomeTickerUpdate(&METRONOME_TICKER_2_POSITION, BUTTON_INDEXED_MOMENTARY_2, true, C5_4, forceNextStep);
    modeMetronomeTickerUpdate(&METRONOME_TICKER_1_POSITION, BUTTON_INDEXED_MOMENTARY_3, true, C7_8, update);

    bool visualize_bpm = binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2);
    if (visualize_bpm)
    {
        dialOnEdgeChangeInitTimerPercentage(&TIMER_METRONOME); // bpm change
        // bpm --> full 12 step circles per minute.   timing is per step. so: 60bpm == 1 circle / second = timer: 1000/12 = 83.333ms/step
        ledDisp->setNumberToDisplayAsDecimal((int16_t)(1 / (12 * (float)TIMER_METRONOME.getInitTimeMillis() / 60000))); // millis/step to fullcirclesp/minute (bpm)
    }
    else
    {
        displayAllSegmentsToScreen();
    }
}

void Apps::modeMetronomeTickerUpdate(int16_t *ticker_counter, uint8_t momentary_id, bool direction, uint8_t sound_at_zero_pass, boolean force_step)
{
    // every ticker gets updated.

    // check for next step
    if (binaryInputsEdgeUp & 1 << momentary_id || force_step)
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
    if (METRONOME_ENABLE_FLASH_AT_BEEP && *ticker_counter == 0)
    {
        this->displayAllSegments = 0xFFFFFFFF;
        lights = 0xff;
    }
    else
    {
        flashPictureToDisplayAllSegments(disp_4digits_animations + ANIMATE_CIRCLE_OFFSET + *ticker_counter * 4);
    }
}

#ifdef ENABLE_SIMON_APP
void Apps::modeSimon()
{
    uint8_t lights = 0b00000000;

    if (this->app_init_edge)
    {
        SIMON_BLINK_TIMER.setInitTimeMillis(-250);

        SIMON_ACTIVE_LIGHT = SIMON_NO_ACTIVE_LIGHT;
        SIMON_PLAYERS_COUNT = 1;
    }

    if (!(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3)))
    {
        // at any time, if play button off: no more playing!
        simonState = simonWaitForNewGame;
    }

    // keep settings lights on, even when buttons are toggled during the game
    if (SIMON_ALL_PLAYERS_PLAY_IN_EACH_LEVEL)
    {
        lights |= 1 << LIGHT_LATCHING_2;
    }
    if (SIMON_CUSTOM_BUILD_UP)
    {
        lights |= 1 << LIGHT_LATCHING_1;
    }

    // check if a momentary button was pressed, and create byte with status: 0000 is no button pressed.  0001, 0010, 0100, 1000
    uint8_t pressed_momentary_button = SIMON_NO_BUTTON_PRESSED;
    for (int k = 0; k < MOMENTARY_BUTTONS_COUNT; ++k)
    {
        if (binaryInputsEdgeUp & 1 << k)
        {
            pressed_momentary_button = k;
        }
    }

    switch (simonState)
    {
    case simonWaitForNewGame:
    {
        setStandardTextToTextBuf(TEXT_SPACES);
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
        {
            simonState = simonNewGame;
        }

        // latching_3_blink();
        if (millis_quarter_second_period())
        {
            lights |= 1 << LIGHT_LATCHING_3;
        }

        // number of players.
        SIMON_PLAYERS_COUNT = (encoder_dial->getValueLimited((SIMON_MAX_PLAYERS - 1) * 4, false) / 4 + 1); // start counting from player 0 to display

        numberToBufAsDecimal(SIMON_PLAYERS_COUNT);
        textBuf[1] = 'P';

        // Instead of computer, user choses the next light in simon sequence.
        SIMON_CUSTOM_BUILD_UP = binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1);

        // random player sequence during game if multiplayer
        SIMON_ALL_PLAYERS_PLAY_IN_EACH_LEVEL = binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2);

        break;
    }

    case simonNewGame:
    {
        playSongHappyDryer();

        SIMON_END_OF_GAME = false;
        SIMON_PLAYERS_ALIVE_COUNT = SIMON_PLAYERS_COUNT;

        // set all players arranged in array. don't bother about real player count.
        for (uint8_t i = 0; i < SIMON_MAX_PLAYERS; i++)
        {
            SIMON_PLAYERS[i] = i;
        }

        // generate new sequence. if 4 momentary buttons: 0,1,2,3,0,1,2,3,0,1,2,3,...
        // it's easier to save indeces than binary values like 0001, 0010, 0100, 1000 because
        // converting from binary to index needs a loop.
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
        shuffle(SIMON_PLAYERS, SIMON_PLAYERS_ALIVE_COUNT);

        // set first player

        SIMON_PLAYER_PLAYING_INDEX = 0; // this is just the index.
        numberToBufAsDecimal(SIMON_LENGTH);

        // let 'maximum length breach' be a happy crash. I can't afford the bytes!
        //   if (SIMON_LENGTH >= bytes_list_bufsize) {
        // 	  // reached maximum length
        // 	  playSongHappyDryer();
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

        if (buzzer->getBuzzerNotesBufferEmpty())
        { // at start, wait for the beginning song to be over.
            simonState = simonPlaySequence;
        }
    }

    case simonPlaySequence:
    {
        if (SIMON_END_OF_GAME)
        {
            if (millis_blink_250_750ms())
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
            if (getCountDownTimerHasElapsed(&SIMON_BLINK_TIMER))
            {
                SIMON_ACTIVE_LIGHT = SIMON_NO_ACTIVE_LIGHT;
            }
        }

        if (SIMON_INDEX >= SIMON_LENGTH && (getCountDownTimerHasElapsed(&SIMON_BLINK_TIMER)))
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

        if (!getCountDownTimerHasElapsed(&SIMON_STEP_TIMER))
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

        // addNoteToBuzzer(A3_8);
        buzzerPlayApproval();
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
        // button lights are on when pressed because of default behaviour (light of button  on at button press)
        textBuf[0] = SIMON_PLAYERS[SIMON_PLAYER_PLAYING_INDEX] + 49;
        textBuf[1] = 'P';
        if (pressed_momentary_button == SIMON_NO_BUTTON_PRESSED)
        {
            break;
        }

        const int expected = SIMON_LIST[SIMON_INDEX];

        if (pressed_momentary_button != expected)
        {
            // if custom build up, last light of the sequence and first player, player gets to choose the next move
            if (SIMON_CUSTOM_BUILD_UP && (SIMON_INDEX + 1) == SIMON_LENGTH && SIMON_PLAYER_PLAYING_INDEX == 0)
            {
                SIMON_LIST[SIMON_INDEX] = pressed_momentary_button;
            }
            else
            {
                // player made mistake, player dies
                this->addNoteToBuzzerRepeated(C4_1, 4);
                simonState = simonPlayerDead;
                break;
            }
        }

        // player pressed correct button
        //addNoteToBuzzer(A3_8);
        buzzerPlayApproval();

        ++SIMON_INDEX;

        if (SIMON_INDEX >= SIMON_LENGTH)
        {
            simonState = simonNextPlayer;
            break;
        }

        break;
    }

    case simonNextPlayer:
    {
        // sequence done!
        // addNoteToBuzzer(E5_4);
        // addNoteToBuzzer(REST_8_8);
        // buzzerPlayApproval();// addNoteToBuzzer(B6_1);

        // check next alive player (assume there is always a player alive.)

        //playSongHappyDryer();
        // addNoteToBuzzerRepeated(C4_8,SIMON_PLAYERS[SIMON_PLAYER_PLAYING_INDEX]);
        //addNoteToBuzzerRepeated(C4_8,3);

        SIMON_PLAYER_PLAYING_INDEX++;
        if (SIMON_ALL_PLAYERS_PLAY_IN_EACH_LEVEL || SIMON_PLAYER_PLAYING_INDEX >= SIMON_PLAYERS_ALIVE_COUNT)
        {
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
    this->lights |= lights;

    textBufToDisplay();
}
#endif //ENABLE_SIMON_APP

uint16_t *Apps::reactionGameLevelToEepromAddress()
{
    return (uint16_t *)(EEPROM_REACTION_GAME_START_ADDRESS +
                        REACTION_GUITAR_APP_SELECT_HERO_ELSE_WHACKING_APP * 48 +
                        REACTION_OPTION_WHACKABIRD_OR_HEXHERO * 24 +
                        REACTION_OPTION_WHACKENDURANCE_OR_HEROPAUSE_OR_HEXCOMPLEMENT * 12 +
                        REACTION_GAME_LEVEL * 2);
}

void Apps::modeReactionGame()
{
#ifdef ENABLE_REACTION_APP

    if (this->app_init_edge)
    {
        reactionGameState = reactionWaitForStart;
        displayAllSegments = 0x00;

        // play by sound, only initiate pattern at start of app. This way, players can get used to it. To change pattern, leave and come back to app.
        // fun fact: when box started in this app, it will always have the same pattern. There is a lesson to be learned about random seeding dear Lucie!
        // never twice the same sound. Only first notes of array will be used.
        uint8_t random_range_selector = random(219, 233); // choose notes from a random chosen interval of 12 notes
        for (uint8_t i = 0; i < 12; i++)
        {
            REACTION_GAME_TEMP_SELECTED_NOTES[i] = random_range_selector + i;
        }
        this->shuffle(REACTION_GAME_TEMP_SELECTED_NOTES, 12);

        for (uint8_t i = 0; i < 4; i++)
        {
            REACTION_GAME_SELECTED_NOTES[i] = REACTION_GAME_TEMP_SELECTED_NOTES[i];
        }
    }

    // at any time, leave game when depressing play button.
    if (this->binaryInputsEdgeDown & (1 << BUTTON_INDEXED_LATCHING_3))
    {
        reactionGameState = reactionWaitForStart;
    }

    switch (reactionGameState)
    {
    case reactionWaitForStart:
    {
        // change level
        REACTION_GAME_LEVEL = (encoder_dial->getValueLimited(80, false) / 16); // only set the default inittime at selecting the game. If multiple games are played, init time stays the same.
        if (encoder_dial->getDelta())
        {
            // for a more pleasant experience (no blinking during knob turning)
            set_blink_offset();
        }

        // check options
        REACTION_GUITAR_APP_SELECT_HERO_ELSE_WHACKING_APP = (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_0)) > 0;
        REACTION_OPTION_WHACKABIRD_OR_HEXHERO = (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1)) > 0;
        REACTION_OPTION_WHACKENDURANCE_OR_HEROPAUSE_OR_HEXCOMPLEMENT = (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2)) > 0;

        // display level and high score
#ifdef ENABLE_EEPROM
        if (millis_blink_250_750ms())
        {
            lights |= 1 << LIGHT_LATCHING_3;
            ledDisp->setNumberToDisplayAsDecimal(
                eeprom_read_word(reactionGameLevelToEepromAddress()));
        }
        else
        {
            intToDigitsString(textBuf, REACTION_GAME_LEVEL + 1, false); // utilities lode
            textBuf[0] = 'L';
            textBufToDisplay();
        }
#else
            intToDigitsString(textBuf, REACTION_GAME_LEVEL + 1, false); // utilities lode
            textBuf[0] = 'L';
            textBufToDisplay();
#endif

        // play game button pressed
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
        {
            if (!REACTION_GUITAR_APP_SELECT_HERO_ELSE_WHACKING_APP && REACTION_OPTION_WHACKABIRD_OR_HEXHERO)
            {
                reactionGameState = reactionSoundInit;
                initiateCountDowntimerWith500Millis(&TIMER_REACTION_END_OF_GAME_DELAY);
                REACTION_WHACK_A_BIRD_SHOW_NOTES = 0;
            }
            else
            {
                reactionGameState = reactionNewGame;
            }
        }
        break;
    }

    case reactionSoundInit:
    {
        // play all soundsn so the player gets a feel for them.
        // show button light at tone sound
        if (TIMER_REACTION_END_OF_GAME_DELAY.getCountDownTimerElapsedAndRestart())
        {
            if (REACTION_WHACK_A_BIRD_SHOW_NOTES >= 4)
            {
                addNoteToBuzzer(REST_8_8);
                reactionGameState = reactionNewGame;
            }
            else
            {
                addNoteToBuzzer(REACTION_GAME_SELECTED_NOTES[REACTION_WHACK_A_BIRD_SHOW_NOTES]);
                REACTION_WHACK_A_BIRD_SHOW_NOTES++;
            }
        }
        lights |= 1 << lights_indexed[REACTION_WHACK_A_BIRD_SHOW_NOTES - 1];
        break;
    }

    case reactionNewGame:
    {
        REACTION_GAME_SCORE = 0;

        if (REACTION_GUITAR_APP_SELECT_HERO_ELSE_WHACKING_APP)
        {
            // guitar hero mode
            REACTION_GAME_STEP_TIME_MILLIS = -10 * pgm_read_byte_near(guitar_hero_level_speeds + REACTION_GAME_LEVEL);

            displayAllSegments = 0;

            if (REACTION_OPTION_WHACKABIRD_OR_HEXHERO)
            {
                // hex geek mode
                fill8BytesArrayWithZero();
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
            // whack a mole mode
            if (buzzer->getBuzzerNotesBufferEmpty())
            { // in sound mode, wait till demo is done

                reactionGameState = reactionNewTurn;

                if (REACTION_OPTION_WHACKENDURANCE_OR_HEROPAUSE_OR_HEXCOMPLEMENT)
                {
                    // if enabled, we go for "as many points in a limited time. --> this to make it more exciting for adults (can be boring after a while if you just have to press the right button in time)
                    // REACTION_GAME_STEP_TIME_MILLIS = (1UL << (REACTION_GAME_LEVEL + 1)) * -4000; // step speed depending on level
                    REACTION_GAME_STEP_TIME_MILLIS = -50 * pgm_read_byte_near(whack_a_mole_countdown_level_step_speeds + REACTION_GAME_LEVEL); // step speed depending on level 12 steps in total cycle
                    REACTION_GAME_TIMER_STEP = 0;
                }
                else
                {
                    // REACTION_GAME_STEP_TIME_MILLIS = (1UL << (6 - REACTION_GAME_LEVEL)) * -35; // step speed depending on level
                    REACTION_GAME_STEP_TIME_MILLIS = -10 * pgm_read_byte_near(whack_a_mole_level_step_speeds + REACTION_GAME_LEVEL); // step speed depending on level, 12 steps in total cycle
                }
            }
        }
        TIMER_REACTION_GAME_SPEED.start(REACTION_GAME_STEP_TIME_MILLIS);
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

        bool complementRequired = 0;

        if (REACTION_OPTION_WHACKENDURANCE_OR_HEROPAUSE_OR_HEXCOMPLEMENT)
        {
            complementRequired = !random(0, 4);
            REACTION_GAME_DECIMAL_POINTS = REACTION_GAME_DECIMAL_POINTS << 1;
            REACTION_GAME_DECIMAL_POINTS |= complementRequired;
        }
        else
        {
            // if changing settings from complemtary mode to normal mode no residual static dots are wanted
            // although it was actually a great little game idea: the dot stays static on the display, so, whenver a value is hovering over that particular spot, the compliment has to be provided.
            REACTION_GAME_DECIMAL_POINTS = 0;
        }

        REACTION_GAME_HEX_MEMORY[0] = random(1 - complementRequired, 16 - complementRequired); // do not include numbers that don't require button presses

        // uint8_t value_to_hex_char = (REACTION_GAME_HEX_MEMORY[0]) + 48;

        // if ( value_to_hex_char > 57){
        // value_to_hex_char += 7; // there are 7 ascii positions between 9 and A
        // }
        // textBuf[0] = value_to_hex_char; // 0-9, A-F

        ledDisp->digitValueToChar(&textBuf[0], REACTION_GAME_HEX_MEMORY[0]);

        // prepare next move
        reactionGameState = reactionHexPlaying;
        TIMER_REACTION_GAME_SPEED.start();
        REACTION_HEX_GUESSED_CORRECTLY = false;
        break;
    }

    case reactionHexWaitForButtonsRelease:
    {
        // all buttons need to be release before we can check for a new press.
        byte momentary_buttons_mask = 1 << BUTTON_INDEXED_MOMENTARY_0 | 1 << BUTTON_INDEXED_MOMENTARY_1 | 1 << BUTTON_INDEXED_MOMENTARY_2 | 1 << BUTTON_INDEXED_MOMENTARY_3;
        if ((binaryInputsValue & momentary_buttons_mask) == 0) // no buttons pressed
        {
            reactionGameState = reactionHexPlaying;
        }

        textBufToDisplay();
        ledDisp->setDecimalPointsToDisplay(REACTION_GAME_DECIMAL_POINTS);
        break;
    }

    case reactionHexPlaying:
    {
        REACTION_GAME_HEX_ACTIVE_DIGIT = 3;
        while (
            REACTION_GAME_HEX_ACTIVE_DIGIT > 0 &&
            (textBuf[REACTION_GAME_HEX_ACTIVE_DIGIT] == SPACE_FAKE_ASCII || textBuf[REACTION_GAME_HEX_ACTIVE_DIGIT] == SPACE_FAKE_ASCII))
        {
            REACTION_GAME_HEX_ACTIVE_DIGIT--;
        }

        uint8_t binary_pattern_to_find = REACTION_GAME_HEX_MEMORY[REACTION_GAME_HEX_ACTIVE_DIGIT];
        if (REACTION_GAME_DECIMAL_POINTS & (1 << REACTION_GAME_HEX_ACTIVE_DIGIT))
        {
            // if a decimal point is found, we will need to input the complement.

            binary_pattern_to_find = 0x0F & ~binary_pattern_to_find;
        }
        byte momentary_buttons_mask = 1 << BUTTON_INDEXED_MOMENTARY_0 | 1 << BUTTON_INDEXED_MOMENTARY_1 | 1 << BUTTON_INDEXED_MOMENTARY_2 | 1 << BUTTON_INDEXED_MOMENTARY_3;
        uint8_t momentary_buttons_pressed_binary_pattern = momentary_buttons_mask & binaryInputsValue; // momentary buttons as binary pattern

        // reverse the pattern.
        uint8_t tmp = 0;
        for (uint8_t i = 0; i < 4; i++)
        {
            bool bitIsSet = momentary_buttons_pressed_binary_pattern & 1 << i;
            tmp |= bitIsSet << (3 - i);
        }
        momentary_buttons_pressed_binary_pattern = tmp;

        uint8_t diff_between_needed_and_pressed = momentary_buttons_pressed_binary_pattern ^ binary_pattern_to_find;      // if zero --> equal (which means "ok")
        uint8_t check_for_wrong_key_presses = diff_between_needed_and_pressed & momentary_buttons_pressed_binary_pattern; // if not zero --> wrong key pressed

        if (check_for_wrong_key_presses != 0)
        {
            reactionGameState = reactionJustDied;
        }

        // check of button press pattern is the sought pattern
        if (!diff_between_needed_and_pressed)
        {
            // buzzerSilentClearBufferAndAddNote(C5_4);
            buzzerPlayApproval();
            reactionGameState = reactionHexWaitForButtonsRelease;
            REACTION_HEX_GUESSED_CORRECTLY = true;
            textBuf[REACTION_GAME_HEX_ACTIVE_DIGIT] = SPACE_FAKE_ASCII;
            REACTION_GAME_SCORE++;
        }

        // end of move
        if (getCountDownTimerHasElapsed(&TIMER_REACTION_GAME_SPEED))
        {
            reactionGameState = reactionHexNextStep;
            if (REACTION_GAME_HEX_ACTIVE_DIGIT == 3)
            {
                reactionGameState = reactionJustDied;
            }
        }

        // update
        textBufToDisplay();
        ledDisp->setDecimalPointsToDisplay(REACTION_GAME_DECIMAL_POINTS);
        break;
    }

    case reactionGuitarHeroNewTurn:
    {
        // like in guitar hero, the horizontal segments in the screen fall down and button combo's have to be pressed to "catch" the falling segments.

        // three rows of four horizontal segments in 4 digits 7 segment display.
        // choose top row random. any combination of 4 (including zero) ==> 16 combinations.

        if (random(0, 10) || !REACTION_OPTION_WHACKENDURANCE_OR_HEROPAUSE_OR_HEXCOMPLEMENT) // if pause enabled, 1 chance in ten that there will be no row added.
        {
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
            if (binaryInputsEdgeUp & (1 << i))
            {
                //if DP of this button was on, switch it off. else, die!
                //check if DP of digit "i" is set

                if (displayAllSegments & (0x80UL << 8 * i))
                {
                    // DP is on, set to zero.
                    // buzzer->addRandomSoundToNotesBuffer(190, 198);
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

        if (getCountDownTimerHasElapsed(&TIMER_REACTION_GAME_SPEED))
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
        if (buzzer->getBuzzerNotesBufferEmpty())
        {
            reactionGameState = reactionNewTurn;
        }
        break;
    }

    case reactionNewTurn:
    {
        if (!REACTION_OPTION_WHACKENDURANCE_OR_HEROPAUSE_OR_HEXCOMPLEMENT)
        {
            REACTION_GAME_TIMER_STEP = 0; //reset animation step
            if (REACTION_GAME_SCORE % 7 == 0)
            {
                // after about hunderd steps, speed doubled in 14 steps.
                TIMER_REACTION_GAME_SPEED.setInitTimeMillis((long)((float)TIMER_REACTION_GAME_SPEED.getInitTimeMillis() * 0.95));
            }
            TIMER_REACTION_GAME_SPEED.start(); // only restart if 12 steps time per turn
        }
        displayAllSegments = 0; //reset animation graphics screen

        REACTION_GAME_TARGET = random(0, MOMENTARY_BUTTONS_COUNT);

        if (REACTION_OPTION_WHACKABIRD_OR_HEXHERO)
        {
            addNoteToBuzzer(REACTION_GAME_SELECTED_NOTES[REACTION_GAME_TARGET]);
        }

        reactionGameState = reactionPlaying;
        break;
    }

    case reactionPlaying:
    {
        if (getCountDownTimerHasElapsed(&TIMER_REACTION_GAME_SPEED))
        {
            this->nextStepRotate(&REACTION_GAME_TIMER_STEP, true, 0, 12);
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

        // set graphics
        for (uint8_t step = 0; step <= REACTION_GAME_TIMER_STEP; step++)
        {
            flashPictureToDisplayAllSegments(disp_4digits_animations + ANIMATE_CIRCLE_OFFSET + step * 4);
        }
        displayAllSegmentsToScreen();

        if (!REACTION_OPTION_WHACKABIRD_OR_HEXHERO || REACTION_GAME_LEVEL == 0)
        {
            // in the easy level of sound mode, we show the lights.

            // show decimal point for digit corresponding with button
            setDecimalPoint(true, REACTION_GAME_TARGET);
            // set appropriate led per button
            lights |= 1 << lights_indexed[REACTION_GAME_TARGET];
        }

        // check player pressed a button.
        for (uint8_t i = 0; i < MOMENTARY_BUTTONS_COUNT; i++)
        {
            // button press
            if (binaryInputsEdgeUp & (1 << i))
            {
                if (i == REACTION_GAME_TARGET)
                {
                    //right button
                    REACTION_GAME_SCORE++;

                    if (REACTION_OPTION_WHACKABIRD_OR_HEXHERO)
                    {
                        // a small pause must be implemented after the button press before the new turn as off not to confuse the player
                        addNoteToBuzzerRepeated(REST_8_8, 4);
                        reactionGameState = reactionWaitBeforeNewTurn;
                    }
                    else
                    {
                        // addNoteToBuzzer(C4_8);
                        buzzerPlayApproval();
                        //addNoteToBuzzer(103 + i);
                        reactionGameState = reactionNewTurn;
                    }
                }
                else
                {
                    //wrong button
                    reactionGameState = reactionJustDied;

                    // zero points if you fail during the timed challenge
                    if (REACTION_OPTION_WHACKENDURANCE_OR_HEROPAUSE_OR_HEXCOMPLEMENT)
                    {
                        REACTION_GAME_SCORE = 0;
                    }
                }
            }
        }

        break;
    }

    case reactionJustDied:
    {
        // play death song
        addNoteToBuzzerRepeated(F4_1, 3);
#ifdef ENABLE_EEPROM
        //check for new high score and save
        // Serial.println("---");
        // Serial.println(EEPROM_REACTION_GAME_START_ADDRESS);
        // Serial.println(REACTION_GUITAR_APP_SELECT_HERO_ELSE_WHACKING_APP * 48);
        // Serial.println(REACTION_OPTION_WHACKABIRD_OR_HEXHERO * 24);
        // Serial.println(REACTION_OPTION_WHACKENDURANCE_OR_HEROPAUSE_OR_HEXCOMPLEMENT * 12);
        // Serial.println(REACTION_GAME_LEVEL);

        if (REACTION_GAME_SCORE > (int16_t)
                                      eeprom_read_word(reactionGameLevelToEepromAddress()))
        {
            eeprom_update_word(
                reactionGameLevelToEepromAddress(),
                REACTION_GAME_SCORE);

            loadBuzzerTrack(SONG_ATTACK);
        }
#endif

        // prepare next game delay.
        TIMER_REACTION_END_OF_GAME_DELAY.start(-2000);
        reactionGameState = reactionFinished;
    }
    break;

    case reactionFinished:
    {
        if (getCountDownTimerHasElapsed(&TIMER_REACTION_END_OF_GAME_DELAY))
        {
            //end of display score, next game
            reactionGameState = reactionNewGame;
        }
        else
        {
            //do nothing.  wait for display score is finished.
            if (TIMER_REACTION_END_OF_GAME_DELAY.getInFirstGivenHundredsPartOfSecond(500))
            {
            }
            else
            {
                ledDisp->setNumberToDisplayAsDecimal(REACTION_GAME_SCORE); //score display. Leave at beginning, to display high score blinking.
            }
        }
    }
    break;
    }

    // option buttons cannot be changed during the game. So, default lights on at button on is not feasable here for the options.
    // https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
    // lights ^= (-REACTION_OPTION_WHACKABIRD_OR_HEXHERO ^ lights) & (1UL << LIGHT_LATCHING_1);
    // lights ^= (-REACTION_OPTION_WHACKENDURANCE_OR_HEROPAUSE_OR_HEXCOMPLEMENT ^ lights) & (1UL << LIGHT_LATCHING_2);
    setButtonLight(LIGHT_LATCHING_1, REACTION_OPTION_WHACKABIRD_OR_HEXHERO);
    setButtonLight(LIGHT_LATCHING_2, REACTION_OPTION_WHACKENDURANCE_OR_HEROPAUSE_OR_HEXCOMPLEMENT);
    // lights &= ~(1<<lights_indexed[LIGHT_LATCHING_1] );
    // lights &= ~(1<<lights_indexed[LIGHT_LATCHING_2] );
    // lights |=  REACTION_OPTION_WHACKABIRD_OR_HEXHERO << lights_indexed[LIGHT_LATCHING_1];
    // lights |=  REACTION_OPTION_WHACKENDURANCE_OR_HEROPAUSE_OR_HEXCOMPLEMENT << lights_indexed[LIGHT_LATCHING_2];

#endif
}

bool Apps::stepChange(int16_t *counter, int8_t increment, int16_t minValue, int16_t maxValue, bool wrapAround)
{
    *counter += increment;
    checkBoundaries(counter, minValue, maxValue, wrapAround);
    return increment != 0;
}

void Apps::nextStep(int16_t *counter, bool countUpElseDown, int16_t minValue, int16_t maxValue, bool wrapAround)
{
    // *counter += -1 + (2 * countUpElseDown);
    int8_t increment = -1 + (2 * countUpElseDown);
    stepChange(counter, increment, minValue, maxValue, wrapAround);
    // countUpElseDown++;
    // checkBoundaries(counter, minValue, maxValue, wrapAround);
}

void Apps::nextStepRotate(int16_t *counter, bool countUpElseDown, int16_t minValue, int16_t maxValue)
{
    nextStep(counter, countUpElseDown, minValue, maxValue, true);
}

bool Apps::checkBoundaries(int16_t *counter, int16_t minValue, int16_t maxValue, bool rotate)
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
        return false;
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
        return false;
    }
    return true;
}

// uint8_t Apps::weightedRandomLetter(){
// 	// make a sum of all weights
// 	// get a random number between 0 and the sum
// 	// check in which interval the random number is
// 	// return index of interval
// 	uint16_t sum = 0;
// 	for (uint8_t i=0;i<26;i++){
// 		sum += pgm_read_byte_near(letter_frequency_table_english_dutch_ish + i);
// 	}

// 	uint16_t pick = random(0,sum);
// 	sum = 0;
// 	uint8_t i = 0;
// 	while (sum <= pick)
// 	{
// 		sum += pgm_read_byte_near(letter_frequency_table_english_dutch_ish + i);
// 		// Serial.println(sum);
// 		i ++;
// 	}
// 	return i-1;

// }

uint8_t Apps::weightedRandomLetter()
{
    // make a sum of all weights
    // get a random number between 0 and the sum
    // check in which interval the random number is
    // return index of interval
    uint16_t pick = 65535;
    while (true)
    {
        uint16_t sum = 0;
        for (uint8_t i = 0; i < 26; i++)
        {
            sum += pgm_read_byte_near(letter_frequency_table_english_dutch_ish + i);
            if (sum > pick)
            {
                return i;
            }
        }
        pick = random(0, sum);
    }
}

uint32_t Apps::fadeInList(uint8_t step, uint32_t startScreen, uint8_t *shuffledSequence)
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
        playSongHappyDryer();
        randomSequence(sequenceList, length);
        *indexVariable = length;
    }

    // draw card off deck
    return sequenceList[--(*indexVariable)];
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

    // load/save data
    uint8_t slot_number = encoder_dial->getValueLimited((slotCount - 1) * 16, false) / 16;

    if (this->encoder_dial->getDelta())
    {
        set_blink_offset();
    }

    //blink alternatively song number and "load" or "save"
    if (millis_blink_250_750ms())
    {
        lights |= 1 << LIGHT_MOMENTARY_0;
        lights |= 1 << LIGHT_LATCHING_3;
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
        {
            setStandardTextToTextHANDLE(TEXT_SAVE);
        }
        else
        {
            setStandardTextToTextHANDLE(TEXT_LOAD);
        }
        // textBufToDisplay();
    }
    else
    {
        ledDisp->setNumberToDisplayAsDecimal(slot_number);
    }

    if (binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0))
    {
        setStandardTextToTextHANDLE(TEXT_DONE);
    }

    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_MOMENTARY_0))
    {
        bool loadElseSave = !(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3));
        saveLoadFromEepromSlot(data, slot_number, eepromSlotLength, eepromStartAddress, loadElseSave);
        return loadElseSave;
    }

    return false;
}

void Apps::saveLoadFromEepromSlot(uint8_t *data, uint8_t slotIndex, uint8_t eepromSlotLength, uint16_t eepromStartAddress, boolean loadElseSave)
{
    // an eeprom slot in this context is a dedicated space of "eepromSlotLength" bytes.

    for (uint8_t i = 0; i < eepromSlotLength; i++)
    {
        uint8_t *eeprom_address = (uint8_t *)(eepromStartAddress +
                                              slotIndex * eepromSlotLength + i);

#ifdef ENABLE_EEPROM
        if (loadElseSave)
        {
            //load
            data[i] = eeprom_read_byte(eeprom_address);
        }
        else
        {
            //save
            eeprom_update_byte(eeprom_address, data[i]);
        }
#else
        if (loadElseSave)
        {
            data[i] = 111;
        }

#endif
    }
}

void Apps::displayAllSegmentsToScreen()
{
    ledDisp->setBinaryToDisplay(this->displayAllSegments);
}

void Apps::flashPictureToDisplayAllSegments(const uint8_t *progmemAddress)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        this->displayAllSegments |= (uint32_t)pgm_read_byte_near(progmemAddress + (i)) << (8 * i); //* 4 --> 4 bytes per dword
    }
}

void Apps::eepromPictureToDisplayAllSegments(int16_t pictureIndex)
{
    for (uint8_t i = 0; i < 4; i++)
    {
#ifdef ENABLE_EEPROM
        this->displayAllSegments |= (uint32_t)(eeprom_read_byte((uint8_t *)(EEPROM_PICTURES_START_ADDRESS + pictureIndex * 4 + i))) << (i * 8);
#else
        this->displayAllSegments |= (uint32_t)(111) << (i * 8);
#endif
    }
}

// void Apps::latching_3_blink(){
// 	// play button light blinking invitingly.
// 		if (millis_quarter_second_period())
// 		{
// 			lights |= 1 << LIGHT_LATCHING_3;
// 		}
// }
bool Apps::millis_second_period()
{
    return millis() % 1000 > 500;
}

bool Apps::millis_quarter_second_period()
{
    return millis() % 250 > 125;
}

bool Apps::millis_half_second_period()
{
    return millis() % 500 > 250;
}

bool Apps::millis_blink_250_750ms()
{
    // true for shorter part of the second
    return (millis() - blink_offset) % 1000 > 750;
}

void Apps::set_blink_offset()
{
    // keeps blink return to the same value. e.g. menu blinking between value and text. while dial rotates, we want to only see the value.
    blink_offset = millis() % 1000;
}

void Apps::textBufToDisplay()
{
    ledDisp->setTextBufToDisplay(textBuf);
}

void Apps::buzzerPlayApproval()
{
    this->buzzerSilentClearBufferAndAddNote(C7_8);
}

void Apps::buzzerPlayDisappointment()
{
    this->buzzerSilentClearBufferAndAddNote(C4_1);
}

void Apps::buzzerSilentClearBufferAndAddNoteAtEncoderDialChange(uint8_t note)
{
    if (encoder_dial->getDelta())
    {
        buzzerSilentClearBufferAndAddNote(note);
    }
}

void Apps::buzzerSilentClearBufferAndAddNote(uint8_t note)
{
    buzzerSilentClearBuffer();
    addNoteToBuzzer(note);
}

void Apps::addNoteToBuzzer(uint8_t note)
{
    buzzer->addNoteToNotesBuffer(note);
}

void Apps::addNoteToBuzzerRepeated(uint8_t note, uint8_t repeater)
{
    for (uint8_t i = 0; i < repeater; i++)
    {
        this->addNoteToBuzzer(note);
    }
}
void Apps::noteToDisplay(uint8_t note)
{
    buzzer->noteToDisplay(textHandle, decimalDotsHandle, note);
}

void Apps::initiateCountDowntimerWith500Millis(SuperTimer *pTimer)
{
    pTimer->start(-500);
}

bool Apps::getCountDownTimerHasElapsed(SuperTimer *timerToBlink)
{
    return !timerToBlink->getTimeIsNegative();
}

void Apps::displayTimerSecondsBlinker(SuperTimer *ptimerToBlink)
{
    setDecimalPoint(ptimerToBlink->getSecondsBlinker(), 1);
}

void Apps::setDecimalPoint(bool onElseOff, uint8_t digit)
{
    ledDisp->setDecimalPointToDisplay(onElseOff, digit);
}

void Apps::setButtonLight(uint8_t button_light, bool onElseOff)
{
    // light = button light i.e. LIGHT_LATCHING_2
    // https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
    lights ^= (-onElseOff ^ lights) & (1UL << button_light);
}

void Apps::setBlankDisplay()
{
    ledDisp->setBlankDisplay();
}
// void Apps::setLedArray()
// {
// ledDisp->setLedArray(lights);
// }
void Apps::textBufToDisplayAllSegments()
{
    ledDisp->convert_text4Bytes_to_32bits(textBuf, &displayAllSegments);
}
void Apps::buzzerSilentClearBuffer()
{
    buzzer->buzzerSilentClearBuffer(); // stop all sounds that were playing in an app.
}

void Apps::buzzerChangeSpeedRatioWithEncoderDial()
{
    buzzer->changeSpeedRatio(encoder_dial->getDelta());
}

uint16_t Apps::dialGetIndexedtime()
{
#ifdef ENABLE_MULTITIMER_STANDALONE_DEPRECATED
    return this->multiTimer.getIndexedTime(encoder_dial->getValueLimited(90, false));
#elif defined ENABLE_MULTITIMER_INTEGRATED

    return indexToTimeSeconds(encoder_dial->getValueLimited(90, false));
#else
    return encoder_dial->getValueLimited(60, false) * 30;
#endif
}

unsigned int Apps::indexToTimeSeconds(int16_t index)
{
#ifdef ENABLE_MULTITIMER_STANDALONE_DEPRECATED
    return this->multiTimer.getIndexedTime(index);

#elif defined ENABLE_MULTITIMER_INTEGRATED
    return timeDialDiscreteSeconds[index];
#else
    return index * index; // untested
#endif
}

void Apps::setStandardTextToTextBuf(uint8_t textPosition)
{
    // set text to a buffer, to be called later onwards.
    ledDisp->setStandardTextToTextBuf(textBuf, textPosition);
}

void Apps::setStandardTextToTextHANDLE(uint8_t textPosition)
{
    // display text on display instantly.
    ledDisp->setStandardTextToTextBuf(textHandle, textPosition);
}

// does not decrease footprint!
// bool  Apps::isNoMomentaryButtonOn(){
// 	return !(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_0)) &&
// 			!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_1)) &&
// 			!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_2)) &&
// 			!(binaryInputsValue & (1 << BUTTON_INDEXED_MOMENTARY_3))
// }

void Apps::numberToBufAsDecimal(int16_t number)
{
    ledDisp->numberToBufAsDecimal(textBuf, number);
}

void Apps::playSongHappyDryer()
{
    loadBuzzerTrack(SONG_DRYER_HAPPY);
}

void Apps::loadBuzzerTrack(uint8_t songIndex)
{
    uint8_t length = 0;
    uint8_t song_start_index = 0;

    buzzerSilentClearBuffer();

    if (songIndex < SONGS_FLASH_COUNT)
    {
        // Flash memory

        // start index is all lenghts from previous songs counted up
        for (uint8_t i = 0; i <= songIndex; i++)
        {
            song_start_index += length;
            length = pgm_read_byte_near(song_lengths + i);
        }
        progmemToBuffer(songs + song_start_index, length);
    }

#ifdef ENABLE_EEPROM
    else if (songIndex - SONGS_FLASH_COUNT < 4)
    {
        // eeprom composer
        saveLoadFromEepromSlot(this->bytes_list, songIndex - SONGS_FLASH_COUNT, EEPROM_COMPOSER_SONG_LENGTH, EEPROM_COMPOSER_SONGS_START_ADDRESS, true);
        length = 20;
    }
    else if (songIndex - SONGS_FLASH_COUNT - EEPROM_COMPOSER_SONG_COUNT < EEPROM_SEQUENCER_SONGS_COUNT)
    {
        // eeprom sequencer
        saveLoadFromEepromSlot(this->bytes_list, songIndex - SONGS_FLASH_COUNT - EEPROM_COMPOSER_SONG_COUNT, EEPROM_SEQUENCER_SONG_LENGTH, EEPROM_SEQUENCER_SONGS_START_ADDRESS, true);

        length = EEPROM_SEQUENCER_SONG_LENGTH * 2;

        int8_t i = EEPROM_SEQUENCER_SONG_LENGTH - 1;

        while (i > 0)
        {
            this->bytes_list[i * 2] = this->bytes_list[i] % 64; // we have to introduce a hack here. All notes to the same timing. Otherwise, the rythm is off. Theoretically, I should add rests until all notes are filled up to the same length, but, it didn't happen, as the buffe is simply not big enough for the worst case scenario. i.e. 1/8th of a note --> add 7 times 1/8th rest.  times 32 = 256notes in buffer . buffer is not long enough.
            this->bytes_list[i * 2 - 1] = REST_2_8;
            i--;
        }
    }
#endif

    for (uint8_t i = 0; i < length; i++)
    {
        buzzer->addNoteToNotesBuffer(this->bytes_list[i]);
    }
}

uint8_t Apps::progmemToBufferUntil(const uint8_t *offset, uint8_t stopConditionValue)
{
    // max length = 255.
    // move from progmem to universal bytes buffer in ram until a value
    // warning: MAKE SURE THERE IS A STOP! Or it will continue ~forever~
    // return length including stopcondition.
    uint8_t i = 0;
    do
    {
        this->bytes_list[i] = pgm_read_byte_near(offset + i);
        i++;

    } while (this->bytes_list[i] != stopConditionValue);

    return i;
}

void Apps::progmemToBuffer(const uint8_t *offset, uint8_t length)
{
    // move from progmem to universal bytes buffer in ram.
    for (uint8_t i = 0; i < length; i++)
    {
        this->bytes_list[i] = pgm_read_byte_near(offset + i);
    }
}

// void Apps::fillArrayWithZero(uint8_t* arr, uint8_t length){
void Apps::fill8BytesArrayWithZero()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        array_8_bytes[i] = 0;
    }
}

#ifdef ENABLE_MULTITIMER_STANDALONE_DEPRECATED
void Apps::miniMultiTimer()
{
    // every player: init time, time left, alive?
    // game: pause, player alive? ,fischertimer active?/time, random starter

    if (this->app_init_edge)
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
    if (binaryInputs[BUTTON_LATCHING_0].getEdgeUp())
    {
        this->multiTimer.start();
    }
    if (binaryInputs[BUTTON_LATCHING_0].getEdgeDown())
    {
        this->multiTimer.init();
    }

    // PAUSE Switch
    this->multiTimer.setStatePause(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3)); // do not only work on edge here, as latching switch can  be in any state.

    // # set number of timers SWITCH
    this->multiTimer.setStateTimersCount(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1)); // do not only work on edge here, as latching switch can  be in any state.

    // set fischer timer SWITCH
    this->multiTimer.setStateFischerTimer(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2)); // do not only work on edge here, as latching switch can  be in any state.

    // THE DIAL
    if (encoder_dial->getDelta())
    {
        // number of timers
        int16_t encoder_mapped = encoder_dial->getValueLimited(90, false);

        this->multiTimer.setTimersCount((uint8_t)((float)encoder_mapped / 25) + 1);
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
    (LIGHT_PAUSE & settingsLights) ? lights |= 1 << LIGHT_LATCHING_3 : false;
    (LIGHT_PLAYING & settingsLights) ? lights |= 1 << LIGHT_LATCHING_0 : false;
    (LIGHT_FISCHER & settingsLights) ? lights |= 1 << LIGHT_LATCHING_2 : false;
    (LIGHT_SET_TIMERS_COUNT & settingsLights) ? lights |= 1 << LIGHT_LATCHING_1 : false;
    this->lights = lights;
    setDecimalPoint(LIGHT_SECONDS_BLINKER & settingsLights, 1);
}
#endif

#ifdef ENABLE_MULTITIMER_INTEGRATED
void Apps::multitimer_integrated()
{
    if (this->app_init_edge)
    {
        this->multitimer_setDefaults();
        this->multitimer_init();
    }

    // TIMER BUTTONS
    for (uint8_t i = 0; i < MULTITIMER_MAX_TIMERS_COUNT; i++)
    {
        if (binaryInputsEdgeUp & (1 << i))
        {
            this->multitimer_playerButtonPressEdgeUp(i);
        }
        if (binaryInputsEdgeDown & (1 << i))
        {
            // this->multitimer_playerButtonPressEdgeDown(i);
            this->multitimer_timerDisplayed = this->multitimer_activeTimer;
        }
    }

    // START STOP Button

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
    if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_LATCHING_3) || (app_init_edge && binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3)))
    {
        this->multitimer_start();
    }
    if (binaryInputsEdgeDown & (1 << BUTTON_INDEXED_LATCHING_3))
    {
        this->multitimer_init();
    }
#else
        if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_LATCHING_3))
        {
            if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_3))
            {
                this->multitimer_start();
            }
            else
            {
                this->multitimer_init();
            }
        }
#endif

    // PAUSE Switch
    // this->multitimer_setStatePause(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2)); // do not only work on edge here, as latching switch can  be in any state.

    // if (this->multitimer_state == initialized)
    // {
    // 	this->multitimer_randomStarter = set;
    // }
    // else if (set && this->multitimer_state == playing)
    // {
    // 	this->multitimer_pause();
    // }
    // else if (!set && this->multitimer_state == paused)
    // {
    // this->multitimer_continu();binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2
    // }

    // UPDATE CYCLIC
    this->multitimer_refresh();
}

void Apps::multitimer_setDefaults()
{
#ifdef ENABLE_EEPROM
    // no defaults, load eeprom values. at eeprom init, it's all zero...
    MULTITIMER_FISCHER_TIME_INDEX = eeprom_read_byte((uint8_t *)EEPROM_MULTITIMER_FISHER_TIME_INDEX);
    // general init
    MULTITIMER_TIMERS_COUNT = (int16_t)eeprom_read_byte((uint8_t *)EEPROM_MULTITIMER_TIMERS_COUNT);
    for (uint8_t i = 0; i < MULTITIMER_MAX_TIMERS_COUNT; i++)
    {
        this->multitimer_setTimerInitCountTimeByTimeIndex(i, eeprom_read_byte((uint8_t *)EEPROM_MULTITIMER_TIMERS_INIT_TIME_START_INDEX + i));
    }
#else
        MULTITIMER_FISCHER_TIME_INDEX = 0;
        // general init
        MULTITIMER_TIMERS_COUNT = 2;
        for (uint8_t i = 0; i < MULTITIMER_MAX_TIMERS_COUNT; i++)
        {
            this->multitimer_setTimerInitCountTimeByTimeIndex(i, 35);
        }
#endif

    this->multitimer_state = initialized;
    this->multitimer_activeTimer = 0;
    this->multitimer_timerDisplayed = this->multitimer_activeTimer;
}

// void Apps::multitimer_setTimersCount(int8_t delta)
// {
// 	// delta > 0: increase, else decrease.
// 	if (this->multitimer_state == initialized)
// 	{
// 		nextStepRotate(&MULTITIMER_TIMERS_COUNT, delta > 0, 1, MULTITIMER_MAX_TIMERS_COUNT);
// 		this->multitimer_activeTimer = 0;
// 		this->multitimer_timerDisplayed = this->multitimer_activeTimer;
// 	}
// }

uint8_t Apps::multitimer_getTimerInitTimeIndex(uint8_t timer)
{
    return MULTITIMER_INIT_TIME_INDECES[timer];
}

void Apps::multitimer_setTimerInitCountTimeByTimeIndex(uint8_t timer, uint8_t index)
{
    MULTITIMER_INIT_TIME_INDECES[timer] = index;
    this->multitimer_timers[timer].setInitCountDownTimeSecs(indexToTimeSeconds(index));
}

void Apps::multitimer_init()
{
    for (uint8_t i = 0; i < MULTITIMER_MAX_TIMERS_COUNT; i++)
    {
        this->multitimer_timers[i].reset();
    }
    this->multitimer_state = initialized;

    // specific
    this->multitimer_activeTimer = 0;
}

// void Apps::multitimer_playerButtonPressEdgeDown(uint8_t index)
// {
// 	// if button released, always display active Timer time.
// 	this->multitimer_timerDisplayed = this->multitimer_activeTimer;
// }

void Apps::multitimer_playerButtonPressEdgeUp(uint8_t index)
{
    // every timer index is linked to a button index.

    if (this->multitimer_state == initialized)
    {
        if ((index + 1) <= MULTITIMER_TIMERS_COUNT)
        {
            this->multitimer_activeTimer = index;
            this->multitimer_timerDisplayed = this->multitimer_activeTimer;
        }
    }
    else if (this->multitimer_state == playing)
    {
        if (this->multitimer_activeTimer == index)
        {
            this->multitimer_next(false);
            // buzzerSilentClearBufferAndAddNote(35);
            buzzerPlayApproval();
        }
        else if ((index + 1) <= MULTITIMER_TIMERS_COUNT)
        {
            // buzzerSilentClearBufferAndAddNote(129);
            buzzerPlayDisappointment();              // althought, good to check time, it also acts as a warning that this is not your button to press
            this->multitimer_timerDisplayed = index; //display time of pressed timer button
        }
    }
    else if (this->multitimer_state == paused)
    {
        buzzerSilentClearBufferAndAddNote(230);
        this->multitimer_timerDisplayed = index; //display time of pressed timer button
    }
}

// void Apps::multitimer_setStatePause(bool set)
// {
// 	// pause button is latching

// 	if (this->multitimer_state == initialized)
// 	{
// 		this->multitimer_randomStarter = set;
// 	}
// 	else if (set && this->multitimer_state == playing)
// 	{
// 		this->multitimer_pause();
// 	}
// 	else if (!set && this->multitimer_state == paused)
// 	{
// 		this->multitimer_continu();
// 	}
// }

void Apps::multitimer_start()
{
#ifdef ENABLE_EEPROM
    // it makes sense to store settings into eeprom at start
    eeprom_update_byte((uint8_t *)EEPROM_MULTITIMER_FISHER_TIME_INDEX, MULTITIMER_FISCHER_TIME_INDEX);
    eeprom_update_byte((uint8_t *)EEPROM_MULTITIMER_TIMERS_COUNT, (uint8_t)MULTITIMER_TIMERS_COUNT);
    for (uint8_t i = 0; i < MULTITIMER_MAX_TIMERS_COUNT; i++)
    {
        eeprom_update_byte((uint8_t *)EEPROM_MULTITIMER_TIMERS_INIT_TIME_START_INDEX + i, this->multitimer_getTimerInitTimeIndex(i));
    }
#endif

    //start and pause all timers.
    for (uint8_t i = 0; i < MULTITIMER_TIMERS_COUNT; i++)
    {
        this->multitimer_timers[i].start();
        this->multitimer_timers[i].pause();
    }

    // this is the moment we chose a random starting timer if enabled.
    if (this->multitimer_randomStarter)
    {
        // randomSeed(millis());
        this->multitimer_activeTimer = (uint8_t)random(0, (long)MULTITIMER_TIMERS_COUNT);
    }

    // activate the starting timer
    this->multitimer_timers[this->multitimer_activeTimer].continu();
    this->multitimer_state = playing;
    this->multitimer_timerDisplayed = this->multitimer_activeTimer;
}

void Apps::multitimer_pause()
{
    this->multitimer_state = paused;
    this->multitimer_timers[this->multitimer_activeTimer].pause();
}

void Apps::multitimer_continu()
{
    this->multitimer_state = playing;
    this->multitimer_timers[this->multitimer_activeTimer].continu();
}

void Apps::multitimer_buzzerRefresh(bool alarm)
{
    if (this->multitimer_timers[this->multitimer_activeTimer].getEdgeSinceLastCallFirstGivenHundredsPartOfSecond(100, true, false))
    {
        if (alarm)
        {
            uint8_t tmp = random(20, 50);
            for (uint8_t i = 0; i < 5; i++)
            {
                addNoteToBuzzer(tmp);

                addNoteToBuzzer(REST_2_8);
            }
        }

        if (this->multitimer_timers[this->multitimer_activeTimer].getTimeSecondsAbsolute() < 11 && (!getCountDownTimerHasElapsed(&(multitimer_timers[this->multitimer_activeTimer]))))
        {
            // check for last ten seconds of countdown timer
            addNoteToBuzzer(34 + this->multitimer_timers[this->multitimer_activeTimer].getTimeSecondsAbsolute());
        }

        // at every minute a nice beep plays~
        if (this->multitimer_timers[this->multitimer_activeTimer].getTimeSecondsAbsolute() % 60 == 0)
        {
            // addNoteToBuzzer(44);
            buzzerPlayApproval();
        }
    }
}

void Apps::multitimer_refresh()
{
    //what should be showing on the display right now?

    uint8_t playerLights = 0;            //lsb is timer 0, 2nd bit is timer 1, ....
    uint8_t settingsLights = 0b00000000; //I tried optimizing this away, but memory size increased... Settings lights are other lights then timer button lights.

    if (this->multitimer_state == initialized)
    {
        this->multitimer_randomStarter = binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2);

#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1))
        {
            this->multitimer_state = setFischer;
        }
#else
            if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_LATCHING_1))
            {
                this->multitimer_state = setFischer;
            }

#endif

        if (encoder_dial->getDelta() != 0)
        {
            byte momentary_buttons_mask = 1 << BUTTON_INDEXED_MOMENTARY_0 | 1 << BUTTON_INDEXED_MOMENTARY_1 | 1 << BUTTON_INDEXED_MOMENTARY_2 | 1 << BUTTON_INDEXED_MOMENTARY_3;
            if ((binaryInputsValue & momentary_buttons_mask) == 0)
            {
                // set number of timers
                MODE_MULTITIMER_SET_COUNTER_COUNT_SENSITIVITY++; // hack to decrease sensitivity of encoder dial for counter setting only.
                if (MODE_MULTITIMER_SET_COUNTER_COUNT_SENSITIVITY > 10)
                {
                    stepChange(&MULTITIMER_TIMERS_COUNT, this->encoder_dial->getDelta(), 1, MULTITIMER_MAX_TIMERS_COUNT, false);
                    this->multitimer_activeTimer = 0;
                    this->multitimer_timerDisplayed = this->multitimer_activeTimer;

                    MODE_MULTITIMER_SET_COUNTER_COUNT_SENSITIVITY = 0;
                }
            }
            else
            {
                // set time for each timerbutton pressed

                encoderDialRefreshTimeIndex(&MULTITIMER_DIAL_TIME_INDEX);
                for (uint8_t i = 0; i < MULTITIMER_MAX_TIMERS_COUNT; i++)
                {
                    if (binaryInputsValue & (1 << i))
                    {
                        this->multitimer_setTimerInitCountTimeByTimeIndex(i, MULTITIMER_DIAL_TIME_INDEX);
                    }
                }
            }
        }

        this->multitimer_timers[this->multitimer_activeTimer].getTimeString(textHandle);

        for (uint8_t i = 0; i < MULTITIMER_TIMERS_COUNT; i++)
        {
            if (this->multitimer_randomStarter)
            {
                if (millis_quarter_second_period())
                {
                    // at random starter, all lights blinking
                    playerLights |= 1 << i;
                }
            }
            else if (i != this->multitimer_activeTimer || millis_quarter_second_period())
            {
                // active timer is blinking. others are solid on.
                playerLights |= 1 << i;
            }
        }

        if (millis_half_second_period())
        {
            settingsLights |= MULTITIMER_LIGHT_PLAYING;
        }
        if (this->multitimer_randomStarter)
        {
            // pause light blinking.
            if (millis_quarter_second_period())
            {
                settingsLights |= MULTITIMER_LIGHT_PAUSE; //pause light on.
            }
        }
        settingsLights |= MULTITIMER_LIGHT_SECONDS_BLINKER;
    }
    else if (this->multitimer_state == playing)
    {
        // never auto power off when timer is on
        resetInactivityTimer();

        if (binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2))
        {
            this->multitimer_pause();
        }

        //check all timers elapsed
        if (this->multitimer_checkAllTimersFinished())
        {
            this->multitimer_state = finished;
        }
        else
        {
            multitimer_buzzerRefresh(false);

            //check active timer time elapsed
            if (this->multitimer_getTimerFinished(this->multitimer_activeTimer))
            {
                this->multitimer_next(true);
            }
        }

        // displayed timer is not always the active timer (i.e. non active player wants to check his time).
        this->multitimer_timers[this->multitimer_timerDisplayed].getTimeString(textHandle);

        // run through all timers to set lights
        for (uint8_t i = 0; i < MULTITIMER_TIMERS_COUNT; i++)
        {
            if (i == this->multitimer_activeTimer)
            {
                // active timer seconds blink
                if (this->multitimer_timers[this->multitimer_activeTimer].getInFirstGivenHundredsPartOfSecond(500))
                {
                    playerLights |= 1 << i;

                    // blinking behaviour of decimal point
                    settingsLights |= MULTITIMER_LIGHT_SECONDS_BLINKER;
                }
            }
            else if (i == this->multitimer_timerDisplayed)
            {
                // displayed timer is not always the active timer (i.e. non active player wants to check his time).
                if (millis_quarter_second_period())
                {
                    playerLights |= 1 << i;
                }

                // solid seconds blinker when displaying other timer
                settingsLights |= MULTITIMER_LIGHT_SECONDS_BLINKER;
            }
            else if (i != this->multitimer_activeTimer && !this->multitimer_getTimerFinished(i))
            {
                //other lights solid on if still alive.
                playerLights |= 1 << i;
            }
        }

        // settingsLights |= MULTITIMER_LIGHT_PLAYING; //After testing: do not switch on while playing. People press it and it screws up the game (reset)when in timers running mode, solid on.
        settingsLights |= MULTITIMER_LIGHT_PAUSE; //After testing: do not switch on while playing. People press it and it screws up the game (reset)when in timers running mode, solid on.
    }
    else if (this->multitimer_state == finished)
    {
        multitimer_buzzerRefresh(MULTITIMER_TIMERS_COUNT == 1); // alarm will sound if it was only one player.

        // last surviving timer is now a chrono for displaying time since end.
        if (this->multitimer_timers[this->multitimer_activeTimer].getInFirstGivenHundredsPartOfSecond(500))
        {
            this->multitimer_timers[this->multitimer_activeTimer].getTimeString(textHandle);
            settingsLights |= MULTITIMER_LIGHT_SECONDS_BLINKER;
        }
        else
        {
            setStandardTextToTextHANDLE(TEXT_END);
        }
        //fast blink last surviving timer light.
        if (millis_quarter_second_period())
        {
            playerLights |= 1 << this->multitimer_activeTimer;
        }
    }
    else if (this->multitimer_state == paused)
    {

        if (!(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_2)))
        {
            this->multitimer_continu();
        }

        if (millis_blink_250_750ms() || this->multitimer_timerDisplayed != this->multitimer_activeTimer)
        {
            // if other timer than the active timer, show always.
            this->multitimer_timers[this->multitimer_timerDisplayed].getTimeString(textHandle);
        }
        else
        {
            setStandardTextToTextHANDLE(TEXT_PAUS);
        }

        // timer lights
        for (uint8_t i = 0; i < MULTITIMER_TIMERS_COUNT; i++)
        {
            //other lights solid on if still alive.
            if (i == this->multitimer_activeTimer)
            {
                if (millis_quarter_second_period())
                {
                    playerLights |= 1 << i;
                }
            }
            else if (i == this->multitimer_timerDisplayed)
            {
                // displayed timer is not always the active timer (i.e. non active player wants to check his time).
                if (millis_quarter_second_period())
                {
                    playerLights |= 1 << i;
                }
            }
            else if (!this->multitimer_getTimerFinished(i))
            {
                playerLights |= 1 << i;
            }
        }
        // settings lights
        // pause light blinking.
        if (millis_quarter_second_period())
        {
            settingsLights |= MULTITIMER_LIGHT_PAUSE; //pause light on.
        }
        // playing light on.
        // settingsLights |= MULTITIMER_LIGHT_PLAYING; //when in timers running mode, solid on.

        settingsLights |= MULTITIMER_LIGHT_SECONDS_BLINKER;
    }
    else if (this->multitimer_state == setFischer)
    {
#ifdef ENABLE_SELECT_APPS_WITH_SELECTOR
        if (!(binaryInputsToggleValue & (1 << BUTTON_INDEXED_LATCHING_1)))
        {
            this->multitimer_state = initialized;
        }
#else
            if (binaryInputsEdgeUp & (1 << BUTTON_INDEXED_LATCHING_1))
            {
                this->multitimer_state = initialized;
            }
#endif

        // fischer timer
        if (encoder_dial->getDelta() != 0)
        {
            encoderDialRefreshTimeIndex(&MULTITIMER_FISCHER_TIME_INDEX);
            set_blink_offset();
        }

        if (millis_blink_250_750ms())
        {
            setStandardTextToTextHANDLE(TEXT_FISH);
        }
        else
        {
            timeSecondsToClockString(textHandle, indexToTimeSeconds(MULTITIMER_FISCHER_TIME_INDEX));
        }

        if (millis_quarter_second_period())
        {
            settingsLights |= MULTITIMER_LIGHT_FISCHER;
        }
        settingsLights |= MULTITIMER_LIGHT_SECONDS_BLINKER;
    }

    // settings lights exceptions
    if (this->multitimer_state != setFischer && MULTITIMER_FISCHER_TIME_INDEX != 0)
    {
        // fischer light always solid on when not zero seconds added. (except during setting, then blinking).
        settingsLights |= MULTITIMER_LIGHT_FISCHER;
    }

    this->lights = 0x0;
    // timer buttons lights to real lights
    for (uint8_t i = 0; i < 4; i++)
    {
        if (1 << i & playerLights)
        {
            lights |= 1 << lights_indexed[i];
        }
    }

    // settings light to real lights (it would look like you could optimize this away, but I tried, and it didn't do anything!)
    (MULTITIMER_LIGHT_PAUSE & settingsLights) ? lights |= 1 << LIGHT_LATCHING_2 : false;
    (MULTITIMER_LIGHT_PLAYING & settingsLights) ? lights |= 1 << LIGHT_LATCHING_3 : false;
    (MULTITIMER_LIGHT_FISCHER & settingsLights) ? lights |= 1 << LIGHT_LATCHING_1 : false;

    setDecimalPoint(MULTITIMER_LIGHT_SECONDS_BLINKER & settingsLights, 1); // "hour:seconds" divider
}

bool Apps::multitimer_getTimerFinished(uint8_t timerIndex)
{
    // return !this->multitimer_timers[timerIndex].getTimeIsNegative();
    return getCountDownTimerHasElapsed(&multitimer_timers[timerIndex]);
}

bool Apps::multitimer_checkAllTimersFinished()
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MULTITIMER_TIMERS_COUNT; i++)
    {
        this->multitimer_getTimerFinished(i) ? count++ : count += 0;
    }

    return count == MULTITIMER_TIMERS_COUNT;
}

void Apps::multitimer_next(bool activePlayerDied)
{
    // don't check for everybody dead here, check at refresh where next is called.

    if (this->multitimer_state == playing)
    {
        this->multitimer_timers[this->multitimer_activeTimer].pause();

        if (!activePlayerDied)
        {
            // add fischer timer (disabled just means: zero seconds).
            this->multitimer_timers[this->multitimer_activeTimer].setOffsetInitTimeMillis(-1000 * (long)(indexToTimeSeconds(MULTITIMER_FISCHER_TIME_INDEX)));
            // if time bigger than initial time, just reset timer. It should not be bigger. I got this from boardgamearena.com. I kindof liked it.
            if (this->multitimer_timers[this->multitimer_activeTimer].getInitTimeMillis() >= this->multitimer_timers[this->multitimer_activeTimer].getTimeMillis())
            {
                this->multitimer_timers[this->multitimer_activeTimer].startPaused(true);
            }
        }

        // Sometimes went into an infinite while loop,
        int test_infinite_loop = 0;
        do
        {
            this->multitimer_activeTimer >= (MULTITIMER_TIMERS_COUNT - 1) ? this->multitimer_activeTimer = 0 : this->multitimer_activeTimer++;
            test_infinite_loop++;
        } while (this->multitimer_getTimerFinished(this->multitimer_activeTimer) //if finished go to next timer.
                 && test_infinite_loop < 6);

        this->multitimer_timers[this->multitimer_activeTimer].continu();
        this->multitimer_timerDisplayed = this->multitimer_activeTimer;
    }
}

#endif