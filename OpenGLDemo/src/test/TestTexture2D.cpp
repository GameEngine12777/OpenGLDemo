#include "TestTexture2D.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include <imgui/imgui.h>

namespace Test {

	TestTexture2D::TestTexture2D()
        : m_TranslationA(0.f, 0.f, 0.f)
        , m_TranslationB(0.f, 0.f, 0.f)
        , m_ProjectionMatrix(glm::ortho<float>(1920.f / 1080.f * -50.f, 1920.f / 1080.f * 50.f, -50.f, 50.f, -1.0f, 1.0f))
        , m_ViewMatrix(glm::translate(glm::mat4(1.f), glm::vec3(0.f)))
	{
        // 开启混合功能，混合开启后，片段着色器输出的颜色值会与帧缓冲中已有的颜色值进行混合，以产生最终的像素颜色。
        glEnable(GL_BLEND);
        // 设置了混合因子（blending factors），决定了新颜色和原颜色如何加权组合。
        // 开启混合模式后得计算公式(FinalColor = SrcColor * SrcFactor + DstColor * DstFactor) (SrcColor 是当前片段着色器输出的颜色。) (DstColor 是当前帧缓冲中已有的颜色。)
        // GL_SRC_ALPHA: 使用源颜色的 alpha 值作为混合因子
        // GL_ONE_MINUS_SRC_ALPHA: 使用 1 - 源 alpha 值作为目标混合因子
        // 最终计算公式(线性透明度混合公式): FinalColor = SrcColor * SrcAlpha + DstColor * (1 - SrcAlpha) 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float vertexBuffer[] = {
            -12.f, -9.f, 0.f, 0.0f,
            -12.f,  9.f, 0.f, 1.0f,
             12.f,  9.f, 1.f, 1.0f,
             12.f, -9.f, 1.f, 0.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };

        // 绑定 VertexArrayObject
        m_VAO = std::make_unique<VertexArray>();

        // 绑定 VertexBuffer
        m_VBO = std::make_unique<VertexBuffer>(vertexBuffer, sizeof(vertexBuffer));

        // 与 VAO 建立链接，绑定缓存布局
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VBO, layout);

        // 绑定 IndexBuffer
        m_IBO = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

        // 绑定着色器程序
        m_ShaderProgram = std::make_unique<Shader>("res/shaders/Basic.shader");

        m_Texture2DA = std::make_unique<Texture>("res/textures/IMG_20220707_191336.jpg");
        m_Texture2DB = std::make_unique<Texture>("res/textures/ChernoLogo.png");
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        Renderer renderer;

        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_TranslationA);

            m_ShaderProgram->Bind();
            m_Texture2DA->Bind();
            m_ShaderProgram->SetUniformMat4f("u_MVP", m_ProjectionMatrix * m_ViewMatrix * modelMatrix);
            m_ShaderProgram->SetUniform1i("u_Texture", 0);
            renderer.Draw(m_VAO.get(), m_IBO.get(), m_ShaderProgram.get());
        }

        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_TranslationB);

            m_ShaderProgram->Bind();
            m_Texture2DB->Bind(1);
            m_ShaderProgram->SetUniformMat4f("u_MVP", m_ProjectionMatrix * m_ViewMatrix * modelMatrix);
            m_ShaderProgram->SetUniform1i("u_Texture", 1);
            renderer.Draw(m_VAO.get(), m_IBO.get(), m_ShaderProgram.get());
        }

	}

	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_TranslationA[0], -50.f, 50.f);
        m_TranslationA.z = 0;
        ImGui::SliderFloat3("Translation B", &m_TranslationB[0], -50.f, 50.f);
        m_TranslationB.z = 0;
	}

}