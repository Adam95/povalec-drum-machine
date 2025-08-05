#include <Arduino.h>
#include <Encoder.h>
#include "valve.h"
#include "selector.h"
#include "button.h"
#include "constants.h"
#include "servo.h"

uint8_t master_tempo_index = 0;
unsigned long master_tempo = TEMPOS[0];
unsigned long last_idle = 0;
bool paused = false;

static const uint8_t SERVO_CHANNELS[] = {0, 1, 2, 3};
static const uint8_t SERVO_BUTTON_PINS[] = {34, 32};
static const size_t NUM_SERVOS = sizeof(SERVO_CHANNELS) / sizeof(uint8_t);
static const size_t NUM_SERVO_BUTTONS = sizeof(SERVO_BUTTON_PINS) / sizeof(uint8_t);
Selector selector(28, 24, 22, 30, 26);
Button button(36);
Adafruit_PWMServoDriver servo_driver;
Servo *servos[NUM_SERVOS];
Button *servoButtons[NUM_SERVO_BUTTONS];

void reset_servos()
{
  for (uint8_t i = 0; i < NUM_SERVOS; i++)
    servos[i]->set_pattern_index(0, true);

  long index = random(NUM_SERVOS);
  uint8_t next = (servos[index]->get_next_pattern_index() + 1) % NUM_PATTERNS;

  Serial.print("Random valve=");
  Serial.print(index);
  Serial.print(" Pattern=");
  Serial.println(next);

  servos[index]->set_pattern_index(next, true);
}

void setup()
{
  Serial.begin(9600);

  unsigned long now = millis();
  last_idle = now;

  // initialize servo driver
  servo_driver.begin();
  servo_driver.setPWMFreq(50);

  // initialize servos
  for (uint8_t i = 0; i < NUM_SERVOS; i++)
  {
    servos[i] = new Servo(SERVO_CHANNELS[i], &servo_driver, i % 2 == 0);
    servos[i]->set_tempo(master_tempo);
    servos[i]->set_start_time(now);
  }

  // initialize servo buttons
  for (uint8_t i = 0; i < NUM_SERVO_BUTTONS; i++)
  {
    servoButtons[i] = new Button(SERVO_BUTTON_PINS[i]);
  }

  // initialize random generator
  randomSeed(analogRead(0));

  Serial.println("Setup finished, DRUMS STARTING!");
}

void loop()
{
  unsigned long now = millis();

  // update servos
  for (uint8_t i = 0; i < NUM_SERVOS; i++)
    servos[i]->tick(now);

  // update buttons
  button.tick(now);

  // update servo buttons
  for (uint8_t i = 0; i < NUM_SERVO_BUTTONS; i++)
  {
    servoButtons[i]->tick(now);
  }

  // get selected servo
  int8_t selected = selector.get_selected();

  // check if the button was pressed
  ButtonState button_state = button.get_state();
  if (button_state == ButtonState::LONG_PRESSED)
  {
    Serial.println("Long pressed => resetting servos");
    reset_servos();

    last_idle = now; // reset idle timer
    paused = false;
  }
  else if (button_state == ButtonState::CLICKED && selected >= 0)
  {
    uint8_t next = (servos[selected]->get_next_pattern_index() + 1) % NUM_PATTERNS;

    Serial.print("Valve=");
    Serial.print(selected);
    Serial.print(" Pattern=");
    Serial.println(next);

    servos[selected]->set_pattern_index(next, false);
    last_idle = now; // reset idle timer
    paused = false;
  }
  else if (!paused && button_state == ButtonState::CLICKED && selected == -1) // pressed button and no valve is selected => cycle through master tempo presets
  {
    master_tempo_index = (master_tempo_index + 1) % NUM_TEMPOS;
    master_tempo = TEMPOS[master_tempo_index];

    Serial.print("Setting master tempo to ");
    Serial.println(master_tempo);

    for (uint8_t i = 0; i < NUM_SERVOS; i++)
      servos[i]->set_tempo(master_tempo);

    last_idle = now; // reset idle timer
    paused = false;
  }
  else if (paused && button_state == ButtonState::CLICKED && selected == -1) // pressed button when paused while no valve is selected => set pattern to random valve
  {
    reset_servos();

    last_idle = now; // reset idle timer
    paused = false;
  }

  // check if servo button is pressed => if yes, open the corresponding valve
  for (uint8_t i = 0; i < NUM_SERVO_BUTTONS; i++)
  {
    ButtonState state = servoButtons[i]->get_state();
    if (state == ButtonState::PRESSED) {
      servos[i]->open(now);
    }
  }

  // idle time passed => set all servos to pattern 0
  if (!paused && now - last_idle >= IDLE_TIME_MS)
  {
    Serial.println("Idle time passed, pausing...");
    for (uint8_t i = 0; i < NUM_SERVOS; i++)
      servos[i]->set_pattern_index(0, true);

    paused = true;
  }
}
