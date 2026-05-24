#include "Floor.h"
#include "GameLog.h"

XYZRoguelike::Floor::Floor(const XYZEngine::Vector2Df& position, int textureMapIndex)
{
	static bool firstFloorLogged = false;
	if (!firstFloorLogged)
	{
		GAME_LOG_INFO("Placing floor tiles");
		firstFloorLogged = true;
	}

	gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Floor");
	auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
	transform->SetWorldPosition(position);

	auto renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
	renderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureMapElementShared("level_floors", textureMapIndex));
	renderer->SetPixelSize(128, 128);
}
