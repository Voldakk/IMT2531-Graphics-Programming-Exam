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

public:

	bool isStatic = false;
	int instanceCount = 0;

	explicit Mesh(const std::vector<Vertex>& vertices);

	void Create();
	void Draw() const;
	void SetIbo(const std::vector<glm::mat4>& models);
	void DrawInstanced(int count) const;

	bool HasIbo() const;

	static std::shared_ptr<Mesh> Load(const std::string& path);
	static std::vector<std::shared_ptr<Mesh>> LoadMultiple(const std::string& path);
	static std::shared_ptr<Mesh> Primitive(PrimitiveType type);

protected:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> faceIndices;

	unsigned int vao{};
	unsigned int vbo{};
	unsigned int ebo{};
	unsigned int ibo{};
};