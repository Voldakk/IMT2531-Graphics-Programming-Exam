#include "EnviromentManager.hpp"
#include "../../EVA/Input.hpp"

REGISTER_COMPONENT_CPP(EnviromentManager, "EnviromentManager")

void EnviromentManager::Awake()
{
	m_Sun = scene->GetLights()[0].get();
	UpdateTime();

	m_Regions.push_back({ 
		0.0f, 0.1f, 
		0.0f, 0.1f,
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f } });
	m_Regions.push_back({ 
		0.1f, 0.2f,
		0.1f, 0.2f,
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f } });
	m_Regions.push_back({ 
		0.2f, 0.3f,
		0.2f, 0.3f,
		{ 0.6f, 0.4f, 0.2f },
		{ 0.6f, 0.4f, 0.2f } });
	m_Regions.push_back({ 
		0.3f, 0.5f,
		0.3f, 0.5f,
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f } });

	// Labels
	m_TimeLabel = scene->CreateUiElement<EVA::Label>("Time:");
	m_TimeLabel->SetAnchorAndPivot(-1.0f, 1.0f); // Top left
	m_TimeLabel->SetOffsetFromAnchor(0.05f);

	m_SeasonLabel = scene->CreateUiElement<EVA::Label>("Month:");
	m_SeasonLabel->SetAnchorAndPivot(1.0f, 1.0f); // Top right
	m_SeasonLabel->SetOffsetFromAnchor(0.05f);
}

void EnviromentManager::Load(const EVA::DataObject data)
{
	m_SecondsPerDay = data.GetFloat("secondsPerDay", m_SecondsPerDay);
	m_SecondsPerYear = data.GetFloat("secondsPerYear", m_SecondsPerYear);

	m_MiddayAngle = data.GetFloat("middayAngle", m_MiddayAngle);

	m_SunriseColor = data.GetVec3("sunriseColor", m_SunriseColor);
	m_MiddayColor = data.GetVec3("middayColor", m_MiddayColor);
	m_SunsetColor = data.GetVec3("sunsetColor", m_SunsetColor);
	m_NightColor = data.GetVec3("nightColor", m_NightColor);
}

void EnviromentManager::Save(EVA::DataObject& data)
{
	data.SetFloat("secondsPerDay", m_SecondsPerDay);
	data.SetFloat("secondsPerYear", m_SecondsPerYear);

	data.SetFloat("middayAngle", m_MiddayAngle);

	data.SetVec3("sunriseColor", m_SunriseColor);
	data.SetVec3("middayColor", m_MiddayColor);
	data.SetVec3("sunsetColor", m_SunsetColor);
	data.SetVec3("nightColor", m_NightColor);
}

void EnviromentManager::Inspector()
{
	ComponentInspector::DragFloat("Season", m_Season, 0.0f, 12.0f);
	ComponentInspector::DragFloat("Time", m_Time, 0.0f, 24.0f);

	ComponentInspector::Float("Seconds per day", m_SecondsPerDay);
	ComponentInspector::Float("Seconds per year", m_SecondsPerYear);

	ComponentInspector::Float("Day length summer", m_DayLengthSummer);
	ComponentInspector::Float("Day length winter", m_DayLengthWinter);

	ComponentInspector::Float("Midday angle", m_MiddayAngle);

	ComponentInspector::ColorPicker("Sunrise color", m_SunriseColor);
	ComponentInspector::ColorPicker("Midday color", m_MiddayColor);
	ComponentInspector::ColorPicker("Sunset color", m_SunsetColor);
	ComponentInspector::ColorPicker("Night color", m_NightColor);

	ComponentInspector::Text("Regions");

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
		daylength = glm::mix(m_DayLengthWinter, m_DayLengthSummer, season / 6.0f);
	else
		daylength = glm::mix(m_DayLengthSummer, m_DayLengthWinter, (season - 6.0f) / 6.0f);

	const auto nightLength = 24.0f - daylength;

	const auto dayTransitionLength = daylength / 2.0f;
	const auto nightTransitionLength = nightLength / 2.0f;

	auto yaw = 0.0f;
	auto pitch = 0.0f;
	auto color = m_Sun->color;

	if (m_Time <= nightTransitionLength) // Midnight to sunrise
	{
		const auto t = m_Time / nightTransitionLength;

		yaw = 0.0f;
		pitch = glm::mix(-90.0f, 0.0f, t);
		color = glm::mix(m_NightColor, m_SunriseColor, t);
	}
	else if (m_Time <= 12) // Sunrise to noon
	{
		const auto t = (m_Time - nightTransitionLength) / dayTransitionLength;

		yaw = glm::mix(0.0f, 90.0f, t);
		pitch = glm::mix(0.0f, m_MiddayAngle, t);
		color = glm::mix(m_SunriseColor, m_MiddayColor, t);
	}
	else if (m_Time <= 12 + dayTransitionLength) // Noon to sunset
	{
		const auto t = (m_Time - 12.0f) / dayTransitionLength;

		yaw = glm::mix(90.0f, 180.0f, t);
		pitch = glm::mix(m_MiddayAngle, 0.0f, t);
		color = glm::mix(m_MiddayColor, m_SunsetColor, t);
	}
	else // Sunset to midnight
	{
		const auto t = (m_Time - (12.0f + dayTransitionLength)) / nightTransitionLength;

		yaw = 180.0f;
		pitch = glm::mix(0.0f, -90.0f, t);
		color = glm::mix(m_SunsetColor, m_NightColor, t);
	}

	m_Sun->SetRotation({ pitch, yaw });
	m_Sun->color = color;
}
