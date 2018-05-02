#include "FollowTarget.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(FollowTarget, "EVA::FollowTarget")

	void FollowTarget::Update(const float deltaTime)
	{
		if (target == nullptr)
			return;

		transform->SetOrientation(glm::toQuat(glm::lookAtLH(transform->position, target->position, YAXIS)));


		const auto targetPos = target->position + target->LocalToWorld(offset);
		transform->SetPosition(glm::mix(transform->position, targetPos, deltaTime * scalar));
	}

	void FollowTarget::Load(const DataObject data)
	{
		offset = data.GetVec3("offset", glm::vec3(0.0f));
		scalar = data.GetFloat("scalar", 1.0f);
	}
}
