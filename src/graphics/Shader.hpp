#pragma once

#include <GL/glew.h>
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

private:
    GLuint programId = 0;

    static std::string readFile(const std::string &path);
    static GLuint compileShader(GLenum type, const std::string &source,
                                const std::string &debugName);
};