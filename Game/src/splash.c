#include "splash.h"

//Colors
const Color SPLASH_BACKGROUND_COLOR = { 20, 20, 20, 255 };
const Color TEXT_COLOR = { 255, 255, 255, 255 };

// Logo Tetxures
Texture2D exoLogo; // eXomorph
Texture2D raylibLogo; // raylib

// Logo Texture paths
const char EXO_LOGO_PATH[] = "assets/exo_logo.png"; // eXomorph
const char RL_LOGO_PATH[] = "assets/raylib_logo.png"; // raylib

const float LOGO_SCALE = 1.f; // scale to adjust size
float logoScale; // based on resolution
float logoWidth; // based on logo scale
Vector2 exoLogoPosition;// "eXomorph" pixel coords based on resolution
Vector2 rlLogoPosition;// "raylib" pixel coords based on resolution

const int FONT_SIZE = 52; // main font size relative to 1080p
const int CR_SIZE = 18; // copyright font size relative to 1080p

const int DURATION = 240; // duration of each logo in frames
int frameCounter = 0; // for animation
int logoNumber = 0; // which logo to display
float transparency = 0.f; // for fade in/out animation

// Run the splash screen animation
void RunSplash(int*gameState, Global global)
{
	InitImages(global);// initialize logos
	int fontSize = FONT_SIZE * (global.windowHeight / 1080.f); // scale main font
	int copyrightSize = CR_SIZE * (global.windowHeight / 1080.f);// scale copyright font
	Font splashFont = LoadFontEx("assets/Montserrat-Bold.ttf", fontSize, 0, 250); // load font file
	
	// 1 second of blank screen
	for (int i = 0; i < 60; i++)
	{
		BeginDrawing();
		ClearBackground(SPLASH_BACKGROUND_COLOR);
		EndDrawing();
	}

	//display logo
	while (logoNumber != 2 && *gameState != -1) // while there is a logo to be displayed and player hasn't closed the window 
	{
		if (WindowShouldClose()) *gameState = -1; // close window if player presses window exit ('x') button

		if (frameCounter == 0) // initialize position on frame 0
		{
			exoLogoPosition = (Vector2){ ((global.windowWidth / 2) - (logoWidth * logoScale / 2)),((global.windowHeight * 0.45) - (logoWidth * logoScale / 2)) };
			rlLogoPosition = (Vector2){ ((global.windowWidth / 2) - (logoWidth * logoScale / 2)),((global.windowHeight * 0.55) - (logoWidth * logoScale / 2)) };
		}
	
		Color newWhite = { TEXT_COLOR.r, TEXT_COLOR.g, TEXT_COLOR.b, (int)transparency }; // white color with animated transperancy
		BeginDrawing();

		ClearBackground(SPLASH_BACKGROUND_COLOR);
		if (logoNumber == 0) // eXomorph Logo
		{
			DrawTextureEx(exoLogo, exoLogoPosition, 0.f, logoScale, newWhite); // use animated color for transperancy animation
			Vector2 origin;
			origin.x = MeasureTextEx(splashFont, "presents...", fontSize, 5.f).x / 2;
			origin.y = MeasureTextEx(splashFont, "presents...", fontSize, 5.f).y / 2;
			DrawTextPro(splashFont, "presents...", (Vector2) { global.windowWidth / 2, global.windowHeight * 0.75f }, origin, 0.f, fontSize, 5.f, newWhite); // use animated color for transperancy animation
		}
		else // raylib Logo
		{
			Vector2 origin;
			origin.x = MeasureTextEx(splashFont, "Powered by:", fontSize, 5.f).x / 2;
			origin.y = MeasureTextEx(splashFont, "Powered by:", fontSize, 5.f).y / 2;
			DrawTextPro(splashFont, "Powered by:", (Vector2) { global.windowWidth / 2, global.windowHeight * 0.25f }, origin, 0.f, fontSize, 5.f, newWhite); // use animated color for transperancy animation
			DrawTextureEx(raylibLogo, rlLogoPosition, 0.f, logoScale, newWhite);
			origin.x = MeasureTextEx(splashFont, "Copyright (c) 2013-2021 Ramon Santamaria (@raysan5)", copyrightSize, 5.f).x / 2;
			origin.y = MeasureTextEx(splashFont, "Copyright (c) 2013-2021 Ramon Santamaria (@raysan5)", copyrightSize, 5.f).y / 2;
			DrawTextPro(splashFont, "Copyright (c) 2013-2021 Ramon Santamaria (@raysan5)", (Vector2) { global.windowWidth / 2, global.windowHeight * 0.9f }, origin, 0.f, copyrightSize, 5.f, newWhite); // use animated color for transperancy animation
		}
		if (frameCounter < 60 && transparency < 255) // if first second: increase opacity
		{
			transparency += 4.25f;
		}
		else if ((frameCounter > (DURATION - 60) && transparency > 0)) // if last second: decrease opacity
		{
			transparency -= 4.25f;
		}

		frameCounter++; // increase frame counter

		if (frameCounter >= DURATION) // move to next logo
		{
			frameCounter = 0;
			logoNumber++;
		}

		EndDrawing();
	}
}

//initialize logo textures and resolution scale
void InitImages(Global global)
{
	exoLogo = LoadTexture(EXO_LOGO_PATH);
	raylibLogo = LoadTexture(RL_LOGO_PATH);
	logoScale = LOGO_SCALE * (global.windowHeight / 1080.f);
	logoWidth = exoLogo.width;
}