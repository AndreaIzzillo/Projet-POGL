#version 430

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;
uniform float uSupernovaTime;

out vec3 vLocalPosition;
out vec3 vViewPosition;
out vec3 vWorldPosition;
out vec3 vViewNormal;
out vec3 vWorldNormal;
out vec3 vLocalCenter;
out vec3 vViewCenter;

void main() {
    mat3 normalMatrix = transpose(inverse(mat3(uModel)));
    mat3 viewNormalMatrix = transpose(inverse(mat3(uView * uModel)));

    vec3 position = aPosition;
    vec3 normal = aNormal;

    float timeOffset = 3.5;
    float explosionTime = uSupernovaTime - timeOffset;

    if (explosionTime > 0.0) {
        float expansionFactor = explosionTime * 150.0;
        position += normal * expansionFactor;
    }

    vLocalPosition = position;
    vViewPosition = (uView * uModel * vec4(position, 1.0)).xyz;
    vWorldPosition = (uModel * vec4(position, 1.0)).xyz;
    vViewNormal = normalize(viewNormalMatrix * -normal);
    vWorldNormal = normalize(normalMatrix * -normal);
    vLocalCenter = uModel[3].xyz;
    vViewCenter = (uView * uModel[3]).xyz;

    gl_Position = uProjection * uView * uModel * vec4(position, 1.0);
}
