int speakerPin = 9; // Pin connected to a piezo or speaker

void setup() {
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  for (int i = 0; i < 255; i++) {
    analogWrite(speakerPin, i);
    delay(10);
  }
  for (int i = 255; i >= 0; i--) {
    analogWrite(speakerPin, i);
    delay(10);
  }
}
