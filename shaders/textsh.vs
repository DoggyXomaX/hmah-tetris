#version 330 core

precision lowp int;
precision lowp float;

layout(location = 0) in vec2 iPos;

out vec2 vTexCoord;

uniform vec2 uResolution;
uniform vec2 uBaseResolution;

uniform vec2 uLocation;
uniform vec2 uSize;
uniform uvec2 uLayout;
uniform uint uIndex;

void main() {
  // Tex coords
  uint cols = uLayout.x;
  uint rows = uLayout.y;

  float w = 1.0 / float(cols);
  float h = 1.0 / float(rows);

  float col = float(uIndex % cols);
  float row = float(uIndex / cols);

  vTexCoord = vec2(
    (col + iPos.x) * w,
    (row + iPos.y) * h
  );

  // Position btw
  vec2 pos = iPos;

  float aspect = uBaseResolution.x / uBaseResolution.y;
  vec2 size = vec2(
    uSize.x / uBaseResolution.x * aspect,
    uSize.y / uBaseResolution.y * 1
  );

  pos *= size;
  pos += vec2(
    uLocation.x / uBaseResolution.x * aspect,
    uLocation.y / uBaseResolution.y
  );

  pos.y = -pos.y;

  pos *= 2;
  pos -= vec2(1.0 * aspect, -1.0);

  pos.x /= aspect;

  pos *= uBaseResolution / uResolution;
  pos *= uResolution.y / uBaseResolution.y;

  gl_Position = vec4(pos, 0.0, 1.0);
}