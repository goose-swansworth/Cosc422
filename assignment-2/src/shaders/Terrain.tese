#version 400

layout(quads, equal_spacing, ccw) in;

uniform mat4 mvpMatrix;
uniform sampler2D heightMap;
uniform sampler2D mask;
uniform int waveTick;
uniform float waterHeight;

out float worldHeight;
out vec3 worldPosition;
out float normalizedDepth;
out vec2 texCoords;
out float waveHeight;
out float roadMask;
out vec3 N;

float exp_mix(float a, float b, float n, float x) {
    return a * exp(log(b / a) * pow(x, n));
}


vec4 sampleHeightMap(vec4 position) {
    float xMin = -45, xMax = +45, zMin = -45, zMax = 45;
    vec2 heightMapTexCoords = vec2(
        (position.x - xMin) / (xMax - xMin),
        (position.z - zMin) / (zMax - zMin)
    );
    float height = texture(heightMap, heightMapTexCoords).r;
    position.y = height * 10.0;

    float aMin = 0.01;
    float aMax = 0.2;
    float omega = 16; 
    float delta = 0.005;

    float d = (waterHeight - position.y) / waterHeight;
    normalizedDepth = d;
    float amp = exp_mix(aMin, aMax, 2.0, 1 - d);
    waveHeight = (amp - aMin) / aMax;

    if (normalizedDepth > 0) {
        position.y = waterHeight + amp * sin(omega*(d + delta * waveTick));
    }
    
    return position;
}

float sampleMask(vec4 position) {
    float xMin = -45, xMax = +45, zMin = -45, zMax = 45;
    vec2 heightMapTexCoords = vec2(
        (position.x - xMin) / (xMax - xMin),
        (position.z - zMin) / (zMax - zMin)
    );
    return texture(mask, heightMapTexCoords).r;
}


void main() {
    
    vec4 vert;
    float epsilon = 0.15;

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    texCoords = vec2(u, v);

    vec4 p0, p1, p2, p3;
    p0 = gl_in[0].gl_Position;
    p1 = gl_in[1].gl_Position;
    p2 = gl_in[2].gl_Position;
    p3 = gl_in[3].gl_Position;

    vert = (1 - u)*(1 - v) * p0 + u*(1-v) * p1 + u*v * p2 + (1 - u)*v * p3;         
    vert = sampleHeightMap(vert);
    worldPosition = vec3(vert);

    float yN = sampleHeightMap(vert - vec4(0, 0, epsilon, 0)).y;
    float yE = sampleHeightMap(vert + vec4(epsilon, 0, 0, 0)).y;
    float yS = sampleHeightMap(vert + vec4(0, 0, epsilon, 0)).y;
    float yW = sampleHeightMap(vert - vec4(epsilon, 0, 0, 0)).y;

    N = normalize(vec3(2*(yW - yE)*epsilon, 4*epsilon*epsilon, 2*(yN - yS)*epsilon));
    roadMask = sampleMask(vert);



    
    // Output in clip coordinates
    gl_Position = mvpMatrix * vert;
}
