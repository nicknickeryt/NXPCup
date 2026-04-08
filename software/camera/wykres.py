import serial
import pyqtgraph as pg
from pyqtgraph.Qt import QtWidgets, QtCore
import bluetooth
import numpy as np
from pyqtgraph.Qt import QtGui
import csv
import time


# ================= CONFIG =================
BAUD_RATE = 460800  
BT_MAC = "98:D3:32:11:A4:34" # Kitty HC-06
# BT_MAC = "00:4B:12:3C:F5:DA"   # ESP32
# BT_MAC = "00:21:13:00:1F:26"      # NXP

START = b'\x00\xff\x00\xff'
FRAME_SIZE = 147
# ==========================================

# ser = serial.Serial(PORT, BAUD, timeout=0)

logging_enabled = False
log_file = None
log_writer = None

max_rpm_record = 0

last_time = time.time()
frame_count = 0
fps = 0

class MyWindow(pg.GraphicsLayoutWidget):
    def closeEvent(self, event):
        print("[App] Zamykam aplikację...")

        try:
            if sock:
                sock.close()
                print("[Bluetooth] Rozłączono")
        except Exception as e:
            print("[Bluetooth] Błąd przy zamykaniu:", e)

        event.accept()  # pozwól zamknąć okno
    def keyPressEvent(self, event):
        key = event.key()

        if key == QtCore.Qt.Key.Key_Space:
            print("[KEY] STOP")
            send_cmd("s")
            
        elif key == QtCore.Qt.Key.Key_Up:
            print("[KEY] FORWARD")
            send_cmd("1")

        elif key == QtCore.Qt.Key.Key_Down:
            print("[KEY] REVERSE")
            send_cmd("2")

        elif key == QtCore.Qt.Key.Key_Right:
            print("[KEY] RIGHT")
            send_cmd("3")

        elif key == QtCore.Qt.Key.Key_Left:
            print("[KEY] LEFT")
            send_cmd("4")
def send_cmd(cmd):
    try:
        if sock:
            sock.send(cmd.encode())
            print(f"[BT] Sent: {cmd}")
    except Exception as e:
        print("[BT] Send error:", e)
        
def reset_all():
    global max_rpm_record
    max_rpm_record = 0
    rpm_record_label.setText("Rekord RPM: 0")
    send_cmd("x")
    

def start_all():
    global max_rpm_record
    max_rpm_record = 0
    rpm_record_label.setText("Rekord RPM: 0")
    send_cmd("r")

def start_logging():
    global logging_enabled, log_file, log_writer

    filename = time.strftime("nxp_zapis_%Y-%m-%d_%H-%M-%S.csv")
    log_file = open(filename, "w", newline="")
    log_writer = csv.writer(log_file)

    header = []
    header += [f"cam_{i}" for i in range(128)]
    header += [
        "time","position","rpmLeft","rpmRight",
        "startRPM","sr04","diffLeft","diffRight","menuActive"
    ]
    log_writer.writerow(header)

    logging_enabled = True

    # 🔥 UI update
    log_status.setText("Zapis: włączony")
    log_status.setStyleSheet("""
        QLabel {
            font-size: 16px;
            font-weight: bold;
            color: white;
            background-color: #27ae60;
            border-radius: 8px;
            padding: 10px;
        }
    """)

    print(f"[LOG] START -> {filename}")


def stop_logging():
    global logging_enabled, log_file

    logging_enabled = False

    if log_file:
        log_file.close()
        log_file = None

    # 🔥 UI update
    log_status.setText("Zapis: wyłączony")
    log_status.setStyleSheet("""
        QLabel {
            font-size: 16px;
            font-weight: bold;
            color: white;
            background-color: #7f8c8d;
            border-radius: 8px;
            padding: 10px;
        }
    """)

    print("[LOG] STOP")

app = QtWidgets.QApplication([])

pg.setConfigOption('background', (245, 245, 245))  # bardzo jasny szary
pg.setConfigOption('foreground', 'k')
win = MyWindow(title="Camera Viewer")

