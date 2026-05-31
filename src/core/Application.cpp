#include "core/Application.hpp"

#include <GL/freeglut.h>

Application *Application::instance = nullptr;

Application::Application(int &argc, char **argv)
    : window(argc, argv, 1024, 768, "Projet POGL")
{
    instance = this;

    renderer.init();

    shader = std::make_unique<Shader>("shaders/basic.vert", "shaders/basic.frag");
    triangle = std::make_unique<Triangle>();

    glutDisplayFunc(Application::displayCallback);
    glutIdleFunc(Application::idleCallback);
    glutKeyboardFunc(Application::keyboardCallback);
}

void Application::run()
{
    window.startMainLoop();
}

void Application::render()
{
    renderer.clear();

    shader->use();
    triangle->draw();

    window.swapBuffers();
}

void Application::displayCallback()
{
    if (instance)
    {
        instance->render();
    }
}

void Application::idleCallback()
{
    glutPostRedisplay();
}

void Application::keyboardCallback(unsigned char key, int, int)
{
    if (key == 27)
    {
        glutLeaveMainLoop();
    }
}