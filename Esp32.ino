#include <OneWire.h>
#include <DallasTemperature.h>

#define PIN_DATI 2

OneWire oneWire(PIN_DATI);
DallasTemperature sensori(&oneWire);

int numeroSensori;

void setup() {
  Serial.begin(9600);
  Serial.println("--- Ricerca Sensori sul Bus ---");
  
  sensori.begin();
  
  // Conta quanti sensori sono presenti sulla linea dati
  numeroSensori = sensori.getDeviceCount();
  
  Serial.print("Sensori trovati sul pin ");
  Serial.print(PIN_DATI);
  Serial.print(": ");
  Serial.println(numeroSensori);
}

void loop() {
  if (numeroSensori == 0) {
    Serial.println("Nessun sensore rilevato. Verifica i collegamenti.");
    delay(5000);
    return;
  }

  // Richiedi la temperatura a tutti i sensori connessi
  sensori.requestTemperatures();
  
  // Cicla attraverso tutti i sensori trovati e stampa la temperatura di ognuno
  for (int i = 0; i < numeroSensori; i++) {
    float tempC = sensori.getTempCByIndex(i);
    
    Serial.print("Sensore #");
    Serial.print(i);
    Serial.print(": ");
    
    if (tempC != DEVICE_DISCONNECTED_C) {
      Serial.print(tempC);
      Serial.println(" *C");
    } else {
      Serial.println("ERRORE LETTURA");
    }
  }
  
  Serial.println("------------------------------------");
  delay(3000); // Aspetta 3 secondi prima del prossimo ciclo
}
