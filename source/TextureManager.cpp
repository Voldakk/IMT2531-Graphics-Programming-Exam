#include "TextureManager.hpp"

#include "SOIL.h"

std::map<std::string, GLuint> TextureManager::textures;

GLuint TextureManager::GetTexture(const std::string& path)
{
	// Return the id if the texture's already loaded
	if (textures.count(path))
		return textures[path];

	// Create texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Load the image
	int width, height, channels;
	const auto image = SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Save and return the id
	textures[path] = texture;
	return texture;
}
