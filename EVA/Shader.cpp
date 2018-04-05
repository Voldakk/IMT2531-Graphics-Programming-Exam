#include "Shader.hpp"

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"

#include "Shaderload.hpp"

namespace EVA
{

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		m_ShaderId = ShaderLoad::CreateProgram(vertexPath.c_str(), fragmentPath.c_str());
	}

	Shader::Shader(const std::string& vertexPath, const std::string& geomertyPath, const std::string& fragmentPath)
	{
		m_ShaderId = ShaderLoad::CreateProgram(vertexPath.c_str(), geomertyPath.c_str(), fragmentPath.c_str());
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ShaderId);
	}

	int Shader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationMap.find(name) != m_UniformLocationMap.end())
			return m_UniformLocationMap[name];

		const auto location = glGetUniformLocation(m_ShaderId, name.c_str());
		m_UniformLocationMap[name] = location;
		return location;
	}

	void Shader::SetUniform1I(const std::string& name, const int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetUniform1F(const std::string& name, const float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetUniform3Fv(const std::string& name, glm::vec3 value)
	{
		glUniform3fv(GetUniformLocation(name), 1, value_ptr(value));
	}

	void Shader::SetUniform4Fv(const std::string& name, glm::vec4 value)
	{
		glUniform4fv(GetUniformLocation(name), 1, value_ptr(value));
	}

	void Shader::SetUniformMatrix4Fv(const std::string& name, glm::mat4 value)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value_ptr(value));
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ShaderId);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

    StandardShader::StandardShader() : Shader("standard.vert", "standard.frag")
	{

	}

	StandardInstancedShader::StandardInstancedShader() : Shader("standard_instanced.vert", "standard_instanced.frag")
	{

	}

	SkyboxShader::SkyboxShader() : Shader("skybox.vert", "skybox.frag")
	{

	}

}