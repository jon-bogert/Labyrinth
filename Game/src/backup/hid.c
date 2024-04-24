#include "hid.h"

float L_DEADZONE_MIN = 0.3f; // inside deadzone for left joystick (>=0.f)
float L_DEADZONE_MAX = 1.0f; // outside deadzone for left joystick (<=1.0f)

float AXIS_RESET = 0.8f; // reset point when using axis as button
float AXIS_TRIGGER = 0.9f; // point in which virtual key is pressed

float AXIS_THROW = 0.25f; // emulates joystick throw (travel time) when using button movement

Vector2 moveAxis = { 0.f, 0.f };

bool LeftAxisInUseX = false;
bool LeftAxisInUseY = false;

bool InputSuspend()
{
	if (IsKeyPressed(KEY_ESCAPE))
		TRUE;
	else if (IsKeyPressed(KEY_BACKSPACE))
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) // Start button
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
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
		TRUE;
	else if (MoveAxisAsButtonUp())
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
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
		TRUE;
	else if (MoveAxisAsButtonDown())
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
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
		TRUE;
	else if (MoveAxisAsButtonLeft())
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
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
		TRUE;
	else if (MoveAxisAsButtonRight())
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
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) // A button
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
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
		TRUE;
	else if (MoveAxisAsButtonUp())
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
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
		TRUE;
	else if (MoveAxisAsButtonDown())
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
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
		TRUE;
	else if (MoveAxisAsButtonLeft())
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
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
		TRUE;
	else if (MoveAxisAsButtonRight())
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
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) // A button
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
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) // X button
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)) // Right Bumper
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
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) // B button
		TRUE;
	else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1)) // Left Bumper
		TRUE;
	else
		FALSE;
}

float DeadZone(float input)
{
	if (fabs(input) < L_DEADZONE_MIN)
	{
		return 0.f;
	}
	else if (fabs(input) > L_DEADZONE_MAX)
	{
		if (input >= 0.f) { return 1.f; } else { return -1.f; }
	}
	else
	{
		if (input >= 0.f)
		{
			return (input - L_DEADZONE_MIN) * (1 / (L_DEADZONE_MAX - L_DEADZONE_MIN));
		}
		else
		{
			return (input + L_DEADZONE_MIN) * (1 / (L_DEADZONE_MAX - L_DEADZONE_MIN));
		}
	}
}

Vector2 MoveAxis()
{
	moveAxis.x = DeadZone(GetGamepadAxisMovement(0, 0));
	moveAxis.y = DeadZone(GetGamepadAxisMovement(0, 1));
		
	return moveAxis;
}

void MoveButtonAxis(Vector2* axis)
{
	Vector2 targetValue = { 0.f, 0.f };
	if (InputRight())
	{
		targetValue.x = 1.f;
	}
	if (InputLeft())
	{
		targetValue.x = -1.f;
	}
	if (InputDown())
	{
		targetValue.y = 1.f;
	}
	if (InputUp())
	{
		targetValue.y = -1.f;
	}

	if ((targetValue.x != 0.f) && (targetValue.y != 0.f))
	{
		targetValue.x = sqrtf(0.5f) * targetValue.x;
		targetValue.y = sqrtf(0.5f) * targetValue.y;
	}

	if ((axis->x < targetValue.x) && (axis->x + AXIS_THROW < targetValue.x))
	{
		axis->x += AXIS_THROW;
	}
	else if ((axis->x > targetValue.x) && (axis->x - AXIS_THROW > targetValue.x))
	{
		axis->x -= AXIS_THROW;
	}
	else if (((axis->x < targetValue.x) && (axis->x + AXIS_THROW >= targetValue.x))
		|| ((axis->x > targetValue.x) && (axis->x - AXIS_THROW <= targetValue.x)))
	{
		axis->x = targetValue.x;
	}

	if ((axis->y < targetValue.y) && (axis->y + AXIS_THROW < targetValue.y))
	{
		axis->y += AXIS_THROW;
	}
	else if ((axis->y > targetValue.y) && (axis->y - AXIS_THROW > targetValue.y))
	{
		axis->y -= AXIS_THROW;
	}
	else if (((axis->y < targetValue.y) && (axis->y + AXIS_THROW >= targetValue.y))
		|| ((axis->y > targetValue.y) && (axis->y - AXIS_THROW <= targetValue.y)))
	{
		axis->y = targetValue.y;
	}
}

bool MoveAxisAsButtonRight()
{
	if (LeftAxisInUseX && MoveAxis().x > AXIS_RESET)
		FALSE;
	else if (LeftAxisInUseX && MoveAxis().x > 0)
		LeftAxisInUseX = false;

	if (MoveAxis().x >= AXIS_TRIGGER)
	{
		LeftAxisInUseX = true;
		TRUE;
	}
	else
		FALSE;
}

bool MoveAxisAsButtonLeft()
{
	if (LeftAxisInUseX && MoveAxis().x < AXIS_RESET * -1)
		FALSE;
	else if (LeftAxisInUseX && MoveAxis().x < 0)
		LeftAxisInUseX = false;

	if (MoveAxis().x <= AXIS_TRIGGER * -1)
	{
		LeftAxisInUseX = true;
		TRUE;
	}
	else
		FALSE;
}

bool MoveAxisAsButtonDown()
{
	if (LeftAxisInUseY && MoveAxis().y > AXIS_RESET)
		FALSE;
	else if (LeftAxisInUseY && MoveAxis().y > 0)
		LeftAxisInUseY = false;

	if (MoveAxis().y >= AXIS_TRIGGER)
	{
		LeftAxisInUseY = true;
		TRUE;
	}
	else
		FALSE;
}

bool MoveAxisAsButtonUp()
{
	if (LeftAxisInUseY && MoveAxis().y < AXIS_RESET * -1)
		FALSE;
	else if (LeftAxisInUseY && MoveAxis().y < 0)
		LeftAxisInUseY = false;

	if (MoveAxis().y <= AXIS_TRIGGER * -1)
	{
		LeftAxisInUseY = true;
		TRUE;
	}
	else
		FALSE;
}