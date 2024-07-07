#ifndef VALVE_H
#define VALVE_H

#define OPEN_TIME_MS 50

#include <Arduino.h>

class Valve
{
public:
  Valve(uint8_t pin);
  void open();
  void tick();

private:
  void close();

  uint8_t m_pin;
  unsigned long m_last_open;
  bool m_open;
};

#endif // VALVE_H