#include <WiFi.h>

// Nome della rete e password che l'ESP32 creera'
const char* ap_ssid = "Stazione_Termica_ESP32";
const char* ap_password = "Password123";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configurazione Access Point in corso...");

  // Avvio in modalita' Access Point
  WiFi.softAP(ap_ssid, ap_password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Access Point avviato! Nome rete: ");
  Serial.println(ap_ssid);
  Serial.print("Indirizzo IP per collegarsi all'ESP32: ");
  Serial.println(myIP);
}

void loop() {
  // Mostra quanti dispositivi sono connessi all'ESP32
  int dispositiviConnessi = WiFi.softAPgetStationNum();
  Serial.print("Dispositivi attualmente connessi: ");
  Serial.println(dispositiviConnessi);
  
  delay(5000);
}
