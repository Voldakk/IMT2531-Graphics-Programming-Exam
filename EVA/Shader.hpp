#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace EVA
{

	class Shader
	{
	private:

		unsigned int m_ShaderId;
		std::unordered_map<std::string, int> m_UniformLocationMap;

	public:

		Shader(std::string vertexPath, std::string fragmentPath);

		~Shader();

		void Bind();

		void Unbind();

		void SetUniform1i(std::string name, int value);

		void SetUniform1f(std::string name, float value);

		void SetUniform3fv(std::string name, glm::vec3 value);

		void SetUniformMatrix4fv(std::string name, glm::mat4 value);

	private:

		int GetUniformLocation(std::string name);

	};

	class StandardShader : public Shader
	{
	public:
		StandardShader();
	};

	class StandardInstancedShader : public Shader
	{
	public:
		StandardInstancedShader();
	};

	class SkyboxShader : public Shader
	{
	public:
		SkyboxShader();
	};

}
