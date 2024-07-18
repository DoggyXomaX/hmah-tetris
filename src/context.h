#ifndef __WINDOW_HANDLER_H__
#define __WINDOW_HANDLER_H__

#include <stdbool.h>
#include <SDL3/SDL.h>
#include "size.h"

typedef struct Context_s {
  bool IsOK;
  Sizei Size;
  Sizef BaseSize;
  const char* Header;
  SDL_Window* Window;
} Context;

extern Context g_Context;

void Context_Create(int width, int height, const char* header);
void Context_Swap(void);
void Context_Destroy(void);
void Context_OnResize(void);

#endif // __WINDOW_HANDLER_H__