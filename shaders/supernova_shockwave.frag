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

    vec3 color = vec3(0.6, 1.0, 1.0);
    float alpha = 1.0;

    // Shockwave effect
    alpha *= pow(rim, 3.0) * 0.5;

    FragColor = vec4(color, alpha);
}
