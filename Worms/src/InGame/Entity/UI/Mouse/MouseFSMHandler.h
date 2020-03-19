#pragma once

#include "Gear.h"
#include "InGame/GameState.h"

namespace InGame {

	class MouseOnGameRunningHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{

			return GameState::OnRunning;
		}
	};

	class MouseOnItemWindowHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
		
			return GameState::OnItemWindow;
		}
	};

	class MouseOnWindowSelectHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			
			return GameState::OnWindowSelect;
		}
	};

	class MouseOnQuitWindowHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			
			return GameState::OnQuitWindow;
		}
	};


}