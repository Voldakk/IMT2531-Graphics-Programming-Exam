#include "CameraController.hpp"

#include "EVA/Input.hpp"

REGISTER_COMPONENT_CPP(CameraController, "CameraController")

void CameraController::Start()
{
	m_FreeLook = gameObject->GetComponentOfType<EVA::FreeLook>();
	m_FollowTarget = gameObject->GetComponentOfType<EVA::FollowTarget>();
}

void CameraController::Update(float deltaTime)
{
	if(EVA::Input::KeyDown(EVA::Input::Slash))
	{
		m_CurrentCamera++;
		if (m_CurrentCamera == Last)
			m_CurrentCamera = 0;

		switch (m_CurrentCamera)
		{
		case FreeLook:
			m_FreeLook->SetActive(true);
			m_FollowTarget->SetActive(false);
			break;
		case Follow:
		default:
			m_FreeLook->SetActive(false);
			m_FollowTarget->SetActive(true);
			break;;
		}
	}
}
