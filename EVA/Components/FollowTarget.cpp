#include "FollowTarget.hpp"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(FollowTarget, "EVA::FollowTarget")

	void FollowTarget::Start()
	{
		const auto go = scene->FindGameObjectByName(m_TargetName);
		if (go != nullptr)
			target = go->transform.get();
	}

	void FollowTarget::Update(const float deltaTime)
	{
		if (target == nullptr)
			return;

		transform->SetOrientation(target->orientation);


		const auto targetPos = target->position + target->LocalToWorld(offset);
		transform->SetPosition(glm::mix(transform->position, targetPos, deltaTime * scalar));
	}

	void FollowTarget::Load(const DataObject data)
	{
		offset = data.GetVec3("offset", offset);
		scalar = data.GetFloat("scalar", scalar);
		m_TargetName = data.GetString("targetName", m_TargetName);
	}

	void FollowTarget::Save(DataObject& data)
	{
		data.SetVec3("offset", offset);
		data.SetFloat("scalar", scalar);
		data.SetString("targetName", m_TargetName);
	}

	void FollowTarget::Inspector()
	{
		ComponentInspector::Float3("Offset", offset);
		ComponentInspector::Float("Scalar", scalar);
		ComponentInspector::EnterString("Target", m_TargetName);
	}
}
