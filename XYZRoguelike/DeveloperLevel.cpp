#include "DeveloperLevel.h"
#include "Wall.h"
#include "Enemy.h"
#include "GameLog.h"
#include "GameWorld.h"
#include "MovementComponent.h"
#include "TransformComponent.h"
#include "EnemyMovementComponent.h"
#include "randomizer.h"
#include <cmath>
#include <sstream>

using namespace XYZEngine;

namespace XYZRoguelike
{
	namespace
	{
		constexpr float kBoostMultiplier = 1.5f;
		constexpr float kHealAmount = 25.f;
		constexpr float kBossSpeed = 180.f;
		constexpr float kBossRange = 500.f;
	}

	void DeveloperLevel::Start()
	{
		GAME_LOG_INFO("DeveloperLevel: building roguelike levels");
		ClearScene();
		currentLevelIndex = 0;
		bossDefeated = false;
		gameCompleted = false;
		boostActive = false;
		boostClock.restart();
		hudStatus = "Defeat the boss to unlock the next location.";
		currentLocation = "Unknown";

		inventory = Inventory();
		inventory.AddItem(InventoryItemType::HealthPotion, 1);
		inventory.AddItem(InventoryItemType::BallBoost, 1);

		levels =
		{
			{
				"Ruins Entrance",
				{ 2.f * 128.f, 6.f * 128.f },
				{ 8.f * 128.f, 3.f * 128.f },
				{ 10.f * 128.f, 2.f * 128.f },
				120.f,
				12,
				9,
				{}
			},
			{
				"Cave Hall",
				{ 2.f * 128.f, 2.f * 128.f },
				{ 7.f * 128.f, 5.f * 128.f },
				{ 12.f * 128.f, 8.f * 128.f },
				120.f,
				14,
				10,
				{{5, 4}, {5, 5}, {9, 5}, {9, 4}, {7, 7}}
			},
			{
				"Crystal Vault",
				{ 1.5f * 128.f, 5.f * 128.f },
				{ 9.f * 128.f, 4.f * 128.f },
				{ 10.5f * 128.f, 8.f * 128.f },
				120.f,
				12,
				12,
				{{3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}}
			}
		};

		BuildLevel(levels[currentLevelIndex]);
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
		ClearScene();
		music.reset();
		player.reset();
		boss.reset();
		walls.clear();
		floors.clear();
	}

	void DeveloperLevel::BuildLevel(const LevelTemplate& level)
	{
		currentLocation = level.name;
		bossDefeated = false;
		boostActive = false;
		boostClock.restart();
		lastActionMessage = "";

		AddBorderTiles(level.width, level.height);
		AddExtraWalls(level.extraWalls);

		player = std::make_shared<Player>(level.playerStart);
		auto* movement = player->GetGameObject()->GetComponent<XYZEngine::MovementComponent>();
		if (movement != nullptr)
		{
			movement->SetSpeed(basePlayerSpeed);
		}
		player->GetCombatStats()->RestoreToFull();

		boss = std::make_unique<Enemy>(level.bossStart, "Boss");
		ConfigureBoss(boss.get());

		UpdateHudText();
	}

	void DeveloperLevel::ClearScene()
	{
		GameWorld::Instance()->Clear();
		boss.reset();
		player.reset();
		walls.clear();
		floors.clear();
		boostActive = false;
		healKeyPressed = false;
		boostKeyPressed = false;
		nextLevelKeyPressed = false;
		restartKeyPressed = false;
	}

	void DeveloperLevel::HandleInput()
	{
		if (player == nullptr)
		{
			return;
		}

		const bool healHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::H);
		if (healHeld && !healKeyPressed)
		{
			UseHealthPotion();
		}
		healKeyPressed = healHeld;

