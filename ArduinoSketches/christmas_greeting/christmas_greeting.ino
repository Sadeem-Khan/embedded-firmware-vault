#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pins
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int LDR_PIN = A0;
const int LED_PIN = 6;
const int BUZZER_PIN = 11;

// Animation & Greeting Variables
#define NUM_FLAKES 15
int flake_x[NUM_FLAKES], flake_y[NUM_FLAKES];
int greetingStep = 0;
unsigned long lastGreetingChange = 0;
String greetings[] = {"MERRY XMAS!", "HAPPY HOLIDAYS", "HO HO HO!", "BE GOOD!"};

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  for(int i=0; i<NUM_FLAKES; i++) {
    flake_x[i] = random(0, 128);
    flake_y[i] = random(0, 64);
  }
}

void playJingle() {
  int notes[] = {262, 262, 262, 262, 262, 262}; // Simple Jingle Bell start
  for (int i = 0; i < 6; i++) {
    tone(BUZZER_PIN, notes[i], 150);
    delay(200);
  }
  noTone(BUZZER_PIN);
}

void loop() {
  // 1. SENSOR READS
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH, 20000); 
  distance = (duration / 2) / 29.1;
  int lightLevel = analogRead(LDR_PIN);

  display.clearDisplay();

  // 2. GREETING LOGIC (Changes every 2 seconds)
  if (millis() - lastGreetingChange > 2000) {
    greetingStep = (greetingStep + 1) % 4;
    lastGreetingChange = millis();
  }

  // 3. SNOW ANIMATION (Always runs)
  for(int i=0; i<NUM_FLAKES; i++) {
    display.drawPixel(flake_x[i], flake_y[i], WHITE);
    flake_y[i] += 1;
    if(flake_y[i] > 64) flake_y[i] = 0;
  }

  // 4. MAIN LOGIC
  if (distance > 0 && distance < 25) {
    // ALERT MODE
    digitalWrite(LED_PIN, HIGH);
    display.setTextSize(2);
    display.setCursor(5, 20);
    display.print("GIFT ALERT!");
    display.display();
    playJingle(); 
  } 
  else if (lightLevel < 400) {
    // NIGHT MODE (Show Greetings)
    digitalWrite(LED_PIN, HIGH);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20, 25);
    display.print(greetings[greetingStep]);
    
    // Draw a small Christmas tree
    display.fillTriangle(100, 45, 120, 45, 110, 25, WHITE);
    display.fillRect(108, 45, 4, 4, WHITE);
  } 
  else {
    // DAY MODE
    digitalWrite(LED_PIN, LOW);
    display.setCursor(30, 25);
    display.print("Daytime Mode");
  }

  // 5. DISTANCE BAR (The "Pro" feature)
  if (distance > 0 && distance < 100) {
    int barWidth = map(distance, 0, 100, 0, 128);
    display.fillRect(0, 60, barWidth, 4, WHITE); // Bottom progress bar
  }

  display.display();
  delay(30); 
}