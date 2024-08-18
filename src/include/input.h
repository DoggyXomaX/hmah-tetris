#ifndef __HMAH_INPUT_H__
#define __HMAH_INPUT_H__

#include <stdbool.h>
#include <inttypes.h>
#include "vector.h"

#define MB_LEFT 0
#define MB_MIDDLE 1
#define MB_RIGHT 2

// Manage

void Input_Init(void);

void Input_SetKeyPressed(uint32_t key, bool value);

void Input_ResetKeyTriggers(void);
void Input_SetKeyDown(uint32_t key);
void Input_SetKeyUp(uint32_t key);

void Input_SetMousePosition(float x, float y);

void Input_SetMousePressed(int button, bool value);

void Input_ResetMouseTriggers(void);
void Input_SetMouseDown(int button);
void Input_SetMouseUp(int button);

// Usage

bool Input_IsKeyPressed(uint32_t key);
bool Input_OnKeyDown(uint32_t key);
bool Input_OnKeyUp(uint32_t key);

Vector2f Input_GetMousePosition(void);
Vector2f Input_GetBaseMousePosition(void);

bool Input_IsMousePressed(int button);

bool Input_OnMouseDown(int button);
bool Input_OnMouseUp(int button);

#endif // __HMAH_INPUT_H__