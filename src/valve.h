#ifndef VALVE_H
#define VALVE_H

#include <Arduino.h>
#include "constants.h"

class Valve
{
public:
  Valve(uint8_t pin);
  uint8_t get_pattern_index();
  uint8_t get_next_pattern_index();
  void set_tempo(unsigned int tempo);
  void set_start_time(unsigned long time);
  void set_pattern_index(uint8_t pattern_index, bool instant);
  void tick(unsigned long now);

private:
  void open(unsigned long now);
  void close();
  bool should_play();

  uint8_t m_pin;
  unsigned long m_last_open;

  unsigned long m_start_time;
  unsigned long m_tempo;
  unsigned long m_interval;
  uint8_t m_divider;
  uint8_t m_offset;
  uint8_t m_current_subdiv;
  uint8_t m_pattern_index;
  uint8_t m_next_pattern_index;

  bool m_pattern_transition;
  bool m_open;
};

#endif // VALVE_H