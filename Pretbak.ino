#include "PotentioSelector.h"
#include "ButtonsDacR2r.h"
#include "BinaryInput.h"
#include "Buzzer.h"

#include "SevSeg5Digits.h" //sevseb -->this should not be included here, but only in DisplayDigitsHandler.h, as it only gets used there (but ARDUINO needs this here!) DEBUG
#include "DisplayDigitsHandler5Digits.h"


// Lode Ameije 2019-05
// Pretbak is a busy box for my newborn niece
// The hardware is a box with
// input: buttons, momentary and switches, a potentiometer, a selector dial, some mercury switches.
// output: lights on buttons, 7seg display (4 digits), buzzer

#define DISPLAY_IS_COMMON_ANODE true  //check led displays both displays should be of same type   //also set in SevSeg5Digits.h : MODEISCOMMONANODE
 
#define PIN_DISPLAY_DIGIT_0 0
#define PIN_DISPLAY_DIGIT_1 5
#define PIN_DISPLAY_DIGIT_2 9
#define PIN_DISPLAY_DIGIT_3 10
#define PIN_DISPLAY_DIGIT_4 11
#define PIN_DISPLAY_DIGIT_BUTTON_LIGHTS 6

#define PIN_DISPLAY_SEGMENT_A 7
#define PIN_DISPLAY_SEGMENT_B 2
#define PIN_DISPLAY_SEGMENT_C 4
#define PIN_DISPLAY_SEGMENT_D 8
#define PIN_DISPLAY_SEGMENT_E 12
#define PIN_DISPLAY_SEGMENT_F 13
#define PIN_DISPLAY_SEGMENT_G 3
#define PIN_DISPLAY_SEGMENT_DP A4

#define PIN_BUZZER A5
  
#define LIGHT_RED 1
#define LIGHT_GREEN 6
#define LIGHT_BLUE 2
#define LIGHT_YELLOW 0
#define LIGHT_LED_1 3
#define LIGHT_LED_2 4
#define LIGHT_LED_3 5

#define PIN_SELECTOR_DIAL A0
#define PIN_BUTTONS_1 A1
#define PIN_BUTTONS_2 A2
#define PIN_POTENTIO A3

#define BINARY_INPUTS_COUNT 8
#define BUTTONS_1_COUNT 4
#define BUTTONS_1_TO_BINARY_INPUT_OFFSET 3

#define BUTTONS_2_COUNT 3
#define BUTTONS_2_TO_BINARY_INPUT_OFFSET 0

#define BUTTON_LATCHING_BIG_RED 2
#define BUTTON_LATCHING_SMALL_RED_LEFT 1
#define BUTTON_LATCHING_SMALL_RED_RIGHT 0
#define BUTTON_LATCHING_YELLOW 6
#define BUTTON_MOMENTARY_RED 5
#define BUTTON_MOMENTARY_GREEN 4
#define BUTTON_MOMENTARY_BLUE 3

#define POTENTIO_SENSITIVITY 5 //value change before value update.
const uint8_t song_happy_dryer [] PROGMEM = {163,126,189,167,126,189,107,63,126,104,63,126,107,63,126,238,238,189,BUZZER_ROLL_SONG_STOPVALUE};
const uint8_t song_unhappy_dryer[] PROGMEM = {226,189,230,189,170,126,167,126,165,126,226,189,189,BUZZER_ROLL_SONG_STOPVALUE};
const uint8_t song_attack [] PROGMEM = {162,126,162,126,162,126, 167,189,189,162,126,  230,230,230 ,BUZZER_ROLL_SONG_STOPVALUE};  //aaanvallueeeeee!

// INPUT
BinaryInput binaryInputs[BINARY_INPUTS_COUNT];
PotentioSelector selectorDial;
ButtonsDacR2r buttons_1;
ButtonsDacR2r buttons_2; // buttons with normally closed. this is a problem for the R-2R ladder. instead, I used a pull down resistor to ground at the switch. so: ON = 5V, OFF = GND over 1Kohm. 10K, 20K R2Rladder.  will only work for limited number of buttons.

int16_t potentio_value;
int16_t potentio_value_stable;
bool potentio_value_stable_changed;

SuperTimer tmptimer;

// OUTPUT
DisplayManagement ledDisp;
char  textBuf [6];
char  scrollBuf [12];
uint8_t lights;

