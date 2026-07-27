// config.h

#pragma once

#define DEBUG_TOUCH  true    // ← set false to silence touch logs

// ─────────────────────────────────────────
// RGB LED Pins
// ─────────────────────────────────────────
#define LED_RED    4
#define LED_GREEN  16
#define LED_BLUE   17

// ─────────────────────────────────────────
// Light Sensor
// ─────────────────────────────────────────
#define LIGHT_SENSOR_PIN 34   // GPIO34 - input only

// ─────────────────────────────────────────
// Touch Controller (GT911) - I2C
// ─────────────────────────────────────────
#define TOUCH_SDA  33
#define TOUCH_SCL  32
#define TOUCH_INT  21
#define TOUCH_RST  25

// ─────────────────────────────────────────
// TFT Display (set in TFT_eSPI user setup)
// ─────────────────────────────────────────
#define TFT_BL     27         // Backlight pin
#define SCREEN_W   480
#define SCREEN_H   320

// ─────────────────────────────────────────
// Serial
// ─────────────────────────────────────────
#define SERIAL_BAUD 115200

// ─── NVS / Persistence ───────────────────────────────
#define NVS_NAMESPACE       "game_cfg"
#define NVS_KEY_MODE        "game_mode"
#define NVS_KEY_POINTS      "points_target"
#define NVS_KEY_REMEMBER    "remember"

// ─── Pages ───────────────────────────────────────────
enum Page {
  PAGE_START,
  PAGE_SETTINGS,
  PAGE_GAME
};

enum GameMode {
  MODE_SINGLE,    // play to X points, first to reach wins
  MODE_BEST_OF_3, // first to win 2 games
  MODE_BEST_OF_5  // first to win 3 games
};

enum PointsTarget {
  POINTS_5  = 5,
  POINTS_7  = 7,
  POINTS_10 = 10
};

// ─────────────────────────────────────────
// Stored Settings Structure
// ─────────────────────────────────────────
struct GameSettings {
  GameMode     mode;
  PointsTarget points;
  bool         remember;
};

// Global game settings
extern GameSettings currentSettings;
extern Page currentPage;

// ─── Defaults ────────────────────────────────────────
#define DEFAULT_MODE    MODE_SINGLE
#define DEFAULT_POINTS  POINTS_7
#define DEFAULT_REMEMBER false       // default to not remembering
