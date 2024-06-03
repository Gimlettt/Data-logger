
#include <avr/pgmspace.h>
#include <math.h>

const int bufferSize = 64; //serial buffer length = 64bytes. 63 Bytes available for TX.
const int analogInPin = A0;  // Analog input pin that the guitar is connected to
unsigned long lastTime = 0;
const int button_up = 3;
const int button_down = 2;
volatile uint8_t effect_code = 0; //the current code of effect. range = 0 to effect_numbers-1
volatile uint8_t effect_numbers = 5; //change this for the total number of effects we have.



const int bitDepth = 4; // Bit depth for bitcrusher effect

const int lfoResolution = 256; // Resolution of the LFO sine wave for chorus effect
int buffer[bufferSize];
int bufferIndex = 0;
float phase = 0.0; // Phase of the LFO

const float tremoloFrequency = 5.0; // Tremolo frequency in Hz
const float tremoloDepth = 0.5; // Tremolo depth (0.0 to 1.0)
const int lfoResolution = 256; // Resolution of the LFO sine wave

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



// Precomputed sine wave
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

void setup() {
  Serial.begin(115200);
  
  //pinmode: 6-13 as output:
  DDRB = DDRB | B00111111;
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  pinMode(button_up, INPUT);
  pinMode(button_down, INPUT);
  attachInterrupt(digitalPinToInterrupt(button_up), effect_next, RISING);
  attachInterrupt(digitalPinToInterrupt(button_down), effect_last, RISING);

  //configure ADC clock prescaler as 16. max ADC sampling rate 66kHz.
  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2)); // clear prescaler bits
  ADCSRA |= bit (ADPS2);    //  16-scale 

  for (int i = 0; i < bufferSize; i++) {
    buffer[i] = 0;
  }
}


void effect_next(){
  effect_code++;
  effect_code %= effect_numbers;
  switch(effect_code){
    case 0:
      Serial.println("Effect:Bypass");
      break;
    
    case 1:
      Serial.println("Effect:Tremolo");
      break;
    
    case 2:
      Serial.println("Effect:Chorus");
      break;
    
    case 3:
      Serial.println("Effect:Distortion");
      break;

    case 4:
      Serial.println("Effect:Bitcrusher"); 
      break;
  }
}

void effect_last(){
  effect_code--;
  effect_code %= effect_numbers;
  switch(effect_code){
    case 0:
      Serial.println("Effect:Bypass");
      break;
    
    case 1:
      Serial.println("Effect:Tremolo");
      break;
    
    case 2:
      Serial.println("Effect:Chorus");
      break;
    
    case 3:
      Serial.println("Effect:Distortion");
      break;

    case 4:
      Serial.println("Effect:Bitcrusher"); 
      break;
  }
}



void loop() {
  
  unsigned long currentTime = micros();
  float deltaTime = (currentTime - lastTime) / 1000000.0;
  lastTime = currentTime;


  
  switch(effect_code){
    case 0://bypass
      int analogValue = analogRead(analogInPin);
      DACoutput(map(analogValue, 0, 1023, 0, 255));
      Serial.println(analogValue);
      break;
    
    case 1://tremolo
      phase += tremoloFrequency * deltaTime;
      if (phase >= 1.0) {
        phase -= 1.0;
      }

      // Calculate LFO value (sine wave)
      float lfo = (1.0 + sin(2 * PI * phase)) / 2.0; // LFO ranges from 0.0 to 1.0

      // Read the analog input
      int analogValue = analogRead(analogInPin);

      // Apply tremolo effect
      int tremoloValue = analogValue * (1.0 - tremoloDepth + (lfo * tremoloDepth));

      // Output the tremolo value to the DAC
      // The DAC accepts values between 0 and 4095, while analogRead() returns values between 0 and 1023
      DACoutput(map(tremoloValue, 0, 1023, 0, 255));
      Serial.println(tremoloValue);
      break;
    
    case 2:
      // Update LFO phase
      phase += 0.5 * deltaTime; // Chorus modulation frequency
      if (phase >= 1.0) {
        phase -= 1.0;
      }

      // Calculate LFO value (sine wave)
      int index = (int)(phase * lfoResolution);
      float lfo = pgm_read_byte_near(sineTable + index) / 255.0; // LFO ranges from 0.0 to 1.0

      int analogValue = analogRead(analogInPin);

      // Apply chorus effect
      float modDepth = 0.5; // Depth of modulation
      int modIndex = bufferIndex - (int)(modDepth * lfo * bufferSize);
      if (modIndex < 0) {
        modIndex += bufferSize;
      }

      int chorusValue = (analogValue + buffer[modIndex]) / 2;
      buffer[bufferIndex] = analogValue;
      bufferIndex = (bufferIndex + 1) % bufferSize;
      DACoutput(map(chorusValue, 0, 1023, 0, 255));
      Serial.println(chorusValue);
      break;
    
    case 3:
      int analogValue = analogRead(analogInPin);

      // Apply distortion effect
      int gain = 3; // Increase the gain for more distortion
      int distortedValue = analogValue * gain;
      if (distortedValue > 1023) {
        distortedValue = 1023; // Clipping
      } else if (distortedValue < 0) {
        distortedValue = 0; // Clipping
      }

      DACoutput(map(distortedValue, 0, 1023, 0, 255));
      Serial.println(distortedValue);
      break;

    case 4:
      int analogValue = analogRead(analogInPin);

      // Apply bitcrusher effect
      int stepSize = pow(2, 10 - bitDepth);
      int crushedValue = (analogValue / stepSize) * stepSize;
      DACoutput(map(crushedValue, 0, 1023, 0, 255));
      Serial.println(crushedValue);
      break;
  }

}

