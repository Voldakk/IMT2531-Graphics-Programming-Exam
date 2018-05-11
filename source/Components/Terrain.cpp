#include "Terrain.hpp"

REGISTER_COMPONENT_CPP(Terrain, "Terrain");

void Terrain::Start()
{

}

void Terrain::Load(const EVA::DataObject data)
{
	const auto heightMapPath = data.GetPath("heightMap", "");
	SetHeightMap(heightMapPath);
}

void Terrain::Save(EVA::DataObject& data)
{
	data.SetString("heightMap", EVA::FileSystem::ToString(m_HeightMapPath));
}

void Terrain::Inspector()
{
	auto heightMapPath = EVA::FileSystem::ToString(m_HeightMapPath);
	if(ComponentInspector::DragDropTargetString("Height map", heightMapPath, "file"))
	{
		SetHeightMap(heightMapPath);
	}
}

void Terrain::SetHeightMap(const FS::path& newHeightMapPath)
{
	m_HeightMapPath = newHeightMapPath;

	const auto data = EVA::TextureManager::LoadRaw(m_HeightMapPath);

	if(data != nullptr)
		LoadHeightMapData(data);
}

void Terrain::LoadHeightMapData(const std::shared_ptr<EVA::RawTexture>& texture)
{
	m_HeightData.clear();

	m_HeightData.resize(texture->height);
	for (unsigned int y = 0; y < texture->height; ++y)
	{
		m_HeightData[y].resize(texture->width);
		for (unsigned int x = 0; x < texture->width; ++x)
		{
			m_HeightData[y][x] = texture->data[y*texture->width + x * texture->channels];
		}
	}
}

void Terrain::CreateMesh()
{

}
