/* [IMPORTANT]
 * For both photocells:
 * LOW <=> nothing (trivial case)
 * HIGH <=> blocked (projectile passing through)
 */

#include "stages.h"

enum State state = INIT;

extern const pin coil_1 = 9;
extern const pin shoot = 7;
extern const pin photocell_1 = 4;
extern const pin photocell_2 = 5;
extern const pin charge_state = 2;

void setup() {
  Serial.begin(9600);
  pinMode(charge_state, INPUT);
  pinMode(shoot, INPUT_PULLUP); // Turn on internal pull-up resitor
  pinMode(photocell_1, INPUT_PULLUP);
  pinMode(photocell_2, INPUT_PULLUP);
  pinMode(coil_1, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  switch(state) {
    case INIT:
      behavior_init();
      break;

    case BUTTONING:
      behavior_buttoning();
      break;

    case FIRING:
      behavior_firing();
      break;

    case STOPPED:
      behavior_stopped();
      break;

    case TIMING:
      behavior_timing();
      break;

    case CALCULATING:
      behavior_calculating();
      break;

    case ERROR:
      behavior_error();
      break;
  }
}
