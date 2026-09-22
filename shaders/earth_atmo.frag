#version 430

in vec3 vViewPosition;
in vec3 vWorldPosition;
in vec3 vViewNormal;
in vec3 vWorldNormal;
in vec3 vViewCenter;
in vec3 vWorldCenter;
in float vAtmoRadius;
in float vTime;

uniform vec3 uSunPosition;

out vec4 FragColor;

void main() {
    vec3 atmoColor = vec3(0.5, 0.8, 1.0);

    vec3 lightDir = normalize(vWorldCenter - uSunPosition);
    vec3 worldNormal = normalize(vWorldNormal);
    float dayFactor = dot(lightDir, worldNormal) + 0.75;
    dayFactor = clamp(dayFactor, 0.0, 1.0);

    vec3 viewNormal = normalize(vViewNormal);
    vec3 viewDirection = normalize(-vViewPosition);

    float facing = clamp(dot(viewNormal, viewDirection), 0.0, 1.0);
    float smoothFacing = pow(facing, 4.0);

    float cameraDistanceToCenter = length(vViewCenter);

    vec3 cameraToEarth = normalize(vViewCenter);
    vec3 cameraToFragment = normalize(vViewPosition);

    float earthSide = clamp(dot(cameraToEarth, cameraToFragment), 0.0, 1.0);
    smoothFacing *= earthSide;

    float transitionWidth = vAtmoRadius * 0.2;
    float insideAmount =
        1.0 - smoothstep(vAtmoRadius - transitionWidth, vAtmoRadius, cameraDistanceToCenter);

    float atmosphereAlpha = 0.9;
    float alpha = mix(smoothFacing, atmosphereAlpha, insideAmount);
    alpha = mix(0.0, alpha, dayFactor);

    FragColor = vec4(atmoColor, alpha);
}
