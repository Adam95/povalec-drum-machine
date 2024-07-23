#include "valve.h"

Valve::Valve(uint8_t pin)
{
  this->m_pin = pin;
  this->m_last_open = 0;
  this->m_tempo = 0;
  this->m_divider = 16;
  this->m_interval = 0;
  this->m_current_subdiv = 0;
  this->m_open = false;
  this->m_pattern_index = 0;
  this->m_next_pattern_index = 0;
  this->m_pattern_transition = false;

  pinMode(pin, OUTPUT);
  this->close();
}

uint8_t Valve::get_pattern_index()
{
  return this->m_pattern_index;
}

uint8_t Valve::get_next_pattern_index()
{
  return this->m_next_pattern_index;
}

void Valve::set_tempo(unsigned int tempo)
{
  this->m_tempo = tempo;
  this->m_interval = this->m_tempo / this->m_divider;
}

void Valve::set_start_time(unsigned long time)
{
  this->m_start_time = time;
}

void Valve::set_pattern_index(uint8_t pattern_index, bool instant)
{
  if (instant)
  {
    this->m_pattern_index = pattern_index;
    this->m_next_pattern_index = pattern_index;
  } else
  {
    this->m_next_pattern_index = pattern_index;
    this->m_pattern_transition = true;
  }
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

bool Valve::should_play()
{
  return (PATTERNS[this->m_pattern_index] & (1 << (15 - this->m_current_subdiv))) != 0;
}

void Valve::tick(unsigned long now)
{
  // if the valve is open and 'open' time passed -> close
  if (this->m_open && (now - this->m_last_open >= VALVE_OPEN_TIME_MS))
    this->close();

  // if the valve is closed, interval is set and interval passed -> open
  unsigned long bar_time = (now - this->m_start_time) % this->m_tempo;
  unsigned long subdiv = bar_time / this->m_interval;

  if (this->m_current_subdiv != subdiv)
  {
    this->m_current_subdiv = subdiv;
    // Serial.print(this->m_current_subdiv);
    // Serial.println(this->should_play() ? " x" : " -");

    // start of the bar and pattern should change
    if (this->m_current_subdiv == 0 && this->m_pattern_transition)
    {
      this->m_pattern_index = this->m_next_pattern_index;
      this->m_pattern_transition = false;
    }

    if (this->should_play())
      this->open(now);
  }
}
