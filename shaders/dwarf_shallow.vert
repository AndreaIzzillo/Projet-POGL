#version 430

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;

out vec3 vLocalPosition;
out vec3 vViewPosition;
out vec3 vWorldPosition;
out vec3 vViewNormal;
out vec3 vWorldNormal;
out vec3 vViewCenter;

float random(vec3 p) {
    p = fract(p * vec3(0.1031, 0.1030, 0.0973));
    p += dot(p, p.yxz + 33.33);
    return fract((p.x + p.y) * p.z);
}

float valueNoise(vec3 p) {
    vec3 i = floor(p);
    vec3 f = fract(p);
    f = smoothstep(0.0, 1.0, f);

    float n000 = random(i + vec3(0.0, 0.0, 0.0));
    float n100 = random(i + vec3(1.0, 0.0, 0.0));
    float n010 = random(i + vec3(0.0, 1.0, 0.0));
    float n110 = random(i + vec3(1.0, 1.0, 0.0));
    float n001 = random(i + vec3(0.0, 0.0, 1.0));
    float n101 = random(i + vec3(1.0, 0.0, 1.0));
    float n011 = random(i + vec3(0.0, 1.0, 1.0));
    float n111 = random(i + vec3(1.0, 1.0, 1.0));

    float nx00 = mix(n000, n100, f.x);
    float nx10 = mix(n010, n110, f.x);
    float nx01 = mix(n001, n101, f.x);
    float nx11 = mix(n011, n111, f.x);
    float nxy0 = mix(nx00, nx10, f.y);
    float nxy1 = mix(nx01, nx11, f.y);

    return mix(nxy0, nxy1, f.z);
}

#define OCTAVES 8
float fbm(vec3 p) {
    float sum = 0.0;
    float amplitude = 0.5;
    for (int i = 0; i < OCTAVES; i++) {
        sum += amplitude * valueNoise(p);
        p *= 2.0;
        amplitude *= 0.5;
    }
    return sum;
}

void main() {
    vec3 position = aPosition;
    vec3 normal = aNormal;

    vec3 seed = position + uTime * 0.03;
    const float waveIntensity = 20.0;
    const float waveHeight = 0.08;
    float noise = fbm(seed * waveIntensity);
    noise *= waveHeight;

    position += normal * noise;
    
    mat3 viewNormalMatrix = transpose(inverse(mat3(uView * uModel)));
    mat3 worldNormalMatrix = transpose(inverse(mat3(uModel)));
    vLocalPosition = position;
    vViewPosition = (uView * uModel * vec4(position, 1.0)).xyz;
    vWorldPosition = (uModel * vec4(position, 1.0)).xyz;
    vViewNormal = normalize(viewNormalMatrix * normal);
    vWorldNormal = normalize(worldNormalMatrix * normal);
    vViewCenter = (uView * uModel * vec4(0.0, 0.0, 0.0, 1.0)).xyz;

    gl_Position = uProjection * uView * uModel * vec4(position, 1.0);
}
