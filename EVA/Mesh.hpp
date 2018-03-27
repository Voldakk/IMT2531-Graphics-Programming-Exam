#pragma once

#include <vector>
#include <memory>
#include <string>

#include "glm/glm/glm.hpp"

namespace EVA
{

	enum PrimitiveType
	{
		Circle, Cone, Cube, CubeInverted, Cylinder, Icosphere, Monkey, MonkeyHigh, Plane, Sphere, Torus
	};

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class Mesh
	{

	private:

		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_FaceIndices;

		unsigned int m_Vao{};
		unsigned int m_Vbo{};
		unsigned int m_Ebo{};
		unsigned int m_Ibo{};

		unsigned int m_InstanceCount;


	public:

		std::string m_Name;
		bool m_IsStatic;

		explicit Mesh(const std::vector<Vertex> &vertices);

		explicit Mesh(const std::vector<Vertex> &vertices, const std::string &name);

		void Create();

		void Draw() const;

		void DrawInstanced() const;

		void SetIbo(const std::vector<glm::mat4> &models);

		bool HasIbo() const;

		static std::shared_ptr<Mesh> Load(const std::string &path);

		static std::vector<std::shared_ptr<Mesh>> LoadMultiple(const std::string &path);

		static std::shared_ptr<Mesh> Primitive(PrimitiveType type);
	};

}