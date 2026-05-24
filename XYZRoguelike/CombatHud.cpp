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
		titleText.setString("Roguelike combat HUD");
		titleText.setPosition(10.f, 10.f);

		playerStatsText.setFont(font);
		playerStatsText.setCharacterSize(18);
		playerStatsText.setFillColor(sf::Color::Green);
		playerStatsText.setPosition(10.f, 40.f);

		enemyStatsText.setFont(font);
		enemyStatsText.setCharacterSize(18);
		enemyStatsText.setFillColor(sf::Color::Red);
		enemyStatsText.setPosition(10.f, 65.f);

		levelText.setFont(font);
		levelText.setCharacterSize(17);
		levelText.setFillColor(sf::Color::Cyan);
		levelText.setPosition(10.f, 95.f);

		inventoryText.setFont(font);
		inventoryText.setCharacterSize(17);
		inventoryText.setFillColor(sf::Color::Magenta);
		inventoryText.setPosition(10.f, 120.f);

		missionText.setFont(font);
		missionText.setCharacterSize(17);
		missionText.setFillColor(sf::Color::White);
		missionText.setPosition(10.f, 145.f);

		controlsText.setFont(font);
		controlsText.setCharacterSize(16);
		controlsText.setFillColor(sf::Color::White);
		controlsText.setString("Move: WASD | Space: attack | H: heal | B: speed boost | N: next level | R: restart");
		controlsText.setPosition(10.f, 175.f);

		GAME_LOG_INFO("CombatHud initialized");
	}

	bool CombatHud::IsReady() const
	{
		return ready;
	}

	void CombatHud::Draw(sf::RenderWindow& window, const Player* player, const Enemy* enemy, const std::string& levelInfo, const std::string& inventoryInfo, const std::string& missionInfo)
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
			oss << "Boss    HP: " << stats->GetHealthPoints() << "/" << stats->GetMaxHealthPoints()
				<< "  Armor: " << stats->GetArmorPoints();
			enemyStatsText.setString(oss.str());
		}
		else
		{
			enemyStatsText.setString("Boss    : defeated");
		}

		levelText.setString(levelInfo);
		inventoryText.setString(inventoryInfo);
		missionText.setString(missionInfo);

		const sf::View previousView = window.getView();
		window.setView(window.getDefaultView());

		window.draw(titleText);
		window.draw(playerStatsText);
		window.draw(enemyStatsText);
		window.draw(levelText);
		window.draw(inventoryText);
		window.draw(missionText);
		window.draw(controlsText);

		window.setView(previousView);
	}
}
