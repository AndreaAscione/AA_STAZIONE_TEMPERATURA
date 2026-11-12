#include <WiFi.h>
#include <WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "time.h"

// =========================================================================
// SCELTA MODALITÀ WI-FI (Scegli commentando o decommentando la riga sotto):
// - Lascia attiva (#define) -> Si collega al Wi-Fi esistente + scarica ora NTP
// - Commenta (// #define)    -> Crea rete Wi-Fi autonoma (Access Point)
// =========================================================================
#define MODALITA_CLIENT

// --- CREDENZIALI MODALITÀ 1: CLIENT (collegamento a router/hotspot) ---
const char* ssid_client = "NOME_RETE_WIFI";
const char* pass_client = "PASSWORD_WIFI";

// --- CREDENZIALI MODALITÀ 2: ACCESS POINT (rete creata dall'ESP32) ---
const char* ssid_ap = "Stazione_Termica_ESP32";
const char* pass_ap = "Password123";

// --- CONFIGURAZIONE ORARIO NTP (Fuso orario Italia) ---
const char* ntpServer = "pool.ntp.org";
const char* timeZone = "CET-1CEST,M3.5.0,M10.5.0/3";

// --- CONFIGURAZIONE SENSORE DS18B20 (Pin TX = GPIO 21) ---
#define ONE_WIRE_BUS 21
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Creazione del Web Server sulla porta 80
WebServer server(80);

// Funzione per ottenere data e ora dall'orologio interno dell'ESP32
String ottieniDataOra() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Orario NTP non disponibile";
  }
  char buffer;
  strftime(buffer, sizeof(buffer), "%d/%m/%Y - %H:%M:%S", &timeinfo);
  return String(buffer);
}

// Funzione che genera la pagina web al momento della richiesta
void mostraPaginaWeb() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<meta http-equiv='refresh' content='5'>"; // Ricarica ogni 5 secondi
  html += "<title>Stazione Termica ESP32-C3</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; background-color: #eef2f5; margin-top: 40px; }";
  html += ".card { background: white; padding: 30px; border-radius: 15px; box-shadow: 0 4px 10px rgba(0,0,0,0.1); display: inline-block; max-width: 400px; width: 90%; }";
  html += ".temp { font-size: 56px; font-weight: bold; color: #007bff; margin: 15px 0; }";
  html += ".info { background: #f8f9fa; padding: 12px; border-radius: 8px; margin-top: 15px; font-size: 15px; color: #333; }";
  html += "</style></head><body>";

  html += "<div class='card'>";
  html += "<h2>Stazione Termica ESP32-C3</h2>";

  // 1. Inserimento Temperatura nella pagina
  if (tempC == DEVICE_DISCONNECTED_C) {
    html += "<div class='temp' style='color: red;'>Errore sensore!</div>";
  } else {
    html += "<div class='temp'>" + String(tempC, 2) + " &deg;C</div>";
  }

  // 2. Inserimento Data/Ora NTP e Modalità Wi-Fi
  html += "<div class='info'>";
  #ifdef MODALITA_CLIENT
    html += "<p><b>Data e Ora (NTP):</b><br>" + ottieniDataOra() + "</p>";
    html += "<p><b>Rete:</b> Wi-Fi Client (Connesso)</p>";
  #else
    html += "<p><b>Data e Ora:</b> Modalit&agrave; offline (Access Point)</p>";
    html += "<p><b>Rete:</b> Access Point Locale</p>";
  #endif
  html += "</div>";

  html += "<p style='color: #888; font-size: 12px; margin-top: 20px;'>Aggiornamento automatico ogni 5 secondi</p>";
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(2000); // Attesa stabilizzazione seriale USB

  sensors.begin();
  Serial.println("\n=== AVVIO STAZIONE TERMICA COMPLETA ===");

  #ifdef MODALITA_CLIENT
    // --- MODALITÀ 1: CLIENT + NTP ---
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_client, pass_client);
    Serial.print("Connessione a: ");
    Serial.println(ssid_client);

    int tentativi = 0;
    while (WiFi.status() != WL_CONNECTED && tentativi < 30) {
      delay(500);
      Serial.print(".");
      tentativi++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWi-Fi connesso!");
      Serial.print("Apri nel browser: http://");
      Serial.println(WiFi.localIP());

      // Sincronizzazione Orario NTP
      configTzTime(timeZone, ntpServer);
      Serial.println("Sincronizzazione ora NTP...");
      delay(2000);
      Serial.print("Ora attuale: ");
      Serial.println(ottieniDataOra());
    } else {
      Serial.println("\nErrore connessione Wi-Fi!");
    }

  #else
    // --- MODALITÀ 2: ACCESS POINT ---
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid_ap, pass_ap);
    Serial.println("Access Point avviato!");
    Serial.print("Collegati alla rete: ");
    Serial.println(ssid_ap);
    Serial.print("Apri nel browser: http://");
    Serial.println(WiFi.softAPIP());
  #endif

  // Avvio del Web Server
  server.on("/", mostraPaginaWeb);
  server.begin();
  Serial.println("Server Web attivo!");
}

void loop() {
  server.handleClient();
}
