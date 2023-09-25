#version 330

in float worldHeight;
in float waveHeight;
in float normalizedDepth;
in vec2 texCoords;
in vec3 worldPosition;
in vec3 N;

uniform vec4 eyePos;
uniform float waterHeight;
uniform sampler2D grassTex;
uniform sampler2D waterTex;
uniform sampler2D sandTex;
uniform sampler2D brownGrassTex;
uniform vec3 lightPos;

vec4 diffuse(float kd, vec3 L, vec3 N, vec4 color) {
     return kd * max(dot(L, N), 0) * color;
}

vec4 specular(float ks, vec3 L, vec3 N, vec4 specColor) {
     vec3 V = normalize(vec3(eyePos) - worldPosition);
     vec3 H = normalize(L + V);
     return ks * pow(max(dot(H, N), 0), 10) * specColor;
}

// Linearly map one range to another
float map(float value, float min1, float max1, float min2, float max2) {
     return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

// Linearly map a range to the unit interval
float mapNormalised(float value, float min1, float max1) {
     return (value - min1) / (max1 - min1);
}

void main() 
{
     vec4 white = vec4(1.0);
     float ka = 0.3;
     float kd = 0.7;
     float ks = 0.5;
     vec3 L = normalize(lightPos - worldPosition);
     vec4 tex;
     float sandHeight = 4;
     vec4 groundColor;
     float landStep;

     if (worldPosition.y < sandHeight) {
          landStep = smoothstep(sandHeight, waterHeight, worldPosition.y);
          groundColor = landStep * texture(sandTex, texCoords) + (1 - landStep) * texture(grassTex, texCoords);
     } else {
          groundColor = mix(
               texture(grassTex, texCoords),
               texture(brownGrassTex, texCoords),
               mapNormalised(worldPosition.y, sandHeight, 10)
          );
     }
     float edge = 0.1;
     float s = smoothstep(-edge, edge, normalizedDepth);
     tex = s * mix(texture(waterTex, texCoords), white, waveHeight) + (1 - s) * groundColor;

     gl_FragColor = ka * tex + diffuse(kd, L, N, tex);
     if (normalizedDepth > 0) {
          gl_FragColor += specular(ks, L, N, white);
     }
     
}
