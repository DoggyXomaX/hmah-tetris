#include "app.h"

#include <SDL3/SDL.h>
#include <unistd.h>

#include "unused.h"
#include "context.h"
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
        Context_OnResize();
        break;
      }
    }
  }
}

int App_Init(int argc, char* argv[]) {
  UNUSED(argc);
  UNUSED(argv);

  Context_Create(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_HEADER);
  if (!g_Context.IsOK) return 1;

  Input_Init();
  Cursor_Init();

  Scenes_CreateAll();
  Scenes_LoadFirst();

  g_TargetFPS = 60.0f;
  g_DeltaTime = 1.0f / (1000.0f / g_TargetFPS);
  g_IsRunning = true;

  while (g_IsRunning) {
    g_Time = SDL_GetTicks() / 1000.0f;
    
    UpdateSDLEvents();
    Scenes_OnUpdate();

    usleep(1000.0f / g_TargetFPS);
  }

  Scenes_Destroy();
  Context_Destroy();

  return 0;
}
