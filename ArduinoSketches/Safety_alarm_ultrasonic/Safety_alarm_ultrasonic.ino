// Pins
const int trigPin = 9;
const int echoPin = 10;
const int buzzerPin = 6;
const int ledPin = 5;

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  // convert to cm

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Alarm condition
  if (distance > 0 && distance < 20) {   // intruder closer than 20cm
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000);  // buzzer ON
  } else {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);      // buzzer OFF
  }

  delay(200);
}
