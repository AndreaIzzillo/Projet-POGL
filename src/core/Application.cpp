#include "core/Application.hpp"

#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>
#include <vector>

#define RED glm::vec3(1.0f, 0.0f, 0.0f)
#define GREEN glm::vec3(0.0f, 1.0f, 0.0f)
#define BLUE glm::vec3(0.0f, 0.0f, 1.0f)
#define CYAN glm::vec3(0.0f, 1.0f, 1.0f)
#define MAGENTA glm::vec3(1.0f, 0.0f, 1.0f)
#define YELLOW glm::vec3(1.0f, 1.0f, 0.0f)
#define WHITE glm::vec3(1.0f, 1.0f, 1.0f)
#define BLACK glm::vec3(0.0f, 0.0f, 0.0f)

#define ESC 27

constexpr float earthAxialTilt = 0.41f; // 23.5 degrés
constexpr float earthRadius = 40.0f;
constexpr float earthOrbitRadius = 700.0f;
constexpr float earthOrbitSpeed = 0.08f;
constexpr float earthSpinSpeed = 0.30f;

constexpr float cloudRadius = 47.5f;
constexpr float cloudSpinSpeed = 0.36f;

constexpr float moonScale = 0.1f;
constexpr float moonOrbitRadius = 125.0f;
constexpr float moonOrbitSpeed = 0.50f;

constexpr float dwarfShallowScale = 80.f;
constexpr float dwarfShallowOrbitRadius = 1200.0f;
constexpr float dwarfShallowOrbitSpeed = 0.1f;
constexpr float dwarfShallowSpinSpeed = 0.10f;

constexpr float ezakiSixAxialTilt = 0.1f;
constexpr float ezakiSixScale = 100.f;
constexpr float ezakiSixOrbitRadius = 1900.0f;
constexpr float ezakiSixOrbitSpeed = 0.15f;
constexpr float ezakiSixSpinSpeed = 0.05f;

constexpr float blackHoleEventHorizon = 100.0f;
constexpr float blackHoleInfluence = 200.0f;
constexpr float blackHoleOrbitRadius = 2500.0f;
constexpr float blackHoleOrbitSpeed = -0.1f;
constexpr float blackHoleOrbitPhase = 3.6f;

Application *Application::instance = nullptr;

Application::Application(int &argc, char **argv)
    : window(argc, argv, 1600, 900, "Projet POGL")
    , camera(static_cast<float>(window.getWidth()) / static_cast<float>(window.getHeight()))
{
    instance = this;
    renderer.init();
    renderer.initFramebuffer(window.getWidth(), window.getHeight());

    loadScene();

    previousTime = std::chrono::steady_clock::now();
    glutDisplayFunc(Application::displayCallback);
    glutIdleFunc(Application::idleCallback);
    glutKeyboardFunc(Application::keyboardDownCallback);
    glutKeyboardUpFunc(Application::keyboardUpCallback);
    glutMouseFunc(Application::mouseButtonCallback);
    glutMotionFunc(Application::mouseMotionCallback);
}

