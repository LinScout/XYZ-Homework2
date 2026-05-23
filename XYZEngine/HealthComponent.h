#pragma once

#include "Component.h"

namespace XYZEngine
{
	class HealthComponent : public Component
	{
	public:
		HealthComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetMaxHealth(int value);
		void SetHealth(int value);
		void SetArmor(int value);

		int GetHealth() const;
		int GetMaxHealth() const;
		int GetArmor() const;

		bool IsAlive() const;
		void TakeDamage(int damage);
		void RestoreToFull();

	private:
		int maxHealth = 100;
		int health = 100;
		int armor = 0;
	};
}
