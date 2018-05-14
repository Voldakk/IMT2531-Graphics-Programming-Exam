#include "HideOnStart.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(HideOnStart, "EVA::HideOnStart")

	void HideOnStart::Update(float deltaTime)
	{
		const auto mrs = gameObject->GetComponentsOfType<MeshRenderer>();

		for (auto& mr : mrs)
		{
			mr->SetActive(false);
		}
	}
}
