#pragma once

#include "InGame/Entity/World/World.h"

namespace InGame {

	namespace TimerStatusHandleType
	{
		enum : unsigned int
		{
			MoveUp,
			MoveDown,
			Pause,
			Resume
		};
	}

	class TimerEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
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
				handled = true;
			}			
			if (worldData.DataType == WorldDataType::RunningStart)
			{
				Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(WorldState::OnRunning);
				timersTimer->SetTimer(World::s_LimitTurnTime);
				timersTimer->Start();
				handled = true;
			}
			if (worldData.DataType == WorldDataType::TurnOver)
			{
				timerFSM->SetCurrentState(WorldState::OnPrepareNextPhase);
				Gear::EntitySystem::GetStatus(entityID)->PushNeedHandleData(TimerStatusHandleType::MoveDown, Gear::Status::StatHandleData(0));
				handled = true;
			}
		}
	};

}