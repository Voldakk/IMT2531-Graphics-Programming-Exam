#include "Material.hpp"

#include "Scene.hpp"
#include "Application.hpp"


namespace EVA
{

	Material *Material::m_ActiveMaterial;

	std::shared_ptr<Texture> Material::textureDefaultDiffuse;
	std::shared_ptr<Texture> Material::textureDefaultSpecular;
	std::shared_ptr<Texture> Material::textureDefaultNormal;
	std::shared_ptr<Texture> Material::textureDefaultEmission;

	void Material::SetTexture(const Texture::Type type, const FS::path& path)
	{
		const auto t = TextureManager::LoadTexture(path);
		t->type = type;
		SetTexture(t);
	}

	void Material::SetTexture(const std::shared_ptr<Texture>& texture)
	{
		switch (texture->type)
		{
		case Texture::Diffuse:
			textureDiffuse = texture;
			break;
		case Texture::Specular:
			textureSpecular = texture;
			break;
		case Texture::Normal:
			textureNormal = texture;
			break;
		case Texture::Height:
			textureHeight = texture;
			break;
		case Texture::Emission:
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

	void Material::SetMaterialUniforms(Scene *scene) const
	{
		// Material
		shader->SetUniform1F("material.shininess", materialShininess);
		shader->SetUniform4Fv("material.tint_diffuse", tintDiffuse);

		// Textures
		SetTextures();

		// Camera
		shader->SetUniform3Fv("cameraPosition", Application::mainCamera->transform->position);

		// View and projection
		shader->SetUniformMatrix4Fv("view", Application::mainCamera->viewMatrix);
		shader->SetUniformMatrix4Fv("projection", Application::GetPerspectiveMatrix());

		if (scene == nullptr)
			return;

		// Lights
		auto lights = scene->GetLights();
		shader->SetUniform1I("numLights", lights.size());

		auto shadowNum = 0;

		for (unsigned int i = 0; i < lights.size(); ++i)
		{
			const auto lightNum = "allLights[" + std::to_string(i) + "].";

			shader->SetUniform3Fv(lightNum + "color", lights[i]->color);
			shader->SetUniform1F(lightNum + "ambientCoefficient", lights[i]->ambientCoefficient);

			if(lights[i]->GetType() == Light::Type::Directional)
			{
				shader->SetUniform4Fv(lightNum + "position", lights[i]->GetDirection());

				if (lights[i]->Shadows())
				{
					glActiveTexture(GL_TEXTURE5 + shadowNum);
					glBindTexture(GL_TEXTURE_2D, lights[i]->GetDepthMap());
					shader->SetUniform1I(lightNum + "shadowMap", 5 + shadowNum);
					shader->SetUniformMatrix4Fv(lightNum + "lightSpaceMatrix", lights[i]->GetLightSpaceMatrix());

					shadowNum++;
				}
			}
			else if (lights[i]->GetType() == Light::Type::Point)
			{
				shader->SetUniform4Fv(lightNum + "position", glm::vec4(lights[i]->position, 1.0f));
				shader->SetUniform1F(lightNum + "attenuation", lights[i]->attenuation);

				if (lights[i]->Shadows())
				{
					glActiveTexture(GL_TEXTURE5 + shadowNum);
					glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i]->GetDepthMap());
					shader->SetUniform1I("shadowCubeMap", 5 + shadowNum);
					shader->SetUniform1F(lightNum + "farPlane", lights[i]->pointFarPlane);

					shadowNum++;
				}
				else
				{
					shader->SetUniform1F(lightNum + "farPlane", -1.0f);
				}
			}
		}
	}

	void Material::SetObjectUniforms(Transform *transform) const
	{
		// Position
		shader->SetUniformMatrix4Fv("model", transform->modelMatrix);
	}

	void Material::SetTextures() const
	{
		// Diffuse
		glActiveTexture(GL_TEXTURE0);
		shader->SetUniform1I("material.texture_diffuse", 0);

		if (textureDiffuse != nullptr)
			glBindTexture(GL_TEXTURE_2D, textureDiffuse->id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultDiffuse->id);

		// Specular
		glActiveTexture(GL_TEXTURE1);
		shader->SetUniform1I("material.texture_specular", 1);

		if (textureSpecular != nullptr)
			glBindTexture(GL_TEXTURE_2D, textureSpecular->id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultSpecular->id);

		// Normal
		glActiveTexture(GL_TEXTURE2);
		shader->SetUniform1I("material.texture_normal", 2);

		if (textureNormal != nullptr)
			glBindTexture(GL_TEXTURE_2D, textureNormal->id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultNormal->id);

		// Emission
		glActiveTexture(GL_TEXTURE3);
		shader->SetUniform1I("material.texture_emission", 3);

		if (textureEmission != nullptr)
			glBindTexture(GL_TEXTURE_2D, textureEmission->id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultEmission->id);

		// Height
		glActiveTexture(GL_TEXTURE4);
		shader->SetUniform1I("material.texture_height", 4);

		if (textureHeight != nullptr)
			glBindTexture(GL_TEXTURE_2D, textureHeight->id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultSpecular->id);
	}

	void Material::Init()
	{
		textureDefaultDiffuse = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_diffuse.png");
		textureDefaultSpecular = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_specular.png");
		textureDefaultNormal = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_normal.png");
		textureDefaultEmission = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_emission.png");
	}

	void ShadowMaterial::SetMaterialUniforms(Scene* scene) const
	{

	}

	void ShadowMaterial::SetObjectUniforms(Transform* transform) const
	{
		// Position
		shader->SetUniformMatrix4Fv("model", transform->modelMatrix);
	}
}
