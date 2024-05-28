#include <SD.h>
#include <SPI.h>

const int chipSelect = 4; // Chip select pin for the SD card module

void setup() {
    Serial.begin(9600);
    if (!SD.begin(chipSelect)) {
        Serial.println("SD card initialization failed!");
        return;
    }
    Serial.println("SD card initialized.");

    // Example: Writing a small amount of data to SD card
    File dataFile = SD.open("test.wav", FILE_WRITE);
    if (dataFile) {
        dataFile.write("Example audio data...");
        dataFile.close();
        Serial.println("Data written to file.");
    } else {
        Serial.println("Error opening file for writing.");
    }

    // Example: Reading data from SD card
    dataFile = SD.open("test.wav");
    if (dataFile) {
        while (dataFile.available()) {
            char data = dataFile.read();
            Serial.print(data);
        }
        dataFile.close();
        Serial.println("Data read from file.");
    } else {
        Serial.println("Error opening file for reading.");
    }
}

void loop() {
    // Add your audio processing code here
}
