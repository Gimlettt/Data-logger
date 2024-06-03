import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial
import time
from collections import deque

# Constants for plotting
samples_per_second = 29000
time_window_ms = 5  # Time window in milliseconds
samples_per_window = int(samples_per_second * time_window_ms / 1000)  # Number of samples in the window

# Create figure for plotting
fig, ax = plt.subplots()
xs = list(range(samples_per_window))
ys = deque([0] * samples_per_window, maxlen=samples_per_window)

# Update the serial port for MacBook
# Replace 'COM3' with your actual serial port name
arduinoSerialData = serial.Serial('COM3', 115200)
time.sleep(2)  # Give some time for the serial connection to establish

line, = ax.plot(xs, ys)

# Define number of samples to read per frame
samples_per_frame = int(samples_per_second / 100)  # Aim for 30 fps

# This function is called periodically from FuncAnimation
def animate(i, ys):
    try:
        # Read the data from the serial port
        data_points = []
        while arduinoSerialData.in_waiting > 0 and len(data_points) < samples_per_frame:
            StringData = arduinoSerialData.readline().decode('utf-8', errors='ignore').strip()
            
            # Convert the serial string into a number
            try:
                data = int(StringData)
                data_points.append(data)
            except ValueError:
                continue  # Skip invalid data
        
        if data_points:
            ys.extend(data_points)
        
        # Update line data
        line.set_ydata(list(ys))
        
        # Adjust y-axis dynamically based on incoming data
        min_y = min(ys)
        max_y = max(ys)
        ax.set_ylim(min_y - 10, max_y + 10)
        
        # Format plot
        ax.set_title('Real-Time Guitar Signal')
        ax.set_xlabel('Sample Index')
        ax.set_ylabel('Analog Signal')
        ax.set_xlim(0, samples_per_window)  # Fix x-axis to the window size
    except Exception as e:
        print(f"Error: {e}")

# Set up plot to call animate() function periodically
anim = animation.FuncAnimation(fig, animate, fargs=(ys,), interval=100)  # Approx 30 fps
plt.show()
