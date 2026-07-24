// nvs_manager.cpp
#include "nvs_manager.h"

static Preferences prefs;

// ─────────────────────────────────────────
void nvs_init() {
  // Just a sanity check open/close on boot
  prefs.begin(NVS_NAMESPACE, false);
  prefs.end();
}

// ─────────────────────────────────────────
void nvs_save(const GameSettings& settings) {
  prefs.begin(NVS_NAMESPACE, false);  // false = read/write
  prefs.putInt( NVS_KEY_MODE,     (int)settings.mode);
  prefs.putInt( NVS_KEY_POINTS,   (int)settings.points);
  prefs.putBool(NVS_KEY_REMEMBER, settings.remember);
  prefs.end();
}

// ─────────────────────────────────────────
GameSettings nvs_load() {
  GameSettings s;

  prefs.begin(NVS_NAMESPACE, true);  // true = read only
  s.mode     = (GameMode)    prefs.getInt( NVS_KEY_MODE,     (int)DEFAULT_MODE);
  s.points   = (PointsTarget)prefs.getInt( NVS_KEY_POINTS,   (int)DEFAULT_POINTS);
  s.remember =               prefs.getBool(NVS_KEY_REMEMBER,      DEFAULT_REMEMBER);
  prefs.end();

  return s;
}

// ─────────────────────────────────────────
void nvs_clear() {
  prefs.begin(NVS_NAMESPACE, false);
  prefs.clear();  // wipes all keys in namespace
  prefs.end();
}