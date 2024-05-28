import serial
import struct
import time

# Configure the serial port
ser = serial.Serial('COM3', 115200)  # Replace 'COM3' with your port
time.sleep(2)  # Allow some time for the serial port to initialize

data = []

# Duration to record data in seconds
record_duration = 10
start_time = time.time()

while time.time() - start_time < record_duration:
    if ser.in_waiting >= 2:  # Ensure we have at least 2 bytes available
        high_byte = ser.read()
        low_byte = ser.read()
        # Combine high and low byte into a single 16-bit integer
        value = struct.unpack('>H', high_byte + low_byte)[0]
        data.append(value)

# Close the serial port
ser.close()

# Save data to a file
with open('adc_data.txt', 'w') as f:
    for value in data:
        f.write(f"{value}\n")
