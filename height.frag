#version 330 core

in vec4 gl_FragCoord;
in vec4 pos;
in vec4 normal;

out vec4 fragColor;

uniform vec4 lightColor = vec4(1.3f,1.3f,1.f, 1);
uniform vec4 lightPos = vec4(1.f,50.f,0.f, 1);

void main() {

    vec4 groundColor;

    // diffuse
    vec4 norm = normalize(normal);
    vec4 lightDir = normalize((lightPos) - pos);
    float diff = max(dot(norm, lightDir), 0);
    vec4 diffuse = diff * lightColor;

    vec4 result;
    float height = pos.y;
    if (height < 0.2) // water
    {
        groundColor = vec4(0, 0, .5f, 1);
        result = diffuse * lightColor * groundColor + height;
    }
    else if (height < 1.4f) // shores
    {
        groundColor = vec4(0.9, 0.8, 0.6, 1);
        result = diffuse * lightColor * groundColor * height/2;
    }
    else // grass
    {
        groundColor = vec4(.2, .7, .2, 1);
        result = diffuse * lightColor * groundColor * height/5;
    }

    fragColor = result;//diffuse * lightColor;

}
