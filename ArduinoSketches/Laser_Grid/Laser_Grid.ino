
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int ldrPin = 34;        // ESP32 ADC pin for LDR
int threshold = 2000;   // Adjust based on light intensity

const char* ssid = "naveedhouse";
const char* password = "7738214945";

void setup() {
  Serial.begin(115200);

  // WiFi Connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // OLED Setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("LASER GRID");
  display.display();
  delay(2000);
}

void loop() {
  int ldrValue = analogRead(ldrPin);
  Serial.println(ldrValue);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("LASER GRID");

  if (ldrValue > threshold) {
    display.setCursor(0, 30);
    display.println("SAFE");
  } else {
    display.setCursor(0, 30);
    display.println("INTRUDER!");
  }
  display.display();

  delay(200);
}
