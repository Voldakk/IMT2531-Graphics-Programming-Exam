#include "Terrain.hpp"

#include <iostream>

#include "EVA/Input.hpp"
#include "EVA/ResourceManagers.hpp"

#include "../TerrainMaterial.hpp"


REGISTER_COMPONENT_CPP(Terrain, "Terrain");


void Terrain::Awake()
{
	m_MeshRenderer = gameObject->GetComponentOfType<EVA::MeshRenderer>();
	m_EnviromentManager = gameObject->GetComponentOfType<EnviromentManager>();


	m_Material = std::make_shared<TerrainMaterial>(this, m_EnviromentManager);
	m_Material->shader = EVA::ShaderManager::LoadShader("./assets/shaders/terrain.shader");
	m_Material->materialShininess = m_MaterialShininess;

	SetHeightMap(m_HeightMapPath);
}

void Terrain::Load(const EVA::DataObject data)
{
	m_HeightMapPath = data.GetPath("heightMap", "");
	
	m_TerrainWidth = data.GetFloat("terrainWidth", m_TerrainWidth);
	m_MaxTerrainHeight = data.GetFloat("maxTerrainHeight", m_MaxTerrainHeight);
	m_VerticesPerUnit = data.GetFloat("verticesPerUnit", m_VerticesPerUnit);
	m_MaterialShininess = data.GetFloat("materialShininess", m_MaterialShininess);
	m_SpecularStrength = data.GetFloat("specularStrength", m_SpecularStrength);
}

void Terrain::Save(EVA::DataObject& data)
{
	data.SetString("heightMap", EVA::FileSystem::ToString(m_HeightMapPath));

	data.SetFloat("terrainWidth", m_TerrainWidth);
	data.SetFloat("maxTerrainHeight", m_MaxTerrainHeight);
	data.SetFloat("verticesPerUnit", m_VerticesPerUnit);
	data.SetFloat("materialShininess", m_MaterialShininess);
	data.SetFloat("specularStrength", m_SpecularStrength);
}

void Terrain::Inspector()
{
	auto heightMapPath = EVA::FileSystem::ToString(m_HeightMapPath);
	if(ComponentInspector::DragDropTargetString("Height map", heightMapPath, "file"))
	{
		SetHeightMap(heightMapPath);
	}

	ComponentInspector::Float("Terrain width", m_TerrainWidth);
	ComponentInspector::Float("Max terrain height", m_MaxTerrainHeight);
	ComponentInspector::Float("Vertices per unit", m_VerticesPerUnit);

	if(ComponentInspector::Button("Regenerate"))
	{
		CreateMesh();
	}

	ComponentInspector::Bool("Contour lines", contourLines);
	ComponentInspector::Float("Material shininess", m_MaterialShininess);
	m_Material->materialShininess = m_MaterialShininess;

	ComponentInspector::Float("Specular strength", m_SpecularStrength);
}

void Terrain::Update(float deltaTime)
{
	if (EVA::Input::KeyDown(EVA::Input::O))
		contourLines = !contourLines;
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
		if(y % 100 == 0)
			std::cout << "Terrain::SetHeightMap - Reading height data: " << y << "/" << data->height << std::endl;
		
		m_HeightData[y].resize(data->width);
		for (unsigned int x = 0; x < data->width; ++x)
		{
			m_HeightData[y][x] = data->data[y*data->width + x * data->channels];
		}
	}
	std::cout << "Terrain::SetHeightMap - Reading height data: done" << std::endl;

	CreateMesh();
}

void Terrain::CreateMesh()
{
	if (m_HeightData.empty() && m_HeightData[0].empty())
		return;

	std::cout << "Terrain::CreateMesh - Generating mesh..." << std::endl;

	m_TerrainLength = (float)m_TerrainWidth * ((float)m_HeightData.size() / (float)m_HeightData[0].size());

	const auto verticesY = (unsigned int)std::floorf(m_TerrainLength * m_VerticesPerUnit);
	const auto verticesX = (unsigned int)std::floorf(m_TerrainWidth * m_VerticesPerUnit);

	// Vertices
	std::vector<EVA::Vertex> vertices;
	vertices.resize(verticesY * verticesX);

	// Position
	std::cout << "Terrain::CreateMesh - Vertex positions" << std::endl;
	for (unsigned int y = 0; y < verticesY; ++y)
	{
		for (unsigned int x = 0; x < verticesX; ++x)
		{
			const auto height = HeightData((float)x / (float)verticesX, (float)y / (float)verticesY);

			vertices[y*verticesX + x].position = glm::vec3(x / m_VerticesPerUnit, height * maxTerrainHeight, y / m_VerticesPerUnit);
			vertices[y*verticesX + x].texCoords = glm::vec2((float)x / (float)verticesX, (float)y/ (float)verticesY);
		}
	}

	// Normals
	std::cout << "Terrain::CreateMesh - Normals" << std::endl;
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
	std::cout << "Terrain::CreateMesh - Indices" << std::endl;
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

	if(m_MeshRenderer != nullptr)
		m_MeshRenderer->Set(mesh, m_Material);
}

float Terrain::HeightData(const float x, const float y)
{
	return m_HeightData[y*m_HeightData.size()][x*m_HeightData[0].size()] / 255.0f;
}
