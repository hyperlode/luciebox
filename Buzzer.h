
#ifndef Buzzer_h
#define Buzzer_h
#include "Arduino.h"

#define rest_8 0
#define A3_8 1
#define As3_8 2
#define B3_8 3
#define C4_8 4
#define Cs4_8 5
#define D4_8 6
#define Ds4_8 7
#define E4_8 8
#define F4_8 9
#define Fs4_8 10
#define G4_8 11
#define Gs4_8 12
#define A4_8 13
#define As4_8 14
#define B4_8 15
#define C5_8 16
#define Cs5_8 17
#define D5_8 18
#define Ds5_8 19
#define E5_8 20
#define F5_8 21
#define Fs5_8 22
#define G5_8 23
#define Gs5_8 24
#define A5_8 25
#define As5_8 26
#define B5_8 27
#define C6_8 28
#define Cs6_8 29
#define D6_8 30
#define Ds6_8 31
#define E6_8 32
#define F6_8 33
#define Fs6_8 34
#define G6_8 35
#define Gs6_8 36
#define A6_8 37
#define As6_8 38
#define B6_8 39
#define C7_8 40
#define Cs7_8 41
#define D7_8 42
#define Ds7_8 43
#define E7_8 44
#define F7_8 45
#define Fs7_8 46
#define G7_8 47
#define Gs7_8 48
#define A7_8 49
#define As7_8 50
#define B7_8 51
#define C8_8 52
#define Cs8_8 53
#define D8_8 54
#define Ds8_8 55
#define E8_8 56
#define F8_8 57
#define Fs8_8 58
#define G8_8 59
#define Gs8_8 60
#define A8_8 61
#define As8_8 62
#define B8_8 63
#define rest_4 64
#define A3_4 65
#define As3_4 66
#define B3_4 67
#define C4_4 68  
#define Cs4_4 69
#define D4_4 70
#define Ds4_4 71
#define E4_4 72
#define F4_4 73
#define Fs4_4 74
#define G4_4 75
#define Gs4_4 76
#define A4_4 77
#define As4_4 78
#define B4_4 79
#define C5_4 80
#define Cs5_4 81
#define D5_4 82
#define Ds5_4 83
#define E5_4 84
#define F5_4 85
#define Fs5_4 86
#define G5_4 87
#define Gs5_4 88
#define A5_4 89
#define As5_4 90
#define B5_4 91
#define C6_4 92
#define Cs6_4 93
#define D6_4 94
#define Ds6_4 95
#define E6_4 96
#define F6_4 97
#define Fs6_4 98
#define G6_4 99
#define Gs6_4 100
#define A6_4 101
#define As6_4 102
#define B6_4 103
#define C7_4 104
#define Cs7_4 105
#define D7_4 106
#define Ds7_4 107
#define E7_4 108
#define F7_4 109
#define Fs7_4 110
#define G7_4 111
#define Gs7_4 112
#define A7_4 113
#define As7_4 114
#define B7_4 115
#define C8_4 116
#define Cs8_4 117
#define D8_4 118
#define Ds8_4 119
#define E8_4 120
#define F8_4 121
#define Fs8_4 122
#define G8_4 123
#define Gs8_4 124
#define A8_4 125
#define As8_4 126
#define B8_4 127
#define rest_2 128
#define A3_2 129
#define As3_2 130
#define B3_2 131
#define C4_2 132
#define Cs4_2 133
#define D4_2 134
#define Ds4_2 135
#define E4_2 136
#define F4_2 137
#define Fs4_2 138
#define G4_2 139
#define Gs4_2 140
#define A4_2 141
#define As4_2 142
#define B4_2 143
#define C5_2 144
#define Cs5_2 145
#define D5_2 146
#define Ds5_2 147
#define E5_2 148
#define F5_2 149
#define Fs5_2 150
#define G5_2 151
#define Gs5_2 152
#define A5_2 153
#define As5_2 154
#define B5_2 155
#define C6_2 156
#define Cs6_2 157
#define D6_2 158
#define Ds6_2 159
#define E6_2 160
#define F6_2 161
#define Fs6_2 162
#define G6_2 163
#define Gs6_2 164
#define A6_2 165
#define As6_2 166
#define B6_2 167
#define C7_2 168
#define Cs7_2 169
#define D7_2 170
#define Ds7_2 171
#define E7_2 172
#define F7_2 173
#define Fs7_2 174
#define G7_2 175
#define Gs7_2 176
#define A7_2 177
#define As7_2 178
#define B7_2 179
#define C8_2 180
#define Cs8_2 181
#define D8_2 182
#define Ds8_2 183
#define E8_2 184
#define F8_2 185
#define Fs8_2 186
#define G8_2 187
#define Gs8_2 188
#define A8_2 189
#define As8_2 190
#define B8_2 191
#define rest_1 192
#define A3_1 193
#define As3_1 194
#define B3_1 195
#define C4_1 196
#define Cs4_1 197
#define D4_1 198
#define Ds4_1 199
#define E4_1 200
#define F4_1 201
#define Fs4_1 202
#define G4_1 203
#define Gs4_1 204
#define A4_1 205
#define As4_1 206
#define B4_1 207
#define C5_1 208
#define Cs5_1 209
#define D5_1 210
#define Ds5_1 211
#define E5_1 212
#define F5_1 213
#define Fs5_1 214
#define G5_1 215
#define Gs5_1 216
#define A5_1 217
#define As5_1 218
#define B5_1 219
#define C6_1 220
#define Cs6_1 221
#define D6_1 222
#define Ds6_1 223
#define E6_1 224
#define F6_1 225
#define Fs6_1 226
#define G6_1 227
#define Gs6_1 228
#define A6_1 229
#define As6_1 230
#define B6_1 231
#define C7_1 232
#define Cs7_1 233
#define D7_1 234
#define Ds7_1 235
#define E7_1 236
#define F7_1 237
#define Fs7_1 238
#define G7_1 239
#define Gs7_1 240
#define A7_1 241
#define As7_1 242
#define B7_1 243
#define C8_1 244
#define Cs8_1 245
#define D8_1 246
#define Ds8_1 247
#define E8_1 248
#define F8_1 249
#define Fs8_1 250
#define G8_1 251
#define Gs8_1 252
#define A8_1 253
#define As8_1 254
// #define rest_top_up_to_1 254 // adds silence to previous note until full note length; i.e. if 1/4 note, will add 3/4 silence.
// #define B8_1 255  = BUZZER_ROLL_SONG_STOPVALUE

