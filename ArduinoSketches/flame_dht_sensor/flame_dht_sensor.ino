#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "DHT.h"

// Pin Definitions
#define DHTPIN 6
#define DHTTYPE DHT11
#define FLAME_A A2
#define FLAME_D 3

// Using the Page Buffer (_1_) to save RAM
// HW_I2C uses the A4/A5 pins you just verified with the scanner
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  dht.begin();
  pinMode(FLAME_D, INPUT);
  
  u8g2.begin();
  u8g2.setI2CAddress(0x3C * 2); // Matches your scanner result
}

void loop() {
  // Read Sensors
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int flameValue = analogRead(FLAME_A);
  int flameAlert = digitalRead(FLAME_D);

  // If DHT fails, we still want to see Flame data, so we don't "return"
  bool dhtValid = !(isnan(h) || isnan(t));

  // --- Display Loop ---
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tf); 
    u8g2.drawStr(0, 10, "SYSTEM CHECK: OK");
    u8g2.drawHLine(0, 12, 128);

    // Temp & Humidity
    u8g2.setCursor(0, 30);
    if(dhtValid) {
      u8g2.print("Temp: "); u8g2.print(t, 1); u8g2.print("C");
    } else {
      u8g2.print("DHT Sensor Error");
    }
    
    u8g2.setCursor(0, 42);
    if(dhtValid) {
      u8g2.print("Hum:  "); u8g2.print(h, 1); u8g2.print("%");
    }

    // Flame Value
    u8g2.setCursor(0, 54);
    u8g2.print("Flame Val: "); u8g2.print(flameValue);

    // Logic for Alert
    u8g2.setCursor(0, 64);
    if (flameAlert == LOW || flameValue > 100) {
      u8g2.print("STATUS: **FIRE!!**");
    } else {
      u8g2.print("STATUS: Safe");
    }
  } while (u8g2.nextPage());

  delay(1000); 
}