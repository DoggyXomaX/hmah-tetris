#ifndef __TEXT_H__
#define __TEXT_H__

#define FONT "resources/ubuntu_mono_16_512.png"

#include "rgba.h"

#include <inttypes.h>

void Text_Init(void);
void Text_Destroy(void);

void Text_PutChar(int x, int y, int charWidth, int charHeight, uint8_t character, const RGBA* color);
void Text_PutString(int x, int y, int width, int height, const char* text, int textLength, const RGBA* color);

#endif // __TEXT_H__