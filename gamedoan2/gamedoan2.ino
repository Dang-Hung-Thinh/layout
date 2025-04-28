// === ESP32 MULTI-GAME PROJECT ===
// MENU + Flappy Bird + Reaction + Pong + Dodge Blocks
// OLED 128x64 + 3 Buttons (UP, DOWN, SELECT)

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Buttons
#define BTN_UP     18
#define BTN_DOWN   19
#define BTN_SELECT 23

const char* games[] = {"Flappy Bird", "Reaction", "Pong", "Dodge Blocks"};
int menuIndex = 0;
const int totalGames = sizeof(games) / sizeof(games[0]);

// Game states
bool runningGame = false;

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);

  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while (1);
  }
  display.clearDisplay();
  display.display();
  showMenu();
}

void loop() {
  if (runningGame) return; // skip menu if game running

  if (digitalRead(BTN_UP) == LOW) {
    menuIndex = (menuIndex - 1 + totalGames) % totalGames;
    showMenu();
    delay(200);
  }
  if (digitalRead(BTN_DOWN) == LOW) {
    menuIndex = (menuIndex + 1) % totalGames;
    showMenu();
    delay(200);
  }
  if (digitalRead(BTN_SELECT) == LOW) {
    runningGame = true;
    runGame(menuIndex);
    runningGame = false;
    showMenu();
    delay(200);
  }
}

void showMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  for (int i = 0; i < totalGames; i++) {
    if (i == menuIndex) display.setCursor(0, i * 12), display.print("> ");
    else display.setCursor(12, i * 12);
    display.println(games[i]);
  }
  display.display();
}

void runGame(int index) {
  switch (index) {
    case 0: flappyBird(); break;
    case 1: reactionGame(); break;
    case 2: pongGame(); break;
    case 3: dodgeBlocks(); break;
  }
}

// === Game 1: Flappy Bird ===
void flappyBird() {
  int birdY = 30, gravity = 1, velocity = 0;
  int pipeX = 128, gapY = 20;
  while (true) {
    if (digitalRead(BTN_SELECT) == LOW) velocity = -5;
    velocity += gravity;
    birdY += velocity;
    pipeX--;
    if (pipeX < -10) {
      pipeX = 128;
      gapY = random(10, 40);
    }
    if (birdY < 0 || birdY > 63 || (pipeX < 16 && (birdY < gapY || birdY > gapY + 20))) break;
    display.clearDisplay();
    display.fillRect(pipeX, 0, 10, gapY, SSD1306_WHITE);
    display.fillRect(pipeX, gapY + 20, 10, 64 - gapY - 20, SSD1306_WHITE);
    display.fillCircle(16, birdY, 2, SSD1306_WHITE);
    display.display();
    delay(40);
  }
  gameOver();
}

// === Game 2: Reaction Game ===
void reactionGame() {
  display.clearDisplay();
  display.setCursor(10, 30);
  display.print("Wait for it...");
  display.display();
  delay(random(2000, 5000));
  display.clearDisplay();
  display.setCursor(10, 30);
  display.print("PRESS!");
  display.display();
  unsigned long start = millis();
  while (digitalRead(BTN_SELECT) == HIGH);
  unsigned long reaction = millis() - start;
  display.clearDisplay();
  display.setCursor(10, 30);
  display.print("Time: "); display.print(reaction); display.print("ms");
  display.display();
  delay(2000);
}

// === Game 3: Pong ===
void pongGame() {
  int paddleY = 24, ballX = 64, ballY = 32;
  int ballSpeedX = -2, ballSpeedY = 1;
  while (true) {
    if (digitalRead(BTN_UP) == LOW && paddleY > 0) paddleY -= 2;
    if (digitalRead(BTN_DOWN) == LOW && paddleY < SCREEN_HEIGHT - 16) paddleY += 2;
    ballX += ballSpeedX; ballY += ballSpeedY;
    if (ballY <= 0 || ballY >= SCREEN_HEIGHT - 2) ballSpeedY = -ballSpeedY;
    if (ballX <= 2 && ballY >= paddleY && ballY <= paddleY + 16) ballSpeedX = -ballSpeedX;
    if (ballX < 0 || ballX > SCREEN_WIDTH) break;
    display.clearDisplay();
    display.fillRect(0, paddleY, 2, 16, SSD1306_WHITE);
    display.fillCircle(ballX, ballY, 2, SSD1306_WHITE);
    display.display();
    delay(20);
  }
  gameOver();
}

// === Game 4: Dodge Blocks ===
void dodgeBlocks() {
  int playerY = 30, blockX = 128, blockY = random(0, 56);
  while (true) {
    if (digitalRead(BTN_UP) == LOW && playerY > 0) playerY -= 2;
    if (digitalRead(BTN_DOWN) == LOW && playerY < 56) playerY += 2;
    blockX -= 3;
    if (blockX < 0) {
      blockX = 128;
      blockY = random(0, 56);
    }
    if (blockX < 16 && abs(blockY - playerY) < 8) break;
    display.clearDisplay();
    display.fillRect(10, playerY, 6, 6, SSD1306_WHITE);
    display.fillRect(blockX, blockY, 6, 6, SSD1306_WHITE);
    display.display();
    delay(30);
  }
  gameOver();
}

void gameOver() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(25, 25);
  display.println("Game Over");
  display.display();
  delay(1500);
}
