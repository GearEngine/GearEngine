#pragma once

#include "WindowSelectorEventHandler.h"

namespace InGame {

	class OnDropSelectHandler : public Gear::FSM::InputHandler
	{
		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WindowSelectorState::OnDropSelect;
		}
	};

	class OnUtilSelectHandler : public Gear::FSM::InputHandler
	{
		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WindowSelectorState::OnUtilSelect;
		}

	};

	class OnNotingSelectHandler : public Gear::FSM::InputHandler
	{
		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WindowSelectorState::OnNothing;

		}

	};

}