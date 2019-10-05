#version 330 core

in vec4 col;
in vec2 UV;

out vec4 FragColor;

//uniform sampler2D ourTexture;
uniform sampler2D texture1;

void main() {


    //fragColor = col;
    //fragColor = vec4 (1,0,0,0) * col;
    FragColor = texture(texture1, UV);
}
