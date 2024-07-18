#ifndef __FIGURE_H__
#define __FIGURE_H__

#include <stdbool.h>
#include <inttypes.h>
#include <stddef.h>

#include "vector.h"
#include "field.h"

typedef struct Figure_s {
  Vector2i Position;
  size_t Size;
  uint8_t Data[16];
} Figure;

Figure Figure_Create_Box();
Figure Figure_Create_SLeft();
Figure Figure_Create_SRight();
Figure Figure_Create_TBlock();
Figure Figure_Create_LLeft();
Figure Figure_Create_LRight();
Figure Figure_Create_Tetris();

void Figure_RotateLeft(Figure* this, const Field* field);
void Figure_RotateRight(Figure* this, const Field* field);
void Figure_MoveLeft(Figure* this, const Field* field);
void Figure_MoveRight(Figure* this, const Field* field);
void Figure_NextIteration(Figure* this, Field* field, bool* isLost, bool* isPlaced);
void Figure_SetColor(Figure* this, uint8_t colorIndex);
void Figure_PlaceOnStart(Figure* this, Field* field);
void Figure_Print(const Figure* this);

#endif // __FIGURE_H__