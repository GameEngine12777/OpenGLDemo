#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include <string>
#include <vector>

#include <cmath>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "glew error!" << std::endl;
    }

    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

    float vertexBuffer[] = {
        -0.5f, -0.5f, 1.f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.f, 0.0f, 1.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    // 绑定 VertexArrayObject
    VertexArray* va = new VertexArray();

    // 绑定 VertexBuffer
    VertexBuffer* vb = new VertexBuffer(vertexBuffer, sizeof(vertexBuffer));

    // 与 VAO 建立链接，绑定缓存布局
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(4);
    va->AddBuffer(*vb, layout);


    // 绑定 IndexBuffer
    IndexBuffer* ib = new IndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));

    // 绑定着色器程序
    Shader* shaderProgram = new Shader("res/shaders/Basic.shader");

    float offset = 0.f;

    // 渲染器
    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        renderer.Draw(va, ib, shaderProgram);

        offset = offset + 0.01f;
        shaderProgram->Bind();
        shaderProgram->SetUniform1f("u_Offset", offset);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    delete va;
    delete vb;
    delete ib;
    delete shaderProgram;

    glfwTerminate();
    return 0;
}