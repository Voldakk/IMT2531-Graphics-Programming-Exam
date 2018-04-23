#pragma once

#include <functional>

#include "Label.hpp"

namespace EVA
{

	class Button : public Label
	{
		enum State { Normal, Highlighted };
		
		State m_State;

		std::unique_ptr<SpriteRenderer> m_Sr;

	public:

		std::function<void()> onClick;

		explicit Button(const std::string& text, float textScale = 0.001f);

		void Update() override;
		void Render() const override;
	};

}
