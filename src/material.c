#include "material.h"

#include <stdio.h>
#include <assert.h>

void Material_ErrPrefix(Material* this) {
  fprintf(stderr, "Material[%s]: ", this->Name);
}

void Material_LoadShader(Material* this, GLenum type, const char* source) {
  if (!this->IsOK) return;

  if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) {
    this->IsOK = false;
  
    Material_ErrPrefix(this);

    fprintf(stderr, "Unknown shader type %u!\n", type);
    return;
  }

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, 0);
  glCompileShader(shader);

  bool isVertex = type == GL_VERTEX_SHADER;

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    this->IsOK = false;

    Material_ErrPrefix(this);

    char errorMessage[1024];
    glGetShaderInfoLog(shader, 1024, NULL, errorMessage);
    fprintf(stderr, "Failed to compile %s shader:\n%s\n", isVertex ? "vertex" : "fragment", errorMessage);
    return;
  }

  if (isVertex) {
    this->VertexShader = shader;
  }
  else {
    this->FragmentShader = shader;
  }
}

void Material_CreateProgram(Material* this) {
  if (!this->IsOK) return;

  GLuint program = glCreateProgram();
  glAttachShader(program, this->VertexShader);
  glAttachShader(program, this->FragmentShader);
  glLinkProgram(program);

  glDeleteShader(this->VertexShader);
  glDeleteShader(this->FragmentShader);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    this->IsOK = false;

    Material_ErrPrefix(this);

    char errorMessage[1024];
    glGetProgramInfoLog(program, 1024, 0, errorMessage);
    fprintf(stderr, "Failed to link program:\n%s\n", errorMessage);
    return;
  }

  this->Program = program;
}

Material Material_Create(
  const char* name,
  const char* vertexSource,
  const char* fragmentSource,
  GLuint* textures,
  size_t texturesCount
) {
  Material material;
  material.IsOK = true;
  material.Name = name;
  material.Textures = textures;
  material.TexturesCount = texturesCount;

  Material_LoadShader(&material, GL_VERTEX_SHADER, vertexSource);
  Material_LoadShader(&material, GL_FRAGMENT_SHADER, fragmentSource);
  Material_CreateProgram(&material);

  return material;
}

void Material_UnloadProgram(Material* this) {
  if (this->Program != 0) {
    glDeleteProgram(this->Program);
    this->Program = 0;
  }
}

void Material_SetActive(Material* this) {
  if (this == NULL) {
    fprintf(stderr, "Material is NULL!\n");
    return;
  }

  glUseProgram(this->Program);
}

void Material_BindTextures(Material* this) {
  for (size_t i = 0; i < this->TexturesCount; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, this->Textures[i]);
  }
}

GLint Material_GetParamLocation(Material* this, const char* name) {
  GLint location = glGetUniformLocation(this->Program, name);

  if (location == -1) {
    Material_ErrPrefix(this);
    fprintf(stderr, "Uniform \"%s\" was not found!\n", name);
  }

  return location;
}

void Material_SetFloat(Material* this, const char* name, GLfloat value) {
  GLint location = Material_GetParamLocation(this, name);
  if (location != -1) glUniform1f(location, value);
}

void Material_SetVector2(Material* this, const char* name,
  GLfloat value1, GLfloat value2
) {
  GLint location = Material_GetParamLocation(this, name);
  if (location != -1) glUniform2f(location, value1, value2);
}

void Material_SetVector3(Material* this, const char* name,
  GLfloat value1, GLfloat value2, GLfloat value3
) {
  GLint location = Material_GetParamLocation(this, name);
  if (location != -1) glUniform3f(location, value1, value2, value3);
}

void Material_SetVector4(Material* this, const char* name,
  GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4
) {
  GLint location = Material_GetParamLocation(this, name);
  if (location != -1) glUniform4f(location, value1, value2, value3, value4);
}
