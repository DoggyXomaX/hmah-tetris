#version 330

in vec2 vTexCoord;

out vec4 vColor;

uniform sampler2D uTexture0;
uniform highp vec4 uColor;

void main() {
  vec4 texColor = texture(uTexture0, vTexCoord);
  if (texColor.a < 0.5) discard;
  vColor = texColor * uColor; 
}