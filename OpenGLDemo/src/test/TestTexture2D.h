#pragma once

#include "Test.h"
#include "glm/glm.hpp"
#include <memory>

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;
class Texture;

namespace Test {

	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		glm::vec3 m_TranslationA, m_TranslationB;
		glm::mat4 m_ProjectionMatrix, m_ViewMatrix;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_ShaderProgram;
		std::unique_ptr<Texture> m_Texture2DA, m_Texture2DB;
	};

}
