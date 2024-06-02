#include <avr/pgmspace.h>
#include <math.h>

// Pin definitions
const int analogInPin = A0;  // Analog input pin that the guitar is connected to

// Constants
const int sampleRate = 33000; // Sample rate in Hz
const int bitDepth = 4; // Bit depth for bitcrusher effect

unsigned long lastTime = 0;



void DACoutput(int value) {    // value=[0,255], pin 6 is LSB, pin 13 is MSB
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
  ADCSRA &= ~(bit(ADPS0) | bit(ADPS1) | bit(ADPS2)); // clear prescaler bits
  ADCSRA |= bit(ADPS2);    //  16-scale 
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {
  unsigned long currentTime = micros();
  float deltaTime = (currentTime - lastTime) / 1000000.0;
  lastTime = currentTime;

  int analogValue = analogRead(analogInPin);

  // Apply bitcrusher effect
  int stepSize = pow(2, 10 - bitDepth);
  int crushedValue = (analogValue / stepSize) * stepSize;

  DACoutput(map(crushedValue, 0, 1023, 0, 255));
}
