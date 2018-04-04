#pragma once

#include <vector>
#include <memory>
#include <string>

#include "glm/glm.hpp"

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

		unsigned int m_Vao{};
		unsigned int m_Vbo{};
		unsigned int m_Ebo{};
		unsigned int m_Ibo{};

		unsigned int m_InstanceCount;


	public:

		std::string name;
		bool isStatic;

		explicit Mesh(std::vector<Vertex> vertices);

		explicit Mesh(std::vector<Vertex> vertices, std::string name);

		void Create();

		void Draw() const;

		void DrawInstanced() const;

		void SetIbo(const std::vector<glm::mat4> &models);

		bool HasIbo() const;

		static std::shared_ptr<Mesh> Load(const std::string &path);

		static std::vector<std::shared_ptr<Mesh>> LoadMultiple(const std::string &path);

		static std::shared_ptr<Mesh> Primitive(PrimitiveType type);

		void CalculateBt();
	};

}