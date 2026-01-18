#include <Servo.h>

Servo claw;

void setup() {
  Serial.begin(9600);

  claw.attach(2);

  pinMode(23, INPUT_PULLUP);
}

void loop() {
  if (!digitalRead(23)){
    claw.write(0);
  } else {
    claw.write(90);
  }
}
