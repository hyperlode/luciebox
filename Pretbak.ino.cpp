# 1 "C:\\Users\\LODE~1.AME\\AppData\\Local\\Temp\\tmpchtbc93o"
#include <Arduino.h>
# 1 "C:/Users/lode.ameije/Documents/PlatformIO/Projects/200816-012457-uno/src/Pretbak.ino"

#include "PretbakSettings.h"
#include "DisplayDigitsHandler5Digits.h"
#include "LedMultiplexer5x8.h"
#include "PotentioSelector.h"
#include "ButtonsDacR2r.h"
#include "BinaryInput.h"
#include "Buzzer.h"
#ifdef ENABLE_APPS
#include "Apps.h"
#endif

#include "RotaryEncoderDial.h"







uint16_t setValues_1[BUTTONS_1_COUNT] = BUTTONS_1_VALUES;
uint16_t setValues_2[BUTTONS_2_COUNT] = BUTTONS_2_VALUES;
uint16_t setValues_mercury[MERCURY_SWITCHES_COUNT] = MERCURY_SWITCHES_VALUES;


#ifdef ENABLE_APPS
Apps pretbak_apps;
#endif


BinaryInput binaryInputs[BINARY_INPUTS_COUNT];
PotentioSelector selectorDial;
ButtonsDacR2r buttons_1;
ButtonsDacR2r buttons_2;
ButtonsDacR2r mercurySwitches;
RotaryEncoderDial encoder_dial;


DisplayManagement visualsManager;
LedMultiplexer5x8 allVisuals;
Buzzer buzzer;





#ifdef ENABLE_ANALOG_PIN_DEBUG

bool debugMode;
#endif
void isr0();
void isr1();
void refresh();
void input_process();
void outputProcess();
void setup();
void loop();
#line 54 "C:/Users/lode.ameije/Documents/PlatformIO/Projects/200816-012457-uno/src/Pretbak.ino"
void isr0(){
    encoder_dial.interruptChannelA();
}
void isr1(){
    encoder_dial.interruptChannelB();
}

void refresh()
{


    input_process();
#ifdef DEBUG_BUTTONS
    for (uint8_t i = 0; i < BINARY_INPUTS_COUNT; i++)
    {

        if (binaryInputs[i].getEdgeUp())
        {

            Serial.println("{{{-------PRESS:-------:");
            Serial.println(i);
            Serial.println((analogRead(PIN_BUTTONS_1)));
            Serial.println((analogRead(PIN_BUTTONS_2)));



        }

        if (binaryInputs[i].getEdgeDown())
        {
            Serial.println("-----RELEASE:");
            Serial.println(i);



        }
    }

#endif



#ifdef ENABLE_ANALOG_PIN_DEBUG
    if (!debugMode)
    {
#endif
# 111 "C:/Users/lode.ameije/Documents/PlatformIO/Projects/200816-012457-uno/src/Pretbak.ino"
        bool init = selectorDial.getValueChangedEdge();

#ifdef DEBUG_SELECTOR_KNOB
    if (selectorDial.getValueChangedEdge()){

        Serial.println("selector:");
        Serial.println(selectorDial.getSelectorValue());
        Serial.println(analogRead(PIN_SELECTOR_DIAL));
    }
#endif


#ifdef ENABLE_APPS
    pretbak_apps.appSelector(init, selectorDial.getSelectorValue() - 1);
#endif

        buzzer.doBuzzerRoll();

#ifdef ENABLE_ANALOG_PIN_DEBUG
    }
    else
    {
        #ifdef ENABLE_APPS
        pretbak_apps.appSelector(false, 12);
        #endif
    }
#endif

    outputProcess();

}

void input_process()
{


    for (uint8_t i = 0; i < BINARY_INPUTS_COUNT; i++)
    {
        binaryInputs[i].refresh();
    }

    selectorDial.refresh();
    buttons_1.refresh();
    buttons_2.refresh();
    mercurySwitches.refresh();
    encoder_dial.refresh();

    if (buttons_2.getValueChangedEdge())
    {
        for (uint8_t i = 0; i < BUTTONS_2_COUNT; i++)
        {
            binaryInputs[BUTTONS_2_TO_BINARY_INPUT_OFFSET + i].setValue(buttons_2.getButtonValueByIndex(i));
        }
    }

    if (buttons_1.getValueChangedEdge())
    {
        for (uint8_t i = 0; i < BUTTONS_1_COUNT; i++)
        {
            binaryInputs[BUTTONS_1_TO_BINARY_INPUT_OFFSET + i].setValue(buttons_1.getButtonValueByIndex(i));
        }
    }

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
}

