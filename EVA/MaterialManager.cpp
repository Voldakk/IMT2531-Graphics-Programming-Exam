#include "MaterialManager.hpp"
#include "Parsers/MaterialParser.hpp"

namespace EVA
{
	std::map<FS::path, std::shared_ptr<Material>> MaterialManager::m_Materials;

	std::shared_ptr<Material> MaterialManager::LoadMaterial(const FS::path& path)
	{
		// Return the id if the material's already loaded
		if (m_Materials.count(path))
			return m_Materials[path];

		auto material = MaterialParser::Load(path);

		if (material == nullptr)
			return nullptr;

		m_Materials[path] = material;
		return material;
	}
}