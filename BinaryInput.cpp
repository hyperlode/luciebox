#include "BinaryInput.h"


//BinaryInput::BinaryInput(){
//}
void BinaryInput::setValue(bool value){
  this->value = value;
}

bool BinaryInput::getValue(){ 
  return this->value;
}

bool BinaryInput::getValueChanged(){
  return this->value != this->previousValue;
}

void BinaryInput::refresh(){
  this->previousValue = this->value;  
}

bool BinaryInput::getEdgeDown(){
  if (this->getValueChanged()){
    return !this->value;
  }else{
    return 0;
  }
}

bool BinaryInput::getEdgeUp(){
  if (this->getValueChanged()){
    return this->value;
  }else{
    return 0;
  }
}


