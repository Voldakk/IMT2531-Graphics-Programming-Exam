#pragma once

#include <vector>
#include <memory>

#include "glm/glm/glm.hpp"

#include "Shader.hpp"
#include "TextureManager.hpp"

namespace EVA
{

	// Different texture types
	enum TextureType
	{
		Diffuse, Specular, Normal, Height, Emission
	};

	// Struct for holding texture info
	struct Texture
	{
		unsigned int id;
		TextureType type;
		std::string path;
	};

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

		Material();

		void AddTexture(TextureType type, const char *path);

		void AddTexture(TextureType type, unsigned int id);

		void Activate(Scene *scene, Transform *transform);

		void SetMaterialUniforms(Scene *scene);

		void SetObjectUniforms(Transform *transform);

		void SetTextures();

	};

}