main_widget = QtWidgets.QWidget()
main_layout = QtWidgets.QVBoxLayout(main_widget)
main_layout.setContentsMargins(5, 5, 5, 5)
main_layout.setSpacing(5)
main_layout.addWidget(win)

plot = win.addPlot(title="Wykres kamery", colspan=5)

win.nextCol()

proxy_info = QtWidgets.QGraphicsProxyWidget()


info_label = QtWidgets.QLabel()
info_label.setAlignment(QtCore.Qt.AlignmentFlag.AlignTop)

info_label.setStyleSheet("""
    QLabel {
        font-size: 16px;
        color: black;
        background-color: #ffffff;
        border: 1px solid #ccc;
        border-radius: 8px;
        padding: 10px;
        min-width: 200px;
    }
""")


proxy_info = QtWidgets.QGraphicsProxyWidget()
proxy_info.setWidget(info_label)
win.addItem(proxy_info)

status_label = QtWidgets.QLabel("Stan nieznany")
status_label.setAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)

status_label.setStyleSheet("""
    QLabel {
        font-size: 32px;
        font-weight: bold;
        color: white;
        background-color: gray;
        border-radius: 10px;
        padding: 15px;
    }
""")

main_layout.addWidget(status_label)


proxy_status = QtWidgets.QGraphicsProxyWidget()
proxy_status.setWidget(status_label)


rpm_record_label = QtWidgets.QLabel("Rekord RPM: 0")
rpm_record_label.setAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)

rpm_record_label.setStyleSheet("""
    QLabel {
        font-size: 20px;
        font-weight: bold;
        color: white;
        background-color: #34495e;
        border-radius: 10px;
        padding: 10px;
    }
""")

main_layout.addWidget(rpm_record_label)


win.nextRow()
win.addItem(proxy_status, colspan=1)

history = np.zeros((128, 128, 3), dtype=np.uint8)


win.nextRow()

img_plot = win.addPlot(title="Piksele kamery", colspan=7)
img_plot.setFixedHeight(120)
img_plot.setMouseEnabled(False, False)
img_plot.hideAxis('left')
img_plot.hideAxis('bottom')
img_plot.setFixedHeight(120)   # 🔥 dużo niższy pasek

win.nextRow()

# LEFT: Camera 2D
history_plot = win.addPlot(title="Obraz Pseudo-2D", colspan=5)
history_plot.setMouseEnabled(False, False)
history_plot.hideAxis('left')
history_plot.hideAxis('bottom')
history_plot.getViewBox().setAspectLocked(True)



history_img = pg.ImageItem()
history_plot.addItem(history_img)

lut = np.array([[i, i, i] for i in range(256)], dtype=np.uint8)
history_img.setLookupTable(lut)
history_img.setLevels((0, 220))
history_img.setAutoDownsample(False)
history_img.setRect(QtCore.QRectF(0, 0, 128, 128))


img_plot.getViewBox().setDefaultPadding(0)
history_plot.getViewBox().setDefaultPadding(0)


win.ci.layout.setColumnStretchFactor(0, 10)  # wykresy (lewa)
win.ci.layout.setColumnStretchFactor(1, 1)  # panel info (prawa)

bars = pg.BarGraphItem(x=list(range(128)), height=[1]*128, width=1, brushes=[(0,0,0)]*128, pen=None)
img_plot.addItem(bars)

pos_dot = pg.ScatterPlotItem(size=10, brush='r')
img_plot.addItem(pos_dot)

img_plot.setXRange(0, 128)
img_plot.setYRange(0, 1)

plot.setYRange(0, 220)
curve = plot.plot(pen='k')
pos_line = pg.InfiniteLine(angle=90, movable=False)
pos_line.setPen(pg.mkPen('r', width=2))  # czerwona linia
plot.addItem(pos_line)

main_widget.resize(1000, 700)
main_widget.show()

win.nextCol()   # 🔥 TO JEST KLUCZ
btn_layout = QtWidgets.QGridLayout()

btn_widget = QtWidgets.QWidget()
btn_widget.setLayout(btn_layout)

