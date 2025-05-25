#pragma once

#include <GL/glew.h>


#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x) GLClearError(); \
    x;\
    ASSERT(GLCheckError(__FILE__, #x, __LINE__));

void GLClearError();

bool GLCheckError(const char* file, const char* function, int line);

class VertexArray;
class IndexBuffer;
class Shader;

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray* va, const IndexBuffer* ib, const Shader* shader) const;
};

