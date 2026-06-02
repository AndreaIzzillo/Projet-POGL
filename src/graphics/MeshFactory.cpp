#include "graphics/MeshFactory.hpp"

std::unique_ptr<Mesh> MeshFactory::createTriangle()
{
    const glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);

    std::vector<Vertex> vertices = {
        { glm::vec3(-0.6f, -0.5f, 0.0f), normal, glm::vec3(1.0f, 0.2f, 0.2f) },
        { glm::vec3(0.6f, -0.5f, 0.0f), normal, glm::vec3(0.2f, 1.0f, 0.2f) },
        { glm::vec3(0.0f, 0.6f, 0.0f), normal, glm::vec3(0.2f, 0.4f, 1.0f) }
    };

    std::vector<unsigned int> indices = { 0, 1, 2 };

    return std::make_unique<Mesh>(vertices, indices);
}

std::unique_ptr<Mesh> MeshFactory::createCube()
{
    auto makeVertex = [](const glm::vec3 &position, const glm::vec3 &color) {
        return Vertex{ position, glm::normalize(position), color };
    };

    std::vector<Vertex> vertices = {
        makeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.2f, 0.2f)),
        makeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.2f, 1.0f, 0.2f)),
        makeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.2f, 0.4f, 1.0f)),
        makeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 0.2f)),
        makeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.2f, 1.0f)),
        makeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.2f, 1.0f, 1.0f)),
        makeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)),
        makeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f))
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 4, 7, 3, 3, 0, 4,
                                          1, 5, 6, 6, 2, 1, 0, 1, 5, 5, 4, 0, 3, 2, 6, 6, 7, 3 };

    return std::make_unique<Mesh>(vertices, indices);
}