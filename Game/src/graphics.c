#include "graphics.h"

Font font; // Font used for HUD
float fontSize = 28.f; // Fint size relative to 1080p

// init font for in-game HUD
void InitFont(Global global)
{
	fontSize = fontSize * (global.windowHeight / 1080.f); // scales font size to resolution
	font = LoadFontEx("assets/Montserrat-Light.ttf", fontSize, 0, 250); // load font at scaled size
}

// Function that draws specified texture based input provided (note: spritePosition in tile coords, position in window pixel position)
void PlaceTexture(Texture2D texture, Vector2 spritePosition, Vector2 position, Global global)
{
	//sets .x and .y for position on sprite sheet
	global.textureFrame.x = spritePosition.x * global.textureResScaled;
	global.textureFrame.y = spritePosition.y * global.textureResScaled;

	DrawTextureRec(texture, global.textureFrame, position, WHITE);// draws texture
}

// Draws all tiles and Items on board
void DrawBoard(Tile board[], Global global)
{
	Vector2 position; // pixel coords for tile/item
	for (int i = 0; i < 49; i++)
	{
		// pixel coords calculated from index
		position.x = (i % global.boardDimensions) * global.textureFrame.width + global.boardOffset.x;
		position.y = (i / global.boardDimensions) * global.textureFrame.height + global.boardOffset.y;

		//sets .x and .y for position on sprite sheet
		global.textureFrame.x = board[i].id * global.textureFrame.width;
		global.textureFrame.y = 0.0f;

		DrawTextureRec(global.tileSprite, global.textureFrame, position, WHITE); // Draw Tile
		if (board[i].hasItem)// if tile has item
		{
			//sets .x and .y for position on sprite sheet
			Vector2 spritePosition =
			{
				(board[i].itemID % (global.itemSprite.width / global.textureResScaled)),
				(board[i].itemID / (global.itemSprite.width / global.textureResScaled))
			};

			//Draw Item
			PlaceTexture(global.itemSprite, spritePosition, position, global);
		}
	}
}

//Draws player and player's starting position
void DrawPlayer(Player player, Global global)
{
	Vector2 spritePosition, playerPosition, startingPosition; // spritePosition = coords for sprite on sprite sheet, playerPosition = pixel coords, startingPosition = pixel coords

	//gets pixel coords of player from tile coords
	playerPosition.x = player.position.x * global.textureFrame.width + global.boardOffset.x;
	playerPosition.y = player.position.y * global.textureFrame.height + global.boardOffset.y;
	//gets pixel coords of player start point from tile coords
	startingPosition.x = player.startingPoint.x * global.textureFrame.width + global.boardOffset.x;
	startingPosition.y = player.startingPoint.y * global.textureFrame.height + global.boardOffset.y;

	//gets sprite position on sprite sheet based on start point based on player number
	spritePosition.x = player.number - 1;
	spritePosition.y = 1; // will alwys be 2nd row

	//Draws start point texture
	PlaceTexture(global.playerSprite, spritePosition, startingPosition, global);

	spritePosition.y = 0;// uses .x from start point, player sprite will always be 1st row
	PlaceTexture(global.playerSprite, spritePosition, playerPosition, global); //Draws player texture
	if (global.turnState == 1) PlaceTexture(global.selectSprite, (Vector2) { 1, 0 }, playerPosition, global); // draws "player selected" sprite if in player movement phase
}

//Draws Edge spaces and floating tile
void DrawEdgeSpaces(EdgeSpace edgeSpace[], Global global)
{
	Vector2 position, spritePosition;// pixel coords
	for (int i = 0; i < global.numEdgeSpaceTotal; i++) // Arrow Textures
	{
		//Get X/Y coords from array index
		position.x = (EdgeSpaceIndexToPos(i, global).x * global.textureFrame.width + global.boardOffset.x);
		position.y = (EdgeSpaceIndexToPos(i, global).y * global.textureFrame.width + global.boardOffset.y);
		//Get sprite position on sprite sheet
		spritePosition.x = edgeSpace[i].id;
		spritePosition.y = 0;// only one row
		PlaceTexture(global.arrowSprite, spritePosition, position, global); // draws arrows
		if (edgeSpace[i].hasTile) // check if space has floating tile
		{
			//get position of tile on it's spritesheet (only one row, .y same as above)
			spritePosition.x = edgeSpace[i].tile.id;
			PlaceTexture(global.tileSprite, spritePosition, position, global);// draw tile

			if (edgeSpace[i].tile.hasItem) // check if tile has item
			{
				//get position of item on it's spritesheet
				spritePosition.x = (edgeSpace[i].tile.itemID % (global.itemSprite.width / global.textureResScaled));
				spritePosition.y = (edgeSpace[i].tile.itemID / (global.itemSprite.width / global.textureResScaled));
				PlaceTexture(global.itemSprite, spritePosition, position, global); // draw item
			}
			if (global.turnState == 0) PlaceTexture(global.selectSprite, (Vector2) { 0, 0 }, position, global); // draw "tile selected" texture if on tile movement phase
		}
	}
}

// Draw HUD and Item related to the item the player is currently looking for
void DrawPlayerItem(Player player, Global global)
{
	float locationStart = 0.01f * global.windowHeight; // starting x&y value for HUD
	char itemLabel[] = "Current Item:"; // Label
	char itemCounter[32]; //init text for item counter
	strcpy(itemCounter, TextFormat("Items Collected %i/%i", player.itemsCollected, global.itemsMax)); // assign counter text of with number of items collected out of items the player needs to collect

	Vector2 position = { locationStart, locationStart };//sets start position for HUD
	DrawTextPro(font, itemLabel, position, (Vector2) { 0, 0 }, 0.f, fontSize, 1.f, WHITE);// draws HUD label

	//Get position for item display
	position.y = position.y + MeasureTextEx(font, itemLabel, fontSize, 1.f).y;
	position.x = (position.x + MeasureTextEx(font, itemLabel, fontSize, 1.f).x - global.textureResScaled) / 2; // Sprite centered under label text
	
	if (player.itemList[player.itemsCollected] == -1) // display starting position if player has collected all assigned items
	{
		//find and draw player start position texture
		Vector2 spritePosition, startingPosition;
		spritePosition.x = player.number - 1;
		spritePosition.y = 1;
		PlaceTexture(global.playerSprite, spritePosition, position, global);
	}
	else // player is currently looking for an item
	{
		//find and draw item texture
		Vector2 spritePosition;
		spritePosition.x = (player.itemList[player.itemsCollected] % (global.itemSprite.width / global.textureResScaled));
		spritePosition.y = (player.itemList[player.itemsCollected] / (global.itemSprite.width / global.textureResScaled));
		PlaceTexture(global.itemSprite, spritePosition, position, global);
	}

	position.x = locationStart; // text to line up with HUD title
	position.y = position.y + global.textureResScaled + locationStart; /// below item sprite
	DrawTextPro(font, itemCounter, position, (Vector2) { 0, 0 }, 0.f, fontSize, 1.f, WHITE);// draw number of items collected out of total assigned
}