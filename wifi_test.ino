#include <WiFi.h>

// =========================================================================
// SCELTA MODALITÀ WI-FI:
// Lascia attiva la riga sotto per collegarti a un Wi-Fi esistente (Client).
// Metti // davanti a #define per creare una rete Wi-Fi (Access Point).
// =========================================================================
#define MODALITA_CLIENT

// --- CREDENZIALI MODALITÀ 1: CLIENT (collegamento a router/hotspot) ---
const char* ssid_client = "NOME_RETE_WIFI";
const char* password_client = "PASSWORD_WIFI";

// --- CREDENZIALI MODALITÀ 2: ACCESS POINT (rete creata dall'ESP32) ---
const char* ssid_ap = "Stazione_Termica_ESP32";
const char* password_ap = "Password123";

void setup() {
  Serial.begin(115200);
  delay(2000); // Attesa per stabilizzare la porta seriale USB dell'ESP32-C3

  Serial.println("\n--- TEST WI-FI ESP32-C3 ---");

  #ifdef MODALITA_CLIENT
    // ==========================================
    // MODALITÀ 1: WI-FI CLIENT (STA)
    // ==========================================
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_client, password_client);

    Serial.print("Modalita' CLIENT -> Connessione a: ");
    Serial.println(ssid_client);

    int tentativi = 0;
    while (WiFi.status() != WL_CONNECTED && tentativi < 30) {
      delay(500);
      Serial.print(".");
      tentativi++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWi-Fi connesso con successo!");
      Serial.print("Indirizzo IP assegnato: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("\nErrore: impossibile connettersi al Wi-Fi!");
    }

  #else
    // ==========================================
    // MODALITÀ 2: ACCESS POINT (AP)
    // ==========================================
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid_ap, password_ap);

    Serial.println("Modalita' ACCESS POINT -> Rete creata!");
    Serial.print("Nome rete (SSID): ");
    Serial.println(ssid_ap);
    Serial.print("Password rete: ");
    Serial.println(password_ap);
    Serial.print("Indirizzo IP ESP32: ");
    Serial.println(WiFi.softAPIP());
  #endif
}

void loop() {
  #ifdef MODALITA_CLIENT
    // Monitoraggio connessione in modalita' Client
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Stato Wi-Fi: CONNESSO");
    } else {
      Serial.println("Stato Wi-Fi: DISCONNESSO");
    }
  #else
    // Monitoraggio dispositivi collegati in modalita' Access Point
    int dispositivi = WiFi.softAPgetStationNum();
    Serial.print("Dispositivi connessi all'ESP32: ");
    Serial.println(dispositivi);
  #endif

  delay(5000);
}
