import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial

# Create figure for plotting
fig, ax = plt.subplots()
xs = []
ys = []

# Update the serial port for MacBook
arduinoSerialData = serial.Serial('/dev/tty.usbmodem143301', 115200)

# This function is called periodically from FuncAnimation
def animate(i, xs, ys):
    # Read and decode the data from the serial port
    StringData = arduinoSerialData.readline().decode('utf-8').strip()
    
    try:
        # Convert the serial string into a number
        data = float(StringData)
    except ValueError:
        # If conversion fails, skip this iteration
        return
    
    # Add sample index and y value to lists
    xs.append(len(xs))
    ys.append(data)
    
    # Limit y list to 100 items, and adjust x list accordingly
    ys = ys[-100:]
    xs = list(range(len(ys)))
    
    # Clear and plot the data
    ax.clear()
    ax.plot(xs, ys)
    
    # Format plot
    plt.title('Real-Time Data')
    plt.xlabel('Sample Index')
    plt.ylabel('Analog Signal')

# Set up plot to call animate() function periodically
anim = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=10)
plt.show()
