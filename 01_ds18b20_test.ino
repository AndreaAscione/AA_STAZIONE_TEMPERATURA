#include <OneWire.h>
#include <DallasTemperature.h>

// Pin TX dell'ESP32-C3 (GPIO 21)
#define ONE_WIRE_BUS 21

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  delay(2000); // Attesa stabilizzazione seriale USB

  sensors.begin();

  Serial.println("--- STAZIONE TEMPERATURA ESP32-C3 ---");
  Serial.print("Sensori attivi: ");
  Serial.println(sensors.getDeviceCount());
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Errore di lettura dal sensore!");
  } else {
    Serial.print("Temperatura attuale: ");
    Serial.print(tempC, 2);
    Serial.println(" °C");
  }

  delay(2000); // Aggiorna ogni 2 secondi
}
