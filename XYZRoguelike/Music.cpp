#include "Music.h"
#include "Logger.h"
#include "EngineException.h"

Music::Music(const std::string& soundName)
{
	LOG_INFO("Starting music: " + soundName);
	try
	{
		auto gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Music: " + soundName);
		auto music = gameObject->AddComponent<XYZEngine::AudioComponent>();
		music->SetAudio(*XYZEngine::ResourceSystem::Instance()->GetSound(soundName));
		music->SetLoop(true);
		music->Play();
	}
	catch (const XYZEngine::EngineException& e)
	{
		LOG_ERROR(std::string("Music failed to start: ") + e.what());
		LOG_WARN("Game continues without music");
	}
}
