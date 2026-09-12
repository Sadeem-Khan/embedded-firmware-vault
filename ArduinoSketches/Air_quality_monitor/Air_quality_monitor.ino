#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// Pins
#define DHTPIN 4
#define LDR_D_PIN 3
#define MQ135_A_PIN A0
#define MQ135_D_PIN 2
#define FLAME_A_PIN A2   // Your specific request
#define FLAME_D_PIN 5    // Your specific request
#define DHTTYPE DHT11

// OLED Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LDR_D_PIN, INPUT);
  pinMode(MQ135_D_PIN, INPUT);
  pinMode(FLAME_D_PIN, INPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;); 
  }
  
  // Professional Boot Sequence
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(15, 20);
  display.println("SAFETY STATION");
  display.setCursor(15, 35);
  display.println("System Loading...");
  display.display();
  delay(2000);
}

void loop() {
  // 1. Read All Sensors
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasRaw = analogRead(MQ135_A_PIN);
  int fireRaw = analogRead(FLAME_A_PIN);
  
  bool gasAlert = (digitalRead(MQ135_D_PIN) == LOW);
  bool fireDetected = (digitalRead(FLAME_D_PIN) == LOW); // LOW is usually "fire detected"
  bool isDark = (digitalRead(LDR_D_PIN) == HIGH);

  // 2. Clear Display for Update
  display.clearDisplay();
  
  // Dashboard Header
  display.fillRect(0, 0, 128, 12, WHITE);
  display.setTextColor(BLACK);
  display.setCursor(4, 2);
  display.print("ENV MONITOR: ACTIVE");

  // Temperature & Humidity (Row 1)
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 18);
  display.print("T:"); display.print(t, 0); display.print("C ");
  display.print("H:"); display.print(h, 0); display.print("% ");
  display.print(isDark ? "[NIGHT]" : "[DAY]");

  // Gas and Fire Intensity (Row 2 - Large Values)
  display.setCursor(0, 32);
  display.print("GAS:");
  display.setCursor(30, 32);
  display.setTextSize(1);
  display.print(gasRaw);

  display.setCursor(65, 32);
  display.print("FIRE:");
  display.setCursor(100, 32);
  display.print(fireRaw);

  // Status Bar (Footer)
  display.drawLine(0, 48, 127, 48, WHITE);
  display.setCursor(0, 54);
  
  if (fireDetected) {
    display.print("CRITICAL: FIRE!!!");
  } else if (gasAlert) {
    display.print("WARN: GAS LEAK");
  } else {
    display.print("SYSTEM STATUS: OK");
  }

  display.display();
  delay(1000); // 1 second refresh rate
}