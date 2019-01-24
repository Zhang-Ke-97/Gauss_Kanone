#include "stage_1.h"
#include "timer.h"
#include <LiquidCrystal.h>

extern enum State state;

// imported from .ino
extern const pin coil_1;
extern const pin shoot;
extern const pin photocell_1;
extern const pin photocell_2;
extern const pin charge_state;
extern LiquidCrystal lcd;

static unsigned long t_fire; // the moment when the MOSFET is switched on (ms)
static const double time_on_optimal = 5.55; // optimal t_on calculated by simulation: 5.55ms
static const double d_photocells = 22; // distance between photocells, measured: 22mm
static double velocity_stage_1 = 0; // velocity in m per s
static Timer timer(0,0);

void behavior_init() {
  if(BUTTON_PUSHED(shoot)) {
    state = BUTTONING;

    #ifdef DEBUG_GAUSS_SERIAL
    Serial.print("BUTTONING\n");
    #endif

    #ifdef DEBUG_GAUSS_LCD
    lcd.clear();
    lcd.print("BUTTONING\n");
    #endif
  }

  if(BLOCKED(photocell_1)) {
    timer.start(); //Entry activity of TIMING
    state = TIMING;

    #ifdef DEBUG_GAUSS_SERIAL
    Serial.print("TIMING\n");
    #endif

    #ifdef DEBUG_GAUSS_LCD
    lcd.clear();
    lcd.print("TIMING\n");
    #endif
  }
}


void behavior_buttoning() {
  if (BUTTON_RELEASED(shoot) ) {
    if (CAPACITOR_CHARGED(charge_state)) {
      t_fire = millis();
      digitalWrite(coil_1, HIGH); // Entry activity of FIRING
      state = FIRING;

      #ifdef DEBUG_GAUSS_SERIAL
        Serial.print("FIRING\n");
      #endif

      #ifdef DEBUG_GAUSS_LCD
      lcd.clear();
      lcd.print("FIRING\n");
      #endif

    }else{
      state = INIT; // Back to INIT if the capacitor is not fully charged

      #ifdef DEBUG_GAUSS_SERIAL
      Serial.print("INIT\n");
      #endif

      #ifdef DEBUG_GAUSS_LCD
      lcd.clear();
      lcd.print("INIT\n");
      #endif
    }
  }
}


void behavior_firing() {
  if(EXPIRED((double)millis(), (double)t_fire, time_on_optimal)){
    digitalWrite(coil_1, LOW); //Exit activity of FIRING
    state = INIT;

    #ifdef DEBUG_GAUSS_SERIAL
      Serial.print("INIT\n");
    #endif

    #ifdef DEBUG_GAUSS_LCD
    lcd.clear();
    lcd.print("INIT\n");
    #endif
  }

  if(BLOCKED(photocell_1)) { // unwanted situation: bad simulation
    digitalWrite(coil_1, LOW);
    state = TIMING;

    #ifdef DEBUG_GAUSS_SERIAL
      Serial.print("TIMING | bad simulation\n");
    #endif

    #ifdef DEBUG_GAUSS_LCD
    lcd.clear();
    lcd.print("TIMING | bad simulation");
    #endif
  }

}


void behavior_timing() {
  if(BLOCKED(photocell_2)) {
    timer.stop();
    state = CALCULATING;

    #ifdef DEBUG_GAUSS_SERIAL
      Serial.print("CALCULATING\n");
    #endif

    #ifdef DEBUG_GAUSS_LCD
    lcd.clear();
    lcd.print("CALCULATING\n");
    #endif
  }
}


void behavior_calculating() {
  unsigned long delta_t = timer.get_interval();
  velocity_stage_1 = d_photocells/(double)delta_t;

  #ifdef DEBUG_GAUSS_SERIAL
  Serial.print("The velocity is ");
  Serial.print(velocity_stage_1, 3); // Print the velocity (m per s)
  Serial.print(" m/s \n");
  #endif

  lcd.clear();
  lcd.print("The velocity is:");
  lcd.setCursor(0, 1);
  lcd.print(velocity_stage_1, 3);
  lcd.print(" m/s");

  // Back to INIT when the projectile leaves the second photocell
  while(BLOCKED(photocell_2));
  timer.reset(); //Exit activity of CALCULATING
  state = INIT;
  lcd.print("...Next Run");
}

void behavior_error() {
  bool built_in_state = true;
  unsigned long t_prev = millis();

  #ifdef DEBUG_GAUSS_SERIAL
  Serial.print("ERROR x_x\n");
  #endif

  #ifdef DEBUG_GAUSS_LCD
  lcd.clear();
  lcd.print("ERROR x_x\n");
  #endif

  while (1) {
    if (millis()-t_prev>300) {
      t_prev = millis();
      built_in_state = !built_in_state;
      digitalWrite(LED_BUILTIN, built_in_state);
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
