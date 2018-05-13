#include "Water.hpp"

#include <iostream>

#include "../WaterMaterial.hpp"

REGISTER_COMPONENT_CPP(Water, "Water")

void Water::Start()
{
	m_Terrain = scene->FindComponentOfType<Terrain>();
	m_EnviromentManager = scene->FindComponentOfType<EnviromentManager>();
	m_MeshRenderer = gameObject->GetComponentOfType<EVA::MeshRenderer>();

	m_Material = std::make_shared<WaterMaterial>(this);
	m_Material->shader = EVA::ShaderManager::LoadShader("./assets/shaders/water.shader");

	GenerateMesh();
}

void Water::Update(const float deltaTime)
{
	time += deltaTime * m_TimeScale;

	transform->SetPosition({ transform->localPosition.x, GetWaterProperties().height, transform->localPosition.z });
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

	const auto maxHeight = glm::max(summerProperties.height, winterProperties.height) + m_Terrain->maxTerrainHeight * 0.01f;

	for (unsigned int y = 0; y < verticesY - 1; ++y)
	{
		for (unsigned int x = 0; x < verticesX - 1; ++x)
		{
			const auto height1 = m_Terrain->HeightData((float)x / (float)verticesX, (float)y / (float)verticesY) * m_Terrain->maxTerrainHeight;
			const auto height2 = m_Terrain->HeightData((float)(x+1) / (float)verticesX, (float)y / (float)verticesY) * m_Terrain->maxTerrainHeight;
			const auto height3 = m_Terrain->HeightData((float)x / (float)verticesX, (float)(y+1) / (float)verticesY) * m_Terrain->maxTerrainHeight;
			const auto height4 = m_Terrain->HeightData((float)(x+1) / (float)verticesX, (float)(y+1) / (float)verticesY) * m_Terrain->maxTerrainHeight;

			if(height1 > maxHeight && height2 > maxHeight && height3 > maxHeight && height4 > maxHeight)
				continue;

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
	m_TimeScale = data.GetFloat("timeScale", m_TimeScale);
	m_VerticesPerUnit = data.GetFloat("m_VerticesPerUnit", m_VerticesPerUnit);

	summerProperties.height = data.GetFloat("summerProperties.height", summerProperties.height);
	summerProperties.waveLength = data.GetFloat("summerProperties.waveLength", summerProperties.waveLength);
	summerProperties.amplitude = data.GetFloat("summerProperties.amplitude", summerProperties.amplitude);
	summerProperties.specularStrength = data.GetFloat("summerProperties.specularStrength", summerProperties.specularStrength);
	summerProperties.materialShininess = data.GetFloat("summerProperties.materialShininess", summerProperties.materialShininess);
	summerProperties.color = data.GetVec4("summerProperties.color", summerProperties.color);

	winterProperties.height = data.GetFloat("winterProperties.height", winterProperties.height);
	winterProperties.waveLength = data.GetFloat("winterProperties.waveLength", winterProperties.waveLength);
	winterProperties.amplitude = data.GetFloat("winterProperties.amplitude", winterProperties.amplitude);
	winterProperties.specularStrength = data.GetFloat("winterProperties.specularStrength", winterProperties.specularStrength);
	winterProperties.materialShininess = data.GetFloat("winterProperties.materialShininess", winterProperties.materialShininess);
	winterProperties.color = data.GetVec4("winterProperties.color", summerProperties.color);
}
void Water::Save(EVA::DataObject& data)
{
	data.SetFloat("timeScale", m_TimeScale);
	data.SetFloat("m_VerticesPerUnit", m_VerticesPerUnit);
	
	data.SetFloat("summerProperties.height", summerProperties.height);
	data.SetFloat("summerProperties.waveLength", summerProperties.waveLength);
	data.SetFloat("summerProperties.amplitude", summerProperties.amplitude);
	data.SetFloat("summerProperties.specularStrength", summerProperties.specularStrength);
	data.SetFloat("summerProperties.materialShininess", summerProperties.materialShininess);
	data.SetVec4("summerProperties.color", summerProperties.color);

	data.SetFloat("winterProperties.height", winterProperties.height);
	data.SetFloat("winterProperties.waveLength", winterProperties.waveLength);
	data.SetFloat("winterProperties.amplitude", winterProperties.amplitude);
	data.SetFloat("winterProperties.specularStrength", winterProperties.specularStrength);
	data.SetFloat("winterProperties.materialShininess", winterProperties.materialShininess);
	data.SetVec4("winterProperties.color", winterProperties.color);
}

void Water::Inspector()
{
	ComponentInspector::Float("Time scale", m_TimeScale);
	
	ComponentInspector::Text("Summer");

	ComponentInspector::Float("Height##Summer", summerProperties.height);

	ComponentInspector::Float("Wave length##Summer", summerProperties.waveLength);
	ComponentInspector::Float("Amplitude##Summer", summerProperties.amplitude);

	ComponentInspector::Float("Specular strength##Summer", summerProperties.specularStrength);
	ComponentInspector::Float("Shininess##Summer", summerProperties.materialShininess);

	ComponentInspector::ColorPicker("Color##Summer", summerProperties.color);


	ComponentInspector::Text("Winter");

	ComponentInspector::Float("Height##Winter", winterProperties.height);

	ComponentInspector::Float("Wave length##Winter", winterProperties.waveLength);
	ComponentInspector::Float("Amplitude##Winter", winterProperties.amplitude);

	ComponentInspector::Float("Specular strength##Winter", winterProperties.specularStrength);
	ComponentInspector::Float("Shininess##Winter", winterProperties.materialShininess);

	ComponentInspector::ColorPicker("Color##Winter", winterProperties.color);
}

Water::Properties Water::GetWaterProperties() const
{
	const auto t = std::abs(m_EnviromentManager->season - 6.0f) / 6.0f;

	Properties p;

	p.height = glm::mix(summerProperties.height, winterProperties.height, t);

	p.waveLength = glm::mix(summerProperties.waveLength, winterProperties.waveLength, t);
	p.amplitude = glm::mix(summerProperties.amplitude, winterProperties.amplitude, t);

	p.materialShininess = glm::mix(summerProperties.materialShininess, winterProperties.materialShininess, t);
	p.specularStrength = glm::mix(summerProperties.specularStrength, winterProperties.specularStrength, t);

	p.color = glm::mix(summerProperties.color, winterProperties.color, t);

	return  p;
}
