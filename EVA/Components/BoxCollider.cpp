#include "BoxCollider.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(BoxCollider, "EVA::BoxCollider")

	BoxCollider::BoxCollider(const Bounds bounds)
	{
		this->bounds = bounds;
	}

	void BoxCollider::Load(const DataObject data)
	{
		const auto center = data.GetVec3("center", bounds.center);
		const auto extents = data.GetVec3("extents", bounds.extents);

		bounds = Bounds(center, extents);
	}

	Bounds BoxCollider::GetBounds()
	{
		return Bounds(bounds.center + transform->position, bounds.extents * transform->scale);
	}
}
