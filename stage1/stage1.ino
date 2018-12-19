typedef uint8_t pin;
enum State {INIT, FIRE, IDLE, STOP, CALCULATE};
enum State state = INIT;

const pin coil_1 = 9;
const pin shoot = 7;
const pin photocell_1 = 4;
const pin photocell_2 = 5;

unsigned long t_fire; // the moment at which the MOSFET is switched on (ms)
unsigned long t_photocell_1 = 0; // the moment at which the projectile blocked the first photocell (ms)
unsigned long t_photocell_2 = 0; // the moment at which the projectile blocked the second photocell (ms)
const double time_on_optimal = 2000.00; // Optimal t_on calculated by simulation (ms)
const double d_photocells = 5; // distance between photocells (cm)
double velocity_stage_1 = 0; // cm per ms <=> 10* m per s

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
        state = FIRE;
      }
      if(digitalRead(photocell_1) == HIGH) {
        t_photocell_1 = millis();
        state = IDLE;
      }
      Serial.print("INIT\n");
      break;

    case FIRE:
      digitalWrite(coil_1, HIGH); // Turn on MOSFET and discharge the capacitor
      t_fire = millis();
      if(millis()-t_fire < time_on_optimal){
        state = IDLE;
      }
      Serial.print("FIRE\n");
      break;

    case IDLE:
      if(digitalRead(photocell_2) == HIGH) {
        t_photocell_2 = millis();
        state = CALCULATE;
      }
      if(millis()-t_fire >= time_on_optimal){
        state = STOP;
      }
      if(digitalRead(photocell_1) == HIGH) {
        t_photocell_1 = millis();
        state = STOP;
      }
      // ...Handle more tasks while discharching the capacitor.
      // e.g. To control an external display while discharching,
      //      just extend a new state DISPLAY
      Serial.print("IDLE\n");
      break;

    case STOP:
      digitalWrite(coil_1, LOW); // Turn off MOSFET
      if(digitalRead(shoot)==LOW){ // Set the state to INIT if the button is released
        state = INIT;
      }
      Serial.print("STOP\n");
      break;

    case CALCULATE:
      if(t_photocell_1!=0 && t_photocell_2!=0){
        velocity_stage_1 = d_photocells/(double)(t_photocell_2-t_photocell_1);
        t_photocell_1 = 0;
        t_photocell_2 = 0;
      }
      state = INIT;
      Serial.print("CALCULATE: ");
      Serial.println(velocity_stage_1*10, 4); // Print the velocity (m per s)
      break;
  }

}
