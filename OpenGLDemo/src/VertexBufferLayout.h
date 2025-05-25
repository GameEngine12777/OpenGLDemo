#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	VertexBufferElement(unsigned int t, unsigned int c, unsigned char n)
		: type(t)
		, count(c)
		, normalized(n)
	{}

	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		default:
			ASSERT(false);
			return 0;
		}
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count)
	{
		// static_assert(false);
		ASSERT(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		VertexBufferElement t(GL_FLOAT, count, GL_FALSE);
		m_Elements.emplace_back(GL_FLOAT, count, GL_FALSE);
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.emplace_back(GL_UNSIGNED_INT, count, GL_FALSE);
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<char>(unsigned int count)
	{
		m_Elements.emplace_back(GL_UNSIGNED_BYTE, count, GL_TRUE);
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }

private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
};

