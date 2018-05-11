#include "Terrain.hpp"

REGISTER_COMPONENT_CPP(Terrain, "Terrain");

void Terrain::Start()
{
	m_MeshRenderer = gameObject->GetComponentOfType<EVA::MeshRenderer>();
	SetHeightMap(m_HeightMapPath);
}

void Terrain::Load(const EVA::DataObject data)
{
	m_HeightMapPath = data.GetPath("heightMap", "");
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

	CreateMesh();
}

void Terrain::CreateMesh()
{
	if (m_HeightData.empty() && m_HeightData[0].empty())
		return;

	const auto dataWidth = m_HeightData[0].size();
	const auto dataHeight = m_HeightData.size();

	const auto verticesPerUnit = 10.0f;
	const auto terrainWidth = 10;
	const auto terrainHeight = (float)terrainWidth * ((float)m_HeightData.size() / (float)m_HeightData[0].size());

	const auto verticesY = (unsigned int)std::floorf(terrainHeight * verticesPerUnit);
	const auto verticesX = (unsigned int)std::floorf(terrainWidth * verticesPerUnit);

	// Vertices
	std::vector<EVA::Vertex> vertices;
	vertices.resize(verticesY * verticesX);

	for (unsigned int y = 0; y < verticesY; ++y)
	{
		for (unsigned int x = 0; x < verticesX; ++x)
		{
			vertices[y*verticesX + x].position = glm::vec3(x / verticesPerUnit, HeightData((float)x / (float)verticesX, (float)y / (float)verticesY), y / verticesPerUnit);
		}
	}

	// Indices
	std::vector<unsigned int> indices;
	indices.reserve((verticesY-1) * (verticesX-1) * 6);

	for (unsigned int y = 0; y < verticesY - 1; ++y)
	{
		for (unsigned int x = 0; x < verticesX - 1; ++x)
		{
			indices.push_back(x + (y + 1) * verticesX);
			indices.push_back((x + 1) + (y + 1) * verticesX);
			indices.push_back(x + y * verticesX);

			indices.push_back((x + 1) + (y + 1) * verticesX);
			indices.push_back((x + 1) + y * verticesX);
			indices.push_back(x + y * verticesX);
		}
	}

	const auto mesh = std::make_shared<EVA::Mesh>(vertices, indices);
	const auto material = EVA::MaterialManager::LoadMaterial("./assets/materials/terrain.mat");

	if(m_MeshRenderer != nullptr)
		m_MeshRenderer->Set(mesh, material);
}

float Terrain::HeightData(const float x, const float y)
{
	return m_HeightData[y*m_HeightData.size()][x*m_HeightData[0].size()] / 255.0f;
}
