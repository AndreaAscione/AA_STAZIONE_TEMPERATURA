#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();
  Serial.println("--- TEST DS18B20 INIZIALIZZATO ---");
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Errore: Sensore non trovato!");
  } else {
    Serial.print("Temperatura: ");
    Serial.print(tempC);
    Serial.println(" °C");
  }

  delay(2000);
}
