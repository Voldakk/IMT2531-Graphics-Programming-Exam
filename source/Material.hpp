#pragma once

#include <vector>

#include "glm/glm/glm.hpp"

#include "TextureManager.hpp"

class Material
{
public:

	float materialShininess = 1000.0f;
	glm::vec3 materialSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);

	std::vector<Texture> textures;

	Material();
	
	void AddTexture(const TextureType type, const char * path);
	void AddTexture(const TextureType type, unsigned int id);

	static Material * activeMaterial;
};

