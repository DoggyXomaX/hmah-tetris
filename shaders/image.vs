#version 330

#define M_PI 3.1415926535897932384626433832795

layout (location=0) in vec2 iPos;      // Should be (0, 0) (1, 0) (1, 1) (0, 1)
layout (location=1) in vec2 iTexCoord; // Should be (0, 0) (1, 0) (1, 1) (0, 1)

out vec2 vTexCoord;

// Context resolution
uniform highp vec2 uResolution;
uniform highp vec2 uBaseResolution;

// Sprite transform
uniform highp vec2 uPosition;
uniform highp float uRotation;
uniform highp float uScale;
uniform highp vec2 uPivot;

// Sprite size
uniform highp vec2 uSize;

vec2 rotate(vec2 pos, float angle) {
  float c = cos(angle);
  float s = sin(angle);
  return vec2(
    pos.x * c - pos.y * s,
    pos.y * c + pos.x * s
  );
}

void main() {
  vTexCoord = iTexCoord;

  vec2 pos = iPos;
  float aspect = uBaseResolution.x / uBaseResolution.y;
  vec2 size = vec2(
    uSize.x / uBaseResolution.x * aspect,
    uSize.y / uBaseResolution.y * 1
  ) * uScale;

  pos -= uPivot;
  pos *= size;
  pos = rotate(pos, uRotation);
  pos += uPivot * size; 

  pos += vec2(
    uPosition.x / uBaseResolution.x * aspect,
    uPosition.y / uBaseResolution.y
  );

  pos.y = -pos.y;

  pos *= 2;
  pos -= vec2(1.0 * aspect, -1.0);

  pos.x /= aspect;

  pos *= uBaseResolution / uResolution;
  pos *= uResolution.y / uBaseResolution.y;

  gl_Position = vec4(pos, 0.0, 1.0);
}
