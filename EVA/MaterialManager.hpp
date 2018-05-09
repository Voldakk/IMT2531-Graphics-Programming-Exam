#pragma once

#include "Material.hpp"

namespace EVA
{

	class MaterialManager
	{
		static std::map<FS::path, std::shared_ptr<Material>> m_Materials;

	public:

		/**
		 * \brief Loads a material from the given path
		 * \param path The path to the material asset file
		 * \return A pointer to the material, or nullptr if no material is found
		 */
		static std::shared_ptr<Material> LoadMaterial(const FS::path& path);
	};

}