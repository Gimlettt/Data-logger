int Use_Pin = A0;    // Select the input pin for the sine wave
const int sampleRate = 33000; // Sample rate in Hz
unsigned long lastSampleTime = 0;
const int amplitude = 511;    // Amplitude of the sine wave (for 10-bit ADC)(for our Arduino)
const int offset = 512;       // Offset to center the sine wave (for 10-bit ADC)
int value = 0;

void setup() {
  Serial.begin(115200);
}
int analogReadFast(byte ADCpin) 
{ 
  byte ADCSRAoriginal = ADCSRA; 
  ADCSRA = (ADCSRA & B11111000) | 4; 
  int adc = analogRead(ADCpin);  
  ADCSRA = ADCSRAoriginal;
  return adc;
}
void loop() {
  unsigned long currentTime = micros();

  // Check if it's time to sample again
  if (currentTime - lastSampleTime >= 1000000 / sampleRate) {
    lastSampleTime = currentTime;

    // Read the value from the analog pin
    int Value = analogFastRead(Use_Pin);

    Serial.println(Value);
  }
  
    //int analogValue = value + offset;  // (analogValue amplitude is 105)
    //Serial.println(analogValue);
