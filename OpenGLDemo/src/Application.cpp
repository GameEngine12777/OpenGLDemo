#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include <string>
#include <vector>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // TODO:: Error Handling
    int res;
    glGetShaderiv(id, GL_COMPILE_STATUS, &res); // 获取着色器得编译结果
    if (res == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // 获取着色器日志信息得长度
        // char* errorLog = (char*)alloca(sizeof(char) * length); // 分配栈内存
        std::vector<char> errorLog(length);
        glGetShaderInfoLog(id, length, &length, &errorLog[0]); // 获取着色器日志信息
        std::cout << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader: "
            << errorLog.data()
            << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program); // 验证有效性

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

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

    float vertexBuffer[18] = {
        -0.5f, -0.5f, 1.f, 0.0f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.f, 0.0f, 1.0f, 1.0f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vertexBuffer, GL_STATIC_DRAW);

    // 启用索引为0得顶点属性（绑定顶点位置信息）
    glEnableVertexAttribArray(0);
    // 设置索引为0得顶点属性数据，这将决定着色器如何在一组顶点缓存数据中读取索引为0得数据（数据绑定）
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)0);

    // 绑定颜色信息
    glEnableVertexAttribArray(1);
    /**
    * 参数解释：
    * 1  属性布局索引
    * 4  构成该属性得数量
    * GL_FLOAT  数据类型
    * GL_FALSE  是否标准化
    * sizeof(float) * 6  每组顶点缓存数据得大小
    * (const void*)(2 * sizeof(float))  当前属性在布局中得偏移
    */
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)(2 * sizeof(float)));

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "layout(location = 1) in vec4 color;\n"
        "\n"
        "out vec4 v_Color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "   v_Color = color;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "in vec4 v_Color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = v_Color;\n"
        "}\n";

    unsigned int shaderProgram = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteBuffers(1, &buffer); // 清理缓存
    glDeleteProgram(shaderProgram); // 清理着色器程序

    glfwTerminate();
    return 0;
}