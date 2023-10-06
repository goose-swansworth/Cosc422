#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vertexTexCoords;

out vec3 TexCoords;
out vec3 N;
out vec3 P;
out vec2 texCoords;

uniform mat4 mvpMatrix;

void main()
{
    vec3 p = position;
    if (p.y <= 0) {
        p = vec3(0, 0, 0);
    }
    P = p;
    N = normal;
    texCoords = vertexTexCoords;
    gl_Position = mvpMatrix * vec4(p, 1.0);
}  