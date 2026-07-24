# 🎮 ESP32 Touch Game Console

A touch-screen game console built on the **Sunton 3248S035C** — a 3.5" 320×480 capacitive touch LCD with an integrated ESP32. Navigate between pages using touch input, configure game settings, and play — all from a clean, page-based UI.

---

## 📁 Project Structure

```
project/
├── main.ino              # Entry point — setup, loop, app state
├── config.h              # Global constants, flags, and settings
├── hardware.h/.cpp       # Display, touch controller, routing
├── nvs_manager.h/.cpp    # Non-volatile storage (save/load settings)
├── page_start.h/.cpp     # Start/home page UI and touch handling
├── page_settings.h/.cpp  # Settings page UI and touch handling
└── page_game.h/.cpp      # Game page UI and touch handling
```

---

## 🖥️ Hardware — Sunton 3248S035C

| Feature | Details |
|---|---|
| Board | ESP32-S3 (or ESP32 variant) |
| Display | 3.5" TFT ILI9488 — 320×480 |
| Touch | GT911 Capacitive Touch Controller |
| Interface | SPI (display) + I2C (touch) |
| Flash | 16MB |
| PSRAM | 8MB |
| USB | USB-C |

> 💡 The **C** suffix in 3248S035**C** means **Capacitive** touch (GT911).
> The **R** variant uses resistive touch instead.

---

## ⚙️ Configuration (`config.h`)

| Flag / Constant | Default | Description |
|---|---|---|
| `DEBUG_TOUCH` | `true` | Print raw touch coordinates to Serial |
| `SCREEN_W` | `480` | Display width in pixels |
| `SCREEN_H` | `320` | Display height in pixels |
| `touchDebounceMs` | `200` | Minimum ms between touch events |

> 💡 Set `DEBUG_TOUCH false` once your touch is calibrated.

---

## 📌 Pin Reference — 3248S035C

### Display (ILI9488 — SPI)

| Signal | GPIO |
|---|---|
| MOSI | 13 |
| SCLK | 14 |
| CS | 15 |
| DC | 2 |
| RST | 12 |
| BL (Backlight) | 27 |

### Touch (GT911 — I2C)

| Signal | GPIO |
|---|---|
| SDA | 33 |
| SCL | 32 |
| INT | 21 |
| RST | 25 |

> ⚠️ Pin numbers may vary slightly by board revision. Always verify against your specific board's schematic.

---

## 🔧 Library Setup

### TFT_eSPI

In your `User_Setup.h` (inside the TFT_eSPI library folder):

```cpp
#define ILI9488_DRIVER

#define TFT_MOSI  13
#define TFT_SCLK  14
#define TFT_CS    15
#define TFT_DC     2
#define TFT_RST   12
#define TFT_BL    27

#define TFT_WIDTH  320
#define TFT_HEIGHT 480

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SPI_FREQUENCY  27000000
```

### GT911 Touch

```cpp
#define GT911_SDA  33
#define GT911_SCL  32
#define GT911_INT  21
#define GT911_RST  25
```

---

## 📄 Pages

### `PAGE_START`
- Default page on boot
- Buttons: **Start Game**, **Settings**

### `PAGE_SETTINGS`
- Adjust game configuration
- Settings saved to NVS automatically

### `PAGE_GAME`
- Main game loop and rendering

---

## 🔁 App Flow

```
Boot
 └─ setup()
      ├─ Init Serial
      ├─ Init display (ILI9488 via TFT_eSPI)
      ├─ Init touch (GT911 via I2C)
      ├─ Load settings from NVS
      └─ Draw PAGE_START

loop()
 └─ touchRead(x, y)
      ├─ Debounce check (touchDebounceMs)
      ├─ [DEBUG] Print raw x, y, page to Serial
      └─ processTouch()
           ├─ PAGE_START    → handleTouchStart()
           ├─ PAGE_SETTINGS → handleTouchSettings()
           └─ PAGE_GAME     → handleTouchGame()
```

---

## 🎯 Touch Calibration

With `DEBUG_TOUCH true`, open **Serial Monitor at 115200 baud**. Touch each corner of the screen and note the raw values printed:

```
[TOUCH] raw x=142  y=87   page=0
```

| Corner | Expected |
|---|---|
| Top-left | x≈0, y≈0 |
| Top-right | x≈480, y≈0 |
| Bottom-left | x≈0, y≈320 |
| Bottom-right | x≈480, y≈320 |

### Common Fixes

**Axes swapped** — swap x and y in `hardware.cpp`:
```cpp
uint16_t tx = tp.points[0].y;
uint16_t ty = tp.points[0].x;
```

**X axis mirrored:**
```cpp
tx = SCREEN_W - tx;
```

**Y axis mirrored:**
```cpp
ty = SCREEN_H - ty;
```

---

## 💾 NVS (Settings Storage)

Game settings are persisted using ESP32's built-in Non-Volatile Storage via `nvs_manager.h/.cpp`.

| Function | Description |
|---|---|
| `loadSettings()` | Load saved settings on boot |
| `saveSettings()` | Save settings when changed |

---

## 🚀 Getting Started

1. Clone or download this project
2. Open `main.ino` in **Arduino IDE** or **PlatformIO**
3. Install required libraries:
   - [`TFT_eSPI`](https://github.com/Bodmer/TFT_eSPI) by Bodmer
   - [`GT911`](https://github.com/TAMCTec/gt911-arduino) touch library
   - `Preferences` (built into ESP32 Arduino core)
4. Configure `TFT_eSPI` using the pin table above
5. Select board: **ESP32 Dev Module** or **ESP32-S3**
6. Set **Flash Size: 16MB** and **PSRAM: Enabled** in board settings
7. Upload and open **Serial Monitor at 115200 baud**
8. Touch the screen and calibrate using the debug output
9. Set `DEBUG_TOUCH false` in `config.h` when done ✅

---

## 🔧 Dependencies

| Library | Source | Purpose |
|---|---|---|
| `TFT_eSPI` | Arduino Library Manager | ILI9488 display driver |
| `GT911` | Arduino Library Manager | Capacitive touch controller |
| `Preferences` | Built-in (ESP32 core) | NVS read/write |

---

## 🙌 Contributing

Pull requests welcome! If you add a new page, follow the existing pattern:

1. Create `page_xxx.h` and `page_xxx.cpp`
2. Add `drawPageXxx()` and `handleTouchXxx()`
3. Add `PAGE_XXX` to the `Page` enum in `config.h`
4. Add a case to `processTouch()` in `hardware.cpp`
5. Include the header in `main.ino`

---

## 📝 License

MIT License — free to use, modify, and distribute.