void Application::loadScene()
{
    RenderObject::StateFunc resetState = []() {
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    };

    RenderObject::StateFunc blendState = []() { glEnable(GL_BLEND); };

    RenderObject::StateFunc atmoState = []() {
        glCullFace(GL_FRONT);
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
    };

    RenderObject::StateFunc cloudsState = []() {
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
    };

    RenderObject::StateFunc dotState = []() {
        glEnable(GL_BLEND);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    };

    // Skybox
    skybox = std::make_unique<Skybox>(std::array<std::string, 6>{
        "assets/skybox/right.png", "assets/skybox/left.png", "assets/skybox/top.png",
        "assets/skybox/bottom.png", "assets/skybox/front.png", "assets/skybox/back.png" });

    // The Sun
    sunShader = std::make_unique<Shader>("shaders/sun.vert", "shaders/sun.frag");

    Transform sunTransform;
    sunTransform.scale = glm::vec3(200.0f);

    loadObjectFromMesh(MeshFactory::createSphere(4, glm::vec3(1.0f, 0.5f, 0.0f)), sunShader.get(),
                       sunTransform, false, resetState, resetState);

    RenderObject::setSunPosition(sunTransform.position);

    // The Sun Flare
    sunFlareShader = std::make_unique<Shader>("shaders/sun_flare.vert", "shaders/sun_flare.frag");

    Transform sunFlareTransform = sunTransform;
    sunFlareTransform.scale = glm::vec3(350.0f);

    loadObjectFromMesh(MeshFactory::createSphere(4, glm::vec3(1.0f, 0.5f, 0.0f)),
                       sunFlareShader.get(), sunFlareTransform, true, atmoState, resetState);

    // The Earth
    earthShader = std::make_unique<Shader>("shaders/earth.vert", "shaders/earth.frag");
    earthIndex = objects.size();

    Transform earthTransform;
    earthTransform.scale = glm::vec3(earthRadius);

    loadObjectFromMesh(MeshFactory::createSphere(4), earthShader.get(), earthTransform, false,
                       resetState, resetState);

    // The clouds
    cloudsShader = std::make_unique<Shader>("shaders/earth.vert", "shaders/clouds.frag");
    cloudsIndex = objects.size();

    Transform cloudsTransform;
    cloudsTransform.scale = glm::vec3(cloudRadius);

    loadObjectFromMesh(MeshFactory::createSphere(4), cloudsShader.get(), cloudsTransform, true,
                       cloudsState, resetState);

    // The Earth's atmosphere
    earthAtmoShader =
        std::make_unique<Shader>("shaders/earth_atmo.vert", "shaders/earth_atmo.frag");
    earthAtmoIndex = objects.size();

    Transform earthAtmoTransform = earthTransform;
    earthAtmoTransform.scale = glm::vec3(earthRadius * 1.8f);

    loadObjectFromMesh(MeshFactory::createSphere(4, glm::vec3(1.0f, 0.5f, 0.0f)),
                       earthAtmoShader.get(), earthAtmoTransform, true, atmoState, resetState);

    // The Moon
    munaShader = std::make_unique<Shader>("shaders/muna.vert", "shaders/muna.frag");
    moonIndex = objects.size();

    Transform moonTransform;
    moonTransform.scale = glm::vec3(moonScale);

    loadObjectFromFile("assets/muna.glb", munaShader.get(), moonTransform, false, resetState,
                       resetState);

    // Dwarf's Shallow
    dwarfShallowShader =
        std::make_unique<Shader>("shaders/dwarf_shallow.vert", "shaders/dwarf_shallow.frag");
    dwarfShallowIndex = objects.size();

    Transform dwarfShallowTransform;
    dwarfShallowTransform.scale = glm::vec3(dwarfShallowScale);

    loadObjectFromMesh(MeshFactory::createSphere(6), dwarfShallowShader.get(),
                       dwarfShallowTransform, true, blendState, resetState);
    // Ezaki Ring
    ezakiRingShader =
        std::make_unique<Shader>("shaders/ezaki_ring.vert", "shaders/ezaki_ring.frag");
    ezakiRingIndex = objects.size();

    Transform ezakiRingTransform;
    ezakiRingTransform.scale = glm::vec3(ezakiSixScale * 2.5f);
    ezakiRingTransform.rotation = glm::vec3(glm::half_pi<float>() - 0.2f, 0.0f, 0.0f);

    loadObjectFromMesh(MeshFactory::createDisk(64), ezakiRingShader.get(), ezakiRingTransform, true,
                       blendState, resetState);

    // Ezaki Six
    ezakiSixShader = std::make_unique<Shader>("shaders/ezaki_six.vert", "shaders/ezaki_six.frag");
    ezakiSixIndex = objects.size();

    Transform ezakiSixTransform;
    ezakiSixTransform.scale = glm::vec3(ezakiSixScale);

    loadObjectFromMesh(MeshFactory::createSphere(4), ezakiSixShader.get(), ezakiSixTransform, true,
                       blendState, resetState);

    // Supernova dots
    supernovaDotsShader =
        std::make_unique<Shader>("shaders/supernova_dots.vert", "shaders/supernova_dots.frag");

    Transform supernovaDotsTransform;
    supernovaDotsTransform.scale = glm::vec3(10.0f);

    loadObjectFromMesh(MeshFactory::createSphere(4, BLUE), supernovaDotsShader.get(),
                       supernovaDotsTransform, true, dotState, resetState);

    // Supernova shockwave
    supernovaShockwaveShader = std::make_unique<Shader>("shaders/supernova_shockwave.vert",
                                                        "shaders/supernova_shockwave.frag");

    Transform supernovaShockwaveTransform;
    supernovaShockwaveTransform.scale = glm::vec3(10.0f);

    loadObjectFromMesh(MeshFactory::createSphere(4, BLUE), supernovaShockwaveShader.get(),
                       supernovaShockwaveTransform, true, atmoState, resetState);

    // Supernova
    supernovaShader = std::make_unique<Shader>("shaders/supernova.vert", "shaders/supernova.frag");

    Transform supernovaTransform;
    supernovaTransform.scale = glm::vec3(10.0f);

    loadObjectFromMesh(MeshFactory::createSphere(6, BLUE), supernovaShader.get(),
                       supernovaTransform, true, blendState, resetState);

    // The Black Hole. It is not in `objects`, so give it a sentinel index past the last
    // object (no real object uses it) to drive the camera-follow logic.
    blackHoleIndex = objects.size();
    const glm::vec3 blackHoleStart = blackHoleOrbitRadius
        * glm::vec3(std::cos(blackHoleOrbitPhase), 0.0f, std::sin(blackHoleOrbitPhase));
    blackHole =
        std::make_unique<BlackHole>(blackHoleStart, blackHoleEventHorizon, blackHoleInfluence);
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
    if (keys['p'])
        supernovaActive = true;
    if (keys['0'])
        cameraAttachedTo = -1;
    if (keys['1'])
        cameraAttachedTo = earthIndex;
    if (keys['2'])
        cameraAttachedTo = dwarfShallowIndex;
    if (keys['3'])
        cameraAttachedTo = ezakiSixIndex;
    if (keys['4'])
        cameraAttachedTo = blackHoleIndex;

    const float time = elapsedTime;

    // The Earth
    const float earthOrbit = time * earthOrbitSpeed;
    const glm::vec3 earthPosition =
        earthOrbitRadius * glm::vec3(std::cos(earthOrbit), 0.0f, std::sin(earthOrbit));
    Transform &earth = objects[earthIndex].transform;
    const glm::vec3 earthOffset = earthPosition - earth.position;
    earth.position = earthPosition;
    earth.rotation = glm::vec3(earthAxialTilt, time * earthSpinSpeed, 0.0f);

    Transform &earthAtmo = objects[earthAtmoIndex].transform;
    earthAtmo.position = earthPosition;

    // The clouds
    Transform &clouds = objects[cloudsIndex].transform;
    clouds.position = earthPosition;
    clouds.rotation = glm::vec3(earthAxialTilt, time * cloudSpinSpeed, 0.0f);

    // The Moon
    const float moonOrbit = time * moonOrbitSpeed;
    const glm::vec3 moonOffset =
        moonOrbitRadius * glm::vec3(std::cos(moonOrbit), 0.0f, std::sin(moonOrbit));
    Transform &moon = objects[moonIndex].transform;
    moon.position = earthPosition + moonOffset;
    moon.rotation = glm::vec3(0.0f, -moonOrbit, 0.0f);

    // Dwarf's Shallow
    float offsetTime = time + 100.f;
    const float dwarfShallowOrbit = offsetTime * dwarfShallowOrbitSpeed;
    const glm::vec3 dwarfShallowPosition = dwarfShallowOrbitRadius
        * glm::vec3(std::cos(dwarfShallowOrbit), 0.0f, std::sin(dwarfShallowOrbit));
    Transform &dwarfShallow = objects[dwarfShallowIndex].transform;
    const glm::vec3 dwarfShallowOffset = dwarfShallowPosition - dwarfShallow.position;
    dwarfShallow.position = dwarfShallowPosition;
    dwarfShallow.rotation = glm::vec3(0.0f, offsetTime * dwarfShallowSpinSpeed, 0.0f);

    // Ezaki Six
    offsetTime = time + 500.f;
    const float ezakiSixOrbit = offsetTime * ezakiSixOrbitSpeed;
    const glm::vec3 ezakiSixPosition =
        ezakiSixOrbitRadius * glm::vec3(std::cos(ezakiSixOrbit), 0.0f, std::sin(ezakiSixOrbit));
    Transform &ezakiSix = objects[ezakiSixIndex].transform;
    const glm::vec3 ezakiSixOffset = ezakiSixPosition - ezakiSix.position;
    ezakiSix.position = ezakiSixPosition;
    ezakiSix.rotation = glm::vec3(ezakiSixAxialTilt, offsetTime * ezakiSixSpinSpeed, 0.0f);

    // Ezaki Ring
    Transform &ezakiRing = objects[ezakiRingIndex].transform;
    ezakiRing.position = ezakiSixPosition;
    // The Black Hole orbits the sun in the XZ plane like the planets.
    const float blackHoleOrbit = time * blackHoleOrbitSpeed + blackHoleOrbitPhase;
    const glm::vec3 blackHolePosition = blackHoleOrbitRadius
        * glm::vec3(std::cos(blackHoleOrbit), 0.0f, std::sin(blackHoleOrbit));
    const glm::vec3 blackHoleOffset = blackHolePosition - blackHole->getPosition();
    blackHole->setPosition(blackHolePosition);

    // Camera attachment
    if (cameraAttachedTo == earthIndex)
        camera.movePosition(earthOffset);
    else if (cameraAttachedTo == dwarfShallowIndex)
        camera.movePosition(dwarfShallowOffset);
    else if (cameraAttachedTo == ezakiSixIndex)
        camera.movePosition(ezakiSixOffset);
    else if (cameraAttachedTo == blackHoleIndex)
        camera.movePosition(blackHoleOffset);
}

