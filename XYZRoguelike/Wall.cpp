#include "Wall.h"
#include <SpriteMovementAnimationComponent.h>
#include <SpriteColliderComponent.h>
#include <Logger.h>

XYZRoguelike::Wall::Wall(const XYZEngine::Vector2Df position, int textureMapIndex)
{
	static int wallCount = 0;
	if (wallCount == 0)
	{
		LOG_INFO("Creating walls");
	}
	wallCount++;

	gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Wall");
	auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
	transform->SetWorldPosition(position);

	auto renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
	renderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureMapElementShared("level_walls", textureMapIndex));
	renderer->SetPixelSize(128, 128);

	auto rigidbody = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();
	rigidbody->SetKinematic(true);

	auto collider = gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();
}
