// page_start.h
#pragma once
#include "hardware.h"
#include "config.h"
#include "nvs_manager.h"
#include "page_settings.h"
#include "page_game.h"

void drawPageStart();
void drawStartBtn();
void drawSettingsBtn();
void drawStartPanel();
void handleTouchStart(uint16_t tx, uint16_t ty);