void Application::render()
{
    renderer.beginSceneCapture();

    skybox->draw(camera);

    std::vector<RenderObject *> opaqueObjects;
    std::vector<RenderObject *> transparentObjects;

    for (RenderObject &object : objects)
    {
        if (object.isTransparentObject())
            transparentObjects.push_back(&object);
        else
            opaqueObjects.push_back(&object);
    }

    auto distanceToCamera = [this](const RenderObject &object) {
        return object.getDistanceToCamera(camera);
    };

    std::sort(opaqueObjects.begin(), opaqueObjects.end(),
              [&distanceToCamera](const RenderObject *a, const RenderObject *b) {
                  return distanceToCamera(*a) < distanceToCamera(*b);
              });

    std::sort(transparentObjects.begin(), transparentObjects.end(),
              [&distanceToCamera](const RenderObject *a, const RenderObject *b) {
                  return distanceToCamera(*a) > distanceToCamera(*b);
              });

    for (const RenderObject *object : opaqueObjects)
    {
        object->draw(camera, elapsedTime, supernovaTime);
    }
    for (const RenderObject *object : transparentObjects)
    {
        object->draw(camera, elapsedTime, supernovaTime);
    }

    renderer.blitSceneToDefault();
    blackHole->draw(camera, renderer.getSceneColorTexture(),
                    glm::vec2(window.getWidth(), window.getHeight()));

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

    const auto now = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(now - instance->previousTime).count();
    instance->previousTime = now;

    deltaTime = std::min(deltaTime, 0.1f);

    instance->elapsedTime += deltaTime;
    if (!instance->supernovaActive)
        instance->supernovaStop += deltaTime;
    instance->supernovaTime = instance->elapsedTime - instance->supernovaStop;

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
                                     RenderObject::StateFunc beforeDrawFunc,
                                     RenderObject::StateFunc afterDrawFunc)
{
    std::vector<std::unique_ptr<Mesh>> loadedMeshes = GltfLoader::load(path);

    for (auto &loadedMesh : loadedMeshes)
    {
        Mesh *mesh = addMesh(std::move(loadedMesh));

        RenderObject object(mesh, shader, isTransparent);
        object.transform = transform;

        object.setBeforeDraw(beforeDrawFunc);
        object.setAfterDraw(afterDrawFunc);

        objects.push_back(object);
    }
}

void Application::loadObjectFromMesh(std::unique_ptr<Mesh> mesh, Shader *shader,
                                     const Transform &transform, bool isTransparent,
                                     RenderObject::StateFunc beforeDrawFunc,
                                     RenderObject::StateFunc afterDrawFunc)
{
    RenderObject object(addMesh(std::move(mesh)), shader, isTransparent);
    object.transform = transform;

    object.setBeforeDraw(beforeDrawFunc);
    object.setAfterDraw(afterDrawFunc);

    objects.push_back(object);
}
