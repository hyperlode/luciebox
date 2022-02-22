#include "Potentio.h"
#include "Arduino.h"

//empty constructor
Potentio::Potentio()
{
}
void Potentio::setPin(byte pin)
{
    this->analogPin = pin;
}

uint16_t Potentio::getValue()
{
    return (int16_t)analogRead(this->analogPin);
}

long Potentio::getValueMapped(long minimumValue, long maximumValue)
{
    long value = map((long)this->getValueStable(), 0, 1023, minimumValue, maximumValue + 1);
    if (value > maximumValue)
    {
        //+1 is a little hack because map function has only at the end the maximum value, better would be to middle the values out (range wise).
        value = maximumValue;
    }
    else if (value < minimumValue)
    {
        value = minimumValue;
    }
    return value;
}

uint16_t Potentio::getValueStable()
{
    // returns the STABLE value
    return this->potentio_value_stable;
}

bool Potentio::getValueStableChangedEdge()
{
    //will only trigger if a change in STABLE value is detected.
    return this->potentio_value_stable_changed;
}

bool Potentio::getLastStableValueChangedUp()
{
    // if last stable value change resulted in a higher value, return True, if lower, return False.
    return this->potentio_value_last_change_up_else_down;
}

bool Potentio::increaseSubtractAtChange(int16_t *container, uint8_t amount)
{
    int16_t original_value = *container;

    if (getValueStableChangedEdge())
    {
        *container += (1 - (2 * getLastStableValueChangedUp())) * amount;
    }
    return *container != original_value;
}

void Potentio::refresh()
{
    //call this to update the value of the analog input. call multiple times to check for stable value.

    potentio_value = this->getValue();

    if (potentio_value > potentio_value_stable + POTENTIO_SENSITIVITY || potentio_value < potentio_value_stable - POTENTIO_SENSITIVITY)
    {
        this->potentio_value_stable_changed = true;                                                   //simple edge detection
        this->potentio_value_last_change_up_else_down = this->potentio_value_stable > potentio_value; // < or > depending on desired rotation direction
        this->potentio_value_stable = potentio_value;
#ifdef DEBUG_POTENTIO
        Serial.println(potentio_value_stable);
#endif
    }
    else
    {
        this->potentio_value_stable_changed = false; //simple edge detection
    }

    // this->selectorPreviousStableValue = this->selectorCurrentStableValue;
    // if (getSelectorValueRaw() == this->selectorPreviousValue){
    // if (this->cyclesValueIsStable < CYCLES_BEFORE_CONSIDERED_STABLE){
    // // take care for overflows.
    // this->cyclesValueIsStable++;
    // }
    // }else{
    // this->cyclesValueIsStable = 0;
    // }
    // if (this->cyclesValueIsStable >=CYCLES_BEFORE_CONSIDERED_STABLE ){
    // this->selectorCurrentStableValue = getSelectorValueRaw();
    // }
    // this->selectorPreviousValue = getSelectorValueRaw();
}
