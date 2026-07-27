// page_game.h
#pragma once
#include "hardware.h"
#include "config.h"
#include "nvs_manager.h"
#include "page_settings.h"
#include "page_start.h"

void drawPageGame();
void drawGameBackBtn();
void handleTouchGame(uint16_t tx, uint16_t ty);