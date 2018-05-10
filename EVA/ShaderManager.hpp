#pragma once

#include <vector>
#include <memory>
#include <map>

#include <GL/glew.h>

#include "Shader.hpp"
#include "FileSystem.hpp"

namespace EVA
{
	/**
	 * \brief A helper class for loading shaders
	 */
	class ShaderManager
	{
		static std::map<FS::path, std::shared_ptr<Shader>> m_Shaders;

	public:
		
		static const inline FS::path STANDARD_SHADERS_PATH = "./assets/standard assets/shaders/";

		/**
		 * \brief Creates a shader program from the provided shader file
		 * \param path The path to the shader file
		 * \return A std::shared_ptr to the shader, nullptr if no shader was found
		 */
		static std::shared_ptr<Shader> LoadShader(const FS::path& path);

		/**
		 * \brief Saves the shader to a file
		 * \param shader The shader to be saved
		 * \param path The path to the file
		 */
		static void SaveShader(Shader* shader, const FS::path& path);

	private:

		/**
		 * \brief Reads the file and stores the content in the provided buffer
		 * \param path The path to the file
		 * \param buffer The buffer the contents of the file should be read to
		 */
		static void ReadShaderSource(const FS::path& path, std::vector<char> &buffer);

		/**
		 * \brief Loads and comples a shader for use in a program
		 * \param path The path to the file
		 * \param shaderType The type of shader to be compiled
		 * \return A refference to the complied shader
		 */
		static unsigned int LoadAndCompileShader(const FS::path& path, GLenum shaderType);
	};

}
