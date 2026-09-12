#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define LDR_PIN A0
#define SOUND_PIN A1
#define FLAME_PIN A2
#define VIBRATION_PIN 4
#define BUZZER_PIN 8

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LDR_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(VIBRATION_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int ldrValue = analogRead(LDR_PIN);
  int soundValue = analogRead(SOUND_PIN);
  int flameValue = analogRead(FLAME_PIN);
  int vibrationState = digitalRead(VIBRATION_PIN);

  // Convert flame intensity to percentage (0-100%)
  int flamePercent = map(flameValue, 0, 1023, 100, 0);

  // Serial Monitor Debugging
  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print("°C  Humidity: "); Serial.print(humidity);
  Serial.print("%  LDR: "); Serial.print(ldrValue);
  Serial.print("  Sound: "); Serial.print(soundValue);
  Serial.print("  Flame: "); Serial.print(flamePercent);
  Serial.print("%  Vibration: "); Serial.println(vibrationState);

  // OLED Display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("📟 EnviroGuard 1.0");
  display.println("------------------");
  display.print("Temp: "); display.print(temperature); display.println(" C");
  display.print("Hum:  "); display.print(humidity); display.println(" %");
  display.print("Light: "); display.println(ldrValue);
  display.print("Sound: "); display.println(soundValue);
  display.print("Flame: "); display.print(flamePercent); display.println("%");
  display.print("Vib: "); display.println(vibrationState == HIGH ? "⚠️ YES" : "OK");
  display.display();

  // Alerts
  if (temperature > 40 || flamePercent < 60 || vibrationState == HIGH || soundValue > 700) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(1000);
}