		const bool boostHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::B);
		if (boostHeld && !boostKeyPressed)
		{
			UseSpeedBoost();
		}
		boostKeyPressed = boostHeld;

		const bool nextLevelHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::N);
		if (nextLevelHeld && !nextLevelKeyPressed && bossDefeated)
		{
			AdvanceLevel();
		}
		nextLevelKeyPressed = nextLevelHeld;

		const bool restartHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::R);
		if (restartHeld && !restartKeyPressed)
		{
			Restart();
		}
		restartKeyPressed = restartHeld;
	}

	void DeveloperLevel::CheckLevelProgress()
	{
		if (player == nullptr)
		{
			return;
		}

		const bool bossIsAlive = boss != nullptr
			&& boss->GetCombatStats() != nullptr
			&& boss->GetCombatStats()->IsAlive();

		if (!bossIsAlive)
		{
			if (!bossDefeated)
			{
				bossDefeated = true;
				inventory.AddItem(InventoryItemType::HealthPotion, 1);
				inventory.AddItem(InventoryItemType::BallBoost, 1);
				lastActionMessage = "Boss defeated! Loot acquired.";

				if (currentLevelIndex + 1 < static_cast<int>(levels.size()))
				{
					AdvanceLevel();
					return;
				}

				gameCompleted = true;
				lastActionMessage = "All locations cleared. Press R to restart.";
			}

			UpdateHudText();
			return;
		}

		if (boostActive && boostClock.getElapsedTime().asSeconds() >= boostDuration)
		{
			ResetSpeedBoost();
		}

		auto playerPosition = player->GetGameObject()->GetComponent<XYZEngine::TransformComponent>()->GetWorldPosition();
		const auto& level = levels[currentLevelIndex];
		const auto delta = playerPosition - level.exitPosition;
		const float distanceToExit = std::sqrt(delta.x * delta.x + delta.y * delta.y);

		if (distanceToExit <= level.exitRadius)
		{
			if (bossDefeated)
			{
				AdvanceLevel();
			}
			else
			{
				lastActionMessage = "The exit is open, but the boss must be defeated first.";
			}
		}

		UpdateHudText();
	}

	void DeveloperLevel::AdvanceLevel()
	{
		if (gameCompleted)
		{
			return;
		}

		if (currentLevelIndex + 1 >= static_cast<int>(levels.size()))
		{
			gameCompleted = true;
			lastActionMessage = "All locations cleared. Press R to restart.";
			UpdateHudText();
			return;
		}

		currentLevelIndex += 1;
		bossDefeated = false;
		boostActive = false;
		boostClock.restart();
		ClearScene();
		BuildLevel(levels[currentLevelIndex]);
		lastActionMessage = "Entering " + levels[currentLevelIndex].name;
		UpdateHudText();
	}

	void DeveloperLevel::UpdateHudText()
	{
		std::ostringstream hud;
		hud << currentLocation << " | Level " << currentLevelIndex + 1 << "/" << levels.size();
		hudStatus = hud.str();
	}

	void DeveloperLevel::UseHealthPotion()
	{
		if (!inventory.ConsumeItem(InventoryItemType::HealthPotion))
		{
			lastActionMessage = "No health potions available.";
			return;
		}

		if (player == nullptr || player->GetCombatStats() == nullptr)
		{
			return;
		}

		player->GetCombatStats()->SetHealthPoints(
			static_cast<int>(std::min<float>(player->GetCombatStats()->GetMaxHealthPoints(), player->GetCombatStats()->GetHealthPoints() + kHealAmount)));
		lastActionMessage = "Used Health Potion (+25 HP).";
		UpdateHudText();
	}

	void DeveloperLevel::UseSpeedBoost()
	{
		if (!inventory.ConsumeItem(InventoryItemType::BallBoost))
		{
			lastActionMessage = "No speed potions available.";
			return;
		}

		if (player == nullptr)
		{
			return;
		}

		auto* movement = player->GetGameObject()->GetComponent<XYZEngine::MovementComponent>();
		if (movement != nullptr)
		{
			movement->SetSpeed(basePlayerSpeed * kBoostMultiplier);
		}

		boostActive = true;
		boostClock.restart();
		lastActionMessage = "Speed boost active for 5 seconds.";
		UpdateHudText();
	}

	void DeveloperLevel::ResetSpeedBoost()
	{
		boostActive = false;
		if (player == nullptr)
		{
			return;
		}

		auto* movement = player->GetGameObject()->GetComponent<XYZEngine::MovementComponent>();
		if (movement != nullptr)
		{
			movement->SetSpeed(basePlayerSpeed);
		}
	}

	void DeveloperLevel::ConfigureBoss(Enemy* bossEnemy)
	{
		if (bossEnemy == nullptr)
		{
			return;
		}

		bossEnemy->GetCombatStats()->SetMaxHealthPoints(130);
		bossEnemy->GetCombatStats()->SetHealthPoints(130);
		bossEnemy->GetCombatStats()->SetArmorPoints(12);
		bossEnemy->GetAttackComponent()->SetDamage(18);
		bossEnemy->GetAttackComponent()->SetCooldown(0.8f);

		auto* movement = bossEnemy->GetGameObject()->GetComponent<XYZEngine::EnemyMovementComponent>();
		if (movement != nullptr)
		{
			movement->SetSpeed(kBossSpeed);
			movement->SetDetectionRadius(kBossRange);
		}
	}

	void DeveloperLevel::AddBorderTiles(int width, int height)
	{
		for (int y = 0; y <= height; ++y)
		{
			for (int x = 0; x <= width; ++x)
			{
				if (x == 0 || x == width || y == 0 || y == height)
				{
					AddWallTile(static_cast<float>(x) * 128.f, static_cast<float>(y) * 128.f);
				}
				else
				{
					AddFloorTile(static_cast<float>(x) * 128.f, static_cast<float>(y) * 128.f);
				}
			}
		}
	}

	void DeveloperLevel::AddExtraWalls(const std::vector<std::pair<int, int>>& extraWalls)
	{
		for (const auto& wall : extraWalls)
		{
			AddWallTile(static_cast<float>(wall.first) * 128.f, static_cast<float>(wall.second) * 128.f);
		}
	}

	void DeveloperLevel::AddFloorTile(float x, float y)
	{
		floors.push_back(std::make_unique<Floor>(XYZEngine::Vector2Df{ x, y }, 0));
	}

	void DeveloperLevel::AddWallTile(float x, float y)
	{
		walls.push_back(std::make_unique<Wall>(XYZEngine::Vector2Df{ x, y }, 38));
	}

	void DeveloperLevel::DrawHud(sf::RenderWindow& window)
	{
		if (!combatHud.IsReady())
		{
			return;
		}

		HandleInput();
		CheckLevelProgress();
		combatHud.Draw(window, player.get(), boss.get(), hudStatus, inventory.GetSummary(), lastActionMessage.empty() ? "Defeat the boss to unlock the next location." : lastActionMessage);
	}
}
