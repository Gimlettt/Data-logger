import tkinter as tk
from tkinter import ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import numpy as np

def apply_effect(effect_name):
    print(f"{effect_name} effect applied")

def on_slider_change(event, effect_name, param_name):
    value = event.widget.get()
    print(f"{effect_name} {param_name} set to {value}")

def update_spectrum():
    # Generate a random signal for demonstration purposes
    t = np.linspace(0, 1, 500)
    signal = np.sin(2 * np.pi * 5 * t) + 0.5 * np.sin(2 * np.pi * 10 * t)
    
    # Compute the Fourier Transform of the signal
    spectrum = np.fft.fft(signal)
    freq = np.fft.fftfreq(len(signal), d=t[1] - t[0])
    
    # Clear the previous plot
    ax.clear()
    
    # Plot the spectrum
    ax.plot(freq, np.abs(spectrum))
    ax.set_xlim(0, 20)  # Limit x-axis to a reasonable range for demonstration
    
    # Redraw the canvas
    canvas.draw()

# Create the main application window
root = tk.Tk()
root.title("Guitar Effects Controller")

# Create a frame for the spectrum and controls
main_frame = tk.Frame(root)
main_frame.pack(fill=tk.BOTH, expand=True)

# Create a frame for the spectrum display
spectrum_frame = tk.Frame(main_frame)
spectrum_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=10, pady=10)

# Create a frame for the buttons and sliders
control_frame = tk.Frame(main_frame)
control_frame.pack(side=tk.RIGHT, fill=tk.Y, padx=10, pady=10)

# Create the Matplotlib figure and axes
fig, ax = plt.subplots(figsize=(5, 4))

# Create the canvas to display the plot
canvas = FigureCanvasTkAgg(fig, master=spectrum_frame)
canvas_widget = canvas.get_tk_widget()
canvas_widget.pack(fill=tk.BOTH, expand=True)

# Create a frame for the buttons
button_frame = tk.Frame(control_frame)
button_frame.pack(pady=10)

# Create buttons for each effect
effects = ["Delay", "Distortion", "Tremolo"]
for effect in effects:
    button = tk.Button(button_frame, text=effect, command=lambda e=effect: apply_effect(e))
    button.pack(side=tk.LEFT, padx=10)

# Create a frame for the sliders
slider_frame = tk.Frame(control_frame)
slider_frame.pack(pady=10)

# Add sliders for each effect parameter
parameters = {
    "Delay": ["Time", "Feedback", "Mix"],
    "Distortion": ["Gain", "Tone", "Level"],
    "Tremolo": ["Rate", "Depth"]
}

for effect, params in parameters.items():
    effect_label = tk.Label(slider_frame, text=effect)
    effect_label.pack(anchor="w", pady=(10, 0))
    for param in params:
        param_label = tk.Label(slider_frame, text=param)
        param_label.pack(anchor="w")
        slider = ttk.Scale(slider_frame, from_=0, to=100, orient=tk.HORIZONTAL)
        slider.set(50)  # Default value
        slider.pack(fill=tk.X, padx=20)
        slider.bind("<Motion>", lambda event, e=effect, p=param: on_slider_change(event, e, p))

# Update the spectrum plot initially
update_spectrum()

# Run the application
root.mainloop()
