#include "hid.h"

//NOTE: Menu and Gameplay controls separated in order to have the possibility to assign them to different keys in settings if desired

//Button for pause in game/back in menu
bool InputSuspend()
{
	if (IsKeyPressed(KEY_ESCAPE))
		TRUE;
	else if (IsKeyPressed(KEY_BACKSPACE))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) // Gamepad 'start' button
		TRUE;
	else
		FALSE;
}

bool InputMenuUp()
{
	if (IsKeyPressed(KEY_W))
		TRUE;
	else if (IsKeyPressed(KEY_UP))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) // Gamepad Dpad Up
		TRUE;
	else
		FALSE;
}

bool InputMenuDown()
{
	if (IsKeyPressed(KEY_S))
		TRUE;
	else if (IsKeyPressed(KEY_DOWN))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) // Gamepad Dpad Down
		TRUE;
	else
		FALSE;
}

bool InputMenuLeft()
{
	if (IsKeyPressed(KEY_A))
		TRUE;
	else if (IsKeyPressed(KEY_LEFT))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) // Gamepad Dpad Left
		TRUE;
	else
		FALSE;
}

bool InputMenuRight()
{
	if (IsKeyPressed(KEY_D))
		TRUE;
	else if (IsKeyPressed(KEY_RIGHT))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) // Gamepad Dpad Right
		TRUE;
	else
		FALSE;
}

bool InputMenuSelect()
{
	if (IsKeyPressed(KEY_F))
		TRUE;
	else if (IsKeyPressed(KEY_ENTER))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) // Gamepad A button
		TRUE;
	else
		FALSE;
}

bool InputUp()
{
	if (IsKeyPressed(KEY_W))
		TRUE;
	else if (IsKeyPressed(KEY_UP))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) // Gamepad Dpad Up
		TRUE;
	else
		FALSE;
}

bool InputDown()
{
	if (IsKeyPressed(KEY_S))
		TRUE;
	else if (IsKeyPressed(KEY_DOWN))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) // Gamepad Dpad Down
		TRUE;
	else
		FALSE;
}

bool InputLeft()
{
	if (IsKeyPressed(KEY_A))
		TRUE;
	else if (IsKeyPressed(KEY_LEFT))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) // Gamepad Dpad Left
		TRUE;
	else
		FALSE;
}

bool InputRight()
{
	if (IsKeyPressed(KEY_D))
		TRUE;
	else if (IsKeyPressed(KEY_RIGHT))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) // Gamepad Dpad Right
		TRUE;
	else
		FALSE;
}

bool InputInteract()
{
	if (IsKeyPressed(KEY_F))
		TRUE;
	else if (IsKeyPressed(KEY_ENTER))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) // Gamepad A button
		TRUE;
	else
		FALSE;
}
bool InputRotateCW()
{
	if (IsKeyPressed(KEY_E))
		TRUE;
	else if (IsKeyPressed(KEY_RIGHT_CONTROL))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) // Gamepad X button
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)) // Gamepad Right Bumper
		TRUE;
	else
		FALSE;
}
bool InputRotateCCW()
{
	if (IsKeyPressed(KEY_Q))
		TRUE;
	else if (IsKeyPressed(KEY_RIGHT_ALT))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) // Gamepad B button
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1)) // Gamepad Left Bumper
		TRUE;
	else
		FALSE;
}