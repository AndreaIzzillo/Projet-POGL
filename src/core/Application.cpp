#include "core/Application.hpp"

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define ESC 27

Application *Application::instance = nullptr;

Application::Application(int &argc, char **argv)
    : window(argc, argv, 1600, 1000, "Projet POGL")
    , camera(static_cast<float>(window.getWidth()) / static_cast<float>(window.getHeight()))
{
    instance = this;

    renderer.init();

    // Load Muna
    munaShader = std::make_unique<Shader>("shaders/basic.vert", "shaders/basic.frag");

    Transform munaTransform;
    munaTransform.position = glm::vec3(0.0f, 0.0f, -500.0f);
    munaTransform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    munaTransform.scale = glm::vec3(1.0f);

    loadObjectFromFile("assets/Muna.glb", munaShader.get(), munaTransform);

    previousTimeMs = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(Application::displayCallback);
    glutIdleFunc(Application::idleCallback);
    glutKeyboardFunc(Application::keyboardDownCallback);
    glutKeyboardUpFunc(Application::keyboardUpCallback);
    glutMouseFunc(Application::mouseButtonCallback);
    glutMotionFunc(Application::mouseMotionCallback);
}

void Application::run()
{
    window.startMainLoop();
}

void Application::update(float dt)
{
    if (keys[ESC])
        glutLeaveMainLoop();
    if (keys['w'])
        camera.moveForward(dt);
    if (keys['s'])
        camera.moveBackward(dt);
    if (keys['a'])
        camera.moveLeft(dt);
    if (keys['d'])
        camera.moveRight(dt);
    if (keys['q'])
        camera.moveUp(dt);
    if (keys['e'])
        camera.moveDown(dt);
    if (keys['j'])
        camera.rotateLeft(dt);
    if (keys['l'])
        camera.rotateRight(dt);
    if (keys['i'])
        camera.rotateUp(dt);
    if (keys['k'])
        camera.rotateDown(dt);
}

void Application::render()
{
    renderer.clear();

    for (const RenderObject &object : objects)
    {
        object.draw(camera);
    }

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
    if (!instance)
    {
        return;
    }

    const int currentTimeMs = glutGet(GLUT_ELAPSED_TIME);
    const float deltaTime = static_cast<float>(currentTimeMs - instance->previousTimeMs) / 1000.0f;
    instance->previousTimeMs = currentTimeMs;

    instance->update(deltaTime);

    glutPostRedisplay();
}

void Application::keyboardDownCallback(unsigned char key, int x, int y)
{
    if (instance)
    {
        instance->keys[key] = true;
    }
}

void Application::keyboardUpCallback(unsigned char key, int x, int y)
{
    if (instance)
    {
        instance->keys[key] = false;
    }
}

void Application::mouseButtonCallback(int button, int state, int x, int y)
{
    if (!instance)
        return;
    if (button == GLUT_LEFT_BUTTON)
    {
        instance->mouseCaptured = (state == GLUT_DOWN);
        instance->lastMouseX = x;
        instance->lastMouseY = y;
        glutSetCursor(instance->mouseCaptured ? GLUT_CURSOR_NONE : GLUT_CURSOR_INHERIT);
    }
}

void Application::mouseMotionCallback(int x, int y)
{
    if (!instance || !instance->mouseCaptured)
        return;

    const int dx = x - instance->lastMouseX;
    const int dy = y - instance->lastMouseY;
    instance->lastMouseX = x;
    instance->lastMouseY = y;

    instance->camera.rotateByMouse(static_cast<float>(dx), static_cast<float>(dy));
}

Mesh *Application::addMesh(std::unique_ptr<Mesh> mesh)
{
    meshes.push_back(std::move(mesh));
    return meshes.back().get();
}

void Application::loadObjectFromFile(const std::string &path, Shader *shader,
                                     const Transform &transform)
{
    std::vector<std::unique_ptr<Mesh>> loadedMeshes = GltfLoader::load(path);

    for (auto &loadedMesh : loadedMeshes)
    {
        Mesh *mesh = addMesh(std::move(loadedMesh));

        RenderObject object(mesh, shader);
        object.transform = transform;

        objects.push_back(object);
    }
}