#define BUZZER_MINUTE_BEEP_FREQUENCY 4000 //beep at each minute with this frequency
#define BUZZER_MINUTEBEEP_DURATION_MILLIS 20
#define BUZZER_MINUTEBEEP_DURATION_MILLIS 20
#define BUZZER_ALARM_FREQUENCY 2000
#define BUZZER_ALARM_DURATION_MILLIS 1000000

#define BUZZER_NEXTPLAYER_BEEP_FREQUENCY 500
#define BUZZER_NEXTPLAYER_DURATION_MILLIS 20
#define BUZZER_NEXTPLAYER_MULTIPLEBEEP_INTERVAL_MILLIS 100

#define BUZZER_ROLLVALUE_BUTTONPRESS
#define BUZZER_ROLLVALUE_PAUSE
#define BUZZER_ROLLVALUE_UNPAUSE
#define BUZZER_ROLLVALUE_STARTGAME
#define BUZZER_ROLLVALUE_ENDGAME

#define BUZZER_ROLL_LENGTH 100 // accomodate for "the longest songs..." or find another way to split songs in half.
#define BUZZER_ROLL_EMPTY_SLOT 255
// #define BUZZER_ROLL_EIGHTNOTE_DURATION_MILLIS 63
#define BUZZER_ROLL_EIGHTNOTE_DURATION_MILLIS 15
#define BUZZER_ROLL_BASE_FREQUENCY 220
#define BUZZER_ROLL_SONG_STOPVALUE 255
//
//const uint8_t song_pause [] PROGMEM = {39,126,170,126,BUZZER_ROLL_SONG_STOPVALUE};
//// const uint8_t song_simple_beep [] PROGMEM = {51,63,BUZZER_ROLL_SONG_STOPVALUE};
//const uint8_t song_simple_beep [] PROGMEM = {51,126,BUZZER_ROLL_SONG_STOPVALUE};
//const uint8_t song_unpause [] PROGMEM = {44,126,165,126,BUZZER_ROLL_SONG_STOPVALUE};
//
//const uint8_t song_nextPlayer_Single [] PROGMEM = {49,63,BUZZER_ROLL_SONG_STOPVALUE};
//
//const uint8_t song_alarm [] PROGMEM = {49,63,49,63,49,63,49,63,49,63,BUZZER_ROLL_SONG_STOPVALUE};
//
//const uint8_t song_nextPlayerSequence [] PROGMEM = {228,126,105,126,228,126,228,126,102,126,49,126,230,126,41,126,227};
//const byte song_happy_dryer [] PROGMEM = {163,126,189,167,126,189,107,63,126,104,63,126,107,63,126,238,238,189,BUZZER_ROLL_SONG_STOPVALUE};

