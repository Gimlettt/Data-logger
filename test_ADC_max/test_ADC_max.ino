// Pin definitions
const int analogInPin = A0;  // Analog input pin that the sine wave is attached to

int analogReadFast(byte ADCpin) {
  byte ADCSRAoriginal = ADCSRA;
  ADCSRA = (ADCSRA & B11111000) | 4;
  int adc = analogRead(ADCpin);
  ADCSRA = ADCSRAoriginal;
  return adc;
}

int maxAnalogValue = 0;

void setup() {
  Serial.begin(115200);
  // Initialize the serial communication
  Serial.println("Starting to read analog values...");
}

void loop() {
  // Read the analog input
  int analogValue = analogReadFast(analogInPin);
  
  // Check if this value is the maximum observed
  if (analogValue > maxAnalogValue) {
    maxAnalogValue = analogValue;
    // Print the new maximum value
    Serial.print("New max value: ");
    Serial.println(maxAnalogValue);
  }

  // Small delay to avoid overwhelming the serial output
  delay(10);
}
