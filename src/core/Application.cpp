#include "core/Application.hpp"

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Application *Application::instance = nullptr;

Application::Application(int &argc, char **argv)
    : window(argc, argv, 1024, 768, "Projet POGL")
    , camera(static_cast<float>(window.getWidth()) / static_cast<float>(window.getHeight()))
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
    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("uModel", model);
    shader->setMat4("uView", camera.getViewMatrix());
    shader->setMat4("uProjection", camera.getProjectionMatrix());

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
