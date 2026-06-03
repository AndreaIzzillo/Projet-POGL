#version 430

in vec3 vViewPosition;
in vec3 vViewNormal;
in float vTime;

out vec4 FragColor;

void main() {
    vec3 flareColor = vec3(1.0, 0.4, 0.1);

    vec3 viewNormal = normalize(vViewNormal);
    vec3 viewDirection = normalize(-vViewPosition);

    float facing = clamp(dot(viewNormal, viewDirection), 0.0, 1.0);
    float rim = 1.0 - facing;

    float smoothFacing = pow(facing, 7.0);

    vec3 center = vec3(0.0, 0.0, 0.0);

    FragColor = vec4(flareColor, smoothFacing);
}