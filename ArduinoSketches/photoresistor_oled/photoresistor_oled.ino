#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LDR_PIN 34  // Analog pin where LDR is connected

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // ESP32 default I2C pins: SDA = 21, SCL = 22

  // OLED Initialization
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while (true);  // Freeze if OLED is not found
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("OLED Ready...");
  display.display();
  delay(1000);
}

void loop() {
  int lightVal = analogRead(LDR_PIN);  // Read LDR value
  Serial.print("LDR Value: ");
  Serial.println(lightVal);

  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.print("Light Intensity:");
  display.setCursor(0, 30);
  display.setTextSize(2);
  display.print(lightVal);
  display.display();

  delay(500);
}
