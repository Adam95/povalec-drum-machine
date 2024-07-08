#include "valve.h"

Valve::Valve(uint8_t pin)
{
  this->m_pin = pin;
  this->m_last_open = 0;
  this->m_tempo = 0;
  this->m_interval = 0;
  this->m_divider = 1;
  this->m_current_subdiv = 0;
  this->m_open = false;

  pinMode(pin, OUTPUT);
  this->close();
}

void Valve::set_tempo(unsigned int tempo)
{
  this->m_tempo = tempo;
}

void Valve::set_divider(uint8_t divider)
{
  this->m_divider = divider;
  this->m_interval = this->m_tempo / divider;
}

void Valve::set_start_time(unsigned long time)
{
  this->m_start_time = time;
}

void Valve::open(unsigned long now)
{
  digitalWrite(this->m_pin, LOW);
  this->m_last_open = now;
  this->m_open = true;
}

void Valve::close()
{
  digitalWrite(this->m_pin, HIGH);
  this->m_open = false;
}

/*

1/1 |-----------------------------------|
1/2 |-----------------|-----------------|
1/3 |-----------|-----------|-----------|
1/4 |--------|--------|--------|--------|
1/8 |---||---|---||---|---||---|---||---|

*/

void Valve::tick(unsigned long now)
{
  // if the valve is open and 'open' time passed -> close
  if (this->m_open && (now - this->m_last_open >= OPEN_TIME_MS))
    this->close();

  // if the valve is closed, interval is set and interval passed -> open
  unsigned long bar_time = (now - this->m_start_time) % this->m_tempo;
  unsigned long subdiv = bar_time / this->m_interval;

  if (!this->m_open && this->m_interval != 0 && this->m_current_subdiv != subdiv)
  {
    this->m_current_subdiv = subdiv;
    this->open(now);
  }
}
