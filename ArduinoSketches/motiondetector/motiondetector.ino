const int pirPin = 25;   // PIR sensor pin
int state = 0;           // Motion state

void setup() {
  pinMode(pirPin, INPUT);      // Set PIR pin to input
  Serial.begin(9600);          // Start serial communication
}

void loop() {
  state = digitalRead(pirPin); // Read PIR state
  if (state == HIGH) {
    Serial.println("Somebody here!"); // Motion detected
  } else {
    Serial.println("Monitoring...");  // No motion
  }
  delay(100);  // Delay between checks
}
