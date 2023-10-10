#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vertexTexCoords;

out vec3 TexCoords;
out vec3 N;
out vec3 P;
out vec2 texCoords;
out float shouldDiscard;

uniform mat4 projViewMatrix;
uniform mat4 modelMatrix;

void main()
{
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