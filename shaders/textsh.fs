#version 330 core

precision lowp float;
precision lowp int;

in vec2 vTexCoord;

out vec4 oColor;

uniform sampler2D uTexture0;
uniform vec4 uColor;

void main() {
  oColor = texture(uTexture0, vTexCoord) * uColor;
}