#pragma once

#include <memory>

#include "Shader.hpp"

namespace EVA
{
	// Struct for holding texture info
	struct Texture
	{
		// Different texture types
		enum Type
		{
			Diffuse, Specular, Normal, Emission, Height
		};

		unsigned int id{};
		Type type;
		std::string path;
	};

	class Scene;

	class Transform;

	/**
	 * \brief A material holds a Shader, textures and other material properties
	 */
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

		glm::vec4 tintDiffuse = glm::vec4(1.0f);

		std::shared_ptr<Shader> shader;

		bool enableInstancing = false;

		float materialShininess = 1000.0f;

		Material() = default;

		void SetTexture(Texture::Type type, const char *path);

		void SetTexture(Texture::Type type, unsigned int id);

		void SetTexture(Texture texture);

		void Activate(Scene* scene, Transform* transform);

		virtual void SetMaterialUniforms(Scene* scene) const;

		virtual void SetObjectUniforms(Transform* transform) const;

		void SetTextures() const;

		static void Init();

	};

	class ShadowMaterial : public Material
	{
	public:
		void SetMaterialUniforms(Scene* scene) const override;
		void SetObjectUniforms(Transform* transform) const override;
	};
}
