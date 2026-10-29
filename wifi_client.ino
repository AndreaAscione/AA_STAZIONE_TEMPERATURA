#include <WiFi.h>

// Sostituisci con le credenziali della tua rete
const char* ssid = "NOME_RETE_WIFI";
const char* password = "PASSWORD_WIFI";

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.print("Connessione a: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Attesa connessione
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi Connesso!");
  Serial.print("Indirizzo IP dell'ESP32: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Verifica se la connessione è ancora attiva
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Stato Wi-Fi: Connesso");
  } else {
    Serial.println("Stato Wi-Fi: Disconnesso");
  }
  delay(5000);
}
