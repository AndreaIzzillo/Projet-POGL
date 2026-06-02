#pragma once

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <string>

class Shader
{
public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader();

    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    void use() const;
    GLuint getId() const;

    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &matrix) const;

private:
    GLuint programId = 0;

    static std::string readFile(const std::string &path);
    static GLuint compileShader(GLenum type, const std::string &source,
                                const std::string &debugName);
};
