// page_settings.h
#pragma once
#include "hardware.h"
#include "config.h"
#include "nvs_manager.h"
#include "page_game.h"
#include "page_start.h"

void drawSettingsBackBtn();
void drawPageSettings();
void handleTouchSettings(uint16_t tx, uint16_t ty);