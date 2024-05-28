#include <arduinoFFT.h>
#define SAMPLES 128             // Must be a power of 2
#define SAMPLING_FREQUENCY 1000 // Hz, must be 2 * Nyquist frequency

arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

void setup() {
  Serial.begin(9600);
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}

void loop() {
  /* Sample the signal */
  for (int i = 0; i < SAMPLES; i++) {
    microseconds = micros(); // Overflows after around 70 minutes!
    vReal[i] = analogRead(A0);
    vImag[i] = 0;

    while (micros() < (microseconds + sampling_period_us)) {
      // wait
    }
  }

  /* Perform FFT */
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  /* Send data over serial */
  for (int i = 0; i < (SAMPLES / 2); i++) {
    Serial.print(vReal[i]);
    if (i < (SAMPLES / 2) - 1) {
      Serial.print(",");
    }
  }
  Serial.println();

  delay(100); // Adjust delay for your requirements
}
