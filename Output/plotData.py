import pandas as pd
import matplotlib.pyplot as plt

# Lade die Daten aus einer CSV-Datei
data = pd.read_csv('batteriedaten.csv', delimiter=',')
data['Zeit'] = pd.to_datetime(data['Zeit'], format='%H:%M:%S')

# Erstelle das erste Diagramm (Zeit vs Kapazität)
plt.figure(figsize=(12, 6))

# Diagramm für Kapazität
plt.subplot(1, 2, 1)
plt.plot(data['Zeit'], data['Kapazität (mAh)'], marker='o', color='b')
plt.xlabel('Zeit')
plt.ylabel('Kapazität (mAh)')
plt.title('Zeit vs Kapazität')

# Diagramm für Spannung
plt.subplot(1, 2, 2)
plt.plot(data['Zeit'], data['Spannung (V)'], marker='o', color='r')
plt.xlabel('Zeit')
plt.ylabel('Spannung (V)')
plt.title('Zeit vs Spannung')

# Zeige die Diagramme
plt.tight_layout()
plt.show()
