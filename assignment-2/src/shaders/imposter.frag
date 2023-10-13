#version 330 core
out vec4 FragColor;

in vec3 N;
in vec3 P;
in vec2 texCoords;
in float shouldDiscard;

uniform vec3 lightPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D imposterTex;
uniform int pass;
uniform float aspect;

vec4 diffuse(float kd, vec3 L, vec3 N, vec4 color) {
     return kd * max(dot(L, N), 0) * color;
}

vec2 getTexCoords(float aspect) {
    float d1 = 0.5 * (1 - aspect);
    float d2 = 0.5 * (1 + aspect);
    if (gl_PointCoord.x < d1 || gl_PointCoord.x > d2) {
        discard;
    }
    return vec2(
        (gl_PointCoord.x - d1) / (d2 - d1),
        gl_PointCoord.y
    );
}

void passOne() {
    if (shouldDiscard > 5.0) {
        discard;
    }
    vec3 L = normalize(lightPos - P);
    vec4 color = texture(texture_diffuse1, texCoords);
    float ka = 0.3;
    FragColor = ka * color + diffuse(1 - ka, L, N, color);
}

void passTwo() {
    vec2 texCoords = getTexCoords(1.8);
    vec4 color = texture(imposterTex, texCoords);
    if (dot(color.rgb, vec3(1.0)) < 0.01) {
        discard;
    }
    FragColor = color;

}

void main()
{   
    if (pass == 0) {
        passOne();
    } else {
        passTwo();
    }
    
   
}