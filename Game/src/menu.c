#include "menu.h"

float scaleFactor; // scaleFactor to multiply based on 1080p
Font TITLE_FONT;
Font BUTTON_FONT;
const int TITLE_SIZE = 125; // relaive to 1080p
const int BUTTON_SPACING = 72; // relaive to 1080p
const int BUTTON_SIZE = 50; // relaive to 1080p
int buttonSize;

// Menu Colors
const Color MENU_BACKGROUND_COLOR = { 20, 20, 20, 255 };
const Color SELECTED_COLOR = { 255, 161, 0, 255 };
const Color DESELECTED_COLOR = { 255, 255, 255, 255 };

Button button[4]; // Button Array
int selectedButton; // array index of button selected

void MainMenu(int* gameState, Global global, bool* newGame)
{
	*newGame = true;
	scaleFactor = (global.windowHeight / 1080.f);
	int TitleSize = TITLE_SIZE * scaleFactor;
	buttonSize = BUTTON_SIZE * scaleFactor;
	TITLE_FONT = LoadFontEx("assets/Montserrat-ExtraBold.ttf", TitleSize, 0, 250);
	InitButtons();
	char controlText[] = "Move - Arrow Keys, WASD\nSelect - ENTER, F";
	int controlSize = 30 * scaleFactor;

	while (*gameState == 0)
	{
		if (IsKeyPressed(KEY_ENTER)) *gameState = 1;
		if (WindowShouldClose()) *gameState = -1;
		if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

		InteractButtons(gameState, &selectedButton, newGame);

		//-------- Draw --------
		BeginDrawing();

		Vector2 origin;
		origin.x = MeasureTextEx(TITLE_FONT, "LABYRINTH", TitleSize, 1.f).x / 2;
		origin.y = 0.f;

		ClearBackground(MENU_BACKGROUND_COLOR);
		DrawTextPro(TITLE_FONT, "LABYRINTH", (Vector2) { global.windowWidth / 2, global.windowHeight / 3 }, origin, 0.f, TitleSize, 1.f, WHITE);
		DrawButtons(global);

		origin.y = MeasureTextEx(TITLE_FONT, controlText, controlSize, 1.f).y;
		origin.x = 0.f;
		DrawTextPro(TITLE_FONT, controlText, (Vector2) { (10.f * scaleFactor) , global.windowHeight - (10.f * scaleFactor)
		}, origin, 0.f, controlSize, 1.f, WHITE);

		EndDrawing();
	}
}

// Init Buttons structs and font for main menu
void InitButtons()
{
	BUTTON_FONT = LoadFontEx("assets/Montserrat-Light.ttf", buttonSize, 0, 250);
	selectedButton = 0;

	strcpy(button[0].text, "Start Game");
	button[0].selected = true;
	button[0].gameState = 1;
	button[0].color = SELECTED_COLOR;

	strcpy(button[1].text, "Instructions");
	button[1].selected = false;
	button[1].gameState = 3;
	button[1].color = DESELECTED_COLOR;

	strcpy(button[2].text, "Settings");
	button[2].selected = false;
	button[2].gameState = 5;
	button[2].color = DESELECTED_COLOR;

	strcpy(button[3].text, "Quit Game");
	button[3].selected = false;
	button[3].gameState = -1;
	button[3].color = DESELECTED_COLOR;
}

//Draw Buttons for main menu
void DrawButtons(Global global)
{
	Vector2 coords = { global.windowWidth / 2, global.windowHeight * 0.6f };
	for (int i = 0; i < 4; i++)
	{
		Vector2 origin;
		origin.x = MeasureTextEx(BUTTON_FONT, button[i].text, buttonSize, 10.f).x / 2;
		origin.y = 0.f;

		DrawTextPro(BUTTON_FONT, button[i].text, coords, origin, 0.f, buttonSize, 10.f, button[i].color);
		coords.y += BUTTON_SPACING * (global.windowHeight / 1080.f);
	}
	
}

