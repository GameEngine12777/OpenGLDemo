#include "Renderer.h"

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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

void Renderer::Draw(const VertexArray* va, const IndexBuffer* ib, const Shader* shader) const
{
    shader->Bind();
    va->Bind();
    ib->Bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
}
