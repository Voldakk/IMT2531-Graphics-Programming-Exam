#pragma once

#include "Json.hpp"
#include "../Material.hpp"
#include "../ShaderManager.hpp"

namespace EVA
{

	/**
	 * \brief Parser for material files
	 */
	class MaterialParser
	{

	public:

		/**
		 * \brief Loads a mertertil form a material asset file
		 * \param path The path to the file
		 * \return A pointer to the material, or nullptr if no material is found
		 */
		static std::shared_ptr<Material> Load(const FS::path& path)
		{
			auto material = std::make_shared<Material>();

			material->path = path;

			const auto sd = Json::Open(path);

			if (sd == nullptr)
				return nullptr;

			auto& d = (*sd);

			DataObject data(d);


			// Shader
			const auto shaderPath = data.GetPath("shader", "");
			if (!shaderPath.empty())
				material->shader = ShaderManager::LoadShader(shaderPath);

			// Instancing
			material->SetUseInstancing(data.GetBool("useInstancing", false));

			// Culling 
			material->cullFront = data.GetBool("cullFront", material->cullFront);
			material->cullBack = data.GetBool("cullBack", material->cullBack);

			// Shininess
			material->materialShininess = data.GetFloat("shininess", material->materialShininess);

			// Alpha cutoff
			material->alphaCutoff = data.GetFloat("alphaCutoff", material->alphaCutoff);

			// Tint
			material->tintDiffuse = data.GetVec4("tintDiffuse", material->tintDiffuse);

			// Diffuse
			const auto diffusePath = data.GetPath("textureDiffuse", "");
			if (!diffusePath.empty())
				material->SetTexture(Texture::Diffuse, diffusePath);

			// Specular
			const auto specularPath = data.GetPath("textureSpecular", "");
			if (!specularPath.empty())
				material->SetTexture(Texture::Specular, specularPath);

			// Normal
			const auto normalPath = data.GetPath("textureNormal", "");
			if (!normalPath.empty())
				material->SetTexture(Texture::Normal, normalPath);

			// Emission
			const auto emissionPath = data.GetPath("textureEmission", "");
			if (!emissionPath.empty())
				material->SetTexture(Texture::Emission, emissionPath);

			return material;
		}

		static void Save(const std::shared_ptr<Material>& material, const FS::path& path)
		{
			Json::Document d;
			d.SetObject();

			auto& a = d.GetAllocator();

			DataObject data(d, &a);

			if (material->shader != nullptr)
				data.SetString("shader", FileSystem::ToString(material->shader->paths->shader));

			data.SetBool("useInstancing", material->useInstancing);

			// Culling 
			data.SetBool("cullFront", material->cullFront);
			data.SetBool("cullBack", material->cullBack);


			data.SetFloat("shininess", material->materialShininess);

			data.SetFloat("alphaCutoff", material->alphaCutoff);

			if (material->tintDiffuse != glm::vec4(1.0f))
				data.SetVec4("tintDiffuse", material->tintDiffuse);

			if (material->textureDiffuse != nullptr)
				data.SetPath("textureDiffuse", material->textureDiffuse->path);

			if (material->textureSpecular != nullptr)
				data.SetPath("textureSpecular", material->textureSpecular->path);

			if (material->textureNormal != nullptr)
				data.SetPath("textureNormal", material->textureNormal->path);

			if (material->textureEmission != nullptr)
				data.SetPath("textureEmission", material->textureEmission->path);

			Json::Save(&d, path);
		}
	};

}
