#include "wmpch.h"
#include "ItemSelector.h"

#include "ItemSelectorPixelCollisionHandler.h"

namespace InGame {

	ItemSelector::ItemSelector(const InitiateData & initData)
	{
		m_ID = Gear::EntitySystem::CreateEntity(true, "ItemSelector");

		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::FSM, Gear::ComponentID::Status
		});

		Gear::EntitySystem::SetFSM(m_ID, {
			{ ItemSelectorInfo::State::OnNotActivate, new ItemSelectorOnNotActivate },
			{ ItemSelectorInfo::State::OnEmergy, new ItemSelectorOnEmergy },
			{ ItemSelectorInfo::State::OnSelect, new ItemSelectorOnSelect },
			{ ItemSelectorInfo::State::OnSink, new ItemSelectorOnSink },
			{ ItemSelectorInfo::State::OnUpdate, new ItemSelectorOnUpdate },
		});


		std::unordered_map<std::string, std::vector<ItemInfo::ItemDescprition>> TeamItems;
		for (int i = 0; i < initData.Teams.size(); ++i)
		{
			TeamItems.insert({ initData.Teams[i].TeamName, initData.Teams[i].TeamItem });
		}
		Gear::EntitySystem::SetStatus(m_ID, {
			{ ItemSelectorInfo::Stat::ItemList, TeamItems }, { ItemSelectorInfo::Stat::ItemSelectorBox, Gear::Util::FRect() }
		});


		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::MouseClick);
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::Worm);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::MouseClick, Gear::CreateRef<ItemSelectorMouseClickEventHandler>());
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::Worm, Gear::CreateRef<ItemSelectorWormEventHandler>());
	}

}