#include <Arduino.h>
#include "valve.h"

#define NUM_VALVES 1

uint8_t pins[NUM_VALVES] = {10};
Valve* valves[NUM_VALVES];

void setup()
{
  // initialize valves
  for (uint8_t i = 0; i < NUM_VALVES; i++)
    valves[i] = new Valve(pins[i]);
}

void loop()
{
  for (uint8_t i = 0; i < NUM_VALVES; i++)
    valves[i]->tick();
}
