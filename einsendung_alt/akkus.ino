#include <Wire.h>

#define MULTIPLEXER_ADDR 0x70  // Adresse des Multiplexers (TCA9548)
#define MAX17055_ADDR 0x36     // Adresse des Max17055
#define MAX17055_REG_SOC 0x06  // Register für den Ladezustand (SOC)
#define MAX17055_REG_VCELL 0x09 // Register für Zellenspannung (VCell)

const float vollKapazitaet = 500.0;  // Kapazität des Akkus in mAh

void setup() {
  Serial.begin(9600);
  Wire.begin();
  delay(1000); 

  Serial.println("Max17055 Ladezustand und Zellenspannung");

  // richtigen Muktiplexer Kanal auswählen
  selectMultiplexerChannel(1);

  // Ladezustand und Zellenspannung abfragen
  byte soc = readSOC();
  float vcell = readVCELL();

  if (soc != 255) {
    Serial.print("Ladezustand (Rohwert): ");
    Serial.println(soc);
    float socPercentage = (soc / 255.0) * 100.0;  // Berechnung des Ladezustands als Prozentsatz
    Serial.print("Ladezustand: ");
    Serial.print(socPercentage);
    Serial.println("%");

    // Berechnung der tatsächlichen geladenen Kapazität (in mAh)
    float geladeneKapazitaet = (socPercentage / 100.0) * vollKapazitaet;
    Serial.print("Geladene Kapazität: ");
    Serial.print(geladeneKapazitaet);
    Serial.println(" mAh");
  } else {
    Serial.println("Fehler beim Auslesen des Ladezustands.");
  }

  if (vcell != -1) {
    Serial.print("Zellenspannung (Rohwert): ");
    Serial.println(vcell);
    float vcellVoltage = vcell * 0.00125;  // Umrechnungsfaktor für 1,25 mV pro Schritt
    Serial.print("Zellenspannung: ");
    Serial.print(vcellVoltage);
    Serial.println(" V");
  } else {
    Serial.println("Fehler beim Auslesen der Zellenspannung.");
  }
}

void selectMultiplexerChannel(uint8_t channel) {
  Wire.beginTransmission(MULTIPLEXER_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
  delay(100); 
}

byte readSOC() {
  byte soc = 255;  // Standardwert bei Fehler

  Wire.beginTransmission(MAX17055_ADDR);
  Wire.write(MAX17055_REG_SOC);  // Register für Ladezustand
  byte error = Wire.endTransmission();

  if (error != 0) {
    Serial.print("Fehler bei der Kommunikation mit MAX17055. Fehlercode: ");
    Serial.println(error);
    return soc;  
  }

  Wire.requestFrom(MAX17055_ADDR, 1);  // Ein Byte anfordern
  if (Wire.available()) {
    soc = Wire.read();
  }

  return soc;
}

float readVCELL() {
  float vcell = -1;  

  Wire.beginTransmission(MAX17055_ADDR);
  Wire.write(MAX17055_REG_VCELL);  // Zellenspannung Register
  byte error = Wire.endTransmission();

  if (error != 0) {
    Serial.print("Fehler bei der Kommunikation mit MAX17055. Fehlercode: ");
    Serial.println(error);
    return vcell;  
  }

  Wire.requestFrom(MAX17055_ADDR, 2);  // Zwei Bytes für Zellenspannung anfordern
  if (Wire.available() == 2) {
    byte highByte = Wire.read();
    byte lowByte = Wire.read();
    int16_t rawVoltage = (highByte << 8) | lowByte;

    // Prüfen, ob der Rohwert negativ ist 
    if (rawVoltage < 0) {
      Serial.println("Fehlerhafte Zellenspannung: Negativer Wert");
      return -1; // Fehlerwert zurückgeben
    }

    // Rohwert zurückgeben, um die Spannung später korrekt zu berechnen
    vcell = rawVoltage;  
  }

  return vcell;
}

void loop() {
  setup();
}
