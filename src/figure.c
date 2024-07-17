#include "figure.h"

#include <stdio.h>
#include <inttypes.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>

Figure Figure_Create_2x2() {
  return (Figure){ 
    .Position = { 0, 0 },
    .Size = 2,
    .Data = {
      1, 1,
      1, 1,
    },
  };
}

Figure Figure_Create_3x3_1() {
  return (Figure){ 
    .Position = { 0, 0 },
    .Size = 3,
    .Data = {
      0, 1, 0,
      1, 1, 0,
      1, 0, 0,
    },
  };
}

Figure Figure_Create_3x3_2() {
  return (Figure){ 
    .Position = { 0, 0 },
    .Size = 3,
    .Data = {
      0, 1, 0,
      0, 1, 1,
      0, 0, 1,
    },
  };
}

Figure Figure_Create_3x3_3() {
  return (Figure){ 
    .Position = { 0, 0 },
    .Size = 3,
    .Data = {
      1, 0, 0,
      1, 1, 0,
      1, 0, 0,
    },
  };
}

Figure Figure_Create_3x3_4() {
  return (Figure){
    .Position = { 0, 0 },
    .Size = 3,
    .Data = {
      0, 1, 0,
      0, 1, 0,
      1, 1, 0,
    },
  };
}

Figure Figure_Create_3x3_5() {
  return (Figure){
    .Position = { 0, 0 },
    .Size = 3,
    .Data = {
      0, 1, 0,
      0, 1, 0,
      0, 1, 1,
    },
  };
}

Figure Figure_Create_4x4() {
  return (Figure){
    .Position = { 0, 0 },
    .Size = 4,
    .Data = {
      0, 1, 0, 0,
      0, 1, 0, 0,
      0, 1, 0, 0,
      0, 1, 0, 0,
    },
  };
}

void Figure_ApplyNewMatrix(Figure* this, const Field* field, uint8_t newMat[16]) {
  for (size_t y = 0, i = 0; y < this->Size; y++) {
    for (size_t x = 0; x < this->Size; x++, i++) {
      // Do not check empty cell btw
      if (!newMat[i]) continue;

      int fx = this->Position.X + x;
      int fy = this->Position.Y + y;

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

  memcpy(this->Data, newMat, this->Size * this->Size);
}

void Figure_ApplyNewPosition(Figure* this, const Field* field, Vector2i newPosition) {
  for (size_t y = 0, i = 0; y < this->Size; y++) {
    for (size_t x = 0; x < this->Size; x++, i++) {
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
  for (size_t y = 0, i = 0; y < this->Size; y++) {
    for (size_t x = 0; x < this->Size; x++, i++) {
      int ty = this->Size - x - 1;
      int tx = y;
      newMat[i] = this->Data[ty * this->Size + tx];
    }
  }

  Figure_ApplyNewMatrix(this, field, newMat);
}

void Figure_RotateRight(Figure* this, const Field* field) {
  uint8_t newMat[16];
  for (size_t y = 0, i = 0; y < this->Size; y++) {
    for (size_t x = 0; x < this->Size; x++, i++) {
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
  for (size_t y = 0, i = 0; y < this->Size; y++) {
    for (size_t x = 0; x < this->Size; x++, i++) {
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

  for (size_t y = 0, i = 0; y < this->Size; y++) {
    for (size_t x = 0; x < this->Size; x++, i++) {
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
  for (size_t y = 0, i = 0; y < this->Size; y++) {
    for (size_t x = 0; x < this->Size; x++, i++) {
      if (this->Data[i]) this->Data[i] = colorIndex;
    }
  }
}

int Figure_GetLeftBound(const Figure* this) {
  for (int x = 0; x < (int)this->Size; x++) {
    for (int y = 0; y < (int)this->Size; y++) {
      if (this->Data[y * this->Size + x] != 0) {
        return (int)x;
      }    
    }   
  }

  return 0;
}

int Figure_GetRightBound(const Figure* this) {
  for (int x = (int)this->Size - 1; x >= 0; x--) {
    for (int y = 0; y < (int)this->Size; y++) {
      if (this->Data[y * this->Size + x] != 0) {
        return (int)x;
      }
    }
  }

  return 0;
}

void Figure_PlaceOnStart(Figure* this, Field* field) {
  int leftOffset = Figure_GetLeftBound(this);
  int rightOffset = Figure_GetRightBound(this);
  int realWidth = rightOffset - leftOffset + 1;
  int allowedWidth = field->Size.Width - realWidth + 1;
  this->Position.X = rand() % allowedWidth - leftOffset;
  this->Position.Y = -this->Size;
  
  Figure_SetColor(this, 1 + rand() % 7); // 1..7
}

void Figure_Print(const Figure* this) {
  for (size_t y = 0; y < this->Size; y++) {
    for (size_t x = 0; x < this->Size; x++) {
      printf("%c", '0' + this->Data[y * this->Size + x]);
    }
    printf("\n");
  }
}
