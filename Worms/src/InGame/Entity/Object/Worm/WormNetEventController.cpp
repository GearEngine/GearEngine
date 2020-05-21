#include "wmpch.h"
#include "WormNetEventController.h"

#include "WormFSMHandler.h"
#include "InGame/Entity/UI/ItemSelector/ItemSelectorEnum.h"

namespace InGame {

	void WormNetEventController::Handle(int entityID, InputMemoryStream & stream)
	{
		WormsPacket::Event e;
		e.Read(stream);

		switch (e.eventType)
		{
		case WormsPacket::Event::EventType::ItemSelect:
			ItemSelectHandle(e);
			break;
		case WormsPacket::Event::EventType::TurnChange:
			TurnChange(e);
			break;
		}
	}
	void WormNetEventController::ItemSelectHandle(const WormsPacket::Event & e)
	{
		auto currentWormID = e.wormID;

		auto wormFSM = Gear::EntitySystem::GetFSM(currentWormID);
		auto wormStatus = Gear::EntitySystem::GetStatus(currentWormID);

		wormStatus->SetStat(WormInfo::Stat::SelectedItem, (ItemInfo::Number)e.customIData1);
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

		int itemSelectorID = Gear::EntitySystem::GetEntityIDFromName("ItemSelector");
		auto itemSelectorFSM = Gear::EntitySystem::GetFSM(itemSelectorID);
		auto itemSelectorState = itemSelectorFSM->GetCurrentState();
		if (itemSelectorState == ItemSelectorInfo::OnSelect)
		{
			itemSelectorFSM->SetCurrentState(ItemSelectorInfo::State::OnSink);
		}
	}
	void WormNetEventController::TurnChange(const WormsPacket::Event & e)
	{
		Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
		Gear::NetController::SendOnlyNull = false;
	}
}
