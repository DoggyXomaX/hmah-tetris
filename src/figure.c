#include "figure.h"

#include <stdio.h>
#include <inttypes.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>

const uint8_t figureBox[] = { 1, 1, 1, 1 };
const uint8_t figureSLeft[] = { 0, 1, 0, 1, 1, 0, 1, 0, 0 };
const uint8_t figureSRight[] = { 0, 1, 0, 0, 1, 1, 0, 0, 1 };
const uint8_t figureTBlock[] = { 1, 0, 0, 1, 1, 0, 1, 0, 0 };
const uint8_t figureLLeft[] = { 0, 1, 0, 0, 1, 0, 1, 1, 0 };
const uint8_t figureLRight[] = { 0, 1, 0, 0, 1, 0, 0, 1, 1 };
const uint8_t figureTetris[] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 };

Figure Figure_Create_Box() {
  Figure new = { .Position = { 0, 0 }, .Size = 2 };
  memcpy(&new.Data, &figureBox, 4);
  return new;
}

Figure Figure_Create_SLeft() {
  Figure new = { .Position = { 0, 0 }, .Size = 3 };
  memcpy(&new.Data, &figureSLeft, 9);
  return new;
}

Figure Figure_Create_SRight() {
  Figure new = { .Position = { 0, 0 }, .Size = 3 };
  memcpy(&new.Data, &figureSRight, 9);
  return new;
}

Figure Figure_Create_TBlock() {
  Figure new = { .Position = { 0, 0 }, .Size = 3 };
  memcpy(&new.Data, &figureTBlock, 9);
  return new;
}

Figure Figure_Create_LLeft() {
  Figure new = { .Position = { 0, 0 }, .Size = 3 };
  memcpy(&new.Data, &figureLLeft, 9);
  return new;
}

Figure Figure_Create_LRight() {
  Figure new = { .Position = { 0, 0 }, .Size = 3 };
  memcpy(&new.Data, &figureLRight, 9);
  return new;
}

Figure Figure_Create_Tetris() {
  Figure new = { .Position = { 0, 0 }, .Size = 4 };
  memcpy(&new.Data, &figureTetris, 16);
  return new;
}

void Figure_ApplyNewMatrix(Figure* this, const Field* field, uint8_t newMat[16]) {
  int minX = 0;
  int maxX = field->Size.Width - 1;

  for (int y = 0, i = 0; y < this->Size; y++) {
    for (int x = 0; x < this->Size; x++, i++) {
      // Do not check empty cell btw
      if (newMat[i] == 0) continue;

      int fx = this->Position.X + x;
      int fy = this->Position.Y + y;
      int fi = fy * field->Size.Width + fx;

      minX = fx < minX ? fx : minX;
      maxX = fx > maxX ? fx : maxX;

      bool inX = 0 <= fx && fx < field->Size.Width;
      bool inY = 0 <= fy && fy < field->Size.Height;
      bool isBelowField = fy >= field->Size.Height;
      bool isTouchFieldCell = inX && inY && field->Data[fi] != 0;

      if (isBelowField || isTouchFieldCell) return;
    }
  }

  this->Position.X -= minX;
  this->Position.X -= maxX - (field->Size.Width - 1);

  memcpy(this->Data, newMat, this->Size * this->Size);
}

void Figure_ApplyNewPosition(Figure* this, const Field* field, Vector2i newPosition) {
  for (int y = 0, i = 0; y < this->Size; y++) {
    for (int x = 0; x < this->Size; x++, i++) {
      // Do not check empty cell btw
      if (this->Data[i] == 0) continue;

      int fx = newPosition.X + x;
      int fy = newPosition.Y + y;

      // Out of bounds or hit field cell
      if (
        fx < 0 || fx >= (int)field->Size.Width ||
        fy >= (int)field->Size.Height ||
        (fy >= 0 && field->Data[fy * field->Size.Width + fx] != 0)
      ) {
        return;
      }
    }
  }

  this->Position.X = newPosition.X;
  this->Position.Y = newPosition.Y;
}

