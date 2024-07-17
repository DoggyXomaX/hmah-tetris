#include "drawable.h"

Drawable Drawable_Create(
  const GLfloat* vertices, size_t verticesCount,
  const GLfloat* texCoords, size_t texCoordsCount,
  const GLuint* indices, size_t indicesCount
) {
  Drawable d;

  glGenVertexArrays(1, &d.VAO);
  glBindVertexArray(d.VAO);

  glGenBuffers(1, &d.VBOPoints);
  glBindBuffer(GL_ARRAY_BUFFER, d.VBOPoints);
  glBufferData(GL_ARRAY_BUFFER, verticesCount * (2 * sizeof(GLfloat)), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &d.VBOTexCoords);
  glBindBuffer(GL_ARRAY_BUFFER, d.VBOTexCoords);
  glBufferData(GL_ARRAY_BUFFER, texCoordsCount * (2 * sizeof(GLfloat)), texCoords, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &d.EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);

  return d;
}