#include "Shader.hpp"

#include <string>

#include "GL/glew.h"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "Shaderload.hpp"

#include "Application.hpp"

#include "Scene.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

unsigned int Shader::activeShader;

Shader::Shader()
{
	id = -1;
}

void Shader::SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material)
{
	
}

StandardShader::StandardShader()
{
	id = ShaderLoad::CreateProgram("../shaders/standard.vert", "../shaders/standard.frag");

	viewID = glGetUniformLocation(id, "view");
	projectionID = glGetUniformLocation(id, "projection");
	modelID = glGetUniformLocation(id, "model");

	materialShininessID = glGetUniformLocation(id, "materialShininess");
	materialSpecularColorID = glGetUniformLocation(id, "materialSpecularColor");

	lightPositionID = glGetUniformLocation(id, "light.position");
	lightIntensitiesID = glGetUniformLocation(id, "light.intensities");

	lightAttenuationID = glGetUniformLocation(id, "light.attenuation");
	lightAmbientCoefficientID = glGetUniformLocation(id, "light.ambientCoefficient");

	cameraPositionID = glGetUniformLocation(id, "cameraPosition");
}

void StandardShader::SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material)
{
	if (id != activeShader)
	{
		activeShader = id;
		glUseProgram(id);

		// Lights
		glUniform3fv(lightPositionID, 1, value_ptr(scene->light.position));
		glUniform3fv(lightIntensitiesID, 1, value_ptr(scene->light.color));

		glUniform1f(lightAttenuationID, scene->light.attenuation);
		glUniform1f(lightAmbientCoefficientID, scene->light.ambientCoefficient);
	}

	// Material
	if (material != nullptr && (material != Material::activeMaterial || id != activeShader))
	{
		Material::activeMaterial = material;

		glUniform1f(materialShininessID, material->materialShininess);
		glUniform3fv(materialSpecularColorID, 1, value_ptr(material->materialSpecularColor));

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
			std::string name = "";

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
	glUniform3fv(cameraPositionID, 1, value_ptr(Application::camera.position));

	// Matrices
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(transform->GetModelMatrix()));
	glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(Application::camera.GetViewMatrix()));
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(Application::GetProjectionMatrix(false)));
}

UnlitTextureShader::UnlitTextureShader()
{
	id = ShaderLoad::CreateProgram("../shaders/unlit_texture.vert", "../shaders/unlit_texture.frag");

	viewID = glGetUniformLocation(id, "view");
	projectionID = glGetUniformLocation(id, "projection");
	modelID = glGetUniformLocation(id, "model");
}

void UnlitTextureShader::SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material)
{
	if (id != activeShader)
	{
		activeShader = id;
		glUseProgram(id);
	}

	// Matrices
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(transform->GetModelMatrix()));
	glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(Application::camera.GetViewMatrix()));
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(Application::GetProjectionMatrix(false)));

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
			std::string name = "";

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
	id = ShaderLoad::CreateProgram("../shaders/skybox.vert", "../shaders/skybox.frag");

	viewID = glGetUniformLocation(id, "view");
	projectionID = glGetUniformLocation(id, "projection");
	modelID = glGetUniformLocation(id, "model");

	textureID = glGetUniformLocation(id, "skytexture");
}

void SkyboxShader::SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material)
{
	if (id != activeShader)
	{
		activeShader = id;
		glUseProgram(id);
	}

	// Matrices
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(transform->GetModelMatrix()));
	glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(Application::camera.GetViewMatrix()));
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(Application::GetProjectionMatrix(false)));

	// Material
	if (material != nullptr && (material != Material::activeMaterial || id != activeShader))
	{
		Material::activeMaterial = material;

		// Activate the texture
		glActiveTexture(GL_TEXTURE0);

		glUniform1i(textureID, 0);

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, material->textures[0].id);
	}
}
