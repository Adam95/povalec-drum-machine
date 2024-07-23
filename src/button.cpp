#include "button.h"

Button::Button(uint8_t pin)
{
  pinMode(pin, INPUT_PULLUP);

  this->m_pin = pin;
  this->m_pinState = digitalRead(pin);
  this->m_released = false;
  this->m_long_pressed = false;
}

ButtonState Button::get_state()
{
  // return true if the button was released, but only on the first call
  if (this->m_released)
  {
    this->m_released = false;
    return ButtonState::CLICKED;
  } else if (this->m_long_pressed)
  {
    this->m_long_pressed = false;
    return ButtonState::LONG_PRESSED;
  }

  return ButtonState::NOP;
}

void Button::tick(unsigned long now)
{
  int read = digitalRead(this->m_pin);

  // rising edge (button released) => mark time for debounce
  if (this->m_pinState == LOW && read == HIGH)
  {
    this->m_lastDebounce = now;
    this->m_lastPressed = 0;
  } else if (this->m_pinState == HIGH && read == LOW) // falling edge (button pressed or signal bounce) => reset debounce time
  {
    this->m_lastDebounce = 0;
    this->m_lastPressed = now;
  }

  // debounce delay passed => mark as released
  if (this->m_lastDebounce != 0 && !this->m_released && now - this->m_lastDebounce > 50)
  {
    this->m_lastDebounce = 0;
    this->m_released = true;
  } else if (this->m_lastPressed != 0 && !this->m_long_pressed && now - this->m_lastPressed > 3000)
  {
    this->m_lastPressed = 0;
    this->m_long_pressed = true;
  }

  this->m_pinState = read;
}
