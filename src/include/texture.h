#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GL/gl3w.h>
#include <stdbool.h>

#include "size.h"

typedef struct Texture_s {
  bool IsOK;
  GLuint Ptr;
  Sizei Size;
} Texture;

Texture Texture_Load(const char* path);

#endif // __TEXTURE_H__