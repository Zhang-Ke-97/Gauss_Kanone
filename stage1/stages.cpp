#include "stages.h"

static unsigned long t_fire; // the moment when the MOSFET is switched on (ms)
static unsigned long t_photocell_1 = 0; // the moment when the projectile blocked the 1.photocell (ms)
static unsigned long t_photocell_2 = 0; // the moment when the projectile blocked the 2.photocell (ms)


Stage::Stage(State* state, pin coil, pin photocell_1, pin photocell_2, double distance_photocells):
current_state(state), coil(coil), photocell_1(photocell_1), photocell_2(photocell_2),
distance_photocells(photocell_2){/*Constructor*/}

Stage::Stage(State* state, pin coil, pin photocell_1, pin photocell_2):
current_state(state), coil(coil), photocell_1(photocell_1), photocell_2(photocell_2){/*Constructor*/}

State* Stage::get_current_state() {
  return current_state;
}
pin Stage::get_photocell_1() {
return photocell_1;
}
pin Stage::get_photocell_2() {
return photocell_2;
}
pin Stage::get_coil() {
return coil;
}
double Stage::get_velocity() {
  return this->velocity;
}
double Stage::get_distance_photocells() {
  return this->distance_photocells;
}
double Stage::get_t_optimal() {
  return this->t_optimal;
}

void Stage::set_current_state(State* state) {
  this->current_state = state;
}
void Stage::set_photocell_1(pin photocell_1) {
  this->photocell_1 = photocell_1;
}
void Stage::set_photocell_2(pin photocell_2) {
  this->photocell_2 = photocell_2;
}
void Stage::set_coil(pin coil) {
  this->coil = coil;
}
void Stage::set_velocity(double velocity){
  this->velocity = velocity;
}
void Stage::set_distance_photocells(double distance_photocells){
  this->distance_photocells = distance_photocells;
}
void Stage::set_t_optimal(double t_optimal){
  this->t_optimal = t_optimal;
}

void Stage::show_current_state() {
  this->current_state->show();
}

void Stage::fire() {
  digitalWrite(coil, HIGH);
}
void Stage::stop() {
  digitalWrite(coil, LOW);
}

bool Stage::photocell_1_blocked(){
  return digitalRead(photocell_1)==HIGH;
}
bool Stage::photocell_2_blocked(){
  return digitalRead(photocell_2)==HIGH;
}

void Stage::calculate_velocity() {
  if (t_photocell_1!=0 || t_photocell_2!=0) {
    velocity=distance_photocells/(double)(t_photocell_2-t_photocell_1);
  }
}
void Stage_1::run_stage() {
  this->get_current_state()->run(this);
}




Stage_1::Stage_1(State* state, pin coil, pin photocell_1, pin photocell_2, double distance_photocells, pin shoot):
Stage(state, coil, photocell_1, photocell_2, distance_photocells), shoot(shoot){/*Constructor*/}

Stage_1::Stage_1(State* state, pin coil, pin photocell_1, pin photocell_2, pin shoot):
Stage(state, coil, photocell_1, photocell_2), shoot(shoot){/*Constructor*/}

bool Stage_1::button_pressed() {
  return digitalRead(shoot)==HIGH;
}
bool Stage_1::button_released() {
  return digitalRead(shoot)==LOW;
}

 void Stage_1::ready() {
  pinMode(shoot, INPUT_PULLUP);
  pinMode(this->get_photocell_1(), INPUT_PULLUP);
  pinMode(this->get_photocell_2(), INPUT_PULLUP);
  pinMode(this->get_coil(), OUTPUT);
}

 void State_INIT::run(Stage_1* stage_1) {
  if(stage_1->button_pressed()) {
    stage_1->fire(); // Turn on MOSFET and discharge the capacitor
    t_fire = millis();
    stage_1->set_current_state(new State_FIRING());
  }
  if(stage_1->photocell_1_blocked()) {
    t_photocell_1 = millis();
    stage_1->set_current_state(new State_WAITING());
  }
}

 void State_INIT::show(){
  Serial.print("INIT\n");
}


 void State_FIRING::run(Stage_1* stage_1) {
  if(EXPIRED((double)millis(), (double)t_fire, stage_1->get_t_optimal())){
    stage_1->stop(); // Turn off MOSFET
    stage_1->set_current_state(new State_STOPPED());
  }
  if(stage_1->photocell_1_blocked()) {
    stage_1->stop(); // Turn off MOSFET
    t_photocell_1 = millis();
    stage_1->set_current_state(new State_WAITING());
  }
}
 void State_FIRING::show() {
  Serial.print("FIRING\n");
}


 void State_STOPPED::run(Stage_1* stage_1){
  if(stage_1->photocell_1_blocked()) {
    t_photocell_1 = millis();
    stage_1->set_current_state(new State_WAITING());
  }
  if(stage_1->button_released()){ // Set the state_1 to INIT if the button is released
    stage_1->set_current_state(new State_INIT());
  }
}

 void State_STOPPED::show(){
  Serial.print("STOPPED\n");
}


 void State_WAITING::run(Stage_1* stage_1) {
  if(stage_1->photocell_2_blocked()) {
    t_photocell_2 = millis();
    stage_1->set_current_state(new State_CALCULATING);
  }
}
 void State_WAITING::show() {
  Serial.print("WAITING\n");
}

 void State_CALCULATING::run(Stage_1* stage_1) {
  if(t_photocell_1!=0 && t_photocell_2!=0){
    stage_1->calculate_velocity();
    t_photocell_1 = 0;
    t_photocell_2 = 0;
  }
  Serial.print("State: CALCULATE | The velocity is ");
  Serial.print(stage_1->get_velocity()*10, 4); // Print the velocity (m per s)
  Serial.print(" m/s \n");
  //TODO: OUTPUT THE VELOCITY ONTO THE EXTERNAL MINI-DISPLAY

  if(!stage_1->photocell_2_blocked()) {
    // Back to INIT when the projectile leaves the second photocell
    stage_1->set_current_state(new State_INIT());
  }
}

 void State_CALCULATING::show() {
  Serial.print("CALCULATING\n");
}
