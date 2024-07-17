#include "app.h"

#include <inttypes.h>
#include <stddef.h>
#include <SDL3/SDL.h>
#include <GL/gl3w.h>

#include "unused.h"
#include "window_handler.h"
#include "input.h"
#include "cursor.h"
#include "timing.h"

#include "scenes/index.h"

bool g_IsRunning = false;

void UpdateSDLEvents() {
  SDL_Event event;

  Input_ResetKeyTriggers();
  Input_ResetMouseTriggers();

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_EVENT_KEY_DOWN: {
        Input_SetKeyDown(event.key.key);
        Input_SetKeyPressed(event.key.key, true);
        break;
      }
      case SDL_EVENT_KEY_UP: {
        Input_SetKeyUp(event.key.key);
        Input_SetKeyPressed(event.key.key, false);
        break;
      }
      case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        Input_SetMouseDown(event.button.button - 1);
        Input_SetMousePressed(event.button.button - 1, true);
        break;
      }
      case SDL_EVENT_MOUSE_BUTTON_UP: {
        Input_SetMouseUp(event.button.button - 1);
        Input_SetMousePressed(event.button.button - 1, false);
        break;
      }
      case SDL_EVENT_MOUSE_MOTION: {
        Input_SetMousePosition(event.motion.x, event.motion.y);
        break;
      }
      case SDL_EVENT_QUIT: {
        g_IsRunning = false;
        break;
      }
      case SDL_EVENT_WINDOW_RESIZED: {
        WindowHandler_OnResize();
        glViewport(0, 0, g_WindowHandler.Size.Width, g_WindowHandler.Size.Height);
        break;
      }
    }
  }
}

void Update() {
  UpdateSDLEvents();
  Scenes_OnUpdate();
}

int App_Init(int argc, char* argv[]) {
  UNUSED(argc);
  UNUSED(argv);

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  WindowHandler_Create(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_HEADER);
  if (!g_WindowHandler.IsOK) return 1;

  SDL_GL_CreateContext(g_WindowHandler.Window);
  gl3wInit();

  Input_Init();
  Cursor_Init();

  Scenes_CreateAll();
  Scenes_LoadFirst();
  g_TargetFPS = 60.0f;

  uint64_t prevTime;
  g_IsRunning = true;
  while (g_IsRunning) {
    uint64_t currentTime = SDL_GetTicks();
    uint64_t delta = currentTime - prevTime;
    g_Time = currentTime / 1000.0f;
    if (delta > 1000.0 / g_TargetFPS) {
      prevTime = currentTime;
      g_DeltaTime = 1.0f / (1000.0f / delta);
      Update();
    }
  }

  Scenes_Destroy();
  WindowHandler_Destroy();

  return 0;
}
