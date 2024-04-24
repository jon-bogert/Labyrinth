#include "init.h"

//Playable Window
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const char WINDOW_NAME[32] = "Labyrinth";

// Reference Dimensions - DO NOT CHANGE!
const int REF_WIDTH = 1920;
const int REF_HEIGHT = 1080;

const int BOARD_DIMENSIONS = 7;

const int CORNER_PIECE_AMT_MOVEABLE = 16;
const int CORNER_PIECE_AMT = 20;
const int STRAIGHT_PIECE_AMT = 12;
const int T_PIECE_AMT_MOVEABLE = 6;
const int T_PIECE_AMT = 14;
const int RANDOM_ITEM_AMT = 6;

const int START_NUM_OF_PLAYER = 1;
const int START_NUM_OF_ITEMS = 5;

const char TILEMAP_PATH[] = "assets/tile.png";
const char PLAYERSPRITE_PATH[] = "assets/player.png";
const char ITEMSPRITE_PATH[] = "assets/item.png";
const char ARROWSPRITE_PATH[] = "assets/arrow.png";
const char SELECTSPRITE_PATH[] = "assets/select.png";

//properties that area required for window startup & settings page
void InitWindowProperties(Global* global)
{
	global->windowWidth = WINDOW_WIDTH;
	global->windowHeight = WINDOW_HEIGHT;
	strcpy(global->windowName, WINDOW_NAME);
	global->itemsMax = START_NUM_OF_ITEMS; // defualt 5, can be changes in settings
}

void InitGlobal(Global* global)
{
	//multiplication factor for reolution scaling
	global->scaleFactor =
		(global->windowWidth >= global->windowHeight) ?
		(global->windowHeight / REF_HEIGHT) :
		(global->windowWidth / REF_WIDTH);

	global->boardDimensions = BOARD_DIMENSIONS;
	global->numEdgeSpaceSide = BOARD_DIMENSIONS / 2;
	global->numEdgeSpaceTotal = global->numEdgeSpaceSide * 4;

	//texture loaded to determine resolution of textures - can use textures of any resolution as long as they all have the same tile resolution and form to original sprite layout
	Image tempImg = LoadImage(TILEMAP_PATH);
	global->textureResOrig = tempImg.height;
	UnloadImage(tempImg);
	global->textureResScaled =
		(global->windowWidth >= global->windowHeight) ?
		((global->windowHeight / (global->boardDimensions + 2))) :
		((global->windowWidth / (global->boardDimensions + 2)));

	global->boardOffset.x =
		(global->windowWidth / 2) - ((global->textureResScaled * BOARD_DIMENSIONS) / 2);
	global->boardOffset.y =
		(global->windowHeight / 2) - ((global->textureResScaled * BOARD_DIMENSIONS) / 2);

	//Init rest of textures
	global->tileSprite = InitSpriteSheet(TILEMAP_PATH, *global);
	global->itemSprite = InitSpriteSheet(ITEMSPRITE_PATH, *global);
	global->playerSprite = InitSpriteSheet(PLAYERSPRITE_PATH, *global);
	global->arrowSprite = InitSpriteSheet(ARROWSPRITE_PATH, *global);
	global->selectSprite = InitSpriteSheet(SELECTSPRITE_PATH, *global);

	global->textureFrame.width = global->textureResScaled;
	global->textureFrame.height = global->textureResScaled;
	// .x and .y assigned before texture is drawn

	global->cornerPieceMoveable = CORNER_PIECE_AMT_MOVEABLE;
	global->cornerPiece = CORNER_PIECE_AMT;
	global->straightPiece = STRAIGHT_PIECE_AMT;
	global->tPieceMovable = T_PIECE_AMT_MOVEABLE;
	global->tPiece = T_PIECE_AMT;
	global->randomItemAmount = RANDOM_ITEM_AMT;

	global->numOfPlayers = START_NUM_OF_PLAYER;

	global->turnState = 0; // start with tile movement phase
	global->lastMoveIndex = -1; // -1 = unassigned -> so there is no limited moves at the start (will be >= 0 after first turn 
}

