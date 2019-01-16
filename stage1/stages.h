#ifndef GAUSS_KANONE_STAGES_H
#define GAUSS_KANONE_STAGES_H

#define EXPIRED(t_now, t_start, T) t_now-t_start >= T

#include "Arduino.h"

#define BUTTON_PUSHED(button_pin) digitalRead(button_pin)==HIGH
#define BUTTON_RELEASED(button_pin) digitalRead(button_pin)==LOW

/* [IMPORTANT]
 * For both photocells:
 * HIGH <=> nothing (trivial case)
 * LOW <=> blocked (projectile passing through)
 */
#define BLOCKED(photo_pin) digitalRead(photo_pin)==LOW
#define UNBLOCKED(photo_pin) digitalRead(photo_pin)==HIGH

typedef unsigned char pin;

/* [State description]
 * INIT: inital state, ready for firing and calclulation
 * FIRING: the MOSFET is turned on, discharging capacitor
 * STOPPED: MOSFET is turned off
 * WAITING: waiting for the projectile reaching the 2.photocell
 * CALCULATING: calculate the velocity
 */
enum State {INIT, FIRING, STOPPED, WAITING, CALCULATING};

void run_stage_1();

#endif
