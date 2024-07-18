#include "sprite.h"

static bool g_IsMeshInitialized = false;
static GLuint g_MeshVAO;
static GLuint g_LastUsedProgram;

void Sprite_InitSpriteMesh() {
  if (g_IsMeshInitialized) return;

  g_IsMeshInitialized = true;

  g_MeshVAO = Drawable_Create(
    (const GLfloat[]){ 0, 0, 1, 0, 1, 1, 0, 1 }, 4,
    (const GLfloat[]){ 0, 0, 1, 0, 1, 1, 0, 1 }, 4,
    (const GLuint[]){ 0, 1, 2, 2, 3, 0 }, 6
  ).VAO;
}

Sprite Sprite_Create(Material* material) {
  Sprite sprite = {
    .Super = Node_Create(),
    .Material = material,
    .Size = { 1.0f, 1.0f },
    .Color = { 1.0f, 1.0f, 1.0f, 1.0f },
  };
  return sprite;
}

void Sprite_Render(Sprite* this, bool forceMaterialUpdate) {
  if (forceMaterialUpdate || this->Material->Program != g_LastUsedProgram) {
    Material_SetActive(this->Material);
    Material_BindTextures(this->Material);
  }

  Material_SetVector2(this->Material, "uResolution", g_WindowHandler.Size.Width, g_WindowHandler.Size.Height);
  Material_SetVector2(this->Material, "uBaseResolution", g_WindowHandler.BaseSize.Width, g_WindowHandler.BaseSize.Height);
  Material_SetVector2(this->Material, "uSize", this->Size.Width, this->Size.Height);
  Material_SetVector4(this->Material, "uColor", this->Color.R, this->Color.G, this->Color.B, this->Color.A);

  const Node* node = (const Node*)this;
  Material_SetVector2(this->Material, "uPivot", node->Transform.Pivot.X, node->Transform.Pivot.Y);
  Material_SetVector2(this->Material, "uPosition", node->Transform.Position.X, node->Transform.Position.Y);
  Material_SetFloat(this->Material, "uRotation", node->Transform.Rotation);
  Material_SetFloat(this->Material, "uScale", node->Transform.Scale);

  glBindVertexArray(g_MeshVAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sprite_SetName(Sprite* this, const char* name) {
  this->Super.Name = name;
}

void Sprite_SetSize(Sprite* this, float width, float height) {
  this->Size.Width = width;
  this->Size.Height = height;
}

void Sprite_SetPosition(Sprite* this, float x, float y) {
  Transform_SetPosition(&this->Super.Transform, x, y);
}

void Sprite_SetPivot(Sprite* this, float x, float y) {
  Transform_SetPivot(&this->Super.Transform, x, y);
}
