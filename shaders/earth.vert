#version 430

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vLocalPosition;
out vec3 vLocalNormal;
out vec3 vViewPosition;
out vec3 vViewNormal;
out vec3 vColor;

void main() {
    vec4 viewPosition = uView * uModel * vec4(aPosition, 1.0);
    mat3 viewNormalMatrix = transpose(inverse(mat3(uView * uModel)));

    vLocalPosition = aPosition;
    vLocalNormal = normalize(aNormal);
    vViewPosition = viewPosition.xyz;
    vViewNormal = normalize(viewNormalMatrix * aNormal);
    vColor = aColor;

    gl_Position = uProjection * viewPosition;
}
