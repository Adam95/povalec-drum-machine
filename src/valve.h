#ifndef VALVE_H
#define VALVE_H

#define OPEN_TIME_MS 50

#include <Arduino.h>

class Valve
{
public:
  Valve(uint8_t pin);
  void set_tempo(unsigned int tempo);
  void set_divider(uint8_t divider);
  void set_start_time(unsigned long time);
  void tick(unsigned long now);
private:
  void open(unsigned long now);
  void close();

  uint8_t m_pin;
  unsigned long m_last_open;
  
  unsigned long m_start_time;
  unsigned long m_tempo;
  unsigned long m_interval;
  uint8_t m_divider;
  uint8_t m_offset;
  uint8_t m_current_subdiv;

  bool m_open;
};

#endif // VALVE_H