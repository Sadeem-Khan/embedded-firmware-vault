#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define BTN 2
#define BUZZER 9

// Game variables
int dinoY = 48;     // ground position
int jump = 0;
int obstacleX = 120;
int score = 0;
bool leg = false;
bool gameStarted = false;
bool gameOver = false;

void setup() {
  pinMode(BTN, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  startScreen();
}

// ================================
// START SCREEN ANIMATION
// ================================
void startScreen() {
  for (int i = -40; i <= 40; i++) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(30 - i, 20);
    display.println("DINO");
    display.setCursor(18 + i, 40);
    display.setTextSize(1);
    display.println("PRESS BUTTON");
    display.display();
    delay(30);
  }

  while (digitalRead(BTN) == HIGH); // wait press
  gameStarted = true;
}

// ================================
// DRAW DINO WITH LEG ANIMATION
// ================================
void drawDino(int x, int y) {
  display.fillRect(x, y, 10, 15, WHITE); // body

  if (leg)
    display.fillRect(x + 2, y + 15, 3, 4, WHITE);
  else
    display.fillRect(x + 5, y + 15, 3, 4, WHITE);

  leg = !leg;
}

// ================================
// GAME OVER ANIMATION
// ================================
void showGameOver() {
  display.clearDisplay();
  for (int i = -60; i <= 32; i++) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(32, i);
    display.setTextColor(WHITE);
    display.println("GAME");
    display.setCursor(32, i + 25);
    display.println("OVER");
    display.display();
    delay(20);
  }

  display.setTextSize(1);
  display.setCursor(30, 55);
  display.print("SCORE: ");
  display.print(score);
  display.display();

  tone(BUZZER, 200, 400); // sad sound
  delay(1500);
  while (digitalRead(BTN) == HIGH); // wait press
  resetGame();
}

// RESET
void resetGame() {
  obstacleX = 120;
  dinoY = 48;
  jump = 0;
  score = 0;
  gameOver = false;
}

// ================================
// MAIN LOOP
// ================================
void loop() {
  if (!gameStarted) return;

  display.clearDisplay();

  // Jump
  if (digitalRead(BTN) == LOW && jump == 0) {
    jump = 18;
    tone(BUZZER, 900, 100);
  }

  if (jump > 0) {
    dinoY -= 3;
    jump--;
  } else if (dinoY < 48) {
    dinoY += 3;
  }

  drawDino(10, dinoY);

  // Obstacle (cactus)
  display.fillRect(obstacleX, 50, 8, 12, WHITE);
  obstacleX -= 4;

  // Reset obstacle and score increment
  if (obstacleX < -10) {
    obstacleX = 128;
    score++;
  }

  // Ground line
  display.drawLine(0, 63, 127, 63, WHITE);

  // Score
  display.setTextSize(1);
  display.setCursor(100, 0);
  display.print(score);

  // Collision
  if (obstacleX < 20 && obstacleX > 5 && dinoY > 35) {
    gameOver = true;
  }

  display.display();
  delay(40);

  if (gameOver) {
    showGameOver();
  }
}
