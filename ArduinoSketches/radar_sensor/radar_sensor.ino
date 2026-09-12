#include <Servo.h>

Servo myServo;        // create servo object
const int trigPin = 10;
const int echoPin = 11;
long duration;
int distance;

void setup() {
  myServo.attach(9);   // servo signal pin
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {

  // Sweep from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle++) {
    myServo.write(angle);
    delay(10);
    distance = getDistance();
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("°   Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(30);
  }

  // Sweep back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle--) {
    myServo.write(angle);
    delay(10);
    distance = getDistance();
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("°   Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(30);
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // distance in cm
}
