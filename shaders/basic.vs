#version 330

layout (location=0) in vec2 iPos;      // Should be (-1, 1) (1, 1) (1, -1) (-1, -1)
layout (location=1) in vec2 iTexCoord; // Should be (0, 0) (1, 0) (1, 1) (0, 1)

out vec2 vTexCoord;

void main() {
  vTexCoord = iTexCoord;
  gl_Position = vec4(iPos, 0.0, 1.0);
}