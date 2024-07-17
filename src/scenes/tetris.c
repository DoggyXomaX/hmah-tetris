#include "tetris.h"

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>

#include "timing.h"
#include "texture.h"
#include "material.h"
#include "node.h"
#include "sprite.h"
#include "field.h"
#include "figure.h"
#include "input.h"
#include "scene.h"

#include "shaders/image.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define FIELD_X 325
#define FIELD_Y 50
#define BLOCK_SIZE 25

Figure (*FigureFuncs[])(void) = {
  Figure_Create_2x2,
  Figure_Create_3x3_1,
  Figure_Create_3x3_2,
  Figure_Create_3x3_3,
  Figure_Create_3x3_4,
  Figure_Create_3x3_5,
  Figure_Create_4x4,
};

struct {
  Texture backgroundTexture;
  Texture blockTexture;
  Material backgroundMaterial;
  Material blockMaterial;
  Sprite background;
  Sprite fieldSprites[FIELD_HEIGHT][FIELD_WIDTH];
} tetris_data;

struct {
  bool isInitialized;
  bool isLost;

  bool isPaused;

  bool isTetris;
  int disappearX;
  int disappearLinePositions[4];
  int score;

  float iterEndTime;
  float iterInterval;

  float inputEndTime;
  float inputInterval;

  float tetrisEndTime;
  float tetrisInterval;

  Field field;
  Figure figure;
  Figure nextFigure;
} tetris_state;

void GenerateRandomFigure() {
  // TODO: show next figure

  int funcsCount = sizeof(FigureFuncs) / sizeof(FigureFuncs[0]);
  int funcIndex = rand() % funcsCount;
  tetris_state.figure = FigureFuncs[funcIndex]();

  Figure_PlaceOnStart(&tetris_state.figure, &tetris_state.field);
}

void Scenes_Tetris_OnLoad() {
  srand(time(NULL));

  tetris_state.isInitialized = false;

  tetris_data.backgroundTexture = Texture_Load("resources/gameFrame.png");
  if (!tetris_data.backgroundTexture.IsOK) return;

  tetris_data.blockTexture = Texture_Load("resources/block.png");
  if (!tetris_data.blockTexture.IsOK) return;

  tetris_data.backgroundMaterial = Material_Create("Background", image_vs, image_fs, &tetris_data.backgroundTexture.Ptr, 1);
  if (!tetris_data.backgroundMaterial.IsOK) return;

  tetris_data.blockMaterial = Material_Create("Block", image_vs, image_fs, &tetris_data.blockTexture.Ptr, 1);
  if (!tetris_data.blockMaterial.IsOK) return;

  tetris_data.background = Sprite_Create(&tetris_data.backgroundMaterial);
  Sprite_SetSize(&tetris_data.background, g_WindowHandler.BaseSize.Width, g_WindowHandler.BaseSize.Height);
  Sprite_SetName(&tetris_data.background, "Background");
  Sprite_SetPivot(&tetris_data.background, 0, 0);

  tetris_state.field = Field_Create(FIELD_WIDTH, FIELD_HEIGHT);
  GenerateRandomFigure();

  for (size_t y = 0; y < FIELD_HEIGHT; y++) {
    for (size_t x = 0; x < FIELD_WIDTH; x++) {
      tetris_data.fieldSprites[y][x] = Sprite_Create(&tetris_data.blockMaterial);
      Sprite_SetSize(&tetris_data.fieldSprites[y][x], BLOCK_SIZE, BLOCK_SIZE);
      Sprite_SetPivot(&tetris_data.fieldSprites[y][x], 0, 0);
      Sprite_SetPosition(&tetris_data.fieldSprites[y][x], FIELD_X + x * BLOCK_SIZE, FIELD_Y + y * BLOCK_SIZE);
    }
  }

  tetris_state.iterInterval = 0.5f;
  tetris_state.iterEndTime = g_Time + tetris_state.iterInterval;

  tetris_state.inputInterval = 0.05f;
  tetris_state.inputEndTime = g_Time + tetris_state.inputInterval;

  tetris_state.isLost = false;
  tetris_state.isPaused = false;
  tetris_state.isTetris = false;
  tetris_state.disappearX = 0;
  for (int i = 0; i < 4; i++) {
    tetris_state.disappearLinePositions[i] = -1;
  }
  tetris_state.score = 0;

  tetris_state.isInitialized = true;

  printf("Tetris scene loaded!\n");
}

