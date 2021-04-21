#ifndef MINIMULTITIMER_h
#define MINIMULTITIMER_h

#include "Arduino.h"
#include "SuperTimer.h"
#include "Buzzer.h"
#include "Utilitieslode.h"

const uint16_t timeDialDiscreteSeconds[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 15, 20, 25, 30, 45, 60, 75, 90, 120,
	150, 180, 210, 240, 270, 300, 330, 360, 390, 420,
	450, 480, 510, 540, 570, 600, 660, 720, 780, 840,
	900, 960, 1020, 1080, 1140, 1200, 1260, 1320, 1380, 1440,
	1500, 1560, 1620, 1680, 1740, 1800, 2100, 2400, 2700, 3000,
	3300, 3600, 3900, 4200, 4500, 4800, 5100, 5400, 6000, 6600,
	7200, 7800, 8400, 9000, 9600, 10200, 10800, 12600, 14400, 16200,
	18000, 19800, 21600, 23400, 25200, 27000, 28800, 30600, 32400, 34200,
	36000};

//1860,1920,1980,2040,2100,2160,2220,2280,2340,2400,2460,
//2520,2580,2640,2700,2760,2820,2880,2940,3000};

class MiniMultiTimer
{

#define MAX_TIMERS_COUNT 4
#define DEFAULT_INIT_TIME_SECS 600 //600
#define DEFAULT_FISCHER_TIMER_SECS 0
#define DEFAULT_TIMERS_COUNT 2

#define LIGHT_PAUSE 0x01
#define LIGHT_PLAYING 0x02
#define LIGHT_FISCHER 0x04
#define LIGHT_SET_TIMERS_COUNT 0x08
#define LIGHT_SECONDS_BLINKER 0b00010000

	enum state : uint8_t
	{
		// enum state{
		initialized = 0,
		playing,
		finished,
		paused,
		setTimers,
		setFischer
	};

public:
	MiniMultiTimer();

	void setDefaults();
	void setBuzzer(Buzzer *buzzer);
	uint16_t getIndexedTime(uint8_t index);
	void setAllInitCountDownTimeSecs(uint16_t initTimeSecs);
	void setTimerInitCountTimeSecs(uint8_t timer, uint16_t initTimeSecs);
	void getDisplay(char *disp, uint8_t *playerLights, uint8_t *settingsLights);
	bool getTimerFinished(uint8_t timerIndex);
	bool checkAllTimersFinished();
	void playerButtonPressEdgeUp(uint8_t index);
	void playerButtonPressEdgeDown(uint8_t index);
	void setTimersCount(uint8_t timers_count);
	void setFischerTimer(uint16_t seconds);
	void setStateTimersCount(bool set);
	void setStateFischerTimer(bool set);
	void setStatePause(bool set);
	void refresh();
	void buzzerRefresh(bool alarm);
	void next();
	//state getState();

	void init();
	void start();
	void pause();
	void continu();
	void reset();

private:
	Buzzer *buzzer;
	SuperTimer timers[MAX_TIMERS_COUNT];
	state state;
	uint8_t activeTimer;
	uint8_t timerDisplayed;
	bool randomStarter;
	bool fisherTimer;

	uint16_t initTimeSecs;
	uint8_t timers_count;

	uint16_t fischerSecs;
};

#endif