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

    //vec4 shore = diffuse * lightColor * vec4(0.9, 0.8, 0.6, 1) * height/2;
    vec4 water = vec4(0, 0, .5f, 1) * (height+150)/100;//
    vec4 shore = vec4(0.9, 0.8, 0.6, 1) ;
    vec4 grass = vec4(.2, .7, .2, 1) * (height+50)/100;

    if (height < 0.2) // water
    {
        result = water;//diffuse * lightColor * groundColor + height;
    }
    else if (height < 1.2f) // shores
    {
        //groundColor = vec4(0.9, 0.8, 0.6, 1);
        result = shore;//diffuse * lightColor * vec4(0.9, 0.8, 0.6, 1) * height/2;
    }

    else if (height < 1.4f) // shores/grass
    {
        float grassWeight = (height-1.2f) /.2f;
        result = (shore*(1.f-grassWeight)) + (grass*grassWeight);
    }
    else // grass
    {
        //groundColor = vec4(.2, .7, .2, 1);
        result = grass;//diffuse * lightColor * vec4(.2, .7, .2, 1) * height/5;
    }

    /*
    if (norm.y < 0.2f) // cliff side
    {
        result = cliff;
    }
    */

    fragColor = result;//diffuse * lightColor;

}
