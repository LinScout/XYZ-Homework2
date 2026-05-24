#include "Inventory.h"

namespace XYZRoguelike
{
	void Inventory::AddItem(InventoryItemType item, int count)
	{
		if (count <= 0)
		{
			return;
		}

		items[item] += count;
	}

	bool Inventory::ConsumeItem(InventoryItemType item)
	{
		const auto it = items.find(item);
		if (it == items.end() || it->second <= 0)
		{
			return false;
		}

		--it->second;
		if (it->second == 0)
		{
			items.erase(it);
		}

		return true;
	}

	int Inventory::GetCount(InventoryItemType item) const
	{
		const auto it = items.find(item);
		return it == items.end() ? 0 : it->second;
	}

	std::vector<InventoryItemType> Inventory::GetItems() const
	{
		std::vector<InventoryItemType> result;
		result.reserve(items.size());
		for (const auto& entry : items)
		{
			result.push_back(entry.first);
		}
		return result;
	}

	std::string Inventory::GetItemName(InventoryItemType item) const
	{
		switch (item)
		{
		case InventoryItemType::HealthPotion:
			return "Health Potion";
		case InventoryItemType::BallBoost:
			return "Speed Potion";
		default:
			return "Unknown Item";
		}
	}

	std::string Inventory::GetSummary() const
	{
		if (items.empty())
		{
			return "Inventory is empty";
		}

		std::string summary;
		for (const auto& entry : items)
		{
			if (!summary.empty())
			{
				summary += ", ";
			}

			summary += GetItemName(entry.first) + " x" + std::to_string(entry.second);
		}
		return summary;
	}
}
