#ifndef __FIELD_H__
#define __FIELD_H__

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

#include "size.h"

#define MAX_FIELD_WIDTH 30
#define MAX_FIELD_HEIGHT 30

typedef struct Field_s {
  uint8_t Data[MAX_FIELD_HEIGHT * MAX_FIELD_WIDTH];
  Sizeu Size;
} Field;

Field Field_Create(size_t width, size_t height);
void Field_Fill(Field* this, uint8_t value);
void Field_CheckTetris(const Field* this, bool* isTetris, int positions[4]);
void Field_CollectEmptyLines(Field* this);
void Field_Print(const Field* this);

#endif // __FIELD_H__