#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 texCoordsGeom[];
in vec3 vertNormalGeom[];
in float normalizedDepthGeom[];

out float worldHeight;
out float vertWaveHeight;
out float normalizedDepth;
out vec2 texCoords;
out vec3 worldPosition;
out vec3 N;



uniform mat4 mvpMatrix;
uniform float waterHeight;
uniform int waveTick;

float exp_mix(float a, float b, float n, float x) {
    return a * exp(log(b / a) * pow(x, n));
}


float waveHeight(vec4 position) {

    float aMin = 0.01;
    float aMax = 0.1;
    float omega = 16;
    float delta = 0.005;

    float d = (waterHeight - position.y) / waterHeight;
    normalizedDepth = d;
    float amp = exp_mix(aMin, aMax, 2.0, 1 - d);
    vertWaveHeight = (amp - aMin) / aMax;

    return amp * sin(omega*(d + delta * waveTick));

}

void main() {
    bool waterFlag;
    if (gl_in[0].gl_Position.y <= waterHeight && gl_in[1].gl_Position.y <= waterHeight && gl_in[2].gl_Position.y <= waterHeight) {
        waterFlag = true;
    } else {
        waterFlag = false;
    }
    for (int i = 0; i < 3; i++) {
        vec4 position = gl_in[i].gl_Position;
        if (waterFlag) {
            position.y = waterHeight + waveHeight(position);
        }
        gl_Position = mvpMatrix * position;
        worldHeight = gl_in[i].gl_Position.y;
        normalizedDepth = normalizedDepthGeom[i];
        texCoords = texCoordsGeom[i];
        worldPosition = vec3(gl_in[i].gl_Position);
        N = vertNormalGeom[i];
        EmitVertex();
    }
    EndPrimitive();
}
