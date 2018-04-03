#include "Material.hpp"

#include "Scene.hpp"
#include "Application.hpp"

namespace EVA
{

	Material *Material::m_ActiveMaterial;

	unsigned int Material::textureDefaultDiffuse;
	unsigned int Material::textureDefaultSpecular;
	unsigned int Material::textureDefaultNormal;
	unsigned int Material::textureDefaultEmission;

	Material::Material() = default;

	void Material::SetTexture(const TextureType type, const char *path)
	{
		Texture t = { 0, type, path };
		t.id = TextureManager::GetTexture(t.path);
		SetTexture(t);
	}

	void Material::SetTexture(const TextureType type, const unsigned int id)
	{
		const Texture t = { id, type, "" };
		SetTexture(t);
	}

	void Material::SetTexture(const Texture texture)
	{
		switch (texture.type)
		{
		case Diffuse:
			textureDiffuse = texture;
			break;
		case Specular:
			textureSpecular = texture;
			break;
		case Normal:
			textureNormal = texture;
			break;
		case Height:
			textureHeight = texture;
			break;
		case Emission:
			textureEmission = texture;
			break;
		default:
			break;
		}
	}

	void Material::Activate(Scene *scene, Transform *transform)
	{
		if (m_ActiveMaterial != this)
		{
			m_ActiveMaterial = this;

			shader->Bind();

			SetMaterialUniforms(scene);
		}

		if (transform != nullptr)
			SetObjectUniforms(transform);
	}

	void Material::SetMaterialUniforms(Scene *scene)
	{
		// Material
		shader->SetUniform1f("material.shininess", materialShininess);

		// Textures
		SetTextures();

		// Camera
		shader->SetUniform3fv("cameraPosition", Application::mainCamera->GetGameObject()->GetTransform()->position);

		// View and projection
		shader->SetUniformMatrix4fv("view", Application::mainCamera->GetViewMatrix());
		shader->SetUniformMatrix4fv("projection", Application::GetPerspectiveMatrix());

		if (scene == nullptr)
			return;

		// Lights

		auto lights = scene->GetLights();
		shader->SetUniform1i("numLights", lights.size());

		for (unsigned int i = 0; i < lights.size(); ++i)
		{
			const auto lightNum = "allLights[" + std::to_string(i) + "].";

			shader->SetUniform3fv(lightNum + "color", lights[i]->color);
			shader->SetUniform1f(lightNum + "ambientCoefficient", lights[i]->ambientCoefficient);

			switch (lights[i]->type)
			{

			case LightType::Directional:


				shader->SetUniform4fv(lightNum + "position", lights[i]->GetDirection());
				break;

			case LightType::Point:
				shader->SetUniform4fv(lightNum + "position", glm::vec4(lights[i]->position, 1.0f));
				shader->SetUniform1f(lightNum + "attenuation", lights[i]->attenuation);
				break;
			}
		}
	}

	void Material::SetObjectUniforms(Transform *transform) const
	{
		// Position
		shader->SetUniformMatrix4fv("model", transform->GetModelMatrix());
	}

	void Material::SetTextures() const
	{
		// Diffuse
		glActiveTexture(GL_TEXTURE0);
		shader->SetUniform1i("material.texture_diffuse", 0);

		if (textureDiffuse.id != 0)
			glBindTexture(GL_TEXTURE_2D, textureDiffuse.id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultDiffuse);

		// Specular
		glActiveTexture(GL_TEXTURE1);
		shader->SetUniform1i("material.texture_specular", 1);

		if (textureSpecular.id != 0)
			glBindTexture(GL_TEXTURE_2D, textureSpecular.id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultSpecular);

		// Normal
		glActiveTexture(GL_TEXTURE2);
		shader->SetUniform1i("material.texture_normal", 2);

		if (textureNormal.id != 0)
			glBindTexture(GL_TEXTURE_2D, textureNormal.id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultNormal);

		// Emission
		glActiveTexture(GL_TEXTURE4);
		shader->SetUniform1i("material.texture_emission", 4);

		if (textureEmission.id != 0)
			glBindTexture(GL_TEXTURE_2D, textureEmission.id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultEmission);

		// Height
		glActiveTexture(GL_TEXTURE3);
		shader->SetUniform1i("material.texture_height", 3);

		if (textureHeight.id != 0)
			glBindTexture(GL_TEXTURE_2D, textureHeight.id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultSpecular);
	}

	void Material::Init()
	{
		textureDefaultDiffuse = TextureManager::GetTexture("./assets/textures/default_diffuse.png");
		textureDefaultSpecular = TextureManager::GetTexture("./assets/textures/default_specular.png");
		textureDefaultNormal = TextureManager::GetTexture("./assets/textures/default_normal.png");
		textureDefaultEmission = TextureManager::GetTexture("./assets/textures/default_emission.png");
	}
}