//Check button movement and interaction
void InteractButtons(int* gameState, int* selectedButton)
{
	if (InputMenuDown())
	{
		int next = (*selectedButton == 3) ? 0 : *selectedButton + 1;
		button[next].selected = true;
		button[next].color = SELECTED_COLOR;
		button[*selectedButton].selected = false;
		button[*selectedButton].color = DESELECTED_COLOR;
		*selectedButton = next;
	}
	if (InputMenuUp())
	{
		int next = (*selectedButton == 0) ? 3 : *selectedButton - 1;
		button[next].selected = true;
		button[next].color = SELECTED_COLOR;
		button[*selectedButton].selected = false;
		button[*selectedButton].color = DESELECTED_COLOR;
		*selectedButton = next;
	}
	
	if (InputMenuSelect())
	{
		*gameState = button[*selectedButton].gameState;
	}
}


// =================== PAUSE MENU ===================

void PauseMenu(int* gameState, Global global)
{
	InteractPauseButtons(gameState, &selectedButton); //Check Movement and user intraction
	DrawPauseButtons(global); // Display buttons
}

//Init button structs and font
void InitPauseButtons()
{
	BUTTON_FONT = LoadFontEx("assets/Montserrat-Light.ttf", buttonSize, 0, 250);
	selectedButton = 0;

	strcpy(button[0].text, "Return to Game");
	button[0].selected = true;
	button[0].gameState = 1;
	button[0].color = SELECTED_COLOR;

	strcpy(button[1].text, "Instructions");
	button[1].selected = false;
	button[1].gameState = 3;
	button[1].color = DESELECTED_COLOR;

	strcpy(button[2].text, "Main Menu");
	button[2].selected = false;
	button[2].gameState = 0;
	button[2].color = DESELECTED_COLOR;
}

// Chack movement and user interaction
void InteractPauseButtons(int* gameState, int* selectedButton)
{
	if (InputMenuDown())
	{
		int next = (*selectedButton == 2) ? 0 : *selectedButton + 1;
		button[next].selected = true;
		button[next].color = SELECTED_COLOR;
		button[*selectedButton].selected = false;
		button[*selectedButton].color = DESELECTED_COLOR;
		*selectedButton = next;
	}
	if (InputMenuUp())
	{
		int next = (*selectedButton == 0) ? 2 : *selectedButton - 1;
		button[next].selected = true;
		button[next].color = SELECTED_COLOR;
		button[*selectedButton].selected = false;
		button[*selectedButton].color = DESELECTED_COLOR;
		*selectedButton = next;
	}

	if (InputMenuSelect())
	{
		*gameState = button[*selectedButton].gameState;
		//if (*selectedButton == 0) 
	}
}
//Display pause menu buttons
void DrawPauseButtons(Global global)
{
	Vector2 coords = { 25 * (global.windowWidth / 1920.f), global.windowHeight * 0.6f };

	for (int i = 0; i < 3; i++)
	{
		Vector2 origin = { 0, 0 };
		DrawTextPro(BUTTON_FONT, button[i].text, coords, origin, 0.f, buttonSize, 1.f, button[i].color);
		coords.y += BUTTON_SPACING * (global.windowHeight / 1080.f);
	}
}


// =================== INSTRUCTIONS ===================

