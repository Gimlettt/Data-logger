// Pin definitions
const int analogInPin = A0;  // Analog input pin that the sine wave is attached to

// Constants
const int sampleRate = 160000; // Sample rate in Hz
const int distortionThreshold = 512; // Threshold for distortion (clipping)

// Function to read analog input quickly
int analogReadFast(byte ADCpin) 
{ 
  byte ADCSRAoriginal = ADCSRA; 
  ADCSRA = (ADCSRA & B11111000) | 4; 
  int adc = analogRead(ADCpin);  
  ADCSRA = ADCSRAoriginal;
  return adc;
}

void setup() {
  Serial.begin(115200);
  // Set PORTB pins as output
  DDRB = DDRB | B00111111;
}

void loop() {
  // Read the analog input
  int analogValue = analogReadFast(analogInPin);

  // Apply distortion (clipping)
  int distortedValue;
  if (analogValue > distortionThreshold) {
    distortedValue = distortionThreshold;
  } else if (analogValue < -distortionThreshold) {
    distortedValue = -distortionThreshold;
  } else {
    distortedValue = analogValue;
  }

  // Output the distorted value to the DAC
  // The DAC accepts values between 0 and 4095, while analogRead() returns values between 0 and 1023
  PORTB = map(distortedValue, 0, 1023, 0, 63);
}
