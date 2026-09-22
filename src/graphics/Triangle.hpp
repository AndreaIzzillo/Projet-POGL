#pragma once

#include <GL/glew.h>

class Triangle
{
public:
    Triangle();
    ~Triangle();

    Triangle(const Triangle &) = delete;
    Triangle &operator=(const Triangle &) = delete;

    void draw() const;

private:
    GLuint vao = 0;
    GLuint vbo = 0;
};