void Figure_RotateLeft(Figure* this, const Field* field) {
  uint8_t newMat[16];
  for (int y = 0, i = 0; y < this->Size; y++) {
    for (int x = 0; x < this->Size; x++, i++) {
      int ty = this->Size - x - 1;
      int tx = y;
      newMat[i] = this->Data[ty * this->Size + tx];
    }
  }

  Figure_ApplyNewMatrix(this, field, newMat);
}

void Figure_RotateRight(Figure* this, const Field* field) {
  uint8_t newMat[16];
  for (int y = 0, i = 0; y < this->Size; y++) {
    for (int x = 0; x < this->Size; x++, i++) {
      int ty = x;
      int tx = this->Size - y - 1;
      newMat[i] = this->Data[ty * this->Size + tx];
    }
  }

  Figure_ApplyNewMatrix(this, field, newMat);
}

void Figure_MoveLeft(Figure* this, const Field* field) {
  Figure_ApplyNewPosition(this, field, (Vector2i){
    .X = this->Position.X - 1,
    .Y = this->Position.Y,
  });
}

void Figure_MoveRight(Figure* this, const Field* field) {
  Figure_ApplyNewPosition(this, field, (Vector2i){
    .X = this->Position.X + 1,
    .Y = this->Position.Y,
  });
}

void Figure_FillField(Figure* this, Field* field) {
  for (int y = 0, i = 0; y < this->Size; y++) {
    for (int x = 0; x < this->Size; x++, i++) {
      if (this->Data[i] == 0) continue;

      int fx = this->Position.X + x;
      int fy = this->Position.Y + y;

      if (
        fx < 0 || fx >= (int)field->Size.Width ||
        fy < 0 || fy >= (int)field->Size.Height
      ) {
        continue;
      }

      field->Data[fy * field->Size.Width + fx] = this->Data[i];
    }
  }
}

void Figure_NextIteration(Figure* this, Field* field, bool* isLost, bool* isPlaced) {
  *isPlaced = false;
  *isLost = false;

  for (int y = 0, i = 0; y < this->Size; y++) {
    for (int x = 0; x < this->Size; x++, i++) {
      if (!this->Data[i]) continue;

      int fx = this->Position.X + x;
      int fy = this->Position.Y + y + 1; // Next position

      if (fy < 0) continue;

      // If figure hit floor or field filled cell
      if ((field->Data[fy * field->Size.Width + fx] != 0) || (fy >= (int)field->Size.Height)) {
        *isLost = this->Position.Y < 0;
        *isPlaced = true;

        Figure_FillField(this, field);
        return;
      }
    }
  }

  this->Position.Y++;
}

void Figure_SetColor(Figure* this, uint8_t colorIndex) {
  for (int y = 0, i = 0; y < this->Size; y++) {
    for (int x = 0; x < this->Size; x++, i++) {
      if (this->Data[i] != 0) this->Data[i] = colorIndex;
    }
  }
}

int Figure_GetLeftBound(const Figure* this) {
  for (int x = 0; x < this->Size; x++) {
    for (int y = 0; y < this->Size; y++) {
      int i = y * this->Size + x;
      if (this->Data[i] != 0) return x;
    }   
  }

  return 0;
}

int Figure_GetRightBound(const Figure* this) {
  for (int x = this->Size - 1; x >= 0; x--) {
    for (int y = 0; y < this->Size; y++) {
      int i = y * this->Size + x;
      if (this->Data[i] != 0) return x;
    }
  }

  return 0;
}

void Figure_PlaceOnStart(Figure* this, const Field* field) {
  int leftOffset = Figure_GetLeftBound(this);
  int rightOffset = Figure_GetRightBound(this);
  int realWidth = rightOffset - leftOffset + 1;
  this->Position.X = (field->Size.Width - realWidth) / 2;
  this->Position.Y = -this->Size;
  
  uint8_t colorIndex = 1 + rand() % 7;
  Figure_SetColor(this, colorIndex);
}

void Figure_Print(const Figure* this) {
  for (int y = 0; y < this->Size; y++) {
    for (int x = 0; x < this->Size; x++) {
      printf("%c", '0' + this->Data[y * this->Size + x]);
    }
    printf("\n");
  }
}
