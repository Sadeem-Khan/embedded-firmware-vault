#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

MCUFRIEND_kbv tft;

// Touchscreen pins for most 2.4" TFT shields
#define YP A3 
#define XM A2 
#define YM 9  
#define XP 8  

// Pressure thresholds
#define MINPRESSURE 10
#define MAXPRESSURE 1000

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Game Variables
int targetX, targetY;
int targetSize = 40;
int score = 0;
float speed = 2.0;
bool gameRunning = true;

void setup() {
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(1); // Landscape
    resetGame();
}

void resetGame() {
    tft.fillScreen(0x0000); // Black
    score = 0;
    speed = 2.0;
    spawnTarget();
    updateScore();
}

void spawnTarget() {
    // Erase old target
    tft.fillRect(targetX, targetY, targetSize, targetSize, 0x0000);
    // New position
    targetX = random(0, tft.width() - targetSize);
    targetY = 0;
}

void updateScore() {
    tft.fillRect(0, 0, 100, 20, 0x0000);
    tft.setCursor(5, 5);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(2);
    tft.print("Score: ");
    tft.print(score);
}

void loop() {
    // 1. Move target down
    tft.fillRect(targetX, targetY, targetSize, targetSize, 0x0000); // Clear old
    targetY += (int)speed;
    tft.fillRect(targetX, targetY, targetSize, targetSize, 0xF800); // Red Square

    // 2. Check for "Ground" Hit (Game Over)
    if (targetY + targetSize >= tft.height()) {
        tft.fillScreen(0xF800);
        tft.setCursor(50, 100);
        tft.setTextSize(3);
        tft.setTextColor(0xFFFF);
        tft.print("GAME OVER!");
        delay(2000);
        resetGame();
    }

    // 3. Check for Touch
    TSPoint p = ts.getPoint();
    
    // Restore pins after touch (needed for some TFT shields)
    pinMode(YP, OUTPUT);
    pinMode(XM, OUTPUT);

    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
        // Map touch coordinates to screen coordinates
        // Note: You might need to adjust these map values based on your specific screen
        int touchX = map(p.y, 900, 100, 0, tft.width());
        int touchY = map(p.x, 900, 100, 0, tft.height());

        // Check if touch is inside the square
        if (touchX >= targetX && touchX <= (targetX + targetSize) &&
            touchY >= targetY && touchY <= (targetY + targetSize)) {
            
            score++;
            speed += 0.2; // Increase difficulty
            spawnTarget();
            updateScore();
        }
    }
    delay(10); 
}