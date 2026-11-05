#include <WiFi.h>

// Credenziali Wi-Fi
const char* ssid = "NOME_RETE_WIFI";
const char* password = "PASSWORD_WIFI";

void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println("--- TEST WIFI CLIENT ---");

  Serial.print("Connessione a: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int tentativi = 0;

  while (WiFi.status() != WL_CONNECTED &&
         tentativi < 30) {

    delay(500);
    Serial.print(".");

    tentativi++;

  }

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println();
    Serial.println("Wi-Fi connesso!");

    Serial.print("Indirizzo IP ESP32: ");
    Serial.println(WiFi.localIP());

  } else {

    Serial.println();
    Serial.println("Errore connessione Wi-Fi!");

  }

}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("Stato Wi-Fi: CONNESSO");

  } else {

    Serial.println("Stato Wi-Fi: DISCONNESSO");

  }

  delay(5000);

}
