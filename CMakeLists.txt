cmake_minimum_required(VERSION 3.20)

project(ProjetPOGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    src/main.cpp

    src/core/Application.cpp
    src/core/Application.hpp
    src/core/Window.cpp
    src/core/Window.hpp

    src/graphics/Renderer.cpp
    src/graphics/Renderer.hpp
    src/graphics/Shader.cpp
    src/graphics/Shader.hpp
    src/graphics/Mesh.cpp
    src/graphics/Mesh.hpp
    src/graphics/MeshFactory.cpp
    src/graphics/MeshFactory.hpp
    src/graphics/Skybox.cpp
    src/graphics/Skybox.hpp
    src/graphics/BlackHole.cpp
    src/graphics/BlackHole.hpp

    src/utils/GltfLoader.cpp
    src/utils/GltfLoader.hpp

    src/scene/Camera.cpp
    src/scene/Camera.hpp
    src/scene/Transform.hpp
    src/scene/RenderObject.cpp
    src/scene/RenderObject.hpp
)

# --- OpenGL ---
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm CONFIG QUIET)

if(NOT TARGET glm::glm)
    find_path(GLM_INCLUDE_DIR glm/glm.hpp REQUIRED)
    add_library(glm::glm INTERFACE IMPORTED)
    set_target_properties(glm::glm
        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${GLM_INCLUDE_DIR}"
    )
endif()
# --- OpenGL ---

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
        libs/stb_image
        libs/tinygltf
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        GLEW::GLEW
        GLUT::GLUT
        OpenGL::GL
        glm::glm
)
