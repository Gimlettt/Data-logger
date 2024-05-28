int analogPin = A0; // Pin where the signal is read
const int numReadings = 10;
int readings[numReadings]; // Array to store readings
int readIndex = 0;
int total = 0;
int average = 0;

void setup() {
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0; // Initialize all readings to 0
  }
}

int readMovingAverageADC() {
  // Subtract the last reading
  total -= readings[readIndex];
  // Read the current value
  readings[readIndex] = analogRead(analogPin);
  // Add the current reading to the total
  total += readings[readIndex];
  // Move to the next position in the array
  readIndex = (readIndex + 1) % numReadings;
  // Calculate the average
  average = total / numReadings;
  return average;
}
