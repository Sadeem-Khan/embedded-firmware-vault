#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// DHT11 settings
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,20);
  display.println("Weather Station");
  display.display();
  delay(2000);
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int ldrValue = analogRead(A0);
  int lightPercent = map(ldrValue, 0, 1023, 0, 100);

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print to Serial Monitor
  Serial.print("Temp: "); Serial.print(temp); Serial.print(" C ");
  Serial.print("Hum: "); Serial.print(hum); Serial.print(" % ");
  Serial.print("Light: "); Serial.print(lightPercent); Serial.println(" %");

  // Show on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temp: ");
  display.print(temp);
  display.print((char)247); // degree symbol
  display.println("C");

  display.setCursor(0,16);
  display.print("Humidity: ");
  display.print(hum);
  display.println("%");

  display.setCursor(0,32);
  display.print("Light: ");
  display.print(lightPercent);
  display.println("%");

  display.display();
  delay(2000);
}
