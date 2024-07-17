#ifndef __SIZE_H__
#define __SIZE_H__

#include "stddef.h"

typedef struct Sizef_s {
  float Width;
  float Height;
} Sizef;

typedef struct Sizei_s {
  int Width;
  int Height;
} Sizei;

typedef struct Sizeu_s {
  size_t Width;
  size_t Height;
} Sizeu;

#endif // __SIZE_H__