#pragma once
#include "raylib.h"

// common variables passed to functions via single struct
typedef struct Global
{
	char windowName[32];
	int windowWidth;
	int windowHeight;
	int scaleFactor; // (current resolution / refernce resolution)
	int boardDimensions; // board lenth and height in tiles
	int numEdgeSpaceSide; // edge spaces per side
	int numEdgeSpaceTotal; // edge spaces total

	Vector2 boardOffset; // position of first board tile (not edge space)
	int textureResOrig; // original sprite resolution (based on resolution height of "tile.png" spritesheet
	int textureResScaled; // new sprite resolution after scaling to window resolution

	Rectangle textureFrame; // used for selecting spite from sprite sheet

	int cornerPieceMoveable; // number of corner shaped pieces that are moveable
	int cornerPiece; // number of corner shaped pieces including perminant ones
	int straightPiece; // number of straight shaped pieces (there are no perminant straight tiles)
	int tPieceMovable; // number of 't' shaped pieces that are moveable
	int tPiece; // number of 't' shaped pieces including perminant ones
	int randomItemAmount; // number of movable tiles

	int numOfPlayers;
	int itemsMax; // number of items each player has to collect per game (can be adjusted in settings window
	int turnState; // 0 - tile movement phase of turn, 1 = player movement phase
	int lastMoveIndex; // index of last new floating tile after the move is over, used prevent player from undoing last move

	//Sprite sheets
	Texture2D tileSprite;
	Texture2D itemSprite;
	Texture2D playerSprite;
	Texture2D arrowSprite;
	Texture2D selectSprite;

} Global;

typedef struct Player
{
	int number;
	Vector2 startingPoint; // starting position (tile coord)
	Vector2 position; // current position (tile coord)
	Vector2 sprite; // sprite placement for texture drawing of player piece
	Vector2 startingPointSprite; // sprite placement for texture drawing of player start area
	int itemsCollected; // current number of items player has collected so far
	int itemList[25]; // 24 possible items + '-1' finish indicator
} Player;

// board tile
typedef struct Tile
{
	Vector2 position; // position in pixel coordinates (board coordinates can be obtained from array index)
	int type; // 0 - straight || 2 - angle || 6 - t-shaped
	int rotation; // 0 = horizontal(straight), bottom left (angle), left (t-shaped) || rotate clockwise
	int id; // .type + .rotation
	bool hasItem; // indicates wherther player can interact/Draw should look for an item to draw
	int itemID; //ID number of item - only read if hasItem == true;

} Tile;

// spaces outside of board for floating tile
typedef struct EdgeSpace
{
	Vector2 position; // tile coords position for space - NOTE: X or Y must be '-1' or '7', with other coord being '1', '3', '5'
	bool hasTile; // indicates whether this position has the floating tile
	int id; // indicates direction the arrow points (for sprite sheet) 0 = down(top of board), 1 = Left, 2 - up, 3 = right
	Tile tile; // floating tile, only read if hasTile == true
} EdgeSpace;

// Used for initializing board - keeps track of the types of tiles spawned in
typedef struct TileCounter
{
	int straight;
	int corner;
	int tShaped;
} TileCounter;

// button for menus
typedef struct Button
{
	bool selected; // whether this button is currently selected (Color/functionality)
	int gameState; // what menu this button leads to
	char text[16]; // displayed text
	Color color; // current color (based on button.selected value)
} Button;