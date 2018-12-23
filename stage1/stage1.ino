/* [IMPORTANT] For both photocells:
 * HIGH <=> nothing (trivial case)
 * LOW <=> blocked (projectile passing through)
 */

#include "stages.h"

/* [stage_1]
 * coil: 9
 * photocell_1: 4
 * photocell_1: 5
 * shoot: 7
 */
Stage_1* stage_1 = new Stage_1(new State_INIT(), 9, 4, 5, 7);

void setup() {
  Serial.begin(9600);

  stage_1->set_t_optimal(2000); // t_optimal=2000ms
  stage_1->set_distance_photocells(20); // d=20 mm
  stage_1->ready();
}

void loop() {
  stage_1->run_stage();
}
