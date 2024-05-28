// test analogfast read maximum value

// Pin definitions
const int analogInPin = A0;  // Analog input pin that the sine wave is attached to

// Constants
const int sampleRate = 160000; // Sample rate in Hz
const int bufferSize = 256;  // Size of the buffer

int buffer[bufferSize];      // Buffer to store the sine wave samples
int writeIndex = 0;          // Write index for the buffer
int delayIndex = 80;        // Delay index (e.g., half buffer size for 50% delay)

int analogReadFast(byte ADCpin) 
{ byte ADCSRAoriginal = ADCSRA; 
  ADCSRA = (ADCSRA & B11111000) | 4; 
  int adc = analogRead(ADCpin);  
  ADCSRA = ADCSRAoriginal;
  return adc;
}
void setup() {
  Serial.begin(115200);
  //print anlaog read
  DDRB = DDRB | B00111111;

}

void loop() {
  // Read the analog input
  int analogValue = analogReadFast(analogInPin);

  // Store the value in the buffer
  buffer[writeIndex] = analogValue;
  
  // Compute the delayed index
  int delayedValue = buffer[delayIndex];

  // Mix the original and delayed values (simple average for this example)
  int mixedValue = (analogValue + delayedValue) / 2;

  // Output the mixed value to the DAC
  // The DAC accepts values between 0 and 4095, while analogRead() returns values between 0 and 1023
  PORTB = map(mixedValue, 0, 200, 0, 64);
  Serial.println(PORTB);





  // Update indices
  writeIndex = (writeIndex + 1) % bufferSize;
  delayIndex = (delayIndex + 1) % bufferSize;

  //delayMicroseconds(1000000 / sampleRate);
}
