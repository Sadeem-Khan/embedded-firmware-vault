int buttonPin = 2;

void setup() {
  Serial.begin(9600);       // start Serial Monitor
  pinMode(buttonPin, INPUT_PULLUP);
  randomSeed(analogRead(0)); // random seed
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {  // button pressed
    int dice = random(1, 7); // random number 1-6
    Serial.print("You rolled: ");
    Serial.println(dice);
    delay(500); // debounce
  }
}
