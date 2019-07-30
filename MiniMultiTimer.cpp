#include "MiniMultiTimer.h"


MiniMultiTimer::MiniMultiTimer(){
	 // this->multiTimer.setDefaults();
	 setDefaults();
}


void MiniMultiTimer::setDefaults(){
	// general init
	this->fisherTimer = DEFAULT_FISHER_TIMER;
	this->randomStarter = DEFAULT_RANDOM_STARTER;
	this->timers_count = DEFAULT_TIMERS_COUNT;
	this->setAllInitCountDownTimeSecs(DEFAULT_INIT_TIME_SECS);
	
	this->state = initialized;
	this->activeTimer = 0;
}
void MiniMultiTimer::setBuzzer(Buzzer* buzzer){
	this->buzzer = buzzer;
	this->initTimeSecs = DEFAULT_INIT_TIME_SECS;
	
}
// state MiniMultiTimer::getState(){
	// return this->state;
// }

void MiniMultiTimer::setTimersCount(uint8_t timers_count){
	if (this->state == setTimers){
		this->timers_count = timers_count;
		this->activeTimer = 0;
	}
}

void MiniMultiTimer::setStateTimersCount(bool set){
	if(!set && this->state == setTimers){
		this->state = initialized;
	}else if (set && this->state == initialized){
		this->state = setTimers;
	}
}

uint16_t MiniMultiTimer::getIndexedTime(uint8_t index){
	// instead of all seconds, only 
	//index max = length of timeDialDiscreteSeconds
	return timeDialDiscreteSeconds[index];
}

void MiniMultiTimer::setAllInitCountDownTimeSecs(uint16_t initTimeSecs){
	if (this->state == initialized){
		this->initTimeSecs = initTimeSecs;
		for(uint8_t i=0;i<MAX_TIMERS_COUNT;i++){
			this->timers[i].setInitCountDownTimeSecs(this->initTimeSecs );
		}
	}
}

void MiniMultiTimer::init(){
	for(uint8_t i=0;i<MAX_TIMERS_COUNT;i++){
		this->timers[i].reset();
		this->timers[i].setInitCountDownTimeSecs(this->initTimeSecs);
		//this->timers[i].startPaused(true);
	}
	this->state = initialized;
	
	// specific
	this->activeTimer = 0;
}

void MiniMultiTimer::playerButtonPressEdgeUp(uint8_t index){
	// every timer index is linked to a button index.
	
	if (this->state == initialized){
		// Serial.println(index);
		// Serial.println(this->timers_count);
		// Serial.println((index+1) <= this->timers_count);
		if ( (index+1) <= this->timers_count){
			this->activeTimer = index;
			// Serial.println("accgge");
			// Serial.println(this->activeTimer);
			// Serial.println(index);
		}	
	}else if (this->state == playing){
		if (this->activeTimer == index){
			this->next();
			(*this->buzzer).programBuzzerRoll(35);
		}else{
			(*this->buzzer).programBuzzerRoll(129);	
		}
	}else if (this->state == paused){
		(*this->buzzer).programBuzzerRoll(230);	
	}
}

void MiniMultiTimer::setStatePause(bool set){
	// pause button is latching
	if(set && this->state == playing){
		this->pause();
	}else if (!set && this->state == paused){
		this->continu();
	}
}

void MiniMultiTimer::start(){
	//start and pause all timers.
	for (uint8_t i=0;i<this->timers_count;i++){
		this->timers[i].start();
		this->timers[i].pause();
	}
	this->timers[this->activeTimer].continu();
	this->state = playing;
}

void MiniMultiTimer::pause(){
	this->state = paused;
	this->timers[this->activeTimer].pause();
}

void MiniMultiTimer::continu(){
	this->state = playing;
	this->timers[this->activeTimer].continu();
}

void MiniMultiTimer::refresh(){
	
	if (this->state == playing){
		
		//check all timers elapsed
		if (this->checkAllTimersFinished()){
			this->state = finished;
		}else{
			//check timers
			uint8_t previousActiveTimer = this->activeTimer;
			if (getTimerFinished(this->activeTimer)){
				this->next();
			}
		}
	}else if (this->state == paused){
		
	}else if (this->state == finished){
		
	}else if (this->state == setTimers){
		
	}
}

void MiniMultiTimer::getDisplay(char* disp, uint8_t* playerLights, uint8_t*	 settingsLights){
	//what should be showing on the display right now?
	*playerLights = 0b00000000; //lsb is timer 0, 2nd bit is timer 1, ....
	*settingsLights = 0b00000000; // bit0 = pause light
	if ( this-> state == initialized){
		this->timers[this->activeTimer].getTimeString(disp+1);	
		
		for (uint8_t i=0;i<this->timers_count;i++){
			if ( i != this->activeTimer  || millis()%250 > 125){
				// active timer is blinking. others are solid on.
				*playerLights |= 1 << i;
			}
		}
	}else if (this->state == playing ){
		this->timers[this->activeTimer].getTimeString(disp+1);	
		
		// active timer seconds blink
		if (this->timers[this->activeTimer].getInFirstGivenHundredsPartOfSecond(500)){
			*playerLights |= 1 << this->activeTimer;
		}
		
		// other alive timers solid on.
		for (uint8_t i=0;i<this->timers_count;i++){
			//other lights solid on if still alive.
			if (i != this->activeTimer && !this->getTimerFinished(i) ) {
				*playerLights |= 1 << i;
			}
		}
		
	}else if (this->state == finished){
		disp[1] = 32;	
		disp[2] = 69;	
		disp[3] = 78;	
		disp[4] = 68;	
		
		//fast blink last surviving timer light.
		if (millis()%250 > 125){
			*playerLights |= 1 << this->activeTimer;
		}
		
	}else if (this->state == paused){
		disp[1] = 80;	
		disp[2] = 65;	
		disp[3] = 'U';	
		disp[4] = 83;	
		
		*settingsLights |= 0b00000001; //pause light on.
		
		// timer lights
		for (uint8_t i=0;i<this->timers_count;i++){
			//other lights solid on if still alive.
			if (i == this->activeTimer){
				if (millis()%250 > 125){
					*playerLights |= 1 << i;
				}
			}else if( !this->getTimerFinished(i) ) {
				*playerLights |= 1 << i;
			}
		}
		
	}else if (this->state == setTimers){
		disp[1] = 'S';	
		disp[2] = 'E';	
		disp[3] = 'T';	
		disp[4] = ' ';
		// Serial.println(potentio->getValueMapped(1,3));
		
		// all active timers lights on
		for (uint8_t i=0;i<this->timers_count;i++){
			*playerLights |= 1 << i;
		}
	}
	//#ifdef DEBUG_MINIMULTITIMER
}

bool MiniMultiTimer::getTimerFinished(uint8_t timerIndex){
	return !this->timers[timerIndex].getTimeIsNegative();
}

bool MiniMultiTimer::checkAllTimersFinished(){
	uint8_t count = 0;
	for (uint8_t i=0;i<this->timers_count;i++){
		this->getTimerFinished(i) ? count++ : count+=0;
	}
	return count == this->timers_count;
}

void MiniMultiTimer::next(){
	// don't check for everybody dead here. If only one timer not finished, will return same activeTimer.
	if (this->checkAllTimersFinished()){
		return ;
	};
	
	if (this->state == playing){
		this->timers[this->activeTimer].pause();
		do{
			this->activeTimer >=(this->timers_count-1) ? this->activeTimer=0 : this->activeTimer++;
		}while(this->getTimerFinished(this->activeTimer)  //if finished go to next timer.
		);	
		this->timers[this->activeTimer].continu();
	}
}