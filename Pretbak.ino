
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
// #include "Potentio.h"
#include "RotaryEncoderDial.h"

// Lode Ameije 2019-05
// Pretbak is a busy box for my newly born niece
// The hardware is a box with:
// input: momentary buttons, latching buttons, a potentiometer, a selector dial, four mercury switches.
// output: lights on buttons (one light per button), 7seg display (4 digits), buzzer

uint16_t setValues_1[BUTTONS_1_COUNT] = BUTTONS_1_VALUES;
uint16_t setValues_2[BUTTONS_2_COUNT] = BUTTONS_2_VALUES;

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
ButtonsDacR2r buttons_1;
ButtonsDacR2r buttons_2;       // buttons without normally closed. this is a problem for the R-2R ladder. instead, I used a pull down resistor to ground at the switch. so: ON = 5V, OFF = GND over 1Kohm. 10K, 20K R2Rladder.  will only work for limited number of buttons.
#ifdef ENABLE_TILT_SWITCHES
ButtonsDacR2r mercurySwitches; // mercury switches go on or off depending on the position. Works with R-2R ladder. No NC in mercury switch, so, pulldown resistor (0.1R)to ground. R=10K
#endif
RotaryEncoderDial encoder_dial;

// OUTPUT
DisplayManagement visualsManager;
LedMultiplexer5x8 allVisuals;
Buzzer buzzer;

//output
//char  textBuf [6];
//char  scrollBuf [1]; //was 40, but, it does not get used anyways!!

#ifdef ENABLE_ANALOG_PIN_DEBUG
//debug mode
bool debugMode;
#endif

// hack to get interrupts nicely going without too much magic.
// https://forum.arduino.cc/index.php?topic=41713.0
void isr0(){
    encoder_dial.interruptChannelA();
}
void isr1(){
    encoder_dial.interruptChannelB();
}

void refresh()
{

    //update inputs
    input_process();
#ifdef DEBUG_BUTTONS
    for (uint8_t i = 0; i < BINARY_INPUTS_COUNT; i++)
    {

        if (binaryInputs[i].getEdgeUp())
        {

            Serial.println("{{{-------PRESS:-------:");
            Serial.println(i);
            Serial.println((analogRead(PIN_BUTTONS_MOMENTARY)));
            Serial.println((analogRead(PIN_BUTTONS_LATCHING)));
            //Serial.println(buttons_1.getButtonsValueRaw(),BIN);
            //Serial.println(buttons_2.getButtonsValueRaw(),BIN);
            //Serial.println("-------PRESS:-------}}}");
        }

        if (binaryInputs[i].getEdgeDown())
        {
            Serial.println("-----RELEASE:");
            Serial.println(i);
            //Serial.println( (analogRead(PIN_BUTTONS_MOMENTARY)));
            //Serial.println(buttons_1.getButtonsValueRaw(),BIN);
            //Serial.println("-----------");
        }
    }

#endif

    //Serial.println("papa help "); // display this PAPA - HELP - PAPA - ... on screen when errors.

#ifdef ENABLE_ANALOG_PIN_DEBUG
    if (!debugMode)
    {
#endif

        //modes functionality (apps)
        //mode change
        //if (selectorDial.getValueChangedEdge()) {


        //}

        //rotary 12 positions selector knob is taken as base for mode selecion. so there are 12 states.

        //check if first iteration at new selector value.
        // bool init = selectorDial.getValueChangedEdge();

#ifdef DEBUG_SELECTOR_KNOB
    if (selectorDial.getValueChangedEdge()){       
         //Serial.println(SELECTOR_DIAL_POSITIONS);
        Serial.println("selector:");
        Serial.println(selectorDial.getSelectorValue());
        Serial.println(analogRead(PIN_SELECTOR_DIAL));
    }
#endif


#ifdef ENABLE_APPS
    pretbak_apps.appSelector(); 
#endif

        buzzer.doBuzzerRoll();

#ifdef ENABLE_ANALOG_PIN_DEBUG
    }
    else
    {
        #ifdef ENABLE_APPS
        pretbak_apps.appSelector(false, 12); //debug mode
        #endif
    }
#endif

    outputProcess();
    
}

