#pragma once

#include "ItemSelectorEnum.h"

namespace InGame {

	class ItemSelectorMouseClickEventHandler : public Gear::EventHandler
	{
		Gear::Ref<Gear::FSM> FSM;

		bool inFirst = true;

		void init(int entityID)
		{
			FSM = Gear::EntitySystem::GetFSM(entityID);
		}

		inline void Handle(std::any data, int entityID, bool& handled) override
		{
			if (inFirst)
			{
				init(entityID);
			}

			auto keycode = std::any_cast<int>(data);
			auto curState = FSM->GetCurrentState();

			if (keycode == GR_MOUSE_BUTTON_RIGHT)
			{
				if (curState == ItemSelectorInfo::State::OnNotActivate)
				{
					GR_TRACE("On Item Selector Get Right Button Click Event");
					FSM->SetCurrentState(ItemSelectorInfo::State::OnUpdate);
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
			}

			handled = true;
		}

	};


}