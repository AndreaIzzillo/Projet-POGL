#pragma once

#include <array>
#include <memory>
#include <vector>

#include "core/Window.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/MeshFactory.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/Shader.hpp"
#include "scene/Camera.hpp"
#include "scene/RenderObject.hpp"
#include "utils/GltfLoader.hpp"

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

    // Shaders
    std::unique_ptr<Shader> munaShader;
    std::unique_ptr<Shader> sunShader;

    // Meshes and objects
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<RenderObject> objects;

    std::array<bool, 256> keys{};
    int previousTimeMs = 0;
    float elapsedTime = 0.0f;

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

    Mesh *addMesh(std::unique_ptr<Mesh> mesh);

    void loadObjectFromFile(const std::string &path, Shader *shader, const Transform &transform);
    void loadObjectFromMesh(std::unique_ptr<Mesh> mesh, Shader *shader, const Transform &transform);
};
