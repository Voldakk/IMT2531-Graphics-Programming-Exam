#pragma once

#include "Collider.hpp"

namespace EVA
{

	class BoxCollider : public Collider
	{
		REGISTER_COMPONENT_HPP(BoxCollider)

	public:


		BoxCollider() = default;
		explicit BoxCollider(Bounds bounds);

		void Load(DataObject data) override;
		Bounds GetBounds() override;
	};

}