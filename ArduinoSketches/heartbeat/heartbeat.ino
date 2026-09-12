#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SENSOR_PIN A0
int threshold = 550;  // Adjust after checking raw values

unsigned long lastBeat = 0;
int beatCount = 0;
float bpm = 0;
float bpmAvg = 0;

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  int value = analogRead(SENSOR_PIN);

  if (value > threshold) {
    unsigned long now = millis();
    if (now - lastBeat > 300) {  // debounce: ignore <300ms intervals
      int interval = now - lastBeat;
      lastBeat = now;
      bpm = 60000.0 / interval;   // calculate BPM
      bpmAvg = (bpmAvg * 0.8) + (bpm * 0.2);  // smooth average
      beatCount++;
    }
  }

  // Display BPM
  display.clearDisplay();
  display.setCursor(0, 20);
  display.print("BPM: ");
  display.println((int)bpmAvg);
  display.display();

  Serial.println(bpmAvg);
}
