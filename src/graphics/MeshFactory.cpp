#include "graphics/MeshFactory.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <unordered_map>
#include <utility>

std::unique_ptr<Mesh> MeshFactory::createSphere(unsigned int subdivisions, const glm::vec3 &color)
{
    std::vector<Vertex> vertices;

    // Icosahedron
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

std::unique_ptr<Mesh> MeshFactory::createCube(const glm::vec3 &color)
{
    std::vector<Vertex> vertices = {
        // Front face
        { { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, color },
        { { 0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, color },
        { { 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, color },
        { { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, color },
        // Back face
        { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, color },
        { { -0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, color },
        { { 0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, color },
        { { 0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, color },
        // Left face
        { { -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, color },
        { { -0.5f, -0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, color },
        { { -0.5f, 0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, color },
        { { -0.5f, 0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, color },
        // Right face
        { { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, color },
        { { 0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, color },
        { { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, color },
        { { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, color },
        // Top face
        { { -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, color },
        { { -0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, color },
        { { 0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, color },
        { { 0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, color },
        // Bottom face
        { { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, color },
        { { 0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, color },
        { { 0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, color },
        { { -0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, color },
    };

    std::vector<unsigned int> indices = {
        0,  1,  2,  0,  2,  3, // Front face
        4,  5,  6,  4,  6,  7, // Back face
        8,  9,  10, 8,  10, 11, // Left face
        12, 13, 14, 12, 14, 15, // Right face
        16, 17, 18, 16, 18, 19, // Top face
        20, 21, 22, 20, 22, 23 // Bottom face
    };

    return std::make_unique<Mesh>(vertices, indices);
}

std::unique_ptr<Mesh> MeshFactory::createDisk(unsigned int subdivisions, const glm::vec3 &color)
{
    const unsigned int segmentCount = subdivisions < 3 ? 3 : subdivisions;
    constexpr float tau = 6.28318530717958647692f;

    std::vector<Vertex> vertices;
    vertices.reserve((segmentCount + 1) * 2);

    const glm::vec3 frontNormal(0.0f, 0.0f, 1.0f);
    const glm::vec3 backNormal(0.0f, 0.0f, -1.0f);

    vertices.push_back(Vertex{ glm::vec3(0.0f), frontNormal, color });
    for (unsigned int i = 0; i < segmentCount; ++i)
    {
        const float angle = tau * static_cast<float>(i) / static_cast<float>(segmentCount);
        vertices.push_back(
            Vertex{ glm::vec3(std::cos(angle), std::sin(angle), 0.0f), frontNormal, color });
    }

    const unsigned int backCenterIndex = static_cast<unsigned int>(vertices.size());
    vertices.push_back(Vertex{ glm::vec3(0.0f), backNormal, color });
    for (unsigned int i = 0; i < segmentCount; ++i)
    {
        const float angle = tau * static_cast<float>(i) / static_cast<float>(segmentCount);
        vertices.push_back(
            Vertex{ glm::vec3(std::cos(angle), std::sin(angle), 0.0f), backNormal, color });
    }

    std::vector<unsigned int> indices;
    indices.reserve(segmentCount * 6);

    for (unsigned int i = 0; i < segmentCount; ++i)
    {
        const unsigned int current = i + 1;
        const unsigned int next = (i + 1) % segmentCount + 1;

        indices.push_back(0);
        indices.push_back(current);
        indices.push_back(next);

        indices.push_back(backCenterIndex);
        indices.push_back(backCenterIndex + next);
        indices.push_back(backCenterIndex + current);
    }

    return std::make_unique<Mesh>(vertices, indices);
}
