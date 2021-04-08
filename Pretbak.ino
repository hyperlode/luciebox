#include "PretbakSettings.h"
#include "DisplayDigitsHandler5Digits.h"
#include "LedMultiplexer5x8.h"
#include "PotentioSelector.h"
#include "ButtonsDacR2r.h"
#include "BinaryInput.h"
#include "Buzzer.h"
#include "RotaryEncoderDial.h"

#ifdef ENABLE_APPS
#include "Apps.h"
#endif

// Lode Ameije 2019-05
// Pretbak is a busy box for my newly born niece
// The hardware is a box with:
// input: 4 momentary buttons, 4 latching buttons, an encoder dial, a selector dial, and optionally four mercury switches.
// output: lights on buttons (one light per button), 7seg display (4 digits), buzzer

uint16_t setValues_1[BUTTONS_MOMENTARY_COUNT] = BUTTONS_MOMENTARY_VALUES;
uint16_t setValues_2[BUTTONS_LATCHING_COUNT] = BUTTONS_LATCHING_VALUES;

#ifdef ENABLE_TILT_SWITCHES
uint16_t setValues_mercury[MERCURY_SWITCHES_COUNT] = MERCURY_SWITCHES_VALUES;
#endif

//pretbak
#ifdef ENABLE_APPS
Apps pretbak_apps;
#endif

// INPUT
BinaryInput binaryInputs[BINARY_INPUTS_COUNT];
PotentioSelector selectorDial;
ButtonsDacR2r buttonsMomentary;
ButtonsDacR2r buttonsLatching; // buttons without normally closed. this is a problem for the R-2R ladder. instead, I used a pull down resistor to ground at the switch. so: ON = 5V, OFF = GND over 1Kohm. 10K, 20K R2Rladder.  will only work for limited number of buttons.
#ifdef ENABLE_TILT_SWITCHES
ButtonsDacR2r mercurySwitches; // mercury switches go on or off depending on the position. Works with R-2R ladder. No NC in mercury switch, so, pulldown resistor (0.1R)to ground. R=10K
#endif
RotaryEncoderDial encoder_dial;

// OUTPUT
DisplayManagement visualsManager;
LedMultiplexer5x8 ledDisplay;
Buzzer buzzer;

bool aMomentaryButtonIsPressed = false;

// hack to get interrupts nicely going without too much magic.
// https://forum.arduino.cc/index.php?topic=41713.0
// interrupts are use for the encoder dial
void isr0()
{
    encoder_dial.interruptChannelA();
}
void isr1()
{
    encoder_dial.interruptChannelB();
}

void lucieboxLoop()
{
    //update inputs
    processInput();

#ifdef DEBUG_BUTTONS
    for (uint8_t i = 0; i < BINARY_INPUTS_COUNT; i++)
    {
        if (binaryInputs[i].getEdgeUp())
        {
            Serial.println("{{{-------PRESS:-------:");
            Serial.println(i);
            Serial.println((analogRead(PIN_BUTTONS_MOMENTARY)));
            Serial.println((analogRead(PIN_BUTTONS_LATCHING)));
            //Serial.println(buttonsMomentary.getButtonsValueRaw(),BIN);
            //Serial.println(buttonsLatching.getButtonsValueRaw(),BIN);
        }

        if (binaryInputs[i].getEdgeDown())
        {
            Serial.println("-----RELEASE:");
            Serial.println(i);
            //Serial.println( (analogRead(PIN_BUTTONS_MOMENTARY)));
            //Serial.println(buttonsMomentary.getButtonsValueRaw(),BIN);
        }
    }
#endif

    //Serial.println("papa help "); // display this PAPA - HELP - PAPA - ... on screen when errors.

#ifdef DEBUG_SELECTOR_KNOB
    //rotary 12 positions selector knob is taken as base for mode selecion. so there are 12 states.
    if (selectorDial.getValueChangedEdge())
    {
        //Serial.println(SELECTOR_DIAL_POSITIONS);
        Serial.println("selector:");
        Serial.println(selectorDial.getSelectorValue());
        Serial.println(analogRead(PIN_SELECTOR_DIAL));
    }
#endif

#ifdef ENABLE_APPS
    pretbak_apps.appSelector();
#endif

    processOuput();
}

