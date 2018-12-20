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

void setup() {
  Serial.begin(9600);
  pinMode(shoot, INPUT_PULLUP); // Turn on internal pull-up resitor
  pinMode(photocell_1, INPUT_PULLUP);
  pinMode(photocell_2, INPUT_PULLUP);
  pinMode(coil_1, OUTPUT);
}

void loop() {
  run_stage_1();
}
