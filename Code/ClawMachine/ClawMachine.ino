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

// Limit Pins
#define yMinPin 22
#define yMaxPin 23
#define xMinPin 24
#define xMaxPin 25

// Input Pins
#define pinClaw 21
#define EStopPin 20
#define pinJoyX A0
#define pinJoyY A1
#define photoResistorPin A2

// Flags
volatile uint8_t mainEventFlags = 0x00;
#define dropClawFlag 1
#define EStopFlag 2

// Constants
const uint8_t deadZone = 40;
const uint8_t photoResistorCutoff = 50;
const uint8_t maxTime = 20; // Seconds

// Variable Defenitions
int16_t xVal;
int16_t yVal;
int direction[2] = {0, 0};
bool play = false;
int16_t startTime;
int16_t endTime;

// Claw
Servo claw;
uint8_t open = 0;
uint8_t close = 180;

unsigned float downTime = 1.5; // Seconds

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

  // Limit Pins
  pinMode(xMinPin, INPUT_PULLUP);
  pinMode(xMaxPin, INPUT_PULLUP);
  pinMode(yMinPin, INPUT_PULLUP);
  pinMode(yMaxPin, INPUT_PULLUP);

  // Claw Interrupt
  pinMode(pinClaw, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinClaw), dropClaw, FALLING);

  // EStop Interrupt
  pinMode(EStopPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EStopPin), EStop, FALLING);

  goHome();
}

void loop() {
  if (!play) {
      if (analogRead(photoResistorPin) < photoResistorCutoff) {
      while (analogRead(photoResistorPin) < photoResistorCutoff);

      Serial.println("Play!");

      startTime = millis();
      endTime = startTime + (maxTime*1000);
      play = true;
    }
  } else { 
    readJoyStick();

    if (mainEventFlags & dropClawFlag) {
      pickup();
      goHome();
      drop();
    }

    if ((!digitalRead(xMinPin) && direction[0] < 0) || (!digitalRead(xMaxPin) && direction[0] > 0)) {
      direction[0] = 0;
    }

    if ((!digitalRead(yMinPin) && direction[1] < 0) || (!digitalRead(yMaxPin) && direction[1] > 0)) {
      direction[1] = 0;
    }

    moveGantry();

    if (startTime + millis() > endTime) {
      pickup();
      goHome();
      drop();
    }
  }
}

void readJoyStick() {
  xVal = analogRead(pinJoyX);
  yVal = analogRead(pinJoyY);

  xVal = map(xVal, 0, 1024, -128, 128);
  yVal = map(yVal, 0, 1024, -128, 128);

  if (xVal > deadZone) {
    direction[0] = -1;
  } else if (xVal < -deadZone) {
    direction[0] = 1;
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

void drop() {
  claw.write(open);
  delay(200);

  claw.write(close);
  delay(100);
}

void goHome() {
  while (digitalRead(xMinPin) || digitalRead(yMinPin)) {
    Serial.println("Going Home");

    if (digitalRead(xMinPin)) {
      direction[0] = -1;
    } else {
      direction[0] = 0;
    }

    if (digitalRead(yMinPin)) {
      direction[1] = -1;
    } else {
      direction[1] = 1;
    }

    moveGantry();
  }

  play = false;
}

void dropClaw() {
  if (!(mainEventFlags & dropClawFlag)) {
    mainEventFlags |= dropClawFlag;
  }
}

void EStop() {
  if (mainEventFlags & EStopFlag) {
    Serial.println("Starting");

    mainEventFlags &= ~EStopFlag;
  } else {
    Serial.println("Stopping");

    mainEventFlags |= EStopFlag;
  }

  delay(1000);
}
