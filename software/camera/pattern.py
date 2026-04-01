import pandas as pd
import matplotlib.pyplot as plt

def getAverageBrightness(frame):
    return sum(frame) / len(frame)

def quantizeFrame(frame, threshold):
    return [100 if value >= threshold else 0 for value in frame]

def cutEdges(frame):
    return frame[28:88]

def printAsCArray(frame):
    print("static const uint32_t pattern[60] = {", end="")
    for i, value in enumerate(frame):
        print(f"{value}, ", end="")
        if (i + 1) % 16 == 0:
            print()
    print("};")

# Read csv fileframe
data = pd.read_csv('pelny_przejazd_5000_ledy_on_pattern_wykryty_zatrzymanie_7.5cm_przed_klocek_30_cm_za_zakretem.csv')
frame = data.iloc[461][0:128].tolist()
quantized_frame = quantizeFrame(frame, 80)
cut_edges_frame = cutEdges(quantized_frame)
printAsCArray(cut_edges_frame)

plt.plot(frame)
plt.plot(cut_edges_frame)
plt.title('Sensor Pattern')
plt.xlabel('Sensor Index')
plt.ylabel('Sensor Value')
plt.grid()
plt.show()