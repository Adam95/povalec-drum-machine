#ifndef SERVO_H
#define SERVO_H

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "constants.h"

#define PULSE_OPEN ((500 + (70 / 180.0) * (2500 - 500)) / 20000) * 4096
#define PULSE_OPEN_INV ((500 + (90 / 180.0) * (2500 - 500)) / 20000) * 4096
#define PULSE_CLOSED ((500 + (80 / 180.0) * (2500 - 500)) / 20000) * 4096

class Servo
{
public:
  Servo(uint8_t channel, Adafruit_PWMServoDriver *driver, bool inverted);
  uint8_t get_pattern_index();
  uint8_t get_next_pattern_index();
  void set_tempo(unsigned int tempo);
  void set_start_time(unsigned long time);
  void set_pattern_index(uint8_t pattern_index, bool instant);
  void tick(unsigned long now);
  void open(unsigned long now);

private:
  void close();
  bool should_play();

  Adafruit_PWMServoDriver *driver;
  uint8_t m_channel;
  bool m_inverted;
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