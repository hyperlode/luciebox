#ifndef DataPlayer_h
#define DataPlayer_h

#include "SuperTimer.h"

#define ANIMATION_STOP_CODE 0x00
class DataPlayer{

  public:
    DataPlayer();
        void loadAllData(const uint8_t* data);
	
        uint16_t getStartIndexByDataSet(uint8_t data_set);
	
	void loadDataSet(uint8_t data_set);
	void nextDataSet(bool stepForwardElseBackward);
	
	
	void setSetIndexDirection(uint8_t dir);
    void next(); // next index step IN data set.
	void moveIndexSteps(int8_t steps);
	void setIndex(uint8_t index);
    
	
	void update();
	void setAutoSteps(uint8_t steps);
	void setAutoStep(bool enable);
	bool getAutoStep();
	void setAutoStepSpeed(long millis);
    
    uint8_t getActiveByte();
    uint32_t getActive32bit();
  
  private:
	//PGM_P allData;
    // PROGMEM const uint8_t* allData; 
	const uint8_t* allData;
	
    bool auto_step_enabled;
	SuperTimer auto_step_timer;
	int8_t auto_steps;
    uint8_t data_set; // the number of the active data set
	uint8_t data_set_length; // length of the active data set.
	uint8_t data_set_start_index;
	uint8_t index; // index of counter in active data set.
	
	bool set_move_forward_else_backward; // direction of index movement in active set.
    // bool value;
	
	
  
};

#endif


