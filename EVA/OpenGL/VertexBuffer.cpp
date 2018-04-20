#include "VertexBuffer.hpp"

#include "EVA/OpenGL.hpp"

namespace EVA
{
	VertexBuffer::VertexBuffer(const void* data, const unsigned int size, const unsigned int usage)
		: m_Usage(usage)
	{
		glGenBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, data, m_Usage);
	}

	VertexBuffer::VertexBuffer(const unsigned size, const unsigned int usage)
		: m_Usage(usage)
	{
		glGenBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, m_Usage);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::BufferData(const void* data, const unsigned size) const
	{
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, m_Usage);
	}
}