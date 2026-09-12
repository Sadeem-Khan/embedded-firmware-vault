int ldrPin = A0;     // LDR connected to A0
int ledPin = 9;      // LED connected to D9
int ldrValue = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  ldrValue = analogRead(ldrPin);  // read LDR value
  Serial.println(ldrValue);       // print value for testing

  // ✅ FIXED condition
  if (ldrValue > 500) {    // if value is high = dark
    digitalWrite(ledPin, HIGH);   // turn ON LED
  } else {
    digitalWrite(ledPin, LOW);    // turn OFF LED
  }

  delay(200);
}
