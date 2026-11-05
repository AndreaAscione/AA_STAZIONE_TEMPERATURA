#include <OneWire.h>
#include <DallasTemperature.h>

// Pin del sensore DS18B20
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {

  Serial.begin(115200);

  sensors.begin();

  Serial.println("--- TEST DS18B20 ---");
  Serial.println("Sensore inizializzato!");

}

void loop() {

  sensors.requestTemperatures();

  float tempC = sensors.getTempCByIndex(0);

  if (tempC == DEVICE_DISCONNECTED_C) {

    Serial.println("Errore: sensore non trovato!");

  } else {

    Serial.print("Temperatura: ");
    Serial.print(tempC, 2);
    Serial.println(" °C");

  }

  delay(2000);

}
