#include "stages.h"
#include "timer.h"

#ifndef DEBUG_GAUSS
#define DEBUG_GAUSS
#endif

// #undef DEBUG_GAUSS

extern enum State state;

// imported from .ino
extern const pin coil_1;
extern const pin shoot;
extern const pin photocell_1;
extern const pin photocell_2;
extern const pin charge_state;

static unsigned long t_fire; // the moment when the MOSFET is switched on (ms)
static const double time_on_optimal = 200.00; // optimal t_on calculated by simulation: 5.55ms
static const double d_photocells = 22; // distance between photocells, measured: 22mm
static double velocity_stage_1 = 0; // velocity in m per s
static Timer timer(0,0);

void behavior_init() {
  if(BUTTON_PUSHED(shoot)) {
    state = BUTTONING;
  }
  #ifdef DEBUG_GAUSS
    Serial.print("INIT\n");
  #endif
}


void behavior_buttoning() {
  if (BUTTON_RELEASED(shoot) ) {
    if (CAPACITOR_CHARGED(charge_state)) {
      t_fire = millis();
      state = FIRING;
    }else{
      state = INIT;
    }
  }

  #ifdef DEBUG_GAUSS
    Serial.print("BUTTONING\n");
  #endif
}


void behavior_firing() {
  digitalWrite(coil_1, HIGH); // Turn on MOSFET and discharge the capacitor

  if(EXPIRED((double)millis(), (double)t_fire, time_on_optimal)){
    state = STOPPED;
  }
  if(BLOCKED(photocell_1)) { // unwanted situation: bad simulation
    digitalWrite(coil_1, LOW);
    state = TIMING;
  }
  #ifdef DEBUG_GAUSS
    Serial.print("FIRING\n");
  #endif
}


void behavior_stopped() {
  digitalWrite(coil_1, LOW); // Turn off MOSFET
  if(BLOCKED(photocell_1)) {
    state = TIMING;
  }
  timer.start();

  #ifdef DEBUG_GAUSS
    Serial.print("STOPPED\n");
  #endif
}


void behavior_timing() {
  if(BLOCKED(photocell_2)) {
    timer.stop();
    state = CALCULATING;
  }
  #ifdef DEBUG_GAUSS
    Serial.print("TIMING\n");
  #endif
}


void behavior_calculating() {
  unsigned long delta_t = timer.get_interval();
  velocity_stage_1 = d_photocells/(double)delta_t;
  timer.reset();

  #ifdef DEBUG_GAUSS
  Serial.print("State: CALCULATE | The velocity is ");
  Serial.print(velocity_stage_1, 5); // Print the velocity (m per s)
  Serial.print(" m/s \n");
  #endif

  // Back to INIT when the projectile leaves the second photocell
  while(BLOCKED(photocell_2));
  state = INIT;

  //TODO: OUTPUT THE VELOCITY ONTO THE EXTERNAL MINI-DISPLAY

}

void behavior_error() {
  bool built_in_state = true;
  unsigned long t_prev = millis();
  Serial.print("ERROR \n");
  while (1) {
    if (millis()-t_prev>300) {
      t_prev = millis();
      built_in_state = !built_in_state;
      digitalWrite(13,built_in_state);
    }
  }
}


void run_stage_1() {
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
