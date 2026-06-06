#version 430

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;
uniform float uSupernovaTime;

out vec3 vViewNormal;   // Normal in view space
out vec3 vViewPosition; // Position in view space
out vec3 vViewCenter;   // Center of the sun in view space
out float vFlareRadius; // Radius of the sun flare in view space

void main() {
    mat3 viewNormalMatrix = transpose(inverse(mat3(uView * uModel)));
    vec4 viewPosition = uView * uModel * vec4(aPosition, 1.0);
    vec4 viewCenter = uView * uModel * vec4(0.0, 0.0, 0.0, 1.0);

    vViewNormal = normalize(viewNormalMatrix * -aNormal);
    vViewPosition = viewPosition.xyz;
    vViewCenter = viewCenter.xyz;
    vFlareRadius = length((uModel * vec4(1.0, 0.0, 0.0, 0.0)).xyz);

    vec3 supernovaPosition = aPosition;
    float minRadius = 0.05;
    float contraction = pow(uSupernovaTime, 2.0) * 0.08;
    float maxContraction = max(length(aPosition) - minRadius, 0.0);
    supernovaPosition -= normalize(aNormal) * min(contraction, maxContraction);

    gl_Position = uProjection * uView * uModel * vec4(supernovaPosition, 1.0);
}
