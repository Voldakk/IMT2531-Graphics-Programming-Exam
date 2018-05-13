#include "Water.hpp"

#include <iostream>

#include "../WaterMaterial.hpp"

REGISTER_COMPONENT_CPP(Water, "Water")

void Water::Start()
{
	m_Terrain = scene->FindComponentOfType<Terrain>();
	m_MeshRenderer = gameObject->GetComponentOfType<EVA::MeshRenderer>();

	m_Material = std::make_shared<WaterMaterial>(this);
	m_Material->shader = EVA::ShaderManager::LoadShader("./assets/shaders/water.shader");

	GenerateMesh();
}

void Water::Update(const float deltaTime)
{
	time += deltaTime * m_TimeScale;
}

void Water::GenerateMesh() const
{
	if (m_Terrain == nullptr || m_MeshRenderer == nullptr)
		return;

	std::cout << "Water::GenerateMesh - Generating mesh" << std::endl;

	const auto verticesY = (unsigned int)std::floorf(m_Terrain->terrainLength * m_VerticesPerUnit);
	const auto verticesX = (unsigned int)std::floorf(m_Terrain->terrainWidth * m_VerticesPerUnit);

	// Vertices
	std::vector<EVA::Vertex> vertices;
	vertices.resize(verticesY * verticesX);

	// Position
	std::cout << "Water::GenerateMesh - Vertex positions" << std::endl;
	for (unsigned int y = 0; y < verticesY; ++y)
	{
		for (unsigned int x = 0; x < verticesX; ++x)
		{
			vertices[y*verticesX + x].position = glm::vec3(x / m_VerticesPerUnit, 0.0f, y / m_VerticesPerUnit);
			vertices[y*verticesX + x].texCoords = glm::vec2((float)x / (float)verticesX, (float)y / (float)verticesY);
		}
	}

	// Indices
	std::cout << "Water::GenerateMesh - Indices" << std::endl;
	std::vector<unsigned int> indices;
	indices.reserve((verticesY - 1) * (verticesX - 1) * 6);

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
	m_MeshRenderer->Set(mesh, m_Material);
}

void Water::Load(const EVA::DataObject data)
{
	waveLength = data.GetFloat("waveLength", waveLength);
	amplitude = data.GetFloat("amplitude", amplitude);
	m_TimeScale = data.GetFloat("timeScale", m_TimeScale);
}

void Water::Save(EVA::DataObject& data)
{
	data.SetFloat("waveLength", waveLength);
	data.SetFloat("amplitude", amplitude);
	data.SetFloat("timeScale", m_TimeScale);
}

void Water::Inspector()
{
	ComponentInspector::Float("Wave length", waveLength);
	ComponentInspector::Float("Amplitude", amplitude);
	ComponentInspector::Float("Time scale", m_TimeScale);
}
