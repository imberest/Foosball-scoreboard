// hardware.cpp
// Implements all hardware initialisation and control functions.
// All other files interact with hardware through the functions declared in hardware.h

// ── Includes ──────────────────────────────────────────────────────────────────
#include "hardware.h"   // Own header — pulls in config.h, TFT_eSPI, Wire, TAMC_GT911
#include <Arduino.h>    // Core Arduino functions (pinMode, analogWrite, etc.)

// ── Shared Object Definitions ─────────────────────────────────────────────────
// These are the actual instances — extern declarations in hardware.h point here
TFT_eSPI   tft = TFT_eSPI();
TAMC_GT911 tp  = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, SCREEN_W, SCREEN_H);

// ── Display ───────────────────────────────────────────────────────────────────

void displaySetup() {
  tft.init();             // Initialise TFT driver
  tft.setRotation(3);     // Landscape orientation (adjust if needed)
  tft.fillScreen(TFT_BLACK); // Clear screen to black on startup
}

// ── LED ───────────────────────────────────────────────────────────────────────
// RGB LED is common-anode — values are inverted (255 = off, 0 = full brightness)

void ledSetup() {
  pinMode(LED_RED,   OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE,  OUTPUT);
  ledOff();   // Ensure LED starts off
}

void ledSet(uint8_t r, uint8_t g, uint8_t b) {
  // Invert values for common-anode LED
  analogWrite(LED_RED,   255 - r);
  analogWrite(LED_GREEN, 255 - g);
  analogWrite(LED_BLUE,  255 - b);
}

// ── LED Presets ───────────────────────────────────────────────────────────────

void ledOff()    { ledSet(0,   0,   0);   }
void ledRed()    { ledSet(255, 0,   0);   }
void ledGreen()  { ledSet(0,   255, 0);   }
void ledBlue()   { ledSet(0,   0,   255); }
void ledWhite()  { ledSet(255, 255, 255); }
void ledOrange() { ledSet(255, 165, 0);   }
void ledPurple() { ledSet(128, 0,   255); }

// ── Sensors ───────────────────────────────────────────────────────────────────

void sensorsSetup() {
  // GPIO34 is input-only — no pinMode needed, just configure ADC
  pinMode(LIGHT_SENSOR_PIN, ANALOG);
  analogSetPinAttenuation(LIGHT_SENSOR_PIN, ADC_11db); // Full range: 0–3.3V
}

int readLight() {
  return analogRead(LIGHT_SENSOR_PIN);  // Returns 0 (dark) → 4095 (bright)
}

// ── Touch ─────────────────────────────────────────────────────────────────────

// Touch Debounce
unsigned long lastTouchTime = 0;
const unsigned long touchDebounceMs = 250;


void touchSetup() {
  Wire.begin(TOUCH_SDA, TOUCH_SCL);  // Start I2C on defined pins
  tp.begin();                         // Initialise GT911 touch controller
  tp.setRotation(3);    // Match display orientation
}

bool touchRead(int &x, int &y) {
  tp.read();                  // Poll touch controller
  if (tp.isTouched) {
    unsigned long now = millis();
    if (now - lastTouchTime > touchDebounceMs) {
      lastTouchTime = now;
      uint16_t tx = tp.points[0].x;
      uint16_t ty = tp.points[0].y;
      #if DEBUG_TOUCH
        Serial.print("[TOUCH] raw x=");
        Serial.print(tx);
        Serial.print("  y=");
        Serial.print(ty);
        Serial.print("  page=");
        Serial.println(currentPage);
      #endif
      processTouch(tx, ty);
    }
    return true;              // Touch detected
  }
  return false;               // No touch
}

// ==========================================
// TOUCH ROUTING
// ==========================================
void processTouch(uint16_t tx, uint16_t ty) {
  switch (currentPage) {
    case PAGE_START:    handleTouchStart(tx, ty);    break;
    case PAGE_SETTINGS: handleTouchSettings(tx, ty); break;
    case PAGE_GAME:     handleTouchGame(tx, ty);     break;
  }
}