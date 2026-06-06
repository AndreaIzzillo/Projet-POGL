#version 430

in vec3 vLocalPosition;
in vec3 vLocalNormal;
in vec3 vViewPosition;
in vec3 vViewNormal;
in vec3 vColor;

uniform float uTime;
uniform float uSupernovaTime;

out vec4 FragColor;

float random(vec3 p) {
    return fract(sin(dot(p, vec3(127.1, 311.7, 74.7))) * 43758.5453);
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

float toonBand(float value, float steps) {
    return floor(clamp(value, 0.0, 1.0) * steps) / (steps - 1.0);
}

void main() {
    vec3 localNormal = normalize(vLocalNormal);
    vec3 viewNormal = normalize(vViewNormal);
    vec3 viewDirection = normalize(-vViewPosition);

    float facing = clamp(dot(viewNormal, viewDirection), 0.0, 1.0);
    float rim = 1.0 - facing;

    vec3 animatedPosition = localNormal * 3 + vec3(uTime * 0.4, 0.0, 0.0) * clamp(pow(uSupernovaTime, 2.0) * 0.5, 0.0, 5.0);
    float heat = valueNoise(animatedPosition);

    float hotPatch = smoothstep(0.61, 0.64, heat);
    float darkPatch = smoothstep(0.34, 0.37, heat) * (1.0 - smoothstep(0.50, 0.53, heat));

    float centerLight = toonBand(facing * 0.8 + heat * 0.2, 4.0);

    float redShift = min(pow(uSupernovaTime, 2.0) * 0.2, 1.0);
    vec3 shadowOrange = vec3(0.92 + redShift, 0.25 - redShift, 0.02 - redShift);
    vec3 baseOrange = vColor;
    vec3 brightYellow = vec3(1.0, 0.86 - redShift, 0.16 - redShift);
    vec3 hotYellow = vec3(1.0, 0.98 - redShift, 0.52 - redShift);
    vec3 rimRed = vec3(0.8 + redShift, 0.08 - redShift, 0.00);

    vec3 color = mix(shadowOrange, brightYellow, centerLight);
    color = mix(color, baseOrange, darkPatch * 0.5);
    color = mix(color, hotYellow, hotPatch * 0.5);
    color = mix(color, rimRed, smoothstep(0.4, 0.9, rim));

    FragColor = vec4(color, 1.0);
}
