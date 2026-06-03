#pragma once

#include <GL/glew.h>
#include <array>
#include <memory>
#include <string>

class Camera;
class Mesh;
class Shader;

class Skybox
{
public:
    Skybox(const std::array<std::string, 6> &faces);
    ~Skybox();

    void draw(const Camera &camera) const;

private:
    GLuint textureID = 0;
    std::unique_ptr<Mesh> cube;
    std::unique_ptr<Shader> shader;

    GLuint loadCubemap(const std::array<std::string, 6> &faces);
};
