#include "DHT.h"

#define DHTPIN 3       // DHT sensor connected to pin 3
#define DHTTYPE DHT11  // Change to DHT22 if using that sensor
DHT dht(DHTPIN, DHTTYPE);

int buzzer = 9;   // Buzzer or LED pin

void setup() {
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float temp = dht.readTemperature(); // Temperature in °C
  float hum = dht.readHumidity();     // Humidity %

  // Print sensor values
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("°C | Hum: ");
  Serial.print(hum);
  Serial.println("%");

  // Fire detection condition (Temp > 35°C)
  if (temp > 35) {
    Serial.println("🔥 FIRE DETECTED!");
    digitalWrite(buzzer, HIGH);  // Alarm ON
  } else {
    Serial.println("✅ Safe");
    digitalWrite(buzzer, LOW);   // Alarm OFF
  }

  delay(1000);
}
