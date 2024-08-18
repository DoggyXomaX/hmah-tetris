#include "transform.h"

Transform Transform_Create() {
  Transform transform = {
    .Position = { .X = 0, .Y = 0 },
    .Pivot = { .X = 0.5f, .Y = 0.5f },
    .Rotation = 0, .Scale = 1,
  };
  return transform;
}

void Transform_SetPosition(Transform* this, float x, float y) {
  this->Position.X = x;
  this->Position.Y = y;
}

void Transform_SetPivot(Transform* this, float x, float y) {
  this->Pivot.X = x;
  this->Pivot.Y = y;
}

void Transform_SetRotation(Transform* this, float rotation) {
  this->Rotation = rotation;
}

void Transform_SetScale(Transform* this, float scale) {
  this->Scale = scale;
}
