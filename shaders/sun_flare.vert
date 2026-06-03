#version 430

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;

out vec3 vViewNormal;
out vec3 vViewPosition;
out float vTime;

void main() {
    mat3 viewNormalMatrix = transpose(inverse(mat3(uView * uModel)));
    vec4 viewPosition = uView * uModel * vec4(aPosition, 1.0);

    vViewNormal = normalize(viewNormalMatrix * -aNormal);
    vViewPosition = viewPosition.xyz;
    vTime = uTime;

    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
