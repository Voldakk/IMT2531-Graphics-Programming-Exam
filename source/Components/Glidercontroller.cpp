#include "GliderController.hpp"

#include "EVA/Input.hpp"

REGISTER_COMPONENT_CPP(GilderController, "GilderController")

void GilderController::Start()
{

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

	// Speed
	transform->Translate(transform->forward * m_CurrentSpeed * deltaTime);
}
