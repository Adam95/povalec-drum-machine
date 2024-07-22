#include <Arduino.h>
#include <Encoder.h>
#include "valve.h"
#include "selector.h"
#include "button.h"

#define NUM_VALVES 5

unsigned long master_tempo = 6000;
uint8_t pins[NUM_VALVES] = {8, 9, 10, 11, 12};
Valve *valves[NUM_VALVES];
Selector selector(28, 24, 22, 30, 26);
Button button(52);

// encoder pins
int pinCLK = 2;
int pinDT = 3;

long oldPosition = -999;
Encoder myEnc(pinDT, pinCLK);

void setup()
{
  Serial.begin(9600);

  unsigned long now = millis();

  // initialize valves
  for (uint8_t i = 0; i < NUM_VALVES; i++)
  {
    valves[i] = new Valve(pins[i]);
    valves[i]->set_tempo(master_tempo);
    valves[i]->set_start_time(now);
  }

  // set initial patterns to valves
  // valves[0]->set_pattern(PATTERNS[10], true);
  // valves[1]->set_pattern(PATTERNS[4], true);
  // // valves[3]->set_divider(3);
}

void loop()
{
  // long newPosition = myEnc.read();
  // if (newPosition != oldPosition) {
  //   oldPosition = newPosition;
  //   Serial.println(newPosition);
  // }

  unsigned long now = millis();

  // update valves
  for (uint8_t i = 0; i < NUM_VALVES; i++)
    valves[i]->tick(now);

  // update button
  button.tick(now);

  // get selected valve
  int8_t selected = selector.get_selected();

  // check if the button was pressed
  if (button.was_released() && selected >= 0 && selected < NUM_VALVES)
  {
    uint8_t next = (valves[selected]->get_next_pattern_index() + 1) % NUM_PATTERNS;
    Serial.print(selected);
    Serial.print(" ");
    Serial.println(next);

    valves[selected]->set_pattern_index(next, false);
  }
}
