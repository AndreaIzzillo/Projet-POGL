#version 430

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vLocalPosition;
out vec3 vViewPosition;
out vec3 vWorldPosition;
out vec3 vViewNormal;
out vec3 vWorldNormal;
out vec3 vLocalCenter;
out vec3 vViewCenter;

void main() {
    mat3 viewNormalMatrix = transpose(inverse(mat3(uView * uModel)));
    mat3 worldNormalMatrix = transpose(inverse(mat3(uModel)));

    vLocalPosition = aPosition;
    vViewPosition = vec3(uView * uModel * vec4(aPosition, 1.0));
    vWorldPosition = vec3(uModel * vec4(aPosition, 1.0));
    vViewNormal = normalize(viewNormalMatrix * aNormal);
    vWorldNormal = normalize(worldNormalMatrix * aNormal);
    vLocalCenter = vec3(0.0, 0.0, 0.0);
    vViewCenter = vec3(uView * uModel * vec4(0.0, 0.0, 0.0, 1.0));

    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
