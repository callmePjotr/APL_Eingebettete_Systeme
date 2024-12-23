#include <Wire.h>
#include <Arduino-MAX17055_Driver.h>

#define MULTIPLEXER_ADDR 0x70  // Adresse des Multiplexers, bis ich überhaupt vertsanden habe, wie ich den Max17055 anspreche
                              // habe ich vermutlich amlängsten gebraucht
                              // falls sie meine i2C scanner dafürauch noch sehen wollen, kann ich die gerne nachreichen
const float BATTERY_CAPACITY = 500.0;  // Kapazität des Akkus in mAh

MAX17055 sensor;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  while (!Serial) {
    delay(1);  // Warten auf Serial bei Native USB
  }
  
  delay(1000);
  
  Serial.println("MAX17055 Batterie-Monitor mit Multiplexer");
  
  // korrekten Multiplexer Kanal auswählen
  selectMultiplexerChannel(1);
  
  // hier ist wahrscheinlich der Fehler zum alten Programm
  // fehlerhafte Initialisierung
  sensor.setCapacity(BATTERY_CAPACITY);

}

void selectMultiplexerChannel(uint8_t channel) {
  Wire.beginTransmission(MULTIPLEXER_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
  delay(100);  // kurz wrten bis auf den Kanal umgeschalten wird
}

void readBatteryData() {
  // Ladezustand (SOC) auslesen
  float soc = sensor.getSOC();
  
  // Spannung auslesen
  float voltage = sensor.getInstantaneousVoltage();
  
  // Strom auslesen
  float current = sensor.getInstantaneousCurrent();
  
  // Restzeit berechnen
  float timeToEmpty = sensor.getTimeToEmpty();
  
  // Berechnete Kapazität
  float actualCapacity = (soc / 100.0) * BATTERY_CAPACITY;
  

  Serial.println("\nBatterie-Status:");
  Serial.print("Ladezustand: ");
  Serial.print(soc, 2);
  Serial.println("%");
  
  Serial.print("Aktuelle Kapazität: ");
  Serial.print(actualCapacity, 2);
  Serial.println(" mAh");
  
  Serial.print("Spannung: ");
  Serial.print(voltage, 3);
  Serial.println(" V");
  
  Serial.print("Strom: ");
  Serial.print(current, 2);
  Serial.println(" mA");
  
  Serial.print("Zeit bis leer: ");
  Serial.print(timeToEmpty, 2);
  Serial.println(" Stunden");
}

void loop() {
  selectMultiplexerChannel(1);
  readBatteryData();
  delay(5000);  // 5 Sekunden Wartezeit zwischen den Messungen
}