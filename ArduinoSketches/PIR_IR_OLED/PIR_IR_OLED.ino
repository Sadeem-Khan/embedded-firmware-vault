#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Sensor pins
const int pirPin = 25;  // D25 for PIR
const int irPin = 26;   // D26 for IR

int pirState = 0;
int irState = 0;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(irPin, INPUT);

  Serial.begin(9600);
  Wire.begin(21, 22); // Confirm your wiring: SDA=21, SCL=22

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  pirState = digitalRead(pirPin);
  irState = digitalRead(irPin);

  display.clearDisplay();
  display.setCursor(0, 0);

  if (pirState == HIGH && irState == LOW) {
    display.println("Motion & Obstacle!");
    Serial.println("Motion & Obstacle!");
  }
  else if (pirState == HIGH) {
    display.println("Only Motion Detected");
    Serial.println("Only motion detected!");
  }
  else if (irState == LOW) {
    display.println("Only Obstacle Detected");
    Serial.println("Only obstacle detected!");
  }
  else {
    display.println("All clear :)");
    Serial.println("All clear.");
  }

  display.display();
  delay(200);
}
