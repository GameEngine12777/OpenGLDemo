#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <cmath>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x) GLClearError(); \
    x;\
    ASSERT(GLCheckError(__FILE__, #x, __LINE__));

static void GLClearError()
{
    // https://docs.gl/gl4/glGetError
    while (glGetError() != GL_NO_ERROR);
}

static bool GLCheckError(const char* file, const char* function, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL error] (" << error << ") " << file << " : " << function << " : " << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));

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

    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program)); // 验证有效性

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


    const int numPoints = 200;
    std::vector<float> vertexBuffer;

    // 中心点（白色或黑色）
    vertexBuffer.push_back(0.0f); // x
    vertexBuffer.push_back(0.0f); // y
    vertexBuffer.push_back(1.0f); // r
    vertexBuffer.push_back(1.0f); // g
    vertexBuffer.push_back(1.0f); // b
    vertexBuffer.push_back(1.0f); // a

    // 缩放因子（把图形控制在 -1 ~ 1）
    float scale = 0.02f;

    for (int i = 0; i <= numPoints; ++i) {
        float t = (float)i / numPoints * 2.0f * 3.1415926;

        float x = 16 * pow(sin(t), 3);
        float y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);

        x *= scale;
        y *= scale;

        // 加红色
        vertexBuffer.push_back(x);
        vertexBuffer.push_back(y);
        vertexBuffer.push_back(1.0f); // R
        vertexBuffer.push_back(0.0f); // G
        vertexBuffer.push_back(0.0f); // B
        vertexBuffer.push_back(1.0f);
    }

    unsigned int buffer;
    GLCALL(glGenBuffers(1, &buffer));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW));

    // 设置位置属性
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)0));

    // 设置颜色属性
    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)(2 * sizeof(float))));

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

    unsigned int shaderProgram = CreateShader(source.VertexSource, source.FragmentSource);
    GLCALL(glUseProgram(shaderProgram));

    int location = glGetUniformLocation(shaderProgram, "u_Time");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //GLClearError();
        //// 请注意所有得索引缓存必须由无符号类型得数据组成
        //glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
        //ASSERT(GLCheckError());

        // GLCALL(glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, nullptr));
        GLCALL(glDrawArrays(GL_TRIANGLE_FAN, 0, numPoints + 2));

        float timeValue = glfwGetTime();
        glUniform1f(location, timeValue);

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