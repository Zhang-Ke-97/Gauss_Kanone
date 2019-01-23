#include "timer.h"
#include "stage_1.h"

extern enum State state; //imported form .ino

void Timer::start() {
  t_entry = millis();
}

void Timer::stop() {
  t_leaving = millis();
}

void Timer::reset() {
  t_entry = 0lu;
  t_leaving = 0lu;
}

unsigned long Timer::get_interval() {
  if (t_leaving-t_entry>0) {
    return t_leaving-t_entry;
  }else{
    state = ERROR;
    return 0lu;
  }
}
