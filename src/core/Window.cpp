#include "core/Window.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdexcept>

Window::Window(int &argc, char **argv, int width, int height, const std::string &title)
    : width(width)
    , height(height)
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow(title.c_str());

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("Failed to initialize GLEW");
    }

    glGetError();

    glViewport(0, 0, width, height);
}

void Window::startMainLoop() const
{
    glutMainLoop();
}

void Window::swapBuffers() const
{
    glutSwapBuffers();
}

int Window::getWidth() const
{
    return width;
}

int Window::getHeight() const
{
    return height;
}

void Window::setSize(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
}
