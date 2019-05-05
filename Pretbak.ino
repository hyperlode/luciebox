#include "PotentioSelector.h"

#define PIN_SELECTOR_DIAL A0
PotentioSelector selectorDial;


void setup() {
  // put your setup code here, to run once:
  

  selectorDial.setPin(PIN_SELECTOR_DIAL);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  selectorDial.refresh();
  
  if (selectorDial.getValueChangedEdge()) {
    Serial.println(selectorDial.getSelectorValue());
    switch (selectorDial.getSelectorValue()) {
      case 0:
        Serial.println("zeor");
        break;
      case 1:
        break;
      default:
        break;
    }
  }   
}
