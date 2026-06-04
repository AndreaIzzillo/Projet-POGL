#version 430

in vec3 vLocalNormal;
in vec3 vViewPosition;
in vec3 vViewNormal;

uniform mat4 uView;
uniform vec3 uSunPosition;

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
    vec3 surface = normalize(vLocalNormal);

    float elevation = fbm(surface * 1.8 + vec3(7.3, 2.1, 5.7)) * 1.05;
    float seaLevel = 0.51;

    vec3 color;
    bool isOcean = elevation < seaLevel;

    if (isOcean) {
        float depth = (seaLevel - elevation) / seaLevel;
        vec3 shallow = vec3(0.06, 0.32, 0.55);
        vec3 deep = vec3(0.02, 0.10, 0.32);
        color = mix(shallow, deep, smoothstep(0.0, 0.6, depth));
    } else {
        float height = (elevation - seaLevel) / (1.0 - seaLevel);
        vec3 sand = vec3(0.78, 0.72, 0.45);
        vec3 grass = vec3(0.20, 0.50, 0.18);
        vec3 forest = vec3(0.12, 0.34, 0.12);
        vec3 rock = vec3(0.40, 0.34, 0.28);
        vec3 snow = vec3(0.95, 0.96, 0.98);

        color = sand;
        color = mix(color, grass, smoothstep(0.03, 0.08, height));
        color = mix(color, forest, smoothstep(0.12, 0.22, height));
        color = mix(color, rock, smoothstep(0.30, 0.45, height));
        color = mix(color, snow, smoothstep(0.60, 0.75, height));
    }

    float latitude = abs(surface.y);
    float iceEdge = 0.74 + 0.10 * fbm(surface * 4.0);
    float ice = smoothstep(iceEdge - 0.03, iceEdge + 0.03, latitude);
    color = mix(color, vec3(0.92, 0.95, 0.99), ice);

    vec3 normal = normalize(vViewNormal);
    vec3 sunViewPosition = (uView * vec4(uSunPosition, 1.0)).xyz;
    vec3 lightDirection = normalize(sunViewPosition - vViewPosition);

    float ndl = dot(normal, lightDirection);
    float dayNight = clamp(ndl * 0.5 + 0.5, 0.0, 1.0);
    float light = mix(0.06, 1.0, dayNight * dayNight);
    color *= light;

    if (isOcean) {
        vec3 viewDirection = normalize(-vViewPosition);
        vec3 halfway = normalize(lightDirection + viewDirection);
        float specular = pow(max(dot(normal, halfway), 0.0), 64.0);
        color += specular * smoothstep(0.0, 0.2, ndl) * vec3(0.7, 0.8, 0.9) * 0.5;
    }

    FragColor = vec4(color, 1.0);
}
