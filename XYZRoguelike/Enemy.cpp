#include "Enemy.h"
#include <ResourceSystem.h>
#include <CombatStatsDisplayComponent.h>
#include "GameLog.h"

namespace XYZRoguelike
{
	Enemy::Enemy(const XYZEngine::Vector2Df& position, const std::string& name)
	{
		GAME_LOG_INFO("Creating enemy entity");

		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject(name);
		auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
		transform->SetWorldPosition(position);

		auto renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
		renderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureMapElementShared("player", 0));
		renderer->SetPixelSize(100, 100);

		auto target = gameObject->AddComponent<XYZEngine::TargetComponent>();
		target->SetTargetByName("Player");

		auto enemyMovement = gameObject->AddComponent<XYZEngine::EnemyMovementComponent>();
		enemyMovement->SetSpeed(150.f);
		enemyMovement->SetDetectionRadius(400.f);
		enemyMovement->SetTargetComponent(target);

		auto rigidbody = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();
		rigidbody->SetKinematic(false);

		gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

		combatStats = gameObject->AddComponent<XYZEngine::CombatStatsComponent>();
		combatStats->SetMaxHealthPoints(50);
		combatStats->SetHealthPoints(50);
		combatStats->SetArmorPoints(5);
		gameObject->AddComponent<XYZEngine::CombatStatsDisplayComponent>();

		attackComponent = gameObject->AddComponent<XYZEngine::AttackComponent>();
		attackComponent->SetDamage(12);
		attackComponent->SetAttackRange(120.f);
		attackComponent->SetCooldown(1.f);
		attackComponent->SetAutoAttack(true);
		attackComponent->SetTargetComponent(target);

		GAME_LOG_INFO("Enemy ready: HP 50/50, armor 5, auto-attack when chasing");
	}

	XYZEngine::GameObject* Enemy::GetGameObject() const
	{
		return gameObject;
	}

	XYZEngine::CombatStatsComponent* Enemy::GetCombatStats() const
	{
		return combatStats;
	}

	XYZEngine::AttackComponent* Enemy::GetAttackComponent() const
	{
		return attackComponent;
	}
}
