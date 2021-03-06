#include "wmpch.h"
#include "ItemSelectorEventHandler.h"

#include "InGame/Entity/World/WorldState.h"
#include "InGame/Entity/Object/Worm/WormEnum.h"
#include "InGame/Entity/Object/Worm/WormFSMHandler.h"

#include "InGame/Entity/World/WorldEventHandler.h"

namespace InGame {

	ItemInfo::Number SettedItem = ItemInfo::Number::ItemEnd;
	ItemSelectorDrawData itemSelectData;

	ItemInfo::Number& GetSettedItem()
	{
		return SettedItem;
	}

	ItemSelectorDrawData & GetItemSelectorDrawData()
	{
		return itemSelectData;
	}
	
	void ItemSelectorMouseClickEventHandler::Handle(std::any data, int entityID, bool & handled)
	{
		if (inFirst)
		{
			init(entityID);
		}

		auto keycode = std::any_cast<int>(data);
		auto curState = FSM->GetCurrentState();

		if (keycode == GR_MOUSE_BUTTON_RIGHT)
		{
			auto worldState = WorldFSM->GetCurrentState();
			if (worldState == WorldState::OnWaiting || worldState == WorldState::InGameStart || worldState == WorldState::OnStart)
			{
				handled = true;
				return;
			}
			
			int curWorm = std::any_cast<int>(Gear::EntitySystem::GetStatus(worldID)->GetStat(WorldInfo::CurrentWormID));
			if (std::any_cast<int>(Gear::EntitySystem::GetStatus(curWorm)->GetStat(WormInfo::ShotgunUseCnt)) == 1)
			{
				handled = true;
				PLAY_SOUND_NAME("WARNINGBEEP", WormsSound::effect);
				return;
			}

			if (curState == ItemSelectorInfo::State::OnNotActivate)
			{
				DEBUG_GR_TRACE("On Item Selector Get Right Button Click Event");
				FSM->SetCurrentState(ItemSelectorInfo::State::OnUpdate);
				int mouseID = Gear::EntitySystem::GetEntityIDFromName("Mouse");

				auto mouseState = Gear::EntitySystem::GetFSM(mouseID)->GetCurrentState();
				if (mouseState == WorldState::OnWindowSelect)
				{
					auto mouseHandler = Gear::EntitySystem::GetFSM(mouseID)->GetHandler(WorldState::OnWindowSelect);
					mouseHandler->OnOut(mouseID);
				}
				auto mouseHandler = Gear::EntitySystem::GetFSM(mouseID)->GetHandler(WorldState::OnItemWindow);
				mouseHandler->OnOut(mouseID);
				Gear::EntitySystem::GetFSM(mouseID)->SetCurrentState(WorldState::OnItemWindow);
				handled = true;
				return;
			}
			if (curState == ItemSelectorInfo::State::OnSink)
			{
				FSM->SetCurrentState(ItemSelectorInfo::State::OnEmergy);
				handled = true;
				return;
			}
			if (curState == ItemSelectorInfo::State::OnEmergy || curState == ItemSelectorInfo::State::OnSelect ||
				curState == ItemSelectorInfo::State::OnSelectOnNotTurn)
			{
				FSM->SetCurrentState(ItemSelectorInfo::State::OnSink);
				handled = true;
				return;
			}
		}
		if (keycode == GR_MOUSE_BUTTON_LEFT)
		{
			if (curState != ItemSelectorInfo::State::OnSelect)
			{
				handled = true;
				return;
			}

			
			if (SettedItem == ItemInfo::Number::ItemEnd)
			{
				PLAY_SOUND_NAME("WARNINGBEEP", WormsSound::Mouse);
				handled = true;
				return;
			}
			else
			{
				PLAY_SOUND_NAME("CursorSelect", WormsSound::Mouse);
			}


			auto currentWormName = std::any_cast<std::string>(Gear::EntitySystem::GetStatus(worldID)->GetStat(WorldInfo::CurrentWormName));
			auto currentTeamName = std::any_cast<std::string>(Gear::EntitySystem::GetStatus(worldID)->GetStat(WorldInfo::CurrnetTeam));
			
			currentWormID = Gear::EntitySystem::GetEntityIDFromName(currentTeamName + currentWormName);
			if (GameMode::Bit::ModeBit == GameMode::NetWork)
			{
				DEBUG_GR_TRACE("Item selected!");
				WormsPacket::Event e;
				e.wormID = currentWormID;
				e.eventType = WormsPacket::Event::EventType::ItemSelect;
				e.customIData1 = SettedItem;
				Gear::NetWorkManager::Get()->Send(e);
				handled = true;
				return;
			}
			
			auto wormFSM = Gear::EntitySystem::GetFSM(currentWormID);
			auto wormStatus = Gear::EntitySystem::GetStatus(currentWormID);

			wormStatus->SetStat(WormInfo::Stat::SelectedItem, SettedItem);
			auto wormDir = std::any_cast<WormInfo::DirectionType>(wormStatus->GetStat(WormInfo::Stat::Direction));
			SetReadyItemUseAnimation(currentWormID, wormDir);

			auto wormState = wormFSM->GetCurrentState();
			if (wormState == WormState::OnReadyItemUse)
			{
				wormFSM->GetHandler(WormState::OnReadyItemUse)->OnOut(currentWormID);
			}
			if (wormState == WormState::OnWaiting)
			{
				wormFSM->GetHandler(WormState::OnWaiting)->OnOut(currentWormID);
			}
			wormFSM->SetCurrentState(WormState::OnReadyItemUse);
			FSM->SetCurrentState(ItemSelectorInfo::State::OnSink);
		}

		handled = true;
	}

	void ItemSelectorWormEventHandler::Handle(std::any data, int entityID, bool & handled)
	{
		auto useItemData = std::any_cast<UseItemData>(data);

		auto currentTeam = useItemData.TeamName;
		auto& itemVector = itemSelectData.CurrentTeamItemVector;
		auto itemNumber = useItemData.ItemNumber;

		bool erased = false;
		for (auto it = itemVector.begin(); it != itemVector.end(); ++it)
		{
			if ((*it).ItemNumber == itemNumber)
			{
				if ((*it).Quantity == -1)
				{
					break;
				}
				else
				{
					(*it).Quantity -= 1;
					if ((*it).Quantity == 0)
					{
						itemVector.erase(it);
						erased = true;
					}
					auto status = Gear::EntitySystem::GetStatus(entityID);
					auto itemlist = std::any_cast<std::unordered_map<std::string, std::vector<ItemInfo::ItemDescprition>>>(status->GetStat(ItemSelectorInfo::Stat::ItemList));
					itemlist[currentTeam] = itemVector;
					status->SetStat(ItemSelectorInfo::ItemList, itemlist);
					break;
				}
			}
		}
		if (erased)
		{
			for (int i = 0; i < WorldWormData::s_ActiveWorms.size(); ++i)
			{
				auto status = Gear::EntitySystem::GetStatus(WorldWormData::s_ActiveWorms[i]);
				auto team = std::any_cast<std::string>(status->GetStat(WormInfo::TeamName));

				if (currentTeam == team)
				{
					auto selectedItem = std::any_cast<ItemInfo::Number>(status->GetStat(WormInfo::SelectedItem));
					if (selectedItem == itemNumber)
					{
						status->SetStat(WormInfo::SelectedItem, itemVector[0].ItemNumber);
					}
				}
			}
		}

		handled = true;
	}

}