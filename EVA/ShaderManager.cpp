#include "ShaderManager.hpp"

#include <vector>
#include <fstream>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace EVA
{

	const FS::path SHADER_PATH = "./assets/standard assets/shaders/";

	std::map<std::string, std::shared_ptr<Shader>> ShaderManager::m_Shaders;

	std::shared_ptr<Shader> ShaderManager::CreateOrGetShader(const std::string& name, const FS::path& vertPath, const FS::path& fragPath)
	{
		if (m_Shaders.count(name))
			return m_Shaders[name];

		auto shaderId = CreateProgram(vertPath, fragPath);
		const auto shader = std::make_shared<Shader>(shaderId);
		
		m_Shaders[name] = shader;

		return shader;
	}

	std::shared_ptr<Shader> ShaderManager::CreateOrGetShader(const std::string& name, const FS::path& vertPath, const FS::path& fragPath, const FS::path& geomPath)
	{
		if (m_Shaders.count(name))
			return m_Shaders[name];

		auto shaderId = CreateProgram(vertPath, fragPath, geomPath);
		const auto shader = std::make_shared<Shader>(shaderId);

		m_Shaders[name] = shader;

		return shader;
	}

	std::shared_ptr<Shader> ShaderManager::GetShader(const std::string& name)
	{
		if (m_Shaders.count(name))
			return m_Shaders[name];

		return nullptr;
	}

	unsigned int ShaderManager::CreateProgram(const FS::path& pathVertShader, const FS::path& pathFragShader)
	{
		// Load and compile the shaders
		const auto vertexShader = LoadAndCompileShader(SHADER_PATH / pathVertShader, GL_VERTEX_SHADER);
		const auto fragmentShader = LoadAndCompileShader(SHADER_PATH / pathFragShader, GL_FRAGMENT_SHADER);

		// Create a program object and attach the two shaders we have compiled
		const auto shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		// Now that the shaders has been attached, we no longer need these two separate objects and should delete them.
		// The attachment to the shader program will keep them alive, as long as we keep the shaderProgram.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Link the different shaders that are bound to this program.
		glLinkProgram(shaderProgram);
		glUseProgram(shaderProgram);

		return shaderProgram;
	}

	unsigned int ShaderManager::CreateProgram(const FS::path& pathVertShader, const FS::path& pathFragShader, const FS::path& pathGeomShader)
	{
		const auto vertexShader =   LoadAndCompileShader(SHADER_PATH / pathVertShader, GL_VERTEX_SHADER);
		const auto fragmentShader = LoadAndCompileShader(SHADER_PATH / pathFragShader, GL_FRAGMENT_SHADER);
		const auto geometryShader = LoadAndCompileShader(SHADER_PATH / pathGeomShader, GL_GEOMETRY_SHADER);

		const auto shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glAttachShader(shaderProgram, geometryShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(geometryShader);

		glLinkProgram(shaderProgram);
		glUseProgram(shaderProgram);

		return shaderProgram;
	}

	void ShaderManager::ReadShaderSource(const FS::path& path, std::vector<char> &buffer)
	{
		std::ifstream in;
		in.open(FileSystem::ToString(path).c_str(), std::ios::binary);

		if (!in.fail())
		{
			// Get the number of bytes stored in this file
			in.seekg(0, std::ios::end);
			const auto length = (size_t) in.tellg();

			// Go to start of the file
			in.seekg(0, std::ios::beg);

			// Read the content of the file in a buffer
			buffer.resize(length + 1);
			in.read(&buffer[0], length);
			in.close();

			// Add a valid C - string end
			buffer[length] = '\0';
		} 
		else
		{
			std::cerr << "ShaderManager::ReadShaderSource - Unable to open file: " << FileSystem::ToString(path).c_str() << " I'm out!" << std::endl;
			exit(-1);
		}
	}

	unsigned int ShaderManager::LoadAndCompileShader(const FS::path& path, const GLenum shaderType)
	{
		std::cout << "ShaderManager::LoadAndCompileShader - " << FileSystem::ToString(path).c_str() << "\n";

		// Load a shader from an external file
		std::vector<char> buffer;
		ReadShaderSource(path, buffer);
		const char *src = &buffer[0];

		// Create shaders
		const auto shader = glCreateShader(shaderType);

		//Attach the shader source code to the shader object
		glShaderSource(shader, 1, &src, nullptr);

		// Compile the shader
		glCompileShader(shader);

		// Comile the shader, translates into internal representation and checks for errors.
		int compileOk;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileOk);
		if (!compileOk)
		{
			char infolog[1024];;
			glGetShaderInfoLog(shader, 1024, nullptr, infolog);
			std::cout << "ShaderManager::LoadAndCompileShader - The program failed to compile with the error:" << std::endl << infolog << std::endl;
			glfwTerminate();
			getchar();
			exit(-1);
		}
		return shader;
	}
}
