#version 330 core
out vec4 FragColor;

in vec3 N;
in vec3 P;
in vec2 texCoords;
in float yPos;

uniform vec3 lightPos;
uniform sampler2D texture_diffuse1;

vec4 diffuse(float kd, vec3 L, vec3 N, vec4 color) {
     return kd * max(dot(L, N), 0) * color;
}

void main()
{  
    vec3 L = normalize(lightPos - P);
    vec4 color = texture(texture_diffuse1, texCoords);
    float ka = 0.3;
    FragColor = ka * color + diffuse(1 - ka, L, N, color);
    FragColor.a = 1.0;
    if (yPos < 2.5) {
        FragColor.a = color.a;
    } 
}