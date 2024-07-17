#include "window_handler.h"

WindowHandler g_WindowHandler = { .IsOK = false, .Window = NULL };

void WindowHandler_Create(int width, int height, const char* header) {
  g_WindowHandler.IsOK = false;

  g_WindowHandler.Size.Width = width;
  g_WindowHandler.Size.Height = height;
  g_WindowHandler.BaseSize.Width = width;
  g_WindowHandler.BaseSize.Height = height;
  g_WindowHandler.Header = header;

  g_WindowHandler.Window = SDL_CreateWindow(
    header,
    width,
    height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
  );

  if (g_WindowHandler.Window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    return;
  }

  g_WindowHandler.IsOK = true;
}

void WindowHandler_Swap() {
  SDL_GL_SwapWindow(g_WindowHandler.Window);
}

void WindowHandler_Destroy() {
  SDL_DestroyWindow(g_WindowHandler.Window);
}

void WindowHandler_OnResize() {
  SDL_GetWindowSizeInPixels(g_WindowHandler.Window, &g_WindowHandler.Size.Width, &g_WindowHandler.Size.Height);

  float aspect = g_WindowHandler.BaseSize.Height / g_WindowHandler.BaseSize.Width;
  g_WindowHandler.Size.Height = g_WindowHandler.Size.Width * aspect;

  SDL_SetWindowSize(g_WindowHandler.Window, g_WindowHandler.Size.Width, g_WindowHandler.Size.Height);
}
