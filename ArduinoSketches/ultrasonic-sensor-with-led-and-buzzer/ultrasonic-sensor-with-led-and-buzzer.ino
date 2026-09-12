const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 3;
const int led = 4;

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Clear trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send 10us pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo
  duration = pulseIn(echoPin, HIGH);

  // Convert to cm
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 10) {
    digitalWrite(led, HIGH);
    tone(buzzer, 1000);  // Fast beep
    delay(100);
    noTone(buzzer);
  } 
  else if (distance < 20) {
    digitalWrite(led, HIGH);
    tone(buzzer, 1000);  // Slow beep
    delay(400);
    noTone(buzzer);
  } 
  else {
    digitalWrite(led, LOW);
    noTone(buzzer);
  }

  delay(200);
}
