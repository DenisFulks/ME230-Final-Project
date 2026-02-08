#define xLimPin 21
#define yLimPin 20

volatile uint8_t mainEventFlags = 0x00;
#define stopXFlag 2
#define stopYFlag 4

void setup() {
  Serial.begin(9600);

  pinMode(yLimPin, INPUT_PULLUP);
}

void loop() {
  if (!digitalRead(yLimPin)) {
    Serial.println("StopY");
  }
}
