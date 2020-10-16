#include "RotaryEncoderDial.h"
#include "Arduino.h"
#include "PretbakSettings.h"

//empty constructor
RotaryEncoderDial::RotaryEncoderDial()
{
  // this->increment = 1;
  // this->encoderPos = 0;

  this->sensitivity = 0;
  this->sensitivity_counter = 0;
  this->maxValue = 32767;
  this->minToMax = false;
}

int8_t RotaryEncoderDial::getValueChanged(){
  // only take into account small increases. if it jumps. 
  return this->delta_memory;
}

void RotaryEncoderDial::setRange(int16_t maxValue, boolean minToMax){
  this->maxValue = maxValue;
  this->minToMax = minToMax;
  if (this->value > this->maxValue){
    this->value = this->maxValue;
  }
}

int16_t RotaryEncoderDial::getValueLimited(int16_t maxValue, boolean jumpAtEnd){
  this->setRange (maxValue, jumpAtEnd);
  return this->getValue();
}

int16_t RotaryEncoderDial::getValueMapped(int16_t minValue, int16_t maxValue){
  // DEPRECATED!
  // hacked. Not yet nicely done!!!
  this->setRange (maxValue, false);
  return this->getValue();
}

void RotaryEncoderDial::refresh(){
  noInterrupts();
  this->delta_memory = this->delta;
  this->delta = 0;
  interrupts();
  this->value += this->delta_memory; // non atomic action (16bit value on 8 bit processor), so an interrupt can ruin it! 


  if (this->value < 0){
    if (this->minToMax){
      this->value = this->maxValue;

    }else{
      this->value = 0;
    }
  }else if (this->value > this->maxValue){
    if (this->minToMax){
      this->value = 0;

    }else{
      this->value = this->maxValue;
    }
  }

  // edge detection here, so it stays 'on' until next refresh.
  // this->value_changed = this->value - this->value_memory;
  // #ifdef ENABLE_SERIAL
  
  // #endif
  
  this->value_memory = this->value;
}

void RotaryEncoderDial::setValue(int16_t value){
  // noInterrupts();
  // this->delta_memory = 0;
  // interrupts();
  this->value = value;
  this->refresh();
  // this->value_changed = 0; // let's not detect edges if we set the value manually.
}

int16_t RotaryEncoderDial::getValue(){
  return this->value;
}

void RotaryEncoderDial::setSensitivity(uint8_t sensitivity){
  // number of steps for encoder before a step is detected and the value incremented.
  // 1 is no extra steps added.
  // 96 is full turn per step.
  this->sensitivity = sensitivity-1;
  this->sensitivity_counter = 0;
}

// void RotaryEncoderDial::setIncrement(uint8_t increment){
//   this->increment = increment;
// }

void RotaryEncoderDial::setPins(byte pinChannelA, byte pinChannelB ){
  // https://www.onetransistor.eu/2019/05/arduino-class-interrupts-and-callbacks.html     
    this->pinChannelA = pinChannelA; // MUST BE pin 2 for arduino uno interrupt CHANGE to work
    this->pinChannelB = pinChannelB; // MUST BE pin 3 for arduino uno interrupt CHANGE to work

    pinMode(this->pinChannelA, INPUT);
    pinMode(this->pinChannelB, INPUT);

    digitalWrite(this->pinChannelA, HIGH);
    digitalWrite(this->pinChannelB, HIGH);

    this->A_set = 0;
    this->B_set = 0;
    this->A_waitfor = 1;
    this->B_waitfor = 1;
    this->A_changedir_waitfor = 1;
    this->B_changedir_waitfor = 1;

    this-> ccwElseCw = 1;
    initialize();
}

void RotaryEncoderDial::initialize(){
  this->A_set = digitalRead(this->pinChannelA);
  this->B_set = digitalRead(this->pinChannelB);  
  this->setNewState();
  this->ccwElseCw = 0;
}

// Interrupt handling

void RotaryEncoderDial::interruptChannelA() {
  this->A_set = digitalRead(this->pinChannelA);
  this->checkState();
}

void RotaryEncoderDial::interruptChannelB() {
  this->B_set = digitalRead(this->pinChannelB);
  this->checkState();
}

void RotaryEncoderDial::checkState(){
  // check the state if it satisfies the requirements for a step or a change of direction
  if (this->A_set == this->A_waitfor && this->B_set == this->B_waitfor){
    // next step

    if (this->sensitivity_counter == this->sensitivity){
      if (ccwElseCw){
        this->delta--;
      }else{
        this->delta++;
      }
     
      this->sensitivity_counter = 0;

    }else{
      this->sensitivity_counter++;
    }
    this->setNewState();
    
  }else if (this->A_set == this->A_changedir_waitfor && this->B_set == this->B_changedir_waitfor){
    // change of dir
    this->ccwElseCw = !this->ccwElseCw;
    this->setNewState();
    this->sensitivity_counter = 0;
  }
}

void RotaryEncoderDial::setNewState(){
  // to visualize: best to draw the two channels duty cycle for a full rotation and look what's going on. Channel phases are (should be) shifted 90degrees.
  this->A_waitfor = (this->B_set == this->ccwElseCw);
  this->B_waitfor = (this->A_set != this->ccwElseCw);
  this->A_changedir_waitfor = !this->A_set;
  this->B_changedir_waitfor = !this->B_set; 
}