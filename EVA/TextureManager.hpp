#pragma once

#include <map>

#include "GL/glew.h"

namespace EVA
{

	// A helper class used to load images from file to GL textures
	class TextureManager
	{

	private:
		static std::map<std::string, GLuint> m_Textures;

	public:
		/// <summary>Loads an image from file to a GL texture. Immediately returns if texture already exists</summary>
		/// <param name="path">The path to the image file</param>
		/// <returns>A refference to the GL texture</returns>
		static GLuint GetTexture(const std::string &path);

		static GLuint GetTextureCubemap(const std::string &folderPath, const std::string &fileType);
	};

}