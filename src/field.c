#include "field.h"

#include <stdio.h>

Field Field_Create(int width, int height) {
  Field field = { .Size = { .Width = width, .Height = height } };
  if (width <= 0 || width > MAX_FIELD_WIDTH) {
    fprintf(stderr, "Error: Field width should be in range 1..%d!\n", MAX_FIELD_WIDTH);
    return field;
  }
  if (height <= 0 || height > MAX_FIELD_HEIGHT) {
    fprintf(stderr, "Error: Field height should be in range 1..%d!\n", MAX_FIELD_HEIGHT);
    return field;
  }

  Field_Fill(&field, 0);

  return field;
}

void Field_Fill(Field* this, uint8_t value) {
  for (int i = 0; i < this->Size.Height * this->Size.Width; i++) {
    this->Data[i] = value;
  }
}

void Field_CheckTetris(const Field* this, bool* isTetris, int positions[4]) {
  // Don't forget to reset first
  *isTetris = false;
  for (int i = 0; i < 4; i++) positions[i] = -1;

  int posIndex = 0;
  for (int y = 0; y < this->Size.Height; y++) {
    for (int x = 0; x < this->Size.Width; x++) {
      // If this line has an empty cell, that is not a tetris
      if (this->Data[y * this->Size.Width + x] == 0) {
        break;
      }

      // If the last cell was not empty like previous, this is really a tetris boy
      if (x == this->Size.Width - 1) {
        *isTetris = true;
        positions[posIndex] = y;
        posIndex++;
      }
    }

    if (posIndex >= 4) break;
  }
}

void Field_CollectEmptyLines(Field* this) {
  for (int y = 0; y < this->Size.Height - 1; y++) {
    bool isCurrentLineEmpty = true;
    for (int x = 0; x < this->Size.Width; x++) {
      int i = y * this->Size.Width + x;
      if (this->Data[i] != 0) {
        isCurrentLineEmpty = false;
        break;
      }
    }

    if (isCurrentLineEmpty) continue;

    bool isNextLineEmpty = true;
    for (int x = 0; x < this->Size.Width; x++) {
      int i = (y + 1) * this->Size.Width + x;
      if (this->Data[i] != 0) {
        isNextLineEmpty = false;
        break;
      }
    }

    if (!isNextLineEmpty) continue;

    for (int dy = y; dy >= 1; dy--) {
      for (int dx = 0; dx < this->Size.Width; dx++) {
        int i = dy * this->Size.Width + dx;
        int nextI = (dy + 1) * this->Size.Width + dx;

        this->Data[nextI] = this->Data[i];
        this->Data[i] = 0;
      }
    }
  }
}

void Field_Print(const Field* this) {
  for (int y = 0, i = 0; y < this->Size.Height; y++) {
    for (int x = 0; x < this->Size.Width; x++, i++) {
      printf("%c", this->Data[i] + '0');
    }
    printf("\n");
  }
}
