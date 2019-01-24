/* [IMPORTANT]
 * For both photocells:
 * LOW <=> nothing (trivial case)
 * HIGH <=> blocked (projectile passing through)
 */
#include "stage_1.h"
#include <LiquidCrystal.h>

enum State state = INIT;

extern LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
extern const pin coil_1 = 9;
extern const pin shoot = 8;
extern const pin photocell_1 = 6;
extern const pin photocell_2 = 7;
extern const pin charge_state = 10;

// 6 7 Diode

void setup() {

  #ifdef DEBUG_GAUSS_SERIAL
  Serial.begin(9600);
  #endif

  lcd.begin(16,2);

  pinMode(charge_state, INPUT);
  pinMode(shoot, INPUT);
  pinMode(photocell_1, INPUT);
  pinMode(photocell_2, INPUT);
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
