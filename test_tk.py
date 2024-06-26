import tkinter as tk
from tkinter import ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
from collections import deque
import random

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
data = deque([0] * max_samples, maxlen=max_samples)

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

# Variables to simulate data
frequency = 5.0  # Frequency of the simulated sine wave
amplitude = 512  # Amplitude of the simulated sine wave
offset = 512  # Offset to center the sine wave
sample_rate = 1000  # Sample rate for the simulated data
time = np.linspace(0, 1, sample_rate)
current_effect = "None"
effects = ["Distortion", "Chorus", "Tremolo", "Delay", "Reverb"]

# Update the plot
def update(frame):
    global current_effect
    # Simulate incoming data
    sine_wave = amplitude * np.sin(2 * np.pi * frequency * (frame % sample_rate) / sample_rate) + offset
    data.append(sine_wave)
    
    # Simulate effect change
    if frame % 200 == 0:  # Change effect every 200 frames (~2 seconds)
        current_effect = random.choice(effects)
        effect_label.config(text=f"Current Effect: {current_effect}")
        print(f"Effect changed to: {current_effect}")  # Debug print
    
    line.set_ydata(data)
    ax.relim()
    ax.autoscale_view()
    canvas.draw()
    return line,

# Animation function
ani = animation.FuncAnimation(fig, update, interval=10, blit=True)

# Start the GUI event loop
root.mainloop()
