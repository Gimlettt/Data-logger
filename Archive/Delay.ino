#include <avr/pgmspace.h>
#include <math.h>

// Pin definitions
const int analogInPin = A0;  // Analog input pin that the sine wave is attached to
const int sampleRate = 330000;
// Constants
const int sampleRate = 330000; // Sample rate in Hz
const uint8_t bufferSize = 64;  // Size of the buffer
unsigned long lastTime = 0;
int buffer[bufferSize];      // Buffer to store the sine wave samples
int writeIndex = 0;          // Write index for the buffer
int delayIndex = 32;        // Delay index (e.g., half buffer size for 50% delay)

int analogReadFast(byte ADCpin) 
{ byte ADCSRAoriginal = ADCSRA; 
  ADCSRA = (ADCSRA & B11111000) | 4; 
  int adc = analogRead(ADCpin);  
  ADCSRA = ADCSRAoriginal;
  return adc;
}

void DACoutput(int value){    //value=[0,255], pin 6 is LSB, pin 13 is MSB
  int PB = value >> 2;
  PORTB = PB;
  
  int VD = PORTD;
  int PD = value & B00000011;
  PD = PD << 6;
  VD = VD & B00111111;
  VD = VD | PD;
  PORTD = VD;  
}

void setup() {
  Serial.begin(115200);
  DDRB = DDRB | B00111111;
  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2)); // clear prescaler bits
  ADCSRA |= bit (ADPS2);    //  16-scale 
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}


void loop() {
  // Read the analog input
  int analogValue = analogRead(analogInPin);

  // Store the value in the buffer
  buffer[writeIndex] = analogValue;
  
  // Compute the delayed index
  int delayedValue = buffer[delayIndex];

  // Mix the original and delayed values (simple average for this example)
  int mixedValue = (analogValue + delayedValue) / 2;

  // Output the mixed value to the DAC
  // The DAC accepts values between 0 and 255, while analogRead() returns values between 0 and 1023(if value cannot swing to 1023, can make it shorter for better amplitude)
  DACoutput(map(mixedValue, 0, 1023, 0, 63));
  
  //Serial.println(PORTB);

  // Update indices
  writeIndex = (writeIndex + 1) % bufferSize;
  delayIndex = (delayIndex + 1) % bufferSize;

}
