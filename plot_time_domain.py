import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

# Serial port configuration
serial_port = '/dev/tty.usbmodem143301'  # Update this to match your port
baud_rate = 115200
arduino_serial = serial.Serial(serial_port, baud_rate)

# Set up plot
fig, ax = plt.subplots()
max_samples = 1000  # Number of samples to display
data = deque([0] * max_samples, maxlen=max_samples)

line, = ax.plot(data)
plt.ylim(0, 1024)  # Updated to match 10-bit ADC range
plt.xlabel('Sample Index')
plt.ylabel('Analog Value')
plt.title('Real-Time Sine Wave')

# Update the plot
def update(frame):
    while arduino_serial.in_waiting:
        try:
            # Read and parse the data from the serial port
            value = int(arduino_serial.readline().strip())
            data.append(value)
        except ValueError:
            continue

    line.set_ydata(data)
    ax.relim()
    ax.autoscale_view()
    return line,

# Animation function
ani = animation.FuncAnimation(fig, update, interval=10, blit=True)

# Show plot
plt.show()

# Close the serial connection when the plot is closed
arduino_serial.close()
