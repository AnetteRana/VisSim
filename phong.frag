#version 330 core

in vec4 col;
in vec4 normal;
in vec4 fragpos;
in vec2 UV;

out vec4 fragColor;

uniform vec3 lightPos = vec3(0,3,0);
uniform vec3 lightColor = vec3(1.0f,0.5f,0.5f);
uniform float ambientStrength = 2.5f;
uniform vec3 ambientLightColor = vec3(0.8f,0.8f,1.0f);
uniform mat4 vMatrix;

uniform sampler2D texture1;

void main() {


    //vec2 newUV = vec2(gl_FragCoord.x*.0005, gl_FragCoord.y*.0005);


    vec4 objColor = vec4(col);

    // ambient
    vec4 ambient = vec4(ambientStrength * ambientLightColor,1);

    // diffuse
    vec4 norm = normalize(normal);
    vec4 lightDir = normalize(vec4(lightPos, 1) - fragpos);

    float diff = max(dot(norm, lightDir), 0);
    vec4 diffuse = diff * vec4(lightColor, 1);

    // specular
    float specularStrength = 1.9f;
    vec4 viewDir = normalize(-fragpos);
    vec4 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0), 32);
    vec4 specular = specularStrength * spec*vec4(lightColor,1);//* objColor;

    vec4 result = (ambient+diffuse+specular)*vec4(lightColor,1);// * objColor;

    fragColor = result * texture(texture1, UV);
}
