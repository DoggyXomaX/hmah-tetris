#ifndef __NODE_H__
#define __NODE_H__

#include <stddef.h>
#include "transform.h"

typedef struct Node_s {
  const char* Name;
  Transform Transform;
  struct Node_s* Parent;
} Node;

Node Node_Create(void);

#endif // __NODE_H__