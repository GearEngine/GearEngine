#pragma once

#include "WorldEventHandler.h"

namespace InGame {

	class InGemeStartHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			static bool inFirst = true;

			auto timer = Gear::EntitySystem::GetTimer(entityID);
			auto LateDrawer = Gear::EntitySystem::GetLateDrawer(entityID);
			if (inFirst)
			{
				inFirst = false;
				timer->SetTimer(2.0f);
				timer->Start();
			}
			if (timer->isExpired())
			{
				inFirst = true;
				LateDrawer->InActivateStuff("Fog", Gear::Stuff::Quard);
				return WorldState::OnStart;
			}
			LateDrawer->GetQuardStuff("Fog").Color.a -= 0.007f;
			return WorldState::InGameStart;
		}
	};

	class WorldOnStartHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart, entityID)));
			return WorldState::OnPrepareRun;
		}
	};

	class WorldOnPrepareRunHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WorldState::OnPrepareRun;
		}
	};

	class WorldOnRunningHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WorldState::OnRunning;
		}
	};

	class WorldOnItemWindowHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WorldState::OnItemWindow;
		}
	};

	class WorldOnWindowSelectHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WorldState::OnWindowSelect;
		}
	};

	class WorldOnQuitWindowHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WorldState::OnQuitWindow;
		}
	};

}