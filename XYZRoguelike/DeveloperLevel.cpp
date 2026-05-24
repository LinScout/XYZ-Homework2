#include "DeveloperLevel.h"
#include "Wall.h"
#include "Enemy.h"
#include "GameLog.h"

using namespace XYZEngine;

namespace XYZRoguelike
{
	void DeveloperLevel::Start()
	{
		GAME_LOG_INFO("DeveloperLevel: building level");

		int width = 15;
		int height = 15;

		for (int y = 0; y < height + 1; y++)
		{
			for (int x = 0; x < width + 1; x++)
			{
				if (x != 0 && x != width && y != 0 && y != height)
				{
					floors.push_back(std::make_unique<Floor>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), std::forward<int>(0)));
				}

				if (x == 0 && y == 0)
				{
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), std::forward<int>(25)));
				}

				if (x == width && y == 0)
				{
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), std::forward<int>(27)));
				}

				if (x == 0 && y == height)
				{
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), std::forward<int>(1)));
				}

				if (x == width && y == height)
				{
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), std::forward<int>(3)));
				}

				if (x == 0 && y != height && y != 0)
				{
					floors.push_back(std::make_unique<Floor>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), std::forward<int>(18)));
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), std::forward<int>(12)));
				}

				if (x == width && y != height && y != 0)
				{
					floors.push_back(std::make_unique<Floor>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), std::forward<int>(19)));
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), std::forward<int>(12)));
				}

				if (y == 0 && x != width && x != 0)
				{
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), std::forward<int>(38)));
				}

				if (y == height && x != width && x != 0)
				{
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), std::forward<int>(38)));
				}
			}
		}

		GAME_LOG_INFO("DeveloperLevel: spawning player with CombatStatsComponent and AttackComponent");
		player = std::make_shared<Player>(std::forward<XYZEngine::Vector2Df>({ width / 2 * 128.f, height / 2 * 128.f }));

		GAME_LOG_INFO("DeveloperLevel: spawning enemy with CombatStatsComponent and AttackComponent");
		enemy = std::make_unique<Enemy>(
			std::forward<XYZEngine::Vector2Df>({ (width / 2 + 3) * 128.f, (height / 2 + 3) * 128.f })
		);

		music = std::make_unique<Music>("music");
		GAME_LOG_INFO("DeveloperLevel: level ready");
	}

	void DeveloperLevel::Restart()
	{
		GAME_LOG_WARN("DeveloperLevel: restart requested");
		Stop();
		Start();
		GAME_LOG_INFO("DeveloperLevel: restart completed");
	}

	void DeveloperLevel::Stop()
	{
		GAME_LOG_INFO("DeveloperLevel: stopping level");
		GameWorld::Instance()->Clear();
		player.reset();
		enemy.reset();
		music.reset();
		walls.clear();
		floors.clear();
	}

	void DeveloperLevel::DrawHud(sf::RenderWindow& window)
	{
		if (!combatHud.IsReady())
		{
			return;
		}
		combatHud.Draw(window, player.get(), enemy.get());
	}
}
