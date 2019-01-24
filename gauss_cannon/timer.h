#ifndef  TIMER_GAUSS_GUN
#define TIMER_GAUSS_GUN

#include "Arduino.h"

class Timer {
private:
  unsigned long t_entry; // in us
  unsigned long t_leaving; // in us

public:
  Timer(unsigned long t1, unsigned long t2):t_entry(t1),t_leaving(t2){};
  void start();
  void stop();
  void reset();
  unsigned long get_interval();
};


#endif
