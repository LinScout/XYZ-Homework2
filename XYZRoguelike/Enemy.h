#pragma once

#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "TargetComponent.h"
#include "EnemyMovementComponent.h"
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"
#include "Vector.h"

namespace XYZRoguelike
{
	class Enemy
	{
	public:
		Enemy(const XYZEngine::Vector2Df& position);
		XYZEngine::GameObject* GetGameObject();

	private:
		XYZEngine::GameObject* gameObject;
	};
}
