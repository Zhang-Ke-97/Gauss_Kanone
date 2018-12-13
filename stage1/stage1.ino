#include "stage1.h"

typedef uint8_t pin;
enum state_stage1 {INIT, FIRE, IDLE, STOP, CALCULATE};
enum state_stage1 state = IDLE;

const pin coil_1 = 9;
const pin shoot = 7;
const pin photocell_1 = 4;
const pin photocell_2 = 5;

unsigned long t_fire; // the moment at which the MOSFET is switched on (ms)
unsigned long t_photocell_1 = 0; // the moment at which the projectile blocked the first photocell (ms)
unsigned long t_photocell_2 = 0; // the moment at which the projectile blocked the second photocell (ms)
const double time_on_optimal = 200.00; // Optimal t_on calculated by simulation (ms)
const double d_photocells = 5; // distance between photocells (cm)
double velocity_stage_1 = 0;

void setup() {
  pinMode(coil_1, OUTPUT);
  pinMode(shoot, INPUT_PULLUP); // Turn on internal pull-up resitor
}

void loop() {
  switch(state) {
    case INIT:
      if(digitalRead(shoot)==HIGH) {
        state = FIRE;
      }
      if(digitalRead(photocell_1) == HIGH) {
        t_photocell_1 = millis();
        state = IDLE;
      }
      break;

    case FIRE:
      digitalWrite(coil_1, HIGH); // Turn on MOSFET and discharge the capacitor
      t_fire = millis();
      if(millis()-t_fire < time_on_optimal){
        state = IDLE;
      }
      break;

    case IDLE:
      if(digitalRead(photocell_2) == HIGH) {
        t_photocell_2 = millis();
        state = CALCULATE;
      }
      if(millis()-t_fire >= time_on_optimal){
        state = STOP;
      }
      if(digitalRead(photocell_1) == HIGH) {
        t_photocell_1 = millis();
        state = STOP;
      }
      // ...Handle more tasks while discharching the capacitor.
      // e.g. To control an external display while discharching,
      //      just extend a new state DISPLAY
      break;

    case STOP:
      digitalWrite(coil_1, LOW); // Turn off MOSFET
      state = INIT;
      break;

    case CALCULATE:
      if(t_photocell_1!=0 && t_photocell_2!=0){
        velocity_stage_1 = d_photocells/(t_photocell_2-t_photocell_1);
        t_photocell_1 = 0;
        t_photocell_2 = 0;
      }
      state = INIT;
      break;
  }

}
