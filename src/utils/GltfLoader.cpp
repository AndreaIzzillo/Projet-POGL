#include "utils/GltfLoader.hpp"

#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include <tiny_gltf.h>

namespace
{
    bool noopLoadImage(tinygltf::Image *, const int, std::string *, std::string *, int, int,
                       const unsigned char *, int, void *)
    {
        return true;
    }

    std::vector<float> readAccessorAsFloats(const tinygltf::Model &model,
                                            const tinygltf::Accessor &accessor)
    {
        const tinygltf::BufferView &view = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer &buffer = model.buffers[view.buffer];

        const int components =
            tinygltf::GetNumComponentsInType(static_cast<uint32_t>(accessor.type));
        const int componentSize =
            tinygltf::GetComponentSizeInBytes(static_cast<uint32_t>(accessor.componentType));
        const size_t stride = static_cast<size_t>(accessor.ByteStride(view));
        const unsigned char *base = buffer.data.data() + view.byteOffset + accessor.byteOffset;

        std::vector<float> out;
        out.reserve(accessor.count * static_cast<size_t>(components));

        for (size_t i = 0; i < accessor.count; ++i)
        {
            const unsigned char *element = base + i * stride;
            for (int c = 0; c < components; ++c)
            {
                const unsigned char *p =
                    element + static_cast<size_t>(c) * static_cast<size_t>(componentSize);
                float value = 0.0f;
                switch (accessor.componentType)
                {
                case TINYGLTF_COMPONENT_TYPE_FLOAT:
                    value = *reinterpret_cast<const float *>(p);
                    break;
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                    value = accessor.normalized ? static_cast<float>(*p) / 255.0f
                                                : static_cast<float>(*p);
                    break;
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                    const uint16_t v = *reinterpret_cast<const uint16_t *>(p);
                    value = accessor.normalized ? static_cast<float>(v) / 65535.0f
                                                : static_cast<float>(v);
                    break;
                }
                case TINYGLTF_COMPONENT_TYPE_BYTE: {
                    const int8_t v = *reinterpret_cast<const int8_t *>(p);
                    value = accessor.normalized ? glm::max(static_cast<float>(v) / 127.0f, -1.0f)
                                                : static_cast<float>(v);
                    break;
                }
                case TINYGLTF_COMPONENT_TYPE_SHORT: {
                    const int16_t v = *reinterpret_cast<const int16_t *>(p);
                    value = accessor.normalized ? glm::max(static_cast<float>(v) / 32767.0f, -1.0f)
                                                : static_cast<float>(v);
                    break;
                }
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                    value = static_cast<float>(*reinterpret_cast<const uint32_t *>(p));
                    break;
                default:
                    break;
                }
                out.push_back(value);
            }
        }

