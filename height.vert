#version 330 core

layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec4 colAttr;

out vec4 pos;
out vec4 normal;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 matrix;

void main() {

//    vec4 newPos = posAttr;
//    if (newPos.y < 0)
//        newPos = vec4(newPos.x, 0, newPos.z, 1);

    gl_Position = pMatrix * vMatrix * matrix * posAttr;
    pos = posAttr;

    normal = normalize(transpose(inverse(matrix))*colAttr);
}
