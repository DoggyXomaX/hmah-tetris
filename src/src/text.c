#include "text.h"

#include <context.h>

#include "material.h"
#include "texture.h"
#include "rgba.h"
#include "shaders/textsh.h"

#include <stdbool.h>
#include <GL/gl3w.h>

typedef struct TextDrawable_s {
  GLuint VBO;
  GLuint EBO;
  GLuint VAO;
} TextDrawable;

const GLfloat _vertices[] = { 0, 0, 1, 0, 1, 1, 0, 1 };
const GLuint _indices[] = { 0, 1, 2, 0, 2, 3 };

static Texture _texture;
static TextDrawable _textDrawable;
static Material _material;
static bool _isMeshInitialized = false;

void Text_Init() {
  if (_isMeshInitialized) return;

  _texture = Texture_Load(FONT);
  if (!_texture.IsOK) return;

  _material = Material_Create("TextMaterial", textsh_vs, textsh_fs, &_texture.Ptr, 1);
  if (!_material.IsOK) {
    glDeleteTextures(1, &_texture.Ptr);
    return;
  }

  glGenVertexArrays(1, &_textDrawable.VAO);
  glBindVertexArray(_textDrawable.VAO);

  glGenBuffers(1, &_textDrawable.VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _textDrawable.VBO);
  glBufferData(GL_ARRAY_BUFFER, 4 * (2 * sizeof(GLfloat)), _vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &_textDrawable.EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _textDrawable.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), _indices, GL_STATIC_DRAW);

  glBindVertexArray(0);

  _isMeshInitialized = true;
}

void Text_Destroy() {
  if (!_isMeshInitialized) return;
  _isMeshInitialized = false;

  glDeleteBuffers(1, &_textDrawable.VBO);
  _textDrawable.VBO = 0;

  glDeleteBuffers(1, &_textDrawable.EBO);
  _textDrawable.EBO = 0;

  glDeleteVertexArrays(1, &_textDrawable.VAO);
  _textDrawable.VAO = 0;
}

void Text_PutChar(const int x, const int y, const int charWidth, const int charHeight, const uint8_t character, const RGBA* color) {
  Material_SetActive(&_material);
  Material_BindTextures(&_material);

  Material_SetVector2(&_material, "uResolution", g_Context.Size.Width, g_Context.Size.Height);
  Material_SetVector2(&_material, "uBaseResolution", g_Context.BaseSize.Width, g_Context.BaseSize.Height);
  Material_SetVector2(&_material, "uLocation", (float)x, (float)y);
  Material_SetVector2(&_material, "uSize", (float)charWidth, (float)charHeight);
  Material_SetUVector2(&_material, "uLayout", 16, 8);
  Material_SetUint(&_material, "uIndex", character);

  if (color == NULL) {
    Material_SetVector4(&_material, "uColor", 1.0f, 1.0f, 1.0f, 1.0f);
  } else {
    Material_SetVector4(&_material, "uColor", color->R, color->G, color->B, color->A);
  }

  glBindVertexArray(_textDrawable.VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Text_PutString(const int x, const int y, const int width, const int height, const char* text, const int textLength, const RGBA* color) {
  const int charWidth = width / textLength;
  const int charHeight = height;
  for (int i = 0; i < textLength; i++) {
    Text_PutChar(x + i * charWidth, y, charWidth, charHeight, text[i], color);
  }
}