        return out;
    }

    std::vector<unsigned int> readIndices(const tinygltf::Model &model,
                                          const tinygltf::Accessor &accessor)
    {
        const tinygltf::BufferView &view = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer &buffer = model.buffers[view.buffer];
        const size_t stride = static_cast<size_t>(accessor.ByteStride(view));
        const unsigned char *base = buffer.data.data() + view.byteOffset + accessor.byteOffset;

        std::vector<unsigned int> indices;
        indices.reserve(accessor.count);

        for (size_t i = 0; i < accessor.count; ++i)
        {
            const unsigned char *p = base + i * stride;
            switch (accessor.componentType)
            {
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                indices.push_back(static_cast<unsigned int>(*p));
                break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                indices.push_back(
                    static_cast<unsigned int>(*reinterpret_cast<const uint16_t *>(p)));
                break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                indices.push_back(*reinterpret_cast<const uint32_t *>(p));
                break;
            default:
                break;
            }
        }

        return indices;
    }

    glm::mat4 nodeLocalMatrix(const tinygltf::Node &node)
    {
        if (node.matrix.size() == 16)
        {
            glm::mat4 matrix(1.0f);
            float *m = glm::value_ptr(matrix);
            for (int i = 0; i < 16; ++i)
            {
                m[i] = static_cast<float>(node.matrix[i]);
            }
            return matrix;
        }

        glm::mat4 matrix(1.0f);
        if (node.translation.size() == 3)
        {
            matrix = glm::translate(
                matrix, glm::vec3(node.translation[0], node.translation[1], node.translation[2]));
        }
        if (node.rotation.size() == 4)
        {
            const glm::quat q(
                static_cast<float>(node.rotation[3]), static_cast<float>(node.rotation[0]),
                static_cast<float>(node.rotation[1]), static_cast<float>(node.rotation[2]));
            matrix *= glm::mat4_cast(q);
        }
        if (node.scale.size() == 3)
        {
            matrix = glm::scale(matrix, glm::vec3(node.scale[0], node.scale[1], node.scale[2]));
        }
        return matrix;
    }

    glm::vec3 primitiveBaseColor(const tinygltf::Model &model, const tinygltf::Primitive &primitive)
    {
        if (primitive.material >= 0
            && primitive.material < static_cast<int>(model.materials.size()))
        {
            const std::vector<double> &factor =
                model.materials[primitive.material].pbrMetallicRoughness.baseColorFactor;
            if (factor.size() >= 3)
            {
                return glm::vec3(static_cast<float>(factor[0]), static_cast<float>(factor[1]),
                                 static_cast<float>(factor[2]));
            }
        }
        return glm::vec3(0.8f);
    }

    void appendPrimitive(const tinygltf::Model &model, const tinygltf::Primitive &primitive,
                         const glm::mat4 &worldMatrix, std::vector<std::unique_ptr<Mesh>> &out)
    {
        if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
        {
            std::cerr << "GltfLoader: primitive ignoree (mode != TRIANGLES)\n";
            return;
        }

        const auto positionIt = primitive.attributes.find("POSITION");
        if (positionIt == primitive.attributes.end())
        {
            return;
        }

        const std::vector<float> positions =
            readAccessorAsFloats(model, model.accessors[positionIt->second]);
        const size_t vertexCount = positions.size() / 3;
        if (vertexCount == 0)
        {
            return;
        }

        std::vector<float> normals;
        const auto normalIt = primitive.attributes.find("NORMAL");
        if (normalIt != primitive.attributes.end())
        {
            normals = readAccessorAsFloats(model, model.accessors[normalIt->second]);
        }

        std::vector<float> vertexColors;
        int vertexColorComponents = 0;
        const auto colorIt = primitive.attributes.find("COLOR_0");
        if (colorIt != primitive.attributes.end())
        {
            const tinygltf::Accessor &accessor = model.accessors[colorIt->second];
            vertexColorComponents =
                tinygltf::GetNumComponentsInType(static_cast<uint32_t>(accessor.type));
            vertexColors = readAccessorAsFloats(model, accessor);
        }

        const glm::vec3 baseColor = primitiveBaseColor(model, primitive);
        const glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(worldMatrix)));
        const glm::vec3 lightDir = glm::normalize(glm::vec3(0.4f, 1.0f, 0.6f));

        std::vector<Vertex> vertices;
        vertices.reserve(vertexCount);

        for (size_t i = 0; i < vertexCount; ++i)
        {
            const glm::vec3 localPos(positions[i * 3 + 0], positions[i * 3 + 1],
                                     positions[i * 3 + 2]);
            const glm::vec3 worldPos = glm::vec3(worldMatrix * glm::vec4(localPos, 1.0f));

            glm::vec3 color = baseColor;
            if (!vertexColors.empty())
            {
                const size_t offset = i * static_cast<size_t>(vertexColorComponents);
                color = glm::vec3(vertexColors[offset + 0], vertexColors[offset + 1],
                                  vertexColors[offset + 2]);
            }

            if (!normals.empty())
            {
                const glm::vec3 n = glm::normalize(
                    normalMatrix
                    * glm::vec3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]));
                const float diffuse = glm::max(glm::dot(n, lightDir), 0.0f);
                color *= 0.25f + 0.75f * diffuse;
            }

            vertices.push_back({ worldPos, color });
        }

        std::vector<unsigned int> indices;
        if (primitive.indices >= 0)
        {
            indices = readIndices(model, model.accessors[primitive.indices]);
        }
        else
        {
            indices.resize(vertexCount);
            for (size_t i = 0; i < vertexCount; ++i)
            {
                indices[i] = static_cast<unsigned int>(i);
            }
        }

        out.push_back(std::make_unique<Mesh>(vertices, indices));
    }

    void processNode(const tinygltf::Model &model, int nodeIndex, const glm::mat4 &parentMatrix,
                     std::vector<std::unique_ptr<Mesh>> &out)
    {
        if (nodeIndex < 0 || nodeIndex >= static_cast<int>(model.nodes.size()))
        {
            return;
        }

        const tinygltf::Node &node = model.nodes[nodeIndex];
        const glm::mat4 worldMatrix = parentMatrix * nodeLocalMatrix(node);

        if (node.mesh >= 0 && node.mesh < static_cast<int>(model.meshes.size()))
        {
            for (const tinygltf::Primitive &primitive : model.meshes[node.mesh].primitives)
            {
                appendPrimitive(model, primitive, worldMatrix, out);
            }
        }

        for (int child : node.children)
        {
            processNode(model, child, worldMatrix, out);
        }
    }
} // namespace

std::vector<std::unique_ptr<Mesh>> GltfLoader::load(const std::string &path)
{
    tinygltf::TinyGLTF loader;
    loader.SetImageLoader(noopLoadImage, nullptr);

    tinygltf::Model model;
    std::string err;
    std::string warn;

    bool ok = false;
    if (path.size() >= 4 && path.compare(path.size() - 4, 4, ".glb") == 0)
    {
        ok = loader.LoadBinaryFromFile(&model, &err, &warn, path);
    }
    else
    {
        ok = loader.LoadASCIIFromFile(&model, &err, &warn, path);
    }

    if (!warn.empty())
    {
        std::cerr << "GltfLoader: avertissement : " << warn << '\n';
    }
    if (!ok)
    {
        std::cerr << "GltfLoader: echec du chargement de '" << path << "' : " << err << '\n';
        return {};
    }

    std::vector<std::unique_ptr<Mesh>> meshes;

    const int sceneIndex = model.defaultScene >= 0 ? model.defaultScene : 0;
    if (sceneIndex < static_cast<int>(model.scenes.size()))
    {
        for (int nodeIndex : model.scenes[sceneIndex].nodes)
        {
            processNode(model, nodeIndex, glm::mat4(1.0f), meshes);
        }
    }
    else
    {
        for (const tinygltf::Mesh &mesh : model.meshes)
        {
            for (const tinygltf::Primitive &primitive : mesh.primitives)
            {
                appendPrimitive(model, primitive, glm::mat4(1.0f), meshes);
            }
        }
    }

    std::cout << "GltfLoader: '" << path << "' -> " << meshes.size() << " mesh(es)\n";
    return meshes;
}
