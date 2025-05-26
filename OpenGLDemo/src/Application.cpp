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
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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

    // 开启混合功能，混合开启后，片段着色器输出的颜色值会与帧缓冲中已有的颜色值进行混合，以产生最终的像素颜色。
    glEnable(GL_BLEND);
    // 设置了混合因子（blending factors），决定了新颜色和原颜色如何加权组合。
    // 开启混合模式后得计算公式(FinalColor = SrcColor * SrcFactor + DstColor * DstFactor) (SrcColor 是当前片段着色器输出的颜色。) (DstColor 是当前帧缓冲中已有的颜色。)
    // GL_SRC_ALPHA: 使用源颜色的 alpha 值作为混合因子
    // GL_ONE_MINUS_SRC_ALPHA: 使用 1 - 源 alpha 值作为目标混合因子
    // 最终计算公式(线性透明度混合公式): FinalColor = SrcColor * SrcAlpha + DstColor * (1 - SrcAlpha) 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float vertexBuffer[] = {
        -0.5f, -0.5f, 0.f, 0.0f,
        -0.5f,  0.5f, 0.f, 1.0f,
         0.5f,  0.5f, 1.f, 1.0f,
         0.5f, -0.5f, 1.f, 0.0f,
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
    layout.Push<float>(2);
    va->AddBuffer(*vb, layout);

    // 绑定 IndexBuffer
    IndexBuffer* ib = new IndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));

    // 创建正交投影矩阵
    glm::mat4 proj = glm::ortho<float>(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

    // 绑定着色器程序
    Shader* shaderProgram = new Shader("res/shaders/Basic.shader");

    Texture* texture = new Texture("res/textures/ChernoLogo.png");
    texture->Bind();
    shaderProgram->Bind();
    shaderProgram->SetUniform1i("u_Texture", 0);
    shaderProgram->SetUniformMat4f("u_MVP", proj);

    // 渲染器
    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        renderer.Draw(va, ib, shaderProgram);

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