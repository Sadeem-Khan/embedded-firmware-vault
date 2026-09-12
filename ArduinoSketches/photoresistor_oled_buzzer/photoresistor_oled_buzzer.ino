#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LDR_PIN 34            // Change if using different analog pin
#define BUZZER_PIN 26
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("OLED Ready...");
  display.display();
  delay(1000);
}

void loop() {
  int lightVal = analogRead(LDR_PIN);
  Serial.println(lightVal);

  // Display on OLED
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.print("Light Intensity:");
  display.setCursor(0, 30);
  display.setTextSize(2);
  display.print(lightVal);
  display.display();

  // Buzzer Logic
  if (lightVal < 1000) { // Adjust this threshold as needed
    tone(BUZZER_PIN, 4000);
  } else {
    noTone(BUZZER_PIN);
  }

  delay(300);
}
