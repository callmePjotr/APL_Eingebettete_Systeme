#include <Wire.h>
#include <Arduino-MAX17055_Driver.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define MULTIPLEXER_ADDR 0x70  // Adresse des Multiplexers (TCA9548)
#define DISPLAY_WIDTH 128 // OLED Display Breite
#define DISPLAY_HEIGHT 64 // OLED Display Höhe
#define OLED_RESET -1   // Share reset with Arduino reset pin

const float BATTERY_CAPACITY = 500.0;  // Kapazität des Akkus in mAh
MAX17055 sensor;
Adafruit_SH1107 display = Adafruit_SH1107(DISPLAY_HEIGHT, DISPLAY_WIDTH, &Wire, OLED_RESET, 1000000, 100000);

unsigned long startTime;  // Speichert die Startzeit in Millisekunden

void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  startTime = millis();  // Speichere Startzeit
 
  while (!Serial) {
    delay(1);  // Warten auf Serial bei Native USB
  }
 
  delay(1000);  // Wartezeit für Stabilisierung
 
  Serial.println("MAX17055 Batterie-Monitor mit Multiplexer und OLED Display");
 
  // Initialisiere das Display
  display.begin(0x3C, true); // Adresse 0x3C für 128x64
  display.clearDisplay();
  display.setRotation(1);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.display();
 
  // Wähle den richtigen Kanal des Multiplexers
  selectMultiplexerChannel(1);
 
  // Initialisiere den MAX17055
  sensor.setCapacity(BATTERY_CAPACITY);
}

void selectMultiplexerChannel(uint8_t channel) {
  Wire.beginTransmission(MULTIPLEXER_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
  delay(100);  // Wartezeit für Kanalumschaltung
}

String getRuntime() {
  unsigned long runtime = millis() - startTime;
  int hours = runtime / 3600000;
  int minutes = (runtime % 3600000) / 60000;
  int seconds = (runtime % 60000) / 1000;
  
  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", hours, minutes, seconds);
  return String(timeStr);
}

void updateDisplay(float soc, float voltage, float current, float timeToEmpty) {
  display.clearDisplay();
  display.setCursor(0,0);
  
  // Laufzeit anzeigen
  display.print("Zeit: ");
  display.println(getRuntime());
 
  // Spannung
  display.print("Spannung: ");
  display.print(voltage, 2);
  display.println("V");
 
  // Strom und Lade-/Entladestatus
  display.print("Strom: ");
  display.print(current, 1);
  display.print("mA ");
  if (current > 0) {
    display.println("(Laden)");
  } else if (current < 0) {
    display.println("(Entladen)");
  } else {
    display.println("(Standby)");
  }
 
  display.display();
}

void readBatteryData() {
  float soc = sensor.getSOC();
  float voltage = sensor.getInstantaneousVoltage();
  float current = sensor.getInstantaneousCurrent();
  float timeToEmpty = sensor.getTimeToEmpty();
  float actualCapacity = (soc / 100.0) * BATTERY_CAPACITY;
 
  // Update OLED Display
  updateDisplay(soc, voltage, current, timeToEmpty);
 
  // Serial-Ausgabe für Debugging
  Serial.print("Laufzeit: ");
  Serial.println(getRuntime());
 
  Serial.print("Aktuelle Kapazität: ");
  Serial.print(actualCapacity, 2);
  Serial.println(" mAh");
 
  Serial.print("Spannung: ");
  Serial.print(voltage, 3);
  Serial.println(" V");
 
  Serial.print("Strom: ");
  Serial.print(current, 2);
  Serial.println(" mA");
 
}

void loop() {
  selectMultiplexerChannel(1);
  readBatteryData();
  delay(5000);  // 5 Sekunden Wartezeit zwischen den Messungen
}