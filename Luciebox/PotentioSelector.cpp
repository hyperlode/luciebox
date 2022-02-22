#include "PotentioSelector.h"
#include "Arduino.h"

//for selector switch where

//empty constructor
PotentioSelector::PotentioSelector()
{
    this->debounceMillis = DEBOUNCE_MILLIS;
}

void PotentioSelector::initialize(byte pin, uint8_t selector_positions_count)
{
    this->analogPin = pin;
    this->analog_range_per_value = (float)1024 / selector_positions_count;
}

uint8_t PotentioSelector::getSelectorValueRaw()
{
    uint8_t tmp = (uint8_t)((float)(analogRead(this->analogPin) + this->analog_range_per_value / 2) / this->analog_range_per_value); //this->analog_range_per_value/2 = get value in middle of the range.
    return tmp;
}

uint8_t PotentioSelector::getSelectorValue()
{
    return this->selectorCurrentStableValue;
}

bool PotentioSelector::getValueChangedEdge()
{
    //will only trigger if a change in STABLE value is detected.
    return this->selectorPreviousStableValue != this->selectorCurrentStableValue;
}

void PotentioSelector::refresh()
{
    //call this to update the value of the analog input. call multiple times to check for stable value.

    this->selectorPreviousStableValue = this->selectorCurrentStableValue;
    uint8_t selector_value_raw = getSelectorValueRaw();

    if (selector_value_raw != this->selectorPreviousValue)
    {
        this->buttonEdgeMillis = millis();
        this->debounceMillis = DEBOUNCE_MILLIS;
    }

    if (millis() - this->buttonEdgeMillis > this->debounceMillis)
    {

        this->selectorCurrentStableValue = selector_value_raw;
        this->debounceMillis = 1000000; // set high, so does not trigger all the time once stable. (for debugging)
    }

    this->selectorPreviousValue = selector_value_raw;
}
