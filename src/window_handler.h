#ifndef __WINDOW_HANDLER_H__
#define __WINDOW_HANDLER_H__

#include <stdbool.h>
#include <SDL3/SDL.h>
#include "size.h"

typedef struct WindowHandler_s {
  bool IsOK;
  Sizei Size;
  Sizef BaseSize;
  const char* Header;
  SDL_Window* Window;
} WindowHandler;

extern WindowHandler g_WindowHandler;

void WindowHandler_Create(int width, int height, const char* header);
void WindowHandler_Swap(void);
void WindowHandler_Destroy(void);
void WindowHandler_OnResize(void);

#endif // __WINDOW_HANDLER_H__