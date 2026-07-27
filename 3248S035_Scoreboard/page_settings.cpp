// page_settings.cpp
#include "page_settings.h"
#include "nvs_manager.h" // Includes nvs_save and nvs_clear

// ── Layout Constants ──────────────────────────────────────
#define TITLE_Y       25

// Game Mode Row
#define MODE_Y        80
#define MODE_BTN_W    130
#define MODE_BTN_H    40

// Target Points Row
#define POINTS_Y      160
#define POINTS_BTN_W  100
#define POINTS_BTN_H  40

// Remember Toggle Row
#define REM_Y         225
#define REM_BTN_W     120
#define REM_BTN_H     35

// Back/Save Button
#define SAVE_BTN_W    120
#define SAVE_BTN_H    45
#define SAVE_BTN_X    ((SCREEN_W - SAVE_BTN_W) / 2)
#define SAVE_BTN_Y    265

// Save settings using nvs_manager helpers
void saveSettings() {
  if (currentSettings.remember) {
    nvs_save(currentSettings); // Writes mode, points, and remember flag
  } else {
    nvs_clear();               // Clears saved keys if user opts out
  }
}

// ── Drawing Helpers ───────────────────────────────────────
void drawOptionButton(int x, int y, int w, int h, const char* label, bool active) {
  uint16_t bgColor   = active ? TFT_BLUE : TFT_DARKGREY;
  uint16_t textColor = TFT_WHITE;

  tft.fillRoundRect(x, y, w, h, 6, bgColor);
  tft.drawRoundRect(x, y, w, h, 6, active ? TFT_CYAN : TFT_WHITE);
  
  tft.setTextColor(textColor, bgColor);
  tft.setTextDatum(MC_DATUM);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.drawString(label, x + (w / 2), y + (h / 2));
}

void drawPageSettings() {
  tft.fillScreen(TFT_BLACK);

  // Title
  tft.setTextDatum(TC_DATUM);
  tft.setTextFont(4);
  tft.setTextSize(1);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.drawString("SETTINGS", SCREEN_W / 2, TITLE_Y);

  // 1. GAME MODE SECTION
  tft.setTextDatum(TL_DATUM);
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("MODE:", 20, MODE_Y - 20);

  drawOptionButton(20,  MODE_Y, MODE_BTN_W, MODE_BTN_H, "SINGLE",    currentSettings.mode == MODE_SINGLE);
  drawOptionButton(170, MODE_Y, MODE_BTN_W, MODE_BTN_H, "BEST OF 3", currentSettings.mode == MODE_BEST_OF_3);
  drawOptionButton(320, MODE_Y, MODE_BTN_W, MODE_BTN_H, "BEST OF 5", currentSettings.mode == MODE_BEST_OF_5);

  // 2. POINTS TARGET SECTION
  tft.drawString("FIRST TO:", 20, POINTS_Y - 20);

  drawOptionButton(20,  POINTS_Y, POINTS_BTN_W, POINTS_BTN_H, "5 PTS",  currentSettings.points == POINTS_5);
  drawOptionButton(140, POINTS_Y, POINTS_BTN_W, POINTS_BTN_H, "7 PTS",  currentSettings.points == POINTS_7);
  drawOptionButton(260, POINTS_Y, POINTS_BTN_W, POINTS_BTN_H, "9 PTS", currentSettings.points == POINTS_9);

  // 3. REMEMBER TOGGLE
  tft.drawString("SAVE CONFIG:", 20, REM_Y + 10);
  const char* remLabel = currentSettings.remember ? "REMEMBER: ON" : "REMEMBER: OFF";
  drawOptionButton(160, REM_Y, REM_BTN_W + 20, REM_BTN_H, remLabel, currentSettings.remember);

  // 4. SAVE & EXIT BUTTON
  tft.fillRoundRect(SAVE_BTN_X, SAVE_BTN_Y, SAVE_BTN_W, SAVE_BTN_H, 8, TFT_GREEN);
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.setTextDatum(MC_DATUM);
  tft.setTextFont(2);
  tft.setTextSize(2);
  tft.drawString("SAVE", SAVE_BTN_X + (SAVE_BTN_W / 2), SAVE_BTN_Y + (SAVE_BTN_H / 2));
}

// ── Touch Handler ─────────────────────────────────────────
void handleTouchSettings(uint16_t tx, uint16_t ty) {
  // --- 1. Mode Selection Hits ---
  if (ty >= MODE_Y && ty <= MODE_Y + MODE_BTN_H) {
    if (tx >= 20 && tx <= 20 + MODE_BTN_W) {
      currentSettings.mode = MODE_SINGLE;
    } else if (tx >= 170 && tx <= 170 + MODE_BTN_W) {
      currentSettings.mode = MODE_BEST_OF_3;
    } else if (tx >= 320 && tx <= 320 + MODE_BTN_W) {
      currentSettings.mode = MODE_BEST_OF_5;
    }
    drawPageSettings();
    return;
  }

  // --- 2. Points Target Hits ---
  if (ty >= POINTS_Y && ty <= POINTS_Y + POINTS_BTN_H) {
    if (tx >= 20 && tx <= 20 + POINTS_BTN_W) {
      currentSettings.points = POINTS_5;
    } else if (tx >= 140 && tx <= 140 + POINTS_BTN_W) {
      currentSettings.points = POINTS_7;
    } else if (tx >= 260 && tx <= 260 + POINTS_BTN_W) {
      currentSettings.points = POINTS_9;
    }
    drawPageSettings();
    return;
  }

  // --- 3. Remember Toggle Hit ---
  if (ty >= REM_Y && ty <= REM_Y + REM_BTN_H) {
    if (tx >= 160 && tx <= 160 + REM_BTN_W + 20) {
      currentSettings.remember = !currentSettings.remember;
      drawPageSettings();
    }
    return;
  }

  // --- 4. Save & Back Button Hit ---
  if (tx >= SAVE_BTN_X && tx <= SAVE_BTN_X + SAVE_BTN_W &&
      ty >= SAVE_BTN_Y && ty <= SAVE_BTN_Y + SAVE_BTN_H) {
    saveSettings();             // Triggers nvs_save() or nvs_clear()
    currentPage = PAGE_START;   // Navigate back
    drawPageStart();
    return;
  }
}