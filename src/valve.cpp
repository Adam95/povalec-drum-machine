#include "valve.h"

Valve::Valve(uint8_t pin)
{
  this->m_pin = pin;
  this->m_last_open = 0;
  this->m_open = false;

  pinMode(pin, OUTPUT);
}

void Valve::open()
{
  digitalWrite(this->m_pin, LOW);
  this->m_last_open = millis();
  this->m_open = true;
}

void Valve::close()
{
  digitalWrite(this->m_pin, HIGH);
  this->m_open = false;
}

void Valve::tick()
{
  unsigned long now = millis();

  // if the valve is open and 'open' time passed -> close
  if (this->m_open && (now - this->m_last_open >= OPEN_TIME_MS))
    this->close();
}
