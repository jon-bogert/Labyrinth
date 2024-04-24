#pragma once
#include <stdio.h>
#include "raylib.h"
#include "structs.h"
#include "player.h"

// See .c file (right click -> peek definition) for descriptions

void InitWindowProperties(Global* global);
void InitGlobal(Global* global);
Texture2D InitSpriteSheet(char path[], Global global);

// Board
int GetUnusedTile(Tile board[]);
void InitEdgeSpaces(EdgeSpace edgeSpace[], Global global);
void InitItems(Tile* board, Global global);
TileCounter InitTileCounter(Tile board[]);
void InitBoard(Tile* board, EdgeSpace* floatingTile, Global global);
void SetPermanentTiles(Tile* tile, int x, int y);
void SetRandomTiles(Tile* tile, int x, int y, TileCounter* counter);

// Player
void InitPlayer(Player* player, int playerIndex, bool usedItems[], Global global);
int GetUnusedItem(bool usedItems[], Global global);