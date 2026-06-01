#pragma once

#include <GL/glew.h>
#include <vector>

#include "graphics/Vertex.hpp"

class Mesh
{
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
    ~Mesh();

    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    Mesh(Mesh &&) = delete;
    Mesh &operator=(Mesh &&) = delete;

    void draw() const;

private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    GLsizei indexCount = 0;
};