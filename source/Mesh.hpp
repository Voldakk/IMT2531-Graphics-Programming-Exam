#pragma once

#include <vector>

#include "glm/glm/glm.hpp"

#include "TextureManager.hpp"

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

class Mesh
{

public:

	Mesh(const char * path);
	~Mesh();

	void Create();
	void Render();

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

protected:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> faceIndices;
	std::vector<Texture> textures;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	unsigned int shader;

	unsigned int viewID;
	unsigned int projectionID;
	unsigned int modelID;

};

