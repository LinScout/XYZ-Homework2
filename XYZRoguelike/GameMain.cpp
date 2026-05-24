// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "DeveloperLevel.h"
#include "Matrix2D.h"
#include "Logger.h"
#include "EngineException.h"
#include "GameLog.h"
#include <string>
#include <memory>

using namespace XYZRoguelike;

int main()
{
	XYZEngine::Engine::Instance();
	SetupGameLogger();

	GAME_LOG_INFO("XYZRoguelike starting");
	LOG_INFO("Engine and game loggers are active");

	XYZEngine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1280, 720), "XYZRoguelike"));

	try
	{
		GAME_LOG_INFO("Loading game resources");
		ResourceSystem::Instance()->LoadTextureMap("player", "Resources/TextureMaps/Player.png", { 48, 63 }, 4, false);
		ResourceSystem::Instance()->LoadTextureMap("level_floors", "Resources/TextureMaps/Floor.png", { 16, 16 }, 49, false);
		ResourceSystem::Instance()->LoadTextureMap("level_walls", "Resources/TextureMaps/Wall.png", { 16, 16 }, 48, false);
		ResourceSystem::Instance()->LoadSound("music", "Resources/Sounds/OST.wav");
		GAME_LOG_INFO("Resources loaded successfully");
	}
	catch (const XYZEngine::EngineException& e)
	{
		GAME_LOG_ERROR(std::string("Resource loading failed: ") + e.what());
		LOG_ERROR(std::string("Engine caught resource error: ") + e.what());
		GAME_LOG_WARN("Continuing without some resources");
	}

	std::shared_ptr<DeveloperLevel> developerLevel;
	try
	{
		developerLevel = std::make_shared<DeveloperLevel>();
		developerLevel->Start();
	}
	catch (const std::exception& e)
	{
		GAME_LOG_ERROR(std::string("Level start failed: ") + e.what());
	}

	try
	{
		ResourceSystem::Instance()->GetSound("missing_sound_for_test");
	}
	catch (const XYZEngine::EngineException& e)
	{
		GAME_LOG_ERROR(std::string("Handled critical situation: ") + e.what());
		LOG_WARN("Handled missing sound test without crashing");
	}

	if (developerLevel)
	{
		XYZEngine::Engine::Instance()->SetOverlayDraw(
			[developerLevel](sf::RenderWindow& window)
			{
				developerLevel->DrawHud(window);
			});
		GAME_LOG_INFO("Combat HUD overlay registered");
	}
	else
	{
		GAME_LOG_WARN("DeveloperLevel was not created, HUD overlay skipped");
	}

	XYZEngine::Engine::Instance()->Run();

	GAME_LOG_INFO("XYZRoguelike finished");
	LOG_INFO("Engine shutdown");
	return 0;
}
