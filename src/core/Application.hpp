#pragma once

#include <array>
#include <memory>
#include <vector>

#include "core/Window.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/MeshFactory.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Skybox.hpp"
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
    std::unique_ptr<Shader> sunFlareShader;
    std::unique_ptr<Shader> earthShader;
    std::unique_ptr<Shader> earthAtmoShader;
    std::unique_ptr<Shader> cloudsShader;
    std::unique_ptr<Shader> dwarfShallowShader;
    std::unique_ptr<Shader> ezakiSixShader;
    std::unique_ptr<Shader> ezakiRingShader;
    std::unique_ptr<Shader> supernovaShader;

    // Meshes and objects
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<RenderObject> objects;
    std::unique_ptr<Skybox> skybox;

    // Animated celestial bodies (indices into objects)
    std::size_t earthIndex = 0;
    std::size_t earthAtmoIndex = 0;
    std::size_t cloudsIndex = 0;
    std::size_t moonIndex = 0;
    std::size_t dwarfShallowIndex = 0;
    std::size_t ezakiSixIndex = 0;
    std::size_t ezakiRingIndex = 0;

    int cameraAttachedTo = -1;

    std::array<bool, 256> keys{};
    int previousTimeMs = 0;
    float elapsedTime = 0.0f;
    float supernovaTime = 0.0f;
    float supernovaStop = 0.0f;
    bool supernovaActive = false;

    bool mouseCaptured = false;
    int lastMouseX = 0;
    int lastMouseY = 0;

    static Application *instance;

    void loadScene();

    static void displayCallback();
    static void idleCallback();
    static void keyboardDownCallback(unsigned char key, int x, int y);
    static void keyboardUpCallback(unsigned char key, int x, int y);
    static void mouseButtonCallback(int button, int state, int x, int y);
    static void mouseMotionCallback(int x, int y);

    Mesh *addMesh(std::unique_ptr<Mesh> mesh);

    void loadObjectFromFile(const std::string &path, Shader *shader, const Transform &transform,
                            bool isTransparent = false,
                            RenderObject::StateFunc beforeDrawFunc = nullptr,
                            RenderObject::StateFunc afterDrawFunc = nullptr);
    void loadObjectFromMesh(std::unique_ptr<Mesh> mesh, Shader *shader, const Transform &transform,
                            bool isTransparent = false,
                            RenderObject::StateFunc beforeDrawFunc = nullptr,
                            RenderObject::StateFunc afterDrawFunc = nullptr);
};
