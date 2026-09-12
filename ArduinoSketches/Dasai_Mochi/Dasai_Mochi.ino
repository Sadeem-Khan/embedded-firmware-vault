#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BUTTON_PIN 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int eyePos = 0;
bool eyeDirection = 1;  // 1 = right, 0 = left
bool smile = true;

void drawSmile(int x, int y) {
  // Draw a curved smile using short line segments
  display.drawLine(x - 10, y, x - 5, y + 3, SSD1306_WHITE);
  display.drawLine(x - 5, y + 3, x, y + 4, SSD1306_WHITE);
  display.drawLine(x, y + 4, x + 5, y + 3, SSD1306_WHITE);
  display.drawLine(x + 5, y + 3, x + 10, y, SSD1306_WHITE);
}

void drawFace(int offset, bool blink, bool smileOn) {
  display.clearDisplay();

  // Head outline
  display.drawRoundRect(20, 5, 88, 55, 10, SSD1306_WHITE);

  // Eyes
  if (blink) {
    display.drawLine(45 + offset, 25, 55 + offset, 25, SSD1306_WHITE);
    display.drawLine(75 + offset, 25, 85 + offset, 25, SSD1306_WHITE);
  } else {
    display.fillCircle(50 + offset, 25, 5, SSD1306_WHITE);
    display.fillCircle(80 + offset, 25, 5, SSD1306_WHITE);
  }

  // Mouth
  if (smileOn)
    drawSmile(65, 40);
  else
    display.drawLine(50, 45, 80, 45, SSD1306_WHITE); // neutral

  display.display();
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN) == LOW;
  static unsigned long lastBlink = 0;
  static bool blink = false;

  // Blink every 2 seconds
  if (millis() - lastBlink > 2000) {
    blink = !blink;
    lastBlink = millis();
  }

  // Eye movement animation
  if (eyeDirection)
    eyePos++;
  else
    eyePos--;
  if (eyePos > 5) eyeDirection = 0;
  if (eyePos < -5) eyeDirection = 1;

  // Button press toggles smile
  if (buttonState)
    smile = !smile;

  drawFace(eyePos, blink, smile);
  delay(120);
}
