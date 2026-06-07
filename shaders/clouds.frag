#version 430

in vec3 vLocalNormal;
in vec3 vViewPosition;
in vec3 vViewNormal;

uniform mat4 uView;
uniform vec3 uSunPosition;
uniform float uTime;

out vec4 FragColor;

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

float fbm(vec3 p) {
    float sum = 0.0;
    float amplitude = 0.5;
    for (int i = 0; i < 5; ++i) {
        sum += amplitude * valueNoise(p);
        p *= 2.0;
        amplitude *= 0.5;
    }
    return sum;
}

void main() {
    vec3 surface = normalize(vLocalNormal);

    float coverage = fbm(surface * 2.6 + vec3(0.0, uTime * 0.01, 0.0));
    float density = smoothstep(0.52, 0.70, coverage);

    if (density < 0.01) {
        discard;
    }

    vec3 normal = normalize(vViewNormal);
    vec3 sunViewPosition = (uView * vec4(uSunPosition, 1.0)).xyz;
    vec3 lightDirection = normalize(sunViewPosition - vViewPosition);

    float ndl = dot(normal, lightDirection);
    float light = clamp(ndl * 0.5 + 0.5, 0.0, 1.0);
    light *= light;
    vec3 color = mix(vec3(0.45, 0.48, 0.55), vec3(1.0), light);

    FragColor = vec4(color, density * (0.25 + 0.75 * light));
}
