#ifndef __SCENE_H__
#define __SCENE_H__

#include <stddef.h>

#define SCENE_MAX_COUNT 64

typedef struct Scene_s {
  const char* Name;
  void (*OnLoad)(void);
  void (*OnUpdate)(void);
  void (*OnDestroy)(void);
} Scene;

extern Scene g_Scenes[SCENE_MAX_COUNT];
extern size_t g_ScenesCount;
extern Scene* g_CurrentScene;

void Scene_Create(const char* name, void(*onLoad)(void), void(*onUpdate)(void), void(*onDestroy)(void));
void Scene_Load(const char* name);
void Scene_Unload(void);

#endif // __SCENE_H__