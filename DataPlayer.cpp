#include "DataPlayer.h"
#include "Arduino.h"

DataPlayer::DataPlayer() {
    //constructor
	this->data_set = 0;
	this->loadDataSet(this->data_set );
	
	set_move_forward_else_backward = true;
	
	auto_step_timer.setInitTimeMillis(-1000);
	auto_step_timer.start();
}



void DataPlayer::nextDataSet(bool stepForwardElseBackward){
	if (stepForwardElseBackward){
		this->data_set++;
	}else{
		if (this->data_set>0){
			this->data_set--;
		}
	}
	this->loadDataSet(this->data_set);
}
void DataPlayer::loadAllData(const uint8_t* data){
	this->allData = data;
}
uint16_t DataPlayer::getStartIndexByDataSet(uint8_t data_set){
	//counter contains length of animation in bytes.
	uint16_t startByte = 0;
	for (uint8_t i=0;i<data_set;i++){
		startByte += (uint16_t)pgm_read_byte_near(allData + startByte); 

		// check for reach end of animation list
		if ((uint16_t)pgm_read_byte_near(allData + startByte) == ANIMATION_STOP_CODE){
			startByte = 0; //if there are for example only 2 animations,and 4 is given, will continue to overflow. (modulo)
		}
	}
	return startByte;
}

void DataPlayer::loadDataSet(uint8_t data_set){
	this->data_set = data_set;
	this->data_set_start_index = this->getStartIndexByDataSet(data_set); // animation offset (start byte)
	this->data_set_length = (int16_t)pgm_read_byte_near(allData + this->data_set_start_index) - 1; // length of animation (one byte less than read, because first byte is the length byte)
	this->index= 0;
}

uint8_t DataPlayer::getActiveByte(){
	return pgm_read_byte_near(allData + (this->data_set_start_index + 1) + this->index );	
}

uint32_t DataPlayer::getActive32bit(){
	uint32_t dw = 0;
	for (uint8_t i=0;i<4;i++){
		dw |= (uint32_t)pgm_read_byte_near(allData + (this->data_set_start_index + 1) + (this->index + i)) << (8*i); //* 4 --> 4 bytes per dword
	}
	return dw;
}

void DataPlayer::setAutoStep(bool enable){
	this->auto_step_enabled = enable;
	this->auto_step_timer.start();
	this->setIndex(0); // restart data set index
}

bool DataPlayer::getAutoStep(){
	return this->auto_step_enabled;
}
	
void DataPlayer::setAutoStepSpeed(long millis){
	this->auto_step_timer.setInitTimeMillis(millis);
	//this->auto_step_timer.start();
}

void DataPlayer::setAutoSteps(uint8_t steps){
	this->auto_steps = steps;
}

void DataPlayer::moveIndexSteps(int8_t steps){
	//this->setSetIndexDirection(steps>0);
	
	for (uint8_t i=0;i<abs(steps);i++){
		this->next();
	}
}

void DataPlayer::next(){
	// only take into account the data bytes, not the "lenght byte" in the beginning of a data set.
	// will always be circular: if limit reached, go back to the other side.
	if (this->set_move_forward_else_backward){
		if (this->index >= this->data_set_length - 1){
			this->index = 0;
		}else{
			this->index++;
		}
	}else{
		if (this->index == 0){
			this->index = this->data_set_length - 1;
		}else{
			this->index--;
		}
	}
}
void DataPlayer::setIndex(uint8_t index){
	// for manual reset: set to 0.
	this->index = index;
}

void DataPlayer::setSetIndexDirection(uint8_t dir){
	// 0: forward
	// 1: backward
	// 2: toggle
	
	if (dir == 2){
		//Serial.println("toggle");
		this->set_move_forward_else_backward = !this->set_move_forward_else_backward; 
	}else{
		this->set_move_forward_else_backward = (bool)dir;
	}
}

void DataPlayer::update(){
	// this->auto_step_timer()
	if (!this->auto_step_timer.getTimeIsNegative()){
		//if (this->auto_step_enabled){
			this->moveIndexSteps(this->auto_steps);
		//}
		this->auto_step_timer.start();
	}
}
