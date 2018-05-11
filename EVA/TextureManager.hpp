#pragma once

#include <map>

#include "FileSystem.hpp"
#include "stb_image.h"

namespace EVA
{
	// Struct for holding texture info
	struct Texture
	{
		// Different texture types
		enum Type
		{
			Diffuse, Specular, Normal, Emission, Height
		};

		unsigned int id{};
		Type type;
		FS::path path;
	};

	struct RawTexture
	{
		unsigned char* data;
		unsigned int width;
		unsigned int height;
		unsigned int channels;

		RawTexture(unsigned char* data, const unsigned int width, const unsigned int height, const unsigned int channels)
		{
			this->data = data;
			this->width = width;
			this->height = height;
			this->channels = channels;
		}

		~RawTexture()
		{
			stbi_image_free(data);
		}
	};

	/**
	 * \brief A helper class used to load images from file to OpenGL textures
	 */
	class TextureManager
	{

		static std::map<FS::path, std::shared_ptr<Texture>> m_Textures;

	public:

		/**
		 * \brief Loads an texture from file.
		 * \param path The path to the texture file
		 * \return A pointer to the texture, or nullptr if no texture is found
		 */
		static std::shared_ptr<Texture> LoadTexture(const FS::path& path);

		/**
		 * \brief Loads a series of textures from a folder to a cubemap texture.
		 * \param folderPath The path to the folder
		 * \param fileType The texture file type
		 * \return A pointer to the texture, or nullptr if no texture is found
		 */
		static std::shared_ptr<Texture> LoadTextureCubemap(const FS::path& folderPath, const std::string &fileType);

		static std::shared_ptr<RawTexture> LoadRaw(const FS::path& path);
	};

}
