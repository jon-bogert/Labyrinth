#pragma once
#include "raylib.h"
#include "structs.h"
#include "hid.h"

// See .c file (right click -> peek definition) for descriptions

void MainMenu(int* gameState, Global global, bool* newGame);
void InitButtons();
void DrawButtons(Global global);
void InteractButtons(int* gameState, int* selectedButton);

void PauseMenu(int* gameState, Global global);
void InitPauseButtons();
void InteractPauseButtons(int* gameState, int* selectedButton);
void DrawPauseButtons(Global global);

void Instructions(int* gameState, bool inGame, Global global);
void Settings(Global* global, int* gameState);