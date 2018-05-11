#include "EnviromentManager.hpp"

REGISTER_COMPONENT_CPP(EnviromentManager, "EnviromentManager")

void EnviromentManager::Awake()
{
	m_Sun = scene->GetLights()[0].get();
	UpdateTime();

	m_Regions.push_back({ 0.0f, 0.1f,{ 0.0f, 0.0f, 1.0f } });
	m_Regions.push_back({ 0.1f, 0.2f,{ 0.0f, 1.0f, 0.0f } });
	m_Regions.push_back({ 0.2f, 0.3f,{ 0.6f, 0.4f, 0.2f } });
	m_Regions.push_back({ 0.3f, 1.0f,{ 1.0f, 1.0f, 1.0f } });
}

void EnviromentManager::Load(const EVA::DataObject data)
{
	m_MiddayAngle = data.GetFloat("middayAngle", m_MiddayAngle);

	m_SunriseColor = data.GetVec3("sunriseColor", m_SunriseColor);
	m_MiddayColor = data.GetVec3("middayColor", m_MiddayColor);
	m_SunsetColor = data.GetVec3("sunsetColor", m_SunsetColor);
	m_NightColor = data.GetVec3("nightColor", m_NightColor);
}

void EnviromentManager::Save(EVA::DataObject& data)
{
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
		ComponentInspector::RangeFloat(("Range##" + std::to_string(i)).c_str(), m_Regions[i].minHeight, m_Regions[i].maxHeight);
		ComponentInspector::ColorPicker(("Color##" + std::to_string(i)).c_str(), m_Regions[i].color);
	}

	UpdateTime();
}

void EnviromentManager::Update(const float deltaTime)
{
	m_Time += (deltaTime * 24.0f) / m_SecondsPerDay;
	if (m_Time >= 24.0f)
		m_Time -= 24.0f;

	UpdateTime();
}

void EnviromentManager::UpdateTime() const
{
	if (m_Sun == nullptr)
		return;

	auto yaw = 0.0f;
	auto pitch = 0.0f;
	auto color = m_Sun->color;

	if (m_Time <= 6.0f) // 00-06
	{
		const auto t = m_Time / 6.0f;

		yaw = 0.0f;
		pitch = 0.0f;
		color = glm::mix(m_NightColor, m_SunriseColor, t);
	}
	else if (m_Time <= 12) // 06-12
	{
		const auto t = (m_Time - 6.0f) / 6.0f;

		yaw = glm::mix(0.0f, 90.0f, t);
		pitch = glm::mix(0.0f, m_MiddayAngle, t);
		color = glm::mix(m_SunriseColor, m_MiddayColor, t);
	}
	else if (m_Time <= 18.0f) // 12-18
	{
		const auto t = (m_Time - 12.0f) / 6.0f;

		yaw = glm::mix(90.0f, 180.0f, t);
		pitch = glm::mix(m_MiddayAngle, 0.0f, t);
		color = glm::mix(m_MiddayColor, m_SunsetColor, t);
	}
	else // 18-24
	{
		const auto t = (m_Time - 18.0f) / 6.0f;

		yaw = 180.0f;
		pitch = 0.0f;
		color = glm::mix(m_SunsetColor, m_NightColor, t);
	}

	m_Sun->SetRotation({ pitch, yaw });
	m_Sun->color = color;
}
