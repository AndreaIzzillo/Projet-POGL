#include "Skybox.hpp"

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "graphics/Mesh.hpp"
#include "graphics/MeshFactory.hpp"
#include "graphics/Shader.hpp"
#include "scene/Camera.hpp"

Skybox::Skybox(const std::array<std::string, 6> &faces)
    : textureID(loadCubemap(faces))
    , cube(MeshFactory::createCube())
    , shader(std::make_unique<Shader>("shaders/skybox.vert", "shaders/skybox.frag"))
{}

Skybox::~Skybox()
{
    glDeleteTextures(1, &textureID);
}

void Skybox::draw(const Camera &camera) const
{
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);

    shader->use();

    glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
    shader->setMat4("uView", view);
    shader->setMat4("uProjection", camera.getProjectionMatrix());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glUniform1i(glGetUniformLocation(shader->getId(), "uSkybox"), 0);
    cube->draw();

    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
}

GLuint Skybox::loadCubemap(const std::array<std::string, 6> &faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            const GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format,
                         GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
