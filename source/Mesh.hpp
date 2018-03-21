#pragma once

#include <vector>

#include "glm/glm/glm.hpp"

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

public:

	Mesh();
	explicit Mesh(const char * path);

	void Create();
	void Draw() const;

protected:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> faceIndices;

	unsigned int vao{};
	unsigned int vbo{};
	unsigned int ebo{};

	void LoadMesh(const char * path);
};