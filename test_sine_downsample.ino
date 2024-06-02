#include <math.h>

const int sampleRate = 33000; // Sample rate in Hz
const float frequency = 5.0; // Frequency of the sine wave in Hz
const int amplitude = 511;    // Amplitude of the sine wave (for 10-bit ADC)
const int offset = 512;       // Offset to center the sine wave (for 10-bit ADC)
const int downsampleFactor = 10; // Send every nth sample
unsigned long lastSampleTime = 0; // Time at which the last sample was taken
int sampleCount = 0; // Counter to keep track of samples

void setup() {
  Serial.begin(115200);  // Set the baud rate to 115200
}

void loop() {
  unsigned long currentTime = millis();

  // Check if it's time to sample again
  if (currentTime - lastSampleTime >= 1000 / sampleRate) {
    lastSampleTime = currentTime;
    
    // Calculate the sine wave value
    float angle = 2.0 * PI * frequency * (currentTime / 1000.0);
    float sineValue = sin(angle);
    
    // Scale to 0-1023 range (assuming 10-bit ADC)
    int analogValue = (sineValue * amplitude) + offset;  // (sineValue is between -1 and 1)
    
    // Only send every nth sample
    if (sampleCount++ % downsampleFactor == 0) {
      Serial.println(analogValue);
    }
  }
}
