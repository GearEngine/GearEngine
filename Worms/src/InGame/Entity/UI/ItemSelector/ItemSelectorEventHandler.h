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

	class ItemSelectorMouseClickEventHandler : public Gear::EventHandler
	{
		Gear::Ref<Gear::FSM> FSM;

		bool inFirst = true;
		int worldID;
		int currentWormID;

		void init(int entityID)
		{
			FSM = Gear::EntitySystem::GetFSM(entityID);
			worldID = Gear::EntitySystem::GetEntityIDFromName("World");
		}

		void Handle(std::any data, int entityID, bool& handled) override;

	};


}