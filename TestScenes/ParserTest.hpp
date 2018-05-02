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
			// Skybox
			skybox = std::make_unique<EVA::Skybox>("./assets/ame_ash/", ".tga");

			// A directional light that fits the skybox
			auto directionalLight = CreateLight(EVA::LightType::Directional, true);
			directionalLight->SetRotation({ 60.0f, -80.0f });
			directionalLight->directionalShadowDistance = 25.0f;
			directionalLight->color = { 1.0f, 1.0f, 1.0f };

			// Camera
			auto camera = EVA::PrefabParser::Load(this, "./assets/prefabs/freelookcamera.prefab");

			// Cube
			auto cube = EVA::PrefabParser::Load(this, "./assets/prefabs/uvcube.prefab");
		}
	};

}
