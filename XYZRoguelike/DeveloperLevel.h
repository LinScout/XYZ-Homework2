#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Scene.h"
#include "Player.h"
#include "Music.h"
#include "Floor.h"
#include "Wall.h"
#include "Enemy.h"
#include "CombatHud.h"
#include "Inventory.h"
#include <SFML/System/Clock.hpp>

using namespace XYZEngine;

namespace XYZRoguelike
{
	struct LevelTemplate
	{
		std::string name;
		XYZEngine::Vector2Df playerStart;
		XYZEngine::Vector2Df bossStart;
		XYZEngine::Vector2Df exitPosition;
		float exitRadius = 120.f;
		int width = 12;
		int height = 9;
		std::vector<std::pair<int, int>> extraWalls;
	};

	class DeveloperLevel : public Scene
	{
	public:
		void Start() override;
		void Restart() override;
		void Stop() override;

		void DrawHud(sf::RenderWindow& window);

		const Player* GetPlayer() const { return player.get(); }
		const Enemy* GetEnemy() const { return boss.get(); }

	private:
		void BuildLevel(const LevelTemplate& level);
		void ClearScene();
		void HandleInput();
		void CheckLevelProgress();
		void AdvanceLevel();
		void UpdateHudText();
		void UseHealthPotion();
		void UseSpeedBoost();
		void ResetSpeedBoost();
		void ConfigureBoss(Enemy* bossEnemy);
		void AddBorderTiles(int width, int height);
		void AddExtraWalls(const std::vector<std::pair<int, int>>& extraWalls);
		void AddFloorTile(float x, float y);
		void AddWallTile(float x, float y);

		std::shared_ptr<Player> player;
		std::unique_ptr<Music> music;
		std::unique_ptr<Enemy> boss;
		CombatHud combatHud;
		Inventory inventory;

		std::vector<LevelTemplate> levels;
		std::vector<std::unique_ptr<Wall>> walls;
		std::vector<std::unique_ptr<Floor>> floors;

		int currentLevelIndex = 0;
		bool bossDefeated = false;
		bool gameCompleted = false;
		bool boostActive = false;
		bool healKeyPressed = false;
		bool boostKeyPressed = false;
		bool nextLevelKeyPressed = false;
		bool restartKeyPressed = false;

		float basePlayerSpeed = 400.f;
		float boostDuration = 5.f;
		sf::Clock boostClock;
		std::string hudStatus;
		std::string currentLocation;
		std::string lastActionMessage;
	};
}
