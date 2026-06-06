#version 430

in vec3 vLocalPosition;
in vec3 vViewPosition;
in vec3 vWorldPosition;
in vec3 vViewNormal;
in vec3 vWorldNormal;
in vec3 vLocalCenter;
in vec3 vViewCenter;

uniform float uTime;
uniform float uSupernovaTime;

out vec4 FragColor;

void main() {
    vec3 viewDirection = normalize(-vViewPosition);

    float facing = max(dot(viewDirection, vViewNormal), 0.0);
    float rim = 1.0 - facing;

    vec3 color = vec3(0.0, 0.6, 1.0);
    float alpha = 1.0;

    // Center glow
    float centerGlow = smoothstep(0.95, 1.0, facing);
    color = mix(color, vec3(1.0, 1.0, 1.0), centerGlow);

    // Outer glow
    float outerGlow = smoothstep(0.0, 0.5, rim);
    color = mix(color, vec3(0.2, 0.9, 1.0), outerGlow);

    // Outer alpha
    alpha = mix(alpha, 0.0, smoothstep(0.5, 1.0, rim));

    FragColor = vec4(color, alpha);
}