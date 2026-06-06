#version 430

in vec3 vViewPosition;
in vec3 vViewNormal;
in vec3 vViewCenter;
in float vFlareRadius;

uniform float uTime;
uniform float uSupernovaTime;

out vec4 FragColor;

void main() {
    vec3 flareColor = vec3(1.0, 0.4, 0.1);

    vec3 viewNormal = normalize(vViewNormal);
    vec3 viewDirection = normalize(-vViewPosition);

    float facing = clamp(dot(viewNormal, viewDirection), 0.0, 1.0);
    float smoothFacing = pow(facing, 5.0);

    float cameraDistanceToCenter = length(vViewCenter);
    
    vec3 cameraToSun = normalize(vViewCenter);
    vec3 cameraToFragment = normalize(vViewPosition);

    float sunSide = clamp(dot(cameraToSun, cameraToFragment), 0.0, 1.0);
    smoothFacing *= sunSide;

    float transitionWidth = vFlareRadius * 0.2;
    float insideAmount =
        1.0 - smoothstep(vFlareRadius - transitionWidth, vFlareRadius, cameraDistanceToCenter);

    insideAmount = insideAmount - smoothstep(0.0, 2.0, uSupernovaTime);
    
    // debug
    // insideAmount = 0.0;
    
    float atmosphereAlpha = 0.2;
    float alpha = mix(smoothFacing, atmosphereAlpha, insideAmount);

    FragColor = vec4(flareColor, alpha);
}
