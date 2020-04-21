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
		bool OnAwake = true;

		Gear::Ref<Gear::FSM> timerFSM;
		Gear::Ref<Gear::Timer> timersTimer;
		Gear::Ref<Gear::FSM> worldFSM;


		void Awake(int entityID)
		{
			timerFSM = Gear::EntitySystem::GetFSM(entityID);
			timersTimer = Gear::EntitySystem::GetTimer(entityID);
			int worldID = Gear::EntitySystem::GetEntityIDFromName("World");
			worldFSM = Gear::EntitySystem::GetFSM(worldID);
			OnAwake = false;
		}


		virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			auto worldData = std::any_cast<WorldData>(data);
			if (worldData.DataType == WorldDataType::NewStart)
			{
				auto worldState = worldFSM->GetCurrentState();
				if (worldState != WorldState::OnGameDraw && worldState != WorldState::OnGameVictory)
				{
					timerFSM->SetCurrentState(WorldState::OnStart);
					Gear::EntitySystem::GetStatus(entityID)->PushNeedHandleData(TimerStatusHandleType::MoveUp, Gear::Status::StatHandleData(0));
					timersTimer->SetTimer(0.7f);
					timersTimer->Start();
				}
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