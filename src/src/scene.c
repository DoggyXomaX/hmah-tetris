#include "scene.h"

#include <string.h>
#include <stdio.h>

Scene g_Scenes[SCENE_MAX_COUNT];
size_t g_ScenesCount = 0;
Scene* g_CurrentScene = NULL;

void Scene_Create(const char* name, void(*onLoad)(void), void(*onUpdate)(void), void(*onDestroy)(void)) {
  if (name == NULL) {
    fprintf(stderr, "Scene name required!\n");
    return;
  }

  g_Scenes[g_ScenesCount].Name = name;
  g_Scenes[g_ScenesCount].OnLoad = onLoad;
  g_Scenes[g_ScenesCount].OnUpdate = onUpdate;
  g_Scenes[g_ScenesCount].OnDestroy = onDestroy;

  g_ScenesCount++;
}

void Scene_Load(const char* name) {
  if (name == NULL) {
    fprintf(stderr, "Can't load NULL scene!\n");
    return;
  }

  for (size_t i = 0; i < g_ScenesCount; i++) {
    if (strcmp(g_Scenes[i].Name, name) == 0) {
      if (g_CurrentScene != NULL && g_CurrentScene->OnDestroy != NULL) {
        g_CurrentScene->OnDestroy();
      }

      g_CurrentScene = &g_Scenes[i];

      if (g_CurrentScene->OnLoad != NULL) {
        g_CurrentScene->OnLoad();
      }
      return;
    }
  }

  fprintf(stderr, "Scene %s not found!\n", name);
}

void Scene_Unload() {
  if (g_CurrentScene != NULL && g_CurrentScene->OnDestroy != NULL) {
    g_CurrentScene->OnDestroy();
  }

  g_CurrentScene = NULL;
}