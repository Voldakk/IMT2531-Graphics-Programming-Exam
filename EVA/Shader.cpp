#include "Shader.hpp"
#include <utility>

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "ShaderManager.hpp"

namespace EVA
{

	Shader::Shader(const std::shared_ptr<ShaderPaths>& paths)
	{
		SetPaths(paths);
	}

	void Shader::SetPaths(const std::shared_ptr<ShaderPaths>& paths)
	{
		if (m_ShaderId != -1)
			glDeleteProgram(m_ShaderId);

		m_Paths = paths;

		m_ShaderId = glCreateProgram();

		if (!paths->vertex.empty())
		{
			const auto vertexShader = ShaderManager::LoadAndCompileShader(paths->vertex, GL_VERTEX_SHADER);
			glAttachShader(m_ShaderId, vertexShader);
			glDeleteShader(vertexShader);
		}

		if (!paths->fragment.empty())
		{
			const auto fragmentShader = ShaderManager::LoadAndCompileShader(paths->fragment, GL_FRAGMENT_SHADER);
			glAttachShader(m_ShaderId, fragmentShader);
			glDeleteShader(fragmentShader);
		}

		if (!paths->geometry.empty())
		{
			const auto geometryShader = ShaderManager::LoadAndCompileShader(paths->geometry, GL_GEOMETRY_SHADER);
			glAttachShader(m_ShaderId, geometryShader);
			glDeleteShader(geometryShader);
		}

		glLinkProgram(m_ShaderId);
		glUseProgram(m_ShaderId);
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

	void Shader::SetUniform2Fv(const std::string& name, glm::vec2 value)
	{
		glUniform2fv(GetUniformLocation(name), 1, value_ptr(value));
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
}