void Scenes_Tetris_UpdateIteration() {
  bool isPlaced;
  Figure_NextIteration(&tetris_state.figure, &tetris_state.field, &tetris_state.isLost, &isPlaced);

  if (tetris_state.isLost) return;

  if (isPlaced) {
    Field_CheckTetris(&tetris_state.field, &tetris_state.isTetris, tetris_state.disappearLinePositions);
    if (tetris_state.isTetris) {
      tetris_state.disappearX = 0;
      tetris_state.tetrisEndTime = g_Time + tetris_state.tetrisInterval;
    }

    GenerateRandomFigure();
  }
}

void Scenes_Tetris_OnIterationUpdate() {
  if (tetris_state.isTetris) return;
  if (tetris_state.isLost) return;
  if (tetris_state.iterEndTime > g_Time) return;

  tetris_state.iterEndTime = g_Time + tetris_state.iterInterval;

  Scenes_Tetris_UpdateIteration();
}

void Scenes_Tetris_OnInputUpdate() {
  if (tetris_state.isTetris) return;
  if (tetris_state.isLost) {
    if (Input_OnKeyDown(SDLK_ESCAPE)) {
      Scene_Load("Tetris");
    }
    return;
  }

  if (Input_OnKeyDown(SDLK_ESCAPE)) {
    tetris_state.isPaused = !tetris_state.isPaused;
  }

  if (tetris_state.isPaused) return;

  if (Input_OnKeyDown(SDLK_UP)) Figure_RotateRight(&tetris_state.figure, &tetris_state.field);
  if (Input_OnKeyDown(SDLK_LEFT)) Figure_MoveLeft(&tetris_state.figure, &tetris_state.field);
  if (Input_OnKeyDown(SDLK_RIGHT)) Figure_MoveRight(&tetris_state.figure, &tetris_state.field);

  if (tetris_state.inputEndTime <= g_Time) {
    tetris_state.inputEndTime = g_Time + tetris_state.inputInterval;

    if (Input_IsKeyPressed(SDLK_DOWN)) {
      Scenes_Tetris_UpdateIteration();
    }
  }
}

void Scenes_Tetris_OnTetrisUpdate() {
  if (!tetris_state.isTetris) return;

  if (tetris_state.tetrisEndTime > g_Time) return;
  tetris_state.tetrisEndTime = g_Time + tetris_state.tetrisInterval;

  for (int i = 0; i < 4; i++) {
    int y = tetris_state.disappearLinePositions[i];
    if (y != -1) {
      tetris_state.field.Data[y * tetris_state.field.Size.Width + tetris_state.disappearX] = 0;
      tetris_state.score++;
    }
  }

  tetris_state.disappearX++;

  if (tetris_state.disappearX >= (int)tetris_state.field.Size.Width) {
    Field_CollectEmptyLines(&tetris_state.field);
    tetris_state.isTetris = false;
    printf("Score: %d\n", tetris_state.score);
  }
}

void Scenes_Tetris_OnUpdate() {
  if (!tetris_state.isInitialized) return;

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  Sprite_Render(&tetris_data.background, true);

  for (size_t y = 0, i = 0; y < FIELD_HEIGHT; y++) {
    for (size_t x = 0; x < FIELD_WIDTH; x++, i++) {
      uint8_t c = tetris_state.field.Data[i];
      if (!c) continue;

      Sprite* cell = &tetris_data.fieldSprites[y][x];
      cell->Color.R = (c & 4) != 0;
      cell->Color.G = (c & 2) != 0;
      cell->Color.B = (c & 1) != 0;

      Sprite_Render(cell, false);
    }
  }

  for (int y = 0, i = 0; y < (int)tetris_state.figure.Size; y++) {
    for (int x = 0; x < (int)tetris_state.figure.Size; x++, i++) {
      uint8_t c = tetris_state.figure.Data[i];
      if (c == 0) continue;

      int fx = tetris_state.figure.Position.X + x;
      int fy = tetris_state.figure.Position.Y + y;
      if (
        fx < 0 || fx >= (int)tetris_state.field.Size.Width ||
        fy < 0 || fy >= (int)tetris_state.field.Size.Height
      ) {
        continue;
      }

      Sprite* cell = &tetris_data.fieldSprites[fy][fx];
      cell->Color.R = (c & 4) != 0;
      cell->Color.G = (c & 2) != 0;
      cell->Color.B = (c & 1) != 0;

      Sprite_Render(cell, false);
    }
  }

  if (!tetris_state.isPaused) {
    Scenes_Tetris_OnIterationUpdate();
    Scenes_Tetris_OnTetrisUpdate();
  }

  Scenes_Tetris_OnInputUpdate();

  WindowHandler_Swap();
}

void Scenes_Tetris_OnDestroy() {
  printf("Tetris scene destroyed!\n");
}