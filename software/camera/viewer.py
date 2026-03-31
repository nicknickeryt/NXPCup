import os
os.environ["QT_QPA_PLATFORM"] = "xcb"

import sys
from PyQt5 import QtWidgets, QtCore

# ⚠️ NAJPIERW QApplication!
app = QtWidgets.QApplication(sys.argv)

# dopiero teraz reszta
import pandas as pd
import pyqtgraph as pg


class Viewer(QtWidgets.QMainWindow):
    def __init__(self, csv_file):
        super().__init__()

        self.setWindowTitle("LineScan Viewer")
        self.resize(1000, 600)

        df = pd.read_csv(csv_file)
        self.data = df[[f"cam_{i}" for i in range(128)]].values
        print(self.data)

        self.index = 0
        self.max_index = len(self.data) - 1

        self.plot_widget = pg.PlotWidget()
        self.setCentralWidget(self.plot_widget)

        self.curve = self.plot_widget.plot(pen=pg.mkPen(width=2))

        self.plot_widget.setYRange(0, 220)
        self.plot_widget.setXRange(0, 127)
        self.plot_widget.showGrid(x=True, y=True)

        self.update_plot()

    def update_plot(self):
        self.curve.setData(range(128), self.data[self.index])
        self.setWindowTitle(f"Frame: {self.index}/{self.max_index}")

    def keyPressEvent(self, event):
        if event.key() == QtCore.Qt.Key_Right:
            self.index = min(self.index + 1, self.max_index)
        elif event.key() == QtCore.Qt.Key_Left:
            self.index = max(self.index - 1, 0)
        elif event.key() == QtCore.Qt.Key_Up:
            self.index = min(self.index + 10, self.max_index)
        elif event.key() == QtCore.Qt.Key_Down:
            self.index = max(self.index - 10, 0)

        self.update_plot()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python viewer.py data.csv")
        sys.exit(1)

    viewer = Viewer(sys.argv[1])
    viewer.show()

    sys.exit(app.exec_())
