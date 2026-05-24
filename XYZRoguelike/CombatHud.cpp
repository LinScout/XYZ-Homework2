#include "CombatHud.h"
#include "CombatStatsComponent.h"
#include "GameLog.h"
#include <sstream>

namespace XYZRoguelike
{
	CombatHud::CombatHud()
	{
		if (!font.loadFromFile("Resources/Fonts/Roboto-Regular.ttf"))
		{
			GAME_LOG_ERROR("CombatHud: failed to load font");
			return;
		}

		ready = true;
		titleText.setFont(font);
		titleText.setCharacterSize(22);
		titleText.setFillColor(sf::Color::Yellow);
		titleText.setString("Combat stats");
		titleText.setPosition(10.f, 10.f);

		playerStatsText.setFont(font);
		playerStatsText.setCharacterSize(18);
		playerStatsText.setFillColor(sf::Color::Green);
		playerStatsText.setPosition(10.f, 40.f);

		enemyStatsText.setFont(font);
		enemyStatsText.setCharacterSize(18);
		enemyStatsText.setFillColor(sf::Color::Red);
		enemyStatsText.setPosition(10.f, 65.f);

		controlsText.setFont(font);
		controlsText.setCharacterSize(16);
		controlsText.setFillColor(sf::Color::White);
		controlsText.setString("Move: WASD | Attack: Space");
		controlsText.setPosition(10.f, 95.f);

		GAME_LOG_INFO("CombatHud initialized");
	}

	bool CombatHud::IsReady() const
	{
		return ready;
	}

	void CombatHud::Draw(sf::RenderWindow& window, const Player* player, const Enemy* enemy)
	{
		if (!ready)
		{
			GAME_LOG_WARN("CombatHud draw skipped: font not loaded");
			return;
		}

		if (player != nullptr && player->GetCombatStats() != nullptr)
		{
			auto* stats = player->GetCombatStats();
			std::ostringstream oss;
			oss << "Player  HP: " << stats->GetHealthPoints() << "/" << stats->GetMaxHealthPoints()
				<< "  Armor: " << stats->GetArmorPoints();
			playerStatsText.setString(oss.str());
		}
		else
		{
			playerStatsText.setString("Player: unavailable");
			GAME_LOG_WARN("CombatHud: player stats unavailable");
		}

		if (enemy != nullptr && enemy->GetCombatStats() != nullptr && enemy->GetCombatStats()->IsAlive())
		{
			auto* stats = enemy->GetCombatStats();
			std::ostringstream oss;
			oss << "Enemy   HP: " << stats->GetHealthPoints() << "/" << stats->GetMaxHealthPoints()
				<< "  Armor: " << stats->GetArmorPoints();
			enemyStatsText.setString(oss.str());
		}
		else
		{
			enemyStatsText.setString("Enemy: defeated or unavailable");
		}

		const sf::View previousView = window.getView();
		window.setView(window.getDefaultView());

		window.draw(titleText);
		window.draw(playerStatsText);
		window.draw(enemyStatsText);
		window.draw(controlsText);

		window.setView(previousView);
	}
}
