#version 330 core

layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec4 colAttr;
layout(location = 2) in vec2 uvAttr;

out vec4 col;
out vec2 UV;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 matrix;

void main() {
    col = colAttr;
    gl_Position = pMatrix * vMatrix * matrix * posAttr ; // felles(pmat * vmat) *matrix * posAt
    UV = uvAttr;
}
