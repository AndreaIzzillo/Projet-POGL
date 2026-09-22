#include "graphics/Triangle.hpp"

Triangle::Triangle()
{
    const float vertices[] = { -0.6f, -0.5f, 0.0f, 1.0f, 0.2f, 0.2f, 0.6f, -0.5f, 0.0f,
                               0.2f,  1.0f,  0.2f, 0.0f, 0.6f, 0.0f, 0.2f, 0.4f,  1.0f };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          reinterpret_cast<void *>(3 * sizeof(float)));

    glBindVertexArray(0);
}

Triangle::~Triangle()
{
    if (vbo != 0)
    {
        glDeleteBuffers(1, &vbo);
    }

    if (vao != 0)
    {
        glDeleteVertexArrays(1, &vao);
    }
}

void Triangle::draw() const
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
