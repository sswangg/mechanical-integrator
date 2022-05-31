// screw no move
// note: 0 is usually not 0

// encoder: red 5v, black gnd, white 2, green 3

int counter = 0;
// serial plotter only holds 500 values at a time, not
// configurable without modifying and recompling the IDE
// should_print makes it only output every 100 cycles to 
// show more data at once
int should_print = 10;

#define disk_pin_1 4
#define disk_pin_2 5
#define disk_pin_3 6
#define disk_pin_4 7
int disk_steps = 0;
// disk stepper motor needs to step half as often as the 
// screw stepper motor or else it dies for unknown reasons
// so go toggles to keep track of when the disk should step
boolean go = true;

int total_steps = 0;  // for printing purposes (for screw)

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  // call the fucntions when the thing happens
  attachInterrupt(0, ai0, RISING);
  attachInterrupt(1, ai1, RISING);
  
  pinMode(disk_pin_1, OUTPUT);
  pinMode(disk_pin_2, OUTPUT);
  pinMode(disk_pin_3, OUTPUT);
  pinMode(disk_pin_4, OUTPUT);
}

void loop() {
  for(int i=0; i<25000; i++){
    // add stuff to plot once ever 100 cycles
    if (should_print == 100) {
      should_print = 0;
      Serial.print("Function:");
      Serial.print("90");
      Serial.print(",");
      Serial.print("Integral:");
      Serial.println(-counter*0.17361111111);
    }
    should_print++;
    // step the stepper motors
    disk_stepper();
    // wait bc the motors go skreeee if you don't
    delayMicroseconds(800);
  }
}

void ai0() {
  if (digitalRead(3) == LOW) {
    counter++;
  } else {
    counter--;
  }
}

void ai1() {
  if (digitalRead(2) == LOW) {
    counter--;
  } else {
    counter++;
  }
}

void disk_stepper(){
  if (go) {
    switch(disk_steps){
      case 0:
        digitalWrite(disk_pin_1, HIGH);
        digitalWrite(disk_pin_2, LOW);
        digitalWrite(disk_pin_3, LOW);
        digitalWrite(disk_pin_4, LOW);
        break;
      case 1:
        digitalWrite(disk_pin_1, LOW);
        digitalWrite(disk_pin_2, HIGH);
        digitalWrite(disk_pin_3, LOW);
        digitalWrite(disk_pin_4, LOW);
        break;
      case 2:
        digitalWrite(disk_pin_1, LOW);
        digitalWrite(disk_pin_2, LOW);
        digitalWrite(disk_pin_3, HIGH);
        digitalWrite(disk_pin_4, LOW);
        break;
      case 3:
        digitalWrite(disk_pin_1, LOW);
        digitalWrite(disk_pin_2, LOW);
        digitalWrite(disk_pin_3, LOW);
        digitalWrite(disk_pin_4, HIGH);
        break;
    }
    disk_steps++;
    if(disk_steps > 3){
      disk_steps = 0;
    }
  } 
  go = !go;
}
