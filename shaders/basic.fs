#version 330

in vec2 vTexCoord;

out vec4 vColor;

uniform sampler2D uTexture0;
uniform highp vec4 uColor;

void main() {
  vColor = texture(uTexture0, vTexCoord) * uColor; 
}