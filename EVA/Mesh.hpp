#pragma once

#include <vector>
#include <memory>
#include <string>

#include "glm/glm.hpp"

#include "EVA/OpenGL.hpp"

namespace EVA
{

	enum PrimitiveType
	{
		Circle, Cone, Cube, CubeInverted, Cylinder, Icosphere, Monkey, MonkeyHigh, Plane, Sphere, Torus
	};

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	class Mesh
	{

		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_FaceIndices;

		unsigned int m_InstanceCount;

		std::unique_ptr<VertexArray> m_Va;
		std::unique_ptr<VertexBuffer> m_Vb;
		std::unique_ptr<IndexBuffer> m_Ib;
		std::unique_ptr<VertexBuffer> m_Mb;


	public:

		bool isStatic;
		std::string name;

		explicit Mesh(std::vector<Vertex> vertices);

		explicit Mesh(std::vector<Vertex> vertices, std::string name);

		void Create();

		void Draw() const;

		void DrawInstanced() const;

		void SetMbo(const std::vector<glm::mat4> &models);

		bool HasMbo() const;

		static std::shared_ptr<Mesh> Load(const std::string &path);

		static std::vector<std::shared_ptr<Mesh>> LoadMultiple(const std::string &path);

		static std::shared_ptr<Mesh> Primitive(PrimitiveType type);

		void CalculateBt();
	};

}