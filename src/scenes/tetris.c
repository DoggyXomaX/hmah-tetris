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

#define NEXT_BLOCK_X 600
#define NEXT_BLOCK_Y 75
#define NEXT_BLOCK_WIDTH 100
#define NEXT_BLOCK_HEIGHT 100

Figure (*FigureFuncs[])(void) = {
  Figure_Create_Box,
  Figure_Create_SLeft,
  Figure_Create_SRight,
  Figure_Create_TBlock,
  Figure_Create_LLeft,
  Figure_Create_LRight,
  Figure_Create_Tetris,
};

struct {
  Texture backgroundTexture;
  Material backgroundMaterial;
  Sprite background;

  Texture backgroundLoseTexture;
  Material backgroundLoseMaterial;
  Sprite backgroundLose;

  Texture blockTexture;
  Material blockMaterial;
  Sprite fieldSprites[FIELD_HEIGHT][FIELD_WIDTH];
  
  Texture pauseTexture;
  Material pauseMaterial;
  Sprite pauseSprite;

  Sprite nextFigureSprites[16];
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
  float slowInputEndTime;
  float slowInputInterval;
  bool wasLeft;
  bool wasRight;

  float tetrisEndTime;
  float tetrisInterval;

  Field field;
  Figure figure;
  Figure nextFigure;
} tetris_state;

void GenerateRandomFigure(Figure* target) {
  int funcsCount = sizeof(FigureFuncs) / sizeof(FigureFuncs[0]);
  int funcIndex = rand() % funcsCount;
  Figure newFigure = FigureFuncs[funcIndex]();

  target->Position = newFigure.Position;
  target->Size = newFigure.Size;
  for (size_t y = 0, i = 0; y < newFigure.Size; y++) {
    for (size_t x = 0; x < newFigure.Size; x++, i++) {
      target->Data[i] = newFigure.Data[i];
    }
  }
}

void SwapFigures() {
  Figure* from = &tetris_state.nextFigure;
  Figure* to = &tetris_state.figure;

  to->Position = from->Position;
  to->Size = from->Size;
  for (size_t y = 0, i = 0; y < from->Size; y++) {
    for (size_t x = 0; x < from->Size; x++, i++) {
      to->Data[i] = from->Data[i];
    }
  }

  Figure_PlaceOnStart(to, &tetris_state.field);
}

void Scenes_Tetris_OnLoad() {
  tetris_state.isInitialized = false;

  // Background

  tetris_data.backgroundTexture = Texture_Load("resources/gameFrame.png");
  if (!tetris_data.backgroundTexture.IsOK) return;

  tetris_data.backgroundMaterial = Material_Create("Background", image_vs, image_fs, &tetris_data.backgroundTexture.Ptr, 1);
  if (!tetris_data.backgroundMaterial.IsOK) return;

  tetris_data.background = Sprite_Create(&tetris_data.backgroundMaterial);
  Sprite_SetSize(&tetris_data.background, g_WindowHandler.BaseSize.Width, g_WindowHandler.BaseSize.Height);
  Sprite_SetName(&tetris_data.background, "Background");
  Sprite_SetPivot(&tetris_data.background, 0, 0);

  // Background lose

  tetris_data.backgroundLoseTexture = Texture_Load("resources/loseFrame.png");
  if (!tetris_data.backgroundLoseTexture.IsOK) return;

  tetris_data.backgroundLoseMaterial = Material_Create("Lose background", image_vs, image_fs, &tetris_data.backgroundLoseTexture.Ptr, 1);
  if (!tetris_data.backgroundLoseMaterial.IsOK) return;

  tetris_data.backgroundLose = Sprite_Create(&tetris_data.backgroundLoseMaterial);
  Sprite_SetSize(&tetris_data.backgroundLose, g_WindowHandler.BaseSize.Width, g_WindowHandler.BaseSize.Height);
  Sprite_SetName(&tetris_data.backgroundLose, "Lose background");
  Sprite_SetPivot(&tetris_data.backgroundLose, 0, 0);

  // Pause icon

  tetris_data.pauseTexture = Texture_Load("resources/pauseIcon.png");
  if (!tetris_data.pauseTexture.IsOK) return;

  tetris_data.pauseMaterial = Material_Create("Pause", image_vs, image_fs, &tetris_data.pauseTexture.Ptr, 1);
  if (!tetris_data.pauseMaterial.IsOK) return;

  tetris_data.pauseSprite = Sprite_Create(&tetris_data.pauseMaterial);
  Sprite_SetSize(&tetris_data.pauseSprite, 56, 56);
  Sprite_SetName(&tetris_data.pauseSprite, "Pause");
  Sprite_SetPivot(&tetris_data.pauseSprite, 0, 0);
  Sprite_SetPosition(&tetris_data.pauseSprite, 600, 195);

  // Blocks

  tetris_data.blockTexture = Texture_Load("resources/block.png");
  if (!tetris_data.blockTexture.IsOK) return;

  tetris_data.blockMaterial = Material_Create("Block", image_vs, image_fs, &tetris_data.blockTexture.Ptr, 1);
  if (!tetris_data.blockMaterial.IsOK) return;

  for (size_t y = 0; y < FIELD_HEIGHT; y++) {
    for (size_t x = 0; x < FIELD_WIDTH; x++) {
      tetris_data.fieldSprites[y][x] = Sprite_Create(&tetris_data.blockMaterial);
      Sprite_SetSize(&tetris_data.fieldSprites[y][x], BLOCK_SIZE, BLOCK_SIZE);
      Sprite_SetPivot(&tetris_data.fieldSprites[y][x], 0, 0);
      Sprite_SetPosition(&tetris_data.fieldSprites[y][x], FIELD_X + x * BLOCK_SIZE, FIELD_Y + y * BLOCK_SIZE);
    }
  }

  // Next figure sprites

  for (size_t i = 0; i < 16; i++) {
    tetris_data.nextFigureSprites[i] = Sprite_Create(&tetris_data.blockMaterial);
    Sprite_SetPivot(&tetris_data.nextFigureSprites[i], 0, 0);
  }

  // Prepare scene

  srand(time(NULL));

  tetris_state.field = Field_Create(FIELD_WIDTH, FIELD_HEIGHT);
  
  // Initial figure create
  GenerateRandomFigure(&tetris_state.nextFigure);
  SwapFigures();
  GenerateRandomFigure(&tetris_state.nextFigure);

  tetris_state.iterInterval = 0.5f;
  tetris_state.iterEndTime = g_Time + tetris_state.iterInterval;

  tetris_state.inputInterval = 0.05f;
  tetris_state.inputEndTime = g_Time + tetris_state.inputInterval;
  tetris_state.slowInputInterval = 0.10f;
  tetris_state.slowInputEndTime = g_Time + tetris_state.slowInputInterval;
  tetris_state.wasLeft = false;
  tetris_state.wasRight = false;

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

    SwapFigures();
    GenerateRandomFigure(&tetris_state.nextFigure);
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
    if (Input_OnKeyDown(SDLK_ESCAPE) || Input_OnMouseDown(MB_LEFT)) {
      Scene_Load("Tetris");
    }
    return;
  }

  if (Input_OnKeyDown(SDLK_ESCAPE)) {
    tetris_state.isPaused = !tetris_state.isPaused;
  }

  if (tetris_state.isPaused) return;

  if (Input_OnKeyDown(SDLK_UP)) {
    Figure_RotateRight(&tetris_state.figure, &tetris_state.field);
  }

  if (Input_OnKeyDown(SDLK_LEFT)) {
    Figure_MoveLeft(&tetris_state.figure, &tetris_state.field);
    tetris_state.wasLeft = true;
  }

  if (Input_OnKeyDown(SDLK_RIGHT)) {
    Figure_MoveRight(&tetris_state.figure, &tetris_state.field);
    tetris_state.wasRight = true;
  }

  if (tetris_state.slowInputEndTime <= g_Time) {
    tetris_state.slowInputEndTime = g_Time + tetris_state.slowInputInterval;
    if (tetris_state.wasLeft) {
      tetris_state.wasLeft = false;
    } else if (Input_IsKeyPressed(SDLK_LEFT)) {
      Figure_MoveLeft(&tetris_state.figure, &tetris_state.field);
    }

    if (tetris_state.wasRight) {
      tetris_state.wasRight = false;
    } else if (Input_IsKeyPressed(SDLK_RIGHT)) {
      Figure_MoveRight(&tetris_state.figure, &tetris_state.field);
    }
  }

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

