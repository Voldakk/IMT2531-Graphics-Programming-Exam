#pragma once

#include "EVA.hpp"

namespace EVA
{

	class HideOnStart : public Component, public IUpdateComponent
	{
		REGISTER_COMPONENT_HPP(HideOnStart);

	public:
		void Update(float deltaTime) override;
	};

}