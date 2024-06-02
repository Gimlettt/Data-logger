int switchPin = 2; // Pin connected to the footswitch
int switchState = 0;

void setup() {
  pinMode(switchPin, INPUT_PULLUP); // Set the switch pin as an input with an internal pull-up resistor
  Serial.begin(9600);
}

void loop() {
  switchState = digitalRead(switchPin); // Read the state of the switch
  if (switchState == LOW) {
    Serial.println("Switch Pressed");
  } else {
    Serial.println("Switch Released");
  }
  delay(100);
}
