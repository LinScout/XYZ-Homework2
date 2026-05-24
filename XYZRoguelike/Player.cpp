#include "Player.h"
#include <ResourceSystem.h>
#include <SpriteColliderComponent.h>
#include <MovementComponent.h>
#include <SpriteDirectionComponent.h>
#include <SpriteMovementAnimationComponent.h>
#include <CombatStatsDisplayComponent.h>
#include "GameLog.h"

namespace XYZRoguelike
{
	Player::Player(const XYZEngine::Vector2Df& position)
	{
		GAME_LOG_INFO("Creating player entity");

		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Player");
		auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
		transform->SetWorldPosition(position);

		auto renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
		renderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureMapElementShared("player", 0));
		renderer->SetPixelSize(100, 100);

		auto camera = gameObject->AddComponent<XYZEngine::CameraComponent>();
		camera->SetWindow(&XYZEngine::RenderSystem::Instance()->GetMainWindow());
		camera->SetBaseResolution(1280, 720);

		gameObject->AddComponent<XYZEngine::InputComponent>();

		auto movement = gameObject->AddComponent<XYZEngine::MovementComponent>();
		movement->SetSpeed(400.f);

		gameObject->AddComponent<XYZEngine::SpriteDirectionComponent>();

		auto rigidbody = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();
		rigidbody->SetKinematic(false);

		gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

		auto animator = gameObject->AddComponent<XYZEngine::SpriteMovementAnimationComponent>();
		animator->Initialize("player", 6.f);

		combatStats = gameObject->AddComponent<XYZEngine::CombatStatsComponent>();
		combatStats->SetMaxHealthPoints(100);
		combatStats->SetHealthPoints(100);
		combatStats->SetArmorPoints(10);
		gameObject->AddComponent<XYZEngine::CombatStatsDisplayComponent>();

		attackComponent = gameObject->AddComponent<XYZEngine::AttackComponent>();
		attackComponent->SetDamage(20);
		attackComponent->SetAttackRange(130.f);
		attackComponent->SetCooldown(0.4f);
		attackComponent->SetAutoAttack(false);
		attackComponent->SetTargetByName("Enemy");

		GAME_LOG_INFO("Player ready: HP 100/100, armor 10, attack on Space");
	}

	XYZEngine::GameObject* Player::GetGameObject() const
	{
		return gameObject;
	}

	XYZEngine::CombatStatsComponent* Player::GetCombatStats() const
	{
		return combatStats;
	}

	XYZEngine::AttackComponent* Player::GetAttackComponent() const
	{
		return attackComponent;
	}
}
