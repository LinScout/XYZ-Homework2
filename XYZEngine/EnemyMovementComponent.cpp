#include "pch.h"
#include "EnemyMovementComponent.h"
#include "Logger.h"
#include <cmath>

XYZEngine::EnemyMovementComponent::EnemyMovementComponent(GameObject* gameObject)
	: Component(gameObject)
{
	transform = gameObject->GetComponent<TransformComponent>();
}

void XYZEngine::EnemyMovementComponent::Update(float deltaTime)
{
	if (targetComponent == nullptr || transform == nullptr)
	{
		return;
	}

	if (!targetComponent->HasTarget())
	{
		isChasing = false;
		return;
	}

	Vector2Df enemyPos = transform->GetWorldPosition();
	Vector2Df targetPos = targetComponent->GetTargetPosition();

	Vector2Df direction = targetPos - enemyPos;
	float distance = direction.GetLength();

	if (distance <= detectionRadius && distance > 0.001f)
	{
		if (!isChasing)
		{
			LOG_INFO("Enemy detected player and started chasing");
		}
		isChasing = true;
		Vector2Df normalizedDir = { direction.x / distance, direction.y / distance };
		Vector2Df movement = normalizedDir * (speed * deltaTime);
		transform->MoveBy(movement);
	}
	else
	{
		if (isChasing)
		{
			LOG_INFO("Enemy lost player and stopped");
		}
		isChasing = false;
	}
}

void XYZEngine::EnemyMovementComponent::Render()
{
}

void XYZEngine::EnemyMovementComponent::SetSpeed(float newSpeed)
{
	speed = newSpeed;
}

float XYZEngine::EnemyMovementComponent::GetSpeed() const
{
	return speed;
}

void XYZEngine::EnemyMovementComponent::SetDetectionRadius(float radius)
{
	detectionRadius = radius;
}

float XYZEngine::EnemyMovementComponent::GetDetectionRadius() const
{
	return detectionRadius;
}

void XYZEngine::EnemyMovementComponent::SetTargetComponent(TargetComponent* targetComp)
{
	targetComponent = targetComp;
}

bool XYZEngine::EnemyMovementComponent::IsChasing() const
{
	return isChasing;
}