void outputProcess(){

    visualsManager.refresh();
    allVisuals.refresh();
}



void setup()
{

    attachInterrupt(0, isr0, CHANGE);
    attachInterrupt(1, isr1, CHANGE);

#ifdef ENABLE_SERIAL
    Serial.begin(9600);
#endif



    selectorDial.initialize(PIN_SELECTOR_DIAL, SELECTOR_DIAL_POSITIONS);

    buttons_1.setPin(PIN_BUTTONS_1, BUTTONS_1_COUNT, setValues_1);

    buttons_2.setPin(PIN_BUTTONS_2, BUTTONS_2_COUNT, setValues_2);

    mercurySwitches.setPin(PIN_MERCURY_SWITCHES, MERCURY_SWITCHES_COUNT, setValues_mercury);
    mercurySwitches.setDebounceMillis(30);

    encoder_dial.setPins(PIN_ROTARY_ENCODER_DIAL_CHANNEL_A, PIN_ROTARY_ENCODER_DIAL_CHANNEL_B);

    allVisuals.Begin(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_BUTTON_LIGHTS, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);
    visualsManager.setMultiplexerBuffer(allVisuals.getDigits());
# 235 "C:/Users/lode.ameije/Documents/PlatformIO/Projects/200816-012457-uno/src/Pretbak.ino"
#ifdef ENABLE_EEPROM
    if (eeprom_read_byte((uint8_t *)EEPROM_SOUND_OFF_BY_DEFAULT))
    {
        buzzer.setPin(PIN_BUZZER_FAKE);
    }
    else
    {
        buzzer.setPin(PIN_BUZZER);
    }
#else
    buzzer.setPin(PIN_BUZZER);
#endif

#ifdef ENABLE_APPS
    pretbak_apps.setPeripherals(binaryInputs, &encoder_dial, &visualsManager, &allVisuals, &buzzer);
    pretbak_apps.setDefaultMode();
#endif

#ifdef ENABLE_ANALOG_PIN_DEBUG

    debugMode = false;
    if (analogRead(PIN_BUTTONS_1) > 10 && analogRead(PIN_BUTTONS_1) < 200)
    {
        debugMode = true;
        pretbak_apps.appSelector(true, 12);
    }
#endif
}

void loop()
{

#ifdef DBUG_REFACTOR_DISP
    input_process();



    if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){

        visualsManager.setCharToDisplay('8',3);
        visualsManager.setCharToDisplay('0',2);
        visualsManager.setCharToDisplay('-',1);
        visualsManager.setCharToDisplay('H',0);
    }
    if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp())
    {
        Serial.println("button 2");
        visualsManager.setBlankDisplay();
    }
     if (binaryInputs[BUTTON_MOMENTARY_1].getEdgeUp()){
        Serial.println("button 1");
        visualsManager.setLedArray(0xFF);
    }
    if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp()){
        Serial.println("button 0");
        visualsManager.setLedArray(0);
    }
    if (binaryInputs[BUTTON_LATCHING_EXTRA].getEdgeUp()){
        Serial.println("button ext");

    }
    if (binaryInputs[BUTTON_LATCHING_BIG_RED].getEdgeUp()){
        Serial.println("BUTTON_LATCHING_BIG_RED");
    }
    if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getEdgeUp()){
        Serial.println("button BUTTON_LATCHING_SMALL_RED_RIGHT");
    }
    if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getEdgeUp()){
        Serial.println("BUTTON_LATCHING_SMALL_RED_LEFT" );
    }
# 333 "C:/Users/lode.ameije/Documents/PlatformIO/Projects/200816-012457-uno/src/Pretbak.ino"
    allVisuals.refresh();
    outputProcess();

#else

    refresh();
#endif
}