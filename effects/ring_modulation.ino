#include <avr/pgmspace.h>
#include <math.h>

// Pin definitions
const int analogInPin = A0;  // Analog input pin that the guitar is connected to

// Constants
const int sampleRate = 33000; // Sample rate in Hz
const float modFrequency = 50.0; // Modulator frequency in Hz
const int sineTableSize = 256; // Size of the sine table

unsigned long lastTime = 0;
unsigned int phase = 0;
const float phaseIncrement = (2 * PI * modFrequency) / sampleRate;

// Precomputed sine wave values stored in PROGMEM
const uint8_t sineTable[sineTableSize] PROGMEM = {
  128, 131, 134, 137, 140, 143, 146, 149, 152, 156, 159, 162, 165, 168, 171, 174, 
  177, 180, 183, 186, 189, 191, 194, 197, 200, 202, 205, 207, 210, 212, 215, 217, 
  219, 222, 224, 226, 228, 230, 232, 234, 236, 238, 240, 241, 243, 244, 246, 247, 
  249, 250, 251, 252, 253, 254, 255, 256, 256, 257, 257, 258, 258, 258, 258, 258, 
  258, 258, 258, 257, 257, 256, 256, 255, 254, 253, 252, 251, 250, 249, 247, 246, 
  244, 243, 241, 240, 238, 236, 234, 232, 230, 228, 226, 224, 222, 219, 217, 215, 
  212, 210, 207, 205, 202, 200, 197, 194, 191, 189, 186, 183, 180, 177, 174, 171, 
  168, 165, 162, 159, 156, 152, 149, 146, 143, 140, 137, 134, 131, 128, 124, 121, 
  118, 115, 112, 109, 106, 103,  99,  96,  93,  90,  87,  84,  81,  78,  75,  72, 
   69,  66,  63,  61,  58,  55,  52,  50,  47,  45,  42,  40,  37,  35,  32,  30, 
   28,  26,  24,  22,  20,  18,  16,  15,  13,  12,  10,   9,   7,   6,   5,   4, 
    3,   2,   1,   0,   0, 255, 254, 254, 253, 253, 253, 253, 253, 253, 253, 254, 
  254, 255, 255, 256, 257, 258, 259, 260, 261, 263, 264, 265, 267, 268, 270, 271, 
  273, 275, 276, 278, 280, 281, 283, 285, 287, 288, 290, 292, 294, 296, 298, 300, 
  302, 304, 306, 308, 310, 312, 314, 316, 318, 320, 322, 324, 326, 328, 330, 332, 
  334, 336, 338, 340, 342, 344, 346, 348, 350, 352, 354, 356, 358, 360, 362, 364, 
  366, 368, 370, 372, 374, 376, 378, 380, 382, 384, 386, 388, 390, 392, 394, 396, 
  398, 400, 402, 404, 406, 408, 410, 412, 414, 416, 418, 420, 422, 424, 426, 428
};

// Function to read analog input quickly
int analogReadFast(byte ADCpin) 
{ 
  byte ADCSRAoriginal = ADCSRA; 
  ADCSRA = (ADCSRA & B11111000) | 4; 
  int adc = analogRead(ADCpin);  
  ADCSRA = ADCSRAoriginal;
  return adc;
}

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

  // Apply ring modulation effect
  phase += phaseIncrement;
  if (phase >= sineTableSize) {
    phase -= sineTableSize;
  }
  int modSignal = pgm_read_byte(&(sineTable[phase]));
  int modulatedValue = (analogValue * (modSignal - 128)) / 128; // Centering around 0

  DACoutput(map(modulatedValue, -1023, 1023, 0, 255));
}
