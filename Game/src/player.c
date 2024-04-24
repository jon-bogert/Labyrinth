#include "player.h"

//Moves player around the board - first: checks if tile can be moved to before moving
void MovePlayer(Player* player, Tile board[], Global global)
{
	if (InputLeft()) // Move Left
	{
		if (CheckCollision(0, *player, board, global))
			player->position.x--;
	}
	else if (InputUp()) // Move Up
	{
		if (CheckCollision(1, *player, board, global))
			player->position.y--;
	}
	else if (InputRight()) // Move Right
	{
		if (CheckCollision(2, *player, board, global))
			player->position.x++;
	}
	else if (InputDown()) // Move Down
	{
		if (CheckCollision(3, *player, board, global))
			player->position.y++;
	}
}

//Checks if player can move to piece based on direction; checks if player is at edge of board; return true if player can move
bool CheckCollision(int direction, Player player, Tile board[], Global global) // direction:  0 = Left, 1 = Up, 2 = Right, 3 = Down
{
	//path directions based on tile id
	bool path[10][4] =
	{
		{true, false, true, false},//stright
		{false, true, false, true},
		{true, false, false, true},//corner
		{true, true, false, false},
		{false, true, true, false},
		{false, false, true, true},
		{true, true, true, false},//t-shaped
		{false, true, true, true},
		{true, false, true, true},
		{true, true, false, true}
	};
	int playerPosX = player.position.x, playerPosY = player.position.y; // player position seperated and in int format
	int tileIndex = playerPosY * global.boardDimensions + playerPosX; // get index from position
	int playerTileID = board[tileIndex].id; // gets id of tile the player is currently on
	Tile destTile; // tile information of the tile the player wants to go to
	if (direction == 0) // Check Moving Left
	{
		if (playerPosX == 0) { return false; } // edge case Left
		destTile = board[tileIndex - 1];
	}
	else if (direction == 1) // Check Moving Up
	{
		if (playerPosY == 0) { return false; } // edge case Up
		destTile = board[tileIndex - global.boardDimensions];
	}
	else if (direction == 2) // Check Moving Right
	{
		if (playerPosX == 6) { return false; } // edge case Right
		destTile = board[tileIndex + 1];
	}
	else if (direction == 3) // Check Moving Down
	{
		if (playerPosY == 6) { return false; } // edge case Down
		destTile = board[tileIndex + global.boardDimensions];
	}
	else { return false; }

	if (path[playerTileID][direction] && path[destTile.id][(direction + 2) % 4]) // Check if paths connect
		return true;
	else //paths don't connect
		return false;
}

//check if the player is on the item they are currently assigned; checks starting position if all items collected
int InteractItem(Player* player, Tile board[], Global global) // return: -1 = no input, 0 = input w/ incorrect/no item, 1 = input w/ correct item, 2 = player wins
{
	//Gets tile array index of tile player is on
	int tileIndex = player->position.y * global.boardDimensions + player->position.x;

	//checks when ineteract button is pressed whean all items are collected
	if (InputInteract() && player->itemList[player->itemsCollected] == -1)
	{
		//find player start tile index
		int playerStartIndex = player->startingPoint.y * global.boardDimensions + player->startingPoint.x;
		if (tileIndex == playerStartIndex) return 2; // return win condition if player is at starting point
	}

	// check item if player is on a tile with item
	if (InputInteract() && board[tileIndex].hasItem)
	{
		//check if item is currently assigned
		if (board[tileIndex].itemID == player->itemList[player->itemsCollected])
		{
			player->itemsCollected++; // increase counter to assign next item
			return 1; // return interation to switch to tile move phase
		}
		else return 0; // item present, but item is not correct, move to tile move phase
	}
	else if (InputInteract()) return 0; // if player presses interact and there is no item on current tile, move to tile move phase
	else return -1; // no input from player, stay in player move phase
}


