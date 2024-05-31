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

// Precomputed sine wave values stored in PROGMEM
const uint8_t sineTable[lfoResolution] PROGMEM = {
  127, 130, 133, 136, 139, 142, 145, 148, 151, 154, 157, 160, 163, 166, 169, 172,
  175, 178, 181, 184, 187, 190, 193, 196, 199, 202, 205, 208, 211, 214, 217, 220,
  223, 226, 229, 231, 234, 237, 240, 243, 246, 249, 251, 254, 257, 260, 263, 265,
  268, 271, 273, 276, 279, 281, 284, 287, 289, 292, 294, 297, 299, 302, 304, 307,
  309, 312, 314, 316, 319, 321, 323, 326, 328, 330, 332, 335, 337, 339, 341, 343,
  345, 347, 349, 351, 353, 355, 357, 359, 361, 363, 364, 366, 368, 370, 371, 373,
  375, 376, 378, 380, 381, 383, 384, 386, 387, 389, 390, 391, 393, 394, 395, 396,
  398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413,
  413, 414, 415, 416, 417, 417, 418, 419, 419, 420, 421, 421, 422, 422, 423, 423,
  424, 424, 424, 425, 425, 425, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426,
  426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426,
  426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426,
  426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426
};

// Function to read analog input quickly,now we don't need to design it
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
  if (phase >= 1.0) {
    phase -= 1.0;op
  int index = (int)(phase * lfoResolution);
  float lfo = pgm_read_byte_near(sineTable + index) / 255.0; // LFO ranges from 0.0 to 1.0

  // Read the analog input
  int analogValue = analogRead(analogInPin);

  // Apply tremolo effect
  int tremoloValue = analogValue * (1.0 - tremoloDepth + (lfo * tremoloDepth));

  // Output the tremolo value to the DAC
  // The DAC accepts values between 0 and 4095, while analogRead() returns values between 0 and 1023
  DACoutput(map(tremoloValue, 0, 1023, 0, 63));
}
