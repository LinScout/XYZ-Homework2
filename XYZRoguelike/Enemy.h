#pragma once

#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "TargetComponent.h"
#include "EnemyMovementComponent.h"
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"
#include "CombatStatsComponent.h"
#include "AttackComponent.h"
#include "Vector.h"

namespace XYZRoguelike
{
	class Enemy
	{
	public:
		Enemy(const XYZEngine::Vector2Df& position);

		XYZEngine::GameObject* GetGameObject() const;
		XYZEngine::CombatStatsComponent* GetCombatStats() const;
		XYZEngine::AttackComponent* GetAttackComponent() const;

	private:
		XYZEngine::GameObject* gameObject = nullptr;
		XYZEngine::CombatStatsComponent* combatStats = nullptr;
		XYZEngine::AttackComponent* attackComponent = nullptr;
	};
}
