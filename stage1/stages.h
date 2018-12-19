#ifndef GAUSS_KANONE_STAGES_H
#define GAUSS_KANONE_STAGES_H
#include "Arduino.h"

typedef unsigned char pin;

enum SuperState {STAGE_1, STAGE_2, STAGE_3, STAGE_4};
enum State {INIT, FIRE, IDLE, STOP, CALCULATE};

// Behavior of the first stage
void run_stage_1();

// Behavior of the second stage
void run_stage_2();

// Behavior of the third stage
void run_stage_3();

// Behavior of the fourth stage
void run_stage_4();


#endif
