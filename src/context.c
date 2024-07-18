#include "context.h"

#include <GL/gl3w.h>

Context g_Context = { .IsOK = false, .Window = NULL };

void Context_Create(int width, int height, const char* header) {
  g_Context.IsOK = false;

  g_Context.Size.Width = width;
  g_Context.Size.Height = height;
  g_Context.BaseSize.Width = width;
  g_Context.BaseSize.Height = height;
  g_Context.Header = header;

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  g_Context.Window = SDL_CreateWindow(
    header,
    width,
    height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
  );

  if (g_Context.Window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    return;
  }

  SDL_GL_CreateContext(g_Context.Window);
  gl3wInit();

  g_Context.IsOK = true;
}

void Context_Swap() {
  SDL_GL_SwapWindow(g_Context.Window);
}

void Context_Destroy() {
  SDL_DestroyWindow(g_Context.Window);
}

void Context_OnResize() {
  SDL_GetWindowSizeInPixels(g_Context.Window, &g_Context.Size.Width, &g_Context.Size.Height);
  glViewport(0, 0, g_Context.Size.Width, g_Context.Size.Height);
}