void Scenes_Tetris_Render() {
  bool isLose = tetris_state.isLost;

  RGBA color = { 0.2706f, 0.3216f, 0.3961f, 1.0f };
  if (isLose) {
    color.R = 1.0f - color.R;
    color.G = 1.0f - color.G;
    color.B = 1.0f - color.B;
  }
  glClearColor(color.R, color.G, color.B, color.A);
  glClear(GL_COLOR_BUFFER_BIT);

  // Background render

  Sprite* background = isLose ? &tetris_data.backgroundLose : &tetris_data.background;
  Sprite_Render(background, true);

  // Game field render

  for (size_t y = 0, i = 0; y < FIELD_HEIGHT; y++) {
    for (size_t x = 0; x < FIELD_WIDTH; x++, i++) {
      uint8_t c = tetris_state.field.Data[i];
      if (!c) continue;

      Sprite* cell = &tetris_data.fieldSprites[y][x];
      cell->Color.R = ((c & 4) >> 2) != isLose;
      cell->Color.G = ((c & 2) >> 1) != isLose;
      cell->Color.B = (c & 1) != isLose;

      Sprite_Render(cell, false);
    }
  }

  // Game field falling figure render

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

  // Next figure render
  
  int figureSize = tetris_state.nextFigure.Size;
  for (int y = 0, i = 0; y < figureSize; y++) {
    for (int x = 0; x < figureSize; x++, i++) {
      uint8_t c = tetris_state.nextFigure.Data[i];
      if (c == 0) continue;

      Sprite* cell = &tetris_data.nextFigureSprites[i];
      float width = NEXT_BLOCK_WIDTH / (float)figureSize;
      float height = NEXT_BLOCK_HEIGHT / (float)figureSize;
      float posX = NEXT_BLOCK_X + x * width;
      float posY = NEXT_BLOCK_Y + y * height;
      Sprite_SetPosition(cell, posX, posY);
      Sprite_SetSize(cell, width, height);
      if (isLose) {
        cell->Color.R = 0.0f;
        cell->Color.G = 0.0f;
        cell->Color.B = 0.0f;
      }

      Sprite_Render(cell, false);
    }
  }

  if (tetris_state.isPaused) {
    Sprite_Render(&tetris_data.pauseSprite, false);
  }
}

void Scenes_Tetris_OnUpdate() {
  if (!tetris_state.isInitialized) return;

  Scenes_Tetris_Render();

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