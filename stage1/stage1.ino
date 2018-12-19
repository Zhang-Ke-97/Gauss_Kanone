
/* [State description]
 * INIT: inital state, ready for firing and calclulation
 * FIRING: the MOSFET is turned on, discharging capacitor
 * STOPPED: MOSFET is turned off
 * WAITING: waiting for the projectile reaching the 2.photocell
 * CALCULATING: calculate the velocity
 */
typedef unsigned char pin;

enum State {INIT, FIRING, STOPPED, WAITING, CALCULATING};
enum State state = INIT;

const pin coil_1 = 9;
const pin shoot = 7;
const pin photocell_1 = 4;
const pin photocell_2 = 5;

static unsigned long t_fire; // the moment when the MOSFET is switched on (ms)
static unsigned long t_photocell_1 = 0; // the moment when the projectile blocked the 1.photocell (ms)
static unsigned long t_photocell_2 = 0; // the moment when the projectile blocked the 2.photocell (ms)
static const double time_on_optimal = 2000.00; // optimal t_on calculated by simulation (ms)
static const double d_photocells = 5; // distance between photocells (cm)
static double velocity_stage_1 = 0; // cm per ms <=> 10* m per s

void setup() {
  Serial.begin(9600);
  pinMode(shoot, INPUT_PULLUP); // Turn on internal pull-up resitor
  pinMode(photocell_1, INPUT_PULLUP);
  pinMode(photocell_2, INPUT_PULLUP);
  pinMode(coil_1, OUTPUT);
}

void loop() {
  switch(state) {
    case INIT:
      if(digitalRead(shoot)==HIGH) {
        digitalWrite(coil_1, HIGH); // Turn on MOSFET and discharge the capacitor
        t_fire = millis();
        state = FIRING;
      }
      if(digitalRead(photocell_1) == HIGH) {
        t_photocell_1 = millis();
        state = WAITING;
      }
      Serial.print("INIT\n");
      break;

    case FIRING:
      if(millis()-t_fire >= time_on_optimal){
        state = STOPPED;
      }
      if(digitalRead(photocell_1) == HIGH) { // In bad case:
        digitalWrite(coil_1, LOW);
        t_photocell_1 = millis();
        state = WAITING;
      }
      Serial.print("FIRING\n");
      break;

    case STOPPED:
      digitalWrite(coil_1, LOW); // Turn off MOSFET
      if(digitalRead(photocell_1) == HIGH) {
        t_photocell_1 = millis();
        state = WAITING;
      }
      if(digitalRead(shoot)==LOW){ // Set the state to INIT if the button is released
        state = INIT;
      }
      Serial.print("STOPPED\n");
      break;

    case WAITING:
      if(digitalRead(photocell_2) == HIGH) {
        t_photocell_2 = millis();
        state = CALCULATING;
      }
      Serial.print("WAITING\n");
      break;

    case CALCULATING:
      if(t_photocell_1!=0 && t_photocell_2!=0){
        velocity_stage_1 = d_photocells/(double)(t_photocell_2-t_photocell_1);
        t_photocell_1 = 0;
        t_photocell_2 = 0;
      }
      state = INIT;
      Serial.print("CALCULATING: ");
      Serial.println(velocity_stage_1*10, 4); // Print the velocity (m per s)
      //TODO: OUTPUT THE VELOCITY ONTO THE EXTERNAL MINI-DISPLAY
      break;
  }

}