//const byte song_unhappy_dryer[] PROGMEM = {226,189,230,189,170,126,167,126,165,126,226,189,189,BUZZER_ROLL_SONG_STOPVALUE};
// const byte song_attack[] PROGMEM = {39,0,39,0,39,107,39,107,44,189,BUZZER_ROLL_SONG_STOPVALUE};
// const byte song_attack[] PROGMEM = {165,126,189,165,126,189,165,126,189,   170,126,189,189,189,165,126,189,  233,233,189,189,189,189  ,BUZZER_ROLL_SONG_STOPVALUE};
//const byte song_attack [] PROGMEM = {162,126,162,126,162,126, 167,189,189,162,126,  230,230,230 ,BUZZER_ROLL_SONG_STOPVALUE};  //aaanvallueeeeee!
//const byte song_gameStart_attack [] PROGMEM = {162,126,162,126,162,126,167,189,189,162,126,  230,230,230 ,BUZZER_ROLL_SONG_STOPVALUE};  //aaanvallueeeeee!

class Buzzer
{
public:
    Buzzer();

    void setPin(uint8_t pin);
    uint8_t getPin();
    //void loadBuzzerTrack(const uint8_t *seq);
    void loadBuzzerTrack(const uint8_t *seq, uint8_t song_index);
    void programBuzzerRoll(uint8_t sound);
    uint8_t addRandomSoundToRoll(uint8_t lowest, uint8_t highest);
    void doBuzzerRoll();
    uint8_t getNextBuzzerRollSlot(bool getNextEmptySlot);
    uint8_t getBuzzerRollFull();
    uint8_t getBuzzerRollEmpty();
    void setTranspose(int8_t offset);
    void cleanBuzzerRoll();
    // void buzzerOn(uint16_t freq);
    void playTone(unsigned int freq, unsigned long duration_millis);
    void buzzerOff();
    void setSpeedRatio(float speedMultiplier);
    void lastPlayedNoteToDisplay(char *textBuf, uint8_t *decimalPoints);
    void noteToDisplay(char *textBuf, uint8_t *decimalPoints, uint8_t note);
    
    // void getOctave(uint8_t* note);
    uint8_t getLength(uint8_t note);
    // void nextOctave(uint8_t* note, bool upElseDown);
    void changeNoteToNextLength(int16_t* note);
    void nextNote(int16_t* note, bool upElseDown, bool stayInSameLength);

private:
    int8_t transpose;
    float speedScale;
    uint8_t pin;
    uint8_t buzzerRoll[BUZZER_ROLL_LENGTH];
    uint8_t playSlotCounter;
    uint8_t programSlotCounter;
    unsigned long soundFinishedTimeMillis;
};
#endif

//
//#ifdef BUZZER_ENABLED
//    //beeps
//
//    //byte numberOfBeepsToBePlayed;
//    //unsigned long beepsWatchMillis;
//
//
//#endif
//
//#ifdef BUZZER_ENABLED
//    byte soundBehaviour;
//#endif
//
