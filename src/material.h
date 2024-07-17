#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <stdbool.h>
#include <stddef.h>
#include <GL/gl3w.h>

typedef struct Material_s {
  bool IsOK;

  const char* Name;

  GLuint VertexShader;
  GLuint FragmentShader;
  GLuint Program;

  GLuint* Textures;
  size_t TexturesCount;
} Material;

Material Material_Create(
  const char* name,
  const char* vertexSource,
  const char* fragmentSource,
  GLuint* textures,
  size_t texturesCount
);

void Material_SetActive(Material* this);
void Material_BindTextures(Material* this);
void Material_UnloadProgram(Material* this);

GLint Material_GetParamLocation(Material* this, const char* name);

void Material_SetFloat(Material* this, const char* name, GLfloat value);
void Material_SetVector2(Material* this, const char* name,
  GLfloat value1, GLfloat value2
);
void Material_SetVector3(Material* this, const char* name,
  GLfloat value1, GLfloat value2, GLfloat value3
);
void Material_SetVector4(Material* this, const char* name,
  GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4
);

#endif // __MATERIAL_H__
