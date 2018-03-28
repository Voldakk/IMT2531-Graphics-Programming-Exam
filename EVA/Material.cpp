#include "Material.hpp"

#include "Scene.hpp"
#include "Application.hpp"

namespace EVA
{

	Material *Material::m_ActiveMaterial;

	Material::Material() = default;

	void Material::AddTexture(const TextureType type, const char *path)
	{
		Texture t = {0, type, path};
		t.id = TextureManager::GetTexture(t.path);
		m_Textures.push_back(t);
	}

	void Material::AddTexture(const TextureType type, const unsigned int id)
	{
		const Texture t = {id, type, ""};
		m_Textures.push_back(t);
	}

	void Material::Activate(Scene *scene, Transform *transform)
	{
		if (m_ActiveMaterial != this)
		{
			m_ActiveMaterial = this;

			m_Shader->Bind();

			SetMaterialUniforms(scene);
		}

		if (transform != nullptr)
			SetObjectUniforms(transform);
	}

	void Material::SetMaterialUniforms(Scene *scene)
	{
		// Material
		m_Shader->SetUniform1f("material.shininess", m_MaterialShininess);

		// Textures
		SetTextures();

		// Camera
		m_Shader->SetUniform3fv("cameraPosition", Application::m_MainCamera->GetGameObject()->GetTransform()->position);

		// View and projection
		m_Shader->SetUniformMatrix4fv("view", Application::m_MainCamera->GetViewMatrix());
		m_Shader->SetUniformMatrix4fv("projection", Application::GetPerspectiveMatrix());

		if (scene == nullptr)
			return;

		// Lights

		auto lights = scene->GetLights();
		m_Shader->SetUniform1i("numLights", lights.size());

		for (unsigned int i = 0; i < lights.size(); ++i)
		{
            std::string lightNum = "allLights[" + std::to_string(i) + "].";

            m_Shader->SetUniform3fv(lightNum + "color", lights[i]->Color);
            m_Shader->SetUniform1f(lightNum + "ambientCoefficient", lights[i]->AmbientCoefficient);

            switch (lights[i]->Type)
            {

                case LightType::Directional:


                    m_Shader->SetUniform4fv(lightNum + "position", lights[i]->GetDirection());
                    break;

                case LightType::Point:
                    m_Shader->SetUniform4fv(lightNum + "position", glm::vec4(lights[i]->Position, 1.0f));
                    m_Shader->SetUniform1f(lightNum + "attenuation", lights[i]->Attenuation);
                    break;
            }
		}
	}

	void Material::SetObjectUniforms(Transform *transform)
	{
		// Position
		m_Shader->SetUniformMatrix4fv("model", transform->GetModelMatrix());
	}

	void Material::SetTextures()
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			// Activate the texture
			glActiveTexture(GL_TEXTURE0 + i);

			// Find the name based on the texture type
			std::string name;

			switch (m_Textures[i].type)
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
			m_Shader->SetUniform1i("material." + name, i);

			// Bind the texture
			glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
		}
	}

}