uint8_t allNotesIndex;

Buzzer buzzer;

//global variables
int16_t counter;
bool numberElseAlphabethMode;

void refresh(){


  //input process  
  input_process();


  for(uint8_t i=0;i<BINARY_INPUTS_COUNT;i++){
    if (binaryInputs[i].getEdgeUp()){
      Serial.println(i);
      Serial.println("PRESSED");
    }
    if (binaryInputs[i].getEdgeDown()){
      Serial.println(i);
      Serial.println("Button released");
    }
   // binaryInputs[i].refresh();
  }
  
  //Serial.println("papa help "); // display this PAPA - HELP - PAPA - ... on screen when errors.

  //modes functionality
  
  //mode change
  if (selectorDial.getValueChangedEdge()) {
    //maybe first do a default behaviour.
    
    // as the selector dial is taken for main mode selection, init the mode here when selected

    //default mode (go to default state at each change)
    setDefaultMode();
    Serial.println(selectorDial.getSelectorValue());
  }

  mode_refresh();

  //output process
  ledDisp.refresh();
  buzzer.doBuzzerRoll();
    
  for(uint8_t i=0;i<BINARY_INPUTS_COUNT;i++){
    binaryInputs[i].refresh();
  }
}

void input_process(){
  selectorDial.refresh();
  buttons_1.refresh();
  buttons_2.refresh();
  potentio_refresh();

  if (buttons_2.getValueChangedEdge()) {
    //Serial.println("analog 2in buttons:");
    //Serial.println(buttons_2.getButtonsValueAnalog());
    //Serial.println(buttons_2.getButtonsValue());
    for (uint8_t i=0; i< BUTTONS_2_COUNT; i++){
      binaryInputs[BUTTONS_2_TO_BINARY_INPUT_OFFSET + i].setValue(buttons_2.getButtonValueByIndex(i));
    }
  }

  if (buttons_1.getValueChangedEdge()) {
    //Serial.println("analog in buttons:");
    //Serial.println(buttons_1.getButtonsValueAnalog());
    //Serial.println(buttons_1.getButtonsValue());
    for (uint8_t i=0; i< BUTTONS_1_COUNT; i++){
      binaryInputs[BUTTONS_1_TO_BINARY_INPUT_OFFSET + i].setValue(buttons_1.getButtonValueByIndex(i));
    }
  }
}

void setDefaultMode(){
  //button lights
  lights = 0b00000000; //no lights
  ledDisp.SetLedArray(lights);

  //display
  textBuf[1]='-';
  textBuf[2]='-';
  textBuf[3]='-';
  textBuf[4]='-';
  //textBuf[5]='/0';
  ledDisp.displayHandler(textBuf);
  
  for (int i=1;i<5;i++){
    ledDisp.setDecimalPoint(true, i);
  }
  ledDisp.setBrightness(0,false);

  //buzzer
  buzzer.buzzerOff();
}

void blankDisplay(){
  textBuf[1]=' ';
  textBuf[2]=' ';
  textBuf[3]=' ';
  textBuf[4]=' ';
  textBuf[5]='/0';
  ledDisp.displayHandler(textBuf);
}

void mode_refresh(){
  
  //rotary 12 positions selector knob is taken as base for mode selecion. so there are 12 states. 

  bool init = selectorDial.getValueChangedEdge();
  switch (selectorDial.getSelectorValue()) {
    case 0:
      modeCountingLettersAndChars(init);
      break;
      
    case 1:
      //sound fun with notes
      modeSoundNotes();
      
      break;
    case 2:
      //sound fun with frequencies.
      //long freqq = 0;
      //freqq = map((long)potentio_value, 0, 1023, 0 , 65535);
      //buzzer.buzzerOn((uint16_t) freqq);
      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
        if (potentio_value_stable_changed){      
          buzzer.buzzerOn( (uint16_t) map((long)potentio_value_stable, 0, 1023, 0 , 65535));
        }
      }else{
        if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue() && binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
          if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
            buzzer.buzzerOn( (uint16_t) map((long)potentio_value_stable, 0, 1023, 0 , 65535));
          }
        }else if (true) {
          
        }
        
      }
      break;
      
    case 3:
      modeScroll(init);
      break;
    case 4:
      break;
    case 5:
      modeSoundSong(init);
      break;
    case 6:
      break;
    case 7:
      modeSimpleButtonsAndLights();    
      break;
    case 8:
      break;
    case 9:
      break;
    case 10:
      break;
    case 11:
      break;
    
    default:
      break;
  }
  
}

