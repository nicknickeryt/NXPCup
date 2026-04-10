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
FRAME_SIZE = 168
# ==========================================

# ser = serial.Serial(PORT, BAUD, timeout=0)

rpmLeft = 0
rpmRight = 0

logging_enabled = False
log_file = None
log_writer = None

# 🔥 GLOBALNE
run_start_time = None
run_end_time   = None
run_finished   = False
elapsed = 0
patterns = 0

max_rpm_record = 0

servoDivider = 0.0
algorithmFilterAlpha = 0.0
patternThreshold = 0.0
pidKp = 0.0
pidKi = 0.0

brakeAll = 0
brakeOne = 0
brakeClamp = 0
cornerOutsideRPM = 0
cornerInsideRPM = 0

last_time = time.time()
frame_count = 0
fps = 0

rpm_history_L = np.zeros(128)
rpm_history_R = np.zeros(128)

power_history_L = np.zeros(128)
power_history_R = np.zeros(128)

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
    
    global run_start_time, run_end_time, run_finished, elapsed
    elapsed = 0
    run_start_time = None
    run_end_time   = None
    run_finished   = False

def add_log(text):
    timestamp = time.strftime("%H:%M:%S")
    item = f"[{timestamp}] {text}"

    log_list.insertItem(0, item)  # NA GÓRĘ

    # limit pamięci
    if log_list.count() > 200:
        log_list.takeItem(log_list.count() - 1)

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
        font-size: 14px;
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

pattern_label = QtWidgets.QLabel("Pattern: UNKNOWN")
pattern_label.setAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)

pattern_label.setStyleSheet("""
    QLabel {
        font-size: 20px;
        font-weight: bold;
        color: white;
        background-color: #7f8c8d;
        border-radius: 10px;
        padding: 10px;
    }
""")


top_row = QtWidgets.QHBoxLayout()
top_row.addWidget(rpm_record_label)
top_row.addWidget(pattern_label)
main_layout.addLayout(top_row)


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
history_plot = win.addPlot(title="Obraz Pseudo-2D", colspan=1)
history_plot.setMouseEnabled(False, False)
history_plot.hideAxis('left')
history_plot.hideAxis('bottom')
history_plot.getViewBox().setAspectLocked(True)

# 👉 przejdź do prawej kolumny
win.nextCol()

# RPM plot (wąski)
rpm_plot = win.addPlot(title="RPM", colspan=1)
rpm_plot.setYRange(0, 6000)
rpm_plot.setMouseEnabled(False, False)

# Czarna, przerywana linia dla target RPM
target_rpm_curve = rpm_plot.plot(pen=pg.mkPen(color='k', style=QtCore.Qt.PenStyle.DashLine, width=2))

rpm_curve_L = rpm_plot.plot(pen=pg.mkPen('r', width=2))
rpm_curve_R = rpm_plot.plot(pen=pg.mkPen('b', width=2))


power_plot = win.addPlot(title="Moc", colspan=1)
power_plot.setYRange(-1.0, 2.0)
power_plot.setMouseEnabled(False, False)

power_curve_L = power_plot.plot(pen=pg.mkPen('r', width=2))
power_curve_R = power_plot.plot(pen=pg.mkPen('b', width=2))

rpm_plot.invertX(True)
power_plot.invertX(True)

win.ci.layout.setColumnStretchFactor(0, 6)
win.ci.layout.setColumnStretchFactor(1, 2)
win.ci.layout.setColumnStretchFactor(2, 6)
win.ci.layout.setColumnStretchFactor(3, 6)

history_img = pg.ImageItem()
history_plot.addItem(history_img)

lut = np.array([[i, i, i] for i in range(256)], dtype=np.uint8)
history_img.setLookupTable(lut)
history_img.setLevels((0, 220))
history_img.setAutoDownsample(False)
history_img.setRect(QtCore.QRectF(0, 0, 128, 128))


img_plot.getViewBox().setDefaultPadding(0)
history_plot.getViewBox().setDefaultPadding(0)



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

btn_layout = QtWidgets.QGridLayout()

btn_widget = QtWidgets.QWidget()
btn_widget.setLayout(btn_layout)

proxy = QtWidgets.QGraphicsProxyWidget()
proxy.setWidget(btn_widget)
win.addItem(proxy)
win.ci.layout.setColumnStretchFactor(1, 1)  # buttony (mała kolumna)

win.ci.layout.setAlignment(proxy, QtCore.Qt.AlignmentFlag.AlignRight)

log_list = QtWidgets.QListWidget()
log_list.setMaximumWidth(350)
log_list.setMinimumHeight(300)

log_list.setStyleSheet("""
QListWidget {
    background-color: #111;
    color: #0f0;
    font-family: Consolas;
    font-size: 12px;
    border-radius: 8px;
}
""")

win.addItem(QtWidgets.QGraphicsProxyWidget())
proxy_log = QtWidgets.QGraphicsProxyWidget()
proxy_log.setWidget(log_list)
win.addItem(proxy_log)

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
#make_btn("Pauza", "p", "#4f4f4f", 0, 1)

#make_btn("Wznów", "o", "#4f4f4f", 1, 0)
make_action_btn("Uruchom", start_all, "#27ae60", 0, 1)

make_btn("StartRPM+", "+", "#3498db", 2, 0)
make_btn("StartRPM-", "-", "#2980b9", 2, 1)

