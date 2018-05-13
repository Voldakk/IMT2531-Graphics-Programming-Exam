#include "EnviromentManager.hpp"

#include "EVA/Input.hpp"

#include "Terrain.hpp"

REGISTER_COMPONENT_CPP(EnviromentManager, "EnviromentManager")

void EnviromentManager::Start()
{
	m_Sun = scene->GetLights()[0].get();
	UpdateTime();

	// Labels
	m_TimeLabel = scene->CreateUiElement<EVA::Label>("Time:");
	m_TimeLabel->SetAnchorAndPivot(-1.0f, 1.0f); // Top left
	m_TimeLabel->SetOffsetFromAnchor(0.05f);

	m_SeasonLabel = scene->CreateUiElement<EVA::Label>("Month:");
	m_SeasonLabel->SetAnchorAndPivot(1.0f, 1.0f); // Top right
	m_SeasonLabel->SetOffsetFromAnchor(0.05f);

	m_Terrain = gameObject->GetComponentOfType<Terrain>();

	PlaceTrees();
}

void EnviromentManager::Load(const EVA::DataObject data)
{
	m_SecondsPerDay = data.GetFloat("secondsPerDay", m_SecondsPerDay);
	m_SecondsPerYear = data.GetFloat("secondsPerYear", m_SecondsPerYear);

	m_DayLengthSummer = data.GetFloat("dayLengthSummer", m_DayLengthSummer);
	m_DayLengthWinter = data.GetFloat("dayLengthWinter", m_DayLengthWinter);
	m_TransitionLength = data.GetFloat("transitionLength", m_TransitionLength);

	m_MiddayAngle = data.GetFloat("middayAngle", m_MiddayAngle);

	m_SunriseColor = data.GetVec3("sunriseColor", m_SunriseColor);
	m_MiddayColor = data.GetVec3("middayColor", m_MiddayColor);
	m_SunsetColor = data.GetVec3("sunsetColor", m_SunsetColor);
	m_NightColor = data.GetVec3("nightColor", m_NightColor);

	// Regions
	regionBlendAmount = data.GetFloat("regionBlendAmount", regionBlendAmount);

	if (data.json.HasMember("regions") && data.json["regions"].IsArray())
	{
		auto regions = data.json["regions"].GetArray();
		m_Regions.resize(regions.Size());

		for (size_t i = 0; i < regions.Size(); i++)
		{
			EVA::DataObject regionData(regions[i]);
			m_Regions[i].minHeightSummer = regionData.GetFloat("minHeightSummer", 0.0f);
			m_Regions[i].maxHeightSummer = regionData.GetFloat("maxHeightSummer", 0.0f);
			m_Regions[i].colorSummer = regionData.GetVec3("colorSummer", glm::vec3(1.0f));

			m_Regions[i].minHeightWinter = regionData.GetFloat("minHeightWinter", 0.0f);
			m_Regions[i].maxHeightWinter = regionData.GetFloat("maxHeightWinter", 0.0f);
			m_Regions[i].colorWinter = regionData.GetVec3("colorWinter", glm::vec3(1.0f));

			m_Regions[i].textureTiling = regionData.GetFloat("textureTiling", 1.0f);

			m_Regions[i].diffuseTexture = EVA::TextureManager::LoadTexture(regionData.GetString("diffuseTexture", ""));

			m_Regions[i].treeDensity = regionData.GetFloat("treeDensity", m_Regions[i].treeDensity);
			m_Regions[i].treeName = regionData.GetString("treeName", m_Regions[i].treeName);
		}
	}
}

void EnviromentManager::Save(EVA::DataObject& data)
{
	data.SetFloat("secondsPerDay", m_SecondsPerDay);
	data.SetFloat("secondsPerYear", m_SecondsPerYear);

	data.SetFloat("dayLengthSummer", m_DayLengthSummer);
	data.SetFloat("dayLengthWinter", m_DayLengthWinter);
	data.SetFloat("transitionLength", m_TransitionLength);

	data.SetFloat("middayAngle", m_MiddayAngle);

	data.SetVec3("sunriseColor", m_SunriseColor);
	data.SetVec3("middayColor", m_MiddayColor);
	data.SetVec3("sunsetColor", m_SunsetColor);
	data.SetVec3("nightColor", m_NightColor);

	// Regions
	data.SetFloat("regionBlendAmount", regionBlendAmount);

	EVA::Json::Value regionsArray;
	regionsArray.SetArray();

	for (auto& region : m_Regions)
	{
		// Create a data object
		EVA::Json::Value regionValue;
		regionValue.SetObject();
		EVA::DataObject regionData(regionValue, data.allocator);

		// Set values
		regionData.SetFloat("minHeightSummer", region.minHeightSummer);
		regionData.SetFloat("maxHeightSummer", region.maxHeightSummer);
		regionData.SetVec3("colorSummer", region.colorSummer);

		regionData.SetFloat("minHeightWinter", region.minHeightWinter);
		regionData.SetFloat("maxHeightWinter", region.maxHeightWinter);
		regionData.SetVec3("colorWinter", region.colorWinter);

		regionData.SetFloat("textureTiling", region.textureTiling);
		regionData.SetPath("diffuseTexture", region.diffuseTexture->path);

		regionData.SetFloat("treeDensity", region.treeDensity);
		regionData.SetString("treeName", region.treeName);

		// Add to array
		regionsArray.PushBack(regionValue, *data.allocator);
	}

	data.json.AddMember("regions", regionsArray, *data.allocator);
}