//Converts Edge space array index to tile coords
Vector2 EdgeSpaceIndexToPos(int index, Global global)
{
	Vector2 position;
	int side[4] = { global.numEdgeSpaceSide, (global.numEdgeSpaceSide * 2), (global.numEdgeSpaceSide * 3), global.numEdgeSpaceTotal };
	if (index < (side[0])) // Top
	{
		position.x = index * 2 + 1;
		position.y = -1;
	}
	else if (index < side[1]) // Right Side
	{
		position.x = global.boardDimensions;
		position.y = (index - side[0]) * 2 + 1;
	}
	else if (index < side[2]) // Bottom
	{
		position.x = (index - side[1]) * -2 + (side[1] - 1);
		position.y = global.boardDimensions;
	}
	else // Left Side
	{
		position.x = -1;
		position.y = (index - side[2]) * -2 + (side[1] - 1);
	}

	return position;
}

//Converts Tile coords to edge space array index
int EdgeSpacePosToIndex(Vector2 position, Global global)
{
	int index;
	int side[4] = { global.numEdgeSpaceSide, (global.numEdgeSpaceSide * 2), (global.numEdgeSpaceSide * 3), global.numEdgeSpaceTotal };
	if ((int)position.y == -1) // Top
	{
		index = (position.x - 1) / 2;
	}
	else if ((int)position.x == global.boardDimensions) // Right Side
	{
		index = (int)((position.y * 0.5f) + 2.5f);
	}
	else if ((int)position.y == global.boardDimensions) // Bottom
	{
		index = ((position.x - (side[1] - 1)) / -2 + side[1]);
	}
	else // Left Side
	{
		index = (int)((position.y * -0.5f) + 11.5f);
	}

	return index;
}

