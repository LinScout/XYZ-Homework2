#include "pch.h"
#include "TargetComponent.h"
#include "GameWorld.h"
#include "Logger.h"

XYZEngine::TargetComponent::TargetComponent(GameObject* gameObject)
	: Component(gameObject)
{
}

void XYZEngine::TargetComponent::TryFindTarget()
{
	if (!targetName.empty() && target == nullptr)
	{
		target = GameWorld::Instance()->FindGameObjectByName(targetName);
	}
}

void XYZEngine::TargetComponent::Update(float deltaTime)
{
	TryFindTarget();

	if (target != nullptr)
	{
		auto targetTransform = target->GetComponent<TransformComponent>();
		if (targetTransform != nullptr)
		{
			targetPosition = targetTransform->GetWorldPosition();
		}
	}
}

void XYZEngine::TargetComponent::Render()
{
}

void XYZEngine::TargetComponent::SetTarget(GameObject* newTarget)
{
	target = newTarget;
	targetName.clear();
}

void XYZEngine::TargetComponent::SetTargetByName(const std::string& name)
{
	targetName = name;
	target = nullptr;
	TryFindTarget();
	if (target == nullptr)
	{
		LOG_WARN("Target not found yet: " + name);
	}
	else
	{
		LOG_INFO("Target assigned: " + name);
	}
}

XYZEngine::GameObject* XYZEngine::TargetComponent::GetTarget() const
{
	return target;
}

const XYZEngine::Vector2Df& XYZEngine::TargetComponent::GetTargetPosition() const
{
	return targetPosition;
}

bool XYZEngine::TargetComponent::HasTarget() const
{
	return target != nullptr;
}
