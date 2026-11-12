#include <WiFi.h>
#include "time.h"

// Credenziali Wi-Fi (usa le stesse che hanno appena funzionato)
const char* ssid = "NOME_RETE_WIFI";
const char* password = "PASSWORD_WIFI";

// Server NTP e fuso orario italiano (gestisce in automatico ora solare e legale)
const char* ntpServer = "pool.ntp.org";
const char* timeZone = "CET-1CEST,M3.5.0,M10.5.0/3";

void stampaDataOra() {
  struct tm timeinfo;
  
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Errore: impossibile ottenere l'ora NTP!");
    return;
  }

  // Formatta data e ora in formato italiano (GG/MM/AAAA HH:MM:SS)
  char buffer[64];
  strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);
  Serial.print("Data e ora attuale: ");
  Serial.println(buffer);
}

void setup() {
  Serial.begin(115200);
  delay(2000); // Attesa stabilizzazione seriale USB

  Serial.println("\n--- TEST SINCRONIZZAZIONE ORARIO NTP ---");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connessione a ");
  Serial.print(ssid);

  int tentativi = 0;
  while (WiFi.status() != WL_CONNECTED && tentativi < 30) {
    delay(500);
    Serial.print(".");
    tentativi++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connesso!");
    
    // Sincronizzazione con server NTP e fuso orario italiano
    configTzTime(timeZone, ntpServer);
    Serial.println("Sincronizzazione ora in corso...");
    delay(2000);
    
    stampaDataOra();
  } else {
    Serial.println("\nErrore connessione Wi-Fi!");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    stampaDataOra();
  } else {
    Serial.println("Wi-Fi disconnesso!");
  }
  
  delay(1000); // Stampa ogni secondo
}
