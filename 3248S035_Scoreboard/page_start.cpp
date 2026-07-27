// page.start.cpp
#include "page_start.h"

// ── Layout constants ──────────────────────────────────────
#define BTN_X        30
#define BTN_W        140
#define BTN_H        70
#define BTN_RADIUS   10
#define BTN_START_Y  90
#define BTN_SETT_Y   190

#define PANEL_X      200
#define PANEL_Y      90       // ← top-aligned with START button
#define PANEL_W      260
#define PANEL_H      150      // ← smaller box

// ── Buttons ───────────────────────────────────────────────
void drawStartBtn() {
  tft.fillRoundRect(BTN_X, BTN_START_Y, BTN_W, BTN_H, BTN_RADIUS, TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(2);
  tft.setTextSize(2);
  tft.setCursor(BTN_X + 28, BTN_START_Y + 22);
  tft.print("START");
}

void drawSettingsBtn() {
  tft.fillRoundRect(BTN_X, BTN_SETT_Y, BTN_W, BTN_H, BTN_RADIUS, TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(2);
  tft.setTextSize(2);
  tft.setCursor(BTN_X + 10, BTN_SETT_Y + 22);
  tft.print("SETTINGS");
}

  const char* modeToString(GameMode mode) {   // ← GameMode enum, not uint8_t
  switch (mode) {
    case MODE_SINGLE:    return "Single";
    case MODE_BEST_OF_3: return "Best of 3";
    case MODE_BEST_OF_5: return "Best of 5";
    default:             return "Unknown";
    }
  }

// ── Settings Panel ────────────────────────────────────────
void drawStartPanel() {
  // Panel border
  tft.drawRoundRect(PANEL_X, PANEL_Y, PANEL_W, PANEL_H, 10, TFT_YELLOW);

  // Panel title — centred in yellow box
  const char* title = "-- GAME SETTINGS --";
  tft.setTextColor(TFT_YELLOW);
  tft.setTextFont(2);
  tft.setTextSize(1);

  int titleW = tft.textWidth(title);                        // measure string width in pixels
  int titleX = PANEL_X + (PANEL_W - titleW) / 2;           // centre within panel
  int titleY = PANEL_Y + 8;                                 // keep same vertical position

  tft.setCursor(titleX, titleY);
  tft.print(title);

  // Settings rows — size 2 now
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);           // ← bumped up



  tft.setCursor(PANEL_X + 10, PANEL_Y + 35);
  tft.print("Mode: ");
  tft.print(modeToString(currentSettings.mode));

  tft.setCursor(PANEL_X + 10, PANEL_Y + 70);
  tft.print("Points: ");
  tft.print(currentSettings.points);
}

// ── Full Page ─────────────────────────────────────────────
void drawPageStart() {   // ← add parameter
  tft.fillScreen(TFT_BLACK);

  // ── Title — centered top ──────────────────────────────
  tft.setTextDatum(TC_DATUM);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextFont(7);
  tft.setTextSize(1);
  tft.drawString("BBQ", 240, 5);

  tft.setTextFont(2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.drawString("BBQ FOOSBALL", 240, 45);

  tft.setTextDatum(TL_DATUM);

  drawStartBtn();
  drawSettingsBtn();
  drawStartPanel();   // ← pass it down
}

void handleTouchStart(uint16_t tx, uint16_t ty) {

  // START button hit?
  if (tx >= BTN_X && tx <= BTN_X + BTN_W &&
      ty >= BTN_START_Y && ty <= BTN_START_Y + BTN_H) {
    currentPage = PAGE_GAME;
    drawPageGame();
    return;
  }

  // SETTINGS button hit?
  if (tx >= BTN_X && tx <= BTN_X + BTN_W &&
      ty >= BTN_SETT_Y && ty <= BTN_SETT_Y + BTN_H) {
    currentPage = PAGE_SETTINGS;
    drawPageSettings();
    return;
  }

  
}