#pragma once

#include <vector>

#include "glm/glm/glm.hpp"

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

class Mesh
{

public:

	Mesh();
	Mesh(const char * path);

	void Create();
	void Draw();

protected:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> faceIndices;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	void LoadMesh(const char * path);
};