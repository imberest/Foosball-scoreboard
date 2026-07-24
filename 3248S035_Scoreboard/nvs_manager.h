// nvs_manager.h

#pragma once

#include <Preferences.h>
#include "config.h"



// ─────────────────────────────────────────
// Public API
// ─────────────────────────────────────────
void          nvs_init();
void          nvs_save(const GameSettings& settings);
GameSettings  nvs_load();
void          nvs_clear();
