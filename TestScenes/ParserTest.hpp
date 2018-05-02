#pragma once

#include "EVA.hpp"
#include "EVA/Parsers.hpp"


namespace EVA_TEST
{
	class ParserTest : public EVA::Scene
	{

	public:

		ParserTest()
		{
			// A directional light that fits the skybox
			auto directionalLight = CreateLight(EVA::LightType::Directional, true);
			directionalLight->SetRotation({ 60.0f, -80.0f });
			directionalLight->directionalShadowDistance = 25.0f;
			directionalLight->color = { 1.0f, 1.0f, 1.0f };

			EVA::SceneParser::Load(this, "./assets/scenes/cubes.scene");
		}
	};

}