//Instructions menu
void Instructions(int* gameState, bool newGame, Global global)
{
	float headerSize = 80.f * scaleFactor;
	float bodySize = 28.f * scaleFactor;
	float space = bodySize * 1.0f;
	Font headerFont = LoadFontEx("assets/Montserrat-Bold.ttf", headerSize, 0, 250);
	Font bodyFont = LoadFontEx("assets/Montserrat-Light.ttf", bodySize, 0, 250);
	char instructionsHeader[] = "INSTRUCTIONS:";
	char instructionsText[] = "You have been assigned a number of items to collect that are scattered throughout the labyrinth and you must collect them in order.\nEvery turn, you may use the extra maze tile to move one row or column of the maze by 1 tile.The restrictions are, you can only\nplace the items where the arrows around the labyrinth are, and you may not directly undo the last move.\nOnce you have collected all your pieces, head back to your starting tile to finish!";
	char controlsHeader[] = "CONTROLS:";
	char controlsText[] = "Move Player/Tile Up: W/Up Arrow\nMove Player/Tile Down: S/Down Arrow\nMove Player/ Tile Left: A/Left Arrow\nMove Player/Tile Right: D/Right Arrow\n\nRotate Tile Clock-wise: E/R-CTRL\nRotate Tile Counter Clock-wise: Q/R-ALT\n\nInteract/Next Move: F/ENTER\n\nPause: ESC/BKSPC";

	while (*gameState == 3)
	{
		Vector2 position;
		position.x = global.windowWidth / 2.f;
		position.y = global.windowHeight * 0.1f;
		Vector2 origin;
		origin.y = 0.f;
		BeginDrawing();
		ClearBackground(MENU_BACKGROUND_COLOR);

		origin.x = MeasureTextEx(headerFont, instructionsHeader, headerSize, 1.f).x / 2;
		DrawTextPro(headerFont, instructionsHeader, position, origin, 0.f, headerSize, 1.f, WHITE);

		position.y = position.y + MeasureTextEx(headerFont, instructionsHeader, headerSize, 1.f).y + space;
		origin.x = MeasureTextEx(bodyFont, instructionsText, bodySize, 1.f).x / 2;
		DrawTextPro(bodyFont, instructionsText, position, origin, 0.f, bodySize, 1.f, WHITE);

		position.y = position.y + MeasureTextEx(bodyFont, instructionsText, bodySize, 1.f).y + space;
		origin.x = MeasureTextEx(headerFont, controlsHeader, headerSize, 1.f).x / 2;
		DrawTextPro(headerFont, controlsHeader, position, origin, 0.f, headerSize, 1.f, WHITE);

		position.y = position.y + MeasureTextEx(headerFont, controlsHeader, headerSize, 1.f).y + space;
		origin.x = MeasureTextEx(bodyFont, controlsText, bodySize, 1.f).x / 2;
		DrawTextPro(bodyFont, controlsText, position, origin, 0.f, bodySize, 1.f, WHITE);

		position = (Vector2){ (global.windowHeight * .02f), (global.windowHeight * .02f) };
		origin.x = 0.f;
		DrawTextPro(bodyFont, "Back - ESC / BKSPS", position, origin, 0.f, bodySize, 1.f, WHITE);

		EndDrawing();
		if (InputSuspend()) *gameState = (newGame)? 0 : 2;
		if (WindowShouldClose()) *gameState = -1;
		if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
	}
}

// =================== SETTINGS ===================

