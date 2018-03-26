#include "Material.hpp"

#include "Scene.hpp"
#include "Transform.hpp"
#include "TextureManager.hpp"
#include "Application.hpp"

Material * Material::activeMaterial;

Material::Material() = default;

void Material::AddTexture(const TextureType type, const char * path)
{
	Texture t = { 0, type, path };
	t.id = TextureManager::GetTexture(t.path);
	textures.push_back(t);
}

void Material::AddTexture(const TextureType type, const unsigned int id)
{
	const Texture t = { id, type, "" };
	textures.push_back(t);
}

void Material::Activate(Scene *scene, Transform *transform)
{
	if(activeMaterial != this)
	{
		activeMaterial = this;

		shader->Bind();

		SetMaterialUniforms(scene);
	}

	if(transform != nullptr)
		SetObjectUniforms(transform);
}

void Material::SetMaterialUniforms(Scene *scene)
{
	// Material
	shader->SetUniform1f("materialShininess", materialShininess);
	shader->SetUniform3fv("materialSpecularColor", materialSpecularColor);

	// Textures
	SetTextures();

	// Camera
	shader->SetUniform3fv("cameraPosition", Application::mainCamera->gameObject->transform->position);

    // View and projection
    shader->SetUniformMatrix4fv("view", Application::mainCamera->GetViewMatrix());
    shader->SetUniformMatrix4fv("projection", Application::GetPerspectiveMatrix());

	if(scene == nullptr)
		return;

	// Lights
	shader->SetUniform3fv("light.position", scene->light.position);
	shader->SetUniform3fv("light.intensities", scene->light.color);

	shader->SetUniform1f("light.attenuation", scene->light.attenuation);
	shader->SetUniform1f("light.ambientCoefficient", scene->light.ambientCoefficient);
}

void Material::SetObjectUniforms(Transform *transform)
{
	// Position
	shader->SetUniformMatrix4fv("model", transform->GetModelMatrix());
}

void Material::SetTextures()
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		// Activate the texture
		glActiveTexture(GL_TEXTURE0 + i);

		// Find the name based on the texture type
		std::string name;

		switch (textures[i].type)
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
		shader->SetUniform1i(name, i);

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
}