//Initialized Texture - return texture scaled to window size
Texture2D InitSpriteSheet(char path[], Global global) // path of texture, global struct
{
	// loaded as image in order to scale before assigning to type Texture2D
	Image ImageLoadTmp = LoadImage(path);
	Texture2D texture;

	//resize without filter to retain pixelated feel
	ImageResizeNN(&ImageLoadTmp, global.textureResScaled * (ImageLoadTmp.width / global.textureResOrig), global.textureResScaled * (ImageLoadTmp.height / global.textureResOrig));
	texture = LoadTextureFromImage(ImageLoadTmp); // scaled image loaded to texture
	
	return texture;
}

// returns ID of tile that has not been assigned yet, for item placement initialization
int GetUnusedTile(Tile board[])
{
	bool isUsed = true; // to check if tile is already used (true to enter loop)
	int index; // ID of tile being checked
	while (isUsed)
	{
		index = GetRandomValue(0, 49); // get random tile ID
		isUsed = (board[index].type == -1) ? false : true; // checks if tile is used
		if (index == 0 || index == 6 || index == 42 || index == 48) isUsed = true; // Corner tiles may not be used
	} // exit loop if tile is unused

	return index;
}

// ================ BOARD ================

// Initialize edge spaces (floating tile will be assigned later in InitBoard();)
void InitEdgeSpaces(EdgeSpace edgeSpace[], Global global)
{
	int direction; // pointing - 0 = Down, 1 = Left, 2 = Up, 3 = Right 
	for (int i = 0; i < global.numEdgeSpaceTotal; i++) // work through all spaces (topside-left going clockwise)
	{
		if (i < (global.numEdgeSpaceSide)) // Top side
			direction = 0; //pointing down
		else if (i < (global.numEdgeSpaceSide * 2)) // Right Side
			direction = 1;// pointing left
		else if (i < (global.numEdgeSpaceSide * 3)) // Bottom side
			direction = 2;// pointing up
		else // Left Side
			direction = 3;// pointing right

		edgeSpace[i].id = direction; // assignt to edgeSpace.id struct property
		edgeSpace[i].position = EdgeSpaceIndexToPos(i, global); // convert index to X/Y position (tile coords)
		edgeSpace->hasTile = false; // indicates if space has the floating tile (will be set 'true' later in InitBoard();)
	}
}

// assigns all items to appropriate perminant or movable pieces
void InitItems(Tile* board, Global global)
{
	// set all .hasItem to "false"
	for (int i = 0; i < 50; i++)
	{
		board[i].hasItem = false;
		board[i].type = -1; // unassigned itemID
	}

	// assigned tile coords for permanent Items on permanent tiles
	for (int id = 0; id <= 11; id++)
	{
		int x, y;
		switch (id)
		{
		case 0: x = 2; y = 0; break;
		case 1: x = 4; y = 0; break;
		case 2: x = 0; y = 2; break;
		case 3: x = 2; y = 2; break;
		case 4: x = 4; y = 2; break;
		case 5: x = 6; y = 2; break;
		case 6: x = 0; y = 4; break;
		case 7: x = 2; y = 4; break;
		case 8: x = 4; y = 4; break;
		case 9: x = 6; y = 4; break;
		case 10: x = 2; y = 6; break;
		case 11: x = 4; y = 6; break;
		}

		int index = (y * global.boardDimensions) + x; //converts coord position to index (for tile array)
		board[index].hasItem = true; // space contains item
		board[index].itemID = id; // sets item id in struct
		board[index].type = -2; // temp value to show this is unassigned - will be properly assigned later ('-1' reserved for unassigned movable tiles)
	}

	// Corner Items
	for (int id = 12; id <= 17; id++)
	{
		int index = GetUnusedTile(board); // gets unused board space for new tile
		board[index].hasItem = true;
		board[index].itemID = id;
		board[index].type = 2; // rotation/id determined later
	}

	// T-Shaped Items
	for (int id = 18; id <= 23; id++)
	{
		int index = GetUnusedTile(board); // gets unused board space for new tile
		board[index].hasItem = true;
		board[index].itemID = id;
		board[index].type = 6; // rotation/id determined later
	}

	//NOTE: there are no straight tiles with items
}

