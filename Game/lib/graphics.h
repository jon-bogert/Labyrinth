#pragma once
#include <stdio.h>
#include "raylib.h"
#include "structs.h"
#include "player.h"

// See .c file (right click -> peek definition) for descriptions

void InitFont(Global global);
void PlaceTexture(Texture2D texture, Vector2 spritePosition, Vector2 position, Global global);
void DrawBoard(Tile board[], Global global);
void DrawPlayer(Player player, Global global);
void DrawEdgeSpaces(EdgeSpace edgeSpace[], Global global);
void DrawPlayerItem(Player player, Global global);