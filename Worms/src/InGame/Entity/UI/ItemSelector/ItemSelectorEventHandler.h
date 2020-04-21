#pragma once

#include "ItemSelectorEnum.h"

namespace InGame {


	struct ItemSelectorDrawData
	{
		Gear::Ref<Gear::Texture2D> ItemSelectorTexture;
		Gear::Ref<Gear::Texture2D> ItemHighLighter;
		std::vector<ItemInfo::ItemDescprition> CurrentTeamItemVector;
		std::vector<std::pair<Gear::Ref<Gear::Texture2D>, std::pair<float, float>>> DrawItemInfoList;
		std::vector<std::string> DrawItemTypeStr;

		glm::mat4 Translate;
		glm::mat4 ItemHighlighterTranslate;

		glm::vec3 ItemNameBasePos;
		glm::vec3 ItemQuantityPos;
		float ItemNameOffset;

		glm::vec3 strScale;
	};
	
	ItemInfo::Number& GetSettedItem();
	ItemSelectorDrawData& GetItemSelectorDrawData();

	class ItemSelectorWormEventHandler : public Gear::EventHandler
	{
		void Handle(std::any data, int entityID, bool& handled) override;
	};

	class ItemSelectorTurnEventHandler : public Gear::EventHandler 
	{
		void Handle(std::any data, int entityID, bool& handled) override
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto itemlist = std::any_cast<std::unordered_map<std::string, std::vector<ItemInfo::ItemDescprition>>>(status->GetStat(ItemSelectorInfo::Stat::ItemList));

			for (auto& teamItemlist : itemlist)
			{
				for (auto& item : teamItemlist.second)
				{
					if (item.RemainTurn)
					{
						item.RemainTurn -= 1;
					}
				}
			}
			status->SetStat(ItemSelectorInfo::Stat::ItemList, itemlist);
			handled = true;
		}
	};

	class ItemSelectorMouseClickEventHandler : public Gear::EventHandler
	{
		Gear::Ref<Gear::FSM> FSM;
		Gear::Ref<Gear::FSM> WorldFSM;

		bool inFirst = true;
		int worldID;
		int currentWormID;

		void init(int entityID)
		{
			FSM = Gear::EntitySystem::GetFSM(entityID);
			worldID = Gear::EntitySystem::GetEntityIDFromName("World");
			WorldFSM = Gear::EntitySystem::GetFSM(worldID);
		}

		void Handle(std::any data, int entityID, bool& handled) override;

	};

}