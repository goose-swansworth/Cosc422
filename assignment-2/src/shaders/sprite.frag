#version 330

uniform sampler2D tree1;
uniform vec3 lightPos;
out vec4 color;


void main() {
    float aspect = 444.0 / 1024;
    float d1 = 0.5 * (1 - aspect);
    float d2 = 0.5 * (1 + aspect);
    if (gl_PointCoord.x < d1 || gl_PointCoord.x > d2) {
        discard;
    }
    vec2 texCoords = vec2(
        (gl_PointCoord.x - d1) / (d2 - d1),
        gl_PointCoord.y
    );

    //vec3 L = normalize()

    color = texture(tree1, texCoords);
    if (color.a < 0.1) {
        discard;
    }
}
	