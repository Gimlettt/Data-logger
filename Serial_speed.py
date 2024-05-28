import serial
import time

# Update the serial port for your setup
arduinoSerialData = serial.Serial('/dev/cu.usbmodem143301', 115200)

# Test parameters
num_bytes_to_read = 100000  # Number of bytes to read for the test

# Ensure the serial buffer is clear
arduinoSerialData.reset_input_buffer()

# Start time
start_time = time.time()

# Read the specified number of bytes
data = arduinoSerialData.read(num_bytes_to_read)

# End time
end_time = time.time()

# Calculate throughput
elapsed_time = end_time - start_time
bytes_per_second = num_bytes_to_read / elapsed_time
bits_per_second = bytes_per_second * 8

print(f"Elapsed Time: {elapsed_time:.2f} seconds")
print(f"Bytes per Second: {bytes_per_second:.2f}")
print(f"Bits per Second: {bits_per_second:.2f} bps")

arduinoSerialData.close()
