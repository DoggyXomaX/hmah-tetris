#include "input.h"

#include "vector.h"
#include "context.h"

#define KEYS_COUNT 0x2000

static bool g_KeyPresses[KEYS_COUNT];
static bool g_KeyDown[KEYS_COUNT];
static bool g_KeyUp[KEYS_COUNT];

static Vector2f g_MousePosition = { 0, 0 };
static bool g_MousePresses[3];
static bool g_MouseDown[3];
static bool g_MouseUp[3];

// Key pressed

void Input_Init() {
  for (int i = 0; i < KEYS_COUNT; i++) {
    g_KeyPresses[i] = false;
    g_KeyDown[i] = false;
    g_KeyUp[i] = false;
  }
  for (int i = 0; i < 3; i++) {
    g_MousePresses[i] = false;
    g_MouseDown[i] = false;
    g_MouseUp[i] = false;
  }
}

void Input_NormalizeKey(uint32_t* key) {
  if (*key > 0x10000000) *key = (*key & 0xfff) | 0x1000;
}

// Key presses

void Input_SetKeyPressed(uint32_t key, bool value) {
  Input_NormalizeKey(&key);
  g_KeyPresses[key] = value;
}

bool Input_IsKeyPressed(uint32_t key) {
  Input_NormalizeKey(&key);
  return g_KeyPresses[key];
}

// KeyDown/KeyUp

void Input_ResetKeyTriggers() {
  for (int i = 0; i < KEYS_COUNT; i++) {
    g_KeyDown[i] = false;
    g_KeyUp[i] = false;
  }
}

void Input_SetKeyDown(uint32_t key) {
  Input_NormalizeKey(&key);
  if (!g_KeyPresses[key]) g_KeyDown[key] = true;
}

void Input_SetKeyUp(uint32_t key) {
  Input_NormalizeKey(&key);
  if (g_KeyPresses[key]) g_KeyUp[key] = true;
}

bool Input_OnKeyDown(uint32_t key) {
  Input_NormalizeKey(&key);
  return g_KeyDown[key];
}

bool Input_OnKeyUp(uint32_t key) {
  Input_NormalizeKey(&key);
  return g_KeyUp[key];
}

// Mouse position

void Input_SetMousePosition(float x, float y) {
  g_MousePosition.X = x;
  g_MousePosition.Y = y;
}

Vector2f Input_GetMousePosition() {
  return g_MousePosition;
}

Vector2f Input_GetBaseMousePosition() {
  float width = g_Context.Size.Width;
  float height = g_Context.Size.Height;
  float baseWidth = g_Context.BaseSize.Width;
  float baseHeight = g_Context.BaseSize.Height;

  float scale = baseHeight / height;
  float scaledBaseWidth = baseWidth * (height / baseHeight);
  float offsetX = (width - scaledBaseWidth) / 2.0f;

  Vector2f basePosition = {
    .X = (g_MousePosition.X - offsetX) * scale,
    .Y = g_MousePosition.Y * scale,
  };

  return basePosition;
}

// Mouse pressed

bool isAllowedButton(int button) {
  return MB_LEFT <= button && button <= MB_RIGHT;
}

void Input_SetMousePressed(int button, bool value) {
  if (isAllowedButton(button)) g_MousePresses[button] = value;
}

bool Input_IsMousePressed(int button) {
  return isAllowedButton(button) && g_MousePresses[button];
}

// MouseDown/MouseUp

void Input_ResetMouseTriggers() {
  for (int i = 0; i < 3; i++) {
    g_MouseDown[i] = false;
    g_MouseUp[i] = false;
  }
}

void Input_SetMouseDown(int button) {
  if (isAllowedButton(button) && !g_MousePresses[button]) {
    g_MouseDown[button] = true; 
  }
}

void Input_SetMouseUp(int button) {
  if (isAllowedButton(button) && g_MousePresses[button]) {
    g_MouseUp[button] = true;
  }
}

bool Input_OnMouseDown(int button) {
  return isAllowedButton(button) && g_MouseDown[button];
}

bool Input_OnMouseUp(int button) {
  return isAllowedButton(button) && g_MouseUp[button];
}
