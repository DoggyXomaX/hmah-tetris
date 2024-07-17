#include "basic.h"

const char *basic_vs =
  "#version 330\n"
  "layout(location=0) in vec2 iPos;"
  "layout(location=1) in vec2 iTexCoord;"
  "out vec2 vTexCoord;"
  "void main()"
  "{"
    "vTexCoord=iTexCoord;"
    "gl_Position=vec4(iPos,0,1);"
  "}";

const char *basic_fs =
  "#version 330\n"
  "in vec2 vTexCoord;"
  "out vec4 vColor;"
  "uniform sampler2D uTexture0;"
  "uniform highp vec4 uColor;"
  "void main()"
  "{"
    "vColor=texture(uTexture0,vTexCoord)*uColor;"
  "}";
