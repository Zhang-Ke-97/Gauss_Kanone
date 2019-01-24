#include "timer.h"
#include "stage_1.h"

extern enum State state; //imported form .ino

void Timer::start() {
  t_entry = micros();
}

void Timer::stop() {
  t_leaving = micros();
}

void Timer::reset() {
  t_entry = 0lu;
  t_leaving = 0lu;
}

unsigned long Timer::get_interval() {
  if (t_leaving-t_entry>0lu) {
    return t_leaving-t_entry;
  }else{
    state = ERROR;
    return 0lu;
  }
}
