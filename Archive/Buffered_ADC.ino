#include <SD.h>
#include <SPI.h>

const int chipSelect = 4; // SD card module chip select pin
const int analogPin = A0; // Analog input pin for the guitar signal
const int samplingRate = 8000; // Sampling rate in Hz (8 kHz)
const int duration = 20; // Duration to record in seconds
const int numSamples = samplingRate * duration; // Total number of samples
const int bufferSize = 512; // Size of buffer for writing to SD card

volatile int buffer[bufferSize]; // Buffer for ADC samples
volatile int bufferIndex = 0; // Current index in the buffer
volatile bool bufferReady = false; // Flag indicating buffer is ready to be written

File dataFile;

void setup() {
    Serial.begin(115200);

    // Initialize the SD card
    if (!SD.begin(chipSelect)) {
        Serial.println("SD card initialization failed!");
        while (1);
    }
    Serial.println("SD card initialized.");

    // Open file for writing
    dataFile = SD.open("recording.bin", FILE_WRITE);
    if (!dataFile) {
        Serial.println("Error opening file for writing!");
        while (1);
    }

    // Timer setup for sampling
    cli(); // Stop interrupts
    TCCR1A = 0; // Clear Timer/Counter Control Registers
    TCCR1B = 0;
    TCNT1 = 0; // Clear counter
    OCR1A = 1999; // Set compare match register (16 MHz / (8 kHz * 2) - 1)
    TCCR1B |= (1 << WGM12); // CTC mode
    TCCR1B |= (1 << CS10) | (1 << CS11); // 64 prescaler
    TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
    sei(); // Allow interrupts

    Serial.println("Recording started.");
}

ISR(TIMER1_COMPA_vect) {
    if (bufferIndex < bufferSize) {
        buffer[bufferIndex++] = analogRead(analogPin); // Read ADC value and store in buffer
    } else {
        bufferReady = true; // Set flag to indicate buffer is ready
        bufferIndex = 0; // Reset buffer index
    }
}

void loop() {
    if (bufferReady) {
        // Write buffer to SD card
        dataFile.write((byte*)buffer, bufferSize * sizeof(int));
        bufferReady = false; // Reset flag
    }

    // Stop recording after the specified duration
    static unsigned long startTime = millis();
    if (millis() - startTime >= duration * 1000) {
        TIMSK1 &= ~(1 << OCIE1A); // Disable timer interrupt
        dataFile.close(); // Close file
        Serial.println("Recording finished.");
        while (1); // Stop further processing
    }
}
