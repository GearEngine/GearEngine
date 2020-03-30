#pragma once

#include "InGame/Entity/World/World.h"

namespace InGame {

	namespace TimerStatusHandleType
	{
		enum : unsigned int
		{
			MoveUp,
			MoveDown
		};
	}

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
				Gear::EntitySystem::GetStatus(entityID)->PushNeedHandleData(TimerStatusHandleType::MoveUp, Gear::Status::StatHandleData(0));
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