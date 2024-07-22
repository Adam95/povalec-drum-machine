#include "selector.h"

Selector::Selector(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5)
{
  this->m_pins[0] = pin1;
  this->m_pins[1] = pin2;
  this->m_pins[2] = pin3;
  this->m_pins[3] = pin4;
  this->m_pins[4] = pin5;

  for (uint8_t i = 0; i < 5; i++)
  {
    pinMode(this->m_pins[i], INPUT_PULLUP);
  }
}

int8_t Selector::get_selected()
{
  for (uint8_t i = 0; i < 5; i++)
  {
    if (digitalRead(this->m_pins[i]) == LOW)
      return i;
  }

  return -1;
}
