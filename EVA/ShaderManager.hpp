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

		static std::map<std::string, std::shared_ptr<Shader>> m_Shaders;

	public:

		/**
		 * \brief Creates a shader program from the provided shader files
		 * \param name A unique identifier for the created shader. Is there already exists a shader with the same name that shader will be returned instead
		 * \param vertPath The path to the vertex shader
		 * \param fragPath The path to the fragment shader
		 * \return A std::shared_ptr to the shader
		 */
		static std::shared_ptr<Shader> CreateOrGetShader(const std::string& name, const FS::path& vertPath, const FS::path& fragPath);

		/**
		 * \brief Creates a shader program from the provided shader files
		 * \param name A unique identifier for the created shader. Is there already exists a shader with the same name that shader will be returned instead
		 * \param vertPath The path to the vertex shader
		 * \param fragPath The path to the fragment shader
		 * \param geomPath The path to the geometry shader
		 * \return A std::shared_ptr to the shader
		 */
		static std::shared_ptr<Shader> CreateOrGetShader(const std::string& name, const FS::path& vertPath, const FS::path& fragPath, const FS::path& geomPath);

		/**
		 * \brief Gets the shader with the specified name
		 * \param name A unique identifier for the created shader. Is there already exists a shader with the same name that shader will be returned instead
		 * \return A std::shared_ptr to the shader
		 */
		static std::shared_ptr<Shader> GetShader(const std::string& name);

	private:

		/**
		 * \brief Creates a shader program from the provided shader files
		 * \param pathVertShader The path to the vertex shader
		 * \param pathFragShader The path to the fragment shader
		 * \return A reference to the shader program
		 */
		static unsigned int CreateProgram(const FS::path& pathVertShader, const FS::path& pathFragShader);

		/**
		* \brief Creates a shader program from the provided shader files
		* \param pathVertShader The path to the vertex shader
		* \param pathFragShader The path to the fragment shader
		* \param pathGeomShader The path to the geometry shader
		* \return A reference to the shader program
		*/
		static unsigned int CreateProgram(const FS::path& pathVertShader, const FS::path& pathFragShader, const FS::path& pathGeomShader);

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
