#pragma once
#include "raylib.h"
#include "raymath.h"

typedef Vector2 Vec2;
typedef Vector3 Vec3;

#define SCREEN_CENTER_X GetScreenWidth()/2
#define SCREEN_CENTER_Y GetScreenHeight()/2
#define CLASSIC_XBOX_GREEN (Color){16, 124, 16, 255}
#define SLIME_GREEN (Color){0, 255, 0, 255}
#define DEFAULT_ERROR_CODE -1


#ifdef DT_LIMIT
#define DT_LIMIT_FIX dt > DT_LIMIT ? dt = DT_LIMIT : dt
#endif