// Initializes tile counter used to limit the number of each tile ( there will be the same number of each tile every game, just in different positions & rotations)
TileCounter InitTileCounter(Tile board[])
{
	TileCounter counter = { 0, 0, 0 };
	for (int i = 0; i < 50; i++)
	{
		//counting number of pieces with that have aleady been assigned by InitItems();
		//straight shouldn't exist yet
		if (board[i].type == 2) // corner pieces
			counter.corner++;
		else if (board[i].type == 6)
			counter.tShaped++;
	}
	return counter;
}

// Initializes Tiles including floating tile
void InitBoard(Tile* board, EdgeSpace* floatingTile, Global global)
{
	TileCounter counter = InitTileCounter(board); // get number of tiles already assigned by InitItems();
	
	for (int i = 0; i < 50; i++) // iterate through all board spaces (including floating tile)
	{
		int x = i % global.boardDimensions; // find x coord ot tile from index
		int y = i / global.boardDimensions; // find y coord of tile from index
		if (x % 2 == 0 && y % 2 == 0) // finds indexes that should be permanent tiles
		{
			SetPermanentTiles(&board[i], x, y); // assigns values for permanent tiles
			if (board[i].id <= 5) // assignes .type and .rotation for corner pieces (note: no straight pieces should exist)
			{
				board[i].type = 2;
				board[i].rotation = board[i].id - board[i].type;
			}	
			else // assignes .type and .rotation for t-shaped pieces
			{
				board[i].type = 6;
				board[i].rotation = board[i].id - board[i].type;
			}
		}
		else
		{
			SetRandomTiles(&board[i], x, y, &counter); // sets values for random tiles
		}
		//assignes pixel position coordinates
		board[i].position.x = (x * global.textureResScaled) + global.boardOffset.x;
		board[i].position.y = (y * global.textureResScaled) + global.boardOffset.y;
	}
	//assignes floating tile info
	floatingTile->hasTile = true;
	floatingTile->tile = board[49];
}

//sets tile id for Permanent tiles
void SetPermanentTiles(Tile* tile, int x, int y)
{
	if (x == 0 && y == 0)
	{
		tile->id = 5;
	}
	else if (x == 2 && y == 0)
	{
		tile->id = 8;
	}
	else if (x == 4 && y == 0)
	{
		tile->id = 8;
	}
	else if (x == 6 && y == 0)
	{
		tile->id = 2;
	}
	else if (x == 0 && y == 2)
	{
		tile->id = 7;
	}
	else if (x == 2 && y == 2)
	{
		tile->id = 7;
	}
	else if (x == 4 && y == 2)
	{
		tile->id = 8;
	}
	else if (x == 6 && y == 2)
	{
		tile->id = 9;
	}
	else if (x == 0 && y == 4)
	{
		tile->id = 7;
	}
	else if (x == 2 && y == 4)
	{
		tile->id = 6;
	}
	else if (x == 4 && y == 4)
	{
		tile->id = 9;
	}
	else if (x == 6 && y == 4)
	{
		tile->id = 9;
	}
	else if (x == 0 && y == 6)
	{
		tile->id = 4;
	}
	else if (x == 2 && y == 6)
	{
		tile->id = 6;
	}
	else if (x == 4 && y == 6)
	{
		tile->id = 6;
	}
	else if (x == 6 && y == 6)
	{
		tile->id = 3;
	}
}

