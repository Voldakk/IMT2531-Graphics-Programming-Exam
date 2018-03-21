#include "Shader.hpp"

#include <string>

#include "GL/glew.h"
#include "glm/glm/gtc/type_ptr.hpp"

#include "Shaderload.hpp"

#include "Application.hpp"

#include "Scene.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

unsigned int Shader::activeShader;

Shader::Shader() = default;

void Shader::SetUniforms(Scene* scene, Transform* transform, Mesh* mesh, Material* material)
{

}

StandardShader::StandardShader()
{
	id = ShaderLoad::CreateProgram("standard.vert", "standard.frag");

	viewId = glGetUniformLocation(id, "view");
	projectionId = glGetUniformLocation(id, "projection");
	modelId = glGetUniformLocation(id, "model");

	materialShininessId = glGetUniformLocation(id, "materialShininess");
	materialSpecularColorId = glGetUniformLocation(id, "materialSpecularColor");

	lightPositionId = glGetUniformLocation(id, "light.position");
	lightIntensitiesId = glGetUniformLocation(id, "light.intensities");

	lightAttenuationId = glGetUniformLocation(id, "light.attenuation");
	lightAmbientCoefficientId = glGetUniformLocation(id, "light.ambientCoefficient");

	cameraPositionId = glGetUniformLocation(id, "cameraPosition");
}

void StandardShader::SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material)
{
	if (id != activeShader)
	{
		activeShader = id;
		glUseProgram(id);

		// Lights
		glUniform3fv(lightPositionId, 1, value_ptr(scene->light.position));
		glUniform3fv(lightIntensitiesId, 1, value_ptr(scene->light.color));

		glUniform1f(lightAttenuationId, scene->light.attenuation);
		glUniform1f(lightAmbientCoefficientId, scene->light.ambientCoefficient);
	}

	// Material
	if (material != nullptr && (material != Material::activeMaterial || id != activeShader))
	{
		Material::activeMaterial = material;

		glUniform1f(materialShininessId, material->materialShininess);
		glUniform3fv(materialSpecularColorId, 1, value_ptr(material->materialSpecularColor));

		// Textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < material->textures.size(); i++)
		{
			// Activate the texture
			glActiveTexture(GL_TEXTURE0 + i);

			// Find the name based on the texture type
			std::string name;

			switch (material->textures[i].type)
			{
			case TextureType::Diffuse:
				name = "texture_diffuse" + std::to_string(diffuseNr++);
				break;
			case TextureType::Specular:
				name = "texture_specular" + std::to_string(specularNr++);
				break;
			case TextureType::Normal:
				name = "texture_normal" + std::to_string(normalNr++);
				break;
			case TextureType::Height:
				name = "texture_height" + std::to_string(heightNr++);
				break;
			default:
				break;
			}

			// Set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(id, name.c_str()), i);

			// Bind the texture
			glBindTexture(GL_TEXTURE_2D, material->textures[i].id);
		}
	}

	// Camera
	glUniform3fv(cameraPositionId, 1, value_ptr(Application::camera.position));

	// Matrices
	glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(transform->GetModelMatrix()));
	glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(Application::camera.GetViewMatrix()));
	glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(Application::GetPerspectiveMatrix()));
}

UnlitTextureShader::UnlitTextureShader()
{
	id = ShaderLoad::CreateProgram("unlit_texture.vert", "unlit_texture.frag");

	viewId = glGetUniformLocation(id, "view");
	projectionId = glGetUniformLocation(id, "projection");
	modelId = glGetUniformLocation(id, "model");
}

void UnlitTextureShader::SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material)
{
	if (id != activeShader)
	{
		activeShader = id;
		glUseProgram(id);
	}

	// Matrices
	glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(transform->GetModelMatrix()));
	glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(Application::camera.GetViewMatrix()));
	glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(Application::GetPerspectiveMatrix()));

	// Material
	if (material != nullptr && (material != Material::activeMaterial || id != activeShader))
	{
		Material::activeMaterial = material;

		// Textures
		unsigned int diffuseNr = 1;

		for (unsigned int i = 0; i < material->textures.size(); i++)
		{
			// Activate the texture
			glActiveTexture(GL_TEXTURE0 + i);

			// Find the name based on the texture type
			std::string name;

			switch (material->textures[i].type)
			{
			case TextureType::Diffuse:
				name = "texture_diffuse" + std::to_string(diffuseNr++);
				break;
			default:
				break;
			}

			// Set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(id, name.c_str()), i);

			// Bind the texture
			glBindTexture(GL_TEXTURE_2D, material->textures[i].id);
		}
	}
}

SkyboxShader::SkyboxShader()
{
	id = ShaderLoad::CreateProgram("skybox.vert", "skybox.frag");

	viewId = glGetUniformLocation(id, "view");
	projectionId = glGetUniformLocation(id, "projection");
	modelId = glGetUniformLocation(id, "model");

	textureId = glGetUniformLocation(id, "skytexture");
}

void SkyboxShader::SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material)
{
	if (id != activeShader)
	{
		activeShader = id;
		glUseProgram(id);
	}

	// Matrices
	glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(transform->GetModelMatrix()));
	glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(Application::camera.GetViewMatrix()));
	glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(Application::GetPerspectiveMatrix()));

	// Material
	if (material != nullptr && (material != Material::activeMaterial || id != activeShader))
	{
		Material::activeMaterial = material;

		// Activate the texture
		glActiveTexture(GL_TEXTURE0);

		glUniform1i(textureId, 0);

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, material->textures[0].id);
	}
}
