#pragma once

#include <map>

#include "GL/glew.h"

// A helper class used to load images from file to GL textures
class TextureManager
{
public:
	/// <summary>Loads an image from file to a GL texture. imeadiatly returns if texture already exists</summary>
	/// <param name="path">The path to the image file</param>  
	/// <returns>A refference to the GL texture</returns>  
	static GLuint GetTexture(const std::string& path);

	static GLuint GetTextureCubemap(const std::string& folderPath, const std::string& fileType);

private:
	static std::map<std::string, GLuint> textures;
};

// Different texture types
enum TextureType { Diffuse, Specular, Normal, Height };

// Struct for holding texture info
struct Texture 
{
	unsigned int id;
	TextureType type;
	std::string path;
};