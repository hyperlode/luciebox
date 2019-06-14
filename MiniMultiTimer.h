#ifndef MINIMULTITIMER_h
#define MINIMULTITIMER_h

#include "Arduino.h"
#include "SuperTimer.h"
#include "Buzzer.h"

class MiniMultiTimer{
	
	#define MAX_TIMERS_COUNT 3
	#define DEFAULT_INIT_TIME_SECS 600  //600
	#define DEFAULT_RANDOM_STARTER true
	#define DEFAULT_FISHER_TIMER false
	
	enum state{
		initialized=0,
		playing,
		finished,
		paused
	};
	
	public:
		MiniMultiTimer();
		
		void setDefaults();
		void setBuzzer(Buzzer* buzzer);
		void setCountDownTimeSeconds(uint16_t initTimeSecs);
		void getDisplay(char* disp, uint8_t* playerLights, uint8_t* settingsLights);
		bool getTimerFinished(uint8_t timerIndex);
		bool checkAllTimersFinished();
		void playerButtonPressEdgeUp(uint8_t index);
		void refresh();
		void next();
		
		void init();
		void start();
		void pause();
		void continu();
		void reset();
		
	private:
		Buzzer* buzzer;
		SuperTimer timers [MAX_TIMERS_COUNT];
		state state;
		uint8_t activeTimer;
		bool randomStarter;
		bool fisherTimer;
	
};

#endif