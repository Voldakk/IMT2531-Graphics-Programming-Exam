#pragma once

#include <vector>

#include "glm/glm/glm.hpp"

#include "TextureManager.hpp"

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

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale = glm::vec3(1.0f);;

	Mesh(const char * path);
	~Mesh();

	void Create();
	void Render();

	void AddTexture(const TextureType type, const char * path);

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

