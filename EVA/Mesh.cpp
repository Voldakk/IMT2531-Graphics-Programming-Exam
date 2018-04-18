#include "Mesh.hpp"

#include <utility>

#include "Scene.hpp"

namespace EVA
{

	Mesh::Mesh(std::vector<Vertex> vertices, std::string name) 
		: m_Vertices(std::move(vertices)), m_InstanceCount(0), name(std::move(name))
	{
		//CalculateBt();
		Create();
	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> faceIndices, std::string name)
		: m_Vertices(std::move(vertices)), m_FaceIndices(std::move(faceIndices)) , m_InstanceCount(0), name(std::move(name))
	{
		//CalculateBt();
		Create();
	}

	void Mesh::Create()
	{
		m_Va = std::make_unique<VertexArray>();
		m_Vb = std::make_unique<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));

		if (!m_FaceIndices.empty())
			m_Ib = std::make_unique<IndexBuffer>(&m_FaceIndices[0], m_FaceIndices.size());

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
			glDrawArrays(GL_TRIANGLES, 0, this->m_Vertices.size());

		m_Va->Unbind();
	}

	void Mesh::SetMbo(const std::vector<glm::mat4> &models)
	{
		m_InstanceCount = models.size();

		m_Va->Bind();

		if(m_Mb)
		{
			m_Mb->BufferData(&models[0], models.size() * sizeof(glm::mat4));
		}
		else
		{
			m_Mb = std::make_unique<VertexBuffer>(&models[0], models.size() * sizeof(glm::mat4));

			VertexBufferLayout layout;
			layout.Push<float>(4, 1); // Model matrix
			layout.Push<float>(4, 1); // Model matrix
			layout.Push<float>(4, 1); // Model matrix
			layout.Push<float>(4, 1); // Model matrix

			m_Va->AddBuffer(*m_Mb, layout);
		}

		m_Mb->Unbind();
		m_Va->Unbind();
	}

	void Mesh::DrawInstanced() const
	{
		// Draw
		m_Va->Bind();

		if (!m_FaceIndices.empty())
		{
			m_Ib->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, m_Ib->GetCount(), GL_UNSIGNED_INT, nullptr, m_InstanceCount);
			m_Ib->Unbind();
		}
		else
			glDrawArraysInstanced(GL_TRIANGLES, 0, this->m_Vertices.size(), m_InstanceCount);

		m_Va->Unbind();
	}

	bool Mesh::HasMbo() const
	{
		return m_Mb != nullptr;
	}

	void Mesh::SetStatic(const bool state)
	{
		m_IsStatic = state;

		if (m_IsStatic)
			isDirty = true;
	}
}