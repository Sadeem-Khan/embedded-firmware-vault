#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- DISPLAY CONFIGURATION ---
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // The I2C address for 128x64 typically 0x3C or 0x3D
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- HARDWARE PIN DEFINITIONS ---
#define BUTTON_PIN 2   // Jump button on Digital Pin 2
#define BUZZER_PIN 9   // Buzzer on Digital Pin 9

// --- GAME VARIABLES ---
bool isJumping = false;
bool gameOver = false;
int dinoY = 48; // Dino's vertical position (lower is better, 64-16)
const int groundY = 56; // The y-coordinate for the ground line
int velocity = 0;
const int gravity = 2;
const int jumpVelocity = -14;
const int dinoX = 10;
const int dinoSize = 8; // Dino is a simple 8x8 block for speed

// Cactus variables
int cactusX = SCREEN_WIDTH;
const int cactusWidth = 4;
const int cactusHeight = 8;
int gameSpeed = 3;
long score = 0;
unsigned long lastFrameTime = 0;
const int frameDelay = 30; // Milliseconds per frame (33ms is ~30 FPS)

// --- FUNCTION PROTOTYPES ---
void setupGame();
void drawDino();
void drawCactus();
void checkCollision();
void drawStartScreen();
void drawGameOverScreen();
void makeJumpSound();
void makeHitSound();

void setup() {
  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Button setup (use internal pull-up resistor)
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Buzzer setup
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Show a splash screen (Adafruit logo by default)
  display.display();
  delay(1000);
  
  drawStartScreen();
}

void loop() {
  if (gameOver) {
    // Wait for the button press to restart
    if (digitalRead(BUTTON_PIN) == LOW) {
      delay(50); // Debounce
      if (digitalRead(BUTTON_PIN) == LOW) {
        setupGame(); // Reset game
      }
    }
    return; // Stay in game over state until restart
  }

  // --- Game Loop Timing (Frame Rate Control) ---
  if (millis() - lastFrameTime < frameDelay) {
    return;
  }
  lastFrameTime = millis();
  
  // --- Input Handling (Jump) ---
  // Button is LOW when pressed due to INPUT_PULLUP
  if (digitalRead(BUTTON_PIN) == LOW && dinoY == groundY) {
    isJumping = true;
    velocity = jumpVelocity;
    makeJumpSound();
  }

  // --- Physics (Gravity) ---
  if (isJumping) {
    dinoY += velocity;
    velocity += gravity;
    if (dinoY >= groundY) {
      dinoY = groundY;
      isJumping = false;
      velocity = 0;
    }
  }

  // --- Move Cactus ---
  cactusX -= gameSpeed;
  if (cactusX < 0) {
    cactusX = SCREEN_WIDTH + random(30, 80); // Spawn new cactus randomly
    score += 10;
    // Increase speed every 100 points (10 successful jumps)
    if (score % 100 == 0 && gameSpeed < 8) {
        gameSpeed++;
    }
  }
  
  // --- Collision Detection ---
  checkCollision();

  // --- Drawing ---
  display.clearDisplay();
  
  // Draw Ground Line
  display.drawLine(0, groundY + dinoSize, SCREEN_WIDTH, groundY + dinoSize, SSD1306_WHITE);
  
  // Draw Dino (as a block)
  drawDino();
  
  // Draw Cactus (as a block)
  drawCactus();
  
  // Display Score
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);
  display.print(" Speed: ");
  display.print(gameSpeed);

  display.display();
}

// --- GAME FUNCTIONS ---

void setupGame() {
  randomSeed(analogRead(0)); // Good practice for better randomness
  isJumping = false;
  gameOver = false;
  dinoY = groundY;
  velocity = 0;
  cactusX = SCREEN_WIDTH;
  gameSpeed = 3;
  score = 0;
  display.clearDisplay();
  display.display();
}

void drawDino() {
    // Simple block dino. You could replace this with a bitmap array for animation.
    display.fillRect(dinoX, dinoY, dinoSize, dinoSize, SSD1306_WHITE);
}

void drawCactus() {
    // Simple block cactus.
    display.fillRect(cactusX, groundY + dinoSize - cactusHeight, cactusWidth, cactusHeight, SSD1306_WHITE);
}

void checkCollision() {
  // Check for horizontal overlap
  if (cactusX < dinoX + dinoSize && cactusX + cactusWidth > dinoX) {
    // Check for vertical overlap (dino's feet hit the top of the cactus)
    if (dinoY + dinoSize > groundY + dinoSize - cactusHeight) {
      gameOver = true;
      makeHitSound();
      drawGameOverScreen();
    }
  }
}

// --- ANIMATION/SCREEN FUNCTIONS ---

void drawStartScreen() {
    display.clearDisplay();
    display.setTextSize(2); // Large text
    display.setTextColor(SSD1306_WHITE);
    
    // Simple intro animation: Text scrolling in/out or a simple countdown
    for (int i = 0; i < 2; i++) {
        display.setCursor(20, 10);
        display.print("DINO JUMP");
        display.setTextSize(1);
        display.setCursor(15, 40);
        display.print("Press Button to Start");
        display.display();
        delay(500);
        display.clearDisplay();
        display.display();
        delay(500);
    }
    
    // Final screen before game starts
    display.clearDisplay();
    display.setTextSize(2); 
    display.setCursor(30, 10);
    display.print("READY!");
    display.setTextSize(1); 
    display.setCursor(35, 40);
    display.print("JUMP with D2");
    display.display();
    delay(2000);
    
    setupGame(); // Initialize game variables
}

void drawGameOverScreen() {
    display.clearDisplay();
    
    // Animated Game Over: A simple vertical wipe effect
    for(int i = 0; i < SCREEN_HEIGHT / 2; i += 4) {
        display.fillRect(0, SCREEN_HEIGHT/2 - i, SCREEN_WIDTH, i*2, SSD1306_WHITE);
        display.display();
    }

    // Draw text inside the white box (need to set color to BLACK for text)
    display.setTextColor(SSD1306_BLACK);
    display.setTextSize(3);
    display.setCursor(20, 10);
    display.print("GAME");
    display.setCursor(20, 35);
    display.print("OVER");
    
    // Draw final score outside the box
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(20, 58);
    display.print("Final Score: ");
    display.print(score);
    
    display.display();
}

// --- BUZZER SOUNDS ---

void makeJumpSound() {
  // Short, high-pitched tone
  tone(BUZZER_PIN, 800, 50); 
}

void makeHitSound() {
  // Quick, descending two-tone sound for collision
  tone(BUZZER_PIN, 500, 100);
  delay(100);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 200, 200);
  delay(200);
  noTone(BUZZER_PIN);
}