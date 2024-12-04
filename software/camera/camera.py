import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import re
import numpy as np
import yaml
import os
import bluetooth

# Define default configuration
default_config = {
    'serial_port': '/dev/cu.NXP',
    'baud_rate': 115200,
    'bluetooth_mac': '00:1A:7D:DA:71:13',
    'debugEnable': 1,
    'logEnable': 1  
}

def logDebug(text):
    if(debugEnable == 1): print("[Debug]:" + text)
    
def logInfo(text):
    if(logEnable == 1): print(text)

# Path to the config file
config_file = 'config.yml'

# Function to load or create the config file
def load_config():
    if not os.path.exists(config_file):
        with open(config_file, 'w') as file:
            yaml.dump(default_config, file)
        print(f"[Config] Initialized default config at {config_file}")
        return default_config
    else:
        with open(config_file, 'r') as file:
            return yaml.safe_load(file)

# Load the config
config = load_config()
serial_port = config.get('serial_port', default_config['serial_port'])
baud_rate = config.get('baud_rate', default_config['baud_rate'])
bluetooth_mac = config.get('bluetooth_mac', default_config['bluetooth_mac'])    
debugEnable = config.get('debugEnable', default_config['debugEnable'])  
logEnable = config.get('logEnable', default_config['logEnable'])        

# Function to connect to Bluetooth via RFCOMM using pybluez
def connect_bluetooth_rfcomm(mac_address):
    try:
        # Create a Bluetooth socket
        sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        logInfo(f"[Bluetooth] Connecting to Bluetooth device at {mac_address} via RFCOMM...")

        # Connect to the Bluetooth device on RFCOMM channel 1 (this might vary)
        sock.connect((mac_address, 1))  # Channel 1 is commonly used for serial communication

        logInfo(f"[Bluetooth] Bluetooth RFCOMM connection to {mac_address} established.")
        return sock  # Return the socket to use it for serial communication

    except bluetooth.BluetoothError as e:
        logInfo(f"[Bluetooth] Failed to connect to Bluetooth device at {mac_address}: \n")
        logDebug(f"{e}\n");
        return None
    
# Attempt to connect via Bluetooth RFCOMM
bluetooth_socket = connect_bluetooth_rfcomm(bluetooth_mac)

# If Bluetooth connection fails, fall back to the serial port
if bluetooth_socket is None:
    logInfo(f"[Serial] Note: Falling back to serial port {serial_port}.\n")

    # Set up the serial connection with error handling
    try:
        ser = serial.Serial(serial_port, baud_rate, timeout=1)
        serBt = 0
    except serial.serialutil.SerialException as e:
        logInfo(f"[Serial] Error: Could not open port {serial_port} with baudrate {baud_rate}. Check your configuration: \n")
        logDebug(f"{e}\n");
        exit()
else:
    # If Bluetooth RFCOMM is successful, use the socket as the serial communication link
    ser = bluetooth_socket.makefile('r')  # Use the socket like a file for reading
    serBt = 1

def read_camera_data():
    if serBt == 1:
        line = ser.readline().strip()
    else:
        line = ser.readline().decode("utf-8").strip()

    logDebug(f"Raw data received: {line}")

    # Initialize data array
    data = []

    # Use try-except to handle all operations that might fail
    try:
        # Extract numbers and validate length in one go
        numbers = re.findall(r'\d+', line)
        if len(numbers) == 129:  # Check if we have exactly 129 numbers
            data = list(map(int, numbers))  # Convert to integers

            if data[0] == 9999:  # Check if the first number is the flag
                logDebug(f"Valid data received: {data[1:]}")
                return data[1:]  # Exclude the first flag value

    except ValueError:
        logDebug("ValueError: Unable to convert received data to integers.")

    logDebug("Data is invalid or does not match expected format.")
    return []  # Return an empty list if conditions are not met


def cleanup(event=None):
    logInfo("Disconnecting from Bluetooth...")
    if bluetooth_socket:
        bluetooth_socket.close()  # Close Bluetooth socket if it exists
    logInfo("Bye <3")
    exit()  # Exit the program

# Initial plot setup
fig, ax = plt.subplots(figsize=(6, 4))  # Adjust size (width, height) to make it smaller
ax.set_title("Camera", color='white')
ax.set_xlabel("Pixel", color='white')
ax.set_ylabel("Value", color='white')
ax.set_xlim(0, 127)  # 128 pixels (0-127)
ax.set_ylim(0, 35000)  # Adjust based on your data range
ax.set_facecolor('black')
fig.patch.set_facecolor('black')

# Hook into the window close event
fig.canvas.mpl_connect('close_event', cleanup)

# Add vertical guidelines
ax.axvline(x=15, color='red', linestyle='--')
ax.axvline(x=104, color='red', linestyle='--')
ax.axvline(x=59, color='red', linestyle='--')
ax.axvline(x=60, color='red', linestyle='--')

# Set dark theme for ticks
ax.tick_params(colors='white')

# Disable minor grid lines and keep only major grid (simplified for speed)
ax.set_xticks(np.arange(0, 128, 5))
ax.grid(True, which='major', color='gray', linestyle='-', linewidth=0.5)

# Initialize the plot with an empty line object
line, = ax.plot([], [], linestyle='-', color='cyan')  # Line without markers for performance

def init():
    # Initialize the line object for blitting
    line.set_data([], [])
    return line,

def update(frame):
    camera_data = read_camera_data()
    if len(camera_data) == 128:  # Ensure we have 128 data points
        line.set_data(np.arange(128), camera_data)  # Set x data as range(128)
    return line,

# Use FuncAnimation for real-time updates with a limit on cached frames (save_count=100)
ani = animation.FuncAnimation(fig, update, init_func=init, interval=25, blit=True, save_count=100)

# Keep the plot window open and interactive
# plt.ion()
plt.show()


try:
    while True:
        plt.pause(0.0001)  # Allow the plot to update with a small pause
except KeyboardInterrupt:
    cleanup(None)  # Call cleanup on keyboard interrupt
finally:
    cleanup(None)  # Call cleanup on keyboard interrupt