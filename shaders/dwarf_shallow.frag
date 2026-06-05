#version 430

in vec3 vLocalPosition;
in vec3 vViewPosition;
in vec3 vWorldPosition;
in vec3 vViewNormal;
in vec3 vWorldNormal;
in vec3 vViewCenter;

uniform mat4 uView;
uniform vec3 uSunPosition;
uniform float uTime;

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

vec3 waterNormal()
{
    vec3 n = normalize(vViewNormal);
    vec3 t = normalize(cross(abs(n.y) < 0.9 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0), n));
    vec3 b = normalize(cross(n, t));

    float scale = 30.0;
    float eps = 0.025;

    vec3 p = normalize(vLocalPosition);
    float h1 = fbm(p * scale + vec3(0.2, 0.6, -0.4) * uTime);
    float hx = fbm((p + t * eps) * scale + vec3(0.2, 0.6, -0.4) * uTime) - h1;
    float hy = fbm((p + b * eps) * scale + vec3(0.2, 0.6, -0.4) * uTime) - h1;

    float strength = 1.8;
    return normalize(n - t * hx * strength - b * hy * strength);
}

const float R = 102.0 / 255.0;
const float G = 81.0 / 255.0;
const float B = 161.0 / 255.0;

void main() {
    vec3 viewSunPosition = (uView * vec4(uSunPosition, 1.0)).xyz;
    vec3 lightDirection = normalize(vViewCenter - viewSunPosition);
    float lightContribution = max(dot(vViewNormal, -lightDirection), 0.0);

    vec3 viewDirection = normalize(-vViewPosition);
    vec3 waterNormal = waterNormal();

    float facing = clamp(dot(vViewNormal, viewDirection), 0.0, 1.0);
    float rim = 1.0 - facing;
    float waterFacing = clamp(dot(waterNormal, viewDirection), 0.0, 1.0);
    float waterRim = 1.0 - waterFacing;

    vec3 color = vec3(R, G, B);

    // Seabed effect
    color *= clamp(pow(rim, 0.6), 0.3, 1.0);

    // Shimmering effect
    waterRim = pow(waterRim, 5.0);
    float waveShimmer = mix(0.0, 0.5, waterRim);
    float waveLight = mix(0.4, 1.0, lightContribution);
    vec3 shimmerColor = vec3(1.0) * waveShimmer * waveLight;
    color += shimmerColor;

    // Pearly effect
    float pearlNoise = valueNoise(waterNormal * 6.0);
    float distance = length(vViewPosition);
    float pearlIntensity = pow(lightContribution, 4.0) * (1.0 - smoothstep(100.0, 300.0, distance));
    if (pearlNoise > 0.90)
        color += vec3(1.0) * pearlIntensity;
    
    // Sun contribution
    color *= mix(0.3, 1.0, lightContribution);

    float alpha = mix(0.85, 1.0, pow(facing, 2.0));
    FragColor = vec4(color, alpha);

}