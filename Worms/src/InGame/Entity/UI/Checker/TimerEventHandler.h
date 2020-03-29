#pragma once

#include "InGame/Entity/World/World.h"

namespace InGame {

	class TimerEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID) override
		{
			auto worldData = std::any_cast<WorldData>(data);
			auto timerFSM = Gear::EntitySystem::GetFSM(entityID);
			auto timersTimer = Gear::EntitySystem::GetTimer(entityID);

			if (worldData.DataType == WorldDataType::NewStart)
			{
				timerFSM->SetCurrentState(WorldState::OnStart);
				timersTimer->SetTimer(0.7f);
				timersTimer->Start();
			}			
			if (worldData.DataType == WorldDataType::RunningStart)
			{
				auto timer = Gear::EntitySystem::GetTimer(entityID);
				Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(WorldState::OnRunning);
				timer->SetTimer(World::s_LimitTurnTime);
				timer->Start();
			}
		}
	};

}