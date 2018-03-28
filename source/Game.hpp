#pragma once

#include "EVA.hpp"

class Game : public EVA::Scene
{
public:

    Game();

    /// <summary>Called every frame.</summary>
    /// <param name="deltaTime">The time in seconds between frames</param>
    void Update(float deltaTime) override;
};
