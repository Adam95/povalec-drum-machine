#include <Arduino.h>
#include "valve.h"

#define NUM_VALVES 3

unsigned long master_tempo = 4000;
uint8_t pins[NUM_VALVES] = {8, 9, 10}; // {8, 9, 10, 11};
Valve *valves[NUM_VALVES];

void setup()
{
  unsigned long now = millis();

  // initialize valves
  for (uint8_t i = 0; i < NUM_VALVES; i++)
  {
    valves[i] = new Valve(pins[i]);
    valves[i]->set_tempo(master_tempo);
    valves[i]->set_start_time(now);
  }

  // set initial dividers to valves
  valves[0]->set_divider(2);
  valves[1]->set_divider(4);
  valves[2]->set_divider(8);
  // valves[3]->set_divider(3);
}

void loop()
{
  unsigned long now = millis();

  for (uint8_t i = 0; i < NUM_VALVES; i++)
    valves[i]->tick(now);
}