//Settings Screen
void Settings(Global* global, int* gameState)
{
	float headerSize = 80.f * scaleFactor;
	float bodySize = 40.f * scaleFactor;
	float space = bodySize * 2.f;
	Font headerFont = LoadFontEx("assets/Montserrat-Bold.ttf", headerSize, 0, 250);
	Font bodyFont = LoadFontEx("assets/Montserrat-Light.ttf", bodySize, 0, 250);
	char headerText[] = "SETTINGS:";
	char buttonText1[] = "Number of Items:";
	char buttonText2[] = "Resolution:";
	char buttonText3[] = "Toggle Fullscreen Mode";
	char buttonText4[] = "Apply";
	char buttonOpt1[8];
	char buttonSel1[8];
	char buttonOpt2[16];
	char buttonSel2[16];
	int numResolutions = 24;
	int resolutionWidth[24] = // all available resolution widths up to 4k
	{
		800, 1024, 1152, 1176, 1280, 1280, 1280, 1280, 1280, 1360, 1366, 1440, 1600, 1600, 1600, 1920, 1920, 1920, 2048, 2048, 2048, 2560, 2560, 3840
	};
	int resolutionHeight[24] = // all available resolution heights up to 4k
	{
		600, 768, 864, 664, 720, 768, 800, 960, 1024, 768, 768, 900, 900, 1024, 1200, 1080, 1200, 1440, 1080, 1280, 1536, 1440, 1600, 2160
	};
	int currentIndex;
	int itemSelect = global->itemsMax;

	for (int i = 0; i < numResolutions; i++)
	{
		if (resolutionWidth[i] == global->windowWidth && resolutionHeight[i] == global->windowHeight)
		{
			currentIndex = i;
		}
	}
	int originalResIndex = currentIndex;
	int currentSelection = 0;

	while (*gameState == 5)
	{
		// different text for selected and unselected options
		strcpy(buttonOpt1, TextFormat("  %i   ", itemSelect));
		strcpy(buttonSel1, TextFormat("< %i > ", itemSelect));
		strcpy(buttonOpt2, TextFormat("  %ix%i  ", resolutionWidth[currentIndex], resolutionHeight[currentIndex]));
		strcpy(buttonSel2, TextFormat("< %ix%i >", resolutionWidth[currentIndex], resolutionHeight[currentIndex]));

		Vector2 position;
		position.x = global->windowWidth / 2.f;
		position.y = global->windowHeight * 0.1f;
		Vector2 origin;
		origin.y = 0.f;

		if (InputMenuDown() && currentSelection >= 3) currentSelection = 0;
		else if (InputMenuUp() && currentSelection <= 0) currentSelection = 3;
		else if (InputMenuDown()) currentSelection++;
		else if (InputMenuUp()) currentSelection--;

		if (currentSelection == 0)
		{
			if (InputMenuRight() && itemSelect >= 24) itemSelect = 1;
			else if (InputMenuLeft() && itemSelect <= 1) itemSelect = 24;
			else if (InputMenuRight()) itemSelect++;
			else if (InputMenuLeft()) itemSelect--;
		}
		else if (currentSelection == 1)
		{
			if (InputMenuRight() && currentIndex >= numResolutions - 1) currentIndex = 0;
			else if (InputMenuLeft() && currentIndex <= 0) currentIndex = numResolutions - 1;
			else if (InputMenuRight()) currentIndex++;
			else if (InputMenuLeft()) currentIndex--;
		}
		else if (currentSelection == 2 && InputMenuSelect())
		{
			currentIndex = originalResIndex;
			ToggleFullscreen();
			if (!IsWindowFullscreen()) SetWindowSize(global->windowWidth, global->windowHeight);
		}
		else if (currentSelection == 3 && InputMenuSelect())
		{
			global->itemsMax = itemSelect;
			if (originalResIndex != currentIndex)
			{
				global->windowWidth = resolutionWidth[currentIndex];
				global->windowHeight = resolutionHeight[currentIndex];
				SetWindowSize(global->windowWidth, global->windowHeight);
			}
			*gameState = 0;
		}

		BeginDrawing();

		ClearBackground(MENU_BACKGROUND_COLOR);

		origin.x = MeasureTextEx(headerFont, headerText, headerSize, 1.f).x / 2;
		DrawTextPro(headerFont, headerText, position, origin, 0.f, headerSize, 1.f, WHITE);
		
		position.y = position.y + (space * 4);
		origin.x = 0.f;
		Vector2 positionLeft = position;
		Vector2 positionRight = position;
		Vector2 originLeft = { 0,0 };
		Vector2 originRight = { 0,0 };

		positionLeft.x = global->windowWidth * 0.3f;
		positionRight.x = global->windowWidth * 0.7f;
		originRight.x = MeasureTextEx(bodyFont, buttonOpt1, bodySize, 1.f).x;
		if (currentSelection == 0)
		{
			DrawTextPro(bodyFont, buttonText1, positionLeft, originLeft, 0.f, bodySize, 1.f, SELECTED_COLOR);
			DrawTextPro(bodyFont, buttonSel1, positionRight, originRight, 0.f, bodySize, 1.f, SELECTED_COLOR);
		}
		else
		{
			DrawTextPro(bodyFont, buttonText1, positionLeft, originLeft, 0.f, bodySize, 1.f, DESELECTED_COLOR);
			DrawTextPro(bodyFont, buttonOpt1, positionRight, originRight, 0.f, bodySize, 1.f, DESELECTED_COLOR);
		}
		positionLeft.y = positionLeft.y + space;
		positionRight.y = positionRight.y + space;
		originRight.x = MeasureTextEx(bodyFont, buttonOpt2, bodySize, 1.f).x;
		if (currentSelection == 1)
		{
			DrawTextPro(bodyFont, buttonText2, positionLeft, originLeft, 0.f, bodySize, 1.f, SELECTED_COLOR);
			DrawTextPro(bodyFont, buttonSel2, positionRight, originRight, 0.f, bodySize, 1.f, SELECTED_COLOR);
		}
		else
		{
			DrawTextPro(bodyFont, buttonText2, positionLeft, originLeft, 0.f, bodySize, 1.f, DESELECTED_COLOR);
			DrawTextPro(bodyFont, buttonOpt2, positionRight, originRight, 0.f, bodySize, 1.f, DESELECTED_COLOR);
		}
		position.y = positionLeft.y + space;
		position.x = positionLeft.x;
		
		if (currentSelection == 2)
		{
			DrawTextPro(bodyFont, buttonText3, position, originLeft, 0.f, bodySize, 1.f, SELECTED_COLOR);
		}
		else
		{
			DrawTextPro(bodyFont, buttonText3, position, originLeft, 0.f, bodySize, 1.f, DESELECTED_COLOR);
		}

		position.y = positionLeft.y + (space * 2);

		if (currentSelection == 3)
		{
			DrawTextPro(bodyFont, buttonText4, position, originLeft, 0.f, bodySize, 1.f, SELECTED_COLOR);
		}
		else
		{
			DrawTextPro(bodyFont, buttonText4, position, originLeft, 0.f, bodySize, 1.f, DESELECTED_COLOR);
		}

		position = (Vector2){ (global->windowHeight * .02f), (global->windowHeight * .02f) };
		origin.x = 0.f;
		DrawTextPro(bodyFont, "Back - ESC / BKSPS", position, origin, 0.f, bodySize, 1.f, WHITE);

		EndDrawing();

		if (InputSuspend()) *gameState = 0;
		if (WindowShouldClose()) *gameState = -1;
		if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
	}
}

