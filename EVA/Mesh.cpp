#include "Mesh.hpp"

#include <utility>

#include "Scene.hpp"

namespace EVA
{

	Mesh::Mesh(const std::vector<Vertex>& vertices, std::string name) 
		: name(std::move(name)), index(0)
	{
		m_VertexCount = vertices.size();
		m_HasFaceIndices = false;

		Create(vertices, std::vector<unsigned int>());
	}

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& faceIndices, std::string name)
		: name(std::move(name)), index(0)
	{
		m_VertexCount = vertices.size();
		m_HasFaceIndices = true;

		Create(vertices, faceIndices);
	}

	Mesh::Mesh(const std::vector<ColorVertex>& vertices, const std::vector<unsigned>& faceIndices, std::string name)
		: name(std::move(name)), index(0)
	{
		m_VertexCount = vertices.size();
		m_HasFaceIndices = true;

		Create(vertices, faceIndices);
	}

	void Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& faceIndices)
	{
		m_Va = std::make_unique<VertexArray>();
		m_Vb = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));

		if (!faceIndices.empty())
			m_Ib = std::make_unique<IndexBuffer>(&faceIndices[0], faceIndices.size());

		VertexBufferLayout layout;
		layout.Push<float>(3); // Position
		layout.Push<float>(3); // Normal
		layout.Push<float>(2); // TexCoords
		layout.Push<float>(3); // Tangent
		layout.Push<float>(3); // Bitangent

		m_Va->AddBuffer(*m_Vb, layout);

		m_Va->Unbind();
		m_Vb->Unbind();
		if (m_Ib)
			m_Ib->Unbind();
	}

	void Mesh::Create(const std::vector<ColorVertex>& vertices, const std::vector<unsigned>& faceIndices)
	{
		m_Va = std::make_unique<VertexArray>();
		m_Vb = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(ColorVertex));

		if (!faceIndices.empty())
			m_Ib = std::make_unique<IndexBuffer>(&faceIndices[0], faceIndices.size());

		VertexBufferLayout layout;
		layout.Push<float>(3); // Position
		layout.Push<float>(3); // Normal
		layout.Push<float>(3); // Color
		layout.Push<float>(3); // Tangent
		layout.Push<float>(3); // Bitangent

		m_Va->AddBuffer(*m_Vb, layout);

		m_Va->Unbind();
		m_Vb->Unbind();
		if (m_Ib)
			m_Ib->Unbind();
	}

	void Mesh::Draw() const
	{
		// Draw
		m_Va->Bind();

		if(m_Ib) 
		{
			m_Ib->Bind();
			glDrawElements(GL_TRIANGLES, m_Ib->GetCount(), GL_UNSIGNED_INT, nullptr); 
			m_Ib->Unbind();
		}
		else
			glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);

		m_Va->Unbind();
	}

	void Mesh::DrawInstanced(const InstancedMeshData* instancedMeshData) const
	{
		// Draw
		m_Va->Bind();

		VertexBufferLayout layout;
		layout.Push<float>(4, 1); // Model matrix
		layout.Push<float>(4, 1); // Model matrix
		layout.Push<float>(4, 1); // Model matrix
		layout.Push<float>(4, 1); // Model matrix

		m_Va->AddTempBuffer(*instancedMeshData->matrixBuffer, layout);

		if (m_HasFaceIndices)
		{
			m_Ib->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, m_Ib->GetCount(), GL_UNSIGNED_INT, nullptr, instancedMeshData->instanceCount);
			m_Ib->Unbind();
		}
		else
			glDrawArraysInstanced(GL_TRIANGLES, 0, m_VertexCount, instancedMeshData->instanceCount);

		m_Va->Unbind();
	}
}