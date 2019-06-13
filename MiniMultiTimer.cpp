#include "MiniMultiTimer.h"


MiniMultiTimer::MiniMultiTimer(){
}

void MiniMultiTimer::init(){
	
	// general init
	this->fisherTimer = DEFAULT_FISHER_TIMER;
	this->randomStarter = DEFAULT_RANDOM_STARTER;
	
	for(uint8_t i=0;i<MAX_TIMERS_COUNT;i++){
		this->timers[i].setInitCountDownTimeSecs(DEFAULT_INIT_TIME_SECS);
	}
	this->state = initialized;
	
	// specific
	this->activeTimer = 0;
	
	//start and pause all timers.
	for (uint8_t i=0;i<MAX_TIMERS_COUNT;i++){
		this->timers[i].start();
		this->timers[i].pause();
	}
	this->timers[this->activeTimer].continu();
	this->state = playing;
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
	}else if (this->state == finished){
		
	}
}

void MiniMultiTimer::getDisplay(char* disp){
	//what should be showing on the display right now?
	if (this->state == playing){
		this->timers[this->activeTimer].getTimeString(disp+1);	
	}else if (this->state == finished){
		disp[1] = 32;	
		disp[2] = 69;	
		disp[3] = 78;	
		disp[4] = 68;	
	}
}

bool MiniMultiTimer::getTimerFinished(uint8_t timerIndex){
	return !this->timers[timerIndex].getTimeIsNegative();
}

bool MiniMultiTimer::checkAllTimersFinished(){
	uint8_t count = 0;
	for (uint8_t i=0;i<MAX_TIMERS_COUNT;i++){
		this->getTimerFinished(i) ? count++ : count+=0;
	}
	return count == MAX_TIMERS_COUNT;
}

void MiniMultiTimer::next(){
	// don't check for everybody dead here. If only one timer not finished, will return same activeTimer.
	if (this->checkAllTimersFinished()){
		return ;
	};
	
	if (this->state == playing){
		this->timers[this->activeTimer].pause();
		do{
			this->activeTimer >=(MAX_TIMERS_COUNT-1) ? this->activeTimer=0 : this->activeTimer++;
		}while(this->getTimerFinished(this->activeTimer)  //if finished go to next timer.
		);	
		this->timers[this->activeTimer].continu();
	}
}