#include "wmpch.h"
#include "ItemSelectorFSMHandler.h"

#include "InGame/Entity/World/WorldState.h"

float itemSelectorYpos[12];
float itemSelectorYOffset = 0.052f;
float itemSelector2Ypos = -0.767f;
float itemTypeStrBottomYpos = -0.76f;
float itemTypeStrYOffset = 0.104f;
float itemTypeStrXOffset = 0.145f;

float item13BottomBase = -0.783f;
float itemLeftBase = 1.218f;

float itemXOffset = 0.0568f;
float itemYOffset = 0.104f;


namespace InGame {

	ItemSelectorDrawData itemSelectData;

	void ItemSelectorOnUpdate::Awake(int entityID)
	{
		worldID = Gear::EntitySystem::GetEntityIDFromName("World");

		worldStatus = Gear::EntitySystem::GetStatus(worldID);
		status = Gear::EntitySystem::GetStatus(entityID);

		for (int i = 0; i < 12; ++i)
		{
			itemSelectorYpos[i] = itemSelector2Ypos + itemSelectorYOffset * i;
		}
	}

	Gear::EnumType ItemSelectorOnUpdate::Handle(int entityID, const Gear::Command & cmd)
	{
		if (OnAwake)
		{
			Awake(entityID);
		}

		auto curTeam = std::any_cast<std::string>(worldStatus->GetStat(WorldInfo::CurrnetTeam));
		auto itemlist = std::any_cast<std::unordered_map<std::string, std::vector<ItemInfo::ItemDescprition>>>(status->GetStat(ItemSelectorInfo::Stat::ItemList));

		itemSelectData.CurrentTeamItemVector = itemlist[curTeam];

		bool itemVariation[13]{0, };
		bool itemInPos[13][5]{ 0, };

		int itemSelectorHeight = 0;
		itemSelectData.DrawItemTypeStr.clear();
		for (int i = 0; i < itemSelectData.CurrentTeamItemVector.size(); ++i)
		{
			ItemInfo::Type type = (ItemInfo::Type)(itemSelectData.CurrentTeamItemVector[i].ItemNumber / 5);
			ItemInfo::Type xPos = (ItemInfo::Type)(itemSelectData.CurrentTeamItemVector[i].ItemNumber % 5);
			itemInPos[type][xPos] = true;
			if (!itemVariation[type])
			{
				itemVariation[type] = true;
				switch (type)
				{
				case InGame::ItemInfo::Util:
					itemSelectData.DrawItemTypeStr.push_back("Util.");
					break;
				case InGame::ItemInfo::F1:
					itemSelectData.DrawItemTypeStr.push_back("F1");
					break;
				case InGame::ItemInfo::F2:
					itemSelectData.DrawItemTypeStr.push_back("F2");
					break;
				case InGame::ItemInfo::F3:
					itemSelectData.DrawItemTypeStr.push_back("F3");
					break;
				case InGame::ItemInfo::F4:
					itemSelectData.DrawItemTypeStr.push_back("F4");
					break;
				case InGame::ItemInfo::F5:
					itemSelectData.DrawItemTypeStr.push_back("F5");
					break;
				case InGame::ItemInfo::F6:
					itemSelectData.DrawItemTypeStr.push_back("F6");
					break;
				case InGame::ItemInfo::F7:
					itemSelectData.DrawItemTypeStr.push_back("F7");
					break;
				case InGame::ItemInfo::F8:
					itemSelectData.DrawItemTypeStr.push_back("F8");
					break;
				case InGame::ItemInfo::F9:
					itemSelectData.DrawItemTypeStr.push_back("F9");
					break;
				case InGame::ItemInfo::F10:
					itemSelectData.DrawItemTypeStr.push_back("F10");
					break;
				case InGame::ItemInfo::F11:
					itemSelectData.DrawItemTypeStr.push_back("F11");
					break;
				case InGame::ItemInfo::F12:
					itemSelectData.DrawItemTypeStr.push_back("F12");
					break;
				}
				itemSelectorHeight++;
			}
		}
		std::string name = "ItemSelector";
		name += std::to_string(itemSelectorHeight);

		itemSelectData.ItemSelectorTexture = Gear::TextureStorage::GetTexture2D(name);

		int width = itemSelectData.ItemSelectorTexture->GetWidth();
		int height = itemSelectData.ItemSelectorTexture->GetHeight();

		glm::vec3 scale(width / 920.0f * 1.8f, height / 500.0f * 1.8f, 1.0f);

		itemSelectData.Translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.3f, itemSelectorYpos[itemSelectorHeight - 2], ZOrder::z_FlatChecker)) * glm::scale(glm::mat4(1.0f), scale);

		int yEmbadePos[13]{ 0, };
		int k = 12;
		for (int i = 12; i >= 0; --i)
		{
			if (itemVariation[i])
			{
				yEmbadePos[i] = k;
				--k;
			}
		}
		int xEmbadePos[13][5]{ 0, };
		for (int i = 0; i < 13; ++i)
		{
			int k = 0;
			for (int j = 0; j < 5; ++j)
			{
				if (itemInPos[i][j])
				{
					xEmbadePos[i][j] = k;
					++k;
				}
			}
		}

		for (int i = 0; i < itemSelectData.CurrentTeamItemVector.size(); ++i)
		{
			int xBaseIndex = itemSelectData.CurrentTeamItemVector[i].ItemNumber % 5;
			int yBaseIndex = itemSelectData.CurrentTeamItemVector[i].ItemNumber / 5;

			int embadedIndexX = xEmbadePos[yBaseIndex][xBaseIndex];
			int embadedIndexY = yEmbadePos[yBaseIndex];

			itemSelectData.CurrentTeamItemVector[i].Translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, ZOrder::z_FlatCheckerImg)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(0.053f, 0.09422f, 1.0f));

			itemSelectData.CurrentTeamItemVector[i].Translate[3][0] = itemLeftBase + embadedIndexX * itemXOffset;
			itemSelectData.CurrentTeamItemVector[i].Translate[3][1] = item13BottomBase + (12 - embadedIndexY) * itemYOffset;
		}


		return ItemSelectorInfo::OnEmergy;
	}
	
	Gear::EnumType ItemSelectorOnEmergy::Handle(int entityID, const Gear::Command & cmd)
	{
		auto& positionX = itemSelectData.Translate[3][0];
		positionX -= 0.02f;

		if (positionX < 0.8f)
		{
			positionX = 0.8f;
			Gear::Renderer2D::DrawFixedQuad(itemSelectData.Translate, itemSelectData.ItemSelectorTexture);
			return ItemSelectorInfo::OnSelect;
		}
		else
		{
			Gear::Renderer2D::DrawFixedQuad(itemSelectData.Translate, itemSelectData.ItemSelectorTexture);
		}

		int strSize = itemSelectData.DrawItemTypeStr.size();
		for (int i = strSize - 1; i >= 0; --i)
		{
			Font::PrintFont(glm::vec3(itemSelectData.Translate[3][0] - itemTypeStrXOffset, itemTypeStrBottomYpos + (strSize - i - 1) * itemTypeStrYOffset, ZOrder::z_FlatFont), 
				glm::vec3(0.025f, 0.03f, 1.0f), itemSelectData.DrawItemTypeStr[i], FontType::GrayTinySmall, 0.01f);
		}

		for (int i = 0; i < itemSelectData.CurrentTeamItemVector.size(); ++i)
		{
			itemSelectData.CurrentTeamItemVector[i].Translate[3][0] -= 0.02f;
			Gear::Renderer2D::DrawFixedQuad(itemSelectData.CurrentTeamItemVector[i].Translate, itemSelectData.CurrentTeamItemVector[i].Texture);
		}

		return ItemSelectorInfo::OnEmergy;
	}

	Gear::EnumType ItemSelectorOnSink::Handle(int entityID, const Gear::Command & cmd)
	{
		auto& positionX = itemSelectData.Translate[3][0];
		positionX += 0.02f;

		if (positionX > 1.3f)
		{
			positionX = 1.3f;
			return ItemSelectorInfo::OnNotActivate;
		}

		Gear::Renderer2D::DrawFixedQuad(itemSelectData.Translate, itemSelectData.ItemSelectorTexture);
		
		int strSize = itemSelectData.DrawItemTypeStr.size();
		for (int i = strSize - 1; i >= 0; --i)
		{
			Font::PrintFont(glm::vec3(itemSelectData.Translate[3][0] - itemTypeStrXOffset, itemTypeStrBottomYpos + (strSize - i - 1) * itemTypeStrYOffset, ZOrder::z_FlatFont), 
				glm::vec3(0.025f, 0.03f, 1.0f), itemSelectData.DrawItemTypeStr[i], FontType::GrayTinySmall, 0.01f);
		}
		for (int i = 0; i < itemSelectData.CurrentTeamItemVector.size(); ++i)
		{
			itemSelectData.CurrentTeamItemVector[i].Translate[3][0] += 0.02f;
			Gear::Renderer2D::DrawFixedQuad(itemSelectData.CurrentTeamItemVector[i].Translate, itemSelectData.CurrentTeamItemVector[i].Texture);
		}

		return ItemSelectorInfo::OnSink;
	}

	Gear::EnumType ItemSelectorOnSelect::Handle(int entityID, const Gear::Command & cmd)
	{
		Gear::Renderer2D::DrawFixedQuad(itemSelectData.Translate, itemSelectData.ItemSelectorTexture);

		int strSize = itemSelectData.DrawItemTypeStr.size();
		for (int i = strSize - 1; i >= 0; --i)
		{
			auto str = itemSelectData.DrawItemTypeStr[i];
			int strlen = str.length();

			float adjustXoffset = (3 - strlen) * 0.005f;

			Font::PrintFont(glm::vec3(itemSelectData.Translate[3][0] - itemTypeStrXOffset - adjustXoffset, itemTypeStrBottomYpos + (strSize - i - 1) * itemTypeStrYOffset, ZOrder::z_FlatFont),
				glm::vec3(0.025f, 0.03f, 1.0f), itemSelectData.DrawItemTypeStr[i], FontType::GrayTinySmall, 0.01f);
		}

		auto& itemVector = itemSelectData.CurrentTeamItemVector;
		for (int i = 0; i < itemSelectData.CurrentTeamItemVector.size(); ++i)
		{
			Gear::Renderer2D::DrawFixedQuad(itemVector[i].Translate,itemVector[i].Texture);
		}


		return ItemSelectorInfo::OnSelect;
	}

	void ItemSelectorOnNotActivate::Awake(int entityID)
	{
		mouseID = Gear::EntitySystem::GetEntityIDFromName("Mouse");
		mouseFSM = Gear::EntitySystem::GetFSM(mouseID);

	}

	Gear::EnumType ItemSelectorOnNotActivate::Handle(int entityID, const Gear::Command & cmd)
	{
		if (OnAwake)
		{
			Awake(entityID);
		}

		if (mouseFSM->GetCurrentState() == WorldState::OnItemWindow)
		{
			mouseFSM->SetCurrentState(WorldState::OnRunning);
		}
		return ItemSelectorInfo::OnNotActivate;
	}
}

