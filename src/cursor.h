#ifndef __CURSOR_H__
#define __CURSOR_H__

#include <SDL3/SDL.h>

enum CursorType {
  CT_DEFAULT = 0,
  CT_POINTER,
};

void Cursor_Init(void);
void Cursor_Set(enum CursorType type);

#endif // __CURSOR_H__