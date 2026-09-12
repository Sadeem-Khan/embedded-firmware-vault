#include "DHT.h"

// Pin where your DHT sensor DATA pin is connected
#define DHTPIN 2

// Uncomment your sensor type:
#define DHTTYPE DHT11
// #define DHTTYPE DHT22

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("DHT sensor test!");

  dht.begin();
}

void loop() {
  delay(2000); // Wait 2 seconds between readings

  // Read temperature in Celsius
  float tempC = dht.readTemperature();
  // Read humidity in %
  float hum = dht.readHumidity();

  // Check if any reads failed
  if (isnan(tempC) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" *C");
}
