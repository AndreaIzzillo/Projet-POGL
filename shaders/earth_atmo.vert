#version 430

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;

out vec3 vViewNormal;    // Normal in view space
out vec3 vWorldNormal;   // Normal in world space
out vec3 vViewPosition;  // Position in view space
out vec3 vWorldPosition; // Position in world space
out vec3 vViewCenter;    // Center of the Earth in view space
out vec3 vWorldCenter;   // Center of the Earth in world space
out float vAtmoRadius;   // Radius of the Earth atmosphere in view space
out float vTime;         // Time for animation

void main() {
    mat3 viewNormalMatrix = transpose(inverse(mat3(uView * uModel)));
    mat3 worldNormalMatrix = transpose(inverse(mat3(uModel)));
    vec4 viewPosition = uView * uModel * vec4(aPosition, 1.0);
    vec4 viewCenter = uView * uModel * vec4(0.0, 0.0, 0.0, 1.0);
    vec4 worldPosition = uModel * vec4(aPosition, 1.0);

    vViewNormal = normalize(viewNormalMatrix * -aNormal);
    vWorldNormal = normalize(worldNormalMatrix * -aNormal);
    vViewPosition = viewPosition.xyz;
    vWorldPosition = worldPosition.xyz;
    vViewCenter = viewCenter.xyz;
    vWorldCenter = (uModel * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
    vAtmoRadius = length((uModel * vec4(1.0, 0.0, 0.0, 0.0)).xyz);
    vTime = uTime;

    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
