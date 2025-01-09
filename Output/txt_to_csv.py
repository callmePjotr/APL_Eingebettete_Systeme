import re
import csv

# Dateiname der Textdatei
datei_name = "data.txt"
# CSV-Dateiname
csv_datei = "batteriedaten.csv"

# Muster für jede Zeile
muster_kapazitaet = re.compile(r"Aktuelle Kapazität: ([\d.]+) mAh")
muster_strom = re.compile(r"Spannung: ([\-\d.]+) V")
muster_zeit = re.compile(r"Laufzeit: ([\d:]+)")

# Daten für die CSV
csv_daten = [["Zeit", "Kapazität (mAh)", "Spannung (V)"]]

# Variablen zum Zwischenspeichern der aktuellen Werte
aktuelle_kapazitaet = None
aktuelle_strom = None
aktuelle_zeit = None

# Datei zeilenweise einlesen und Daten extrahieren
with open(datei_name, "r", encoding="utf-8") as f:
    for line in f:
        if kapazitaet_match := muster_kapazitaet.match(line):
            aktuelle_kapazitaet = float(kapazitaet_match.group(1))
        elif strom_match := muster_strom.match(line):
            aktuelle_strom = float(strom_match.group(1))
        elif zeit_match := muster_zeit.match(line):
            aktuelle_zeit = zeit_match.group(1)

            # Wenn alle Werte vorhanden sind, in die CSV-Daten hinzufügen
            if aktuelle_kapazitaet is not None and aktuelle_strom is not None and aktuelle_zeit is not None:
                csv_daten.append([aktuelle_zeit, aktuelle_kapazitaet, aktuelle_strom])

# CSV-Datei schreiben
with open(csv_datei, mode="w", newline="", encoding="utf-8") as file:
    writer = csv.writer(file)
    writer.writerows(csv_daten)

print(f"Die Datei {csv_datei} wurde erfolgreich erstellt.")
