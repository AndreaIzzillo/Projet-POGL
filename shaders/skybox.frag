#version 430

in vec3 vTexCoords;

out vec4 FragColor;

uniform samplerCube uSkybox;

void main() {
    FragColor = texture(uSkybox, vTexCoords);
}
