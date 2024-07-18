#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <GL/gl3w.h>
#include <stdbool.h>

#include "node.h"
#include "drawable.h"
#include "material.h"
#include "transform.h"
#include "context.h"
#include "rgba.h"
#include "size.h"

typedef struct Sprite_s {
  Node Super;
  Material* Material;
  RGBA Color;
  Sizef Size;
} Sprite;

void Sprite_InitSpriteMesh(void);
Sprite Sprite_Create(Material* material);
void Sprite_Render(Sprite* this, bool forceMaterialUpdate);

void Sprite_SetName(Sprite* this, const char* name);
void Sprite_SetSize(Sprite* this, float width, float height);
void Sprite_SetPosition(Sprite* this, float x, float y);
void Sprite_SetPivot(Sprite* this, float x, float y);

#endif // __SPRITE_H__