void input_process()
{

    // process edges from binaryInputs here, think of it as the previous cycle (done before new input processed.)
    for (uint8_t i = 0; i < BINARY_INPUTS_COUNT; i++)
    {
        binaryInputs[i].refresh();
    }

    selectorDial.refresh();
    buttons_1.refresh();
    buttons_2.refresh();
#ifdef ENABLE_TILT_SWITCHES
    mercurySwitches.refresh();
#endif
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

void outputProcess(){
    //output process
    visualsManager.refresh();
    allVisuals.refresh();
}



void setup()
{
    // trick to use the easy arduino libary instead of complicated native functions
    attachInterrupt(0, isr0, CHANGE);
    attachInterrupt(1, isr1, CHANGE);

#ifdef ENABLE_SERIAL
    Serial.begin(9600);
#endif

    
    // put your setup code here, to run once

    selectorDial.initialize(PIN_SELECTOR_DIAL, SELECTOR_DIAL_POSITIONS);

    buttons_1.setPin(PIN_BUTTONS_MOMENTARY, BUTTONS_1_COUNT, setValues_1);

    buttons_2.setPin(PIN_BUTTONS_LATCHING, BUTTONS_2_COUNT, setValues_2);

#ifdef ENABLE_TILT_SWITCHES
    mercurySwitches.setPin(PIN_MERCURY_SWITCHES, MERCURY_SWITCHES_COUNT, setValues_mercury);
    mercurySwitches.setDebounceMillis(30);
#endif

    encoder_dial.setPins(PIN_ROTARY_ENCODER_DIAL_CHANNEL_A, PIN_ROTARY_ENCODER_DIAL_CHANNEL_B);

    allVisuals.Begin(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_BUTTON_LIGHTS, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);
    visualsManager.setMultiplexerBuffer(allVisuals.getDigits());

#ifdef ENABLE_EEPROM
    
    uint8_t soundDisabled = eeprom_read_byte((uint8_t *)EEPROM_SOUND_DISABLED);

    // set sound
    int buzzer_pin = PIN_BUZZER;
    if (soundDisabled == 1)
    {
        buzzer_pin = PIN_BUZZER_FAKE;
    }
    buzzer.setPin(buzzer_pin);

    if (soundDisabled == 0xFF){
        //first power on ever.
        // do total reset
        // confirmation song will not play because we are not yet in our refresh loop at setup
        pretbak_apps.eraseEepromRangeLimited(EEPROM_TOTAL_RESET);
    }

    // add 1 to power cycles tally keeper
    eeprom_update_word((uint16_t*) EEPROM_LUCIEBOX_POWER_CYCLE_COUNTER, eeprom_read_word((uint16_t*)EEPROM_LUCIEBOX_POWER_CYCLE_COUNTER)+1);

#else
    buzzer.setPin(PIN_BUZZER);
#endif

#ifdef ENABLE_APPS
    pretbak_apps.setPeripherals(binaryInputs, &encoder_dial, &visualsManager, &allVisuals, &buzzer, &selectorDial);
    pretbak_apps.setDefaultMode();
#endif

#ifdef ENABLE_ANALOG_PIN_DEBUG
    // if one of the push buttons is pressed at startup, activate debugmode.
    debugMode = false;
    if (analogRead(PIN_BUTTONS_MOMENTARY) > 10 && analogRead(PIN_BUTTONS_MOMENTARY) < 200)
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

    // byte *tmp = allVisuals.getDigits();

    if (binaryInputs[BUTTON_MOMENTARY_3].getEdgeUp()){
        
        visualsManager.setCharToDisplay('8',3);
        visualsManager.setCharToDisplay('0',2);
        visualsManager.setCharToDisplay('-',1);
        visualsManager.setCharToDisplay('H',0);
         Serial.println("button 3");
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
    if (binaryInputs[BUTTON_LATCHING_3].getEdgeUp()){
        Serial.println("button ext");

    }
    if (binaryInputs[BUTTON_LATCHING_0].getEdgeUp()){
        Serial.println("BUTTON_LATCHING_0");
    }
    if (binaryInputs[BUTTON_LATCHING_2].getEdgeUp()){
        Serial.println("button BUTTON_LATCHING_2");
    }
    if (binaryInputs[BUTTON_LATCHING_1].getEdgeUp()){
        Serial.println("BUTTON_LATCHING_1" );
    }

    // if (binaryInputs[BUTTON_MOMENTARY_2].getEdgeUp())
    // {
    //     visualsManager.setBinaryToDisplay(0xFF000000);
    //     visualsManager.setLedArray(0x0F);
    //     visualsManager.setDecimalPointToDisplay(false,2);
    // }

    // if (binaryInputs[BUTTON_MOMENTARY_0].getEdgeUp())
    // {
    //     allVisuals.setBrightness((byte)potentio.getValueMapped(0, 50), false);
    //     //Serial.println(potentio.getValueMapped(0,50));
    //    // visualsManager.setBlankDisplay();
    //    visualsManager.setBlankDisplay();
    // }
    // visualsManager.refresh();

    // // quick check for all lights.
    // if (binaryInputs[BUTTON_MOMENTARY_1].getValue())
    // {
    //     for (uint8_t i = 0; i < 5; i++)
    //     {
    //         tmp[i] = 0xff;
    //     }
    // }

    // allVisuals.refresh();

    allVisuals.refresh();
    outputProcess();

#else
    // put your main code here, to run repeatedly:
    refresh();
#endif
}