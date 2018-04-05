#pragma once

#include <vector>
#include <memory>

#include "Shader.hpp"

namespace EVA
{

	// Different texture types
	enum TextureType
	{
		Diffuse, Specular, Normal, Emission, Height, ShadowMap
	};

	// Struct for holding texture info
	struct Texture
	{
		unsigned int id{};
		TextureType type;
		std::string path;
	};

	class Scene;

	class Transform;

	class Material
	{

		static Material* m_ActiveMaterial;

	public:

		static unsigned int textureDefaultDiffuse;
		static unsigned int textureDefaultSpecular;
		static unsigned int textureDefaultNormal;
		static unsigned int textureDefaultEmission;

		Texture textureDiffuse;
		Texture textureSpecular;
		Texture textureNormal;
		Texture textureEmission;
		Texture textureHeight;
		Texture textureShadowMap;

		std::shared_ptr<Shader> shader;

		bool enableInstancing = false;

		float materialShininess = 1000.0f;

		Material();

		void SetTexture(TextureType type, const char *path);

		void SetTexture(TextureType type, unsigned int id);

		void SetTexture(Texture texture);

		void Activate(Scene* scene, Transform* transform);

		void SetMaterialUniforms(Scene* scene) const;

		void SetObjectUniforms(Transform* transform) const;

		void SetTextures() const;

		static void Init();

	};

}