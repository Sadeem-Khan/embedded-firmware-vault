const int irPin = 26; // GPIO26
int state = 0;

void setup() {
  pinMode(irPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  state = digitalRead(irPin);

  if (state == LOW) {
    Serial.println("Obstacle detected!");
  } else {
    Serial.println("Clear path.");
  }

  delay(100);
}
