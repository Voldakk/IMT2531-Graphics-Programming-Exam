#pragma once

#include <utility>
#include <vector>

#include "EVA.hpp"

#include "Ghost.hpp"

/**
 * \brief Follows Pacman
 */
class GhostShadow : public Ghost
{
public:

	void Start() override
	{
		Ghost::Start();

		SetColor({ 1.0f, 0.0f, 0.0f });
		m_ScatterTile = game->tileMap->GetUniqueTile('B');

	}

protected:

	Direction ChooseChaseTarget(const std::vector<Ghost::Direction>& directions) const override
	{
		return DirectionToTarget(directions, m_Pacman->currentTile);
	}
};

/**
* \brief Tries to move in front Pacman
*/
class GhostSpeedy : public Ghost
{
public:

	void Start() override
	{
		Ghost::Start();

		SetColor({ 0.85f, 0.35f, 0.85f });
		m_ScatterTile = game->tileMap->GetUniqueTile('S');

	}

private:

	Direction ChooseChaseTarget(const std::vector<Ghost::Direction>& directions) const override
	{
		return DirectionToTarget(directions, m_Pacman->currentTile + m_Pacman->currentDirection * 4);
	}
};

/**
* \brief Targets the tile opposite of Pacman in relation to Shadow
*/
class GhostBashful : public Ghost
{

public:
	
	GhostShadow* shadow;

	void Start() override
	{
		Ghost::Start();

		SetColor({ 0.5f, 0.9f, 0.9f });
		m_ScatterTile = game->tileMap->GetUniqueTile('I');

	}

private:

	Direction ChooseChaseTarget(const std::vector<Ghost::Direction>& directions) const override
	{
		if (shadow != nullptr)
		{
			const auto pacmanFront = m_Pacman->currentTile + m_Pacman->currentDirection * 2;
			const auto shadowToPacmanFront = pacmanFront - shadow->currentTile;

			return DirectionToTarget(directions, pacmanFront + shadowToPacmanFront);
		}
		return DirectionToTarget(directions, m_Pacman->currentTile);
	}
};

/**
* \brief Normaly follows Pacman. Scatters when closer than 8 tiles
*/
class GhostPokey : public Ghost
{
public:

	void Start() override
	{
		Ghost::Start();

		SetColor({ 0.95f, 0.8f, 0.3f });
		m_ScatterTile = game->tileMap->GetUniqueTile('C');

	}

private:

	Direction ChooseChaseTarget(const std::vector<Ghost::Direction>& directions) const override
	{
		if (glm::distance(glm::vec2(m_CurrentTile.x, m_CurrentTile.y), glm::vec2(m_Pacman->currentTile.x, m_Pacman->currentTile.y)) > 8)
			return DirectionToTarget(directions, m_Pacman->currentTile);
			
		return DirectionToTarget(directions, m_ScatterTile);
	}
};