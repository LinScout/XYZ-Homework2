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
#include <string>

using namespace XYZRoguelike;

int main()
{
	XYZEngine::Engine::Instance();

	LOG_INFO("XYZRoguelike starting");

	XYZEngine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1280, 720), "XYZRoguelike"));

	try
	{
		ResourceSystem::Instance()->LoadTextureMap("player", "Resources/TextureMaps/Player.png", { 48, 63 }, 4, false);
		ResourceSystem::Instance()->LoadTextureMap("level_floors", "Resources/TextureMaps/Floor.png", { 16, 16 }, 49, false);
		ResourceSystem::Instance()->LoadTextureMap("level_walls", "Resources/TextureMaps/Wall.png", { 16, 16 }, 48, false);
		ResourceSystem::Instance()->LoadSound("music", "Resources/Sounds/OST.wav");
	}
	catch (const XYZEngine::EngineException& e)
	{
		LOG_ERROR(std::string("Resource loading failed: ") + e.what());
		LOG_WARN("Continuing without some resources");
	}

	try
	{
		auto developerLevel = std::make_shared<DeveloperLevel>();
		developerLevel->Start();
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(std::string("Level start failed: ") + e.what());
	}

	try
	{
		ResourceSystem::Instance()->GetSound("missing_sound_for_test");
	}
	catch (const XYZEngine::EngineException& e)
	{
		LOG_ERROR(std::string("Handled critical situation: ") + e.what());
		LOG_WARN("Game continues after handled exception");
	}

	XYZEngine::Engine::Instance()->Run();

	LOG_INFO("XYZRoguelike finished");
	return 0;
}
