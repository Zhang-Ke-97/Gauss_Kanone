/* [IMPORTANT] For both photocells:
 * HIGH <=> nothing (trivial case)
 * LOW <=> blocked (projectile passing through)
 */

#include "stages.h"

enum SuperState super_state = STAGE_1;
enum State state_1 = INIT;

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
  switch(super_state) {
    case STAGE_1:
      run_stage_1();
      if(false /*TODO: ADD CONDITION*/) {
        super_state = STAGE_2;
      }
      break;

    case STAGE_2:
      run_stage_2();
      if(false /*TODO: ADD CONDITION*/){
        super_state = STAGE_3;
      }
      break;

    case STAGE_3:
      run_stage_3();
      if(false /*TODO: ADD CONDITION*/){
        super_state = STAGE_4;
      }
      break;

    case STAGE_4:
      run_stage_4();
      if(false /*TODO: ADD CONDITION*/){
        super_state = STAGE_1;
      }
      break;
  }

}
