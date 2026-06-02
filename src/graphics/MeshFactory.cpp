#include "graphics/MeshFactory.hpp"

std::unique_ptr<Mesh> MeshFactory::createSphere(float radius, unsigned int sectors,
                                                unsigned int stacks, const glm::vec3 &color)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    constexpr float PI = 3.14159265359f;

    for (unsigned int stack = 0; stack <= stacks; ++stack)
    {
        float stackRatio = static_cast<float>(stack) / static_cast<float>(stacks);
        float stackAngle = PI / 2.0f - stackRatio * PI;

        float xy = radius * std::cos(stackAngle);
        float z = radius * std::sin(stackAngle);

        for (unsigned int sector = 0; sector <= sectors; ++sector)
        {
            float sectorRatio = static_cast<float>(sector) / static_cast<float>(sectors);
            float sectorAngle = sectorRatio * 2.0f * PI;

            float x = xy * std::cos(sectorAngle);
            float y = xy * std::sin(sectorAngle);

            glm::vec3 position(x, y, z);
            glm::vec3 normal = glm::normalize(position);

            vertices.push_back(Vertex{ position, normal, color });
        }
    }

    for (unsigned int stack = 0; stack < stacks; ++stack)
    {
        unsigned int k1 = stack * (sectors + 1);
        unsigned int k2 = k1 + sectors + 1;

        for (unsigned int sector = 0; sector < sectors; ++sector, ++k1, ++k2)
        {
            if (stack != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (stack != stacks - 1)
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return std::make_unique<Mesh>(vertices, indices);
}

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