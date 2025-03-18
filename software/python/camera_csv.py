import pyqtgraph as pg
import numpy as np
from pyqtgraph.Qt import QtCore
import csv
import os

from PySide6.QtWidgets import QApplication, QPushButton, QWidget, QHBoxLayout
from pyqtgraph.Qt import QtWidgets

# Path to the CSV file
routeFile = "route.csv"

# Initialize some global variables
data = np.zeros(128)  # Initialize data array with 128 zeros
x = np.arange(128)
all_data = []  # List to store all rows of data

currentRow = 1

timer = QtCore.QTimer() # Plot update timer

isTimerRunning = False

def read_csv_data():
    """Reads the CSV file and returns the latest data line."""
    global data, all_data, currentRow
    try:
        with open(routeFile, 'r') as csvfile:
            reader = csv.reader(csvfile)
            rows = list(reader)
            if rows:
                if currentRow < len(rows):
                    latest_row = rows[currentRow]  # Get the most recent row
                    pixel_data = list(map(int, latest_row[:128]))  # p0, p1, ..., p127
                    brightness_data = list(map(int, latest_row[128:]))  # b0, b1, ..., b127
                    data = np.array(pixel_data)  # For this example, we use the pixel data
                    all_data.append(data)  # Append new data to all_data list
                    
                    currentRow = currentRow + 1
                else:
                    on_start_stop_click()
                    
    except Exception as e:
        print(f"Error reading CSV: {e}")
        data = np.zeros(128)  # If error occurs, reset data
        
    return data

def update_plot():
    global data
    data = read_csv_data()

    if data is not None:
        line0.setData(x, data)

        # Create image from all_data
        img = np.array(all_data)  # Convert list of data to a numpy array
        img_item.setImage(img.T)  # Transpose the image for correct orientation

def on_start_stop_click():
    global timer, isTimerRunning, button_start_stop
    if(not isTimerRunning):
        print("[Play] Started")
        timer.start(14)
        button_start_stop.setText("Pauza")
        isTimerRunning = True
    else:
        print("[Play] Stopped")
        timer.stop();
        button_start_stop.setText("Start")
        isTimerRunning = False
    
def on_route_click():
    global timer
    print("[Play] Write image started")
    timer.start() 
    
def on_reset_click():
    global timer, currentRow, plot1, all_data, line0, img, img_item
    print("[Play] Reset")
    currentRow = 1
    all_data = []
    line0.setData(x, [])
    img = np.array(all_data)
    img_item.clear()
    

# PyQt application setup
app = QApplication([])

graphics_layout = pg.GraphicsLayoutWidget(show=True, title="Kitty reader")

# Creating buttons for start and stop functionality (no functionality in this simple example)
button_start_stop = QPushButton("Start")
button_route = QPushButton("Trasa")
button_reset = QPushButton("Reset")

button_start_stop.clicked.connect(on_start_stop_click)
button_route.clicked.connect(on_route_click)
button_reset.clicked.connect(on_reset_click)

button_widget = QWidget()
button_layout = QHBoxLayout()
button_layout.addWidget(button_start_stop)
button_layout.addWidget(button_route)
button_layout.addWidget(button_reset)

button_widget.setLayout(button_layout)
button_widget.setStyleSheet("background-color: #f1f1f1; padding: 10px;")

# Create proxy for the button widget
proxy = QtWidgets.QGraphicsProxyWidget()
proxy.setWidget(button_widget)

graphics_layout.addItem(proxy, row=2, col=0)

# Plotting data
plot0 = graphics_layout.addPlot(row=0, col=0, title="Wykres")
plot1 = graphics_layout.addPlot(row=1, col=0, title="Obraz")

graphics_layout.ci.layout.setRowStretchFactor(0, 4)
graphics_layout.ci.layout.setRowStretchFactor(1, 3)
graphics_layout.ci.layout.setRowStretchFactor(2, 0)

graphics_layout.setBackground("#f1f1f1")

plot0.setLabel('left', 'Jasność')
plot0.setLabel('bottom', 'Piksel')
plot0.showGrid(x=True, y=True)

# Create a red line for the plot
line0 = plot0.plot(pen='r')

# Image for visualizing the data
img = np.ones((128, 1))  # Initial dummy image
img_item = pg.ImageItem(img)
plot1.addItem(img_item)
plot1.getAxis('left').setVisible(False)

# Timer to update the plot every 14ms
timer.timeout.connect(update_plot)

# Start the Qt event loop
app.exec()

