#include "VertexBufferLayout.hpp"

template<>
void EVA::VertexBufferLayout::Push<unsigned char>(const unsigned int count, const unsigned int divisor)
{
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE, divisor });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}

template<>
void EVA::VertexBufferLayout::Push<unsigned int>(const unsigned int count, const unsigned int divisor)
{
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, divisor });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void EVA::VertexBufferLayout::Push<float>(const unsigned int count, const unsigned int divisor)
{
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE, divisor });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}