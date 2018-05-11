#pragma once

#include <vector>
#include <memory>
#include <string>

#include "glm/glm.hpp"

#include "EVA/OpenGL.hpp"

#include "FileSystem.hpp"

namespace EVA
{
	struct InstancedMeshData;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	struct ColorVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	/**
	 * \brief A mesh
	 */
	class Mesh
	{
		std::unique_ptr<VertexArray> m_Va;
		std::unique_ptr<VertexBuffer> m_Vb;
		std::unique_ptr<IndexBuffer> m_Ib;

		unsigned int m_VertexCount;
		bool m_HasFaceIndices;
		
	public:

		std::string name;

		FS::path path;
		unsigned int index;

		explicit Mesh(const std::vector<Vertex>& vertices, std::string name = "");

		explicit Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& faceIndices, std::string name = "");

		explicit Mesh(const std::vector<ColorVertex>& vertices, const std::vector<unsigned int>& faceIndices, std::string name = "");

		void Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& faceIndices);

		void Create(const std::vector<ColorVertex>& vertices, const std::vector<unsigned int>& faceIndices);

		void Draw() const;

		void DrawInstanced(const InstancedMeshData* instancedMeshData) const;
	};

}