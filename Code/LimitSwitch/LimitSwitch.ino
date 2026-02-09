#define yMinPin 22
#define yMaxPin 23
#define xMinPin 24
#define xMaxPin 25


void setup() {
  Serial.begin(9600);

  pinMode(xMinPin, INPUT_PULLUP);
  pinMode(xMaxPin, INPUT_PULLUP);
  pinMode(yMinPin, INPUT_PULLUP);
  pinMode(yMaxPin, INPUT_PULLUP);
}

void loop() {
  if (!digitalRead(xMinPin)) {
    Serial.println("Stop X Min");
  } else if (!digitalRead(xMaxPin)) {
    Serial.println("Stop X Max");
  }

  if (!digitalRead(yMinPin)) {
    Serial.println("Stop Y Min");
  } else if (!digitalRead(yMaxPin)) {
    Serial.println("Stop Y Max");
  }
}
