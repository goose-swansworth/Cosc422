#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vertexTexCoords;

out vec3 TexCoords;
out vec3 N;
out vec3 P;
out vec2 texCoords;
out float shouldDiscard;
out float texIndex;

uniform mat4 projViewMatrix;
uniform mat4 modelMatrix;
uniform int pass;
uniform sampler2D heightMap;

float sampleHeightMap(vec3 position) {
    float xMin = -45, xMax = 45, zMin = -45, zMax = 45;
    vec2 heightMapTexCoords = vec2(
        (position.x - xMin) / (xMax - xMin),
        (position.z - zMin) / (zMax - zMin)
    );
    return 10.0 * texture(heightMap, heightMapTexCoords).r;
}

// Random number between 0 and 1 based on the sprites position
float rand(vec4 p) {
    return abs(sin(dot(p.xyz, p.zyx)));
}

// Linearly map a range to the unit interval
float mapNormalised(float value, float min1, float max1) {
    return (value - min1) / (max1 - min1);
}

// Linearly map one range to another
float map(float value, float min1, float max1, float min2, float max2) {
     return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

float probFunc(float h, float hMin, float hMax) {
    // float m = (hMax - hMin) / 2;
    // float d = 1 - abs(h - m - hMin) / m;
    // return clamp(pow(d, 2), 0, 1);
    return mapNormalised(h, hMin, hMax);
}

void passOne() {
    if (position.y == 0) {
        shouldDiscard = 10.0;
    } else {
        shouldDiscard = 0.0;
    }
    P = vec3(modelMatrix * vec4(position, 1.0));
    N = normal;
    texCoords = vertexTexCoords;
    gl_Position = projViewMatrix * modelMatrix * vec4(position, 1.0);
}

void passTwo() {
    float epsilon = 0.15;
    float minSpriteHeight = 3;
    float maxSpriteHeight = 11;
    float h = sampleHeightMap(position);

    // Compute the surface normal for lighting
    float yN = sampleHeightMap(position - vec3(0, 0, epsilon));
    float yE = sampleHeightMap(position + vec3(epsilon, 0, 0));
    float yS = sampleHeightMap(position + vec3(0, 0, epsilon));
    float yW = sampleHeightMap(position - vec3(epsilon, 0, 0));
    N = normalize(vec3(2*(yW - yE)*epsilon, 4*epsilon*epsilon, 2*(yN - yS)*epsilon));
    

    vec4 vert = vec4(position.x, h + 0.05, position.z, 1.0);
    // The probability the sprite will be accepted, increases linearly with height
    float q = probFunc(h, minSpriteHeight, maxSpriteHeight);
    // If the sprite is below the height threshold, or is not accepted remove it
    float r = rand(vert);
    float spriteSize;
    if (h < minSpriteHeight || r > q) {
        spriteSize = 0;
    } else {
        spriteSize = 16000 * map(rand(vert.zwxy), 0, 1, 0.75, 2);
    }
    P = vec3(vert);
    texIndex = 100.0 * rand(vert.yzxw);
    vec4 posnC = projViewMatrix * vert;
    gl_PointSize = (1.0 - posnC.z / posnC.w) * spriteSize;
    gl_Position = posnC;
}

void main()
{
    if (pass == 0) {
        passOne();
    } else {
        passTwo();
    }
}  