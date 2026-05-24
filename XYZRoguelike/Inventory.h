#pragma once

#include <map>
#include <string>
#include <vector>

namespace XYZRoguelike
{
	enum class InventoryItemType
	{
		HealthPotion,
		BallBoost
	};

	class Inventory
	{
	public:
		void AddItem(InventoryItemType item, int count = 1);
		bool ConsumeItem(InventoryItemType item);
		int GetCount(InventoryItemType item) const;
		std::vector<InventoryItemType> GetItems() const;
		std::string GetItemName(InventoryItemType item) const;
		std::string GetSummary() const;

	private:
		std::map<InventoryItemType, int> items;
	};
}
