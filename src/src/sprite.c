#include "sprite.h"

static bool g_isMeshInitialized = false;
static Drawable g_drawable;
static GLuint g_lastUsedProgram;

void Sprite_InitSpriteMesh() {
  if (g_isMeshInitialized) return;

  g_isMeshInitialized = true;

  g_drawable = Drawable_Create(
    (const GLfloat[]){ 0, 0, 1, 0, 1, 1, 0, 1 }, 4,
    (const GLfloat[]){ 0, 0, 1, 0, 1, 1, 0, 1 }, 4,
    (const GLuint[]){ 0, 1, 2, 2, 3, 0 }, 6
  );
}

void Sprite_DestroySpriteMesh() {
  if (!g_isMeshInitialized) return;

  g_isMeshInitialized = false;

  Drawable_Destroy(&g_drawable);
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

void Sprite_Destroy(Sprite* this) {
  if (this == NULL) return;
}

void Sprite_Render(Sprite* this, bool forceMaterialUpdate) {
  if (forceMaterialUpdate || this->Material->Program != g_lastUsedProgram) {
    Material_SetActive(this->Material);
    Material_BindTextures(this->Material);
  }

  Material_SetVector2(this->Material, "uResolution", g_Context.Size.Width, g_Context.Size.Height);
  Material_SetVector2(this->Material, "uBaseResolution", g_Context.BaseSize.Width, g_Context.BaseSize.Height);
  Material_SetVector2(this->Material, "uSize", this->Size.Width, this->Size.Height);
  Material_SetVector4(this->Material, "uColor", this->Color.R, this->Color.G, this->Color.B, this->Color.A);

  const Node* node = (const Node*)this;
  Material_SetVector2(this->Material, "uPivot", node->Transform.Pivot.X, node->Transform.Pivot.Y);
  Material_SetVector2(this->Material, "uPosition", node->Transform.Position.X, node->Transform.Position.Y);
  Material_SetFloat(this->Material, "uRotation", node->Transform.Rotation);
  Material_SetFloat(this->Material, "uScale", node->Transform.Scale);

  glBindVertexArray(g_drawable.VAO);
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
