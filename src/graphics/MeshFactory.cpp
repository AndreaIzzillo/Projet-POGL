#include "graphics/MeshFactory.hpp"

std::unique_ptr<Mesh> MeshFactory::createTriangle()
{
    std::vector<Vertex> vertices = { { glm::vec3(-0.6f, -0.5f, 0.0f), glm::vec3(1.0f, 0.2f, 0.2f) },
                                     { glm::vec3(0.6f, -0.5f, 0.0f), glm::vec3(0.2f, 1.0f, 0.2f) },
                                     { glm::vec3(0.0f, 0.6f, 0.0f), glm::vec3(0.2f, 0.4f, 1.0f) } };

    std::vector<unsigned int> indices = { 0, 1, 2 };

    return std::make_unique<Mesh>(vertices, indices);
}

std::unique_ptr<Mesh> MeshFactory::createCube()
{
    std::vector<Vertex> vertices = {
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.2f, 0.2f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.2f, 1.0f, 0.2f) },
        { glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.2f, 0.4f, 1.0f) },
        { glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 0.2f) },
        { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.2f, 1.0f) },
        { glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.2f, 1.0f, 1.0f) },
        { glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f) },
        { glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f) }
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 4, 7, 3, 3, 0, 4,
                                          1, 5, 6, 6, 2, 1, 0, 1, 5, 5, 4, 0, 3, 2, 6, 6, 7, 3 };

    return std::make_unique<Mesh>(vertices, indices);
}