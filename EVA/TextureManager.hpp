#pragma once

#include <map>

namespace EVA
{

	/**
	 * \brief A helper class used to load images from file to OpenGL textures
	 */
	class TextureManager
	{

		static std::map<std::string, unsigned int> m_Textures;

	public:
		/// <summary>Loads an image from file to a GL texture. Immediately returns if texture already exists</summary>
		/// <param name="path">The path to the image file</param>
		/// <returns>A refference to the GL texture</returns>
		static unsigned int GetTexture(const std::string &path);

		static unsigned int GetTextureCubemap(const std::string &folderPath, const std::string &fileType);
	};

}