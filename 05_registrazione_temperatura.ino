
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "time.h"

// -------------------------
// CONFIGURAZIONE WIFI
// -------------------------

const char* ssid = "NOME_RETE_WIFI";
const char* password = "PASSWORD_WIFI";

// -------------------------
// CONFIGURAZIONE NTP
// -------------------------

const char* ntpServer = "pool.ntp.org";

const char* timezone =
  "CET-1CEST,M3.5.0/2,M10.5.0/3";

// -------------------------
// CONFIGURAZIONE SENSORE
// -------------------------

#define ONE_WIRE_BUS 15

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// -------------------------
// INTERVALLO DI MISURAZIONE
// -------------------------

unsigned long intervallo = 60000;
unsigned long ultimaMisura = 0;

void setup() {

  Serial.begin(115200);

  Serial.println("--- STAZIONE TERMICA ---");

  // Inizializzazione sensore
  sensors.begin();

  // Connessione Wi-Fi
  WiFi.begin(ssid, password);

  Serial.print("Connessione Wi-Fi");

  int tentativi = 0;

  while (WiFi.status() != WL_CONNECTED &&
         tentativi < 30) {

    delay(500);
    Serial.print(".");

    tentativi++;

  }

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("\nErrore Wi-Fi!");
    Serial.println("Registrazione non avviata.");

    return;

  }

  Serial.println("\nWi-Fi connesso!");

  // Configurazione orologio NTP
  configTzTime(timezone, ntpServer);

  struct tm timeinfo;

  if (getLocalTime(&timeinfo, 15000)) {

    Serial.println("Orologio sincronizzato!");

  } else {

    Serial.println("Attenzione: ora non disponibile!");

  }

  // Intestazione CSV
  Serial.println("Data,Ora,Temperatura_C");

}

void loop() {

  if (millis() - ultimaMisura >= intervallo) {

    ultimaMisura = millis();

    struct tm timeinfo;

    // Controllo orario
    if (!getLocalTime(&timeinfo, 2000)) {

      Serial.println("Errore: orario non disponibile!");
      return;

    }

    // Lettura temperatura
    sensors.requestTemperatures();

    float temperatura =
      sensors.getTempCByIndex(0);

    if (temperatura == DEVICE_DISCONNECTED_C) {

      Serial.println("Errore: sensore non trovato!");
      return;

    }

    // Registrazione dati CSV
    Serial.printf(
      "%02d/%02d/%04d,%02d:%02d:%02d,%.2f\n",
      timeinfo.tm_mday,
      timeinfo.tm_mon + 1,
      timeinfo.tm_year + 1900,
      timeinfo.tm_hour,
      timeinfo.tm_min,
      timeinfo.tm_sec,
      temperatura
    );

  }

}
