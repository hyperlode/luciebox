#include "SuperTimer.h"
SuperTimer::SuperTimer(){
	setIsStarted(false);
	setIsPaused(false);
	this->callibrationConstant = 1; //default is 1 // DISABLED
	this-> initTimeMillis = 0;
}


//INIT TIME 

void SuperTimer::setInitTimeMillis(long milliSeconds){
//	//if not yet started it is easy, if already started, we don't affect the original time, only the runtime. so, when timer resets, we see again the original time.
//	if (getIsStarted()){
//		//if started, we don't touch the initial init time. only the ingame time!
//		if (getIsPaused()){
//			//this->startedMillis = this->pauseStartedMillis + milliSeconds;
//			//this->pauseStartedMillis = this->pauseStartedMillis - milliSeconds; //to test debug
////			this->startedMillis = this->startedMillis - milliSeconds; //to test debug
//			this->startedMillis =  this->pauseStartedMillis - milliSeconds;
//		}else{
//			this->startedMillis =  (long)getMillisCallibrated() - milliSeconds; // we have after a pause a new starting point, but, from that new starting point, we subtract the already exceeded time.
//		}
//		
//	}else{
//		this-> initTimeMillis = milliSeconds;	
//	}	

  this-> initTimeMillis = milliSeconds;  
}


void SuperTimer::setOffsetInitTimeMillis(long offsetMillis){
	//make distinction between change during runtime and not, if during runtime: the original set time will not be affected.
	//if offset is negative, time goes down. if positive, time goes up.
	if (getIsStarted()){
		if (getIsPaused()){
			this->startedMillis =  this->startedMillis - offsetMillis;
		}else{
			this->startedMillis =  this->startedMillis - offsetMillis; //-// we have after a pause a new starting point, but, from that new starting point, we subtract the already exceeded time.
		}
	}else{
		this-> initTimeMillis += offsetMillis;	
	}
}

// void SuperTimer::setInitTimeSecs(int seconds){
	// setInitTimeMillis((long)(seconds) * 1000); //FIRST convert seconds to long, THEN *1000!!!
// }

void SuperTimer::setInitCountDownTimeSecs(unsigned int countDownSeconds){
	this->setInitCountDownTimeMillis(((long)countDownSeconds) * 1000);
}

void SuperTimer::setInitCountDownTimeMillis(long countDownMillis){
  setInitTimeMillis((countDownMillis) * -1);
}

long SuperTimer::getInitTimeMillis(){
	return this->initTimeMillis;
}

int SuperTimer::getInitTimeSecs(){
	return timeMillisToSeconds(getInitTimeMillis());
}

//START BEHAVIOUR

void SuperTimer::start(long initTimeMillis){
	//positive initTimeMillis is chrono with offset..., negative is countdowntimer
	this-> initTimeMillis = initTimeMillis;
	start();
}

void SuperTimer::startCountDownWithSeconds(unsigned int countDownSeconds){
	start(-1000 * (long)countDownSeconds);	
}

void SuperTimer::start(){
	startPaused(false);
}

void SuperTimer::startPaused(bool paused){
	startComplete(paused, getMillisCallibrated());
}

void SuperTimer::startComplete(bool startInPauseMode, unsigned long startedMillisPassing){
	//boolean argument: if true, start in paused mode, is better than first start and then pause (no lag at all!)
	//unsigned long: provided time for correcting the start value (if timer has to connect seamlessly to another event, include the millis() of the event (i.e. the ending of another timer)
		//Serial.println("superstarted:");

		this->startedMillis = (long)startedMillisPassing - this->initTimeMillis; //if initTimeMillis is positive: chronooffset, if negative, countdowntimer (time will be negative until zero reached)
		setIsStarted( true);
		
		//make sure if we start in pause mode, that the exact time of start is the start of the pause.
		if (startInPauseMode){
			setIsPaused(true);
			this->pauseStartedMillis = (long)getMillisCallibrated();
		}else{
			setIsPaused(false);
		}
}

