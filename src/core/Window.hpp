#pragma once

#include <string>

class Window
{
public:
    Window(int &argc, char **argv, int width, int height, const std::string &title);
    ~Window() = default;

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    void startMainLoop() const;
    void swapBuffers() const;

    int getWidth() const;
    int getHeight() const;

private:
    int width;
    int height;
};