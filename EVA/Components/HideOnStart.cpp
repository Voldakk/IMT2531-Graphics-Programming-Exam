#include "HideOnStart.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(HideOnStart, "EVA::HideOnStart")

	void HideOnStart::Update(float deltaTime)
	{
		gameObject->SetActive(false);
	}
}
