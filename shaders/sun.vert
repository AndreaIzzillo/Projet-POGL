#version 430

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;
uniform float uSupernovaTime;

out vec3 vLocalPosition; // Position in local space
out vec3 vLocalNormal;   // Normal in local space
out vec3 vViewPosition;  // Position in view space
out vec3 vViewNormal;    // Normal in view space
out vec3 vColor;         // Color from vertex attributes

void main() {
    vec4 worldPosition = uModel * vec4(aPosition, 1.0);
    vec4 viewPosition = uView * worldPosition;
    mat3 viewNormalMatrix = transpose(inverse(mat3(uView * uModel)));

    vLocalPosition = aPosition;
    vLocalNormal = normalize(aNormal);
    vViewPosition = viewPosition.xyz;
    vViewNormal = normalize(viewNormalMatrix * aNormal);
    vColor = aColor;

    vec3 supernovaPosition = aPosition;
    float minRadius = 0.05;
    float contraction = pow(uSupernovaTime, 2.0) * 0.08;
    float maxContraction = max(length(aPosition) - minRadius, 0.0);
    supernovaPosition -= normalize(aNormal) * min(contraction, maxContraction);

    gl_Position = uProjection * uView * uModel * vec4(supernovaPosition, 1.0);
}
