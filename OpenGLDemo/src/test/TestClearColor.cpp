#include "TestClearColor.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace Test {

	TestClearColor::TestClearColor()
		: m_ClearColor(0.f)
	{
	}

	TestClearColor::TestClearColor(glm::vec4 clearColor)
		: m_ClearColor(clearColor)
	{
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::OnUpdate(float deltaTime)
	{
	}

	void TestClearColor::OnRender()
	{
		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("ClearColor", &m_ClearColor.x);
	}

}