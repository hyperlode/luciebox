//Written by Lode Ameije 2013 
#ifndef SuperTimer_h 
#define SuperTimer_h 
#if defined(ARDUINO) && ARDUINO >= 100 
#include "Arduino.h"
#else 
#include "WProgram.h" 
#endif 
#include "Utilitieslode.h"
class SuperTimer{ 
	public: 
		SuperTimer();
		void setInitTimeMillis(long milliSeconds);
		void setInitTimeSecs(long seconds);
		long getInitTimeMillis();
		int getInitTimeSecs();
		void setInitCountDownTimeSecs(unsigned int countDownSeconds);
    void setInitCountDownTimeMillis(long countDownMillis);

		void start(long initTimeMillis);
		void startCountDownWithSeconds(unsigned int countDownSeconds);
		void start();
		void startPaused(bool paused);
		void startComplete(bool startInPauseMode, unsigned long startedMillis);
		bool getIsStarted();
		void setIsStarted(bool isStarted);

		void continu();
		void pause();
		void paused(bool pause);
		bool getIsPaused();
		

		void reset();

		// void offsetRunningTimeMillis(long offsetMillis);
		void setOffsetInitTimeMillis(long offsetMillis);
		//void setNewTimeAtRunTime (long timeSetMillis);

		long getTimeMillis();
		bool getTimeIsNegative();
		long getTimeSeconds();
		unsigned int getTimeSecondsAbsolute();
		unsigned int getTimeSecondsCountDownTimer();
		long getTimeMillisCountDownTimer();
		void getTimeString(char * textBuf);

		//unsigned long getMillisAtLastCheck();
		void setCallibrationMillis(float* callibrationRatio);
		float getCallibrationMillis();
		unsigned long getMillisCallibrated();

		bool getInFirstGivenHundredsPartOfSecond(int hundreds);
		bool getEdgeSinceLastCallFirstGivenHundredsPartOfSecond(int hundreds, bool positiveEdge, bool negativeEdge);
	//	bool secondsChangedSinceLastCheck();
		
	private: //not accessible in subclasses (work with getters and setter to access then) 
	//protected: 
		void setIsPaused(bool paused); //only for internal use, to set the variable...
		//bool isStarted;
	//	bool isPaused;
		
		//int secondsLastCheck; //get rid of this one! 
		//unsigned long millisAtLastCheck; //check if this one is needed (stores the millis(), It could be usefull if we stored the time!)
		
		float* callibrationConstant;

		long startedMillis;
		long pauseStartedMillis;
		long initTimeMillis;

		//bool previousCheckInFirstPartOfSecond;
		
		uint8_t boolContainer;
		#define BITLOCATION_ISSTARTED 0
		#define BITLOCATION_ISPAUSED 1
		#define BITLOCATION_INFIRSTPARTOFSECOND_EGDE 2
	//	#define BITLOCATION_NEWSECOND_EGDEDETECTION 3
		
}; 
#endif
































































































































