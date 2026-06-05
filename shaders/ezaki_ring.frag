#version 430

in vec3 vLocalPosition;
in vec3 vViewPosition;
in vec3 vWorldPosition;
in vec3 vViewNormal;
in vec3 vWorldNormal;
in vec3 vLocalCenter;
in vec3 vViewCenter;
in float vRingRadius;

uniform mat4 uView;
uniform vec3 uSunPosition;
uniform float uTime;

out vec4 FragColor;

void main() {
    float innerRadius = vRingRadius * 0.5;

    float distanceFromCenter = length(vViewPosition - vViewCenter);
    if (distanceFromCenter < innerRadius) {
        discard;
    }

    vec3 normal = vViewNormal;
    float outerFactor = smoothstep(innerRadius, vRingRadius * 1.5f, distanceFromCenter);
    vec3 awayNormal = normalize(vViewPosition - vViewCenter);
    vec3 blendedNormal = mix(normal, awayNormal, outerFactor);

    vec3 viewSunPosition = (uView * vec4(uSunPosition, 1.0)).xyz;
    vec3 lightDirection = normalize(vViewCenter - viewSunPosition);
    float lightContribution = max(dot(blendedNormal, -lightDirection), 0.0);

    vec3 color = vec3(0.6, 0.8, 0.6);
    vec3 bandColor = color * 0.8;

    // Regular band
    float regularNoise = sin((distanceFromCenter) * 0.2);
    color = mix(color, bandColor, regularNoise);

    // Dark bands
    float darkBandFactorA = 0.9;
    float darkBandFactorB = 0.7;
    if (distanceFromCenter > vRingRadius * darkBandFactorA && distanceFromCenter < vRingRadius * (darkBandFactorA + 0.008)) {
        color *= 0.3;
    }
    if (distanceFromCenter > vRingRadius * darkBandFactorB && distanceFromCenter < vRingRadius * (darkBandFactorB + 0.008)) {
        color *= 0.3;
    }

    // Outer effect
    float outerEffect = smoothstep(vRingRadius * 0.8, vRingRadius, distanceFromCenter);
    color *= (1.0 - mix(0.0, 0.8, outerEffect));

    // Inner effect
    float innerEffect = smoothstep(innerRadius, innerRadius * 1.2, distanceFromCenter);
    color = mix(color, bandColor, 1.0 - innerEffect);

    // Sun contribution
    color *= mix(0.03, 1.0, lightContribution);


    FragColor = vec4(color, 1.0);
}
