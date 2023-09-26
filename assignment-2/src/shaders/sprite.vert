#version 330
layout (location = 0) in vec3 position;

uniform mat4 mvpMatrix;
uniform sampler2D heightMap;
uniform float treeHeight;
uniform float sandHeight;
uniform float offset;

float sampleHeightMap(vec3 position) {
    float xMin = -45, xMax = +45, zMin = 0, zMax = -90;
    vec2 heightMapTexCoords = vec2(
        (position.x - xMin) / (xMax - xMin),
        (position.z - zMin) / (zMax - zMin)
    );
    return 10.0 * texture(heightMap, heightMapTexCoords).r;
}

void main() {
    float spriteSize = 96.0;
    float h = sampleHeightMap(position);
    vec4 vert = vec4(position.x, h + offset, position.z, 1.0);
     if (h < 4 || h > treeHeight) {
        vert = vec4(0.0, 0.0, 0.0, 1.0);
    }
    vec4 posnC = mvpMatrix * vert;
    gl_PointSize = (1.0 - posnC.z / posnC.w) * spriteSize;
    gl_Position = posnC;
}