#version 400

layout(vertices = 4) out;
uniform vec4 eyePos;
uniform int zNear;
uniform int zFar;

float map(float value, float min1, float max1, float min2, float max2) {
     return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main() {
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

  int maxTessellation = 64;
  int minTessellation = 2;
  int distMax = 90;
  int distMin = 15;

  vec4 centre = 0.25 * (
    gl_in[0].gl_Position + 
    gl_in[1].gl_Position + 
    gl_in[2].gl_Position + 
    gl_in[3].gl_Position
  );

  float dist = distance(eyePos, centre);
  int innerLevel = int(map(dist, distMin, distMax, maxTessellation, minTessellation));
  float distEdge1 = distance(eyePos, 0.5 * (gl_in[0].gl_Position + gl_in[3].gl_Position));
  float distEdge2 = distance(eyePos, 0.5 * (gl_in[0].gl_Position + gl_in[1].gl_Position));
  float distEdge3 = distance(eyePos, 0.5 * (gl_in[1].gl_Position + gl_in[2].gl_Position));
  float distEdge4 = distance(eyePos, 0.5 * (gl_in[2].gl_Position + gl_in[3].gl_Position));

  gl_TessLevelOuter[0] = int(map(distEdge1, distMin, distMax, maxTessellation, minTessellation));
  gl_TessLevelOuter[1] = int(map(distEdge2, distMin, distMax, maxTessellation, minTessellation));
  gl_TessLevelOuter[2] = int(map(distEdge3, distMin, distMax, maxTessellation, minTessellation));
  gl_TessLevelOuter[3] = int(map(distEdge4, distMin, distMax, maxTessellation, minTessellation));
  gl_TessLevelInner[0] = innerLevel;
  gl_TessLevelInner[1] = innerLevel;
}