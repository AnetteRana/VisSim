#version 330 core

layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec4 colAttr;
layout(location = 2) in vec2 uvAttr;

out vec4 col;
out vec4 normal;
out vec4 fragpos;
out vec2 UV;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 matrix;

void main() {

    UV = uvAttr;
    col = colAttr;
    gl_Position = pMatrix * vMatrix * matrix * posAttr;

    //fragpos = matrix * posAttr;
    fragpos = vec4(matrix * posAttr);

    // should use this but it gets weird...
    normal = normalize(transpose(inverse(matrix))*colAttr);
    //normal = colAttr;
}
