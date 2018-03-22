#pragma once

#include <vector>

#include "glm/glm/glm.hpp"

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

	Mesh();
	explicit Mesh(const char * path);

	void Create();
	void Draw() const;
	void SetIbo(const std::vector<glm::mat4>& models);
	void DrawInstanced(int count) const;

	bool HasIbo();

protected:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> faceIndices;

	unsigned int vao{};
	unsigned int vbo{};
	unsigned int ebo{};
	unsigned int ibo{};

	void LoadMesh(const char * path);
};