import pyqtgraph as pg
import numpy as np
from pyqtgraph.Qt import QtCore
from scipy.ndimage import convolve1d
import serial
import bluetooth
import csv
import os

from PySide6.QtWidgets import QApplication, QPushButton, QWidget, QHBoxLayout, QLabel
from pyqtgraph.Qt import QtWidgets
from PySide6.QtGui import QFont

# Serial configuration
BAUD_RATE = 921600  
BT_MAC = "00:21:13:00:1F:26"

isRecording = False
is_running = False 
routeFile = "route.csv" 

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

# Car run/stop
def on_stop_start_click():
    global is_running
    if is_running:
        write_serial_data(serial_port, "s")  # Wysyła "s" (STOP)
        button_stop_start.setText("START")
        is_running = False
    else:
        write_serial_data(serial_port, "r")  # Wysyła "r" (RUN)
        button_stop_start.setText("STOP")
        is_running = True

# Car startVelocity
def on_plus_click():
    write_serial_data(serial_port, "+")

def on_minus_click():
    write_serial_data(serial_port, "-")
    
# Car startVelocity
def on_dplus_click():
    write_serial_data(serial_port, "a")

def on_dminus_click():
    write_serial_data(serial_port, "b")

# Recording start/stop
def on_start_stop_click():
    global isRecording, button_start_stop
    if(not isRecording):
        print("[Record] Started")
        isRecording = True
        
        if os.path.exists(routeFile):
            os.remove(routeFile)

        # Utwórz nowy plik i zapisz nagłówki
        with open(routeFile, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            header_pixels = [f'p{i}' for i in range(128)]
            header_brightness = "brightness"
            header_servo = "servo"
            header_servo_raw = "servo_raw"
            writer.writerow(list(header_pixels) + [header_brightness, header_servo, header_servo_raw])  # Combine headers for pixels and brightness
        
        button_start_stop.setText("Zatrzymaj zapis")
    else:
        isRecording = False
        print("[Record] Stopped")
        button_start_stop.setText("Rozpocznij zapis")

    
app = QApplication([])

bluetooth_socket = connect_bluetooth_rfcomm(BT_MAC)
serial_port = bluetooth_socket.makefile('r')  # Use the socket like a file for reading

graphics_layout = pg.GraphicsLayoutWidget(show=True, title="Kitty")

button_start_stop = QPushButton("Rozpocznij zapis")

# Car control buttons
button_start_stop.clicked.connect(on_start_stop_click)

button_stop_start = QPushButton("STOP")
button_plus = QPushButton("SV+")
button_minus = QPushButton("SV-")
button_dplus = QPushButton("DR+")
button_dminus = QPushButton("DR-")

control_layout = QHBoxLayout()
control_layout.addWidget(button_start_stop)
control_layout.addWidget(button_plus)
control_layout.addWidget(button_minus)
control_layout.addWidget(button_dplus)
control_layout.addWidget(button_dminus)

control_widget = QWidget()
control_widget.setLayout(control_layout)

controlProxy = QtWidgets.QGraphicsProxyWidget()
controlProxy.setWidget(control_widget)

button_stop_start.clicked.connect(on_stop_start_click)
button_plus.clicked.connect(on_plus_click)
button_minus.clicked.connect(on_minus_click)
button_dplus.clicked.connect(on_dplus_click)
button_dminus.clicked.connect(on_dminus_click)

button_widget = QWidget()
button_layout = QHBoxLayout()
button_layout.addWidget(button_stop_start)
button_widget.setLayout(button_layout)
button_widget.setStyleSheet("background-color: #f1f1f1; padding: 10px;")

buttonProxy = QtWidgets.QGraphicsProxyWidget()
buttonProxy.setWidget(button_widget)

############
# ENCODERS #
############

encodersWidget = QWidget()
distanceDiffWidget = QWidget()
encodersLayout = QHBoxLayout()
distanceDiffLayout = QHBoxLayout()

font = QFont("Arial", 14)  # Arial, rozmiar 20

left_label = QLabel("0")
right_label = QLabel("0")
startVelocityLabel = QLabel("0")
diffRatioLabel = QLabel("170?")

klzDistanceMMLabel = QLabel("255?")
diffLeftLabel = QLabel("70?")
diffRightLabel = QLabel("70?")

left_label.setFont(font)
right_label.setFont(font)
startVelocityLabel.setFont(font)
diffRatioLabel.setFont(font)

klzDistanceMMLabel.setFont(font)
diffLeftLabel.setFont(font)
diffRightLabel.setFont(font)

encodersLayout.addWidget(left_label)
encodersLayout.addStretch()  
encodersLayout.addWidget(startVelocityLabel)
encodersLayout.addStretch()
encodersLayout.addWidget(diffRatioLabel)
encodersLayout.addStretch()
encodersLayout.addWidget(right_label)

distanceDiffLayout.addWidget(klzDistanceMMLabel)
distanceDiffLayout.addStretch()  
distanceDiffLayout.addWidget(diffLeftLabel)
distanceDiffLayout.addStretch()  
distanceDiffLayout.addWidget(diffRightLabel)

encodersWidget.setLayout(encodersLayout)
encodersWidget.setStyleSheet("background-color: #f1f1f1; padding: 10px;")

distanceDiffWidget.setLayout(distanceDiffLayout)
distanceDiffWidget.setStyleSheet("background-color: #f1f1f1; padding: 10px;")

encodersProxy = QtWidgets.QGraphicsProxyWidget()
encodersProxy.setWidget(encodersWidget)

distanceDiffProxy = QtWidgets.QGraphicsProxyWidget()
distanceDiffProxy.setWidget(distanceDiffWidget)


# Graphics layout
plot0 = graphics_layout.addPlot(row=0, col=0, title="Wykres")
plot1 = graphics_layout.addPlot(row=2, col=0, title="Obraz")
graphics_layout.addItem(encodersProxy, row=3, col=0) 
graphics_layout.addItem(distanceDiffProxy, row=4, col=0) 
graphics_layout.addItem(buttonProxy, row=5, col=0) 
graphics_layout.addItem(controlProxy, row=6, col=0)

# Stretch
graphics_layout.ci.layout.setRowStretchFactor(0, 2) 
graphics_layout.ci.layout.setRowStretchFactor(1, 0) 
graphics_layout.ci.layout.setRowStretchFactor(2, 3) 
graphics_layout.ci.layout.setRowStretchFactor(3, 0) 
graphics_layout.ci.layout.setRowStretchFactor(4, 0) 
graphics_layout.ci.layout.setRowStretchFactor(5, 0) 
graphics_layout.ci.layout.setRowStretchFactor(6, 0) 

graphics_layout.setBackground("#f1f1f1")

plot0.setLabel('left', 'Jasność')
plot0.setLabel('bottom', 'Piksel')
plot0.showGrid(x=True, y=True)

x = np.arange(128)

plot0.setYRange(0, 256)
plot0.setXRange(0, 128)

line0 = plot0.plot(pen='r')

img = np.ones((128, 10))  
img_item = pg.ImageItem(img) 
plot1.addItem(img_item)  
plot1.getAxis('left').setVisible(False)

servo_line = pg.InfiniteLine(pos=0, angle=90, pen='g')  # Linia pionowa na wykresie
plot0.addItem(servo_line)  # Dodaj linię do wykresu

brightness_line = pg.InfiniteLine(pos=0, angle=0, pen='b')  # Linia pionowa na wykresie
plot0.addItem(brightness_line)  # Dodaj linię do wykresu

servo_raw_line = pg.InfiniteLine(pos=0, angle=90, pen='y')  # Linia pionowa na wykresie
plot0.addItem(servo_raw_line)  # Dodaj linię do wykresu

data = [0] * 128    
servo_position = None 
brightness = None
raw_servo = None
encoderLeftRPM = 0
encoderRightRPM = 0
startVelocity = 0
diffRatio = 0

klzDistanceMM = 0
diffLeft = 0
diffRight = 0

last_ten_data = [data, data, data, data, data, data, data, data, data, data]

def read_serial_data(serial_port):
    """Reads data from UART and returns a list of 128 values."""
    global servo_position, brightness, raw_servo, encoderLeftRPM, encoderRightRPM, startVelocityLabel, klzDistanceMM, diffLeft, diffRight
    try:
        # Próba odczytu z portu szeregowego
        raw_line = serial_port.readline()
        line = raw_line.strip()
        
        # print(line)
        
        #line = bluetooth_socket.recv(1000000).decode('utf-8').strip()
        
        if line.startswith("CAML"):
            data_str = line[6:]  # Remove "CAML."
            data_str = data_str.replace("\x00", "")
            values = list(map(int, data_str.split('.')))
            
            if len(values) == 135:
                servo_position = values[128]
                brightness = values[129]  # Przyjmujemy, że brightness jest podzielony przez 2
                encoderLeftRPM = values[130]
                encoderRightRPM = values[131]
                
                klzDistanceMM = values[132]
                diffLeft = int(values[133]) - 100
                diffRight = int(values[134]) - 100
                
                
                return values[:128]
        elif line.startswith("kittySV"):  
            try:
                startVelocity = int(line[7:]) 
                startVelocityLabel.setText("SV: " + str(startVelocity * 100))
            except ValueError:
                print(f"⚠ Błąd parsowania startVelocity: {line}")
        elif line.startswith("kittyDR"):  
            try:
                diffRatio = int(line[7:]) 
                diffRatioLabel.setText("DR: " + str(diffRatio))
            except ValueError:
                print(f"⚠ Błąd parsowania startVelocity: {line}")
            
    
    except ValueError:
        print("⚠ Error parsing data:", line)
    except serial.SerialTimeoutException:
        # Obsługa TimeoutError: ignorujemy błąd i kontynuujemy próbę
        print("⚠ Timeout: Czekam na dane...")
        time.sleep(0.1)  # Krótkie opóźnienie przed ponowną próbą
    except Exception as e:
        # Obsługuje inne wyjątki, które mogą wystąpić
        print(f"⚠ Error: {e}")

    return None

def write_serial_data(serial_port, data_str):
    try:
        bluetooth_socket.send(data_str)
    except Exception as e:
        print(f"⚠ Error writing data: {e}")

def update_plot():
    global data, servo_position, last_ten_data, encoderLeftRPM, encoderRightRPM, left_label, right_label
    
    data = read_serial_data(serial_port)

    if data:
        line0.setData(x, data)
        
        img[:, 0] = last_ten_data[0]
        img[:, 1] = last_ten_data[1]
        img[:, 2] = last_ten_data[2]
        img[:, 3] = last_ten_data[3]
        img[:, 4] = last_ten_data[4]
        img[:, 5] = last_ten_data[5]
        img[:, 6] = last_ten_data[6]
        img[:, 7] = last_ten_data[7]
        img[:, 8] = last_ten_data[8]
        img[:, 9] = last_ten_data[9]
        img_item.setImage(img)
        
        if servo_position is not None:
            servo_line.setPos(servo_position)  # Aktualizacja pozycji linii
        
        if raw_servo is not None:
            servo_raw_line.setPos(raw_servo + 63)  # Aktualizacja pozycji linii
        
        if brightness is not None:
            brightness_line.setPos(brightness)  # Aktualizacja pozycji linii
                
        left_label.setText(str(encoderLeftRPM) + "RPM")
        right_label.setText(str(encoderRightRPM) + "RPM")
        
        klzDistanceMMLabel.setText(str(klzDistanceMM) + "mm")
        diffLeftLabel.setText(str(diffLeft))
        diffRightLabel.setText(str(diffRight) )
        
        # Dodaj nowe dane do listy i usuń najstarsze, jeśli jest ich więcej niż 5
        last_ten_data.append(data)
        if len(last_ten_data) > 10:
            last_ten_data.pop(0)

        # Zapisz dane do pliku, jeśli trwa nagrywanie
        if isRecording:
            with open(routeFile, 'a', newline='') as csvfile:
                writer = csv.writer(csvfile)
                row_data = list(data) + [brightness, servo_position, raw_servo]
                writer.writerow(row_data)

timer = QtCore.QTimer()
timer.timeout.connect(update_plot)
timer.start(0)  

# Start the Qt event loop
#pg.QtCore.QCoreApplication.instance().exec()

app.exec()

serial_port.close()

