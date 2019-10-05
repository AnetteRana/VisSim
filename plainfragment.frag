#version 330 core

in vec4 col;
out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
   fragColor = col;
}
