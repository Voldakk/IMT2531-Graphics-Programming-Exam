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

	if(data == nullptr)
		return;

	m_HeightData.clear();

	m_HeightData.resize(data->height);
	for (unsigned int y = 0; y < data->height; ++y)
	{
		m_HeightData[y].resize(data->width);
		for (unsigned int x = 0; x < data->width; ++x)
		{
			m_HeightData[y][x] = data->data[y*data->width + x * data->channels];
		}
	}

	CreateMesh();
}

void Terrain::CreateMesh()
{
	if (m_HeightData.empty() && m_HeightData[0].empty())
		return;

	const auto verticesPerUnit = 1.0f;
	const auto terrainWidth = 100;
	const auto terrainLength = (float)terrainWidth * ((float)m_HeightData.size() / (float)m_HeightData[0].size());
	const auto terrainMaxHeight = 40.0f;

	const auto verticesY = (unsigned int)std::floorf(terrainLength * verticesPerUnit);
	const auto verticesX = (unsigned int)std::floorf(terrainWidth * verticesPerUnit);

	// Vertices
	std::vector<EVA::ColorVertex> vertices;
	vertices.resize(verticesY * verticesX);

	// Position
	for (unsigned int y = 0; y < verticesY; ++y)
	{
		for (unsigned int x = 0; x < verticesX; ++x)
		{
			const auto height = HeightData((float)x / (float)verticesX, (float)y / (float)verticesY);

			glm::vec3 color;
			if (height < 0.1)
				color = { 0.0f, 0.0f, 1.0f };
			else if (height < 0.2f)
				color = { 0.0f, 1.0f, 0.0f };
			else if (height < 0.3f)
				color = { 0.5f, 0.5f, 0.5f };
			else
				color = { 1.0f, 1.0f, 1.0f };

			vertices[y*verticesX + x].color = color;

			vertices[y*verticesX + x].position = glm::vec3(x / verticesPerUnit, terrainMaxHeight * height, y / verticesPerUnit);
		}
	}

	// Normals
	for (unsigned int y = 1; y < verticesY-1; ++y)
	{
		for (unsigned int x = 1; x < verticesX-1; ++x)
		{
			// Read neightbor heights
			const auto xm = vertices[y*verticesX + x - 1].position.y;
			const auto xp = vertices[y*verticesX + x + 1].position.y;
			const auto ym = vertices[(y-1)*verticesX + x].position.y;
			const auto yp = vertices[(y+1)*verticesX + x].position.y;

			// Deduce terrain normal
			glm::vec3 normal;
			normal.x = xm - xp;
			normal.z = ym - yp;
			normal.y = 2.0;

			vertices[y*verticesX + x].normal = glm::normalize(normal);
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
