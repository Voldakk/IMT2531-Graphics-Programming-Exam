#include "GliderController.hpp"

#include "EVA/Input.hpp"

REGISTER_COMPONENT_CPP(GilderController, "GilderController")

void GilderController::Start()
{
	// Label
	m_ThrottleLabel = scene->CreateUiElement<EVA::Label>("Throttle:");
	m_ThrottleLabel->SetAnchorAndPivot(-1.0f, -1.0f); // Bottom left
	m_ThrottleLabel->SetOffsetFromAnchor(0.05f);

	// Rememder start
	m_StartLocation = transform->position;
	m_StartOrientation = transform->orientation;

	// Teleport points
	auto go = scene->FindGameObjectByName("TeleportPoint1");
	auto i = 1;
	while (go != nullptr)
	{
		m_TeleportPoints.push_back(go->transform.get());

		i++;
		go = scene->FindGameObjectByName("TeleportPoint" + std::to_string(i));
	}

	// Find parts
	m_Propeller = scene->FindGameObjectByName("Propeller")->transform.get();

	m_AileronLeft = scene->FindGameObjectByName("AileronLeft")->transform.get();
	m_AileronRight = scene->FindGameObjectByName("AileronRight")->transform.get();

	m_Profondeur = scene->FindGameObjectByName("Profondeur")->transform.get();
	m_Direction = scene->FindGameObjectByName("Direction")->transform.get();

	m_Stick = scene->FindGameObjectByName("Stick")->transform.get();
	m_Throtle = scene->FindGameObjectByName("Throttle")->transform.get();
	m_MaxThrottlePositionZ = m_Throtle->localPosition.z;
}

void GilderController::Update(const float deltaTime)
{
	auto pitch = 0.0f;
	auto yaw = 0.0f;
	auto roll = 0.0f;

	// Turn
	if (EVA::Input::Key(EVA::Input::W))
		pitch--;

	if (EVA::Input::Key(EVA::Input::S))
		pitch++;

	if (EVA::Input::Key(EVA::Input::Q))
		yaw++;

	if (EVA::Input::Key(EVA::Input::E))
		yaw--;

	if (EVA::Input::Key(EVA::Input::A))
		roll--;

	if (EVA::Input::Key(EVA::Input::D))
		roll++;

	transform->Rotate(transform->right, pitch * m_PitchSpeed * deltaTime);
	transform->Rotate(transform->up, yaw * m_YawSpeed * deltaTime);
	transform->Rotate(transform->forward, roll * m_RollSpeed * deltaTime);

	m_Stick->SetOrientation(-pitch * 30.0f, 0.0f, roll * 30.0f);

	m_Direction->SetOrientation(EVA::YAXIS, -yaw * 30.0f);
	m_Profondeur->SetOrientation(EVA::XAXIS, pitch * 30.0f);

	m_AileronLeft->SetOrientation(EVA::XAXIS, -pitch * 30.0f - roll * 30.0f);
	m_AileronRight->SetOrientation(EVA::XAXIS, -pitch * 30.0f + roll * 30.0f);

	// Speed
	if (EVA::Input::Key(EVA::Input::Period))
		m_CurrentSpeed += deltaTime * (m_MaxSpeed - m_MinSpeed) / m_AccelerationTime;

	if (EVA::Input::Key(EVA::Input::Comma))
		m_CurrentSpeed -= deltaTime * (m_MaxSpeed - m_MinSpeed) / m_AccelerationTime;

	m_CurrentSpeed = glm::clamp(m_CurrentSpeed, m_MinSpeed, m_MaxSpeed);

	const auto speedPersent = 100 * (m_CurrentSpeed - m_MinSpeed) / (m_MaxSpeed - m_MinSpeed);
	m_ThrottleLabel->SetText("Throttle: " + std::to_string((int)std::roundf(speedPersent)) + "%");

	transform->Translate(transform->forward * m_CurrentSpeed * deltaTime);

	m_Throtle->SetPosition({ 0.0f, 0.0f, glm::mix(0.0f, m_MaxThrottlePositionZ, speedPersent / 100.0f) });
	m_Propeller->Rotate(EVA::ZAXIS, m_CurrentSpeed * deltaTime * 100.0f);

	// Reset
	if (EVA::Input::Key(EVA::Input::R))
	{
		transform->SetPosition(m_StartLocation);
		transform->SetOrientation(m_StartOrientation);
		m_CurrentSpeed = m_MinSpeed;
	}

	// Teleport
	if(m_TeleportPoints.empty())
		return;

	if(EVA::Input::KeyDown(EVA::Input::F))
	{
		m_CurrentTeleportPoint++;
		if (m_CurrentTeleportPoint >= m_TeleportPoints.size())
			m_CurrentTeleportPoint = 0;

		transform->SetPosition(m_TeleportPoints[m_CurrentTeleportPoint]->position);
		transform->SetOrientation(m_TeleportPoints[m_CurrentTeleportPoint]->orientation);
	}

}

void GilderController::Load(const EVA::DataObject data)
{

}

void GilderController::Save(EVA::DataObject& data)
{

}

void GilderController::Inspector()
{

}