#include <Arduino.h>
#include <Encoder.h>
#include "valve.h"
#include "selector.h"
#include "button.h"
#include "constants.h"

uint8_t master_tempo_index = 0;
unsigned long master_tempo = TEMPOS[0];
unsigned long last_idle = 0;
bool paused = false;

static const uint8_t VALVE_PINS[] = {8, 9, 10, 11, 12};
static const size_t NUM_VALVES = sizeof(VALVE_PINS) / sizeof(uint8_t);
Valve *valves[NUM_VALVES];
Selector selector(28, 24, 22, 30, 26);
Button button(52);

void reset_valves()
{
  for (uint8_t i = 0; i < NUM_VALVES; i++)
    valves[i]->set_pattern_index(0, true);

  long index = random(NUM_VALVES);
  uint8_t next = (valves[index]->get_next_pattern_index() + 1) % NUM_PATTERNS;

  Serial.print("Random valve=");
  Serial.print(index);
  Serial.print(" Pattern=");
  Serial.println(next);

  valves[index]->set_pattern_index(next, true);
}

void setup()
{
  Serial.begin(9600);

  unsigned long now = millis();
  last_idle = now;

  // initialize valves
  for (uint8_t i = 0; i < NUM_VALVES; i++)
  {
    valves[i] = new Valve(VALVE_PINS[i]);
    valves[i]->set_tempo(master_tempo);
    valves[i]->set_start_time(now);
  }

  // initialize random generator
  randomSeed(analogRead(0));

  Serial.println("Setup finished, DRUMS STARTING!");
}

void loop()
{
  unsigned long now = millis();

  // update valves
  for (uint8_t i = 0; i < NUM_VALVES; i++)
    valves[i]->tick(now);

  // update button
  button.tick(now);

  // get selected valve
  int8_t selected = selector.get_selected();

  // check if the button was pressed
  ButtonState button_state = button.get_state();
  if (button_state == ButtonState::LONG_PRESSED)
  {
    Serial.println("Long pressed => resetting valves");
    reset_valves();

    last_idle = now; // reset idle timer
    paused = false;
  }
  else if (button_state == ButtonState::CLICKED && selected >= 0)
  {
    uint8_t next = (valves[selected]->get_next_pattern_index() + 1) % NUM_PATTERNS;

    Serial.print("Valve=");
    Serial.print(selected);
    Serial.print(" Pattern=");
    Serial.println(next);

    valves[selected]->set_pattern_index(next, false);
    last_idle = now; // reset idle timer
    paused = false;
  }
  else if (!paused && button_state == ButtonState::CLICKED && selected == -1) // pressed button and no valve is selected => cycle through master tempo presets
  {
    master_tempo_index = (master_tempo_index + 1) % NUM_TEMPOS;
    master_tempo = TEMPOS[master_tempo_index];

    Serial.print("Setting master tempo to ");
    Serial.println(master_tempo);

    for (uint8_t i = 0; i < NUM_VALVES; i++)
      valves[i]->set_tempo(master_tempo);

    last_idle = now; // reset idle timer
    paused = false;
  }
  else if (paused && button_state == ButtonState::CLICKED && selected == -1) // pressed button when paused while no valve is selected => set pattern to random valve
  {
    reset_valves();

    last_idle = now; // reset idle timer
    paused = false;
  }

  // idle time passed => set all valves to pattern 0
  if (!paused && now - last_idle >= IDLE_TIME_MS)
  {
    Serial.println("Idle time passed, pausing...");
    for (uint8_t i = 0; i < NUM_VALVES; i++)
      valves[i]->set_pattern_index(0, true);

    paused = true;
  }
}
