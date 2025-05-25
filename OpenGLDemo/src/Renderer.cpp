#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    // https://docs.gl/gl4/glGetError
    while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* file, const char* function, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL error] (" << error << ") " << file << " : " << function << " : " << line << std::endl;
        return false;
    }
    return true;
}