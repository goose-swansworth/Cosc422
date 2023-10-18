#version 330 core
out vec4 FragColor;

in vec3 N;
in vec3 P;
in vec2 texCoords;
in float shouldDiscard;
in float texIndex;

uniform vec3 lightPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D imposterTex0;
uniform sampler2D imposterTex1;
uniform sampler2D imposterTex2;
uniform sampler2D imposterTex3;
uniform sampler2D imposterTex4;
uniform sampler2D imposterTex5;
uniform sampler2D imposterTex6;
uniform sampler2D imposterTex7;
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
    vec4 color;
    int i = int(texIndex) % 8;
    if (i == 0) {
        color = texture(imposterTex0, texCoords);
    } else if (i == 1) {
        color = texture(imposterTex1, texCoords);
    } else if (i == 2) {
        color = texture(imposterTex2, texCoords);
    } else if (i == 3) {
        color = texture(imposterTex3, texCoords);
    } else if (i == 4) {
        color = texture(imposterTex4, texCoords);
    } else if (i == 5) {
        color = texture(imposterTex5, texCoords);
    } else if (i == 6) {
        color = texture(imposterTex6, texCoords);
    } else if (i == 7) {
        color = texture(imposterTex7, texCoords);
    }
    if (dot(color.rgb, vec3(1.0)) < 0.01) {
        discard;
    }
    float ka = 0.4;
    vec3 L = normalize(lightPos - P);
    FragColor = ka * color + diffuse(1 - ka, L, N, color);

}

void main()
{   
    if (pass == 0) {
        passOne();
    } else {
        passTwo();
    }
    
   
}