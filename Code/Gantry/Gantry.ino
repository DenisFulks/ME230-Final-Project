// Motor Pins
#define pinXPos 30
#define pinXNeg 31
#define pinYPos 32
#define pinYNeg 33

// Input Pins
#define pinEStop 2
#define pinJoyX A0
#define pinJoyY A1 

// Other Pins
#define xEnable 22
#define yEnable 23

// Constants
const uint8_t deadZone = 40;

// Variable Defenitions
uint8_t EStop = 0;
int16_t xVal;
int16_t yVal;
int direction[2] = {0, 0};

void setup() {
  Serial.begin(9600);

  // Motor Pins
  pinMode(pinXPos, OUTPUT);
  pinMode(pinXNeg, OUTPUT);
  pinMode(pinYPos, OUTPUT);
  pinMode(pinYNeg, OUTPUT);

  // Enable Pins
  pinMode(xEnable, OUTPUT);
  pinMode(yEnable, OUTPUT);

  // E-Stop Interrupt
  pinMode(pinEStop, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinEStop), EmergencyStop, FALLING);

  // Closing Setup
  sei();
  digitalWrite(xEnable, HIGH);
  digitalWrite(yEnable, HIGH);
}

void loop() {
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

  Serial.println(String(direction[0]) + String(", ") + String(direction[1]));
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

void EmergencyStop() {
  if (!EStop){
    Serial.println(String("Stopping"));

    digitalWrite(xEnable, LOW);
    digitalWrite(yEnable, LOW);
  } else {
    Serial.println(String("Starting"));

    digitalWrite(xEnable, HIGH);
    digitalWrite(yEnable, HIGH);
  }

  EStop = !EStop;
}
