#pragma once

#include <array>
#include <memory>

#include "core/Window.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/MeshFactory.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/Shader.hpp"
#include "scene/Camera.hpp"

class Application
{
public:
    Application(int &argc, char **argv);
    ~Application() = default;

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    void run();
    void update(float dt);
    void render();

private:
    Window window;
    Renderer renderer;
    Camera camera;

    std::unique_ptr<Shader> shader;
    std::unique_ptr<Mesh> mesh;

    std::array<bool, 256> keys{};
    int previousTimeMs = 0;

    bool mouseCaptured = false;
    int lastMouseX = 0;
    int lastMouseY = 0;

    static Application *instance;

    static void displayCallback();
    static void idleCallback();
    static void keyboardDownCallback(unsigned char key, int x, int y);
    static void keyboardUpCallback(unsigned char key, int x, int y);
    static void mouseButtonCallback(int button, int state, int x, int y);
    static void mouseMotionCallback(int x, int y);
};