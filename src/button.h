#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
public:
  Button(uint8_t pin);
  bool was_released();
  void tick(unsigned long now);
private:
  uint8_t m_pin;
  int m_state;
  bool m_released;
  unsigned long m_lastDebounce;
};

#endif // SELECTOR_H