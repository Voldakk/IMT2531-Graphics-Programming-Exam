#pragma once

#include <vector>

#include "GL/glew.h"

namespace EVA
{

	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;
		unsigned int divisor;

		static unsigned int VertexBufferElement::GetSizeOfType(const unsigned type)
		{
			switch (type)
			{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			default: return 0;
			}
		}
	};

	class VertexBufferLayout
	{

		std::vector<VertexBufferElement> m_Elements;

		unsigned int m_Stride = 0;

	public:

		VertexBufferLayout() = default;
		~VertexBufferLayout() = default;

		template<typename T>
		void Push(const unsigned int count, const unsigned int divisor = 0){};

		template<>
		void Push<float>(const unsigned int count, const unsigned int divisor)
		{
			m_Elements.push_back({ GL_FLOAT, count, GL_FALSE, divisor });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
		}

		template<>
		void Push<unsigned int>(const unsigned int count, const unsigned int divisor)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, divisor });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
		}

		template<>
		void Push<unsigned char>(const unsigned int count, const unsigned int divisor)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE, divisor });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
		}

		inline unsigned int GetStride() const { return m_Stride; }

		inline std::vector<VertexBufferElement> GetElements() const { return m_Elements; }

	};
}