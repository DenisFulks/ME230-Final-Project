#include <Servo.h>

// Motor Pins
#define pinXPos 48
#define pinXNeg 49
#define pinYPos 46
#define pinYNeg 47

// Claw Pins
#define spoolUpPin 50
#define spoolDownPin 51
#define servoPin 2

// Input Pins
#define pinClaw 21
#define pinJoyX A0
#define pinJoyY A1 

// Flags
volatile uint8_t mainEventFlags = 0x00;
#define dropClawFlag 1

// Constants
const uint8_t deadZone = 40;

// Variable Defenitions
int16_t xVal;
int16_t yVal;
int direction[2] = {0, 0};

// Claw
Servo claw;
uint8_t open = 0;
uint8_t close = 180;

unsigned int downTime = 1.2; // s

void setup() {
  Serial.begin(9600);

  // Motor Pins
  pinMode(pinXPos, OUTPUT);
  pinMode(pinXNeg, OUTPUT);
  pinMode(pinYPos, OUTPUT);
  pinMode(pinYNeg, OUTPUT);

  // Setup Claw Servo
  claw.attach(servoPin);
  claw.write(close);

  // Claw Interrupt
  pinMode(pinClaw, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinClaw), dropClaw, FALLING);
}

void loop() {
  if (mainEventFlags & dropClawFlag) {
    Serial.println("Test");

    pickup();
  }

  readJoyStick();
  moveGantry();
}

void readJoyStick() {
  xVal = analogRead(pinJoyX);
  yVal = analogRead(pinJoyY);

  xVal = map(xVal, 0, 1024, -128, 128);
  yVal = map(yVal, 0, 1024, -128, 128);

  if (xVal > deadZone) {
    direction[0] = 1;
  } else if (xVal < -deadZone) {
    direction[0] = -1;
  } else {
    direction[0] = 0;
  }
  
  if (yVal > deadZone) {
    direction[1] = -1;
  } else if (yVal < -deadZone) {
    direction[1] = 1;
  } else {
    direction[1] = 0;
  }

  Serial.println(direction[1]);
}

void moveGantry() {
  // X-Direction
  if (direction[0] > 0) {
    digitalWrite(pinXNeg, LOW);
    digitalWrite(pinXPos, HIGH);
  } else if (direction[0] < 0) {
    digitalWrite(pinXPos, LOW);
    digitalWrite(pinXNeg, HIGH);
  } else {
    digitalWrite(pinXPos, LOW);
    digitalWrite(pinXNeg, LOW);
  }

  // Y-Direction
  if (direction[1] > 0) {
    digitalWrite(pinYNeg, LOW);
    digitalWrite(pinYPos, HIGH);
  } else if (direction[1] < 0) {
    digitalWrite(pinYPos, LOW);
    digitalWrite(pinYNeg, HIGH);
  } else {
    digitalWrite(pinYPos, LOW);
    digitalWrite(pinYNeg, LOW);
  }
}

void pickup() {
  claw.write(open);

  digitalWrite(spoolDownPin, HIGH);
  delay(downTime * 1000);
  digitalWrite(spoolDownPin, LOW);

  delay(200);
  claw.write(close);
  delay(100);

  digitalWrite(spoolUpPin, HIGH);
  delay(downTime * 1000);
  digitalWrite(spoolUpPin, LOW);

  mainEventFlags &= ~dropClawFlag;
}

void dropClaw() {
  if (!(mainEventFlags & dropClawFlag)) {
    mainEventFlags |= dropClawFlag;
  }
}
