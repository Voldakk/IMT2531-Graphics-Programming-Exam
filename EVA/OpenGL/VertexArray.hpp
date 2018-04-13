#pragma once

namespace EVA
{
	class VertexBuffer;
	class VertexBufferLayout;

	class VertexArray
	{
		unsigned int m_RendererId;
		unsigned int m_Attributes;

	public:

		VertexArray();
		~VertexArray();

		void Bind() const;
		static void Unbind();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	};
}