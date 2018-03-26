#include "Shader.hpp"

#include "GL/glew.h"
#include "glm/glm/gtc/type_ptr.hpp"

#include "Shaderload.hpp"

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
	id = ShaderLoad::CreateProgram(vertexPath.c_str(), fragmentPath.c_str());
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

int Shader::GetUniformLocation(std::string name)
{
	if(uniformLocationMap.find(name) != uniformLocationMap.end())
		return uniformLocationMap[name];

	int location = glGetUniformLocation(id, name.c_str());
	uniformLocationMap[name] = location;
	return location;
}

void Shader::SetUniform1i(std::string name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(std::string name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform3fv(std::string name, glm::vec3 value)
{
	glUniform3fv(GetUniformLocation(name), 1, value_ptr(value));
}

void Shader::SetUniformMatrix4fv(std::string name, glm::mat4 value)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value_ptr(value));
}

void Shader::Bind()
{
	glUseProgram(id);
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
