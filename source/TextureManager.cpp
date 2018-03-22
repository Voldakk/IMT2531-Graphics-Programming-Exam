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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, image);
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

GLuint TextureManager::GetTextureCubemap(const std::string& folderPath, const std::string& fileType)
{
	// Return the id if the texture's already loaded
	if (textures.count(folderPath + fileType))
		return textures[folderPath + fileType];

	// Create texture
	GLuint texture;
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Load files
	std::string sideNames[] = { "xpos", "xneg", "ypos", "yneg", "zpos", "zneg" };

	unsigned int sideIds[] =
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (size_t i = 0; i < 6; i++)
	{
		auto fullPath = folderPath + sideNames[i];
		fullPath.append(fileType);

		int width, height, channels;

		const auto image = SOIL_load_image(fullPath.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);

		glTexImage2D(sideIds[i], 0, GL_RGBA, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, image);

		SOIL_free_image_data(image);
	}

	// Save and return the id
	textures[folderPath + fileType] = texture;
	return texture;
}
