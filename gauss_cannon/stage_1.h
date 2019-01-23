#ifndef GAUSS_KANONE_STAGES_H
#define GAUSS_KANONE_STAGES_H

// #ifndef DEBUG_GAUSS
// #define DEBUG_GAUSS
// #endif

#include "Arduino.h"

typedef unsigned char pin;

/* [IMPORTANT]
 * For both photocells:
 * HIGH <=> nothing (trivial case)
 * LOW <=> blocked (projectile passing through)
 */
#define BLOCKED(photo_pin) digitalRead(photo_pin)==LOW
#define UNBLOCKED(photo_pin) digitalRead(photo_pin)==HIGH

#define BUTTON_PUSHED(button_pin) digitalRead(button_pin)==HIGH
#define BUTTON_RELEASED(button_pin) digitalRead(button_pin)==LOW

#define CAPACITOR_CHARGED(charge_state) digitalRead(charge_state)==HIGH
#define CAPACITOR_UNCHARGED(charge_state) digitalRead(charge_state)==LOW

#define EXPIRED(t_now, t_start, T) t_now-t_start >= T


/* [State description]
 * INIT: inital state, ready for firing and calclulation
 * BUTTONING: button is being pressed
 * FIRING: button were relea and turn on the MOSFET to discharge capacitor
 * STOPPED: MOSFET is turned off
 * TIMING: TIMING for the projectile reaching the 2.photocell
 * CALCULATING: calculate the velocity
 * ERROR: something unexpected occurs
 */
enum State {INIT, BUTTONING, FIRING, STOPPED, TIMING, CALCULATING, ERROR};

void behavior_init();
void behavior_buttoning();
void behavior_firing();
void behavior_stopped();
void behavior_timing();
void behavior_calculating();
void behavior_error();

void run_stage_1();


#endif
