#include "graphics/Shader.hpp"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    const std::string vertexSource = readFile(vertexPath);
    const std::string fragmentSource = readFile(fragmentPath);

    const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource, vertexPath);
    const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource, fragmentPath);

    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);

    GLint success = GL_FALSE;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);

    if (!success)
    {
        GLint logLength = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);

        std::string log(logLength, '\0');
        glGetProgramInfoLog(programId, logLength, nullptr, log.data());

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        throw std::runtime_error("Shader program link error:\n" + log);
    }

    glDetachShader(programId, vertexShader);
    glDetachShader(programId, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    if (programId != 0)
    {
        glDeleteProgram(programId);
    }
}

void Shader::use() const
{
    glUseProgram(programId);
}

GLuint Shader::getId() const
{
    return programId;
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    const GLint location = glGetUniformLocation(programId, name.c_str());

    if (location == -1)
    {
        return;
    }

    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string &name, const glm::mat4 &matrix) const
{
    const GLint location = glGetUniformLocation(programId, name.c_str());

    if (location == -1)
    {
        return;
    }

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string Shader::readFile(const std::string &path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Shader::compileShader(GLenum type, const std::string &source, const std::string &debugName)
{
    const GLuint shader = glCreateShader(type);

    const char *sourcePtr = source.c_str();
    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::string log(logLength, '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());

        glDeleteShader(shader);

        throw std::runtime_error("Shader compile error in " + debugName + ":\n" + log);
    }

    return shader;
}