#pragma once

#include <memory>

#include "core/Window.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Triangle.hpp"
#include "scene/Camera.hpp"

class Application
{
public:
    Application(int &argc, char **argv);
    ~Application() = default;

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    void run();
    void render();

private:
    Window window;
    Renderer renderer;
    Camera camera;

    std::unique_ptr<Shader> shader;
    std::unique_ptr<Triangle> triangle;

    static Application *instance;

    static void displayCallback();
    static void idleCallback();
    static void keyboardCallback(unsigned char key, int x, int y);
};