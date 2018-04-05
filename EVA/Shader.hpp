#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace EVA
{

	class Shader
	{

		unsigned int m_ShaderId;
		std::unordered_map<std::string, int> m_UniformLocationMap;

	public:

		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		Shader(const std::string& vertexPath, const std::string& geomertyPath, const std::string& fragmentPath);

		~Shader();

		void Bind() const;

		static void Unbind();

		void SetUniform1I(const std::string& name, int value);

		void SetUniform1F(const std::string& name, float value);

		void SetUniform3Fv(const std::string& name, glm::vec3 value);

		void SetUniform4Fv(const std::string& name, glm::vec4 value);

		void SetUniformMatrix4Fv(const std::string& name, glm::mat4 value);

	private:

		int GetUniformLocation(const std::string& name);

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
