const int spule_1 = 9; // pin to fire the first stage
const int shoot = 7; // shoot button
const int photocell_1 = 4;
const int photocell_2 = 5;

boolean flag = true;
const double distance_photocell = 2; // 2 centimeter

// generate a single impulse
void pulse(double t);

// calculate the velocity of the projectile passing through the photocell
double calculate_velocity();

void setup() {
  pinMode(spule_1, OUTPUT);
  pinMode(shoot, INPUT);
  digitalWrite(shoot, HIGH); // Turn on internal pull-up resitor

}

void loop() {
  if(digitalRead(shoot)==HIGH && flag ==true){ // first stage
      pulse(250); //50.72 milliseconds according to simulation
      calculate_velocity();
      flag=false;
  }

  //...

}



void pulse(double t) {
  int t0 = millis();
  digitalWrite(spule_1, HIGH);
  while(true){
    if(millis()-t0 > t){
      digitalWrite(spule_1, LOW);
      break;
    }
  }
}

double calculate_velocity() {
  int t0 = 0;
  if(digitalRead(photocell_1) == HIGH){
     t0 = millis(); // t0: the moment when projectile meets the first photocell
  }

  if(digitalRead(photocell_2) == HIGH){
     delta_t = millis() - t0; // t0: the moment when projectile meets the first photocell
  }

  return distance_photocell/(double)delta_t;

}
