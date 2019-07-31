#include "MiniMultiTimer.h"


MiniMultiTimer::MiniMultiTimer(){
	 // this->multiTimer.setDefaults();
	 setDefaults();
}


void MiniMultiTimer::setDefaults(){
	// general init
	//this->fisherTimer = DEFAULT_FISHER_TIMER;
    this->fischerSecs = DEFAULT_FISCHER_TIMER_SECS;
	this->randomStarter = DEFAULT_RANDOM_STARTER;
	this->timers_count = DEFAULT_TIMERS_COUNT;
	this->setAllInitCountDownTimeSecs(DEFAULT_INIT_TIME_SECS);
	
	this->state = initialized;
	this->activeTimer = 0;
    this->timerDisplayed = this->activeTimer ;

    
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
        this->timerDisplayed = this->activeTimer ;

	}
}

void MiniMultiTimer::setStateTimersCount(bool set){
	if(!set && this->state == setTimers){
		this->state = initialized;
	}else if (set && this->state == initialized){
		this->state = setTimers;
	}
}

void MiniMultiTimer::setFischerTimer(uint16_t seconds){
    this->fischerSecs = seconds;
}

void MiniMultiTimer::setStateFischerTimer(bool set){
    if(!set && this->state == setFischer){
		this->state = initialized;
	}else if (set && this->state == initialized){
		this->state = setFischer;
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

void MiniMultiTimer::playerButtonPressEdgeDown(uint8_t index){
    // if button released, always display active Timer time.
	// if (index +1) <= this->timers_count){
		this->timerDisplayed = this->activeTimer;
	// }
}

void MiniMultiTimer::playerButtonPressEdgeUp(uint8_t index){
	// every timer index is linked to a button index.
	
	if (this->state == initialized){
		if ( (index+1) <= this->timers_count){
			this->activeTimer = index;
            this->timerDisplayed = this->activeTimer ;
		}	
	}else if (this->state == playing){
		if (this->activeTimer == index){
			this->next();
			(*this->buzzer).programBuzzerRoll(35);
		}else if ( (index+1) <= this->timers_count){
			(*this->buzzer).programBuzzerRoll(129);	
			this->timerDisplayed = index; //display time of pressed timer button 
			
		}
	}else if (this->state == paused){
		(*this->buzzer).programBuzzerRoll(230);	
        this->timerDisplayed = index; //display time of pressed timer button 
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
        
	}else if (this->state == initialized){
        
	}else if (this->state == paused){
		
	}else if (this->state == finished){
		
	}else if (this->state == setTimers){
        
	}
}

void MiniMultiTimer::getDisplay(char* disp, uint8_t* playerLights, uint8_t*	 settingsLights){
	//what should be showing on the display right now?
	*playerLights = 0b00000000; //lsb is timer 0, 2nd bit is timer 1, ....
	*settingsLights = 0b00000000; // bit0 = pause light
    
    disp[1] = ' ';	
    disp[2] = ' ';	
    disp[3] = ' ';	
    disp[4] = ' ';
    
	if ( this-> state == initialized){
		this->timers[this->activeTimer].getTimeString(disp+1);	
		
		for (uint8_t i=0;i<this->timers_count;i++){
			if ( i != this->activeTimer  || millis()%250 > 125){
				// active timer is blinking. others are solid on.
				*playerLights |= 1 << i;
			}
		}
	}else if (this->state == playing ){
        
        // displayed timer is not always the active timer (i.e. non active player wants to check his time).
		this->timers[this->timerDisplayed].getTimeString(disp+1);	
		
		// other alive timers solid on.
		for (uint8_t i=0;i<this->timers_count;i++){
			
            if (i == activeTimer){
                // active timer seconds blink
                if (this->timers[this->activeTimer].getInFirstGivenHundredsPartOfSecond(500)){
                    *playerLights |= 1 << i;
                }
            }else if (i == timerDisplayed){
                // displayed timer is not always the active timer (i.e. non active player wants to check his time).
                if (millis()%250 > 125){
                    *playerLights |= 1 << i;
                }
            }else if (i != this->activeTimer && !this->getTimerFinished(i) ) {
                //other lights solid on if still alive.
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
		
        // pause light blinking.
        if (millis()%250 > 125){
            *settingsLights |= 0b00000001; //pause light on.
        }

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
		
	}else if (this->state == setFischer){
        if (millis()%1000 > 650){
        disp[1] = 'F';	
		disp[2] = 'I';	
		disp[3] = 'S';	
		disp[4] = 'H';
        }else{
            intToDigitsString(disp+1, (unsigned int) this->fischerSecs, false);  // utilities lode
        }
	}else if (this->state == setTimers){
        if (millis()%1000 > 650){
			disp[1] = 'Q';	
            disp[2] = 'T';	
            disp[3] = 'Y';	
            disp[4] = ' ';
		}else{
            intToDigitsString(disp+1, (unsigned int) this->timers_count, false);  // utilities lode
        }
		
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
        
        // add fischer timer (disabled just means: zero seconds).
		this->timers[this->activeTimer].setOffsetInitTimeMillis(-1000 * long(this->fischerSecs));
        
		do{
			this->activeTimer >=(this->timers_count-1) ? this->activeTimer=0 : this->activeTimer++;
		}while(this->getTimerFinished(this->activeTimer)  //if finished go to next timer.
		);	
		this->timers[this->activeTimer].continu();
	}
}