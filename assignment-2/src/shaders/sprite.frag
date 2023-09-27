#version 330

uniform sampler2D tree1;
uniform float tree1Aspect;
uniform sampler2D tree2;
uniform float tree2Aspect;
uniform sampler2D tree3;
uniform float tree3Aspect;
uniform sampler2D tree4;
uniform float tree4Aspect;
uniform vec3 lightPos;

in vec3 N;
in vec3 P;
in float texIndex;

out vec4 color;

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

void main() {
    float aspect;
    vec2 texCoords;
    if (texIndex < 1) {
        texCoords = getTexCoords(tree1Aspect);
        color = texture(tree1, texCoords);
    } else if (texIndex >= 1 && texIndex < 2) {
        texCoords = getTexCoords(tree2Aspect);
        color = texture(tree2, texCoords);
    } else if (texIndex >= 2 && texIndex < 3) {
        texCoords = getTexCoords(tree3Aspect);
        color = texture(tree3, texCoords);
    } else if (texIndex >= 3 && texIndex < 4) {
        texCoords = getTexCoords(tree4Aspect);
        color = texture(tree4, texCoords);
    }
    // The ambient coeffient
    float ka = 0.6;
    // The light sourse vector
    vec3 L = normalize(lightPos - P);
    if (color.a < 0.1) {
        discard;
    }
    // Flat diffuse lighting
    color = ka * color + diffuse(1 - ka, L, N, color);
    
}
	