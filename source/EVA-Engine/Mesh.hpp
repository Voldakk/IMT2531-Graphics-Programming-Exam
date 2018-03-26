#pragma once

#include <vector>
#include <memory>
#include <string>

#include "glm/glm/glm.hpp"

enum PrimitiveType { Circle, Cone, Cube, CubeInverted, Cylinder, Icosphere, Monkey, MonkeyHigh, Plane, Sphere, Torus };

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh
{

protected:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> faceIndices;

	unsigned int vao{};
	unsigned int vbo{};
	unsigned int ebo{};
	unsigned int ibo{};

	unsigned int instanceCount;

public:

	std::string name;

	bool isStatic;

	explicit Mesh(const std::vector<Vertex>& vertices);
	explicit Mesh(const std::vector<Vertex>& vertices, const std::string& name);

	void Create();

	void Draw() const;
	void DrawInstanced() const;

	void SetIbo(const std::vector<glm::mat4>& models);
	bool HasIbo() const;

	static std::shared_ptr<Mesh> Load(const std::string& path);
	static std::vector<std::shared_ptr<Mesh>> LoadMultiple(const std::string& path);
	static std::shared_ptr<Mesh> Primitive(PrimitiveType type);
};