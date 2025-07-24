#include "servo.h"

Servo::Servo(uint8_t channel, Adafruit_PWMServoDriver *driver, bool inverted)
{
  this->driver = driver;
  this->m_channel = channel;
  this->m_last_open = 0;
  this->m_tempo = 0;
  this->m_divider = 16;
  this->m_interval = 0;
  this->m_current_subdiv = 0;
  this->m_open = false;
  this->m_pattern_index = 0;
  this->m_next_pattern_index = 0;
  this->m_pattern_transition = false;
  this->m_inverted = inverted;

  Serial.print("Initialize servo (channel=");
  Serial.print(m_channel);
  Serial.println(")");
  this->close();
}

uint8_t Servo::get_pattern_index()
{
  return this->m_pattern_index;
}

uint8_t Servo::get_next_pattern_index()
{
  return this->m_next_pattern_index;
}

void Servo::set_tempo(unsigned int tempo)
{
  this->m_tempo = tempo;
  this->m_interval = this->m_tempo / this->m_divider;
}

void Servo::set_start_time(unsigned long time)
{
  this->m_start_time = time;
}

void Servo::set_pattern_index(uint8_t pattern_index, bool instant)
{
  if (instant)
  {
    this->m_pattern_index = pattern_index;
    this->m_next_pattern_index = pattern_index;
  }
  else
  {
    this->m_next_pattern_index = pattern_index;
    this->m_pattern_transition = true;
  }
}

void Servo::open(unsigned long now)
{
  driver->setPWM(m_channel, 0, !m_inverted ? PULSE_OPEN : PULSE_OPEN_INV);
  this->m_last_open = now;
  this->m_open = true;
}

void Servo::close()
{
  driver->setPWM(m_channel, 0, PULSE_CLOSED);
  this->m_open = false;
}

bool Servo::should_play()
{
  return (PATTERNS[this->m_pattern_index] & (1 << (15 - this->m_current_subdiv))) != 0;
}

void Servo::tick(unsigned long now)
{
  // if the valve is open and 'open' time passed -> close
  if (this->m_open && (now - this->m_last_open >= SERVO_OPEN_TIME_MS))
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
