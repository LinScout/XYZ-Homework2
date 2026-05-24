#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Player.h"
#include "Enemy.h"

namespace XYZRoguelike
{
	class CombatHud
	{
	public:
		CombatHud();

		void Draw(sf::RenderWindow& window, const Player* player, const Enemy* enemy, const std::string& levelInfo, const std::string& inventoryInfo, const std::string& missionInfo);
		bool IsReady() const;

	private:
		sf::Font font;
		sf::Text titleText;
		sf::Text playerStatsText;
		sf::Text enemyStatsText;
		sf::Text levelText;
		sf::Text inventoryText;
		sf::Text missionText;
		sf::Text controlsText;
		bool ready = false;
	};
}
