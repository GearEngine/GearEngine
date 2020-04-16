#include "wmpch.h"
#include "ItemSelectorFSMHandler.h"

#include "InGame/Entity/World/WorldState.h"

float itemSelectorYpos[12];
float itemSelectorYOffset = 0.052f;
float itemSelector2Ypos = -0.767f;
float itemTypeStrBottomYpos = -0.76f;
float itemTypeStrYOffset = 0.104f;
float itemTypeStrXOffset = 0.145f;

namespace InGame {

	ItemSelectorDrawData data;

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
		auto itemlist = std::any_cast<std::unordered_map<std::string, std::vector<std::pair<ItemInfo::Number, int>>>>(status->GetStat(ItemSelectorInfo::Stat::ItemList));

		data.CurrentTeamItemVector = itemlist[curTeam];

		bool itemVariation[13]{0, };
		int size = 0;
		data.DrawItemTypeStr.clear();
		for (int i = 0; i < data.CurrentTeamItemVector.size(); ++i)
		{
			ItemInfo::Type type = (ItemInfo::Type)(data.CurrentTeamItemVector[i].first / 5);
			if (!itemVariation[type])
			{
				itemVariation[type] = true;
				switch (type)
				{
				case InGame::ItemInfo::Util:
					data.DrawItemTypeStr.push_back("Util.");
					break;
				case InGame::ItemInfo::F1:
					data.DrawItemTypeStr.push_back("F1");
					break;
				case InGame::ItemInfo::F2:
					data.DrawItemTypeStr.push_back("F2");
					break;
				case InGame::ItemInfo::F3:
					data.DrawItemTypeStr.push_back("F3");
					break;
				case InGame::ItemInfo::F4:
					data.DrawItemTypeStr.push_back("F4");
					break;
				case InGame::ItemInfo::F5:
					data.DrawItemTypeStr.push_back("F5");
					break;
				case InGame::ItemInfo::F6:
					data.DrawItemTypeStr.push_back("F6");
					break;
				case InGame::ItemInfo::F7:
					data.DrawItemTypeStr.push_back("F7");
					break;
				case InGame::ItemInfo::F8:
					data.DrawItemTypeStr.push_back("F8");
					break;
				case InGame::ItemInfo::F9:
					data.DrawItemTypeStr.push_back("F9");
					break;
				case InGame::ItemInfo::F10:
					data.DrawItemTypeStr.push_back("F10");
					break;
				case InGame::ItemInfo::F11:
					data.DrawItemTypeStr.push_back("F11");
					break;
				case InGame::ItemInfo::F12:
					data.DrawItemTypeStr.push_back("F12");
					break;
				}
				size++;
			}
		}
		std::string name = "ItemSelector";
		name += std::to_string(size);

		data.ItemSelectorTexture = Gear::TextureStorage::GetTexture2D(name);

		int width = data.ItemSelectorTexture->GetWidth();
		int height = data.ItemSelectorTexture->GetHeight();

		glm::vec3 scale(width / 920.0f * 1.8f, height / 500.0f * 1.8f, 1.0f);

		data.Translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.3f, itemSelectorYpos[size - 2], ZOrder::z_FlatChecker)) * glm::scale(glm::mat4(1.0f), scale);

		return ItemSelectorInfo::OnEmergy;
	}
	
	Gear::EnumType ItemSelectorOnEmergy::Handle(int entityID, const Gear::Command & cmd)
	{
		auto& positionX = data.Translate[3][0];
		positionX -= 0.02f;


		if (positionX < 0.8f)
		{
			positionX = 0.8f;
			Gear::Renderer2D::DrawFixedQuad(data.Translate, data.ItemSelectorTexture);
			return ItemSelectorInfo::OnSelect;
		}
		else
		{
			Gear::Renderer2D::DrawFixedQuad(data.Translate, data.ItemSelectorTexture);
		}

		int strSize = data.DrawItemTypeStr.size();
		for (int i = strSize - 1; i >= 0; --i)
		{
			Font::PrintFont(glm::vec3(data.Translate[3][0] - itemTypeStrXOffset, itemTypeStrBottomYpos + (strSize - i - 1) * itemTypeStrYOffset, ZOrder::z_FlatFont), 
				glm::vec3(0.025f, 0.03f, 1.0f), data.DrawItemTypeStr[i], FontType::GrayTinySmall, 0.01f);
		}

		return ItemSelectorInfo::OnEmergy;
	}

	Gear::EnumType ItemSelectorOnSink::Handle(int entityID, const Gear::Command & cmd)
	{
		auto& positionX = data.Translate[3][0];
		positionX += 0.02f;

		if (positionX > 1.3f)
		{
			positionX = 1.3f;
			return ItemSelectorInfo::OnNotActivate;
		}

		Gear::Renderer2D::DrawFixedQuad(data.Translate, data.ItemSelectorTexture);
		
		int strSize = data.DrawItemTypeStr.size();
		for (int i = strSize - 1; i >= 0; --i)
		{
			Font::PrintFont(glm::vec3(data.Translate[3][0] - itemTypeStrXOffset, itemTypeStrBottomYpos + (strSize - i - 1) * itemTypeStrYOffset, ZOrder::z_FlatFont), 
				glm::vec3(0.025f, 0.03f, 1.0f), data.DrawItemTypeStr[i], FontType::GrayTinySmall, 0.01f);
		}

		return ItemSelectorInfo::OnSink;
	}

	Gear::EnumType ItemSelectorOnSelect::Handle(int entityID, const Gear::Command & cmd)
	{
		Gear::Renderer2D::DrawFixedQuad(data.Translate, data.ItemSelectorTexture);

		int strSize = data.DrawItemTypeStr.size();
		for (int i = strSize - 1; i >= 0; --i)
		{
			auto str = data.DrawItemTypeStr[i];
			int strlen = str.length();

			float adjustXoffset = (3 - strlen) * 0.005f;

			Font::PrintFont(glm::vec3(data.Translate[3][0] - itemTypeStrXOffset - adjustXoffset, itemTypeStrBottomYpos + (strSize - i - 1) * itemTypeStrYOffset, ZOrder::z_FlatFont),
				glm::vec3(0.025f, 0.03f, 1.0f), data.DrawItemTypeStr[i], FontType::GrayTinySmall, 0.01f);
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

