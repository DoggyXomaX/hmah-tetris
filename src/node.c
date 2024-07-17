#include "node.h"

Node Node_Create() {
  Node node = {
    .Name = NULL,
    .Transform = Transform_Create(),
    .Parent = NULL,
  };
  return node;
}