make_btn("brakeOne+", "a", "#9b59b6", 3, 0)
make_btn("BrakeOne-", "b", "#8e44ad", 3, 1)

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
    
    global run_start_time, run_end_time, run_finished

    global rpmLeft, rpmRight
        
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

        # 🔹 PIERWSZA RAMKA po uruchomieniu, ale tylko jeśli oba enkodery > 0
        if run_start_time is None and rpmLeft > 0 and rpmRight > 0:
            run_start_time = time.time()
            print(f"[TIME] Start pomiaru czasu: {run_start_time:.3f}s")

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
        global np, patterns
        # 🔥 NORMALIZACJA
        rpm_norm_L = min(rpmLeft, 6000)
        rpm_norm_R = min(rpmRight, 6000)    
        
        # Wypełnij całą długość historii target RPM
        target_line = np.full(128, startRPM)
        target_rpm_curve.setData(target_line)

        power_L = max(0.0, min(1.0, diffLeft))
        power_R = max(0.0, min(1.0, diffRight))

        # 🔥 przesuwanie historii
        rpm_history_L[1:] = rpm_history_L[:-1]
        rpm_history_R[1:] = rpm_history_R[:-1]

        power_history_L[1:] = power_history_L[:-1]
        power_history_R[1:] = power_history_R[:-1]

        rpm_history_L[0] = rpm_norm_L
        rpm_history_R[0] = rpm_norm_R

        power_history_L[0] = power_L
        power_history_R[0] = power_R

        buffer = buffer[start_idx + FRAME_SIZE:]
        
        menuActive = frame[146]  # 1 = STOPPED, 0 = RUNNING
        
        isPatternDetected = frame[147]
        if isPatternDetected: patterns = patterns + 1
        
        servoDivider = (frame[148] << 8) | frame[149]
        algorithmFilterAlpha = (frame[150] << 8) | frame[151]
        patternThreshold = (frame[152] << 8) | frame[153]
        pidKp = (frame[154] << 8) | frame[155]
        pidKi = (frame[156] << 8) | frame[157]
        
        brakeAll = (frame[158] << 8) | frame[159]
        brakeOne = (frame[160] << 8) | frame[161]
        brakeClamp = (frame[162] << 8) | frame[163]
        cornerOutsideRPM = (frame[164] << 8) | frame[165]
        cornerInsideRPM = (frame[166] << 8) | frame[167]
        
        servoDivider = servoDivider / 100.0
        algorithmFilterAlpha = algorithmFilterAlpha / 1000.0
        patternThreshold = patternThreshold / 1000.0
        pidKp = pidKp / 1000.0
        pidKi = pidKi / 100000.0
        
       
        brakeAll /= 1000
        brakeOne /= 1000
        brakeClamp /= 1000
        
        # 🔹 KONIEC - startRPM = 800
        elapsed_temp = time.time() - run_start_time if run_start_time else 0
        if isPatternDetected == True and elapsed_temp > 1.0:
            run_end_time = time.time()
            run_finished = True
            elapsed = run_end_time - run_start_time if run_start_time else 0
            print(f"[TIME] Koniec trasy: {run_end_time:.3f}s | Czas trwania: {elapsed:.3f}s (liczone od ruszenia autaka do patternu, a potem miedzy kolejnymi patternami)")
            add_log(f"Czas: {elapsed:.3f}s")  
            
            # resecik i od nowa liczymy
            elapsed = 0
            run_start_time = None
            run_end_time   = None
            run_finished   = False
            
            run_start_time = time.time()
            print(f"[TIME] Start pomiaru czasu: {run_start_time:.3f}s")
        
        global max_rpm_record

        current_max = max(rpmLeft, rpmRight)
        if current_max <= 15000 and current_max > max_rpm_record:
            max_rpm_record = current_max
            rpm_record_label.setText(f"Rekord RPM: {max_rpm_record}")
        if isPatternDetected == 1:
            pattern_label.setText("Pattern: wykryty")
            pattern_label.setStyleSheet("""
                QLabel {
                    font-size: 20px;
                    font-weight: bold;
                    color: white;
                    background-color: #e67e22;
                    border-radius: 10px;
                    padding: 10px;
                }
            """)
        else:
            pattern_label.setText("Pattern: brak")
            pattern_label.setStyleSheet("""
                QLabel {
                    font-size: 20px;
                    font-weight: bold;
                    color: white;
                    background-color: #7f8c8d;
                    border-radius: 10px;
                    padding: 10px;
                }
            """)

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
        
        rpm_curve_L.setData(rpm_history_L)
        rpm_curve_R.setData(rpm_history_R)

        power_curve_L.setData(power_history_L)
        power_curve_R.setData(power_history_R)

        # 🔥 wyświetlenie
        history_img.setImage(np.rot90(history, k=1), autoLevels=False)

        
        text = f"""
        <b>RPM:</b> L {rpmLeft} | R {rpmRight} | <b>StartRPM:</b> <u>{startRPM}</u><br>
        <b>Silniki:</b> L {diffLeft:.2f} | R {diffRight:.2f}<br>
        <b>Odl:</b> {sr04} mm | <b>Odśw.:</b> {fps:.1f} Hz<br>
        <b>Pattern:</b> {patterns} | <b>Threshold :</b> {patternThreshold:.2f}<br>
        <b>Servo divider:</b> {servoDivider} | <b>Filter:</b> {algorithmFilterAlpha:.2f}<br>
        <b>PID Kp:</b> {pidKp:.3f} | <b>PID Ki:</b> {pidKi:.4f}<br>
        <b>Brake all:</b> {brakeAll:.1f} | <b>Brake one:</b> {brakeOne:.1f}<br>
        <b>Brake clamp:</b> {brakeClamp:.1f}<br>
        <b>Outside RPM:</b> {cornerOutsideRPM} | <b>Inside RPM:</b> {cornerInsideRPM}<br>
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
