#ifndef MINIMULTITIMER_h
#define MINIMULTITIMER_h

#include "Arduino.h"
#include "SuperTimer.h"

class MiniMultiTimer{
	
	#define MAX_TIMERS_COUNT 3
	#define DEFAULT_INIT_TIME_SECS 3  //600
	#define DEFAULT_RANDOM_STARTER true
	#define DEFAULT_FISHER_TIMER false
	
	enum state{
		initialized=0,
		playing,
		finished
	};
	
	public:
		MiniMultiTimer();
		void init();
		void setCountDownTimeSeconds(uint16_t initTimeSecs);
		void getDisplay(char* disp);
		bool getTimerFinished(uint8_t timerIndex);
		bool checkAllTimersFinished();
		void refresh();
		void next();
		void start();
		void pause();
		void reset();
		
	private:
		SuperTimer timers [MAX_TIMERS_COUNT];
		state state;
		uint8_t activeTimer;
		bool randomStarter;
		bool fisherTimer;
	
};

#endif