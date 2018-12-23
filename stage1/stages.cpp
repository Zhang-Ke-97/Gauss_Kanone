#include "stages.h"

#ifndef DEBUG_GAUSS
#define DEBUG_GAUSS
#endif

extern enum State state_1;

extern const pin coil_1;
extern const pin shoot;
extern const pin photocell_1;
extern const pin photocell_2;

static unsigned long t_fire; // the moment when the MOSFET is switched on (ms)
static unsigned long t_photocell_1 = 0; // the moment when the projectile blocked the 1.photocell (ms)
static unsigned long t_photocell_2 = 0; // the moment when the projectile blocked the 2.photocell (ms)
static const double time_on_optimal = 2000.00; // optimal t_on calculated by simulation (ms)
static const double d_photocells = 20; // distance between photocells (mm)
static double velocity_stage_1 = 0; // (m per s)

void run_stage_1() {
  switch(state_1) {
    case INIT:
      if(BUTTON_PUSHED(shoot)) {
        digitalWrite(coil_1, HIGH); // Turn on MOSFET and discharge the capacitor
        t_fire = millis();
        state_1 = FIRING;
      }
      if(BLOCKED(photocell_1)) {
        t_photocell_1 = millis();
        state_1 = WAITING;
      }
      #ifdef DEBUG_GAUSS
        Serial.print("INIT\n");
      #endif
      break;

    case FIRING:
      if(EXPIRED((double)millis(), (double)t_fire, time_on_optimal)){
        state_1 = STOPPED;
      }
      if(BLOCKED(photocell_1)) { //
        digitalWrite(coil_1, LOW);
        t_photocell_1 = millis();
        state_1 = WAITING;
      }
      #ifdef DEBUG_GAUSS
        Serial.print("FIRING\n");
      #endif
      break;

    case STOPPED:
      digitalWrite(coil_1, LOW); // Turn off MOSFET
      if(BLOCKED(photocell_1)) {
        t_photocell_1 = millis();
        state_1 = WAITING;
      }
      if(BUTTON_RELEASED(shoot)){ // Set the state_1 to INIT if the button is released
        state_1 = INIT;
      }
      #ifdef DEBUG_GAUSS
        Serial.print("STOPPED\n");
      #endif
      break;

    case WAITING:
      if(BLOCKED(photocell_2)) {
        t_photocell_2 = millis();
        state_1 = CALCULATING;
      }
      #ifdef DEBUG_GAUSS
        Serial.print("WAITING\n");
      #endif
      break;

    case CALCULATING:
      if(t_photocell_1!=0 && t_photocell_2!=0){
        velocity_stage_1 = d_photocells/(double)(t_photocell_2-t_photocell_1);
        t_photocell_1 = 0;
        t_photocell_2 = 0;
      }
      Serial.print("State: CALCULATE | The velocity is ");
      Serial.print(velocity_stage_1, 4); // Print the velocity (m per s)
      Serial.print(" m/s \n");
      //TODO: OUTPUT THE VELOCITY ONTO THE EXTERNAL MINI-DISPLAY

      if (UNBLOCKED(photocell_2)) {
        // Back to INIT when the projectile leaves the second photocell
        state_1 = INIT;
      }
      break;
  }

}
