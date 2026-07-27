// page_settings.cpp
#include "page_settings.h"

// ── Layout constants ──────────────────────────────────────
#define BTN_X        200
#define BTN_W        140
#define BTN_H        70
#define BTN_RADIUS   10
#define BTN_START_Y  250
#define BTN_SETT_Y   190

#define PANEL_X      200
#define PANEL_Y      90       // ← top-aligned with START button
#define PANEL_W      260
#define PANEL_H      150      // ← smaller box


void drawSettingsBackBtn(){
  tft.fillRoundRect(BTN_X, BTN_START_Y, BTN_W, BTN_H, BTN_RADIUS, TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(2);
  tft.setTextSize(2);
  tft.setCursor(BTN_X + 28, BTN_START_Y + 22);
  tft.print("BACK");
}


void drawPageSettings() {
  tft.fillScreen(TFT_BLACK); // Clear screen to black on startup
  tft.setTextFont(2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.drawString("SETTINGS PAGE", 240, 45);
  drawSettingsBackBtn();
}

void handleTouchSettings(uint16_t tx, uint16_t ty){
  // BACK button hit?
  if (tx >= BTN_X && tx <= BTN_X + BTN_W &&
      ty >= BTN_START_Y && ty <= BTN_START_Y + BTN_H) {
    currentPage = PAGE_START;
    drawPageStart();
    return;
  }
}