proxy = QtWidgets.QGraphicsProxyWidget()
proxy.setWidget(btn_widget)
win.addItem(proxy)
win.ci.layout.setColumnStretchFactor(1, 1)  # buttony (mała kolumna)

win.ci.layout.setAlignment(proxy, QtCore.Qt.AlignmentFlag.AlignRight)


def make_btn(text, cmd, color, row, col):
    btn = QtWidgets.QPushButton(text)

    btn.setMinimumHeight(60)
    btn.setMinimumWidth(80)

    btn.setStyleSheet(f"""
        QPushButton {{
            background-color: {color};
            color: white;
            font-size: 14px;
            font-weight: bold;
            border-radius: 8px;
        }}
        QPushButton:hover {{
            background-color: #4f4f4f;
        }}
        QPushButton:pressed {{
            background-color: #333333;
        }}
    """)

    btn.clicked.connect(lambda: send_cmd(cmd))
    btn_layout.addWidget(btn, row, col)
    return btn  
    

def make_action_btn(text, func, color, row, col):
    btn = QtWidgets.QPushButton(text)

    btn.setMinimumHeight(60)
    btn.setMinimumWidth(80)

    btn.setStyleSheet(f"""
        QPushButton {{
            background-color: {color};
            color: white;
            font-size: 14px;
            font-weight: bold;
            border-radius: 8px;
        }}
        QPushButton:hover {{
            background-color: #4f4f4f;
        }}
        QPushButton:pressed {{
            background-color: #333333;
        }}
    """)

    btn.clicked.connect(func)
    btn_layout.addWidget(btn, row, col)
    return btn

make_btn("STOP", "s", "#e74c3c", 0, 0)
make_btn("Pauza", "p", "#4f4f4f", 0, 1)

make_btn("Wznów", "o", "#4f4f4f", 1, 0)
make_action_btn("Uruchom", start_all, "#27ae60", 1, 1)

make_btn("Początkowe RPM+", "+", "#3498db", 2, 0)
make_btn("Początkowe RPM-", "-", "#2980b9", 2, 1)

make_btn("DiffRatio+", "a", "#9b59b6", 3, 0)
make_btn("DiffRatio-", "b", "#8e44ad", 3, 1)

make_action_btn("RESET", reset_all, "#f7c214", 4, 0)
# 🔥 LOGGING BUTTONS (takie same jak reszta)

# 🔥 STATUS OBOK PRZYCISKÓW
log_status = QtWidgets.QLabel("Zapis: wyłączony")
log_status.setAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)

log_status.setMinimumWidth(120)

log_status.setStyleSheet("""
    QLabel {
        font-size: 16px;
        font-weight: bold;
        color: white;
        background-color: #7f8c8d;
        border-radius: 8px;
        padding: 10px;
    }
""")

btn_layout.addWidget(log_status)



make_action_btn("Zapis start", start_logging, "#16a085", 5, 0)
make_action_btn("Zapis stop", stop_logging, "#c0392b", 5, 1)

btn_layout.addWidget(log_status, 6, 0, 1, 2)  # 🔥 span na 2 kolumny

buffer = bytearray()


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


