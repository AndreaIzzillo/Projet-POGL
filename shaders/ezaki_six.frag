#version 430

in vec3 vLocalPosition;
in vec3 vViewPosition;
in vec3 vWorldPosition;
in vec3 vViewNormal;
in vec3 vWorldNormal;
in vec3 vLocalCenter;
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

void main() {
    vec3 color = vec3(0.4, 0.5, 0.2);
    vec3 noiseTextureColor = vec3(1.0);
    vec3 bandColor = vec3(0.8, 0.7, 0.3);

    vec3 viewSunPosition = (uView * vec4(uSunPosition, 1.0)).xyz;
    vec3 lightDirection = normalize(vViewCenter - viewSunPosition);
    float lightContribution = max(dot(vViewNormal, -lightDirection), 0.0);

    vec3 viewDirection = normalize(-vViewPosition);

    float facing = clamp(dot(vViewNormal, viewDirection), 0.0, 1.0);
    float rim = 1.0 - facing;

    float alpha = 1.0;

    float height = abs(dot(vec3(0.0, 1.0, 0.0), normalize(vLocalPosition - vLocalCenter)));

    // Noise texture effect
    float noiseTextureEffect = fbm(vLocalPosition * 5.0 + vec3(-uTime * 0.5, uTime * 0.4, uTime * 0.3));
    color = mix(color, noiseTextureColor * 0.7, noiseTextureEffect);

    // Large bands
    float bandNoise = fbm(vLocalPosition * 5.0 + vec3(uTime * 0.2, -uTime * 0.1, uTime * 0.3));
    float bandHeight = height + fbm(vLocalPosition * 10.0 + vec3(uTime * 0.3, uTime * 0.2, -uTime * 0.1)) * 0.2;
    float bandIntensity = smoothstep(0.0, 1.0, sin(bandHeight * 15.0)) * bandNoise;
    color = mix(color, bandColor, bandIntensity * 0.4);

    // Sun contribution
    color *= mix(0.1, 1.0, pow(lightContribution, 1.5));

    // Outer smoke effect (alpha)
    float smokeMask = smoothstep(0.4, 1.0, rim);
    alpha = mix(1.0, 0.0, smokeMask);

    FragColor = vec4(color, alpha);
}