bool SuperTimer::getIsStarted(){
	//return this->isStarted;
	return getBit(&this->boolContainer, BITLOCATION_ISSTARTED);
	
}

void SuperTimer::setIsStarted(bool isStarted){
	//this->isStarted = isStarted;
	setBit(&this->boolContainer, isStarted, BITLOCATION_ISSTARTED);
	
}

//PAUSE BEHAVIOUR

void SuperTimer::continu(){
	paused(false);
}

void SuperTimer::pause(){
	paused(true);
}

void SuperTimer::paused(bool pause){
	if (pause){
		if (!getIsPaused()){ //only do this when not yet paused!
			this->pauseStartedMillis =  (long)getMillisCallibrated();
		}
	}else{
		if (getIsPaused()){
			this->startedMillis =  (long)getMillisCallibrated() - (this-> pauseStartedMillis - this-> startedMillis); // we have after a pause a new starting point, but, from that new starting point, we subtract the already exceeded time.
		}

	}
	
	setIsPaused(pause);
}

bool SuperTimer::getIsPaused(){
	//return this->isPaused ;
	return getBit(&this->boolContainer, BITLOCATION_ISPAUSED);
	
}

void SuperTimer::setIsPaused(bool paused){
	
	setBit(&this->boolContainer, paused, BITLOCATION_ISPAUSED);

}




//RESET
void SuperTimer::reset(){
	
	setIsPaused(false);
	setIsStarted(false);
}

//MODIFY 

//INQUIRY

long SuperTimer::getTimeMillis(){
	//get the timertimer  (pauses will change startedMillis, so this time is without the pauses.)
	//if negative, countdowntimer
	//if positive, chrono
	
	if (getIsStarted()){
		if (getIsPaused()){
			return this->pauseStartedMillis - this->startedMillis;
		}else{
			return (long)getMillisCallibrated() - this->startedMillis;
      
		}
	}else{
		return getInitTimeMillis();
	}
}

// bool SuperTimer::getInitTimeElapsed(){
bool SuperTimer::getTimeIsNegative(){
	//if time is negative: counting down.
	return getTimeMillis() <= 0; 
}

long SuperTimer::getTimeSeconds(){
	//if (getTimeIsNegative()){
	//	return -1 * getTimeSecondsAbsolute();
	//}else{
	//	return getTimeSecondsAbsolute();
	//}
        return (long)getTimeSecondsAbsolute() * (1 -2*getTimeIsNegative()); //-1 if negative  
}

unsigned int SuperTimer::getTimeSecondsAbsolute(){
	return timeMillisToSeconds(getTimeMillis());
}

unsigned int SuperTimer::getTimeSecondsCountDownTimer(){
	//no negative time, if timer elapsed, then zero.
	//will always return a positive number! 
	if (getTimeIsNegative()){
		return timeMillisToSeconds(getTimeMillis());
	}else{
		return 0;
	};
}

long SuperTimer::getTimeMillisCountDownTimer(){
	//no negative time, if timer elapsed, then zero.
	//will always return a positive number! 
	
	if (getTimeIsNegative()){
		return -getTimeMillis();
	}else{
		return 0;
	}
}


void SuperTimer::getTimeString(char * textBuf){
	//will always return a string with a positive time value ...
	timeMillisToClockString(textBuf, getTimeMillis());
}





//ADMINISTRATION 
// unsigned long SuperTimer::getMillisAtLastCheck(){
	// return this->millisAtLastCheck;
// }

//callibration. should be done at a higher level. every clock should have the same speed.... , so the millis() function should be modified, 
void SuperTimer::setCallibrationMillis(float* callibrationRatio){
	// if higher than 1: clock goes faster, if lower than 1 clock goes slower
	this->callibrationConstant =  callibrationRatio;
}

float SuperTimer::getCallibrationMillis(){
	return *this->callibrationConstant;
}

unsigned long SuperTimer::getMillisCallibrated(){
  return millis(); // bridge callibration  todo
  return (unsigned long)millis(); // bridge callibration  todo
	return (unsigned long)(millis() *
#if defined DISABLE_CALIBRATION && defined ARDUINO_SIMULATION
		1
#else
		*this->callibrationConstant
#endif

	);
}
//original countdowntimer