// =================== WIN SCREEN ===================
// display win screen
void WinScreen(int* gameState, Global global)
{
	int headerSize = 80.f * scaleFactor;
	Font headerFont = LoadFontEx("assets/Montserrat-ExtraBold.ttf", headerSize, 0, 250);
	char headerText[] = "CONGRATULATIONS!!!";

	float bodySize = 28.f * scaleFactor;
	Font bodyFont = LoadFontEx("assets/Montserrat-Light.ttf", bodySize, 0, 250);
	char bodyText1[] = "You found all the items!";
	char bodyText2[] = "Press ESC to return to the Main Menu...";

	while (*gameState == 4)
	{
		BeginDrawing();
		ClearBackground(MENU_BACKGROUND_COLOR);

		Vector2 position = { (global.windowWidth * 0.5), (global.windowHeight * 0.4) };

		Vector2 origin = { 0, 0 };

		origin.x = MeasureTextEx(headerFont, headerText, headerSize, 1.f).x / 2;
		DrawTextPro(headerFont, headerText, position, origin, 0.f, headerSize, 1.f, WHITE);

		position.y = position.y + MeasureTextEx(headerFont, headerText, headerSize, 1.f).y * 2;

		origin.x = MeasureTextEx(bodyFont, bodyText1, bodySize, 1.f).x / 2;
		DrawTextPro(bodyFont, bodyText1, position, origin, 0.f, bodySize, 1.f, WHITE);

		position.y = position.y + MeasureTextEx(bodyFont, bodyText1, bodySize, 1.f).y * 2;

		origin.x = MeasureTextEx(bodyFont, bodyText2, bodySize, 1.f).x / 2;
		DrawTextPro(bodyFont, bodyText2, position, origin, 0.f, bodySize, 1.f, WHITE);

		EndDrawing();

		if (InputSuspend()) *gameState = 0;
		if (WindowShouldClose()) *gameState = -1;
		if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
	}



}