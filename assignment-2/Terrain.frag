#version 330

in float worldHeight;
in vec2 texCoords;
in vec3 worldPosition;
in vec3 N;

uniform vec4 eyePos;
uniform float waterHeight;
uniform sampler2D grassTex;
uniform sampler2D waterTex;

void main() 
{
     vec4 white = vec4(1.0);
     float ka = 0.3;
     float kd = 0.7;
     float ks = 0.5;
     vec3 lightPos = vec3(0, 20, 0);
     vec3 L = normalize(lightPos - worldPosition); 
     //vec3 N = normalize(cross(dFdx(worldPosition), dFdy(worldPosition)));
     vec3 V = normalize(vec3(eyePos) - worldPosition);
     vec3 H = normalize(L + V);

     if (worldPosition.y <= waterHeight + 0.05) {
          gl_FragColor = texture(waterTex, texCoords);
          gl_FragColor = ka * gl_FragColor + kd * max(dot(L, N), 0) * gl_FragColor + ks * pow(max(dot(H, N), 0), 10) * white;
     } else {
          gl_FragColor = texture(grassTex, texCoords);
          gl_FragColor = ka * gl_FragColor + (1 - ka) * max(dot(L, N), 0) * gl_FragColor;
     }
     
     
}
