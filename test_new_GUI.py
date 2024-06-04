import serial
import tkinter as tk
from tkinter import ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

# Serial port configuration
serial_port = '/dev/tty.usbmodem143301'  # Update this to match your port
baud_rate = 115200
arduino_serial = serial.Serial(serial_port, baud_rate)

# Create the main window
root = tk.Tk()
root.title("Real-Time Guitar Effects")
root.configure(bg="#2E2E2E")  # Set background color

# Create a frame for the plot
frame = ttk.Frame(root, padding="10")
frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))

# Create a label to display the effect name
effect_label = tk.Label(root, text="Current Effect: None", font=("Helvetica", 20, "bold"), foreground="#FFFFFF", background="#00CED1", padx=10, pady=10)
effect_label.grid(row=1, column=0, padx=10, pady=10)

# Set up plot
fig, ax = plt.subplots()
max_samples = 1000  # Number of samples to display
data = deque([0] * max_samples, maxlen=max_samples)#array that could be cut from both ends

line, = ax.plot(data, color='#FF4500')
ax.set_ylim(0, 1024)  # Updated to match 10-bit ADC range
ax.set_xlabel('Sample Index', fontsize=12, color='#FFFFFF')
ax.set_ylabel('Analog Value', fontsize=12, color='#FFFFFF')
ax.set_title('Real-Time Guitar Signal', fontsize=16, color='#FFFFFF')
ax.tick_params(axis='x', colors='#FFFFFF')
ax.tick_params(axis='y', colors='#FFFFFF')
fig.patch.set_facecolor('#2E2E2E')
ax.set_facecolor('#1E1E1E')

# Integrate plot with Tkinter
canvas = FigureCanvasTkAgg(fig, master=frame)
canvas.get_tk_widget().grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))

# Update the plot
def update(frame):
    while arduino_serial.in_waiting:
        try:
            line = arduino_serial.readline().strip().decode('utf-8')
            if line.startswith("Effect:"):
                effect_name = line.split(":")[1].strip()
                effect_label.config(text=f"Current Effect: {effect_name}")
            else:
                value = int(line)
                data.append(value)
        except ValueError:
            continue

    line.set_ydata(data)
    ax.relim()
    ax.autoscale_view()
    canvas.draw()
    return line,
# def update(frame):
#     if arduino_serial.in_waiting:
#         try:
#             line = arduino_serial.readline().strip().decode('utf-8')
#             if line.startswith("Effect:"):
#                 effect_name = line.split(":")[1].strip()
#                 effect_label.config(text=f"Current Effect: {effect_name}")
#             else:
#                 value = int(line)
#                 data.append(value)
#         except ValueError:
#             pass

#     line.set_ydata(data)
#     ax.relim()
#     ax.autoscale_view()
#     canvas.draw()

# Animation function
ani = animation.FuncAnimation(fig, update, interval=10, blit=True)

# Start the GUI event loop
root.mainloop()

# Close the serial connection when the GUI is closed
arduino_serial.close()
