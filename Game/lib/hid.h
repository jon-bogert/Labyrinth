#pragma once
#include <stdio.h>
#include "raylib.h"
#include "math.h"

// See .c file (right click -> peek definition) for descriptions

#define TRUE return true
#define FALSE return false

bool InputSuspend();
bool InputMenuUp();
bool InputMenuDown();
bool InputMenuSelect();

bool InputUp();
bool InputDown();
bool InputLeft();
bool InputRight();

bool InputInteract();

bool InputRotateCW();
bool InputRotateCCW();