#include "GliderController.hpp"

#include "EVA/Input.hpp"

REGISTER_COMPONENT_CPP(GilderController, "GilderController")

void GilderController::Start()
{
	m_SpeedLabel = scene->CreateUiElement<EVA::Label>("Speed:");
	m_SpeedLabel->SetAnchorAndPivot(-1.0f, -1.0f); // Bottom left
	m_SpeedLabel->SetOffsetFromAnchor(0.05f);
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

void GilderController::Update(const float deltaTime)
{
	// Turn
	if (EVA::Input::Key(EVA::Input::W))
		transform->Rotate(transform->right, -m_PitchSpeed * deltaTime);

	if (EVA::Input::Key(EVA::Input::S))
		transform->Rotate(transform->right, m_PitchSpeed * deltaTime);

	if (EVA::Input::Key(EVA::Input::A))
		transform->Rotate(transform->up, m_YawSpeed * deltaTime);

	if (EVA::Input::Key(EVA::Input::D))
		transform->Rotate(transform->up, -m_YawSpeed * deltaTime);

	if (EVA::Input::Key(EVA::Input::Q))
		transform->Rotate(transform->forward, -m_YawSpeed * deltaTime);

	if (EVA::Input::Key(EVA::Input::E))
		transform->Rotate(transform->forward, m_YawSpeed * deltaTime);

	if (EVA::Input::Key(EVA::Input::Comma))
		m_CurrentSpeed += deltaTime * (m_MaxSpeed - m_MinSpeed) / m_AccelerationTime;

	if (EVA::Input::Key(EVA::Input::Period))
		m_CurrentSpeed -= deltaTime * (m_MaxSpeed - m_MinSpeed) / m_AccelerationTime;

	m_CurrentSpeed = glm::clamp(m_CurrentSpeed, m_MinSpeed, m_MaxSpeed);
	m_SpeedLabel->SetText("Speed: " + std::to_string((int)std::roundf(100 * (m_CurrentSpeed - m_MinSpeed) / (m_MaxSpeed - m_MinSpeed))) + "%");

	// Speed
	transform->Translate(transform->forward * m_CurrentSpeed * deltaTime);
}