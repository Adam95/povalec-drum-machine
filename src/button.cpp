#include "button.h"

Button::Button(uint8_t pin)
{
  pinMode(pin, INPUT_PULLUP);

  this->m_pin = pin;
  this->m_state = digitalRead(pin);
  this->m_released = false;
}

bool Button::was_released()
{
  // return true if the button was released, but only on the first call
  if (this->m_released)
  {
    this->m_released = false;
    return true;
  }

  return false;
}

void Button::tick(unsigned long now)
{
  int read = digitalRead(this->m_pin);

  // rising edge (button released) => mark time for debounce
  if (this->m_state == LOW && read == HIGH)
    this->m_lastDebounce = now;
  else if (this->m_state == HIGH && read == LOW) // falling edge (button pressed or signal bounce) => reset debounce time
    this->m_lastDebounce = 0;

  // debounce delay passed => mark as released
  if (this->m_lastDebounce != 0 && !this->m_released && now - this->m_lastDebounce > 50)
  {
    this->m_lastDebounce = 0;
    this->m_released = true;
  }

  this->m_state = read;
}
