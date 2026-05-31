cmake_minimum_required(VERSION 3.20)

project(ProjetPOGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/core/Application.cpp
    src/core/Application.hpp
    src/graphics/Renderer.cpp
    src/graphics/Renderer.hpp
    src/graphics/Shader.cpp
    src/graphics/Shader.hpp
    src/graphics/Triangle.cpp
    src/graphics/Triangle.hpp
    src/core/Window.cpp
    src/core/Window.hpp
)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        GLEW::GLEW
        GLUT::GLUT
        OpenGL::GL
)
