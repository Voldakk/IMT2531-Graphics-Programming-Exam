#include "Shader.hpp"

#include <string>

#include "GL/glew.h"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "Shaderload.hpp"

#include "Application.hpp"

#include "Scene.hpp"
#include "GameObject.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

Shader::Shader()
{
	shader = ShaderLoad::CreateProgram("../shaders/standard.vert", "../shaders/standard.frag");

	viewID = glGetUniformLocation(shader, "view");
	projectionID = glGetUniformLocation(shader, "projection");
	modelID = glGetUniformLocation(shader, "model");

	materialShininessID = glGetUniformLocation(shader, "materialShininess");
	materialSpecularColorID = glGetUniformLocation(shader, "materialSpecularColor");

	lightPositionID = glGetUniformLocation(shader, "light.position");
	lightIntensitiesID = glGetUniformLocation(shader, "light.intensities");

	lightAttenuationID = glGetUniformLocation(shader, "light.attenuation");
	lightAmbientCoefficientID = glGetUniformLocation(shader, "light.ambientCoefficient");

	cameraPositionID = glGetUniformLocation(shader, "cameraPosition");
}

void Shader::SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material)
{
	// Lights
	glUniform1f(materialShininessID, material->materialShininess);
	glUniform3fv(materialSpecularColorID, 1, value_ptr(material->materialSpecularColor));

	glUniform3fv(lightPositionID, 1, value_ptr(scene->light.position));
	glUniform3fv(lightIntensitiesID, 1, value_ptr(scene->light.color));

	glUniform1f(lightAttenuationID, scene->light.attenuation);
	glUniform1f(lightAmbientCoefficientID, scene->light.ambientCoefficient);

	// Camera
	glUniform3fv(cameraPositionID, 1, value_ptr(Application::camera.position));

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
		glUniform1i(glGetUniformLocation(shader, name.c_str()), i);

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, material->textures[i].id);
	}

	// Matrices
	glm::mat4 model;
	model = glm::translate(model, transform->position);
	model = glm::scale(model, transform->scale);
	model = glm::rotate(model, transform->rotation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, transform->rotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, transform->rotation.z, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = Application::camera.GetViewMatrix();
	glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = Application::GetProjectionMatrix(false);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projection));
}