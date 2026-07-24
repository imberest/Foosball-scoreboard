# 🎮 ESP32 Touch Game Console

A touch-screen game console built on the ESP32 platform, featuring a GT911 capacitive touch controller and TFT display. Navigate between pages using touch input, configure game settings, and play — all from a clean, page-based UI.

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

## ⚙️ Configuration (`config.h`)

| Flag / Constant | Default | Description |
|---|---|---|
| `DEBUG_TOUCH` | `true` | Print raw touch coordinates to Serial |
| `SCREEN_W` | `480` | Display width in pixels |
| `SCREEN_H` | `320` | Display height in pixels |
| `touchDebounceMs` | `200` | Minimum ms between touch events |

> 💡 Set `DEBUG_TOUCH false` once your touch is calibrated.

---

## 🖥️ Hardware

| Component | Details |
|---|---|
| Microcontroller | ESP32 |
| Display | TFT (480×320) |
| Touch Controller | GT911 Capacitive |
| Storage | NVS (Non-Volatile Storage) |

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
      ├─ Init display
      ├─ Init touch (GT911)
      ├─ Load settings from NVS
      └─ Draw PAGE_START

loop()
 └─ touchRead(x, y)
      ├─ Debounce check
      ├─ [DEBUG] Print x, y, page to Serial
      └─ processTouch()
           ├─ PAGE_START    → handleTouchStart()
           ├─ PAGE_SETTINGS → handleTouchSettings()
           └─ PAGE_GAME     → handleTouchGame()
```

---

## 🎯 Touch Calibration

With `DEBUG_TOUCH true`, open the **Serial Monitor** at your configured baud rate. Touch each corner of the screen and note the raw values:

| Corner | Expected |
|---|---|
| Top-left | x≈0, y≈0 |
| Top-right | x≈max, y≈0 |
| Bottom-left | x≈0, y≈max |
| Bottom-right | x≈max, y≈max |

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
2. Open `main.ino` in Arduino IDE or PlatformIO
3. Install required libraries:
   - `TFT_eSPI` or equivalent display library
   - `GT911` touch library
4. Set your board to **ESP32**
5. Upload and open **Serial Monitor**
6. Touch the screen — calibrate using the debug output
7. Set `DEBUG_TOUCH false` in `config.h` when done

---

## 🔧 Dependencies

| Library | Purpose |
|---|---|
| `TFT_eSPI` | TFT display driver |
| `GT911` | Capacitive touch controller |
| `Preferences` | NVS read/write on ESP32 |

---

## 📝 License

MIT License — free to use, modify, and distribute.

---

## 🙌 Contributing

Pull requests welcome! If you add a new page, follow the existing pattern:

1. Create `page_xxx.h` and `page_xxx.cpp`
2. Add `drawPageXxx()` and `handleTouchXxx()`
3. Add `PAGE_XXX` to the `Page` enum in `config.h`
4. Add a case to `processTouch()` in `hardware.cpp`
5. Include the header in `main.ino`