void potentio_refresh(){
  potentio_value = (int16_t)analogRead(PIN_POTENTIO);
  
  if (potentio_value > potentio_value_stable + POTENTIO_SENSITIVITY || potentio_value < potentio_value_stable - POTENTIO_SENSITIVITY  ){
    potentio_value_stable_changed = true;  //simple edge detection
    potentio_value_stable = potentio_value;
    
//    Serial.println(potentio_value_stable);
  }else{
    potentio_value_stable_changed = false;  //simple edge detection
    
  }
  
}

void setup() {
  // put your setup code here, to run once:
  selectorDial.setPin(PIN_SELECTOR_DIAL);
  buttons_1.setPin(PIN_BUTTONS_1,BUTTONS_1_COUNT);
  buttons_2.setPin(PIN_BUTTONS_2,BUTTONS_2_COUNT);

  buzzer.setPin(PIN_BUZZER);
  
  ledDisp.startUp(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_4, PIN_DISPLAY_DIGIT_BUTTON_LIGHTS, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);
  //ledDisp.startUp(DISPLAY_IS_COMMON_ANODE, PIN_DISPLAY_DIGIT_0, PIN_DISPLAY_DIGIT_1, PIN_DISPLAY_DIGIT_2, PIN_DISPLAY_DIGIT_3, PIN_DISPLAY_DIGIT_4, PIN_DISPLAY_SEGMENT_A, PIN_DISPLAY_SEGMENT_B, PIN_DISPLAY_SEGMENT_C, PIN_DISPLAY_SEGMENT_D, PIN_DISPLAY_SEGMENT_E, PIN_DISPLAY_SEGMENT_F, PIN_DISPLAY_SEGMENT_G, PIN_DISPLAY_SEGMENT_DP);


  setDefaultMode();
  

//  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  refresh();
}

void modeScroll(bool init){
  
  // display scroll mode
  
  if (init){
    // display scroll mode
    scrollBuf[0]='L';
    scrollBuf[1]='U';
    scrollBuf[2]='C';
    scrollBuf[3]='I';
    scrollBuf[4]='E';
    scrollBuf[5]=' ';
    scrollBuf[6]='B';
    scrollBuf[7]='A';
    scrollBuf[8]='B';
    scrollBuf[9]='Y';
    scrollBuf[10]='/0';
    ledDisp.dispHandlerWithScroll(scrollBuf, true, false);
      
  }
  
  if (!binaryInputs[BUTTON_MOMENTARY_BLUE].getValue()){
    ledDisp.doScroll();
  }

  if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
    ledDisp.setScrollSpeed((long)potentio_value_stable);
  }else{
    ledDisp.setBrightness((byte)(potentio_value_stable/20),false);
  }
  
}
void modeSimpleButtonsAndLights(){
  
      // simple repetitive, predictive mode.
      // each button triggers its corresponding light. 
      // potentio sets display brightness
      // no buzzer
      // display lights up a segment for each button.
      bool aButtonIsPressed = false;
  
      blankDisplay();
      
      
      lights = 0b00000000; //reset before switch enquiry
      if (binaryInputs[BUTTON_MOMENTARY_RED].getValue()){
        lights|= 1<<LIGHT_RED;
        aButtonIsPressed = true;
      }
      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getValue()){
        lights|= 1<<LIGHT_BLUE;
        aButtonIsPressed = true;
      }
      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getValue()){
        lights|= 1<<LIGHT_GREEN;
        aButtonIsPressed = true;
      }
      

      if (aButtonIsPressed){
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
        aButtonIsPressed = true;
      }else{
        textBuf[3]=' ';
      }
      if (binaryInputs[BUTTON_LATCHING_YELLOW].getValue()){
        lights|= 1<<LIGHT_YELLOW;
        aButtonIsPressed = true;
      }else{
        textBuf[4]=' ';
      }
      
      ledDisp.displayHandler(textBuf);
      
      ledDisp.SetLedArray(lights);
      ledDisp.setBrightness((byte)(potentio_value_stable/20),false);
}

