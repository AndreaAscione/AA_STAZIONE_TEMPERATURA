
#include <WiFi.h>
#include "time.h"

// Credenziali Wi-Fi
const char* ssid = "NOME_RETE_WIFI";
const char* password = "PASSWORD_WIFI";

// Server NTP
const char* ntpServer = "pool.ntp.org";

// Fuso orario italiano
const char* timezone =
  "CET-1CEST,M3.5.0/2,M10.5.0/3";

void setup() {

  Serial.begin(115200);

  Serial.println("--- TEST OROLOGIO NTP ---");

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

    Serial.println("\nConnessione Wi-Fi fallita!");
    return;

  }

  Serial.println("\nWi-Fi connesso!");

  // Configurazione orologio
  configTzTime(timezone, ntpServer);

  Serial.println("Sincronizzazione orologio...");
}

void loop() {

  struct tm timeinfo;

  if (WiFi.status() == WL_CONNECTED &&
      getLocalTime(&timeinfo, 5000)) {

    Serial.println(
      &timeinfo,
      "Data e ora: %d/%m/%Y - %H:%M:%S"
    );

  } else {

    Serial.println("Ora non disponibile!");

  }

  delay(1000);

}

void loop() {
  delay(1000);
  printLocalTime(); // Stampa l'ora aggiornata ogni secondo
}
