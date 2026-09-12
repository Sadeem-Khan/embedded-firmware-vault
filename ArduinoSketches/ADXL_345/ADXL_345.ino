#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

/* ---------- LCD ---------- */
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* ---------- RFID ---------- */
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

/* ---------- SERVO ---------- */
Servo lockServo;
#define SERVO_PIN 6

/* ---------- PASSWORD ---------- */
String password = "5050";
String input = "";

/* ---------- KEYPAD (3x4) ---------- */
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {A0, A1, A2, A3};
byte colPins[COLS] = {2, 3, 4};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/* ---------- RFID UID ---------- */
byte validUID[4] = {0x72, 0x11, 0xB3, 0xAB};

/* ---------- SETUP ---------- */
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Door Lock System");
  lcd.setCursor(0,1);
  lcd.print("Enter PIN/RFID");

  lockServo.attach(SERVO_PIN);
  lockServo.write(0);   // locked

  SPI.begin();
  rfid.PCD_Init();
}

/* ---------- LOOP ---------- */
void loop() {
  keypadCheck();
  rfidCheck();
}

/* ---------- KEYPAD ---------- */
void keypadCheck() {
  char key = keypad.getKey();

  if (key) {
    if (key == '#') {
      if (input == password) unlock();
      else wrong();
      input = "";
    }
    else if (key == '*') {
      input = "";
      lcd.clear();
      lcd.print("Cleared");
      delay(1000);
      lcd.clear();
      lcd.print("Enter PIN:");
    }
    else {
      input += key;
      lcd.setCursor(0,1);
      lcd.print(input);
    }
  }
}

/* ---------- RFID ---------- */
void rfidCheck() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  bool match = true;
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != validUID[i]) {
      match = false;
    }
  }

  if (match) unlock();
  else wrong();

  rfid.PICC_HaltA();
}

/* ---------- UNLOCK ---------- */
void unlock() {
  lcd.clear();
  lcd.print("Access Granted");

  lockServo.write(0);   // unlock
  delay(3000);
  lockServo.write(90);    // lock

  lcd.clear();
  lcd.print("Enter PIN/RFID");
}

/* ---------- WRONG ---------- */
void wrong() {
  lcd.clear();
  lcd.print("Access Denied");
  delay(2000);
  lcd.clear();
  lcd.print("Try Again");
}