#include "Buzzer.h"
#include "Arduino.h"

Buzzer::Buzzer()
{
    //constructor
    cleanBuzzerRoll();
    this->soundFinishedTimeMillis = 0;
    this->speedScale = 1;
    this->transpose = 0;
}

void Buzzer::changeTranspose(int8_t delta){
    this->setTranspose(this->transpose + delta);
}

void Buzzer::setTranspose(int8_t offset)
{
    if (offset < -12)
    {
        this->transpose = -12;
    }
    else if (offset > 12)
    {
        this->transpose = 12;
    }
    else
    {
        this->transpose = offset;
    }
}

void Buzzer::setPin(uint8_t pin)
{
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
}

uint8_t Buzzer::getPin()
{
    return this->pin;
}

// void Buzzer::loadBuzzerTrack(const uint8_t *seq, uint8_t song_index)
// {
//     // list of songs is an array in flash, every song starts with a length byte of the song. (song cannot be more than 255 bytes)

//     // fast forward to correct index.
//     uint16_t song_start_index = 0;
//     for (uint16_t song=0;song<song_index;song++){
//         song_start_index += pgm_read_byte_near(seq +song_start_index); // add length of song to start index (to get to the start of the next song.)
//     }

//     for (uint16_t note_index=1;note_index < pgm_read_byte_near(seq + song_start_index); note_index++){
    
//         uint8_t note = pgm_read_byte_near(seq + song_start_index + note_index);
//         if (note == BUZZER_ROLL_SONG_STOPVALUE){
//             break;
//         }
//         programBuzzerRoll(note);
//     }    
// }

void Buzzer::programBuzzerRoll(uint8_t sound)
{
    //load one sound to next slot in roll

    //sound:
    //0 = empty slot
    //+ 1-62 = note (1 = 110Hz A , and going up 1/12 of an octave each number
    //+ 63, 126, 189, 252 = duration = 1/8, 1/4, 1/2, 1
    //program in next available slot AFTER playSlotCounter

    //A0sharp 1/2 = 191
    //1/8 stop = 63

    this->programSlotCounter = getNextBuzzerRollSlot(true);
    this->buzzerRoll[this->programSlotCounter] = sound;

    //http://members.efn.org/~qehn/global/building/cents.htm
    //F = {[(2)^1/12]^n} * 220 Hz //220Hz for A 440 , 880 .... for other octaves
}

uint8_t Buzzer::addRandomSoundToRoll(uint8_t lowest, uint8_t highest)
{
    uint8_t r = random(lowest, highest);
    programBuzzerRoll(r);
    return r;
}

void Buzzer::doBuzzerRoll()
{
    //play all the sounds in the array.
    //array of 16 bytes
    //0 stands for free and programmable
    //one playSlotCounter

    if (millis() > this->soundFinishedTimeMillis)
    {
        //erase previous slot
        this->buzzerRoll[this->playSlotCounter] = BUZZER_ROLL_EMPTY_SLOT;

        //move active slot
        this->playSlotCounter = getNextBuzzerRollSlot(false);

        if (this->buzzerRoll[this->playSlotCounter] != BUZZER_ROLL_EMPTY_SLOT)
        {
            //uint8_t noteNumber = buzzerRoll[this->playSlotCounter]%63;
            //F = {[(2)^1/12]^n} * 220 Hz //220Hz for A 440 , 880 .... for other octaves
            float freq = 1;

            {
                // convert note to freq
                for (uint8_t i = 1; i < (buzzerRoll[this->playSlotCounter] % 64) + this->transpose; i++)
                {
                    //same result as: (but 2K less memory!!)
                    //freq = pow(1.059463,(buzzerRoll[this->playSlotCounter])-1 % 63);
                    freq *= 1.059463;
                }
            }

            // check if no "rust"
            if (buzzerRoll[this->playSlotCounter] % 64 != 0)
            {
                freq *= BUZZER_ROLL_BASE_FREQUENCY; //frequency
            }

            tone(this->pin, (unsigned int)freq,
                 (unsigned long)(this->speedScale * BUZZER_ROLL_EIGHTNOTE_DURATION_MILLIS *
                                 (1 << buzzerRoll[this->playSlotCounter] / 63))); //duration, number is exponent of 2.

            this->soundFinishedTimeMillis = millis() + (unsigned long)(this->speedScale * BUZZER_ROLL_EIGHTNOTE_DURATION_MILLIS * (1 << buzzerRoll[this->playSlotCounter] / 63));
        }
    }
}

void Buzzer::changeSpeedRatio(int8_t delta){
    // provide -1, 0 or 1 for best results
    this->speedScale *= 1 - 0.01 * (float)delta;
}

void Buzzer::setSpeedRatio(float speedMultiplier)
{
    // argument is a long: 1 is normal speed, 0.5 is double, 2 is half, ....
    this->speedScale = speedMultiplier;
}