bool SuperTimer::getInFirstGivenHundredsPartOfSecond(int hundreds){
	//calc like this: if timeleft is 9600 millis ==> first 400 millis of seconds is passed...
	// bool inFirstPartOfSecond = getTimeLeftMillis()%1000 <= 1000 - hundreds
	if (getTimeMillis() > 0 ){
		//chrono
		return abs(getTimeMillis()%1000) <= (hundreds);
	}else{
		//for chrono,
		//countdown
		
		//return getExceededMillis()%1000<=hundreds;
		return abs(getTimeMillis())%1000>=(1000-hundreds);
	}
}

bool SuperTimer::getEdgeSinceLastCallFirstGivenHundredsPartOfSecond(int hundreds, bool positiveEdge, bool negativeEdge){
	//check edge of first given hundreds since this function was last called
	// if onlyPositiveEdge = false : edge on start and end (positive and negative edge) of first given part of hundreds
	// if onlyPositiveEdge = true :  edge on start  (positive edge) of first given part of hundreds
	bool inFirstGivenHundredsPartOfSecond = getInFirstGivenHundredsPartOfSecond(hundreds);

	
	// if ( inFirstGivenHundredsPartOfSecond != getBit(&this->boolContainer,BITLOCATION_INFIRSTPARTOFSECOND_EGDE) && (!positiveEdge || inFirstGivenHundredsPartOfSecond  )){
	if ( inFirstGivenHundredsPartOfSecond != getBit(&this->boolContainer,BITLOCATION_INFIRSTPARTOFSECOND_EGDE) && ( (positiveEdge && negativeEdge) || (positiveEdge && inFirstGivenHundredsPartOfSecond) || (negativeEdge && !inFirstGivenHundredsPartOfSecond  ))){
		//previousCheckInFirstPartOfSecond = inFirstGivenHundredsPartOfSecond;
		setBit(&this->boolContainer,inFirstGivenHundredsPartOfSecond,BITLOCATION_INFIRSTPARTOFSECOND_EGDE);
		return true;
	}else{
		// previousCheckInFirstPartOfSecond = inFirstGivenHundredsPartOfSecond;
		setBit(&this->boolContainer,inFirstGivenHundredsPartOfSecond,BITLOCATION_INFIRSTPARTOFSECOND_EGDE);
		return false;
	}
}


// bool SuperTimer::getIsPaused(){
	// //return this->isPaused ;
	// return getBit(&this->boolContainer, BITLOCATION_ISPAUSED);
	
// }

// void SuperTimer::setIsPaused(bool paused){
	
	// setBit(&this->boolContainer, paused, BITLOCATION_ISPAUSED);

// }


// bool SuperTimer::secondsChangedSinceLastCheck(){
	// //did the second digit changed since the last check?
	
	// //this is the new version, but millis should be "getTimeMillisAtLastCheck."
	// // if (getTimeMillisAtLastCheck()/3600 == getMillis / 3600){
		// // return false;
	// // }else{
		// // return true;
	// // };
	
	
	// int timeSecs = getTimeSeconds();
	// int secondsLastCheckWorking = this->secondsLastCheck;
	// this-> secondsLastCheck = timeSecs;
	// if (timeSecs == secondsLastCheckWorking){
		// return false;
	// }else{
		// return true;
	// };
	
	// // bool newSecondEdge = false;
	// // //check if it is the same with previous
	// // if ( getBit(&this->boolContainer, BITLOCATION_NEWSECOND_EGDEDETECTION) == 
	
	// // //store previous
	
	
	
	// // int timeSecs = getTimeSeconds();
	// // int secondsLastCheckWorking = this->secondsLastCheck;
	// // this-> secondsLastCheck = timeSecs;
	
	// // if (timeSecs == secondsLastCheckWorking){
		// // return false;
	// // }else{
		// // return true;
	// // };
	
// }























































































































































































