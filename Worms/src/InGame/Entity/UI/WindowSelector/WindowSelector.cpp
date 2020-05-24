#include "wmpch.h"
#include "WindowSelector.h"

namespace InGame {

	WindowSelector::WindowSelector(const InitiateData& initData)
	{
		m_ID = Gear::EntitySystem::CreateEntity(true, "WindowSelector");

		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::FSM, Gear::ComponentID::Status,
			Gear::ComponentID::Controller
			});

		Gear::EntitySystem::SetFSM(m_ID, {
			{ WindowSelectorState::OnNothing, new OnNotingSelectHandler  },
			{ WindowSelectorState::OnDropSelect, new OnNotingSelectHandler  },
			{ WindowSelectorState::OnUtilSelect, new OnUtilSelectHandler  }
			});

		Gear::EntitySystem::SetStatus(m_ID, {
			{ WindowSelectorStat::FromID , -1 },
			{ WindowSelectorStat::ItemNum, ItemInfo::Number::ItemEnd }
		});

		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::Worm);
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::MouseClick);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::Worm, Gear::CreateRef<WindowSelectorWormEventHandler>());
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::MouseClick, Gear::CreateRef<WindowSelectorMouseEventHandler>());
	}

	WindowSelector::~WindowSelector()
	{
	}

}

