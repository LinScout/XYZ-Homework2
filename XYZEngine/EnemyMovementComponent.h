#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include "TargetComponent.h"
#include "Vector.h"

namespace XYZEngine
{
	class EnemyMovementComponent : public Component
	{
	public:
		EnemyMovementComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetSpeed(float newSpeed);
		float GetSpeed() const;

		void SetDetectionRadius(float radius);
		float GetDetectionRadius() const;

		void SetTargetComponent(TargetComponent* targetComp);
		bool IsChasing() const;

	private:
		TransformComponent* transform;
		TargetComponent* targetComponent;

		float speed = 200.f;
		float detectionRadius = 300.f;
		bool isChasing = false;
	};
}
