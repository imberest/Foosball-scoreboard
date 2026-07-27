// page_game.cpp
#include "page_game.h"
#include "config.h"

// ── Layout constants ──────────────────────────────────────
#define BTN_W        100
#define BTN_H        45
#define BTN_RADIUS   8
#define BTN_X        ((SCREEN_W - BTN_W) / 2) // Centered horizontally
#define BTN_Y        265                      // Positioned near bottom

int leftCounter = 0;
int rightCounter = 0;

// Match/Series tracking
int leftGames  = 0;
int rightGames = 0;
bool gameOver   = false;
bool matchOver  = false;

// History structure to hold completed set scores (Max 5 sets)
struct SetScore {
  int left;
  int right;
  bool completed;
};

SetScore seriesHistory[5];
int currentSetIndex = 0;

int getGamesToWinSeries() {
  if (currentSettings.mode == MODE_BEST_OF_3) return 2;
  if (currentSettings.mode == MODE_BEST_OF_5) return 3;
  return 1; // MODE_SINGLE
}

void drawGameBackBtn() {
  tft.fillRoundRect(BTN_X, BTN_Y, BTN_W, BTN_H, BTN_RADIUS, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextDatum(MC_DATUM);
  tft.setTextFont(2);
  tft.setTextSize(2);
  tft.drawString("BACK", BTN_X + (BTN_W / 2), BTN_Y + (BTN_H / 2));
}

// Draws two side-by-side squares per set vertically down the center column
void drawSeriesTracker() {
  if (currentSettings.mode == MODE_SINGLE) return;

  int maxGames = (currentSettings.mode == MODE_BEST_OF_3) ? 3 : 5;
  int subBoxW = 28;  // Width of each individual score box (Left / Right)
  int boxH    = 24;  // Height of each set row
  int gap     = 4;   // Small gap between left & right score boxes
  int spacing = 6;   // Vertical spacing between set rows
  
  // Total width of a set row: (28 * 2) + 4 = 60px total
  int totalRowW = (subBoxW * 2) + gap;
  int startX    = (SCREEN_W / 2) - (totalRowW / 2); // Center column horizontally
  int startY    = 60;                               // Top starting point

  for (int i = 0; i < maxGames; i++) {
    int boxY = startY + i * (boxH + spacing);
    int leftBoxX  = startX;
    int rightBoxX = startX + subBoxW + gap;

    if (seriesHistory[i].completed) {
      int leftScore  = seriesHistory[i].left;
      int rightScore = seriesHistory[i].right;

      // Determine Winner vs Loser Colors
      bool leftWon = leftScore > rightScore;

      uint16_t leftBg   = leftWon ? TFT_GREEN : TFT_NAVY;
      uint16_t leftTxt  = leftWon ? TFT_WHITE : TFT_SILVER;

      uint16_t rightBg  = !leftWon ? TFT_GREEN : TFT_NAVY;
      uint16_t rightTxt = !leftWon ? TFT_WHITE : TFT_SILVER;

      // --- Left Score Square ---
      tft.fillRect(leftBoxX, boxY, subBoxW, boxH, leftBg);
      tft.drawRect(leftBoxX, boxY, subBoxW, boxH, leftWon ? TFT_WHITE : TFT_DARKGREY);
      tft.setTextColor(leftTxt, leftBg);
      tft.setTextDatum(MC_DATUM);
      tft.setTextFont(1);
      tft.drawNumber(leftScore, leftBoxX + (subBoxW / 2), boxY + (boxH / 2));

      // --- Right Score Square ---
      tft.fillRect(rightBoxX, boxY, subBoxW, boxH, rightBg);
      tft.drawRect(rightBoxX, boxY, subBoxW, boxH, !leftWon ? TFT_WHITE : TFT_DARKGREY);
      tft.setTextColor(rightTxt, rightBg);
      tft.setTextDatum(MC_DATUM);
      tft.setTextFont(1);
      tft.drawNumber(rightScore, rightBoxX + (subBoxW / 2), boxY + (boxH / 2));

    } else {
      // --- Unplayed / Active Set Row ---
      uint16_t borderCol = (i == currentSetIndex) ? TFT_YELLOW : TFT_DARKGREY;

      // Left Box (Empty / Game Label)
      tft.fillRect(leftBoxX, boxY, subBoxW, boxH, TFT_BLACK);
      tft.drawRect(leftBoxX, boxY, subBoxW, boxH, borderCol);

      // Right Box (Empty / Game Label)
      tft.fillRect(rightBoxX, boxY, subBoxW, boxH, TFT_BLACK);
      tft.drawRect(rightBoxX, boxY, subBoxW, boxH, borderCol);

      // Label set number across the row for active/unplayed games (e.g. "G1")
      if (i == currentSetIndex) {
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.setTextFont(1);
        String label = "G" + String(i + 1);
        tft.drawString(label.c_str(), SCREEN_W / 2, boxY + (boxH / 2));
      }
    }
  }
}

void drawScores() {
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextFont(7);
  tft.setTextSize(4);

  // Left Score (1/5th screen width)
  tft.drawNumber(leftCounter, SCREEN_W / 5, SCREEN_H / 2 - 10);

  // Right Score (4/5ths screen width)
  tft.drawNumber(rightCounter, (SCREEN_W * 4) / 5, SCREEN_H / 2 - 10);
}

void drawPageGame() {
  leftCounter = 0;
  rightCounter = 0;
  leftGames = 0;
  rightGames = 0;
  currentSetIndex = 0;
  gameOver = false;
  matchOver = false;

  for (int i = 0; i < 5; i++) {
    seriesHistory[i].completed = false;
    seriesHistory[i].left = 0;
    seriesHistory[i].right = 0;
  }
  
  tft.fillScreen(TFT_BLACK);
  
  // Header Title
  tft.setTextDatum(TC_DATUM);
  tft.setTextFont(2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  
  int target = (int)currentSettings.points;
  String title = "GAME PAGE (FIRST TO " + String(target) + ")";
  tft.drawString(title.c_str(), SCREEN_W / 2, 15);

  drawScores();
  drawSeriesTracker();
  drawGameBackBtn();
}

void resetForNextGame() {
  leftCounter = 0;
  rightCounter = 0;
  gameOver = false;

  // Clear middle prompt line
  tft.fillRect(50, SCREEN_H - 80, SCREEN_W - 100, 30, TFT_BLACK);

  drawScores();
  drawSeriesTracker();
}

void handleTouchGame(uint16_t tx, uint16_t ty) {
  // 1. BACK button hit check
  if (tx >= BTN_X && tx <= BTN_X + BTN_W &&
      ty >= BTN_Y && ty <= BTN_Y + BTN_H) {
    currentPage = PAGE_START;
    drawPageStart();
    return;
  }

  int target = (int)currentSettings.points;

  // 2. Handle post-game or post-match tap
  if (gameOver || matchOver) {
    if (matchOver || currentSettings.mode == MODE_SINGLE) {
      drawPageGame();
    } else {
      resetForNextGame();
    }
    return;
  }

  // 3. Bottom dead-zone check (near BACK button)
  if (ty >= BTN_Y - 10) {
    return;
  }

  // 4. Middle dead-zone check for vertical set column
  uint16_t midLeftBound  = (SCREEN_W * 35) / 100;
  uint16_t midRightBound = (SCREEN_W * 65) / 100;

  if (tx >= midLeftBound && tx <= midRightBound) {
    return; 
  }

  // 5. Score Increment
  if (tx < midLeftBound) {
    if (leftCounter < target) leftCounter++;
  } else if (tx > midRightBound) {
    if (rightCounter < target) rightCounter++;
  }

  drawScores();

  // 6. Game or Match Won Check
  if (leftCounter >= target || rightCounter >= target) {
    gameOver = true;

    // Save final game score to set tracker
    if (currentSetIndex < 5) {
      seriesHistory[currentSetIndex].left = leftCounter;
      seriesHistory[currentSetIndex].right = rightCounter;
      seriesHistory[currentSetIndex].completed = true;
      currentSetIndex++;
    }

    if (leftCounter >= target)  leftGames++;
    if (rightCounter >= target) rightGames++;

    drawSeriesTracker();

    int gamesNeeded = getGamesToWinSeries();
    if (leftGames >= gamesNeeded || rightGames >= gamesNeeded || currentSettings.mode == MODE_SINGLE) {
      matchOver = true;
    }

    // Display Result Text
    tft.setTextDatum(TC_DATUM);
    tft.setTextFont(2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);

    if (matchOver) {
      if (leftGames > rightGames) {
        tft.drawString("LEFT WINS MATCH!", SCREEN_W / 2, SCREEN_H - 80);
      } else {
        tft.drawString("RIGHT WINS MATCH!", SCREEN_W / 2, SCREEN_H - 80);
      }
    } else {
      tft.drawString("TAP TO START NEXT GAME", SCREEN_W / 2, SCREEN_H - 80);
    }
  }
}