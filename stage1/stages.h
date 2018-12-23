/* [Class description]
 * State_INIT: inital state, ready for firing and calclulation
 * State_FIRING: the MOSFET is turned on, discharging capacitor
 * State_STOPPED: MOSFET is turned off
 * State_WAITING: waiting for the projectile reaching the 2.photocell
 * State_CALCULATING: calculate the velocity
 */

#ifndef GAUSS_KANONE_STAGES_H
#define GAUSS_KANONE_STAGES_H

#define EXPIRED(t_now, t_start, T) t_now-t_start >= T

#include "Arduino.h"


typedef unsigned char pin;

class Stage;
class Stage_1;

// Abstract class State, implemented by State_INIT, State_FIRING, etc
class State{
private:

public:
  virtual void run(Stage_1* stage_1) =0;
  virtual void show() =0;
};

class State_INIT: public State{
private:

public:
  virtual void run(Stage_1* stage_1);
  virtual void show();
};

class State_FIRING: public State{
private:

public:
  virtual void run(Stage_1* stage_1);
  virtual void show();
};

class State_STOPPED: public State{
private:

public:
  virtual void run(Stage_1* stage_1);
  virtual void show();
};

class State_WAITING: public State{
private:

public:
  virtual void run(Stage_1* stage_1);
  virtual void show();
};

class State_CALCULATING: public State{
private:

public:
  virtual void run(Stage_1* stage_1);
  virtual void show();
};



// Abstract class Stage, implemented by State_1, State_2, etc
class Stage{
private:
  State* current_state; //MUST have
  pin coil; // MUST have
  pin photocell_1; // MUST have
  pin photocell_2; // MUST have
  double velocity; // in meter per second
  double distance_photocells; // in millimeter
  double t_optimal; // in millisecond

public:
  Stage(State* state, pin coil, pin photocell_1, pin photocell_2);
  Stage(State* state, pin coil, pin photocell_1, pin photocell_2, double distance_photocells);

  virtual void ready() =0;
  virtual void run_stage() =0;

  State* get_current_state();
  pin get_photocell_1();
  pin get_photocell_2();
  pin get_coil();
  double get_velocity();
  double get_distance_photocells();
  double get_t_optimal();

  void set_photocell_1(pin photocell_1);
  void set_photocell_2(pin photocell_2);
  void set_coil(pin coil);
  void set_current_state(State* state);
  void set_velocity(double velocity);
  void set_distance_photocells(double distance_photocells);
  void set_t_optimal(double t_optimal);

  void show_current_state();
  void fire();
  void stop();
  bool photocell_1_blocked();
  bool photocell_2_blocked();
  void calculate_velocity();

};


class Stage_1: public Stage{
private:
  pin shoot; // MUST have

public:
  Stage_1(State* state, pin coil, pin photocell_1, pin photocell_2, pin shoot);
  Stage_1(State* state, pin coil, pin photocell_1, pin photocell_2, double distance_photocells, pin shoot);

  bool button_pressed();
  bool button_released();

  virtual void run_stage();
  virtual void ready();
};


#endif
