#include "index.h"

#include "scene.h"
#include "intro.h"
#include "menu.h"
#include "tetris.h"

void Scenes_CreateAll() {
  Scene_Create("Intro", &Scenes_Intro_OnLoad, &Scenes_Intro_OnUpdate, &Scenes_Intro_OnDestroy);
  Scene_Create("Menu", &Scenes_Menu_OnLoad, &Scenes_Menu_OnUpdate, &Scenes_Menu_OnDestroy);
  Scene_Create("Tetris", &Scenes_Tetris_OnLoad, &Scenes_Tetris_OnUpdate, &Scenes_Tetris_OnDestroy);
}

void Scenes_LoadFirst() {
  Scene_Load("Intro");
}

void Scenes_OnUpdate() {
  if (g_CurrentScene == NULL || g_CurrentScene->OnUpdate == NULL) {
    return;
  }

  g_CurrentScene->OnUpdate();
}

void Scenes_Destroy() {
  Scene_Unload();
}
