#ifndef SELECTOR_H
#define SELECTOR_H

#include <Arduino.h>

class Selector
{
public:
  Selector(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5);
  int8_t get_selected();

private:
  uint8_t m_pins[5];
};

#endif // SELECTOR_H