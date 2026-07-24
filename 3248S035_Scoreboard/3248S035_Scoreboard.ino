// main.ino
#include "config.h"
#include "hardware.h"
#include "nvs_manager.h"
#include "page_start.h"
#include "page_settings.h"
#include "page_game.h"

// ── App State ─────────────────────────────────────────────────────────────────
GameSettings currentSettings;
Page currentPage = PAGE_START;

// ── Page Renderer ─────────────────────────────────────────────────────────────
// Call whenever currentPage changes — redraws the active screen
void renderPage() {
  switch (currentPage) {
    case PAGE_START:    drawPageStart(currentSettings);    break;
    case PAGE_SETTINGS: drawPageSettings(); break;
    case PAGE_GAME:     drawPageGame();     break;
  }
}

// ── Setup ─────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(SERIAL_BAUD);

  // Hardware init
  ledSetup();
  sensorsSetup();
  displaySetup();
  touchSetup();

  // Load saved settings from NVS
  nvs_init();
  currentSettings = nvs_load();

  // If remember was off last session, reset to defaults
  if (!currentSettings.remember) {
    currentSettings.mode    = DEFAULT_MODE;
    currentSettings.points  = DEFAULT_POINTS;
    currentSettings.remember = DEFAULT_REMEMBER;
    Serial.println("Remember off — defaults loaded");
  } else {
    Serial.println("Settings restored from NVS");
  }

  // Draw initial screen
  renderPage();

  Serial.println("System ready!");
}

// ── Loop ──────────────────────────────────────────────────────────────────────
void loop() {
  int x, y;
  touchRead(x, y);    // handles debounce + routing internally
}