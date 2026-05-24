#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"

namespace XYZRoguelike
{
	class CombatHud
	{
	public:
		CombatHud();

		void Draw(sf::RenderWindow& window, const Player* player, const Enemy* enemy);
		bool IsReady() const;

	private:
		sf::Font font;
		sf::Text titleText;
		sf::Text playerStatsText;
		sf::Text enemyStatsText;
		sf::Text controlsText;
		bool ready = false;
	};
}
