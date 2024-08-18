#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include <stddef.h>
#include <GL/gl3w.h>

typedef struct Drawable_s {
  GLuint VAO;
  GLuint VBOPoints;
  GLuint VBOTexCoords;
  GLuint EBO;
} Drawable;

Drawable Drawable_Create(
  const GLfloat* vertices, size_t verticesCount,
  const GLfloat* texCoords, size_t texCoordsCount,
  const GLuint* indices, size_t indicesCount
);

void Drawable_Destroy(Drawable* this);

#endif // __DRAWABLE_H__