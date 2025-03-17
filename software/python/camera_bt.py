import pyqtgraph as pg
import numpy as np
from pyqtgraph.Qt import QtCore
import serial
import bluetooth

# Serial configuration
BAUD_RATE = 921600  
BT_MAC = "00:21:13:00:1F:26"

def connect_bluetooth_rfcomm(mac_address):
    try:
        # Create a Bluetooth socket
        sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        print(f"[Bluetooth] Connecting to Bluetooth device at {mac_address} via RFCOMM...")

        # Connect to the Bluetooth device on RFCOMM channel 1 (this might vary)
        sock.connect((mac_address, 1))  # Channel 1 is commonly used for serial communication

        print(f"[Bluetooth] Bluetooth RFCOMM connection to {mac_address} established.")
        return sock  # Return the socket to use it for serial communication

    except bluetooth.BluetoothError as e:
        print(f"[Bluetooth] Failed to connect to Bluetooth device at {mac_address}: \n")
        print(f"{e}\n");
        return None

bluetooth_socket = connect_bluetooth_rfcomm(BT_MAC)
serial_port = bluetooth_socket.makefile('r')  # Use the socket like a file for reading

graphics_layout = pg.GraphicsLayoutWidget(show=True, title="Kitty")

plot0 = graphics_layout.addPlot(row=0, col=0, title="Wykres")
plot1 = graphics_layout.addPlot(row=1, col=0, title="Obraz")

graphics_layout.ci.layout.setRowStretchFactor(0, 4) 
graphics_layout.ci.layout.setRowStretchFactor(1, 1) 

graphics_layout.setBackground("#f1f1f1")

plot0.setLabel('left', 'Jasność')
plot0.setLabel('bottom', 'Piksel')
plot0.showGrid(x=True, y=True)

x = np.arange(128)

plot0.setYRange(0, 256)
plot0.setXRange(0, 128)

line0 = plot0.plot(pen='r')

img = np.ones((128, 1))  
img_item = pg.ImageItem(img) 
plot1.addItem(img_item)  
plot1.getAxis('left').setVisible(False)

data = [0] * 128

def read_serial_data(serial_port):
    """Reads data from UART and returns a list of 128 values."""
    try:
            raw_line = serial_port.readline()
            line = raw_line.strip()
            if line.startswith("CAML"):
                data_str = line[6:]  # Remove "CAML."
                data_str = data_str.replace("\x00", "")
                values = list(map(int, data_str.split('.')))
                if len(values) == 128:
                    return values
    except ValueError:
        print("⚠ Error parsing data:", line)
    return None

def update_plot():
    global data
    data = read_serial_data(serial_port)

    if data:
        line0.setData(x, data)
        
        img[:, 0] = data
        img_item.setImage(img)

timer = QtCore.QTimer()
timer.timeout.connect(update_plot)
timer.start(0)  

# Start the Qt event loop
pg.QtCore.QCoreApplication.instance().exec()

serial_port.close()

