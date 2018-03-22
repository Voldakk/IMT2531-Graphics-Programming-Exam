#pragma once

#include <vector>
#include <memory>

#include "glm/glm/glm.hpp"

#include "Shader.hpp"
#include "TextureManager.hpp"

class Material
{
public:

	float materialShininess = 1000.0f;
	glm::vec3 materialSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);

	std::shared_ptr<Shader> shader;
	std::vector<Texture> textures;

	Material();
	
	void AddTexture(TextureType type, const char * path);
	void AddTexture(TextureType type, unsigned int id);

	static Material * activeMaterial;
};

