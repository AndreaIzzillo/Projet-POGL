#pragma once

#include <GL/glew.h>

class Renderer
{
public:
    ~Renderer();

    void init();

    // Offscreen scene capture (used for the black hole's gravitational lensing).
    // Renders the whole scene into an FBO so a later pass can sample it as a texture.
    void initFramebuffer(int width, int height);
    void beginSceneCapture() const;
    void blitSceneToDefault() const;
    GLuint getSceneColorTexture() const;

    void clear() const;

private:
    int width = 0;
    int height = 0;

    GLuint sceneFbo = 0;
    GLuint sceneColorTex = 0;
    GLuint depthRbo = 0;
};