uint8_t Buzzer::getNextBuzzerRollSlot(bool getNextEmptySlot)
{
    //returns value of next slot
    //if argument true, will search for next free slot. If no free slots, returns current slot.
    uint8_t slot = this->playSlotCounter;
    do
    {
        slot >= BUZZER_ROLL_LENGTH - 1 ? slot = 0 : slot++;
    } while (getNextEmptySlot && buzzerRoll[slot] != BUZZER_ROLL_EMPTY_SLOT && this->playSlotCounter != slot);
    return slot;
}

void Buzzer::cleanBuzzerRoll()
{
    //all slots empty
    for (uint8_t i = 0; i < BUZZER_ROLL_LENGTH; i++)
    {
        buzzerRoll[i] = BUZZER_ROLL_EMPTY_SLOT;
    }
}

void Buzzer::buzzerOff()
{
    //erase contents of buzzerRoll and switch off.
    this->cleanBuzzerRoll();
    noTone(this->pin);
}

void Buzzer::playTone(unsigned int freq, unsigned long duration_millis)
{
    // set 0 to duration_millis for indefinate length (will sound until notone or another tone command is given. )

    this->buzzerOff();
    if (duration_millis == 0)
    {
        tone(this->pin, freq);
    }
    else
    {
        tone(this->pin, freq, duration_millis);
    }
}

uint8_t Buzzer::getBuzzerRollFull()
{
    //check if there are free slots.
    return (getNextBuzzerRollSlot(true) == this->playSlotCounter);
}

uint8_t Buzzer::getBuzzerRollEmpty()
{
    int sum = 0;
    for (uint8_t i = 0; i < BUZZER_ROLL_LENGTH; i++)
    {
        sum += buzzerRoll[i];
    }
    return sum == BUZZER_ROLL_LENGTH * BUZZER_ROLL_EMPTY_SLOT;
}

void Buzzer::lastPlayedNoteToDisplay(char *textBuf, uint8_t *decimalPoints)
{
    noteToDisplay(textBuf, decimalPoints, this->buzzerRoll[this->playSlotCounter]);
}

void Buzzer::noteToDisplay(char *textBuf, uint8_t *decimalPoints, uint8_t note)
{
    // assume 4 chars textBuf (empty at arrival (4 spaces))
    // assumme value of decimalPoints at start is 0x00

    uint8_t noteVal = (note) % 64;

    if (noteVal == 0)
    {
        // rest
        textBuf[0] = 59; // ONLY_MIDDLE_SEGMENT_FAKE_ASCII '-';;
        textBuf[1] = 59; // ONLY_MIDDLE_SEGMENT_FAKE_ASCII '-';;
    }
    else
    {

        bool sharps [12] = {false,true,false,false,true,false,true,false,false,true,false,true}; 
        char notes_chars [12] = {'A','A','B','C','C','D','D','E','F','F','G','G'};
        noteVal--;
        noteVal %= 12;
        
        // sharp indicated by a decimal point
        *decimalPoints = 0;
        *decimalPoints =  sharps[noteVal] << 0;

        textBuf[0] = notes_chars[noteVal];

        // octave
        if (note < 4)
        {
            // exceptions, because we don't want negative numbers when doing -4 in our formula
            textBuf[1] = 48 + 3;
        }
        else
        {
            textBuf[1] = ((note % 64) - 4) / 12 + 4 + 48;
        }
    }

    textBuf[2] = ' '; // optimized: assume empty at start

    // note length
    textBuf[3] = this->getLength(note) + 48; // 2^(3 -x) --> note length is 8,4,2,1
}

uint8_t Buzzer::getLength(uint8_t note){
    return 0x01 << (3 - (note / 64)); // 2^(3 -x) --> note length is 8,4,2,1
}

void Buzzer::nextNote(int16_t* note, bool upElseDown, bool stayInSameLength){
    // stayInSameLength: at every length changes, the octaves don't line up. Adjust manually.
    int16_t note_without_length = *note%64;

    if (upElseDown){
        note_without_length++;
        *note += 1;
    }else{
        note_without_length--;
        *note -= 1;
    }

    if(stayInSameLength){  
        if (note_without_length > 60){
            *note -= 60; 
        }else if(note_without_length <= 0){
            *note += 60;
        }
        
    }else{
        // check boundaries within note length (and allow overflow)
        if (note_without_length > 63){
            *note += 4; 
        }else if(note_without_length <= 0){
            *note -= 4;
        }
    
        // check boundaries over overall range
        if (*note <=0){
            *note = 252;
        }
        if (*note > 254){
            *note = 3;
        }
    }
}

void Buzzer::changeNoteToNextLength(int16_t* note){

    *note += 64;
    
    if (*note>254){
        note -= 4*64;
    }
}
