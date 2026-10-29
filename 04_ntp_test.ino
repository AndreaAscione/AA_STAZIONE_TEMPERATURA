#include <WiFi.h>
#include "time.h"

// Credenziali Wi-Fi per la connessione a Internet
const char* ssid     = "NOME_RETE_WIFI";
const char* password = "PASSWORD_WIFI";

// Server NTP e configurazione fuso orario Italia
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;      // UTC +1 (Ora solare in Italia)
const int   daylightOffset_sec = 3600; // +1 ora durante l'ora legale

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Errore: Impossibile ottenere l'ora da NTP!");
    return;
  }
  // Stampa l'ora formattata: GG/MM/AAAA - HH:MM:SS
  Serial.println(&timeinfo, "Ora attuale: %d/%m/%Y - %H:%M:%S");
}

void setup() {
  Serial.begin(115200);

  // Connessione al Wi-Fi
  Serial.print("Connessione a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connesso!");

  // Inizializzazione e sincronizzazione dell'ora via NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  delay(1000);
  printLocalTime(); // Stampa l'ora aggiornata ogni secondo
}
