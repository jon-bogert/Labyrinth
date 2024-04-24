#pragma once

#include "structs.h"
#include "hid.h"

// See .c file (right click -> peek definition) for descriptions

void MovePlayer(Player* player, Tile board[], Global global);
bool CheckCollision(int direction, Player player, Tile board[], Global global);
int InteractItem(Player* player, Tile board[], Global global); // return: -1 = no input, 0 = input w/ incorrect/no item, 1 = input w/ correct item, 2 = player wins

Vector2 EdgeSpaceIndexToPos(int index, Global global);
int EdgeSpacePosToIndex(Vector2 position, Global global);

void MoveFloatingPiece(int* spaceWithTile, EdgeSpace edgeSpace[], Global global);
void MoveTiles(int* spaceWithTile, EdgeSpace edgespace[], Tile board[], Player* player, Global* global);