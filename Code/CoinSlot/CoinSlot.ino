#define photoResistorPin A2
#define cutoff 50

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (analogRead(photoResistorPin) < cutoff) {
    while (analogRead(photoResistorPin) < cutoff);

    Serial.println("Play!");
  }
}
