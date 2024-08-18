#ifndef __RGBA_H__
#define __RGBA_H__

typedef struct RGBA_s {
  float R;
  float G;
  float B;
  float A;
} RGBA;

RGBA RGBA_Invert(const RGBA* this);

#endif // __RGBA_H__