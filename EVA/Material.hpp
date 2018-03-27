#pragma once

#include <vector>
#include <memory>

#include "glm/glm/glm.hpp"

#include "Shader.hpp"
#include "TextureManager.hpp"

namespace EVA
{

	class Scene;

	class Transform;

	class Material
	{

	private:

		static Material *m_ActiveMaterial;

	public:

		std::shared_ptr<Shader> m_Shader;
		std::vector<Texture> m_Textures;

		bool m_EnableInstancing = false;

		float m_MaterialShininess = 1000.0f;
		glm::vec3 m_MaterialSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);

		Material();

		void AddTexture(TextureType type, const char *path);

		void AddTexture(TextureType type, unsigned int id);

		void Activate(Scene *scene, Transform *transform);

		void SetMaterialUniforms(Scene *scene);

		void SetObjectUniforms(Transform *transform);

		void SetTextures();

	};

}