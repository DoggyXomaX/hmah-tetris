#include "cursor.h"

SDL_Cursor* g_CT_cursors[2];

void Cursor_Init() {
  g_CT_cursors[0] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
  g_CT_cursors[1] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
}

void Cursor_Set(enum CursorType type) {
  if (CT_DEFAULT <= type && type <= CT_POINTER) {
    SDL_SetCursor(g_CT_cursors[type]);
  }
}
