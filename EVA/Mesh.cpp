#include "Mesh.hpp"

#include <utility>

#include "Scene.hpp"
#include "OBJLoader.hpp"

namespace EVA
{

	Mesh::Mesh(std::vector<Vertex> vertices) :
			m_Vertices(std::move(vertices)), m_InstanceCount(0), isStatic(false)
	{
		CalculateBt();
		Create();
	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::string name) :
			m_Vertices(std::move(vertices)), m_InstanceCount(0), isStatic(false), name(std::move(name))
	{
		CalculateBt();
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

	std::shared_ptr<Mesh> Mesh::Load(const std::string &path)
	{
		return OBJLoader::LoadSingle(path.c_str());
	}

	std::vector<std::shared_ptr<Mesh>> Mesh::LoadMultiple(const std::string &path)
	{
		return OBJLoader::LoadMultiple(path.c_str());
	}

	std::shared_ptr<Mesh> Mesh::Primitive(const PrimitiveType type)
	{
		std::string path = "./assets/models/primitives/";

		switch (type)
		{
			case Circle:
				path += "circle.obj";
				break;
			case Cone:
				path += "cone.obj";
				break;
			case Cube:
				path += "cube.obj";
				break;
			case CubeInverted:
				path += "cube_inverted.obj";
				break;
			case Cylinder:
				path += "cylinder.obj";
				break;
			case Monkey:
				path += "monkey_low.obj";
				break;
			case MonkeyHigh:
				path += "monkey_high.obj";
				break;
			case Icosphere:
				path += "icosphere.obj";
				break;
			case Plane:
				path += "plane.obj";
				break;
			case Sphere:
				path += "sphere.obj";
				break;
			case Torus:
				path += "torus.obj";
				break;
				// Default to cube
			default:
				path += "cube.obj";
				break;
		}

		return Load(path);
	}

	void Mesh::CalculateBt()
	{
		glm::vec3 tangent;
		glm::vec3 bitangent;

		for (unsigned int i = 0; i < m_Vertices.size(); i += 3)
		{
			const auto edge1 = m_Vertices[i+1].position - m_Vertices[i + 0].position;
			const auto edge2 = m_Vertices[i + 2].position - m_Vertices[i + 0].position;
			const auto deltaUv1 = m_Vertices[i + 1].texCoords - m_Vertices[i + 0].texCoords;
			const auto deltaUv2 = m_Vertices[i + 2].texCoords - m_Vertices[i + 0].texCoords;

			const auto f = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv2.x * deltaUv1.y);

			tangent.x = f * (deltaUv2.y * edge1.x - deltaUv1.y * edge2.x);
			tangent.y = f * (deltaUv2.y * edge1.y - deltaUv1.y * edge2.y);
			tangent.z = f * (deltaUv2.y * edge1.z - deltaUv1.y * edge2.z);
			tangent = glm::normalize(tangent);

			bitangent.x = f * (-deltaUv2.x * edge1.x + deltaUv1.x * edge2.x);
			bitangent.y = f * (-deltaUv2.x * edge1.y + deltaUv1.x * edge2.y);
			bitangent.z = f * (-deltaUv2.x * edge1.z + deltaUv1.x * edge2.z);
			bitangent = glm::normalize(bitangent);

			m_Vertices[i + 0].tangent = tangent;
			m_Vertices[i + 1].tangent = tangent;
			m_Vertices[i + 2].tangent = tangent;

			m_Vertices[i + 0].bitangent = bitangent;
			m_Vertices[i + 1].bitangent = bitangent;
			m_Vertices[i + 2].bitangent = bitangent;
		}
	}

}