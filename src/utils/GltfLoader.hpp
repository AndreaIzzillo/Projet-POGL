#pragma once

#include <memory>
#include <string>
#include <vector>

#include "graphics/Mesh.hpp"

class GltfLoader
{
public:
    static std::vector<std::unique_ptr<Mesh>> load(const std::string &path);
};
