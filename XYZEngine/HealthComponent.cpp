#include "pch.h"
#include "HealthComponent.h"
#include "GameObject.h"
#include "Logger.h"
#include <sstream>

namespace XYZEngine
{
	HealthComponent::HealthComponent(GameObject* gameObject)
		: Component(gameObject)
	{
	}

	void HealthComponent::Update(float deltaTime)
	{
	}

	void HealthComponent::Render()
	{
	}

	void HealthComponent::SetMaxHealth(int value)
	{
		maxHealth = value;
		if (health > maxHealth)
		{
			health = maxHealth;
		}
	}

	void HealthComponent::SetHealth(int value)
	{
		health = value;
		if (health > maxHealth)
		{
			health = maxHealth;
		}
		if (health < 0)
		{
			health = 0;
		}
	}

	void HealthComponent::SetArmor(int value)
	{
		armor = value < 0 ? 0 : value;
	}

	int HealthComponent::GetHealth() const
	{
		return health;
	}

	int HealthComponent::GetMaxHealth() const
	{
		return maxHealth;
	}

	int HealthComponent::GetArmor() const
	{
		return armor;
	}

	bool HealthComponent::IsAlive() const
	{
		return health > 0;
	}

	void HealthComponent::RestoreToFull()
	{
		health = maxHealth;
	}

	void HealthComponent::TakeDamage(int damage)
	{
		if (damage <= 0 || !IsAlive())
		{
			return;
		}

		int remainingDamage = damage;
		if (armor > 0)
		{
			int absorbed = remainingDamage > armor ? armor : remainingDamage;
			armor -= absorbed;
			remainingDamage -= absorbed;

			std::ostringstream oss;
			oss << gameObject->GetName() << " armor absorbed " << absorbed << " damage (armor left: " << armor << ")";
			LOG_INFO(oss.str());
		}

		if (remainingDamage > 0)
		{
			health -= remainingDamage;
			if (health < 0)
			{
				health = 0;
			}

			std::ostringstream oss;
			oss << gameObject->GetName() << " took " << remainingDamage << " damage (HP: " << health << "/" << maxHealth << ")";
			LOG_INFO(oss.str());
		}

		if (!IsAlive())
		{
			std::ostringstream oss;
			oss << gameObject->GetName() << " has been defeated";
			LOG_WARN(oss.str());
		}
	}
}
