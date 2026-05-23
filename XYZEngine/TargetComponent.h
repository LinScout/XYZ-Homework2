#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include <string>

namespace XYZEngine
{
	class TargetComponent : public Component
	{
	public:
		TargetComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetTarget(GameObject* target);
		void SetTargetByName(const std::string& name);
		GameObject* GetTarget() const;
		const Vector2Df& GetTargetPosition() const;
		bool HasTarget() const;

	private:
		void TryFindTarget();

		GameObject* target = nullptr;
		std::string targetName;
		Vector2Df targetPosition = { 0, 0 };
	};
}
