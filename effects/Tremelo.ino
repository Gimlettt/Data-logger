#include <avr/pgmspace.h>
#include <math.h>

// Pin definitions
const int analogInPin = A0;  // Analog input pin that the sine wave is attached to
// Constants
const int sampleRate = 330000; // Sample rate in Hz
const float tremoloFrequency = 5.0; // Tremolo frequency in Hz
const float tremoloDepth = 0.5; // Tremolo depth (0.0 to 1.0)
const int lfoResolution = 256; // Resolution of the LFO sine wave

unsigned long lastTime = 0;
float phase = 0.0; // Phase of the LFO



// Function to read analog input quickly
int analogReadFast(byte ADCpin) 
{ 
  byte ADCSRAoriginal = ADCSRA; 
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
  // Calculate time delta
  unsigned long currentTime = micros();
  float deltaTime = (currentTime - lastTime) / 1000000.0;
  lastTime = currentTime;

  // Update LFO phase
  phase += tremoloFrequency * deltaTime;
 // Update LFO phase
  
  if (phase >= 1.0) {
    phase -= 1.0;
  }

  // Calculate LFO value (sine wave)
  float lfo = (1.0 + sin(2 * PI * phase)) / 2.0; // LFO ranges from 0.0 to 1.0

  // Read the analog input
  int analogValue = analogReadFast(analogInPin);

  // Apply tremolo effect
  int tremoloValue = analogValue * (1.0 - tremoloDepth + (lfo * tremoloDepth));

  // Output the tremolo value to the DAC
  // The DAC accepts values between 0 and 4095, while analogRead() returns values between 0 and 1023
  DACoutput(map(tremoloValue, 0, 1023, 0, 255));
}
