#version 430

in vec3 vColor;

uniform float uSupernovaTime;

out vec4 FragColor;

void main() {
    float alpha = 1.0 - smoothstep(4.0, 10.0, uSupernovaTime);

    FragColor = vec4(vColor, alpha);
}
