#ifndef VALVE_H
#define VALVE_H

#define OPEN_TIME_MS 50

#include <Arduino.h>

static const uint8_t NUM_PATTERNS = 34;
static const uint16_t PATTERNS[NUM_PATTERNS] =
{
  0b0000000000000000,
  0b1000100010001000,
  0b0010001000100010,
  0b1000000010000000,
  0b0000100000001000,
  0b1111111111111111,
  0b1010101010101010,
  0b0101010101010101,
  0b1000000010100000,
  0b1010000010100000,
  0b1000000000100000,
  0b1000001010000010,
  0b1101110111011101,
  0b0000100000001010,
  0b1111000000000000,
  0b0000111100000000,
  0b0000000011110000,
  0b0000000000001111,
  0b1000000000000000,
  0b0000100000000000,
  0b0000000010000000,
  0b0000000000001000,
  0b0100000000000000,
  0b0000010000000000,
  0b0000000001000000,
  0b0000000000000100,
  0b0010000000000000,
  0b0000001000000000,
  0b0000000000100000,
  0b0000000000000010,
  0b0001000000000000,
  0b0000000100000000,
  0b0000000000010000,
  0b0000000000000001
};

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