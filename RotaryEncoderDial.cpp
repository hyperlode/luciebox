#include "RotaryEncoderDial.h"
#include "Arduino.h"
#include "PretbakSettings.h"

//empty constructor
RotaryEncoderDial::RotaryEncoderDial()
{
  this->increment = 1;
}

bool RotaryEncoderDial::getValueChanged(){
  return this->value_changed;
}

void RotaryEncoderDial::refresh(){
  noInterrupts();
  this->value = this->encoderPos; // non atomic action (16bit value on 8 bit processor), so an interrupt can ruin it! 
  interrupts();

  // edge detection here, so it stays 'on' until next refresh.
  this->value_changed = this->value_memory != this->value;
  
  this->value_memory = this->value;
}

void RotaryEncoderDial::setValue(int16_t value){
  noInterrupts();
  this->encoderPos = value;
  interrupts();
  this->refresh();
  this->value_changed = false; // let's not detect edges if we set the value manually.
}

int16_t RotaryEncoderDial::getValue(){
  return this->value;
}

void RotaryEncoderDial::setIncrement(uint8_t increment){
  this->increment = increment;
}

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
    if (ccwElseCw){
      this->encoderPos -=this->increment;
    }else{
      this->encoderPos +=this->increment;
    }
    this->setNewState();
    
  }else if (this->A_set == this->A_changedir_waitfor && this->B_set == this->B_changedir_waitfor){
    // change of dir
    this->ccwElseCw = !this->ccwElseCw;
    this->setNewState();
  }
}

void RotaryEncoderDial::setNewState(){
  // to visualize: best to draw the two channels duty cycle for a full rotation and look what's going on. Channel phases are (should be) shifted 90degrees.
  this->A_waitfor = (this->B_set == this->ccwElseCw);
  this->B_waitfor = (this->A_set != this->ccwElseCw);
  this->A_changedir_waitfor = !this->A_set;
  this->B_changedir_waitfor = !this->B_set; 
}