def read_uart():
    global buffer
    
    global last_time, frame_count, fps
    frame_count += 1
    current_time = time.time()
    if current_time - last_time >= 1.0:  # co sekundę
        fps = frame_count / (current_time - last_time)
        frame_count = 0
        last_time = current_time

    # 🔥 czytaj wszystko co przyszło
    while True:
        try:
            data = sock.recv(4096)
            if not data:
                break
            buffer += data
        except Exception:
            break

    last_frame = None

    while True:
        start_idx = buffer.find(START)

        if start_idx == -1:
            break

        if len(buffer) < start_idx + FRAME_SIZE:
            break

        frame = buffer[start_idx:start_idx+FRAME_SIZE]

        last_frame = frame[4:132]  # kamera

        # 🔥 nowe pola
        position = frame[132] - 63

        rpmLeft  = (frame[134] << 8) | frame[135]
        rpmRight = (frame[136] << 8) | frame[137]

        startRPM = (frame[138] << 8) | frame[139]
        sr04     = (frame[140] << 8) | frame[141]

        diffLeft  = (frame[142] << 8) | frame[143]
        diffRight = (frame[144] << 8) | frame[145]

        # opcjonalnie powrót do -1..1
        diffLeft  = (diffLeft / 100.0) - 1
        diffRight = (diffRight / 100.0) - 1

        buffer = buffer[start_idx + FRAME_SIZE:]
        
        menuActive = frame[146]  # 1 = STOPPED, 0 = RUNNING
        
        global max_rpm_record

        current_max = max(rpmLeft, rpmRight)
        if current_max <= 15000 and current_max > max_rpm_record:
            max_rpm_record = current_max
            rpm_record_label.setText(f"Rekord RPM: {max_rpm_record}")

    # 🔥 rysuj tylko najnowsze
    if last_frame:
        curve.setData(last_frame)

        # 🔥 konwersja na obraz 1x128
        import numpy as np
        line = np.array(last_frame, dtype=np.uint8)
        line = line.reshape(1, -1)  # (1, 128)

        # zakres zabezpieczenia (ważne!)
        pos_clamped = max(0, min(127, position + 64))

        pos_line.setPos(pos_clamped)

        # 🔥 generuj kolory (0-220 -> czarny-biały)
        colors = []
        for i, v in enumerate(last_frame):
            v = min(v, 220)
            gray = int(v / 220 * 255)

            if i == pos_clamped:
                colors.append((255, 0, 0))  # 🔴 czerwony bar
            else:
                colors.append((gray, gray, gray))

        bars.setOpts(brushes=colors)
        
        
        # 🔥 update historii (scroll w dół)
        row = np.zeros((128, 3), dtype=np.uint8)
        for i, v in enumerate(last_frame):
            v = min(v, 220)
            gray = int(v / 220 * 255)

            if i == pos_clamped:
                row[i] = [255, 0, 0]  # 🔴 czerwony pixel
            else:
                row[i] = [gray, gray, gray]

        history[1:] = history[:-1]
        history[0] = row

        # 🔥 wyświetlenie
        history_img.setImage(np.rot90(history, k=1), autoLevels=False)

        
        text = f"""
        <b>RPM:</b> L {rpmLeft} | R {rpmRight}<br>
        <b>Początkowe RPM:</b> {startRPM}<br>
        <b>Silniki:</b> L {diffLeft:.2f} | R {diffRight:.2f}<br>
        <b>Odległość:</b> {sr04} mm<br>
        <b>Odśw. kamery:</b> {fps:.1f} Hz
        """

        info_label.setText(text)
        
        if menuActive == 1:
            status_label.setText("Zatrzymany")
            status_label.setStyleSheet("""
                QLabel {
                    font-size: 24px;
                    font-weight: bold;
                    color: white;
                    background-color: #e74c3c;  /* czerwony */
                    border-radius: 10px;
                    padding: 20px;
                }
            """)
        else:
            status_label.setText("Uruchomiony")
            status_label.setStyleSheet("""
                QLabel {
                    font-size: 24px;
                    font-weight: bold;
                    color: white;
                    background-color: #2ecc71;  /* zielony */
                    border-radius: 10px;
                    padding: 20px;
                }
            """)
            
        if logging_enabled and log_writer:
            row = []
            
            # 🔥 dodaj kamerę
            row += list(last_frame)
            
            row += [
                time.time(),
                position,
                rpmLeft,
                rpmRight,
                startRPM,
                sr04,
                diffLeft,
                diffRight,
                menuActive
            ]

            log_writer.writerow(row)
  
    # 🔥 anty-zapchanie
    if len(buffer) > 20000:
        buffer = buffer[-1000:]
        
bluetooth_socket = connect_bluetooth_rfcomm(BT_MAC)
sock = bluetooth_socket
sock.setblocking(False)

timer = QtCore.QTimer()
timer.timeout.connect(read_uart)
timer.start(1)  # ~150 fps

app.exec()
