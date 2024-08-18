#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdbool.h>
#include <inttypes.h>
#include "vector.h"

enum MouseButton {
  MB_LEFT,
  MB_MIDDLE,
  MB_RIGHT,
};

// Manage

void Input_Init(void);

void Input_SetKeyPressed(uint32_t key, bool value);

void Input_ResetKeyTriggers(void);
void Input_SetKeyDown(uint32_t key);
void Input_SetKeyUp(uint32_t key);

void Input_SetMousePosition(float x, float y);

void Input_SetMousePressed(enum MouseButton button, bool value);

void Input_ResetMouseTriggers(void);
void Input_SetMouseDown(enum MouseButton button);
void Input_SetMouseUp(enum MouseButton button);

// Usage

bool Input_IsKeyPressed(uint32_t key);
bool Input_OnKeyDown(uint32_t key);
bool Input_OnKeyUp(uint32_t key);

Vector2f Input_GetMousePosition(void);
Vector2f Input_GetBaseMousePosition(void);

bool Input_IsMousePressed(enum MouseButton button);

bool Input_OnMouseDown(enum MouseButton button);
bool Input_OnMouseUp(enum MouseButton button);

#endif // __INPUT_H__