void processInput()
{
    // process edges from binaryInputs here, think of it as the previous cycle (done before new input processed.)
    for (uint8_t i = 0; i < BINARY_INPUTS_COUNT; i++)
    {
        binaryInputs[i].refresh();
    }

    selectorDial.refresh();
    buttonsMomentary.refresh();
    buttonsLatching.refresh();
#ifdef ENABLE_TILT_SWITCHES
    mercurySwitches.refresh();
#endif
    encoder_dial.refresh();

    if (buttonsLatching.getValueChangedEdge())
    {
        for (uint8_t i = 0; i < BUTTONS_LATCHING_COUNT; i++)
        {
            binaryInputs[BUTTONS_LATCHING_TO_BINARY_INPUT_OFFSET + i].setValue(buttonsLatching.getButtonValueByIndex(i));
        }
    }

    if (buttonsMomentary.getValueChangedEdge())
    {
        if (!aMomentaryButtonIsPressed)
        {
            // add 1 to power cycles tally keeper. It's done a key press to avoid gigantic number in case of multiple brown outs.
            eeprom_update_word((uint16_t *)EEPROM_LUCIEBOX_POWER_CYCLE_COUNTER, eeprom_read_word((uint16_t *)EEPROM_LUCIEBOX_POWER_CYCLE_COUNTER) + 1);
            aMomentaryButtonIsPressed = true;
        }

        for (uint8_t i = 0; i < BUTTONS_MOMENTARY_COUNT; i++)
        {
            binaryInputs[BUTTONS_MOMENTARY_TO_BINARY_INPUT_OFFSET + i].setValue(buttonsMomentary.getButtonValueByIndex(i));
        }
    }

#ifdef ENABLE_TILT_SWITCHES
    if (mercurySwitches.getValueChangedEdge())
    {
#ifdef DEBUG_MERCURY
        Serial.println("analog in mercury switches:");
        Serial.println(analogRead(A4));
        Serial.println(mercurySwitches.getButtonsValueRaw(), BIN);
        Serial.println(mercurySwitches.getButtonsValue());
#endif

        for (uint8_t i = 0; i < MERCURY_SWITCHES_COUNT; i++)
        {
#ifdef DEBUG_MERCURY
            Serial.println(MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET);
            Serial.println(i);
#endif
            binaryInputs[MERCURY_SWITCHES_TO_BINARY_INPUT_OFFSET + i].setValue(mercurySwitches.getButtonValueByIndex(i));
        }
    }
#endif
}

void processOuput()
{
    buzzer.doBuzzerRoll();
    visualsManager.refresh();
    ledDisplay.refresh();
}

void setup()
{
    // trick to use the easy arduino libary
    // instead of complicated native functions
    attachInterrupt(0, isr0, CHANGE);
    attachInterrupt(1, isr1, CHANGE);

#ifdef ENABLE_SERIAL
    Serial.begin(9600);
#endif

    selectorDial.initialize(PIN_SELECTOR_DIAL, SELECTOR_DIAL_POSITIONS);
    buttonsMomentary.setPin(PIN_BUTTONS_MOMENTARY, BUTTONS_MOMENTARY_COUNT, setValues_1);
    buttonsLatching.setPin(PIN_BUTTONS_LATCHING, BUTTONS_LATCHING_COUNT, setValues_2);
    encoder_dial.setPins(PIN_ROTARY_ENCODER_DIAL_CHANNEL_A, PIN_ROTARY_ENCODER_DIAL_CHANNEL_B);
    ledDisplay.Begin(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_BUTTON_LIGHTS, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);
    visualsManager.setMultiplexerBuffer(ledDisplay.getDigits());
#ifdef ENABLE_TILT_SWITCHES
    mercurySwitches.setPin(PIN_MERCURY_SWITCHES, MERCURY_SWITCHES_COUNT, setValues_mercury);
    mercurySwitches.setDebounceMillis(30);
#endif

#ifdef ENABLE_EEPROM

    uint8_t soundDisabled = eeprom_read_byte((uint8_t *)EEPROM_SOUND_DISABLED);

    // set sound
    int buzzer_pin = PIN_BUZZER;
    if (soundDisabled == 1)
    {
        buzzer_pin = PIN_BUZZER_FAKE;
    }
    buzzer.setPin(buzzer_pin);

    if (soundDisabled == 0xFF)
    {
        //first power on ever.
        // do total reset
        // confirmation song will not play because we are not yet in our refresh loop at setup
        pretbak_apps.eraseEepromRangeLimited(EEPROM_TOTAL_RESET);
    }

#else
    buzzer.setPin(PIN_BUZZER);
#endif

#ifdef ENABLE_APPS
    pretbak_apps.setPeripherals(binaryInputs, &encoder_dial, &visualsManager, &ledDisplay, &buzzer, &selectorDial);
    pretbak_apps.setDefaultMode();
#endif
}

void loop()
{
    lucieboxLoop();
}