void modeCountingLettersAndChars(bool init){
        //counting mode: numbers and letters.
      bool aButtonIsPressed = false;

      if (init){
        aButtonIsPressed = true;
      }
      
      numberElseAlphabethMode = binaryInputs[BUTTON_LATCHING_YELLOW].getValue();
  
      if (binaryInputs[BUTTON_LATCHING_YELLOW].getValueChanged()){
        aButtonIsPressed = true;
      }
    
      if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
        counter++;
        aButtonIsPressed = true;
      }
       
      if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
        counter--;
        aButtonIsPressed = true;
      } 
      if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
        if (numberElseAlphabethMode){
          counter = 0;
        }else{
          counter = 1;
        }
        aButtonIsPressed = true;
      }
      
      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getEdgeUp()){
        
        tmptimer.setInitTimeMillis(-1000);
        tmptimer.start();
//        Serial.println(tmptimer.getTimeMillis());
//        Serial.println(tmptimer.getTimeIsNegative());
      }  
      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getEdgeDown()){
        tmptimer.pause();        
        //tmptimer.setInitTimeMillis(-10000);
        //tmptimer.start();
//        Serial.println(tmptimer.getTimeMillis());
//        Serial.println(tmptimer.getTimeIsNegative());
      }  
//      
      if (binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue() && !tmptimer.getTimeIsNegative()){
        if (binaryInputs[BUTTON_LATCHING_SMALL_RED_RIGHT].getValue()){
          counter++;  
        }else{
          counter--;
        }
        
        tmptimer.start();
        aButtonIsPressed = true;
      }
      
      //potentio behaviour
      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
        if (numberElseAlphabethMode){
          counter = (int16_t)potentio_value/10;
        }else{
          counter = (int16_t)potentio_value/39; //1024 to 26 letters.
        }
        aButtonIsPressed = true;
      }
      else if(binaryInputs[BUTTON_LATCHING_SMALL_RED_LEFT].getValue()){
        // if autocounter is on.
        
        tmptimer.setInitTimeMillis(-100 * (long)(potentio_value_stable/10)); //divided by ten, this way, we can set the timer very accurately as displayed on screen when big red is pressed. *100ms
        if (potentio_value_stable_changed){
          tmptimer.start();
        }
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

      if (aButtonIsPressed){
        //Serial.println(counter);
        if (numberElseAlphabethMode){
          ledDisp.showNumber(counter);
        }else{
          ledDisp.showNumberAsChars(counter);
        }
      }
}

void modeSoundSong(bool init){
  if (init){
    buzzer.loadBuzzerTrack(song_happy_dryer);
//    buzzer.loadBuzzerTrack(song_unhappy_dryer);
  }

  
  if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
    buzzer.loadBuzzerTrack(song_happy_dryer);
  }
  
  if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
    buzzer.loadBuzzerTrack(song_unhappy_dryer);
  }
  if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
    buzzer.loadBuzzerTrack(song_attack );
  }

  


}

void modeSoundNotes(){
  //buzzer with buzzer roll (notes).
      
      if (binaryInputs[BUTTON_LATCHING_BIG_RED].getValue()){
        if (potentio_value_stable_changed){
          //buzzer.programBuzzerRoll(potentio_value_stable /4);;
           allNotesIndex = potentio_value_stable /4;
        }
        
        if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
          buzzer.programBuzzerRoll(allNotesIndex);
          allNotesIndex--;
        }
        if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
          buzzer.programBuzzerRoll(allNotesIndex);
        }
        
        if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
          buzzer.programBuzzerRoll(allNotesIndex);
          allNotesIndex++;
        }
        ledDisp.showNumber(allNotesIndex);
      }else{
        if (binaryInputs[BUTTON_MOMENTARY_RED].getEdgeUp()){
          ledDisp.showNumber(buzzer.addRandomSoundToRoll(223, 235));
          
        }
        if (binaryInputs[BUTTON_MOMENTARY_GREEN].getEdgeUp()){
          ledDisp.showNumber(buzzer.addRandomSoundToRoll(160, 223));
          
        }
        if (binaryInputs[BUTTON_MOMENTARY_BLUE].getEdgeUp()){
          ledDisp.showNumber(buzzer.addRandomSoundToRoll(97, 160));
          
        }  
      }
}

