#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "vector.h"

typedef struct Transform_s {
  Vector2f Position;
  Vector2f Pivot;
  float Rotation;
  float Scale;
} Transform;

Transform Transform_Create(void);
void Transform_SetPosition(Transform* this, float x, float y);
void Transform_SetPivot(Transform* this, float x, float y);
void Transform_SetRotation(Transform* this, float rotation);
void Transform_SetScale(Transform* this, float scale);

#endif // __TRANSFORM_H__