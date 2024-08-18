#include "texture.h"

#include <stdio.h>
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture Texture_Load(const char* path) {
  Texture texture;

  texture.IsOK = false;

  if (path == NULL) {
    fprintf(stderr, "Path is NULL!\n");
    return texture;
  }

  int w, h, comp;
  uint8_t* data = stbi_load(path, &w, &h, &comp, STBI_rgb_alpha);
  if (data == NULL) {
    fprintf(stderr, "Failed to load \"%s\"\n", path);
    return texture;
  }

  texture.Size.Width = w;
  texture.Size.Height = h;

  glGenTextures(1, &texture.Ptr);
  glBindTexture(GL_TEXTURE_2D, texture.Ptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  texture.IsOK = true;

  stbi_image_free(data);

  return texture;
}