// sets values of tiles not assigned in InitItems(); - sets .type and .rotation based on .id for tiles set in InitItems();
void SetRandomTiles(Tile* tile, int x, int y, TileCounter* counter)
{
	// check if unassigned ('-1' for movable tiles. '-2' was used for permanent tiles)
	if (tile->type == -1)
	{
		bool again = true; // true if  maximum number of that type of tiles has already been reached
		while (again)
		{
			int temp = GetRandomValue(0, 2); // gets random tile of tile to check
			if (temp == 0 && counter->straight < STRAIGHT_PIECE_AMT)//straight
			{
				again = false;
				tile->type = 0;
				counter->straight++;
			}
			else if (temp == 1 && counter->corner < CORNER_PIECE_AMT_MOVEABLE)//corner
			{
				again = false;
				tile->type = 2;
				counter->corner++;
			}
			if (temp == 2 && counter->tShaped < T_PIECE_AMT_MOVEABLE)//t-shaped
			{
				again = false;
				tile->type = 6;
				counter->tShaped++;
			}
		}
	}
	
	//set rotation (no limit)
	if (tile->type == 0) // straight (only 2 rotations)
	{
		tile->rotation = GetRandomValue(0, 1);
	}
	else // corner and t-shaped (4 rotations)
	{
		tile->rotation = GetRandomValue(0, 3);
	}
	// if derived from .type + .rotation
	tile->id = tile->type + tile->rotation;
}

// ================ PLAYER ================

//Initializes player
void InitPlayer(Player* player, int playerIndex, bool usedItems[], Global global)
{
	player->number = playerIndex + 1; // sets stored player number based on array index
	switch (playerIndex) // assigns starting point for participating players
	{
	case 0: player->startingPoint.x = 0; player->startingPoint.y = 0; break;
	case 1: player->startingPoint.x = 6; player->startingPoint.y = 0; break;
	case 2: player->startingPoint.x = 6; player->startingPoint.y = 6; break;
	case 3: player->startingPoint.x = 0; player->startingPoint.y = 6; break;
	}
	
	player[playerIndex].position = player[playerIndex].startingPoint; //sets starting position to starting point for beginning of game

	switch (playerIndex) // assigns player sprite position on sprite sheet
	{
	case 0: player->sprite.x = 0; player->sprite.y = 0; break;
	case 1: player->sprite.x = 1; player->sprite.y = 0; break;
	case 2: player->sprite.x = 2; player->sprite.y = 0; break;
	case 3: player->sprite.x = 3; player->sprite.y = 0; break;
	}

	switch (playerIndex) // assigns player starting point sprite position on sprite sheet
	{
	case 0: player->startingPointSprite.x = 0; player->startingPointSprite.y = 1; break;
	case 1: player->startingPointSprite.x = 1; player->startingPointSprite.y = 1; break;
	case 2: player->startingPointSprite.x = 2; player->startingPointSprite.y = 1; break;
	case 3: player->startingPointSprite.x = 3; player->startingPointSprite.y = 1; break;
	}

	player->itemsCollected = 0;// initializes number of items collected to 0 for start of game
	int i = 0;// ised in loop
	for (int i = 0; i < global.itemsMax; i++)// iterates through the list of items to collect, assigning them until we reach specified amount (from settings [default = 5])
	{
		player->itemList[i] = GetUnusedItem(usedItems, global);// gets unused item
	}
	player->itemList[global.itemsMax] = -1; //End value -1 to indicate player has collected all items assigned
}

//returns id of an item that is not yet assigned to a player to collect
int GetUnusedItem(bool usedItems[], Global global)
{
	bool isUsed = true; // true if item has been already assigned (init 'true' to enter loop
	int index; // item id (index of usedItems Bool array)
	while (isUsed)
	{
		index = GetRandomValue(0, 23); // gets random itemID
		isUsed = (usedItems[index]);// chacks if item has already assigned
	}//exit if item is unassigned to other player
	usedItems[index] = true;// sets as assigned for later iterations
	return index; // returns itemID nimber
}