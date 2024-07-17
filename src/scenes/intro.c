#include "intro.h"

#include <GL/gl3w.h>
#include <stdio.h>

#include "texture.h"
#include "material.h"
#include "window_handler.h"
#include "drawable.h"
#include "sprite.h"
#include "curve.h"
#include "input.h"
#include "scene.h"
#include "timing.h"

#include "shaders/basic.h"

#define INTRO_PATHS_COUNT 2
#define NEXT_SCENE_NAME "Menu"
#define UNFADE_DURATION 2.0f
#define STAY_DURATION 3.0f
#define FADE_DURATION 1.0f

const char* IntroPaths[] = {
  "resources/intro1.png",
  "resources/intro2.png",
};

struct {
  Texture textures[INTRO_PATHS_COUNT];
  Material material;
  GLuint mesh;
  AnimationCurve curve;
  float curveDuration;
} intro_data;

struct {
  bool isInitialized;
  size_t stage;
  float curveValue;
} intro_state;

void Scenes_Intro_OnLoad() {
  intro_state.isInitialized = false;

  for (size_t i = 0; i < INTRO_PATHS_COUNT; i++) {
    intro_data.textures[i] = Texture_Load(IntroPaths[i]);
    if (!intro_data.textures[i].IsOK) return;
  }

  intro_data.material = Material_Create("Intro", basic_vs, basic_fs, NULL, 0);
  intro_data.mesh = Drawable_Create(
    (const GLfloat[]){ -1, 1, 1, 1, 1, -1, -1, -1 }, 4,
    (const GLfloat[]){ 0, 0, 1, 0, 1, 1, 0, 1 }, 4,
    (const GLuint[]){ 0, 1, 2, 0, 2, 3 }, 6
  ).VAO;
  intro_data.curve = AnimationCurve_CreateLinearFade(0.2f, UNFADE_DURATION, STAY_DURATION, FADE_DURATION, 0.2f);
  intro_data.curveDuration = AnimationCurve_GetMaxTime(&intro_data.curve);

  intro_state.stage = 0;
  intro_state.curveValue = 0.0f;

  intro_state.isInitialized = true;

  printf("Intro loaded!\n");
}

void Scenes_Intro_OnUpdate() {
  if (!intro_state.isInitialized) return;

  if (intro_state.stage >= INTRO_PATHS_COUNT) {
    Scene_Load(NEXT_SCENE_NAME);
    return;
  }

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  Material_SetActive(&intro_data.material);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, intro_data.textures[intro_state.stage].Ptr);

  float v = AnimationCurve_GetValue(&intro_data.curve, intro_state.curveValue);
  Material_SetVector4(&intro_data.material, "uColor", v, v, v, 1.0f);

  glBindVertexArray(intro_data.mesh);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  intro_state.curveValue += g_DeltaTime;

  bool needSkip = Input_OnKeyDown(SDLK_ESCAPE) || Input_OnMouseDown(MB_LEFT); 
  if (needSkip || intro_state.curveValue > intro_data.curveDuration) {
    intro_state.curveValue = 0.0f;
    intro_state.stage++;
  }

  WindowHandler_Swap();
}

void Scenes_Intro_OnDestroy() {
  Material_UnloadProgram(&intro_data.material);
  printf("Intro destroyed!\n");
}