#include "graphics/MeshFactory.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <unordered_map>
#include <utility>

std::unique_ptr<Mesh> MeshFactory::createSphere(unsigned int subdivisions,
                                                   const glm::vec3 &color)
{
    std::vector<Vertex> vertices;
    std::vector<std::array<unsigned int, 3>> faces = { std::array<unsigned int, 3>{ 0, 11, 5 },
                                                       { 0, 5, 1 },
                                                       { 0, 1, 7 },
                                                       { 0, 7, 10 },
                                                       { 0, 10, 11 },
                                                       { 1, 5, 9 },
                                                       { 5, 11, 4 },
                                                       { 11, 10, 2 },
                                                       { 10, 7, 6 },
                                                       { 7, 1, 8 },
                                                       { 3, 9, 4 },
                                                       { 3, 4, 2 },
                                                       { 3, 2, 6 },
                                                       { 3, 6, 8 },
                                                       { 3, 8, 9 },
                                                       { 4, 9, 5 },
                                                       { 2, 4, 11 },
                                                       { 6, 2, 10 },
                                                       { 8, 6, 7 },
                                                       { 9, 8, 1 } };

    auto addVertex = [&vertices, &color](const glm::vec3 &position) {
        const glm::vec3 normalizedPosition = glm::normalize(position);
        vertices.push_back(Vertex{ normalizedPosition, normalizedPosition, color });
        return static_cast<unsigned int>(vertices.size() - 1);
    };

    const float t = (1.0f + std::sqrt(5.0f)) * 0.5f;
    addVertex(glm::vec3(-1.0f, t, 0.0f));
    addVertex(glm::vec3(1.0f, t, 0.0f));
    addVertex(glm::vec3(-1.0f, -t, 0.0f));
    addVertex(glm::vec3(1.0f, -t, 0.0f));
    addVertex(glm::vec3(0.0f, -1.0f, t));
    addVertex(glm::vec3(0.0f, 1.0f, t));
    addVertex(glm::vec3(0.0f, -1.0f, -t));
    addVertex(glm::vec3(0.0f, 1.0f, -t));
    addVertex(glm::vec3(t, 0.0f, -1.0f));
    addVertex(glm::vec3(t, 0.0f, 1.0f));
    addVertex(glm::vec3(-t, 0.0f, -1.0f));
    addVertex(glm::vec3(-t, 0.0f, 1.0f));

    auto edgeKey = [](unsigned int a, unsigned int b) {
        const unsigned int minIndex = a < b ? a : b;
        const unsigned int maxIndex = a < b ? b : a;
        return (static_cast<std::uint64_t>(minIndex) << 32) | maxIndex;
    };

    for (unsigned int i = 0; i < subdivisions; ++i)
    {
        std::unordered_map<std::uint64_t, unsigned int> midpointCache;
        std::vector<std::array<unsigned int, 3>> subdividedFaces;
        subdividedFaces.reserve(faces.size() * 4);

        auto midpointIndex = [&](unsigned int a, unsigned int b) {
            const std::uint64_t key = edgeKey(a, b);
            const auto it = midpointCache.find(key);
            if (it != midpointCache.end())
            {
                return it->second;
            }

            const unsigned int midpoint =
                addVertex((vertices[a].position + vertices[b].position) * 0.5f);
            midpointCache.emplace(key, midpoint);
            return midpoint;
        };

        for (const auto &face : faces)
        {
            const unsigned int a = midpointIndex(face[0], face[1]);
            const unsigned int b = midpointIndex(face[1], face[2]);
            const unsigned int c = midpointIndex(face[2], face[0]);

            subdividedFaces.push_back({ face[0], a, c });
            subdividedFaces.push_back({ face[1], b, a });
            subdividedFaces.push_back({ face[2], c, b });
            subdividedFaces.push_back({ a, b, c });
        }

        faces = std::move(subdividedFaces);
    }

    std::vector<unsigned int> indices;
    indices.reserve(faces.size() * 3);
    for (const auto &face : faces)
    {
        indices.push_back(face[0]);
        indices.push_back(face[1]);
        indices.push_back(face[2]);
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
