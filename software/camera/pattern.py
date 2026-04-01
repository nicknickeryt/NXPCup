import pandas as pd
import matplotlib.pyplot as plt

def getAverageBrightness(frame):
    return sum(frame) / len(frame)

def quantizeFrame(frame, threshold):
    return [100 if value >= threshold else 0 for value in frame]

# Read csv file
data = pd.read_csv('pelny_przejazd_5000_ledy_on_pattern_wykryty_zatrzymanie_7.5cm_przed_klocek_30_cm_za_zakretem.csv')
frame = data.iloc[461][0:128].tolist()
quantized_frame = quantizeFrame(frame, 80)

plt.plot(frame)
plt.plot(quantized_frame)
plt.title('Sensor Pattern')
plt.xlabel('Sensor Index')
plt.ylabel('Sensor Value')
plt.grid()
plt.show()