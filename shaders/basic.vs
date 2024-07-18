#version 330

layout (location=0) in vec2 iPos;      // Should be (-1, 1) (1, 1) (1, -1) (-1, -1)
layout (location=1) in vec2 iTexCoord; // Should be (0, 0) (1, 0) (1, 1) (0, 1)

out vec2 vTexCoord;

uniform highp vec2 uResolution;
uniform highp vec2 uBaseResolution;

void main() {
  vTexCoord = iTexCoord;

  vec2 pos = iPos;

  float scale = uResolution.y / uBaseResolution.y;

  pos.x *= (uBaseResolution.x * scale) / uResolution.x;

  gl_Position = vec4(pos, 0.0, 1.0);
}