void EnviromentManager::Inspector()
{
	ComponentInspector::DragFloat("Season", m_Season, 0.0f, 12.0f);
	ComponentInspector::DragFloat("Time", m_Time, 0.0f, 24.0f);

	ComponentInspector::Float("Seconds per day", m_SecondsPerDay);
	ComponentInspector::Float("Seconds per year", m_SecondsPerYear);

	ComponentInspector::Float("Day length summer", m_DayLengthSummer);
	ComponentInspector::Float("Day length winter", m_DayLengthWinter);
	ComponentInspector::Float("Transition length", m_TransitionLength);

	ComponentInspector::Float("Midday angle", m_MiddayAngle);

	ComponentInspector::ColorPicker("Sunrise color", m_SunriseColor);
	ComponentInspector::ColorPicker("Midday color", m_MiddayColor);
	ComponentInspector::ColorPicker("Sunset color", m_SunsetColor);
	ComponentInspector::ColorPicker("Night color", m_NightColor);

	ComponentInspector::Text("Regions");

	ComponentInspector::Float("Region blend amount", regionBlendAmount);

	int numRegions = m_Regions.size();
	if (ComponentInspector::EnterInt("Number of regions", numRegions))
		m_Regions.resize(numRegions);


	for (unsigned int i = 0; i < m_Regions.size(); ++i)
	{
		ComponentInspector::Text(("Region #" + std::to_string(i)).c_str());
		ComponentInspector::RangeFloat(("Range summer##" + std::to_string(i)).c_str(), m_Regions[i].minHeightSummer, m_Regions[i].maxHeightSummer);
		ComponentInspector::RangeFloat(("Range winter##" + std::to_string(i)).c_str(), m_Regions[i].minHeightWinter, m_Regions[i].maxHeightWinter);
		ComponentInspector::ColorPicker(("Color summer##" + std::to_string(i)).c_str(), m_Regions[i].colorSummer);
		ComponentInspector::ColorPicker(("Color winter##" + std::to_string(i)).c_str(), m_Regions[i].colorWinter);

		ComponentInspector::Float(("Texture tiling##" + std::to_string(i)).c_str(), m_Regions[i].textureTiling);

		auto path = m_Regions[i].diffuseTexture == nullptr ? "" : EVA::FileSystem::ToString(m_Regions[i].diffuseTexture->path);
		if (ComponentInspector::DragDropTargetString("Diffuse texture", path, "file"))
		{
			m_Regions[i].diffuseTexture = EVA::TextureManager::LoadTexture(path);
		}

		auto name = m_Regions[i].treeName;
		if (ComponentInspector::EnterString(("Tree name##" + std::to_string(i)).c_str(), name))
		{
			m_Regions[i].treeName = name;
		}

		ComponentInspector::Float(("Tree density##" + std::to_string(i)).c_str(), m_Regions[i].treeDensity);
	}

	if(ComponentInspector::Button("Place trees"))
	{
		PlaceTrees();
	}

	UpdateTime();
}

void EnviromentManager::Update(const float deltaTime)
{
	// Season controls
	if (EVA::Input::KeyDown(EVA::Input::Alpha1))
		m_Season = 3.0f;
	if (EVA::Input::KeyDown(EVA::Input::Alpha2))
		m_Season = 6.0f;
	if (EVA::Input::KeyDown(EVA::Input::Alpha3))
		m_Season = 9.0f;
	if (EVA::Input::KeyDown(EVA::Input::Alpha4))
		m_Season = 0.0f;
	if (EVA::Input::KeyDown(EVA::Input::Alpha5))
		m_SeasonPaused = !m_SeasonPaused;

	// Time controls
	if (EVA::Input::KeyDown(EVA::Input::Alpha6))
		m_Time = 6.0f;
	if (EVA::Input::KeyDown(EVA::Input::Alpha7))
		m_Time = 12.0f;
	if (EVA::Input::KeyDown(EVA::Input::Alpha8))
		m_Time = 18.0f;
	if (EVA::Input::KeyDown(EVA::Input::Alpha9))
		m_Time = 0.0f;
	if (EVA::Input::KeyDown(EVA::Input::Alpha0))
		m_TimePaused = !m_TimePaused;

	// Update
	if (!m_SeasonPaused)
	{
		m_Season += (deltaTime * 12.0f) / m_SecondsPerYear;
		if (m_Season >= 12.0f)
			m_Season -= 12.0f;
	}
	
	if (!m_TimePaused)
	{
		m_Time += (deltaTime * 24.0f) / m_SecondsPerDay;
		if (m_Time >= 24.0f)
			m_Time -= 24.0f;
	}

	UpdateTime();

	// Labels
	m_TimeLabel->SetText("Time: " + std::to_string((int)ceilf(m_Time)));
	m_SeasonLabel->SetText("Month: " + std::to_string((int)ceilf(m_Season)));
}

