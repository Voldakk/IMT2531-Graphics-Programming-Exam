#pragma once

namespace EVA
{
	class VertexBuffer
	{

		unsigned int m_RendererId{};

	public:

		VertexBuffer(const void* data, const unsigned int size);
		~VertexBuffer();

		void Bind() const;
		static void Unbind();

		void BufferData(const void* data, const unsigned int size);
	};
}