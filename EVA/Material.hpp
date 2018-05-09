#pragma once

#include <memory>

#include "Shader.hpp"

#include "FileSystem.hpp"
#include "TextureManager.hpp"

namespace EVA
{
	class Scene;

	class Transform;

	/**
	 * \brief A material holds a Shader, textures and other material properties
	 */
	class Material
	{

		static Material* m_ActiveMaterial;
		inline static const FS::path DEFAULT_TEXTURES_PATH = "./assets/standard assets/textures/";

	public:

		FS::path path;

		static std::shared_ptr<Texture> textureDefaultDiffuse;
		static std::shared_ptr<Texture> textureDefaultSpecular;
		static std::shared_ptr<Texture> textureDefaultNormal;
		static std::shared_ptr<Texture> textureDefaultEmission;

		std::shared_ptr<Texture> textureDiffuse;
		std::shared_ptr<Texture> textureSpecular;
		std::shared_ptr<Texture> textureNormal;
		std::shared_ptr<Texture> textureEmission;
		std::shared_ptr<Texture> textureHeight;

		glm::vec4 tintDiffuse = glm::vec4(1.0f);

		std::shared_ptr<Shader> shader;

		bool enableInstancing = false;

		float materialShininess = 32.0f;

		Material() = default;

		void SetTexture(Texture::Type type, const FS::path& path);

		void SetTexture(const std::shared_ptr<Texture>& texture);

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
