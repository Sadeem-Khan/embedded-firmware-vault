#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    for(;;); 
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // Line 1: Main hook
  showLyrics("Manne Sambh-", "Sambh Rakhe", 2);
  delay(2000);

  showLyrics("Tere Jhanjran", "Ke Jode", 2);
  delay(2000);

  showLyrics("Jeene Koni", "Deti Haaye", 2);
  delay(1500);

  showLyrics("BAIRAN", "TERI AAYI", 3); // Large text for the drop
  delay(2500);

  showLyrics("Geeta Mein Gayi", "Kade Chhaati", 1);
   delay(1500);
   
  showLyrics("KE LAGAYI", "MANNE", 2);
  delay(2500);

  showLyrics("Jit Bhi Gaya Re", "Teri Yaad", 2);
  delay(2000);

  showLyrics("KHADI PAYI", "MANNE...", 2);
  delay(3000);
}

void showLyrics(String line1, String line2, int textSize) {
  display.clearDisplay();
  display.setTextSize(1);
  
  // Center Line 1
  int16_t x1, y1;
  uint16_t w1, h1;
  display.getTextBounds(line1, 0, 0, &x1, &y1, &w1, &h1);
  display.setCursor((160 - w1) / 2 - 16, 15); // Adjust for 128px center
  display.print(line1);

  // Center Line 2 (If it exists)
  if(line2 != ""){
    uint16_t w2, h2;
    display.getTextBounds(line2, 0, 0, &x1, &y1, &w2, &h2);
    display.setCursor((160 - w2) / 2 - 16, 40);
    display.print(line2);
  }
  
  display.display();
}