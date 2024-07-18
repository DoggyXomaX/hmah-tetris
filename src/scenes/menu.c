#include "menu.h"

#include <stdio.h>
#include <GL/gl3w.h>

#include "window_handler.h"
#include "app.h"
#include "texture.h"
#include "material.h"
#include "sprite.h"
#include "vector.h"
#include "input.h"
#include "cursor.h"
#include "scene.h"

#include "shaders/image.h"

struct {
  Texture menuTexture;
  Texture startTexture;
  Texture exitTexture;
  Material menuMaterial;
  Material startMaterial;
  Material exitMaterial;
  Sprite menuSprite;
  Sprite startButton;
  Sprite exitButton;
  SDL_Cursor* defaultCursor;
  SDL_Cursor* pointerCursor;
} menu_data;

struct {
  bool isInitialized;
  float x;
} menu_state;

void Scenes_Menu_OnLoad() {
  menu_state.isInitialized = false;

  menu_data.menuTexture = Texture_Load("resources/menu.png");
  if (!menu_data.menuTexture.IsOK) return;

  menu_data.startTexture = Texture_Load("resources/startButton.png");
  if (!menu_data.startTexture.IsOK) return;

  menu_data.exitTexture = Texture_Load("resources/exitButton.png");
  if (!menu_data.exitTexture.IsOK) return;

  menu_data.menuMaterial = Material_Create("Menu", image_vs, image_fs, &menu_data.menuTexture.Ptr, 1);
  if (!menu_data.menuMaterial.IsOK) return;

  menu_data.startMaterial = Material_Create("Start", image_vs, image_fs, &menu_data.startTexture.Ptr, 1);
  if (!menu_data.startMaterial.IsOK) return;

  menu_data.exitMaterial = Material_Create("Exit", image_vs, image_fs, &menu_data.exitTexture.Ptr, 1);
  if (!menu_data.exitMaterial.IsOK) return;

  Sprite_InitSpriteMesh();

  menu_data.menuSprite = Sprite_Create(&menu_data.menuMaterial);
  Sprite_SetSize(&menu_data.menuSprite, g_WindowHandler.BaseSize.Width, g_WindowHandler.BaseSize.Height);
  Sprite_SetName(&menu_data.menuSprite, "Menu");
  Sprite_SetPivot(&menu_data.menuSprite, 0, 0);

  menu_data.startButton = Sprite_Create(&menu_data.startMaterial);
  Sprite_SetSize(&menu_data.startButton, 300, 75);
  Sprite_SetName(&menu_data.startButton, "Start button");
  Sprite_SetPivot(&menu_data.startButton, 0.5, 0);
  Sprite_SetPosition(&menu_data.startButton, 300, 200);

  menu_data.exitButton = Sprite_Create(&menu_data.exitMaterial);
  Sprite_SetSize(&menu_data.exitButton, 300, 75);
  Sprite_SetName(&menu_data.exitButton, "Exit button");
  Sprite_SetPivot(&menu_data.exitButton, 0.5, 0);
  Sprite_SetPosition(&menu_data.exitButton, 300, 326);

  menu_data.defaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
  menu_data.pointerCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);

  menu_state.isInitialized = true;
  printf("Menu loaded!\n");
}

void SetDefaultCursor() {
  SDL_SetCursor(menu_data.defaultCursor);
}

void SetPointerCursor() {
  SDL_SetCursor(menu_data.pointerCursor);
}

void OnStartButtonClick() {
  Cursor_Set(CT_DEFAULT);
  Scene_Load("Tetris");
}

void OnExitButtonClick() {
  g_IsRunning = false;
}

void UpdateSpriteMouseBehavior(Sprite* target, void(*onClick)(void)) {
  Vector2f mouse = Input_GetBaseMousePosition();
  Vector2f pos = target->Super.Transform.Position;

  bool isOn = (
    pos.X <= mouse.X && mouse.X <= pos.X + target->Size.Width &&
    pos.Y <= mouse.Y && mouse.Y <= pos.Y + target->Size.Height
  );

  float v = isOn ? 0.8f : 1.0f;

  target->Color.R = v;
  target->Color.G = v;
  target->Color.B = v;

  if (isOn) {
    Cursor_Set(CT_POINTER);
    if (onClick != NULL && Input_OnMouseDown(MB_LEFT)) {
      onClick();
    }
  }
}


void Scenes_Menu_OnUpdate() {
  if (!menu_state.isInitialized) return;

  glClearColor(0.2706f, 0.3216f, 0.3961f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  Sprite_Render(&menu_data.menuSprite, true);

  Cursor_Set(CT_DEFAULT);

  UpdateSpriteMouseBehavior(&menu_data.startButton, OnStartButtonClick);
  Sprite_Render(&menu_data.startButton, true);

  UpdateSpriteMouseBehavior(&menu_data.exitButton, OnExitButtonClick);
  Sprite_Render(&menu_data.exitButton, true);

  WindowHandler_Swap();
}

void Scenes_Menu_OnDestroy() {
  printf("Menu destroyed!\n");
}
