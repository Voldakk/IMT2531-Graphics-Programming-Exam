#pragma once

#include <memory>

#include "EVA.hpp"
#include "EVA/ResourceManagers.hpp"

class Terrain : public EVA::Component
{
	REGISTER_COMPONENT_HPP(Terrain);

	FS::path m_HeightMapPath;

	std::vector<std::vector<float>> m_HeightData;

public:

	void Start() override;
	void Load(const EVA::DataObject data) override;
	void Save(EVA::DataObject& data) override;
	void Inspector() override;

private:
	
	void SetHeightMap(const FS::path& newHeightMapPath);
	void LoadHeightMapData(const std::shared_ptr<EVA::RawTexture>& texture);
	void CreateMesh();

	float HeightData(unsigned int x, unsigned int y);
};
