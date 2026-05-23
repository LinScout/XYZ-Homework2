#include "Enemy.h"
#include <ResourceSystem.h>
#include <HealthComponent.h>
#include <AttackComponent.h>
#include <Logger.h>

namespace XYZRoguelike
{
	Enemy::Enemy(const XYZEngine::Vector2Df& position)
	{
		LOG_INFO("Creating enemy");
		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Enemy");
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

		auto health = gameObject->AddComponent<XYZEngine::HealthComponent>();
		health->SetMaxHealth(50);
		health->SetHealth(50);
		health->SetArmor(5);

		auto attack = gameObject->AddComponent<XYZEngine::AttackComponent>();
		attack->SetDamage(12);
		attack->SetAttackRange(120.f);
		attack->SetCooldown(1.f);
		attack->SetAutoAttack(true);
		attack->SetTargetComponent(target);

		LOG_INFO("Enemy created (HP: 50, armor: 5)");
	}

	XYZEngine::GameObject* Enemy::GetGameObject()
	{
		return gameObject;
	}
}
