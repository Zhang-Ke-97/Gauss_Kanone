#include "stages.h"

extern enum State state_1;

extern const pin coil_1;
extern const pin shoot;
extern const pin photocell_1;
extern const pin photocell_2;

static unsigned long t_fire; // the moment when the MOSFET is switched on (ms)
static unsigned long t_photocell_1 = 0; // the moment when the projectile blocked the 1.photocell (ms)
static unsigned long t_photocell_2 = 0; // the moment when the projectile blocked the 2.photocell (ms)
static const double time_on_optimal = 2000.00; // optimal t_on calculated by simulation (ms)
static const double d_photocells = 5; // distance between photocells (cm)
static double velocity_stage_1 = 0; // cm per ms <=> 10* m per s

void run_stage_1() {
  switch(state_1) {
    case INIT:
      if(digitalRead(shoot)==HIGH) {
        state_1 = FIRE;
      }
      if(digitalRead(photocell_1) == LOW) {
        t_photocell_1 = millis();
        state_1 = IDLE;
      }
      Serial.print("INIT\n");
      break;

    case FIRE:
      digitalWrite(coil_1, HIGH); // Turn on MOSFET and discharge the capacitor
      t_fire = millis();
      if(millis()-t_fire < time_on_optimal){
        state_1 = IDLE;
      }
      Serial.print("FIRE\n");
      break;

    case IDLE:
      if(digitalRead(photocell_2) == LOW) {
        t_photocell_2 = millis();
        state_1 = CALCULATE;
      }
      if(millis()-t_fire >= time_on_optimal){ //Stop firing if time expires
        state_1 = STOP;
      }
      if(digitalRead(photocell_1) == LOW) {
        t_photocell_1 = millis();
        state_1 = STOP;
      }
      // ...Handle more tasks while discharching the capacitor.
      // e.g. To control an external display while discharching,
      //      just extend a new state DISPLAY
      Serial.print("IDLE\n");
      break;

    case STOP:
      digitalWrite(coil_1, LOW); // Turn off MOSFET
      if(digitalRead(shoot)==LOW){ // Set the state to INIT if the button is released
        state_1 = INIT;
      }

      Serial.print("STOP\n");
      break;

    case CALCULATE:
      if(t_photocell_1!=0 && t_photocell_2!=0){
        velocity_stage_1 = d_photocells/(double)(t_photocell_2-t_photocell_1);
        t_photocell_1 = 0;
        t_photocell_2 = 0;
      }
      Serial.print("State: CALCULATE | The velocity is ");
      Serial.print(velocity_stage_1*10, 4); // Print the velocity (m per s)
      Serial.print(" m/s");
      // state_1 = INIT;
      break;
  }

}

void run_stage_2() {
  //TODO: IMPLEMENTATION OF THE BEHAVIOR OF THE SECOND SATGE
}

void run_stage_3() {
  //TODO: IMPLEMENTATION OF THE BEHAVIOR OF THE THIRD SATGE
}

void run_stage_4() {
  //TODO: IMPLEMENTATION OF THE BEHAVIOR OF THE FOURTH SATGE
}
