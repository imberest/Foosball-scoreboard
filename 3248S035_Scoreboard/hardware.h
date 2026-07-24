#pragma once

// ── External Libraries ────────────────────────────────────────────────────────
#include "config.h"       // Pin definitions, screen size, and project constants
#include <TFT_eSPI.h>     // TFT display driver
#include <Wire.h>         // I2C communication (used by touch controller)
#include <TAMC_GT911.h>   // GT911 capacitive touch controller driver


#include "page_start.h"      // handleTouchStart
#include "page_settings.h"   // handleTouchSettings
#include "page_game.h"       // handleTouchGame

extern Page currentPage;     // currentPage visible in hardware.cpp

// ── Shared Hardware Objects ───────────────────────────────────────────────────
// Defined once in hardware.cpp — declared here so any file that includes
// hardware.h can access tft and tp directly
extern TFT_eSPI   tft;   // TFT display object
extern TAMC_GT911 tp;    // Touch controller object

// ── Display ───────────────────────────────────────────────────────────────────
void displaySetup();      // Initialise display, set rotation, clear screen

// ── LED ───────────────────────────────────────────────────────────────────────
void ledSetup();                          // Set LED pins as outputs, turn off
void ledSet(uint8_t r, uint8_t g, uint8_t b); // Set LED to any RGB colour (0–255)
void ledOff();                            // Turn LED off
void ledRed();                            // Preset: red
void ledGreen();                          // Preset: green
void ledBlue();                           // Preset: blue
void ledWhite();                          // Preset: white
void ledOrange();                         // Preset: orange
void ledPurple();                         // Preset: purple

// ── Sensors ───────────────────────────────────────────────────────────────────
void sensorsSetup();  // Configure light sensor pin and ADC attenuation
int  readLight();     // Read ambient light level — returns 0 (dark) to 4095 (bright)

// ── Touch ─────────────────────────────────────────────────────────────────────
void touchSetup();              // Initialise I2C and GT911 touch controller
bool touchRead(int &x, int &y); // Read touch — returns true if touched, fills x and y
void processTouch(uint16_t tx, uint16_t ty);

