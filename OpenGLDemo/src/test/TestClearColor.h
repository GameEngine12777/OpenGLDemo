#pragma once

#include "Test.h"
#include "glm/glm.hpp"

namespace Test {

	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		TestClearColor(glm::vec4 clearColor);
		~TestClearColor();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	private:
		glm::vec4 m_ClearColor;
	};

}

