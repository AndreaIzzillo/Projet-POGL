#include "core/Application.hpp"

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define ESC 27

Application *Application::instance = nullptr;

Application::Application(int &argc, char **argv)
    : window(argc, argv, 1600 * 2, 1000 * 2, "Projet POGL")
    , camera(static_cast<float>(window.getWidth()) / static_cast<float>(window.getHeight()))
{
    instance = this;

    renderer.init();

    // Skybox
    skybox = std::make_unique<Skybox>(std::array<std::string, 6>{
        "assets/skybox/right.png", "assets/skybox/left.png", "assets/skybox/top.png",
        "assets/skybox/bottom.png", "assets/skybox/front.png", "assets/skybox/back.png" });

    // The Sun
    sunShader = std::make_unique<Shader>("shaders/sun.vert", "shaders/sun.frag");
    Transform sunTransform;
    sunTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    sunTransform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    sunTransform.scale = glm::vec3(200.0f);
    loadObjectFromMesh(MeshFactory::createSphere(4, glm::vec3(1.0f, 0.5f, 0.0f)), sunShader.get(),
                       sunTransform);

    sunFlareShader = std::make_unique<Shader>("shaders/sun_flare.vert", "shaders/sun_flare.frag");
    Transform sunFlareTransform = sunTransform;
    sunFlareTransform.scale = glm::vec3(350.0f);
    loadObjectFromMesh(MeshFactory::createSphere(4, glm::vec3(1.0f, 0.5f, 0.0f)),
                       sunFlareShader.get(), sunFlareTransform, true, true);

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
    if (keys['c'])
        camera.increaseSpeed(dt);
    if (keys['x'])
        camera.decreaseSpeed(dt);
}

void Application::render()
{
    renderer.clear();

    skybox->draw(camera);

    for (const RenderObject &object : objects)
    {
        object.draw(camera, elapsedTime);
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
    instance->elapsedTime += deltaTime;

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
                                     const Transform &transform, bool isTransparent,
                                     bool reverseCullFace)
{
    std::vector<std::unique_ptr<Mesh>> loadedMeshes = GltfLoader::load(path);

    for (auto &loadedMesh : loadedMeshes)
    {
        Mesh *mesh = addMesh(std::move(loadedMesh));

        RenderObject object(mesh, shader, isTransparent, reverseCullFace);
        object.transform = transform;

        objects.push_back(object);
    }
}

void Application::loadObjectFromMesh(std::unique_ptr<Mesh> mesh, Shader *shader,
                                     const Transform &transform, bool isTransparent,
                                     bool reverseCullFace)
{
    RenderObject object(addMesh(std::move(mesh)), shader, isTransparent, reverseCullFace);
    object.transform = transform;
    objects.push_back(object);
}
