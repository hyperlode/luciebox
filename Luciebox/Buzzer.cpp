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

void Buzzer::cleanBuzzerRoll()
{
	playSlotCounter = 1;
    programSlotCounter = 0;
}

void Buzzer::changeTranspose(int8_t delta)
{
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

void Buzzer::addNoteToRoll(uint8_t note)
{
    //load one note to next slot in roll

    //sound:
    //0 = empty slot
    //+ 1-62 = note (1 = 110Hz A , and going up 1/12 of an octave each number
    //+ 63, 126, 189, 252 = duration = 1/8, 1/4, 1/2, 1
    //program in next available slot AFTER playSlotCounter

    //A0sharp 1/2 = 191
    //1/8 stop = 63

    //this->programSlotCounter = getNextBuzzerRollSlot(true);
	this->programSlotCounter = getNextProgramSlot();
	
	
    this->buzzerRoll[this->programSlotCounter] = note;

    //http://members.efn.org/~qehn/global/building/cents.htm
    //F = {[(2)^1/12]^n} * 220 Hz //220Hz for A 440 , 880 .... for other octaves
}

uint8_t Buzzer::addRandomSoundToRoll(uint8_t lowest, uint8_t highest)
{
    uint8_t r = random(lowest, highest);
    addNoteToRoll(r);
    return r;
}

void Buzzer::doBuzzerRoll()
{
    //play all the sounds in the roll (buffer)
    //0 stands for free and programmable
    //one playSlotCounter
	//delay(100);
    if (millis() > this->soundFinishedTimeMillis)
    {
		
        // Lucie, this code contains the remnants of an attempt to optimize the memory footprint for songs. As it seemed smart to have a specific value for filling notespaces i.e. instead of 1/8note + 1/8rest + 1/4rest + 1/2rest for a full note. Just do 1/8note + 7/8s(aka all the space needed to fill a full note duration). But, in reality, it's not worth it. AT ALL.
        //erase previous slot
        // uint8_t previous_note = this->buzzerRoll[this->playSlotCounter];
        //this->buzzerRoll[this->playSlotCounter] = BUZZER_ROLL_EMPTY_SLOT;

		if (this->getNextProgramSlot() != this->playSlotCounter)
		{	
			uint8_t current_note = this->buzzerRoll[this->playSlotCounter];

       
            //F = {[(2)^1/12]^n} * 220 Hz //220Hz for A 440 , 880 .... for other octaves
            float freq = 1;

            {
                // convert note to freq
                for (uint8_t i = 0; i < (current_note % NOTES_COUNT) + this->transpose; i++)
                {
                    //same result as: (but 2K less memory!!)
                    //freq = pow(1.059463,(buzzerRoll[this->playSlotCounter])-1 % 63);
                    freq *= 1.059463;
                }
            }
           
			uint8_t eight_notes_length_multiplier; 
		
			if (current_note <= LAST_NOTE)
            {
                freq *= BUZZER_ROLL_BASE_FREQUENCY; //frequency
				eight_notes_length_multiplier = (1 << (current_note / NOTES_COUNT)); 
				tone(this->pin, (unsigned int)freq); //duration, number is exponent of 2.
				noteFinishedEdge = true;
				
            }else{
				eight_notes_length_multiplier = current_note - LAST_NOTE; // rests are situated behind the last notes in the .h file
				this->buzzerSilent();
			}
			
			unsigned long toneLength = (unsigned long)(this->speedScale * BUZZER_ROLL_EIGHTNOTE_DURATION_MILLIS * eight_notes_length_multiplier);
			
            this->soundFinishedTimeMillis = millis() + toneLength ;
		
			//move active slot
			this->playSlotCounter = this->getNextPlaySlot();
			
		}else{
			if (noteFinishedEdge){
				noteFinishedEdge = false;
				this->buzzerSilent();
			}
			//soundFinishedTimeMillis = 10000000;
		}
    }
}

void Buzzer::changeSpeedRatio(int8_t delta)
{
    // provide -1, 0 or 1 for best results
    this->speedScale *= 1 - 0.01 * (float)delta;
}

void Buzzer::setSpeedRatio(float speedMultiplier)
{
    // argument is a long: 1 is normal speed, 0.5 is double, 2 is half, ....
    this->speedScale = speedMultiplier;
}

uint8_t Buzzer::getNextProgramSlot()
{
    //returns value of next slot
	uint8_t index = this->programSlotCounter;
	index++;
	if (index >= BUZZER_ROLL_LENGTH){
		index = 0;
	}
	return index;
	
}

uint8_t Buzzer::getNextPlaySlot()
{
    //returns value of next slot
	uint8_t index = this->playSlotCounter;
	index++;
	if (index >= BUZZER_ROLL_LENGTH){
		index = 0;
	}
	return index;
}

void Buzzer::buzzerSilent(){
	 noTone(this->pin);	
}

void Buzzer::buzzerOff()
{
    //erase contents of buzzerRoll and switch off.
    this->cleanBuzzerRoll();
    this->buzzerSilent();
}

void Buzzer::playTone(unsigned int freq, unsigned long duration_millis)
{
    // set 0 to duration_millis for indefinite length (will sound until notone or another tone command is given. )

    //this->buzzerOff();
    if (duration_millis == 0)
    {
        tone(this->pin, freq);
    }
    else
    {
        tone(this->pin, freq, duration_millis);
    }
}

uint8_t Buzzer::getBuzzerRollEmpty()
{
	 return getNextProgramSlot() == this->playSlotCounter;
}

void Buzzer::lastPlayedNoteToDisplay(char *textBuf, uint8_t *decimalPoints)
{
    noteToDisplay(textBuf, decimalPoints, this->buzzerRoll[this->playSlotCounter]);
}

void Buzzer::noteToDisplay(char *textBuf, uint8_t *decimalPoints, uint8_t note)
{
    // assume 4 chars textBuf (empty at arrival (4 spaces))
    // assumme value of decimalPoints at start is 0x00

    uint8_t noteVal = note % NOTES_COUNT;
	
    if (note > LAST_NOTE)
    {
        // rest
        textBuf[0] = 59; // ONLY_MIDDLE_SEGMENT_FAKE_ASCII '-';;
        textBuf[1] = 59; // ONLY_MIDDLE_SEGMENT_FAKE_ASCII '-';;
    }
    else
    {
		textBuf[1] = (noteVal + 45) / 12 + 48;
        bool sharps[12] = {false, true, false, false, true, false, true, false, false, true, false, true};
        char notes_chars[12] = {'A', 'A', 'B', 'C', 'C', 'D', 'D', 'E', 'F', 'F', 'G', 'G'};       
        noteVal %= 12;

        // sharp indicated by a decimal point
        *decimalPoints = 0;
        *decimalPoints = sharps[noteVal] << 0;

        textBuf[0] = notes_chars[noteVal];
    }

    textBuf[2] = 62; // SPACE_FAKE_ASCII = 62 optimized: assume empty at start

    // note length
    textBuf[3] = this->getLength(note) + 48;
}

uint8_t Buzzer::getLength(uint8_t note)
{
    if (note > LAST_NOTE){
		return note - LAST_NOTE; // lenght in multiple of 1/8.
	
	}else{
		return 0x01 << (3 - (note / NOTES_COUNT)); // 2^(3 -x) --> note length is 8,4,2,1	
	}
}

void Buzzer::nextNote(int16_t *note, bool upElseDown, bool stayInSameLength)
{
    // stayInSameLength: four note lenghts in total a fourth of the byte space is for each length
    int16_t note_without_length = *note % NOTES_COUNT;

    if (upElseDown)
    {
        note_without_length++;
        *note += 1;
    }
    else
    {
        note_without_length--;
        *note -= 1;
    }

    if (stayInSameLength)
    {
        if (note_without_length > NOTES_COUNT)
        {
            *note -= NOTES_COUNT ;
        }
        else if (note_without_length < 0)
        {
            *note += NOTES_COUNT ;
        }
    }
    else
    {
        // check boundaries over overall range
        if (*note <= 0)
        {
            *note = LAST_NOTE;
        }
        if (*note > LAST_NOTE)
        {
            *note = 0;
        }
    }
}

void Buzzer::changeNoteToNextLength(int16_t *note)
{
    *note += NOTES_COUNT;
	if (*note > LAST_NOTE){
		*note -= 4*NOTES_COUNT;
	}
}