void EnviromentManager::UpdateTime() const
{
	if (m_Sun == nullptr)
		return;

	auto daylength = 0.0f;
	if (season <= 6)
		daylength = glm::mix(m_DayLengthWinter, m_DayLengthSummer, season / 6.0f) - -m_TransitionLength;
	else
		daylength = glm::mix(m_DayLengthSummer, m_DayLengthWinter, (season - 6.0f) / 6.0f) - -m_TransitionLength;

	const auto nightLength = 24.0f - daylength - m_TransitionLength * 2.0f;

	const auto halfDayLength = daylength / 2.0f;
	const auto halfNightLength = nightLength / 2.0f;

	const auto degPerHour = 180.0f / (24.0f - nightLength);

	const auto yaw = glm::mix(0.0f, 180.0f, m_Time / 24.0f);
	auto pitch = 0.0f;
	auto color = m_Sun->color;

	if (m_Time <= halfNightLength) // Night
	{
		const auto t = m_Time / halfNightLength;

		pitch = glm::mix(-90.0f, 0.0f, t);
		color = m_NightColor;
	}
	else if (m_Time >= 24 - halfNightLength) // Night
	{
		const auto t = (m_Time - (24 - halfNightLength)) / halfNightLength;

		pitch = glm::mix(180.0f, 270.0f, t);
		color = m_NightColor;
	}
	else if (m_Time >= 12 - halfDayLength && m_Time <= 12) // Day
	{
		const auto t = (m_Time - (12 - halfDayLength)) / halfDayLength;

		pitch = glm::mix(m_TransitionLength * degPerHour, m_MiddayAngle, t);
		color = m_MiddayColor;
	}
	else if (m_Time >= 12 && m_Time <= 12 + halfDayLength) // Day
	{
		const auto t = (m_Time - 12) / halfDayLength;

		pitch = glm::mix(m_MiddayAngle, 180.0f - m_TransitionLength * degPerHour, t);
		color = m_MiddayColor;
	}
	else if(m_Time < 12) // Sunrise
	{
		const auto t = (m_Time - halfNightLength) / m_TransitionLength;

		pitch = glm::mix(0.0f, m_TransitionLength * degPerHour, t);
		if (t < 0.5)
			color = glm::mix(m_NightColor, m_SunriseColor, t * 2.0f);
		else
			color = glm::mix(m_SunriseColor, m_MiddayColor, (t - 0.5f) * 2.0f);
	}
	else // Sunset
	{
		const auto t = (m_Time - (12 + halfDayLength)) / m_TransitionLength;

		pitch = glm::mix(180.0f - m_TransitionLength * degPerHour, 180.0f, t);
		if (t < 0.5)
			color = glm::mix(m_MiddayColor, m_SunsetColor, t * 2.0f);
		else
			color = glm::mix(m_SunsetColor, m_NightColor, (t - 0.5f) * 2.0f);
	}

	m_Sun->SetRotation({ pitch, yaw });
	m_Sun->color = color;

	if (scene->skybox != nullptr)
		scene->skybox->SetTint(color);
}

void EnviromentManager::PlaceTrees()
{
	for(auto& region : m_Regions)
	{
		if (region.treeName.empty() || region.treeDensity == 0.0f)
			continue;

		// Find the tree
		auto go = scene->FindGameObjectByName(region.treeName);
		if (go == nullptr)
			continue;

		auto treeTransform = go->transform.get();

		// Positions
		std::vector<glm::mat4> positions;

		for (size_t x = 0; x < m_Terrain->terrainWidth; x++)
		{
			for (size_t z = 0; z < m_Terrain->terrainLength; z++)
			{
				if (std::rand() % 100000 > region.treeDensity * 100000.0f)
					continue;

				const auto height = m_Terrain->HeightData(x / m_Terrain->terrainWidth, z / m_Terrain->terrainLength);

				if (height > region.MinHeight(m_Season) && height < region.MaxHeight(m_Season))
				{
					treeTransform->SetPosition(glm::vec3(x, height * m_Terrain->maxTerrainHeight, z));
					positions.push_back(treeTransform->modelMatrix);
				}
			}
		}

		// Set the material MBOs
		auto meshRenderers = go->GetComponentsOfType<EVA::MeshRenderer>();
		for (auto& mr : meshRenderers)
		{
			auto material = mr->material.get();

			material->SetMbo(mr->mesh, positions);
		}
	}
}
