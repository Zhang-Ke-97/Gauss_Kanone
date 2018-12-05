int spule_1 = 9;
int shoot = 7;
boolean flag = true;

void pulse(double t);

void stage1(double t);

void setup() {
  pinMode(spule_1, OUTPUT);
  pinMode(shoot, INPUT);
  digitalWrite(shoot, HIGH); // Turn on internal pull-up resitor

}

void loop() {

  
  if(digitalRead(shoot)==HIGH && flag ==true){ //Erste Stufe
      pulse(250); //50.72 aus Simulation
      flag=false;
  }

  //...

}



void pulse(double t){
  int t0 = millis();
  digitalWrite(spule_1, HIGH);
  while(true){
    if(millis()-t0 > t){
      digitalWrite(spule_1, LOW);
      break;
    }
  }
}