// Moves floating piece around the edge of the board
void MoveFloatingPiece(int* spaceWithTile, EdgeSpace edgeSpace[], Global global)
{
	if (edgeSpace[*spaceWithTile].position.y == -1) // Top
	{
		if ((InputDown() || InputLeft()) && edgeSpace[*spaceWithTile].position.x == 1) // Edge Case Left
		{
			edgeSpace[global.numEdgeSpaceTotal - 1].hasTile = true; // new edge space has tile
			edgeSpace[global.numEdgeSpaceTotal - 1].tile = edgeSpace[*spaceWithTile].tile; // assign tile values to new edge space
			edgeSpace[*spaceWithTile].hasTile = false; // old space no longer has tile
			*spaceWithTile = global.numEdgeSpaceTotal - 1; // new array index position of tile
		}
		else if (InputLeft())
		{
			edgeSpace[*spaceWithTile - 1].hasTile = true;
			edgeSpace[*spaceWithTile - 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile -= 1;
		}
		
		if ((InputDown() || InputRight()) && edgeSpace[*spaceWithTile].position.x == global.boardDimensions - 2) // Edge Case Right
		{
			edgeSpace[*spaceWithTile + 1].hasTile = true;
			edgeSpace[*spaceWithTile + 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile += 1;
		}
		else if (InputRight())
		{
			edgeSpace[*spaceWithTile + 1].hasTile = true;
			edgeSpace[*spaceWithTile + 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile += 1;
		}
	}



	else if (edgeSpace[*spaceWithTile].position.x == global.boardDimensions) // Right
	{
		if ((InputLeft() || InputUp()) && edgeSpace[*spaceWithTile].position.y == 1) // Edge Case Top
		{
			edgeSpace[*spaceWithTile - 1].hasTile = true;
			edgeSpace[*spaceWithTile - 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile -= 1;
		}
		else if (InputUp())
		{
			edgeSpace[*spaceWithTile - 1].hasTile = true;
			edgeSpace[*spaceWithTile - 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile -= 1;
		}

		if ((InputLeft() || InputDown()) && edgeSpace[*spaceWithTile].position.y == global.boardDimensions - 2) // Edge Case Bottom
		{
			edgeSpace[*spaceWithTile + 1].hasTile = true;
			edgeSpace[*spaceWithTile + 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile += 1;
		}
		else if (InputDown())
		{
			edgeSpace[*spaceWithTile + 1].hasTile = true;
			edgeSpace[*spaceWithTile + 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile += 1;
		}
	}

	else if (edgeSpace[*spaceWithTile].position.y == global.boardDimensions) // Bottom
	{
		if ((InputUp() || InputLeft()) && edgeSpace[*spaceWithTile].position.x == 1) // Edge Case Left
		{
			edgeSpace[*spaceWithTile + 1].hasTile = true;
			edgeSpace[*spaceWithTile + 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile += 1;
		}
		else if (InputLeft())
		{
			edgeSpace[*spaceWithTile + 1].hasTile = true;
			edgeSpace[*spaceWithTile + 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile += 1;
		}

		if ((InputUp() || InputRight()) && edgeSpace[*spaceWithTile].position.x == global.boardDimensions - 2) // Edge Case Right
		{
			edgeSpace[*spaceWithTile - 1].hasTile = true;
			edgeSpace[*spaceWithTile - 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile -= 1;
		}
		else if (InputRight())
		{
			edgeSpace[*spaceWithTile - 1].hasTile = true;
			edgeSpace[*spaceWithTile - 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile -= 1;
		}
	}



	else if (edgeSpace[*spaceWithTile].position.x == -1) // Left
	{
		if ((InputRight() || InputUp()) && edgeSpace[*spaceWithTile].position.y == 1) // Edge Case Top
		{
			edgeSpace[0].hasTile = true;
			edgeSpace[0].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile = 0;
		}
		else if (InputUp())
		{
			edgeSpace[*spaceWithTile + 1].hasTile = true;
			edgeSpace[*spaceWithTile + 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile += 1;
		}

		if ((InputRight() || InputDown()) && edgeSpace[*spaceWithTile].position.y == global.boardDimensions - 2) // Edge Case Bottom
		{
			edgeSpace[*spaceWithTile - 1].hasTile = true;
			edgeSpace[*spaceWithTile - 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile -= 1;
		}
		else if (InputDown())
		{
			edgeSpace[*spaceWithTile - 1].hasTile = true;
			edgeSpace[*spaceWithTile - 1].tile = edgeSpace[*spaceWithTile].tile;
			edgeSpace[*spaceWithTile].hasTile = false;
			*spaceWithTile -= 1;
		}
	}

	// ============== ROTATION ==============

	if (InputRotateCW()) // Clockwise Rotation input pressed
	{
		if (edgeSpace[*spaceWithTile].tile.type == 0) // is straight piece (2 rotations)
			edgeSpace[*spaceWithTile].tile.rotation = (edgeSpace[*spaceWithTile].tile.rotation + 1) % 2;
		else // corner and t-shaped pieces (4 rotations)
			edgeSpace[*spaceWithTile].tile.rotation = (edgeSpace[*spaceWithTile].tile.rotation + 1) % 4;
		//sets id from type + rotatation
		edgeSpace[*spaceWithTile].tile.id = edgeSpace[*spaceWithTile].tile.type + edgeSpace[*spaceWithTile].tile.rotation;
	}
	if (InputRotateCCW()) // Counter-Clockwise Rotation input pressed
	{
		if (edgeSpace[*spaceWithTile].tile.type == 0)
			edgeSpace[*spaceWithTile].tile.rotation = (edgeSpace[*spaceWithTile].tile.rotation + 1) % 2;
		else
		{
			if (edgeSpace[*spaceWithTile].tile.rotation == 0)
			{
				edgeSpace[*spaceWithTile].tile.rotation = 3;
			}
			else
			{
				edgeSpace[*spaceWithTile].tile.rotation--;
			}
		}
		edgeSpace[*spaceWithTile].tile.id = edgeSpace[*spaceWithTile].tile.type + edgeSpace[*spaceWithTile].tile.rotation;
	}
}


//Moves all tiles in row/column of floating tile and creates new floating tile at other end
void MoveTiles(int* spaceWithTile, EdgeSpace edgespace[], Tile board[], Player* player, Global *global)
{
	Vector2 floatingPosition = EdgeSpaceIndexToPos(*spaceWithTile, *global); // tile coords of 'old' floating tile
	Vector2 newTilePosition = floatingPosition; //sets 'new' floating tile coords to those of the 'old' tile (appropriate x or y adjusted later)
	
	if (floatingPosition.y == -1) // Top to Bottom
	{
		if (player->position.x == floatingPosition.x)// checks if player is currently in moving column
		{
			if (player->position.y == global->boardDimensions - 1) return; // do not allow row to move if player is at the edge and will be pushed off
			else player->position.y++;// adjust player position if player is safe
		}

		newTilePosition.y = global->boardDimensions; // setting appropriate y coord for 'new' floating tile
		int newTileIndex = EdgeSpacePosToIndex(newTilePosition, *global); // converting 'new' floating tile coords to edgespace array index
		edgespace[newTileIndex].hasTile = true; // new space now contains tile
		edgespace[newTileIndex].tile = board[(int)(((newTilePosition.y - 1) * global->boardDimensions) + newTilePosition.x)]; // transfer tile infomation from tile above
		for (int i = global->boardDimensions - 1; i > 0; i--)// go through remaining tiles in column (not including top)
		{
			//get tile information from tile above
			board[(int)((i * global->boardDimensions) + newTilePosition.x)] = board[(int)(((i - 1) * global->boardDimensions) + newTilePosition.x)];
		}
		board[(int)newTilePosition.x] = edgespace[*spaceWithTile].tile; //last tile gets tile information from 'old' floating tile
		edgespace[*spaceWithTile].hasTile = false; // 'old' floating tile space no longer contains tile
		*spaceWithTile = newTileIndex; // index now assigned to 'new' edge space index
	}
	else if (floatingPosition.x == global->boardDimensions) // From Right to left
	{
		if (player->position.y == floatingPosition.y)
		{
			if (player->position.x == 0) return;
			else player->position.x--;
		}

		newTilePosition.x = -1;
		int newTileIndex = EdgeSpacePosToIndex(newTilePosition, *global);
		edgespace[newTileIndex].hasTile = true;
		edgespace[newTileIndex].tile = board[(int)(((newTilePosition.y) * global->boardDimensions) + (newTilePosition.x + 1))];
		for (int i = 0; i < global->boardDimensions; i++)
		{
			board[(int)((newTilePosition.y * global->boardDimensions) + i)] = board[(int)((newTilePosition.y * global->boardDimensions) + i + 1)];
		}
		board[(int)((newTilePosition.y * global->boardDimensions) + global->boardDimensions - 1)] = edgespace[*spaceWithTile].tile;
		edgespace[*spaceWithTile].hasTile = false;
		*spaceWithTile = newTileIndex;
	}
	else if (floatingPosition.y == global->boardDimensions) // From Bottom to top
	{
		if (player->position.x == floatingPosition.x)
		{
			if (player->position.y == 0) return;
			else player->position.y--;
		}

		newTilePosition.y = -1;
		int newTileIndex = EdgeSpacePosToIndex(newTilePosition, *global);
		edgespace[newTileIndex].hasTile = true;
		edgespace[newTileIndex].tile = board[(int)(((newTilePosition.y + 1) * global->boardDimensions) + (newTilePosition.x))];
		for (int i = 0; i < global->boardDimensions; i++)
		{
			board[(int)((i * global->boardDimensions) + newTilePosition.x)] = board[(int)(((i + 1) * global->boardDimensions) + newTilePosition.x)];
		}
		board[(int)(((global->boardDimensions - 1) * global->boardDimensions) + newTilePosition.x)] = edgespace[*spaceWithTile].tile;
		edgespace[*spaceWithTile].hasTile = false;
		*spaceWithTile = newTileIndex;
	}
	else if (floatingPosition.x == -1) // From Left to right
	{
		if (player->position.y == floatingPosition.y)
		{
			if (player->position.x == global->boardDimensions - 1) return;
			else player->position.x++;
		}

		newTilePosition.x = global->boardDimensions;
		int newTileIndex = EdgeSpacePosToIndex(newTilePosition, *global);
		edgespace[newTileIndex].hasTile = true;
		edgespace[newTileIndex].tile = board[(int)(((newTilePosition.y) * global->boardDimensions) + (global->boardDimensions - 1))];
		for (int i = global->boardDimensions - 1; i > 0; i--)
		{
			board[(int)((newTilePosition.y * global->boardDimensions) + i)] = board[(int)((newTilePosition.y * global->boardDimensions) + i - 1)];
		}
		board[(int)(newTilePosition.y * global->boardDimensions)] = edgespace[*spaceWithTile].tile;
		edgespace[*spaceWithTile].hasTile = false;
		*spaceWithTile = newTileIndex;
	}
	global->turnState = 1;
}