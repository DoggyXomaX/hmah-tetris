#include "rgba.h"

#include <stddef.h>

RGBA RGBA_Invert(const RGBA* this) {
  return (RGBA){
    .R = this != NULL ? 1.0f - this->R : 0.0f,
    .G = this != NULL ? 1.0f - this->G : 0.0f,
    .B = this != NULL ? 1.0f - this->B : 0.0f,
    .A = this != NULL ? this->A : 1.0f,
  };
}
