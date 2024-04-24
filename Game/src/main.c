// Header Files
#include <stdio.h>
#include "raylib.h"

#include "init.h"
#include "graphics.h"
#include "player.h"
#include "hid.h"
#include "menu.h"

//Global Variables

Global global; // common variables passed to functions via single struct
const Color BACKGROUND_COLOR = { 20, 20, 20, 255 };

Tile board[50]; // 49 board tiles + floating tile (for initialization)
EdgeSpace edgeSpace[12]; // spaces surrounding the board for floating tile
int spaceWithTile = 0;
Player player[4]; // only single player funtionallity implemented, planned multiplayer in future

bool newGame; // true = game is reset, used to maintain current play conditions through state changes in pause menu

int gameState = 0; // -1 = Close Window, 0 = Main Menu, 1 = Gameplay, 2 = Pause Menu, 3 = Instructions, 4 = Win Screen, 5 = Settings

void Start(); // (re-)init gameplay variables 
void Update();// update game info per frame
void Draw(); // Visuals

int main()
{
	// Init Window Settings
	InitWindowProperties(&global);// Resolution required before InitWindow();
	InitWindow(global.windowWidth, global.windowHeight, global.windowName);
	HideCursor();
	SetTargetFPS(60);
	SetExitKey(KEY_NULL); // disables ESC key so it can be used for pause menu while retaining window close (x button) functionality

	//Splash Screen
	RunSplash(&gameState, global);

	//while game is not cosed
	while (gameState != -1)
	{
		if (gameState == 0) MainMenu(&gameState, global, &newGame);
		if (gameState == 5) Settings(&global, &gameState);
		if (gameState == 3) Instructions(&gameState, newGame, global);
		if (gameState == 1)
		{
			// init/reset gameplay variables if newGame == true
			if (newGame) Start();
			newGame = false; // no longer reset variables until Main Menu is reached
			//<ain Gameplay
			while (gameState == 1)
			{
				if (IsKeyPressed(KEY_F11)) ToggleFullscreen(); // fullscreen can be toggled with F11
				Update(); // Update info per frame

				BeginDrawing();

				ClearBackground(BACKGROUND_COLOR);
				Draw(); // Draw GAme info

				EndDrawing();
				if (WindowShouldClose()) gameState = -1; // press window close button to close game
				if (InputSuspend())gameState = 2; // goto pause menu
			}
		}
		else if (gameState == 2) // Pause
		{
			// Initialize button functionality
			InitPauseButtons();
			while (gameState == 2)
			{
				if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
				BeginDrawing();

				ClearBackground(BACKGROUND_COLOR);
				Draw(); // Show game in background, No Update -> no control over the game while in the pause menu
				DrawRectangle(0, 0, global.windowWidth, global.windowHeight, (Color) { 0, 0, 0, 127 }); // Large dark object w/ transparency to darken background
				PauseMenu(&gameState, global); // Update functionality for Pause menu

				EndDrawing();
				if (WindowShouldClose()) gameState = -1;
				if (InputSuspend()) gameState = 1; // Exit pause menu -> back to game
			}
		}
		else if (gameState == 4) WinScreen(&gameState, global); // goto Win screen
	}

	return 0;
}


// Game init/reset
void Start()
{
	edgeSpace[spaceWithTile].hasTile = false;//resets floating tile from previous game
	spaceWithTile = 0; // set to first space
	InitFont(global); // Init font for game HUD
	InitGlobal(&global); // init/reset global variables
	InitEdgeSpaces(&edgeSpace, global); // Init edge spaces for floating tile
	InitItems(board, global); // assignes items to perminant and random tiles
	InitBoard(board, &edgeSpace[0], global); //assigns placement of tiles on board
	bool usedItems[24]; // used to assign random items for player to find
	for (int i = 0; i < 24; i++)
	{
		usedItems[i] = false; // will be assigned true when item of itemID of index i is assigned to player
	}
	for (int i = 0; i < global.numOfPlayers; i++) // For future multiplayer, not currently operating past player 1
	{
		InitPlayer(&player[i], i, usedItems, global); // init/reset player variables
	}
}

//Update game info per frame
void Update()
{
	if (global.turnState == 0) // player is in tile movement phase
	{
		MoveFloatingPiece(&spaceWithTile, edgeSpace, global);// moves floating tile around the outside of the board with player input
		if (InputInteract() && (spaceWithTile != global.lastMoveIndex))// when player has chosen where they want to place their tile
		{
			// move all tiles in row/column with floating tile
			MoveTiles(&spaceWithTile, edgeSpace, board, &player[0], &global); // TODO - change 0 to variable for multiplayer
			global.lastMoveIndex = spaceWithTile; // lastMoveIndex used in preventing player from undoing previous action
		}
	}
	else // player piece movement phase (turnState == 1)
	{
		int interact; // -1 - no input, 0 = input w/ incorrect/no item, 1 = input w/ correct item, 2 = player wins
		interact = InteractItem(&player[0], board, global); // TODO - change 0 to variable for multiplayer 
		if (interact == -1) // no input
			MovePlayer(&player[0], board, global); // allows player to move
		else if (interact == 2) // goto win screen
			gameState = 4;
		else // if 0, 1 - go back to tile movement
			global.turnState = 0;
	}
}

void Draw()
{
	DrawEdgeSpaces(edgeSpace, global); // draw arrows around board and floating tile
	DrawBoard(board, global); // draw tiles and items
	for (int i = 0; i < global.numOfPlayers; i++) // For future multiplayer, not currently operating past player 1
	{
		DrawPlayer(player[i], global); // draws player and their starting position
		DrawPlayerItem(player[i], global); // draws the iem the player is looking for
	}
}